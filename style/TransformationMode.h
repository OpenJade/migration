// Copyright (c) 1999 Matthias Clasen
// See the file copying.txt for copying permission.

#ifndef TransformationMode_INCLUDED
#define TransformationMode_INCLUDED 1

#include <OpenSP/Resource.h>
#include <OpenSP/Ptr.h>
#include <OpenSP/Vector.h>
#include <OpenSP/NCVector.h>
#include "Insn.h"
#include <OpenSP/Owner.h>
#include "ELObj.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

class Expression;

class TransformationMode {
public:

 class Action : public Resource {
  public:
    Action(unsigned partIndex, Owner<Expression> &, const Location &);
    void compile(Interpreter &);
    void get(InsnPtr &, Vector<CreateSpecObj *>&) const;
    const Location &location() const;
    unsigned partIndex() const;
  private:
    Location defLoc_;
    Owner<Expression> expr_;
    // must be permanent
    Vector<CreateSpecObj *> specs_; 
    InsnPtr insn_;
    unsigned partIndex_;
  };

  class Association {
  public:
    Association();
    Association(Owner<Expression> &,Owner<Expression> &,const Ptr<Action> &);
    const Action &action() const;
    Action &action();
    long priority() const;
    const Location &location() const;
    void compile(Interpreter &, const NodePtr &);
    void swap(Association &);
  private:
    Owner<Expression> qexpr_;
    Owner<Expression> pexpr_;
    long priority_;
    NodeListObj *nl_; 
    InsnPtr insn_;
    Ptr<Action> action_;
  };


  TransformationMode();
  void addAssociation(Owner<Expression> &, Owner<Expression> &, 
                      Owner<Expression>&, const Location &, Interpreter &);
  void compile(Interpreter &, const NodePtr &);
private:
  NCVector<Association> associations_;
  friend class TransformContext;
};


#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* not TransformationMode_INCLUDED */

