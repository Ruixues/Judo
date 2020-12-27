# SPDX-License-Identifier: GPL-2.0
# ==========================================================================
# Build elf
# ==========================================================================

#
# Include bin rule
include $(BUILD_HOME)/auxiliary/elf_rule.mk

########################################
# Bin options                          #
########################################

elf_flags	+= $(elf-flags-y)

ifeq ($(elf_flags),)
elf_flags	:= 
endif 

########################################
# Start build                          #
########################################

# Create executable from a single .c file
# host-csingle -> Executable
quiet_cmd_build_elf = $(ECHO_ELF)  $@
      cmd_build_elf	= $(MKELF) $(elf_flags) -o $@ $(elf-objs) $(elf-directs)
$(elf): $(elf-objs) FORCE
	$(call if_changed,build_elf)

targets += $(elf)