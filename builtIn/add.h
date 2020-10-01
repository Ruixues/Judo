#pragma once

#include "../type.h"
#include "../llvmInclude.h"
#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif
class Module;
void bindAdd (Module* module);
