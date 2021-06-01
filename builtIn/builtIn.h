#pragma once

#include "add.h"

class Module;

inline void BindAllBuiltIn(Module *module) {
    bindAdd(module);
}