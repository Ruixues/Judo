#pragma once

#include "../ast/AST.h"
#include "../ast/variable.h"
#include "../core.h"

namespace Parser {
    std::unique_ptr<AST::ExprAST> ParserVariableDefine(Module *module,bool global = false);
}