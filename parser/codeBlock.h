#pragma once

#include "../ast/AST.h"

namespace Parser {
    std::unique_ptr<AST::ExprAST> ParseCodeBlock(Module *module);
}