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

template<typename T, typename S>
std::unique_ptr<T> dynamic_pass(std::unique_ptr<S> source) {
    T *tmp = dynamic_cast<T *> (source.get());
    source.release();
    std::unique_ptr<T> ret;
    ret.reset(tmp);
    return ret;
}