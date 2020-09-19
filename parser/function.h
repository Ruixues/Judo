#pragma once

#include "../ast/function.h"
#include <memory>
#include <vector>
#include "../reader.h"
class Module;   //解决循环引用
namespace Parser
{
    static std::unique_ptr<AST::FunctionProto> ParseFunctionProto (Module* module);
    static std::unique_ptr<AST::FunctionAST> ParseFunction(Module *module);
    static std::unique_ptr<AST::ExprAST> ParseExpression();
    static std::unique_ptr<AST::ExprAST> ParsePrimary(Module *module,std::unique_ptr<RToken> token);
    static std::unique_ptr<AST::ExprAST> ParseParenExpr(Module *module);
} // namespace Parser