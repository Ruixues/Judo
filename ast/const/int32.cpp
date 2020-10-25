#include "int32.h"
#include "../../core.h"

namespace AST {
    namespace Const {
        llvm::Value *TInt32::genCode() {
            return llvm::ConstantInt::get(module->core->context, llvm::APInt(32, data, true));
        }
    }
}