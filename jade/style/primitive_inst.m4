// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#include "stylelib.h"

#ifdef SP_MANUAL_INST

#define SP_DEFINE_TEMPLATES
#include "Ptr.h"
#undef SP_DEFINE_TEMPLATES

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

__instantiate(ConstPtr<ElementPattern>)
__instantiate(Ptr<ElementPattern>)
__instantiate(ConstPtr<MapNodeListObj::Context>)
__instantiate(Ptr<MapNodeListObj::Context>)

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* SP_MANUAL_INST */
