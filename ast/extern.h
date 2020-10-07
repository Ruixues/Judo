#pragma once

#include "AST.h"

namespace AST {
    class ExternAST : public ExprAST {
    public:
        llvm::Value *genCode() { return nullptr; }
    };
}
