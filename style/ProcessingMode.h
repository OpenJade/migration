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
#include "IList.h"

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
    bool toInitial_; // 1 if the match fell through from a named processing mode to
	             // the initial processing mode
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
  bool findMatch(const NodePtr &, Messenger &,
		 Specificity &, InsnPtr &, SosofoObj *&) const;
  void compile(Interpreter &);
  class Rule {
  public:
    Rule(Owner<Expression> &, const Location &);
    virtual ~Rule() { }
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
  struct GroveRules;
  class ComplexRule : public Rule, public Link {
  public:
    ComplexRule(Owner<Expression> &, const Location &);
    virtual void add(GroveRules &, const NodePtr &, Messenger &) const = 0;
  };
  class IdRule : public ComplexRule {
  public:
    IdRule(const StringC &, Owner<Expression> &, const Location &);
    void add(GroveRules &, const NodePtr &, Messenger &) const;
  private:
    StringC id_;
  };
  struct ElementRule : public ComplexRule {
  public:
    ElementRule(Vector<StringC> &, Owner<Expression> &, const Location &);
    void add(GroveRules &, const NodePtr &, Messenger &) const;
  private:
    Vector<StringC> qGi_;
  };
  struct GroveIdRule : public Named {
    GroveIdRule(const StringC &, const IdRule *);
    const IdRule *rule;
  };
  class ElementRules : public Named {
  public:
    ElementRules(const StringC &);
    void addRule(const StringC *parents, size_t nParents, const ElementRule *,
		 Messenger &);
    const Rule *findMatch(const NodePtr &, unsigned &nQual) const;
    static void add(NamedTable<ElementRules> &, const StringC *gis, size_t nGis,
		    const ElementRule *, Messenger &);
  private:
    const ElementRule *unqual_;
    NamedTable<ElementRules> parent_;
  };
  struct GroveRules {
    GroveRules();
    bool built;
    NamedTable<ElementRules> elementTable;
    NamedTable<GroveIdRule> idTable;
  };
  struct Part {
    Owner<Rule> defaultRule;
    Owner<Rule> rootRule;
    IList<ComplexRule> complexRules;
    // Indexed by the groveIndex
    NCVector<GroveRules> groveRules;
    void prepare(const NodePtr &, Messenger &) const;
  };
private:
  NCVector<Part> parts_;
  const ProcessingMode *initial_; // 0 for initial mode
};

inline
ProcessingMode::Specificity::Specificity()
: part_(0), ruleType_(noRule), toInitial_(0)
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
