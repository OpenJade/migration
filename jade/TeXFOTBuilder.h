// This is a -*-c++-*- header.
// TeXFOTBuilder.h: a generic TeX backend for Jade.
// Written by David Megginson <dmeggins@microstar.com>

#ifndef TeXFOTBuilder_INCLUDED
#define TeXFOTBuilder_INCLUDED 1

#include <iostream.h>
#include "FOTBuilder.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

FOTBuilder *makeTeXFOTBuilder(streambuf *);

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* not TeXFOTBuilder_INCLUDED */
