// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#ifndef RtfFOTBuilder_INCLUDED
#define RtfFOTBuilder_INCLUDED 1

#include <OpenSP/OutputByteStream.h>
#include "FOTBuilder.h"
#include <OpenSP/Ptr.h>
#include <OpenSP/ExtendEntityManager.h>
#include <OpenSP/CharsetInfo.h>
#include <OpenSP/Message.h>

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

FOTBuilder *
makeRtfFOTBuilder(OutputByteStream *,
		  const Vector<StringC> &options,
		  const Ptr<ExtendEntityManager> &,
		  const CharsetInfo &, Messenger *,
		  const FOTBuilder::Description *&);


#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* not RtfFOTBuilder_INCLUDED */
