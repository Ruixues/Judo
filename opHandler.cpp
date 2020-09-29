#include "opHandler.h"
#include "core.h"

llvm::Function *OpHandler::getBinaryOp(std::string opt, llvm::Value *a, llvm::Value *b) {
    if (opLink.find(a->getType()->getTypeID()) == opLink.end()) {
        return module->loger->GetFunctionError("Undefined opt of " + opt);
    }
    if (opLink[a->getType()->getTypeID()].find(b->getType()->getTypeID()) == opLink[a->getType()->getTypeID()].end()) {
        return module->loger->GetFunctionError("Undefined opt of " + opt);
    }
    if (opLink[a->getType()->getTypeID()][b->getType()->getTypeID()].find(opt) ==
        opLink[a->getType()->getTypeID()][b->getType()->getTypeID()].end()) {
        return module->loger->GetFunctionError("Undefined opt of " + opt);
    }
    return opLink[a->getType()->getTypeID()][b->getType()->getTypeID()][opt];
}

bool OpHandler::linkOp(llvm::Type::TypeID a, llvm::Type::TypeID b, std::string opt, llvm::Function *function) {
    opLink[a][b][opt] = function;
    return true;
}