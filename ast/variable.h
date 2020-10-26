#pragma once

#include "AST.h"
#include <string>
#include "../type.h"
#include <iostream>
namespace AST {
    class VariableExpr : public ExprAST {
    private:
        std::string name;
        std::unique_ptr<AST::ExprAST> lv;  //对于是数组索引的
        std::unique_ptr<AST::ExprAST> index;
        std::unique_ptr<AST::ExprAST> accessRight;  //处理.
    public:
        VariableExpr(const std::string &name) : name(name) {
        }

        VariableExpr(std::unique_ptr<AST::ExprAST> lv, std::unique_ptr<AST::ExprAST> index) : lv(std::move(lv)),
                                                                                              index(std::move(index)) {
        }
        VariableExpr(std::unique_ptr<AST::ExprAST> accessRight):accessRight(std::move(accessRight)) {}   //创建结构体访问
        llvm::Value* getRealV();

        std::string GetName() { return name; }

        llvm::Value *genCode();
    };

    class VariableDefine : public ExprAST {
    private:
        std::string name;
        std::unique_ptr<ExprAST> value;
        JudoType type;
        llvm::Type *realType;
        bool isGlobal = false;
        std::vector<size_t> levelSize;  //每一维的大小，数组才需要提供
    public:

        VariableDefine(const std::string &name, std::unique_ptr<ExprAST> value, JudoType type,
                       std::vector<size_t> level,bool isGlobal) : name(name),
                                                    value(std::move(
                                                            value)), levelSize(std::move(level)),
                                                    type(type),isGlobal(isGlobal) {}

        llvm::Value *genCode();
    };
}