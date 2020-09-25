#pragma once

#include "../llvmInclude.h"
#include <memory>
#include <vector>
class Module;
namespace AST {
    class ExprAST {
    private:
        std::shared_ptr<Module> module;
    public:
        //virtual ~ExprAST();
        void SetModule (std::shared_ptr<Module> module) {
            this->module = module;
        }
        virtual llvm::Value *genCode(); //生成LLVM IR
    };
} // namespace AST