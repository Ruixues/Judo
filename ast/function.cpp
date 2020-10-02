#include "function.h"
#include "../core.h"
#include "../other/defer.h"

namespace AST {
    llvm::Function *FunctionProto::genFunction() {
        std::vector<llvm::Type *> argTypes;
        for (auto &arg:args) {
            argTypes.push_back(arg->type.getType(module->core->context));
        }
        llvm::FunctionType *FT =
                llvm::FunctionType::get(returnType.getType(module->core->context), argTypes, false);
        llvm::Function *F =
                llvm::Function::Create(FT, llvm::Function::ExternalLinkage, name, module->module.get());
        size_t index = 0;
        for (auto &arg:F->args()) {
            arg.setName(args[index++]->name);
        }
        return F;
    }

    llvm::Value *FunctionAST::genCode() {
        auto &P = *proto;
        module->FunctionProto [proto->name] = std::move (proto);
        auto f = module->module->getFunction(P.name);
        if (!f) {
            f = P.genFunction();
        }
        if (!f) {
            return nullptr;
        }
        if (!f->empty()) {
            return (llvm::Function *) module->loger->GenCodeError("Function cannot be redefined.");
        }
        auto BB = llvm::BasicBlock::Create(module->core->context, "entry", f);
        module->Builder.SetInsertPoint(BB);
        //开始进入新的作用域，应当改变部分变量的引用
        for (auto &arg:f->args()) {
            module->namedValues[arg.getName()] = &arg;
        }
        defer (for (auto &arg:f->args()) {
            module->namedValues.erase(arg.getName());
        });
        if (code->genCode()) {
            //开始判断当前位置，是否已经创建了return语句
            if (module->Builder.GetInsertBlock()->getParent()->getName() == proto->name) {  //函数没有返回
                module->Builder.CreateRetVoid();
            }
            llvm::verifyFunction(*f);
            module->core->FPM->run(*f);
            return f;
        }
        //开始根据参数设置
        f->removeFromParent();
        return nullptr;
    }
}