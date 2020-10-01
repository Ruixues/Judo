#include "binaryOpt.h"
#include "../core.h"

namespace AST {
    std::map<std::string, int> BinopPrecedence = {{"<", 10},
                                                  {"+", 20},
                                                  {"-", 30},
                                                  {"*", 40}};

    llvm::Value *BinaryExprAST::genCode() {
        auto ll = l->genCode(), rr = r->genCode();
        auto f = module->opHandler->getBinaryOp(opt, ll, rr);
        if (!f)
            return nullptr;
        llvm::Value *Ops[2] = {ll, rr};
        return module->Builder.CreateCall(f, Ops, "binaryOp");
    }
}