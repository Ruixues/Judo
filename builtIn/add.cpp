#include "add.h"
#include "../core.h"
#include <iostream>
#include "../type.h"

llvm::Value *int32Int32Add(Module *module, llvm::Value *a, llvm::Value *b);

llvm::Value *int32Int32Sub(Module *module, llvm::Value *a, llvm::Value *b);

llvm::Value *int32Int32Equal(Module *module, llvm::Value *a, llvm::Value *b);
void bindAdd(Module *module) {
    module->opHandler->linkOp("Int32", "Int32", "+", int32Int32Add);
    module->opHandler->linkOp("Int32", "Int32", "-", int32Int32Sub);
    module->opHandler->linkOp("Int32","Int32","==",int32Int32Equal);
    return;
}
llvm::Value *int32Int32Equal(Module *module, llvm::Value *a, llvm::Value *b) {
    return module->Builder.CreateICmpEQ(a,b);
}

llvm::Value *int32Int32Add(Module *module, llvm::Value *a, llvm::Value *b) {
    return module->Builder.CreateAdd(a, b);
}

llvm::Value *int32Int32Sub(Module *module, llvm::Value *a, llvm::Value *b) {
    return module->Builder.CreateSub(a, b);
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