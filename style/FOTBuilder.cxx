// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#include "stylelib.h"
#include "FOTBuilder.h"
#include "Owner.h"
#include "macros.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

const char *FOTBuilder::symbolName(Symbol sym)
{
  if (sym < 2 || sym >= nSymbols)
    return 0;
  // These must match the order in Symbol
  static const char *names[] = {
    "not-applicable",
    "ultra-condensed",
    "extra-condensed",
    "condensed",
    "semi-condensed",
    "ultra-light",
    "extra-light",
    "light",
    "semi-light",
    "medium",
    "semi-expanded",
    "expanded",
    "extra-expanded",
    "ultra-expanded",
    "semi-bold",
    "bold",
    "extra-bold",
    "ultra-bold",
    "upright",
    "oblique",
    "back-slanted-oblique",
    "italic",
    "back-slanted-italic",
    "start",
    "end",
    "center",
    "justify",
    "spread-inside",
    "spread-outside",
    "page-inside",
    "page-outside",
    "wrap",
    "asis",
    "asis-wrap",
    "asis-truncate",
    "none",
    "before",
    "through",
    "after",
    "top-to-bottom",
    "left-to-right",
    "bottom-to-top",
    "right-to-left",
    "inside",
    "outside",
    "horizontal",
    "vertical",
    "escapement",
    "line-progression",
    "math",
    "ordinary",
    "operator",
    "binary",
    "relation",
    "opening",
    "closing",
    "punctuation",
    "inner",
    "space",
    "page",
    "page-region",
    "column-set",
    "column",
    "max",
    "max-uniform",
    "miter",
    "round",
    "join",
    "butt",
    "square",
    "loose",
    "normal",
    "kern",
    "tight",
    "touch",
    "preserve",
    "collapse",
    "ignore",
    "relative",
    "display",
    "inline",
    "border",
    "background",
    "both",
    "base",
    "font",
    "top",
    "bottom",
    "spread",
    "solid",
    "outline",
    "with",
    "against",
    "force",
  };
  ASSERT(SIZEOF(names) == nSymbols - 2);
  return names[sym - 2];
}

SaveFOTBuilder *FOTBuilder::asSaveFOTBuilder()
{
  return 0;
}

FOTBuilder::~FOTBuilder()
{
}

void FOTBuilder::start()
{
}

void FOTBuilder::end()
{
}

void FOTBuilder::atomic()
{
  start();
  end();
}

void FOTBuilder::characters(const Char *, size_t)
{
  atomic();
}
  
void FOTBuilder::charactersFromNode(const NodePtr &, const Char *s, size_t n)
{
  characters(s, n);
}

void FOTBuilder::character(const CharacterNIC &nic)
{
  if (nic.specifiedC & CharacterNIC::cChar)
    characters(&nic.ch, 1);
}
  
void FOTBuilder::startSequence()
{
  start();
}
  
void FOTBuilder::endSequence()
{
  end();
}

void FOTBuilder::startLineField(const LineFieldNIC &)
{
  start();
}
  
void FOTBuilder::endLineField()
{
  end();
}

void FOTBuilder::paragraphBreak(const ParagraphNIC &)
{
  atomic();
}

void FOTBuilder::externalGraphic(const ExternalGraphicNIC &)
{
  atomic();
}

void FOTBuilder::rule(const RuleNIC &)
{
  atomic();
}

void FOTBuilder::alignmentPoint()
{
  atomic();
}

void FOTBuilder::formattingInstruction(const StringC &)
{
  atomic();
}

void FOTBuilder::startParagraph(const ParagraphNIC &)
{
  start();
}
  
void FOTBuilder::endParagraph()
{
  end();
}
  
void FOTBuilder::startDisplayGroup(const DisplayGroupNIC &)
{
  start();
}
  
void FOTBuilder::endDisplayGroup()
{
  end();
}
  
void FOTBuilder::startScroll()
{
  start();
}

void FOTBuilder::endScroll()
{
  end();
}

void FOTBuilder::startLink(const Address &)
{
  start();
}

void FOTBuilder::endLink()
{
  end();
}

void FOTBuilder::startMarginalia()
{
  start();
}

void FOTBuilder::endMarginalia()
{
  end();
}

void FOTBuilder::startMultiMode(const FOTBuilder::MultiMode *,
				const Vector<FOTBuilder::MultiMode> &,
      				Vector<FOTBuilder *> &)
{
  start();
}

void FOTBuilder::endMultiMode()
{
  end();
}

void FOTBuilder::startScore(Symbol)
{
  start();
}

void FOTBuilder::startScore(Char)
{
  start();
}

void FOTBuilder::startScore(const LengthSpec &)
{
  start();
}

void FOTBuilder::endScore()
{
  end();
}

void FOTBuilder::startLeader(const LeaderNIC &)
{
  start();
}

void FOTBuilder::endLeader()
{
  end();
}

void FOTBuilder::startBox(const BoxNIC &)
{
  start();
}

void FOTBuilder::endBox()
{
  end();
}

void FOTBuilder::startSideline()
{
}

void FOTBuilder::endSideline()
{
}

void FOTBuilder::startSimplePageSequence()
{
  start();
}

void FOTBuilder::endSimplePageSequence()
{
  end();
}

void FOTBuilder::startSimplePageSequenceHeaderFooter(unsigned)
{
}

void FOTBuilder::endSimplePageSequenceHeaderFooter(unsigned)
{
}

void FOTBuilder::endAllSimplePageSequenceHeaderFooter()
{
}

void FOTBuilder::pageNumber()
{
}

void FOTBuilder::startTable(const TableNIC &)
{
  start();
}

void FOTBuilder::endTable()
{
  end();
}

void FOTBuilder::tableBeforeRowBorder()
{
  atomic();
}

void FOTBuilder::tableAfterRowBorder()
{
  atomic();
}

void FOTBuilder::tableBeforeColumnBorder()
{
  atomic();
}

void FOTBuilder::tableAfterColumnBorder()
{
  atomic();
}

void FOTBuilder::startTablePart(const TablePartNIC &, FOTBuilder *&header, FOTBuilder *&footer)
{
  start();
  header = footer = this;
}

void FOTBuilder::endTablePart()
{
  end();
}

void FOTBuilder::tableColumn(const TableColumnNIC &)
{
  atomic();
}

void FOTBuilder::startTableRow()
{
  start();
}

void FOTBuilder::endTableRow()
{
  end();
}

void FOTBuilder::startTableCell(const TableCellNIC &)
{
  start();
}

void FOTBuilder::endTableCell()
{
  end();
}

void FOTBuilder::tableCellBeforeRowBorder()
{
  atomic();
}

void FOTBuilder::tableCellAfterRowBorder()
{
  atomic();
}

void FOTBuilder::tableCellBeforeColumnBorder()
{
  atomic();
}

void FOTBuilder::tableCellAfterColumnBorder()
{
  atomic();
}

void FOTBuilder::startFraction(FOTBuilder *&numerator, FOTBuilder *&denominator)
{
  start();
  numerator = denominator = this;
}

void FOTBuilder::endFraction()
{
  end();
}
 
void FOTBuilder::startElement(const ElementNIC &)
{
  start();
}

void FOTBuilder::endElement()
{
  end();
}

void FOTBuilder::emptyElement(const ElementNIC &)
{
  atomic();
}

void FOTBuilder::documentType(const DocumentTypeNIC &)
{
  atomic();
}

void FOTBuilder::processingInstruction(const StringC &)
{
  atomic();
}

void FOTBuilder::setFontSize(Length)
{
}
  
void FOTBuilder::setFontFamilyName(const StringC &)
{
}

void FOTBuilder::setFontWeight(Symbol)
{
}

void FOTBuilder::setFontPosture(Symbol)
{
}

void FOTBuilder::setStartIndent(const LengthSpec &)
{
}

void FOTBuilder::setEndIndent(const LengthSpec &)
{
}

void FOTBuilder::setFirstLineStartIndent(const LengthSpec &)
{
}

void FOTBuilder::setLastLineEndIndent(const LengthSpec &)
{
}

void FOTBuilder::setLineSpacing(const LengthSpec &)
{
}

void FOTBuilder::setFieldWidth(const LengthSpec &)
{
}

void FOTBuilder::setQuadding(Symbol)
{
}

void FOTBuilder::setDisplayAlignment(Symbol)
{
}

void FOTBuilder::setFieldAlign(Symbol)
{
}

void FOTBuilder::setLines(Symbol)
{
}

void FOTBuilder::setColor(const DeviceRGBColor &)
{
}

void FOTBuilder::setBackgroundColor(const DeviceRGBColor &)
{
}

void FOTBuilder::setBackgroundColor()
{
}

void FOTBuilder::setPageWidth(Length)
{
}

void FOTBuilder::setPageHeight(Length)
{
}

void FOTBuilder::setLeftMargin(Length)
{
}

void FOTBuilder::setRightMargin(Length)
{
}

void FOTBuilder::setTopMargin(Length)
{
}

void FOTBuilder::setBottomMargin(Length)
{
}

void FOTBuilder::setHeaderMargin(Length)
{
}

void FOTBuilder::setFooterMargin(Length)
{
}

void FOTBuilder::setBorderPresent(bool)
{
}

void FOTBuilder::setLineThickness(Length)
{
}

void FOTBuilder::setCellBeforeRowMargin(Length)
{
}

void FOTBuilder::setCellAfterRowMargin(Length)
{
}

void FOTBuilder::setCellBeforeColumnMargin(Length)
{
}

void FOTBuilder::setCellAfterColumnMargin(Length)
{
}

void FOTBuilder::setLineSep(Length)
{
}

void FOTBuilder::setBoxSizeBefore(Length)
{
}

void FOTBuilder::setBoxSizeAfter(Length)
{
}

void FOTBuilder::setPositionPointShift(const LengthSpec &)
{
}

void FOTBuilder::setStartMargin(const LengthSpec &)
{
}

void FOTBuilder::setEndMargin(const LengthSpec &)
{
}

void FOTBuilder::setSidelineSep(const LengthSpec &)
{
}

void FOTBuilder::setAsisWrapIndent(const LengthSpec &)
{
}

void FOTBuilder::setLineNumberSep(const LengthSpec &)
{
}

void FOTBuilder::setLastLineJustifyLimit(const LengthSpec &)
{
}

void FOTBuilder::setJustifyGlyphSpaceMaxAdd(const LengthSpec &)
{
}

void FOTBuilder::setJustifyGlyphSpaceMaxRemove(const LengthSpec &)
{
}

void FOTBuilder::setTableCornerRadius(const LengthSpec &)
{
}

void FOTBuilder::setBoxCornerRadius(const LengthSpec &)
{
}

void FOTBuilder::setMarginaliaSep(const LengthSpec &)
{
}

void FOTBuilder::setMinPreLineSpacing(const OptLengthSpec &)
{
}

void FOTBuilder::setMinPostLineSpacing(const OptLengthSpec &)
{
}

void FOTBuilder::setMinLeading(const OptLengthSpec &)
{
}

void FOTBuilder::setInhibitLineBreaks(bool)
{
}

void FOTBuilder::setHyphenate(bool)
{
}

void FOTBuilder::setKern(bool)
{
}

void FOTBuilder::setLigature(bool)
{
}

void FOTBuilder::setScoreSpaces(bool)
{
}

void FOTBuilder::setFloatOutMarginalia(bool)
{
}

void FOTBuilder::setFloatOutSidelines(bool)
{
}

void FOTBuilder::setFloatOutLineNumbers(bool)
{
}

void FOTBuilder::setCellBackground(bool)
{
}

void FOTBuilder::setSpanWeak(bool)
{
}

void FOTBuilder::setIgnoreRecordEnd(bool)
{
}

void FOTBuilder::setNumberedLines(bool)
{
}

void FOTBuilder::setHangingPunct(bool)
{
}

void FOTBuilder::setBoxOpenEnd(bool)
{
}

void FOTBuilder::setTruncateLeader(bool)
{
}

void FOTBuilder::setAlignLeader(bool)
{
}

void FOTBuilder::setTablePartOmitMiddleHeader(bool)
{
}

void FOTBuilder::setTablePartOmitMiddleFooter(bool)
{
}

void FOTBuilder::setBorderOmitAtBreak(bool)
{
}

void FOTBuilder::setPrincipalModeSimultaneous(bool)
{
}

void FOTBuilder::setMarginaliaKeepWithPrevious(bool)
{
}

void FOTBuilder::setLineJoin(Symbol)
{
}

void FOTBuilder::setLineCap(Symbol)
{
}

void FOTBuilder::setLineNumberSide(Symbol)
{
}

void FOTBuilder::setKernMode(Symbol)
{
}

void FOTBuilder::setInputWhitespaceTreatment(Symbol)
{
}

void FOTBuilder::setFillingDirection(Symbol)
{
}

void FOTBuilder::setWritingMode(Symbol)
{
}

void FOTBuilder::setLastLineQuadding(Symbol)
{
}

void FOTBuilder::setMathDisplayMode(Symbol)
{
}

void FOTBuilder::setBoxType(Symbol)
{
}

void FOTBuilder::setGlyphAlignmentMode(Symbol)
{
}

void FOTBuilder::setBoxBorderAlignment(Symbol)
{
}

void FOTBuilder::setCellRowAlignment(Symbol)
{
}

void FOTBuilder::setBorderAlignment(Symbol)
{
}

void FOTBuilder::setSidelineSide(Symbol)
{
}

void FOTBuilder::setHyphenationKeep(Symbol)
{
}

void FOTBuilder::setPositionPreference(Symbol)
{
}

void FOTBuilder::setFontStructure(Symbol)
{
}

void FOTBuilder::setFontProportionateWidth(Symbol)
{
}

void FOTBuilder::setCellCrossed(Symbol)
{
}

void FOTBuilder::setMarginaliaSide(Symbol)
{
}

void FOTBuilder::setLayer(long)
{
}

void FOTBuilder::setBackgroundLayer(long)
{
}

void FOTBuilder::setBorderPriority(long)
{
}

void FOTBuilder::setLineRepeat(long)
{
}

void FOTBuilder::setSpan(long)
{
}

void FOTBuilder::setMinLeaderRepeat(long)
{
}

void FOTBuilder::setHyphenationRemainCharCount(long)
{
}

void FOTBuilder::setHyphenationPushCharCount(long)
{
}

void FOTBuilder::setWidowCount(long)
{
}

void FOTBuilder::setOrphanCount(long)
{
}

void FOTBuilder::setExpandTabs(long)
{
}

void FOTBuilder::setHyphenationLadderCount(long)
{
}

void FOTBuilder::setBackgroundTile(PublicId)
{
}

void FOTBuilder::setLineBreakingMethod(PublicId)
{
}

void FOTBuilder::setLineCompositionMethod(PublicId)
{
}

void FOTBuilder::setImplicitBidiMethod(PublicId)
{
}

void FOTBuilder::setGlyphSubstMethod(PublicId)
{
}

void FOTBuilder::setGlyphReorderMethod(PublicId)
{
}

void FOTBuilder::setHyphenationMethod(PublicId)
{
}

void FOTBuilder::setTableAutoWidthMethod(PublicId)
{
}

void FOTBuilder::setFontName(PublicId)
{
}

void FOTBuilder::setLanguage(Letter2)
{
}

void FOTBuilder::setCountry(Letter2)
{
}

void FOTBuilder::setEscapementSpaceBefore(const InlineSpace &)
{
}

void FOTBuilder::setEscapementSpaceAfter(const InlineSpace &)
{
}

void FOTBuilder::setGlyphSubstTable(const Vector<ConstPtr<GlyphSubstTable> > &)
{
}

void FOTBuilder::startNode(const NodePtr &, const StringC &, RuleType)
{
}
  
void FOTBuilder::endNode()
{
}

void FOTBuilder::currentNodePageNumber(const NodePtr &)
{
}

void FOTBuilder::extensionSet(void (FOTBuilder::*func)(bool), bool arg)
{
  (this->*func)(arg);
}

void FOTBuilder::extensionSet(void (FOTBuilder::*func)(const StringC &), const StringC &arg)
{
  (this->*func)(arg);
}

void FOTBuilder::extensionSet(void (FOTBuilder::*func)(long), long arg)
{
  (this->*func)(arg);
}


FOTBuilder::DisplayNIC::DisplayNIC()
: keep(symbolFalse), breakBefore(symbolFalse), breakAfter(symbolFalse),
  keepWithPrevious(0), keepWithNext(0),
  mayViolateKeepBefore(0), mayViolateKeepAfter(0)
{
}

FOTBuilder::DisplayGroupNIC::DisplayGroupNIC()
: hasCoalesceId(0)
{
}

FOTBuilder::InlineNIC::InlineNIC()
: breakBeforePriority(0),
  breakAfterPriority(0)
{
}

FOTBuilder::ExternalGraphicNIC::ExternalGraphicNIC()
: scaleType(symbolMaxUniform),
  isDisplay(1),
  hasMaxWidth(0),
  hasMaxHeight(0),
  escapementDirection(symbolFalse)
{
}

FOTBuilder::BoxNIC::BoxNIC()
: isDisplay(0)
{
}

FOTBuilder::RuleNIC::RuleNIC()
: orientation(symbolHorizontal),
  hasLength(0)
{
}

FOTBuilder::LeaderNIC::LeaderNIC()
: hasLength(0)
{
}

FOTBuilder::CharacterNIC::CharacterNIC()
: specifiedC(0),
  stretchFactor(1.0)
{
}

FOTBuilder::TableNIC::TableNIC()
: widthType(widthFull)
{
}

FOTBuilder::TableColumnNIC::TableColumnNIC()
: columnIndex(0), nColumnsSpanned(1), hasWidth(0)
{
}

FOTBuilder::TableCellNIC::TableCellNIC()
: columnIndex(0), nColumnsSpanned(1), nRowsSpanned(1), missing(0)
{
}

FOTBuilder::MultiMode::MultiMode()
: hasDesc(0)
{
}

FOTBuilder::GlyphId FOTBuilder::GlyphSubstTable::subst(const FOTBuilder::GlyphId &gid) const
{
  for (size_t i = 0; i < pairs.size(); i += 2)
    if (gid == pairs[i])
      return pairs[i + 1];
  return gid;
}

FOTBuilder::DocumentTypeNIC::DocumentTypeNIC()
{
}

SaveFOTBuilder::SaveFOTBuilder()
: calls_(0), tail_(&calls_)
{
}

SaveFOTBuilder::SaveFOTBuilder(const NodePtr &currentNode,
			       const StringC &processingMode,
			       RuleType ruleType)
: currentNode_(currentNode),
  processingMode_(processingMode),
  ruleType_(ruleType),
  calls_(0), tail_(&calls_)
{
}

SaveFOTBuilder::~SaveFOTBuilder()
{
  *tail_ = 0;
  while (calls_) {
    Call *tem = calls_;
    calls_ = calls_->next;
    delete tem;
  }
}

SaveFOTBuilder *SaveFOTBuilder::asSaveFOTBuilder()
{
  return this;
}

void SaveFOTBuilder::emit(FOTBuilder &fotb)
{
  if (currentNode_)
    fotb.startNode(currentNode_, processingMode_, ruleType_);
  SaveFOTBuilder *save = fotb.asSaveFOTBuilder();
  if (save) {
    if (calls_) {
      *save->tail_ = calls_;
      save->tail_ = tail_;
      calls_ = 0;
      tail_ = &calls_;
    }
  }
  else {
    *tail_ = 0;
    while (calls_) {
      Call *tem = calls_;
      calls_ = calls_->next;
      tem->emit(fotb);
      delete tem;
    }
    tail_ = &calls_;
  }
  if (currentNode_)
    fotb.endNode();
}

#define NO_ARG_CALL(F) \
  void SaveFOTBuilder::F() { \
    *tail_ = new NoArgCall(&FOTBuilder::F); \
     tail_ = &(*tail_)->next; }

NO_ARG_CALL(startSequence)
NO_ARG_CALL(endSequence)
NO_ARG_CALL(endLineField)
NO_ARG_CALL(endParagraph)
NO_ARG_CALL(endDisplayGroup)
NO_ARG_CALL(alignmentPoint)
NO_ARG_CALL(startScroll)
NO_ARG_CALL(endScroll)
NO_ARG_CALL(endLeader)
NO_ARG_CALL(endLink)
NO_ARG_CALL(startMarginalia)
NO_ARG_CALL(endMarginalia)
NO_ARG_CALL(endMultiMode)
NO_ARG_CALL(endScore)
NO_ARG_CALL(endBox)
NO_ARG_CALL(startSideline)
NO_ARG_CALL(endSideline)
NO_ARG_CALL(endNode)
NO_ARG_CALL(endFraction)
NO_ARG_CALL(startSimplePageSequence)
NO_ARG_CALL(endSimplePageSequence)
NO_ARG_CALL(endAllSimplePageSequenceHeaderFooter)
NO_ARG_CALL(pageNumber)
NO_ARG_CALL(endTable)
NO_ARG_CALL(tableBeforeRowBorder)
NO_ARG_CALL(tableAfterRowBorder)
NO_ARG_CALL(tableBeforeColumnBorder)
NO_ARG_CALL(tableAfterColumnBorder)
NO_ARG_CALL(endTablePart)
NO_ARG_CALL(startTableRow)
NO_ARG_CALL(endTableRow)
NO_ARG_CALL(endTableCell)
NO_ARG_CALL(tableCellBeforeRowBorder)
NO_ARG_CALL(tableCellAfterRowBorder)
NO_ARG_CALL(tableCellBeforeColumnBorder)
NO_ARG_CALL(tableCellAfterColumnBorder)
NO_ARG_CALL(endElement)

#define LENGTH_SPEC_ARG_CALL(F) \
  void SaveFOTBuilder::F(const LengthSpec &lengthSpec) { \
    *tail_ = new LengthSpecArgCall(&FOTBuilder::F, lengthSpec); \
     tail_ = &(*tail_)->next; }

LENGTH_SPEC_ARG_CALL(setStartIndent)
LENGTH_SPEC_ARG_CALL(setEndIndent)
LENGTH_SPEC_ARG_CALL(setFirstLineStartIndent)
LENGTH_SPEC_ARG_CALL(setLastLineEndIndent)
LENGTH_SPEC_ARG_CALL(setLineSpacing)
LENGTH_SPEC_ARG_CALL(setFieldWidth)
LENGTH_SPEC_ARG_CALL(setPositionPointShift)
LENGTH_SPEC_ARG_CALL(setStartMargin)
LENGTH_SPEC_ARG_CALL(setEndMargin)
LENGTH_SPEC_ARG_CALL(setSidelineSep)
LENGTH_SPEC_ARG_CALL(setAsisWrapIndent)
LENGTH_SPEC_ARG_CALL(setLineNumberSep)
LENGTH_SPEC_ARG_CALL(setLastLineJustifyLimit)
LENGTH_SPEC_ARG_CALL(setJustifyGlyphSpaceMaxAdd)
LENGTH_SPEC_ARG_CALL(setJustifyGlyphSpaceMaxRemove)
LENGTH_SPEC_ARG_CALL(setTableCornerRadius)
LENGTH_SPEC_ARG_CALL(setBoxCornerRadius)
LENGTH_SPEC_ARG_CALL(setMarginaliaSep)

#define OPT_LENGTH_SPEC_ARG_CALL(F) \
  void SaveFOTBuilder::F(const OptLengthSpec &optLengthSpec) { \
    *tail_ = new OptLengthSpecArgCall(&FOTBuilder::F, optLengthSpec); \
     tail_ = &(*tail_)->next; }

OPT_LENGTH_SPEC_ARG_CALL(setMinPreLineSpacing)
OPT_LENGTH_SPEC_ARG_CALL(setMinPostLineSpacing)
OPT_LENGTH_SPEC_ARG_CALL(setMinLeading)

#define LONG_ARG_CALL(F) \
  void SaveFOTBuilder::F(long n) { \
    *tail_ = new LongArgCall(&FOTBuilder::F, n); \
     tail_ = &(*tail_)->next; }


LONG_ARG_CALL(setFontSize)
LONG_ARG_CALL(setPageWidth)
LONG_ARG_CALL(setPageHeight)
LONG_ARG_CALL(setLeftMargin)
LONG_ARG_CALL(setRightMargin)
LONG_ARG_CALL(setTopMargin)
LONG_ARG_CALL(setBottomMargin)
LONG_ARG_CALL(setHeaderMargin)
LONG_ARG_CALL(setFooterMargin)
LONG_ARG_CALL(setLineThickness)
LONG_ARG_CALL(setCellBeforeRowMargin)
LONG_ARG_CALL(setCellAfterRowMargin)
LONG_ARG_CALL(setCellBeforeColumnMargin)
LONG_ARG_CALL(setCellAfterColumnMargin)
LONG_ARG_CALL(setLineSep)
LONG_ARG_CALL(setBoxSizeBefore)
LONG_ARG_CALL(setBoxSizeAfter)
LONG_ARG_CALL(setLayer)
LONG_ARG_CALL(setBackgroundLayer)
LONG_ARG_CALL(setBorderPriority)
LONG_ARG_CALL(setLineRepeat)
LONG_ARG_CALL(setSpan)
LONG_ARG_CALL(setMinLeaderRepeat)
LONG_ARG_CALL(setHyphenationRemainCharCount)
LONG_ARG_CALL(setHyphenationPushCharCount)
LONG_ARG_CALL(setWidowCount)
LONG_ARG_CALL(setOrphanCount)
LONG_ARG_CALL(setExpandTabs)
LONG_ARG_CALL(setHyphenationLadderCount)

#define BOOL_ARG_CALL(F) \
  void SaveFOTBuilder::F(bool b) { \
    *tail_ = new BoolArgCall(&FOTBuilder::F, b); \
     tail_ = &(*tail_)->next; }

BOOL_ARG_CALL(setBorderPresent)
BOOL_ARG_CALL(setInhibitLineBreaks)
BOOL_ARG_CALL(setHyphenate)
BOOL_ARG_CALL(setKern)
BOOL_ARG_CALL(setLigature)
BOOL_ARG_CALL(setScoreSpaces)
BOOL_ARG_CALL(setFloatOutMarginalia)
BOOL_ARG_CALL(setFloatOutSidelines)
BOOL_ARG_CALL(setFloatOutLineNumbers)
BOOL_ARG_CALL(setCellBackground)
BOOL_ARG_CALL(setSpanWeak)
BOOL_ARG_CALL(setIgnoreRecordEnd)
BOOL_ARG_CALL(setNumberedLines)
BOOL_ARG_CALL(setHangingPunct)
BOOL_ARG_CALL(setBoxOpenEnd)
BOOL_ARG_CALL(setTruncateLeader)
BOOL_ARG_CALL(setAlignLeader)
BOOL_ARG_CALL(setTablePartOmitMiddleHeader)
BOOL_ARG_CALL(setTablePartOmitMiddleFooter)
BOOL_ARG_CALL(setBorderOmitAtBreak)
BOOL_ARG_CALL(setPrincipalModeSimultaneous)
BOOL_ARG_CALL(setMarginaliaKeepWithPrevious)

#define SYMBOL_ARG_CALL(F) \
  void SaveFOTBuilder::F(Symbol sym) { \
    *tail_ = new SymbolArgCall(&FOTBuilder::F, sym); \
     tail_ = &(*tail_)->next; }

SYMBOL_ARG_CALL(setFontWeight)
SYMBOL_ARG_CALL(setFontPosture)
SYMBOL_ARG_CALL(setLines)
SYMBOL_ARG_CALL(setQuadding)
SYMBOL_ARG_CALL(setDisplayAlignment)
SYMBOL_ARG_CALL(setFieldAlign)
SYMBOL_ARG_CALL(setLineJoin)
SYMBOL_ARG_CALL(setLineCap)
SYMBOL_ARG_CALL(setLineNumberSide)
SYMBOL_ARG_CALL(setKernMode)
SYMBOL_ARG_CALL(setInputWhitespaceTreatment)
SYMBOL_ARG_CALL(setFillingDirection)
SYMBOL_ARG_CALL(setWritingMode)
SYMBOL_ARG_CALL(setLastLineQuadding)
SYMBOL_ARG_CALL(setMathDisplayMode)
SYMBOL_ARG_CALL(setBoxType)
SYMBOL_ARG_CALL(setGlyphAlignmentMode)
SYMBOL_ARG_CALL(setBoxBorderAlignment)
SYMBOL_ARG_CALL(setCellRowAlignment)
SYMBOL_ARG_CALL(setBorderAlignment)
SYMBOL_ARG_CALL(setSidelineSide)
SYMBOL_ARG_CALL(setHyphenationKeep)
SYMBOL_ARG_CALL(setPositionPreference)
SYMBOL_ARG_CALL(setFontStructure)
SYMBOL_ARG_CALL(setFontProportionateWidth)
SYMBOL_ARG_CALL(setCellCrossed)
SYMBOL_ARG_CALL(setMarginaliaSide)

#define PUBLIC_ID_ARG_CALL(F) \
  void SaveFOTBuilder::F(PublicId pubid) { \
    *tail_ = new PublicIdArgCall(&FOTBuilder::F, pubid); \
     tail_ = &(*tail_)->next; }

PUBLIC_ID_ARG_CALL(setBackgroundTile)
PUBLIC_ID_ARG_CALL(setLineBreakingMethod)
PUBLIC_ID_ARG_CALL(setLineCompositionMethod)
PUBLIC_ID_ARG_CALL(setImplicitBidiMethod)
PUBLIC_ID_ARG_CALL(setGlyphSubstMethod)
PUBLIC_ID_ARG_CALL(setGlyphReorderMethod)
PUBLIC_ID_ARG_CALL(setHyphenationMethod)
PUBLIC_ID_ARG_CALL(setTableAutoWidthMethod)
PUBLIC_ID_ARG_CALL(setFontName)

#define UNSIGNED_ARG_CALL(F) \
  void SaveFOTBuilder::F(unsigned n) { \
    *tail_ = new UnsignedArgCall(&FOTBuilder::F, n); \
     tail_ = &(*tail_)->next; }

UNSIGNED_ARG_CALL(setLanguage)
UNSIGNED_ARG_CALL(setCountry)
UNSIGNED_ARG_CALL(startSimplePageSequenceHeaderFooter)
UNSIGNED_ARG_CALL(endSimplePageSequenceHeaderFooter)

#define STRING_ARG_CALL(F) \
  void SaveFOTBuilder::F(const StringC &str) { \
    *tail_ = new StringArgCall(&FOTBuilder::F, str); \
     tail_ = &(*tail_)->next; }


STRING_ARG_CALL(setFontFamilyName)
STRING_ARG_CALL(formattingInstruction)
STRING_ARG_CALL(processingInstruction)

#define INLINE_SPACE_ARG_CALL(F) \
  void SaveFOTBuilder::F(const InlineSpace &is) { \
    *tail_ = new InlineSpaceArgCall(&FOTBuilder::F, is); \
     tail_ = &(*tail_)->next; }

INLINE_SPACE_ARG_CALL(setEscapementSpaceBefore)
INLINE_SPACE_ARG_CALL(setEscapementSpaceAfter)

SaveFOTBuilder::Call::~Call()
{
}

void SaveFOTBuilder::NoArgCall::emit(FOTBuilder &fotb)
{
  (fotb.*func)();
}

void SaveFOTBuilder::LongArgCall::emit(FOTBuilder &fotb)
{
  (fotb.*func)(arg);
}

void SaveFOTBuilder::ExtensionLongArgCall::emit(FOTBuilder &fotb)
{
  fotb.extensionSet(func, arg);
}

void SaveFOTBuilder::BoolArgCall::emit(FOTBuilder &fotb)
{
  (fotb.*func)(arg);
}

void SaveFOTBuilder::ExtensionBoolArgCall::emit(FOTBuilder &fotb)
{
  fotb.extensionSet(func, arg);
}

void SaveFOTBuilder::SymbolArgCall::emit(FOTBuilder &fotb)
{
  (fotb.*func)(arg);
}

void SaveFOTBuilder::PublicIdArgCall::emit(FOTBuilder &fotb)
{
  (fotb.*func)(arg);
}

void SaveFOTBuilder::UnsignedArgCall::emit(FOTBuilder &fotb)
{
  (fotb.*func)(arg);
}

void SaveFOTBuilder::InlineSpaceArgCall::emit(FOTBuilder &fotb)
{
  (fotb.*func)(arg);
}

void SaveFOTBuilder::StringArgCall::emit(FOTBuilder &fotb)
{
  (fotb.*func)(arg);
}

void SaveFOTBuilder::ExtensionStringArgCall::emit(FOTBuilder &fotb)
{
  fotb.extensionSet(func, arg);
}

void SaveFOTBuilder::CharArgCall::emit(FOTBuilder &fotb)
{
  (fotb.*func)(arg);
}

void SaveFOTBuilder::LengthSpecArgCall::emit(FOTBuilder &fotb)
{
  (fotb.*func)(arg);
}

void SaveFOTBuilder::OptLengthSpecArgCall::emit(FOTBuilder &fotb)
{
  (fotb.*func)(arg);
}

void SaveFOTBuilder::NodePtrArgCall::emit(FOTBuilder &fotb)
{
  (fotb.*func)(arg);
}

void SaveFOTBuilder::DeviceRGBColorArgCall::emit(FOTBuilder &fotb)
{
  (fotb.*func)(arg);
}

void SaveFOTBuilder::charactersFromNode(const NodePtr &node, const Char *s, size_t n)
{
  *tail_ = new CharactersFromNodeCall(node, s, n);
  tail_ = &(*tail_)->next;
}

SaveFOTBuilder::CharactersFromNodeCall::CharactersFromNodeCall(const NodePtr &nd,
							       const Char *s, size_t n)
  : data(s), size(n), node(nd)
{
}

void SaveFOTBuilder::CharactersFromNodeCall::emit(FOTBuilder &fotb)
{
  fotb.charactersFromNode(node, data, size);
}

void SaveFOTBuilder::characters(const Char *s, size_t n)
{
  *tail_ = new CharactersCall(s, n);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::CharacterCall::emit(FOTBuilder &fotb)
{
  fotb.character(arg);
}

void SaveFOTBuilder::character(const CharacterNIC &nic)
{
  *tail_ = new CharacterCall(nic);
  tail_ = &(*tail_)->next;
}

SaveFOTBuilder::StartNodeCall::StartNodeCall(const NodePtr &nd,
					     const StringC &m,
					     RuleType r)
: node(nd), mode(m), rule(r)
{
}

void SaveFOTBuilder::StartNodeCall::emit(FOTBuilder &fotb)
{
  fotb.startNode(node, mode, rule);
}

void SaveFOTBuilder::startNode(const NodePtr &node, const StringC &mode,
			       RuleType rule)
{
  *tail_ = new StartNodeCall(node, mode, rule);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::currentNodePageNumber(const NodePtr &node)
{
  *tail_ = new NodePtrArgCall(&FOTBuilder::currentNodePageNumber, node);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::setColor(const DeviceRGBColor &color)
{
  *tail_ = new DeviceRGBColorArgCall(&FOTBuilder::setColor, color);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::setBackgroundColor(const DeviceRGBColor &color)
{
  DeviceRGBColorArgCall::FuncPtr f = &FOTBuilder::setBackgroundColor;
  *tail_ = new DeviceRGBColorArgCall(f, color);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::setBackgroundColor()
{
  NoArgCall::FuncPtr f = &FOTBuilder::setBackgroundColor;
  *tail_ = new NoArgCall(f);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::startParagraph(const ParagraphNIC &nic)

{
  *tail_ = new StartParagraphCall(nic);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::paragraphBreak(const ParagraphNIC &nic)

{
  *tail_ = new ParagraphBreakCall(nic);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::startDisplayGroup(const DisplayGroupNIC &nic)
{
  *tail_ = new StartDisplayGroupCall(nic);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::externalGraphic(const ExternalGraphicNIC &nic)
{
  *tail_ = new ExternalGraphicCall(nic);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::rule(const RuleNIC &nic)
{
  *tail_ = new RuleCall(nic);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::startLeader(const LeaderNIC &nic)
{
  *tail_ = new StartLeaderCall(nic);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::startLink(const Address &addr)
{
  *tail_ = new StartLinkCall(addr);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::startScore(Symbol type)
{
  SymbolArgCall::FuncPtr func = &FOTBuilder::startScore;
  *tail_ = new SymbolArgCall(func, type);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::startScore(Char c)
{
  CharArgCall::FuncPtr func = &FOTBuilder::startScore;
  *tail_ = new CharArgCall(func, c);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::startScore(const LengthSpec &len)
{
  LengthSpecArgCall::FuncPtr func = &FOTBuilder::startScore;
  *tail_ = new LengthSpecArgCall(func, len);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::startLineField(const LineFieldNIC &nic)
{
  *tail_ = new StartLineFieldCall(nic);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::startBox(const BoxNIC &nic)
{
  *tail_ = new StartBoxCall(nic);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::startTable(const TableNIC &nic)
{
  *tail_ = new StartTableCall(nic);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::startTablePart(const TablePartNIC &nic, FOTBuilder *&header, FOTBuilder *&footer)
{
  *tail_ = new StartTablePartCall(nic, header, footer);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::tableColumn(const TableColumnNIC &nic)
{
  *tail_ = new TableColumnCall(nic);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::startTableCell(const TableCellNIC &nic)
{
  *tail_ = new StartTableCellCall(nic);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::startFraction(FOTBuilder *&numerator, FOTBuilder *&denominator)
{
  *tail_ = new StartFractionCall(numerator, denominator);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::startMultiMode(const MultiMode *principalMode,
				    const Vector<MultiMode> &namedModes,
				    Vector<FOTBuilder *> &namedPorts)
{
  *tail_ = new StartMultiModeCall(principalMode, namedModes, namedPorts);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::startElement(const ElementNIC &nic)
{
  *tail_ = new StartElementCall(nic);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::emptyElement(const ElementNIC &nic)
{
  *tail_ = new EmptyElementCall(nic);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::documentType(const DocumentTypeNIC &nic)
{
  *tail_ = new DocumentTypeCall(nic);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::setGlyphSubstTable(const Vector<ConstPtr<GlyphSubstTable> > &tables)
{
  *tail_ = new SetGlyphSubstTableCall(tables);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::extensionSet(void (FOTBuilder::*func)(bool), bool arg)
{
  *tail_ = new ExtensionBoolArgCall(func, arg);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::extensionSet(void (FOTBuilder::*func)(const StringC &), const StringC &arg)
{
  *tail_ = new ExtensionStringArgCall(func, arg);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::extensionSet(void (FOTBuilder::*func)(long), long arg)
{
  *tail_ = new ExtensionLongArgCall(func, arg);
  tail_ = &(*tail_)->next;
}


SaveFOTBuilder::CharactersCall::CharactersCall(const Char *s, size_t n)
: str(s, n)
{
}

void SaveFOTBuilder::CharactersCall::emit(FOTBuilder &fotb)
{
  fotb.characters(str.data(), str.size());
}

StartFractionCall::StartFractionCall(FOTBuilder *&n, FOTBuilder *&d)
{
  n = &numerator;
  d = &denominator;
}

void StartFractionCall::emit(FOTBuilder &fotb)
{
  FOTBuilder *n, *d;
  fotb.startFraction(n, d);
  numerator.emit(*n);
  denominator.emit(*d);
}

StartMultiModeCall::StartMultiModeCall(const FOTBuilder::MultiMode *pm,
				       const Vector<FOTBuilder::MultiMode> &nm,
				       Vector<FOTBuilder *> &v)
: namedModes(nm)
{
  if (pm) {
    hasPrincipalMode = 1;
    principalMode = *pm;
  }
  else
    hasPrincipalMode = 0;
  for (size_t i = v.size(); i > 0; i--) {
    ports.insert(new SaveFOTBuilder);
    v[i - 1] = ports.head();
  }
}

void StartMultiModeCall::emit(FOTBuilder &fotb)
{
  Vector<FOTBuilder *> v(namedModes.size());
  fotb.startMultiMode(hasPrincipalMode ? &principalMode : 0, namedModes, v);
  for (size_t i = 0; i < v.size(); i++) {
    Owner<SaveFOTBuilder> tem = ports.get();
    tem->emit(*v[i]);
  }
}

void SaveFOTBuilder::StartTableCall::emit(FOTBuilder &fotb)
{
  fotb.startTable(arg);
}

void SaveFOTBuilder::TableColumnCall::emit(FOTBuilder &fotb)
{
  fotb.tableColumn(arg);
}

void SaveFOTBuilder::StartTableCellCall::emit(FOTBuilder &fotb)
{
  fotb.startTableCell(arg);
}

StartTablePartCall::StartTablePartCall(const SaveFOTBuilder::TablePartNIC &nic,
				       FOTBuilder *&h, FOTBuilder *&f)
: arg(nic)
{
  h = &header;
  f = &footer;
}

void StartTablePartCall::emit(FOTBuilder &fotb)
{
  FOTBuilder *h, *f;
  fotb.startTablePart(arg, h, f);
  header.emit(*h);
  footer.emit(*f);
}

SaveFOTBuilder::StartParagraphCall::StartParagraphCall(const ParagraphNIC &nic)
: arg(nic)
{
}

void SaveFOTBuilder::StartParagraphCall::emit(FOTBuilder &fotb)
{
  fotb.startParagraph(arg);
}

SaveFOTBuilder::ParagraphBreakCall::ParagraphBreakCall(const ParagraphNIC &nic)
: arg(nic)
{
}

void SaveFOTBuilder::ParagraphBreakCall::emit(FOTBuilder &fotb)
{
  fotb.paragraphBreak(arg);
}

SaveFOTBuilder
::StartDisplayGroupCall::StartDisplayGroupCall(const DisplayGroupNIC &nic)
: arg(nic)
{
}

void SaveFOTBuilder::StartDisplayGroupCall::emit(FOTBuilder &fotb)
{
  fotb.startDisplayGroup(arg);
}

void SaveFOTBuilder::ExternalGraphicCall::emit(FOTBuilder &fotb)
{
  fotb.externalGraphic(arg);
}

void SaveFOTBuilder::RuleCall::emit(FOTBuilder &fotb)
{
  fotb.rule(arg);
}

void SaveFOTBuilder::StartLeaderCall::emit(FOTBuilder &fotb)
{
  fotb.startLeader(arg);
}

void SaveFOTBuilder::StartBoxCall::emit(FOTBuilder &fotb)
{
  fotb.startBox(arg);
}

void SaveFOTBuilder::StartLineFieldCall::emit(FOTBuilder &fotb)
{
  fotb.startLineField(arg);
}

void SaveFOTBuilder::StartLinkCall::emit(FOTBuilder &fotb)
{
  fotb.startLink(arg);
}

void SaveFOTBuilder::SetGlyphSubstTableCall::emit(FOTBuilder &fotb)
{
  fotb.setGlyphSubstTable(arg);
}

void SaveFOTBuilder::StartElementCall::emit(FOTBuilder &fotb)
{
  fotb.startElement(arg);
}

void SaveFOTBuilder::EmptyElementCall::emit(FOTBuilder &fotb)
{
  fotb.emptyElement(arg);
}

void SaveFOTBuilder::DocumentTypeCall::emit(FOTBuilder &fotb)
{
  fotb.documentType(arg);
}

SerialFOTBuilder::SerialFOTBuilder()
{
}

void SerialFOTBuilder::startFraction(FOTBuilder *&numerator, FOTBuilder *&denominator)
{
  numerator = this;
  save_.insert(new SaveFOTBuilder);
  denominator = save_.head();
  startFractionSerial();
  startFractionNumerator();
}

void SerialFOTBuilder::endFraction()
{
  endFractionNumerator();
  Owner<SaveFOTBuilder> denominator = save_.get();
  startFractionDenominator();
  denominator->emit(*this);
  endFractionDenominator();
  endFractionSerial();
}

void SerialFOTBuilder::startFractionSerial()
{
  start();
}

void SerialFOTBuilder::endFractionSerial()
{
  end();
}

void SerialFOTBuilder::startFractionNumerator()
{
}

void SerialFOTBuilder::endFractionNumerator()
{
}

void SerialFOTBuilder::startFractionDenominator()
{
}

void SerialFOTBuilder::endFractionDenominator()
{
}

void SerialFOTBuilder::startTablePart(const FOTBuilder::TablePartNIC &nic,
				      FOTBuilder *&header, FOTBuilder *&footer)
{
  save_.insert(new SaveFOTBuilder);
  footer = save_.head();
  save_.insert(new SaveFOTBuilder);
  header = save_.head();
  startTablePartSerial(nic);
}

void SerialFOTBuilder::endTablePart()
{
  Owner<SaveFOTBuilder> header = save_.get();
  startTablePartHeader();
  header->emit(*this);
  endTablePartHeader();
  Owner<SaveFOTBuilder> footer = save_.get();
  startTablePartFooter();
  footer->emit(*this);
  endTablePartFooter();
  endTablePartSerial();
}

void SerialFOTBuilder::startTablePartSerial(const TablePartNIC &)
{
  start();
}

void SerialFOTBuilder::endTablePartSerial()
{
  end();
}

void SerialFOTBuilder::startTablePartHeader()
{
}

void SerialFOTBuilder::endTablePartHeader()
{
}

void SerialFOTBuilder::startTablePartFooter()
{
}

void SerialFOTBuilder::endTablePartFooter()
{
}

void SerialFOTBuilder::startMultiMode(const MultiMode *principalMode,
				      const Vector<MultiMode> &namedModes,
				      Vector<FOTBuilder *> &namedPorts)
{
  for (size_t i = namedModes.size(); i > 0; i--) {
    save_.insert(new SaveFOTBuilder);
    namedPorts[i - 1] = save_.head();
  }
  multiModeStack_.push_back(namedModes);
  startMultiModeSerial(principalMode);
}

void SerialFOTBuilder::endMultiMode()
{
  const Vector<MultiMode> &namedModes = multiModeStack_.back();
  for (size_t i = 0; i < namedModes.size(); i++) {
    Owner<SaveFOTBuilder> mode = save_.get();
    startMultiModeMode(namedModes[i]);
    mode->emit(*this);
    endMultiModeMode();
  }
  endMultiModeSerial();
  multiModeStack_.resize(multiModeStack_.size() - 1);
}

void SerialFOTBuilder::startMultiModeSerial(const MultiMode *)
{
  start();
}

void SerialFOTBuilder::endMultiModeSerial()
{
  end();
}

void SerialFOTBuilder::startMultiModeMode(const MultiMode &)
{
}

void SerialFOTBuilder::endMultiModeMode()
{
}

#ifdef DSSSL_NAMESPACE
}
#endif
