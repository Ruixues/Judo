#include "for.h"
#include "../core.h"
#include "../other/defer.h"
namespace AST {
    llvm::Value* For::genCode() {
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
        module->Builder.SetInsertPoint(LoopBodyBB);
        module->NowForBlock.push(LoopBB);
        module->ForThenBlock.push(AfterLoop);
        defer(
                module->NowForBlock.pop();
                module->ForThenBlock.pop();
        );
        auto nowF = module->Builder.GetInsertBlock()->getParent();
        if (!body->genCode()) {
            return nullptr;
        }
        //开始添加重新回到循环头的指令
        if (module->Builder.GetInsertBlock()->getParent() != nowF) {    //已经return了
            return llvm::Constant::getNullValue(llvm::Type::getDoubleTy(module->core->context));
        }
        //否则那就回到头部
        module->Builder.CreateBr(LoopBB);
        module->Builder.SetInsertPoint(AfterLoop);
        return (llvm::Value*)1;
    }
}