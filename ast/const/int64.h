#pragma once

#include "../AST.h"
#include "../../llvmInclude.h"
#include "../../type.h"

/**
 * 该文件用来处理一些内置类型常量 比如double,int等
 **/
namespace AST {
    namespace Const {
        class TInt64 : public AST::ExprAST {
        private:
            int64 data;
        public:
            TInt64(int64 data) : data(data) {}

            llvm::Value *genCode();
        };
    }; // namespace Const
};     // namespace AST