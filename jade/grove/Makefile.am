## Process this file with automake to produce Makefile.in

lib_LTLIBRARIES = libogrove.la

libogrove_la_LDFLAGS = -version-info 1:0:0

libogrove_la_SOURCES = Node.h Node.cxx LocNode.h LocNode.cxx

# This is needed by ../style/FOTBuilder.h
pkginclude_HEADERS = Node.h

EXTRA_DIST = grove.dsp

MAINTAINERCLEANFILES = Makefile.in
