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
enum Token
{
    token_codeBlock = 1,
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
};
 static bool isMustSingle (std::string str) {
    static std::set<std::string> t = {"{","}","(",")","+","-","*","/","%"};
    return t.find(str) != t.end();
}
class RToken
{
public:
    Token type;
    std::unique_ptr<void *> data; //真正的数据
    std::string GetSign ();
    std::string GetStr ();
    int64 GetInt64 ();
    bool IsSign (std::string sign);
    RToken(Token type, std::unique_ptr<void *> data) : type(type), data(std::move(data)) {}
};
class RxReader
{
private:
    std::wifstream* fstream;
    std::string str; //获取的str
    wchar_t lastChar = ' ';
public:
    RxReader(std::wifstream* instream) {
        fstream = instream;
    }
    std::shared_ptr<RToken> ReadAToken();
};
static bool isspace(wchar_t cr)
{
    return cr == ' ';
}