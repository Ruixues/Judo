#include "codeBlock.h"
#include "../ast/codeBlock.h"
#include "../core.h"
#include <vector>
#include "function.h"
namespace Parser {
    std::unique_ptr<AST::ExprAST> ParseCodeBlock (Module* module) {
        if (module->nowToken->type != token_codeBlock) {
            return module->loger->ParseError("Code Block","unexpected call to ParseCodeBlock.");
        }
        //开始解析
        module->ReadAToken();
        std::vector<std::unique_ptr<AST::ExprAST>> codes;
        while (!module->nowToken->IsSign("}") && module->nowToken) {
            auto tmp = ParseExpression (module);
            if (!tmp) {
                return nullptr;
            }
            codes.push_back(std::move (tmp));
        }
        //那就表示完成了
        module->ReadAToken();   //吃掉}
        return std::make_unique<AST::CodeBlockAST>(std::move (codes));
    }
}