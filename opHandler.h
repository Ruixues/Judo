#pragma once

#include "llvmInclude.h"
#include <map>

//用来获取运算符处理函数的
class Module;

class OpHandler {
private:
public:
    Module *module;

    OpHandler(Module *module) : module(module) {}

    std::map<llvm::Type::TypeID, std::map<llvm::Type::TypeID, std::map<std::string, llvm::Function *>>> opLink;

    // 获取二元操作符的对应函数
    llvm::Function *getBinaryOp(std::string opt, llvm::Value *a, llvm::Value *b);

    bool linkOp(llvm::Type::TypeID a, llvm::Type::TypeID b, std::string opt, llvm::Function *function);
};