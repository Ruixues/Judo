# SPDX-License-Identifier: GPL-2.0
# ===========================================================================
# New kconfig

#
# Include BuildSystem function
include $(BUILD_HOME)/include/define.mk



PHONY += config
kmenuconfig: $(obj)/oldconfig.py
		$< $(Kconfig)

PHONY += menuconfig
menuconfig: $(obj)/menuconfig.py
		$< $(Kconfig)

help:
	$(Q)echo  '  kmenuconfig	  - Update current config utilising a line-oriented program'
	




.PHONY: $(PHONY)
