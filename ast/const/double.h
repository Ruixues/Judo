#pragma once

#include "../AST.h"
#include "../../llvmInclude.h"
/**
 * 该文件用来处理一些内置类型常量 比如double,int等
 **/
namespace AST {
    namespace Const {
        class TDouble : public AST::ExprAST {
        private:
            double data;
        public:
            TDouble(double data) : data(data) {}

            llvm::Value *genCode();
        };
    }; // namespace Const
};     // namespace AST