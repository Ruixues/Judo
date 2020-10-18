#include "binaryOpt.h"
#include "../core.h"
#include "variable.h"

namespace AST {
    std::map<std::string, int> BinopPrecedence = {
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
        auto v = module->opHandler->getBinaryOp(opt, ll, rr);
        return v;
    }
}