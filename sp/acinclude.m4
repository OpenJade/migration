dnl OJ_CHECK_SIZEOF(TYPE, HEADER [, CROSS-SIZE])
AC_DEFUN(OJ_CHECK_SIZEOF,
[changequote(<<, >>)dnl
dnl The name to #define.
define(<<AC_TYPE_NAME>>, translit(sizeof_$1, [a-z *], [A-Z_P]))dnl
dnl The cache variable name.
define(<<AC_CV_NAME>>, translit(ac_cv_sizeof_$1, [ *], [_p]))dnl
changequote([, ])dnl
AC_MSG_CHECKING(size of $1)
AC_CACHE_VAL(AC_CV_NAME,
[AC_TRY_RUN([#include <stdio.h>
#include <$2>
main()
{
  FILE *f=fopen("conftestval", "w");
  if (!f) exit(1);
  fprintf(f, "%d\n", sizeof($1));
  exit(0);
}], AC_CV_NAME=`cat conftestval`, AC_CV_NAME=0, ifelse([$3] , , AC_CV_NAME=$3))])dnl
AC_MSG_RESULT($AC_CV_NAME)
AC_DEFINE_UNQUOTED(AC_TYPE_NAME, $AC_CV_NAME)
undefine([AC_TYPE_NAME])dnl
undefine([AC_CV_NAME])dnl
])


dnl @synopsis AC_CXX_PLACEMENT_OPERATOR_DELETE
dnl
dnl If the compiler supports void delete(size_t,void*), define
dnl HAVE_PLACEMENT_OPERATOR_DELETE.
dnl
dnl @author Matthias Clasen
dnl
AC_DEFUN(AC_CXX_PLACEMENT_OPERATOR_DELETE,
[AC_CACHE_CHECK(whether the compiler supports placement operator delete,
ac_cv_cxx_placement_operator_delete,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <stddef.h>],
  [class Thing {
   public:
    Thing() { };
    void *operator new(size_t,bool) { };
    void operator delete(size_t,void*) { };
   };],
   ac_cv_cxx_placement_operator_delete=yes, 
   ac_cv_cxx_placement_operator_delete=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_placement_operator_delete" = yes; then
  AC_DEFINE(HAVE_PLACEMENT_OPERATOR_DELETE,,
            [define if the compiler supports placement operator delete])
fi
])


dnl @synopsis AC_CXX_BOOL
dnl
dnl If the compiler recognizes bool as a separate built-in type,
dnl define HAVE_BOOL. Note that a typedef is not a separate
dnl type since you cannot overload a function such that it accepts either
dnl the basic type or the typedef.
dnl
dnl @version $Id$
dnl @author Luc Maisonobe
dnl
AC_DEFUN(AC_CXX_BOOL,
[AC_CACHE_CHECK(whether the compiler recognizes bool as a built-in type,
ac_cv_cxx_bool,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([
int f(int  x){return 1;}
int f(char x){return 1;}
int f(bool x){return 1;}
],[bool b = true; return f(b);],
 ac_cv_cxx_bool=yes, ac_cv_cxx_bool=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_bool" = yes; then
  AC_DEFINE(HAVE_BOOL,,[define if bool is a built-in type])
fi
])


dnl @synopsis AC_CXX_NEW_FOR_SCOPING
dnl
dnl If the compiler accepts the new for scoping rules (the scope of a
dnl variable declared inside the parentheses is restricted to the
dnl for-body), define HAVE_NEW_FOR_SCOPING.
dnl
dnl @version $Id$
dnl @author Luc Maisonobe
dnl
AC_DEFUN(AC_CXX_NEW_FOR_SCOPING,
[AC_CACHE_CHECK(whether the compiler accepts the new for scoping rules,
ac_cv_cxx_new_for_scoping,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE(,[
  int z = 0;
  for (int i = 0; i < 10; ++i)
    z = z + i;
  for (int i = 0; i < 10; ++i)
    z = z - i;
  return z;],
 ac_cv_cxx_new_for_scoping=yes, ac_cv_cxx_new_for_scoping=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_new_for_scoping" = yes; then
  AC_DEFINE(HAVE_NEW_FOR_SCOPING,,[define if the compiler accepts the new for scoping rules])
fi
])


dnl @synopsis AC_CXX_DYNAMIC_CAST
dnl
dnl If the compiler supports dynamic_cast<>, define HAVE_DYNAMIC_CAST.
dnl
dnl @version $Id$
dnl @author Luc Maisonobe
dnl
AC_DEFUN(AC_CXX_DYNAMIC_CAST,
[AC_CACHE_CHECK(whether the compiler supports dynamic_cast<>,
ac_cv_cxx_dynamic_cast,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <typeinfo>
class Base { public : Base () {} virtual void f () = 0;};
class Derived : public Base { public : Derived () {} virtual void f () {} };],[
Derived d; Base& b=d; return dynamic_cast<Derived*>(&b) ? 0 : 1;],
 ac_cv_cxx_dynamic_cast=yes, ac_cv_cxx_dynamic_cast=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_dynamic_cast" = yes; then
  AC_DEFINE(HAVE_DYNAMIC_CAST,,[define if the compiler supports dynamic_cast<>])
fi
])


dnl @synopsis AC_CXX_NAMESPACES
dnl
dnl If the compiler can prevent names clashes using namespaces, define
dnl HAVE_NAMESPACES.
dnl
dnl @version $Id$
dnl @author Luc Maisonobe
dnl
AC_DEFUN(AC_CXX_NAMESPACES,
[AC_CACHE_CHECK(whether the compiler implements namespaces,
ac_cv_cxx_namespaces,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([namespace Outer { namespace Inner { int i = 0; }}],
                [using namespace Outer::Inner; return i;],
 ac_cv_cxx_namespaces=yes, ac_cv_cxx_namespaces=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_namespaces" = yes; then
  AC_DEFINE(HAVE_NAMESPACES,,[define if the compiler implements namespaces])
fi
])


dnl @synopsis ACX_CHECK_PATHNAME_STYLE_DOS
dnl
dnl Check if host OS uses DOS-style pathnames. This includes the use
dnl of drive letters and backslashes. Under DOS, Windows, and OS/2,
dnl defines HAVE_PATHNAME_STYLE_DOS and PATH_SEPARATOR to ';'.
dnl Otherwise, defines PATH_SEPARATOR to ':'.
dnl
dnl Use for enabling code to handle drive letters, backslashes in
dnl filenames and semicolons in the PATH.
dnl
dnl @version $Id$
dnl @author Mark Elbrecht <snowball3@bigfoot.com>
dnl
AC_DEFUN(ACX_CHECK_PATHNAME_STYLE_DOS,
[AC_MSG_CHECKING(for Windows and DOS and OS/2 style pathnames)
AC_CACHE_VAL(acx_cv_pathname_style_dos,
[AC_REQUIRE([AC_CANONICAL_HOST])

acx_cv_pathname_style_dos="no"
case ${host_os} in
  *djgpp | *mingw32 | *emx*) acx_cv_pathname_style_dos="yes" ;;
esac
])
AC_MSG_RESULT($acx_cv_pathname_style_dos)
if test $acx_cv_pathname_style_dos = "yes"; then
  AC_DEFINE(HAVE_PATHNAME_STYLE_DOS)
  AC_DEFINE(PATH_SEPARATOR, ';')
else
  AC_DEFINE(PATH_SEPARATOR, ':')
fi
])


dnl @synopsis AC_CXX_EXPLICIT_INSTANTIATIONS
dnl
dnl If the C++ compiler supports explicit instanciations syntax,
dnl define HAVE_INSTANTIATIONS.
dnl
dnl @version $Id$
dnl @author Luc Maisonobe
dnl
AC_DEFUN(AC_CXX_EXPLICIT_INSTANTIATIONS,
[AC_CACHE_CHECK(whether the compiler supports explicit instantiations,
ac_cv_cxx_explinst,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([template <class T> class A { T t; }; template class A<int>;],
 [], ac_cv_cxx_explinst=yes, ac_cv_cxx_explinst=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_explinst" = yes; then
  AC_DEFINE(HAVE_INSTANTIATIONS,,
            [define if the compiler supports explicit instantiations])
fi
])
AC_PREREQ(2.5)

AC_DEFUN(md_TYPE_PTRDIFF_T,
  [AC_CACHE_CHECK([for ptrdiff_t], ac_cv_type_ptrdiff_t,
     [AC_TRY_COMPILE(stddef.h, [ptrdiff_t p], ac_cv_type_ptrdiff_t=yes,
		     ac_cv_type_ptrdiff_t=no)])
   if test $ac_cv_type_ptrdiff_t = yes; then
     AC_DEFINE(HAVE_PTRDIFF_T)
   fi
])

AC_DEFUN(md_PATH_PROG,
  [AC_PATH_PROG($1,$2,$3)dnl
   if echo $$1 | grep openwin > /dev/null; then
     echo "WARNING: Do not use OpenWin's $2.  (Better remove it.) >&AC_FD_MSG"
     ac_cv_path_$1=$2
     $1=$2
   fi
])

dnl ------------------------
dnl Emacs LISP file handling
dnl ------------------------
AC_DEFUN(ud_PATH_LISPDIR,
 [AC_PATH_PROG(EMACS, emacs, no)
  if test $EMACS != "no"; then
    AC_MSG_CHECKING([where .elc files should go])
    dnl Set default value
    LISPDIR="\$(datadir)/emacs/site-lisp"
    if test "x$prefix" = "xNONE"; then
      if test -d $ac_default_prefix/share/emacs/site-lisp; then
	LISPDIR="\$(prefix)/share/emacs/site-lisp"
      else
	if test -d $ac_default_prefix/lib/emacs/site-lisp; then
	  LISPDIR="\$(prefix)/lib/emacs/site-lisp"
	fi
      fi
    else
      if test -d $prefix/share/emacs/site-lisp; then
	LISPDIR="\$(prefix)/share/emacs/site-lisp"
      else
	if test -d $prefix/lib/emacs/site-lisp; then
	  LISPDIR="\$(prefix)/lib/emacs/site-lisp"
	fi
      fi
    fi
    AC_MSG_RESULT($LISPDIR)
    ELCFILES="\$(ELCFILES)"
  fi
  AC_SUBST(LISPDIR)
  AC_SUBST(ELCFILES)])

dnl --------------------------------------------------------- ##
dnl Use AC_PROG_INSTALL, supplementing it with INSTALL_SCRIPT ##
dnl substitution.                                             ##
dnl --------------------------------------------------------- ##

AC_DEFUN(fp_PROG_INSTALL,
[AC_PROG_INSTALL
test -z "$INSTALL_SCRIPT" && INSTALL_SCRIPT='${INSTALL} -m 755'
AC_SUBST(INSTALL_SCRIPT)dnl
])

dnl Check NLS options

AC_DEFUN(ud_LC_MESSAGES,
  [if test $ac_cv_header_locale_h = yes; then
    AC_CACHE_CHECK([for LC_MESSAGES], ud_cv_val_LC_MESSAGES,
      [AC_TRY_LINK([#include <locale.h>], [return LC_MESSAGES],
       ud_cv_val_LC_MESSAGES=yes, ud_cv_val_LC_MESSAGES=no)])
    if test $ud_cv_val_LC_MESSAGES = yes; then
      AC_DEFINE(HAVE_LC_MESSAGES)
    fi
  fi])

AC_DEFUN(ud_WITH_NLS,
  [AC_MSG_CHECKING([whether NLS is requested])
    dnl Default is enabled NLS
    AC_ARG_ENABLE(nls,
      [  --disable-nls           do not use Native Language Support],
      nls_cv_use_nls=$enableval, nls_cv_use_nls=yes)
    AC_MSG_RESULT($nls_cv_use_nls)

    dnl If we use NLS figure out what method
    if test "$nls_cv_use_nls" = "yes"; then
      AC_DEFINE(ENABLE_NLS)
      AC_MSG_CHECKING([for explicitly using GNU gettext])
      AC_ARG_WITH(gnu-gettext,
        [  --with-gnu-gettext      use the GNU gettext library],
        nls_cv_force_use_gnu_gettext=$withval,
        nls_cv_force_use_gnu_gettext=no)
      AC_MSG_RESULT($nls_cv_force_use_gnu_gettext)

      if test "$nls_cv_force_use_gnu_gettext" = "yes"; then
        nls_cv_use_gnu_gettext=yes
      else
        dnl User does not insist on using GNU NLS library.  Figure out what
        dnl to use.  If gettext or catgets are available (in this order) we
        dnl use this.  Else we have to fall back to GNU NLS library.
        AC_CHECK_LIB(intl, main)
        AC_CHECK_LIB(i, main)
        CATOBJEXT=NONE
        AC_CHECK_FUNC(gettext,
          [AC_DEFINE(HAVE_GETTEXT)
           md_PATH_PROG(MSGFMT, msgfmt, no)dnl
	   if test "$MSGFMT" != "no"; then
	     AC_CHECK_FUNCS(dcgettext)
	     md_PATH_PROG(GMSGFMT, gmsgfmt, $MSGFMT)
	     md_PATH_PROG(XGETTEXT, xgettext, xgettext)
             CATOBJEXT=.mo
	     INSTOBJEXT=.mo
	     DATADIRNAME=lib
	   fi])

        if test "$CATOBJEXT" = "NONE"; then
          dnl No gettext in C library.  Try catgets next.
          AC_CHECK_FUNC(catgets,
            [AC_DEFINE(HAVE_CATGETS)
             INTLOBJS="\$(CATOBJS)"
             AC_PATH_PROG(GENCAT, gencat, no)dnl
	     if test "$GENCAT" != "no"; then
	       AC_PATH_PROGS(GMSGFMT, [gmsgfmt msgfmt], msgfmt)
	       md_PATH_PROG(XGETTEXT, xgettext, xgettext)
               CATOBJEXT=.cat
	       INSTOBJEXT=.cat
	       DATADIRNAME=lib
	       INTLDEPS="../intl/libintl.a"
	       INTLLIBS=$INTLDEPS
	       LIBS=`echo $LIBS | sed -e 's/-lintl//'`
	       nls_cv_header_intl=intl/libintl.h
	       nls_cv_header_libgt=intl/libgettext.h
	     fi])
        fi

        if test "$CATOBJEXT" = "NONE"; then
	  dnl Neither gettext nor catgets in included in the C library.
	  dnl Fall back on GNU gettext library.
	  nls_cv_use_gnu_gettext=yes
        fi
      fi

      if test "$nls_cv_use_gnu_gettext" = "yes"; then
        dnl Mark actions used to generate GNU NLS library.
        INTLOBJS="\$(GETTOBJS)"
        md_PATH_PROG(MSGFMT, msgfmt, msgfmt)
        md_PATH_PROG(GMSGFMT, gmsgfmt, $MSGFMT)
        md_PATH_PROG(XGETTEXT, xgettext, xgettext)
        AC_SUBST(MSGFMT)
        CATOBJEXT=.gmo
        INSTOBJEXT=.mo
        DATADIRNAME=share
        INTLDEPS="../intl/libintl.a"
        INTLLIBS=$INTLDEPS
	LIBS=`echo $LIBS | sed -e 's/-lintl//'`
        nls_cv_header_intl=intl/libintl.h
        nls_cv_header_libgt=intl/libgettext.h
      fi

      # We need to process the intl/ and po/ directory.
      INTLSUB=intl
      POSUB=po
    else
      DATADIRNAME=share
      nls_cv_header_intl=intl/libintl.h
      nls_cv_header_libgt=intl/libgettext.h
    fi

    dnl These rules are solely for the distribution goal.  While doing this
    dnl we only have to keep exactly one list of the available catalogs
    dnl in configure.in.
    for lang in $ALL_LINGUAS; do
      GMOFILES="$GMOFILES $lang.gmo"
      POFILES="$POFILES $lang.po"
    done

    dnl Make all variables we use known to autoconf.
    AC_SUBST(CATALOGS)
    AC_SUBST(CATOBJEXT)
    AC_SUBST(DATADIRNAME)
    AC_SUBST(GMOFILES)
    AC_SUBST(INSTOBJEXT)
    AC_SUBST(INTLDEPS)
    AC_SUBST(INTLLIBS)
    AC_SUBST(INTLOBJS)
    AC_SUBST(INTLSUB)
    AC_SUBST(POFILES)
    AC_SUBST(POSUB)
  ])

AC_DEFUN(udx_GNU_GETTEXT,
  [AC_REQUIRE([AC_PROG_MAKE_SET])dnl
   AC_REQUIRE([AC_PROG_CC])dnl
   AC_REQUIRE([AC_PROG_RANLIB])dnl
   AC_REQUIRE([AC_HEADER_STDC])dnl
   AC_REQUIRE([AC_C_CONST])dnl
   AC_REQUIRE([AC_C_INLINE])dnl
   AC_REQUIRE([AC_TYPE_OFF_T])dnl
   AC_REQUIRE([AC_TYPE_SIZE_T])dnl
   AC_REQUIRE([AC_FUNC_ALLOCA])dnl
   AC_REQUIRE([AC_FUNC_MMAP])dnl

   AC_CHECK_HEADERS([limits.h locale.h nl_types.h malloc.h string.h unistd.h values.h])
   AC_CHECK_FUNCS([getcwd munmap putenv setenv setlocale strchr strcasecmp])

   if test "${ac_cv_func_stpcpy+set}" != "set"; then
     AC_CHECK_FUNCS(stpcpy)
   fi
   if test "${ac_cv_func_stpcpy}" = "yes"; then
     AC_DEFINE(HAVE_STPCPY)
   fi

   ud_LC_MESSAGES
   ud_WITH_NLS

   if test "x$CATOBJEXT" != "x"; then
     if test "x$ALL_LINGUAS" = "x"; then
       LINGUAS=
     else
       AC_MSG_CHECKING(for catalogs to be installed)
       NEW_LINGUAS=
       for lang in ${LINGUAS=$ALL_LINGUAS}; do
         case "$ALL_LINGUAS" in
          *$lang*) NEW_LINGUAS="$NEW_LINGUAS $lang" ;;
         esac
       done
       LINGUAS=$NEW_LINGUAS
       AC_MSG_RESULT($LINGUAS)
     fi

     dnl Construct list of names of catalog files to be constructed.
     if test -n "$LINGUAS"; then
       for lang in $LINGUAS; do CATALOGS="$CATALOGS $lang$CATOBJEXT"; done
     fi
   fi

   dnl Determine which catalog format we have (if any is needed)
   dnl For now we know about two different formats:
   dnl   Linux and the normal X/Open format
   test -d intl || mkdir intl
   if test "$CATOBJEXT" = ".cat"; then
     AC_CHECK_HEADER(linux/version.h, msgformat=linux, msgformat=xopen)

     dnl Transform the SED scripts while copying because some dumb SEDs
     dnl cannot handle comments.
     sed -e '/^#/d' $srcdir/intl/$msgformat-msg.sed > intl/po2msg.sed
   fi
   dnl po2tbl.sed is always needed.
   sed -e '/^#.*[^\\]$/d' -e '/^#$/d' \
     $srcdir/intl/po2tbl.sed.in > intl/po2tbl.sed

   dnl Generate list of files to be processed by xgettext which will
   dnl be included in po/Makefile.
   test -d po || mkdir po
   if test "x$srcdir" != "x."; then
     if test "x`echo $srcdir | sed 's@/.*@@'`" = "x"; then
       posrcprefix="$srcdir/"
     else
       posrcprefix="../$srcdir/"
     fi
   else
     posrcprefix="../"
   fi
   sed -e "/^#/d" -e "/^\$/d" -e "s,.*,	$posrcprefix& \\\\," -e "\$s/\(.*\) \\\\/\1/" \
	< $srcdir/po/POTFILES.in > po/POTFILES
  ])

