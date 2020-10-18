#include "reader.h"
#include <iostream>


static wchar_t ReadChar(std::wifstream *file) {
    wchar_t ret;
    *file >> ret;
    if (file->eof()) {
        return WEOF;
    }
    return ret;
}

std::shared_ptr<RToken> RxReader::ReadAToken() {
    if (lastChar == WEOF) {
        return std::make_shared<RToken>(token_eof);
    }
    std::string str;
    // 跳跃空格
    while (isspace(lastChar) && !fstream->eof()) {
        lastChar = ReadChar(fstream);
    }
    if (iswalpha(lastChar)) //判断是否是英文字母
    {                      // identifier: [a-zA-Z][a-zA-Z0-9]*
        str = lastChar;
        lastChar = ReadChar(fstream);
        while (iswalnum(lastChar)) {
            str += lastChar;
            lastChar = ReadChar(fstream);
        }
        if (str == "func") { //定义函数
            return std::make_shared<RToken>(token_func);
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
        if (str == "for") {
            return std::make_shared<RToken>(token_for);
        }
        if (str == "break") {
            return std::make_shared<RToken>(token_break);
        }
        if (str == "continue") {
            return std::make_shared<RToken>(token_continue);
        }
        return std::make_shared<RToken>(token_str, str);
    }
    /*
        TODO : 实现输入字符串
    */
    if (lastChar == L'\"') { //是字符串
        //开始读取字符串
        bool turn = false;  //下一个字符是否转义
        std::string ret = "";
        bool normalReturn = false;
        while (iswdigit(lastChar)) {
            lastChar = ReadChar(fstream);
            if (turn) {
                ret += lastChar;
                turn = false;
            } else if (lastChar == '\\') {  //转义
                turn = true;
            } else if (lastChar == '"') {
                //字符串结束
                normalReturn = true;
                break;
            }
        }
        if (!normalReturn) {
            return nullptr;
        }
        return std::make_shared<RToken>(token_string, ret);

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
    while (isAllowed(ret)) {
        lastChar = ReadChar(fstream);
        auto tmp = ret;
        tmp += lastChar;
        if (isAllowed(tmp)) {
            ret += lastChar;
        } else {
            break ;
        }
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
