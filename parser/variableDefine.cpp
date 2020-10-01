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
                                             "expect to get the type of the variable after the name of it");
        }
        //开始获取类型
        auto type = module->ReadAToken();
        if (token->type != token_str) {
            return module->loger->ParseError("Variable Define",
                                             "expect to get the type of the variable after the name of it");
        }
        token = module->ReadAToken();
        if (!token->IsSign("=")) {  //那就是没有默认值
            return std::make_unique<AST::VariableDefine>(name->GetStr(), nullptr, JudoType(type->GetStr()));
        }
        //否则那就解析
        auto initialValue = ParsePrimary(module);
        return std::make_unique<AST::VariableDefine>(name->GetStr(), std::move(initialValue), JudoType(type->GetStr()));
    }
}