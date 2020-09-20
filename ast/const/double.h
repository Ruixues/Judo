#pragma once

#include "../AST.h"
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
        };
    }; // namespace Const
};     // namespace AST