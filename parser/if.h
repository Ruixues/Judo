#pragma once

#include <memory>
#include <vector>
#include "../reader.h"
#include "../ast/AST.h"

class Module;   //解决循环引用

namespace Parser {
    std::unique_ptr<AST::ExprAST> ParseIf(Module *module);
}