# SPDX-License-Identifier: GPL-2.0
# ==========================================================================
# cust rule
# ==========================================================================

########################################
# Always build                         #
########################################

# cust-always-y += foo
# ... is a shorthand for
# cust += foo
# always-y  += foo
cust 		+= $(cust-always-y)
cust-always += $(cust-always-y)

########################################
# Sort files                           #
########################################

cust := $(sort $(cust))

########################################
# Filter files                         #
########################################

cust-single := $(foreach m,$(cust), \
			$(if $($(m)-obj-y),,$(m)))
			
cust-multi := $(foreach m,$(cust), \
			$(if $($(m)-obj-y),$(m)))

cust-objs	:= $(sort $(foreach m,$(cust),$($(m)-obj-y)))

########################################
# Add path                             #
########################################

cust-single := $(addprefix $(obj)/,$(cust-single))
cust-multi	:= $(addprefix $(obj)/,$(cust-multi))
cust-objs	:= $(addprefix $(obj)/,$(cust-objs))
cust-always	:= $(addprefix $(obj)/,$(cust-always))


cust-targets	+= $(cust-objs)
cust-targets	+= $(cust-single)
cust-targets	+= $(cust-multi)
targets			+= $(cust-targets)

########################################
# Always rule                          #
########################################

always-y	+= $(cust-always)

########################################
# clean rule                           #
########################################

clean-files += $(cust-targets)
clean-subdir += $()
