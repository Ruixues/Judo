#include "extern.h"
#include "../core.h"
#include "function.h"
#include "../ast/extern.h"

namespace Parser {
    std::unique_ptr<AST::ExprAST> ParseExtern(Module *module) {
        module->ReadAToken();   //吃掉extern
        auto proto = ParseFunctionProto(module);
        if (proto) {
            module->FunctionProto[proto->name] = std::move(proto);
        }
        return make_AST<AST::ExternAST>(module);
    }
}