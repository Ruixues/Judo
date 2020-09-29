#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include "ast/function.h"
#include "ast/AST.h"
#include <string>

class Log {
private:
    std::ostream *ostream;
public:
    Log(std::ostream *ostream = nullptr) {
        if (ostream == nullptr) {
            this->ostream = &std::cout;
        }
    }

    std::unique_ptr<AST::ExprAST> ParseError(std::string ParseModule, std::string data) {
        *ostream << ParseModule << " " << data << std::endl;
        return nullptr;
    }

    std::unique_ptr<AST::FunctionProto> FunctionProtoParseError(std::string data) {
        *ostream << data << std::endl;
        return nullptr;
    }

    llvm::Function *GetFunctionError(std::string data) {
        *ostream << data << std::endl;
        return nullptr;
    }

    void *GenCodeError(std::string data) {
        *ostream << data << std::endl;
        return nullptr;
    }
};