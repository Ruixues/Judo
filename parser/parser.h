#pragma once

#include "function.h"
#include "Paren.h"
#include "const/int32.h"
#include "codeBlock.h"
#include "variableDefine.h"
#include "const/number.h"
#include "if.h"
#include <memory>
class Module;
template<typename T, typename... _Args>
std::shared_ptr<T> make_AST (std::shared_ptr<Module> module,_Args&&... __args) {
    auto ret = std::make_shared<T>(__args);
    ret->SetModule (module);
    return ret;
}