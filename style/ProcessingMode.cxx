// Copyright (c) 1996, 1997 James Clark, 2000 Matthias Clasen
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

extern "C" {

static
int ruleCompare(const void *p1, const void *p2)
{
  const ProcessingMode::Rule *const *r1 
    = (const ProcessingMode::Rule *const *)p1;
  const ProcessingMode::Rule *const *r2 
    = (const ProcessingMode::Rule *const *)p2;
  return ((*r1)->compareSpecificity(**r2));
}

}

static void sortRules(ProcessingMode::Rule **v, size_t size)
{
#if 1
  //FIXME I had problems getting the right versions of compareSpecificity
  // called in ruleCompare, but now it seems to work.   
  qsort(&(v[0]), size, sizeof(ProcessingMode::Rule *), ruleCompare);
#else
  // FIXME implement a better sorting algorithm
  for (size_t i = 0; i + 1 < size;) {
    if (v[i]->compareSpecificity(*(v[i + 1])) > 0) {
      ProcessingMode::Rule *tem = v[i];
      v[i] = v[i + 1];
      v[i + 1] = tem;
      i = 0;
    } 
    else 
      i++;
  }
#endif
}

ProcessingMode::ProcessingMode(const StringC &name, const ProcessingMode *initial)
: Named(name), initial_(initial), defined_(0), hasCharRules_(0)
{
}

void ProcessingMode::GroveRules::build(const IList<ElementRule> *lists,
                                       const Vector<QueryRule *> *qvecs,
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
  for (int ruleType = 0; ruleType < nRuleType; ruleType++) 
    for (size_t j = 0; j < qvecs[ruleType].size(); j++)
      if (qvecs[ruleType][j]->matches(ComponentName::idElement)) 
	otherRules[ruleType].push_back(qvecs[ruleType][j]);
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
      sortRules((Rule **)p->rules[ruleType].begin(), p->rules[ruleType].size());
    }
    sortRules((Rule **)otherRules[ruleType].begin(), otherRules[ruleType].size());
  }
}

void ProcessingMode::compile(Interpreter &interp, const NodePtr &root)
{
  for (int ruleType = 0; ruleType < nRuleType; ruleType++) {
    for (size_t j = 0; j < rootRules_[ruleType].size(); j++)
      rootRules_[ruleType][j]->action().compile(interp, RuleType(ruleType));
    for (size_t j = 0; j < queryRules_[ruleType].size(); j++) {
      queryRules_[ruleType][j]->action().compile(interp, RuleType(ruleType));
      queryRules_[ruleType][j]->compile(root);
    }
    sortRules((Rule **)queryRules_[ruleType].begin(), queryRules_[ruleType].size());
    for (size_t j = 0; j < queryRules_[ruleType].size(); j++)
      if (queryRules_[ruleType][j]->matches(ComponentName::idSgmlDocument)) 
	rootRules_[ruleType].push_back(queryRules_[ruleType][j]);
    sortRules((Rule **)rootRules_[ruleType].begin(), rootRules_[ruleType].size());
    for (size_t j = 0; !hasCharRules_ && j < queryRules_[ruleType].size(); j++)
      if (queryRules_[ruleType][j]->matches(ComponentName::idDataChar)) 
        hasCharRules_ = 1; 
    for (IListIter<ElementRule> iter(elementRules_[ruleType]); !iter.done(); iter.next())
      iter.cur()->action().compile(interp, RuleType(ruleType)); 
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

int ProcessingMode::Rule::compareSpecificity2(const RootRule *rule) const
{
  return - compareParts(rule);
}

int ProcessingMode::Rule::compareSpecificity2(const ElementRule *rule) const
{
  return - compareParts(rule);
}

int ProcessingMode::Rule::compareSpecificity2(const QueryRule *rule) const
{
  return - compareParts(rule);
}

int ProcessingMode::Rule::compareParts(const Rule *r) const
{
  unsigned i1 = action().partIndex();
  unsigned i2 = r->action().partIndex();
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

int ProcessingMode::ElementRule::compareSpecificity(const Rule &rule) const
{
  return rule.compareSpecificity2(this);
}

int ProcessingMode::ElementRule::compareSpecificity2(const ElementRule *rule) const
{
  int result = Rule::compareSpecificity2(rule);
  if (result)
    return result;
  return Pattern::compareSpecificity(*rule, *this);
}

int ProcessingMode::ElementRule::compareSpecificity2(const QueryRule *rule) const
{
  int result = Rule::compareSpecificity2(rule);
  if (result)
    return result;
  return -1;
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

void ProcessingMode::QueryRule::compile(const NodePtr &root)
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
      return;
    }
    if (!val->exactIntegerValue(priority_)) {
      interp_->setNextLocation(location());
      interp_->message(InterpreterMessages::sorryPriority);
      return;
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
      return;
    }
    nl_ = val->asNodeList();
    interp_->makeReadOnly(nl_);
  }
}

bool ProcessingMode::QueryRule::matches(const NodePtr &nd, Pattern::MatchContext &) const
{
  EvalContext ec;
  NodePtr tem = nl_->nodeListFirst(ec, *interp_);
  if (!tem)
    return 0;
  tem->getGroveRoot(tem);
  EvalContext::CurrentNodeSetter cns(tem, pm_, ec);
  return nl_->contains(ec, *interp_, nd);
}

bool ProcessingMode::QueryRule::matches(ComponentName::Id cls) const
{
  EvalContext ec;
  NodePtr tem = nl_->nodeListFirst(ec, *interp_);
  if (!tem)
    return 0;
  tem->getGroveRoot(tem);
  EvalContext::CurrentNodeSetter cns(tem, pm_, ec);
  bool ret = nl_->contains(ec, *interp_, cls);
  return ret;
}

int ProcessingMode::QueryRule::compareSpecificity(const Rule &rule) const
{
  return rule.compareSpecificity2(this);
}

int ProcessingMode::QueryRule::compareSpecificity2(const QueryRule *rule) const
{
  int result = Rule::compareSpecificity2(rule);
  if (result)
    return result;
  if (priority_ == rule->priority_)
    return 0;
  return priority_ < rule->priority_ ? -1 : 1;
}

int ProcessingMode::QueryRule::compareSpecificity2(const ElementRule *rule) const
{
  int result = Rule::compareSpecificity2(rule);
  if (result)
    return result;
  return 1;
}

int ProcessingMode::QueryRule::compareSpecificity2(const RootRule *rule) const
{
  int result = Rule::compareSpecificity2(rule);
  if (result)
    return result;
  return 1;
}

void ProcessingMode::QueryRule::trace(Collector &c) const
{
  if (nl_)
    c.trace(nl_);
}

ProcessingMode::RootRule::RootRule() 
{
}

ProcessingMode::RootRule::RootRule(const Ptr<Action> &action)
: MatchBase(1), Rule(action) 
{
}

int ProcessingMode::RootRule::compareSpecificity(const Rule &rule) const
{
  rule.compareSpecificity2(this);
}

int ProcessingMode::RootRule::compareSpecificity2(const QueryRule *rule) const
{
  int result = Rule::compareSpecificity2(rule);
  if (result)
    return result;
  return -1;
}

int ProcessingMode::RootRule::compareSpecificity2(const RootRule *rule) const
{
  int result = Rule::compareSpecificity2(rule);
  if (result)
    return result;
  return 0;
}

bool ProcessingMode::RootRule::matches(const NodePtr &nd, Pattern::MatchContext &) const
{
  // Doesn't matter since root rules are trivial
  return 1;
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
  Vector<Rule *> &rules = rootRules_[ruleType];
  rules.push_back(new RootRule(action));
  for (size_t i = rules.size() - 1; i > 0; i--) {
    int cmp = rules[i - 1]->compareSpecificity(*(rules[i]));
    if (cmp <= 0) {
      if (cmp == 0 && ruleType == constructionRule) {
	interp.setNextLocation(loc);
	interp.message(InterpreterMessages::duplicateRootRule,
		       rules[i - 1]->location());
      }
      break;
    }
    Rule *tem = rules[i-1];
    rules[i-1] = rules[i];
    rules[i] = tem;
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
  return findQueryMatch(node, context, mgr, specificity);
}


const ProcessingMode::Rule *
ProcessingMode::findElementMatch(const StringC &gi,
				 const NodePtr &node,
				 Pattern::MatchContext &context,
				 Messenger &mgr,
				 Specificity &specificity) const
{
  const Vector<const Rule *> *vecP = 0;

  for (;;) {
    for (;;) {
      const ProcessingMode &mode
	= *(initial_ && specificity.toInitial_ ? initial_ : this);
      if (!vecP) {
        const GroveRules &gr = mode.groveRules(node, mgr);
	const ElementRules *er = gr.elementTable.lookup(gi);
	vecP = er ? er->rules : gr.otherRules;
      }
      const Vector<const Rule *> &vec = vecP[specificity.ruleType_];
      ASSERT(specificity.nextRuleIndex_ <= vec.size());
      for (size_t &i = specificity.nextRuleIndex_; i < vec.size(); i++) {
	if (vec[i]->trivial() || vec[i]->matches(node, context)) {
	  const Rule *rule = vec[i];
	  ruleAdvance(node, context, mgr, specificity, vec);
	  return rule;
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
      const Vector<Rule *> &vec = mode.rootRules_[specificity.ruleType_];
      ASSERT(specificity.nextRuleIndex_ <= vec.size());
      for (size_t &i = specificity.nextRuleIndex_; i < vec.size(); i++) {
        if (vec[i]->trivial() || vec[i]->matches(node, context)) {
          const Rule *rule = vec[i];
          ruleAdvance(node, context, mgr, specificity, vec);
          return rule;
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

const ProcessingMode::Rule *
ProcessingMode::findQueryMatch(const NodePtr &node,
			      Pattern::MatchContext &context,
			      Messenger &mgr,
			      Specificity &specificity) const
{
  for (;;) {
    for (;;) {
      const ProcessingMode &mode = *(initial_ && specificity.toInitial_ ? initial_ : this);
      const Vector<QueryRule *> &vec = mode.queryRules_[specificity.ruleType_];
      ASSERT(specificity.nextRuleIndex_ <= vec.size());
      for (size_t &i = specificity.nextRuleIndex_; i < vec.size(); i++) {
	if (vec[i]->matches(node, context)) {
	  const Rule *rule = vec[i];
	  ruleAdvance(node, context, mgr, specificity, vec);
	  return rule;
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
    cache->groveRules_[n].build(elementRules_, queryRules_, node, mgr);
  return groveRules_[n];
}

void ProcessingMode::ruleAdvance(const NodePtr &node,
			        Pattern::MatchContext &context,
			        Messenger &mgr,
			        Specificity &specificity,
			        const Vector<const Rule *> &vec) const
{
  size_t &i = specificity.nextRuleIndex_;
  if (specificity.ruleType_ != constructionRule) {
    ++i;
    return;
  }
  size_t hit = i;
  do {
    ++i;
    if (i >= vec.size()
	|| vec[hit]->compareSpecificity(*vec[i]) != 0)
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
           && vec[hit]->compareSpecificity(*vec[i]) == 0);
}

void ProcessingMode::ruleAdvance(const NodePtr &node,
			        Pattern::MatchContext &context,
			        Messenger &mgr,
			        Specificity &specificity,
			        const Vector<Rule *> &vec) const
{
  size_t &i = specificity.nextRuleIndex_;
  if (specificity.ruleType_ != constructionRule) {
    ++i;
    return;
  }
  size_t hit = i;
  do {
    ++i;
    if (i >= vec.size()
	|| vec[hit]->compareSpecificity(*vec[i]) != 0)
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
           && vec[hit]->compareSpecificity(*vec[i]) == 0);
}

void ProcessingMode::ruleAdvance(const NodePtr &node,
			        Pattern::MatchContext &context,
			        Messenger &mgr,
			        Specificity &specificity,
			        const Vector<QueryRule *> &vec) const
{
  size_t &i = specificity.nextRuleIndex_;
  if (specificity.ruleType_ != constructionRule) {
    ++i;
    return;
  }
  size_t hit = i;
  do {
    ++i;
    if (i >= vec.size()
	|| vec[hit]->compareSpecificity(*vec[i]) != 0)
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
           && vec[hit]->compareSpecificity(*vec[i]) == 0);
}

#ifdef DSSSL_NAMESPACE
}
#endif
