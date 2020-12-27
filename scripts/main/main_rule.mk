# SPDX-License-Identifier: GPL-2.0
# ==========================================================================
# main rule
# ==========================================================================

subdir-y		:= $(obj-y) $(subdir-y)
subdir-y		:= $(strip $(sort $(subdir-y)))
subdir-y		:= $(filter %/, $(subdir-y))
subdir-y		:= $(patsubst %/,%,$(subdir-y))

dis-subdir-y 	:= $(obj-)
dis-subdir-y	:= $(strip $(sort $(dis-subdir-y)))
dis-subdir-y	:= $(filter %/, $(dis-subdir-y))
dis-subdir-y	:= $(patsubst %/,%,$(dis-subdir-y))

obj-file	:= $(filter-out %/, $(obj-y))

obj-subfile	:= $(filter %/, $(obj-y))
obj-subfile	:= $(patsubst %/, %/built-in.o, $(obj-subfile))

obj-dis		:= $(filter-out %/, $(obj-))

ifneq ($(strip $(obj-y) $(obj-) $(subdir-m) $(lib-target)),)
builtin-target := $(obj)/built-in.o
endif

########################################
# Add path                             #
########################################

subdir-y		:= $(addprefix $(obj)/,$(subdir-y))
dis-subdir-y	:= $(addprefix $(obj)/,$(dis-subdir-y))
obj-dis			:= $(addprefix $(obj)/,$(obj-dis))
obj-file		:= $(addprefix $(obj)/,$(obj-file))
obj-subfile		:= $(addprefix $(obj)/,$(obj-subfile))

########################################
# targets rule                         #
########################################

main-targets	+= $(obj-file)
main-targets	+= $(builtin-target)
main-targets 	+= $(lib-target)
targets			+= $(main-targets)

########################################
# Always rule                          #
########################################

always-y	+= $(builtin-target)

########################################
# clean rule                           #
########################################

clean-files += $(main-targets) $(obj-dis)
clean-subdir += $(subdir-y) $(dis-subdir-y)
