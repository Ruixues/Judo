#pragma once
// 类型系统
// 内置类型 (后期实现类之后使用类替代)

#define int32 int32_t
#define int64 int64_t
enum BuildinType
{
    Type_customType = 0, //用户自定义类型，比如类等
    Type_int32 = 1,
    Type_double = 2,
};
class JotoType
{
private:
    BuildinType t;

public:
    // 暂时没有加入自定义类型
    JotoType(BuildinType Type) : t(Type) {}
};