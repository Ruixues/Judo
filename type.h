#pragma once
// 类型系统
// 内置类型 (后期实现类之后使用类替代)
#include <string>
#include "llvmInclude.h"

#define int32 int32_t
#define int64 int64_t
enum BuildinType {
    Type_customType = 0, //用户自定义类型，比如类等
    Type_int32 = 1,
    Type_double = 2,
    Type_void = 3,
    Type_Undefined = 4,
    Type_int64 = 5,
};

class JudoType {
private:
    BuildinType t = Type_Undefined;
    std::string CustomTypeName;
public:
    // 暂时没有加入自定义类型
    JudoType(BuildinType Type) : t(Type) {}

    bool isType(BuildinType Type) {
        return t == Type;
    }

    JudoType(std::string TypeName) {
        if (TypeName == "int32") {
            t = Type_int32;
        } else if (TypeName == "double") {
            t = Type_double;
        } else if (TypeName == "void") {
            t = Type_void;
        } else if (TypeName == "int64") {
            t = Type_int64;
        } else {
            CustomTypeName = TypeName;
        }
    }

    llvm::Type *getType(llvm::LLVMContext &context) {
        switch (t) {
            case Type_int32:
                return llvm::Type::getInt32Ty(context);
            case Type_void:
                return llvm::Type::getVoidTy(context);
            case Type_int64:
                return llvm::Type::getInt64Ty(context);
        }
        return llvm::Type::getFloatTy(context);
    }
};