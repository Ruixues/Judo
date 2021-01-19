# SPDX-License-Identifier: GPL-2.0
# ==========================================================================
# Build sub system
# ==========================================================================
src := $(obj)

PHONY := _build
_build:

#
# Include Buildsystem function
include $(BUILD_HOME)/include/define.mk

#
# Read auto.conf if it exists, otherwise ignore
-include $(MAKE_HOME)/include/config/auto.conf

#
# Include obj makefile
build-dir := $(if $(filter /%,$(src)),$(src),$(MAKE_HOME)/$(src))
build-file := $(if $(wildcard $(build-dir)/Kbuild),$(build-dir)/Kbuild,$(build-dir)/Makefile)
include $(build-file)

########################################
# Start include                        #
########################################

include-file	:= $(include-y)
include-file	:= $(strip $(sort $(include-file)))
include-file	:= $(filter %/, $(include-file))
include-file	:= $(patsubst %/,%,$(include-file))
include-file	:= $(addprefix $(obj)/,$(include-file))

INCLUDE			:= $(include-file) $(include-direct-y) $(INCLUDE)
export INCLUDE

########################################
# Old compatible                       #
########################################

#
# always -> always-y
ifneq ($(always),)
$(warning 'always' is deprecated. Please use 'always-y' instead)
always-y += $(always)
endif

#
# elf-y -> elf
ifneq ($(elf-y),)
$(warning 'elf-y' is deprecated. Please use 'elf' instead)
elf  += $(elf-y)
endif

#
# hostprog-y -> hostprog
ifneq ($(hostprogs-y),)
$(warning 'hostprogs-y' is deprecated. Please use 'hostprogs' instead)
hostprogs  += $(hostprogs-y)
endif

########################################
# Always build                         #
########################################

# nasm-always-y += foo
# ... is a shorthand for
# nasm += foo
nasm 		+= $(nasm-always-y)

# cust-always-y += foo
# ... is a shorthand for
# cust += foo
cust 		+= $(cust-always-y)

# hostprogs-always-y += foo
# ... is a shorthand for
# hostprogs += foo
hostprogs 	+= $(hostprogs-always-y)

########################################
# basic rule                           #
########################################

ifneq ($(obj-y),)
rules += rule_main
endif

#
# Independent rules: assembly to bin
rule_main:
	$(Q)$(MAKE) $(build_main)=$(obj)

########################################
# lib rule                             #
########################################

ifneq ($(lib-y),)
rules += rule_lib
endif

#
# Independent rules: assembly to bin
rule_lib:
	$(Q)$(MAKE) $(build_lib)=$(obj)

########################################
# Nasm Module                          #
########################################

ifneq ($(nasm),)
rules += rule_nasm
endif

#
# Independent rules: assembly to bin
rule_nasm:
	$(Q)$(MAKE) $(build_nasm)=$(obj)

########################################
# Cust Module                          #
########################################

ifneq ($(cust),)
rules += rule_cust
endif

#
# Independent rules: cust rules to elf
rule_cust:
	$(Q)$(MAKE) $(build_cust)=$(obj)

########################################
# Host Module                          #
########################################

ifneq ($(hostprogs),)
rules += rule_host
endif

#
# Independent rules: hostprogs ruled to rules
rule_host:
	$(Q)$(MAKE) $(build_host)=$(obj)

########################################
# Start build                          #
########################################

ifeq ($(rules),)
rules += rule_main
endif

_build: $(rules) 

########################################
# Descending build                     #
########################################

PHONY += $(subdir-y)
$(subdir-y):
	$(Q)$(MAKE) $(build)=$@

########################################
# Start FORCE                          #
########################################

PHONY += FORCE 
FORCE:
