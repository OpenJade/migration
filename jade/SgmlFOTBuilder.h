// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#ifndef SgmlFOTBuilder_INCLUDED
#define SgmlFOTBuilder_INCLUDED 1

#include "FOTBuilder.h"
#include <OpenSP/OutputCharStream.h>

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

FOTBuilder *makeSgmlFOTBuilder(OutputCharStream *,
	                       const FOTBuilder::Description *&);

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* not SgmlFOTBuilder_INCLUDED */
