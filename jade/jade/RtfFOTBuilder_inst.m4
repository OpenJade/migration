// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

// Copyright (c) 1996 James Clark
// See the file COPYING for copying permission.

#include "config.h"

#ifdef SP_MANUAL_INST

#define SP_DEFINE_TEMPLATES
#include "Vector.h"
#include "HashTable.h"
#undef SP_DEFINE_TEMPLATES

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

__instantiate(Vector<RtfFOTBuilder::ParaFormat>)
__instantiate(Vector<RtfFOTBuilder::PageFormat>)
__instantiate(Vector<RtfFOTBuilder::DisplayInfo>)
__instantiate(Vector<RtfFOTBuilder::Format>)
__instantiate(Vector<RtfFOTBuilder::Cell>)
__instantiate(Vector<Vector<RtfFOTBuilder::Cell> >)
__instantiate(Vector<RtfFOTBuilder::Column>)
__instantiate(Vector<NodePtr>)
__instantiate(Vector<long>)
__instantiate(Vector<RtfFOTBuilder::FontFamilyCharsets>)
__instantiate(`HashTableIter<StringC,int>')

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* SP_MANUAL_INST */
