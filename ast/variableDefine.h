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
    };

    class VariableDefine : public ExprAST {
    private:
        std::string name;
        std::unique_ptr<ExprAST> value;
        JudoType type;
    public:
        VariableDefine(const std::string &name, std::unique_ptr<ExprAST> value, JudoType type) : name(name),
                                                                                                 value(std::move(
                                                                                                         value)),
                                                                                                 type(type) {}
    };
}