#include "add.h"
#include "../core.h"

void bindAdd(Module *module) {
    const std::string as[] = {"Int32", "Double", "Double", "Int32"};
    const std::string bs[] = {"Int32", "Double", "Int32", "Double"};
    size_t len = sizeof(as) / sizeof(as[1]);
    for (size_t i = 0; i < len; ++i) {
        module->opHandler->linkOp(as[i], bs[i], "+", module->module->getFunction("add" + as[i] + bs[i]));
    }
    return;
}
#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

extern "C" DLLEXPORT double addDoubleDouble(double a, double b) {
    return a + b;
}
extern "C" DLLEXPORT double addInt32Int32(int32 a, int32 b) {
    return a + b;
}
extern "C" DLLEXPORT double addDoubleInt32(double a, int32 b) {
    return a + b;
}
extern "C" DLLEXPORT double addInt32Double(int32 a, double b) {
    return a + b;
}