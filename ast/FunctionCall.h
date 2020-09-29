#pragma once

#include "AST.h"
#include <string>
#include <memory>
#include <vector>

namespace AST {
    class FunctionCall : public ExprAST {
    private:
        std::string functionName;
        std::vector<std::unique_ptr<ExprAST>> args;
    public:
        FunctionCall(std::string functionName, std::vector<std::unique_ptr<ExprAST>> args) : functionName(functionName),
                                                                                             args(std::move(args)) {

        }
        llvm::Value * genCode() {
            auto f = module->module->getFunction(functionName);
            if (!f) {
                return module->loger->GenCodeError("undefined function:" + functionName);
            }
            if (args.size() != f->arg_size()) {
                return module->loger->GenCodeError("the number of arguments doesn't match function:" + functionName);
            }
            //开始创建调用
            std::vector<llvm::Value*> argVs;
            for (auto &v:args) {
                auto tmp = v->genCode();
                if (!tmp) {
                    return nullptr;
                }
                argVs.push_back(tmp);
            }
            return module->Builder.CreateCall(f,argVs,"calltmp");
        }
    };
}