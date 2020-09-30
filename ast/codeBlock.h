#pragma once

#include "AST.h"
#include <vector>

namespace AST {
    class CodeBlockAST : public ExprAST {
    private:
        std::vector<std::unique_ptr<ExprAST>> codes;

    public:
        CodeBlockAST(std::vector<std::unique_ptr<ExprAST>> codes) : codes(std::move(codes)) {}

        llvm::Value *genCode() {
            for (auto &v:codes) {
                v->genCode();
            }
            return nullptr;
            // auto TheFunction = module->Builder.GetInsertBlock()->getParent(); //获取插入点
            // auto BB = llvm::BasicBlock::Create(module->context, "block", TheFunction);
            // auto Then = llvm::BasicBlock::Create(module->context, "then", TheFunction);
            // module->Builder.CreateCondBr(llvm::ConstantInt::getTrue(module->context),BB,BB);
            // module->Builder.SetInsertPoint(BB);
            // for (auto &v : codes) {
            //     v->genCode();
            // }
            // // 在完成了构建后，开始添加跳转指令
            // module->Builder.CreateBr(Then);
        }
    };
}; // namespace AST