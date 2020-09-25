#pragma once

#include <fstream>
#include <map>
#include <string>
#include <memory>
#include "reader.h"
#include "parser/parser.h"
#include "loger.h"
//每个文件都是一个module.
class Module
{
private:
    std::wifstream file;
    std::unique_ptr<RxReader> reader;

public:
    std::unique_ptr<Log> loger;
    std::shared_ptr<RToken> nowToken;
    // llvm::LLVMContext context;
    // llvm::IRBuilder<> Builder;
    // std::unique_ptr<llvm::Module> module;
    Module(std::string file)// : Builder(context)
    {
        this->file.open(file, std::ios::in);
        if (!this->file.good())
        {
            std::cout << "Open Module " << file << " Error" << std::endl;
            return;
        }
        this->file >> std::noskipws;
        reader = std::make_unique<RxReader>(&(this->file));
        loger = std::make_unique<Log>();
    }

    void Parse();

    std::unique_ptr<AST::ExprAST> HandleToken(std::shared_ptr<RToken> token);

    std::shared_ptr<RToken> ReadAToken()
    {
        nowToken = reader->ReadAToken();
        return nowToken;
    }
};

class Judo
{
private:
    std::map<std::string, std::shared_ptr<Module>> modules;
    Module *mainModule; // 入口Module
public:
    Judo(std::string EnterFile)
    {
        mainModule = new Module(EnterFile);
        modules[EnterFile] = std::shared_ptr<Module>(mainModule); 
        // 准备开始解析模块
        mainModule->Parse();
    }
};