#pragma once
// 类型系统
// 内置类型 (后期实现类之后使用类替代)
#include <string>
#include "llvmInclude.h"
#include <unordered_map>
#define int32 int32_t
#define int64 int64_t
enum BuiltinType {
    Type_int32 = 1,
    Type_double = 2,
    Type_void = 3,
    Type_Undefined = 4,
    Type_int64 = 5,
};
namespace AST {
    class ClassItem;
}
class JudoClassMateData {   //Core Data of judo class
public:
    ~JudoClassMateData();
    std::vector<std::unique_ptr<AST::ClassItem>> privates, publics;
};
class Module;
class JudoTypeRef { //对类型的指定
private:
    std::string type;
    Module* module;
    llvm::Type* pack = nullptr;
public:
    JudoTypeRef(Module* module,std::string type):type(type),module (module) {}
    JudoTypeRef(Module* module,BuiltinType type);
    JudoTypeRef(Module* module,llvm::Type* type):pack (type) {}
    llvm::Type* ToType ();
};
namespace AST {
    class ClassAST;
}
class JudoTypeSystem {
private:
    Module *module;
    std::unordered_map<std::string, llvm::Type *> rlink;
    std::unordered_map<std::string, std::unique_ptr<JudoClassMateData>> rclass;
    llvm::Type *getBuiltInType(std::string TypeName);

public:
    JudoTypeSystem(Module *module);

    bool BindType(std::string name, llvm::Type *type);

    llvm::Type *GetBuiltInType(BuiltinType type);

    llvm::Type *GetType(std::string name);
    JudoClassMateData* getClass (std::string name);
    bool BindClass (std::string name,std::unique_ptr<JudoClassMateData> data);
};