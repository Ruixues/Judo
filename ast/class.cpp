#include "class.h"
#include "../core.h"

namespace AST {
    llvm::Value *ClassAST::genCode() {
        if (module->module->getTypeByName(name)) {
            return module->loger->GenCodeError("can't redefine the struct:" + name);
        }
        auto type = llvm::StructType::create(module->core->context, name);
        std::vector<llvm::Type *> elements;
        for (auto &v:publics) {
            if (v->type != Variable) continue;
            elements.push_back((llvm::Type *) (v->variable->type));
        }
        type->setBody(elements);
        if (!module->Type.BindType(name, type)) {
            return module->loger->GenCodeError("Can't bind class:" + name + " to Judo System");
        }
        return llvm::ConstantInt::getTrue(module->core->context);
    }
}
