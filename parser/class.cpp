#include "class.h"
#include "../core.h"
#include "../ast/class.h"
#include "variableDefine.h"

namespace Parser {
    std::unique_ptr<AST::ClassItem> ParseClassItem(Module *module) {
        //目前只支持成员变量
        if (module->nowToken->type != token_var) {
            module->loger->ParseError("Class Item", "unsupported item type");
            return nullptr;
        }
        auto var = ParserVariableDefine(module, false);
        if (!var) {
            return nullptr;
        }
        auto item = std::make_unique<AST::ClassItem>(std::move(dynamic_pass<AST::VariableDefine>(std::move(var))));
        return item;
    }

    std::unique_ptr<AST::ExprAST> ParseClass(Module *module) {
        module->ReadAToken();    //吃掉class
        auto token = module->nowToken;
        std::string name;
        if (token->type != token_str) {
            return module->loger->ParseError("Class", "expect name for class");
        }
        name = token->GetStr();
        token = module->ReadAToken();
        if (!token->IsSign("{")) {
            return module->loger->ParseError("Class", "expect { for class");
        }
        module->ReadAToken();   //吃掉{
        std::vector<std::unique_ptr<AST::ClassItem>> items;
        while (!module->nowToken->IsSign("}")) {
            auto tmp = ParseClassItem(module);
            if (!tmp) {
                return nullptr;
            }
            items.push_back(std::move(tmp));
        }
        module->ReadAToken();   //吃掉}
        return make_AST<AST::ClassAST>(module, name, std::move(items));
    }
}