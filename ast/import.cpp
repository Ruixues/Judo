#include "import.h"
#include "../core.h"

namespace AST {
    ImportAST::ImportAST(std::vector<std::string> _TargetModule, std::vector<std::string> _AnotherName) : TargetModule(
            TargetModule), AnotherName(AnotherName) {
        for (size_t i = 0; i < _TargetModule.size(); ++i) {
            auto v = _TargetModule[i];
            auto _route = ParseRoute(v);
            auto another = _AnotherName[i];
            if (another.empty()) {
                another = *(_route.end());
            }
            TargetModule.push_back(_route);
            AnotherName.push_back(another);
        }
    }
    llvm::Value * ImportAST::genCode() {
        return nullptr;
    }
}