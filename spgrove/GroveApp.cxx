// Copyright (c) 1996 James Clark
// See the file COPYING for copying permission.

#ifdef __GNUG__
#pragma implementation
#endif

#include "config.h"
#include "GroveApp.h"
#include "GroveBuilder.h"
#include "threads.h"

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

static
Thread::ReturnType THREAD_CALLING_CONVENTION generateEventsThread(void *args)
{
  return (*(GroveApp::GenerateEventArgs *)args)();
}

int GroveApp::generateEvents(ErrorCountEventHandler *eceh)
{
#if 0
  int ret = ParserApp::generateEvents(eceh);
  processGrove();
  return ret;
#else
  GenerateEventArgs args(eceh, this);
  Thread thread(generateEventsThread, &args);
  processGrove();
  rootNode_.clear();
  return thread.wait();
#endif
}

ErrorCountEventHandler *GroveApp::makeEventHandler()
{
  return GroveBuilder::make(this, rootNode_);
}

void GroveApp::dispatchMessage(const Message &message)
{
  static Mutex mutex;
  Mutex::Lock lock(&mutex);
  MessageReporter::dispatchMessage(message);
}

#ifdef SP_NAMESPACE
}
#endif
