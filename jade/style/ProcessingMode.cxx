// Copyright (c) 1996, 1997 James Clark
// See the file copying.txt for copying permission.

#include "stylelib.h"
#include "ProcessingMode.h"
#include "Interpreter.h"
#include "MessageArg.h"
#include "InterpreterMessages.h"
#include "Expression.h"
#include "Insn.h"
#include "Insn2.h"
#include "IListIter.h"
#include "macros.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

ProcessingMode::ProcessingMode(const StringC &name, const ProcessingMode *initial)
: Named(name), initial_(initial)
{
}

ProcessingMode::ElementRules::ElementRules(const StringC &name)
: Named(name), unqual_(0)
{
}

void ProcessingMode::addElement(Vector<StringC> &qgi, Owner<Expression> &expr,
				const Location &loc, Interpreter &interp)
{
  if (interp.currentPartIndex() >= parts_.size())
    parts_.resize(interp.currentPartIndex() + 1);

  parts_[interp.currentPartIndex()].complexRules.insert(new ElementRule(qgi, expr, loc));
}

void ProcessingMode::ElementRule::add(GroveRules &groveRules, const NodePtr &node, Messenger &mgr) const
{
  Vector<StringC> normQGi(qGi_);
  for (size_t i = 0; i < qGi_.size(); i++)
    Interpreter::normalizeGeneralName(node, normQGi[i]);
  ElementRules::add(groveRules.elementTable, normQGi.begin(), normQGi.size(),
		    this, mgr);
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
  parts_[pi].complexRules.insert(new IdRule(id, expr, loc));
}

void ProcessingMode::IdRule::add(GroveRules &groveRules, const NodePtr &nd,
				 Messenger &mgr) const
{
  StringC normId(id_);
  Interpreter::normalizeGeneralName(nd, normId);
  const GroveIdRule *r = groveRules.idTable.lookup(normId);
  if (r) {
    mgr.setNextLocation(location());
    mgr.message(InterpreterMessages::duplicateIdRule,
	        StringMessageArg(normId),
		r->rule->location());
    return;
  }
  groveRules.idTable.insert(new GroveIdRule(normId, this));
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

bool ProcessingMode::findMatch(const NodePtr &node, Messenger &mgr,
			       Specificity &specificity,
			       InsnPtr &insn, SosofoObj *&sosofoObj) const
{
  if (initial_ && specificity.toInitial_)
    return initial_->findMatch(node, mgr, specificity, insn, sosofoObj);
  GroveString gi;
  if (node->getGi(gi) == accessOK) {
    for (; specificity.part_ < parts_.size(); specificity.part_++) {
      const Part &part = parts_[specificity.part_];
      switch (specificity.ruleType_) {
      case Specificity::noRule:
      case Specificity::queryRule:
	// try the id rule
	{
	  part.prepare(node, mgr);
	  const NamedTable<GroveIdRule> &idTable
	    = part.groveRules[node->groveIndex()].idTable;
	  if (idTable.count() > 0) {
	    GroveString str;
	    if (node->getId(str) == accessOK) {
	      StringC tem(str.data(), str.size());
	      const GroveIdRule *r = idTable.lookup(tem);
	      if (r) {
		r->rule->get(insn, sosofoObj);
		specificity.ruleType_ = Specificity::idRule;
		return 1;
	      }
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
	  part.prepare(node, mgr);
	  StringC tem(gi.data(), gi.size());
	  const NamedTable<ElementRules> &elementTable
	    = part.groveRules[node->groveIndex()].elementTable;
	  const ElementRules *er = elementTable.lookup(tem);
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
  if (initial_) {
    specificity.toInitial_ = 1;
    specificity.ruleType_ = Specificity::noRule;
    specificity.part_ = 0;
    return initial_->findMatch(node, mgr, specificity, insn, sosofoObj);
  }
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
    for (IListIter<ComplexRule> iter(part.complexRules);
         !iter.done();
	 iter.next())
      iter.cur()->compile(interp);
  }
}

void ProcessingMode::ElementRules::addRule(const StringC *parents, size_t nParents,
					   const ElementRule *rule,
					   Messenger &mgr)
{
  if (nParents == 0) {
    if (unqual_) {
      mgr.setNextLocation(rule->location());
      mgr.message(InterpreterMessages::duplicateElementRule,
		  unqual_->location());
    }
    else
      unqual_ = rule;
  }
  else
    add(parent_, parents, nParents, rule, mgr);
}

void ProcessingMode::ElementRules::add(NamedTable<ElementRules> &table,
				       const StringC *gis, size_t nGis,
				       const ElementRule *rule,
				       Messenger &mgr)
{
  ASSERT(nGis > 0);
  ElementRules *er = table.lookup(gis[nGis - 1]);
  if (!er) {
    er = new ElementRules(gis[nGis - 1]);
    table.insert(er);
  }
  er->addRule(gis, nGis - 1, rule, mgr);
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
  return unqual_;
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
: id_(id), ProcessingMode::ComplexRule(expr, loc)
{
}

void ProcessingMode::Part::prepare(const NodePtr &node, Messenger &mgr) const
{
  unsigned long n = node->groveIndex();
  ProcessingMode::Part *cache
    = (ProcessingMode::Part *)this;
  if (n >= groveRules.size()) {
    if (groveRules.size() == 0) {
      // reverse the list
      IList<ComplexRule> tem;
      tem.swap(cache->complexRules);
      while (!tem.empty())
	cache->complexRules.insert(tem.get());
    }
    cache->groveRules.resize(n + 1);
  }
  if (!groveRules[n].built) {
    cache->groveRules[n].built = 1;
    for (IListIter<ComplexRule> iter(complexRules); !iter.done(); iter.next())
      iter.cur()->add(cache->groveRules[n], node, mgr);
  }
}

ProcessingMode::GroveRules::GroveRules()
: built(0)
{
}

ProcessingMode::ComplexRule::ComplexRule(Owner<Expression> &expr, const Location &loc)
: Rule(expr, loc)
{
}

ProcessingMode::GroveIdRule::GroveIdRule(const StringC &id, const IdRule *r)
: Named(id), rule(r)
{
}

ProcessingMode::ElementRule::ElementRule(Vector<StringC> &qGi, Owner<Expression> &expr, const Location &loc)
: ComplexRule(expr, loc)
{
  qGi_.swap(qGi);
}

#ifdef DSSSL_NAMESPACE
}
#endif
