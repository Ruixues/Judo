#include "reader.h"
#include <iostream>
std::shared_ptr<RToken> RxReader::ReadAToken()
{
    std::string str;
    // 跳跃空格
    while (isspace(lastChar)) {
        *fstream >> lastChar;
    }
    std::cout << "Here" << std::endl;
    if (isalpha(lastChar)) //判断是否是英文字母
    {                      // identifier: [a-zA-Z][a-zA-Z0-9]*
        str = lastChar;
        *fstream >> lastChar;
        while (iswalnum(lastChar)) {
            str += lastChar;
            *fstream >> lastChar;
        }
        if (str == "func") //定义函数
            return std::make_shared<RToken>(token_func, nullptr);
        if (str == "extern") //外部函数
            return std::make_shared<RToken>(token_extern, nullptr);
        return std::make_shared<RToken>(token_str, std::make_unique<void*>(&str));
    }
    std::cout << "No" << std::endl;
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
            return std::make_shared<RToken>(token_double, std::make_unique<void*>(new double(atof(NumStr.c_str()))));
        }
        return std::make_shared<RToken>(token_int, std::make_unique<void*>(new int64(atoll(NumStr.c_str()))));
    }
    if (lastChar == EOF)
        return std::make_shared<RToken>(token_eof, nullptr);
    // 说明啥都不是，那就连续读，一直到EOF或者空格
    std::string ret;
    ret += lastChar;
    if (!isMustSingle(ret)) {
        while (lastChar != EOF && lastChar != ' ' && !iswdigit(lastChar)) {
            ret += lastChar;
            *fstream >> lastChar;
        }
    }
    return std::make_shared<RToken>(token_sign,std::make_unique<void*>(&ret));
}
std::string RToken::GetSign () {
    return *(std::string*)(*(this->data));
}
std::string RToken::GetStr () {
    return *(std::string*)(*(this->data));
}
int64 RToken::GetInt64() {
    return *(int64*)(*(this->data));
}
bool RToken::IsSign (std::string sign) {
    if (this->type != token_sign) {
        return false;
    }
    return *(std::string*)(*(this->data)) == sign;
}