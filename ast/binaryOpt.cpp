#include "binaryOpt.h"
#include "../core.h"
#include "variable.h"

namespace AST {
    std::map<std::string, int> BinopPrecedence = {
            {"[",  1},
            {"]",  1},
            {"==", 7},
            {"!=", 7},
            {"=",  14},
            {"<",  6},
            {"+",  4},
            {"-",  4},
            {".", 1},
            {"*",  3}};

    llvm::Value *BinaryExprAST::genCode() {
        if (opt == "=") {   //赋值
            VariableExpr *LHSE = dynamic_cast<VariableExpr *>(l.get());
            if (!LHSE)
                return module->loger->GenCodeError("destination of '=' must be a variable");
            auto val = r->genCode();
            if (!val) return nullptr;
            llvm::Value *variable = LHSE->getRealV();
            if (!variable) {
                return module->loger->GenCodeError("unepected variable");
            }
            //那就开始赋值
            module->Builder.CreateStore(val, variable);
            return val;
        }
        auto ll = l->genCode(), rr = r->genCode();
        auto v = module->opHandler->getBinaryOp(opt, ll, rr);
        return v;
    }
}