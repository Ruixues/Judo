/**
 * 本文件用来进行token的读取
 * 
 **/
#pragma once
#include <set>
#include <string>
#include <fstream>
#include <memory>
#include "type.h"
static std::set<std::string> Keywords = {
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
class RToken
{
public:
    Token type;
    std::unique_ptr<void *> data; //真正的数据
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
bool isspace(wchar_t cr);
std::shared_ptr<RToken> RxReader::ReadAToken()
{
    std::string str;
    // 跳跃空格
    while (isspace(lastChar))
        *fstream >> lastChar;
    if (isalpha(lastChar)) //判断是否是英文字母
    {                      // identifier: [a-zA-Z][a-zA-Z0-9]*
        str = lastChar;
        *fstream >> lastChar;
        while (iswalnum(lastChar))
            str += lastChar;
        if (str == "func") //定义函数
            return std::make_shared<RToken>(token_func, nullptr);
        if (str == "extern") //外部函数
            return std::make_shared<RToken>(token_extern, nullptr);
        return std::make_shared<RToken>(token_str, std::make_unique<std::string>(str));
    }
    /*
        TODO : 实现输入字符串
    */
    if (lastChar == '\"')
    { //是字符串
    }
    if (iswdigit(lastChar) || lastChar == '.') //如果是数字
    {                                          // Number: [0-9.]+
        std::string NumStr;
        bool tdouble = false;
        do
        {
            if (lastChar == '.')
            {
                tdouble = true;
            }
            NumStr += lastChar;
            *fstream >> lastChar;
        } while (iswdigit(lastChar) || lastChar == '.');
        if (tdouble)
        {
            return std::make_shared<RToken>(token_double, std::make_unique<double*>(new double(atof(NumStr.c_str()))));
        }
        return std::make_shared<RToken>(token_int, std::make_unique<int64*>(new int64(atoll(NumStr.c_str()))));
    }
    if (lastChar == EOF)
        return std::make_shared<RToken>(token_eof, nullptr);
    // 说明啥都不是，那就连续读，一直到EOF或者空格
    std::string ret;
    while (lastChar != EOF && lastChar != ' ' && !iswdigit(lastChar)) {
        ret += lastChar;
        *fstream >> lastChar;
    }
    return std::make_shared<RToken>(token_sign,std::make_unique<std::string*>(&ret));
}
bool isspace(wchar_t cr)
{
    return cr == ' ';
}