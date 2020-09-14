/**
 * 本文件用来进行token的读取
 * 
 **/
#pragma
#include <set>
#include <string>
#include <sstream>
#include <memory>
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
};
class RxReader
{
private:
    std::unique_ptr<std::istringstream> instream;
    std::string str; //获取的str
    wchar_t lastChar = ' ';
public:
    RxReader(std::unique_ptr<std::istringstream> instream) : instream(std::move(instream)) {}
    Token ReadAToken();
    std::string GetStr();
};

Token RxReader::ReadAToken()
{
    str = "";
    // 跳跃空格
    while (isspace(lastChar))
        (*instream) >> lastChar;
    if (isalpha(LastChar)) //判断是否是英文字母
    {                      // identifier: [a-zA-Z][a-zA-Z0-9]*
        str = LastChar;
        while (isalnum((LastChar = rgetchar())))
            IdentifierStr += LastChar;
        if (IdentifierStr == "func") //定义函数
            return tok_func;
        if (IdentifierStr == "extern") //外部函数
            return tok_extern;
        return tok_identifier;
    }
    if (isdigit(LastChar) || LastChar == '.') //如果是数字
    {                                         // Number: [0-9.]+
        std::string NumStr;
        do
        {
            NumStr += LastChar;
            LastChar = rgetchar();
        } while (isdigit(LastChar) || LastChar == '.');
        NumVal = strtod(NumStr.c_str(), 0);
        return tok_number;
    }
    if (LastChar == EOF)
        return tok_eof;

    // Otherwise, just return the character as its ascii value.
    int ThisChar = LastChar;
    LastChar = rgetchar();
    return ThisChar;
}
bool isspace (wchar_t cr) {
    return cr == ' ';
}