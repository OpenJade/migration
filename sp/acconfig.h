// Copyright (C) 1999 Matthias Clasen, Peter Nilsson
// See the file COPYING for copying permissions.


/* these are needed for GNU gettext */
#undef ENABLE_NLS
#undef HAVE_CATGETS
#undef HAVE_GETTEXT
#undef HAVE_LC_MESSAGES
#undef HAVE_STPCPY

/* set this to the default catalog files list */
#undef SGML_CATALOG_FILES_DEFAULT

/* set this to the default SGML search path */ 
#undef SGML_SEARCH_PATH_DEFAULT

/* define this if h_errno should be declared extern int */
#undef SP_DECLARE_H_ERRNO

/* define this if memmove  should be declared extern "C" */
#undef DECLARE_MEMMOVE

/* define this if strerror should be declared extern "C" */
#undef DECLARE_STRERROR

/* define this if sockets are available */
#undef SP_HAVE_SOCKET

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

/* define this if the new handler takes size_t and returns int. */
#undef SP_FANCY_NEW_HANDLER

/* set this to the path separator */
#undef PATH_SEPARATOR

/* Define if you have the gettext() function */
#undef HAVE_GETTEXT

/*** I think everything below this line can safely be ignored ***/

/* if this is not set it defaults to 200 */
#undef DEFAULT_ERROR_LIMIT

/* define this if you can't or don't want to use the builtin assert() macro */ 
#undef SP_USE_OWN_ASSERT 

/* set these to the line term character(s) */
#undef SP_LINE_TERM1 
#undef SP_LINE_TERM2

/*** I think everything below this line is windows only ***/

/* define this to use win32 mutex mechanisms */
#undef SP_MUTEX_WIN32

/* define this if _setmode is supported */
#undef SP_HAVE_SETMODE

/* define this to not compile message texts in */
#undef SP_NO_MESSAGE_TEXT

/* define the following to build a dll */
#undef SP_USE_DLL
#undef SP_DLLEXPORT
#undef SP_DLLIMPORT

/* define this if the OS provides wide character interfaces */
#undef SP_WIDE_SYSTEM 

/* define this to enable precompiled header support */
#undef SP_PCH

/* the location we install our messages */
#undef SP_LOCALE_DIR

@TOP@

#error The stuff above TOP goes to the top of config.h.in
#error What appears below BOTTOM goes to the bottom
#error This text should not appear anywhere!

@BOTTOM@

/* do not change below this line */
/* derived flags and dependencies between flags */

#ifdef HAVE_UNISTD_H
#define SP_INCLUDE_UNISTD_H 
#endif

#ifdef HAVE_IO_H
#undef SP_INCLUDE_IO_H     
#endif

#ifdef HAVE_OSFCN_H
#undef SP_INCLUDE_OSFCN_H     
#endif

#ifndef SP_MSDOS_FILENAMES
#define SP_POSIX_FILENAMES
#endif

#ifdef SP_WIDE_SYSTEM
#define SP_MULTI_BYTE
#endif

#ifdef HAVE_ST_BLKSIZE
#define SP_STAT_BLKSIZE 
#endif

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

#if defined(HAVE_GETTEXT) && defined(ENABLE_NLS)
#define SP_HAVE_GETTEXT 
#endif /* HAVE_GETTEXT && ENABLE_NLS */

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

#if defined(HAVE_NEW) && defined(HAVE_CASSERT)
#define SP_ANSI_LIB 1
#endif /* HAVE_NEW && HAVE_CASSERT */
