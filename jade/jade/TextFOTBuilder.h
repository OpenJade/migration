// Copyright (c) 1999 Avi Kivity

#ifndef TextFOTBuilder__H_INCLUDED
#define TextFOTBuilder__H_INCLUDED

#include "OutputByteStream.h"
#include "FOTBuilder.h"
#include "Message.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

FOTBuilder *makeTextFOTBuilder(OutputByteStream *, Messenger *,
			      const FOTBuilder::Description *&);

#ifdef DSSSL_NAMESPACE
}
#endif


#endif
