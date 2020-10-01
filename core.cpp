#include "core.h"
#include "parser/parser.h"

void Module::Parse() {
    // 开始解析
    auto token = reader->ReadAToken();
    std::vector<std::unique_ptr<AST::ExprAST>> codes;
    while (1) {
        if (token->type == token_eof) {
            break;
        }
        auto tmp = HandleToken(token);
        if (!tmp) {
            return;
        }
        codes.push_back(std::move(tmp));
        token = reader->ReadAToken();
    }
}

Module::Module(std::string file) : Builder(context) {
    this->file.open(file, std::ios::in);
    if (!this->file.good()) {
        std::cout << "Open Module " << file << " Error" << std::endl;
        return;
    }
    this->file >> std::noskipws;
    reader = std::make_unique<RxReader>(&(this->file));
    loger = std::make_unique<Log>();
    opHandler = std::make_unique<OpHandler>(this);
}

std::unique_ptr<AST::ExprAST> Module::HandleToken(std::shared_ptr<RToken> token) {
    switch (token->type) {
        case token_func:
            return Parser::ParseFunction(this);
        case token_str:
            return Parser::ParseIdentifierExpr(this);
        case token_sign:
            if (token->IsSign("{")) {   //代码块
                return Parser::ParseCodeBlock(this);
            }
        case token_var:
            return Parser::ParserVariableDefine(this);
        default:
            return loger->ParseError("Core", "unexpected token type");
    }
    return nullptr;
}