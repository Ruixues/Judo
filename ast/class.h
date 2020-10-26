#pragma once

#include "AST.h"

namespace AST {
    enum ClassItemType {
        Variable = 0,
        Function = 1,
    };
    class ClassItem {
    public:
        ClassItemType type;
        void* v;
        ClassItem (ClassItemType type,void* v):type (type),v (v) {}
    };
    class ClassAST : public ExprAST {
    private:
        std::string name;
        std::vector<ClassItem> privates,publics;
    public:
        ClassAST(std::string name,std::vector<ClassItem> privates,std::vector<ClassItem> publics):name(name),privates(privates),publics(publics) {}

        llvm::Value *genCode();
    };
}; // namespace AST