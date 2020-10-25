#include "for.h"
#include "../core.h"
#include "function.h"
#include "../ast/for.h"

namespace Parser {
    std::unique_ptr<AST::ExprAST> ParseFor(Module *module) {
        module->ReadAToken();   //吃掉for
        std::unique_ptr<AST::ExprAST> init, cond, update;
        cond = ParseExpression(module);
        if (!cond) return nullptr;
        if (module->nowToken->IsSign(";")) {   //for init;cond;update
            init = std::move(cond);
            module->ReadAToken();   //吃掉;
            cond = ParseExpression(module);
            if (!cond) return nullptr;
            auto token = module->ReadAToken();
            if (!token->IsSign(";")) {
                return module->loger->ParseError("For", "expect ; after condition of for");
            }
            module->ReadAToken();   //吃掉;
            update = ParseExpression(module);
            if (!update) return nullptr;
        }
        auto body = ParseExpression(module);
        return make_AST<AST::For>(module, std::move(init), std::move(cond), std::move(update), std::move(body));
    }
}