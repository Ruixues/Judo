#pragma once
#include <fstream>
#include <map>
#include <string>
#include <memory>
#include "reader.h"
//每个文件都是一个module.
class Module
{
private:
    std::ifstream file;
    std::unique_ptr<RxReader> reader;

public:
    Module(std::string file)
    {
        this->file.open(file, std::ios::in);
        reader = std::make_unique<RxReader>(this->file);
    }
    void Parse()
    {
        // 开始解析
        auto token = reader->ReadAToken();
        while (1)
        {
            switch (token->type)
            {
            case token_eof:
            {
                break;
            }
            }
        }
    }
    std::unique_ptr<RToken> ReadAToken()
    {
        return reader->ReadAToken();
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