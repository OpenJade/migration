// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#include "stylelib.h"
#include "ELObj.h"
#include "Interpreter.h"
#include "InterpreterMessages.h"
#include "macros.h"
#include <string.h>
#include <math.h>
#include <stdio.h>

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

ELObj::ELObj()
{
  hasSubObjects_ = 0;
}

bool ELObj::isEqual(ELObj &)
{
  return false;
}
    
bool ELObj::isNil() const
{
  return false;
}

bool ELObj::isList() const
{
  return false;
}

bool ELObj::isTrue() const
{
  return true;
}

PairObj *ELObj::asPair()
{
  return 0;
}

SymbolObj *ELObj::asSymbol()
{
  return 0;
}

KeywordObj *ELObj::asKeyword()
{
  return 0;
}

StringObj *ELObj::convertToString()
{
  return 0;
}

FunctionObj *ELObj::asFunction()
{
  return 0;
}

SosofoObj *ELObj::asSosofo()
{
  return 0;
}

ColorObj *ELObj::asColor()
{
  return 0;
}

ColorSpaceObj *ELObj::asColorSpace()
{
  return 0;
}

AddressObj *ELObj::asAddress()
{
  return 0;
}

DisplaySpaceObj *ELObj::asDisplaySpace()
{
  return 0;
}

InlineSpaceObj *ELObj::asInlineSpace()
{
  return 0;
}

GlyphSubstTableObj *ELObj::asGlyphSubstTable()
{
  return 0;
}

bool ELObj::optSingletonNodeList(QueryContext &, NodePtr &)
{
  return 0;
}

NodeListObj *ELObj::asNodeList()
{
  return 0;
}

NamedNodeListObj *ELObj::asNamedNodeList()
{
  return 0;
}

StyleObj *ELObj::asStyle()
{
  return 0;
}

BoxObj *ELObj::asBox()
{
  return 0;
}

void ELObj::print(Interpreter &, OutputCharStream &out)
{
  out << "#<unknown object " << (unsigned long)this << ">";
}

bool ELObj::exactIntegerValue(long &)
{
  return false;
}

bool ELObj::realValue(double &)
{
  return false;
}

bool ELObj::inexactRealValue(double &)
{
  return false;
}

bool ELObj::lengthValue(long &n)
{
  return false;
}

ELObj::QuantityType ELObj::quantityValue(long &, double &, int &)
{
  return noQuantity;
}

const LengthSpec *ELObj::lengthSpec() const
{
  return 0;
}

bool ELObj::stringData(const Char *&, size_t &)
{
  return false;
}

bool ELObj::charValue(Char &)
{
  return false;
}

const FOTBuilder::GlyphId *ELObj::glyphId() const
{
  return 0;
}

ELObj *ELObj::resolveQuantities(bool, Interpreter &, const Location &)
{
  return this;
}

ErrorObj::ErrorObj()
{
}

UnspecifiedObj::UnspecifiedObj()
{
}

NilObj::NilObj()
{
}

bool NilObj::isNil() const
{
  return true;
}

bool NilObj::isList() const
{
  return true;
}

void NilObj::print(Interpreter &, OutputCharStream &out)
{
  out << "()";
}

TrueObj::TrueObj()
{
}

void TrueObj::print(Interpreter &, OutputCharStream &out)
{
  out << "#t";
}

FalseObj::FalseObj()
{
}

bool FalseObj::isTrue() const
{
  return false;
}

void FalseObj::print(Interpreter &, OutputCharStream &out)
{
  out << "#f";
}

SymbolObj::SymbolObj(StringObj *name)
: name_(name), cValue_(FOTBuilder::symbolFalse)
{
  hasSubObjects_ = 1;
}

void SymbolObj::traceSubObjects(Collector &c) const
{
  c.trace(name_);
}

void SymbolObj::print(Interpreter &interp, OutputCharStream &out)
{
  out.write(name()->data(), name()->size());
}

SymbolObj *SymbolObj::asSymbol()
{
  return this;
}

StringObj *SymbolObj::convertToString()
{
  return name();
}

KeywordObj::KeywordObj(const Identifier *ident)
: ident_(ident)
{
}

KeywordObj *KeywordObj::asKeyword()
{
  return this;
}

bool KeywordObj::isEqual(ELObj &obj)
{
  KeywordObj *k = obj.asKeyword();
  return k && k->ident_ == ident_;
}

void KeywordObj::print(Interpreter &interp, OutputCharStream &out)
{
  out << ident_->name() << ":";
}

PairObj::PairObj(ELObj *car, ELObj *cdr)
: car_(car), cdr_(cdr)
{
  hasSubObjects_ = 1;
}

bool PairObj::isEqual(ELObj &obj)
{
  PairObj *p = obj.asPair();
  // FIXME need non-recursive implementation
  return p && *p->car() == *car() && *p->cdr() == *cdr();
}

void PairObj::traceSubObjects(Collector &c) const
{
  c.trace(car_);
  c.trace(cdr_);
}

ELObj *PairObj::resolveQuantities(bool force, Interpreter &interp,
				  const Location &loc)
{
  bool fail = 0;
  PairObj *pair = this;
  for (;;) {
    ELObj *tem = pair->car_->resolveQuantities(force, interp, loc);
    if (!tem)
      fail = 1;
    else
      pair->car_ = tem;
    PairObj *nextPair = pair->cdr_->asPair();
    if (!nextPair)
      break;
    pair = nextPair;
  }
  ELObj *tem = pair->cdr_->resolveQuantities(force, interp, loc);
  if (!tem)
    fail = 1;
  else
    pair->cdr_ = tem;
  if (fail)
    return 0;
  else
    return this;
}


PairObj *PairObj::asPair()
{
  return this;
}

bool PairObj::isList() const
{
  ELObj *p = cdr_;
  while (!p->isNil()) {
    PairObj *pair = p->asPair();
    if (!pair)
      return false;
    p = pair->cdr();
  }
  return true;
}

void PairObj::print(Interpreter &interp, OutputCharStream &out)
{
  out << "(";
  car_->print(interp, out);
  ELObj *p = cdr_;
  for (;;) {
    if (p->isNil()) {
      out << ")";
      return;
    }
    PairObj *pair = p->asPair();
    if (!pair) {
      out << " . ";
      p->print(interp, out);
      out << ")";
      return;
    }
    out << " ";
    pair->car()->print(interp, out);
    p = pair->cdr();
  }
}

CharObj::CharObj(Char ch)
: ch_(ch)
{
}

void CharObj::display(Interpreter &interp, OutputCharStream &out) const
{
  out.put(ch_);		// FIXME
}

void CharObj::print(Interpreter &, OutputCharStream &out)
{
  out << "#\\";
  out.put(ch_);	// FIXME
}

bool CharObj::charValue(Char &c)
{
  c = ch_;
  return 1;
}

bool CharObj::isEqual(ELObj &obj)
{
  Char c;
  return obj.charValue(c) && c == ch_;
}

StringObj::StringObj()
{
}

StringObj::StringObj(const StringC &str)
: StringC(str)
{
}

StringObj::StringObj(const Char *s, size_t len)
: StringC(s, len)
{
}

bool StringObj::stringData(const Char *&s, size_t &n)
{
  s = data();
  n = size();
  return true;
}

bool StringObj::isEqual(ELObj &obj)
{
  const Char *s;
  size_t n;
  return (obj.stringData(s, n) 
	  && n == size()
	  && (n == 0 || memcmp(s, data(), n*sizeof(Char)) == 0));
}

StringObj *StringObj::convertToString()
{
  return this;
}

void StringObj::print(Interpreter &interp, OutputCharStream &out)
{
  // FIXME
  out << "\"";
  const Char *s = data();
  for (size_t i = 0; i < size(); i++)
    switch (s[i]) {
    case '\\':
    case '"':
      out << "\\";
      // fall through
    default:
      out.put(s[i]);
      break;
    }
  out << "\"";
}

IntegerObj::IntegerObj()
: n_(0)
{
}

IntegerObj::IntegerObj(long n)
: n_(n)
{
}

bool IntegerObj::isEqual(ELObj &obj)
{
  long n;
  return obj.exactIntegerValue(n) && n == n_;
}

void IntegerObj::print(Interpreter &, OutputCharStream &out)
{
  if (n_ < 0)
    out << '-' << (unsigned long)-n_;
  else
    out << (unsigned long)n_;
}

bool IntegerObj::exactIntegerValue(long &n)
{
  n = n_;
  return true;
}

bool IntegerObj::realValue(double &n)
{
  n = n_;
  return true;
}

ELObj::QuantityType IntegerObj::quantityValue(long &val, double &, int &dim)
{
  val = n_;
  dim = 0;
  return longQuantity;
}

RealObj::RealObj(double n)
: n_(n)
{
}

bool RealObj::isEqual(ELObj &obj)
{
  double n;
  return obj.inexactRealValue(n) && n == n_;
}


bool RealObj::realValue(double &n)
{
  n = n_;
  return true;
}

bool RealObj::inexactRealValue(double &n)
{
  n = n_;
  return true;
}

ELObj::QuantityType RealObj::quantityValue(long &, double &val, int &dim)
{
  val = n_;
  dim = 0;
  return doubleQuantity;
}

void RealObj::print(Interpreter &, OutputCharStream &out)
{
  char buf[1024];
  sprintf(buf, "%g", n_);
  out << buf;
}

LengthObj::LengthObj(long n)
: n_(n)
{
}

bool LengthObj::lengthValue(long &n)
{
  n = n_;
  return true;
}

ELObj::QuantityType LengthObj::quantityValue(long &val, double &, int &dim)
{
  val = n_;
  dim = 1;
  return longQuantity;
}

bool LengthObj::isEqual(ELObj &obj)
{
  long n;
  double d;
  int dim;
  switch (obj.quantityValue(n, d, dim)) {
  case noQuantity:
    break;
  case doubleQuantity:
    return dim == 1 && d == n_;
  case longQuantity:
    return dim == 1 && n == n_;
  }
  return 0;
}

void LengthObj::print(Interpreter &interp, OutputCharStream &out)
{
  char buf[1024];
  sprintf(buf, "%gpt", n_ * 72.0/interp.unitsPerInch());
  out << buf;
}

QuantityObj::QuantityObj(double val, int dim)
: val_(val), dim_(dim)
{
}

bool QuantityObj::isEqual(ELObj &obj)
{
  long n;
  double d;
  int dim;
  switch (obj.quantityValue(n, d, dim)) {
  case noQuantity:
    break;
  case doubleQuantity:
    return dim == dim_ && d == val_;
  case longQuantity:
    return dim == dim_ && n == val_;
  }
  return 0;
}

ELObj::QuantityType QuantityObj::quantityValue(long &, double &val, int &dim)
{
  val = val_;
  dim = dim_;
  return doubleQuantity;
}

void QuantityObj::print(Interpreter &interp, OutputCharStream &out)
{
  char buf[1024];
  sprintf(buf, "%gpt%d", val_ * pow(72.0/interp.unitsPerInch(), double(dim_)),
	  dim_);
  out << buf;
}

bool QuantityObj::realValue(double &d)
{
  if (dim_ != 0)
    return 0;
  d = val_;
  return 1;
}

bool QuantityObj::inexactRealValue(double &d)
{
  if (dim_ != 0)
    return 0;
  d = val_;
  return 1;
}

LengthSpecObj::LengthSpecObj(const LengthSpec &spec)
: lengthSpec_(new LengthSpec(spec))
{
}

const LengthSpec *LengthSpecObj::lengthSpec() const
{
  return lengthSpec_.pointer();
}

LengthSpec::LengthSpec()
{
  for (int i = 0; i < nVals; i++)
    val_[i] = 0.0;
}

LengthSpec::LengthSpec(double d)
{
  val_[0] = d;
  for (int i = 1; i < nVals; i++)
    val_[i] = 0.0;
}
 
LengthSpec::LengthSpec(Unknown unknown, double d)
{
  int i;
  for (i = 0; i < unknown; i++)
    val_[i] = 0.0;
  val_[unknown] = d;
  for (i = unknown + 1; i < nVals; i++)
    val_[i] = 0.0;
}

void LengthSpec::operator+=(const LengthSpec &ls)
{
  for (int i = 0; i < nVals; i++)
    val_[i] += ls.val_[i];
}

void LengthSpec::operator-=(const LengthSpec &ls)
{
  for (int i = 0; i < nVals; i++)
    val_[i] -= ls.val_[i];
}

void LengthSpec::operator*=(double d)
{
  for (int i = 0; i < nVals; i++)
    val_[i] *= d;
}

void LengthSpec::operator/=(double d)
{
  for (int i = 0; i < nVals; i++)
    val_[i] /= d;
}

bool LengthSpec::convert(FOTBuilder::LengthSpec &result) const
{
  // FIXME do some checking
  result.length = long(val_[0] < 0.0 ? val_[0] - 0.5 : val_[0] + .5);
  result.displaySizeFactor = val_[1];
  return 1;
}

bool LengthSpec::convert(FOTBuilder::TableLengthSpec &result) const
{
  // FIXME do some checking
  result.length = long(val_[0] < 0.0 ? val_[0] - 0.5 : val_[0] + .5);
  result.displaySizeFactor = val_[1];
  result.tableUnitFactor = val_[2];
  return 1;
}

DisplaySpaceObj::DisplaySpaceObj(const FOTBuilder::DisplaySpace &displaySpace)
: displaySpace_(new FOTBuilder::DisplaySpace(displaySpace))
{
}
 
const FOTBuilder::DisplaySpace &DisplaySpaceObj::displaySpace() const
{
  return *displaySpace_;
}

DisplaySpaceObj *DisplaySpaceObj::asDisplaySpace()
{
  return this;
}

InlineSpaceObj::InlineSpaceObj(const FOTBuilder::InlineSpace &inlineSpace)
: inlineSpace_(new FOTBuilder::InlineSpace(inlineSpace))
{
}
 
const FOTBuilder::InlineSpace &InlineSpaceObj::inlineSpace() const
{
  return *inlineSpace_;
}

InlineSpaceObj *InlineSpaceObj::asInlineSpace()
{
  return this;
}

UnresolvedLengthObj::UnresolvedLengthObj(long val, int valExp, Unit *unit)
: val_(val), valExp_(valExp), unit_(unit)
{
}

ELObj *UnresolvedLengthObj::resolveQuantities(bool force,
					      Interpreter &interp,
					      const Location &loc)
{
  unsigned part;
  Location defLoc;
  if (!unit_->defined(part, defLoc)) {
    interp.setNextLocation(loc);
    interp.message(InterpreterMessages::undefinedQuantity,
		   StringMessageArg(unit_->name()));
    return interp.makeError();
  }
  return unit_->resolveQuantity(force, interp, val_, valExp_);
}

UnresolvedQuantityObj
::UnresolvedQuantityObj(double val, Unit *unit, int unitExp)
: val_(val), unit_(unit), unitExp_(unitExp)
{
}

ELObj *UnresolvedQuantityObj::resolveQuantities(bool force,
						Interpreter &interp,
						const Location &loc)
{
  unsigned part;
  Location defLoc;
  if (!unit_->defined(part, defLoc)) {
    interp.setNextLocation(loc);
    interp.message(InterpreterMessages::undefinedQuantity,
		   StringMessageArg(unit_->name()));
    return interp.makeError();
  }
  return unit_->resolveQuantity(force, interp, val_, unitExp_);
}

GlyphIdObj::GlyphIdObj(const FOTBuilder::GlyphId &glyphId)
: glyphId_(glyphId)
{
}

const FOTBuilder::GlyphId *GlyphIdObj::glyphId() const
{
  return &glyphId_;
}

bool GlyphIdObj::isEqual(ELObj &obj)
{
  const FOTBuilder::GlyphId *p = obj.glyphId();
  return (p
          && p->publicId == glyphId_.publicId
	  && p->suffix == glyphId_.suffix);
}

GlyphSubstTableObj::GlyphSubstTableObj(const ConstPtr<FOTBuilder::GlyphSubstTable> &table)
: table_(table)
{
}

GlyphSubstTableObj *GlyphSubstTableObj::asGlyphSubstTable()
{
  return this;
}

AddressObj::AddressObj(FOTBuilder::Address::Type type,
		       const NodePtr &node,
		       const StringC &str1,
		       const StringC &str2,
		       const StringC &str3)
{
  address_ = new FOTBuilder::Address;
  address_->type = type;
  address_->node = node;
  address_->params[0] = str1;
  address_->params[1] = str2;
  address_->params[2] = str3;
}

AddressObj *AddressObj::asAddress()
{
  return this;
}

NodeListObj *NodeListObj::asNodeList()
{
  return this;
}

NodeListObj *NodeListObj::nodeListNoOrder(QueryContext &)
{
  return this;
}

NodeListObj *NodeListObj::nodeListChunkRest(QueryContext &c)
{
  return nodeListRest(c);
}

bool NodeListObj::optSingletonNodeList(QueryContext &c, NodePtr &node)
{
  NodeListObj *rest = nodeListRest(c);
  ELObjDynamicRoot protect(c, rest);
  if (rest->nodeListFirst(c))
    return 0;
  node = nodeListFirst(c);
  return 1;
}

bool NodeListObj::chunkComplete()
{
  return 1;
}

NamedNodeListObj *NamedNodeListObj::asNamedNodeList()
{
  return this;
}

NodePtrNodeListObj::NodePtrNodeListObj()
{
}

NodePtrNodeListObj::NodePtrNodeListObj(const NodePtr &node)
: node_(node)
{
}

bool NodePtrNodeListObj::optSingletonNodeList(QueryContext &, NodePtr &node)
{
  node = node_;
  return 1;
}

NodePtr NodePtrNodeListObj::nodeListFirst(QueryContext &)
{
  return node_;
}

NodeListObj *NodePtrNodeListObj::nodeListRest(QueryContext &c)
{
  if (node_)
    return new (c) NodePtrNodeListObj;
  else
    return this;
}

bool NodePtrNodeListObj::chunkComplete()
{
  return 0;
}

NodeListPtrNodeListObj::NodeListPtrNodeListObj(const NodeListPtr &nodeList)
: nodeList_(nodeList)
{
}

NodePtr NodeListPtrNodeListObj::nodeListFirst(QueryContext &)
{
  NodePtr nd;
  if (nodeList_->first(nd) == accessOK)
    return nd;
  else
    return NodePtr();
}

NodeListObj *NodeListPtrNodeListObj::nodeListRest(QueryContext &c)
{
  NodeListPtr nl;
  if (nodeList_->rest(nl) == accessOK)
    return new (c) NodeListPtrNodeListObj(nl);
  else
    return new (c) NodePtrNodeListObj;
}

NodeListObj *NodeListPtrNodeListObj::nodeListChunkRest(QueryContext &c)
{
  NodeListPtr nl;
  if (nodeList_->chunkRest(nl) == accessOK)
    return new (c) NodeListPtrNodeListObj(nl);
  else
    return new (c) NodePtrNodeListObj;
}

NamedNodeListPtrNodeListObj
::NamedNodeListPtrNodeListObj(const NamedNodeListPtr &nnl)
: namedNodeList_(nnl)
{
}

NodePtr NamedNodeListPtrNodeListObj::namedNode(const Char *s, size_t n)
{
  NodePtr node;
  if (namedNodeList_->namedNode(GroveString(s, n), node) != accessOK)
    node.clear();
  return node;
}

bool NamedNodeListPtrNodeListObj::nodeName(const NodePtr &nd, GroveString &str)
{
  return namedNodeList_->nodeName(nd, str) == accessOK;
}

NodePtr NamedNodeListPtrNodeListObj::nodeListFirst(QueryContext &)
{
  if (!nodeList_)
    nodeList_ = namedNodeList_->nodeList();
  NodePtr nd;
  if (nodeList_->first(nd) == accessOK)
    return nd;
  else
    return NodePtr();
}
 
NodeListObj *NamedNodeListPtrNodeListObj::nodeListRest(QueryContext &c)
{
  if (!nodeList_)
    nodeList_ = namedNodeList_->nodeList();
  NodeListPtr nl;
  if (nodeList_->rest(nl) == accessOK)
    return new (c) NodeListPtrNodeListObj(nl);
  else
    return new (c) NodePtrNodeListObj;
}

NodeListObj *NamedNodeListPtrNodeListObj::nodeListNoOrder(QueryContext &c)
{
  return new (c) NodeListPtrNodeListObj(namedNodeList_->nodeListNoOrder());
}

SelectElementsNodeListObj::SelectElementsNodeListObj(NodeListObj *nodeList,
						     const ConstPtr<ElementPattern> &pattern)
: nodeList_(nodeList), pattern_(pattern)
{
  hasSubObjects_ = 1;
}

void SelectElementsNodeListObj::traceSubObjects(Collector &c) const
{
  c.trace(nodeList_);
}

NodePtr SelectElementsNodeListObj::nodeListFirst(QueryContext &c)
{
  for (;;) {
    NodePtr nd = nodeList_->nodeListFirst(c);
    if (!nd || pattern_->matches(nd, c))
      return nd;
    nodeList_ = nodeList_->nodeListChunkRest(c);
  }
  // not reached
  return NodePtr();
}

NodeListObj *SelectElementsNodeListObj::nodeListRest(QueryContext &c)
{
  ASSERT(!pattern_.isNull());
  for (;;) {
    NodePtr nd = nodeList_->nodeListFirst(c);
    if (!nd || pattern_->matches(nd, c))
      break;
    nodeList_ = nodeList_->nodeListChunkRest(c);
  }
  NodeListObj *tem = nodeList_->nodeListChunkRest(c);
  ELObjDynamicRoot protect(c, tem);
  return new (c) SelectElementsNodeListObj(tem, pattern_);
}

ElementPattern::~ElementPattern()
{
}

#ifdef DSSSL_NAMESPACE
}
#endif
