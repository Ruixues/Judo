# SPDX-License-Identifier: GPL-2.0
# ==========================================================================
# Make dump
# ==========================================================================

#
# Include bin rule
include $(BUILD_HOME)/auxiliary/dump_rule.mk

########################################
# Bin options                          #
########################################

dump_flags	+= $(dump-flags-y)

ifeq ($(dump_flags),)
dump_flags	:= -D
endif 

########################################
# Start build                          #
########################################

# Create executable from a single .c file
# host-csingle -> Executable
quiet_cmd_build_dump = $(ECHO_DUMP)  $@
      cmd_build_dump = $(OBJDUMP) $(dump_flags) $(dump-objs) > $@ 
$(dump): $(dump-objs) FORCE
	$(call if_changed,build_dump)

targets += $(dump)