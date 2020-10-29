#pragma once
// 类型系统
// 内置类型 (后期实现类之后使用类替代)
#include <string>
#include "llvmInclude.h"

#define int32 int32_t
#define int64 int64_t
enum BuiltinType {
    Type_customType = 0, //用户自定义类型，比如类等
    Type_int32 = 1,
    Type_double = 2,
    Type_void = 3,
    Type_Undefined = 4,
    Type_int64 = 5,
};

class Module;

class JudoTypeSystem {
private:
    Module *module;
    std::map<std::string, llvm::Type *> link;
private:
    llvm::Type *getBuiltInType(std::string TypeName);

public:
    JudoTypeSystem(Module *module);

    bool BindType(std::string name, llvm::Type *type);
    llvm::Type* GetBuiltInType (BuiltinType type);
    llvm::Type *GetType(std::string name);
};