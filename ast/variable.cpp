#include "variable.h"
#include "../core.h"
#include "class.h"

namespace AST {
    llvm::Value *VariableExpr::getRealV() {
        if (!lv) {  //单纯的变量
            return module->GetNamedValue(name);
        }
        if (callFunction) { //创建函数调用
            return nullptr;
        }
        if (structIndex) {  //是结构体索引
            return getStructElementPtr();
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

    llvm::Value *VariableExpr::getStructElementPtr() {
        //开始向类型系统请求类
        auto lvar = dynamic_cast<AST::VariableExpr *> (lv.get());
        if (!lvar) {
            return module->loger->GenCodeError("expect struct to access by '.'");
        }
        auto l = lv->genCode();
        if (!l) {
            return nullptr;
        }
        if (!l->getType()->isStructTy()) {
            return module->loger->GenCodeError("expect struct to access by '.'");
        }
        auto name = l->getType()->getStructName();
        auto rclass = module->Type.getClass(std::string(name));
        if (!rclass) {
            return module->loger->GenCodeError("undefined class:" + std::string(name));
        }
        auto index = dynamic_cast<VariableExpr *>(structIndex.get());
        if (!index) {
            return module->loger->GenCodeError("expect string to access the struct by '.'");
        }
        //开始获取这个段的位置
        auto itemName = index->GetName();
        size_t i = 0, got = -1;
        for (auto &item:rclass->publics) {
            if (item->variable->GetName() == itemName) {
                got = i;
                break;
            }
            ++i;
        }
        if (got == -1) {
            return module->loger->GenCodeError(
                    "unexpected item:" + std::string(itemName) + " of class:" + std::string(name));
        }
        return module->Builder.CreateStructGEP(lvar->getRealV(), got);
    }

    llvm::Value *VariableExpr::genCode() {
        // If it is a function
        if (!name.empty()) {
            auto f = module->getFunction(name);
            if (f != nullptr) {
                return f;
            }
        }
        if (callFunction) { //创建函数调用
            auto f = callFunction->genCode();
            if (!f) return nullptr;
            if (!f->getType()->isFunctionTy() && !f->getType()->isPointerTy()) {
                return module->loger->GenCodeError("It must be a function to call");
            }
            //开始创建调用
            std::vector<llvm::Value *> argVs;
            for (auto &v:callArgs) {
                auto tmp = v->genCode();
                if (!tmp) {
                    return nullptr;
                }
                argVs.push_back(tmp);
            }
            return module->Builder.CreateCall((llvm::Function *) f, argVs, "calltmp");
        }
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
        llvm::Type *ttype;
        if (value) {
            initV = value->genCode();
            if (!initV) return nullptr;
        }
        if (!type) {
            //需要自己生成类型
            ttype = initV->getType();
        } else {
            ttype = type->ToType();
            for (auto size:levelSize) {
                ttype = llvm::ArrayType::get(ttype, size);
            }
        }
        if (isGlobal) { //全局变量特殊处理
            if (module->globalVariable.find(name) != module->globalVariable.end()) {
                return module->loger->GenCodeError("global variable:" + name + " has been defined");
            }
            module->globalVariable[name] = new llvm::GlobalVariable(*(module->module.get()), ttype, false,
                                                                    llvm::GlobalValue::LinkageTypes::ExternalLinkage,
                                                                    llvm::ConstantAggregateZero::get(ttype), name);
            auto var = module->globalVariable[name];
            var->setAlignment(llvm::MaybeAlign(4));
            module->SetNamedValue(name, var);
            //return module->Builder.CreateLoad(var, name);
            return llvm::ConstantFP::get(module->core->context, llvm::APFloat(1.0));;
        } else {
            v = module->CreateAlloca(module->Builder.GetInsertBlock()->getParent(), name, ttype);
            if (value) {
                module->Builder.CreateStore(initV, v);
            }
            module->SetNamedValue(name, v);
            return module->Builder.CreateLoad(v, name);
        }

    }
}