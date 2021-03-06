#include "type.h"
#include "core.h"
#include "ast/class.h"

JudoTypeSystem::JudoTypeSystem(Module *module) : module(module) {

}

JudoClassMateData::~JudoClassMateData() = default;

JudoTypeRef::JudoTypeRef(Module *module, BuiltinType type) {
    pack = module->Type.GetBuiltInType(type);
}

bool JudoTypeSystem::BindType(std::string name, llvm::Type *type) {
    if (rlink.find(name) != rlink.end()) {
        return false;
    }
    rlink.insert(std::make_pair(name, type));
    return true;
}

llvm::Type *JudoTypeSystem::GetBuiltInType(BuiltinType t) {
    auto &context = module->core->context;
    switch (t) {
        case Type_int32:
            return llvm::Type::getInt32Ty(context);
        case Type_void:
            return llvm::Type::getVoidTy(context);
        case Type_int64: {
            return llvm::Type::getInt64Ty(context);
        }
        case Type_Undefined: {
            return nullptr;
        }
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
    if (rlink.find(name) == rlink.end()) {
        return getBuiltInType(name);
    }
    return rlink[name];
}

llvm::Type *JudoTypeRef::ToType() {
    if (pack) return pack;
    return module->Type.GetType(type);
}

bool JudoTypeSystem::BindClass(std::string name, std::unique_ptr<JudoClassMateData> data) {
    if (rclass.find(name) != rclass.end()) {
        return false;
    }
    rclass[name] = std::move(data);
    return true;
}

JudoClassMateData *JudoTypeSystem::getClass(std::string name) {
    auto v = rclass.find(name);
    if (v == rclass.end()) return nullptr;
    return v->second.get();
}