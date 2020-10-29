#pragma once

#include "AST.h"
#include "variable.h"
namespace AST {
    enum ClassItemType {
        Variable = 0,
        Function = 1,
    };

    class ClassItem {
    public:
        ClassItemType type;
        std::unique_ptr<AST::VariableDefine> variable;

        ClassItem(ClassItemType type, std::unique_ptr<AST::VariableDefine> v) : type(type), variable(std::move (v)) {}
    };

    class ClassAST : public ExprAST {
    private:
        std::string name;
        std::vector<ClassItem> privates, publics;
    public:
        ClassAST(std::string name, std::vector<ClassItem> privates, std::vector<ClassItem> publics) : name(name),
                                                                                                      privates(
                                                                                                              std::move(privates)),
                                                                                                      publics(std::move(publics)) {}

        llvm::Value *genCode();
    };
}; // namespace AST