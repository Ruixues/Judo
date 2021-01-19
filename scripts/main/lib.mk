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
include $(BUILD_HOME)/main/lib_rule.mk

########################################
# OBJ options                          #
########################################

include_file := $(addprefix -I ,$(INCLUDE))

ifneq (asflags-y,)
asflags-y	:= $(asflags-sub-y)
endif

ifneq (ccflags-y,)
ccflags-y	:= $(ccflags-sub-y)
endif

ifneq (cppflags-y,)
cppflags-y	:= $(cppflags-sub-y)
endif

ifneq (ldflags-y,)
ldflags-y	:= $(ldflags-sub-y)
endif

export asflags-sub-y ccflags-sub-y cppflags-sub-y asflags-sub-y ldflags-sub-y


a_flags		= -Wp,-MD,$(depfile) $(include_file) $(asflags-y)

c_flags		= -Wp,-MD,$(depfile) $(include_file) $(ccflags-y)

cpp_flags	= -Wp,-MD,$(depfile) $(include_file) $(cppflags-y)

ld_flags	= $(LDFLAGS) $(ldflags-y)

########################################
# Start rule                           #
########################################

#
# include rules: all auxiliary rule
include $(BUILD_HOME)/include/auxiliary.mk

# include process: all auxiliary rule
include $(BUILD_HOME)/generic/process.mk

$(lib-subfile):$(subdir-y)

#
# Rule to compile a set of .o files into one .a file
quiet_cmd_link_l_target = $(ECHO_AR)	$@
	  cmd_link_l_target = rm -f $@; $(AR) rcs$(KBUILD_ARFLAGS) $@ $(lib-file) $(lib-subfile)
$(library-target): $(lib-file) $(lib-subfile) FORCE
	$(call if_changed,link_l_target)

########################################
# Start build                          #
########################################

_build: $(always-y) $(subdir-y) 

########################################
# Descending build                     #
########################################

PHONY += $(subdir-y)
$(subdir-y):
	$(Q)$(MAKE) $(build_lib)=$@

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
 
