#include "function.h"
#include "../core.h"
namespace AST {
    llvm::Value *FunctionAST::genCode() {
        auto f = module->module->getFunction(proto->name);
        if (!f) {
            f = proto->genFunction();
        }
        if (!f) {
            return nullptr;
        }
        if (!f->empty()) {
            return (llvm::Function *) module->loger->GenCodeError("Function cannot be redefined.");
        }
        auto BB = llvm::BasicBlock::Create(module->context, "entry", f);
        module->Builder.SetInsertPoint(BB);
        //开始进入新的作用域，应当改变部分变量的引用
        std::map<std::string, llvm::Value *> buf;
        //开始根据参数设置

    }
}