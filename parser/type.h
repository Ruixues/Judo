#pragma once

#include "../type.h"

class Module;
namespace Parser {
    std::unique_ptr<JudoTypeRef> ParseType(Module *module);
}