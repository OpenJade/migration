// Copyright (c) 1997 James Clark
// See the file COPYING for copying permission.

#ifndef SdNode_INCLUDED
#define SdNode_INCLUDED 1

#include "Boolean.h"
#include "Node.h"
#include "Sd.h"
#include "Syntax.h"
#include "GroveBuilder.h"

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

#ifdef GROVE_NAMESPACE
using namespace GROVE_NAMESPACE;
#endif

class SPGROVE_API SdNode {
public:
  virtual AccessResult getSd(ConstPtr<Sd> &sd,
			     ConstPtr<Syntax> &prologSyntax,
			     ConstPtr<Syntax> &instanceSyntax) const = 0;
  static const Node::IID iid;
  static const SdNode *convert(const NodePtr &nd) {
    const void *p;
    if (nd && nd->queryInterface(iid, p))
      return (const SdNode *)p;
    else
      return 0;
  }
};

#ifdef SP_NAMESPACE
}
#endif

#endif /* not SdNode_INCLUDED */
