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
    std::string filePath;
public:
    std::unique_ptr<llvm::Module> Compile ();
    std::stack<llvm::BasicBlock *> ForThenBlock;    //记录当前处于的for作用域的下一个块,帮助实现continue,for等东西
    std::stack<llvm::BasicBlock *> NowForBlock;  //当前循环体的判断体
    std::unique_ptr<Log> loger;
    std::shared_ptr<RToken> nowToken;
    llvm::IRBuilder<> Builder;
    std::unique_ptr<llvm::Module> module;
    JudoTypeSystem Type;
    std::unique_ptr<llvm::legacy::FunctionPassManager> FPM;
    llvm::Function *getFunction(std::string Name);

    std::map<std::string, llvm::GlobalVariable *> globalVariable;

    void EnterScope();  //进入作用域
    void ExitScope();  //退出作用域
    std::map<std::string, std::unique_ptr<AST::FunctionProto>> FunctionProto;
    std::map<std::string, std::stack<llvm::Value *>> namedValues;
    llvm::Value *nowFunctionReturnValue;    //当前函数的返回值
    std::unique_ptr<OpHandler> opHandler;

    Module(std::string file, Judo *core);

    Judo *core;

    void Parse();

    llvm::Value *GetNamedValue(const std::string &Name);

    void SetNamedValue(const std::string &Name, llvm::Value *Value);

    void EraseValue(const std::string &Name);

    std::unique_ptr<AST::ExprAST> HandleToken(std::shared_ptr<RToken> token);

    llvm::Value *CreateAlloca(llvm::Function *Function,
                              const std::string &VarName, llvm::Type *type);

    ~Module();

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
    Judo(std::string EnterFile);

    bool CallFunctionFromMainModule(std::string FunctionName);
    bool RequireModule (std::string Module);    // 加载Module
    std::unique_ptr<llvm::legacy::FunctionPassManager> InitializeModuleAndPassManager(llvm::Module* module);
};