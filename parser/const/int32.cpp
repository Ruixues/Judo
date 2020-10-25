#include "int32.h"

namespace Parser {
    std::unique_ptr<AST::ExprAST> ParseInt32(Module *module) {
        if (module->nowToken->type != token_int) {
            return module->loger->ParseError("int32", "Unexpected call to ParseInt32");
        }
        auto num = module->nowToken->GetInt64();
        module->ReadAToken();   //吃掉数字
        return make_AST<AST::Const::TInt32>(module, int32(num));
    }
}