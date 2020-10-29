#pragma once

#include "../type.h"

class Module;
namespace Parser {
    std::unique_ptr<JudoType> ParseType(Module *module);
}