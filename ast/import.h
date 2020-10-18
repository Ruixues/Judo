#pragma once

#include "AST.h"
#include <vector>
#include "../other/split.h"

namespace AST {
    class ImportAST : public ExprAST {
    private:
        std::vector<std::string> ParseRoute(std::string path) {
            auto elements = split(path, "/");
            return elements;
        }

        std::vector<std::vector<std::string>> TargetModule;
        std::vector<std::string> AnotherName;
    public:
        ImportAST(std::vector<std::string> TargetModule, std::vector<std::string> AnotherName);
        llvm::Value * genCode();
    };
}