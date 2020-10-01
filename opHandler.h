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
    static std::string typeToStr (llvm::Type* type) {
        if (type->isStructTy()) return type->getStructName();
        switch (type->getTypeID ()) {
        default:
            return "unknown";
        case llvm::Type::IntegerTyID:
            return "int";
        case llvm::Type::FloatTyID:
            return "double";
        }
    }
    std::map<std::string, std::map<std::string, std::map<std::string, llvm::Function *>>> opLink;
    // 获取二元操作符的对应函数
    llvm::Function *getBinaryOp(std::string opt, llvm::Value *a, llvm::Value *b);

    bool linkOp(const std::string &a,const std::string &b,const std::string &opt, llvm::Function *function);
};