#include "for.h"
#include "../core.h"
#include "../other/defer.h"

namespace AST {
    llvm::Value *For::genCode() {
        llvm::Function *f = module->Builder.GetInsertBlock()->getParent();
        llvm::BasicBlock *LoopBB =
                llvm::BasicBlock::Create(module->core->context, "loop", f);
        llvm::BasicBlock *LoopBodyBB =
                llvm::BasicBlock::Create(module->core->context, "loopBody", f);
        llvm::BasicBlock *AfterLoop =
                llvm::BasicBlock::Create(module->core->context, "afterLoop", f);
        //创建进入
        module->Builder.CreateBr(LoopBB);
        module->Builder.SetInsertPoint(LoopBB);
        auto condV = cond->genCode();
        if (!condV) {
            return nullptr;
        }
        //开始添加执行判断
        auto CondV = module->Builder.CreateICmpEQ(
                module->Builder.CreateIntCast(condV, llvm::Type::getInt1Ty(module->core->context), false),
                llvm::ConstantInt::getTrue(module->core->context), "ifLoop");
        module->Builder.CreateCondBr(CondV, LoopBodyBB, AfterLoop);
        module->NowForBlock.push(LoopBB);
        module->ForThenBlock.push(AfterLoop);
        module->Builder.SetInsertPoint(LoopBodyBB); //开始填写body
        if (!body->genCode()) {
            module->NowForBlock.pop();
            module->ForThenBlock.pop();
            return nullptr;
        }
        module->NowForBlock.pop();
        module->ForThenBlock.pop();
        module->Builder.CreateBr(LoopBB);   // 添加回到头部的跳转
        module->Builder.SetInsertPoint(AfterLoop);
        return (llvm::Value *) 1;
    }
}