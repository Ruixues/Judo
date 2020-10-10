#include "variableDefine.h"
#include "function.h"

namespace Parser {
    std::unique_ptr<AST::ExprAST> ParserVariableDefine(Module *module) {
        if (module->nowToken->type != token_var) {
            return module->loger->ParseError("Variable Define", "unexpected call to ParseVariableDefine");
        }
        auto name = module->ReadAToken();
        if (name->type != token_str) {
            return module->loger->ParseError("Variable Define", "expect the name of the variable after var");
        }
        //开始获取类型
        auto token = module->ReadAToken();
        std::shared_ptr<RToken> type;
        if (token->IsSign(":")) {
            //那就是指定了类型
            type = module->ReadAToken();    //吃掉了:
            if (type->type != token_str) {
                return module->loger->ParseError("Variable Define",
                                                 "expect to get the type of the variable after ':'");
            }
        }
        token = module->ReadAToken();
        if (!token->IsSign("=")) {  //那就是没有默认值
            if (!type) {
                //没有默认值，又没有名称，那就不可能推断类型
                return module->loger->ParseError("Variable Define",
                                                 "expect the type of variable or the initial value");
            }
            return make_AST<AST::VariableDefine>(module, name->GetStr(), nullptr, JudoType(type->GetStr()));
        }
        //有默认值
        module->ReadAToken();   //吃掉=
        auto initialValue = ParsePrimary(module);   //获取初始值
        if (!initialValue) return nullptr;
        if (!type) {
            return make_AST<AST::VariableDefine>(module, name->GetStr(), std::move(initialValue),
                                                 JudoType(Type_Undefined));
        }
        return make_AST<AST::VariableDefine>(module, name->GetStr(), std::move(initialValue), JudoType(type->GetStr()));
    }
}