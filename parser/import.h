#pragma once

#include <memory>
#include <vector>
#include "../reader.h"
#include "../ast/AST.h"

namespace Parser {
    std::unique_ptr<AST::ExprAST> ParseImport(Module *module);
}