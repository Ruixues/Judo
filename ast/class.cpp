#include "class.h"
#include "../core.h"

namespace AST {
    llvm::Value *ClassAST::genCode() {
        if (module->module->getTypeByName(name)) {
            return module->loger->GenCodeError("can't redefine the class:" + name);
        }
        auto type = llvm::StructType::create(module->core->context, name);
        std::vector<llvm::Type *> elements;
        for (auto &v:publics) {
            if (v->type != Variable) continue;
            elements.push_back(v->variable->type->ToType());
        }
        type->setBody(elements);
        if (!module->Type.BindType(name, type) || !module->Type.BindClass(name, this)) {
            return module->loger->GenCodeError("Can't bind class:" + name + " to Judo System");
        }
        return llvm::ConstantInt::getTrue(module->core->context);
    }
}
