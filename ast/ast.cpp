#include "AST.h"
#include "../core.h"
#include <utility>


namespace AST {
    void ExprAST::SetModule(Module *module) {
        this->module = module;
    }
}