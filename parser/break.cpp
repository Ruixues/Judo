#include "break.h"
#include "../core.h"

namespace Parser {
    std::unique_ptr<AST::ExprAST> ParseBreak(Module *module) {
        module->ReadAToken();   //吃掉break
        return make_AST<AST::BreakAST>(module);
    }

    std::unique_ptr<AST::ExprAST> ParseContinue(Module *module) {
        module->ReadAToken();   //吃掉continue
        return make_AST<AST::ContinueAST>(module);
    }
}