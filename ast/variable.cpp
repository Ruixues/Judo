#include "variable.h"
#include "../core.h"

namespace AST {
    llvm::Value *VariableExpr::getRealV() {
        if (!lv) {  //单纯的变量
            return module->GetNamedValue(name);
        }
        //否则那就索引数组
        auto la = dynamic_cast<VariableExpr *>(lv.get());
        if (!la) {
            module->loger->GenCodeError("expect a variable to access");
            return nullptr;
        }
        auto ll = la->genCode(), rr = index->genCode();
        if (!ll->getType()->isPointerTy()) {
            module->loger->GenCodeError("expect pointer for [] to index");
            return nullptr;
        }
        if (!rr->getType()->isIntegerTy()) {
            module->loger->GenCodeError("expect integer inside []");
            return nullptr;
        }
        //开始索引
        return module->Builder.CreateGEP(ll, rr);
    }

    llvm::Value *VariableExpr::genCode() {
        if (!name.empty()) {
            auto tt = module->GetNamedValue(name);
            if (tt->getType()->getPointerElementType()->isArrayTy()) {
                return module->Builder.CreateBitCast(tt, llvm::PointerType::get(
                        tt->getType()->getPointerElementType()->getArrayElementType(), 0));
            }
        }
        return module->Builder.CreateLoad(getRealV());
    }

    llvm::Value *VariableDefine::genCode() {
        //生成一个变量
        llvm::Value *v;
        llvm::Value *initV;
        if (value) {
            initV = value->genCode();
            if (!initV) return nullptr;
        }
        if (!type) {
            //需要自己生成类型
            type = initV->getType();
        } else {
            if (!levelSize.empty()) {
                for (auto size:levelSize) {
                    type = llvm::ArrayType::get(type, size);
                }
            }
        }
        if (isGlobal) { //全局变量特殊处理
            if (module->globalVariable.find(name) != module->globalVariable.end()) {
                return module->loger->GenCodeError("global variable:" + name + " has been defined");
            }
            module->globalVariable[name] = new llvm::GlobalVariable(*(module->module.get()), type, false,
                                                                    llvm::GlobalValue::LinkageTypes::ExternalLinkage,
                                                                    llvm::ConstantAggregateZero::get(type), name);
            auto var = module->globalVariable[name];
            var->setAlignment(llvm::MaybeAlign(4));
            module->SetNamedValue(name, var);
            return module->Builder.CreateLoad(var, name);
        } else {
            v = module->CreateAlloca(module->Builder.GetInsertBlock()->getParent(), name, type);
            if (value) {
                module->Builder.CreateStore(initV, v);
            }
            module->SetNamedValue(name, v);
            return module->Builder.CreateLoad(v, name);
        }

    }
}