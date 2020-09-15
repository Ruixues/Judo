#pragma once
#include <string>
#include <vector>
#include <memory>
#include "../type.h"
#include "AST.h"
// 函数参数
namespace AST
{
    class FunctionArg
    {
        JotoType type;
        std::string name;
    };
    class FunctionProto
    {
    private:
        std::unique_ptr<std::vector<FunctionArg>> args;

    public:
        FunctionProto(std::unique_ptr<std::vector<FunctionArg>> args) : args(std::move(args))
        {
        }
    };
    class FunctionAST : public ExprAST
    {
    private:
        std::unique_ptr<FunctionProto> proto;
        std::unique_ptr<ExprAST> code; //函数内容
    public:
        FunctionAST(std::unique_ptr<FunctionProto> proto, std::unique_ptr<ExprAST> code) : proto(std::move(proto)), code(std::move(code))
        {
        }
    };
}; // namespace AST