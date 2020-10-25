#include "parser.h"
#include "Paren.h"

namespace Parser {
    std::unique_ptr<AST::ExprAST> ParseParenExpr(Module *module) {
        module->ReadAToken();   //吃掉 (
        auto V = ParseExpression(module); //解析中间的表达式
        if (!V)
            return nullptr;
        if (!module->nowToken->IsSign(")"))
            return module->loger->ParseError("Paren", "There must be a ) matching (");
        module->ReadAToken();
        return V;
    }
}