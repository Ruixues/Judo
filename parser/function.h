#pragma once
#include "../ast/function.h"
#include <memory>
#include <vector>
#include "../core.h"
#include "../type.h"
#include "../reader.h"
namespace Parser
{
    std::unique_ptr<AST::FunctionProto> ParseFunctionProto (Module *module) {
        if (module->nowToken->type != token_str) {
            return module->loger->FunctionProtoParseError("There must be the name of function after 'func'");
        }
        std::string FunctionName = *((std::string*)(*(module->nowToken->data)));
        auto token = module->ReadAToken();
        if (token->type != token_sign || *(std::string*)(*(token->data)) != "(") {
            return module->loger->FunctionProtoParseError("There must be a '(' after Function Name");
        }
        //开始解析参数
        std::unique_ptr<std::vector<AST::FunctionArg>> args = std::make_unique<std::vector<AST::FunctionArg>>();
        token = module->ReadAToken();
        while (token->type == token_str) {
            //获取到了参数名称
            std::string name = *((std::string*)(*(module->nowToken->data)));
            //开始获取参数类型
            auto type = module->ReadAToken();
            if (type->type != token_str) {
                return module->loger->FunctionProtoParseError("There must be the Type Name after the name of variable");
            }
            args->push_back(AST::FunctionArg(JudoType(*((std::string*)(*(type->data)))),name));
        }
        //开始处理返回值
        if (token->type != token_sign || *(std::string*)(*(token->data)) != ")") {
            return module->loger->FunctionProtoParseError("There must be a ')' after Args");
        }
        //开始处理返回值
        token = module->ReadAToken();
        if (token->type != token_sign || *(std::string*)(*(token->data)) != "(") {  //无返回值
            //返回值是null
            return std::make_unique<AST::FunctionProto>(FunctionName,std::move(args),std::string(""),JudoType(Type_void));
        }
        token = module->ReadAToken();
        auto token2 = module->ReadAToken();
        if (token->type != token_str) {
            return module->loger->FunctionProtoParseError("Once you decided to add a return value,you can't make the () contains nothing");
        }
        if (token2->type == token_sign) {   //只是规定了类型
            if (*(std::string*)(*(token2->data)) != ")") {
                return module->loger->FunctionProtoParseError("There must be a ) after the Type of Return");
            }
            return std::make_unique<AST::FunctionProto>(FunctionName,std::move(args),"",JudoType(*(std::string*)(*(token2->data))));
        }
        return std::make_unique<AST::FunctionProto>(FunctionName,std::move(args),*(std::string*)(*(token->data)),JudoType(*(std::string*)(*(token2->data))));
    }
    std::unique_ptr<AST::FunctionAST> ParseFunction(Module *module)
    {
        auto token = module->ReadAToken();  //吃掉 func 标识符
        //开始解析函数的定义
        auto proto = ParseFunctionProto (module);
        //开始解析函数内部

    }
    static std::unique_ptr<AST::ExprAST> ParseExpression()
    {
        auto LHS = ParsePrimary();
        if (!LHS)
            return nullptr;
        return ParseBinOpRHS(0, std::move(LHS));
    }
}; // namespace Parser