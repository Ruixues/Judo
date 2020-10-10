#include "if.h"
#include "../ast/if.h"
#include "../core.h"
#include "function.h"

namespace Parser {
    std::unique_ptr<AST::ExprAST> ParseIf(Module *module) {
        module->ReadAToken();   //吃掉if
        //开始解析条件
        auto condition = ParseExpression(module);
        if (!condition) {
            return nullptr;
        }
        auto token = module->nowToken;
        std::unique_ptr<AST::ExprAST> init, relse;
        if (token->IsSign(";")) {   //刚刚那个是初始化的
            init = std::move(condition);
            module->ReadAToken();   //吃掉;
            condition = ParseExpression(module);
            if (!init) {
                return nullptr;
            }
        }
        auto then = ParseExpression(module);
        if (!then) {
            return nullptr;
        }
        //判断有没有else
        if (module->nowToken->type == token_else) {
            module->ReadAToken();   //吃掉else
            relse = ParseExpression(module);
            if (!relse) {
                return nullptr;
            }
        }
        //return make_AST<AST::If>(module,(init != nullptr)?std::move(init):nullptr, (condition != nullptr)?std::move(condition):nullptr, (then != nullptr)?std::move(then): nullptr, (relse != nullptr)?std::move(relse): nullptr);
        return make_AST<AST::If>(module, std::move(init), std::move(condition), std::move(then), std::move(relse));
    }
}