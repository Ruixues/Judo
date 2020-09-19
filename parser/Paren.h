// 解析括号
#pragma once
#include "../core.h"
#include "../ast/AST.h"
#include "../loger.h"
namespace Parser {
    static std::unique_ptr<AST::ExprAST> ParseParenExpr(Module *module)
    {
        module->ReadAToken();   //吃掉 (
        auto V = ParseExpression(); //解析中间的表达式
        if (!V)
            return nullptr;
        if (CurTok != ')')
            return module->loger->ParseError("There must be a ) matching (");
        module->ReadAToken();
        return V;
    }
}