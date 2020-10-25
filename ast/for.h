#pragma once

#include "AST.h"

namespace AST {
    class For : public ExprAST {
    private:
        std::unique_ptr<AST::ExprAST> init, cond, update, body;
    public:
        For(std::unique_ptr<AST::ExprAST> init, std::unique_ptr<AST::ExprAST> cond,
            std::unique_ptr<AST::ExprAST> update, std::unique_ptr<AST::ExprAST> body) :
                init(std::move(init)), cond(std::move(cond)), update(std::move(update)), body(std::move(body)) {}

        llvm::Value *genCode();
    };
}