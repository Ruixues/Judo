#pragma once

#include "../type.h"

class Module;
namespace Parser {
    llvm::Type* ParseType(Module *module);
}