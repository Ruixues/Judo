#include "int32.h"

namespace Parser {
    std::unique_ptr<AST::ExprAST> ParseInt32(Module *module) {
        if (module->nowToken->type != token_int) {
            return module->loger->ParseError("int32", "Unexpected call to ParseInt32");
        }
        return std::make_unique<AST::Const::TInt32>(int32(module->nowToken->GetInt64()));
    }
}