// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#ifndef StyleEngine_INCLUDED
#define StyleEngine_INCLUDED 1

#include "Boolean.h"
#include "Node.h"
#include "Message.h"
#include "SgmlParser.h"
#include "FOTBuilder.h"
#include "dsssl_ns.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

class Interpreter;

class STYLE_API StyleEngine {
public:
  StyleEngine(Messenger &, const NodePtr &root,
	      int unitsPerInch, const FOTBuilder::Extension * = 0);
  void defineVariable(const StringC &);
  void parseSpec(SgmlParser &specParser,
		 const CharsetInfo &charset,
		 const StringC &id,
		 Messenger &mgr);
  void process(FOTBuilder &);
  ~StyleEngine();
private:
  StyleEngine(const StyleEngine &);  // undefined
  void operator=(const StyleEngine &); // undefined

  Interpreter *interpreter_;
};

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* not StyleEngine_INCLUDED */
