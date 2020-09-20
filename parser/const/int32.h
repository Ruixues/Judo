#pragma once
#include "../../ast/const/int32.h"
#include "../../ast/AST.h"
#include <memory>
#include "../../core.h"
namespace Parser {
    std::unique_ptr<AST::ExprAST> ParseInt32 (Module* module);
}