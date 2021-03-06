#pragma once

#include "llvmInclude.h"
#include <map>
#include <iostream>

//用来获取运算符处理函数的
class Module;

typedef std::function<llvm::Value *(Module *, llvm::Value *, llvm::Value *)> GetOptFunc;

class OpHandler {
private:
public:
    Module *module;

    OpHandler(Module *module) : module(module) {}

    static std::string typeToStr(llvm::Type *type) {
        if (type->isStructTy()) return std::string(type->getStructName());
        switch (type->getTypeID()) {
            default:
                return "unknown";
            case llvm::Type::IntegerTyID: {
                auto bit = type->getIntegerBitWidth();
                if (bit == 32) {
                    return "Int32";
                }
                if (bit == 64) {
                    return "Int64";
                }
                return "unknown";
            }
            case llvm::Type::FloatTyID:
                return "Double";
        }
    }

    std::map<std::string, std::map<std::string, std::map<std::string, GetOptFunc>>> opLink;

    // 获取二元操作符的对应函数
    llvm::Value *getBinaryOp(std::string opt, llvm::Value *a, llvm::Value *b);

    bool linkOp(const std::string &a, const std::string &b, const std::string &opt, GetOptFunc function);
};