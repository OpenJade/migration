// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#include "stylelib.h"
#include "Interpreter.h"
#include "Insn.h"
#include "InterpreterMessages.h"
#include "LocNode.h"
#include "VM.h"
#include "macros.h"
#include <stdlib.h>

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

const Char defaultChar = 0xfffd;

static
size_t maxObjSize()
{
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
    sizeof(SetNonInheritedCsSosofoObj),
    sizeof(LabelSosofoObj),
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
			 const FOTBuilder::Extension *extensionTable)
: groveManager_(groveManager),
  messenger_(messenger),
  extensionTable_(extensionTable),
  Collector(maxObjSize()),
  partIndex_(1),  // 0 is for command-line definitions
  unitsPerInch_(unitsPerInch),
  nInheritedC_(0),
  initialProcessingMode_(StringC()),
  in_(0),
  lexCategory_(lexOther),
  currentPartFirstInitialValue_(0),
  initialStyle_(0),
  nextGlyphSubstTableUniqueId_(0),
  debugMode_(debugMode)
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
  {
    StringC tem(makeStringC("ISO/IEC 10036/RA//Glyphs"));
    afiiPublicId_ = storePublicId(tem.data(), tem.size(), Location());
  }
  installSyntacticKeys();
  installCValueSymbols();
  installPortNames();
  installPrimitives();
  installUnits();
  installCharNames();
  installFlowObjs();
  installInheritedCs();
  installSdata();
  installNodeProperties();
  defMode_ = &initialProcessingMode_;
  static const char *lexCategories[] = {
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz",
    "!$%&*/<=>?~_^:",
    "0123456789",
    "-+.",
    "();\"",
    " \t\r\n\f",
  };
  for (size_t i = 0; i < SIZEOF(lexCategories); i++)
    for (const char *s = lexCategories[i]; *s; s++)
      lexCategory_.setChar(*s, i);
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

void Interpreter::parse(Owner<InputSource> &in)
{
  in_.clear();
  in_.swap(in);
  bool recovering = 0;
  for (;;) {
    Token tok;
    if (!getToken(recovering ? ~0 : allowOpenParen|allowEndOfEntity,
		 tok))
      recovering = 1;
    else {
      if (tok == tokenEndOfEntity)
	break;
      if (tok != tokenOpenParen
	  || !getToken(recovering ? ~0 : unsigned(allowIdentifier), tok)
	  || tok != tokenIdentifier)
	recovering = 1;
      else {
	const Identifier *ident = lookup(currentToken_);
	Identifier::SyntacticKey key;
	if (!ident->syntacticKey(key)) {
	  if (!recovering)
	    message(InterpreterMessages::unknownTopLevelForm,
		    StringMessageArg(currentToken_));
	  recovering = 1;
	}
	else {
	  switch (key) {
	  case Identifier::keyDefine:
	    recovering = !doDefine();
	    break;
	  case Identifier::keyDefineUnit:
	    recovering = !doDefineUnit();
	    break;
	  case Identifier::keyDefault:
	    recovering = !doDefault();
	    break;
	  case Identifier::keyElement:
	    recovering = !doElement();
	    break;
	  case Identifier::keyRoot:
	    recovering = !doRoot();
	    break;
	  case Identifier::keyId:
	    recovering = !doId();
	    break;
	  case Identifier::keyMode:
	    recovering = !doMode();
	    break;
	  case Identifier::keyDeclareInitialValue:
	    recovering = !doDeclareInitialValue();
	    break;
	  case Identifier::keyDeclareCharacteristic:
	    recovering = !doDeclareCharacteristic();
	    break;
	  case Identifier::keyDeclareFlowObjectClass:
	    recovering = !doDeclareFlowObjectClass();
	    break;
	  case Identifier::keyDeclareCharCharacteristicAndProperty:
	  case Identifier::keyDeclareReferenceValueType:
	  case Identifier::keyDeclareDefaultLanguage:
	  case Identifier::keyDeclareCharProperty:
	  case Identifier::keyDefinePageModel:
	  case Identifier::keyDefineColumnSetModel:
	  case Identifier::keyDefineLanguage:
	  case Identifier::keyAddCharProperties:
	    recovering = !skipForm();
	    break;
	  default:
	    if (!recovering)
	      message(InterpreterMessages::unknownTopLevelForm,
		      StringMessageArg(currentToken_));
	    recovering = 1;
	    break;
	  }
	}
      }
    }
  }
#if 0
  NamedTableIter<Identifier> iter(identTable_);
  for (;;) {
    Identifier *ident = iter.next();
    if (!ident)
      break;
    Location loc;
    unsigned part;
    if (ident->defined(part, loc)) {
      ELObj *obj = ident->computeValue(1, *this);
      if (!isError(obj)) {
	*os_ << ident->name() << "=";
	obj->print(*this, *os_);
	*os_ << OutputCharStream::newline;
	os_->flush();
      }
    }
  }
#endif
  in_.clear();
}

bool Interpreter::doMode()
{
  Token tok;
  if (!getToken(allowIdentifier, tok))
    return 0;
  defMode_ = lookupProcessingMode(currentToken_);
  for (;;) {
    if (!getToken(allowOpenParen|allowCloseParen, tok))
      return 0;
    if (tok == tokenCloseParen)
      break;
    if (!getToken(allowIdentifier, tok))
      return 0;
    const Identifier *ident = lookup(currentToken_);
    Identifier::SyntacticKey key;
    if (!ident->syntacticKey(key)) {
      message(InterpreterMessages::badModeForm,
	      StringMessageArg(currentToken_));
      return 0;
    }
    else {
      switch (key) {
      case Identifier::keyDefault:
	if (!doDefault())
	  return 0;
	break;
      case Identifier::keyElement:
	if (!doElement())
	  return 0;
	break;
      case Identifier::keyRoot:
	if (!doRoot())
	  return 0;
	break;
      case Identifier::keyId:
	if (!doId())
	  return 0;
	break;
      default:
	message(InterpreterMessages::badModeForm,
		StringMessageArg(currentToken_));
	return 0;
      }
    }
  }
  defMode_ = &initialProcessingMode_;
  return 1;
}

bool Interpreter::doElement()
{
  Location loc(in_->currentLocation());
  Token tok;
  if (!getToken(allowOpenParen|allowString|allowIdentifier, tok))
    return 0;
  Vector<StringC> gis;
  if (tok == tokenOpenParen) {
    if (!getToken(allowString|allowIdentifier, tok))
      return 0;
    for (;;) {
      gis.push_back(currentToken_);
      if (!getToken(allowString|allowIdentifier|allowCloseParen, tok))
	return 0;
      if (tok == tokenCloseParen)
	break;
    }
  }
  else
    gis.push_back(currentToken_);
  Identifier::SyntacticKey key;
  Owner<Expression> expr;
  if (!parseExpression(0, expr, key, tok))
    return 0;
  if (!getToken(allowCloseParen, tok))
    return 0;
  defMode_->addElement(gis, expr, loc, *this);
  return 1;
}

bool Interpreter::doId()
{
  Location loc(in_->currentLocation());
  Token tok;
  if (!getToken(allowString|allowIdentifier, tok))
    return 0;
  StringC id(currentToken_);
  Identifier::SyntacticKey key;
  Owner<Expression> expr;
  if (!parseExpression(0, expr, key, tok))
    return 0;
  if (!getToken(allowCloseParen, tok))
    return 0;
  defMode_->addId(id, expr, loc, *this);
  return 1;
}

bool Interpreter::doDefault()
{
  Location loc(in_->currentLocation());
  Token tok;
  Identifier::SyntacticKey key;
  Owner<Expression> expr;
  if (!parseExpression(0, expr, key, tok))
    return 0;
  if (!getToken(allowCloseParen, tok))
    return 0;
  defMode_->addDefault(expr, loc, *this);
  return 1;
}

bool Interpreter::doRoot()
{
  Location loc(in_->currentLocation());
  Token tok;
  Identifier::SyntacticKey key;
  Owner<Expression> expr;
  if (!parseExpression(0, expr, key, tok))
    return 0;
  if (!getToken(allowCloseParen, tok))
    return 0;
  defMode_->addRoot(expr, loc, *this);
  return 1;
}

bool Interpreter::doDeclareInitialValue()
{
  Token tok;
  if (!getToken(allowIdentifier, tok))
    return 0;
  Identifier *ident = lookup(currentToken_);
  if (ident->inheritedC().isNull())
    message(InterpreterMessages::notABuiltinInheritedC,
	    StringMessageArg(ident->name()));
  Owner<Expression> expr;
  Identifier::SyntacticKey key;
  if (!parseExpression(0, expr, key, tok))
    return 0;
  if (!getToken(allowCloseParen, tok))
    return 0;
  if (ident->inheritedC().isNull())
    return 1;
  installInitialValue(ident, expr);
  return 1;
}

bool Interpreter::doDeclareCharacteristic()
{
  Location loc(in_->currentLocation());
  Token tok;
  if (!getToken(allowIdentifier, tok))
    return 0;
  Identifier *ident = lookup(currentToken_);
  if (!getToken(allowString, tok))
    return 0;
  StringC pubid(currentToken_);
  Owner<Expression> expr;
  Identifier::SyntacticKey key;
  if (!parseExpression(0, expr, key, tok))
    return 0;
  if (!getToken(allowCloseParen, tok))
    return 0;
  Location defLoc;
  unsigned defPart;
  if (ident->inheritedCDefined(defPart, defLoc)
      && defPart <= currentPartIndex()) {
    if (defPart == currentPartIndex()) {
      setNextLocation(loc);
      message(InterpreterMessages::duplicateCharacteristic,
	      StringMessageArg(ident->name()),
	      defLoc);
    }
  }
  else {
    installExtensionInheritedC(ident, pubid, loc);
    installInitialValue(ident, expr);
  }
  return 1;
}

bool Interpreter::doDeclareFlowObjectClass()
{
  Location loc(in_->currentLocation());
  Token tok;
  if (!getToken(allowIdentifier, tok))
    return 0;
  Identifier *ident = lookup(currentToken_);
  if (!getToken(allowString, tok))
    return 0;
  Location defLoc;
  unsigned defPart;
  if (ident->inheritedCDefined(defPart, defLoc)
      && defPart <= currentPartIndex()) {
    if (defPart == currentPartIndex()) {
      setNextLocation(loc);
      message(InterpreterMessages::duplicateFlowObjectClass,
	      StringMessageArg(ident->name()),
	      defLoc);
    }
  }
  else
    installExtensionFlowObjectClass(ident, currentToken_, loc);
  if (!getToken(allowCloseParen, tok))
    return 0;
  return 1;
}

void Interpreter::installInitialValue(Identifier *ident, Owner<Expression> &expr)
{
  for (size_t i = 0; i < initialValueNames_.size(); i++) {
    if (ident == initialValueNames_[i]) {
      if (i >= currentPartFirstInitialValue_)
	message(InterpreterMessages::duplicateInitialValue,
	        StringMessageArg(ident->name()),
		initialValueValues_[i]->location());
      return;
    }
  }
  initialValueValues_.resize(initialValueValues_.size() + 1);
  expr.swap(initialValueValues_.back());
  initialValueNames_.push_back(ident);
}

void Interpreter::defineVariable(const StringC &str)
{
  lookup(str)->setValue(makeTrue(), 0);
}

bool Interpreter::doDefine()
{
  Location loc(in_->currentLocation());
  Token tok;
  if (!getToken(allowOpenParen|allowIdentifier, tok))
    return 0;
  Vector<const Identifier *> formals;
  bool isProcedure;
  if (tok == tokenOpenParen) {
    if (!getToken(allowIdentifier, tok))
      return 0;
    isProcedure = 1;
  }
  else
    isProcedure = 0;
  Identifier *ident = lookup(currentToken_);
  Identifier::SyntacticKey key;
  if (ident->syntacticKey(key) && key <= Identifier::lastSyntacticKey)
    message(InterpreterMessages::syntacticKeywordAsVariable,
	    StringMessageArg(currentToken_));
  NCVector<Owner<Expression> > inits;
  int nOptional;
  int nKey;
  bool hasRest;
  if (isProcedure && !parseFormals(formals, inits, nOptional, hasRest, nKey))
    return 0;
  Owner<Expression> expr;
  if (!parseExpression(0, expr, key, tok))
    return 0;
  if (!getToken(allowCloseParen, tok))
    return 0;
  if (isProcedure)
    expr = new LambdaExpression(formals, inits, nOptional, hasRest, nKey,
				expr, loc);
  Location defLoc;
  unsigned defPart;
  if (ident->defined(defPart, defLoc) && defPart <= partIndex_) {
    if (defPart == partIndex_)
      message(InterpreterMessages::duplicateDefinition,
	      StringMessageArg(ident->name()),
	      defLoc);
  }
  else
    ident->setDefinition(expr, partIndex_, loc);
  return 1;
}

bool Interpreter::doDefineUnit()
{
  Location loc(in_->currentLocation());
  Token tok;
  if (!getToken(allowIdentifier, tok))
    return 0;
  Unit *unit = lookupUnit(currentToken_);
  Owner<Expression> expr;
  Identifier::SyntacticKey key;
  if (!parseExpression(0, expr, key, tok))
    return 0;
  if (!getToken(allowCloseParen, tok))
    return 0;
  Location defLoc;
  unsigned defPart;
  if (unit->defined(defPart, defLoc) && defPart <= partIndex_) {
    if (defPart == partIndex_)
      message(InterpreterMessages::duplicateUnitDefinition,
	      StringMessageArg(unit->name()),
	      defLoc);
  }
  else
    unit->setDefinition(expr, partIndex_, loc);
  return 1;
}

bool Interpreter::skipForm()
{
  static const unsigned allow = (~0 & ~allowEndOfEntity);
  unsigned level = 0;
  for (;;) {
    Token tok;
    if (!getToken(allow, tok))
      break;
    switch (tok) {
    case tokenOpenParen:
      level++;
      break;
    case tokenCloseParen:
      if (level == 0)
	return 1;
      level--;
      break;
    default:
      break;
    }
  }
  return 0;
}

bool Interpreter::parseExpression(unsigned allowed,
				  Owner<Expression> &expr,
				  Identifier::SyntacticKey &key,
				  Token &tok)
{
  expr.clear();
  key = Identifier::notKey;
  ELObj *obj;
  if (!parseSelfEvaluating(allowed, obj, tok))
    return 0;
  if (obj) {
    makePermanent(obj);
    expr = new ConstantExpression(obj, in_->currentLocation());
    return 1;
  }
  switch (tok) {
  case tokenQuote:
    {
      Location loc;
      if (!parseDatum(0, obj, loc, tok))
	return 0;
      makePermanent(obj);
      expr = new ConstantExpression(obj, loc);
      break;
    }
  case tokenQuasiquote:
    {
      bool spliced;
      return parseQuasiquoteTemplate(0, 0, expr, key, tok, spliced);
    }
  case tokenOpenParen:
    {
      Location loc(in_->currentLocation());
      if (!parseExpression(allowExpressionKey, expr, key, tok))
	return 0;
      if (expr) {
	NCVector<Owner<Expression> > args;
	for (;;) {
	  args.resize(args.size() + 1);
	  if (!parseExpression(allowCloseParen, args.back(), key, tok))
	    return 0;
	  if (!args.back()) {
	    args.resize(args.size() - 1);
	    break;
	  }
	}
	expr = new CallExpression(expr, args, loc);
      }
      else {
	switch (key) {
	case Identifier::keyQuote:
          return parseQuote(expr);
	case Identifier::keyLambda:
	  return parseLambda(expr);
	case Identifier::keyIf:
	  return parseIf(expr);
	case Identifier::keyCond:
	  return parseCond(expr);
	case Identifier::keyAnd:
	  return parseAnd(expr);
	case Identifier::keyOr:
	  return parseOr(expr);
	case Identifier::keyCase:
	  return parseCase(expr);
	case Identifier::keyLet:
	  return parseLet(expr);
	case Identifier::keyLetStar:
	  return parseLetStar(expr);
	case Identifier::keyLetrec:
	  return parseLetrec(expr);
	case Identifier::keyMake:
	  return parseMake(expr);
	case Identifier::keyStyle:
	  return parseStyle(expr);
	case Identifier::keyWithMode:
	  return parseWithMode(expr);
	case Identifier::keyQuasiquote:
	  return parseQuasiquote(expr);
	default:
	  CANNOT_HAPPEN();
	}
      }
      break;
    }
  case tokenIdentifier:
    {
      const Identifier *ident = lookup(currentToken_);
      if (ident->syntacticKey(key) && key <= Identifier::lastSyntacticKey) {
	switch (key) {
	case Identifier::keyDefine:
	  if (allowed & allowKeyDefine)
	    return 1;
	  break;
	case Identifier::keyArrow:
	  if (allowed & allowKeyArrow)
	    return 1;
	  break;
	case Identifier::keyElse:
	  if (allowed & allowKeyElse)
	    return 1;
	  break;
	case Identifier::keyUnquote:
	case Identifier::keyUnquoteSplicing:
	  break;
	default:
	  if (allowed & allowExpressionKey)
	    return 1;
	  break;
	}
	message(InterpreterMessages::syntacticKeywordAsVariable,
		StringMessageArg(currentToken_));
      }
      expr = new VariableExpression(ident, in_->currentLocation());
    }
    break;
  default:
    break;
  }
  return 1;
}

void Interpreter::installUnits()
{
  static struct {
    const char *name;
    int numer;
    int denom;
  } units[] = {
    { "m", 5000, 127 },
    { "cm", 50, 127 },
    { "mm", 5, 127 },
    { "in", 1, 1 },
    { "pt", 1, 72 },
    { "pica", 1, 6 }
  };
  for (size_t i = 0; i < SIZEOF(units); i++) {
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
    { "make", Identifier::keyMake },
    { "style", Identifier::keyStyle },
    { "with-mode", Identifier::keyWithMode },
    { "define-unit", Identifier::keyDefineUnit },
    { "element", Identifier::keyElement },
    { "default", Identifier::keyDefault },
    { "root", Identifier::keyRoot },
    { "id", Identifier::keyId },
    { "mode", Identifier::keyMode },
    { "declare-initial-value", Identifier::keyDeclareInitialValue },
    { "declare-characteristic", Identifier::keyDeclareCharacteristic },
    { "declare-flow-object-class", Identifier::keyDeclareFlowObjectClass },
    { "declare-char-characteristic+property", Identifier::keyDeclareCharCharacteristicAndProperty },
    { "declare-reference-value-type", Identifier::keyDeclareReferenceValueType },
    { "declare-default-language", Identifier::keyDeclareDefaultLanguage },
    { "declare-char-property", Identifier::keyDeclareCharProperty },
    { "define-page-model", Identifier::keyDefinePageModel },
    { "define-column-set-model", Identifier::keyDefineColumnSetModel },
    { "define-language", Identifier::keyDefineLanguage },
    { "add-char-properties", Identifier::keyAddCharProperties },
    { "use", Identifier::keyUse },
    { "label", Identifier::keyLabel },
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
    { "null", Identifier::keyNull },
    { "rcs?", Identifier::keyIsRcs },
    { "parent", Identifier::keyParent },
    { "active", Identifier::keyActive },
  };
  for (size_t i = 0; i < SIZEOF(keys); i++)
    lookup(makeStringC(keys[i].name))->setSyntacticKey(keys[i].key);
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
  static struct {
    Char c;
    const char *name;
  } chars[] = {
    { 0x0020, "space" },
    { 0x0021, "exclamation-mark" },
    { 0x0022, "quotation-mark" },
    { 0x0023, "number-sign" },
    { 0x0024, "dollar-sign" },
    { 0x0025, "percent-sign" },
    { 0x0026, "ampersand" },
    { 0x0027, "apostrophe" },
    { 0x0028, "left-parenthesis" },
    { 0x0029, "right-parenthesis" },
    { 0x002a, "asterisk" },
    { 0x002b, "plus-sign" },
    { 0x002c, "comma" },
    { 0x002d, "hyphen-minus" },
    { 0x002e, "full-stop" },
    { 0x002f, "solidus" },
    { 0x0030, "digit-zero" },
    { 0x0031, "digit-one" },
    { 0x0032, "digit-two" },
    { 0x0033, "digit-three" },
    { 0x0034, "digit-four" },
    { 0x0035, "digit-five" },
    { 0x0036, "digit-six" },
    { 0x0037, "digit-seven" },
    { 0x0038, "digit-eight" },
    { 0x0039, "digit-nine" },
    { 0x003a, "colon" },
    { 0x003b, "semicolon" },
    { 0x003c, "less-than-sign" },
    { 0x003d, "equals-sign" },
    { 0x003e, "greater-than-sign" },
    { 0x003f, "question-mark" },
    { 0x0040, "commercial-at" },
    { 0x0041, "latin-capital-letter-a" },
    { 0x0042, "latin-capital-letter-b" },
    { 0x0043, "latin-capital-letter-c" },
    { 0x0044, "latin-capital-letter-d" },
    { 0x0045, "latin-capital-letter-e" },
    { 0x0046, "latin-capital-letter-f" },
    { 0x0047, "latin-capital-letter-g" },
    { 0x0048, "latin-capital-letter-h" },
    { 0x0049, "latin-capital-letter-i" },
    { 0x004a, "latin-capital-letter-j" },
    { 0x004b, "latin-capital-letter-k" },
    { 0x004c, "latin-capital-letter-l" },
    { 0x004d, "latin-capital-letter-m" },
    { 0x004e, "latin-capital-letter-n" },
    { 0x004f, "latin-capital-letter-o" },
    { 0x0050, "latin-capital-letter-p" },
    { 0x0051, "latin-capital-letter-q" },
    { 0x0052, "latin-capital-letter-r" },
    { 0x0053, "latin-capital-letter-s" },
    { 0x0054, "latin-capital-letter-t" },
    { 0x0055, "latin-capital-letter-u" },
    { 0x0056, "latin-capital-letter-v" },
    { 0x0057, "latin-capital-letter-w" },
    { 0x0058, "latin-capital-letter-x" },
    { 0x0059, "latin-capital-letter-y" },
    { 0x005a, "latin-capital-letter-z" },
    { 0x005b, "left-square-bracket" },
    { 0x005c, "reverse-solidus" },
    { 0x005d, "right-square-bracket" },
    { 0x005e, "circumflex-accent" },
    { 0x005f, "low-line" },
    { 0x0060, "grave-accent" },
    { 0x0061, "latin-small-letter-a" },
    { 0x0062, "latin-small-letter-b" },
    { 0x0063, "latin-small-letter-c" },
    { 0x0064, "latin-small-letter-d" },
    { 0x0065, "latin-small-letter-e" },
    { 0x0066, "latin-small-letter-f" },
    { 0x0067, "latin-small-letter-g" },
    { 0x0068, "latin-small-letter-h" },
    { 0x0069, "latin-small-letter-i" },
    { 0x006a, "latin-small-letter-j" },
    { 0x006b, "latin-small-letter-k" },
    { 0x006c, "latin-small-letter-l" },
    { 0x006d, "latin-small-letter-m" },
    { 0x006e, "latin-small-letter-n" },
    { 0x006f, "latin-small-letter-o" },
    { 0x0070, "latin-small-letter-p" },
    { 0x0071, "latin-small-letter-q" },
    { 0x0072, "latin-small-letter-r" },
    { 0x0073, "latin-small-letter-s" },
    { 0x0074, "latin-small-letter-t" },
    { 0x0075, "latin-small-letter-u" },
    { 0x0076, "latin-small-letter-v" },
    { 0x0077, "latin-small-letter-w" },
    { 0x0078, "latin-small-letter-x" },
    { 0x0079, "latin-small-letter-y" },
    { 0x007a, "latin-small-letter-z" },
    { 0x007b, "left-curly-bracket" },
    { 0x007c, "vertical-line" },
    { 0x007d, "right-curly-bracket" },
    { 0x007e, "tilde" },
    { 0x00a0, "no-break-space" },
    { 0x00a1, "inverted-exclamation-mark" },
    { 0x00a2, "cent-sign" },
    { 0x00a3, "pound-sign" },
    { 0x00a4, "currency-sign" },
    { 0x00a5, "yen-sign" },
    { 0x00a6, "broken-bar" },
    { 0x00a7, "section-sign" },
    { 0x00a8, "diaeresis" },
    { 0x00a9, "copyright-sign" },
    { 0x00aa, "feminine-ordinal-indicator" },
    { 0x00ab, "left-pointing-double-angle-quotation-mark" },
    { 0x00ac, "not-sign" },
    { 0x00ad, "soft-hyphen" },
    { 0x00ae, "registered-sign" },
    { 0x00af, "macron" },
    { 0x00b0, "degree-sign" },
    { 0x00b1, "plus-minus-sign" },
    { 0x00b2, "superscript-two" },
    { 0x00b3, "superscript-three" },
    { 0x00b4, "acute-accent" },
    { 0x00b5, "micro-sign" },
    { 0x00b6, "pilcrow-sign" },
    { 0x00b7, "middle-dot" },
    { 0x00b8, "cedilla" },
    { 0x00b9, "superscript-one" },
    { 0x00ba, "masculine-ordinal-indicator" },
    { 0x00bb, "right-pointing-double-angle-quotation-mark" },
    { 0x00bc, "vulgar-fraction-one-quarter" },
    { 0x00bd, "vulgar-fraction-one-half" },
    { 0x00be, "vulgar-fraction-three-quarters" },
    { 0x00bf, "inverted-question-mark" },
    { 0x00c0, "latin-capital-letter-a-with-grave" },
    { 0x00c1, "latin-capital-letter-a-with-acute" },
    { 0x00c2, "latin-capital-letter-a-with-circumflex" },
    { 0x00c3, "latin-capital-letter-a-with-tilde" },
    { 0x00c4, "latin-capital-letter-a-with-diaeresis" },
    { 0x00c5, "latin-capital-letter-a-with-ring-above" },
    { 0x00c6, "latin-capital-letter-ae" },
    { 0x00c7, "latin-capital-letter-c-with-cedilla" },
    { 0x00c8, "latin-capital-letter-e-with-grave" },
    { 0x00c9, "latin-capital-letter-e-with-acute" },
    { 0x00ca, "latin-capital-letter-e-with-circumflex" },
    { 0x00cb, "latin-capital-letter-e-with-diaeresis" },
    { 0x00cc, "latin-capital-letter-i-with-grave" },
    { 0x00cd, "latin-capital-letter-i-with-acute" },
    { 0x00ce, "latin-capital-letter-i-with-circumflex" },
    { 0x00cf, "latin-capital-letter-i-with-diaeresis" },
    { 0x00d0, "latin-capital-letter-eth" },
    { 0x00d1, "latin-capital-letter-n-with-tilde" },
    { 0x00d2, "latin-capital-letter-o-with-grave" },
    { 0x00d3, "latin-capital-letter-o-with-acute" },
    { 0x00d4, "latin-capital-letter-o-with-circumflex" },
    { 0x00d5, "latin-capital-letter-o-with-tilde" },
    { 0x00d6, "latin-capital-letter-o-with-diaeresis" },
    { 0x00d7, "multiplication-sign" },
    { 0x00d8, "latin-capital-letter-o-with-stroke" },
    { 0x00d9, "latin-capital-letter-u-with-grave" },
    { 0x00da, "latin-capital-letter-u-with-acute" },
    { 0x00db, "latin-capital-letter-u-with-circumflex" },
    { 0x00dc, "latin-capital-letter-u-with-diaeresis" },
    { 0x00dd, "latin-capital-letter-y-with-acute" },
    { 0x00de, "latin-capital-letter-thorn" },
    { 0x00df, "latin-small-letter-sharp-s" },
    { 0x00e0, "latin-small-letter-a-with-grave" },
    { 0x00e1, "latin-small-letter-a-with-acute" },
    { 0x00e2, "latin-small-letter-a-with-circumflex" },
    { 0x00e3, "latin-small-letter-a-with-tilde" },
    { 0x00e4, "latin-small-letter-a-with-diaeresis" },
    { 0x00e5, "latin-small-letter-a-with-ring-above" },
    { 0x00e6, "latin-small-letter-ae" },
    { 0x00e7, "latin-small-letter-c-with-cedilla" },
    { 0x00e8, "latin-small-letter-e-with-grave" },
    { 0x00e9, "latin-small-letter-e-with-acute" },
    { 0x00ea, "latin-small-letter-e-with-circumflex" },
    { 0x00eb, "latin-small-letter-e-with-diaeresis" },
    { 0x00ec, "latin-small-letter-i-with-grave" },
    { 0x00ed, "latin-small-letter-i-with-acute" },
    { 0x00ee, "latin-small-letter-i-with-circumflex" },
    { 0x00ef, "latin-small-letter-i-with-diaeresis" },
    { 0x00f0, "latin-small-letter-eth" },
    { 0x00f1, "latin-small-letter-n-with-tilde" },
    { 0x00f2, "latin-small-letter-o-with-grave" },
    { 0x00f3, "latin-small-letter-o-with-acute" },
    { 0x00f4, "latin-small-letter-o-with-circumflex" },
    { 0x00f5, "latin-small-letter-o-with-tilde" },
    { 0x00f6, "latin-small-letter-o-with-diaeresis" },
    { 0x00f7, "division-sign" },
    { 0x00f8, "latin-small-letter-o-with-stroke" },
    { 0x00f9, "latin-small-letter-u-with-grave" },
    { 0x00fa, "latin-small-letter-u-with-acute" },
    { 0x00fb, "latin-small-letter-u-with-circumflex" },
    { 0x00fc, "latin-small-letter-u-with-diaeresis" },
    { 0x00fd, "latin-small-letter-y-with-acute" },
    { 0x00fe, "latin-small-letter-thorn" },
    { 0x00ff, "latin-small-letter-y-with-diaeresis" },
    { 0x0100, "latin-capital-letter-a-with-macron" },
    { 0x0101, "latin-small-letter-a-with-macron" },
    { 0x0102, "latin-capital-letter-a-with-breve" },
    { 0x0103, "latin-small-letter-a-with-breve" },
    { 0x0104, "latin-capital-letter-a-with-ogonek" },
    { 0x0105, "latin-small-letter-a-with-ogonek" },
    { 0x0106, "latin-capital-letter-c-with-acute" },
    { 0x0107, "latin-small-letter-c-with-acute" },
    { 0x0108, "latin-capital-letter-c-with-circumflex" },
    { 0x0109, "latin-small-letter-c-with-circumflex" },
    { 0x010a, "latin-capital-letter-c-with-dot-above" },
    { 0x010b, "latin-small-letter-c-with-dot-above" },
    { 0x010c, "latin-capital-letter-c-with-caron" },
    { 0x010d, "latin-small-letter-c-with-caron" },
    { 0x010e, "latin-capital-letter-d-with-caron" },
    { 0x010f, "latin-small-letter-d-with-caron" },
    { 0x0110, "latin-capital-letter-d-with-stroke" },
    { 0x0111, "latin-small-letter-d-with-stroke" },
    { 0x0112, "latin-capital-letter-e-with-macron" },
    { 0x0113, "latin-small-letter-e-with-macron" },
    { 0x0114, "latin-capital-letter-e-with-breve" },
    { 0x0115, "latin-small-letter-e-with-breve" },
    { 0x0116, "latin-capital-letter-e-with-dot-above" },
    { 0x0117, "latin-small-letter-e-with-dot-above" },
    { 0x0118, "latin-capital-letter-e-with-ogonek" },
    { 0x0119, "latin-small-letter-e-with-ogonek" },
    { 0x011a, "latin-capital-letter-e-with-caron" },
    { 0x011b, "latin-small-letter-e-with-caron" },
    { 0x011c, "latin-capital-letter-g-with-circumflex" },
    { 0x011d, "latin-small-letter-g-with-circumflex" },
    { 0x011e, "latin-capital-letter-g-with-breve" },
    { 0x011f, "latin-small-letter-g-with-breve" },
    { 0x0120, "latin-capital-letter-g-with-dot-above" },
    { 0x0121, "latin-small-letter-g-with-dot-above" },
    { 0x0122, "latin-capital-letter-g-with-cedilla" },
    { 0x0123, "latin-small-letter-g-with-cedilla" },
    { 0x0124, "latin-capital-letter-h-with-circumflex" },
    { 0x0125, "latin-small-letter-h-with-circumflex" },
    { 0x0126, "latin-capital-letter-h-with-stroke" },
    { 0x0127, "latin-small-letter-h-with-stroke" },
    { 0x0128, "latin-capital-letter-i-with-tilde" },
    { 0x0129, "latin-small-letter-i-with-tilde" },
    { 0x012a, "latin-capital-letter-i-with-macron" },
    { 0x012b, "latin-small-letter-i-with-macron" },
    { 0x012c, "latin-capital-letter-i-with-breve" },
    { 0x012d, "latin-small-letter-i-with-breve" },
    { 0x012e, "latin-capital-letter-i-with-ogonek" },
    { 0x012f, "latin-small-letter-i-with-ogonek" },
    { 0x0130, "latin-capital-letter-i-with-dot-above" },
    { 0x0131, "latin-small-letter-dotless-i" },
    { 0x0132, "latin-capital-ligature-ij" },
    { 0x0133, "latin-small-ligature-ij" },
    { 0x0134, "latin-capital-letter-j-with-circumflex" },
    { 0x0135, "latin-small-letter-j-with-circumflex" },
    { 0x0136, "latin-capital-letter-k-with-cedilla" },
    { 0x0137, "latin-small-letter-k-with-cedilla" },
    { 0x0138, "latin-small-letter-kra" },
    { 0x0139, "latin-capital-letter-l-with-acute" },
    { 0x013a, "latin-small-letter-l-with-acute" },
    { 0x013b, "latin-capital-letter-l-with-cedilla" },
    { 0x013c, "latin-small-letter-l-with-cedilla" },
    { 0x013d, "latin-capital-letter-l-with-caron" },
    { 0x013e, "latin-small-letter-l-with-caron" },
    { 0x013f, "latin-capital-letter-l-with-middle-dot" },
    { 0x0140, "latin-small-letter-l-with-middle-dot" },
    { 0x0141, "latin-capital-letter-l-with-stroke" },
    { 0x0142, "latin-small-letter-l-with-stroke" },
    { 0x0143, "latin-capital-letter-n-with-acute" },
    { 0x0144, "latin-small-letter-n-with-acute" },
    { 0x0145, "latin-capital-letter-n-with-cedilla" },
    { 0x0146, "latin-small-letter-n-with-cedilla" },
    { 0x0147, "latin-capital-letter-n-with-caron" },
    { 0x0148, "latin-small-letter-n-with-caron" },
    { 0x0149, "latin-small-letter-n-preceded-by-apostrophe" },
    { 0x014a, "latin-capital-letter-eng" },
    { 0x014b, "latin-small-letter-eng" },
    { 0x014c, "latin-capital-letter-o-with-macron" },
    { 0x014d, "latin-small-letter-o-with-macron" },
    { 0x014e, "latin-capital-letter-o-with-breve" },
    { 0x014f, "latin-small-letter-o-with-breve" },
    { 0x0150, "latin-capital-letter-o-with-double-acute" },
    { 0x0151, "latin-small-letter-o-with-double-acute" },
    { 0x0152, "latin-capital-ligature-oe" },
    { 0x0153, "latin-small-ligature-oe" },
    { 0x0154, "latin-capital-letter-r-with-acute" },
    { 0x0155, "latin-small-letter-r-with-acute" },
    { 0x0156, "latin-capital-letter-r-with-cedilla" },
    { 0x0157, "latin-small-letter-r-with-cedilla" },
    { 0x0158, "latin-capital-letter-r-with-caron" },
    { 0x0159, "latin-small-letter-r-with-caron" },
    { 0x015a, "latin-capital-letter-s-with-acute" },
    { 0x015b, "latin-small-letter-s-with-acute" },
    { 0x015c, "latin-capital-letter-s-with-circumflex" },
    { 0x015d, "latin-small-letter-s-with-circumflex" },
    { 0x015e, "latin-capital-letter-s-with-cedilla" },
    { 0x015f, "latin-small-letter-s-with-cedilla" },
    { 0x0160, "latin-capital-letter-s-with-caron" },
    { 0x0161, "latin-small-letter-s-with-caron" },
    { 0x0162, "latin-capital-letter-t-with-cedilla" },
    { 0x0163, "latin-small-letter-t-with-cedilla" },
    { 0x0164, "latin-capital-letter-t-with-caron" },
    { 0x0165, "latin-small-letter-t-with-caron" },
    { 0x0166, "latin-capital-letter-t-with-stroke" },
    { 0x0167, "latin-small-letter-t-with-stroke" },
    { 0x0168, "latin-capital-letter-u-with-tilde" },
    { 0x0169, "latin-small-letter-u-with-tilde" },
    { 0x016a, "latin-capital-letter-u-with-macron" },
    { 0x016b, "latin-small-letter-u-with-macron" },
    { 0x016c, "latin-capital-letter-u-with-breve" },
    { 0x016d, "latin-small-letter-u-with-breve" },
    { 0x016e, "latin-capital-letter-u-with-ring-above" },
    { 0x016f, "latin-small-letter-u-with-ring-above" },
    { 0x0170, "latin-capital-letter-u-with-double-acute" },
    { 0x0171, "latin-small-letter-u-with-double-acute" },
    { 0x0172, "latin-capital-letter-u-with-ogonek" },
    { 0x0173, "latin-small-letter-u-with-ogonek" },
    { 0x0174, "latin-capital-letter-w-with-circumflex" },
    { 0x0175, "latin-small-letter-w-with-circumflex" },
    { 0x0176, "latin-capital-letter-y-with-circumflex" },
    { 0x0177, "latin-small-letter-y-with-circumflex" },
    { 0x0178, "latin-capital-letter-y-with-diaeresis" },
    { 0x0179, "latin-capital-letter-z-with-acute" },
    { 0x017a, "latin-small-letter-z-with-acute" },
    { 0x017b, "latin-capital-letter-z-with-dot-above" },
    { 0x017c, "latin-small-letter-z-with-dot-above" },
    { 0x017d, "latin-capital-letter-z-with-caron" },
    { 0x017e, "latin-small-letter-z-with-caron" },
    { 0x017f, "latin-small-letter-long-s" },
    { 0x0192, "latin-small-letter-f-with-hook" },
    { 0x01fa, "latin-capital-letter-a-with-ring-above-and-acute" },
    { 0x01fb, "latin-small-letter-a-with-ring-above-and-acute" },
    { 0x01fc, "latin-capital-letter-ae-with-acute" },
    { 0x01fd, "latin-small-letter-ae-with-acute" },
    { 0x01fe, "latin-capital-letter-o-with-stroke-and-acute" },
    { 0x01ff, "latin-small-letter-o-with-stroke-and-acute" },
    { 0x02c6, "modifier-letter-circumflex-accent" },
    { 0x02c7, "caron" },
    { 0x02c9, "modifier-letter-macron" },
    { 0x02d8, "breve" },
    { 0x02d9, "dot-above" },
    { 0x02da, "ring-above" },
    { 0x02db, "ogonek" },
    { 0x02dc, "small-tilde" },
    { 0x02dd, "double-acute-accent" },
    { 0x0384, "greek-tonos" },
    { 0x0385, "greek-dialytika-tonos" },
    { 0x0386, "greek-capital-letter-alpha-with-tonos" },
    { 0x0387, "greek-ano-teleia" },
    { 0x0388, "greek-capital-letter-epsilon-with-tonos" },
    { 0x0389, "greek-capital-letter-eta-with-tonos" },
    { 0x038a, "greek-capital-letter-iota-with-tonos" },
    { 0x038c, "greek-capital-letter-omicron-with-tonos" },
    { 0x038e, "greek-capital-letter-upsilon-with-tonos" },
    { 0x038f, "greek-capital-letter-omega-with-tonos" },
    { 0x0390, "greek-small-letter-iota-with-dialytika-and-tonos" },
    { 0x0391, "greek-capital-letter-alpha" },
    { 0x0392, "greek-capital-letter-beta" },
    { 0x0393, "greek-capital-letter-gamma" },
    { 0x0394, "greek-capital-letter-delta" },
    { 0x0395, "greek-capital-letter-epsilon" },
    { 0x0396, "greek-capital-letter-zeta" },
    { 0x0397, "greek-capital-letter-eta" },
    { 0x0398, "greek-capital-letter-theta" },
    { 0x0399, "greek-capital-letter-iota" },
    { 0x039a, "greek-capital-letter-kappa" },
    { 0x039b, "greek-capital-letter-lamda" },
    { 0x039c, "greek-capital-letter-mu" },
    { 0x039d, "greek-capital-letter-nu" },
    { 0x039e, "greek-capital-letter-xi" },
    { 0x039f, "greek-capital-letter-omicron" },
    { 0x03a0, "greek-capital-letter-pi" },
    { 0x03a1, "greek-capital-letter-rho" },
    { 0x03a3, "greek-capital-letter-sigma" },
    { 0x03a4, "greek-capital-letter-tau" },
    { 0x03a5, "greek-capital-letter-upsilon" },
    { 0x03a6, "greek-capital-letter-phi" },
    { 0x03a7, "greek-capital-letter-chi" },
    { 0x03a8, "greek-capital-letter-psi" },
    { 0x03a9, "greek-capital-letter-omega" },
    { 0x03aa, "greek-capital-letter-iota-with-dialytika" },
    { 0x03ab, "greek-capital-letter-upsilon-with-dialytika" },
    { 0x03ac, "greek-small-letter-alpha-with-tonos" },
    { 0x03ad, "greek-small-letter-epsilon-with-tonos" },
    { 0x03ae, "greek-small-letter-eta-with-tonos" },
    { 0x03af, "greek-small-letter-iota-with-tonos" },
    { 0x03b0, "greek-small-letter-upsilon-with-dialytika-and-tonos" },
    { 0x03b1, "greek-small-letter-alpha" },
    { 0x03b2, "greek-small-letter-beta" },
    { 0x03b3, "greek-small-letter-gamma" },
    { 0x03b4, "greek-small-letter-delta" },
    { 0x03b5, "greek-small-letter-epsilon" },
    { 0x03b6, "greek-small-letter-zeta" },
    { 0x03b7, "greek-small-letter-eta" },
    { 0x03b8, "greek-small-letter-theta" },
    { 0x03b9, "greek-small-letter-iota" },
    { 0x03ba, "greek-small-letter-kappa" },
    { 0x03bb, "greek-small-letter-lamda" },
    { 0x03bc, "greek-small-letter-mu" },
    { 0x03bd, "greek-small-letter-nu" },
    { 0x03be, "greek-small-letter-xi" },
    { 0x03bf, "greek-small-letter-omicron" },
    { 0x03c0, "greek-small-letter-pi" },
    { 0x03c1, "greek-small-letter-rho" },
    { 0x03c2, "greek-small-letter-final-sigma" },
    { 0x03c3, "greek-small-letter-sigma" },
    { 0x03c4, "greek-small-letter-tau" },
    { 0x03c5, "greek-small-letter-upsilon" },
    { 0x03c6, "greek-small-letter-phi" },
    { 0x03c7, "greek-small-letter-chi" },
    { 0x03c8, "greek-small-letter-psi" },
    { 0x03c9, "greek-small-letter-omega" },
    { 0x03ca, "greek-small-letter-iota-with-dialytika" },
    { 0x03cb, "greek-small-letter-upsilon-with-dialytika" },
    { 0x03cc, "greek-small-letter-omicron-with-tonos" },
    { 0x03cd, "greek-small-letter-upsilon-with-tonos" },
    { 0x03ce, "greek-small-letter-omega-with-tonos" },
    { 0x03d1, "greek-theta-symbol" },
    { 0x03d2, "greek-upsilon-with-hook-symbol" },
    { 0x03d5, "greek-phi-symbol" },
    { 0x03d6, "greek-pi-symbol" },
    { 0x0401, "cyrillic-capital-letter-io" },
    { 0x0402, "cyrillic-capital-letter-dje" },
    { 0x0403, "cyrillic-capital-letter-gje" },
    { 0x0404, "cyrillic-capital-letter-ukrainian-ie" },
    { 0x0405, "cyrillic-capital-letter-dze" },
    { 0x0406, "cyrillic-capital-letter-byelorussian-ukrainian-i" },
    { 0x0407, "cyrillic-capital-letter-yi" },
    { 0x0408, "cyrillic-capital-letter-je" },
    { 0x0409, "cyrillic-capital-letter-lje" },
    { 0x040a, "cyrillic-capital-letter-nje" },
    { 0x040b, "cyrillic-capital-letter-tshe" },
    { 0x040c, "cyrillic-capital-letter-kje" },
    { 0x040e, "cyrillic-capital-letter-short-u" },
    { 0x040f, "cyrillic-capital-letter-dzhe" },
    { 0x0410, "cyrillic-capital-letter-a" },
    { 0x0411, "cyrillic-capital-letter-be" },
    { 0x0412, "cyrillic-capital-letter-ve" },
    { 0x0413, "cyrillic-capital-letter-ghe" },
    { 0x0414, "cyrillic-capital-letter-de" },
    { 0x0415, "cyrillic-capital-letter-ie" },
    { 0x0416, "cyrillic-capital-letter-zhe" },
    { 0x0417, "cyrillic-capital-letter-ze" },
    { 0x0418, "cyrillic-capital-letter-i" },
    { 0x0419, "cyrillic-capital-letter-short-i" },
    { 0x041a, "cyrillic-capital-letter-ka" },
    { 0x041b, "cyrillic-capital-letter-el" },
    { 0x041c, "cyrillic-capital-letter-em" },
    { 0x041d, "cyrillic-capital-letter-en" },
    { 0x041e, "cyrillic-capital-letter-o" },
    { 0x041f, "cyrillic-capital-letter-pe" },
    { 0x0420, "cyrillic-capital-letter-er" },
    { 0x0421, "cyrillic-capital-letter-es" },
    { 0x0422, "cyrillic-capital-letter-te" },
    { 0x0423, "cyrillic-capital-letter-u" },
    { 0x0424, "cyrillic-capital-letter-ef" },
    { 0x0425, "cyrillic-capital-letter-ha" },
    { 0x0426, "cyrillic-capital-letter-tse" },
    { 0x0427, "cyrillic-capital-letter-che" },
    { 0x0428, "cyrillic-capital-letter-sha" },
    { 0x0429, "cyrillic-capital-letter-shcha" },
    { 0x042a, "cyrillic-capital-letter-hard-sign" },
    { 0x042b, "cyrillic-capital-letter-yeru" },
    { 0x042c, "cyrillic-capital-letter-soft-sign" },
    { 0x042d, "cyrillic-capital-letter-e" },
    { 0x042e, "cyrillic-capital-letter-yu" },
    { 0x042f, "cyrillic-capital-letter-ya" },
    { 0x0430, "cyrillic-small-letter-a" },
    { 0x0431, "cyrillic-small-letter-be" },
    { 0x0432, "cyrillic-small-letter-ve" },
    { 0x0433, "cyrillic-small-letter-ghe" },
    { 0x0434, "cyrillic-small-letter-de" },
    { 0x0435, "cyrillic-small-letter-ie" },
    { 0x0436, "cyrillic-small-letter-zhe" },
    { 0x0437, "cyrillic-small-letter-ze" },
    { 0x0438, "cyrillic-small-letter-i" },
    { 0x0439, "cyrillic-small-letter-short-i" },
    { 0x043a, "cyrillic-small-letter-ka" },
    { 0x043b, "cyrillic-small-letter-el" },
    { 0x043c, "cyrillic-small-letter-em" },
    { 0x043d, "cyrillic-small-letter-en" },
    { 0x043e, "cyrillic-small-letter-o" },
    { 0x043f, "cyrillic-small-letter-pe" },
    { 0x0440, "cyrillic-small-letter-er" },
    { 0x0441, "cyrillic-small-letter-es" },
    { 0x0442, "cyrillic-small-letter-te" },
    { 0x0443, "cyrillic-small-letter-u" },
    { 0x0444, "cyrillic-small-letter-ef" },
    { 0x0445, "cyrillic-small-letter-ha" },
    { 0x0446, "cyrillic-small-letter-tse" },
    { 0x0447, "cyrillic-small-letter-che" },
    { 0x0448, "cyrillic-small-letter-sha" },
    { 0x0449, "cyrillic-small-letter-shcha" },
    { 0x044a, "cyrillic-small-letter-hard-sign" },
    { 0x044b, "cyrillic-small-letter-yeru" },
    { 0x044c, "cyrillic-small-letter-soft-sign" },
    { 0x044d, "cyrillic-small-letter-e" },
    { 0x044e, "cyrillic-small-letter-yu" },
    { 0x044f, "cyrillic-small-letter-ya" },
    { 0x0451, "cyrillic-small-letter-io" },
    { 0x0452, "cyrillic-small-letter-dje" },
    { 0x0453, "cyrillic-small-letter-gje" },
    { 0x0454, "cyrillic-small-letter-ukrainian-ie" },
    { 0x0455, "cyrillic-small-letter-dze" },
    { 0x0456, "cyrillic-small-letter-byelorussian-ukrainian-i" },
    { 0x0457, "cyrillic-small-letter-yi" },
    { 0x0458, "cyrillic-small-letter-je" },
    { 0x0459, "cyrillic-small-letter-lje" },
    { 0x045a, "cyrillic-small-letter-nje" },
    { 0x045b, "cyrillic-small-letter-tshe" },
    { 0x045c, "cyrillic-small-letter-kje" },
    { 0x045e, "cyrillic-small-letter-short-u" },
    { 0x045f, "cyrillic-small-letter-dzhe" },
    { 0x0490, "cyrillic-capital-letter-ghe-with-upturn" },
    { 0x0491, "cyrillic-small-letter-ghe-with-upturn" },
    { 0x0950, "devanagari-om" },
    { 0x1e80, "latin-capital-letter-w-with-grave" },
    { 0x1e81, "latin-small-letter-w-with-grave" },
    { 0x1e82, "latin-capital-letter-w-with-acute" },
    { 0x1e83, "latin-small-letter-w-with-acute" },
    { 0x1e84, "latin-capital-letter-w-with-diaeresis" },
    { 0x1e85, "latin-small-letter-w-with-diaeresis" },
    { 0x1ef2, "latin-capital-letter-y-with-grave" },
    { 0x1ef3, "latin-small-letter-y-with-grave" },
    { 0x2013, "en-dash" },
    { 0x2014, "em-dash" },
    { 0x2015, "horizontal-bar" },
    { 0x2017, "double-low-line" },
    { 0x2018, "left-single-quotation-mark" },
    { 0x2019, "right-single-quotation-mark" },
    { 0x201a, "single-low-9-quotation-mark" },
    { 0x201b, "single-high-reversed-9-quotation-mark" },
    { 0x201c, "left-double-quotation-mark" },
    { 0x201d, "right-double-quotation-mark" },
    { 0x201e, "double-low-9-quotation-mark" },
    { 0x2020, "dagger" },
    { 0x2021, "double-dagger" },
    { 0x2022, "bullet" },
    { 0x2026, "horizontal-ellipsis" },
    { 0x2030, "per-mille-sign" },
    { 0x2032, "prime" },
    { 0x2033, "double-prime" },
    { 0x2039, "single-left-pointing-angle-quotation-mark" },
    { 0x203a, "single-right-pointing-angle-quotation-mark" },
    { 0x203c, "double-exclamation-mark" },
    { 0x203e, "overline" },
    { 0x2044, "fraction-slash" },
    { 0x207f, "superscript-latin-small-letter-n" },
    { 0x20a3, "french-franc-sign" },
    { 0x20a4, "lira-sign" },
    { 0x20a7, "peseta-sign" },
    { 0x2105, "care-of" },
    { 0x2111, "black-letter-capital-i" },
    { 0x2113, "script-small-l" },
    { 0x2116, "numero-sign" },
    { 0x2118, "script-capital-p" },
    { 0x211c, "black-letter-capital-r" },
    { 0x2122, "trade-mark-sign" },
    { 0x2126, "ohm-sign" },
    { 0x212e, "estimated-symbol" },
    { 0x2135, "alef-symbol" },
    { 0x215b, "vulgar-fraction-one-eighth" },
    { 0x215c, "vulgar-fraction-three-eighths" },
    { 0x215d, "vulgar-fraction-five-eighths" },
    { 0x215e, "vulgar-fraction-seven-eighths" },
    { 0x2190, "leftwards-arrow" },
    { 0x2191, "upwards-arrow" },
    { 0x2192, "rightwards-arrow" },
    { 0x2193, "downwards-arrow" },
    { 0x2194, "left-right-arrow" },
    { 0x2195, "up-down-arrow" },
    { 0x21a8, "up-down-arrow-with-base" },
    { 0x21b5, "downwards-arrow-with-corner-leftwards" },
    { 0x21d0, "leftwards-double-arrow" },
    { 0x21d1, "upwards-double-arrow" },
    { 0x21d2, "rightwards-double-arrow" },
    { 0x21d3, "downwards-double-arrow" },
    { 0x21d4, "left-right-double-arrow" },
    { 0x2200, "for-all" },
    { 0x2202, "partial-differential" },
    { 0x2203, "there-exists" },
    { 0x2205, "empty-set" },
    { 0x2206, "increment" },
    { 0x2207, "nabla" },
    { 0x2208, "element-of" },
    { 0x2209, "not-an-element-of" },
    { 0x220b, "contains-as-member" },
    { 0x220f, "n-ary-product" },
    { 0x2211, "n-ary-summation" },
    { 0x2212, "minus-sign" },
    { 0x2215, "division-slash" },
    { 0x2217, "asterisk-operator" },
    { 0x2219, "bullet-operator" },
    { 0x221a, "square-root" },
    { 0x221d, "proportional-to" },
    { 0x221e, "infinity" },
    { 0x221f, "right-angle" },
    { 0x2220, "angle" },
    { 0x2227, "logical-and" },
    { 0x2228, "logical-or" },
    { 0x2229, "intersection" },
    { 0x222a, "union" },
    { 0x222b, "integral" },
    { 0x2234, "therefore" },
    { 0x223c, "tilde-operator" },
    { 0x2245, "approximately-equal-to" },
    { 0x2248, "almost-equal-to" },
    { 0x2260, "not-equal-to" },
    { 0x2261, "identical-to" },
    { 0x2264, "less-than-or-equal-to" },
    { 0x2265, "greater-than-or-equal-to" },
    { 0x2282, "subset-of" },
    { 0x2283, "superset-of" },
    { 0x2284, "not-a-subset-of" },
    { 0x2286, "subset-of-or-equal-to" },
    { 0x2287, "superset-of-or-equal-to" },
    { 0x2295, "circled-plus" },
    { 0x2297, "circled-times" },
    { 0x22a5, "up-tack" },
    { 0x22c5, "dot-operator" },
    { 0x2302, "house" },
    { 0x2310, "reversed-not-sign" },
    { 0x2320, "top-half-integral" },
    { 0x2321, "bottom-half-integral" },
    { 0x2326, "erase-to-the-right" },
    { 0x2328, "keyboard" },
    { 0x2329, "left-pointing-angle-bracket" },
    { 0x232a, "right-pointing-angle-bracket" },
    { 0x232b, "erase-to-the-left" },
    { 0x2460, "circled-digit-one" },
    { 0x2461, "circled-digit-two" },
    { 0x2462, "circled-digit-three" },
    { 0x2463, "circled-digit-four" },
    { 0x2464, "circled-digit-five" },
    { 0x2465, "circled-digit-six" },
    { 0x2466, "circled-digit-seven" },
    { 0x2467, "circled-digit-eight" },
    { 0x2468, "circled-digit-nine" },
    { 0x2469, "circled-number-ten" },
    { 0x2500, "box-drawings-light-horizontal" },
    { 0x2502, "box-drawings-light-vertical" },
    { 0x250c, "box-drawings-light-down-and-right" },
    { 0x2510, "box-drawings-light-down-and-left" },
    { 0x2514, "box-drawings-light-up-and-right" },
    { 0x2518, "box-drawings-light-up-and-left" },
    { 0x251c, "box-drawings-light-vertical-and-right" },
    { 0x2524, "box-drawings-light-vertical-and-left" },
    { 0x252c, "box-drawings-light-down-and-horizontal" },
    { 0x2534, "box-drawings-light-up-and-horizontal" },
    { 0x253c, "box-drawings-light-vertical-and-horizontal" },
    { 0x2550, "box-drawings-double-horizontal" },
    { 0x2551, "box-drawings-double-vertical" },
    { 0x2552, "box-drawings-down-single-and-right-double" },
    { 0x2553, "box-drawings-down-double-and-right-single" },
    { 0x2554, "box-drawings-double-down-and-right" },
    { 0x2555, "box-drawings-down-single-and-left-double" },
    { 0x2556, "box-drawings-down-double-and-left-single" },
    { 0x2557, "box-drawings-double-down-and-left" },
    { 0x2558, "box-drawings-up-single-and-right-double" },
    { 0x2559, "box-drawings-up-double-and-right-single" },
    { 0x255a, "box-drawings-double-up-and-right" },
    { 0x255b, "box-drawings-up-single-and-left-double" },
    { 0x255c, "box-drawings-up-double-and-left-single" },
    { 0x255d, "box-drawings-double-up-and-left" },
    { 0x255e, "box-drawings-vertical-single-and-right-double" },
    { 0x255f, "box-drawings-vertical-double-and-right-single" },
    { 0x2560, "box-drawings-double-vertical-and-right" },
    { 0x2561, "box-drawings-vertical-single-and-left-double" },
    { 0x2562, "box-drawings-vertical-double-and-left-single" },
    { 0x2563, "box-drawings-double-vertical-and-left" },
    { 0x2564, "box-drawings-down-single-and-horizontal-double" },
    { 0x2565, "box-drawings-down-double-and-horizontal-single" },
    { 0x2566, "box-drawings-double-down-and-horizontal" },
    { 0x2567, "box-drawings-up-single-and-horizontal-double" },
    { 0x2568, "box-drawings-up-double-and-horizontal-single" },
    { 0x2569, "box-drawings-double-up-and-horizontal" },
    { 0x256a, "box-drawings-vertical-single-and-horizontal-double" },
    { 0x256b, "box-drawings-vertical-double-and-horizontal-single" },
    { 0x256c, "box-drawings-double-vertical-and-horizontal" },
    { 0x2580, "upper-half-block" },
    { 0x2584, "lower-half-block" },
    { 0x2588, "full-block" },
    { 0x258c, "left-half-block" },
    { 0x2590, "right-half-block" },
    { 0x2591, "light-shade" },
    { 0x2592, "medium-shade" },
    { 0x2593, "dark-shade" },
    { 0x25a0, "black-square" },
    { 0x25a1, "white-square" },
    { 0x25aa, "black-small-square" },
    { 0x25ab, "white-small-square" },
    { 0x25ac, "black-rectangle" },
    { 0x25b2, "black-up-pointing-triangle" },
    { 0x25ba, "black-right-pointing-pointer" },
    { 0x25bc, "black-down-pointing-triangle" },
    { 0x25c4, "black-left-pointing-pointer" },
    { 0x25c6, "black-diamond" },
    { 0x25ca, "lozenge" },
    { 0x25cb, "white-circle" },
    { 0x25cf, "black-circle" },
    { 0x25d8, "inverse-bullet" },
    { 0x25d9, "inverse-white-circle" },
    { 0x25e6, "white-bullet" },
    { 0x2605, "black-star" },
    { 0x260e, "black-telephone" },
    { 0x2611, "ballot-box-with-check" },
    { 0x2612, "ballot-box-with-x" },
    { 0x261b, "black-right-pointing-index" },
    { 0x261c, "white-left-pointing-index" },
    { 0x261d, "white-up-pointing-index" },
    { 0x261e, "white-right-pointing-index" },
    { 0x261f, "white-down-pointing-index" },
    { 0x2620, "skull-and-crossbones" },
    { 0x262a, "star-and-crescent" },
    { 0x262f, "yin-yang" },
    { 0x2638, "wheel-of-dharma" },
    { 0x2639, "white-frowning-face" },
    { 0x263a, "white-smiling-face" },
    { 0x263b, "black-smiling-face" },
    { 0x263c, "white-sun-with-rays" },
    { 0x2640, "female-sign" },
    { 0x2642, "male-sign" },
    { 0x2648, "aries" },
    { 0x2649, "taurus" },
    { 0x264a, "gemini" },
    { 0x264b, "cancer" },
    { 0x264c, "leo" },
    { 0x264d, "virgo" },
    { 0x264e, "libra" },
    { 0x264f, "scorpius" },
    { 0x2650, "sagittarius" },
    { 0x2651, "capricorn" },
    { 0x2652, "aquarius" },
    { 0x2653, "pisces" },
    { 0x2660, "black-spade-suit" },
    { 0x2663, "black-club-suit" },
    { 0x2665, "black-heart-suit" },
    { 0x2666, "black-diamond-suit" },
    { 0x266a, "eighth-note" },
    { 0x266b, "beamed-eighth-notes" },
    { 0x2701, "upper-blade-scissors" },
    { 0x2702, "black-scissors" },
    { 0x2703, "lower-blade-scissors" },
    { 0x2704, "white-scissors" },
    { 0x2706, "telephone-location-sign" },
    { 0x2707, "tape-drive" },
    { 0x2708, "airplane" },
    { 0x2709, "envelope" },
    { 0x270c, "victory-hand" },
    { 0x270d, "writing-hand" },
    { 0x270e, "lower-right-pencil" },
    { 0x270f, "pencil" },
    { 0x2710, "upper-right-pencil" },
    { 0x2711, "white-nib" },
    { 0x2712, "black-nib" },
    { 0x2713, "check-mark" },
    { 0x2714, "heavy-check-mark" },
    { 0x2715, "multiplication-x" },
    { 0x2716, "heavy-multiplication-x" },
    { 0x2717, "ballot-x" },
    { 0x2718, "heavy-ballot-x" },
    { 0x2719, "outlined-greek-cross" },
    { 0x271a, "heavy-greek-cross" },
    { 0x271b, "open-centre-cross" },
    { 0x271c, "heavy-open-centre-cross" },
    { 0x271d, "latin-cross" },
    { 0x271e, "shadowed-white-latin-cross" },
    { 0x271f, "outlined-latin-cross" },
    { 0x2720, "maltese-cross" },
    { 0x2721, "star-of-david" },
    { 0x2722, "four-teardrop-spoked-asterisk" },
    { 0x2723, "four-balloon-spoked-asterisk" },
    { 0x2724, "heavy-four-balloon-spoked-asterisk" },
    { 0x2725, "four-club-spoked-asterisk" },
    { 0x2726, "black-four-pointed-star" },
    { 0x2727, "white-four-pointed-star" },
    { 0x2729, "stress-outlined-white-star" },
    { 0x272a, "circled-white-star" },
    { 0x272b, "open-centre-black-star" },
    { 0x272c, "black-centre-white-star" },
    { 0x272d, "outlined-black-star" },
    { 0x272e, "heavy-outlined-black-star" },
    { 0x272f, "pinwheel-star" },
    { 0x2730, "shadowed-white-star" },
    { 0x2731, "heavy-asterisk" },
    { 0x2732, "open-centre-asterisk" },
    { 0x2733, "eight-spoked-asterisk" },
    { 0x2734, "eight-pointed-black-star" },
    { 0x2735, "eight-pointed-pinwheel-star" },
    { 0x2736, "six-pointed-black-star" },
    { 0x2737, "eight-pointed-rectilinear-black-star" },
    { 0x2738, "heavy-eight-pointed-rectilinear-black-star" },
    { 0x2739, "twelve-pointed-black-star" },
    { 0x273a, "sixteen-pointed-asterisk" },
    { 0x273b, "teardrop-spoked-asterisk" },
    { 0x273c, "open-centre-teardrop-spoked-asterisk" },
    { 0x273d, "heavy-teardrop-spoked-asterisk" },
    { 0x273e, "six-petalled-black-and-white-florette" },
    { 0x273f, "black-florette" },
    { 0x2740, "white-florette" },
    { 0x2741, "eight-petalled-outlined-black-florette" },
    { 0x2742, "circled-open-centre-eight-pointed-star" },
    { 0x2743, "heavy-teardrop-spoked-pinwheel-asterisk" },
    { 0x2744, "snowflake" },
    { 0x2745, "tight-trifoliate-snowflake" },
    { 0x2746, "heavy-chevron-snowflake" },
    { 0x2747, "sparkle" },
    { 0x2748, "heavy-sparkle" },
    { 0x2749, "balloon-spoked-asterisk" },
    { 0x274a, "eight-teardrop-spoked-propeller-asterisk" },
    { 0x274b, "heavy-eight-teardrop-spoked-propeller-asterisk" },
    { 0x274d, "shadowed-white-circle" },
    { 0x274f, "lower-right-drop-shadowed-white-square" },
    { 0x2750, "upper-right-drop-shadowed-white-square" },
    { 0x2751, "lower-right-shadowed-white-square" },
    { 0x2752, "upper-right-shadowed-white-square" },
    { 0x2756, "black-diamond-minus-white-x" },
    { 0x2758, "light-vertical-bar" },
    { 0x2759, "medium-vertical-bar" },
    { 0x275a, "heavy-vertical-bar" },
    { 0x275b, "heavy-single-turned-comma-quotation-mark-ornament" },
    { 0x275c, "heavy-single-comma-quotation-mark-ornament" },
    { 0x275d, "heavy-double-turned-comma-quotation-mark-ornament" },
    { 0x275e, "heavy-double-comma-quotation-mark-ornament" },
    { 0x2761, "curved-stem-paragraph-sign-ornament" },
    { 0x2762, "heavy-exclamation-mark-ornament" },
    { 0x2763, "heavy-heart-exclamation-mark-ornament" },
    { 0x2764, "heavy-black-heart" },
    { 0x2765, "rotated-heavy-black-heart-bullet" },
    { 0x2766, "floral-heart" },
    { 0x2767, "rotated-floral-heart-bullet" },
    { 0x2776, "dingbat-negative-circled-digit-one" },
    { 0x2777, "dingbat-negative-circled-digit-two" },
    { 0x2778, "dingbat-negative-circled-digit-three" },
    { 0x2779, "dingbat-negative-circled-digit-four" },
    { 0x277a, "dingbat-negative-circled-digit-five" },
    { 0x277b, "dingbat-negative-circled-digit-six" },
    { 0x277c, "dingbat-negative-circled-digit-seven" },
    { 0x277d, "dingbat-negative-circled-digit-eight" },
    { 0x277e, "dingbat-negative-circled-digit-nine" },
    { 0x277f, "dingbat-negative-circled-number-ten" },
    { 0x2780, "dingbat-circled-sans-serif-digit-one" },
    { 0x2781, "dingbat-circled-sans-serif-digit-two" },
    { 0x2782, "dingbat-circled-sans-serif-digit-three" },
    { 0x2783, "dingbat-circled-sans-serif-digit-four" },
    { 0x2784, "dingbat-circled-sans-serif-digit-five" },
    { 0x2785, "dingbat-circled-sans-serif-digit-six" },
    { 0x2786, "dingbat-circled-sans-serif-digit-seven" },
    { 0x2787, "dingbat-circled-sans-serif-digit-eight" },
    { 0x2788, "dingbat-circled-sans-serif-digit-nine" },
    { 0x2789, "dingbat-circled-sans-serif-number-ten" },
    { 0x278a, "dingbat-negative-circled-sans-serif-digit-one" },
    { 0x278b, "dingbat-negative-circled-sans-serif-digit-two" },
    { 0x278c, "dingbat-negative-circled-sans-serif-digit-three" },
    { 0x278d, "dingbat-negative-circled-sans-serif-digit-four" },
    { 0x278e, "dingbat-negative-circled-sans-serif-digit-five" },
    { 0x278f, "dingbat-negative-circled-sans-serif-digit-six" },
    { 0x2790, "dingbat-negative-circled-sans-serif-digit-seven" },
    { 0x2791, "dingbat-negative-circled-sans-serif-digit-eight" },
    { 0x2792, "dingbat-negative-circled-sans-serif-digit-nine" },
    { 0x2793, "dingbat-negative-circled-sans-serif-number-ten" },
    { 0x2794, "heavy-wide-headed-rightwards-arrow" },
    { 0x2798, "heavy-south-east-arrow" },
    { 0x2799, "heavy-rightwards-arrow" },
    { 0x279a, "heavy-north-east-arrow" },
    { 0x279b, "drafting-point-rightwards-arrow" },
    { 0x279c, "heavy-round-tipped-rightwards-arrow" },
    { 0x279d, "triangle-headed-rightwards-arrow" },
    { 0x279e, "heavy-triangle-headed-rightwards-arrow" },
    { 0x279f, "dashed-triangle-headed-rightwards-arrow" },
    { 0x27a0, "heavy-dashed-triangle-headed-rightwards-arrow" },
    { 0x27a1, "black-rightwards-arrow" },
    { 0x27a2, "three-d-top-lighted-rightwards-arrowhead" },
    { 0x27a3, "three-d-bottom-lighted-rightwards-arrowhead" },
    { 0x27a4, "black-rightwards-arrowhead" },
    { 0x27a5, "heavy-black-curved-downwards-and-rightwards-arrow" },
    { 0x27a6, "heavy-black-curved-upwards-and-rightwards-arrow" },
    { 0x27a7, "squat-black-rightwards-arrow" },
    { 0x27a8, "heavy-concave-pointed-black-rightwards-arrow" },
    { 0x27a9, "right-shaded-white-rightwards-arrow" },
    { 0x27aa, "left-shaded-white-rightwards-arrow" },
    { 0x27ab, "back-tilted-shadowed-white-rightwards-arrow" },
    { 0x27ac, "front-tilted-shadowed-white-rightwards-arrow" },
    { 0x27ad, "heavy-lower-right-shadowed-white-rightwards-arrow" },
    { 0x27ae, "heavy-upper-right-shadowed-white-rightwards-arrow" },
    { 0x27af, "notched-lower-right-shadowed-white-rightwards-arrow" },
    { 0x27b1, "notched-upper-right-shadowed-white-rightwards-arrow" },
    { 0x27b2, "circled-heavy-white-rightwards-arrow" },
    { 0x27b3, "white-feathered-rightwards-arrow" },
    { 0x27b4, "black-feathered-south-east-arrow" },
    { 0x27b5, "black-feathered-rightwards-arrow" },
    { 0x27b6, "black-feathered-north-east-arrow" },
    { 0x27b7, "heavy-black-feathered-south-east-arrow" },
    { 0x27b8, "heavy-black-feathered-rightwards-arrow" },
    { 0x27b9, "heavy-black-feathered-north-east-arrow" },
    { 0x27ba, "teardrop-barbed-rightwards-arrow" },
    { 0x27bb, "heavy-teardrop-shanked-rightwards-arrow" },
    { 0x27bc, "wedge-tailed-rightwards-arrow" },
    { 0x27bd, "heavy-wedge-tailed-rightwards-arrow" },
    { 0x27be, "open-outlined-rightwards-arrow" },
    { 0xfb01, "latin-small-ligature-fi" },
    { 0xfb02, "latin-small-ligature-fl" },
  };
  for (size_t i = 0; i < SIZEOF(chars); i++)
    namedCharTable_.insert(makeStringC(chars[i].name), chars[i].c);
}

void Interpreter::installSdata()
{
  // This comes from uni2sgml.txt on ftp://unicode.org.
  // It is marked there as obsolete, so it probably ought to be checked.
  // The definitions of apos and quot have been fixed for consistency with XML.
  static struct {
    Char c;
    const char *name;
  } entities[] = {
  { 0x0021, "excl" },
  { 0x0022, "quot" },
  { 0x0023, "num" },
  { 0x0024, "dollar" },
  { 0x0025, "percnt" },
  { 0x0026, "amp" },
  { 0x0027, "apos" },
  { 0x0028, "lpar" },
  { 0x0029, "rpar" },
  { 0x002A, "ast" },
  { 0x002B, "plus" },
  { 0x002C, "comma" },
  { 0x002D, "hyphen" },
  { 0x002E, "period" },
  { 0x002F, "sol" },
  { 0x003A, "colon" },
  { 0x003B, "semi" },
  { 0x003C, "lt" },
  { 0x003D, "equals" },
  { 0x003E, "gt" },
  { 0x003F, "quest" },
  { 0x0040, "commat" },
  { 0x005B, "lsqb" },
  { 0x005C, "bsol" },
  { 0x005D, "rsqb" },
  { 0x005E, "circ" },
  { 0x005F, "lowbar" },
  { 0x0060, "grave" },
  { 0x007B, "lcub" },
  { 0x007C, "verbar" },
  { 0x007D, "rcub" },
  { 0x007E, "tilde" },
  { 0x00A0, "nbsp" },
  { 0x00A1, "iexcl" },
  { 0x00A2, "cent" },
  { 0x00A3, "pound" },
  { 0x00A4, "curren" },
  { 0x00A5, "yen" },
  { 0x00A6, "brvbar" },
  { 0x00A7, "sect" },
  { 0x00A8, "die" },
  { 0x00A8, "Dot" },
  { 0x00A8, "uml" },
  { 0x00A9, "copy" },
  { 0x00AA, "ordf" },
  { 0x00AB, "laquo" },
  { 0x00AC, "not" },
  { 0x00AD, "shy" },
  { 0x00AE, "reg" },
  { 0x00AF, "macr" },
  { 0x00B0, "deg" },
  { 0x00B1, "plusmn" },
  { 0x00B2, "sup2" },
  { 0x00B3, "sup3" },
  { 0x00B4, "acute" },
  { 0x00B5, "micro" },
  { 0x00B6, "para" },
  { 0x00B7, "middot" },
  { 0x00B8, "cedil" },
  { 0x00B9, "sup1" },
  { 0x00BA, "ordm" },
  { 0x00BB, "raquo" },
  { 0x00BC, "frac14" },
  { 0x00BD, "frac12" },
  { 0x00BE, "frac34" },
  { 0x00BF, "iquest" },
  { 0x00C0, "Agrave" },
  { 0x00C1, "Aacute" },
  { 0x00C2, "Acirc" },
  { 0x00C3, "Atilde" },
  { 0x00C4, "Auml" },
  { 0x00C5, "Aring" },
  { 0x00C6, "AElig" },
  { 0x00C7, "Ccedil" },
  { 0x00C8, "Egrave" },
  { 0x00C9, "Eacute" },
  { 0x00CA, "Ecirc" },
  { 0x00CB, "Euml" },
  { 0x00CC, "Igrave" },
  { 0x00CD, "Iacute" },
  { 0x00CE, "Icirc" },
  { 0x00CF, "Iuml" },
  { 0x00D0, "ETH" },
  { 0x00D1, "Ntilde" },
  { 0x00D2, "Ograve" },
  { 0x00D3, "Oacute" },
  { 0x00D4, "Ocirc" },
  { 0x00D5, "Otilde" },
  { 0x00D6, "Ouml" },
  { 0x00D7, "times" },
  { 0x00D8, "Oslash" },
  { 0x00D9, "Ugrave" },
  { 0x00DA, "Uacute" },
  { 0x00DB, "Ucirc" },
  { 0x00DC, "Uuml" },
  { 0x00DD, "Yacute" },
  { 0x00DE, "THORN" },
  { 0x00DF, "szlig" },
  { 0x00E0, "agrave" },
  { 0x00E1, "aacute" },
  { 0x00E2, "acirc" },
  { 0x00E3, "atilde" },
  { 0x00E4, "auml" },
  { 0x00E5, "aring" },
  { 0x00E6, "aelig" },
  { 0x00E7, "ccedil" },
  { 0x00E8, "egrave" },
  { 0x00E9, "eacute" },
  { 0x00EA, "ecirc" },
  { 0x00EB, "euml" },
  { 0x00EC, "igrave" },
  { 0x00ED, "iacute" },
  { 0x00EE, "icirc" },
  { 0x00EF, "iuml" },
  { 0x00F0, "eth" },
  { 0x00F1, "ntilde" },
  { 0x00F2, "ograve" },
  { 0x00F3, "oacute" },
  { 0x00F4, "ocirc" },
  { 0x00F5, "otilde" },
  { 0x00F6, "ouml" },
  { 0x00F7, "divide" },
  { 0x00F8, "oslash" },
  { 0x00F9, "ugrave" },
  { 0x00FA, "uacute" },
  { 0x00FB, "ucirc" },
  { 0x00FC, "uuml" },
  { 0x00FD, "yacute" },
  { 0x00FE, "thorn" },
  { 0x00FF, "yuml" },
  { 0x0100, "Amacr" },
  { 0x0101, "amacr" },
  { 0x0102, "Abreve" },
  { 0x0103, "abreve" },
  { 0x0104, "Aogon" },
  { 0x0105, "aogon" },
  { 0x0106, "Cacute" },
  { 0x0107, "cacute" },
  { 0x0108, "Ccirc" },
  { 0x0109, "ccirc" },
  { 0x010A, "Cdot" },
  { 0x010B, "cdot" },
  { 0x010C, "Ccaron" },
  { 0x010D, "ccaron" },
  { 0x010E, "Dcaron" },
  { 0x010F, "dcaron" },
  { 0x0110, "Dstrok" },
  { 0x0111, "dstrok" },
  { 0x0112, "Emacr" },
  { 0x0113, "emacr" },
  { 0x0116, "Edot" },
  { 0x0117, "edot" },
  { 0x0118, "Eogon" },
  { 0x0119, "eogon" },
  { 0x011A, "Ecaron" },
  { 0x011B, "ecaron" },
  { 0x011C, "Gcirc" },
  { 0x011D, "gcirc" },
  { 0x011E, "Gbreve" },
  { 0x011F, "gbreve" },
  { 0x0120, "Gdot" },
  { 0x0121, "gdot" },
  { 0x0122, "Gcedil" },
  { 0x0123, "gcedil" },
  { 0x0124, "Hcirc" },
  { 0x0125, "hcirc" },
  { 0x0126, "Hstrok" },
  { 0x0127, "hstrok" },
  { 0x0128, "Itilde" },
  { 0x0129, "itilde" },
  { 0x012A, "Imacr" },
  { 0x012B, "imacr" },
  { 0x012E, "Iogon" },
  { 0x012F, "iogon" },
  { 0x0130, "Idot" },
  { 0x0131, "inodot" },
  { 0x0132, "IJlig" },
  { 0x0133, "ijlig" },
  { 0x0134, "Jcirc" },
  { 0x0135, "jcirc" },
  { 0x0136, "Kcedil" },
  { 0x0137, "kcedil" },
  { 0x0138, "kgreen" },
  { 0x0139, "Lacute" },
  { 0x013A, "lacute" },
  { 0x013B, "Lcedil" },
  { 0x013C, "lcedil" },
  { 0x013D, "Lcaron" },
  { 0x013E, "lcaron" },
  { 0x013F, "Lmidot" },
  { 0x0140, "lmidot" },
  { 0x0141, "Lstrok" },
  { 0x0142, "lstrok" },
  { 0x0143, "Nacute" },
  { 0x0144, "nacute" },
  { 0x0145, "Ncedil" },
  { 0x0146, "ncedil" },
  { 0x0147, "Ncaron" },
  { 0x0148, "ncaron" },
  { 0x0149, "napos" },
  { 0x014A, "ENG" },
  { 0x014B, "eng" },
  { 0x014C, "Omacr" },
  { 0x014D, "omacr" },
  { 0x0150, "Odblac" },
  { 0x0151, "odblac" },
  { 0x0152, "OElig" },
  { 0x0153, "oelig" },
  { 0x0154, "Racute" },
  { 0x0155, "racute" },
  { 0x0156, "Rcedil" },
  { 0x0157, "rcedil" },
  { 0x0158, "Rcaron" },
  { 0x0159, "rcaron" },
  { 0x015A, "Sacute" },
  { 0x015B, "sacute" },
  { 0x015C, "Scirc" },
  { 0x015D, "scirc" },
  { 0x015E, "Scedil" },
  { 0x015F, "scedil" },
  { 0x0160, "Scaron" },
  { 0x0161, "scaron" },
  { 0x0162, "Tcedil" },
  { 0x0163, "tcedil" },
  { 0x0164, "Tcaron" },
  { 0x0165, "tcaron" },
  { 0x0166, "Tstrok" },
  { 0x0167, "tstrok" },
  { 0x0168, "Utilde" },
  { 0x0169, "utilde" },
  { 0x016A, "Umacr" },
  { 0x016B, "umacr" },
  { 0x016C, "Ubreve" },
  { 0x016D, "ubreve" },
  { 0x016E, "Uring" },
  { 0x016F, "uring" },
  { 0x0170, "Udblac" },
  { 0x0171, "udblac" },
  { 0x0172, "Uogon" },
  { 0x0173, "uogon" },
  { 0x0174, "Wcirc" },
  { 0x0175, "wcirc" },
  { 0x0176, "Ycirc" },
  { 0x0177, "ycirc" },
  { 0x0178, "Yuml" },
  { 0x0179, "Zacute" },
  { 0x017A, "zacute" },
  { 0x017B, "Zdot" },
  { 0x017C, "zdot" },
  { 0x017D, "Zcaron" },
  { 0x017E, "zcaron" },
  { 0x0192, "fnof" },
  { 0x02C7, "caron" },
  { 0x02D8, "breve" },
  { 0x02D9, "dot" },
  { 0x02DA, "ring" },
  { 0x02DB, "ogon" },
  { 0x02DC, "tilde" },
  { 0x02DD, "dblac" },
  { 0x0386, "Aacgr" },
  { 0x0388, "Eacgr" },
  { 0x0389, "EEacgr" },
  { 0x038A, "Iacgr" },
  { 0x038C, "Oacgr" },
  { 0x038E, "Uacgr" },
  { 0x038F, "OHacgr" },
  { 0x0390, "idiagr" },
  { 0x0391, "Agr" },
  { 0x0392, "Bgr" },
  { 0x0393, "Ggr" },
  { 0x0393, "Gamma" },
  { 0x0394, "Dgr" },
  { 0x0394, "Delta" },
  { 0x0395, "Egr" },
  { 0x0396, "Zgr" },
  { 0x0397, "EEgr" },
  { 0x0398, "THgr" },
  { 0x0398, "Theta" },
  { 0x0399, "Igr" },
  { 0x039A, "Kgr" },
  { 0x039B, "Lgr" },
  { 0x039B, "Lambda" },
  { 0x039C, "Mgr" },
  { 0x039D, "Ngr" },
  { 0x039E, "Xgr" },
  { 0x039E, "Xi" },
  { 0x039F, "Ogr" },
  { 0x03A0, "Pgr" },
  { 0x03A0, "Pi" },
  { 0x03A1, "Rgr" },
  { 0x03A3, "Sgr" },
  { 0x03A3, "Sigma" },
  { 0x03A4, "Tgr" },
  { 0x03A5, "Ugr" },
  { 0x03A5, "Upsi" },
  { 0x03A6, "PHgr" },
  { 0x03A6, "Phi" },
  { 0x03A7, "KHgr" },
  { 0x03A8, "PSgr" },
  { 0x03A8, "Psi" },
  { 0x03A9, "OHgr" },
  { 0x03A9, "Omega" },
  { 0x03AA, "Idigr" },
  { 0x03AB, "Udigr" },
  { 0x03AC, "aacgr" },
  { 0x03AD, "eacgr" },
  { 0x03AE, "eeacgr" },
  { 0x03AF, "iacgr" },
  { 0x03B0, "udiagr" },
  { 0x03B1, "agr" },
  { 0x03B1, "alpha" },
  { 0x03B2, "bgr" },
  { 0x03B2, "beta" },
  { 0x03B3, "ggr" },
  { 0x03B3, "gamma" },
  { 0x03B4, "dgr" },
  { 0x03B4, "delta" },
  { 0x03B5, "egr" },
  { 0x03B5, "epsi" },
  { 0x03B6, "zgr" },
  { 0x03B6, "zeta" },
  { 0x03B7, "eegr" },
  { 0x03B7, "eta" },
  { 0x03B8, "thetas" },
  { 0x03B8, "thgr" },
  { 0x03B9, "igr" },
  { 0x03B9, "iota" },
  { 0x03BA, "kgr" },
  { 0x03BA, "kappa" },
  { 0x03BB, "lgr" },
  { 0x03BB, "lambda" },
  { 0x03BC, "mgr" },
  { 0x03BC, "mu" },
  { 0x03BD, "ngr" },
  { 0x03BD, "nu" },
  { 0x03BE, "xgr" },
  { 0x03BE, "xi" },
  { 0x03BF, "ogr" },
  { 0x03C0, "pgr" },
  { 0x03C0, "pi" },
  { 0x03C1, "rgr" },
  { 0x03C1, "rho" },
  { 0x03C2, "sfgr" },
  { 0x03C2, "sigmav" },
  { 0x03C3, "sgr" },
  { 0x03C3, "sigma" },
  { 0x03C4, "tgr" },
  { 0x03C4, "tau" },
  { 0x03C5, "ugr" },
  { 0x03C5, "upsi" },
  { 0x03C6, "phgr" },
  { 0x03C6, "phis" },
  { 0x03C7, "khgr" },
  { 0x03C7, "chi" },
  { 0x03C8, "psgr" },
  { 0x03C8, "psi" },
  { 0x03C9, "ohgr" },
  { 0x03C9, "omega" },
  { 0x03CA, "idigr" },
  { 0x03CB, "udigr" },
  { 0x03CC, "oacgr" },
  { 0x03CD, "uacgr" },
  { 0x03CE, "ohacgr" },
  { 0x03D1, "thetav" },
  { 0x03D5, "phiv" },
  { 0x03D6, "piv" },
  { 0x03DD, "gammad" },
  { 0x03F0, "kappav" },
  { 0x03F1, "rhov" },
  { 0x0401, "IOcy" },
  { 0x0402, "DJcy" },
  { 0x0403, "GJcy" },
  { 0x0404, "Jukcy" },
  { 0x0405, "DScy" },
  { 0x0406, "Iukcy" },
  { 0x0407, "YIcy" },
  { 0x0408, "Jsercy" },
  { 0x0409, "LJcy" },
  { 0x040A, "NJcy" },
  { 0x040B, "TSHcy" },
  { 0x040C, "KJcy" },
  { 0x040E, "Ubrcy" },
  { 0x040F, "DZcy" },
  { 0x0410, "Acy" },
  { 0x0411, "Bcy" },
  { 0x0412, "Vcy" },
  { 0x0413, "Gcy" },
  { 0x0414, "dcy" },
  { 0x0415, "IEcy" },
  { 0x0416, "ZHcy" },
  { 0x0417, "Zcy" },
  { 0x0418, "Icy" },
  { 0x0419, "Jcy" },
  { 0x041A, "Kcy" },
  { 0x041B, "Lcy" },
  { 0x041C, "Mcy" },
  { 0x041D, "Ncy" },
  { 0x041E, "Ocy" },
  { 0x041F, "Pcy" },
  { 0x0420, "Rcy" },
  { 0x0421, "Scy" },
  { 0x0422, "Tcy" },
  { 0x0423, "Ucy" },
  { 0x0424, "Fcy" },
  { 0x0425, "KHcy" },
  { 0x0426, "TScy" },
  { 0x0427, "CHcy" },
  { 0x0428, "SHcy" },
  { 0x0429, "SHCHcy" },
  { 0x042A, "HARDcy" },
  { 0x042B, "Ycy" },
  { 0x042C, "SOFTcy" },
  { 0x042D, "Ecy" },
  { 0x042E, "YUcy" },
  { 0x042F, "YAcy" },
  { 0x0430, "acy" },
  { 0x0431, "bcy" },
  { 0x0432, "vcy" },
  { 0x0433, "gcy" },
  { 0x0434, "dcy" },
  { 0x0435, "iecy" },
  { 0x0436, "zhcy" },
  { 0x0437, "zcy" },
  { 0x0438, "icy" },
  { 0x0439, "jcy" },
  { 0x043A, "kcy" },
  { 0x043B, "lcy" },
  { 0x043C, "mcy" },
  { 0x043D, "ncy" },
  { 0x043E, "ocy" },
  { 0x043F, "pcy" },
  { 0x0440, "rcy" },
  { 0x0441, "scy" },
  { 0x0442, "tcy" },
  { 0x0443, "ucy" },
  { 0x0444, "fcy" },
  { 0x0445, "khcy" },
  { 0x0446, "tscy" },
  { 0x0447, "chcy" },
  { 0x0448, "shcy" },
  { 0x0449, "shchcy" },
  { 0x044A, "hardcy" },
  { 0x044B, "ycy" },
  { 0x044C, "softcy" },
  { 0x044D, "ecy" },
  { 0x044E, "yucy" },
  { 0x044F, "yacy" },
  { 0x0451, "iocy" },
  { 0x0452, "djcy" },
  { 0x0453, "gjcy" },
  { 0x0454, "jukcy" },
  { 0x0455, "dscy" },
  { 0x0456, "iukcy" },
  { 0x0457, "yicy" },
  { 0x0458, "jsercy" },
  { 0x0459, "ljcy" },
  { 0x045A, "njcy" },
  { 0x045B, "tshcy" },
  { 0x045C, "kjcy" },
  { 0x045E, "ubrcy" },
  { 0x045F, "dzcy" },
  { 0x2002, "ensp" },
  { 0x2003, "emsp" },
  { 0x2004, "emsp13" },
  { 0x2005, "emsp14" },
  { 0x2007, "numsp" },
  { 0x2008, "puncsp" },
  { 0x2009, "thinsp" },
  { 0x200A, "hairsp" },
  { 0x2010, "dash" },
  { 0x2013, "ndash" },
  { 0x2014, "mdash" },
  { 0x2015, "horbar" },
  { 0x2016, "Verbar" },
  { 0x2018, "lsquo" },
  { 0x2018, "rsquor" },
  { 0x2019, "rsquo" },
  { 0x201A, "lsquor" },
  { 0x201C, "ldquo" },
  { 0x201C, "rdquor" },
  { 0x201D, "rdquo" },
  { 0x201E, "ldquor" },
  { 0x2020, "dagger" },
  { 0x2021, "Dagger" },
  { 0x2022, "bull" },
  { 0x2025, "nldr" },
  { 0x2026, "hellip" },
  { 0x2026, "mldr" },
  { 0x2030, "permil" },
  { 0x2032, "prime" },
  { 0x2032, "vprime" },
  { 0x2033, "Prime" },
  { 0x2034, "tprime" },
  { 0x2035, "bprime" },
  { 0x2041, "caret" },
  { 0x2043, "hybull" },
  { 0x20DB, "tdot" },
  { 0x20DC, "DotDot" },
  { 0x2105, "incare" },
  { 0x210B, "hamilt" },
  { 0x210F, "planck" },
  { 0x2111, "image" },
  { 0x2112, "lagran" },
  { 0x2113, "ell" },
  { 0x2116, "numero" },
  { 0x2117, "copysr" },
  { 0x2118, "weierp" },
  { 0x211C, "real" },
  { 0x211E, "rx" },
  { 0x2122, "trade" },
  { 0x2126, "ohm" },
  { 0x212B, "angst" },
  { 0x212C, "bernou" },
  { 0x2133, "phmmat" },
  { 0x2134, "order" },
  { 0x2135, "aleph" },
  { 0x2136, "beth" },
  { 0x2137, "gimel" },
  { 0x2138, "daleth" },
  { 0x2153, "frac13" },
  { 0x2154, "frac23" },
  { 0x2155, "frac15" },
  { 0x2156, "frac25" },
  { 0x2157, "frac35" },
  { 0x2158, "frac45" },
  { 0x2159, "frac16" },
  { 0x215A, "frac56" },
  { 0x215B, "frac18" },
  { 0x215C, "frac38" },
  { 0x215D, "frac58" },
  { 0x215E, "frac78" },
  { 0x2190, "larr" },
  { 0x2191, "uarr" },
  { 0x2192, "rarr" },
  { 0x2193, "darr" },
  { 0x2194, "harr" },
  { 0x2195, "varr" },
  { 0x2196, "nwarr" },
  { 0x2197, "nearr" },
  { 0x2198, "drarr" },
  { 0x2199, "dlarr" },
  { 0x219A, "nlarr" },
  { 0x219B, "nrarr" },
  { 0x219D, "rarrw" },
  { 0x219E, "Larr" },
  { 0x21A0, "Rarr" },
  { 0x21A2, "larrtl" },
  { 0x21A3, "rarrtl" },
  { 0x21A6, "map" },
  { 0x21A9, "larrhk" },
  { 0x21AA, "rarrhk" },
  { 0x21AB, "larrlp" },
  { 0x21AC, "rarrlp" },
  { 0x21AD, "harrw" },
  { 0x21AE, "nharr" },
  { 0x21B0, "lsh" },
  { 0x21B1, "rsh" },
  { 0x21B6, "cularr" },
  { 0x21B7, "curarr" },
  { 0x21BA, "olarr" },
  { 0x21BB, "orarr" },
  { 0x21BC, "lharu" },
  { 0x21BD, "lhard" },
  { 0x21BE, "uharr" },
  { 0x21BF, "uharl" },
  { 0x21C0, "rharu" },
  { 0x21C1, "rhard" },
  { 0x21C2, "dharr" },
  { 0x21C3, "dharl" },
  { 0x21C4, "rlarr2" },
  { 0x21C6, "lrarr2" },
  { 0x21C7, "larr2" },
  { 0x21C8, "uarr2" },
  { 0x21C9, "rarr2" },
  { 0x21CA, "darr2" },
  { 0x21CB, "lrhar2" },
  { 0x21CC, "rlhar2" },
  { 0x21CD, "nlArr" },
  { 0x21CE, "nhArr" },
  { 0x21CF, "nrArr" },
  { 0x21D0, "lArr" },
  { 0x21D1, "uArr" },
  { 0x21D2, "rArr" },
  { 0x21D3, "dArr" },
  { 0x21D4, "hArr" },
  { 0x21D4, "iff" },
  { 0x21D5, "vArr" },
  { 0x21DA, "lAarr" },
  { 0x21DB, "rAarr" },
  { 0x21DD, "rarrw" },
  { 0x2200, "forall" },
  { 0x2201, "comp" },
  { 0x2202, "part" },
  { 0x2203, "exist" },
  { 0x2204, "nexist" },
  { 0x2205, "empty" },
  { 0x2207, "nabla" },
  { 0x2208, "isin" },
  { 0x2209, "notin" },
  { 0x220A, "epsis" },
  { 0x220B, "ni" },
  { 0x220D, "bepsi" },
  { 0x220F, "prod" },
  { 0x2210, "coprod" },
  { 0x2210, "amalg" },
  { 0x2210, "samalg" },
  { 0x2211, "sum" },
  { 0x2212, "minus" },
  { 0x2213, "mnplus" },
  { 0x2214, "plusdo" },
  { 0x2216, "setmn" },
  { 0x2218, "compfn" },
  { 0x221A, "radic" },
  { 0x221D, "vprop" },
  { 0x221D, "prop" },
  { 0x221E, "infin" },
  { 0x221F, "ang90" },
  { 0x2220, "ang" },
  { 0x2221, "angmsd" },
  { 0x2222, "angsph" },
  { 0x2223, "mid" },
  { 0x2224, "nmid" },
  { 0x2225, "par" },
  { 0x2226, "npar" },
  { 0x2227, "and" },
  { 0x2228, "or" },
  { 0x2229, "cap" },
  { 0x222A, "cup" },
  { 0x222B, "int" },
  { 0x222E, "conint" },
  { 0x2234, "there4" },
  { 0x2235, "becaus" },
  { 0x223C, "thksim" },
  { 0x223C, "sim" },
  { 0x223D, "bsim" },
  { 0x2240, "wreath" },
  { 0x2241, "nsim" },
  { 0x2243, "sime" },
  { 0x2244, "nsime" },
  { 0x2245, "cong" },
  { 0x2247, "ncong" },
  { 0x2248, "thkap" },
  { 0x2248, "ap" },
  { 0x2249, "nap" },
  { 0x224A, "ape" },
  { 0x224C, "bcong" },
  { 0x224D, "asymp" },
  { 0x224E, "bump" },
  { 0x224F, "bumpe" },
  { 0x2250, "esdot" },
  { 0x2251, "eDot" },
  { 0x2252, "efDot" },
  { 0x2253, "erDot" },
  { 0x2254, "colone" },
  { 0x2255, "ecolon" },
  { 0x2256, "ecir" },
  { 0x2257, "cire" },
  { 0x2259, "wedgeq" },
  { 0x225C, "trie" },
  { 0x2260, "ne" },
  { 0x2261, "equiv" },
  { 0x2262, "nequiv" },
  { 0x2264, "le" },
  { 0x2264, "les" },
  { 0x2265, "ge" },
  { 0x2265, "ges" },
  { 0x2266, "lE" },
  { 0x2267, "gE" },
  { 0x2268, "lnE" },
  { 0x2268, "lvnE" },
  { 0x2269, "gnE" },
  { 0x2269, "gvnE" },
  { 0x226A, "Lt" },
  { 0x226B, "Gt" },
  { 0x226C, "twixt" },
  { 0x226E, "nlt" },
  { 0x226F, "ngt" },
  { 0x2270, "nles" },
  { 0x2270, "nle" },
  { 0x2271, "nges" },
  { 0x2271, "nge" },
  { 0x2272, "lsim" },
  { 0x2273, "gsim" },
  { 0x2276, "lg" },
  { 0x2277, "gl" },
  { 0x227A, "pr" },
  { 0x227B, "sc" },
  { 0x227C, "cupre" },
  { 0x227D, "sccue" },
  { 0x227E, "prsim" },
  { 0x227F, "scsim" },
  { 0x2280, "npr" },
  { 0x2281, "nsc" },
  { 0x2282, "sub" },
  { 0x2283, "sup" },
  { 0x2284, "nsub" },
  { 0x2285, "nsup" },
  { 0x2286, "sube" },
  { 0x2287, "supe" },
  { 0x2288, "nsube" },
  { 0x2289, "nsupe" },
  { 0x228A, "subnE" },
  { 0x228B, "supnE" },
  { 0x228E, "uplus" },
  { 0x228F, "sqsub" },
  { 0x2290, "sqsup" },
  { 0x2291, "sqsube" },
  { 0x2292, "sqsupe" },
  { 0x2293, "sqcap" },
  { 0x2294, "sqcup" },
  { 0x2295, "oplus" },
  { 0x2296, "ominus" },
  { 0x2297, "otimes" },
  { 0x2298, "osol" },
  { 0x2299, "odot" },
  { 0x229A, "ocir" },
  { 0x229B, "oast" },
  { 0x229D, "odash" },
  { 0x229E, "plusb" },
  { 0x229F, "minusb" },
  { 0x22A0, "timesb" },
  { 0x22A1, "sdotb" },
  { 0x22A2, "vdash" },
  { 0x22A3, "dashv" },
  { 0x22A4, "top" },
  { 0x22A5, "bottom" },
  { 0x22A5, "perp" },
  { 0x22A7, "models" },
  { 0x22A8, "vDash" },
  { 0x22A9, "Vdash" },
  { 0x22AA, "Vvdash" },
  { 0x22AC, "nvdash" },
  { 0x22AD, "nvDash" },
  { 0x22AE, "nVdash" },
  { 0x22AF, "nVDash" },
  { 0x22B2, "vltri" },
  { 0x22B3, "vrtri" },
  { 0x22B4, "ltrie" },
  { 0x22B5, "rtrie" },
  { 0x22B8, "mumap" },
  { 0x22BA, "intcal" },
  { 0x22BB, "veebar" },
  { 0x22BC, "barwed" },
  { 0x22C4, "diam" },
  { 0x22C5, "sdot" },
  { 0x22C6, "sstarf" },
  { 0x22C7, "divonx" },
  { 0x22C8, "bowtie" },
  { 0x22C9, "ltimes" },
  { 0x22CA, "rtimes" },
  { 0x22CB, "lthree" },
  { 0x22CC, "rthree" },
  { 0x22CD, "bsime" },
  { 0x22CE, "cuvee" },
  { 0x22CF, "cuwed" },
  { 0x22D0, "Sub" },
  { 0x22D1, "Sup" },
  { 0x22D2, "Cap" },
  { 0x22D3, "Cup" },
  { 0x22D4, "fork" },
  { 0x22D6, "ldot" },
  { 0x22D7, "gsdot" },
  { 0x22D8, "Ll" },
  { 0x22D9, "Gg" },
  { 0x22DA, "leg" },
  { 0x22DB, "gel" },
  { 0x22DC, "els" },
  { 0x22DD, "egs" },
  { 0x22DE, "cuepr" },
  { 0x22DF, "cuesc" },
  { 0x22E0, "npre" },
  { 0x22E1, "nsce" },
  { 0x22E6, "lnsim" },
  { 0x22E7, "gnsim" },
  { 0x22E8, "prnsim" },
  { 0x22E9, "scnsim" },
  { 0x22EA, "nltri" },
  { 0x22EB, "nrtri" },
  { 0x22EC, "nltrie" },
  { 0x22ED, "nrtrie" },
  { 0x22EE, "vellip" },
  { 0x2306, "Barwed" },
  { 0x2308, "lceil" },
  { 0x2309, "rceil" },
  { 0x230A, "lfloor" },
  { 0x230B, "rfloor" },
  { 0x230C, "drcrop" },
  { 0x230D, "dlcrop" },
  { 0x230E, "urcrop" },
  { 0x230F, "ulcrop" },
  { 0x2315, "telrec" },
  { 0x2316, "target" },
  { 0x231C, "ulcorn" },
  { 0x231D, "urcorn" },
  { 0x231E, "dlcorn" },
  { 0x231F, "drcorn" },
  { 0x2322, "frown" },
  { 0x2323, "smile" },
  { 0x2329, "lang" },
  { 0x232A, "rang" },
  { 0x2423, "blank" },
  { 0x24C8, "oS" },
  { 0x2500, "boxh" },
  { 0x2502, "boxv" },
  { 0x250C, "boxdr" },
  { 0x2510, "boxdl" },
  { 0x2514, "boxur" },
  { 0x2518, "boxul" },
  { 0x251C, "boxvr" },
  { 0x2524, "boxvl" },
  { 0x252C, "boxhd" },
  { 0x2534, "boxhu" },
  { 0x253C, "boxvh" },
  { 0x2550, "boxH" },
  { 0x2551, "boxV" },
  { 0x2552, "boxdR" },
  { 0x2553, "boxDr" },
  { 0x2554, "boxDR" },
  { 0x2555, "boxdL" },
  { 0x2556, "boxDl" },
  { 0x2557, "boxDL" },
  { 0x2558, "boxuR" },
  { 0x2559, "boxUr" },
  { 0x255A, "boxUR" },
  { 0x255B, "boxuL" },
  { 0x255C, "boxUl" },
  { 0x255D, "boxUL" },
  { 0x255E, "boxvR" },
  { 0x255F, "boxVr" },
  { 0x2560, "boxVR" },
  { 0x2561, "boxvL" },
  { 0x2562, "boxVl" },
  { 0x2563, "boxVL" },
  { 0x2564, "boxHd" },
  { 0x2565, "boxhD" },
  { 0x2566, "boxHD" },
  { 0x2567, "boxHu" },
  { 0x2568, "boxhU" },
  { 0x2569, "boxHU" },
  { 0x256A, "boxvH" },
  { 0x256B, "boxVh" },
  { 0x256C, "boxVH" },
  { 0x2580, "uhblk" },
  { 0x2584, "lhblk" },
  { 0x2588, "block" },
  { 0x2591, "blk14" },
  { 0x2592, "blk12" },
  { 0x2593, "blk34" },
  { 0x25A1, "squ" },
  { 0x25A1, "square" },
  { 0x25AA, "squf" },
  { 0x25AD, "rect" },
  { 0x25AE, "marker" },
  { 0x25B3, "xutri" },
  { 0x25B4, "utrif" },
  { 0x25B5, "utri" },
  { 0x25B8, "rtrif" },
  { 0x25B9, "rtri" },
  { 0x25BD, "xdtri" },
  { 0x25BE, "dtrif" },
  { 0x25BF, "dtri" },
  { 0x25C2, "ltrif" },
  { 0x25C3, "ltri" },
  { 0x25CB, "cir" },
  { 0x25CB, "xcirc" },
  { 0x2605, "starf" },
  { 0x2606, "star" },
  { 0x260E, "phone" },
  { 0x2640, "female" },
  { 0x2642, "male" },
  { 0x2660, "spades" },
  { 0x2661, "hearts" },
  { 0x2662, "diams" },
  { 0x2663, "clubs" },
  { 0x266A, "sung" },
  { 0x266D, "flat" },
  { 0x266E, "natur" },
  { 0x266F, "sharp" },
  { 0x2713, "check" },
  { 0x2717, "cross" },
  { 0x2720, "malt" },
  { 0x2726, "lozf" },
  { 0x2727, "loz" },
  { 0x2736, "sextile" },
  };
  for (size_t i = 0; i < SIZEOF(entities); i++)
    sdataEntityNameTable_.insert(makeStringC(entities[i].name), entities[i].c);
}

void Interpreter::installNodeProperties()
{
  for (int i = 0; i < ComponentName::nIds; i++) {
    ComponentName::Id id = ComponentName::Id(i);
    nodePropertyTable_.insert(makeStringC(ComponentName::rcsName(id)), i);
    nodePropertyTable_.insert(makeStringC(ComponentName::sdqlName(id)), i);
  }
}

bool Interpreter::sdataMap(GroveString name, GroveString, GroveChar &c) const
{
  StringC tem(name.data(), name.size());
  const Char *cp = sdataEntityNameTable_.lookup(tem);
  if (cp) {
    c = *cp;
    return 1;
  }
  if (convertUnicodeCharName(tem, c))
    return 1;
  // I think this is the most thing to do.
  // At least it makes preserve-sdata work with unknown SDATA entities.
  c = defaultChar;
  return 1;
}

bool Interpreter::parseQuote(Owner<Expression> &expr)
{
  Token tok;
  Location loc;
  ELObj *obj;
  if (!parseDatum(0, obj, loc, tok))
    return 0;
  if (!getToken(allowCloseParen, tok))
    return 0;
  makePermanent(obj);
  expr = new ConstantExpression(obj, loc);
  return 1;
}

bool Interpreter::parseQuasiquote(Owner<Expression> &expr)
{
  bool spliced;
  Token tok;
  Identifier::SyntacticKey key;
  if (!parseQuasiquoteTemplate(0, 0, expr, key, tok, spliced))
    return 0;
  return getToken(allowCloseParen, tok);
}

bool Interpreter::parseQuasiquoteTemplate(unsigned level,
					  unsigned allowed,
					  Owner<Expression> &expr,
					  Identifier::SyntacticKey &key,
					  Token &tok,
					  bool &spliced)
{
  key = Identifier::notKey;
  spliced = 0;
  ELObj *obj;
  if (!parseSelfEvaluating(allowed|allowUnquote, obj, tok))
    return 0;
  switch (tok) {
  case tokenQuasiquote:
    if (!parseQuasiquoteTemplate(level + 1, 0, expr, key, tok, spliced))
      return 0;
    createQuasiquoteAbbreviation("quasiquote", expr);
    break;
  case tokenQuote:
    if (!parseQuasiquoteTemplate(level, 0, expr, key, tok, spliced))
      break;
    createQuasiquoteAbbreviation("quote", expr);
    break;
  case tokenUnquote:
  case tokenUnquoteSplicing:
    if (level == 0) {
      spliced = (tok == tokenUnquoteSplicing);
      if (!parseExpression(0, expr, key, tok))
	return 0;
    }
    else {
      Token tem;
      if (!parseQuasiquoteTemplate(level - 1, 0, expr, key, tem, spliced))
	break;
      createQuasiquoteAbbreviation(tok == tokenUnquote ? "unquote" : "unquote-splicing", expr);
    }
    break;
  case tokenOpenParen:
    {
      Location loc(in_->currentLocation());
      NCVector<Owner<Expression> > exprs(1);
      Vector<PackedBoolean> exprsSpliced;
      bool temSpliced;
      if (!parseQuasiquoteTemplate(level,
				   allowCloseParen|allowQuasiquoteKey|allowUnquoteSplicing,
				   exprs[0], key, tok, temSpliced))
	return 0;
      if (!exprs[0]) {
	switch (key) {
	case Identifier::keyQuasiquote:
	  if (!parseQuasiquoteTemplate(level + 1, 0, expr, key, tok, spliced))
	    return 0;
	  createQuasiquoteAbbreviation("quasiquotation", expr);
	  break;
	case Identifier::keyUnquoteSplicing:
	  spliced = 1;
	  // fall through
	case Identifier::keyUnquote:
	  if (level == 0) {
	    if (!parseExpression(0, expr, key, tok))
	      return 0;
	  }
	  else {
	    if (!parseQuasiquoteTemplate(level - 1, 0, expr, key, tok, temSpliced))
	      return 0;
	    createQuasiquoteAbbreviation(spliced ? "unquote-splicing" : "unquote", expr);
	    spliced = 0;
	  }
	  break;
	default:
	   expr = new ConstantExpression(makeNil(), loc);
	   return 1;
	}
	return getToken(allowCloseParen, tok);
      }
      exprsSpliced.push_back(temSpliced);
      bool improper = 0;
      for (;;) {
	Owner<Expression> tem;
	if (!parseQuasiquoteTemplate(level,
				     allowCloseParen|allowPeriod|allowUnquoteSplicing,
				     tem, key, tok, temSpliced))
	  return 0;
	if (!tem) {
	  if (tok == tokenCloseParen)
	    break;
	  exprs.resize(exprs.size() + 1);
	  improper = 1;
	  if (!parseQuasiquoteTemplate(level, 0, exprs.back(), key, tok, temSpliced))
	    return 0;
	  if (!getToken(allowCloseParen, tok))
	    return 0;
	  exprsSpliced.push_back(0);
	  break;
	}
	exprs.resize(exprs.size() + 1);
	exprs.back().swap(tem);
	exprsSpliced.push_back(temSpliced);
      }
      expr = new QuasiquoteExpression(exprs, exprsSpliced, improper, loc);
    }
    break;
  case tokenIdentifier:
    if (allowed & allowQuasiquoteKey) {
      const Identifier *ident = lookup(currentToken_);
      if (ident->syntacticKey(key)) {
	switch (key) {
	case Identifier::keyUnquoteSplicing:
	case Identifier::keyUnquote:
	case Identifier::keyQuasiquote:
	  return 1;
	default:
	  break;
	}
      }
    }
    obj = makeSymbol(currentToken_);
    // fall through
  default:
    if (obj) {
      makePermanent(obj);
      expr = new ConstantExpression(obj, in_->currentLocation());
    }
    break;
  }
  return 1;
}

void Interpreter::createQuasiquoteAbbreviation(const char *sym, Owner<Expression> &expr)
{
  Location loc(expr->location());
  NCVector<Owner<Expression> > v(2);
  v[1].swap(expr);
  v[0] = new ConstantExpression(makeSymbol(makeStringC(sym)), loc);
  Vector<PackedBoolean> spliced;
  spliced.push_back(0);
  spliced.push_back(0);
  expr = new QuasiquoteExpression(v, spliced, 0, loc);
}

bool Interpreter::parseIf(Owner<Expression> &expr)
{
  Location loc(in_->currentLocation());
  Owner<Expression> exprs[3];
  Token tok;
  for (int i = 0; i < 3; i++) {
    Identifier::SyntacticKey key;
    if (!parseExpression(0, exprs[i], key, tok))
      return 0;
  }
  if (!getToken(allowCloseParen, tok))
    return 0;
  expr = new IfExpression(exprs[0], exprs[1], exprs[2], loc);
  return 1;
}

bool Interpreter::parseCond(Owner<Expression> &expr, bool opt)
{
  Location loc(in_->currentLocation());
  Token tok;
  if (!getToken(allowOpenParen|(opt ? unsigned(allowCloseParen) : 0), tok))
    return 0;
  if (tok == tokenCloseParen) {
    expr = new CondFailExpression(loc);
    return 1;
  }
  Identifier::SyntacticKey key;
  Owner<Expression> testExpr;
  if (!parseExpression(allowKeyElse, testExpr, key, tok))
    return 0;
  if (!testExpr) {
    if (!parseExpression(0, expr, key, tok))
      return 0;
    return getToken(allowCloseParen, tok) && getToken(allowCloseParen, tok);
  }
  Owner<Expression> valExpr;
  if (!parseExpression(allowCloseParen, valExpr, key, tok))
    return 0;
  if (valExpr) {
    if (!getToken(allowCloseParen, tok))
      return 0;
  }
  Owner<Expression> elseExpr;
  if (!parseCond(elseExpr, 1))
    return 0;
  if (valExpr)
    expr = new IfExpression(testExpr, valExpr, elseExpr, loc);
  else
    expr = new OrExpression(testExpr, elseExpr, loc);
  return 1;
}

bool Interpreter::parseCase(Owner<Expression> &expr)
{
  Owner<Expression> keyExpr;
  Owner<Expression> elseClause;
  NCVector<CaseExpression::Case> cases;
  Location loc(in_->currentLocation());
  Token tok;
  Identifier::SyntacticKey key;
  if (!parseExpression(0, keyExpr, key, tok))
    return 0;
  for (;;) {
    if (!getToken(allowOpenParen
      |(cases.size() ? unsigned(allowCloseParen) : 0), tok))
      return 0;
    if (tok == tokenCloseParen)
      break;
    if (!getToken(allowOpenParen|allowIdentifier, tok))
      return 0;
    if (tok == tokenOpenParen) {
      cases.resize(cases.size() + 1);
      Location loc;
      for (;;) {
	ELObj *obj;
	if (!parseDatum(allowCloseParen, obj, loc, tok))
	  return 0;
	if (tok == tokenCloseParen)
	  break;
	makePermanent(obj);
	cases.back().datums.push_back(obj);
      }
      if (!parseExpression(0, cases.back().expr, key, tok))
	return 0;
      if (!getToken(allowCloseParen, tok))
	return 0;
    }
    else {
      const Identifier *ident = lookup(currentToken_);
      if (ident->syntacticKey(key) && key == Identifier::keyElse) {
	if (!parseExpression(0, elseClause, key, tok))
	  return 0;
	if (!getToken(allowCloseParen, tok)
	    || !getToken(allowCloseParen, tok))
	  return 0;
	break;
      }
      else {
	message(InterpreterMessages::caseElse,
		StringMessageArg(currentToken_));
	return 0;
      }
    }
  }
  expr = new CaseExpression(keyExpr, cases, elseClause, loc);
  return 1;
}

bool Interpreter::parseOr(Owner<Expression> &expr)
{
  Location loc(in_->currentLocation());
  Token tok;
  Identifier::SyntacticKey key;
  Owner<Expression> test1Expr;
  if (!parseExpression(allowCloseParen, test1Expr, key, tok))
    return 0;
  if (!test1Expr) {
    expr = new ConstantExpression(makeFalse(), loc);
    return 1;
  }
  Owner<Expression> test2Expr;
  if (!parseOr(test2Expr))
    return 0;
  expr = new OrExpression(test1Expr, test2Expr, loc);
  return 1;
}

bool Interpreter::parseAnd(Owner<Expression> &expr, bool opt)
{
  Location loc(in_->currentLocation());
  Token tok;
  Identifier::SyntacticKey key;
  Owner<Expression> testExpr;
  if (!parseExpression(allowCloseParen, testExpr, key, tok))
    return 0;
  if (!testExpr) {
    if (!opt)
      expr = new ConstantExpression(makeTrue(), loc);
    return 1;
  }
  Owner<Expression> restExpr;
  if (!parseAnd(restExpr, 1))
    return 0;
  if (!restExpr)
    testExpr.swap(expr);
  else {
    // This relies on the fact that #f is the only false value.
    Owner<Expression> falseExpr(new ConstantExpression(makeFalse(), loc));
    expr = new IfExpression(testExpr, restExpr, falseExpr, loc);
  }
  return 1;
}

bool Interpreter::parseWithMode(Owner<Expression> &expr)
{
  Location loc(in_->currentLocation());
  Token tok;
  if (!getToken(allowIdentifier|allowFalse, tok))
    return 0;
  const ProcessingMode *mode;
  if (tok == tokenFalse)
    mode = &initialProcessingMode_;
  else
    mode = lookupProcessingMode(currentToken_);
  Owner<Expression> content;
  Identifier::SyntacticKey key;
  if (!parseExpression(0, content, key, tok))
    return 0;
  if (!getToken(allowCloseParen, tok))
    return 0;
  expr = new WithModeExpression(mode, content, loc);
  return 1;
}

bool Interpreter::parseMake(Owner<Expression> &expr)
{
  Location loc(in_->currentLocation());
  Token tok;
  if (!getToken(allowIdentifier, tok))
    return 0;
  const Identifier *foc = lookup(currentToken_);
  NCVector<Owner<Expression> > exprs;
  Vector<const Identifier *> keys;
  for (;;) {
    Owner<Expression> tem;
    Identifier::SyntacticKey key;
    if (!parseExpression(allowCloseParen, tem, key, tok))
      return 0;
    if (!tem)
      break;
    // FIXME check for duplicates
    if (keys.size() == exprs.size()) {
      const Identifier *k = tem->keyword();
      if (k) {
	keys.push_back(k);
	tem.clear();
	if (!parseExpression(0, tem, key, tok))
	  return 0;
      }
    }
    exprs.resize(exprs.size() + 1);
    tem.swap(exprs.back());
  }
  expr = new MakeExpression(foc, keys, exprs, loc);
  return 1; 
}

bool Interpreter::parseStyle(Owner<Expression> &expr)
{
  Location loc(in_->currentLocation());
  NCVector<Owner<Expression> > exprs;
  Vector<const Identifier *> keys;
  for (;;) {
    Token tok;
    if (!getToken(allowKeyword|allowCloseParen, tok))
      return 0;
    if (tok == tokenCloseParen)
      break;
    keys.resize(keys.size() + 1);
    keys.back() = lookup(currentToken_);
    exprs.resize(exprs.size() + 1);
    Identifier::SyntacticKey key;
    if (!parseExpression(0, exprs.back(), key, tok))
      return 0;
  }
  expr = new StyleExpression(keys, exprs, loc);
  return 1; 
}

bool Interpreter::parseLambda(Owner<Expression> &expr)
{
  Location loc(in_->currentLocation());
  Token tok;
  if (!getToken(allowOpenParen, tok))
    return 0;
  Vector<const Identifier *> formals;
  NCVector<Owner<Expression> > inits;
  int nOptional;
  int nKey;
  bool hasRest;
  if (!parseFormals(formals, inits, nOptional, hasRest, nKey))
    return 0;
  Owner<Expression> body;
  Identifier::SyntacticKey key;
  if (!parseExpression(0, body, key, tok))
    return 0;
  if (!getToken(allowCloseParen, tok))
    return 0;
  expr = new LambdaExpression(formals, inits, nOptional, hasRest, nKey,
			      body, loc);
  return 1;
}

// The rest arg is put last.

bool Interpreter::parseFormals(Vector<const Identifier *> &formals,
			       NCVector<Owner<Expression> > &inits,
			       int &nOptional,
			       bool &hasRest,
			       int &nKey)
{
  Token tok;
  enum FormalType { required, optional, rest, key } type = required;
  unsigned allowed = (allowCloseParen|allowIdentifier
		      |allowHashOptional|allowHashRest|allowHashKey);
  int argCount[4];
  for (int i = 0; i < 4; i++)
    argCount[i] = 0;
  for (;;) {
    if (!getToken(allowed, tok))
      return 0;
    switch (tok) {
    case tokenHashOptional:
      allowed |= allowOpenParen;
      allowed &= ~allowHashOptional;
      type = optional;
      break;
    case tokenHashRest:
      allowed = allowIdentifier;
      type = rest;
      break;
    case tokenHashKey:
      allowed = (allowOpenParen|allowCloseParen|allowIdentifier);
      type = key;
      break;
    case tokenOpenParen:
      {
	if (!getToken(allowIdentifier, tok))
	  return 0;
	argCount[type]++;
	formals.push_back(lookup(currentToken_));
	inits.resize(argCount[optional] + argCount[key]);
	Identifier::SyntacticKey key;
	if (!parseExpression(0, inits.back(), key, tok))
	  return 0;
	if (!getToken(allowCloseParen, tok))
	  return 0;
      }
      break;
    case tokenIdentifier:
      {
	formals.push_back(lookup(currentToken_));
	argCount[type]++;
	if (type == rest)
	  allowed = (allowHashKey|allowCloseParen);
      }
      break;
    case tokenCloseParen:
      goto done;
    default:
      CANNOT_HAPPEN();
    }
  }
done:
  nOptional = argCount[optional];
  nKey = argCount[key];
  inits.resize(nOptional + nKey);
  hasRest = argCount[rest];
  return 1;
}

bool Interpreter::parseLet(Owner<Expression> &expr)
{
  Location loc(in_->currentLocation());
  Token tok;
  if (!getToken(allowOpenParen|allowIdentifier, tok))
    return 0;
  Vector<const Identifier *> vars;
  NCVector<Owner<Expression> > inits;
  Owner<Expression> body;
  const Identifier *name;
  if (tok == tokenOpenParen) {
    name = 0;
    if (!parseBindingsAndBody1(vars, inits, body))
      return 0;
  }
  else {
    name = lookup(currentToken_);
    if (!parseBindingsAndBody(vars, inits, body))
      return 0;
  }
  if (name) {
    // Named let
    NCVector<Owner<Expression> > loopInit(1);
    NCVector<Owner<Expression> > argsInit;
    loopInit[0] = new LambdaExpression(vars, argsInit, 0, 0, 0, body, loc);
    Vector<const Identifier *> loopFormal(1);
    loopFormal[0] = name;
    expr = new VariableExpression(name, loc);
    expr = new LetrecExpression(loopFormal, loopInit, expr, loc);
    expr = new CallExpression(expr, inits, loc);
 }
 else
   expr = new LetExpression(vars, inits, body, loc);
 return 1;
}

bool Interpreter::parseLetStar(Owner<Expression> &expr)
{
  Location loc(in_->currentLocation());
  Vector<const Identifier *> vars;
  NCVector<Owner<Expression> > inits;
  Owner<Expression> body;
  if (!parseBindingsAndBody(vars, inits, body))
    return 0;
  expr = new LetStarExpression(vars, inits, body, loc);
  return 1;
}

bool Interpreter::parseLetrec(Owner<Expression> &expr)
{
  Location loc(in_->currentLocation());
  Vector<const Identifier *> vars;
  NCVector<Owner<Expression> > inits;
  Owner<Expression> body;
  if (!parseBindingsAndBody(vars, inits, body))
    return 0;
  expr = new LetrecExpression(vars, inits, body, loc);
  return 1;
}

bool Interpreter::parseBindingsAndBody(Vector<const Identifier *> &vars,
				       NCVector<Owner<Expression> > &inits,
				       Owner<Expression> &body)
{
  Token tok;
  if (!getToken(allowOpenParen, tok))
    return 0;
  return parseBindingsAndBody1(vars, inits, body);
}

bool Interpreter::parseBindingsAndBody1(Vector<const Identifier *> &vars,
					NCVector<Owner<Expression> > &inits,
					Owner<Expression> &body)
{
  Token tok;
  Identifier::SyntacticKey key;
  for (;;) {
    if (!getToken(allowCloseParen|allowOpenParen, tok))
      return 0;
    if (tok == tokenCloseParen)
      break;
    if (!getToken(allowIdentifier, tok))
      return 0;
    vars.push_back(lookup(currentToken_));
    inits.resize(inits.size() + 1);
    if (!parseExpression(0, inits.back(), key, tok))
      return 0;
    if (!getToken(allowCloseParen, tok))
      return 0;
  }
  if (!parseExpression(0, body, key, tok))
    return 0;
  return getToken(allowCloseParen, tok);
}

bool Interpreter::parseDatum(unsigned otherAllowed,
			     ELObj *&result,
			     Location &loc,
			     Token &tok)
{
  if (!parseSelfEvaluating(otherAllowed|allowUnquote|allowUnquoteSplicing, result, tok))
    return 0;
  loc = in_->currentLocation();
  if (result)
    return 1;
  switch (tok) {
  case tokenIdentifier:
    result = makeSymbol(currentToken_);
    break;
  case tokenQuote:
    return parseAbbreviation("quote", result);
  case tokenQuasiquote:
    return parseAbbreviation("quasiquote", result);
  case tokenUnquote:
    return parseAbbreviation("unquote", result);
  case tokenUnquoteSplicing:
    return parseAbbreviation("unquote-splicing", result);
  case tokenOpenParen:
    {
      ELObj *tem;
      Location ignore;
      if (!parseDatum(allowCloseParen, tem, ignore, tok))
	return 0;
      if (!tem) {
	result = makeNil();
	break;
      }
      ELObjDynamicRoot list(*this, tem);
      PairObj *last = new (*this) PairObj(tem, 0);
      list = last;
      for (;;) {
	if (!parseDatum(allowCloseParen|allowPeriod, tem, ignore, tok))
	  return 0;
	if (!tem) {
	  if (tok == tokenCloseParen) {
	    last->setCdr(makeNil());
	    break;
	  }
	  if (!parseDatum(0, tem, ignore, tok))
	    return 0;
	  last->setCdr(tem);
	  if (!getToken(allowCloseParen, tok))
	    return 0;
	  break;
	}
	last->setCdr(tem); // to protect it
	PairObj *p = new (*this) PairObj(tem, 0);
	last->setCdr(p);
	last = p;
      }
      result = list;
    }
    break;
  default:
    break;
  }
  return 1;
}

bool Interpreter::parseSelfEvaluating(unsigned otherAllowed,
				      ELObj *&result,
				      Token &tok)
{
  if (!getToken(allowDatum|otherAllowed, tok))
    return 0;
  switch (tok) {
  case tokenTrue:
    result = makeTrue();
    break;
  case tokenFalse:
    result = makeFalse();
    break;
  case tokenString:
    result = new (*this) StringObj(currentToken_);
    break;
  case tokenKeyword:
    result = makeKeyword(currentToken_);
    break;
  case tokenChar:
    result = makeChar(currentToken_[0]);
    break;
  case tokenNumber:
    result = convertNumber(currentToken_, 1);
    break;
  case tokenGlyphId:
    result = convertAfiiGlyphId(currentToken_);
    break;
  default:
    result = 0;
    break;
  }
  return 1;
}

bool Interpreter::parseAbbreviation(const char *sym, ELObj *&result)
{
  SymbolObj *quoteSym = makeSymbol(makeStringC(sym));
  ELObj *obj;
  Location ignore;
  Token tok;
  if (!parseDatum(0, obj, ignore, tok))
    return 0;
  ELObjDynamicRoot protect(*this, obj);
  protect = new (*this) PairObj(protect, makeNil());
  result = makePair(quoteSym, protect);
  return 1;
}

ELObj *Interpreter::convertNumber(const StringC &str, bool giveError,
				  int radix)
{
  {
    if (str.size() == 0)
      goto reportError;
    size_t i = 0;
    if (str[0] == '#') {
      if (str.size() < 2)
	goto reportError;
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
	goto reportError;
      }
      i += 2;
    }
    if (i >= str.size())
      goto reportError;
    bool negative;
    if (str[i] == '-') {
      negative = 1;
      i++;
    }
    else
      negative = 0;
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
	      goto reportError;
	    return convertNumberFloat(str, giveError);
	  }
	  else
	    n = n*radix - weight;
	}
	else {
	  if (n > (LONG_MAX - weight)/radix) {
	    if (radix != 10)
	      goto reportError;
	    return convertNumberFloat(str, giveError);
	  }
	  else
	    n = n*radix + weight;
	}
	if (hadDecimalPoint)
	  exp--;
      }
      else if (c == '.' && radix == 10) {
	if (hadDecimalPoint)
	  goto reportError;
	hadDecimalPoint = 1;
      }
      else
	break;
    }
    if (!hadDigit || (radix != 10 && i < str.size()))
      goto reportError;
    if (i + 1 < str.size() && str[i] == 'e'
        && lexCategory_[str[i + 1]] != lexLetter) {
      hadDecimalPoint = 1;
      i++;
      int e;
      if (!scanSignDigits(str, i, e))
	goto reportError;
      exp += e;
    }
    if (i < str.size()) {
      int unitExp;
      Unit *unit = scanUnit(str, i, giveError, unitExp);
      if (!unit)
	goto returnError;
      if (unitExp == 1)
	return new (*this) UnresolvedLengthObj(n, exp, unit);
      else
	return convertNumberFloat(str, giveError);
    }
    if (hadDecimalPoint)
      return convertNumberFloat(str, giveError);
    return new (*this) IntegerObj(n);
  }
 reportError:
  if (giveError)
    message(InterpreterMessages::invalidNumber, StringMessageArg(str));
 returnError:
  if (giveError)
    return makeError();
  else
    return makeFalse();
}

bool Interpreter::scanSignDigits(const StringC &str, size_t &i, int &n)
{
  bool negative;
  if (i < str.size() && str[i] == '-') {
    i++;
    negative = 1;
  }
  else
    negative = 0;
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

ELObj *Interpreter::convertNumberFloat(const StringC &str, bool giveError)
{
  String<char> buf;
  for (size_t i = 0; i < str.size(); i++) {
    if (str[i] > CHAR_MAX || str[i] == '\0') {
      if (giveError) {
	message(InterpreterMessages::invalidNumber, StringMessageArg(str));
	return makeError();
      }
      else
	return makeFalse();
    }
    // 'E' is a valid exponent marker for C but not us
    if (str[i] == 'E')
      break;
    buf += char(str[i]);
  }
  buf += '\0';
  const char *endPtr;
  double val = strtod((char *)buf.data(), (char **)&endPtr);
  if (endPtr - buf.data() == str.size())
    return new (*this) RealObj(val);
  if (endPtr == buf.data()) {
    if (giveError) {
      message(InterpreterMessages::invalidNumber, StringMessageArg(str));
      return makeError();
    }
    else
      return makeFalse();
  }
  int unitExp;
  Unit *unit = scanUnit(str, endPtr - buf.data(), giveError, unitExp);
  if (!unit) {
    if (giveError)
      return makeError();
    else
      return makeFalse();
  }
  return new (*this) UnresolvedQuantityObj(val, unit, unitExp);
}

// Return 0 for error.

Unit *Interpreter::scanUnit(const StringC &str, size_t i, bool giveError,
			    int &unitExp)
{
  StringC unitName;
  while (i < str.size()) {
    if (str[i] == '-' || ('0' <= str[i] && str[i] <= '9'))
      break;
    unitName += str[i++];
  }
  if (i >= str.size())
    unitExp = 1;
  else {
    unitExp = 0;
    bool neg = 0;
    if (str[i] == '-') {
      i++;
      neg = 1;
      if (i >= str.size()) {
	if (giveError)
	  message(InterpreterMessages::invalidNumber,
		  StringMessageArg(str));
	return 0;
      }
    }
    while (i < str.size()) {
      if (str[i] < '0' || str[i] > '9') {
	if (giveError)
	  message(InterpreterMessages::invalidNumber,
		  StringMessageArg(str));
	return 0;
      }
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

bool Interpreter::getToken(unsigned allowed, Token &tok)
{
  InputSource *in = in_.pointer();
  for (;;) {
    in->startToken();
    Xchar c = in->tokenChar(*this);
    switch (c) {
    case InputSource::eE:
      if (!(allowed & allowEndOfEntity))
	return tokenRecover(allowed, tok);
      tok = tokenEndOfEntity;
      return 1;
    case '(':
      if (!(allowed & allowOpenParen))
	return tokenRecover(allowed, tok);
      tok = tokenOpenParen;
      return 1;
    case ')':
      if (!(allowed & allowCloseParen))
	return tokenRecover(allowed, tok);
      tok = tokenCloseParen;
      return 1;
    case '\'':
      if (!(allowed & allowOtherDatum))
	return tokenRecover(allowed, tok);
      tok = tokenQuote;
      return 1;
    case '`':
      if (!(allowed & allowOtherDatum))
	return tokenRecover(allowed, tok);
      tok = tokenQuasiquote;
      return 1;
    case ',':
      c = in->tokenChar(*this);
      if (c == '@') {
	if (!(allowed & allowUnquoteSplicing))
	  return tokenRecover(allowed, tok);
	tok = tokenUnquoteSplicing;
      }
      else {
	if (!(allowed & allowUnquote))
	  return tokenRecover(allowed, tok);
	tok = tokenUnquote;
	in->endToken(1);
      }
      return 1;
    case ' ':
    case '\r':
    case '\n':
    case '\t':
    case '\f':
      // whitespace
      break;
    case '#':
      c = in->tokenChar(*this);
      switch (c) {
      case 't':
	if (!(allowed & allowOtherDatum))
	  return tokenRecover(allowed, tok);
	tok = tokenTrue;
	return 1;
      case 'f':
	if (!(allowed & allowFalse))
	  return tokenRecover(allowed, tok);
	tok = tokenFalse;
	return 1;
      case '\\':
	{
	  c = in->tokenChar(*this);
	  if (c == InputSource::eE) {
	    message(InterpreterMessages::unexpectedEof);
	    if (allowed & allowEndOfEntity) {
	      tok = tokenEndOfEntity;
	      return 1;
	    }
	    return 0;
	  }
	  if (!(allowed & allowOtherDatum)) {
	    extendToken();
	    return tokenRecover(allowed, tok);
	  }
	  in->discardInitial();
	  extendToken();
      	  tok = tokenChar;
	  if (in->currentTokenLength() == 1) {
	    currentToken_.assign(in->currentTokenStart(), 1);
	  }
	  else {
	    StringC tem(in->currentTokenStart(), in->currentTokenLength());
	    currentToken_.resize(1);
	    if (!convertCharName(tem, currentToken_[0])) {
	      message(InterpreterMessages::unknownCharName,
		      StringMessageArg(tem));
	      currentToken_[0] = defaultChar;
	    }
	  }
	  return 1;
	}
      case '!':
	{
	  extendToken();
	  StringC tem(in->currentTokenStart() + 2,
	              in->currentTokenLength() - 2);
	  if (tem == makeStringC("optional")) {
	    if (!(allowed & allowHashOptional))
	      return tokenRecover(allowed, tok);
	    tok = tokenHashOptional;
	    return 1;
	  }
	  if (tem == makeStringC("key")) {
	    if (!(allowed & allowHashKey))
	      return tokenRecover(allowed, tok);
	    tok = tokenHashKey;
	    return 1;
	  }
	  if (tem == makeStringC("rest")) {
	    if (!(allowed & allowHashRest))
	      return tokenRecover(allowed, tok);
	    tok = tokenHashRest;
	    return 1;
	  }
	  message(InterpreterMessages::unknownNamedConstant,
		  StringMessageArg(tem));
	  break;
	}
      case 'b':
      case 'o':
      case 'x':
      case 'd':
	extendToken();
	if (!(allowed & allowOtherDatum))
	  return tokenRecover(allowed, tok);
	tok = tokenNumber;
	currentToken_.assign(in->currentTokenStart(),
                             in->currentTokenLength());
	return 1;
      case 'A':
	extendToken();
	if (!(allowed & allowOtherDatum))
	  return tokenRecover(allowed, tok);
	tok = tokenGlyphId;
	currentToken_.assign(in->currentTokenStart() + 2,
                             in->currentTokenLength() - 2);
	return 1;
      case InputSource::eE:
	message(InterpreterMessages::unexpectedEof);
	if (allowed & allowEndOfEntity) {
	  tok = tokenEndOfEntity;
	  return 1;
	}
	return 0;
      default:
	message(InterpreterMessages::unknownHash);
	break;  
      }
      break;
    case '"':
      if (!(allowed & allowString))
	return tokenRecover(allowed, tok);
      return scanString(allowed, tok);
    case ';':
      skipComment();
      break;
    default:
      if (c < ' ') {
	// ignore it
	message(InterpreterMessages::invalidChar);
	break;
      }
      extendToken();
      if (tokenIsNumber()) {
	if (!(allowed & allowOtherDatum))
	  return tokenRecover(allowed, tok);
	tok = tokenNumber;
	currentToken_.assign(in->currentTokenStart(),
                             in->currentTokenLength());
	return 1;
      }
      else if (in_->currentTokenEnd()[-1] == ':') {
	if (!(allowed & allowKeyword))
	  return tokenRecover(allowed, tok);
	currentToken_.assign(in->currentTokenStart(),
                             in->currentTokenLength() - 1);
	tok = tokenKeyword;
	return 1;
      }
      else if (*in->currentTokenStart() == '.'
	       && in->currentTokenLength() == 1) {
	if (!(allowed & allowPeriod))
	  return tokenRecover(allowed, tok);
	tok = tokenPeriod;
	return 1;
      }
      else {
	if (!(allowed & allowIdentifier))
	  return tokenRecover(allowed, tok);
	currentToken_.assign(in->currentTokenStart(),
                             in->currentTokenLength());
#if 0
	if (!isValidIdentifier(currentToken_))
	  message();
#endif
	tok = tokenIdentifier;
	return 1;
      }
    }
  }
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

ELObj *Interpreter::convertAfiiGlyphId(const StringC &str)
{
  unsigned long n = 0;
  for (size_t i = 0; i < str.size(); i++) {
    if (str[i] < '0' || str[i] > '9') {
      n = 0;
      break;
    }
    // FIXME check for overflow
    n = n*10 + (str[i] - '0');
  }
  if (n == 0) {
    message(InterpreterMessages::invalidAfiiGlyphId, StringMessageArg(str));
    return 0;
  }
  return new (*this) GlyphIdObj(FOTBuilder::GlyphId(afiiPublicId_, n));
}

bool Interpreter::tokenRecover(unsigned allowed, Token &tok)
{
  if (allowed == allowCloseParen) {
    in_->ungetToken();
    tok = tokenCloseParen;
    message(InterpreterMessages::missingCloseParen);
    return 1;
  }
  if (in_->currentTokenLength() == 0)
    message(InterpreterMessages::unexpectedEof);
  else
    message(InterpreterMessages::unexpectedToken,
            StringMessageArg(StringC(in_->currentTokenStart(),
	                             in_->currentTokenLength())));
  return 0;
}

bool Interpreter::convertCharName(const StringC &str, Char &c) const
{
  const Char *cp = namedCharTable_.lookup(str);
  if (cp) {
    c = *cp;
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

bool Interpreter::tokenIsNumber()
{
  switch (*in_->currentTokenStart()) {
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
    return 1;
  case '+':
  case '-':
    return in_->currentTokenLength() > 1;
  case '.':
    if (in_->currentTokenLength() == 1
        || (in_->currentTokenLength() == 3
            && in_->currentTokenStart()[1] == '.'
	    && in_->currentTokenStart()[2] == '.'))
      return 0;
    return 1;
  }
  return 0;
}

void Interpreter::extendToken()
{
  // extend to a delimiter
  InputSource *in = in_.pointer();
  size_t length = in->currentTokenLength();
  while (!isDelimiter(in->tokenChar(*this)))
    length++;
  in->endToken(length);
}

bool Interpreter::isDelimiter(Xchar c)
{
  switch (c) {
  case InputSource::eE:
  case '(':
  case ')':
  case '"':
  case ';':
  case ' ':
  case '\t':
  case '\f':
  case '\r':
  case '\n':
    return 1;
  default:
    if (c < ' ') {
       // FIXME check not added name character
       return 1;
    }
  }
  // FIXME return 1 if added white space char
  return 0;
}

bool Interpreter::scanString(unsigned allowed, Token &tok)
{
  InputSource *in = in_.pointer();
  currentToken_.resize(0);
  for (;;) {
    Xchar c = in->tokenChar(*this);
    switch (c) {
    case InputSource::eE:
      message(InterpreterMessages::unterminatedString);
      in->endToken(1);
      return 0;
    case '"':
      tok = tokenString;
      return 1;
    case '\\':
      c = in->tokenChar(*this);
      if (c == '\\' || c == '"')
	currentToken_ += c;
      else if (c == InputSource::eE)
	break;
      else {
	StringC name;
	name += c;
	while (lexCategory_[c = in->tokenChar(*this)] < lexDelimiter)
	  name += c;
	if (c != ';')
	  in->endToken(in->currentTokenLength() - 1);
	Char ch;
	if (convertCharName(name, ch))
	  currentToken_ += ch;
	else
	  message(InterpreterMessages::unknownCharName, StringMessageArg(name));
      }
      break;
    default:
      currentToken_ += c;
      break;
    }
  }
  return 0; // not reached
}

void Interpreter::skipComment()
{
  for (;;) {
    Xchar c = in_->get(*this);
    if (c == InputSource::eE || c == '\r')
      break;
  }
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
   // FIXME
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

bool Interpreter::convertLengthSpecC(ELObj *obj, const Identifier *ident,
				     const Location &loc,
				     FOTBuilder::LengthSpec &result)
{
  if (convertLengthSpec(obj, result))
    return 1;
  invalidCharacteristicValue(ident, loc);
  return 0;
}

bool Interpreter::convertOptLengthSpecC(ELObj *obj, const Identifier *ident,
					const Location &loc,
					FOTBuilder::OptLengthSpec &result)
{
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

bool Interpreter::convertIntegerC(ELObj *obj, const Identifier *ident, const Location &loc,
				  long &result)
{
  if (obj->exactIntegerValue(result))
    return 1;
  // FIXME allow inexact value
  invalidCharacteristicValue(ident, loc);
  return 0;
}

bool Interpreter::convertCharC(ELObj *obj, const Identifier *ident, const Location &loc,
			       Char &result)
{
  if (obj->charValue(result))
    return 1;
  invalidCharacteristicValue(ident, loc);
  return 0;
}

bool Interpreter::convertRealC(ELObj *obj, const Identifier *ident, const Location &loc,
			       double &result)
{
  if (obj->realValue(result))
    return 1;
  invalidCharacteristicValue(ident, loc);
  return 0;
}

bool Interpreter::convertEnumC(const FOTBuilder::Symbol *syms,  size_t nSyms,
			       ELObj *obj, const Identifier *ident, const Location &loc,
			       FOTBuilder::Symbol &result)
{
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

void Interpreter::setNodeLocation(const NodePtr &nd)
{
  const LocNode *lnp;
  Location nodeLoc;
  if ((lnp = LocNode::convert(nd)) != 0
      && lnp->getLocation(nodeLoc) == accessOK)
    setNextLocation(nodeLoc);
}

void Interpreter::dispatchMessage(Message &msg)
{
  messenger_->dispatchMessage(msg);
}

void Interpreter::dispatchMessage(const Message &msg)
{
  messenger_->dispatchMessage(msg);
}

void Interpreter::initMessage(Message &msg)
{
  if (in_)
    msg.loc = in_->currentLocation();
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

Identifier::Identifier(const StringC &name)
: Named(name), value_(0), syntacticKey_(notKey), beingComputed_(0), flowObj_(0)
{
}

void Identifier::setDefinition(Owner<Expression> &expr,
			       unsigned part,
			       const Location &loc)
{
  def_.swap(expr);
  defPart_ = part;
  defLoc_ = loc;
  value_ = 0;
}

void Identifier::setValue(ELObj *value, unsigned partIndex)
{
  value_ = value;
  // Built in functions have lowest priority.
  defPart_ = partIndex;
}

bool Identifier::defined(unsigned &part, Location &loc) const
{
  if (!def_ && !value_)
    return 0;
  part = defPart_;
  loc = defLoc_;
  return 1;
}

ELObj *Identifier::computeValue(bool force, Interpreter &interp) const
{
  if (value_)
    return value_;
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
  return value_;
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

#ifdef DSSSL_NAMESPACE
}
#endif
