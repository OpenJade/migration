// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#include "stylelib.h"
#include "FOTBuilder.h"
#include <OpenSP/Owner.h>
#include <OpenSP/macros.h>
#include <OpenSP/IListIter.h>

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

const char *FOTBuilder::symbolName(Symbol sym)
{
  if (sym < 2 || sym >= int(nSymbols))
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
    "initial",
    "final",
    "wrap",
    "asis",
    "asis-wrap",
    "asis-truncate",
    "none",
    "indent",
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
    "independent",
    "pile",
    "sup-out",
    "sub-out",
    "lead-edge",
    "trail-edge",
    "explicit",
    "row-major",
    "column-major",
    "uppercase",
    "lowercase",
    "capitalize",
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
}
  
void FOTBuilder::characters(const Vector<CharacterNIC> &v)
{
  // This is really slow. A FOTBuilder should either override this or
  // set Description::wantCharPropertyNICs=false.
  Char s[v.size()];
  size_t j(0);
  for (size_t i = 0; i< v.size(); ++i)
    if (v[i].specifiedC & (1 << CharacterNIC::cChar))
      s[j++] = v[i].ch;
  characters(s, j);
}

void FOTBuilder::charactersFromNode(const NodePtr &, const Char *s, size_t n)
{
  characters(s, n);
}

void FOTBuilder::character(const CharacterNIC &nic)
{
  if (nic.specifiedC & FOTBuilder::CharacterNIC::cChar)
    characters(&nic.ch, 1);
  atomic();
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
      				Vector<FOTBuilder *> &ports)
{
  start();
  for (size_t i = 0; i < ports.size(); i++)
    ports[i] = this;
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

void FOTBuilder::startSideBySide(const DisplayNIC&)
{
  start();
}

void FOTBuilder::endSideBySide()
{
  end();
}
  
void FOTBuilder::startSideBySideItem()
{
  start();
}

void FOTBuilder::endSideBySideItem()
{
  end();
}

void FOTBuilder::startSideline()
{
}

void FOTBuilder::endSideline()
{
}

void FOTBuilder::startSimplePageSequence(FOTBuilder* headerFooter[nHF])
{
  for ( unsigned i = 0; i < nHF; ++i )
    headerFooter[i] = this;
  start();
}

void FOTBuilder::endSimplePageSequenceHeaderFooter()
{
}

void FOTBuilder::endSimplePageSequence()
{
  end();
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

void FOTBuilder::startMathSequence()
{
  start();
}

void FOTBuilder::endMathSequence()
{
  end();
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

void FOTBuilder::fractionBar()
{
  atomic();
}

void FOTBuilder::startUnmath()
{
  start();
}

void FOTBuilder::endUnmath()
{
  end();
}

void FOTBuilder::startSuperscript()
{
  start();
}

void FOTBuilder::endSuperscript()
{
  end();
}

void FOTBuilder::startSubscript()
{
  start();
}

void FOTBuilder::endSubscript()
{
  end();
}

void FOTBuilder::startScript(FOTBuilder *&preSup,
			     FOTBuilder *&preSub,
			     FOTBuilder *&postSup,
			     FOTBuilder *&postSub,
			     FOTBuilder *&midSup,
			     FOTBuilder *&midSub)
{
  start();
  preSup = preSub = postSup = postSub = midSup = midSub = this;
}

void FOTBuilder::endScript()
{
  end();
}

void FOTBuilder::startMark(FOTBuilder *&overMark, FOTBuilder *&underMark)
{
  start();
  overMark = underMark = this;
}

void FOTBuilder::endMark()
{
  end();
}

void FOTBuilder::startFence(FOTBuilder *&open, FOTBuilder *&close)
{
  start();
  open = close = this;
}

void FOTBuilder::endFence()
{
  end();
}

void FOTBuilder::startRadical(FOTBuilder *&degree)
{
  start();
  degree = this;
}

void FOTBuilder::endRadical()
{
  end();
}

void FOTBuilder::radicalRadical(const CharacterNIC &)
{
  atomic();
}

void FOTBuilder::radicalRadicalDefaulted()
{
}

void FOTBuilder::startMathOperator(FOTBuilder *&oper,
				   FOTBuilder *&lowerLimit,
				   FOTBuilder *&upperLimit)
{
  start();
  oper = lowerLimit = upperLimit = this;
}

void FOTBuilder::endMathOperator()
{
  end();
}

void FOTBuilder::startGrid(const GridNIC &)
{
  start();
}

void FOTBuilder::endGrid()
{
  end();
}

void FOTBuilder::startGridCell(const GridCellNIC &)
{
  start();
}

void FOTBuilder::endGridCell()
{
  end();
}

void FOTBuilder::extension(const ExtensionFlowObj &, const NodePtr &)
{
  atomic();
}
 
void FOTBuilder::startExtension(const CompoundExtensionFlowObj &,
				const NodePtr &,
				Vector<FOTBuilder *> &ports)
{
  for (size_t i = 0; i < ports.size(); i++)
    ports[i] = this;
  start();
}

void FOTBuilder::endExtension(const CompoundExtensionFlowObj &)
{
  end();
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

void FOTBuilder::setSideBySideOverlapControl(Symbol)
{
}

void FOTBuilder::setSideBySidePreAlign(Symbol)
{
}

void FOTBuilder::setSideBySidePostAlign(Symbol)
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

void FOTBuilder::setGridEquidistantRows(bool)
{
}
 
void FOTBuilder::setGridEquidistantColumns(bool)
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

void FOTBuilder::setScriptPreAlign(Symbol)
{
}

void FOTBuilder::setScriptPostAlign(Symbol)
{
}

void FOTBuilder::setScriptMidSupAlign(Symbol)
{
}

void FOTBuilder::setScriptMidSubAlign(Symbol)
{
}

void FOTBuilder::setNumeratorAlign(Symbol)
{
}

void FOTBuilder::setDenominatorAlign(Symbol)
{
}

void FOTBuilder::setGridPositionCellType(Symbol)
{
}

void FOTBuilder::setGridColumnAlignment(Symbol)
{
}

void FOTBuilder::setGridRowAlignment(Symbol)
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

void FOTBuilder::setInlineSpaceSpace(const OptInlineSpace &)
{
}

void FOTBuilder::setCharMap(Symbol)
{
}

void FOTBuilder::setGlyphSubstTable(const Vector<ConstPtr<GlyphSubstTable> > &)
{
}

void FOTBuilder::startNode(const NodePtr &, const StringC &)
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
: positionPreference(symbolFalse),
  keep(symbolFalse),
  breakBefore(symbolFalse),
  breakAfter(symbolFalse),
  keepWithPrevious(0),
  keepWithNext(0),
  mayViolateKeepBefore(0),
  mayViolateKeepAfter(0)
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
  isDisplay(0),
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
: valid(0), specifiedC(0), stretchFactor(1.0)
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

FOTBuilder::GridNIC::GridNIC()
: nColumns(0), nRows(0)
{
}

FOTBuilder::GridCellNIC::GridCellNIC()
: columnNumber(0), rowNumber(0)
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

SaveFOTBuilder::SaveFOTBuilder()
: calls_(0), tail_(&calls_)
{
}

SaveFOTBuilder::SaveFOTBuilder(const SaveFOTBuilder& other)
: calls_(0), tail_(&calls_)
{
  for (Call* call = other.calls_; call; call = call->next) {
    *tail_ = call->clone();
    tail_ = &(*tail_)->next;
  }
}

NodeSaveFOTBuilder::NodeSaveFOTBuilder(const NodePtr &currentNode,
			       const StringC &processingMode)
: currentNode_(currentNode),
  processingMode_(processingMode)
{
}

void
NodeSaveFOTBuilder::emit(FOTBuilder& fotb) const
{
  if (currentNode_)
    fotb.startNode(currentNode_, processingMode_);
  SaveFOTBuilder::emit(fotb);
  if (currentNode_)
    fotb.endNode();
}

SaveFOTBuilder::~SaveFOTBuilder()
{
  clear();
}

SaveFOTBuilder&
SaveFOTBuilder::operator=(const SaveFOTBuilder& other)
{
  if (this != &other) {
    clear();
    for (Call* call = other.calls_; call; call = call->next) {
      *tail_ = call->clone();
      tail_ = &(*tail_)->next;
    }
  }
  return *this;
}

void SaveFOTBuilder::clear()
{
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

void SaveFOTBuilder::emit(FOTBuilder &fotb) const
{
#if 0
  SaveFOTBuilder *save = fotb.asSaveFOTBuilder();
  if (save) {
    if (calls_) {
      *save->tail_ = calls_;
      save->tail_ = tail_;
      calls_ = 0;
      tail_ = &calls_;
    }
  }
  else 
#endif
  {
    for (const Call* tem = calls_; tem; tem = tem->next) 
      tem->emit(fotb);
  }
}

#define NO_ARG_CALL(F) \
  void SaveFOTBuilder::F() { \
    *tail_ = new NoArgCall(&FOTBuilder::F); \
     tail_ = &(*tail_)->next; }

#define ONEARG_CALL(F, T) \
  void SaveFOTBuilder::F(T a) { \
    *tail_ = new OneArgCall<T>(&FOTBuilder::F, a); \
     tail_ = &(*tail_)->next; }

#define ONEREFARG_CALL(F, T) \
  void SaveFOTBuilder::F(const T& a) { \
    *tail_ = new OneRefArgCall<T>(&FOTBuilder::F, a); \
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
NO_ARG_CALL(endSideBySide);
NO_ARG_CALL(startSideBySideItem);
NO_ARG_CALL(endSideBySideItem);
NO_ARG_CALL(startSideline)
NO_ARG_CALL(endSideline)
NO_ARG_CALL(endNode)
NO_ARG_CALL(endSimplePageSequenceHeaderFooter)
NO_ARG_CALL(endSimplePageSequence)
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
NO_ARG_CALL(startMathSequence)
NO_ARG_CALL(endMathSequence)
NO_ARG_CALL(endFraction)
NO_ARG_CALL(fractionBar)
NO_ARG_CALL(startUnmath)
NO_ARG_CALL(endUnmath)
NO_ARG_CALL(startSuperscript)
NO_ARG_CALL(endSuperscript)
NO_ARG_CALL(startSubscript)
NO_ARG_CALL(endSubscript)
NO_ARG_CALL(endScript)
NO_ARG_CALL(endMark)
NO_ARG_CALL(endFence)
NO_ARG_CALL(endRadical)
NO_ARG_CALL(radicalRadicalDefaulted)
NO_ARG_CALL(endMathOperator)
NO_ARG_CALL(endGrid)
NO_ARG_CALL(endGridCell)

#define LENGTH_SPEC_ARG_CALL(F) ONEREFARG_CALL(F, LengthSpec)

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

#define OPT_LENGTH_SPEC_ARG_CALL(F) ONEREFARG_CALL(F, OptLengthSpec)

OPT_LENGTH_SPEC_ARG_CALL(setMinPreLineSpacing)
OPT_LENGTH_SPEC_ARG_CALL(setMinPostLineSpacing)
OPT_LENGTH_SPEC_ARG_CALL(setMinLeading)

#define LONG_ARG_CALL(F) ONEARG_CALL(F, long)

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

#define BOOL_ARG_CALL(F) ONEARG_CALL(F, bool)

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
BOOL_ARG_CALL(setGridEquidistantRows)
BOOL_ARG_CALL(setGridEquidistantColumns)

#define SYMBOL_ARG_CALL(F) ONEARG_CALL(F, Symbol)

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
SYMBOL_ARG_CALL(setScriptPreAlign)
SYMBOL_ARG_CALL(setScriptPostAlign)
SYMBOL_ARG_CALL(setScriptMidSupAlign)
SYMBOL_ARG_CALL(setScriptMidSubAlign)
SYMBOL_ARG_CALL(setNumeratorAlign)
SYMBOL_ARG_CALL(setDenominatorAlign)
SYMBOL_ARG_CALL(setGridPositionCellType)
SYMBOL_ARG_CALL(setGridColumnAlignment)
SYMBOL_ARG_CALL(setGridRowAlignment)
SYMBOL_ARG_CALL(setBoxType)
SYMBOL_ARG_CALL(setSideBySideOverlapControl)
SYMBOL_ARG_CALL(setSideBySidePreAlign)
SYMBOL_ARG_CALL(setSideBySidePostAlign)
SYMBOL_ARG_CALL(setGlyphAlignmentMode)
SYMBOL_ARG_CALL(setBoxBorderAlignment)
SYMBOL_ARG_CALL(setCellRowAlignment)
SYMBOL_ARG_CALL(setBorderAlignment)
SYMBOL_ARG_CALL(setSidelineSide)
SYMBOL_ARG_CALL(setHyphenationKeep)
SYMBOL_ARG_CALL(setFontStructure)
SYMBOL_ARG_CALL(setFontProportionateWidth)
SYMBOL_ARG_CALL(setCellCrossed)
SYMBOL_ARG_CALL(setMarginaliaSide)

#define PUBLIC_ID_ARG_CALL(F) ONEARG_CALL(F, PublicId)

PUBLIC_ID_ARG_CALL(setBackgroundTile)
PUBLIC_ID_ARG_CALL(setLineBreakingMethod)
PUBLIC_ID_ARG_CALL(setLineCompositionMethod)
PUBLIC_ID_ARG_CALL(setImplicitBidiMethod)
PUBLIC_ID_ARG_CALL(setGlyphSubstMethod)
PUBLIC_ID_ARG_CALL(setGlyphReorderMethod)
PUBLIC_ID_ARG_CALL(setHyphenationMethod)
PUBLIC_ID_ARG_CALL(setTableAutoWidthMethod)
PUBLIC_ID_ARG_CALL(setFontName)

#define UNSIGNED_ARG_CALL(F) ONEARG_CALL(F, unsigned)

UNSIGNED_ARG_CALL(setLanguage)
UNSIGNED_ARG_CALL(setCountry)

#define STRING_ARG_CALL(F) ONEREFARG_CALL(F, StringC)

STRING_ARG_CALL(setFontFamilyName)
STRING_ARG_CALL(formattingInstruction)

#define INLINE_SPACE_ARG_CALL(F) ONEREFARG_CALL(F, InlineSpace)

INLINE_SPACE_ARG_CALL(setEscapementSpaceBefore)
INLINE_SPACE_ARG_CALL(setEscapementSpaceAfter)
ONEREFARG_CALL(setInlineSpaceSpace, OptInlineSpace);
SYMBOL_ARG_CALL(setCharMap);

SaveFOTBuilder::Call::~Call()
{
}

void SaveFOTBuilder::NoArgCall::emit(FOTBuilder &fotb) const
{
  (fotb.*func)();
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

void SaveFOTBuilder::CharactersFromNodeCall::emit(FOTBuilder &fotb) const
{
  fotb.charactersFromNode(node, data, size);
}

void SaveFOTBuilder::characters(const Char *s, size_t n) 
{
  *tail_ = new CharactersCall(s, n);
  tail_ = &(*tail_)->next;
}

ONEREFARG_CALL(characters, Vector<CharacterNIC>);

ONEREFARG_CALL(character, CharacterNIC);

SaveFOTBuilder::StartNodeCall::StartNodeCall(const NodePtr &nd,
					     const StringC &m)
: node(nd), mode(m)
{
}

void SaveFOTBuilder::StartNodeCall::emit(FOTBuilder &fotb) const
{
  fotb.startNode(node, mode);
}

void SaveFOTBuilder::startNode(const NodePtr &node, const StringC &mode)
{
  *tail_ = new StartNodeCall(node, mode);
  tail_ = &(*tail_)->next;
}

ONEREFARG_CALL(currentNodePageNumber, NodePtr)
ONEREFARG_CALL(setColor, DeviceRGBColor)
ONEREFARG_CALL(setBackgroundColor, DeviceRGBColor)
NO_ARG_CALL(setBackgroundColor)
ONEREFARG_CALL(startParagraph, ParagraphNIC)
ONEREFARG_CALL(paragraphBreak, ParagraphNIC)
ONEREFARG_CALL(startDisplayGroup, DisplayGroupNIC)
ONEREFARG_CALL(externalGraphic, ExternalGraphicNIC)
ONEREFARG_CALL(rule, RuleNIC)
ONEREFARG_CALL(startLeader, LeaderNIC)
ONEREFARG_CALL(startLink, Address)
ONEARG_CALL(startScore, Symbol)
ONEARG_CALL(startScore, Char)
ONEREFARG_CALL(startScore, LengthSpec)
ONEREFARG_CALL(startLineField, LineFieldNIC)
ONEREFARG_CALL(startBox, BoxNIC)
ONEREFARG_CALL(startSideBySide, DisplayNIC)
typedef Vector<ConstPtr<FOTBuilder::GlyphSubstTable> > VectorConstPtrFOTBuilderGlyphSubstTable;
ONEREFARG_CALL(setGlyphSubstTable, VectorConstPtrFOTBuilderGlyphSubstTable)

void SaveFOTBuilder::startSimplePageSequence(FOTBuilder* headerFooter[nHF])
{
  *tail_ = new StartSimplePageSequenceCall(headerFooter);
  tail_ = &(*tail_)->next;
}

ONEREFARG_CALL(startTable, TableNIC)

void SaveFOTBuilder::startTablePart(const TablePartNIC &nic, FOTBuilder *&header, FOTBuilder *&footer)
{
  *tail_ = new StartTablePartCall(nic, header, footer);
  tail_ = &(*tail_)->next;
}

ONEREFARG_CALL(tableColumn, TableColumnNIC)
ONEREFARG_CALL(startTableCell, TableCellNIC)

void SaveFOTBuilder::startFraction(FOTBuilder *&numerator, FOTBuilder *&denominator)
{
  *tail_ = new StartFractionCall(numerator, denominator);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::startScript(FOTBuilder *&preSup,
				 FOTBuilder *&preSub,
				 FOTBuilder *&postSup,
				 FOTBuilder *&postSub,
				 FOTBuilder *&midSup,
				 FOTBuilder *&midSub)
{
  *tail_ = new StartScriptCall(preSup, preSub,
			       postSup, postSub,
			       midSup, midSub);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::startMark(FOTBuilder *&overMark, FOTBuilder *&underMark)
{
  *tail_ = new StartMarkCall(overMark, underMark);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::startFence(FOTBuilder *&open, FOTBuilder *&close)
{
  *tail_ = new StartFenceCall(open, close);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::startRadical(FOTBuilder *&degree)
{
  *tail_ = new StartRadicalCall(degree);
  tail_ = &(*tail_)->next;
}

ONEREFARG_CALL(radicalRadical, CharacterNIC)

void SaveFOTBuilder::startMathOperator(FOTBuilder *&oper,
				       FOTBuilder *&lowerLimit,
				       FOTBuilder *&upperLimit)
{
  *tail_ = new StartMathOperatorCall(oper, lowerLimit, upperLimit);
  tail_ = &(*tail_)->next;
}
 
ONEREFARG_CALL(startGrid, GridNIC)
ONEREFARG_CALL(startGridCell, GridCellNIC)

void SaveFOTBuilder::startMultiMode(const MultiMode *principalMode,
				    const Vector<MultiMode> &namedModes,
				    Vector<FOTBuilder *> &namedPorts)
{
  *tail_ = new StartMultiModeCall(principalMode, namedModes, namedPorts);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::extensionSet(void (FOTBuilder::*func)(bool), bool arg)
{
  typedef OneArgCall<bool> ExtensionBoolArgCall;
  *tail_ = new ExtensionBoolArgCall(func, arg);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::extensionSet(void (FOTBuilder::*func)(const StringC &), const StringC &arg)
{
  typedef OneRefArgCall<StringC> ExtensionStringArgCall;
  *tail_ = new ExtensionStringArgCall(func, arg);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::extensionSet(void (FOTBuilder::*func)(long), long arg)
{
  typedef OneArgCall<long> ExtensionLongArgCall;
  *tail_ = new ExtensionLongArgCall(func, arg);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::startExtension(const CompoundExtensionFlowObj &fo,
				    const NodePtr &node,
				    Vector<FOTBuilder *> &ports)
{
  *tail_ = new StartExtensionCall(fo, node, ports);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::endExtension(const CompoundExtensionFlowObj &fo)
{
  *tail_ = new EndExtensionCall(fo);
  tail_ = &(*tail_)->next;
}

void SaveFOTBuilder::extension(const ExtensionFlowObj &fo, const NodePtr &node)
{
  *tail_ = new ExtensionCall(fo, node);
  tail_ = &(*tail_)->next;
}

SaveFOTBuilder::CharactersCall::CharactersCall(const Char *s, size_t n)
: str(s, n)
{
}

void SaveFOTBuilder::CharactersCall::emit(FOTBuilder &fotb) const
{
  fotb.characters(str.data(), str.size());
}

StartSimplePageSequenceCall::StartSimplePageSequenceCall(FOTBuilder* hf[FOTBuilder::nHF])
{
  for ( unsigned i = 0; i < FOTBuilder::nHF; ++i )
    hf[i] = &headerFooter[i];
}

void StartSimplePageSequenceCall::emit(FOTBuilder& fotb) const
{
  FOTBuilder* hf[FOTBuilder::nHF];
  fotb.startSimplePageSequence(hf);
  for ( unsigned i = 0; i < FOTBuilder::nHF; ++i )
    headerFooter[i].emit(*hf[i]);
}

StartFractionCall::StartFractionCall(FOTBuilder *&n, FOTBuilder *&d)
{
  n = &numerator;
  d = &denominator;
}

void StartFractionCall::emit(FOTBuilder &fotb) const
{
  FOTBuilder *n, *d;
  fotb.startFraction(n, d);
  numerator.emit(*n);
  denominator.emit(*d);
}

StartScriptCall::StartScriptCall(FOTBuilder *&p0,
				 FOTBuilder *&p1,
				 FOTBuilder *&p2,
				 FOTBuilder *&p3,
				 FOTBuilder *&p4,
				 FOTBuilder *&p5)
{
  p0 = &preSup;
  p1 = &preSub;
  p2 = &postSup;
  p3 = &postSub;
  p4 = &midSup;
  p5 = &midSub;
}

void StartScriptCall::emit(FOTBuilder &fotb) const
{
  FOTBuilder *v[6];
  fotb.startScript(v[0], v[1], v[2], v[3], v[4], v[5]);
  preSup.emit(*v[0]);
  preSub.emit(*v[1]);
  postSup.emit(*v[2]);
  postSub.emit(*v[3]);
  midSup.emit(*v[4]);
  midSub.emit(*v[5]);
}

StartMarkCall::StartMarkCall(FOTBuilder *&o, FOTBuilder *&u)
{
  o = &overMark;
  u = &underMark;
}

void StartMarkCall::emit(FOTBuilder &fotb) const
{
  FOTBuilder *o, *u;
  fotb.startMark(o, u);
  overMark.emit(*o);
  underMark.emit(*u);
}

StartFenceCall::StartFenceCall(FOTBuilder *&o, FOTBuilder *&c)
{
  o = &open;
  c = &close;
}

void StartFenceCall::emit(FOTBuilder &fotb) const
{
  FOTBuilder *o, *c;
  fotb.startFence(o, c);
  open.emit(*o);
  close.emit(*c);
}

StartRadicalCall::StartRadicalCall(FOTBuilder *&d)
{
  d = &degree;
}

void StartRadicalCall::emit(FOTBuilder &fotb) const
{
  FOTBuilder *d;
  fotb.startRadical(d);
  degree.emit(*d);
}

StartMathOperatorCall::StartMathOperatorCall(FOTBuilder *&o,
					     FOTBuilder *&l,
					     FOTBuilder *&u)
{
  o = &oper;
  l = &lowerLimit;
  u = &upperLimit;
}

void StartMathOperatorCall::emit(FOTBuilder &fotb) const
{
  FOTBuilder *o, *l, *u;
  fotb.startMathOperator(o, l, u);
  oper.emit(*o);
  lowerLimit.emit(*l);
  upperLimit.emit(*u);
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

StartMultiModeCall::StartMultiModeCall(const StartMultiModeCall& other)
: principalMode(other.principalMode)
, hasPrincipalMode(other.hasPrincipalMode)
, namedModes(other.namedModes)
, ports()
{
  for (IListIter<SaveFOTBuilder> p(other.ports); !p.done(); p.next() )
    ports.append(new SaveFOTBuilder(*p.cur()));
}

void StartMultiModeCall::emit(FOTBuilder &fotb) const
{
  Vector<FOTBuilder *> v(namedModes.size());
  fotb.startMultiMode(hasPrincipalMode ? &principalMode : 0, namedModes, v);
  IListIter<SaveFOTBuilder> p(ports);
  for (size_t i = 0; i < v.size(); i++) {
    p.cur()->emit(*v[i]);
    p.next();
  }
}

StartTablePartCall::StartTablePartCall(const SaveFOTBuilder::TablePartNIC &nic,
				       FOTBuilder *&h, FOTBuilder *&f)
: arg(nic)
{
  h = &header;
  f = &footer;
}

void StartTablePartCall::emit(FOTBuilder &fotb) const
{
  FOTBuilder *h, *f;
  fotb.startTablePart(arg, h, f);
  header.emit(*h);
  footer.emit(*f);
}

SaveFOTBuilder::ExtensionCall::ExtensionCall(const ExtensionCall& other)
: arg ( other.arg->copy()->asCompoundExtensionFlowObj() )
, node ( other.node )
{
}
  
void SaveFOTBuilder::ExtensionCall::emit(FOTBuilder &fotb) const
{
  fotb.extension(*arg, node);
}

SaveFOTBuilder::EndExtensionCall::EndExtensionCall(const EndExtensionCall& other)
: arg ( other.arg->copy()->asCompoundExtensionFlowObj() )
{
}
  
void SaveFOTBuilder::EndExtensionCall::emit(FOTBuilder &fotb) const
{
  fotb.endExtension(*arg);
}

StartExtensionCall::StartExtensionCall(const FOTBuilder::CompoundExtensionFlowObj &fo,
				       const NodePtr &nd,
				       Vector<FOTBuilder *> &v)
: flowObj(fo.copy()->asCompoundExtensionFlowObj()), node(nd)
{
  for (size_t i = v.size(); i > 0; i--) {
    ports.insert(new SaveFOTBuilder);
    v[i - 1] = ports.head();
  }
}

StartExtensionCall::StartExtensionCall(const StartExtensionCall& other)
: ports()
, node(other.node)
, flowObj(other.flowObj->copy()->asCompoundExtensionFlowObj())
{
  for (IListIter<SaveFOTBuilder> p(other.ports); !p.done(); p.next() )
    ports.append(new SaveFOTBuilder(*p.cur()));
}

void StartExtensionCall::emit(FOTBuilder &fotb) const
{
  Vector<StringC> portNames;
  flowObj->portNames(portNames);
  Vector<FOTBuilder *> v(portNames.size());
  fotb.startExtension(*flowObj, node, v);
  IListIter<SaveFOTBuilder> p(ports);
  for (size_t i = 0; i < v.size(); i++) {
    p.cur()->emit(*v[i]);
    p.next();
  }
}

SerialFOTBuilder::SerialFOTBuilder()
{
}

void SerialFOTBuilder::startSimplePageSequence(FOTBuilder* headerFooter[FOTBuilder::nHF])
{
  for ( unsigned i = 0; i < nHF; ++i ) {
    save_.insert(new SaveFOTBuilder);
    headerFooter[nHF-1-i] = save_.head();
  }
  startSimplePageSequenceSerial();
}

void SerialFOTBuilder::endSimplePageSequenceHeaderFooter()
{
  Owner<SaveFOTBuilder> hf[nHF];  
  for ( unsigned k = 0; k < nHF; ++k ) 
    hf[k] = save_.get();
  // output all 24 parts, but in same order as 1.2.1, for regression testing
  // replace with single loop later.
  // sorry about all those constants :(
  for (int i = 0; i < (1 << 2); i++) {
    for (int j = 0; j < 6; j++) {
      unsigned k = i | (j << 2);
      startSimplePageSequenceHeaderFooter(k);
      hf[k]->emit(*this);
      endSimplePageSequenceHeaderFooter(k);
    }
  }
  endAllSimplePageSequenceHeaderFooter();
}

void SerialFOTBuilder::endSimplePageSequence()
{
  endSimplePageSequenceSerial();
}


void SerialFOTBuilder::startFraction(FOTBuilder *&numerator, FOTBuilder *&denominator)
{
  save_.insert(new SaveFOTBuilder);
  denominator = save_.head();
  save_.insert(new SaveFOTBuilder);
  numerator = save_.head();
  startFractionSerial();
}

void SerialFOTBuilder::endFraction()
{
  {
    Owner<SaveFOTBuilder> numerator(save_.get());
    startFractionNumerator();
    numerator->emit(*this);
    endFractionNumerator();
  }
  {
    Owner<SaveFOTBuilder> denominator(save_.get());
    startFractionDenominator();
    denominator->emit(*this);
    endFractionDenominator();
  }
  endFractionSerial();
}

void SerialFOTBuilder::startScript(FOTBuilder *&preSup,
				   FOTBuilder *&preSub,
				   FOTBuilder *&postSup,
				   FOTBuilder *&postSub,
				   FOTBuilder *&midSup,
				   FOTBuilder *&midSub)
{
  save_.insert(new SaveFOTBuilder);
  midSub = save_.head();
  save_.insert(new SaveFOTBuilder);
  midSup = save_.head();
  save_.insert(new SaveFOTBuilder);
  postSub = save_.head();
  save_.insert(new SaveFOTBuilder);
  postSup = save_.head();
  save_.insert(new SaveFOTBuilder);
  preSub = save_.head();
  save_.insert(new SaveFOTBuilder);
  preSup = save_.head();
  startScriptSerial();
}

void SerialFOTBuilder::endScript()
{
  {
    Owner<SaveFOTBuilder> tem(save_.get());
    startScriptPreSup();
    tem->emit(*this);
    endScriptPreSup();
  }
  {
    Owner<SaveFOTBuilder> tem(save_.get());
    startScriptPreSub();
    tem->emit(*this);
    endScriptPreSub();
  }
  {
    Owner<SaveFOTBuilder> tem(save_.get());
    startScriptPostSup();
    tem->emit(*this);
    endScriptPostSup();
  }
  {
    Owner<SaveFOTBuilder> tem(save_.get());
    startScriptPostSub();
    tem->emit(*this);
    endScriptPostSub();
  }
  {
    Owner<SaveFOTBuilder> tem(save_.get());
    startScriptMidSup();
    tem->emit(*this);
    endScriptMidSup();
  }
  {
    Owner<SaveFOTBuilder> tem(save_.get());
    startScriptMidSub();
    tem->emit(*this);
    endScriptMidSub();
  }
  endScriptSerial();
}

void SerialFOTBuilder::startMark(FOTBuilder *&overMark, FOTBuilder *&underMark)
{
  save_.insert(new SaveFOTBuilder);
  underMark = save_.head();
  save_.insert(new SaveFOTBuilder);
  overMark = save_.head();
  startMarkSerial();
}

void SerialFOTBuilder::endMark()
{
  {
    Owner<SaveFOTBuilder> tem(save_.get());
    startMarkOver();
    tem->emit(*this);
    endMarkOver();
  }
  {
    Owner<SaveFOTBuilder> tem(save_.get());
    startMarkUnder();
    tem->emit(*this);
    endMarkUnder();
  }
  endMarkSerial();
}

void SerialFOTBuilder::startFence(FOTBuilder *&open, FOTBuilder *&close)
{
  save_.insert(new SaveFOTBuilder);
  close = save_.head();
  save_.insert(new SaveFOTBuilder);
  open = save_.head();
  startFenceSerial();
}

void SerialFOTBuilder::endFence()
{
  {
    Owner<SaveFOTBuilder> tem(save_.get());
    startFenceOpen();
    tem->emit(*this);
    endFenceOpen();
  }
  {
    Owner<SaveFOTBuilder> tem(save_.get());
    startFenceClose();
    tem->emit(*this);
    endFenceClose();
  }
  endFenceSerial();
}

void SerialFOTBuilder::startRadical(FOTBuilder *&degree)
{
  save_.insert(new SaveFOTBuilder);
  degree = save_.head();
  startRadicalSerial();
}

void SerialFOTBuilder::endRadical()
{
  Owner<SaveFOTBuilder> tem(save_.get());
  startRadicalDegree();
  tem->emit(*this);
  endRadicalDegree();
  endRadicalSerial();
}

void SerialFOTBuilder::startMathOperator(FOTBuilder *&oper,
					 FOTBuilder *&lowerLimit,
					 FOTBuilder *&upperLimit)
{
  save_.insert(new SaveFOTBuilder);
  upperLimit = save_.head();
  save_.insert(new SaveFOTBuilder);
  lowerLimit = save_.head();
  save_.insert(new SaveFOTBuilder);
  oper = save_.head();
  startMathOperatorSerial();
}

void SerialFOTBuilder::endMathOperator()
{
  {
    Owner<SaveFOTBuilder> tem(save_.get());
    startMathOperatorOperator();
    tem->emit(*this);
    endMathOperatorOperator();
  }
  {
    Owner<SaveFOTBuilder> tem(save_.get());
    startMathOperatorLowerLimit();
    tem->emit(*this);
    endMathOperatorLowerLimit();
  }
  {
    Owner<SaveFOTBuilder> tem(save_.get());
    startMathOperatorUpperLimit();
    tem->emit(*this);
    endMathOperatorUpperLimit();
  }
  endMathOperatorSerial();
}

void SerialFOTBuilder::startSimplePageSequenceSerial()
{
  start();
}

void SerialFOTBuilder::endSimplePageSequenceSerial()
{
  end();
}

void SerialFOTBuilder::startSimplePageSequenceHeaderFooter(unsigned)
{
  start();
}

void SerialFOTBuilder::endSimplePageSequenceHeaderFooter(unsigned)
{
  end();
}

void SerialFOTBuilder::endAllSimplePageSequenceHeaderFooter()
{
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

void SerialFOTBuilder::startScriptSerial()
{
  start();
}

void SerialFOTBuilder::endScriptSerial()
{
  end();
}

void SerialFOTBuilder::startScriptPreSup()
{
}

void SerialFOTBuilder::endScriptPreSup()
{
}

void SerialFOTBuilder::startScriptPreSub()
{
}

void SerialFOTBuilder::endScriptPreSub()
{
}

void SerialFOTBuilder::startScriptPostSup()
{
}

void SerialFOTBuilder::endScriptPostSup()
{
}

void SerialFOTBuilder::startScriptPostSub()
{
}

void SerialFOTBuilder::endScriptPostSub()
{
}

void SerialFOTBuilder::startScriptMidSup()
{
}

void SerialFOTBuilder::endScriptMidSup()
{
}

void SerialFOTBuilder::startScriptMidSub()
{
}

void SerialFOTBuilder::endScriptMidSub()
{
}

void SerialFOTBuilder::startMarkSerial()
{
  start();
}

void SerialFOTBuilder::endMarkSerial()
{
  end();
}

void SerialFOTBuilder::startMarkOver()
{
}

void SerialFOTBuilder::endMarkOver()
{
}

void SerialFOTBuilder::startMarkUnder()
{
}

void SerialFOTBuilder::endMarkUnder()
{
}

void SerialFOTBuilder::startFenceSerial()
{
  start();
}

void SerialFOTBuilder::endFenceSerial()
{
  end();
}

void SerialFOTBuilder::startFenceOpen()
{
}

void SerialFOTBuilder::endFenceOpen()
{
}

void SerialFOTBuilder::startFenceClose()
{
}

void SerialFOTBuilder::endFenceClose()
{
}

void SerialFOTBuilder::startRadicalSerial()
{
  start();
}

void SerialFOTBuilder::endRadicalSerial()
{
  end();
}

void SerialFOTBuilder::startRadicalDegree()
{
}

void SerialFOTBuilder::endRadicalDegree()
{
}

void SerialFOTBuilder::startMathOperatorSerial()
{
  start();
}

void SerialFOTBuilder::endMathOperatorSerial()
{
  end();
}

void SerialFOTBuilder::startMathOperatorOperator()
{
}

void SerialFOTBuilder::endMathOperatorOperator()
{
}

void SerialFOTBuilder::startMathOperatorLowerLimit()
{
}

void SerialFOTBuilder::endMathOperatorLowerLimit()
{
}

void SerialFOTBuilder::startMathOperatorUpperLimit()
{
}

void SerialFOTBuilder::endMathOperatorUpperLimit()
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
  Owner<SaveFOTBuilder> header(save_.get());
  startTablePartHeader();
  header->emit(*this);
  endTablePartHeader();
  Owner<SaveFOTBuilder> footer(save_.get());
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
    Owner<SaveFOTBuilder> mode(save_.get());
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

void SerialFOTBuilder::startExtension(const CompoundExtensionFlowObj &flowObj,
				      const NodePtr &nd,
				      Vector<FOTBuilder *> &ports)
{
  for (size_t i = ports.size(); i > 0; i--) {
    save_.insert(new SaveFOTBuilder);
    ports[i - 1] = save_.head();
  }
  startExtensionSerial(flowObj, nd);
}

void SerialFOTBuilder::endExtension(const CompoundExtensionFlowObj &flowObj)
{
  Vector<StringC> portNames;
  flowObj.portNames(portNames);
  for (size_t i = 0; i < portNames.size(); i++) {
    Owner<SaveFOTBuilder> stream(save_.get());
    startExtensionStream(portNames[i]);
    stream->emit(*this);
    endExtensionStream(portNames[i]);
  }
  endExtensionSerial(flowObj);
}

void SerialFOTBuilder::startExtensionSerial(const CompoundExtensionFlowObj &, const NodePtr &)
{
  start();
}

void SerialFOTBuilder::endExtensionSerial(const CompoundExtensionFlowObj &)
{
  end();
}

void SerialFOTBuilder::startExtensionStream(const StringC &)
{
}

void SerialFOTBuilder::endExtensionStream(const StringC &)
{
}

FOTBuilder::ExtensionFlowObj::~ExtensionFlowObj()
{
}

FOTBuilder::CompoundExtensionFlowObj *
FOTBuilder::ExtensionFlowObj::asCompoundExtensionFlowObj()
{
  return 0;
}

const FOTBuilder::CompoundExtensionFlowObj *
FOTBuilder::ExtensionFlowObj::asCompoundExtensionFlowObj() const
{
  return 0;
}

bool FOTBuilder::ExtensionFlowObj::hasNIC(const StringC &) const
{
  return 0;
}

void FOTBuilder::ExtensionFlowObj::setNIC(const StringC &, const Value &)
{
  CANNOT_HAPPEN();
}

FOTBuilder::CompoundExtensionFlowObj *
FOTBuilder::CompoundExtensionFlowObj::asCompoundExtensionFlowObj()
{
  return this;
}

const FOTBuilder::CompoundExtensionFlowObj *
FOTBuilder::CompoundExtensionFlowObj::asCompoundExtensionFlowObj() const
{
  return this;
}

bool FOTBuilder::CompoundExtensionFlowObj::hasPrincipalPort() const
{
  return 1;
}
 
void FOTBuilder::CompoundExtensionFlowObj::portNames(Vector<StringC> &) const
{
}

#ifdef DSSSL_NAMESPACE
}
#endif
