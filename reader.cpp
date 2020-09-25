#include "reader.h"
#include <iostream>


static wchar_t ReadChar(std::wifstream *file) {
    wchar_t ret;
    if (file->eof()) {
        return WEOF;
    }
    *file >> ret;
    return ret;
}

std::shared_ptr<RToken> RxReader::ReadAToken() {
//    std::string ttt;
//    for (auto t = ReadChar(fstream);t != WEOF;t = ReadChar(fstream)) {
//        ttt += t;
//    }
//    std::cout << ttt << std::endl;
//    exit (0);
    if (lastChar == WEOF) {
        return std::make_shared<RToken>(token_eof);
    }
    std::string str;
    // 跳跃空格
    while (isspace(lastChar) && !fstream->eof()) {
        lastChar = ReadChar(fstream);
    }
    if (isalpha(lastChar)) //判断是否是英文字母
    {                      // identifier: [a-zA-Z][a-zA-Z0-9]*
        str = lastChar;
        lastChar = ReadChar(fstream);
        while (iswalnum(lastChar)) {
            str += lastChar;
            lastChar = ReadChar(fstream);
        }
        if (str == "func") { //定义函数
            auto ret = std::make_shared<RToken>(token_func);
            return ret;
        }
        if (str == "extern") //外部函数
            return std::make_shared<RToken>(token_extern);
        if (str == "var") {
            return std::make_shared<RToken>(token_var);
        }
        if (str == "if") {
            return std::make_shared<RToken>(token_if);
        }
        if (str == "else") {
            return std::make_shared<RToken>(token_else);
        }
        return std::make_shared<RToken>(token_str, str);
    }
    /*
        TODO : 实现输入字符串
    */
    if (lastChar == '\"') { //是字符串
    }
    if (iswdigit(lastChar) || lastChar == '.') //如果是数字
    {                                          // Number: [0-9.]+
        std::string NumStr;
        bool tdouble = false;
        do {
            if (lastChar == '.') {
                tdouble = true;
            }
            NumStr += lastChar;
            lastChar = ReadChar(fstream);
        } while (iswdigit(lastChar) || lastChar == '.');
        if (tdouble) {
            return std::make_shared<RToken>(token_double, atof(NumStr.c_str()));
        }
        return std::make_shared<RToken>(token_int, int64(atoll(NumStr.c_str())));
    }
    if (lastChar == WEOF)
        return std::make_shared<RToken>(token_eof);
    // 说明啥都不是，那就连续读，一直到EOF或者空格
    std::string ret;
    ret += lastChar;
    if (!isMustSingle(ret)) {
        while (lastChar != WEOF && lastChar != ' ' && !iswdigit(lastChar)) {
            ret += lastChar;
            lastChar = ReadChar(fstream);
        }
    } else {
        lastChar = ReadChar(fstream);
    }
    return std::make_shared<RToken>(token_sign, ret);
}

std::string RToken::GetSign() {
    return this->strData;
}

std::string RToken::GetStr() {
    return this->strData;
}

int64 RToken::GetInt64() {
    return this->int64Data;
}

bool RToken::IsSign(std::string sign) {
    if (this->type != token_sign) {
        return false;
    }
    return GetSign() == sign;
}
