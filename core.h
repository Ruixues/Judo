#pragma once

#include <fstream>
#include <map>
#include <string>
#include <memory>
#include "reader.h"
#include "loger.h"
#include "opHandler.h"
//每个文件都是一个module.

class Module {
private:
    std::wifstream file;
    std::unique_ptr<RxReader> reader;
public:
    std::unique_ptr<Log> loger;
    std::shared_ptr<RToken> nowToken;
    llvm::LLVMContext context;
    llvm::IRBuilder<> Builder;
    std::unique_ptr<llvm::Module> module;
    std::map<std::string, llvm::Value *> namedValues;
    std::unique_ptr<OpHandler> opHandler;

    Module(std::string file);

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
    Judo(std::string EnterFile) {
        mainModule = new Module(EnterFile);
        modules[EnterFile] = std::shared_ptr<Module>(mainModule);
        // 准备开始解析模块
        mainModule->Parse();
    }
};