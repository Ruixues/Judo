#include "codeBlock.h"
#include "../core.h"
#include "../other/defer.h"

namespace AST {
    llvm::Value *CodeBlockAST::genCode() {
        module->EnterScope();
        defer(
                module->ExitScope();
        );
        for (auto &v: codes) {
            if (!v->genCode()) {
                return nullptr;
            }
        }
        return (llvm::Value*)1;   //占位
    }
}