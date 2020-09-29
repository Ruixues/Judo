#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../type.h"
#include "AST.h"

namespace AST {
    // 函数参数
    class FunctionArg {
    public:
        JudoType type;
        std::string name;

        FunctionArg(JudoType type, std::string name) : type(type), name(name) {}
    };

    class FunctionProto : public ExprAST {
    public:
        std::string name;
        std::vector<std::unique_ptr<FunctionArg>> args;
        std::string returnName; //返回变量的名字
        JudoType returnType;

        FunctionProto(std::string name, std::vector<std::unique_ptr<FunctionArg>> args, std::string returnName,
                      JudoType returnType) : name(name), args(std::move(args)), returnName(returnName),
                                             returnType(returnType) {
        }

        llvm::Value *genCode() {
            return nullptr;
        }

        llvm::Function *genFunction() {
            std::vector<llvm::Type *> argTypes;
            for (auto &arg:args) {

            }
        }
    };

    class FunctionAST : public ExprAST {
    private:
        std::unique_ptr<FunctionProto> proto;
        std::unique_ptr<ExprAST> code; //函数内容
    public:
        FunctionAST(std::unique_ptr<FunctionProto> proto, std::unique_ptr<ExprAST> code) : proto(std::move(proto)),
                                                                                           code(std::move(code)) {

        }

        llvm::Value *genCode() {
            auto f = module->module->getFunction(proto->name);
            if (!f) {
                f = proto->genFunction();
            }
            if (!f) {
                return nullptr;
            }
            if (!f->empty()) {
                return (llvm::Function *) module->loger->GenCodeError("Function cannot be redefined.");
            }
            auto BB = llvm::BasicBlock::Create(module->context, "entry", f);
            module->Builder.SetInsertPoint(BB);
            //开始进入新的作用域，应当改变部分变量的引用
            std::map<std::string, llvm::Value *> buf;
            //开始根据参数设置

        }

    };
}; // namespace AST