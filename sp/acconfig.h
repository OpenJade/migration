/* set this to the default catalog files list */
#undef SGML_CATALOG_FILES_DEFAULT

/* set this to the default SGML search path */ 
#undef SGML_SEARCH_PATH_DEFAULT

/* define this if h_errno should be declared extern int */
#undef SP_DECLARE_H_ERRNO

/* define this if sockets are available */
#undef SP_HAVE_SOCKET

/* define this appropriately if the type sig_atomic_t is not available */
#undef sig_atomic_t

/* define this if your system misses new.h */
#undef SP_NEW_H_MISSING 

/* define this if  set_new_handler() has to be declared extern "C" */
#undef SP_SET_NEW_HANDLER_EXTERN_C 

#undef SP_ANSI_LIB       

/* define this if  std:: namespace is not supported */
#undef SP_NO_STD_NAMESPACE 

/* define this if setlocale() is available */
#undef SP_HAVE_LOCALE

/* define this if gettext() is available */
#undef SP_HAVE_GETTEXT 

/* define this to not compile message texts in (windows only) */
#undef SP_NO_MESSAGE_TEXT

/* define this if the OS provides wide character interfaces */
#undef SP_WIDE_SYSTEM 

/* define this to build a multi-byte version */
#undef SP_MULTI_BYTE

/* set this to the first line term character */
#undef SP_LINE_TERM1 

/* set this to the second line term character */
#undef SP_LINE_TERM2

/* define this to include template definitions in the headers */
#undef SP_DEFINE_TEMPLATES

/* define this to compile explicit template instantiations */
#undef SP_MANUAL_INST

/* define the following to build a dll (windows only) */
#undef SP_USE_DLL
#undef SP_DLLEXPORT
#undef SP_DLLIMPORT

/* define this to enable precompiled header support (windows only) */
#undef SP_PCH

/* define this to use win32 mutex mechanisms (windows only) */
#undef SP_MUTEX_WIN32

/* define this to use Mach mutex mechanisms (mach only) */
#undef SP_MUTEX_MACH

/* define this to use pthreads based mutex mechanisms */
#undef SP_MUTEX_PTHREADS

/* define this if qualified templated destructors like T::~T are broken */
#undef SP_QUAL_TEMPLATE_DTOR_BROKEN

/* define this if new.h doesn't declare void *operator new(size_t, void *p) */
#undef SP_DECLARE_PLACEMENT_OPERATOR_NEW

/* define this if void T::operator delete(void *p, Allocator &); is supported */
#undef SP_HAVE_PLACEMENT_OPERATOR_DELETE

/* define this if the new handler takes size_t and returns int. */
#undef SP_FANCY_NEW_HANDLER

/* define this if _setmode is supported */
#undef SP_HAVE_SETMODE
#undef _O_BINARY 
#undef S_IFMT 

/* set this to the path separator */
#undef PATH_SEPARATOR

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

#ifndef MSDOS_FILENAMES
#define POSIX_FILENAMES
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

