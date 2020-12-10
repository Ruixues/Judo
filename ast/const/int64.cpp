#include "int64.h"
#include "../../core.h"

namespace AST {
    namespace Const {
        llvm::Value *TInt64::genCode() {
            return llvm::ConstantInt::get(module->core->context, llvm::APInt(64, data, true));
        }
    }
}