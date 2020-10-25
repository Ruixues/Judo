#pragma once

#include "add.h"

class Module;

void BindAllBuiltIn(Module *module) {
    bindAdd(module);
}