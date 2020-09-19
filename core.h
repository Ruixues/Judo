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
    Module(std::string file)
    {
        this->file.open(file, std::ios::in);
        reader = std::make_unique<RxReader>(&(this->file));
        loger = std::make_unique<Log>();
    }
    void Parse()
    {
        // 开始解析
        auto token = reader->ReadAToken();
        std::cout << token->type << std::endl;
        while (1)
        {
            if (token->type == token_eof)
            {
                break;
            }
//            switch (token->type)
//            {
//            case token_func:
//                Parser::ParseFunction(this);
//            }
        }
    }
    std::shared_ptr<RToken> ReadAToken()
    {
        nowToken = reader->ReadAToken();
        return nowToken;
    }
};
class Judo
{
private:
    std::map<std::string, std::unique_ptr<Module>> modules;
    Module *mainModule; // 入口Module
public:
    Judo(std::string EnterFile)
    {
        mainModule = new Module(EnterFile);
        modules[EnterFile] = std::unique_ptr<Module>(mainModule);
        // 准备开始解析模块
        mainModule->Parse();
    }
};