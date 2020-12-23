#include "function.h"
#include "../core.h"
#include "../ast/binaryOpt.h"
#include <memory>
#include <vector>
#include "../ast/variable.h"
#include "const/number.h"
#include "codeBlock.h"
#include "if.h"
#include "variableDefine.h"
#include "for.h"
#include "break.h"
#include "return.h"
#include "type.h"

namespace Parser {
    std::unique_ptr<AST::FunctionProto> ParseFunctionProto(Module *module) {
        if (module->nowToken->type != token_str) {
            return module->loger->FunctionProtoParseError("There must be the name of function after 'func'");
        }
        std::string FunctionName = module->nowToken->GetStr();
        auto token = module->ReadAToken();
        if (token->type != token_sign || token->GetSign() != "(") {
            return module->loger->FunctionProtoParseError("There must be a '(' after Function Name");
        }
        //开始解析参数
        std::vector<std::unique_ptr<AST::FunctionArg>> args;
        token = module->ReadAToken();
        while (token->type == token_str) {
            //获取到了参数名称
            std::string name = token->GetStr();
            //开始获取参数类型
            auto type = module->ReadAToken();
            if (type->type != token_str) {
                return module->loger->FunctionProtoParseError("There must be the Type Name after the name of variable");
            }
            args.push_back(std::make_unique<AST::FunctionArg>(module->Type.GetType(type->strData), name));
            token = module->ReadAToken();
        }
        //开始处理返回值
        if (!token->IsSign(")")) {
            return module->loger->FunctionProtoParseError("There must be a ')' after Args");
        }
        token = module->ReadAToken();
        if (!token->IsSign("(")) {  //无返回值
            //返回值是null
            return make_AST<AST::FunctionProto>(module, FunctionName, std::move(args),
                                                std::make_unique<JudoTypeRef>(module, Type_void));
        }
        //吃掉(
        module->ReadAToken();
        auto type = ParseType(module);
        if (!type) {
            return nullptr;
        }
        module->ReadAToken();   //吃掉)
        return make_AST<AST::FunctionProto>(module, FunctionName, std::move(args),
                                            std::move(type));
    }

    int GetTokPrecedence(Module *module) {
        if (module->nowToken->type != token_sign) {
            return 10000;
        }
        //确定是一个存在的运算符
        if (AST::BinopPrecedence.find(module->nowToken->GetSign()) == AST::BinopPrecedence.end()) {
            return 10000;
        }
        int TokPrec = AST::BinopPrecedence[module->nowToken->GetSign()];
        return TokPrec;
    }

    std::unique_ptr<AST::ExprAST>
    ParseBinOpRHS(Module *module, int ExprPrec, std::unique_ptr<AST::ExprAST> LHS) {
        while (1) {
            int TokPrec = GetTokPrecedence(module);
            if (TokPrec > ExprPrec) {
                return LHS;
            }
            // 已经确定了可以计算
            std::string BinOp = module->nowToken->GetSign();
            module->ReadAToken();
            if (BinOp == "(") { //特殊处理，因为有多个参数
                std::vector<std::unique_ptr<AST::ExprAST>> args;
                while (true) {
                    if (module->nowToken->IsSign(")")) {
                        break;
                    }
                    auto tmp = ParsePrimary(module);
                    if (!tmp) {
                        return tmp;
                    }
                    args.push_back(std::move(tmp));
                }
                module->ReadAToken();   //吃掉)
                // 开始构建
                return make_AST<AST::VariableExpr>(module, std::move(LHS), std::move(args));
            }
            // 获取操作符的右边第一个
            auto RHS = ParsePrimary(module);
            if (!RHS)
                return nullptr;
            int NextPrec = GetTokPrecedence(module); //获取右边的下一个运算符
            if (TokPrec > NextPrec)            //下下一个比自己和下一个的优先级小 所以就要先下一个，不能先自己.否则就直接自己和下一个结合
            {
                RHS = ParseBinOpRHS(module, TokPrec - 1, std::move(RHS)); //不能和自己与下一个大小相同
                if (!RHS)
                    return nullptr;
            }
            if (BinOp == "[") { //那末尾还有一个]
                if (!module->nowToken->IsSign("]")) {
                    return module->loger->ParseError("BinOp", "expect ] for [");
                }
                module->ReadAToken();   //吃掉]
                LHS = make_AST<AST::VariableExpr>(module, std::move(LHS), std::move(RHS));
            } else if (BinOp == ".") {
                LHS = make_AST<AST::VariableExpr>(module, std::move(LHS), std::move(RHS), true);
            } else {
                LHS = make_AST<AST::BinaryExprAST>(module, BinOp, std::move(LHS),
                                                   std::move(RHS));
            }
        }
    }

    std::unique_ptr<AST::ExprAST> ParseExpression(Module *module) {
        auto LHS = ParsePrimary(module);
        if (!LHS)
            return nullptr;
        return ParseBinOpRHS(module, 100, std::move(LHS));
    }

    std::unique_ptr<AST::ExprAST> ParseIdentifierExpr(Module *module) {
        //判断是函数调用还是啥
        std::string strName = module->nowToken->GetStr();
        module->ReadAToken();
        //auto next = module->ReadAToken();
        //if (!next->IsSign("(")) {
        //就是变量
        return make_AST<AST::VariableExpr>(module, strName);
        //}
        //否则就是函数调用
        module->ReadAToken();   //吃掉(
        //开始处理参数
//        std::vector<std::unique_ptr<AST::ExprAST>> Args;
//        if (!module->nowToken->IsSign(")")) {
//            //那就是有参数
//            while (true) {
//                if (auto Arg = Parser::ParseExpression(module))
//                    Args.push_back(std::move(Arg));
//                else
//                    return nullptr;
//                if (module->nowToken->IsSign(")")) {
//                    break;
//                }
//                if (!module->nowToken->IsSign(",")) {
//                    return module->loger->ParseError("Function Call", "Expected ')' or ',' in argument list");
//                }
//                module->ReadAToken();   //吃掉分隔符
//            }
//        }
//        module->ReadAToken();   //吃掉)
//        return make_AST<AST::FunctionCall>(module, strName, std::move(Args));
    }

    std::unique_ptr<AST::ExprAST> ParsePrimary(Module *module) {
        auto token = module->nowToken;
        switch (token->type) {
            default:
                return module->loger->ParseError("Parse Primary", "未知的标记类型");
            case token_str:
                return ParseIdentifierExpr(module);   //可能是变量，也可能是函数调用
            case token_int:
                return ParseNumber(module);
            case token_var:
                return ParserVariableDefine(module);
            case token_eof:
                return nullptr;
            case token_sign: {
                if (token->IsSign("(")) { //括号开始
                    return ParseParenExpr(module);
                }
                if (token->IsSign("{")) {
                    return ParseCodeBlock(module);
                }
                return module->loger->ParseError("Parse Primary", "未知符号:" + token->GetSign());
            }
            case token_for:
                return ParseFor(module);
            case token_if:
                return ParseIf(module);
            case token_break:
                return ParseBreak(module);
            case token_continue:
                return ParseContinue(module);
            case token_return:
                return ParseReturn(module);
        }
    }

    std::unique_ptr<AST::FunctionAST> ParseFunction(Module *module) {
        module->ReadAToken();  //吃掉 func 标识符
        //开始解析函数的定义
        auto proto = ParseFunctionProto(module);
        if (!proto)
            return nullptr;
        //开始解析函数内部
        auto inside = ParseExpression(module);
        if (!inside) {
            return nullptr;
        }
        return make_AST<AST::FunctionAST>(module, std::move(proto), std::move(inside));
    }
}