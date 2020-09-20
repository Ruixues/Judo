//二元运算符
#pragma once

#include "AST.h"
#include <memory>
#include <map>

namespace AST {
    std::map<std::string, int> BinopPrecedence = {{"<", 10},
                                                        {"+", 20},
                                                        {"-", 30},
                                                        {"*", 40}};
    class BinaryExprAST : public ExprAST {
        std::string opt;
        std::unique_ptr<ExprAST> l, r;
    public:
        BinaryExprAST(std::string opt, std::unique_ptr<ExprAST> l, std::unique_ptr<ExprAST> r) : opt(opt),
                                                                                                 l(std::move(l)),
                                                                                                 r(std::move(r)) {

        }
    };
}