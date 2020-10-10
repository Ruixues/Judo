#pragma once

#include <fstream>
#include <map>
#include <string>
#include <memory>
#include "reader.h"
#include "loger.h"
#include "opHandler.h"
#include "jit/jit.h"
#include <queue>
#include <stack>

//每个文件都是一个module.
class Judo;

class Module {
private:
    std::wifstream file;
    std::unique_ptr<RxReader> reader;
    std::stack<std::vector<std::string>> ScopeVariables;  //记录当前作用域的变量
public:
    std::unique_ptr<Log> loger;
    std::shared_ptr<RToken> nowToken;
    llvm::IRBuilder<> Builder;
    std::unique_ptr<llvm::Module> module;

    llvm::Function *getFunction(std::string Name);

    void EnterScope();  //进入作用域
    void ExitScope();  //退出作用域
    std::map<std::string, std::unique_ptr<AST::FunctionProto>> FunctionProto;
    std::map<std::string, std::stack<llvm::AllocaInst *>> namedValues;
    std::unique_ptr<OpHandler> opHandler;

    Module(std::string file, Judo *core);

    Judo *core;

    void Parse();

    llvm::AllocaInst *GetNamedValue(const std::string &Name);

    void SetNamedValue(const std::string &Name, llvm::AllocaInst *Value);

    void EraseValue(const std::string &Name);

    std::unique_ptr<AST::ExprAST> HandleToken(std::shared_ptr<RToken> token);

    llvm::AllocaInst *CreateAlloca(llvm::Function *Function,
                                   const std::string &VarName, llvm::Type *type);

    std::shared_ptr<RToken> ReadAToken() {
        nowToken = reader->ReadAToken();
        return nowToken;
    }
};

class Judo {
private:
    std::map<std::string, std::shared_ptr<Module>> modules;
    Module *mainModule; // 入口Module
public:
    llvm::LLVMContext context;
    std::unique_ptr<RJIT> JIT;
    std::unique_ptr<llvm::legacy::FunctionPassManager> FPM;

    Judo(std::string EnterFile);

    void InitializeModuleAndPassManager();
};