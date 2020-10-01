#pragma once

#include "../llvmInclude.h"
#include <memory>
#include <vector>
#include <utility>

class Module;
namespace AST {
    class ExprAST {
    public:
        Module *module;

        virtual ~ExprAST() = default;

        void SetModule(Module *module);

        virtual llvm::Value *genCode() = 0; //生成LLVM IR
    };
} // namespace AST

template<typename T, typename... _Args>
std::unique_ptr<T> make_AST(Module *module, _Args &&... __args) {
    auto tmp = std::unique_ptr<T>(new T(std::forward<_Args>(__args)...));
    tmp->SetModule(module);
    return tmp;
}