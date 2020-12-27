# SPDX-License-Identifier: GPL-2.0
# ==========================================================================
# fixdep system
# ==========================================================================

#
# Include Buildsystem function
include $(BUILD_HOME)/include/define.mk

# Basic helpers built in scripts/
PHONY += scripts_basic
scripts_basic:
	$(Q)$(MAKE) $(build)=$(BUILD_HOME)/basic 

########################################
# Start FORCE                          #
########################################

PHONY += FORCE 
FORCE:

# Declare the contents of the PHONY variable as phony.  We keep that
# information in a variable so we can use it in if_changed and friends.
.PHONY: $(PHONY)
