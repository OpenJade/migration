// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#ifndef Interpreter_INCLUDED
#define Interpreter_INCLUDED 1

#include "ELObj.h"
#include "Expression.h"
#include <OpenSP/Message.h>
#include <OpenSP/PointerTable.h>
#include <OpenSP/NamedTable.h>
#include "Collector.h"
#include <OpenSP/InputSource.h>
#include <OpenSP/XcharMap.h>
#include <OpenSP/Owner.h>
#include "Style.h"
#include "SosofoObj.h"
#include "ProcessingMode.h"
#include "NumberCache.h"
#include <OpenSP/HashTable.h>
#include "FOTBuilder.h"
#include <OpenSP/Owner.h>
#include <OpenSP/Boolean.h>
#include "Node.h"
#include "GroveManager.h"
#include "Pattern.h"
#include <OpenSP/CharMap.h>
#include "TransformationMode.h"

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
    keySet,
    keyBegin,
    keyThereExists,
    keyForAll,
    keySelectEach,
    keyUnionForEach,
    keyMake,
    keyStyle,
    keyWithMode,
    keyDefineUnit,
    keyQuery,
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
    keyRowNumber,
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
    keyPriority,
    keyGridNRows,
    keyGridNColumns,
    keyRadical,
    keyNull,
    keyIsRcs,
    keyParent,
    keyActive,
    keyAttributes,
    keyChildren,
    keyRepeat,
    keyPosition,
    keyOnly,
    keyClass,
    keyImportance,
    keyDeclareClassAttribute,
    keyDeclareIdAttribute,
    keyDeclareFlowObjectMacro,
    keyOrElement,
    keyPositionPreference,
    keyCollate,
    keyToupper,
    keyTolower,
    keySymbol,
    keyOrder,
    keyForward,
    keyBackward,
    keyWhitePoint,
    keyBlackPoint,
    keyRange,
    keyRangeAbc,
    keyRangeLmn,
    keyRangeA,
    keyDecodeAbc,
    keyDecodeLmn,
    keyDecodeA, 
    keyMatrixAbc,
    keyMatrixLmn,
    keyMatrixA,
    keyArchitecture,
    keyDefineTransliterationMap,
    keyNode,
    keySubgrove,
    keyAdd,
    keyRemove,
    keySub,
    keySortChildren,
    keyOptional,
    keyUnique,
    keyProperty,
    keyResultPath,
  };
  enum { lastSyntacticKey = keyWithMode };
  Identifier(const StringC &name);
  // Return 0 is value can't yet be computed.
  ELObj *computeValue(bool force, Interpreter &) const;
  ELObj *computeBuiltinValue(bool force, Interpreter &) const;
  bool syntacticKey(SyntacticKey &) const;
  void setSyntacticKey(SyntacticKey);
  bool defined(unsigned &, Location &) const;
  void setDefinition(Owner<Expression> &, unsigned part, const Location &);
  void setBuiltinDefinition(Owner<Expression> &, unsigned part, const Location &);
  void setValue(ELObj *, unsigned defPart = unsigned(-1));
  bool evaluated() const;
  const ConstPtr<InheritedC> &inheritedC() const;
  bool inheritedCDefined(unsigned &, Location &) const;
  bool charNICDefined(unsigned &, Location &) const;
  void setCharNIC(unsigned, const Location &);
  void setInheritedC(const ConstPtr<InheritedC> &);
  void setInheritedC(const ConstPtr<InheritedC> &, unsigned, const Location &);
  FlowObj *flowObj() const;
  bool flowObjDefined(unsigned &, Location &) const;
  void setFlowObj(FlowObj *);
  void setFlowObj(FlowObj *, unsigned part, const Location &);
  void setFeature(int);
  bool requireFeature(Interpreter &, const Location &, bool fo = 0) const;
private:
  unsigned defPart_;
  Owner<Expression> def_;
  InsnPtr insn_;
  // Value in top-level environment.
  ELObj *value_;		// must be permanent
  FlowObj *flowObj_;    // prototype FlowObj with this name
  unsigned flowObjPart_;
  Location flowObjLoc_;
  Location defLoc_;
  SyntacticKey syntacticKey_;
  bool beingComputed_;
  bool charNIC_;
  ConstPtr<InheritedC> inheritedC_;
  unsigned inheritedCPart_;
  Location inheritedCLoc_;
  void maybeSaveBuiltin();
  Identifier *builtin_;
  static bool preferBuiltin_;
  friend class Interpreter;
  // FIXME This should be Interpreter::Feature
  int feature_;
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

struct CharPart {
  Char c;
  unsigned defPart;
};

struct ELObjPart {
  ELObjPart();
  ELObjPart(ELObj *x, unsigned y);
  void operator=(const ELObjPart &);
  bool operator==(const ELObjPart &) const;
  bool operator!=(const ELObjPart &) const;
  ELObj *obj;
  unsigned defPart;
};

class CharPropValues {
public:
  virtual bool setDefault(const StringC &, const Location &,
			  ELObj *, Interpreter &)=0;
  virtual bool setValue(const StringC &,const StringC &,const Location &,
			ELObj *, Interpreter &)=0;
  virtual ELObj *value(Char, Interpreter &) const =0;
  virtual ELObj *defaultValue(Interpreter &) const =0;
};

// These have to be in the header file, see Interpreter.

class IntegerCharPropValues : public CharPropValues {
public:
  IntegerCharPropValues(long def=0)
    : def_(def) {}
  long getValue(Char) const;
  void setRange(Char, Char, long);
private:
  IntegerCharPropValues(const IntegerCharPropValues &); // Undefined.
  IntegerCharPropValues &operator=(const IntegerCharPropValues &); // Undefined
  long def_;
  struct ValT_ {
    long l_;
    bool hasValue_;
    ValT_() : hasValue_(false) {}
    ValT_(long l) : hasValue_(true), l_(l) {}
    bool operator==(const ValT_ &v) const
      { return (hasValue_==v.hasValue_) && (!hasValue_ || l_==v.l_); }
    bool operator!=(const ValT_& v) const
      { return !(*this == v); }
  };
  CharMap<ValT_> map_;

  bool setDefault(const StringC &, const Location &,
		  ELObj *, Interpreter &);
  bool setValue(const StringC &, const StringC &, const Location &,
		ELObj *,Interpreter &);
  ELObj *value(Char, Interpreter &) const;
  ELObj *defaultValue(Interpreter &) const;
};

class MaybeIntegerCharPropValues : public CharPropValues {
public:
  MaybeIntegerCharPropValues(bool defFalse=true, long def=0)
    : defFalse_(defFalse), def_(def) {}
  // Returns true and sets 2nd arg if not #f, otherwise returns false.
  bool getValue(Char, long &) const;
  void setRange(Char, Char, long);
  void setValue(Char, long);
private:
  MaybeIntegerCharPropValues(const MaybeIntegerCharPropValues &); // Undefined.
  MaybeIntegerCharPropValues &operator=(const MaybeIntegerCharPropValues &);
  // Above undefined.
  long def_;
  bool defFalse_;
  struct ValT_ {
    long l_;
    bool hasValue_;
    bool isFalse_;
    ValT_() : hasValue_(false) {}
    ValT_(long l) : l_(l), hasValue_(true), isFalse_(false) {}
    bool operator==(const ValT_ &v) const
      { return (hasValue_==v.hasValue_)
	&& (!hasValue_ || (isFalse_ && v.isFalse_) || l_==v.l_); }
    bool operator!=(const ValT_& v) const
      { return !(*this == v); }
  };
  CharMap<ValT_> map_;
  bool setDefault(const StringC &, const Location &,
		  ELObj *, Interpreter &);
  bool setValue(const StringC &, const StringC &, const Location &,
		ELObj *,Interpreter &);
  ELObj *value(Char, Interpreter &) const;
  ELObj *defaultValue(Interpreter &) const;
};

class BooleanCharPropValues : public CharPropValues {
public:
  BooleanCharPropValues(bool def=false)
    : def_(def) {}
  bool getValue(Char) const;
  void setRange(Char, Char, bool);
private:
  BooleanCharPropValues(const BooleanCharPropValues &); // Undefined.
  BooleanCharPropValues &operator=(const BooleanCharPropValues &); // Undefined.
  bool def_;
  struct ValT_ {
    bool b_:1;
    bool hasValue_:1;
    ValT_() : hasValue_(false) {}
    ValT_(bool b) : b_(b), hasValue_(true) {}
    bool operator==(const ValT_ &v) const
      { return (hasValue_==v.hasValue_) && (!hasValue_ || b_==v.b_); }
    bool operator!=(const ValT_& v) const
      { return !(*this == v); }
  };
  CharMap<ValT_> map_;

  bool setDefault(const StringC &, const Location &,
		  ELObj *, Interpreter &);
  bool setValue(const StringC &, const StringC &, const Location &,
		ELObj *,Interpreter &);
  ELObj *value(Char, Interpreter &) const;
  ELObj *defaultValue(Interpreter &) const;
};

class PublicIdCharPropValues : public CharPropValues {
public:
  PublicIdCharPropValues(FOTBuilder::PublicId def=0)
    : def_(def) {}
  FOTBuilder::PublicId getValue(Char) const;
  void setRange(Char, Char, const FOTBuilder::PublicId);
private:
  PublicIdCharPropValues(const PublicIdCharPropValues &); // Undefined.
  PublicIdCharPropValues &operator=(const PublicIdCharPropValues &); // Undefined.
  FOTBuilder::PublicId def_;
  struct ValT_ {
    FOTBuilder::PublicId p_;
    bool hasValue_;
    ValT_() : hasValue_(false) {}
    ValT_(FOTBuilder::PublicId p)
      : hasValue_(true), p_(p) {}
    bool operator==(const ValT_ &v) const
      { return (hasValue_==v.hasValue_) && (!hasValue_ || p_==v.p_); }
    bool operator!=(const ValT_& v) const
      { return !(*this == v); }
  };
  CharMap<ValT_> map_;
  bool setDefault(const StringC &, const Location &,
		  ELObj *, Interpreter &);
  bool setValue(const StringC &, const StringC &, const Location &,
		ELObj *,Interpreter &);
  ELObj *value(Char, Interpreter &) const;
  ELObj *defaultValue(Interpreter &) const;
};

class SymbolCharPropValues : public CharPropValues {
public:
  SymbolCharPropValues(FOTBuilder::Symbol def=FOTBuilder::symbolFalse)
    : def_(def) {}
  FOTBuilder::Symbol getValue(Char) const;
  void setRange(Char, Char, FOTBuilder::Symbol);
private:
  SymbolCharPropValues(const SymbolCharPropValues &); // Undefined.
  SymbolCharPropValues &operator=(const SymbolCharPropValues &); // Undefined.
  FOTBuilder::Symbol def_;
  struct ValT_ {
    FOTBuilder::Symbol s_;
    bool hasValue_;
    ValT_() : hasValue_(false) {}
    ValT_(FOTBuilder::Symbol s)
      : hasValue_(true), s_(s) {}
    bool operator==(const ValT_ &v) const
      { return (hasValue_==v.hasValue_) && (!hasValue_ || s_==v.s_); }
    bool operator!=(const ValT_& v) const
      { return !(*this == v); }
  };
  CharMap<ValT_> map_;
  bool setDefault(const StringC &, const Location &,
		  ELObj *, Interpreter &);
  bool setValue(const StringC &, const StringC &, const Location &,
		ELObj *,Interpreter &);
  ELObj *value(Char, Interpreter &) const;
  ELObj *defaultValue(Interpreter &) const;
};

class CharProp : public Named {
public:
  CharProp(const StringC& name);

  // def is used if char doesn't have property. If def==0, property default
  // value is used.
  ELObj *value(Char ch,ELObj *def, const Location &, Interpreter &);

  // Use only before compile is called.
  // Set value for characters in string.
  void setValue(StringC &,Owner<Expression> &,unsigned,const Location&);
  void declare(Owner<Expression> &, unsigned, const Location&,
	       CharPropValues * =0);
  void declarePredefined(CharPropValues *);
  bool hasAddedValue(Char,unsigned&,Location&) const;
  bool declared(unsigned&,Location&) const;

  void compile(Interpreter &);
protected:
  enum computedType_ {
    notComputed,
    beingComputed,
    computedError,
    computedOK
  };

  // Default value.
  unsigned defPart_;
  Location defLoc_;
  Owner<Expression> defExpr_;
  computedType_ defComputed_;

  // Added properties.
  struct addedProp_ {
    StringC chars_;
    unsigned part_;
    Location loc_;
    Owner<Expression> expr_;
    computedType_ computed_;
  };
  // Have to store in NCVector because of Owner non-copyable.
  NCVector<addedProp_> addedPropsVec_;
  // We have to store indices, since NCVector isn't really NC (ofcourse).
  Owner<CharMap<NCVector<addedProp_>::size_type> > addedProps_;
  enum { noAddedVal_ = NCVector<addedProp_>::size_type(-1) };
  CharPropValues *values_;
  // True if errors when compiling any of the added values.
  bool hadError_;

  void compileDef_(Interpreter &);
  void compileAdded_(addedProp_ &,Interpreter &);
  const addedProp_ *added_(Char) const;
private:
  CharProp(const CharProp &); // Undefined.
  CharProp &operator=(const CharProp &); // Undefined.
};

class Interpreter : 
  public Collector,
  public Pattern::MatchContext,
  public NumberCache,
  public Messenger {
public:
  enum PortName {
    portNumerator,
    portDenominator,
    portPreSup,
    portPreSub,
    portPostSup,
    portPostSub,
    portMidSup,
    portMidSub,
    portOverMark,
    portUnderMark,
    portOpen,
    portClose,
    portDegree,
    portOperator,
    portLowerLimit,
    portUpperLimit,
    portHeader,
    portFooter
  };
  enum { nPortNames = portFooter + 1 };
  Interpreter(GroveManager *, Messenger *, int unitsPerInch, bool debugMode,
	      bool dsssl2, bool style, bool strictMode, 
	      const FOTBuilder::Description &);
  void endPart();
  void dEndPart();
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
  AddressObj *makeAddressNone();
  NodeListObj *makeEmptyNodeList();
  void dispatchMessage(Message &);
  void dispatchMessage(const Message &);
  Identifier *lookup(const StringC &);
  Unit *lookupUnit(const StringC &);
  FunctionObj *lookupExternalProc(const StringC &);
  CharProp *lookupCharProperty(const StringC &);
  int unitsPerInch() const;
  unsigned currentPartIndex() const;
  const FOTBuilder::Description &fotbDescr() const;
  void compile(const NodePtr &);
  static StringC makeStringC(const char *);
  SymbolObj *portName(PortName);
  ELObj *cValueSymbol(FOTBuilder::Symbol);
  void compileCharProperties();
  // Map of LexCategory
  XcharMap<char> lexCategory_;
  static void normalizeGeneralName(const NodePtr &, StringC &);
  GroveManager *groveManager() const;
  StyleObj *initialStyle() const;
  StyleObj *borderTrueStyle() const;
  StyleObj *borderFalseStyle() const;
  bool convertBooleanC(ELObj *, const Identifier *, const Location &, bool &);
  bool convertPublicIdC(ELObj *, const Identifier *, const Location &,
			FOTBuilder::PublicId &);
  bool convertStringC(ELObj *, const Identifier *, const Location &, StringC &);
  bool convertLengthC(ELObj *, const Identifier *, const Location &, FOTBuilder::Length &);
  bool convertLengthSpecC(ELObj *, const Identifier *, const Location &, FOTBuilder::LengthSpec &);
  bool convertLetter2C(ELObj *, const Identifier *, const Location &, FOTBuilder::Letter2 &);
  bool convertOptLengthSpecC(ELObj *, const Identifier *, const Location &, FOTBuilder::OptLengthSpec &);
  bool convertCharC(ELObj *, const Identifier *, const Location &, Char &);
  bool convertColorC(ELObj *, const Identifier *, const Location &, ColorObj *&);
  bool convertOptColorC(ELObj *, const Identifier *, const Location &, ColorObj *&);
  // FIXME allow inexact value
  bool convertIntegerC(ELObj *, const Identifier *, const Location &, long &);
  bool convertOptPositiveIntegerC(ELObj *, const Identifier *, const Location &, long &);
  bool convertRealC(ELObj *, const Identifier *, const Location &, double &);
  bool convertEnumC(const FOTBuilder::Symbol *, size_t,
                    ELObj *, const Identifier *, const Location &, FOTBuilder::Symbol &);
  bool convertEnumC(ELObj *, const Identifier *, const Location &, FOTBuilder::Symbol &);
  void invalidCharacteristicValue(const Identifier *ident, const Location &loc);
  bool convertLengthSpec(ELObj *, FOTBuilder::LengthSpec &);
  bool convertToPattern(ELObj *, const Location &, Pattern &);
  const ConstPtr<InheritedC> &charMapC() const;
  const ConstPtr<InheritedC> &tableBorderC() const;
  const ConstPtr<InheritedC> &cellBeforeRowBorderC() const;
  const ConstPtr<InheritedC> &cellAfterRowBorderC() const;
  const ConstPtr<InheritedC> &cellBeforeColumnBorderC() const;
  const ConstPtr<InheritedC> &cellAfterColumnBorderC() const;
  const ConstPtr<InheritedC> &fractionBarC() const;
  const char *storePublicId(const Char *, size_t, const Location &);
  unsigned allocGlyphSubstTableUniqueId();
  bool lookupNodeProperty(const StringC &, ComponentName::Id &);
  bool debugMode() const;
  bool dsssl2() const;
  bool style() const;
  bool strictMode() const;
  void setNodeLocation(const NodePtr &);
  void setDefaultLanguage(Owner<Expression> &,unsigned part,const Location &);
  ELObj *defaultLanguage() const;
  bool defaultLanguageSet(unsigned &,Location &) const;
  void compileDefaultLanguage();
  void makeReadOnly(ELObj *);
  ProcessingMode *lookupProcessingMode(const StringC &);
  ProcessingMode *initialProcessingMode();
  TransformationMode *transformationMode();
  void addClassAttributeName(const StringC &name);
  void addIdAttributeName(const StringC &name);
  void installInitialValue(Identifier *, Owner<Expression> &);
  void installExtensionInheritedC(Identifier *, const StringC &, const Location &);
  CharPropValues *installExtensionCharNIC(Identifier *, const StringC &,
					  const Location &);
  void installExtensionFlowObjectClass(Identifier *, const StringC &, const Location &);
  // Return 0 if an invalid number.
  ELObj *convertNumber(const StringC &, int radix = 10);
  bool convertCharName(const StringC &str, Char &c) const;
  enum LexCategory {
    lexLetter,			// a - z A - Z
    lexOtherNameStart,		// !$%&*/<=>?~_^:
    lexAddNameStart,
    lexDigit,			// 0-9
    lexOtherNumberStart,	// -+.
    lexOther,
    lexDelimiter,		// ;()"
    lexWhiteSpace,
    lexAddWhiteSpace
  };
  LexCategory lexCategory(Xchar);
  void addStandardChar(const StringC &, const StringC &);
  void addSdataEntity(const StringC &, const StringC &, const StringC &);
  void addNameChar(const StringC &);
  void addSeparatorChar(const StringC &);
  void setCharRepertoire(const StringC &);
  enum Feature {
    noFeature,
    combineChar,
    keyword,
    multiSource,
    multiResult,
    regexp,
    word,
    hytime,
    charset,
    expression,
    multiProcess,
    query,
    sideBySide,
    sideline,
    alignedColumn,
    bidi,
    vertical,
    math,
    table,
    tableAutoWidth,
    simplePage,
    page,
    multiColumn,
    nestedColumnSet,
    generalIndirect,
    inlineNote,
    glyphAnnotation,
    emphasizingMark,
    includedContainer,
    actualCharacteristic,
    online,
    fontInfo,
    crossReference
  };
  enum { nFeatures = crossReference + 1 };
  enum Support {
    notSupported,
    partiallySupported,
    supported
  };
  struct Status {
    bool declared;
    Support supported;
    StringC rcsname;
    StringC appname;
  };
  bool convertFeature(const StringC &, Feature &);
  void explicitFeatures();
  void declareFeature(const StringC &);
  void declareFeature(const Feature &);
  bool requireFeature(const Feature &, const Location &);
  enum Module {
    baseabs,
    prlgabs0,
    prlgabs1,
    instabs,
    basesds0,
    basesds1,
    instsds0,
    subdcabs,
    sdclabs,
    sdclsds,
    prlgsds,
    instsds1,
    dtgabs,
    rankabs,
    srabs,
    srsds,
    linkabs,
    linksds,
    subdcsds,
    fpiabs
  };
  enum { nModules = fpiabs + 1 };
  void explicitModules();
  void addModule(const StringC &);
private:
  Interpreter(const Interpreter &); // undefined
  void operator=(const Interpreter &); // undefined
  void installSyntacticKeys();
  void installPortNames();
  void installCValueSymbols();
  void installPrimitives();
  void installPrimitive(const char *, PrimitiveObj *, Feature f);
  void installXPrimitive(const char *, const char *, PrimitiveObj *);
  void installBuiltins();
  void installUnits();
  void installCharNames();
  void installInheritedCs();
  void installInheritedC(const char *, InheritedC *);
  void installInheritedCProc(const Identifier *);
  void installFlowObjs();
  void installSdata();
  void installNodeProperties();
  void installCharProperties();
  void installFeatures(const FOTBuilder::Feature *);
  void installModules();
  void checkGrovePlan();
  void compileInitialValues();
  bool sdataMap(GroveString, GroveString, GroveChar &) const;
  static bool convertUnicodeCharName(const StringC &str, Char &c);
  bool convertToPattern(ELObj *obj, const Location &loc,
			bool isChild, IList<Pattern::Element> &list);
  bool patternAddAttributeQualifiers(ELObj *obj,
				     const Location &loc,
				     Pattern::Element &elem);
  enum {
    convertAllowBoolean = 01,
    convertAllowSymbol = 02,
    convertAllowNumber = 04
  };
  ELObj *convertFromString(ELObj *, unsigned hints, const Location &);
  ELObj *convertNumberFloat(const StringC &);
  bool scanSignDigits(const StringC &str, size_t &i, int &n);
  Unit *scanUnit(const StringC &str, size_t i, int &unitExp);

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
  const FOTBuilder::Description &fotbDescr_;
  unsigned partIndex_;
  unsigned dPartIndex_;
  int unitsPerInch_;
  unsigned nInheritedC_;
  GroveManager *groveManager_;
  ProcessingMode initialProcessingMode_;
  NamedTable<ProcessingMode> processingModeTable_;
  SymbolObj *portNames_[nPortNames];
  ELObj *cValueSymbols_[FOTBuilder::nSymbols];
  HashTable<StringC, CharPart> namedCharTable_;
  HashTable<StringC, CharPart> sdataEntityNameTable_;
  HashTable<StringC, CharPart> sdataEntityTextTable_;
  Vector<const Identifier *> initialValueNames_;
  NCVector<Owner<Expression> > initialValueValues_;
  size_t currentPartFirstInitialValue_;
  StyleObj *initialStyle_;
  StyleObj *borderTrueStyle_;
  StyleObj *borderFalseStyle_;
  ConstPtr<InheritedC> charMapC_;
  ConstPtr<InheritedC> tableBorderC_;
  ConstPtr<InheritedC> cellBeforeRowBorderC_;
  ConstPtr<InheritedC> cellAfterRowBorderC_;
  ConstPtr<InheritedC> cellBeforeColumnBorderC_;
  ConstPtr<InheritedC> cellAfterColumnBorderC_;
  ConstPtr<InheritedC> fractionBarC_;
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
  unsigned nextGlyphSubstTableUniqueId_;
  AddressObj *addressNoneObj_;
  NodeListObj *emptyNodeListObj_;
  HashTable<StringC,int> nodePropertyTable_;
  bool debugMode_;
  bool dsssl2_;
  bool style_;
  bool strictMode_;
  ELObj *defaultLanguage_;
  Owner<Expression> defaultLanguageDef_;
  unsigned defaultLanguageDefPart_;
  Location defaultLanguageDefLoc_;
  friend class Identifier;
  NamedTable<CharProp> charPropTable_;

  // No access function, since this is not a character NIC.
  MaybeIntegerCharPropValues numericEquivCPV_;
#define CP(t, n) \
private: t ## CharPropValues n ## CPV_; \
public: const t ## CharPropValues &n () const \
  { return n ## CPV_; }
  CP(Boolean, isSpace);
  CP(Boolean, isRecordEnd);
  CP(Boolean, isBlank);
  CP(Boolean, isInputTab);
  CP(Boolean, isInputWhitespace);
  CP(Boolean, isPunct);
  CP(PublicId, script);
  // FIXME! Shall not be PUblicId, but GlyphId.
  CP(PublicId, glyphId);
  CP(Boolean, isDropAfterLineBreak);
  CP(Boolean, isDropUnlessBeforeLineBreak);
  CP(Integer, breakBeforePriority);
  CP(Integer, breakAfterPriority);
  CP(Symbol, mathClass);
  CP(Symbol, mathFontPosture);

  Status feature_[nFeatures];
  bool explicitFeatures_;
  Status module_[nModules];
  bool explicitModules_;
  TransformationMode transformationMode_;
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
AddressObj *Interpreter::makeAddressNone()
{
  return addressNoneObj_;
}

inline
NodeListObj *Interpreter::makeEmptyNodeList()
{
  return emptyNodeListObj_;
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
ProcessingMode *Interpreter::initialProcessingMode()
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
const FOTBuilder::Description &Interpreter::fotbDescr() const
{
  return fotbDescr_;
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
GroveManager *Interpreter::groveManager() const
{
  return groveManager_;
}

inline
const ConstPtr<InheritedC> &Interpreter::charMapC() const
{
  return charMapC_;
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
const ConstPtr<InheritedC> &Interpreter::fractionBarC() const
{
  return fractionBarC_;
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
bool Interpreter::debugMode() const
{
  return debugMode_;
}

inline
bool Interpreter::dsssl2() const
{
  return dsssl2_;
}

inline
bool Interpreter::style() const
{
  return style_;
}

inline
bool Interpreter::strictMode() const
{
  return strictMode_;
}

inline
void Interpreter::makeReadOnly(ELObj *obj)
{
  if (dsssl2())
    Collector::makeReadOnly(obj);
}

inline
void Interpreter::addClassAttributeName(const StringC &name)
{
  classAttributeNames_.push_back(name);
}

inline
void Interpreter::addIdAttributeName(const StringC &name)
{
  idAttributeNames_.push_back(name);
}

inline
Interpreter::LexCategory Interpreter::lexCategory(Xchar c)
{
  return LexCategory(lexCategory_[c]);
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
bool Identifier::inheritedCDefined(unsigned &part, Location &loc) const
{
  if (inheritedC_.isNull())
    return 0;
  part = inheritedCPart_;
  loc = inheritedCLoc_;
  return 1;
}

inline
bool Identifier::charNICDefined(unsigned &part, Location &loc) const
{
  if (!charNIC_)
    return 0;
  part = inheritedCPart_;
  loc = inheritedCLoc_;
  return 1;
}

inline
void Identifier::setCharNIC(unsigned part,
			    const Location &loc)
{
  charNIC_ = 1;
  inheritedC_ = ConstPtr<InheritedC>(0);
  inheritedCPart_ = part;
  inheritedCLoc_ = loc;
}

inline
void Identifier::setInheritedC(const ConstPtr<InheritedC> &ic, unsigned part,
			       const Location &loc)
{
  inheritedC_ = ic;
  inheritedCPart_ = part;
  inheritedCLoc_ = loc;
}

inline
void Identifier::setInheritedC(const ConstPtr<InheritedC> &ic)
{
  inheritedC_ = ic;
  inheritedCPart_ = unsigned(-1);
  inheritedCLoc_ = Location();
}

inline
FlowObj *Identifier::flowObj() const
{
  return flowObj_;
}

inline
bool Identifier::flowObjDefined(unsigned &part, Location &loc) const
{
  if (!flowObj_)
    return 0;
  part = flowObjPart_;
  loc = flowObjLoc_;
  return 1;
}

inline
void Identifier::setFlowObj(FlowObj *fo)
{
  flowObj_ = fo;
  flowObjPart_ = unsigned(-1);
}

inline
void Identifier::setFlowObj(FlowObj *fo, unsigned part, const Location &loc)
{
  flowObj_ = fo;
  flowObjPart_ = part;
  flowObjLoc_ = loc;
}

inline
ELObjPart::ELObjPart()
: obj(0), defPart(0)
{
}

inline
ELObjPart::ELObjPart(ELObj *o, unsigned p)
: obj(o), defPart(p)
{
}

inline
void ELObjPart::operator=(const ELObjPart &x)
{
  obj = x.obj;
  defPart = x.defPart;
}

inline
bool ELObjPart::operator==(const ELObjPart &x) const
{
  return defPart == x.defPart && obj && x.obj && ELObj::eqv(*obj, *x.obj);
}

inline
bool ELObjPart::operator!=(const ELObjPart &x) const
{
  return !(*this == x);
}

inline
long IntegerCharPropValues::getValue(Char ch) const
{
  ValT_ v(map_[ch]);
  if (v.hasValue_)
    return v.l_;
  else
    return def_;
}

inline
void IntegerCharPropValues::setRange(Char from, Char to, long l)
{
  map_.setRange(from, to, l);
}

inline
bool MaybeIntegerCharPropValues::getValue(Char ch, long &l) const
{
  ValT_ v(map_[ch]);
  if (v.hasValue_) {
    if (!v.isFalse_)
      l = v.l_;
    return !v.isFalse_;
  }
  l = def_;
  return true;
}

inline
void MaybeIntegerCharPropValues::setRange(Char from, Char to, long l)
{
  map_.setRange(from, to, l);
}

inline
void MaybeIntegerCharPropValues::setValue(Char ch, long l)
{
  map_.setChar(ch, l);
}

inline
bool BooleanCharPropValues::getValue(Char ch) const
{
  ValT_ v(map_[ch]);
  if (v.hasValue_)
    return v.b_;
  return def_;
}

inline
void BooleanCharPropValues::setRange(Char from, Char to, bool b)
{
  map_.setRange(from, to, b);
}

inline
FOTBuilder::PublicId PublicIdCharPropValues::getValue(Char ch) const
{
  ValT_ v(map_[ch]);
  if (v.hasValue_)
    return v.p_;
  return def_;
}

inline
void PublicIdCharPropValues::setRange(Char from, Char to,
				 FOTBuilder::PublicId p)
{
  map_.setRange(from, to, p);
}

inline
FOTBuilder::Symbol SymbolCharPropValues::getValue(Char ch) const
{
  ValT_ v(map_[ch]);
  if (v.hasValue_)
    return v.s_;
  return def_;
}

inline
void SymbolCharPropValues::setRange(Char from, Char to,
				    FOTBuilder::Symbol s)
{
  map_.setRange(from, to, s);
}

inline
CharProp::CharProp(const StringC &name)
  : Named(name), defComputed_(notComputed), values_(0),
    hadError_(false)
{
}

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* not Interpreter_INCLUDED */
