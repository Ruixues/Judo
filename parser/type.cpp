#include "type.h"
#include "../core.h"

namespace Parser {
    std::unique_ptr<JudoTypeRef> ParseType(Module *module) {
        if (module->nowToken->type != token_str) {
            return nullptr;
        }
        auto t = module->nowToken;
        module->ReadAToken();  //吃掉类型
        return std::make_unique<JudoTypeRef>(module, t->GetStr());
        //return module->Type.GetType(t->GetStr());
    }
}