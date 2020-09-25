#pragma once

#include "../llvmInclude.h"
#include <memory>
#include <vector>
class Module;
namespace AST {
    class ExprAST {
    private:
        Module* module;
    public:
        //virtual ~ExprAST();
        void SetModule (Module* module) {
            this->module = module;
        }
        //virtual llvm::Value *genCode(); //生成LLVM IR
    };
} // namespace AST
class Module;
template<typename T, typename... _Args>
std::unique_ptr<T> make_AST (Module* module,_Args&&... __args) {
    auto tmp = std::unique_ptr<T>(new T(std::forward<_Args>(__args)...));
    tmp->SetModule (module);
    return tmp;
}