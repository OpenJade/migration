// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#include "stylelib.h"
#include "ProcessingMode.h"
#include "Interpreter.h"
#include "MessageArg.h"
#include "InterpreterMessages.h"
#include "Expression.h"
#include "Insn.h"
#include "Insn2.h"
#include "macros.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

ProcessingMode::ProcessingMode(const StringC &name, const ProcessingMode *initial)
: Named(name), initial_(initial)
{
}

ProcessingMode::ElementRules::ElementRules(const StringC &name)
: Named(name)
{
}

void ProcessingMode::addElement(Vector<StringC> &qgi, Owner<Expression> &expr,
				const Location &loc, Interpreter &interp)
{
  if (interp.currentPartIndex() >= parts_.size())
    parts_.resize(interp.currentPartIndex() + 1);
  ElementRules::add(parts_[interp.currentPartIndex()].elementTable, qgi.begin(), qgi.size(),
		    expr, loc, interp);
}

void ProcessingMode::addDefault(Owner<Expression> &expr,
				const Location &loc, Interpreter &interp)
{
  unsigned pi = interp.currentPartIndex();
  if (pi >= parts_.size())
    parts_.resize(pi + 1);
  else if (parts_[pi].defaultRule) {
    interp.setNextLocation(loc);
    interp.message(InterpreterMessages::duplicateDefaultRule,
		   parts_[pi].defaultRule->location());
    return;
  }
  parts_[pi].defaultRule = new Rule(expr, loc);
}

void ProcessingMode::addId(const StringC &id, Owner<Expression> &expr,
			   const Location &loc, Interpreter &interp)
{
  unsigned pi = interp.currentPartIndex();
  if (pi >= parts_.size())
    parts_.resize(pi + 1);
  else {
    const Rule *r = parts_[pi].idTable.lookup(id);
    if (r) {
      interp.setNextLocation(loc);
      interp.message(InterpreterMessages::duplicateIdRule,
		     StringMessageArg(id),
		     parts_[pi].defaultRule->location());
      return;
    }
  }
  parts_[pi].idTable.insert(new IdRule(id, expr, loc));
}

void ProcessingMode::addRoot(Owner<Expression> &expr,
			     const Location &loc, Interpreter &interp)
{
  unsigned pi = interp.currentPartIndex();
  if (pi >= parts_.size())
    parts_.resize(pi + 1);
  else if (parts_[pi].rootRule) {
    interp.setNextLocation(loc);
    interp.message(InterpreterMessages::duplicateRootRule,
		   parts_[pi].rootRule->location());
    return;
  }
  parts_[pi].rootRule = new Rule(expr, loc);
}

// Specificity gives specificity of last match; get specificity of current match.

bool ProcessingMode::findMatch(const NodePtr &node, Specificity &specificity,
			       InsnPtr &insn, SosofoObj *&sosofoObj) const
{
  GroveString gi;
  if (node->getGi(gi) == accessOK) {
    for (; specificity.part_ < parts_.size(); specificity.part_++) {
      const Part &part = parts_[specificity.part_];
      switch (specificity.ruleType_) {
      case Specificity::noRule:
      case Specificity::queryRule:
	// try the id rule
	if (part.idTable.count() > 0) {
	  GroveString str;
	  if (node->getId(str) == accessOK) {
	    StringC tem(str.data(), str.size());
	    IdRule *r = part.idTable.lookup(tem);
	    if (r) {
	      r->get(insn, sosofoObj);
	      specificity.ruleType_ = Specificity::idRule;
	      return 1;
	    }
	  }
	}
	// fall through
      case Specificity::idRule:
	// try the element rule
	specificity.nQual_ = unsigned(-1);
	// fall through
      case Specificity::elementRule:
	if (specificity.nQual_ > 0) {
	  StringC tem(gi.data(), gi.size());
	  ElementRules *er = part.elementTable.lookup(tem);
	  if (er) {
	    const Rule *r = er->findMatch(node, specificity.nQual_);
	    if (r) {
	      r->get(insn, sosofoObj);
	      specificity.ruleType_ = Specificity::elementRule;
	      return 1;
	    }
	  }
	}
	if (part.defaultRule) {
	  part.defaultRule->get(insn, sosofoObj);
	  specificity.ruleType_ = Specificity::defaultRule;
	  return 1;
	}
	// fall through
      case Specificity::defaultRule:
      case Specificity::rootRule:
	break;
      }
      specificity.ruleType_ = Specificity::noRule;
    }	
  }
  else {
    NodePtr nd;
    if (node->getDocumentElement(nd) == accessOK) {
      for (; specificity.part_ < parts_.size(); specificity.part_++) {
	if (specificity.ruleType_ == Specificity::rootRule)
	  specificity.ruleType_ = Specificity::noRule;
	else {
	  const Part &part = parts_[specificity.part_];
	  if (part.rootRule) {
	    part.rootRule->get(insn, sosofoObj);
	    specificity.ruleType_ = Specificity::rootRule;
	    return 1;
	  }
	}
      }
    }
  }
  if (initial_)
    return initial_->findMatch(node, specificity, insn, sosofoObj);
  return 0;
}

void ProcessingMode::compile(Interpreter &interp)
{
  for (size_t i = 0; i < parts_.size(); i++) {
    Part &part = parts_[i];
    if (part.defaultRule)
      part.defaultRule->compile(interp);
    if (part.rootRule)
      part.rootRule->compile(interp);
    NamedTableIter<IdRule> idIter(part.idTable);
    for (;;) {
      IdRule *r = idIter.next();
      if (!r)
	break;
      r->compile(interp);
    }
    ProcessingMode::ElementRules::compile(part.elementTable, interp);
  }
}

void ProcessingMode::ElementRules::addRule(const StringC *parents, size_t nParents,
					   Owner<Expression> &expr, const Location &loc,
					   Interpreter &interp)
{
  if (nParents == 0) {
    if (unqual_) {
      interp.setNextLocation(loc);
      interp.message(InterpreterMessages::duplicateElementRule,
		     unqual_->location());
    }
    else
      unqual_ = new Rule(expr, loc);
  }
  else
    add(parent_, parents, nParents, expr, loc, interp);
}

void ProcessingMode::ElementRules::add(NamedTable<ElementRules> &table,
				       const StringC *gis, size_t nGis,
				       Owner<Expression> &expr, const Location &loc,
				       Interpreter &interp)
{
  ASSERT(nGis > 0);
  ElementRules *er = table.lookup(gis[nGis - 1]);
  if (!er) {
    er = new ElementRules(gis[nGis - 1]);
    table.insert(er);
  }
  er->addRule(gis, nGis - 1, expr, loc, interp);
}

void ProcessingMode::ElementRules::compile(NamedTable<ElementRules> &table, Interpreter &interp)
{
  NamedTableIter<ElementRules> iter(table);
  for (;;) {
    ElementRules *er = iter.next();
    if (!er)
      break;
    if (er->unqual_)
      er->unqual_->compile(interp);
    if (er->parent_.count() > 0)
      compile(er->parent_, interp);
  }
}

const ProcessingMode::Rule *
ProcessingMode::ElementRules::findMatch(const NodePtr &node, unsigned &nQual) const
{
  ASSERT(nQual > 0);
  // nQual is number of qualifiers in previous match
  // we must have fewer than that number
  if (nQual > 1 && parent_.count() > 0) {
    NodePtr parent;
    if (node->getParent(parent) == accessOK) {
      GroveString gi;
      if (parent->getGi(gi) == accessOK) {
	StringC tem(gi.data(), gi.size());
	ElementRules *er = parent_.lookup(tem);
        if (er) {
	  unsigned nPQual = nQual - 1;
	  const Rule *r = er->findMatch(parent, nPQual);
	  if (r) {
	    nQual = nPQual + 1;
	    return r;
	  }
	}
      }
    }
  }
  if (!unqual_)
    return 0;
  nQual = 0;
  return unqual_.pointer();
}

ProcessingMode::Rule::Rule(Owner<Expression> &expr, const Location &loc)
: defLoc_(loc), obj_(0)
{
  expr.swap(expr_);
}

void ProcessingMode::Rule::compile(Interpreter &interp)
{
  expr_->optimize(interp, Environment(), expr_);
  ELObj *tem = expr_->constantValue();
  if (tem) {
    obj_ = tem->asSosofo();
    if (obj_)
      return;
  }
  insn_ = expr_->compile(interp, Environment(), 0,
			 new CheckSosofoInsn(defLoc_, InsnPtr()));
}

const Location &ProcessingMode::Rule::location() const
{
  return defLoc_;
}

ProcessingMode::IdRule::IdRule(const StringC &id, Owner<Expression> &expr, const Location &loc)
: Named(id), ProcessingMode::Rule(expr, loc)
{
}

#ifdef DSSSL_NAMESPACE
}
#endif

