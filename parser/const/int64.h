#pragma once

#include "../../ast/const/int64.h"
#include "../../ast/AST.h"
#include <memory>
#include "../../core.h"

namespace Parser {
    std::unique_ptr<AST::ExprAST> ParseInt64(Module *module);
}