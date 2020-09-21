#pragma once

#include "AST.h"
#include <vector>

namespace AST {
    class CodeBlockAST : public ExprAST {
    private:
        std::vector<std::unique_ptr<ExprAST>> codes;
    public:
        CodeBlockAST(std::vector<std::unique_ptr<ExprAST>> codes) : codes(std::move(codes)) {}
    };
}; // namespace AST