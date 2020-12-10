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
        ~ClassItem() = default;
        ClassItem(std::unique_ptr<AST::VariableDefine> v) : variable(std::move(v)), type(Variable) {}
    };

    class ClassAST : public ExprAST {
    private:
    public:
        std::string name;
        std::vector<std::unique_ptr<ClassItem>> privates, publics;
        ClassAST(std::string name, std::vector<std::unique_ptr<ClassItem>> publics) : name(name),
                                                                                      publics(std::move(publics)) {
        }

        llvm::Value *genCode();
    };
}; // namespace AST