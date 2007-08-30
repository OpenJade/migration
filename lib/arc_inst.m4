// Copyright (c) 1996 James Clark
// See the file COPYING for copying permission.

#include "splib.h"

#ifdef SP_MANUAL_INST

#define SP_DEFINE_TEMPLATES
#include <vector>
#include "Owner.h"
#undef SP_DEFINE_TEMPLATES

#include "ArcProcessor.h"

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

__instantiate(std::vector<ArcProcessor>)
__instantiate(Owner<ArcProcessor::MetaMapCache>)
__instantiate(std::vector<Owner<ArcProcessor::MetaMapCache> >)

#ifdef SP_NAMESPACE
}
#endif

#endif /* SP_MANUAL_INST */
