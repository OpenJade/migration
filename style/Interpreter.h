// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#ifndef Interpreter_INCLUDED
#define Interpreter_INCLUDED 1

#include "ELObj.h"
#include "Expression.h"
#include "Message.h"
#include "PointerTable.h"
#include "NamedTable.h"
#include "Collector.h"
#include "InputSource.h"
#include "Owner.h"
#include "Style.h"
#include "SosofoObj.h"
#include "ProcessingMode.h"
#include "XcharMap.h"
#include "NumberCache.h"
#include "HashTable.h"
#include "FOTBuilder.h"
#include "Owner.h"
#include "Node.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

class Interpreter;

class Identifier : public Named {
public:
  enum SyntacticKey {
    notKey,
    keyQuote,
    keyLambda,
    keyIf,
    keyCond,
    keyAnd, 
    keyOr,
    keyCase,
    keyLet, 
    keyLetStar, 
    keyLetrec,
    keyQuasiquote,
    keyUnquote, 
    keyUnquoteSplicing,
    keyDefine,
    keyElse,
    keyArrow,
    keyMake,
    keyStyle,
    keyWithMode,
    keyDefineUnit,
    keyElement,
    keyDefault,
    keyRoot,
    keyId,
    keyMode,
    keyDeclareInitialValue,
    keyDeclareCharacteristic,
    keyDeclareFlowObjectClass,
    keyDeclareCharCharacteristicAndProperty,
    keyDeclareReferenceValueType,
    keyDeclareDefaultLanguage,
    keyDeclareCharProperty,
    keyDefinePageModel,
    keyDefineColumnSetModel,
    keyDefineLanguage,
    keyAddCharProperties,
    keyUse,
    keyLabel,
    keyContentMap,
    keyIsKeepWithPrevious,
    keyIsKeepWithNext,
    keySpaceBefore,
    keySpaceAfter,
    keyLeftHeader,
    keyCenterHeader,
    keyRightHeader,
    keyLeftFooter,
    keyCenterFooter,
    keyRightFooter,
    keyDestination,
    keyType,
    keyCoalesceId,
    keyIsDisplay,
    keyScale,
    keyMaxWidth,
    keyMaxHeight,
    keyEntitySystemId,
    keyNotationSystemId,
    keyPositionPointX,
    keyPositionPointY,
    keyEscapementDirection,
    keyBreakBeforePriority,
    keyBreakAfterPriority,
    keyOrientation,
    keyLength,
    keyChar,
    keyGlyphId,
    keyIsSpace,
    keyIsRecordEnd,
    keyIsInputTab,
    keyIsInputWhitespace,
    keyIsPunct,
    keyIsDropAfterLineBreak,
    keyIsDropUnlessBeforeLineBreak,
    keyMathClass,
    keyMathFontPosture,
    keyScript,
    keyStretchFactor,
    keyKeep,
    keyBreakBefore,
    keyBreakAfter,
    keyIsMayViolateKeepBefore,
    keyIsMayViolateKeepAfter,
    keyBeforeRowBorder,
    keyAfterRowBorder,
    keyBeforeColumnBorder,
    keyAfterColumnBorder,
    keyColumnNumber,
    keyNColumnsSpanned,
    keyNRowsSpanned,
    keyWidth,
    keyIsStartsRow,
    keyIsEndsRow,
    keyTableWidth,
    keyMultiModes,
    keyData,
    keyMin,
    keyMax,
    keyIsConditional,
    keyPriority
  };
  enum { lastSyntacticKey = keyWithMode };
  Identifier(const StringC &name);
  // Return 0 is value can't yet be computed.
  ELObj *computeValue(bool force, Interpreter &) const;
  bool syntacticKey(SyntacticKey &) const;
  void setSyntacticKey(SyntacticKey);
  bool defined(unsigned &, Location &) const;
  void setDefinition(Owner<Expression> &, unsigned part,
                     const Location &);
  void setValue(ELObj *, unsigned defPart = unsigned(-1));
  bool evaluated() const;
  const ConstPtr<InheritedC> &inheritedC() const;
  void setInheritedC(const ConstPtr<InheritedC> &);
  FlowObj *flowObj() const;
  void setFlowObj(FlowObj *);
private:
  unsigned defPart_;
  Owner<Expression> def_;
  InsnPtr insn_;
  // Value in top-level environment.
  ELObj *value_;		// must be permanent
  FlowObj *flowObj_;    // prototype FlowObj with this name
  Location defLoc_;
  SyntacticKey syntacticKey_;
  bool beingComputed_;
  ConstPtr<InheritedC> inheritedC_;
};

class Unit : public Named {
public:
  Unit(const StringC &);
  void setValue(long);
  void setValue(double);
  bool defined(unsigned &, Location &) const;
  // return 0 if it can't be done
  ELObj *resolveQuantity(bool force, Interpreter &, double val, int unitExp);
  ELObj *resolveQuantity(bool force, Interpreter &, long val, int valExp);
  void setDefinition(Owner<Expression> &, unsigned part, const Location &);
private:
  void tryCompute(bool force, Interpreter &);
  static bool scale(long val, int valExp, long num, long &result);

  unsigned defPart_;
  Location defLoc_;
  Owner<Expression> def_;
  InsnPtr insn_;
  enum {
    notComputed,
    beingComputed,
    computedExact,
    computedInexact,
    computedError
    } computed_;
  union {
    long exact_;
    double inexact_;
  };
  int dim_;
};

class ELObjDynamicRoot : public Collector::DynamicRoot {
public:
  ELObjDynamicRoot(Collector &c, ELObj *obj = 0)
    : Collector::DynamicRoot(c), obj_(obj) { }
  void operator=(ELObj *obj) { obj_ = obj; }
  operator ELObj *() const { return obj_; }
private:
  void trace(Collector &) const;
  ELObj *obj_;
};

class Interpreter : public QueryContext, public NumberCache, public Messenger {
public:
  enum PortName {
    portNumerator,
    portDenominator,
    portHeader,
    portFooter
  };
  enum { nPortNames = portFooter + 1 };
  Interpreter(const NodePtr &rootNode, Messenger *, int unitsPerInch, const FOTBuilder::Extension *);
  void defineVariable(const StringC &);
  void parse(Owner<InputSource> &);
  void endPart();
  FalseObj *makeFalse();
  TrueObj *makeTrue();
  NilObj *makeNil();
  SymbolObj *makeSymbol(const StringC &);
  KeywordObj *makeKeyword(const StringC &);
  IntegerObj *makeInteger(long n);
  ErrorObj *makeError();
  UnspecifiedObj *makeUnspecified();
  PairObj *makePair(ELObj *, ELObj *);
  ELObj *convertGlyphId(const Char *, size_t, const Location &);
  bool isError(const ELObj *) const;
  bool isUnspecified(const ELObj *) const;
  CharObj *makeChar(Char);
  ELObj *makeLengthSpec(const FOTBuilder::LengthSpec &);
  void dispatchMessage(Message &);
  void dispatchMessage(const Message &);
  void initMessage(Message &);
  Identifier *lookup(const StringC &);
  Unit *lookupUnit(const StringC &);
  FunctionObj *lookupExternalProc(const StringC &);
  int unitsPerInch() const;
  unsigned currentPartIndex() const;
  const ProcessingMode *initialProcessingMode() const;
  void compile();
  static StringC makeStringC(const char *);
  SymbolObj *portName(PortName);
  ELObj *cValueSymbol(FOTBuilder::Symbol);
  ELObj *convertNumber(const StringC &, bool giveError, int radix = 10);
  ELObj *convertAfiiGlyphId(const StringC &);
  void normalizeGeneralName(StringC &);
  void normalizeEntityName(StringC &);
  const NodePtr &rootNode() const;
  StyleObj *initialStyle() const;
  StyleObj *borderTrueStyle() const;
  StyleObj *borderFalseStyle() const;
  bool convertBooleanC(ELObj *, const Identifier *, const Location &, bool &);
  bool convertPublicIdC(ELObj *, const Identifier *, const Location &,
			FOTBuilder::PublicId &);
  bool convertStringC(ELObj *, const Identifier *, const Location &, StringC &);
  bool convertLengthSpecC(ELObj *, const Identifier *, const Location &, FOTBuilder::LengthSpec &);
  bool convertOptLengthSpecC(ELObj *, const Identifier *, const Location &, FOTBuilder::OptLengthSpec &);
  bool convertCharC(ELObj *, const Identifier *, const Location &, Char &);
  // FIXME allow inexact value
  bool convertIntegerC(ELObj *, const Identifier *, const Location &, long &);
  bool convertRealC(ELObj *, const Identifier *, const Location &, double &);
  bool convertEnumC(const FOTBuilder::Symbol *, size_t,
                    ELObj *, const Identifier *, const Location &, FOTBuilder::Symbol &);
  void invalidCharacteristicValue(const Identifier *ident, const Location &loc);
  bool convertLengthSpec(ELObj *, FOTBuilder::LengthSpec &);
  const ConstPtr<InheritedC> &tableBorderC() const;
  const ConstPtr<InheritedC> &cellBeforeRowBorderC() const;
  const ConstPtr<InheritedC> &cellAfterRowBorderC() const;
  const ConstPtr<InheritedC> &cellBeforeColumnBorderC() const;
  const ConstPtr<InheritedC> &cellAfterColumnBorderC() const;
  const char *storePublicId(const Char *, size_t, const Location &);
  unsigned allocGlyphSubstTableUniqueId();
private:
  Interpreter(const Interpreter &); // undefined
  void operator=(const Interpreter &); // undefined

  enum {
    allowEndOfEntity = 01,
    allowFalse = 02,
    allowKeyword = 04,
    allowOpenParen = 010,
    allowCloseParen = 020,
    allowIdentifier = 040,
    allowPeriod = 0100,
    allowOtherDatum = 0200,  // number, character, glyph-id quote backquote
    allowExpressionKey = 0400,
    allowKeyDefine = 01000,
    allowKeyElse = 02000,
    allowKeyArrow = 04000, // =>
    allowString = 010000,
    allowHashOptional = 020000,
    allowHashKey = 040000,
    allowHashRest = 0100000,
    allowDatum = (allowFalse|allowKeyword|allowOpenParen|allowIdentifier
		  |allowString|allowHashOptional|allowHashKey|allowHashRest
		  |allowOtherDatum)
  };
  enum Token {
    tokenEndOfEntity,
    tokenTrue,
    tokenFalse,
    tokenString,
    tokenIdentifier,
    tokenKeyword,
    tokenChar,
    tokenNumber,
    tokenGlyphId,
    tokenOpenParen,
    tokenCloseParen,
    tokenPeriod,
    tokenQuote,
    tokenHashRest,
    tokenHashOptional,
    tokenHashKey
  };
  enum LexCategory {
    lexLetter,			// a - z A - Z
    lexOtherNameStart,		// 
    lexDigit,			// 0-9
    lexOtherNumberStart,	// -+.
    lexDelimiter,		// ;()"
    lexWhiteSpace,
    lexOther
  };
  bool doDefine();
  bool doDefineUnit();
  bool doElement();
  bool doDefault();
  bool doId();
  bool doRoot();
  bool doMode();
  bool doDeclareInitialValue();
  bool doDeclareCharacteristic();
  bool doDeclareFlowObjectClass();
  bool skipForm();
  void installInitialValue(Identifier *, Owner<Expression> &);
  void installExtensionInheritedC(Identifier *, const StringC &);
  void installExtensionFlowObjectClass(Identifier *, const StringC &);
  void installSyntacticKeys();
  void installPortNames();
  void installCValueSymbols();
  void installPrimitives();
  void installPrimitive(const char *s, PrimitiveObj *value);
  void installXPrimitive(const char *s, PrimitiveObj *value);
  void installUnits();
  void installCharNames();
  void installInheritedCs();
  void installInheritedCProc(const Identifier *);
  void installFlowObjs();
  void installSdata();
  bool parseExpression(unsigned allowed, Owner<Expression> &,
		       Identifier::SyntacticKey &, Token &);
  bool parseLambda(Owner<Expression> &);
  bool parseLet(Owner<Expression> &);
  bool parseLetStar(Owner<Expression> &);
  bool parseLetrec(Owner<Expression> &);
  bool parseBindingsAndBody(Vector<const Identifier *> &vars,
			    NCVector<Owner<Expression> > &inits,
			    Owner<Expression> &body);
  bool parseBindingsAndBody1(Vector<const Identifier *> &vars,
			     NCVector<Owner<Expression> > &inits,
			     Owner<Expression> &body);
  bool parseQuote(Owner<Expression> &);
  bool parseIf(Owner<Expression> &);
  bool parseCond(Owner<Expression> &, bool opt = 0);
  bool parseCase(Owner<Expression> &);
  bool parseOr(Owner<Expression> &);
  bool parseAnd(Owner<Expression> &, bool opt = 0);
  bool parseMake(Owner<Expression> &);
  bool parseStyle(Owner<Expression> &);
  bool parseWithMode(Owner<Expression> &);
  bool parseFormals(Vector<const Identifier *> &,
		    NCVector<Owner<Expression> > &,
		    int &, bool &, int &);
  bool parseDatum(unsigned otherAllowed, ELObj *&, Location &, Token &);
  bool parseSelfEvaluating(unsigned otherAllowed, ELObj *&, Token &);
  bool getToken(unsigned, Token &);
  bool isDelimiter(Xchar);
  void extendToken();
  bool scanString();
  void skipComment();
  bool tokenRecover(unsigned, Token &);
  bool tokenIsNumber();
  bool scanString(unsigned, Token &);
  ELObj *convertNumberFloat(const StringC &, bool giveError);
  bool scanSignDigits(const StringC &str, size_t &i, int &n);
  Unit *scanUnit(const StringC &str, size_t i, bool giveError, int &unitExp);
  ProcessingMode *lookupProcessingMode(const StringC &);
  void compileInitialValues();
  bool sdataMap(GroveString, GroveString, GroveChar &) const;
  bool convertCharName(const StringC &str, Char &c) const;
  static bool convertUnicodeCharName(const StringC &str, Char &c);

  NilObj *theNilObj_;
  TrueObj *theTrueObj_;
  FalseObj *theFalseObj_;
  ErrorObj *theErrorObj_;
  UnspecifiedObj *theUnspecifiedObj_;
  typedef PointerTable<SymbolObj *, StringC, Hash, SymbolObj>
    SymbolTable;
  SymbolTable symbolTable_;
  NamedTable<Identifier> identTable_;
  NamedTable<Unit> unitTable_;
  HashTable<StringC,FunctionObj *> externalProcTable_;
  Messenger *messenger_;
  const FOTBuilder::Extension *extensionTable_;
  Owner<InputSource> in_;
  StringC currentToken_;
  unsigned partIndex_;
  int unitsPerInch_;
  unsigned nInheritedC_;
  NodePtr rootNode_;
  NamedNodeListPtr elementsNamedNodeList_;
  NamedNodeListPtr entitiesNamedNodeList_;
  ProcessingMode initialProcessingMode_;
  NamedTable<ProcessingMode> processingModeTable_;
  ProcessingMode *defMode_;
  SymbolObj *portNames_[nPortNames];
  ELObj *cValueSymbols_[FOTBuilder::nSymbols];
  // Map of LexCategory
  XcharMap<char> lexCategory_;
  HashTable<StringC,Char> namedCharTable_;
  Vector<const Identifier *> initialValueNames_;
  NCVector<Owner<Expression> > initialValueValues_;
  size_t currentPartFirstInitialValue_;
  StyleObj *initialStyle_;
  StyleObj *borderTrueStyle_;
  StyleObj *borderFalseStyle_;
  ConstPtr<InheritedC> tableBorderC_;
  ConstPtr<InheritedC> cellBeforeRowBorderC_;
  ConstPtr<InheritedC> cellAfterRowBorderC_;
  ConstPtr<InheritedC> cellBeforeColumnBorderC_;
  ConstPtr<InheritedC> cellAfterColumnBorderC_;
  class StringSet {
  public:
    StringSet();
    const char *store(String<char> &);
    static unsigned long hash(const String<char> &);
    static inline const String<char> &key(const String<char> &str) { return str; }
  private:
    OwnerTable<String<char>, String<char>, StringSet, StringSet> table_;
  };
  StringSet publicIds_;
  HashTable<StringC,Char> sdataEntityNameTable_;
  const char *afiiPublicId_;
  unsigned nextGlyphSubstTableUniqueId_;
  friend class Identifier;
};

inline
ErrorObj *Interpreter::makeError()
{
  return theErrorObj_;
}

inline
bool Interpreter::isError(const ELObj *obj) const
{
  return obj == theErrorObj_;
}

inline
bool Interpreter::isUnspecified(const ELObj *obj) const
{
  return obj == theUnspecifiedObj_;
}

inline
FalseObj *Interpreter::makeFalse()
{
  return theFalseObj_;
}

inline
TrueObj *Interpreter::makeTrue()
{
  return theTrueObj_;
}

inline
NilObj *Interpreter::makeNil()
{
  return theNilObj_;
}

inline
UnspecifiedObj *Interpreter::makeUnspecified()
{
  return theUnspecifiedObj_;
}

inline
IntegerObj *Interpreter::makeInteger(long n)
{
  return new (*this) IntegerObj(n);
}

inline
PairObj *Interpreter::makePair(ELObj *car, ELObj *cdr)
{
  return new (*this) PairObj(car, cdr);
}

inline
CharObj *Interpreter::makeChar(Char c)
{
  return new (*this) CharObj(c);
}

inline
ELObj *Interpreter::cValueSymbol(FOTBuilder::Symbol sym)
{
  return cValueSymbols_[sym];
}

inline
SymbolObj *Interpreter::portName(PortName i)
{
  return portNames_[i];
}

inline
const ProcessingMode *Interpreter::initialProcessingMode() const
{
  return &initialProcessingMode_;
}

inline
int Interpreter::unitsPerInch() const
{
  return unitsPerInch_;
}

inline
unsigned Interpreter::currentPartIndex() const
{
  return partIndex_;
}

inline
KeywordObj *Interpreter::makeKeyword(const StringC &str)
{
  return new (*this) KeywordObj(lookup(str));
}

inline
StyleObj *Interpreter::initialStyle() const
{
  return initialStyle_;
}

inline
StyleObj *Interpreter::borderTrueStyle() const
{
  return borderTrueStyle_;
}

inline
StyleObj *Interpreter::borderFalseStyle() const
{
  return borderFalseStyle_;
}

inline
const NodePtr &Interpreter::rootNode() const
{
  return rootNode_;
}

inline
const ConstPtr<InheritedC> &Interpreter::tableBorderC() const
{
  return tableBorderC_;
}

inline
const ConstPtr<InheritedC> &Interpreter::cellBeforeRowBorderC() const
{
  return cellBeforeRowBorderC_;
}

inline
const ConstPtr<InheritedC> &Interpreter::cellAfterRowBorderC() const
{
  return cellAfterRowBorderC_;
}

inline
const ConstPtr<InheritedC> &Interpreter::cellBeforeColumnBorderC() const
{
  return cellBeforeColumnBorderC_;
}

inline
const ConstPtr<InheritedC> &Interpreter::cellAfterColumnBorderC() const
{
  return cellAfterColumnBorderC_;
}

inline
FunctionObj *Interpreter::lookupExternalProc(const StringC &pubid)
{
  FunctionObj *const *func = externalProcTable_.lookup(pubid);
  return func ? *func : 0;
}

inline
unsigned Interpreter::allocGlyphSubstTableUniqueId()
{
  return nextGlyphSubstTableUniqueId_++;
}

inline
bool Identifier::syntacticKey(SyntacticKey &key) const
{
  if (syntacticKey_ == notKey)
    return 0;
  key = syntacticKey_;
  return 1;
}

inline
void Identifier::setSyntacticKey(SyntacticKey key)
{
  syntacticKey_ = key;
}

inline
bool Identifier::evaluated() const
{
  return value_ != 0;
}

inline
const ConstPtr<InheritedC> &Identifier::inheritedC() const
{
  return inheritedC_;
}

inline
void Identifier::setInheritedC(const ConstPtr<InheritedC> &ic)
{
  inheritedC_ = ic;
}

inline
FlowObj *Identifier::flowObj() const
{
  return flowObj_;
}

inline
void Identifier::setFlowObj(FlowObj *fo)
{
  flowObj_ = fo;
}

bool operator==(const StringC &, const char *);

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* not Interpreter_INCLUDED */
