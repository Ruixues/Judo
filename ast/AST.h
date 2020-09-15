#pragma once
#include "llvmInclude.h"
#include <memory>
#include <vector>
namespace AST
{
    class ExprAST
    {
    public:
        virtual ~ExprAST();
        virtual llvm::Value *genCode(); //生成LLVM IR
    };
} // namespace AST