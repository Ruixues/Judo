#include "int64.h"

namespace Parser {
    std::unique_ptr<AST::ExprAST> ParseInt64(Module *module) {
        if (module->nowToken->type != token_int) {
            return module->loger->ParseError("int64", "Unexpected call to ParseInt32");
        }
        auto num = module->nowToken->GetInt64();
        module->ReadAToken();   //吃掉数字
        return make_AST<AST::Const::TInt64>(module, int64(num));
    }
}