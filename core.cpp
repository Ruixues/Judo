#include "core.h"

void Module::Parse() {
    // 开始解析
    auto token = reader->ReadAToken();
    std::cout << token->type << std::endl;
    while (1) {
        if (token->type == token_eof) {
            break;
        }
        switch (token->type) {
            case token_func:
                Parser::ParseFunction(this);
            case token_str:
                Parser::ParseIdentifierExpr(this);
        }
    }
}