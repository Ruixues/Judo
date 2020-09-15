#pragma once
#include <fstream>
#include <map>
#include <string>
#include <memory>

//每个文件都是一个module.
class Module {
private:
    std::ifstream file;
public:
    Module (std::string file) {
        //开始初始化module
        this->file.open(file, std::ios::in);
        //准备开始解析
    }
};
class Judo {
private:
    std::map<std::string,std::unique_ptr<Module> modules;
    Module* mainModule; //入口Module
public:
    Judo(std::string EnterFile) {
        mainModule = new Module ();
    }
};