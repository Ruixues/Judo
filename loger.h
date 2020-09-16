#pragma once
#include <fstream>
#include <iostream>
#include <memory>
#include "ast/function.h"
class Log {
private:
    std::ostream* ostream;
public:
    Log (std::ostream* ostream = nullptr) {
        if (ostream == nullptr) {
            this->ostream = std::cout;
        }
    }
    void ParseError (string ParseModule,string data) {

    }
    std::unique_ptr<AST::FunctionProto> FunctionProtoParseError (string data) {
        *ostream << data;
        return nullptr;
    }
};