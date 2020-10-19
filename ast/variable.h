#pragma once

#include "AST.h"
#include <string>
#include "../type.h"

namespace AST {
    class VariableExpr : public ExprAST {
    private:
        std::string name;
    public:
        VariableExpr(const std::string &name) : name(name) {}

        std::string GetName() { return name; }

        llvm::Value *genCode();
    };

    class VariableDefine : public ExprAST {
    private:
        std::string name;
        std::unique_ptr<ExprAST> value;
        JudoType type;
        llvm::Type* realType;
        std::vector<size_t> levelSize;  //每一维的大小，数组才需要提供
    public:
        VariableDefine(const std::string &name, std::unique_ptr<ExprAST> value, JudoType type, std::vector<size_t> level) : name(name),
                                                                                                 value(std::move(
                                                                                                         value)),levelSize(std::move(level)),
                                                                                                 type(type) {}

        llvm::Value *genCode();
    };
}