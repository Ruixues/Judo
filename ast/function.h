#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../type.h"
#include "AST.h"

namespace AST {
    // 函数参数
    class FunctionArg {
    public:
        JudoType type;
        std::string name;

        FunctionArg(JudoType type, std::string name) : type(type), name(name) {}
    };

    class FunctionProto : public ExprAST {
    public:
        std::string name;
        std::vector<std::unique_ptr<FunctionArg>> args;
        std::string returnName; //返回变量的名字
        JudoType returnType;

        FunctionProto(std::string name, std::vector<std::unique_ptr<FunctionArg>> args, std::string returnName,
                      JudoType returnType) : name(name), args(std::move(args)), returnName(returnName),
                                             returnType(returnType) {
        }

        llvm::Value *genCode() {
            return nullptr;
        }

        llvm::Function *genFunction() {
            std::vector<llvm::Type *> argTypes;
            for (auto &arg:args) {

            }
        }
    };

    class FunctionAST : public ExprAST {
    private:
        std::unique_ptr<FunctionProto> proto;
        std::unique_ptr<ExprAST> code; //函数内容
    public:
        FunctionAST(std::unique_ptr<FunctionProto> proto, std::unique_ptr<ExprAST> code) : proto(std::move(proto)),
                                                                                           code(std::move(code)) {

        }
        llvm::Value *genCode();
    };
}; // namespace AST