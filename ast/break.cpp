#pragma once
#include "break.h"
#include "../core.h"

namespace AST {
    llvm::Value * BreakAST::genCode() {
        //创建跳转
        if (module->ForThenBlock.empty()) {
            return module->loger->GenCodeError("calling break outside for is not allowed");
        }
        return module->Builder.CreateBr(module->ForThenBlock.top());
    }
    llvm::Value * ContinueAST::genCode() {
        //创建跳转
        if (module->NowForBlock.empty()) {
            return module->loger->GenCodeError("calling break outside for is not allowed");
        }
        return module->Builder.CreateBr(module->NowForBlock.top());
    }
}