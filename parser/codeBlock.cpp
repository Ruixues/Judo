#include "codeBlock.h"
#include "../ast/codeBlock.h"
#include "../core.h"
#include <vector>
#include "function.h"
#include "../other/defer.h"

namespace Parser {
    std::unique_ptr<AST::ExprAST> ParseCodeBlock(Module *module) {
        if (!module->nowToken->IsSign("{")) {
            return module->loger->ParseError("Code Block", "unexpected call to ParseCodeBlock.");
        }
        //开始解析
        module->ReadAToken();   //吃掉{
        std::vector<std::unique_ptr<AST::ExprAST>> codes;
        while (module->nowToken && !module->nowToken->IsSign("}")) {
            auto tmp = ParseExpression(module);
            if (!tmp) {
                return nullptr;
            }
            codes.push_back(std::move(tmp));
        }
        if (module->nowToken && module->nowToken->IsSign("}")) {
            module->ReadAToken();   //吃掉}
        }
        return make_AST<AST::CodeBlockAST>(module, std::move(codes));
    }
}