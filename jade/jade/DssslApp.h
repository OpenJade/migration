// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#ifndef DssslApp_INCLUDED
#define DssslApp_INCLUDED 1

#include "GroveApp.h"
#include "FOTBuilder.h"
#include "dsssl_ns.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

class DssslApp : public GroveApp {
public:
  DssslApp(int unitsPerInch);
  virtual FOTBuilder *makeFOTBuilder(const FOTBuilder::Extension *&) = 0;
  int processSysid(const StringC &);
protected:
  void processOption(AppChar opt, const AppChar *arg);
  int init(int argc, AppChar **argv);
  int unitsPerInch_;
  StringC defaultOutputBasename_;
private:
  void processGrove();
  StringC dssslSpecSysid_; // system ID of doc
  StringC dssslSpecId_; // unique ID in doc
  // Variables to be defined as true
  Vector<StringC> defineVars_;
  SgmlParser specParser_;
};

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* not DssslApp_INCLUDED */
