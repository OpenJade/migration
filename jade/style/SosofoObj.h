// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#ifndef SosofoObj_INCLUDED
#define SosofoObj_INCLUDED 1

#include "ELObj.h"
#include <OpenSP/Location.h>
#include "FOTBuilder.h"
#include <OpenSP/Owner.h>
#include <OpenSP/CopyOwner.h>
#include "Insn.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

class ProcessContext;
class Interpreter;

class SosofoObj : public ELObj {
public:
  virtual void process(ProcessContext &) = 0;
  SosofoObj *asSosofo();
  virtual bool tableBorderStyle(StyleObj *&);
  virtual bool ruleStyle(ProcessContext &, StyleObj *&);
  virtual bool isRule();
  virtual bool characterStyle(ProcessContext &, StyleObj *&, FOTBuilder::CharacterNIC &);
  virtual bool isCharacter();
};

class NextMatchSosofoObj : public SosofoObj {
public:
  NextMatchSosofoObj(StyleObj *, const Location &);
  void process(ProcessContext &);
private:
  StyleObj *style_;
  Location loc_;
};

class EmptySosofoObj : public SosofoObj {
public:
  void process(ProcessContext &);
};

class ProcessingMode;

class ProcessNodeSosofoObj : public SosofoObj {
public:
  void *operator new(size_t, Collector &c) {
    return c.allocateObject(1);
  }
  ProcessNodeSosofoObj(const NodePtr &node, const ProcessingMode *mode,
		       const Location &loc);
  void process(ProcessContext &);
private:
  NodePtr node_;
  const ProcessingMode *mode_;
  Location loc_;
};

class ProcessNodeListSosofoObj : public SosofoObj {
public:
  ProcessNodeListSosofoObj(NodeListObj *, const ProcessingMode *,
			   const Location &loc);
  void process(ProcessContext &);
  void traceSubObjects(Collector &) const;
private:
  NodeListObj *nodeList_;
  const ProcessingMode *mode_;
  Location loc_;
};

class AppendSosofoObj : public SosofoObj {
public:
  void *operator new(size_t, Collector &c) {
    return c.allocateObject(1);
  }
  AppendSosofoObj() { hasSubObjects_ = 1; }
  AppendSosofoObj *asAppendSosofo();
  void process(ProcessContext &);
  void append(SosofoObj *);
  void traceSubObjects(Collector &) const;
private:
  Vector<SosofoObj *> v_;
};

class LiteralSosofoObj : public SosofoObj {
public:
  LiteralSosofoObj(ELObj *str, const Location &loc)
    : str_(str), loc_(loc) { hasSubObjects_ = 1; }
  void process(ProcessContext &);
  void traceSubObjects(Collector &) const;
private:
  ELObj *str_;
  Location loc_;
};

class CurrentNodePageNumberSosofoObj : public SosofoObj {
public:
  void *operator new(size_t, Collector &c) {
    return c.allocateObject(1);
  }
  CurrentNodePageNumberSosofoObj(const NodePtr &node)
    : node_(node) { }
  void process(ProcessContext &);
private:
  NodePtr node_;
  Location loc_;
};

class PageNumberSosofoObj : public SosofoObj {
public:
  PageNumberSosofoObj() { }
  void process(ProcessContext &);
};

class CompoundFlowObj;

class FlowObj : public SosofoObj {
public:
  void *operator new(size_t, Collector &c) {
    return c.allocateObject(1);
  }
  FlowObj();
  FlowObj(const FlowObj &fo)
    : SosofoObj(fo), style_(fo.style_),
    locp_(fo.locp_ ? new Location(*fo.locp_) : 0)
    { }
  virtual FlowObj *copy(Collector &) const = 0;
  virtual CompoundFlowObj *asCompoundFlowObj();
  virtual bool hasNonInheritedC(const Identifier *) const;
  virtual bool hasPseudoNonInheritedC(const Identifier *) const;
  virtual void setNonInheritedC(const Identifier *, ELObj *, const Location &, Interpreter &);
  virtual void setImplicitChar(Char);
  static void fixCharNICs(const Char *, size_t,
				  FOTBuilder::CharacterNIC *,
				  const Location &,
				  ProcessContext &);
  void traceSubObjects(Collector &) const;
  void setStyle(StyleObj *);
  void setLocation(const Location &loc);
  void process(ProcessContext &);
  virtual void pushStyle(ProcessContext &, unsigned &);
  virtual void popStyle(ProcessContext &, unsigned);
  virtual void processInner(ProcessContext &) = 0;
  const Location &location() const
    { return *locp_; }
  // Accept flags. Used by ProcessContext::Validator descendants.
  enum {
    afNever=0,  // I.e. only as characteristic value.
    afAlways=1<<0,  // Always accepted
    afScroll=1<<1,  // This is a scroll object.
    afPaging=1<<2,  // A paging FO (i.e. (simple-)page-sequence
    afInline=1<<3,
    afDisplay=1<<4,
    afParagraphBreak=1<<5,
    afMathSequence=1<<6,  // FO accepted in a math-sequence
    afChar=1<<7,
    afGridCell=1<<8,
    afSideBySideItem,
    afTablePart=1<<10,
    afTableColumn=1<<11,
    afTableRow=1<<12,
    afTableCell=1<<13,
  };
  // Change when more than 16 flags (to be safe).
  typedef unsigned AcceptFlags;
  // This method has to be called AFTER pushStyle, since some FOs will
  // change flags according to some characteristic values.
  virtual AcceptFlags acceptFlags(ProcessContext &) const = 0;
protected:
  static
    bool setDisplayNIC(FOTBuilder::DisplayNIC &,
                       const Identifier *, ELObj *,
		       const Location &, Interpreter &);
  static bool isDisplayNIC(const Identifier *);
  StyleObj *style_;
  Owner<Location> locp_;
  // We have two "pointer-sized" data members. Room for two more in
  // derived classes.
};

class CompoundFlowObj : public FlowObj {
public:
  CompoundFlowObj() : content_(0) { }
  void processInner(ProcessContext &);
  void traceSubObjects(Collector &) const;
  void setContent(SosofoObj *content) { content_ = content; }
  CompoundFlowObj *asCompoundFlowObj();
protected:
  SosofoObj *content() const;
private:
  SosofoObj *content_;
  // One pointer left for NICs.
};

class SequenceFlowObj : public CompoundFlowObj {
public:
  SequenceFlowObj() { }
  void processInner(ProcessContext &);
  FlowObj *copy(Collector &) const;
  AcceptFlags acceptFlags(ProcessContext &) const
    { return afAlways; }
};

class ProcessingMode;

class ProcessChildrenSosofoObj : public SosofoObj {
public:
  ProcessChildrenSosofoObj(const ProcessingMode *mode, const Location &loc)
    : mode_(mode), loc_(loc) { }
  void process(ProcessContext &);
private:
  const ProcessingMode *mode_;
  Location loc_;
};

class ProcessChildrenTrimSosofoObj : public SosofoObj {
public:
  ProcessChildrenTrimSosofoObj(const ProcessingMode *mode, const Location &loc)
    : mode_(mode), loc_(loc) { }
  void process(ProcessContext &);
private:
  const ProcessingMode *mode_;
  Location loc_;
};

class SetNonInheritedCsSosofoObj : public SosofoObj {
public:
  void *operator new(size_t, Collector &c) {
    return c.allocateObject(1);
  }
  SetNonInheritedCsSosofoObj(FlowObj *, const InsnPtr &, ELObj **, const NodePtr &);
  ~SetNonInheritedCsSosofoObj();
  void process(ProcessContext &);
  void traceSubObjects(Collector &) const;
  bool characterStyle(ProcessContext &, StyleObj *&, FOTBuilder::CharacterNIC &);
  bool isCharacter();
  bool ruleStyle(ProcessContext &, StyleObj *&);
  bool isRule();
private:
  ELObj *resolve(ProcessContext &);

  FlowObj *flowObj_;
  ELObj **display_;
  InsnPtr code_;
  NodePtr node_;
};

class Expression;

// This wraps a flow object that has a label:.
class LabelSosofoObj : public SosofoObj {
public:
  void *operator new(size_t, Collector &c) {
    return c.allocateObject(1);
  }
  LabelSosofoObj(SymbolObj *, const Location &, SosofoObj *content);
  void process(ProcessContext &);
  void traceSubObjects(Collector &) const;
private:
  SymbolObj *label_;
  Owner<Location> locp_;
  SosofoObj *content_;
};

// This wraps the content of a sosofo with a content-map:.

class ContentMapSosofoObj : public SosofoObj {
public:
  ContentMapSosofoObj(ELObj *, const Location *, SosofoObj *content);
  void process(ProcessContext &);
  void traceSubObjects(Collector &) const;
private:
  ELObj *contentMap_;
  const Location *locp_;
  SosofoObj *content_; 
};

class DiscardLabeledSosofoObj : public SosofoObj {
public:
  DiscardLabeledSosofoObj(SymbolObj *, SosofoObj *);
  void process(ProcessContext &);
  void traceSubObjects(Collector &) const;
private:
  SymbolObj *label_;
  SosofoObj *content_; 
};

class PageTypeSosofoObj : public SosofoObj {
public:
  PageTypeSosofoObj(unsigned, SosofoObj *, SosofoObj *);
  void process(ProcessContext &);
  void traceSubObjects(Collector &) const;
private:
  unsigned pageTypeFlag_;
  SosofoObj *match_;
  SosofoObj *noMatch_;
};

inline
void FlowObj::setStyle(StyleObj *style)
{
  style_ = style;
}

inline
void FlowObj::setLocation(const Location &loc)
{
  locp_ = new Location(loc);
}

inline
void AppendSosofoObj::append(SosofoObj *obj)
{
  v_.push_back(obj);
}

inline
SosofoObj *CompoundFlowObj::content() const
{
  return content_;
}

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* not SosofoObj_INCLUDED */

