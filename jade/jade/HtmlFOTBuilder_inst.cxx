#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif


#ifdef SP_NAMESPACE
}
#endif
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

#ifdef __DECCXX
#pragma define_template Ptr<HtmlFOTBuilder::CharStyle>
#else
#ifdef __xlC__
#pragma define(Ptr<HtmlFOTBuilder::CharStyle>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Ptr<HtmlFOTBuilder::CharStyle>;
#else
typedef Ptr<HtmlFOTBuilder::CharStyle> Dummy_0;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template ConstPtr<HtmlFOTBuilder::CharStyle>
#else
#ifdef __xlC__
#pragma define(ConstPtr<HtmlFOTBuilder::CharStyle>)
#else
#ifdef SP_ANSI_CLASS_INST
template class ConstPtr<HtmlFOTBuilder::CharStyle>;
#else
typedef ConstPtr<HtmlFOTBuilder::CharStyle> Dummy_1;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Ptr<HtmlFOTBuilder::ParaStyle>
#else
#ifdef __xlC__
#pragma define(Ptr<HtmlFOTBuilder::ParaStyle>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Ptr<HtmlFOTBuilder::ParaStyle>;
#else
typedef Ptr<HtmlFOTBuilder::ParaStyle> Dummy_2;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template ConstPtr<HtmlFOTBuilder::ParaStyle>
#else
#ifdef __xlC__
#pragma define(ConstPtr<HtmlFOTBuilder::ParaStyle>)
#else
#ifdef SP_ANSI_CLASS_INST
template class ConstPtr<HtmlFOTBuilder::ParaStyle>;
#else
typedef ConstPtr<HtmlFOTBuilder::ParaStyle> Dummy_3;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template IList<HtmlFOTBuilder::Item>
#else
#ifdef __xlC__
#pragma define(IList<HtmlFOTBuilder::Item>)
#else
#ifdef SP_ANSI_CLASS_INST
template class IList<HtmlFOTBuilder::Item>;
#else
typedef IList<HtmlFOTBuilder::Item> Dummy_4;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template IListIter<HtmlFOTBuilder::Item>
#else
#ifdef __xlC__
#pragma define(IListIter<HtmlFOTBuilder::Item>)
#else
#ifdef SP_ANSI_CLASS_INST
template class IListIter<HtmlFOTBuilder::Item>;
#else
typedef IListIter<HtmlFOTBuilder::Item> Dummy_5;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template IList<HtmlFOTBuilder::DestInfo>
#else
#ifdef __xlC__
#pragma define(IList<HtmlFOTBuilder::DestInfo>)
#else
#ifdef SP_ANSI_CLASS_INST
template class IList<HtmlFOTBuilder::DestInfo>;
#else
typedef IList<HtmlFOTBuilder::DestInfo> Dummy_6;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template IList<HtmlFOTBuilder::FlowObjectInfo>
#else
#ifdef __xlC__
#pragma define(IList<HtmlFOTBuilder::FlowObjectInfo>)
#else
#ifdef SP_ANSI_CLASS_INST
template class IList<HtmlFOTBuilder::FlowObjectInfo>;
#else
typedef IList<HtmlFOTBuilder::FlowObjectInfo> Dummy_7;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Vector<HtmlFOTBuilder::Addressable *>
#else
#ifdef __xlC__
#pragma define(Vector<HtmlFOTBuilder::Addressable *>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Vector<HtmlFOTBuilder::Addressable *>;
#else
typedef Vector<HtmlFOTBuilder::Addressable *> Dummy_8;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Vector<Vector<size_t> >
#else
#ifdef __xlC__
#pragma define(Vector<Vector<size_t> >)
#else
#ifdef SP_ANSI_CLASS_INST
template class Vector<Vector<size_t> >;
#else
typedef Vector<Vector<size_t> > Dummy_9;
#endif
#endif
#endif

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* SP_MANUAL_INST */
