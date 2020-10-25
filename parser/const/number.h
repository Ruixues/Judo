#pragma once

#include "int32.h"

namespace Parser {
    std::unique_ptr<AST::ExprAST> ParseNumber(Module *module);
}