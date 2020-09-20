#include "core.h"

void Module::Parse() {
    // 开始解析
    auto token = reader->ReadAToken();
    while (1) {
        if (token->type == token_eof) {
            std::cout << "Eof" << std::endl;
            break;
        }
        switch (token->type) {
            case token_func:
                Parser::ParseFunction(this);
            case token_str:
                Parser::ParseIdentifierExpr(this);
        }
        exit (0);
    }
}