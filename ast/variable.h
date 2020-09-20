#pragma once

#include "AST.h"
#include <string>

namespace AST {
    class VariableExpr : public ExprAST {
    private:
        std::string name;
    public:
        VariableExpr(const std::string &name) : name(name) {}
    };
}