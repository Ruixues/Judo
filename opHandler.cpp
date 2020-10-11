#include "opHandler.h"
#include "core.h"

llvm::Value *OpHandler::getBinaryOp(std::string opt, llvm::Value *a, llvm::Value *b) {
    if (opLink.find(typeToStr(a->getType())) == opLink.end()) {
        return module->loger->GetFunctionError("Undefined opt of " + opt);
    }
    if (opLink[typeToStr(a->getType())].find(typeToStr(b->getType())) == opLink[typeToStr(a->getType())].end()) {
        return module->loger->GetFunctionError("Undefined opt of " + opt);
    }
    if (opLink[typeToStr(a->getType())][typeToStr(b->getType())].find(opt) ==
        opLink[typeToStr(a->getType())][typeToStr(b->getType())].end()) {
        return module->loger->GetFunctionError("Undefined opt of " + opt);
    }
    return opLink[typeToStr(a->getType())][typeToStr(b->getType())][opt] (module,a,b);
}

bool OpHandler::linkOp(const std::string &a, const std::string &b, const std::string &opt, GetOptFunc function) {
    opLink[a][b][opt] = function;
    return true;
}