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
                relse(std::move(relse)) {
        }

        llvm::Value *genCode() {
            // auto cond = condition->genCode();
            // if (!cond) return nullptr;
            // auto t = then->genCode();
            // if (!t) return nullptr;
            // auto CondV = module->Builder.CreateFCmpONE(cond, llvm::ConstantInt::getTrue(module->context), "ifcond");
            // if (!CondV) return nullptr;
            // llvm::Value* el;
            // if (relse) {
            //     el = relse->genCode();
            //     if (!el) {
            //         return nullptr;
            //     }
            // }
            // llvm::Function *TheFunction = module->Builder.GetInsertBlock()->getParent();
            // llvm::BasicBlock *ThenBB =
            //     llvm::BasicBlock::Create(module->context, "then", TheFunction);
            // return module->Builder.CreateCondBr(CondV,t,el);

        }
    };
}