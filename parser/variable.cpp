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
        if (!token->IsSign(":")) {
            return module->loger->ParseError("Variable Define",
                                             "need : to separate the type and name");
        }
        //开始获取类型
        auto type = module->ReadAToken();
        if (type->type != token_str) {
            return module->loger->ParseError("Variable Define",
                                             "expect to get the type of the variable after the name of it");
        }
        token = module->ReadAToken();
        if (!token->IsSign("=")) {  //那就是没有默认值
            return make_AST<AST::VariableDefine>(module,name->GetStr(), nullptr, JudoType(type->GetStr()));
        }
        //否则那就解析
        token = module->ReadAToken();   //吃掉=
        auto initialValue = ParsePrimary(module);
        if (!initialValue) return nullptr;
        auto t = make_AST<AST::VariableDefine>(module,name->GetStr(), std::move(initialValue), JudoType(type->GetStr()));
        return t;
    }
}