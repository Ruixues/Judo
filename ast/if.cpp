#include "if.h"
#include "../core.h"

namespace AST {
    llvm::Value *If::genCode() {
        return nullptr;
//        auto cond = condition->genCode();
//        if (!cond) return nullptr;
//        auto t = then->genCode();
//        if (!t) return nullptr;
//        auto CondV = module->Builder.CreateFCmpONE(cond, llvm::ConstantInt::getTrue(module->context), "ifcond");
//        if (!CondV) return nullptr;
//        llvm::Value* el;
//        if (relse) {
//            el = relse->genCode();
//            if (!el) {
//                return nullptr;
//            }
//        }
//        llvm::Function *TheFunction = module->Builder.GetInsertBlock()->getParent();
//        llvm::BasicBlock *ThenBB =
//                llvm::BasicBlock::Create(module->context, "then", TheFunction);
//        return module->Builder.CreateCondBr(CondV,t,el);
    }
}