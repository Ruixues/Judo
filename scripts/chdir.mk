# SPDX-License-Identifier: GPL-2.0
# ==========================================================================
# chdir system
# ==========================================================================

_all:

#
# Include Buildsystem function
include $(BUILD_HOME)/include/define.mk

########################################
# System start                         #
########################################

_all: makesubmk

########################################
# Start chdir                          #
########################################


makesubmk:
# 	$(Q)$(LN) $(BUILD_HOME) scripts
# 	$(Q)$(LN) $(BUILD_HOME)/Makefile Makefile
# 	$(Q)$(MAKE) $(top) $(fun) O=$(empty)

########################################
# Start FORCE                          #
########################################

PHONY += FORCE 
FORCE: