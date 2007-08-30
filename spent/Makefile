# -*- Makefile -*- Time-stamp: <07/07/29 20:44:17 ptr>

SRCROOT := ..

MSG_HEADERS = Spent

define genmsg
EXTRA_PRE += $(1)Messages.h
EXTRA_PRE_DBG += $(1)Messages.h
ifndef WITHOUT_STLPORT
EXTRA_PRE_STLDBG += $(1)Messages.h
endif

$(1)Messages.h:	$(1)Messages.msg
	../msggen.pl -l appModule $(1)Messages.msg
endef

$(foreach name,$(MSG_HEADERS),$(eval $(call genmsg,$(name))))

include Makefile.inc
include ${SRCROOT}/Makefiles/gmake/top.mak

# INCLUDES += -I$(SRCROOT)/include
INCLUDES += -I../include -I../generic
DEFS += -DSP_NAMESPACE=osp -DSP_API= -DSP_ANSI_LIB -DSP_HAVE_BOOL -DSP_DEFINE_TEMPLATES=1
release-shared:	LDFLAGS += -L../lib/$(OUTPUT_DIR) -losp
dbg-shared:	LDFLAGS += -L../lib/$(OUTPUT_DIR_DBG) -lospg
ifndef WITHOUT_STLPORT
stldbg-shared:	LDFLAGS += -L../lib/$(OUTPUT_DIR_STLDBG) -lospstlg
endif

maintainer-clean::
	for f in $(MSG_HEADERS); do rm -f $${f}Messages.h; done
