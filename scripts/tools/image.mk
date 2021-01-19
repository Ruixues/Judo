# Commands useful for building a boot image
# ===========================================================================
# 
#	Use as following:
#
#	target: source(s) FORCE
#		$(if_changed,ld/objcopy/gzip)
#
#	and add target to extra-y so that we know we have to
#	read in the saved command line

# Linking
# ---------------------------------------------------------------------------

quiet_cmd_ld = LD      $@
cmd_ld = $(LD) $(LDFLAGS) $(ldflags-y) $(LDFLAGS_$(@F)) \
	       $(filter-out FORCE,$^) -o $@ 

# Objcopy
# ---------------------------------------------------------------------------

quiet_cmd_objcopy = OBJCOPY $@
cmd_objcopy = $(OBJCOPY) $(OBJCOPYFLAGS) $(OBJCOPYFLAGS_$(@F)) $< $@

# Gzip
# ---------------------------------------------------------------------------

quiet_cmd_gzip = GZIP    $@
cmd_gzip = (cat $(filter-out FORCE,$^) | gzip -n -f -9 > $@) || \
	(rm -f $@ ; false)

# Bzip2
# ---------------------------------------------------------------------------

# Bzip2 and LZMA do not include size in file... so we have to fake that;
# append the size as a 32-bit littleendian number as gzip does.
size_append = printf $(shell						\
dec_size=0;								\
for F in $1; do								\
	fsize=$$(stat -c "%s" $$F);					\
	dec_size=$$(expr $$dec_size + $$fsize);				\
done;									\
printf "%08x\n" $$dec_size |						\
	sed 's/\(..\)/\1 /g' | {					\
		read ch0 ch1 ch2 ch3;					\
		for ch in $$ch3 $$ch2 $$ch1 $$ch0; do			\
			printf '%s%03o' '\\' $$((0x$$ch)); 		\
		done;							\
	}								\
)

quiet_cmd_bzip2 = BZIP2   $@
cmd_bzip2 = (cat $(filter-out FORCE,$^) | \
	bzip2 -9 && $(call size_append, $(filter-out FORCE,$^))) > $@ || \
	(rm -f $@ ; false)

# Lzma
# ---------------------------------------------------------------------------

quiet_cmd_lzma = LZMA    $@
cmd_lzma = (cat $(filter-out FORCE,$^) | \
	lzma -9 && $(call size_append, $(filter-out FORCE,$^))) > $@ || \
	(rm -f $@ ; false)

quiet_cmd_lzo = LZO     $@
cmd_lzo = (cat $(filter-out FORCE,$^) | \
	lzop -9 && $(call size_append, $(filter-out FORCE,$^))) > $@ || \
	(rm -f $@ ; false)

# XZ
# ---------------------------------------------------------------------------
# Use xzkern to compress the kernel image and xzmisc to compress other things.
#
# xzkern uses a big LZMA2 dictionary since it doesn't increase memory usage
# of the kernel decompressor. A BCJ filter is used if it is available for
# the target architecture. xzkern also appends uncompressed size of the data
# using size_append. The .xz format has the size information available at
# the end of the file too, but it's in more complex format and it's good to
# avoid changing the part of the boot code that reads the uncompressed size.
# Note that the bytes added by size_append will make the xz tool think that
# the file is corrupt. This is expected.
#
# xzmisc doesn't use size_append, so it can be used to create normal .xz
# files. xzmisc uses smaller LZMA2 dictionary than xzkern, because a very
# big dictionary would increase the memory usage too much in the multi-call
# decompression mode. A BCJ filter isn't used either.
quiet_cmd_xzkern = XZKERN  $@
cmd_xzkern = (cat $(filter-out FORCE,$^) | \
	sh $(srctree)/scripts/xz_wrap.sh && \
	$(call size_append, $(filter-out FORCE,$^))) > $@ || \
	(rm -f $@ ; false)

quiet_cmd_xzmisc = XZMISC  $@
cmd_xzmisc = (cat $(filter-out FORCE,$^) | \
	xz --check=crc32 --lzma2=dict=1MiB) > $@ || \
	(rm -f $@ ; false) 
