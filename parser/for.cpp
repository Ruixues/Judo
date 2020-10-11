#include "for.h"
#include "../core.h"
#include "function.h"
#include "../ast/for.h"
namespace Parser {
    std::unique_ptr<AST::ExprAST> ParseFor(Module *module) {
        module->ReadAToken();   //吃掉for
        std::unique_ptr<AST::ExprAST> init,cond,update;
        cond = ParseExpression(module);
        if (cond) return nullptr;
        auto token = module->ReadAToken();
        if (token->IsSign(";")) {   //for init;cond;update
            init = std::move(cond);
            cond = ParseExpression(module);
            if (!cond) return nullptr;
            token = module->ReadAToken();
            if (!token->IsSign(";")) {
                return module->loger->ParseError("For","expect ; after condition of for");
            }
            update = ParseExpression(module);
            if (!update) return nullptr;
        }
        auto body = ParseExpression(module);
        if (!body) return nullptr;
        return make_AST<AST::For>(module,std::move (init),std::move (cond),std::move (update),std::move (body));
    }
}