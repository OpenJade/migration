// Copyright (c) 1999 Matthias Clasen
// See the file copying.txt for copying permission.

#ifndef DocumentGenerator_INCLUDED
#define DocumentGenerator_INCLUDED 1

#include "config.h"
#include "Node.h"
#include "OutputCharStream.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

#ifdef GROVE_NAMESPACE
using namespace GROVE_NAMESPACE;
#endif

class DocumentGenerator {
public:
  DocumentGenerator(SdataMapper *, OutputCharStream *);
  void emit(const NodePtr &);
  void emit(const NodeListPtr &, const GroveString &);
private:
  OutputCharStream &os() { return *os_; }
  Owner<OutputCharStream> os_;
  SdataMapper *smap_;
};

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* not DocumentGenerator_INCLUDED */

