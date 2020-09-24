#include "number.h"

namespace Parser {
    std::unique_ptr<AST::ExprAST> ParseNumber(Module *module) {
        auto token = module->nowToken;
        if (token->type == token_int) {
            return ParseInt32(module);
        }
        return nullptr;
    }
}