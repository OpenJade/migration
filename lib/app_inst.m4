// Copyright (c) 1994 James Clark
// See the file COPYING for copying permission.

#include "splib.h"

#ifdef SP_MANUAL_INST

#define SP_DEFINE_TEMPLATES
#include <vector>
#include "Owner.h"
#include "Options.h"
#undef SP_DEFINE_TEMPLATES

#include <stddef.h>
#include "CodingSystem.h"
#include "CmdLineApp.h"
#include "Event.h"

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

__instantiate(std::vector<const CmdLineApp::AppChar *>)
__instantiate(Owner<Encoder>)
__instantiate(Options<CmdLineApp::AppChar>)
__instantiate(Owner<EventHandler>)
__instantiate(std::vector<MessageFragment>)
__instantiate(std::vector<MessageType1>)
__instantiate(LongOption<CmdLineApp::AppChar>)
__instantiate(std::vector<LongOption<CmdLineApp::AppChar> >)
__instantiate(Owner<OutputCharStream>)

#ifdef SP_NAMESPACE
}
#endif

#endif /* SP_MANUAL_INST */
