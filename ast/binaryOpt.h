//二元运算符
#pragma once

#include "AST.h"
#include <memory>
#include <map>
#include <iostream>
namespace AST {
    extern std::map<std::string, int> BinopPrecedence;

    class BinaryExprAST : public ExprAST {
        std::string opt;
        std::unique_ptr<ExprAST> l, r;
    public:
        BinaryExprAST(std::string opt, std::unique_ptr<ExprAST> l, std::unique_ptr<ExprAST> r) : opt(opt),
                                                                                                 l(std::move(l)),
                                                                                                 r(std::move(r)) {
        }

        llvm::Value *genCode();
    };
}