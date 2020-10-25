#pragma once

#include "AST.h"
#include <string>
#include <memory>
#include <vector>

namespace AST {
    class FunctionCall : public ExprAST {
    private:
        std::string functionName;
        std::vector<std::unique_ptr<ExprAST>> args;
    public:
        FunctionCall(std::string functionName, std::vector<std::unique_ptr<ExprAST>> args) : functionName(functionName),
                                                                                             args(std::move(args)) {

        }

        llvm::Value *genCode();
    };
}