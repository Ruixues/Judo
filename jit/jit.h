// Source : LLVM Official Code
#pragma once
#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/iterator_range.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JITSymbol.h"
#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include "llvm/ExecutionEngine/Orc/IRCompileLayer.h"
#include "llvm/ExecutionEngine/Orc/LambdaResolver.h"
#include "llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h"
#include "llvm/ExecutionEngine/RTDyldMemoryManager.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Mangler.h"
#include "llvm/Support/DynamicLibrary.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
    class RJIT {
    public:

        using ObjLayerT = llvm::orc::LegacyRTDyldObjectLinkingLayer;
        using CompileLayerT = llvm::orc::LegacyIRCompileLayer<ObjLayerT, llvm::orc::SimpleCompiler>;

        RJIT()
                : Resolver(createLegacyLookupResolver(
                ES,
                [this](llvm::StringRef Name) {
                    return findMangledSymbol(std::string(Name));
                },
                [](llvm::Error Err) { cantFail(std::move(Err), "lookupFlags failed"); })),
                  TM(llvm::EngineBuilder().selectTarget()), DL(TM->createDataLayout()),
                  ObjectLayer(llvm::AcknowledgeORCv1Deprecation, ES,
                              [this](llvm::orc::VModuleKey) {
                                  return ObjLayerT::Resources{
                                          std::make_shared<llvm::SectionMemoryManager>(), Resolver};
                              }),
                  CompileLayer(llvm::AcknowledgeORCv1Deprecation, ObjectLayer,
                               llvm::orc::SimpleCompiler(*TM)) {
            llvm::sys::DynamicLibrary::LoadLibraryPermanently(nullptr);
        }

        llvm::TargetMachine &getTargetMachine() { return *TM; }

        llvm::orc::VModuleKey addModule(std::unique_ptr<llvm::Module> M) {
            auto K = ES.allocateVModule();
            cantFail(CompileLayer.addModule(K, std::move(M)));
            ModuleKeys.push_back(K);
            return K;
        }

        void removeModule(llvm::orc::VModuleKey K) {
            ModuleKeys.erase(llvm::find(ModuleKeys, K));
            cantFail(CompileLayer.removeModule(K));
        }

        llvm::JITSymbol findSymbol(const std::string Name) {
            return findMangledSymbol(mangle(Name));
        }

    private:
        std::string mangle(const std::string &Name) {
            std::string MangledName;
            {
                llvm::raw_string_ostream MangledNameStream(MangledName);
                llvm::Mangler::getNameWithPrefix(MangledNameStream, Name, DL);
            }
            return MangledName;
        }

        llvm::JITSymbol findMangledSymbol(const std::string &Name) {
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
            if (auto SymAddr = llvm::RTDyldMemoryManager::getSymbolAddressInProcess(Name))
                return llvm::JITSymbol(SymAddr, llvm::JITSymbolFlags::Exported);

#ifdef _WIN32
            // For Windows retry without "_" at beginning, as RTDyldMemoryManager uses
            // GetProcAddress and standard libraries like msvcrt.dll use names
            // with and without "_" (for example "_itoa" but "sin").
            if (Name.length() > 2 && Name[0] == '_')
                if (auto SymAddr =
                        llvm::RTDyldMemoryManager::getSymbolAddressInProcess(Name.substr(1)))
                    return llvm::JITSymbol(SymAddr, llvm::JITSymbolFlags::Exported);
#endif

            return nullptr;
        }

        llvm::orc::ExecutionSession ES;
        std::shared_ptr<llvm::orc::SymbolResolver> Resolver;
        std::unique_ptr<llvm::TargetMachine> TM;
        const llvm::DataLayout DL;
        ObjLayerT ObjectLayer;
        CompileLayerT CompileLayer;
        std::vector<llvm::orc::VModuleKey> ModuleKeys;
    };