#pragma once
#include "AST.h"
#include <vector>
namespace AST
{
    class CodeBlockAST : public ExprAST
    {
    private:
        std::vector<ExprAST> codes;

    public:
        CodeBlockAST(std::vector<ExprAST> codes) : codes(codes) {}
    };
}; // namespace AST