#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif


#ifdef SP_NAMESPACE
}
#endif
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

#ifdef __DECCXX
#pragma define_template ConstPtr<ElementPattern>
#else
#ifdef __xlC__
#pragma define(ConstPtr<ElementPattern>)
#else
#ifdef SP_ANSI_CLASS_INST
template class ConstPtr<ElementPattern>;
#else
typedef ConstPtr<ElementPattern> Dummy_0;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Ptr<ElementPattern>
#else
#ifdef __xlC__
#pragma define(Ptr<ElementPattern>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Ptr<ElementPattern>;
#else
typedef Ptr<ElementPattern> Dummy_1;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template ConstPtr<MapNodeListObj::Context>
#else
#ifdef __xlC__
#pragma define(ConstPtr<MapNodeListObj::Context>)
#else
#ifdef SP_ANSI_CLASS_INST
template class ConstPtr<MapNodeListObj::Context>;
#else
typedef ConstPtr<MapNodeListObj::Context> Dummy_2;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Ptr<MapNodeListObj::Context>
#else
#ifdef __xlC__
#pragma define(Ptr<MapNodeListObj::Context>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Ptr<MapNodeListObj::Context>;
#else
typedef Ptr<MapNodeListObj::Context> Dummy_3;
#endif
#endif
#endif

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* SP_MANUAL_INST */
