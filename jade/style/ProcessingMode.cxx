// Copyright (c) 1996, 1997 James Clark
// See the file copying.txt for copying permission.

#include "stylelib.h"
#include "ProcessingMode.h"
#include "Interpreter.h"
#include <OpenSP/MessageArg.h>
#include "InterpreterMessages.h"
#include "Expression.h"
#include "Insn.h"
#include "Insn2.h"
#include "VM.h"
#include <OpenSP/IListIter.h>
#include "LocNode.h"
#include <OpenSP/macros.h>
#include <stdlib.h>

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

ProcessingMode::ProcessingMode(const StringC &name, const ProcessingMode *initial)
: Named(name), initial_(initial), defined_(0)
{
}

void ProcessingMode::GroveRules::build(const IList<ElementRule> *lists,
				       const NodePtr &node,
				       Messenger &)
{
  built = 1;
  for (int ruleType = 0; ruleType < nRuleType; ruleType++) {
    for (IListIter<ElementRule> iter(lists[ruleType]); !iter.done(); iter.next()) {
      StringC gi;
      if (iter.cur()->mustHaveGi(gi)) {
	Interpreter::normalizeGeneralName(node, gi);
	ElementRules *p = elementTable.lookup(gi);
	if (!p) {
	  p = new ElementRules(gi);
	  elementTable.insert(p);
	}
	p->rules[ruleType].push_back(iter.cur());
      }
      else
	otherRules[ruleType].push_back(iter.cur());
    }
  }
  for (int ruleType = 0; ruleType < nRuleType; ruleType++) {
    NamedTableIter<ElementRules> iter(elementTable);
    for (;;) {
      ElementRules *p = iter.next();
      if (!p)
	break;
      size_t j = p->rules[ruleType].size();
      p->rules[ruleType].resize(p->rules[ruleType].size() + otherRules[ruleType].size());
      for (size_t i = 0; i < otherRules[ruleType].size(); i++)
	p->rules[ruleType][j++] = otherRules[ruleType][i];
      sortRules(p->rules[ruleType]);
    }
    sortRules(otherRules[ruleType]);
  }
}

extern "C" {

static
int ruleCompare(const void *p1, const void *p2)
{
  return (*(const ProcessingMode::Rule *const *)p1)
         ->compareSpecificity(**(const ProcessingMode::Rule *const *)p2);
}

}

void ProcessingMode::GroveRules::sortRules(Vector<const ElementRule *> &v)
{
  qsort(&v[0], v.size(), sizeof(v[0]), ruleCompare);
}

void ProcessingMode::compile(Interpreter &interp, const NodePtr &root)
{
  for (int i = 0; i < nRuleType; i++) {
    for (size_t j = 0; j < rootRules_[i].size(); j++)
      rootRules_[i][j].action().compile(interp, RuleType(i));
    for (size_t j = 0; j < queryRules_[i].size(); j++) {
      queryRules_[i][j]->action().compile(interp, RuleType(i));
      queryRules_[i][j]->compile(root);
    }
    qsort(&(queryRules_[i][0]), queryRules_[i].size(), 
          sizeof(queryRules_[i][0]), ruleCompare);  
    for (IListIter<ElementRule> iter(elementRules_[i]); !iter.done(); iter.next())
      iter.cur()->action().compile(interp, RuleType(i));
  }
}

ProcessingMode::Action::Action(unsigned partIndex, Owner<Expression> &expr,
			   const Location &loc)
: partIndex_(partIndex), defLoc_(loc), sosofo_(0)
{
  expr.swap(expr_);
}

ProcessingMode::Rule::Rule()
{
}

ProcessingMode::Rule::Rule(const Ptr<Action> &action)
: action_(action)
{
}

int ProcessingMode::Rule::compareSpecificity(const Rule &r) const
{
  unsigned i1 = action().partIndex();
  unsigned i2 = r.action().partIndex();
  if (i1 == i2)
    return 0;
  return i1 < i2 ? -1 : 1;
}

void ProcessingMode::Action::compile(Interpreter &interp, RuleType ruleType)
{
  expr_->optimize(interp, Environment(), expr_);
  ELObj *tem = expr_->constantValue();
  if (tem) {
    if (ruleType == constructionRule) {
      sosofo_ = tem->asSosofo();
      if (sosofo_)
	return;
    }
  }
  InsnPtr check;
  if (ruleType == constructionRule)
    check = new CheckSosofoInsn(defLoc_, check);
  insn_ = expr_->compile(interp, Environment(), 0, check);
}

ProcessingMode::ElementRule::ElementRule(const Ptr<Action> &action,
					 Pattern &pattern)
: Rule(action)
{
  pattern.swap(*this);
}

int ProcessingMode::ElementRule::compareSpecificity(const Rule &r) const
{
  int result = Rule::compareSpecificity(r);
  if (result)
    return result;
  return Pattern::compareSpecificity(*this, (const ElementRule &)r);
}

ProcessingMode::QueryRule::QueryRule(const Ptr<Action> &action,
                                     Owner<Expression> &query,
                                     Owner<Expression> &priority,
				     ProcessingMode *pm,
				     Interpreter *interp)
: Rule(action), nl_(0), priority_(0), pm_(pm), interp_(interp), 
Collector::DynamicRoot(*interp)
{
  nlExpr_.swap(query);
  priorityExpr_.swap(priority);
} 

void ProcessingMode::QueryRule::compile()
{
  if (priorityExpr_) {
    InsnPtr insn = 
       priorityExpr_->compile(*interp_, Environment(), 0, InsnPtr());
    EvalContext ec;
    VM vm(ec, *interp_);
    ELObj *val = vm.eval(insn.pointer());
    double tem;
    if (!val || !val->realValue(tem)) {
      interp_->setNextLocation(location());
      interp_->message(InterpreterMessages::priorityNotNumber);
      return 0;
    }
    if (!val->exactIntegerValue(priority_)) {
      interp_->setNextLocation(location());
      interp_->message(InterpreterMessages::sorryPriority);
      return 0;
    }
  }

  {
    EvalContext ec;
    EvalContext::CurrentNodeSetter cns(root, pm_, ec);
    InsnPtr insn = nlExpr_->compile(*interp_, Environment(), 0, InsnPtr());
    VM vm(ec, *interp_);
    ELObj *val = vm.eval(insn.pointer());
    if (!val || !val->asNodeList()) {
      interp_->setNextLocation(location());
      interp_->message(InterpreterMessages::queryNotNodelist);
      return 0;
    }
    nl_ = val->asNodeList();
    interp_->makeReadOnly(nl_);
  }
}

bool ProcessingMode::QueryRule::matches(const NodePtr &nd) const
{
  EvalContext ec;
  NodePtr tem = nl_->nodeListFirst(ec, *interp_);
  if (!tem)
    return 0;
  tem->getGroveRoot(tem);
  EvalContext::CurrentNodeSetter cns(tem, pm_, ec);
  return nl_->contains(ec, *interp_, nd);
}

int ProcessingMode::QueryRule::compareSpecificity(const Rule &r) const
{
  int result = Rule::compareSpecificity(r);
  if (result)
    return result;
  return ((const QueryRule &)r).priority_ - priority_; 
}

void
ProcessingMode::QueryRule::trace(Collector &c) const
{
  if (nl_)
    c.trace(nl_);
}

void ProcessingMode::addElementRule(NCVector<Pattern> &patterns,
			     Owner<Expression> &expr,
			     RuleType ruleType,
			     const Location &loc,
			     Interpreter &interp)
{
  Ptr<Action> action = new Action(interp.currentPartIndex(), expr, loc);
  for (size_t i = 0; i < patterns.size(); i++)
    elementRules_[ruleType].insert(new ElementRule(action, patterns[i]));
}

void ProcessingMode::addRootRule(Owner<Expression> &expr,
			     RuleType ruleType,
			     const Location &loc,
			     Interpreter &interp)
{
  Ptr<Action> action = new Action(interp.currentPartIndex(), expr, loc);
  Vector<Rule> &rules = rootRules_[ruleType];
  rules.push_back(Rule(action));
  for (size_t i = rules.size() - 1; i > 0; i--) {
    int cmp = rules[i - 1].compareSpecificity(rules[i]);
    if (cmp <= 0) {
      if (cmp == 0 && ruleType == constructionRule) {
	interp.setNextLocation(loc);
	interp.message(InterpreterMessages::duplicateRootRule,
		       rules[i - 1].location());
      }
      break;
    }
    rules[i - 1].swap(rules[i]);
  }
}

void ProcessingMode::addQueryRule(Owner<Expression> &query,
                             Owner<Expression> &expr,
                             Owner<Expression> &priority,
			     RuleType ruleType,
			     const Location &loc,
			     Interpreter &interp)
{
  Ptr<Action> action = new Action(interp.currentPartIndex(), expr, loc);
  queryRules_[ruleType].push_back(
     new QueryRule(action, query, priority, (ProcessingMode *)this, &interp));
}

ProcessingMode::GroveRules::GroveRules()
: built(0)
{
}

ProcessingMode::ElementRules::ElementRules(const StringC &name)
: Named(name)
{
}

// Specificity gives specificity of last match; get specificity of current match.
const ProcessingMode::Rule *
ProcessingMode::findMatch(const NodePtr &node,
			  Pattern::MatchContext &context,
			  Messenger &mgr,
			  Specificity &specificity) const
{
  GroveString gi;
  if (node->getGi(gi) == accessOK) 
    return findElementMatch(StringC(gi.data(), gi.size()), 
	                    node, context, mgr, specificity);
  NodePtr tem;
  if (node->getOrigin(tem) != accessOK) 
    return findRootMatch(node, context, mgr, specificity);
  if (hasQuery())
    return findQueryMatch(node, context, mgr, specificity);
  return 0;
}


const ProcessingMode::Rule *
ProcessingMode::findElementMatch(const StringC &gi,
				 const NodePtr &node,
				 Pattern::MatchContext &context,
				 Messenger &mgr,
				 Specificity &specificity) const
{
  const Vector<const ElementRule *> *vecP = 0;

  for (;;) {
    for (;;) {
      const ProcessingMode &mode
	= *(initial_ && specificity.toInitial_ ? initial_ : this);
      for (;;) {
        if (specificity.query_) { 
          const Vector<QueryRule *> &vec = mode.queryRules_[specificity.ruleType_];
          ASSERT(specificity.nextRuleIndex_ <= vec.size());
          for (size_t &i = specificity.nextRuleIndex_; i < vec.size(); i++) {
	    if (vec[i]->matches(node)) {
	      const Rule *rule = vec[i];
	      queryRuleAdvance(node, mgr, specificity, vec);
	      return rule;
	    }
          }
          specificity.query_ = 0;
          specificity.nextRuleIndex_ = 0;
        }
        else {
          if (!vecP) {
	    const GroveRules &gr = mode.groveRules(node, mgr);
	    const ElementRules *er = gr.elementTable.lookup(gi);
	    vecP = er ? er->rules : gr.otherRules;
          }
          const Vector<const ElementRule *> &vec = vecP[specificity.ruleType_];
          ASSERT(specificity.nextRuleIndex_ <= vec.size());
          for (size_t &i = specificity.nextRuleIndex_; i < vec.size(); i++) {
	    if (vec[i]->trivial() || vec[i]->matches(node, context)) {
	      const Rule *rule = vec[i];
	      elementRuleAdvance(node, context, mgr, specificity, vec);
	      return rule;
	    }
          }
          specificity.query_ = 1;
          break;
        }
      }
      if (!initial_)
	break;
      vecP = 0;
      if (specificity.toInitial_)
	break;
      specificity.nextRuleIndex_ = 0;
      specificity.toInitial_ = 1;
    }
    if (specificity.ruleType_ == constructionRule)
      break;
    specificity.ruleType_ = constructionRule;
    specificity.nextRuleIndex_ = 0;
    specificity.toInitial_ = 0;
  }
  return 0;
}

const ProcessingMode::Rule *
ProcessingMode::findRootMatch(const NodePtr &node,
			      Pattern::MatchContext &context,
			      Messenger &mgr,
			      Specificity &specificity) const
{
  for (;;) {
    for (;;) {
      const ProcessingMode &mode = *(initial_ && specificity.toInitial_ ? initial_ : this);
      for (;;) {
        if (specificity.query_) {
          const Vector<QueryRule *> &vec = mode.queryRules_[specificity.ruleType_];
          ASSERT(specificity.nextRuleIndex_ <= vec.size());
          for (size_t &i = specificity.nextRuleIndex_; i < vec.size(); i++) {
	    if (vec[i]->matches(node)) {
	      const Rule *rule = vec[i];
	      queryRuleAdvance(node, mgr, specificity, vec);
	      return rule;
	    }
          }
          specificity.query_ = 0;
          specificity.nextRuleIndex_ = 0;
        }
        else {
          const Vector<Rule> &rules = mode.rootRules_[specificity.ruleType_];
          if (specificity.nextRuleIndex_ < rules.size())
	    return &rules[specificity.nextRuleIndex_++];
          specificity.query_ = 1;
          break;
        }
      } 
      if (!initial_ || specificity.toInitial_)
        break;
      specificity.nextRuleIndex_ = 0;
      specificity.toInitial_ = 1;
    }
    if (specificity.ruleType_ == constructionRule)
      break;
    specificity.ruleType_ = constructionRule;
    specificity.nextRuleIndex_ = 0;
    specificity.toInitial_ = 0;
    specificity.query_ = 1; // start looking at query construction rules
  }
  return 0;
}

const ProcessingMode::Rule *
ProcessingMode::findQueryMatch(const NodePtr &node,
			      Pattern::MatchContext &context,
			      Messenger &mgr,
			      Specificity &specificity) const
{
  for (;;) {
    for (;;) {
      const ProcessingMode &mode = *(initial_ && specificity.toInitial_ ? initial_ : this);
      for (;;) {
        if (specificity.query_) {
          const Vector<QueryRule *> &vec = mode.queryRules_[specificity.ruleType_];
          ASSERT(specificity.nextRuleIndex_ <= vec.size());
          for (size_t &i = specificity.nextRuleIndex_; i < vec.size(); i++) {
	    if (vec[i]->matches(node)) {
	      const Rule *rule = vec[i];
	      queryRuleAdvance(node, mgr, specificity, vec);
	      return rule;
	    }
          }
          specificity.query_ = 0;
          specificity.nextRuleIndex_ = 0;
        }
        else { 
          specificity.query_ = 1;
          break;
        }
      } 
      if (!initial_ || specificity.toInitial_)
        break;
      specificity.nextRuleIndex_ = 0;
      specificity.toInitial_ = 1;
    }
    if (specificity.ruleType_ == constructionRule)
      break;
    specificity.ruleType_ = constructionRule;
    specificity.nextRuleIndex_ = 0;
    specificity.toInitial_ = 0;
  }
  return 0;
}

const ProcessingMode::GroveRules &ProcessingMode::groveRules(const NodePtr &node,
							     Messenger &mgr) const
{
  unsigned long n = node->groveIndex();
  ProcessingMode *cache = (ProcessingMode *)this;
  if (n >= groveRules_.size())
    cache->groveRules_.resize(n + 1);
  if (!groveRules_[n].built)
    cache->groveRules_[n].build(elementRules_, node, mgr);
  return groveRules_[n];
}

void ProcessingMode::elementRuleAdvance(const NodePtr &node,
				        Pattern::MatchContext &context,
				        Messenger &mgr,
				        Specificity &specificity,
				        const Vector<const ElementRule *> &vec)
{
  specificity.query_ = 0;
  size_t &i = specificity.nextRuleIndex_;
  if (specificity.ruleType_ != constructionRule) {
    ++i;
    return;
  }
  size_t hit = i;
  do {
    ++i;
    if (i >= vec.size()
	|| vec[hit]->ElementRule::compareSpecificity(*vec[i]) != 0)
      return;
  } while (!(vec[i]->trivial() || vec[i]->matches(node, context)));

  const LocNode *lnp;
  Location nodeLoc;
  if ((lnp = LocNode::convert(node)) != 0
      && lnp->getLocation(nodeLoc) == accessOK)
    mgr.setNextLocation(nodeLoc);
  mgr.message(InterpreterMessages::ambiguousMatch);
  do {
    ++i;
  } while (i < vec.size()
           && vec[hit]->ElementRule::compareSpecificity(*vec[i]) == 0);
}

void ProcessingMode::queryRuleAdvance(const NodePtr &node,
				      Messenger &mgr,
				      Specificity &specificity,
				      const Vector<QueryRule *> &vec)
{
  size_t &i = specificity.nextRuleIndex_;
  if (specificity.ruleType_ != constructionRule) {
    i++;
    return;
  }
  size_t hit = i;
  do {
    ++i;
    if (i >= vec.size()
	|| vec[hit]->compareSpecificity(*vec[i]) != 0)
      return;
  } while (!vec[i]->matches(node));

  const LocNode *lnp;
  Location nodeLoc;
  if ((lnp = LocNode::convert(node)) != 0
      && lnp->getLocation(nodeLoc) == accessOK)
    mgr.setNextLocation(nodeLoc);
  mgr.message(InterpreterMessages::ambiguousMatch);
  do {
    ++i;
  } while (i < vec.size()
           && vec[hit]->compareSpecificity(*vec[i]) == 0);
}

#ifdef DSSSL_NAMESPACE
}
#endif
