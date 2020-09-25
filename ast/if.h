#pragma once

#include "AST.h"

namespace AST {
    class If : public ExprAST {
        std::unique_ptr<ExprAST> condition;
        std::unique_ptr<ExprAST> then, relse, init;
    public:
        If(std::unique_ptr<ExprAST> init, std::unique_ptr<ExprAST> condition, std::unique_ptr<ExprAST> then,
           std::unique_ptr<ExprAST> relse) :
                init(std::move(init)), condition(std::move(condition)), then(std::move(then)),
                relse(std::move(relse)) {}
    };
}