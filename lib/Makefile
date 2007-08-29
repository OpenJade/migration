# -*- Makefile -*- Time-stamp: <07/07/29 20:44:17 ptr>

SRCROOT := ..

MSG_HEADERS = ArcEngine Parser CmdLineApp EntityApp EntityManager MessageFormatter MessageReporter Catalog ParserApp PosixStorage StdioStorage URLStorage
INST_GEN = app_inst arc_inst entmgr_inst parser_inst xentmgr_inst

define genmsg
EXTRA_PRE += $(1)Messages.h
EXTRA_PRE_DBG += $(1)Messages.h
ifndef WITHOUT_STLPORT
EXTRA_PRE_STLDBG += $(1)Messages.h
endif

$(1)Messages.h:	$(1)Messages.msg
	../msggen.pl -l libModule $(1)Messages.msg
endef

define geninst
EXTRA_PRE += $(1).cxx
EXTRA_PRE_DBG += $(1).cxx
ifndef WITHOUT_STLPORT
EXTRA_PRE_STLDBG += $(1).cxx
endif

$(1).cxx:	$(1).m4
	../instmac.pl $(1).m4 > $(1).cxx
endef

$(foreach name,$(MSG_HEADERS),$(eval $(call genmsg,$(name))))
$(foreach name,$(INST_GEN),$(eval $(call geninst,$(name))))

include Makefile.inc
include ${SRCROOT}/Makefiles/gmake/top.mak

# INCLUDES += -I$(SRCROOT)/include
INCLUDES += -I../include -I../generic
DEFS += -DSP_NAMESPACE=osp -DSP_API= -DSP_ANSI_LIB -DSP_HAVE_BOOL -DSP_DEFINE_TEMPLATES=1

maintainer-clean::
	for f in $(MSG_HEADERS); do rm -f $${f}Messages.h; done
	for f in $(INST_GEN) ; do rm -f $${f}.cxx; done
