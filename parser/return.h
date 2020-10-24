#pragma once

#include "../ast/return.h"

namespace Parser {
    std::unique_ptr<AST::ReturnAST> ParseReturn(Module *module);
}