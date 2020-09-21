#include "core.h"



void Module::Parse() {
    // 开始解析
    auto token = reader->ReadAToken();
    std::vector<std::unique_ptr<AST::ExprAST>> codes;
    while (1) {
        if (token->type == token_eof) {
            std::cout << "Eof" << std::endl;
            break;
        }
        auto tmp = HandleToken(token);
        if (!tmp) {
            return;
        }
        codes.push_back(std::move (tmp));
    }
}
std::unique_ptr<AST::ExprAST> Module::HandleToken (std::shared_ptr<RToken> token) {
    switch (token->type) {
        case token_func:
            return Parser::ParseFunction(this);
        case token_str:
            return Parser::ParseIdentifierExpr(this);
        case token_sign:
            if (token->IsSign("{")) {   //代码块
                return Parser::ParseCodeBlock(this);
            }
    }
    return nullptr;
}