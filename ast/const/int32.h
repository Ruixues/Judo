#pragma once

#include "../AST.h"
//#include "../../llvmInclude.h"
/**
 * 该文件用来处理一些内置类型常量 比如double,int等
 **/
namespace AST {
    namespace Const {
        class TInt32 : public AST::ExprAST {
        private:
            int32_t data;
        public:
            TInt32(int32_t data) : data(data) {}
//            llvm::Value* genCode () {
//
//            }
        };
    }; // namespace Const
};     // namespace AST