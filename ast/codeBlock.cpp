#include "codeBlock.h"
#include "../core.h"

namespace AST {
    llvm::Value *CodeBlockAST::genCode() {
        for (auto &v: codes) {
            if (!v->genCode()) {
                return nullptr;
            }
        }
        return (llvm::Value *) (1);   //占位
    }
}