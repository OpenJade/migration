// Copyright (c) 1997 James Clark
// See the file COPYING for copying permission.

#ifndef LocNode_INCLUDED
#define LocNode_INCLUDED 1

#include "Boolean.h"
#include "Node.h"

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

class Location;

#ifdef GROVE_NAMESPACE
using namespace GROVE_NAMESPACE;
#endif

class GROVE_API LocNode {
public:
  virtual AccessResult getLocation(Location &) const = 0;
  static const Node::IID iid;
  static const LocNode *convert(const NodePtr &nd) {
    const void *p;
    if (nd && nd->queryInterface(iid, p))
      return (const LocNode *)p;
    else
      return 0;
  }
};

#ifdef SP_NAMESPACE
}
#endif

#endif /* not LocNode_INCLUDED */
