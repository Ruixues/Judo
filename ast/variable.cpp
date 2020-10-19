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
        llvm::AllocaInst *v;
        llvm::Value *initV;
        if (value) {
            initV = value->genCode();
            if (!initV) return nullptr;
        }
        size_t array_size = 0;
        if (!levelSize.empty()) {   //是数组
            for (auto v:levelSize) {
                array_size *= v;
            }
        }
        llvm::Type* valType;
        if (type.isType(Type_Undefined)) {
            //需要自己生成类型
            valType = initV->getType();
        } else {
            valType = type.getType(module->core->context);
        }
        if (!levelSize.empty()) {
            for (auto size:levelSize) {
                valType = llvm::ArrayType::get(valType,size);
            }
        }
        v = module->CreateAlloca(module->Builder.GetInsertBlock()->getParent(), name,valType);
        if (value) {
            module->Builder.CreateStore(initV, v);
        }
        module->SetNamedValue(name, v);
        return module->Builder.CreateLoad(v, name);
    }
}