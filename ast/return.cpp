#include "return.h"
#include "../core.h"

namespace AST {
    llvm::Value *ReturnAST::genCode() {
        if (!retValue) {
            module->Builder.CreateRetVoid();
        } else {
            auto value = retValue->genCode();
            if (!value) return nullptr;
            module->Builder.CreateRet(value);
        }
        return (llvm::Value *) 1;
    }
}