// Copyright (c) 1997 James Clark
// See the file copying.txt for copying permission.

#ifndef GroveManager_INCLUDED
#define GroveManager_INCLUDED 1

#include <OpenSP/Boolean.h>
#include "Node.h"
#include <OpenSP/StringC.h>
#include <OpenSP/Vector.h>
#include "FOTBuilder.h"
#include <OpenSP/Boolean.h>
#include "dsssl_ns.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

class STYLE_API GroveManager {
public:
  virtual ~GroveManager();
  virtual bool load(const StringC &, const Vector<StringC> &active, const NodePtr &parent,
		    NodePtr &, const Vector<StringC> &architecture) = 0;
  virtual bool readEntity(const StringC &, StringC &) = 0;
  virtual void mapSysid(StringC &) = 0;
};

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* not GroveManager_INCLUDED */
