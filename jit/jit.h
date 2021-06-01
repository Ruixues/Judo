#pragma once

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
#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

typedef llvm::Module llvmModule;
class RJIT {
public:
    using ObjLayerT = llvm::orc::LegacyRTDyldObjectLinkingLayer;
    using CompileLayerT = llvm::orc::LegacyIRCompileLayer<ObjLayerT, llvm::orc::SimpleCompiler>;

    RJIT();

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
    std::string mangle(const std::string &Name) ;

    llvm::JITSymbol findMangledSymbol(const std::string &Name);

    llvm::orc::ExecutionSession ES;
    std::shared_ptr<llvm::orc::SymbolResolver> Resolver;
    std::unique_ptr<llvm::TargetMachine> TM;
    const llvm::DataLayout DL;
    ObjLayerT ObjectLayer;
    CompileLayerT CompileLayer;
    std::vector<llvm::orc::VModuleKey> ModuleKeys;
};