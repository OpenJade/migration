// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#ifndef ProcessingMode_INCLUDED
#define ProcessingMode_INCLUDED 1

#include "Named.h"
#include "StringC.h"
#include "Location.h"
#include "Owner.h"
#include "Vector.h"
#include "NCVector.h"
#include "NamedTable.h"
#include "Expression.h"
#include "Insn.h"
#include "Node.h"
#include "FOTBuilder.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

class Interpreter;
class SosofoObj;

class ProcessingMode : public Named {
public:
  class Specificity {
  public:
    Specificity();
    FOTBuilder::RuleType ruleType() const;
  private:
    unsigned part_;
    enum RuleType {
      noRule,
      queryRule,
      idRule,
      elementRule,
      defaultRule,
      rootRule
    };
    RuleType ruleType_;
    unsigned nQual_; // number of qualifying elements applies if ruleType_ == elementRule
    friend class ProcessingMode;
  };
  ProcessingMode(const StringC &, const ProcessingMode *initial = 0);
  void addRoot(Owner<Expression> &expr, const Location &loc, Interpreter &);
  // ids and gis must have been normalized using documents substitution rules
  void addId(const StringC &id, Owner<Expression> &expr, const Location &loc, Interpreter &);
  void addDefault(Owner<Expression> &expr, const Location &loc, Interpreter &);
  void addElement(Vector<StringC> &qgi, Owner<Expression> &expr,
		  const Location &loc, Interpreter &);
  // Specificity gives specificity of last match; get specificity of current match.
  bool findMatch(const NodePtr &, Specificity &, InsnPtr &, SosofoObj *&) const;
  void compile(Interpreter &);
  class Rule {
  public:
    Rule(Owner<Expression> &, const Location &);
    void compile(Interpreter &);
    void get(InsnPtr &, SosofoObj *&) const;
    const Location &location() const;
  private:
    Location defLoc_;
    Owner<Expression> expr_;
    // One of these will be non-null.
    InsnPtr insn_;
    // must be permanent
    SosofoObj *obj_;
  };
private:
  struct IdRule : public Rule, public Named {
    IdRule(const StringC &, Owner<Expression> &, const Location &);
  };
  struct ElementRules : public Named {
  public:
    ElementRules(const StringC &);
    void addRule(const StringC *parents, size_t nParents, Owner<Expression> &, const Location &,
		 Interpreter &);
    static void compile(NamedTable<ElementRules> &, Interpreter &);
    const Rule *findMatch(const NodePtr &, unsigned &nQual) const;
    static void add(NamedTable<ElementRules> &, const StringC *gis, size_t nGis,
		    Owner<Expression> &, const Location &, Interpreter &);
  private:
    Owner<Rule> unqual_;
    NamedTable<ElementRules> parent_;
  };
  struct Part {
    Owner<Rule> defaultRule;
    Owner<Rule> rootRule;
    NamedTable<IdRule> idTable;
    NamedTable<ElementRules> elementTable;
  };
  NCVector<Part> parts_;
  const ProcessingMode *initial_; // 0 for initial mode
};


inline
ProcessingMode::Specificity::Specificity()
: part_(0), ruleType_(noRule)
{
}

inline
void ProcessingMode::Rule::get(InsnPtr &insn, SosofoObj *&obj) const
{
  insn = insn_;
  obj = obj_;
}

inline
FOTBuilder::RuleType ProcessingMode::Specificity::ruleType() const
{
  switch (ruleType_) {
  case queryRule:
    return FOTBuilder::ruleQuery;
  case idRule:
    return FOTBuilder::ruleId;
  case elementRule:
    return FOTBuilder::RuleType(FOTBuilder::ruleElement1 + nQual_);
  case defaultRule:
    return FOTBuilder::ruleDefault;
  case rootRule:
    return FOTBuilder::ruleRoot;
  default:
    break;
  }
  return FOTBuilder::ruleNone;
}

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* not ProcessingMode_INCLUDED */
