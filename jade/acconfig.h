// Copyright (C) 1999 Matthias Clasen, Peter Nilsson
// See the file COPYING for copying permissions.


#include <OpenSP/config.h>

@TOP@

/* these are needed for GNU gettext */
#undef ENABLE_NLS
#undef HAVE_CATGETS
#undef HAVE_GETTEXT
#undef HAVE_LC_MESSAGES
#undef HAVE_STPCPY

#undef OPENJADE_PACKAGE
#undef OPENJADE_VERSION
#undef OPENJADE_LOCALE_DIR

/* set this to the location of builtins.dsl */
#define DEFAULT_SCHEME_BUILTINS "builtins.dsl"

/* define this to build the html backend */
#undef JADE_HTML

/* define this to build the mif backend */
#undef JADE_MIF

/* define this to build the grove in a separate thread, if possible */
#undef USE_THREADS
 
/* define this appropriately if the type sig_atomic_t is not available */
#undef sig_atomic_t

/* define this if your system misses new.h */
#undef SP_NEW_H_MISSING

/* define this if  set_new_handler() has to be declared extern "C" */
#undef SP_SET_NEW_HANDLER_EXTERN_C 

/* define this to build a multi-byte version */
#undef SP_MULTI_BYTE

/* define this to include template definitions in the headers */
#undef SP_DEFINE_TEMPLATES

/* define this to compile explicit template instantiations */
#undef SP_MANUAL_INST

/* define this if new.h doesn't declare void *operator new(size_t, void *p) */
#undef SP_DECLARE_PLACEMENT_OPERATOR_NEW

/* define this if the new handler takes size_t and returns int. */
#undef SP_FANCY_NEW_HANDLER

@BOTTOM@

#ifdef HAVE_BOOL 
#define SP_HAVE_BOOL
#endif /* HAVE_BOOL */

#if (SIZEOF_BOOL == 1)
#define SP_SIZEOF_BOOL_1
#endif

#if (SIZEOF_WCHAR_T == SIZEOF_UNSIGNED_SHORT)
#define SP_WCHAR_T_USHORT
#endif

#ifdef SP_HAVE_SETMODE
#ifndef SP_LINE_TERM1
#define SP_LINE_TERM1 '\r'
#define SP_LINE_TERM2 '\n'
#endif
#endif /* not SP_HAVE_SETMODE */

#ifndef SP_LINE_TERM1
#define SP_LINE_TERM1 '\n'
#endif

#ifndef HAVE_NEW_FOR_SCOPING
// This simulates the new ANSI "for" scope rules
#define for if (0); else for
#endif /* HAVE_NEW_FOR_SCOPING */

#ifndef SP_HAVE_TYPENAME
#define typename /* as nothing */
#endif

#ifndef SP_DLLEXPORT
#define SP_DLLEXPORT /* as nothing */
#endif

#ifndef SP_DLLIMPORT
#define SP_DLLIMPORT /* as nothing */
#endif

#ifdef SP_USE_DLL

#ifdef BUILD_LIBSP
#define SP_API SP_DLLEXPORT
#else
#define SP_API SP_DLLIMPORT
#endif

#else /* not SP_USE_DLL */

#define SP_API /* as nothing */

#endif /* not SP_USE_DLL */

#ifdef SP_WIDE_SYSTEM
#ifndef SP_MULTI_BYTE
#define SP_MULTI_BYTE
#endif
#endif

#ifdef HAVE_NAMESPACES
#define SP_NAMESPACE OpenSP
#define DSSSL_NAMESPACE OpenJade_DSSSL
#define GROVE_NAMESPACE OpenJade_Grove
#endif /* HAVE_NAMESPACES */

#ifdef SP_NAMESPACE
#define SP_NAMESPACE_SCOPE SP_NAMESPACE::
#else
#define SP_NAMESPACE_SCOPE
#endif /* SP_NAMESPACE */

#ifdef HAVE_DYNAMIC_CAST
#define SP_HAVE_RTTI
#endif /* HAVE_DYNAMIC_CAST */

#ifdef HAVE_TYPENAME
#define SP_HAVE_TYPENAME
#endif /* HAVE_TYPENAME */

#ifdef HAVE_PATHNAME_STYLE_DOS
#define SP_MSDOS_FILENAMES 
#else 
#define SP_POSIX_FILENAMES  
#endif /* HAVE_PATHNAME_STYLE_DOS */

#ifdef HAVE_INSTANTIATIONS
#define SP_ANSI_CLASS_INST
#endif /* HAVE_INSTANTIATIONS */ 

#ifdef HAVE_SETLOCALE
#define SP_HAVE_LOCALE
#endif /* HAVE_SETLOCALE */

#ifdef HAVE_GETTEXT
#define SP_HAVE_GETTEXT 
#endif /* HAVE_GETTEXT */

#ifdef WORDS_BIGENDIAN
#define SP_BIG_ENDIAN
#endif /* WORDS_BIGENDIAN */

#ifdef HAVE_LIBTHREADS
#define SP_MUTEX_MACH
#endif /* HAVE_LIBTHREADS */

#ifdef HAVE_LIBPTHREAD
#define SP_MUTEX_PTHREADS
#endif /* HAVE_LIBPTHREAD */

#ifdef HAVE_PLACEMENT_OPERATOR_DELETE
#define SP_HAVE_PLACEMENT_OPERATOR_DELETE
#endif /* HAVE_PLACEMENT_OPERATOR_DELETE */

#ifndef HAVE_PLACEMENT_OPERATOR_NEW
#define SP_DECLARE_PLACEMENT_OPERATOR_NEW
#endif /* HAVE_PLACEMENT_OPERATOR_NEW */

#ifndef HAVE_NAMESPACE_STD
#define SP_NO_STD_NAMESPACE
#endif /* HAVE_NAMESPACE_STD */
