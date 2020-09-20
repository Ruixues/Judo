// 解析括号
#pragma once
#include "../core.h"
#include "../ast/AST.h"
#include "../loger.h"
namespace Parser {
     std::unique_ptr<AST::ExprAST> ParseParenExpr(Module* module);
}