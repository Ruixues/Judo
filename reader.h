/**
 * 本文件用来进行token的读取
 **/
#pragma once

#include <set>
#include <string>
#include <fstream>
#include <memory>
#include <set>
#include "type.h"

static bool isspace(wchar_t cr);

const std::set<std::string> Keywords = {
        "func",
        "return",
        "void",
        "extern",
        "if",
        "else",
};
enum Token {
    token_var = 1,
    token_str = 2,
    token_func = 3,
    token_return = 4,
    token_extern = 5,
    token_if = 6,
    token_else = 7,
    token_eof = 8,
    token_double = 9,
    token_int = 10,
    token_sign = 11,
    token_string = 12,
    token_for = 13,
    token_break = 14,
    token_continue = 15,
    token_import = 16,
};

#include <iostream>

static bool isAllowed(std::string str) {
    static std::set<std::string> t = {"." ,"{", "}", "(", ")", "+", "-", "*", "/", "%", ":", ";", "=", "==", "!=", "[", "]"};
    return t.find(str) != t.end();
}

class RToken {
public:
    Token type;
    std::string strData;
    int64 int64Data;
    double doubleData;

    std::string GetSign();

    std::string GetStr();

    int64 GetInt64();

    bool IsSign(std::string sign);

    bool IsSign(char sign);

    RToken(Token type, int64 i64Data) : type(type), int64Data(i64Data) {}

    RToken(Token type) : type(type) {}

    RToken(Token type, double doubleData) : type(type), doubleData(doubleData) {

    }

    RToken(Token type, std::string strData) : type(type), strData(strData) {}


};

class RxReader {
private:
    std::wifstream *fstream;
    std::string str; //获取的str
    wchar_t lastChar = ' ';
public:
    RxReader(std::wifstream *instream) {
        fstream = instream;
    }

    std::shared_ptr<RToken> ReadAToken();
};

static bool isspace(wchar_t cr) {
    return iswspace(cr);
}