#include "type.h"
#include "core.h"

JudoTypeSystem::JudoTypeSystem(Module *module) : module(module) {

}

bool JudoTypeSystem::BindType(std::string name, llvm::Type *type) {
    if (link.find(name) != link.end()) {
        return false;
    }
    link[name] = type;
}
llvm::Type* JudoTypeSystem::GetBuiltInType (BuiltinType t) {
    auto &context = module->core->context;
    switch (t) {
        case Type_int32:
            return llvm::Type::getInt32Ty(context);
        case Type_void:
            return llvm::Type::getVoidTy(context);
        case Type_int64:
            return llvm::Type::getInt64Ty(context);
    }
    return nullptr;
}
llvm::Type *JudoTypeSystem::getBuiltInType(std::string TypeName) {
    BuiltinType t = Type_Undefined;
    auto &context = module->core->context;
    if (TypeName == "int32") {
        t = Type_int32;
    } else if (TypeName == "double") {
        t = Type_double;
    } else if (TypeName == "void") {
        t = Type_void;
    } else if (TypeName == "int64") {
        t = Type_int64;
    }
    return GetBuiltInType(t);
}

llvm::Type *JudoTypeSystem::GetType(std::string name) {
    if (!link[name]) {
        return getBuiltInType(name);
    }
    return link[name];
}