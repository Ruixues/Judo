 
# SPDX-License-Identifier: GPL-2.0
# ==========================================================================
# dump rule
# ==========================================================================

########################################
# Always build                         #
########################################

# bin-always-y += foo
# ... is a shorthand for
# bin += foo
# always-y  += foo
dump 		+= $(dump-always-y)
dump-always += $(dump-always-y)

########################################
# Sort files                           #
########################################

dump	:= $(sort $(dump))

########################################
# Filter files                         #
########################################

dump-objs	:= $(sort $(foreach m,$(dump),$($(m)-obj-y)))

########################################
# Add path                             #
########################################

dump			:= $(addprefix $(obj)/,$(dump))
dump-objs		:= $(addprefix $(obj)/,$(dump-objs))
dump-always 	:= $(addprefix $(obj)/,$(dump-always))

########################################
# Always rule                          #
########################################

always-y	+= $(dump-always)

########################################
# clean rule                           #
########################################

clean-files += $(dump)