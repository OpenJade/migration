// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#ifndef StyleEngine_INCLUDED
#define StyleEngine_INCLUDED 1

#include "Boolean.h"
#include "Node.h"
#include "Message.h"
#include "SgmlParser.h"
#include "FOTBuilder.h"
#include "GroveManager.h"
#include "dsssl_ns.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

class Interpreter;

class STYLE_API StyleEngine {
public:
  StyleEngine(Messenger &, GroveManager &,
	      int unitsPerInch, bool debugMode, bool dsssl2,
	      bool strictMode, const FOTBuilder::Description &);
  void parseSpec(SgmlParser &specParser,
		 const CharsetInfo &charset,
		 const StringC &id,
		 Messenger &mgr,
                 const Vector<StringC> &defVars);
  void process(const NodePtr &, FOTBuilder &);
  ~StyleEngine();
private:
  void defineVariable(const StringC &, StringC &);
  StyleEngine(const StyleEngine &);  // undefined
  void operator=(const StyleEngine &); // undefined

  Interpreter *interpreter_;
  Messenger *mgr_;
  GroveManager *groveManager_;
  int unitsPerInch_;
  bool debugMode_;
  bool dsssl2_;
  bool strictMode_;
  const FOTBuilder::Description *fotbDescr_;
};

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* not StyleEngine_INCLUDED */