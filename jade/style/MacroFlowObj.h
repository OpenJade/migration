// Copyright (c) 1997 James Clark
// See the file copying.txt for copying permission.

#ifndef MacroFlowObj_INCLUDED
#define MacroFlowObj_INCLUDED 1

#include "SosofoObj.h"
#include "Insn.h"
#include <OpenSP/Resource.h>
#include <OpenSP/Ptr.h>
#include <OpenSP/Vector.h>
#include <OpenSP/NCVector.h>
#include <OpenSP/Owner.h>
#include "Expression.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

class ProcessContext;
class Identifier;
class Interpreter;

class MacroFlowObj : public CompoundFlowObj {
public:
  void *operator new(size_t, Collector &c) {
    return c.allocateObject(1);
  }
  class Definition : public Resource {
  public:
    Definition(Vector<const Identifier *> &,
	       NCVector<Owner<Expression> > &,
	       const Identifier *contentsId,
	       Owner<Expression> &);

    void process(ProcessContext &, MacroFlowObj *);
    bool isCompound() const;
    const Vector<const Identifier *> &nics() const;
  private:
    Definition(const Definition &); // undefined
    void operator=(const Definition &); // undefined
    void compile(Interpreter &);

    Vector<const Identifier *> charics_;
    NCVector<Owner<Expression> > charicInits_;
    const Identifier *contentsId_;
    Owner<Expression> body_;
    InsnPtr code_;
  };

  MacroFlowObj(Vector<const Identifier *> &,
	       NCVector<Owner<Expression> > &,
	       const Identifier *contentsId,
	       Owner<Expression> &);
  MacroFlowObj(const MacroFlowObj &);
  ~MacroFlowObj();
  FlowObj *copy(Collector &) const;
  CompoundFlowObj *asCompoundFlowObj();
  bool hasNonInheritedC(const Identifier *) const;
  void setNonInheritedC(const Identifier *, ELObj *, const Location &, Interpreter &);
  void traceSubObjects(Collector &) const;
  void processInner(ProcessContext &);
  AcceptFlags acceptFlags(ProcessContext &) const
    { return afAlways; }
  void unpack(VM &);
private:
  void operator=(const MacroFlowObj &); // undefined
  // Only one pointer/owner allowed.
  struct S {
    S(Definition *d, ELObj **c = 0)
      : def_(d), charicVals_(c) {}
    Ptr<Definition> def_;
    ELObj **charicVals_;
  };
  Owner<S> sp_;
};

inline
bool MacroFlowObj::Definition::isCompound() const
{
  return contentsId_ != 0;
}

inline
const Vector<const Identifier *> &MacroFlowObj::Definition::nics() const
{
  return charics_;
}

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* not MacroFlowObj_INCLUDED */
