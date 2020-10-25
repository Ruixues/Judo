#pragma once

#include "../ast/AST.h"

class Module;
namespace Parser {
    std::unique_ptr<AST::ExprAST> ParseCodeBlock(Module *module);
}