set(CMAKE_CXX_STANDARD 14)
cmake_minimum_required(VERSION 3.1)

# 项目信息

SET(CMAKE_C_COMPILER clang)
SET(CMAKE_CXX_COMPILER clang++)
option(LLVM_ENABLE_LLD ON)   #使用LLD作为链接器
#SET(CMAKE_C_COMPILER gcc)
#SET(CMAKE_CXX_COMPILER g++)
project(Judo)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(LLVM REQUIRED CONFIG)
message(STATUS "include path: ${LLVM_INCLUDE_DIRS}")
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

IF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "current platform: Windows")
    add_definitions(-D _WIN32)
    set(CMAKE_CXXFLAGS "${CMAKE_CXX_FLAGS} -rdynamic -Xlinker --export-dynamic")
ENDIF ()
if (CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-flto)
endif (CMAKE_COMPILER_IS_GNUCXX)
# 指定生成目标
aux_source_directory(. files)
#Dont need orcjit
llvm_map_components_to_libnames(llvm_libs support core irreader native orcjit)
add_executable(main ${files} jit/jit.h opHandler.h llvmInclude.h loger.h parser/parser.h parser/Paren.h other/objectPool.h other/defer.h thirdparty/argparse/argparse.hpp args.h config.h parser/function.cpp reader.cpp ast/binaryOpt.h ast/variable.h ast/FunctionCall.h parser/const/int32.h parser/paren.cpp parser/const/int32.cpp core.cpp parser/codeBlock.h parser/codeBlock.cpp parser/variableDefine.h parser/variable.cpp parser/const/number.h parser/const/number.cpp parser/if.h parser/if.cpp ast/if.h opHandler.cpp ast/function.cpp ast/if.cpp ast/const/int32.cpp ast/FunctionCall.cpp ast/codeBlock.cpp ast/binaryOpt.cpp ast/ast.cpp ast/const/double.cpp builtIn/add.cpp builtIn/add.h builtIn/builtIn.h parser/extern.h parser/extern.cpp ast/extern.h ast/variable.cpp ast/for.h ast/for.cpp parser/for.h parser/for.cpp parser/break.h ast/break.h ast/break.cpp parser/break.cpp env.h env.cpp parser/import.h parser/import.cpp ast/import.h other/split.h ast/import.cpp ast/return.h other/split.cpp ast/return.cpp parser/return.h parser/return.cpp)
target_link_libraries(main ${llvm_libs})
