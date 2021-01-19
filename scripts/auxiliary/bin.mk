# SPDX-License-Identifier: GPL-2.0
# ==========================================================================
# Make bin
# ==========================================================================

#
# Include bin rule
include $(BUILD_HOME)/auxiliary/bin_rule.mk

########################################
# Bin options                          #
########################################

bin_flags	+= $(bin-flags-y)

ifeq ($(bin_flags),)
bin_flags	:= -O binary
endif 

########################################
# Start build                          #
########################################

quiet_cmd_build_bin = $(ECHO_BIN)  $@
      cmd_build_bin	= $(OBJCOPY) $(bin_flags) $(bin-objs) $@ 
$(bin): $(bin-objs) FORCE
	$(call if_changed,build_bin)

targets += $(bin)