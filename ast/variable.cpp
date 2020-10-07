#include "variable.h"
#include "../core.h"

namespace AST {
    llvm::Value *VariableExpr::genCode() {
        auto value = module->GetNamedValue(name);
        if (!value) {
            return module->loger->GenCodeError("Undefined variable:" + name);
        }
        return module->Builder.CreateLoad(value, name.c_str());
    }

    llvm::Value *VariableDefine::genCode() {
        //生成一个变量
        auto v = module->CreateAlloca(module->Builder.GetInsertBlock()->getParent(), name,
                                      type.getType(module->core->context));
        if (value) {
            auto initV = value->genCode();
            if (!initV) return nullptr;
            module->Builder.CreateStore(initV,v);
        }
        module->SetNamedValue(name, v);
        return module->Builder.CreateLoad(v, name);
    }
}