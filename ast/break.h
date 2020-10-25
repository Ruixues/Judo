#pragma once

#include "AST.h"

namespace AST {
    class BreakAST : public ExprAST {
    public:
        BreakAST() {}

        llvm::Value *genCode();
    };

    class ContinueAST : public ExprAST {
    public:
        ContinueAST() {}

        llvm::Value *genCode();
    };
}