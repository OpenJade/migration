// Jade MIF backend
// Copyright (c) 1997,1998 Kathleen Marszalek
// Version 1.0a
// Date: March 27th, 1998

#ifndef MifFOTBuilder_INCLUDED
#define MifFOTBuilder_INCLUDED 1

#include "FOTBuilder.h"
#include <OpenSP/Ptr.h>
#include <OpenSP/ExtendEntityManager.h>
#include <OpenSP/CharsetInfo.h>
#include <OpenSP/CmdLineApp.h>

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

FOTBuilder *
makeMifFOTBuilder(const String<CmdLineApp::AppChar> &fileLoc,
		  const Ptr<ExtendEntityManager> &,
		  const CharsetInfo &,
		  CmdLineApp *app,
		  const FOTBuilder::Description *&descr);

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* not MifFOTBuilder_INCLUDED */
