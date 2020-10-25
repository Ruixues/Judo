#include "number.h"
#include "int32.h"
#include "int64.h"
namespace Parser {
    std::unique_ptr<AST::ExprAST> ParseNumber(Module *module) {
        auto token = module->nowToken;
        if (token->type == token_int) {
            return ParseInt64(module);
        }
        return nullptr;
    }
}