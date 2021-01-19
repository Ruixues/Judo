include scripts/top.mk
include scripts/include/llvm.mk
all:build

MKELF := g++
elf-always-y += Judo
Judo-obj-y += built-in.o 
Judo-direct-y += $(llvm-libs)

include-y += $(llvm-include)

cxxflags-y := -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -flto -std=gnu++17
export cxxflags-y

obj-y += ast/
obj-y += builtIn/
obj-y += other/
obj-y += parser/
obj-y += core.o env.o main.o opHandler.o reader.o type.o
