// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

// Copyright (c) 1996 James Clark
// See the file COPYING for copying permission.

#include "config.h"

#ifdef SP_MANUAL_INST

#define SP_DEFINE_TEMPLATES
#include "Ptr.h"
#include "IList.h"
#include "IListIter.h"
#include "Vector.h"
#undef SP_DEFINE_TEMPLATES

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

__instantiate(Ptr<HtmlFOTBuilder::CharStyle>)
__instantiate(ConstPtr<HtmlFOTBuilder::CharStyle>)
__instantiate(Ptr<HtmlFOTBuilder::ParaStyle>)
__instantiate(ConstPtr<HtmlFOTBuilder::ParaStyle>)
__instantiate(IList<HtmlFOTBuilder::Item>)
__instantiate(IListIter<HtmlFOTBuilder::Item>)
__instantiate(IList<HtmlFOTBuilder::DestInfo>)
__instantiate(IList<HtmlFOTBuilder::FlowObjectInfo>)
__instantiate(Vector<HtmlFOTBuilder::Addressable *>)
__instantiate(Vector<Vector<size_t> >)

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* SP_MANUAL_INST */
