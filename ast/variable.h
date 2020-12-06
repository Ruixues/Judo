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
        std::unique_ptr<AST::ExprAST> structIndex;  //索引.
        llvm::Value* getStructElementPtr ();
    public:
        VariableExpr(const std::string &name) : name(name) {
        }

        VariableExpr(std::unique_ptr<AST::ExprAST> l,std::unique_ptr<AST::ExprAST> structIndex,bool) : lv (std::move (l)),structIndex(std::move(structIndex)) {}

        VariableExpr(std::unique_ptr<AST::ExprAST> lv, std::unique_ptr<AST::ExprAST> index) : lv(std::move(lv)),
                                                                                              index(std::move(index)) {
        }

        llvm::Value *getRealV();
        std::string GetName() { return name; }

        llvm::Value *genCode();
    };

    class VariableDefine : public ExprAST {
    private:
        std::string name;
        std::unique_ptr<ExprAST> value;
        bool isGlobal = false;
        std::vector<size_t> levelSize;  //每一维的大小，数组才需要提供
    public:
        std::unique_ptr<JudoTypeRef> type;
        std::string GetName () {return name;}
        VariableDefine(const std::string &name, std::unique_ptr<ExprAST> value, std::unique_ptr<JudoTypeRef> type,
                       std::vector<size_t> level, bool isGlobal) : name(name),
                                                                   value(std::move(
                                                                           value)), levelSize(std::move(level)),
                                                                   type(std::move(type)), isGlobal(isGlobal) {}

        llvm::Value *genCode();
    };
}