// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#include "stylelib.h"
#include "Interpreter.h"
#include "Insn.h"
#include "InterpreterMessages.h"
#include "LocNode.h"
#include "Pattern.h"
#include "MacroFlowObj.h"
#include "ELObjMessageArg.h"
#include "VM.h"
#include <OpenSP/Owner.h> 
#include "SchemeParser.h"
#include <OpenSP/macros.h>
#include <OpenSP/InternalInputSource.h>
#include <stdlib.h>
#include "LangObj.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

class ELObjCharPropValues : public CharPropValues {
public:
  ELObjCharPropValues()
    : def_(0), map_(0) {}
private:
  ELObjCharPropValues(const ELObjCharPropValues &); // Undefined.
  ELObjCharPropValues &operator=(const ELObjCharPropValues &); // Undefined.
  // 0 means no value.
  ELObj *def_;
  CharMap<ELObj *> map_;
  bool setDefault(const StringC &, const Location &,
		  ELObj *, Interpreter &);
  bool setValue(const StringC &, const StringC &, const Location &,
		ELObj *,Interpreter &);
  ELObj *value(Char, Interpreter &) const;
  ELObj *defaultValue(Interpreter &) const;
};

const Char defaultChar = 0xfffd;

static
size_t maxObjSize()
{
  // FlowObj classes are declared in FlowObj.cxx.
  class Dummy : public CompoundFlowObj {
  private:
    Owner<Location> locp_;
  };
  static size_t sz[] = {
    sizeof(UnresolvedQuantityObj),
    sizeof(VarStyleObj),
    sizeof(OverriddenStyleObj),
    sizeof(MergeStyleObj),
    sizeof(DeviceRGBColorObj),
    sizeof(ColorSpaceObj),
    sizeof(PairObj),
    sizeof(QuantityObj),
    sizeof(GlyphIdObj),
    sizeof(NamedNodeListPtrNodeListObj),
    sizeof(ProcessNodeSosofoObj),
    sizeof(AppendSosofoObj),
    sizeof(LiteralSosofoObj),
    sizeof(SetNonInheritedCsSosofoObj),
    sizeof(LabelSosofoObj),
    sizeof(MacroFlowObj),
    sizeof(Dummy),
    sizeof(LangObj),
#ifdef SP_HAVE_LOCALE
    sizeof(RefLangObj),
#endif
    sizeof(SubgroveSpecObj),
    sizeof(CreateSpecObj),
  };
  size_t n = sz[0];
  for (size_t i = 1; i < SIZEOF(sz); i++)
    if (sz[i] > n)
      n = sz[i];
  return n;
}

Interpreter::Interpreter(GroveManager *groveManager,
			 Messenger *messenger,
			 int unitsPerInch,
			 bool debugMode,
			 bool dsssl2,
			 bool style,
                         bool strictMode,
			 const FOTBuilder::Description &fotbDescr)

: groveManager_(groveManager),
  messenger_(messenger),
  fotbDescr_(fotbDescr),
  Collector(maxObjSize()),
  partIndex_(0),  // 0 is for command-line definitions
  dPartIndex_(1),
  lexCategory_(lexOther),
  unitsPerInch_(unitsPerInch),
  nInheritedC_(0),
  initialProcessingMode_(StringC()),
  currentPartFirstInitialValue_(0),
  initialStyle_(0),
  nextGlyphSubstTableUniqueId_(0),
  debugMode_(debugMode),
  dsssl2_(dsssl2),
  mathClassCPV_(FOTBuilder::symbolOrdinary),
  style_(style),
  strictMode_(strictMode),
  explicitFeatures_(0),
  explicitModules_(0)
{
  makePermanent(theNilObj_ = new (*this) NilObj);
  makePermanent(theFalseObj_ = new (*this) FalseObj);
  makePermanent(theTrueObj_ = new (*this) TrueObj);
  makePermanent(theErrorObj_ = new (*this) ErrorObj);
  makePermanent(theUnspecifiedObj_ = new (*this) UnspecifiedObj);
  makePermanent(addressNoneObj_
                = new (*this) AddressObj(FOTBuilder::Address::none));
  makePermanent(emptyNodeListObj_
		= new (*this) NodePtrNodeListObj);
  defaultLanguage_ = theFalseObj_;
  installSyntacticKeys();
  installCValueSymbols();
  installPortNames();
  installPrimitives();
  installUnits();
  installFeatures(fotbDescr_.features);
  installModules();
  installCharNames();
  installSdata();
  installFlowObjs();
  installInheritedCs();
  installNodeProperties();

  static const char *lexCategories[] = {
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz",
    "!$%&*/<=>?~_^:",
    "",
    "0123456789",
    "-+.",
    "",
    "();\"",
    " \t\r\n\f",
    "",
  };
  lexCategory_.setEe(lexDelimiter);
  for (size_t i = 0; i < SIZEOF(lexCategories); i++)
    for (const char *s = lexCategories[i]; *s; s++)
      lexCategory_.setChar(*s, i);

  // #',@[\\]`{|} are ASCII chars not mentioned above,
  // but I guess we don't want to allow any of these
  // in names (as most of them have special meaning in
  // scheme/dsssl).
  if (!strictMode_)
    for (Char i = 127; i < charMax; i++)
      lexCategory_.setChar(i, lexAddNameStart);

  initialProcessingMode_.setDefined();
  // can't be done before initializing lexCategory_
  installBuiltins();
  installCharProperties();
}

void Interpreter::compile()
{
  // FIXME compile all definitions
  compileInitialValues();
  initialProcessingMode_.compile(*this);
  NamedTableIter<ProcessingMode> iter(processingModeTable_);
  for (;;) {
    ProcessingMode *mode = iter.next();
    if (!mode)
      break;
    mode->compile(*this);
  }
  compileCharProperties();
  compileDefaultLanguage();
  checkGrovePlan();
}

void Interpreter::compileInitialValues()
{
  Vector<ConstPtr<InheritedC> > ics;
  for (size_t i = 0; i < initialValueNames_.size(); i++) {
    const Identifier *ident = initialValueNames_[i];
    Owner<Expression> &expr = initialValueValues_[i];
    ConstPtr<InheritedC> ic(ident->inheritedC());
    expr->optimize(*this, Environment(), expr);
    ELObj *val = expr->constantValue();
    if (val) {
      ConstPtr<InheritedC> tem(ic->make(val, expr->location(), *this));
      if (!tem.isNull())
	ics.push_back(tem);
    }
    else
      ics.push_back(new VarInheritedC(ic,
    			              expr->compile(*this, Environment(), 0, InsnPtr()),
			              expr->location()));
  }
  if (ics.size()) {
    Vector<ConstPtr<InheritedC> > forceIcs;
    initialStyle_ = new (*this) VarStyleObj(new StyleSpec(forceIcs, ics), 0, 0, NodePtr());
    makePermanent(initialStyle_);
  }
}

void Interpreter::installInitialValue(Identifier *ident, Owner<Expression> &expr)
{
  for (size_t i = 0; i < initialValueNames_.size(); i++) {
    if (ident == initialValueNames_[i]) {
      if (i >= currentPartFirstInitialValue_) {
	setNextLocation(expr->location());
	message(InterpreterMessages::duplicateInitialValue,
	        StringMessageArg(ident->name()),
		initialValueValues_[i]->location());
      }
      return;
    }
  }
  initialValueValues_.resize(initialValueValues_.size() + 1);
  expr.swap(initialValueValues_.back());
  initialValueNames_.push_back(ident);
}

void Interpreter::installUnits()
{
  static struct {
    const char *name;
    int numer;
    int denom;
    bool dsssl2;
  } units[] = {
    { "m", 5000, 127 },
    { "cm", 50, 127 },
    { "mm", 5, 127 },
    { "in", 1, 1 },
    { "pt", 1, 72 },
    { "pica", 1, 6 },
    { "pc", 1, 6 } // a DSSSL2 addition
  };
  size_t nUnits = dsssl2() ? SIZEOF(units) : SIZEOF(units) - 1;
  for (size_t i = 0; i < nUnits; i++) {
    Unit *unit = lookupUnit(makeStringC(units[i].name));
    long n = unitsPerInch_ * units[i].numer;
    if (n % units[i].denom == 0)
      unit->setValue(long(n / units[i].denom));
    else
      unit->setValue(double(n)/units[i].denom);
  }
}

void Interpreter::installSyntacticKeys()
{
  static struct {
    const char *name;
    Identifier::SyntacticKey key;
  } keys[] = {
    { "quote", Identifier::keyQuote },
    { "lambda", Identifier::keyLambda },
    { "if", Identifier::keyIf },
    { "cond", Identifier::keyCond },
    { "and", Identifier::keyAnd },
    { "or", Identifier::keyOr },
    { "case", Identifier::keyCase },
    { "let", Identifier::keyLet },
    { "let*", Identifier::keyLetStar },
    { "letrec", Identifier::keyLetrec },
    { "quasiquote", Identifier::keyQuasiquote },
    { "unquote", Identifier::keyUnquote },
    { "unquote-splicing", Identifier::keyUnquoteSplicing },
    { "define", Identifier::keyDefine },
    { "else", Identifier::keyElse },
    { "=>", Identifier::keyArrow },
    { "there-exists?", Identifier::keyThereExists },
    { "for-all?", Identifier::keyForAll },
    { "select-each", Identifier::keySelectEach },
    { "union-for-each", Identifier::keyUnionForEach },
    { "define-unit", Identifier::keyDefineUnit },
    { "declare-default-language", Identifier::keyDeclareDefaultLanguage },
    { "declare-char-property", Identifier::keyDeclareCharProperty },
    { "define-language", Identifier::keyDefineLanguage },
    { "collate", Identifier::keyCollate },
    { "toupper", Identifier::keyToupper },
    { "tolower", Identifier::keyTolower },
    { "symbol", Identifier::keySymbol },
    { "order", Identifier::keyOrder },
    { "forward", Identifier::keyForward },
    { "backward", Identifier::keyBackward },
    { "add-char-properties", Identifier::keyAddCharProperties },
    { "architecture", Identifier::keyArchitecture },
    { "default", Identifier::keyDefault },
    { "null", Identifier::keyNull },
    { "rcs?", Identifier::keyIsRcs },
    { "parent", Identifier::keyParent },
    { "active", Identifier::keyActive },
    { "label", Identifier::keyLabel },
    { "children", Identifier::keyChildren },
    { "class", Identifier::keyClass },
   }, styleKeys[] = {
    { "make", Identifier::keyMake },
    { "style", Identifier::keyStyle },
    { "with-mode", Identifier::keyWithMode },
    { "query", Identifier::keyQuery },
    { "element", Identifier::keyElement },
    { "root", Identifier::keyRoot },
    { "id", Identifier::keyId },
    { "mode", Identifier::keyMode },
    { "declare-initial-value", Identifier::keyDeclareInitialValue },
    { "declare-characteristic", Identifier::keyDeclareCharacteristic },
    { "declare-flow-object-class", Identifier::keyDeclareFlowObjectClass },
    { "declare-char-characteristic+property", Identifier::keyDeclareCharCharacteristicAndProperty },
    { "declare-reference-value-type", Identifier::keyDeclareReferenceValueType },
    { "define-page-model", Identifier::keyDefinePageModel },
    { "define-column-set-model", Identifier::keyDefineColumnSetModel },
    { "use", Identifier::keyUse },
    { "content-map", Identifier::keyContentMap },
    { "keep-with-previous?", Identifier::keyIsKeepWithPrevious },
    { "keep-with-next?", Identifier::keyIsKeepWithNext },
    { "space-before", Identifier::keySpaceBefore },
    { "space-after", Identifier::keySpaceAfter },
    { "left-header", Identifier::keyLeftHeader },
    { "center-header", Identifier::keyCenterHeader },
    { "right-header", Identifier::keyRightHeader },
    { "left-footer", Identifier::keyLeftFooter },
    { "center-footer", Identifier::keyCenterFooter },
    { "right-footer", Identifier::keyRightFooter },
    { "destination", Identifier::keyDestination },
    { "type", Identifier::keyType },
    { "coalesce-id", Identifier::keyCoalesceId },
    { "display?", Identifier::keyIsDisplay },
    { "scale", Identifier::keyScale },
    { "max-width", Identifier::keyMaxWidth },
    { "max-height", Identifier::keyMaxHeight },
    { "entity-system-id", Identifier::keyEntitySystemId },
    { "notation-system-id", Identifier::keyNotationSystemId },
    { "position-point-x", Identifier::keyPositionPointX },
    { "position-point-y", Identifier::keyPositionPointY },
    { "escapement-direction", Identifier::keyEscapementDirection },
    { "break-before-priority", Identifier::keyBreakBeforePriority },
    { "break-after-priority", Identifier::keyBreakAfterPriority },
    { "orientation", Identifier::keyOrientation },
    { "length", Identifier::keyLength },
    { "char", Identifier::keyChar },
    { "glyph-id", Identifier::keyGlyphId },
    { "space?", Identifier::keyIsSpace },
    { "record-end?", Identifier::keyIsRecordEnd },
    { "input-tab?", Identifier::keyIsInputTab },
    { "input-whitespace?", Identifier::keyIsInputWhitespace },
    { "punct?", Identifier::keyIsPunct },
    { "drop-after-line-break?", Identifier::keyIsDropAfterLineBreak },
    { "drop-unless-before-line-break?", Identifier::keyIsDropUnlessBeforeLineBreak },
    { "math-class", Identifier::keyMathClass },
    { "math-font-posture", Identifier::keyMathFontPosture },
    { "script", Identifier::keyScript },
    { "stretch-factor", Identifier::keyStretchFactor },
    { "keep", Identifier::keyKeep },
    { "break-before", Identifier::keyBreakBefore },
    { "break-after", Identifier::keyBreakAfter },
    { "may-violate-keep-before?", Identifier::keyIsMayViolateKeepBefore },
    { "may-violate-keep-after?", Identifier::keyIsMayViolateKeepAfter },
    { "before-row-border", Identifier::keyBeforeRowBorder },
    { "after-row-border", Identifier::keyAfterRowBorder },
    { "before-column-border", Identifier::keyBeforeColumnBorder },
    { "after-column-border", Identifier::keyAfterColumnBorder },
    { "column-number", Identifier::keyColumnNumber },
    { "row-number", Identifier::keyRowNumber },
    { "n-columns-spanned", Identifier::keyNColumnsSpanned },
    { "n-rows-spanned", Identifier::keyNRowsSpanned },
    { "width", Identifier::keyWidth },
    { "starts-row?", Identifier::keyIsStartsRow },
    { "ends-row?", Identifier::keyIsEndsRow },
    { "table-width", Identifier::keyTableWidth },
    { "multi-modes", Identifier::keyMultiModes },
    { "data", Identifier::keyData },
    { "min", Identifier::keyMin },
    { "max", Identifier::keyMax },
    { "conditional?", Identifier::keyIsConditional },
    { "priority", Identifier::keyPriority },
    { "grid-n-rows", Identifier::keyGridNRows },
    { "grid-n-columns", Identifier::keyGridNColumns },
    { "radical", Identifier::keyRadical },
    { "attributes", Identifier::keyAttributes },
    { "repeat", Identifier::keyRepeat },
    { "position", Identifier::keyPosition },
    { "only", Identifier::keyOnly },
    { "importance", Identifier::keyImportance },
    { "position-preference", Identifier::keyPositionPreference },
    { "white-point", Identifier::keyWhitePoint },
    { "black-point", Identifier::keyBlackPoint },
    { "range", Identifier::keyRange },
    { "range-abc", Identifier::keyRangeAbc },
    { "range-lmn", Identifier::keyRangeLmn },
    { "range-a", Identifier::keyRangeA },
    { "decode-abc", Identifier::keyDecodeAbc },
    { "decode-lmn", Identifier::keyDecodeLmn },
    { "decode-a", Identifier::keyDecodeA },
    { "matrix-abc", Identifier::keyMatrixAbc },
    { "matrix-lmn", Identifier::keyMatrixLmn },
    { "matrix-a", Identifier::keyMatrixA },
  }, transformKeys[] = {
    { "define-transliteration-map", Identifier::keyDefineTransliterationMap },
    { "node", Identifier::keyNode },
    { "subgrove", Identifier::keySubgrove },
    { "add", Identifier::keyAdd },
    { "remove", Identifier::keyRemove },
    { "sub", Identifier::keySub },
    { "sort-children", Identifier::keySortChildren },
    { "optional", Identifier::keyOptional },
    { "unique", Identifier::keyUnique },
    { "result-path", Identifier::keyResultPath },
    { "property", Identifier::keyProperty },
  }, keys2[] = {
    { "declare-class-attribute", Identifier::keyDeclareClassAttribute },
    { "declare-id-attribute", Identifier::keyDeclareIdAttribute },
    { "declare-flow-object-macro", Identifier::keyDeclareFlowObjectMacro },
    { "or-element", Identifier::keyOrElement },
    { "set!", Identifier::keySet },
    { "begin", Identifier::keyBegin },
  };
  for (size_t i = 0; i < SIZEOF(keys); i++) {
    StringC tem(makeStringC(keys[i].name));
    lookup(tem)->setSyntacticKey(keys[i].key);
    if (dsssl2() && tem[tem.size() - 1] == '?') {
      tem.resize(tem.size() - 1);
      lookup(tem)->setSyntacticKey(keys[i].key);
    }
  }
  if (style()) 
    for (size_t i = 0; i < SIZEOF(styleKeys); i++) {
      StringC tem(makeStringC(styleKeys[i].name));
      lookup(tem)->setSyntacticKey(styleKeys[i].key);
      if (dsssl2() && tem[tem.size() - 1] == '?') {
        tem.resize(tem.size() - 1);
        lookup(tem)->setSyntacticKey(styleKeys[i].key);
      }
    }
  else 
    for (size_t i = 0; i < SIZEOF(transformKeys); i++) {
      StringC tem(makeStringC(transformKeys[i].name));
      lookup(tem)->setSyntacticKey(transformKeys[i].key);
    }
  if (dsssl2()) {
    for (size_t i = 0; i < SIZEOF(keys2); i++)
      lookup(makeStringC(keys2[i].name))->setSyntacticKey(keys2[i].key);
  }
}

void Interpreter::installCValueSymbols()
{
  cValueSymbols_[0] = makeFalse();
  cValueSymbols_[1] = makeTrue();
  for (size_t i = 2; i < FOTBuilder::nSymbols; i++) {
    SymbolObj *sym = makeSymbol(makeStringC(FOTBuilder::symbolName(FOTBuilder::Symbol(i))));
    sym->setCValue(FOTBuilder::Symbol(i));
    cValueSymbols_[i] = sym;
  }
}

void Interpreter::installPortNames()
{
  // These must match the order in SymbolObj.
  static const char *names[] = {
    "numerator",
    "denominator",
    "pre-sup",
    "pre-sub",
    "post-sup",
    "post-sub",
    "mid-sup",
    "mid-sub",
    "over-mark",
    "under-mark",
    "open",
    "close",
    "degree",
    "operator",
    "lower-limit",
    "upper-limit",
    "header",
    "footer"
  };
  ASSERT(SIZEOF(names) == nPortNames);
  for (size_t i = 0; i < SIZEOF(names); i++)
    portNames_[i] = makeSymbol(makeStringC(names[i]));
}

void Interpreter::installCharNames()
{
  if (!strictMode()) {
    static struct {
      Char c;
      const char *name;
    } chars[] = {
#include "charNames.h"
    };
    for (size_t i = 0; i < SIZEOF(chars); i++) {
      CharPart ch;
      ch.c = chars[i].c;
      ch.defPart = unsigned(-1);
      namedCharTable_.insert(makeStringC(chars[i].name), ch, 1);
    }
  }
}

void Interpreter::installSdata()
{
  if (!strictMode()) {
    // This comes from uni2sgml.txt on ftp://unicode.org.
    // It is marked there as obsolete, so it probably ought to be checked.
    // The definitions of apos and quot have been fixed for consistency with XML.
    static struct {
      Char c;
      const char *name;
    } entities[] = {
#include "sdata.h"
    };
    for (size_t i = 0; i < SIZEOF(entities); i++) { 
      CharPart ch;
      ch.c = entities[i].c;
      ch.defPart = unsigned(-1);
      sdataEntityNameTable_.insert(makeStringC(entities[i].name), ch, 1);
    }
  }
}

void Interpreter::installNodeProperties()
{
  for (int i = 0; i < ComponentName::nIds; i++) {
    ComponentName::Id id = ComponentName::Id(i);
    nodePropertyTable_.insert(makeStringC(ComponentName::rcsName(id)), i);
    nodePropertyTable_.insert(makeStringC(ComponentName::sdqlName(id)), i);
  }
}

void Interpreter::setCharRepertoire(const StringC &pubid)
{
  if (pubid == "UNREGISTERED::OpenJade//Character Repertoire::OpenJade") {
    if (strictMode_) {
      installCharNames();
      installSdata();
      // This assumes that we process char-repertoire
      // declaration before any declarations which change
      // lexical categories.
      for (Char i = 127; i < charMax; i++)
        lexCategory_.setChar(i, lexAddNameStart);
      strictMode_ = 0;
    }
   } else 
     message(InterpreterMessages::unsupportedCharRepertoire,
                StringMessageArg(pubid));
}
 
void Interpreter::addStandardChar(const StringC &name, const StringC &num)
{
  int n;
  size_t i = 0;
  if (!scanSignDigits(num, i, n)) {
    message(InterpreterMessages::invalidCharNumber, StringMessageArg(num));
    return;
  }
  
  const CharPart *def = namedCharTable_.lookup(name);
  CharPart ch;
  ch.c = n;
  ch.defPart = dPartIndex_;
  if (def) {
    if (dPartIndex_ < def->defPart)
      namedCharTable_.insert(name, ch, 1);
    else if (def->defPart == dPartIndex_ && def->c != ch.c) 
      message(InterpreterMessages::duplicateCharName, 
    	      StringMessageArg(name));
  }
  else 
    namedCharTable_.insert(name, ch, 1);
}

void Interpreter::addNameChar(const StringC &name)
{
  const CharPart *cp = namedCharTable_.lookup(name);
  if (!cp) 
    message(InterpreterMessages::badCharName, 
            StringMessageArg(name));
  else if (lexCategory_[cp->c] != lexOther)
    // FIXME give a more specific error
    message(InterpreterMessages::badDeclaration);
  else
    lexCategory_.setChar(cp->c, lexAddNameStart);
}

void Interpreter::addSeparatorChar(const StringC &name)
{
  const CharPart *cp = namedCharTable_.lookup(name);
  if (!cp)
    message(InterpreterMessages::badCharName, 
            StringMessageArg(name));
  else if (lexCategory_[cp->c] != lexOther)
    // FIXME give a more specific error
    message(InterpreterMessages::badDeclaration);
  else
    lexCategory_.setChar(cp->c, lexAddWhiteSpace);
}

void Interpreter::addSdataEntity(const StringC &ename, const StringC &etext, const StringC &name)
{
  const CharPart *cp = namedCharTable_.lookup(name);
  if (!cp) { 
    message(InterpreterMessages::badCharName, 
            StringMessageArg(name));
    return; 
  }

  CharPart ch;
  ch.c = cp->c;
  ch.defPart = dPartIndex_;

  if (ename.size() > 0) {
    const CharPart *def = sdataEntityNameTable_.lookup(ename);
    if (def) {
      if (dPartIndex_ < def->defPart)
	sdataEntityNameTable_.insert(ename, ch);
      else if (def->defPart == dPartIndex_ && def->c != cp->c) 
       	message(InterpreterMessages::duplicateSdataEntityName, 
		StringMessageArg(ename));
    }
    else
      sdataEntityNameTable_.insert(ename, ch);
  }

  if (etext.size() > 0) {
    const CharPart *def = sdataEntityTextTable_.lookup(etext);
    if (def) {
      if (dPartIndex_ < def->defPart)
	sdataEntityTextTable_.insert(etext, ch);
      else if (def->defPart == dPartIndex_ && def->c != cp->c) 
       	message(InterpreterMessages::duplicateSdataEntityText, 
		StringMessageArg(etext));
    }
    else
      sdataEntityTextTable_.insert(etext, ch);
  }
}

bool Interpreter::sdataMap(GroveString name, GroveString text, GroveChar &c) const
{
  StringC tem(name.data(), name.size());
  StringC tem2(text.data(), text.size());

  const CharPart *cp = sdataEntityNameTable_.lookup(tem);
  if (cp) {
    c = cp->c;
    return 1;
  }
  
  cp = sdataEntityTextTable_.lookup(tem2);
  if (cp) {
    c = cp->c;
    return 1;
  }
  
  if (convertUnicodeCharName(tem, c))
    return 1;
  // I think this is the best thing to do.
  // At least it makes preserve-sdata work with unknown SDATA entities.
  c = defaultChar;
  return 1;
}

ELObj *Interpreter::convertGlyphId(const Char *str, size_t len, const Location &loc)
{
  unsigned long n = 0;
  const char *publicId = 0;
  for (size_t i = len; i > 1; --i) {
    if (str[i - 1] == ':' && str[i - 2] == ':' && i < len && str[i] != '0') {
      for (size_t j = i; j < len; j++)
	n = n*10 + (str[j] - '0');
      publicId = storePublicId(str, i - 2, loc);
      break;
    }
    if (str[i - 1] < '0' || str[i - 1] > '9')
      break;
  }
  if (!publicId)
    publicId = storePublicId(str, len, loc);
  return new (*this) GlyphIdObj(FOTBuilder::GlyphId(publicId, n));
}

bool Interpreter::convertCharName(const StringC &str, Char &c) const
{
  const CharPart *cp = namedCharTable_.lookup(str);
  if (cp) {
    c = cp->c;
    return 1;
  }
  return convertUnicodeCharName(str, c);
}

bool Interpreter::convertUnicodeCharName(const StringC &str, Char &c)
{
  if (str.size() != 6 || str[0] != 'U' || str[1] != '-')
    return 0;
  Char value = 0;
  for (int i = 2; i < 6; i++) {
    value <<= 4;
    switch (str[i]) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      value |= str[i] - '0';
      break;
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
      value |= 10 + (str[i] - 'A');
      break;
    default:
      return 0;
    }
  }
  c = value;
  return 1;
}

SymbolObj *Interpreter::makeSymbol(const StringC &str)
{
  SymbolObj *sym = symbolTable_.lookup(str);
  if (!sym) {
    StringObj *strObj = new (*this) StringObj(str);
    makePermanent(strObj);
    sym = new (*this) SymbolObj(strObj);
    makePermanent(sym);
    symbolTable_.insert(sym);
  }
  return sym;
}

Identifier *Interpreter::lookup(const StringC &str)
{
  Identifier *ident = identTable_.lookup(str);
  if (!ident) {
    ident = new Identifier(str);
    identTable_.insert(ident);
  }
  return ident;
}

bool Interpreter::lookupNodeProperty(const StringC &str, ComponentName::Id &id)
{
  const int *val = nodePropertyTable_.lookup(str);
  if (!val) {
    StringC tem(str);
    for (size_t i = 0; i < tem.size(); i++) {
      if (tem[i] >= 'A' && tem[i] <= 'Z')
        tem[i] = 'a' + (tem[i] - 'A');
    }
    val = nodePropertyTable_.lookup(tem);
    if (!val)
      return 0;
  }
  id = ComponentName::Id(*val);
  return 1;
}

Unit *Interpreter::lookupUnit(const StringC &str)
{
  Unit *unit = unitTable_.lookup(str);
  if (!unit) {
    unit = new Unit(str);
    unitTable_.insert(unit);
  }
  return unit;
}

CharProp *Interpreter::lookupCharProperty(const StringC &n)
{
  CharProp *p = charPropTable_.lookup(n);
  if (!p) {
    p = new CharProp(n);
    charPropTable_.insert(p);
  }
  return p;
}

ProcessingMode *Interpreter::lookupProcessingMode(const StringC &str)
{
  ProcessingMode *mode = processingModeTable_.lookup(str);
  if (!mode) {
    mode = new ProcessingMode(str, &initialProcessingMode_);
    processingModeTable_.insert(mode);
  }
  return mode;
}

StringC Interpreter::makeStringC(const char *s)
{
  StringC tem;
  if (s)
    while (*s)
      tem += (unsigned char)*s++;
  return tem;
}

void Interpreter::endPart()
{
  currentPartFirstInitialValue_ = initialValueNames_.size();
  partIndex_++;
}

void Interpreter::dEndPart()
{
  dPartIndex_++;
}

void Interpreter::normalizeGeneralName(const NodePtr &nd, StringC &str)
{
  NamedNodeListPtr nnl;
  NodePtr root;
  if (nd->getGroveRoot(root) == accessOK
      && root->getElements(nnl) == accessOK)
    str.resize(nnl->normalize(str.begin(), str.size()));
}

ELObj *Interpreter::makeLengthSpec(const FOTBuilder::LengthSpec &ls)
{
  if (ls.displaySizeFactor != 0.0) {
    LengthSpec result(LengthSpec::displaySize, ls.displaySizeFactor);
    result += double(ls.length);
    return new (*this) LengthSpecObj(result);
  }
  else
    return new (*this) LengthObj(ls.length);
}

bool Interpreter::convertBooleanC(ELObj *obj, const Identifier *ident, const Location &loc,
				  bool &result)
{
  obj = convertFromString(obj, convertAllowBoolean, loc);
  if (obj == makeFalse()) {
    result = 0;
    return 1;
  }
  if (obj == makeTrue()) {
    result = 1;
    return 1;
  }
  invalidCharacteristicValue(ident, loc);
  return 0;
}

bool Interpreter::convertPublicIdC(ELObj *obj, const Identifier *ident,
				   const Location &loc,
				   FOTBuilder::PublicId &pubid)
{
  if (obj == makeFalse()) {
    pubid = 0;
    return 1;
  }
  const Char *s;
  size_t n;
  if (obj->stringData(s, n)) {
    if (n == 0)
      pubid = 0;
    else
      pubid = storePublicId(s, n, loc);
    return 1;
  }
  invalidCharacteristicValue(ident, loc);
  return 0;
}

const char *Interpreter::storePublicId(const Char *s, size_t n, const Location &loc)
{
  String<char> buf;
  for (; n > 0; s++, n--) {
    if (*s >= 128) {
      setNextLocation(loc);
      message(InterpreterMessages::invalidPublicIdChar,
	      StringMessageArg(StringC(s, 1)));
    }
    else
      buf += char(*s);
  }
  buf += '\0';
  return publicIds_.store(buf);
}

bool Interpreter::convertStringC(ELObj *obj, const Identifier *ident, const Location &loc,
				 StringC &result)
{
  const Char *s;
  size_t n;
  if (obj->stringData(s, n)) {
    result.assign(s, n);
    return 1;
  }
  invalidCharacteristicValue(ident, loc);
  return 0;
}

bool Interpreter::convertLengthSpec(ELObj *obj,
				    FOTBuilder::LengthSpec &result)
{
  int dim;
  double d;
  switch (obj->quantityValue(result.length, d, dim)) {
  case ELObj::longQuantity:
    if (dim == 1)
      return 1;
    break;
  case ELObj::doubleQuantity:
    if (dim == 1) {
      // FIXME catch overflow
      result.length = d < 0.0 ? long(d - .5) : long(d + .5);
      return 1;
    }
    break;
  default:
    {
      const LengthSpec *ls = obj->lengthSpec();
      if (ls)
	return ls->convert(result);
    }
    break;
  }
  return 0;
}

bool Interpreter::convertLengthC(ELObj *obj, const Identifier *ident,
				 const Location &loc,
				 FOTBuilder::Length &n)
{
  obj = convertFromString(obj, convertAllowNumber, loc);
  int dim;
  double d;
  switch (obj->quantityValue(n, d, dim)) {
  case ELObj::longQuantity:
    if (dim == 1)
      return 1;
    break;
  case ELObj::doubleQuantity:
    if (dim == 1) {
      // FIXME catch overflow
      n = long(d);
      return 1;
    }
    break;
  default:
    break;
  }
  invalidCharacteristicValue(ident, loc);
  return 0;
}

bool Interpreter::convertLengthSpecC(ELObj *obj, const Identifier *ident,
				     const Location &loc,
				     FOTBuilder::LengthSpec &result)
{
  obj = convertFromString(obj, convertAllowNumber, loc);
  if (convertLengthSpec(obj, result))
    return 1;
  invalidCharacteristicValue(ident, loc);
  return 0;
}

bool Interpreter::convertOptLengthSpecC(ELObj *obj, const Identifier *ident,
					const Location &loc,
					FOTBuilder::OptLengthSpec &result)
{
  obj = convertFromString(obj, convertAllowBoolean|convertAllowNumber, loc);
  if (obj == makeFalse()) {
    result.hasLength = 0;
    return 1;
  }
  if (convertLengthSpecC(obj, ident, loc, result.length)) {
    result.hasLength = 1;
    return 1;
  }
  return 0;
}

bool Interpreter::convertOptPositiveIntegerC(ELObj *obj, const Identifier *ident, const Location &loc,
					     long &result)
{
  obj = convertFromString(obj, convertAllowNumber|convertAllowBoolean, loc);
  if (obj == makeFalse()) {
    result = 0;
    return 1;
  }
  if (obj->exactIntegerValue(result) && result > 0)
    return 1;
  // FIXME allow inexact value
  invalidCharacteristicValue(ident, loc);
  return 0;
}

bool Interpreter::convertIntegerC(ELObj *obj, const Identifier *ident, const Location &loc,
				  long &result)
{
  obj = convertFromString(obj, convertAllowNumber, loc);
  if (obj->exactIntegerValue(result))
    return 1;
  // FIXME allow inexact value
  invalidCharacteristicValue(ident, loc);
  return 0;
}


bool Interpreter::convertLetter2C(ELObj *obj, const Identifier *ident, const Location &loc,
				  FOTBuilder::Letter2 &code)
{
  StringObj *strObj = obj->convertToString();
  if (strObj) {
    const StringC &str = *strObj;
    if (str.size() == 2
	&& str[0] >= 'A' && str[0] <= 'Z'
	&& str[1] >= 'A' && str[1] <= 'Z') {
      code = SP_LETTER2(str[0], str[1]);
      return 1;
    }
    if (str.size() == 0) {
      code = 0;
      return 1;
    }
  }
  else if (obj == makeFalse()) {
    code = 0;
    return 1;
  }
  invalidCharacteristicValue(ident, loc);
  return 0;
}

bool Interpreter::convertCharC(ELObj *obj, const Identifier *ident, const Location &loc,
			       Char &result)
{
  if (obj->charValue(result))
    return 1;
  const Char *s;
  size_t n;
  if (obj->stringData(s, n) && n == 1) {
    result = s[0];
    return 1;
  }
  invalidCharacteristicValue(ident, loc);
  return 0;
}

bool Interpreter::convertColorC(ELObj *obj, const Identifier *ident, const Location &loc, ColorObj *&color)
{
  color = obj->asColor();
  if (color)
    return 1;
  invalidCharacteristicValue(ident, loc);
  return 0;
}

bool Interpreter::convertOptColorC(ELObj *obj, const Identifier *ident, const Location &loc, ColorObj *&color)
{
  color = obj->asColor();
  if (color)
    return 1;
  if (obj == makeFalse())
    return 1;
  invalidCharacteristicValue(ident, loc);
  return 0;
}

bool Interpreter::convertRealC(ELObj *obj, const Identifier *ident, const Location &loc,
			       double &result)
{
  obj = convertFromString(obj, convertAllowNumber, loc);
  if (obj->realValue(result))
    return 1;
  invalidCharacteristicValue(ident, loc);
  return 0;
}

bool Interpreter::convertEnumC(ELObj *obj, const Identifier *ident, const Location &loc,
			       FOTBuilder::Symbol &sym)
{
  obj = convertFromString(obj, convertAllowSymbol|convertAllowBoolean, loc);
  if (obj == makeFalse()) {
    sym = FOTBuilder::symbolFalse;
    return 1;
  }
  SymbolObj *symObj = obj->asSymbol();
  if (symObj) {
    sym = symObj->cValue();
    if (sym != FOTBuilder::symbolFalse)
      return 1;
  }
  if (obj == makeTrue()) {
    sym = FOTBuilder::symbolTrue;
    return 1;
  }
  invalidCharacteristicValue(ident, loc);
  return 0;
}

bool Interpreter::convertEnumC(const FOTBuilder::Symbol *syms,  size_t nSyms,
			       ELObj *obj, const Identifier *ident, const Location &loc,
			       FOTBuilder::Symbol &result)
{
  obj = convertFromString(obj, convertAllowSymbol|convertAllowBoolean, loc);
  SymbolObj *symObj = obj->asSymbol();
  FOTBuilder::Symbol val;
  if (symObj) {
    val = symObj->cValue();
    if (val == FOTBuilder::symbolFalse) {
      invalidCharacteristicValue(ident, loc);
      return 0;
    }
  }
  else if (obj == makeFalse())
    val = FOTBuilder::symbolFalse;
  else if (obj == makeTrue())
    val = FOTBuilder::symbolTrue;
  else {
    invalidCharacteristicValue(ident, loc);
    return 0;
  }
  for (size_t i = 0; i < nSyms; i++)
    if (val == syms[i]) {
      result = val;
      return 1;
    }
  invalidCharacteristicValue(ident, loc);
  return 0;
}

void Interpreter::invalidCharacteristicValue(const Identifier *ident, const Location &loc)
{
  setNextLocation(loc);
  message(InterpreterMessages::invalidCharacteristicValue,
	  StringMessageArg(ident->name()));
}

static
bool equal(const Char *s1, const char *s2, size_t n)
{
  while (n > 0) {
    if (*s1++ != (unsigned char)*s2++)
      return 0;
    --n;
  }
  return 1;
}

ELObj *Interpreter::convertFromString(ELObj *obj, unsigned hints, const Location &loc)
{
  // FIXME fold to lower case
  const Char *s;
  size_t n;
  if (!dsssl2() || !obj->stringData(s, n))
    return obj;
  if (hints & convertAllowNumber) {
    ELObj *tem = convertNumber(StringC(s, n));
    if (tem)
      return tem->resolveQuantities(1, *this, loc);
  }
  if (hints & convertAllowSymbol) {
    StringC tem(s, n);
    SymbolObj *sym = symbolTable_.lookup(tem);
    if (sym && sym->cValue() != FOTBuilder::symbolFalse)
      return sym;
  }
  if (hints & convertAllowBoolean) {
    switch (n) {
    case 2:
      if (equal(s, "no", n))
	return makeFalse();
      break;
    case 3:
      if (equal(s, "yes", n))
	return makeTrue();
      break;
    case 4:
      if (equal(s, "true", n))
	return makeTrue();
      break;
    case 5:
      if (equal(s, "false", n))
	return makeFalse();
      break;
    }
  }
  return obj;
}

ELObj *Interpreter::convertNumber(const StringC &str, int radix)
{
  {
    if (str.size() == 0)
      return 0;
    size_t i = 0;
    if (str[0] == '#') {
      if (str.size() < 2)
	return 0;
      switch (str[1]) {
      case 'd':
	radix = 10;
	break;
      case 'x':
	radix = 16;
	break;
      case 'o':
	radix = 8;
	break;
      case 'b':
	radix = 2;
	break;
      default:
	return 0;
      }
      i += 2;
    }
    if (i >= str.size())
      return 0;
    bool negative;
    if (str[i] == '-') {
      negative = 1;
      i++;
    }
    else {
      negative = 0;
      if (str[i] == '+')
      i++;
    }
    bool hadDecimalPoint = 0;
    bool hadDigit = 0;
    long n = 0;
    int exp = 0;
    for (; i < str.size(); i++) {
      Char c = str[i];
      int weight;
      switch (c) {
      case '0':
	weight = 0;
	break;
      case '1':
	weight = 1;
	break;
      case '2':
	weight = 2;
	break;
      case '3':
	weight = 3;
	break;
      case '4':
	weight = 4;
	break;
      case '5':
	weight = 5;
	break;
      case '6':
	weight = 6;
	break;
      case '7':
	weight = 7;
	break;
      case '8':
	weight = 8;
	break;
      case '9':
	weight = 9;
	break;
      case 'a':
	weight = 10;
	break;
      case 'b':
	weight = 11;
	break;
      case 'c':
	weight = 12;
	break;
      case 'd':
	weight = 13;
	break;
      case 'e':
	weight = 14;
	break;
      case 'f':
	weight = 15;
	break;
      default:
	weight = -1;
	break;
      }
      if (weight >= 0 && weight < radix) {
	hadDigit = 1;
	if (negative) {
	  if (-(unsigned long)n > (-(unsigned long)LONG_MIN - weight)/radix) {
	    if (radix != 10)
	      return 0;
	    return convertNumberFloat(str);
	  }
	  else
	    n = n*radix - weight;
	}
	else {
	  if (n > (LONG_MAX - weight)/radix) {
	    if (radix != 10)
	      return 0;
	    return convertNumberFloat(str);
	  }
	  else
	    n = n*radix + weight;
	}
	if (hadDecimalPoint)
	  exp--;
      }
      else if (c == '.' && radix == 10) {
	if (hadDecimalPoint)
	  return 0;
	hadDecimalPoint = 1;
      }
      else
	break;
    }
    if (!hadDigit || (radix != 10 && i < str.size()))
      return 0;
    if (i + 1 < str.size() && str[i] == 'e'
        && lexCategory(str[i + 1]) != lexLetter) {
      hadDecimalPoint = 1;
      i++;
      int e;
      if (!scanSignDigits(str, i, e))
	return 0;
      exp += e;
    }
    if (i < str.size()) {
      int unitExp;
      Unit *unit = scanUnit(str, i, unitExp);
      if (!unit)
	return 0;
      if (unitExp == 1)
	return new (*this) UnresolvedLengthObj(n, exp, unit);
      else
	return convertNumberFloat(str);
    }
    if (hadDecimalPoint)
      return convertNumberFloat(str);
    return makeInteger(n);
  }
}

bool Interpreter::scanSignDigits(const StringC &str, size_t &i, int &n)
{
  bool negative = 0;
  if (i < str.size()) {
    if (str[i] == '-') {
      i++;
      negative = 1;
    } else if (str[i] == '+')
      i++;
  }
  size_t j = i;
  n = 0;
  while (i < str.size()
	 && ('0' <= str[i] && str[i] <= '9')) {
    if (negative)
      n = n*10 - (str[i] - '0');
    else
      n = n*10 + (str[i] - '0');
    i++;
  }
  if (i == j)
    return 0;
  return 1;
}

ELObj *Interpreter::convertNumberFloat(const StringC &str)
{
  String<char> buf;
  // omit an optional radix prefix
  size_t i0 = 0;
  if (str.size() > 1 && str[0] == '#' && str[1] == 'd')
    i0 = 2;
  for (size_t i = i0; i < str.size(); i++) {
    if (str[i] > CHAR_MAX || str[i] == '\0')
      return 0;
    // 'E' is a valid exponent marker for C but not us
    if (str[i] == 'E')
      break;
    buf += char(str[i]);
  }
  buf += '\0';
  const char *endPtr;
  double val = strtod((char *)buf.data(), (char **)&endPtr);
  if (endPtr - buf.data() == str.size() - i0)
    return new (*this) RealObj(val);
  if (endPtr == buf.data())
    return 0;
  int unitExp;
  Unit *unit = scanUnit(str, endPtr - buf.data() + i0, unitExp);
  if (!unit)
    return 0;
  return new (*this) UnresolvedQuantityObj(val, unit, unitExp);
}

// Return 0 for error.

Unit *Interpreter::scanUnit(const StringC &str, size_t i, int &unitExp)
{
  StringC unitName;
  while (i < str.size()) {
    if (str[i] == '-' || str[i] == '+' || ('0' <= str[i] && str[i] <= '9'))
      break;
    unitName += str[i++];
  }
  if (i >= str.size())
    unitExp = 1;
  else {
    unitExp = 0;
    bool neg = 0;
    if (str[i] == '-' || str[i] == '+') {
      if (str[i] == '-')
      neg = 1;
      i++;
      if (i >= str.size())
	return 0;
    }
    while (i < str.size()) {
      if (str[i] < '0' || str[i] > '9')
	return 0;
      unitExp *= 10;
      if (neg)
	unitExp -= (str[i] - '0');
      else
	unitExp += (str[i] - '0');
      i++;
    }
  }
  return lookupUnit(unitName);
}

void Interpreter::setNodeLocation(const NodePtr &nd)
{
  const LocNode *lnp;
  Location nodeLoc;
  if ((lnp = LocNode::convert(nd)) != 0
      && lnp->getLocation(nodeLoc) == accessOK)
    setNextLocation(nodeLoc);
}

bool Interpreter::convertToPattern(ELObj *obj, const Location &loc, Pattern &pattern)
{
  IList<Pattern::Element> list;
  if (!convertToPattern(obj, loc, 0, list))
    return 0;
  Pattern tem(list);
  tem.swap(pattern);
  return 1;
}

bool Interpreter::convertToPattern(ELObj *obj, const Location &loc,
				   bool isChild,
				   IList<Pattern::Element> &list)
{
  StringObj *str = obj->convertToString();
  if (str) {
    const Char *s;
    size_t n;
    str->stringData(s, n);
    if (!n) {
      setNextLocation(loc);
      message(InterpreterMessages::patternEmptyGi);
      return 0;
    }
    list.insert(new Pattern::Element(StringC(s, n)));
    return 1;
  }
  if (obj == makeTrue()) {
    list.insert(new Pattern::Element(StringC()));
    return 1;
  }
  Pattern::Element *curElement = 0;
  while (!obj->isNil()) {
    PairObj *pair = obj->asPair();
    if (!pair) {
      setNextLocation(loc);
      message(InterpreterMessages::patternNotList);
      return 0;
    }
    ELObj *head = pair->car();
    obj = pair->cdr();
    if (head == makeTrue() && dsssl2()) {
      list.insert(curElement = new Pattern::Element(StringC()));
      continue;
    }
    str = head->convertToString();
    if (str) {
      const Char *s;
      size_t n;
      str->stringData(s, n);
      if (!n) {
	setNextLocation(loc);
	message(InterpreterMessages::patternEmptyGi);
	return 0;
      }
      list.insert(curElement = new Pattern::Element(StringC(s, n)));
      continue;
    }
    if (!curElement) {
      setNextLocation(loc);
      message(InterpreterMessages::patternBadGi,
	      ELObjMessageArg(head, *this));
      return 0;
    }
    if (head->isNil())
      continue; // empty attribute list
    if (head->asPair()) {
      if (!patternAddAttributeQualifiers(head, loc, *curElement)) {
	setNextLocation(loc);
	message(InterpreterMessages::patternBadAttributeQualifier);
	return 0;
      }
      continue;
    }
    KeywordObj *key = dsssl2() ? head->asKeyword() : 0;
    if (!key) {
      setNextLocation(loc);
      message(InterpreterMessages::patternBadMember,
	      ELObjMessageArg(head, *this));
      return 0;
    }
    pair = obj->asPair();
    if (!pair) {
      setNextLocation(loc);
      message(obj->isNil()
	      ? InterpreterMessages::patternMissingQualifierValue
	      : InterpreterMessages::patternNotList);
      return 0;
    }
    ELObj *value = pair->car();
    obj = pair->cdr();
    Identifier::SyntacticKey k;
    if (!key->identifier()->syntacticKey(k)) {
      setNextLocation(loc);
      message(InterpreterMessages::patternUnknownQualifier,
	      StringMessageArg(key->identifier()->name()));
      return 0;
    }
    switch (k) {
    case Identifier::keyAttributes:
      if (!patternAddAttributeQualifiers(value, loc, *curElement)) {
	setNextLocation(loc);
	message(InterpreterMessages::patternBadAttributeQualifier);
	return 0;
      }
      break;
    case Identifier::keyChildren:
      {
	IList<Pattern::Element> children;
	if (!convertToPattern(value, loc, 1, children))
	  return 0;
	if (!children.empty())
	  curElement->addQualifier(new Pattern::ChildrenQualifier(children));
      }
      break;
    case Identifier::keyRepeat:
      {
	if (isChild) {
	  setNextLocation(loc);
	  message(InterpreterMessages::patternChildRepeat);
	  return 0;
	}
        SymbolObj *sym = value->asSymbol();
	if (sym) {
	  const StringC &str = *sym->name();
	  if (str.size() == 1) {
	    switch (str[0]) {
	    case '*':
	      curElement->setRepeat(0, Pattern::Repeat(-1));
	      value = 0;
	      break;
	    case '?':
	      curElement->setRepeat(0, 1);
	      value = 0;
	      break;
	    case '+':
	      curElement->setRepeat(1, Pattern::Repeat(-1));
	      value = 0;
	      break;
	    default:
	      break;
	    }
	  }
	}
	if (value) {
	  setNextLocation(loc);
	  message(InterpreterMessages::patternBadQualifierValue,
	          ELObjMessageArg(value, *this),
		  StringMessageArg(key->identifier()->name()));
	  return 0;
	}
      }
      break;
    case Identifier::keyPosition:
      {
        SymbolObj *sym = value->asSymbol();
	if (sym) {
	  Pattern::Qualifier *qual = 0;
	  const StringC &str = *sym->name();
	  if (str == "first-of-type")
	    qual = new Pattern::FirstOfTypeQualifier;
	  else if (str == "last-of-type")
	    qual = new Pattern::LastOfTypeQualifier;
	  else if (str == "first-of-any")
	    qual = new Pattern::FirstOfAnyQualifier;
	  else if (str == "last-of-any")
	    qual = new Pattern::LastOfAnyQualifier;
	  if (qual) {
	    curElement->addQualifier(qual);
	    break;
	  }
	}
        setNextLocation(loc);
	message(InterpreterMessages::patternBadQualifierValue,
	        ELObjMessageArg(value, *this),
		StringMessageArg(key->identifier()->name()));
	return 0;
      }
    case Identifier::keyOnly:
      {
        SymbolObj *sym = value->asSymbol();
	if (sym) {
	  Pattern::Qualifier *qual = 0;
	  const StringC &str = *sym->name();
	  if (str == "of-type")
	    qual = new Pattern::OnlyOfTypeQualifier;
	  else if (str == "of-any")
	    qual = new Pattern::OnlyOfAnyQualifier;
	  if (qual) {
	    curElement->addQualifier(qual);
	    break;
	  }
	}
	setNextLocation(loc);
    	message(InterpreterMessages::patternBadQualifierValue,
	        ELObjMessageArg(value, *this),
		StringMessageArg(key->identifier()->name()));
	return 0;
      }
      break;
    case Identifier::keyId:
      {
	StringObj *str = value->convertToString();
	if (!str) {
	  setNextLocation(loc);
	  message(InterpreterMessages::patternBadQualifierValue,
	          ELObjMessageArg(value, *this),
		  StringMessageArg(key->identifier()->name()));
	  return 0;
	}
	const Char *s;
	size_t n;
	str->stringData(s, n);
	curElement->addQualifier(new Pattern::IdQualifier(StringC(s, n)));
      }
      break;
    case Identifier::keyClass:
      {
	StringObj *str = value->convertToString();
	if (!str) {
	  setNextLocation(loc);
	  message(InterpreterMessages::patternBadQualifierValue,
	          ELObjMessageArg(value, *this),
		  StringMessageArg(key->identifier()->name()));
	  return 0;
	}
	const Char *s;
	size_t n;
	str->stringData(s, n);
	curElement->addQualifier(new Pattern::ClassQualifier(StringC(s, n)));
      }
      break;
    case Identifier::keyImportance:
      {
	long n;
	if (!value->exactIntegerValue(n)) {
	  setNextLocation(loc);
	  message(InterpreterMessages::patternBadQualifierValue,
	          ELObjMessageArg(value, *this),
		  StringMessageArg(key->identifier()->name()));
	  return 0;
	}
	curElement->addQualifier(new Pattern::ImportanceQualifier(n));
      }
      break;
    case Identifier::keyPriority:
      {
	long n;
	if (!value->exactIntegerValue(n)) {
	  setNextLocation(loc);
	  message(InterpreterMessages::patternBadQualifierValue,
	          ELObjMessageArg(value, *this),
		  StringMessageArg(key->identifier()->name()));
	  return 0;
	}
	curElement->addQualifier(new Pattern::PriorityQualifier(n));
      }
      break;
    default:
      setNextLocation(loc);
      message(InterpreterMessages::patternUnknownQualifier,
	      StringMessageArg(key->identifier()->name()));
      return 0;
    }
  }
  return 1;
}

bool Interpreter::patternAddAttributeQualifiers(ELObj *obj,
						const Location &loc,
						Pattern::Element &elem)
{
  while (!obj->isNil()) {
    PairObj *pair = obj->asPair();
    if (!pair)
      return 0;
    StringObj *tem = pair->car()->convertToString();
    if (!tem)
      return 0;
    const Char *s;
    size_t n;
    tem->stringData(s, n);
    if (n == 0)
      return 0;
    StringC name(s, n);
    obj = pair->cdr();
    pair = obj->asPair();
    if (!pair)
      return 0;
    obj = pair->cdr();
    if (pair->car() == makeFalse() && dsssl2())
      elem.addQualifier(new Pattern::AttributeMissingValueQualifier(name));
    else if (pair->car() == makeTrue() && dsssl2())
      elem.addQualifier(new Pattern::AttributeHasValueQualifier(name));
    else {
      tem = pair->car()->convertToString();
      if (!tem)
	return 0;
      tem->stringData(s, n);
      elem.addQualifier(new Pattern::AttributeQualifier(name, StringC(s, n)));
    }
  }
  return 1;
}

void Interpreter::dispatchMessage(Message &msg)
{
  messenger_->dispatchMessage(msg);
}

void Interpreter::dispatchMessage(const Message &msg)
{
  messenger_->dispatchMessage(msg);
}

Interpreter::StringSet::StringSet()
{
}

const char *Interpreter::StringSet::store(String<char> &str)
{
  str += '\0';
  const String<char> *p = table_.lookup(str);
  if (!p) {
    String<char> *tem = new String<char>;
    str.swap(*tem);
    table_.insert(tem);
    p = tem;
  }
  return p->data();
}

unsigned long Interpreter::StringSet::hash(const String<char> &str)
{
  const char *p = str.data();
  unsigned long h = 0;
  for (size_t n = str.size(); n > 0; n--)
    h = (h << 5) + h + (unsigned char)*p++;	// from Chris Torek
  return h;
}

bool Identifier::preferBuiltin_ = 0;

Identifier::Identifier(const StringC &name)
: Named(name), value_(0), syntacticKey_(notKey), beingComputed_(0), 
  flowObj_(0), builtin_(0), defPart_(0), charNIC_(0), 
  feature_(Interpreter::noFeature)
{
}

void Identifier::maybeSaveBuiltin()
{
  if (defPart_ == unsigned(-1) && !builtin_) {
    builtin_ = new Identifier(name());
    if (value_)
      builtin_->setValue(value_, defPart_);
    else
      builtin_->setDefinition(def_, defPart_, defLoc_);
  }
}
 
void Identifier::setDefinition(Owner<Expression> &expr,
			       unsigned part,
			       const Location &loc)
{
  maybeSaveBuiltin();
  def_.swap(expr);
  defPart_ = part;
  defLoc_ = loc;
  value_ = 0;
}

void Identifier::setBuiltinDefinition(Owner<Expression> &expr,
				     unsigned part,
				     const Location &loc)
{
  if (!builtin_) { 
    builtin_ = new Identifier(name());
    builtin_->setDefinition(expr, part, loc);
  }
}

void Identifier::setValue(ELObj *value, unsigned partIndex)
{
  maybeSaveBuiltin();
  value_ = value;
  // Built in functions have lowest priority.
  defPart_ = partIndex;
}

bool Identifier::defined(unsigned &part, Location &loc) const
{
  if (builtin_ && preferBuiltin_)
    return builtin_->defined(part, loc);
  if (!def_ && !value_) 
    return 0;
  part = defPart_;
  loc = defLoc_;
  return 1;
}

ELObj *Identifier::computeValue(bool force, Interpreter &interp) const
{
  if (builtin_ && preferBuiltin_)
    return builtin_->computeValue(force, interp);
  if (value_)
    return value_;
  bool preferred = 0;
  if (defPart_ == unsigned(-1) && !preferBuiltin_) {
    preferBuiltin_ = 1;
    preferred = 1;
  }
  ASSERT(def_);
  if (beingComputed_) {
    if (force) {
      interp.setNextLocation(defLoc_);
      interp.message(InterpreterMessages::identifierLoop,
	             StringMessageArg(name()));
      ((Identifier *)this)->value_ = interp.makeError();
    }
  }
  else {
    ((Identifier *)this)->beingComputed_ = 1;
    if (insn_.isNull())
      ((Identifier *)this)->insn_ 
        = Expression::optimizeCompile(((Identifier *)this)->def_, interp,
	                              Environment(), 0, InsnPtr());
    if (force || def_->canEval(0)) {
      VM vm(interp);
      ELObj *v = vm.eval(insn_.pointer());
      interp.makePermanent(v);
      ((Identifier *)this)->value_ = v;
    }
    ((Identifier *)this)->beingComputed_ = 0;
  }
  if (preferred)
    preferBuiltin_ = 0;
  return value_;
}

ELObj *Identifier::computeBuiltinValue(bool force, Interpreter &interp) const
{
  preferBuiltin_ = 1;
  ELObj *res = computeValue(force, interp);
  preferBuiltin_ = 0;
  return res;
}

Unit::Unit(const StringC &name)
: Named(name), computed_(notComputed)
{
}

bool Unit::defined(unsigned &part, Location &loc) const
{
  if (!def_ && computed_ == notComputed)
    return 0;
  part = defPart_;
  loc = defLoc_;
  return 1;
}

void Unit::setDefinition(Owner<Expression> &expr,
			 unsigned part,
			 const Location &loc)
{
  def_.swap(expr);
  defPart_ = part;
  defLoc_ = loc;
  computed_ = notComputed;
}

void Unit::setValue(long n)
{
  computed_ = computedExact;
  exact_ = n;
  dim_ = 1;
  defPart_ = unsigned(-1);
}

void Unit::setValue(double n)
{
  computed_ = computedInexact;
  inexact_ = n;
  dim_ = 1;
  defPart_ = unsigned(-1);
}

void Unit::tryCompute(bool force, Interpreter &interp)
{
  if (computed_ == notComputed) {
    computed_ = beingComputed;
    if (insn_.isNull())
      insn_ = Expression::optimizeCompile(def_, interp, Environment(), 0, InsnPtr());
    if (force || def_->canEval(0)) {
      VM vm(interp);
      ELObj *v = vm.eval(insn_.pointer());
      switch (v->quantityValue(exact_, inexact_, dim_)) {
      case ELObj::noQuantity:
	if (!interp.isError(v)) {
	  interp.setNextLocation(defLoc_);
	  interp.message(InterpreterMessages::badUnitDefinition,
			 StringMessageArg(name()));
	}
	computed_ = computedError;
	break;
      case ELObj::longQuantity:
	computed_ = computedExact;
	break;
      case ELObj::doubleQuantity:
	computed_ = computedInexact;
	break;
      default:
	CANNOT_HAPPEN();
      }
    }
    if (computed_ == beingComputed)
      computed_ = notComputed;
  }
  else if (computed_ == beingComputed) {
    interp.setNextLocation(defLoc_);
    interp.message(InterpreterMessages::unitLoop,
		   StringMessageArg(name()));
    computed_ = computedError;
  }
}

// multiply by 10^valExp
// quantity has exponent of 1

ELObj *Unit::resolveQuantity(bool force, Interpreter &interp,
			     long val, int valExp)
{
  tryCompute(force, interp);
  long result;
  if (computed_ == computedExact && scale(val, valExp, exact_, result))
    return new (interp) LengthObj(result);
  double x = val;
  while (valExp > 0) {
    x *= 10.0;
    valExp--;
  }
  while (valExp < 0) {
    x /= 10.0;
    valExp++;
  }
  return resolveQuantity(force, interp, x, 1);
}

// val * 10^valExp * factor
// return 0 if it can't be done without overflow

bool Unit::scale(long val, int valExp, long factor, long &result)
{
  if (factor <= 0)
    return 0; // feeble
  while (valExp > 0) {
    if (factor > LONG_MAX/10)
      return 0;
    valExp--;
    factor *= 10;
  }
  if (val >= 0) {
    if (val > LONG_MAX/factor)
      return 0;
  }
  else {
    if (-(unsigned long)val > -(unsigned long)LONG_MIN/factor)
      return 0;
  }
  result = val*factor;
  while (valExp < 0) {
    result /= 10;
    valExp++;
  }
  return 1;
}

ELObj *Unit::resolveQuantity(bool force, Interpreter &interp,
			     double val, int unitExp)
{
  tryCompute(force, interp);
  double factor;
  switch (computed_) {
  case computedExact:
    factor = exact_;
    break;
  case computedInexact:
    factor = inexact_;
    break;
  case computedError:
    return interp.makeError();
  default:
    return 0;
  }
  int resultDim = 0;
  double resultVal = val;
  while (unitExp > 0) {
    resultDim += dim_;
    resultVal *= factor;
    unitExp--;
  }
  while (unitExp < 0) {
    resultDim -= dim_;
    resultVal /= factor;
    unitExp++;
  }
  if (resultDim == 0)
    return new (interp) RealObj(resultVal);
  return new (interp) QuantityObj(resultVal, resultDim);
}

void ELObjDynamicRoot::trace(Collector &c) const
{
  c.trace(obj_);
}

bool operator==(const StringC &s, const char *p)
{
  for (size_t i = 0; i < s.size(); i++)
    if (p[i] == '\0' || (unsigned char)p[i] != s[i])
      return 0;
  return p[s.size()] == '\0';
}

void Interpreter::installBuiltins()
{
  partIndex_ = unsigned(-1);
  StringC sysid(makeStringC(DEFAULT_SCHEME_BUILTINS));
  StringC src;
  groveManager_->mapSysid(sysid);
  if (groveManager_->readEntity(sysid, src)) {
    Owner<InputSource> in(new InternalInputSource(src,
                              InputSourceOrigin::make()));
    SchemeParser scm(*this, in);
    scm.parse();
  }
  endPart();
  partIndex_ = 0;
}

void Interpreter::setDefaultLanguage(Owner<Expression> &expr,
				     unsigned part,
				     const Location &loc)
{
  defaultLanguageDef_.swap(expr);
  defaultLanguageDefPart_ = part;
  defaultLanguageDefLoc_ = loc;
}

ELObj *Interpreter::defaultLanguage() const
{
  return defaultLanguage_;
}

bool Interpreter::defaultLanguageSet(unsigned &part,Location &loc) const
{
  if(defaultLanguageDef_) {
    part = defaultLanguageDefPart_;
    loc = defaultLanguageDefLoc_;
    return true;
  }
  return false;
}

void Interpreter::compileDefaultLanguage()
{
  if(defaultLanguageDef_) {
    InsnPtr insn = Expression::optimizeCompile(defaultLanguageDef_, *this,
					       Environment(), 0, InsnPtr());
    VM vm(*this);
    ELObj *obj = vm.eval(insn.pointer());
    if(!obj->asLanguage()) {
      if(!isError(obj)) {
	setNextLocation(defaultLanguageDefLoc_);
	message(InterpreterMessages::defLangDeclRequiresLanguage,
		ELObjMessageArg(obj, *this));
      }
      return;
    }
    makePermanent(obj);
    defaultLanguage_ = obj;
  }
}

void Interpreter::installCharProperties()
{
  //FIXME convert this to tables

  // NOte that default values are set in the constructors.

  lookupCharProperty(makeStringC("numeric-equiv"))->
    declarePredefined(&numericEquivCPV_);
  if (!strictMode_) {
    for (int i = 0; i < 10; i++) 
      numericEquivCPV_.setValue(i+'0', i);
  }
  
  if (style()) {
    lookupCharProperty(makeStringC("space?"))->
      declarePredefined(&isSpaceCPV_);
    if (!strictMode_) {
      static struct {
	Char c;
	unsigned short num;
      } chars[] = {
#define SPACE 
#include "charProps.h"
#undef SPACE
      };
      for (size_t i = 0; i < SIZEOF(chars); i++) 
	isSpaceCPV_.setRange(chars[i].c, chars[i].c+chars[i].num-1, true);
    }

    lookupCharProperty(makeStringC("record-end?"))->
      declarePredefined(&isRecordEndCPV_);
    if (!strictMode_) {
      static struct {
	Char c;
	unsigned short num;
      } chars[] = {
#define RECORD_END
#include "charProps.h"
#undef RECORD_END
      };
      for (size_t i = 0; i < SIZEOF(chars); i++) 
	isRecordEndCPV_.setRange(chars[i].c, chars[i].c+chars[i].num-1, true);
    }

    lookupCharProperty(makeStringC("blank?"))->
      declarePredefined(&isBlankCPV_);
    if (!strictMode_) {
      static struct {
	Char c;
	unsigned short num;
      } chars[] = {
#define BLANK
#include "charProps.h"
#undef BLANK
      };
      for (size_t i = 0; i < SIZEOF(chars); i++) 
	isBlankCPV_.setRange(chars[i].c, chars[i].c+chars[i].num-1, true);
    }

    lookupCharProperty(makeStringC("input-tab?"))->
      declarePredefined(&isInputTabCPV_);
    if (!strictMode_) {
      static struct {
	Char c;
	unsigned short num;
      } chars[] = {
#define INPUT_TAB
#include "charProps.h"
#undef INPUT_TAB
      };
      for (size_t i = 0; i < SIZEOF(chars); i++) 
	isInputTabCPV_.setRange(chars[i].c, chars[i].c+chars[i].num-1, true);
    }

    lookupCharProperty(makeStringC("input-whitespace?"))->
      declarePredefined(&isInputWhitespaceCPV_);
    if (!strictMode_) {
      static struct {
	Char c;
	unsigned short num;
      } chars[] = {
#define INPUT_WHITESPACE
#include "charProps.h"
#undef INPUT_WHITESPACE
      };
      for (size_t i = 0; i < SIZEOF(chars); i++) 
	isInputWhitespaceCPV_.setRange(chars[i].c,
				       chars[i].c+chars[i].num-1, true);
    }

    lookupCharProperty(makeStringC("punct?"))->
      declarePredefined(&isPunctCPV_);
    if (!strictMode_) {
      static struct {
	Char c;
	unsigned short num;
      } chars[] = {
#define PUNCT
#include "charProps.h"
#undef PUNCT
      };
      for (size_t i = 0; i < SIZEOF(chars); i++) 
	isPunctCPV_.setRange(chars[i].c, chars[i].c+chars[i].num-1, true);
    }
    
    lookupCharProperty(makeStringC("script"))->
      declarePredefined(&scriptCPV_);
    if (!strictMode_) {
      static struct {
	Char c1, c2;
	char *suffix;
      } chars[] = {
#define SCRIPT
#include "charProps.h"
#undef SCRIPT
      };
      StringC prefix = makeStringC("ISO/IEC 10179::1996//Script::");
      for (size_t i = 0; i < SIZEOF(chars); i++) { 
	StringC tem(prefix);
	tem += makeStringC(chars[i].suffix);
	scriptCPV_.setRange(chars[i].c1, chars[i].c2,
			    storePublicId(tem.data(), tem.size(), Location()));
      }
    }

    lookupCharProperty(makeStringC("glyph-id"))->
      declarePredefined(&glyphIdCPV_);
    // FIXME. Default values?

    lookupCharProperty(makeStringC("drop-after-line-break?"))->
      declarePredefined(&isDropAfterLineBreakCPV_);
    // FIXME: Values.

    lookupCharProperty(makeStringC("drop-unless-before-line-break?"))->
      declarePredefined(&isDropUnlessBeforeLineBreakCPV_);
    // FIXME: Values.

    lookupCharProperty(makeStringC("break-before-priority"))->
      declarePredefined(&breakBeforePriorityCPV_);
    lookupCharProperty(makeStringC("break-after-priority"))->
      declarePredefined(&breakAfterPriorityCPV_);
    if(!strictMode_) {
      static struct {
	Char c;
	unsigned short num;
	unsigned short bbp;
	unsigned short bap;
      } chars[] = {
#define BREAK_PRIORITIES
#include "charProps.h"
#undef BREAK_PRIORITIES
      };
      for (size_t i = 0; i < SIZEOF(chars); ++i) {
	breakBeforePriorityCPV_.setRange(chars[i].c, chars[i].c + chars[i].num-1,
					 chars[i].bbp);
	breakAfterPriorityCPV_.setRange(chars[i].c, chars[i].c + chars[i].num-1,
					chars[i].bap);
      }
    }

    lookupCharProperty(makeStringC("math-class"))->
      declarePredefined(&mathClassCPV_);
    // FIXME: Values.

    lookupCharProperty(makeStringC("math-font-posture"))->
      declarePredefined(&mathFontPostureCPV_);
    // FIXME: Values.
  }
}

void
Interpreter::compileCharProperties()
{
  NamedTableIter<CharProp> iter(charPropTable_);
  CharProp *val;
  while ((val = iter.next())) 
    val->compile(*this);
}

CharPropValues *Interpreter::installExtensionCharNIC(Identifier *ident,
					  const StringC &pubid,
					  const Location &loc)
{
  ident->setCharNIC(currentPartIndex(), loc);
  // FIXME! Shouldn't even get into CVS!
  return new ELObjCharPropValues;
}

ELObj *CharProp::value(Char ch, ELObj *def,
		       const Location &loc, Interpreter &interp)
{
  addedProp_ *p=(addedProp_ *)added_(ch);
  if (p != 0 && p->computed_ != computedOK) {
    compileAdded_(*p,interp);
    if(p->computed_ == computedError) {
      if (!values_) {
	interp.setNextLocation (loc);
	interp.message (InterpreterMessages::unknownCharProperty,
			StringMessageArg (name()));
      }
      return interp.makeError();
    }
    ASSERT(p->computed_ == computedOK);
  }
  ASSERT(values_);
  ELObj *tem = values_->value(ch, interp);
  if(tem)
    return tem;
  else if (def)
    return def;
  if (defComputed_ != computedOK) {
    compileDef_ (interp);
    if (defComputed_ == computedError)
      return interp.makeError();
    ASSERT(defComputed_ == computedOK);
  }
  tem = values_->defaultValue(interp);
  ASSERT(tem);
  return tem;
}

void CharProp::setValue(StringC &chars, Owner<Expression> &expr,
			unsigned part, const Location &loc)
{
  ASSERT(chars.size()>0);
  addedPropsVec_.resize (addedPropsVec_.size() + 1);
  addedProp_ &p = addedPropsVec_.back();
  p.chars_.swap(chars);
  p.part_ = part;
  p.loc_ = loc;
  p.expr_.swap(expr);
  p.computed_ = notComputed;

  if(!addedProps_)
    addedProps_ = new CharMap<NCVector<addedProp_>::size_type> (noAddedVal_);
  for(size_t i = 0; i < p.chars_.size(); ++i) {
    ASSERT((*addedProps_)[p.chars_[i]] == noAddedVal_);
    addedProps_->setChar(p.chars_[i],addedPropsVec_.size() - 1);
  }
}

void CharProp::declare(Owner<Expression> &expr,
		       unsigned part,
		       const Location &loc,
		       CharPropValues *values)
{
  ASSERT(!defExpr_);
  ASSERT(expr);
  defExpr_.swap(expr);
  defPart_ = part;
  defLoc_ = loc;
  if (values)
    values_ = values;
  else if (!values_) {
    // FIXME! Shall not get into CVS!
    values_ = new ELObjCharPropValues ();
  }
}

void CharProp::declarePredefined(CharPropValues *values)
{
  values_ = values;
}

bool CharProp::hasAddedValue(Char ch,unsigned &part,Location &loc) const
{
  const addedProp_ *p = added_(ch);
  if(p) {
    part = p->part_;
    loc = p->loc_;
    return true;
  }
  return false;
}

bool CharProp::declared(unsigned &part, Location &loc) const
{
  if(defExpr_) {
    part = defPart_;
    loc = defLoc_;
    return true;
  }
  return false;
}

void CharProp::compile(Interpreter &interp)
{
  compileDef_(interp);
  for(NCVector<addedProp_>::size_type i = 0; i < addedPropsVec_.size(); ++i)
    compileAdded_(addedPropsVec_[i],interp);
  if (!hadError_) {
    addedProps_.clear();
    addedPropsVec_.clear();
  }
}
    
void CharProp::compileDef_(Interpreter &interp)
{
  switch(defComputed_) {
  case notComputed:
    if (defExpr_) {
      defComputed_ = beingComputed;
      InsnPtr insn = Expression::optimizeCompile (defExpr_, interp,
						  Environment(), 0, InsnPtr());
      VM vm (interp);
      ELObj *p = vm.eval (insn.pointer());
      if (!interp.isError(p)
	  && values_->setDefault(name(), defLoc_, p, interp))
	defComputed_ = computedOK;
      else
	defComputed_ = computedError;
    }
    else if (values_) {
      defComputed_ = computedOK;
    } else
      defComputed_ = computedError;
    break;
  case beingComputed:
    interp.setNextLocation(defLoc_);
    interp.message(InterpreterMessages::charPropertyLoop,
		    StringMessageArg(name()));
    defComputed_ = computedError;
    break;
  default:
    ;
  }
}

void CharProp::compileAdded_(addedProp_ &added, Interpreter &interp)
{
  switch(added.computed_) {
  case notComputed: { // Own block since InsnPtr and VM have destructors.
    // Check that property is declared.
    if (!values_) {
      interp.setNextLocation (added.loc_);
      interp.message (InterpreterMessages::unknownCharProperty,
		      StringMessageArg (name()));
      added.computed_ = computedError;
      hadError_ = true;
      return;
    }
    ASSERT(added.expr_);
    added.computed_ = beingComputed;
    InsnPtr insn = Expression::optimizeCompile (added.expr_, interp,
						Environment(), 0, InsnPtr());
    VM vm (interp);
    ELObj *p = vm.eval (insn.pointer());
    if (!interp.isError(p) &&
	values_->setValue(name(), added.chars_, added.loc_, p, interp))
      added.computed_ = computedOK;
    else {
      added.computed_ = computedError;
      hadError_ = true;
    }
    break;
  }
  case beingComputed:
    interp.setNextLocation(added.loc_);
    interp.message(InterpreterMessages::charPropertyLoop,
		    StringMessageArg(name()));
    added.computed_ = computedError;
    break;
  default:
    ;
  }
}

const CharProp::addedProp_ *CharProp::added_(Char ch) const
{
  if (!addedProps_)
    return 0;
  NCVector<addedProp_>::size_type n((*addedProps_)[ch]);
  if (n == noAddedVal_)
    return 0;
  return &addedPropsVec_[n];
}

bool IntegerCharPropValues::setDefault(const StringC &name,
				       const Location &loc,
				       ELObj *obj, Interpreter &interp)
{
  if (!obj->exactIntegerValue(def_)) {
    interp.setNextLocation(loc);
    interp.message(InterpreterMessages::charPropertyNotInteger,
		   StringMessageArg(name),
		   ELObjMessageArg(obj, interp));
    return false;
  }
  return true;
}

bool IntegerCharPropValues::setValue(const StringC &name, const StringC &chars,
				     const Location &loc,
				     ELObj *obj, Interpreter &interp)
{
  ValT_ v;
  if (!obj->exactIntegerValue(v.l_)) {
    interp.setNextLocation(loc);
    interp.message(InterpreterMessages::charPropertyNotInteger,
		   StringMessageArg(name),
		   ELObjMessageArg(obj, interp));
    return false;
  }
  v.hasValue_ = true;
  for(size_t i=0; i<chars.size(); ++i)
    map_.setChar(chars[i],v);
  return true;
}
  
ELObj *IntegerCharPropValues::value(Char ch, Interpreter &interp) const
{
  ValT_ v=map_[ch];
  if(v.hasValue_)
    return interp.makeInteger(v.l_);
  return 0;
}

ELObj *IntegerCharPropValues::defaultValue(Interpreter &interp) const
{
  return interp.makeInteger(def_);
}

bool MaybeIntegerCharPropValues::setDefault(const StringC &name,
					    const Location &loc,
					    ELObj *obj, Interpreter &interp)
{
  if (obj->exactIntegerValue(def_) || (defFalse_ = (!obj->isTrue())))
    return true;
  interp.setNextLocation(loc);
  interp.message(InterpreterMessages::charPropertyNotIntegerOrFalse,
		 StringMessageArg(name),
		 ELObjMessageArg(obj, interp));
}

bool MaybeIntegerCharPropValues::setValue(const StringC &name,
					  const StringC &chars,
					  const Location &loc,
					  ELObj *obj,
					  Interpreter &interp)
{
  ValT_ v;
  if (obj->exactIntegerValue(v.l_) || (v.isFalse_ = (!obj->isTrue()))) {
    v.hasValue_ = true;
    for (size_t i = 0; i < chars.size(); ++i)
      map_.setChar(chars[i], v);
    return true;
  }
  interp.setNextLocation(loc);
  interp.message(InterpreterMessages::charPropertyNotIntegerOrFalse,
		 StringMessageArg(name),
		 ELObjMessageArg(obj, interp));
  return false;
}

ELObj *MaybeIntegerCharPropValues::value(Char ch, Interpreter &interp) const
{
  ValT_ v(map_[ch]);
  if (v.hasValue_) {
    if (v.isFalse_)
      return interp.makeFalse();
    else
      return interp.makeInteger(v.l_);
  }
  return 0;
}
    
ELObj *MaybeIntegerCharPropValues::defaultValue(Interpreter &interp) const
{
  if (defFalse_)
    return interp.makeFalse();
  return interp.makeInteger(def_);
}

bool BooleanCharPropValues::setDefault(const StringC &, const Location &,
				       ELObj *obj, Interpreter &)
{
  def_ = obj->isTrue();
  return 1;
}

bool BooleanCharPropValues::setValue(const StringC &, const StringC &chars,
				     const Location &, ELObj *obj,
				     Interpreter &)
{
  ValT_ v;
  v.hasValue_ = true;
  v.b_ = obj->isTrue();
  for (size_t i = 0; i < chars.size(); ++i)
    map_.setChar (chars[i], v);
  return true;
}

ELObj *BooleanCharPropValues::value(Char ch, Interpreter &interp) const
{
  ValT_ v(map_[ch]);
  if (v.hasValue_)
    return v.b_ ? interp.makeTrue() : interp.makeFalse();
  return 0;
}

ELObj *BooleanCharPropValues::defaultValue(Interpreter &interp) const
{
  return def_ ? interp.makeTrue() : interp.makeFalse();
}

bool PublicIdCharPropValues::setDefault(const StringC &name,
					const Location &loc,
					ELObj *obj, Interpreter &interp)
{
  const Char *c;
  size_t n;
  if (obj->stringData (c, n)) {
    def_ = interp.storePublicId(c, n, loc);
    return 1;
  }
  if (obj == interp.makeFalse()) {
    def_ = 0;
    return 1;
  }
  interp.setNextLocation(loc);
  interp.message(InterpreterMessages::charPropertyNotPublicId,
		 StringMessageArg(name),
		 ELObjMessageArg(obj, interp));
  return 0;
}

bool PublicIdCharPropValues::setValue(const StringC &name,
				      const StringC &chars,
				      const Location &loc,
				      ELObj *obj, Interpreter &interp)
{
  const Char *c;
  size_t n;
  FOTBuilder::PublicId pubid;
  if (obj->stringData(c, n))
    pubid = interp.storePublicId(c, n, loc);
  else if (obj == interp.makeFalse())
    pubid = 0;
  else {
    interp.setNextLocation(loc);
    interp.message(InterpreterMessages::charPropertyNotPublicId,
		   StringMessageArg(name),
		   ELObjMessageArg(obj, interp));
    return 0;
  }
  for (size_t i = 0; i < chars.size(); ++i)
    map_[chars[i]] = pubid;
  return 1;
}

ELObj *PublicIdCharPropValues::value(Char, Interpreter &) const
{
}

ELObj *PublicIdCharPropValues::defaultValue(Interpreter &) const
{
}

bool SymbolCharPropValues::setDefault(const StringC &, const Location &,
		  ELObj *, Interpreter &)
{
}

bool SymbolCharPropValues::setValue(const StringC &, const StringC &, const Location &,
		ELObj *,Interpreter &)
{
}

ELObj *SymbolCharPropValues::value(Char, Interpreter &) const
{
}

ELObj *SymbolCharPropValues::defaultValue(Interpreter &) const
{
}

bool ELObjCharPropValues::setDefault(const StringC &, const Location &,
				     ELObj *obj, Interpreter &interp)
{
  ASSERT(obj);
  interp.makePermanent (obj);
  def_ = obj;
}

bool ELObjCharPropValues::setValue(const StringC &, const StringC &chars,
				   const Location &,
				   ELObj *obj, Interpreter &interp)
{
  ASSERT(obj);
  interp.makePermanent (obj);
  for(size_t i = 0; i < chars.size(); ++i)
    map_.setChar(chars[i], obj);
}

ELObj *ELObjCharPropValues::value(Char ch, Interpreter &) const
{
  return map_[ch];
}

ELObj *ELObjCharPropValues::defaultValue(Interpreter &interp) const
{
  return def_;
}

void Interpreter::installFeatures(const FOTBuilder::Feature *backendFeatures)
{
  feature_[combineChar].rcsname =          makeStringC("combine-char");
  feature_[keyword].rcsname =              makeStringC("keyword");
  feature_[multiSource].rcsname =          makeStringC("multi-source");
  feature_[multiResult].rcsname =          makeStringC("multi-result");
  feature_[regexp].rcsname =               makeStringC("regexp");
  feature_[word].rcsname =                 makeStringC("word");
  feature_[hytime].rcsname =               makeStringC("hytime");
  feature_[charset].rcsname =              makeStringC("charset");
  feature_[expression].rcsname =           makeStringC("expression");
  feature_[multiProcess].rcsname =         makeStringC("multi-process");
  feature_[query].rcsname =                makeStringC("query");
  feature_[sideBySide].rcsname =           makeStringC("side-by-side");
  feature_[sideline].rcsname =             makeStringC("sideline");
  feature_[alignedColumn].rcsname =        makeStringC("aligned-column");
  feature_[bidi].rcsname =                 makeStringC("bidi");
  feature_[vertical].rcsname =             makeStringC("vertical");
  feature_[math].rcsname =                 makeStringC("math");
  feature_[table].rcsname =                makeStringC("table");
  feature_[tableAutoWidth].rcsname =       makeStringC("table-auto-width");
  feature_[simplePage].rcsname =           makeStringC("simple-page");
  feature_[page].rcsname =                 makeStringC("page");
  feature_[multiColumn].rcsname =          makeStringC("multi-column");
  feature_[nestedColumnSet].rcsname =      makeStringC("nested-column-set");
  feature_[generalIndirect].rcsname =      makeStringC("general-indirect");
  feature_[inlineNote].rcsname =           makeStringC("inline-note");
  feature_[glyphAnnotation].rcsname =      makeStringC("glyph-annotation");
  feature_[emphasizingMark].rcsname =      makeStringC("emphasizing-mark");
  feature_[includedContainer].rcsname =    makeStringC("included-container");
  feature_[actualCharacteristic].rcsname = makeStringC("actual-characteristic");
  feature_[online].rcsname =               makeStringC("online");
  feature_[fontInfo].rcsname =             makeStringC("font-info");
  feature_[crossReference].rcsname =       makeStringC("cross-reference");

  for (int i = 0; i < nFeatures; i++) 
    feature_[i].supported = notSupported;
  feature_[charset].supported = partiallySupported;
  feature_[expression].supported = supported;
  feature_[multiProcess].supported = supported;
  feature_[query].supported = partiallySupported;
  feature_[actualCharacteristic].supported = supported;
  feature_[crossReference].supported = supported;
  if (backendFeatures) 
    for (const FOTBuilder::Feature *b = backendFeatures; b->name; b++) {
      StringC name = makeStringC(b->name);
      for (int i = 0; i < nFeatures; i++) 
	if (name == feature_[i].rcsname) {
	  feature_[i].supported = b->partial ? partiallySupported : supported;
	  break;
	}
    }

  feature_[noFeature].declared = 1;
  for (int i = 1; i < nFeatures; i++) 
    feature_[i].declared = 0;
  if (!strictMode_) 
    for (int i = 0; i < nFeatures; i++) 
      if (feature_[i].supported != notSupported)
	feature_[i].declared = 1;
  if (!style()) {
    feature_[query].declared = 1; 
    feature_[expression].declared = 1;
  }
}

bool Interpreter::requireFeature(const Interpreter::Feature &f,
                                 const Location &loc)
{
   //FIXME: Hack to avoid warnings about features used in builtins.dsl
   if (!feature_[f].declared 
       && !Identifier::preferBuiltin_  
       && (partIndex_ != unsigned(-1))) {
     setNextLocation(loc);
     message(InterpreterMessages::missingFeature,
             StringMessageArg(feature_[f].rcsname));
   }
   return feature_[f].declared;
}

bool Interpreter::convertFeature(const StringC &name, Interpreter::Feature &f)
{
  for (int i = 1; i < nFeatures; i++) 
    if (feature_[i].rcsname == name) {
      f = Feature(i);
      return 1;
    }
  return 0;
}

void Interpreter::explicitFeatures()
{
  if (!explicitFeatures_) {
    explicitFeatures_ = 1;
    for (int i = 1; i < nFeatures; i++) 
      feature_[i].declared = 0;
    if (!style()) {
      feature_[query].declared = 1; 
      feature_[expression].declared = 1;
    }
  }
}

void Interpreter::declareFeature(const StringC &name)
{  
  Feature feature;
  if (!convertFeature(name, feature)) {
    message(InterpreterMessages::unknownFeature,
	     StringMessageArg(name));
    return;
  }
  declareFeature(feature);
}

void Interpreter::declareFeature(const Feature &feature)
{
  Status &f = feature_[feature];
  f.declared = 1;
  if (f.supported == notSupported) 
    message(InterpreterMessages::unsupportedFeature,
	    StringMessageArg(f.rcsname));
  else if (f.supported == partiallySupported) 
    message(InterpreterMessages::partiallySupportedFeature,
	    StringMessageArg(f.rcsname));
  // Handle implied features
  switch (feature) {
  case query:
    declareFeature(multiProcess);
    break;
  case tableAutoWidth:
    declareFeature(table);
    break;
  case nestedColumnSet:
    declareFeature(multiColumn);
    break;
  case multiColumn:
    declareFeature(page);
    break;
  default:; // do nothing
  }
}


bool Identifier::requireFeature(Interpreter &interp, const Location &loc, bool fo) const
{
  if ((fo ? flowObjPart_ : defPart_) == unsigned(-1))
    return interp.requireFeature(Interpreter::Feature(feature_), loc);
  else
    return 1;
}

void Identifier::setFeature(int f)
{
  feature_ = f;
}

void Interpreter::installModules()
{
  module_[baseabs].rcsname = makeStringC("BASEABS"); 
  module_[prlgabs0].rcsname = makeStringC("PRLGABS0"); 
  module_[prlgabs1].rcsname = makeStringC("PRLGABS1"); 
  module_[instabs].rcsname = makeStringC("INSTABS"); 
  module_[basesds0].rcsname = makeStringC("BASESDS0"); 
  module_[basesds1].rcsname = makeStringC("BASESDS1"); 
  module_[instsds0].rcsname = makeStringC("INSTSDS0"); 
  module_[subdcabs].rcsname = makeStringC("SUBDCABS"); 
  module_[sdclabs].rcsname = makeStringC("SDCLABS"); 
  module_[sdclsds].rcsname = makeStringC("SDCLSDS"); 
  module_[prlgsds].rcsname = makeStringC("PRLGSDS"); 
  module_[instsds1].rcsname = makeStringC("INSTSDS1"); 
  module_[dtgabs].rcsname = makeStringC("DTGABS"); 
  module_[rankabs].rcsname = makeStringC("RANKABS"); 
  module_[srabs].rcsname = makeStringC("SRABS"); 
  module_[srsds].rcsname = makeStringC("SRSDS"); 
  module_[linkabs].rcsname = makeStringC("LINKABS"); 
  module_[linksds].rcsname = makeStringC("LINKSDS"); 
  module_[subdcsds].rcsname = makeStringC("SUBDCSDS"); 
  module_[fpiabs].rcsname = makeStringC("FPIABS"); 

  module_[baseabs].appname = makeStringC("base abstract"); 
  module_[prlgabs0].appname = makeStringC("prolog abstract level 0"); 
  module_[prlgabs1].appname = makeStringC("prolog abstract level 1"); 
  module_[instabs].appname = makeStringC("instance abstract"); 
  module_[basesds0].appname = makeStringC("base SGML document string level 0"); 
  module_[basesds1].appname = makeStringC("base SGML document string level 1"); 
  module_[instsds0].appname = makeStringC("instance SGML document string"); 
  module_[subdcabs].appname = makeStringC("subdoc abstract"); 
  module_[sdclabs].appname = makeStringC("SGML declaration abstract"); 
  module_[sdclsds].appname = makeStringC("SGML declaration SGML document string"); 
  module_[prlgsds].appname = makeStringC("prolog SGML document string"); 
  module_[instsds1].appname = makeStringC("instance SGML document string level 1"); 
  module_[dtgabs].appname = makeStringC("datatag abstract"); 
  module_[rankabs].appname = makeStringC("rank abstract"); 
  module_[srabs].appname = makeStringC("shortref abstract"); 
  module_[srsds].appname = makeStringC("shortref SGML document string"); 
  module_[linkabs].appname = makeStringC("link abstract"); 
  module_[linksds].appname = makeStringC("link SGML document string"); 
  module_[subdcsds].appname = makeStringC("subdoc SGML document string"); 
  module_[fpiabs].appname = makeStringC("formal public identifier abstract"); 

  for (int i = 0; i < nModules; i++) 
    module_[i].supported = notSupported;
  module_[baseabs].supported = supported; 
  module_[prlgabs0].supported = supported; 
  module_[instabs].supported = supported; 
  module_[prlgabs1].supported = supported; 
  module_[basesds0].supported = supported;
  module_[instsds0].supported = supported;
  module_[subdcabs].supported = supported;

  for (int i = 0; i < nModules; i++) 
    module_[i].declared = 0;
  module_[baseabs].declared = 1;
  module_[prlgabs0].declared = 1;
  module_[instabs].declared = 1;
  if (!style()) 
    module_[prlgabs1].declared = 1; 
  if (!strictMode()) {
    module_[basesds0].declared = 1;
    module_[instsds0].declared = 1;
    module_[subdcabs].declared = 1;
    module_[prlgabs1].declared = 1;
  }
}

void Interpreter::explicitModules()
{
  if (!explicitModules_) {
    explicitModules_ = 1;
    for (int i = 0; i < nModules; i++)
      module_[i].declared = 0;

    module_[baseabs].declared = 1; 
    module_[prlgabs0].declared = 1; 
    module_[instabs].declared = 1; 
    if (!style()) 
      module_[prlgabs1].declared = 1; 
  }
}

void Interpreter::addModule(const StringC &name)
{
  for (int i = 0; i < nModules; i++)
    if (module_[i].rcsname == name) {
      module_[i].declared = 1;
      return;
    }   
  message(InterpreterMessages::unknownModule, StringMessageArg(name));
}

void Interpreter::checkGrovePlan()
{
  for (int i = 0; i < nModules; i++) 
    if (module_[i].supported != notSupported) {
      if (!module_[i].declared) 
        message(InterpreterMessages::cantOmitModule, 
                StringMessageArg(module_[i].appname));
    }
    else {
      if (module_[i].declared) 
        message(InterpreterMessages::cantAddModule, 
                StringMessageArg(module_[i].appname));
    }
}

TransformationMode *Interpreter::transformationMode()
{
  return &transformationMode_;
}

#ifdef DSSSL_NAMESPACE
}
#endif
