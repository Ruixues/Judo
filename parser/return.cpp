#include "return.h"
#include "../core.h"
#include "function.h"
namespace Parser {
    std::unique_ptr<AST::ReturnAST> ParseReturn (Module* module) {
        module->ReadAToken();   //吃掉return
        auto v = ParseExpression(module);
        if (!v) return nullptr;
        return make_AST<AST::ReturnAST>(module,std::move(v));
    }
}