# SPDX-License-Identifier: GPL-2.0
# ==========================================================================
# Build cust
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
# Include cust rule
include $(BUILD_HOME)/modules/cust_rule.mk

########################################
# Cust options                         #
########################################

ifneq ($(cust-cpp),)
CUST_CPP		:= $(cust-cpp)
endif

ifneq ($(cust-as),)
CUST_AS			:= $(cust-as)
endif

ifneq ($(cust-cc),)
CUST_CC			:= $(cust-cc)
endif

include_file := $(addprefix -I ,$(INCLUDE))

cust_a_flags	:= $(include_file) $(cust-asflags-y)
cust_c_flags	:= $(include_file) $(cust-ccflags-y)
cust_ld_flags	:= $(include_file) $(cust-ldflags-y)

########################################
# Start rule                           #
########################################

#
# include rules: all auxiliary rule
include $(BUILD_HOME)/include/auxiliary.mk

# Create executable from a single .c file
# cust-csingle -> Executable
quiet_cmd_cust-csingle 	= $(ECHO_CUSTCC) $@
      cmd_cust-csingle	= $(CUST_CC) $(cust_c_flags) -o $@ $<
$(cust-single): $(obj)/%: $(src)/%.c FORCE
	$(call if_changed,cust-csingle)

# Link an executable based on list of .o files, all plain c
# cust-multi -> executable
quiet_cmd_cust-cmulti	= $(ECHO_CUSTLD) $@
      cmd_cust-cmulti	= $(CUST_LD) $(cust_ld_flags) -o $@ \
	  					$(addprefix $(obj)/,$($(@F)-obj-y)) 
$(cust-multi): FORCE
	$(call if_changed,cust-cmulti)
$(call multi_depend, $(cust-multi), , -obj-y)

#
# Create single .s middle file from single .c file
quiet_cmd_cc_s_c = $(ECHO_CUSTCC) $@
	  cmd_cc_s_c = $(CUST_CC) $(cust_c_flags) -fverbose-asm -S -o $@ $<
$(obj)/%.s: $(src)/%.c FORCE
	$(call if_changed,cc_s_c)

#
# Create single .s middle file from single .c file	
quiet_cmd_cc_i_c = $(ECHO_CUSTCPP) $@
	  cmd_cc_i_c = $(CUST_CPP) $(cust_c_flags)   -o $@ $<
$(obj)/%.i: $(src)/%.c FORCE
	$(call if_changed,cc_i_c)

#
# Create single .s middle file from single .c file
quiet_cmd_cc_o_c = $(ECHO_CUSTCC) $@
	  cmd_cc_o_c = $(CUST_CC) $(cust_c_flags) -c -o $@ $<
$(obj)/%.o: $(src)/%.c FORCE
	$(call if_changed,cc_o_c)

quiet_cmd_cc_lst_c = MKLST   $@
      cmd_cc_lst_c = $(CUST_CC) $(cust_c_flags) -g -c -o $*.o $< && \
		     $(CONFIG_SHELL) $(srctree)/scripts/makelst $*.o \
				     System.map $(OBJDUMP) > $@
$(obj)/%.lst: $(src)/%.c FORCE
	$(call if_changed,cc_lst_c)

quiet_cmd_as_s_S	= $(ECHO_CUSTCPP) $@
	  cmd_as_s_S	= $(CUST_CPP) $(cust_a_flags) -o $@ $< 
$(obj)/%.s: $(src)/%.S FORCE
	$(call if_changed,as_s_S)

quiet_cmd_as_o_S = $(ECHO_CUSTAS) $@
	  cmd_as_o_S = $(CUST_AS) $(cust_a_flags) -o $@ $<
$(obj)/%.o: $(src)/%.S FORCE
	$(call if_changed,as_o_S)

########################################
# Start build                          #
########################################

_build: $(subdir-y) $(always-y)

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

