// Copyright (c) 1999 Matthias Clasen
// See the file copying.txt for copying permission.

#include "stylelib.h"
#include "TransformationMode.h"
#include "Expression.h"
#include "Interpreter.h"
#include "InterpreterMessages.h"
#include "ELObjMessageArg.h"
#include "VM.h"
#include <stdlib.h> // for qsort

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif


TransformationMode::TransformationMode()
{
}

void TransformationMode::Action::get(InsnPtr &insn, Vector<CreateSpecObj *> &specs) const
{
  insn = insn_;
  specs = specs_;
}

const Location &TransformationMode::Action::location() const
{
  return defLoc_;
}

TransformationMode::Action &TransformationMode::Association::action()
{
  return *action_;
}

long TransformationMode::Association::priority() const
{
  return priority_;
}

const TransformationMode::Action &TransformationMode::Association::action() const
{
  return *action_;
}

unsigned TransformationMode::Action::partIndex() const
{
  return partIndex_;
}

const Location &TransformationMode::Association::location() const
{
  return action_->location();
}

extern "C" {

static int associationCompare(const void *p1, const void *p2)
{
  return (*(const TransformationMode::Association *const *)p1)->priority()
       - (*(const TransformationMode::Association *const *)p2)->priority();
};

}

void TransformationMode::compile(Interpreter &interp, const NodePtr &node)
{
  for (size_t j = 0; j < associations_.size(); j++)
    associations_[j].compile(interp, node);
  qsort(&associations_[0], associations_.size(), 
        sizeof(associations_[0]), associationCompare);
}

TransformationMode::Action::Action(unsigned partIndex, Owner<Expression> &expr,
                                   const Location &loc)
: partIndex_(partIndex), defLoc_(loc)
{
  expr.swap(expr_);
}


TransformationMode::Association::Association()
: action_(0), priority_(0), nl_(0)
{
}

TransformationMode::Association::Association(Owner<Expression> &qexpr,
	               Owner<Expression> &pexpr,
                       const Ptr<TransformationMode::Action> &action)
: action_(action), priority_(0), nl_(0)
{
  qexpr.swap(qexpr_);
  pexpr.swap(pexpr_);
}

void TransformationMode::Association::swap(Association &a)
{
  action_.swap(a.action_);
  qexpr_.swap(a.qexpr_);
  pexpr_.swap(a.pexpr_);
}

void TransformationMode::Association::compile(Interpreter &interp, const NodePtr &node)
{
  action_->compile(interp);
  qexpr_->optimize(interp, Environment(), qexpr_);
  ELObj *tem = qexpr_->constantValue();
  if (!tem) {
    InsnPtr insn = qexpr_->compile(interp, Environment(), 0, InsnPtr());
    VM vm(interp);
    EvalContext::CurrentNodeSetter cns(node, 0, vm);
    tem = vm.eval(insn.pointer());
  }
  if (tem->asNodeList()) 
    nl_ = tem->asNodeList();
  else {
    interp.setNextLocation(qexpr_->location());
    interp.message(InterpreterMessages::queryNotANodeList, 
		   ELObjMessageArg(tem, interp));
    nl_ = interp.makeEmptyNodeList();
  }

  pexpr_->optimize(interp, Environment(), pexpr_);
  tem = pexpr_->constantValue();
  if (!tem) {
    InsnPtr insn = pexpr_->compile(interp, Environment(), 0, InsnPtr());
    VM vm(interp);
    EvalContext::CurrentNodeSetter cns(node, 0, vm);
    tem = vm.eval(insn.pointer());
  }
  if (!tem->exactIntegerValue(priority_)) { 
    interp.setNextLocation(pexpr_->location());
    interp.message(InterpreterMessages::priorityNotAnExactInteger,
		   ELObjMessageArg(tem, interp));
    priority_ = 0;
  }
}

void TransformationMode::Action::compile(Interpreter &interp)
{
  expr_->optimize(interp, Environment(), expr_);
  ELObj *tem = expr_->constantValue();
  if (tem) {
    if (tem->asCreateSpec()) 
      specs_.push_back(tem->asCreateSpec());
    else if (tem->asPair())  
      // must be a list of create-specs then
      for (;;) {
        PairObj *pair = tem->asPair();
        if (pair) {
          CreateSpecObj *cspec = pair->car()->asCreateSpec();
          if (!cspec) {
	    interp.setNextLocation(expr_->location());
	    interp.message(InterpreterMessages::notACspecList, 
			   ELObjMessageArg(tem, interp));
	  }
          else 
	    specs_.push_back(cspec);
          tem = pair->cdr();
        } 
        else if (tem->isNil())
          break;
        else {
	  interp.setNextLocation(expr_->location());
	  interp.message(InterpreterMessages::notACspecList,
			 ELObjMessageArg(tem, interp));
          return;
        }
      }
    else {
      interp.setNextLocation(expr_->location());
      interp.message(InterpreterMessages::notACspecOrCspecList,
		     ELObjMessageArg(tem, interp));
      return;
    }
  }
  else {
    insn_ = expr_->compile(interp, Environment(), 0, InsnPtr());
  }
}

void TransformationMode::addAssociation(Owner<Expression> &qexpr,
                                        Owner<Expression> &texpr,
                                        Owner<Expression> &pexpr,
                                        const Location &loc,
                                        Interpreter &interp)
{
  Ptr<Action> action = new Action(interp.currentPartIndex(), texpr, loc);
  Association assoc(qexpr, pexpr, action);
  associations_.resize(associations_.size() + 1);
  associations_.back().swap(assoc);
} 


#ifdef DSSSL_NAMESPACE
}
#endif
