#include "type.h"
#include "../core.h"

namespace Parser {
    llvm::Type* ParseType(Module *module) {
        if (module->nowToken->type != token_str) {
            return nullptr;
        }
        auto t = module->nowToken;
        module->ReadAToken();  //吃掉类型
        return module->Type.GetType(t->GetStr());
    }
}