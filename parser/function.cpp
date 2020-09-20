#include "function.h"
#include "../core.h"
#include "../ast/binaryOpt.h"
#include "../ast/variable.h"
#include "../ast/FunctionCall.h"
#include <memory>
#include <vector>

namespace Parser {
     std::unique_ptr<AST::FunctionProto> ParseFunctionProto(Module *module) {
        if (module->nowToken->type != token_str) {
            return module->loger->FunctionProtoParseError("There must be the name of function after 'func'");
        }
        std::string FunctionName = *((std::string *) (*(module->nowToken->data)));
        auto token = module->ReadAToken();
        if (token->type != token_sign || *(std::string *) (*(token->data)) != "(") {
            return module->loger->FunctionProtoParseError("There must be a '(' after Function Name");
        }
        //开始解析参数
        std::vector<std::unique_ptr<AST::FunctionArg>> args;
        token = module->ReadAToken();
        while (token->type == token_str) {
            //获取到了参数名称
            std::string name = *((std::string *) (*(module->nowToken->data)));
            //开始获取参数类型
            auto type = module->ReadAToken();
            if (type->type != token_str) {
                return module->loger->FunctionProtoParseError("There must be the Type Name after the name of variable");
            }
            args.push_back(std::make_unique<AST::FunctionArg>(JudoType(*((std::string *) (*(type->data)))), name));
        }
        //开始处理返回值
        if (token->type != token_sign || *(std::string *) (*(token->data)) != ")") {
            return module->loger->FunctionProtoParseError("There must be a ')' after Args");
        }
        //开始处理返回值
        token = module->ReadAToken();
        if (token->type != token_sign || *(std::string *) (*(token->data)) != "(") {  //无返回值
            //返回值是null
            return std::make_unique<AST::FunctionProto>(FunctionName, std::move(args), "",
                                                        JudoType(Type_void));
        }
        token = module->ReadAToken();
        auto token2 = module->ReadAToken();
        if (token->type != token_str) {
            return module->loger->FunctionProtoParseError(
                    "Once you decided to add a return value,you can't make the () contains nothing");
        }
        if (token2->type == token_sign) {   //只是规定了类型
            if (*(std::string *) (*(token2->data)) != ")") {
                return module->loger->FunctionProtoParseError("There must be a ) after the Type of Return");
            }
            return std::make_unique<AST::FunctionProto>(FunctionName, std::move(args), "",
                                                        JudoType(*(std::string *) (*(token2->data))));
        }
        return std::make_unique<AST::FunctionProto>(FunctionName, std::move(args), *(std::string *) (*(token->data)),
                                                    JudoType(*(std::string *) (*(token2->data))));
    }

     int GetTokPrecedence(Module *module) {
        if (module->nowToken->type != token_str)
            return -1;
        //确定是一个存在的运算符
        if (AST::BinopPrecedence.find(module->nowToken->GetSign()) == AST::BinopPrecedence.end()) {
            return -1; //不存在就返回-1
        }
        int TokPrec = AST::BinopPrecedence[module->nowToken->GetSign()];
        if (TokPrec <= 0)
            return -1;
        return TokPrec;
    }

     std::unique_ptr<AST::ExprAST>
    ParseBinOpRHS(Module *module, int ExprPrec, std::unique_ptr<AST::ExprAST> LHS) {
        while (1) {
            int TokPrec = GetTokPrecedence(module);
            if (TokPrec < ExprPrec) //如果比上一个运算符小的话,比如1*2+3 则+3不能计算进去，就直接返回
                return LHS;
            // 已经确定了可以计算
            std::string BinOp = module->nowToken->GetSign();
            module->ReadAToken();
            // 获取操作符的右边第一个
            auto RHS = ParsePrimary(module);
            if (!RHS)
                return nullptr;
            int NextPrec = GetTokPrecedence(module); //获取右边的下一个运算符
            if (TokPrec < NextPrec)            //下下一个比自己和下一个的优先级大 所以就要先下一个，不能先自己.否则就直接自己和下一个结合
            {
                RHS = ParseBinOpRHS(module, TokPrec + 1, std::move(RHS)); //不能和自己与下一个大小相同
                if (!RHS)
                    return nullptr;
            }
            LHS = std::make_unique<AST::BinaryExprAST>(BinOp, std::move(LHS),
                                                       std::move(RHS));
        }
    }

     std::unique_ptr<AST::ExprAST> ParseExpression(Module *module) {
        auto LHS = ParsePrimary(module);
        if (!LHS)
            return nullptr;
        return ParseBinOpRHS(module, 0, std::move(LHS));
    }

     std::unique_ptr<AST::ExprAST> ParseIdentifierExpr(Module *module) {
        //判断是函数调用还是啥
        std::string strName = module->nowToken->GetStr();
        auto next = module->ReadAToken();
        if (next->type != token_str || next->GetStr() != "(") {
            //就是变量
            return std::make_unique<AST::VariableExpr>(strName);
        }
        //否则就是函数调用
        module->ReadAToken();   //吃掉(
        //开始处理参数
        std::vector<std::unique_ptr<AST::ExprAST>> Args;
        if (module->nowToken->type != token_sign || module->nowToken->GetSign() != ")") {
            //那就是有参数
            while (1) {
                if (auto Arg = Parser::ParseExpression(module))
                    Args.push_back(std::move(Arg));
                else
                    return nullptr;
                if (module->nowToken->type == token_sign && module->nowToken->GetSign() == ")") {
                    break;
                }
                if (module->nowToken->type != token_sign || module->nowToken->GetSign() != ",") {
                    return module->loger->ParseError("Function Call", "Expected ')' or ',' in argument list");
                }
            }
        }
        module->ReadAToken();   //吃掉)
        return std::make_unique<AST::FunctionCall>(strName, std::move(Args));
    }

     std::unique_ptr<AST::ExprAST> ParsePrimary(Module *module) {
        auto token = module->nowToken;
        switch (token->type) {
            default:
                return module->loger->ParseError("function", "未知的标记类型");
            case token_str:
                return ParseIdentifierExpr(module);   //可能是变量，也可能是函数调用
            case token_int:
//                return ParseNumberExpr();
            case token_sign:
            {
                if (token->GetSign() == "(") { //括号开始
                    return ParseParenExpr(module);
                }
            }
        }
        return nullptr;
    }

    std::unique_ptr<AST::FunctionAST> ParseFunction(Module *module) {
        auto token = module->ReadAToken();  //吃掉 func 标识符
        //开始解析函数的定义
        auto proto = ParseFunctionProto(module);
        if (!proto)
            return nullptr;
        //开始解析函数内部
        auto inside = ParseExpression(module);
        if (!inside)
            return nullptr;
        return std::make_unique<AST::FunctionAST>(std::move(proto), std::move(inside));
    }
}