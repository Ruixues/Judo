#include "FunctionCall.h"
#include "../core.h"

namespace AST {

    llvm::Value *FunctionCall::genCode() {
        auto f = module->getFunction(functionName);
        if (!f) {
            return module->loger->GenCodeError("undefined function:" + functionName);
        }
        if (args.size() != f->arg_size()) {
            return module->loger->GenCodeError("the number of arguments doesn't match function:" + functionName);
        }
        //开始创建调用
        std::vector<llvm::Value *> argVs;
        for (auto &v:args) {
            auto tmp = v->genCode();
            if (!tmp) {
                return nullptr;
            }
            argVs.push_back(tmp);
        }
        return module->Builder.CreateCall(f, argVs, "calltmp");
    }
}