# SPDX-License-Identifier: GPL-2.0
# ==========================================================================
# Recursion build system
# ==========================================================================
submk := $(obj)

_build:

#
# Include Buildsystem function
include $(BUILD_HOME)/include/define.mk

#
# Read auto.conf if it exists, otherwise ignore
-include $(MAKE_HOME)/include/config/auto.conf

#
# Include sub makefile
sub-dir := $(if $(filter /%,$(submk)),$(submk),$(MAKE_HOME)/$(submk))
sub-file := $(if $(wildcard $(sub-dir)/Kbuild),$(sub-dir)/Kbuild,$(sub-dir)/Makefile)
include $(sub-file)

########################################
# Start project                        #
########################################

project		:= $(project-y)
project		:= $(strip $(sort $(project)))
project		:= $(filter %/, $(project))
project		:= $(patsubst %/,%,$(project))
project		:= $(addprefix $(obj)/,$(project))

project-n	:= $(project-)
project-n	:= $(strip $(sort $(project-n)))
project-n	:= $(filter %/, $(project-n))
project-n	:= $(patsubst %/,%,$(project-n))
project-n	:= $(addprefix $(obj)/,$(project-n))

########################################
# include dirs                         #
########################################

INCLUDE		:= $(addprefix $(obj)/,$(project-include-y)) \
				$(project-include-direct-y) $(INCLUDE)
export INCLUDE

########################################
# Start build                          #
########################################

PHONY += _build
_build: $(project)
	$(Q)$(MAKE) $(basic)
	$(Q)$(MAKE) $(build)=$(sub-dir)
	$(call hook_build)

########################################
# Start remake                         #
########################################

PHONY += _remake
_remake: 
	$(Q)$(MAKE) $(submake)=$(MAKE_HOME) _clean
	$(Q)$(MAKE) $(submake)=$(MAKE_HOME) _build

########################################
# Start build                          #
########################################

PHONY += _env
_env: $(project)
	$(Q)$(MAKE) $(env)=$(sub-dir)

########################################
# Start clean                          #
########################################

#
# clean
PHONY += $(clean-dirs) clean 

RCS_FIND_IGNORE := \( -name SCCS -o -name BitKeeper -o \
                      -name .svn -o -name CVS -o -name .pc -o \
                      -name .hg -o -name .git \) -prune -o

_clean: $(project) $(project-n)
	$(Q)$(MAKE) $(clean)=$(sub-dir)
	$(call hook_clean)

#
# mrproper
MRPROPER_DIRS	+= include/config include/generated
MRPROPER_FILES	+= .config .config.old tags TAGS cscope* GPATH GTAGS GRTAGS GSYMS

MRPROPER_DIRS	:= $(addprefix $(obj)/,$(MRPROPER_DIRS))
MRPROPER_FILES	:= $(addprefix $(obj)/,$(MRPROPER_FILES))

rm-dirs  := $(wildcard $(MRPROPER_DIRS))
rm-files := $(wildcard $(MRPROPER_FILES))
mrproper-dirs := $(addprefix _mrproper_,$(rm-dirs))
mrproper-files := $(addprefix _mrproper_,$(rm-files))

PHONY += $(mrproper-dirs) 
$(mrproper-dirs):
	$(Q)$(ECHO) "  $(ECHO_RMDIR) $(patsubst _mrproper_%,%,$@)"
	$(Q)$(RMDIR) $(patsubst _mrproper_%,%,$@)

PHONY += $(mrproper-files)
$(mrproper-files):
	$(Q)$(ECHO) "  $(ECHO_RM) $(patsubst _mrproper_%,%,$@)"
	$(Q)$(RM) $(patsubst _mrproper_%,%,$@)

PHONY += _mrproper
_mrproper: $(mrproper-files) $(mrproper-dirs) _clean
	$(call cmd,rmdirs)
	$(call cmd,rmfiles)

# distclean
#
PHONY += _distclean

_distclean: $(project) _mrproper
	$(Q)find $(MAKE_HOME) \
		\( -name '*.orig' -o -name '*.rej' -o -name '*~' \
		-o -name '*.bak' -o -name '#*#' -o -name '.*.orig' \
		-o -name '.*.rej' -o -size 0 \
		-o -name '*%' -o -name '.*.cmd' -o -name 'core' \) \
		-type f -print | xargs rm -f


########################################
# Start help                           #
########################################

PHONY += _help
_help:
	$(Q)$(ECHO)  'System information:'
	$(Q)$(ECHO)  '  Build-Version  = $(LIGHYBUILD_VERSION)'
	$(Q)$(ECHO)  '  CC             = $(CC)'
	$(Q)$(ECHO)  '  CC-Version     = $(call cc-version)'
	$(Q)$(ECHO)  '  Real Home      = $(home)'
	$(Q)$(ECHO)  '  Make Home      = $(MAKE_HOME)'
	$(Q)$(ECHO)  '  Build Home     = $(BUILD_HOME)'
	$(Q)$(ECHO)  '  Kconfig file   = $(Kconfig)'
	$(Q)$(ECHO)  ''
	$(Q)$(ECHO)  'Auto targets:'
	$(Q)$(ECHO)  '  remake		 - The code is automatically cleared and built'
	$(Q)$(ECHO)  ''
	$(Q)$(ECHO)  'Build targets:'
	$(Q)$(ECHO)  '  build		 - Build all necessary images depending on configuration'
	$(Q)$(ECHO)  ''
	$(Q)$(ECHO)  'Configuration targets:'
	$(Q)$(MAKE)   -f $(MAKE_HOME)/scripts/kconfig/Makefile help
	$(Q)$(ECHO)  ''
	$(Q)$(ECHO)  'Py configuration targets:'
	$(Q)$(MAKE)   -f $(MAKE_HOME)/scripts/newconfig/Makefile help
	$(Q)$(ECHO)  ''
	$(Q)$(ECHO)  'Other generic targets:'
	$(Q)$(ECHO)  '  info		  - Build targets informatio'
	$(Q)$(ECHO)  ''
	$(Q)$(ECHO)  'Cleaning project:'
	$(Q)$(ECHO)  '  clean		  - Only use rules to clean targets'
	$(Q)$(ECHO)  '  mrproper	  - Remove all generated files + config + various backup files'
	$(Q)$(ECHO)  '  distclean	  - mrproper + remove editor backup and patch files'
	$(Q)$(ECHO)  '  cleantools	  - Remove buildsystem tools generated files'
	$(Q)$(ECHO)  ''
	$(Q)$(ECHO)  'Static analysers'
	$(Q)$(ECHO)  '  checkstack      - Generate a list of stack hogs'
	$(Q)$(ECHO)  '  coccicheck      - Execute static code analysis with Coccinelle'
	$(Q)$(ECHO)  ''
	$(Q)$(ECHO)  '  make V=0|1 [targets] 0 => quiet build (default), 1 => verbose build'
	$(Q)$(ECHO)  '  make V=2   [targets] 2 => give reason for rebuild of target'
	$(Q)$(ECHO)  '  make G=1   [targets] 0 => None debug info  1 => Enable debug info'
	$(Q)$(ECHO)  '  make O=dir [targets] Locate all output files in "dir", including .config'
	$(Q)$(ECHO)  '  make C=1   [targets] Check all c source with $$CHECK (sparse by default)'
	$(Q)$(ECHO)  '  make C=2   [targets] Force check of all c source with $$CHECK'
	$(Q)$(ECHO)  '  make RECORDMCOUNT_WARN=1 [targets] Warn about ignored mcount sections'
	$(Q)$(ECHO)  '  make W=n   [targets] Enable extra gcc checks, n=1,2,3 where'
	$(Q)$(ECHO)  '		1: warnings which may be relevant and do not occur too often'
	$(Q)$(ECHO)  '		2: warnings which occur quite often but may still be relevant'
	$(Q)$(ECHO)  '		3: more obscure warnings, can most likely be ignored'
	$(Q)$(ECHO)  '		Multiple levels can be combined with W=12 or W=123'

########################################
# Start checkstack                     #
########################################

CHECKSTACK_ARCH := $(ARCH)
CHECKSTACK_EXE  := 

_checkstack:
	$(OBJDUMP) -d $(CHECKSTACK_EXE) | $(PERL) \
	$(BUILD_HOME)/checkstack.pl $(CHECKSTACK_ARCH)

########################################
# Start coccicheck                     #
########################################

_coccicheck:
	$(Q)$(SHELL) $(BUILD_HOME)/$@

########################################
# Descending operation                 #
########################################

PHONY += $(project)
$(project): FORCE
	$(Q)$(MAKE) $(submake)=$@ $(MAKECMDGOALS)

PHONY += $(project-n)
$(project-n): FORCE
	$(Q)$(MAKE) $(submake)=$@ $(MAKECMDGOALS)

########################################
# Start FORCE                          #
########################################

PHONY += FORCE 
FORCE:

# Declare the contents of the PHONY variable as phony.  We keep that
# information in a variable so we can use it in if_changed and friends.
.PHONY: $(PHONY)
