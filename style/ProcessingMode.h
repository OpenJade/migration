// Copyright (c) 1996 James Clark, 2000 Matthias Clasen
// See the file copying.txt for copying permission.

#ifndef ProcessingMode_INCLUDED
#define ProcessingMode_INCLUDED 1

#include <OpenSP/Named.h>
#include <OpenSP/StringC.h>
#include <OpenSP/Location.h>
#include <OpenSP/Owner.h>
#include <OpenSP/Vector.h>
#include <OpenSP/NCVector.h>
#include <OpenSP/NamedTable.h>
#include "Expression.h"
#include "Insn.h"
#include <OpenSP/Boolean.h>
#include "Node.h"
#include "FOTBuilder.h"
#include <OpenSP/IList.h>
#include "Pattern.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

class Interpreter;
class SosofoObj;

class ProcessingMode : public Named {
public:
  enum RuleType {
    styleRule,
    constructionRule
  };
  enum { nRuleType = 2 };

  class Specificity {
  public:
    Specificity();
    bool isStyle() const;
  private:
    bool toInitial_; // 1 if the match fell through from a named processing mode to
	             // the initial processing mode
    RuleType ruleType_;
    size_t nextRuleIndex_;
    friend class ProcessingMode;
  };

  class Action : public Resource {
  public:
    Action(unsigned partIndex, Owner<Expression> &, const Location &);
    void compile(Interpreter &, RuleType);
    void get(InsnPtr &, SosofoObj *&) const;
    const Location &location() const;
    unsigned partIndex() const;
  private:
    Location defLoc_;
    Owner<Expression> expr_;
    // One of these will be non-null.
    InsnPtr insn_;
    // must be permanent
    SosofoObj *sosofo_;
    unsigned partIndex_;
  };

  class ElementRule;
  class RootRule;
  class QueryRule;

  class Rule : virtual public MatchBase {
  public:
    Rule();
    Rule(const Ptr<Action> &);
    const Action &action() const;
    Action &action();
    virtual int compareSpecificity(const Rule &) const = 0;
    virtual int compareSpecificity2(const ElementRule *) const;
    virtual int compareSpecificity2(const RootRule *) const;
    virtual int compareSpecificity2(const QueryRule *) const;
    const Location &location() const;
    void swap(Rule &);
  private:
    Ptr<Action> action_;
    int compareParts(const Rule *) const;
  };

  class ElementRule : public Rule, public Pattern, public Link {
  public:
    ElementRule(const Ptr<Action> &, Pattern &);
    int compareSpecificity(const Rule &) const;
    int compareSpecificity2(const ElementRule *) const;
    int compareSpecificity2(const QueryRule *) const;
  };

  class QueryRule : public Rule, private Collector::DynamicRoot {
  public:
    QueryRule(const Ptr<Action> &, Owner<Expression> &, Owner<Expression> &,
              ProcessingMode *, Interpreter *);
    int compareSpecificity(const Rule &) const;
    int compareSpecificity2(const ElementRule *) const;
    int compareSpecificity2(const RootRule *) const;
    int compareSpecificity2(const QueryRule *) const;
    void trace(Collector &) const;
    bool matches(const NodePtr &, Pattern::MatchContext &) const;
    bool matches(ComponentName::Id) const;
    void compile(const NodePtr &);
  private:
    ProcessingMode *pm_;
    Interpreter *interp_;
    Owner<Expression> nlExpr_;
    mutable Owner<Expression> priorityExpr_;
    mutable NodeListObj *nl_;
    mutable long priority_;
  };
 
  class RootRule : public Rule {
  public:
    RootRule();
    RootRule(const Ptr<Action> &);
    int compareSpecificity(const Rule &) const;
    int compareSpecificity2(const RootRule *) const;
    int compareSpecificity2(const QueryRule *) const;
    bool matches(const NodePtr &, Pattern::MatchContext &) const;
  };

  ProcessingMode(const StringC &, const ProcessingMode *initial = 0);
  void addElementRule(NCVector<Pattern> &, Owner<Expression> &expr,
	       RuleType, const Location &, Interpreter &);
  void addRootRule(Owner<Expression> &expr, 
               RuleType, const Location &, Interpreter &);
  void addQueryRule(Owner<Expression> &query,
               Owner<Expression> &expr,
               Owner<Expression> &priority,
               RuleType,
               const Location &, Interpreter &);
  // Specificity gives specificity of last match; gets specificity of current match.
  const Rule *findMatch(const NodePtr &, Pattern::MatchContext &, Messenger &,
			Specificity &) const;
  void compile(Interpreter &, const NodePtr &);
  bool defined() const;
  void setDefined();
  bool hasCharRules() const;

  struct ElementRules : public Named {
  public:
    ElementRules(const StringC &);
    Vector<const Rule *> rules[nRuleType];
  };

  struct GroveRules {
    GroveRules();
    bool built;
    NamedTable<ElementRules> elementTable;
    Vector<const Rule *> otherRules[nRuleType];
    void build(const IList<ElementRule> *, const Vector<QueryRule *> *, 
               const NodePtr &, Messenger &);
  };
private:
  const Rule *findElementMatch(const StringC &, const NodePtr &,
			       Pattern::MatchContext &, Messenger &,
			       Specificity &) const;
  const Rule *findRootMatch(const NodePtr &, Pattern::MatchContext &, Messenger &,
			    Specificity &) const;
  const Rule *findQueryMatch(const NodePtr &, Pattern::MatchContext &, Messenger &,
			    Specificity &) const;
  const GroveRules &groveRules(const NodePtr &, Messenger &) const;
  void ruleAdvance(const NodePtr &nd, Pattern::MatchContext &context,
	          Messenger &mgr, Specificity &specificity,
		  const Vector<const Rule *> &vec) const;
  void ruleAdvance(const NodePtr &nd, Pattern::MatchContext &context,
	          Messenger &mgr, Specificity &specificity,
		  const Vector<Rule *> &vec) const;
  void ruleAdvance(const NodePtr &nd, Pattern::MatchContext &context,
	          Messenger &mgr, Specificity &specificity,
		  const Vector<QueryRule *> &vec) const;

  Vector<Rule *> rootRules_[nRuleType];
  Vector<QueryRule *> queryRules_[nRuleType];
  IList<ElementRule> elementRules_[nRuleType];
  NCVector<GroveRules> groveRules_;
  const ProcessingMode *initial_; // 0 for initial mode
  bool defined_;
  bool hasCharRules_;
};


inline
bool ProcessingMode::defined() const
{
  return defined_;
}

inline
void ProcessingMode::setDefined()
{
  defined_ = 1;
}

inline
bool ProcessingMode::hasCharRules() const
{
  if (hasCharRules_)
    return 1;
  if (initial_)
    return initial_->hasCharRules(); 
  return 0;
}

inline
ProcessingMode::Specificity::Specificity()
: toInitial_(0), nextRuleIndex_(0), ruleType_(styleRule)
{
}

inline
bool ProcessingMode::Specificity::isStyle() const
{
  return ruleType_ == styleRule;
}

inline
void ProcessingMode::Action::get(InsnPtr &insn, SosofoObj *&sosofo) const
{
  insn = insn_;
  sosofo = sosofo_;
}

inline
const Location &ProcessingMode::Action::location() const
{
  return defLoc_;
}

inline
ProcessingMode::Action &ProcessingMode::Rule::action()
{
  return *action_;
}

inline
const ProcessingMode::Action &ProcessingMode::Rule::action() const
{
  return *action_;
}

inline
unsigned ProcessingMode::Action::partIndex() const
{
  return partIndex_;
}

inline
const Location &ProcessingMode::Rule::location() const
{
  return action_->location();
}

inline
void ProcessingMode::Rule::swap(Rule &r)
{
  action_.swap(r.action_);
}

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* not ProcessingMode_INCLUDED */
