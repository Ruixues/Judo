#include "add.h"
#include "../core.h"
#include <iostream>
#include "../type.h"
llvm::Value* int32Int32Add (Module* module,llvm::Value* a,llvm::Value* b);
void bindAdd(Module *module) {
    module->opHandler->linkOp("Int32","Int32","+",int32Int32Add);
    return;
}

llvm::Value* int32Int32Add (Module* module,llvm::Value* a,llvm::Value* b) {
    return module->Builder.CreateAdd(a,b);
}

#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

extern "C" DLLEXPORT int test(int32 v) {
    std::cout << "v:" << v << std::endl;
    return 0;
}