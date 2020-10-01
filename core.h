#pragma once

#include <fstream>
#include <map>
#include <string>
#include <memory>
#include "reader.h"
#include "loger.h"
#include "opHandler.h"
//每个文件都是一个module.
class RJIT; //加快编译速度
class Judo;
class Module {
private:
    std::wifstream file;
    std::unique_ptr<RxReader> reader;
public:
    std::unique_ptr<Log> loger;
    std::shared_ptr<RToken> nowToken;
    llvm::IRBuilder<> Builder;
    std::unique_ptr<llvm::Module> module;
    std::map<std::string, llvm::Value *> namedValues;
    std::unique_ptr<OpHandler> opHandler;
    Module(std::string file,Judo* core);
    Judo* core;
    void Parse();
    std::unique_ptr<AST::ExprAST> HandleToken(std::shared_ptr<RToken> token);

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