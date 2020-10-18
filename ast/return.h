#pragma once

#include "AST.h"

namespace AST {
    class ReturnAST : public ExprAST {
    private:
        std::unique_ptr<ExprAST> retValue;
    public:
        ReturnAST (std::unique_ptr<ExprAST> retValue) : retValue (std::move(retValue)) {}
        llvm::Value * genCode();
    };
}