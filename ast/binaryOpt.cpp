#include "binaryOpt.h"
#include "../core.h"
#include "variable.h"

namespace AST {
    std::map<std::string, int> BinopPrecedence = {
            {"[", 2},
            {"]", 2},
            {"==",2},
            {"=", 2},
            {"<", 10},
            {"+", 20},
            {"-", 30},
            {"*", 40}};
    llvm::Value *BinaryExprAST::genCode() {
        auto ll = l->genCode(), rr = r->genCode();
        if (opt == "=") {   //赋值
            VariableExpr *LHSE = dynamic_cast<VariableExpr *>(l.get());
            if (!LHSE)
                return module->loger->GenCodeError("destination of '=' must be a variable");
            auto val = r->genCode();
            if (!val) return nullptr;
            auto variable = module->GetNamedValue(LHSE->GetName());
            if (!variable) {
                return module->loger->GenCodeError("Unknown variable name");
            }
            //那就开始赋值
            module->Builder.CreateStore(val, variable);
            return val;
        }
        if (opt == "[") {   //进行索引
            auto realL = dynamic_cast<VariableExpr *>(l.get());
            if (!realL) {   //不是变量
                return module->loger->GenCodeError("source of '[' must be an array");
            }
            auto variable = module->GetNamedValue(realL->GetName());
            if (!variable->getAllocatedType()->isArrayTy()) { //必须是数组才能使用[]
                return module->loger->GenCodeError("source of '[' must be an array");
            }
            if (!ll->getType()->isArrayTy()) {
                return module->loger->GenCodeError("expect array for [] index");
            }
            if (!rr->getType()->isIntegerTy()) {
                return module->loger->GenCodeError("expect integer inside []");
            }
            auto elePtr = module->Builder.CreateGEP(variable,rr);
            //开始加载值
            llvm::Value* eleVal = module->Builder.CreateLoad(variable->getAllocatedType()->getArrayElementType(),elePtr);
            return eleVal;
        }
        auto v = module->opHandler->getBinaryOp(opt, ll, rr);
        return v;
    }
}