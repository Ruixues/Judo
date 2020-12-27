# SPDX-License-Identifier: GPL-2.0
# ==========================================================================
# Main build system
# ==========================================================================
src := $(obj)

PHONY := _build
_build:

#
# Read auto.conf if it exists, otherwise ignore
-include $(MAKE_HOME)/include/config/auto.conf

#
# Include obj makefile
build-dir := $(if $(filter /%,$(src)),$(src),$(MAKE_HOME)/$(src))
build-file := $(if $(wildcard $(build-dir)/Kbuild),$(build-dir)/Kbuild,$(build-dir)/Makefile)
include $(build-file)

#
# Include Buildsystem function
include $(BUILD_HOME)/include/define.mk

#
# Include main rule
include $(BUILD_HOME)/main/main_rule.mk

########################################
# OBJ options                          #
########################################

include_file := $(addprefix -I ,$(INCLUDE))

ifeq (asflags-y,)
asflags-y	:= $(asflags-sub-y)
endif

ifeq (ccflags-y,)
ccflags-y	:= $(ccflags-sub-y)
endif

ifeq (cppflags-y,)
cppflags-y	:= $(cppflags-sub-y)
endif

ifeq (ldflags-y,)
ldflags-y	:= $(ldflags-sub-y)
endif

export asflags-sub-y ccflags-sub-y cppflags-sub-y asflags-sub-y ldflags-sub-y


a_flags		= -Wp,-MD,$(depfile) $(include_file) $(gcc-warning) $(asflags-y) $(acflags-y) 

c_flags		= -Wp,-MD,$(depfile) $(include_file) $(gcc-warning) $(ccflags-y) $(acflags-y) 

cxx_flags	= -Wp,-MD,$(depfile) $(include_file) $(gcc-warning) $(cxxflags-y)

cpp_flags	= -Wp,-MD,$(depfile) $(include_file) $(gcc-warning) $(cppflags-y)

ld_flags	= $(LDFLAGS) $(ldflags-y)

########################################
# Start rule                           #
########################################

#
# include rules: all auxiliary rule
include $(BUILD_HOME)/include/auxiliary.mk

# include process: all auxiliary rule
include $(BUILD_HOME)/generic/process.mk


$(obj-subfile):$(subdir-y)

#
# Rule to compile a set of .o files into one .o file
quiet_cmd_link_o_target = $(ECHO_LD) $@
	  cmd_link_o_target = $(if $(strip $(obj-file) $(obj-subfile) $(subdir-y)),\
		      $(LD) $(ld_flags) -r -o $@ $(obj-file) $(obj-subfile), \
		      rm -f $@; $(AR) rcs$(KBUILD_ARFLAGS) $@)
$(builtin-target): $(obj-file) $(obj-subfile) FORCE
	$(call if_changed,link_o_target)

########################################
# Start build                          #
########################################

_build:  $(subdir-y) $(always-y)

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
	
# Read all saved command lines and dependencies for the $(targets) we
# may be building above, using $(if_changed{,_dep}). As an
# optimization, we don't need to read them if the target does not
# exist, we will rebuild anyway in that case.

targets := $(wildcard $(sort $(targets)))
cmd_files := $(wildcard $(foreach f,$(targets),$(dir $(f)).$(notdir $(f)).cmd))

ifneq ($(cmd_files),)
  include $(cmd_files)
endif

.PHONY: $(PHONY)
