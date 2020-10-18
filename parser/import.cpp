#include "import.h"
#include "../core.h"
#include "../ast/import.h"

namespace Parser {
    std::unique_ptr<AST::ExprAST> ParseImport(Module *module) {
        auto token = module->ReadAToken();  //吃掉import
        //import必须以一个括号开始
        if (!token->IsSign("(")) {
            return module->loger->ParseError("Import", "expect ( after import");
        }
        //开始解析
        module->ReadAToken();
        std::vector<std::string> modules, anotherNames;
        while (!module->nowToken->IsSign(")")) {
            auto moduleName = module->nowToken;
            if (moduleName->type != token_string) {
                module->loger->ParseError("Import", "expect module name in import");
            }
            auto anotherName = module->ReadAToken();
            if (anotherName->type != token_str) {   //没有别名
                modules.push_back(moduleName->GetStr());
                anotherNames.emplace_back();
                continue;
            }
            module->ReadAToken();   //吃掉名字
            modules.push_back(moduleName->GetStr());
            anotherNames.push_back(anotherName->GetStr());
        }
        return make_AST<AST::ImportAST>(module, modules, anotherNames);
    }
}