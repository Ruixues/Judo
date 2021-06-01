#include "jit.h"
RJIT::RJIT()
    : Resolver(createLegacyLookupResolver(
          ES,
          [this](llvm::StringRef Name) {
            return findMangledSymbol(std::string(Name));
          },
          [](llvm::Error Err) {
            cantFail(std::move(Err), "lookupFlags failed");
          })),
      TM(llvm::EngineBuilder().selectTarget()), DL(TM->createDataLayout()),
      ObjectLayer(llvm::AcknowledgeORCv1Deprecation, ES,
                  [this](llvm::orc::VModuleKey) {
                    return ObjLayerT::Resources{
                        std::make_shared<llvm::SectionMemoryManager>(),
                        Resolver};
                  }),
      CompileLayer(llvm::AcknowledgeORCv1Deprecation, ObjectLayer,
                   llvm::orc::SimpleCompiler(*TM)) {
  llvm::sys::DynamicLibrary::LoadLibraryPermanently(nullptr);
}
llvm::JITSymbol RJIT::findMangledSymbol(const std::string &Name) {
#ifdef _WIN32
  // The symbol lookup of ObjectLinkingLayer uses the SymbolRef::SF_Exported
  // flag to decide whether a symbol will be visible or not, when we call
  // IRCompileLayer::findSymbolIn with ExportedSymbolsOnly set to true.
  //
  // But for Windows COFF objects, this flag is currently never set.
  // For a potential solution see: https://reviews.llvm.org/rL258665
  // For now, we allow non-exported symbols on Windows as a workaround.
  const bool ExportedSymbolsOnly = false;
#else
  const bool ExportedSymbolsOnly = true;
#endif

  // Search modules in reverse order: from last added to first added.
  // This is the opposite of the usual search order for dlsym, but makes more
  // sense in a REPL where we want to bind to the newest available definition.
  for (auto H : llvm::make_range(ModuleKeys.rbegin(), ModuleKeys.rend()))
    if (auto Sym = CompileLayer.findSymbolIn(H, Name, ExportedSymbolsOnly))
      return Sym;

  // If we can't find the symbol in the JIT, try looking in the host process.
  if (auto SymAddr =
          llvm::RTDyldMemoryManager::getSymbolAddressInProcess(Name)) {
    return llvm::JITSymbol(SymAddr, llvm::JITSymbolFlags::Exported);
  }

#ifdef _WIN32
  // For Windows retry without "_" at beginning, as RTDyldMemoryManager uses
  // GetProcAddress and standard libraries like msvcrt.dll use names
  // with and without "_" (for example "_itoa" but "sin").
  if (Name.length() > 2 && Name[0] == '_')
    if (auto SymAddr = llvm::RTDyldMemoryManager::getSymbolAddressInProcess(
            Name.substr(1)))
      return llvm::JITSymbol(SymAddr, llvm::JITSymbolFlags::Exported);
#endif

  return nullptr;
}
std::string RJIT::mangle(const std::string &Name) {
  std::string MangledName;
  {
    llvm::raw_string_ostream MangledNameStream(MangledName);
    llvm::Mangler::getNameWithPrefix(MangledNameStream, Name, DL);
  }
  return MangledName;
}