# SPDX-License-Identifier: GPL-2.0
# ==========================================================================
# main rule
# ==========================================================================

lib-subdir-y	:= $(lib-y) $(subdir-y)
lib-subdir-y	:= $(strip $(sort $(lib-subdir-y)))
lib-subdir-y	:= $(filter %/, $(lib-subdir-y))
lib-subdir-y	:= $(patsubst %/,%,$(lib-subdir-y))

lib-file	:= $(filter-out %/, $(lib-y))

lib-subfile	:= $(filter %/, $(lib-y))
lib-subfile	:= $(patsubst %/, %/lib.a, $(lib-subfile))

ifneq ($(strip $(lib-y) $(lib-) $(subdir-m) $(library-target)),)
library-target := $(obj)/lib.a
endif

########################################
# Add path                             #
########################################

lib-subdir-y	:= $(addprefix $(obj)/,$(lib-subdir-y))
lib-file		:= $(addprefix $(obj)/,$(lib-file))
lib-subfile		:= $(addprefix $(obj)/,$(lib-subfile))

########################################
# targets rule                         #
########################################

lib-targets		+= $(lib-file)
lib-targets		+= $(library-target)
targets			+= $(lib-targets)

########################################
# Always rule                          #
########################################

always-y	+= $(library-target)

########################################
# clean rule                           #
########################################

clean-files += $(lib-targets)
clean-subdir += $(lib-subdir-y)
