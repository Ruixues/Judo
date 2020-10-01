#include "codeBlock.h"
#include "../core.h"
namespace AST {
    llvm::Value *CodeBlockAST::genCode() {
        auto block = llvm::BasicBlock::Create(module->context, "", module->Builder.GetInsertBlock()->getParent());
        module->Builder.SetInsertPoint(block);
        for (auto &v: codes) {
            v->genCode();
        }
        if (module->Builder.GetInsertBlock() == block) { //那就帮助退出
            module->Builder.CreateRetVoid();
        }
        return block;
    }
}