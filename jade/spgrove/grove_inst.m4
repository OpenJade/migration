// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

// Copyright (c) 1996 James Clark
// See the file COPYING for copying permission.

#include "config.h"

#ifdef SP_MANUAL_INST

#define SP_DEFINE_TEMPLATES
#include "PointerTable.h"
#include "Vector.h"
#undef SP_DEFINE_TEMPLATES

#include "Hash.h"
#include "StringC.h"

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

class ElementChunk;

__instantiate(`PointerTable<ElementChunk *,StringC,Hash,ElementChunk>')
__instantiate(`Vector<ElementChunk *>')

#ifdef SP_NAMESPACE
}
#endif

#endif /* SP_MANUAL_INST */
