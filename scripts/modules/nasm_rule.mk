# SPDX-License-Identifier: GPL-2.0
# ==========================================================================
# nasm rule
# ==========================================================================

########################################
# Always build                         #
########################################

# bin-always-y += foo
# ... is a shorthand for
# bin += foo
# always-y  += foo
nasm 		+= $(nasm-always-y)
always-y 	+= $(nasm-always-y)

########################################
# Sort files                           #
########################################

nasm := $(sort $(nasm))

########################################
# Filter files                         #
########################################

# nasm code
# Executables compiled from a single .S file
nasm-single	:= $(foreach m,$(nasm), \
			$(if $($(m)-objs),,$(m)))

# C executables linked based on several .o files
nasm-multi	:= $(foreach m,$(nasm),$(if $($(m)-objs),$(m)))

# Object (.o) files compiled from .S files
nasm-objs	:= $(sort $(foreach m,$(nasm),$($(m)-objs)))

########################################
# Add path                             #
########################################

nasm-single	:= $(addprefix $(obj)/,$(nasm-single))
always-y	:= $(addprefix $(obj)/,$(always-y))

targets += $(nasm-single) 

########################################
# clean rule                           #
########################################

clean-files += $(nasm-single)