# SPDX-License-Identifier: GPL-2.0
# ==========================================================================
# System top
# ==========================================================================

########################################
# System start                         #
########################################

start:

########################################
# Start path                           #
########################################

#
# Real home
ifndef home
home 		:= $(CURDIR)
endif

#
# Project home
MAKE_HOME 	:= $(CURDIR)

#
# Build system home
BUILD_HOME 	:= $(abspath $(lastword $(MAKEFILE_LIST)/../))

#
# Build relative path
objtree		:= .

#
# Kconfig path config
ifndef Kconfig
Kconfig := $(MAKE_HOME)/Kconfig
endif

export home MAKE_HOME BUILD_HOME objtree Kconfig

########################################
# Start env                            #
########################################

LIGHYBUILD_VERSION := v1.0

# Do not use make's built-in rules and variables
# Do not print "Entering directory ...",
MAKEFLAGS	+= -rR
MAKEFLAGS	+= --no-print-directory

# To put more focus on warnings, be less verbose as default
# Use 'make V=1' to see the full commands

ifndef V
  DEBUG_MODE := 0
endif

ifeq ("$(origin V)", "command line")
  DEBUG_MODE = $(V)
endif

ifeq ($(DEBUG_MODE),0)
  quiet		=quiet_
  MAKEFLAGS	+= -s
  Q 		= @
endif
ifeq ($(DEBUG_MODE),1)
  quiet =
  Q =
endif
export quiet Q

ifeq ("$(origin W)", "command line")
  export BUILD_ENABLE_EXTRA_GCC_CHECKS := $(W)
endif

ifeq ("$(origin G)", "command line")
  export BUILD_ENABLE_EXTRA_GCC_DEBUG := $(G)
endif

# OK, Make called in directory where kernel src resides
# Do we want to locate output files in a separate directory?
ifeq ("$(origin O)", "command line")
export BUILD_OUTPUT := $(addprefix $(MAKE_HOME)/,$(O))
endif

ifeq ($(BUILD_OUTPUT),)
start: all
endif
ifneq ($(BUILD_OUTPUT),)
$(filter-out start , $(MAKECMDGOALS)) start:
	$(Q)$(MKDIR) $(BUILD_OUTPUT)
	$(Q)$(MAKE) -C $(BUILD_OUTPUT) $(chdir) \
	fun=$(MAKECMDGOALS)
endif

#
# Extre Warning
include $(BUILD_HOME)/include/warn.mk

#
# Read auto.conf if it exists, otherwise ignore
-include $(MAKE_HOME)/include/config/auto.conf

#
# Tool Define  
include $(BUILD_HOME)/include/define.mk

ifeq ($(BUILD_OUTPUT),)
########################################
# Start config                         #
########################################

# *config targets only - make sure prerequisites are updated, and descend
# in scripts/kconfig to make the *config target

config_dir := include/config configs
config_dir := $(addprefix $(MAKE_HOME)/,$(config_dir))

config: FORCE
	$(Q)$(MAKE) $(basic)
	$(Q)$(MKDIR) $(config_dir)
	$(Q)$(MAKE) $(build_host)=$(BUILD_HOME)/kconfig $@
	$(Q)$(MAKE) $(build_host)=$(BUILD_HOME)/kconfig syncconfig

menuconfig: FORCE
	$(Q)$(MAKE) $(basic)
	$(Q)$(MKDIR) $(config_dir)
	$(Q)$(MAKE) $(build_host)=$(BUILD_HOME)/newconfig $@
	$(Q)$(MAKE) $(build_host)=$(BUILD_HOME)/kconfig syncconfig

%config: FORCE
	$(Q)$(MAKE) $(basic)
	$(Q)$(MKDIR) $(config_dir)
	$(Q)$(MAKE) $(build_host)=$(BUILD_HOME)/kconfig $@
	$(Q)$(MAKE) $(build_host)=$(BUILD_HOME)/kconfig syncconfig

########################################
# Start submake                        #
########################################

PHONY += $(submake_fun) submake

submake_fun += remake build env clean mrproper distclean checkstack coccicheck help

$(submake_fun): submake
submake: FORCE 
	$(Q)$(MAKE) $(submake)=$(MAKE_HOME) $(if $(MAKECMDGOALS),_$(MAKECMDGOALS))

########################################
# clean tools                          #
########################################

cleantools:
	$(Q)$(MAKE) $(clean)=$(BUILD_HOME)

########################################
# Start version                        #
########################################

version:
	$(Q)$(ECHO) $(LIGHYBUILD_VERSION)

endif #BUILD_OUTPUT

########################################
# Start FORCE                          #
########################################

PHONY += FORCE
FORCE:

# Declare the contents of the PHONY variable as phony.  We keep that
# information in a variable so we can use it in if_changed and friends.
.PHONY: $(PHONY)
