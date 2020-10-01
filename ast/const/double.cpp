#include "double.h"
#include "../../core.h"

namespace AST {
    namespace Const {
        llvm::Value *TDouble::genCode() {
            return llvm::ConstantFP::get(module->context, llvm::APFloat(data));
        }
    }
}