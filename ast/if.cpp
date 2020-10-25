#include "if.h"
#include "../core.h"

namespace AST {
    llvm::Value *If::genCode() {
        auto cond = condition->genCode();
        if (!cond) return nullptr;
        auto CondV = module->Builder.CreateICmpEQ(
                module->Builder.CreateIntCast(cond, llvm::Type::getInt1Ty(module->core->context), false),
                llvm::ConstantInt::getTrue(module->core->context), "ifcond");
        if (!CondV) return nullptr;
        llvm::Function *TheFunction = module->Builder.GetInsertBlock()->getParent();
        llvm::BasicBlock *ThenBB =
                llvm::BasicBlock::Create(module->core->context, "then", TheFunction);
        llvm::BasicBlock *Else = llvm::BasicBlock::Create(module->core->context, "else", TheFunction);;
        llvm::BasicBlock *Next =
                llvm::BasicBlock::Create(module->core->context, "next", TheFunction);
        auto ret = module->Builder.CreateCondBr(CondV, ThenBB, Else);
        module->Builder.SetInsertPoint(ThenBB);
        if (!then->genCode()) {
            return nullptr;
        }
        if (module->Builder.GetInsertBlock() == ThenBB) {
            module->Builder.CreateBr(Next);
        }
        module->Builder.SetInsertPoint(Else);
        if (relse) {
            if (!relse->genCode()) {
                return nullptr;
            }
            if (module->Builder.GetInsertBlock() == Else) {
                module->Builder.CreateBr(Next);
            }
        } else {
            module->Builder.CreateBr(Next);
        }
        module->Builder.SetInsertPoint(Next);
        return ret;
    }
}