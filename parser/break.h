#pragma once

#include "../ast/AST.h"
#include "../ast/break.h"

namespace Parser {
    std::unique_ptr<AST::ExprAST> ParseBreak(Module *module);

    std::unique_ptr<AST::ExprAST> ParseContinue(Module *module);
}