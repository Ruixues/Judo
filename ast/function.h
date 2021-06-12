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
        llvm::Type *type;
        std::string name;

        FunctionArg(llvm::Type *type, std::string name) : type(type), name(name) {}
    };

    class FunctionProto : public ExprAST {
    public:
        std::string name;
        std::vector<std::unique_ptr<FunctionArg>> args;
        std::unique_ptr<JudoTypeRef> returnType;
        bool isClass = false;
        FunctionProto(std::string name, std::vector<std::unique_ptr<FunctionArg>> args,
                      std::unique_ptr<JudoTypeRef> returnType) : name(name), args(std::move(args)),
                                                                 returnType(std::move(returnType)) {
        }
        llvm::Value *genCode() {
            return nullptr;
        }

        llvm::Function *genFunction();
    };

    class FunctionAST : public ExprAST {
    private:
        std::unique_ptr<FunctionProto> proto;
        std::unique_ptr<ExprAST> code; //函数内容
        std::string belongClass;
        std::string variableName;
        bool isClass;
    public:
        FunctionAST(std::unique_ptr<FunctionProto> proto, std::unique_ptr<ExprAST> code) : proto(std::move(proto)),
                                                                                           code(std::move(code)),isClass(false) {

        }
        FunctionAST(std::string variableName,std::string belongClass,std::unique_ptr<FunctionProto> proto, std::unique_ptr<ExprAST> code) : proto(std::move(proto)),
                                                                                           code(std::move(code)),belongClass(belongClass),variableName(variableName),isClass(true) {
                                                                                           }

        llvm::Value *genCode();
    };
}; // namespace AST