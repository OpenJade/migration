// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#include "config.h"
#include "SgmlFOTBuilder.h"
#include "macros.h"
#include <stdio.h>

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

class SgmlFOTBuilder : public SerialFOTBuilder {
public:
  SgmlFOTBuilder(OutputCharStream *);
  void setFontSize(Length);
  void setFontFamilyName(const StringC &);
  void setFontWeight(Symbol);
  void setFontPosture(Symbol);
  void setStartIndent(const LengthSpec &);
  void setEndIndent(const LengthSpec &);
  void setFirstLineStartIndent(const LengthSpec &);
  void setLastLineEndIndent(const LengthSpec &);
  void setLineSpacing(const LengthSpec &);
  void setFieldWidth(const LengthSpec &);
  void setPositionPointShift(const LengthSpec &);
  void setStartMargin(const LengthSpec &);
  void setEndMargin(const LengthSpec &);
  void setSidelineSep(const LengthSpec &);
  void setAsisWrapIndent(const LengthSpec &);
  void setLineNumberSep(const LengthSpec &);
  void setLastLineJustifyLimit(const LengthSpec &);
  void setJustifyGlyphSpaceMaxAdd(const LengthSpec &);
  void setJustifyGlyphSpaceMaxRemove(const LengthSpec &);
  void setTableCornerRadius(const LengthSpec &);
  void setBoxCornerRadius(const LengthSpec &);
  void setMarginaliaSep(const LengthSpec &);
  void setMinPreLineSpacing(const OptLengthSpec &);
  void setMinPostLineSpacing(const OptLengthSpec &);
  void setMinLeading(const OptLengthSpec &);
  void setLines(Symbol);
  void setQuadding(Symbol);
  void setDisplayAlignment(Symbol sym);
  void setFieldAlign(Symbol);
  void setLineJoin(Symbol);
  void setLineCap(Symbol);
  void setLineNumberSide(Symbol);
  void setKernMode(Symbol);
  void setInputWhitespaceTreatment(Symbol);
  void setFillingDirection(Symbol);
  void setWritingMode(Symbol);
  void setLastLineQuadding(Symbol);
  void setMathDisplayMode(Symbol);
  void setScriptPreAlign(Symbol);
  void setScriptPostAlign(Symbol);
  void setScriptMidSupAlign(Symbol);
  void setScriptMidSubAlign(Symbol);
  void setNumeratorAlign(Symbol);
  void setDenominatorAlign(Symbol);
  void setGridPositionCellType(Symbol);
  void setGridColumnAlignment(Symbol);
  void setGridRowAlignment(Symbol);
  void setBoxType(Symbol);
  void setGlyphAlignmentMode(Symbol);
  void setBoxBorderAlignment(Symbol);
  void setCellRowAlignment(Symbol);
  void setBorderAlignment(Symbol);
  void setSidelineSide(Symbol);
  void setHyphenationKeep(Symbol);
  void setPositionPreference(Symbol);
  void setFontStructure(Symbol);
  void setFontProportionateWidth(Symbol);
  void setCellCrossed(Symbol);
  void setMarginaliaSide(Symbol);
  void setColor(const DeviceRGBColor &);
  void setBackgroundColor(const DeviceRGBColor &);
  void setBackgroundColor();
  void setPageWidth(Length);
  void setPageHeight(Length);
  void setLeftMargin(Length);
  void setRightMargin(Length);
  void setTopMargin(Length);
  void setBottomMargin(Length);
  void setHeaderMargin(Length);
  void setFooterMargin(Length);
  void setLineThickness(Length);
  void setCellBeforeRowMargin(Length);
  void setCellAfterRowMargin(Length);
  void setCellBeforeColumnMargin(Length);
  void setCellAfterColumnMargin(Length);
  void setLineSep(Length);
  void setBoxSizeBefore(Length);
  void setBoxSizeAfter(Length);
  void setLayer(long);
  void setBackgroundLayer(long);
  void setBorderPriority(long);
  void setLineRepeat(long);
  void setSpan(long);
  void setMinLeaderRepeat(long);
  void setHyphenationRemainCharCount(long);
  void setHyphenationPushCharCount(long);
  void setWidowCount(long);
  void setOrphanCount(long);
  void setExpandTabs(long);
  void setHyphenationLadderCount(long);
  void setLanguage(Letter2);
  void setCountry(Letter2);
  void setBackgroundTile(PublicId);
  void setLineBreakingMethod(PublicId);
  void setLineCompositionMethod(PublicId);
  void setImplicitBidiMethod(PublicId);
  void setGlyphSubstMethod(PublicId);
  void setGlyphReorderMethod(PublicId);
  void setHyphenationMethod(PublicId);
  void setTableAutoWidthMethod(PublicId);
  void setFontName(PublicId);
  void setBorderPresent(bool);
  void setInhibitLineBreaks(bool);
  void setHyphenate(bool);
  void setKern(bool);
  void setLigature(bool);
  void setScoreSpaces(bool);
  void setFloatOutMarginalia(bool);
  void setFloatOutSidelines(bool);
  void setFloatOutLineNumbers(bool);
  void setCellBackground(bool);
  void setSpanWeak(bool);
  void setIgnoreRecordEnd(bool);
  void setNumberedLines(bool);
  void setHangingPunct(bool);
  void setBoxOpenEnd(bool);
  void setTruncateLeader(bool);
  void setAlignLeader(bool);
  void setTablePartOmitMiddleHeader(bool);
  void setTablePartOmitMiddleFooter(bool);
  void setBorderOmitAtBreak(bool);
  void setPrincipalModeSimultaneous(bool);
  void setMarginaliaKeepWithPrevious(bool);
  void setGridEquidistantRows(bool);
  void setGridEquidistantColumns(bool);
  void setEscapementSpaceBefore(const InlineSpace &);
  void setEscapementSpaceAfter(const InlineSpace &);
  void setGlyphSubstTable(const Vector<ConstPtr<GlyphSubstTable> > &tables);

  void paragraphBreak(const ParagraphNIC &);
  void characters(const Char *, size_t);
  void character(const CharacterNIC &);
  void externalGraphic(const ExternalGraphicNIC &);
  void rule(const RuleNIC &);
  void alignmentPoint();

  void startSimplePageSequence();
  void endSimplePageSequence();
  void pageNumber();
  void startSimplePageSequenceHeaderFooter(unsigned);
  void endSimplePageSequenceHeaderFooter(unsigned);
  void startScroll();
  void endScroll();
  void startLink(const Address &);
  void endLink();
  void startMarginalia();
  void endMarginalia();
  void startMultiModeSerial(const MultiMode *);
  void endMultiModeSerial();
  void startMultiModeMode(const MultiMode &);
  void endMultiModeMode();
  void startScore(Char);
  void startScore(const LengthSpec &);
  void startScore(Symbol);
  void endScore();
  void startLeader(const LeaderNIC &);
  void endLeader();
  void startSideline();
  void endSideline();
  void startBox(const BoxNIC &);
  void endBox();
  void startParagraph(const ParagraphNIC &);
  void endParagraph();
  void startSequence();
  void endSequence();
  void startLineField(const LineFieldNIC &);
  void endLineField();
  void startDisplayGroup(const DisplayGroupNIC &);
  void endDisplayGroup();

  void startTable(const TableNIC &);
  void endTable();
  void tableBeforeRowBorder();
  void tableAfterRowBorder();
  void tableBeforeColumnBorder();
  void tableAfterColumnBorder();
  void tableColumn(const TableColumnNIC &);
  void startTablePartSerial(const TablePartNIC &);
  void endTablePartSerial();
  void startTablePartHeader();
  void endTablePartHeader();
  void startTablePartFooter();
  void endTablePartFooter();
  void startTableRow();
  void endTableRow();
  void startTableCell(const TableCellNIC &);
  void endTableCell();
  void tableCellBeforeRowBorder();
  void tableCellAfterRowBorder();
  void tableCellBeforeColumnBorder();
  void tableCellAfterColumnBorder();
  void startMathSequence();
  void endMathSequence();
  void startFractionSerial();
  void endFractionSerial();
  void startFractionNumerator();
  void endFractionNumerator();
  void startFractionDenominator();
  void endFractionDenominator();
  void fractionBar();
  void startUnmath();
  void endUnmath();
  void startSuperscript();
  void endSuperscript();
  void startSubscript();
  void endSubscript();
  void startScriptSerial();
  void endScriptSerial();
  void startScriptPreSup();
  void endScriptPreSup();
  void startScriptPreSub();
  void endScriptPreSub();
  void startScriptPostSup();
  void endScriptPostSup();
  void startScriptPostSub();
  void endScriptPostSub();
  void startScriptMidSup();
  void endScriptMidSup();
  void startScriptMidSub();
  void endScriptMidSub();
  void startMarkSerial();
  void endMarkSerial();
  void startMarkOver();
  void endMarkOver();
  void startMarkUnder();
  void endMarkUnder();
  void startFenceSerial();
  void endFenceSerial();
  void startFenceOpen();
  void endFenceOpen();
  void startFenceClose();
  void endFenceClose();
  void startRadicalSerial();
  void endRadicalSerial();
  void startRadicalDegree();
  void endRadicalDegree();
  void radicalRadical(const CharacterNIC &);
  void radicalRadicalDefaulted();
  void startMathOperatorSerial();
  void endMathOperatorSerial();
  void startMathOperatorOperator();
  void endMathOperatorOperator();
  void startMathOperatorLowerLimit();
  void endMathOperatorLowerLimit();
  void startMathOperatorUpperLimit();
  void endMathOperatorUpperLimit();
  void startGrid(const GridNIC &);
  void endGrid();
  void startGridCell(const GridCellNIC &);
  void endGridCell();
  void startNode(const NodePtr &, const StringC &, RuleType);
  void endNode();
  void currentNodePageNumber(const NodePtr &);
private:
  OutputCharStream &os() { return *os_; }
  void outputIcs();
  void startSimpleFlowObj(const char *name);
  void simpleFlowObj(const char *name);
  void startPortFlow(const char *name);
  void endFlow(const char *name);
  void characterNIC(const CharacterNIC &nic);
  void displayNIC(const DisplayNIC &nic);
  void inlineNIC(const InlineNIC &nic);
  void boolC(const char *, bool);
  void lengthC(const char *, Length);
  void lengthSpecC(const char *, const LengthSpec &);
  void optLengthSpecC(const char *, const OptLengthSpec &);
  void symbolC(const char *name, Symbol sym);
  void integerC(const char *, long);
  void publicIdC(const char *, PublicId);
  void inlineSpaceC(const char *name, const InlineSpace &);
  void displaySpaceNIC(const char *name, const DisplaySpace &);

  Owner<OutputCharStream> os_;
  StrOutputCharStream ics_;
  unsigned nodeDepth_;
  unsigned elementNodeDepth_;
  Vector<PackedBoolean> multiModeHasPrincipalMode_;
  Vector<PackedBoolean> glyphSubstTableDefined_;
};

const char RE = '\r';

inline
OutputCharStream &operator<<(OutputCharStream &os, 
#ifdef GROVE_NAMESPACE
			     GROVE_NAMESPACE::
#endif
			     GroveString &str)
{
  return os.write(str.data(), str.size());
}

inline
OutputCharStream &operator<<(OutputCharStream &os, long n)
{
  if (n < 0)
    return os << '-' << (unsigned long)-n;
  else
    return os << (unsigned long)n;
}

inline
OutputCharStream &operator<<(OutputCharStream &os, FOTBuilder::Symbol sym)
{
  switch (sym) {
  case FOTBuilder::symbolFalse:
    os << '0';
    break;
  case FOTBuilder::symbolTrue:
    os << '1';
    break;
  default:
    os << FOTBuilder::symbolName(sym);
    break;
  }
  return os;
}

inline
OutputCharStream &operator<<(OutputCharStream &os, double d)
{
  char buf[128];
  sprintf(buf, "%g", d);
  return os << buf;
}

inline
OutputCharStream &operator<<(OutputCharStream &os,
			     const FOTBuilder::LengthSpec &ls)
{
  if (ls.displaySizeFactor != 0.0) {
    // 100+20%
    os << '"';
    if (ls.length) {
      os << ls.length;
      if (ls.displaySizeFactor >= 0.0)
	os << '+';
    }
    char buf[128];
    sprintf(buf, "%.2f%%", ls.displaySizeFactor * 100.0);
    os << buf << '"';
  }
  else
    os << ls.length;
  return os;
}

OutputCharStream &operator<<(OutputCharStream &os,
			     const FOTBuilder::TableLengthSpec &ls)
{
  if (ls.displaySizeFactor != 0.0 || ls.tableUnitFactor != 0.0) {
    // 100+20%
    os << '"';
    bool needSign = 0;
    if (ls.length) {
      os << ls.length;
      needSign = 1;
    }
    char buf[128];
    if (ls.displaySizeFactor != 0.0) {
      if (needSign && ls.displaySizeFactor >= 0.0)
	os << '+';
      sprintf(buf, "%.2f%%", ls.displaySizeFactor * 100.0);
      needSign = 1;
      os << buf;
    }
    if (ls.tableUnitFactor != 0.0) {
      if (needSign && ls.tableUnitFactor >= 0.0)
	os << '+';
      sprintf(buf, "%.2f*", ls.tableUnitFactor);
      os << buf;
    }
    os << '"';
  }
  else
    os << ls.length;
  return os;
}

inline
OutputCharStream &operator<<(OutputCharStream &os,
			     const FOTBuilder::GlyphId &gid)
{
  if (gid.publicId) {
    os << '"' << gid.publicId;
    if (gid.suffix)
      os << "::" << gid.suffix;
    os << '"';
  }
  else
    os << '0';
  return os;
}

FOTBuilder *makeSgmlFOTBuilder(OutputCharStream *os)
{
  return new SgmlFOTBuilder(os);
}

static
void outputNumericCharRef(OutputCharStream &os, Char c)
{
  os << "&#" << (unsigned long)c << ';';
}

SgmlFOTBuilder::SgmlFOTBuilder(OutputCharStream *os)
: os_(os), nodeDepth_(0), elementNodeDepth_(0)
{
  os->setEscaper(outputNumericCharRef);
  *os_ << "<!doctype fot public \"-//James Clark//DTD DSSSL Flow Object Tree//EN\">"
       << RE;
}

void SgmlFOTBuilder::characters(const Char *s, size_t n)
{
  os() << "<chars>" << RE;
  for (; n > 0; n--, s++) {
    switch (*s) {
    default:
      os().put(*s);
      break;
    case '&':
    case '<':
    case '>':
      os() << "&#" << (unsigned long)*s << ';';
      break;
    }
  }
  os() << RE << "</chars>" << RE;
}

void SgmlFOTBuilder::characterNIC(const CharacterNIC &nic)
{
  if (nic.specifiedC) {
    if (nic.specifiedC & (1 << CharacterNIC::cChar))
      os() << " char=\"&#" << (unsigned long)nic.ch << ";\"";
    if (nic.specifiedC & (1 << CharacterNIC::cGlyphId))
      os() << " glyph-id=" << nic.glyphId;
    if (nic.specifiedC & (1 << CharacterNIC::cIsDropAfterLineBreak))
      os() << " drop-after-line-break=" << (nic.isDropAfterLineBreak ? '1' : '0');
    if (nic.specifiedC & (1 << CharacterNIC::cIsDropUnlessBeforeLineBreak))
      os() << " drop-unless-before-line-break=" << (nic.isDropUnlessBeforeLineBreak ? '1' : '0');
    if (nic.specifiedC & (1 << CharacterNIC::cIsPunct))
      os() << " punct=" << (nic.isPunct ? '1' : '0');
    if (nic.specifiedC & (1 << CharacterNIC::cIsInputWhitespace))
      os() << " input-whitespace=" << (nic.isInputWhitespace ? '1' : '0');
    if (nic.specifiedC & (1 << CharacterNIC::cIsInputTab))
      os() << " input-tab=" << (nic.isInputTab ? '1' : '0');
    if (nic.specifiedC & (1 << CharacterNIC::cIsRecordEnd))
      os() << " record-end=" << (nic.isRecordEnd ? '1' : '0');
    if (nic.specifiedC & (1 << CharacterNIC::cIsSpace))
      os() << " space=" << (nic.isSpace ? '1' : '0');
    if (nic.specifiedC & (1 << CharacterNIC::cScript)) {
      if (nic.script)
	os() << " script=\"" << nic.script << '"';
      else
	os() << " script=0";
    }
    if (nic.specifiedC & (1 << CharacterNIC::cMathClass))
      os() << " math-class=" << nic.mathClass;
    if (nic.specifiedC & (1 << CharacterNIC::cMathFontPosture))
      os() << " math-font-posture=" << nic.mathFontPosture;
    if (nic.specifiedC & (1 << CharacterNIC::cBreakBeforePriority)) 
      os() << " break-before-priority=" << nic.breakBeforePriority;
    if (nic.specifiedC & (1 << CharacterNIC::cBreakAfterPriority))
      os() << " break-after-priority=" << nic.breakAfterPriority;
  }
  if (nic.stretchFactor != 1.0)
    os() << " stretch-factor=" << nic.stretchFactor;
}

void SgmlFOTBuilder::character(const CharacterNIC &nic)
{
  os() << "<character";
  characterNIC(nic);
  outputIcs();
  os() << '>' << RE;
}

void SgmlFOTBuilder::outputIcs()
{
  StringC str;
  ics_.extractString(str);
  os() << str;
}

void SgmlFOTBuilder::setFontSize(Length n)
{
  lengthC("font-size", n);
}

void SgmlFOTBuilder::setFontFamilyName(const StringC &str)
{
  ics_ << " font-family-name=\"" << str << "\"";
}

void SgmlFOTBuilder::setFontWeight(Symbol weight)
{
  symbolC("font-weight", weight);
}

void SgmlFOTBuilder::setFontPosture(Symbol posture)
{
  symbolC("font-posture", posture);
}

void SgmlFOTBuilder::setStartIndent(const LengthSpec &ls)
{
  lengthSpecC("start-indent", ls);
}

void SgmlFOTBuilder::setEndIndent(const LengthSpec &ls)
{
  lengthSpecC("end-indent", ls);
}

void SgmlFOTBuilder::setFirstLineStartIndent(const LengthSpec &ls)
{
  lengthSpecC("first-line-start-indent", ls);
}

void SgmlFOTBuilder::setLastLineEndIndent(const LengthSpec &ls)
{
  lengthSpecC("last-line-end-indent", ls);
}

void SgmlFOTBuilder::setLineSpacing(const LengthSpec &ls)
{
  lengthSpecC("line-spacing", ls);
}

void SgmlFOTBuilder::setFieldWidth(const LengthSpec &ls)
{
  lengthSpecC("field-width", ls);
}

void SgmlFOTBuilder::setPositionPointShift(const LengthSpec &ls)
{
  lengthSpecC("position-point-shift", ls);
}

void SgmlFOTBuilder::setStartMargin(const LengthSpec &ls)
{
  lengthSpecC("start-margin", ls);
}

void SgmlFOTBuilder::setEndMargin(const LengthSpec &ls)
{
  lengthSpecC("end-margin", ls);
}

void SgmlFOTBuilder::setSidelineSep(const LengthSpec &ls)
{
  lengthSpecC("sideline-sep", ls);
}

void SgmlFOTBuilder::setAsisWrapIndent(const LengthSpec &ls)
{
  lengthSpecC("asis-wrap-indent", ls);
}

void SgmlFOTBuilder::setLineNumberSep(const LengthSpec &ls)
{
  lengthSpecC("line-number-sep", ls);
}

void SgmlFOTBuilder::setLastLineJustifyLimit(const LengthSpec &ls)
{
  lengthSpecC("last-line-justify-limit", ls);
}

void SgmlFOTBuilder::setJustifyGlyphSpaceMaxAdd(const LengthSpec &ls)
{
  lengthSpecC("justify-glyph-space-max-add", ls);
}

void SgmlFOTBuilder::setJustifyGlyphSpaceMaxRemove(const LengthSpec &ls)
{
  lengthSpecC("justify-glyph-space-max-remove", ls);
}

void SgmlFOTBuilder::setTableCornerRadius(const LengthSpec &ls)
{
  lengthSpecC("table-corner-radius", ls);
}

void SgmlFOTBuilder::setBoxCornerRadius(const LengthSpec &ls)
{
  lengthSpecC("box-corner-radius", ls);
}

void SgmlFOTBuilder::setMarginaliaSep(const LengthSpec &ls)
{
  lengthSpecC("marginalia-sep", ls);
}

void SgmlFOTBuilder::lengthSpecC(const char *s, const LengthSpec &ls)
{
  ics_ << ' ' << s << '=' << ls;
}

void SgmlFOTBuilder::setMinPreLineSpacing(const OptLengthSpec &ols)
{
  optLengthSpecC("min-pre-line-spacing", ols);
}

void SgmlFOTBuilder::setMinPostLineSpacing(const OptLengthSpec &ols)
{
  optLengthSpecC("min-post-line-spacing", ols);
}

void SgmlFOTBuilder::setMinLeading(const OptLengthSpec &ols)
{
  optLengthSpecC("min-leading", ols);
}

void SgmlFOTBuilder::optLengthSpecC(const char *s, const OptLengthSpec &ols)
{
  if (ols.hasLength)
    lengthSpecC(s, ols.length);
  else
    ics_ << ' ' << s << "=\"\"";
}

void SgmlFOTBuilder::setLines(Symbol sym)
{
  symbolC("lines", sym);
}

void SgmlFOTBuilder::setQuadding(Symbol sym)
{
  symbolC("quadding", sym);
}

void SgmlFOTBuilder::setDisplayAlignment(Symbol sym)
{
  symbolC("display-alignment", sym);
}

void SgmlFOTBuilder::setFieldAlign(Symbol sym)
{
  symbolC("field-align", sym);
}

void SgmlFOTBuilder::setLineJoin(Symbol sym)
{
  symbolC("line-join", sym);
}

void SgmlFOTBuilder::setLineCap(Symbol sym)
{
  symbolC("line-cap", sym);
}

void SgmlFOTBuilder::setLineNumberSide(Symbol sym)
{
  symbolC("line-number-side", sym);
}

void SgmlFOTBuilder::setKernMode(Symbol sym)
{
  symbolC("kern-mode", sym);
}

void SgmlFOTBuilder::setInputWhitespaceTreatment(Symbol sym)
{
  symbolC("input-whitespace-treatment", sym);
}

void SgmlFOTBuilder::setFillingDirection(Symbol sym)
{
  symbolC("filling-direction", sym);
}

void SgmlFOTBuilder::setWritingMode(Symbol sym)
{
  symbolC("writing-mode", sym);
}

void SgmlFOTBuilder::setLastLineQuadding(Symbol sym)
{
  symbolC("last-line-quadding", sym);
}

void SgmlFOTBuilder::setMathDisplayMode(Symbol sym)
{
  symbolC("math-display-mode", sym);
}

void SgmlFOTBuilder::setScriptPreAlign(Symbol sym)
{
  symbolC("script-pre-align", sym);
}

void SgmlFOTBuilder::setScriptPostAlign(Symbol sym)
{
  symbolC("script-post-align", sym);
}

void SgmlFOTBuilder::setScriptMidSupAlign(Symbol sym)
{
  symbolC("script-mid-sup-align", sym);
}

void SgmlFOTBuilder::setScriptMidSubAlign(Symbol sym)
{
  symbolC("script-mid-sub-align", sym);
}

void SgmlFOTBuilder::setNumeratorAlign(Symbol sym)
{
  symbolC("numerator-align", sym);
}

void SgmlFOTBuilder::setDenominatorAlign(Symbol sym)
{
  symbolC("denominator-align", sym);
}

void SgmlFOTBuilder::setGridPositionCellType(Symbol sym)
{
  symbolC("grid-position-cell-type", sym);
}

void SgmlFOTBuilder::setGridColumnAlignment(Symbol sym)
{
  symbolC("grid-column-alignment", sym);
}

void SgmlFOTBuilder::setGridRowAlignment(Symbol sym)
{
  symbolC("grid-row-alignment", sym);
}

void SgmlFOTBuilder::setBoxType(Symbol sym)
{
  symbolC("box-type", sym);
}

void SgmlFOTBuilder::setGlyphAlignmentMode(Symbol sym)
{
  symbolC("glyph-alignment-mode", sym);
}

void SgmlFOTBuilder::setBoxBorderAlignment(Symbol sym)
{
  symbolC("box-border-alignment", sym);
}

void SgmlFOTBuilder::setCellRowAlignment(Symbol sym)
{
  symbolC("cell-row-alignment", sym);
}

void SgmlFOTBuilder::setBorderAlignment(Symbol sym)
{
  symbolC("border-alignment", sym);
}

void SgmlFOTBuilder::setSidelineSide(Symbol sym)
{
  symbolC("sideline-side", sym);
}

void SgmlFOTBuilder::setHyphenationKeep(Symbol sym)
{
  symbolC("hyphenation-keep", sym);
}

void SgmlFOTBuilder::setPositionPreference(Symbol sym)
{
  symbolC("position-preference", sym);
}

void SgmlFOTBuilder::setFontStructure(Symbol sym)
{
  symbolC("font-structure", sym);
}

void SgmlFOTBuilder::setFontProportionateWidth(Symbol sym)
{
  symbolC("font-proportionate-width", sym);
}

void SgmlFOTBuilder::setCellCrossed(Symbol sym)
{
  symbolC("cell-crossed", sym);
}

void SgmlFOTBuilder::setMarginaliaSide(Symbol sym)
{
  symbolC("marginalia-side", sym);
}

void SgmlFOTBuilder::symbolC(const char *name, Symbol sym)
{
  ics_ << ' ' << name << '=' << sym;
}

void SgmlFOTBuilder::setColor(const DeviceRGBColor &color)
{
  ics_ << " color.red=" << int(color.red)
       << " color.green=" << int(color.green)
       << " color.blue=" << int(color.blue);
}

void SgmlFOTBuilder::setBackgroundColor()
{
  ics_ << " background-color=0";
}

void SgmlFOTBuilder::setBackgroundColor(const DeviceRGBColor &color)
{
  ics_ << " background-color.red=" << int(color.red)
       << " background-color.green=" << int(color.green)
       << " background-color.blue=" << int(color.blue);
}

void SgmlFOTBuilder::setPageWidth(Length units)
{
  lengthC("page-width", units);
}

void SgmlFOTBuilder::setPageHeight(Length units)
{
  lengthC("page-height", units);
}

void SgmlFOTBuilder::setLeftMargin(Length units)
{
  lengthC("left-margin", units);
}

void SgmlFOTBuilder::setRightMargin(Length units)
{
  lengthC("right-margin", units);
}

void SgmlFOTBuilder::setTopMargin(Length units)
{
  lengthC("top-margin", units);
}

void SgmlFOTBuilder::setBottomMargin(Length units)
{
  lengthC("bottom-margin", units);
}

void SgmlFOTBuilder::setHeaderMargin(Length units)
{
  lengthC("header-margin", units);
}

void SgmlFOTBuilder::setFooterMargin(Length units)
{
  lengthC("footer-margin", units);
}

void SgmlFOTBuilder::setLineThickness(Length units)
{
  lengthC("line-thickness", units);
}

void SgmlFOTBuilder::setCellBeforeRowMargin(Length units)
{
  lengthC("cell-before-row-margin", units);
}

void SgmlFOTBuilder::setCellAfterRowMargin(Length units)
{
  lengthC("cell-after-row-margin", units);
}

void SgmlFOTBuilder::setCellBeforeColumnMargin(Length units)
{
  lengthC("cell-before-column-margin", units);
}

void SgmlFOTBuilder::setCellAfterColumnMargin(Length units)
{
  lengthC("cell-after-column-margin", units);
}

void SgmlFOTBuilder::setLineSep(Length units)
{
  lengthC("line-sep", units);
}

void SgmlFOTBuilder::setBoxSizeBefore(Length units)
{
  lengthC("box-size-before", units);
}

void SgmlFOTBuilder::setBoxSizeAfter(Length units)
{
  lengthC("box-size-after", units);
}

void SgmlFOTBuilder::lengthC(const char *s, Length units)
{
  ics_ << ' ' << s << '=' << units;
}

void SgmlFOTBuilder::setLayer(long n)
{
  integerC("layer", n);
}

void SgmlFOTBuilder::setBackgroundLayer(long n)
{
  integerC("background-layer", n);
}

void SgmlFOTBuilder::setBorderPriority(long n)
{
  integerC("border-priority", n);
}

void SgmlFOTBuilder::setLineRepeat(long n)
{
  integerC("line-repeat", n);
}

void SgmlFOTBuilder::setSpan(long n)
{
  integerC("span", n);
}

void SgmlFOTBuilder::setMinLeaderRepeat(long n)
{
  integerC("min-leader-repeat", n);
}

void SgmlFOTBuilder::setHyphenationRemainCharCount(long n)
{
  integerC("hyphenation-remain-char-count", n);
}

void SgmlFOTBuilder::setHyphenationPushCharCount(long n)
{
  integerC("hyphenation-push-char-count", n);
}

void SgmlFOTBuilder::setWidowCount(long n)
{
  integerC("widow-count", n);
}

void SgmlFOTBuilder::setOrphanCount(long n)
{
  integerC("orphan-count", n);
}

void SgmlFOTBuilder::integerC(const char *s, long n)
{
  ics_ << ' ' << s << '=' << n;
}

void SgmlFOTBuilder::setExpandTabs(long n)
{
  integerC("expand-tabs", n);
}

void SgmlFOTBuilder::setHyphenationLadderCount(long n)
{
  integerC("hyphenation-ladder-count", n);
}

void SgmlFOTBuilder::setCountry(Letter2 code)
{
  ics_ << " country=";
  if (code)
    ics_ << char((code >> 8) & 0xff) << char(code & 0xff);
  else
    ics_ << '0';
}

void SgmlFOTBuilder::setLanguage(Letter2 code)
{
  ics_ << " language=";
  if (code)
    ics_ << char((code >> 8) & 0xff) << char(code & 0xff);
  else
    ics_ << '0';
}

void SgmlFOTBuilder::setBackgroundTile(PublicId pubid)
{
  publicIdC("background-tile", pubid);
}

void SgmlFOTBuilder::setLineBreakingMethod(PublicId pubid)
{
  publicIdC("line-breaking-method", pubid);
}

void SgmlFOTBuilder::setLineCompositionMethod(PublicId pubid)
{
  publicIdC("line-composition-method", pubid);
}

void SgmlFOTBuilder::setImplicitBidiMethod(PublicId pubid)
{
  publicIdC("implicit-bidi-method", pubid);
}

void SgmlFOTBuilder::setGlyphSubstMethod(PublicId pubid)
{
  publicIdC("glyph-subst-method", pubid);
}

void SgmlFOTBuilder::setGlyphReorderMethod(PublicId pubid)
{
  publicIdC("glyph-reorder-method", pubid);
}

void SgmlFOTBuilder::setHyphenationMethod(PublicId pubid)
{
  publicIdC("hyphenation-method", pubid);
}

void SgmlFOTBuilder::setTableAutoWidthMethod(PublicId pubid)
{
  publicIdC("table-auto-width-method", pubid);
}

void SgmlFOTBuilder::setFontName(PublicId pubid)
{
  publicIdC("font-name", pubid);
}

void SgmlFOTBuilder::publicIdC(const char *s, PublicId pubid)
{
  ics_ << ' ' << s << '=';
  if (pubid)
    ics_ << '"' << pubid << '"';
  else
    ics_ << '0';
}

void SgmlFOTBuilder::setBorderPresent(bool b)
{
  boolC("border-present", b);
}

void SgmlFOTBuilder::setInhibitLineBreaks(bool b)
{
  boolC("border-present", b);
}

void SgmlFOTBuilder::setHyphenate(bool b)
{
  boolC("hyphenate", b);
}

void SgmlFOTBuilder::setKern(bool b)
{
  boolC("kern", b);
}

void SgmlFOTBuilder::setLigature(bool b)
{
  boolC("ligature", b);
}

void SgmlFOTBuilder::setScoreSpaces(bool b)
{
  boolC("score-spaces", b);
}

void SgmlFOTBuilder::setFloatOutMarginalia(bool b)
{
  boolC("float-out-marginalia", b);
}

void SgmlFOTBuilder::setFloatOutSidelines(bool b)
{
  boolC("float-out-sidelines", b);
}

void SgmlFOTBuilder::setFloatOutLineNumbers(bool b)
{
  boolC("float-out-line-numbers", b);
}

void SgmlFOTBuilder::setCellBackground(bool b)
{
  boolC("cell-background", b);
}

void SgmlFOTBuilder::setSpanWeak(bool b)
{
  boolC("span-weak", b);
}

void SgmlFOTBuilder::setIgnoreRecordEnd(bool b)
{
  boolC("ignore-record-end", b);
}

void SgmlFOTBuilder::setNumberedLines(bool b)
{
  boolC("numbered-lines", b);
}

void SgmlFOTBuilder::setHangingPunct(bool b)
{
  boolC("hanging-punct", b);
}

void SgmlFOTBuilder::setBoxOpenEnd(bool b)
{
  boolC("box-open-end", b);
}

void SgmlFOTBuilder::setTruncateLeader(bool b)
{
  boolC("truncate-leader", b);
}

void SgmlFOTBuilder::setAlignLeader(bool b)
{
  boolC("align-leader", b);
}

void SgmlFOTBuilder::setTablePartOmitMiddleHeader(bool b)
{
  boolC("table-part-omit-middle-header", b);
}

void SgmlFOTBuilder::setTablePartOmitMiddleFooter(bool b)
{
  boolC("table-part-omit-middle-footer", b);
}

void SgmlFOTBuilder::setBorderOmitAtBreak(bool b)
{
  boolC("border-omit-at-break", b);
}

void SgmlFOTBuilder::setPrincipalModeSimultaneous(bool b)
{
  boolC("principal-mode-simultaneous", b);
}

void SgmlFOTBuilder::setMarginaliaKeepWithPrevious(bool b)
{
  boolC("marginalia-keep-with-previous", b);
}

void SgmlFOTBuilder::setGridEquidistantRows(bool b)
{
  boolC("grid-equidistant-rows", b);
}

void SgmlFOTBuilder::setGridEquidistantColumns(bool b)
{
  boolC("grid-equidistant-columns", b);
}

void SgmlFOTBuilder::boolC(const char *s, bool b)
{
  ics_ << ' ' << s << (b ? "=1" : "=0");
}

void SgmlFOTBuilder::setEscapementSpaceBefore(const InlineSpace &is)
{
  inlineSpaceC("escapement-space-before", is);
}

void SgmlFOTBuilder::setEscapementSpaceAfter(const InlineSpace &is)
{
  inlineSpaceC("escapement-space-after", is);
}

void SgmlFOTBuilder::setGlyphSubstTable(const Vector<ConstPtr<GlyphSubstTable> > &tables)
{
  if (tables.size() == 0) {
    ics_ << " glyph-subst-table=0";
    return;
  }
  static const char idPrefix[] = "gst";
  ics_ << " glyph-subst-table=\"";
  for (size_t i = 0; i < tables.size(); i++) {
    unsigned long n = tables[i]->uniqueId;
    if (n >= glyphSubstTableDefined_.size()) {
      for (size_t j = glyphSubstTableDefined_.size(); j < n + 1; j++)
	glyphSubstTableDefined_.push_back(0);
    }
    if (!glyphSubstTableDefined_[n]) {
      glyphSubstTableDefined_[n] = 1;
      os() << "<define-glyph-subst-table id=" << idPrefix << n << '>' << RE;
      const Vector<GlyphId> &pairs = tables[i]->pairs;
      for (size_t i = 0; i < pairs.size(); i += 2)
	os() << "<glyph-subst old=" << pairs[i] << " new=" << pairs[i + 1] << '>' << RE;
      os() << "</define-glyph-subst-table>";
    }
    if (i > 0)
      ics_ << ' ';
    ics_ << idPrefix << n;
  }
  ics_ << '"';
}

void SgmlFOTBuilder::inlineSpaceC(const char *s, const InlineSpace &is)
{
  if (is.min)
    ics_ << ' ' << s << ".min=" << is.min;
  if (is.nominal)
    ics_ << ' ' << s << ".nominal=" << is.nominal;
  if (is.max)
    ics_ << ' ' << s << ".max=" << is.max;
}

void SgmlFOTBuilder::displayNIC(const DisplayNIC &nic)
{
  if (nic.keepWithPrevious)
    os() << " keep-with-previous";
  if (nic.keepWithNext)
    os() << " keep-with-next";
  if (nic.mayViolateKeepBefore)
    os() << " may-violate-keep-before";
  if (nic.mayViolateKeepAfter)
    os() << " may-violate-keep-after";
  if (nic.keep != symbolFalse)
    os() << " keep=" << nic.keep;
  if (nic.breakBefore != symbolFalse)
    os() << " break-before=" << nic.breakBefore;
  if (nic.breakAfter != symbolFalse)
    os() << " break-after=" << nic.breakAfter;
  displaySpaceNIC("space-before", nic.spaceBefore);
  displaySpaceNIC("space-after", nic.spaceAfter);
}

void SgmlFOTBuilder::displaySpaceNIC(const char *s, const DisplaySpace &ds)
{
  if (ds.min)
    os() << ' ' << s << ".min=" << ds.min;
  if (ds.nominal)
    os() << ' ' << s << ".nominal=" << ds.nominal;
  if (ds.max)
    os() << ' ' << s << ".max=" << ds.max;
  if (ds.force)
    os() << ' ' << s << ".priority=force";
  else if (ds.priority)
    os() << ' ' << s << ".priority=" << ds.priority;
  if (!ds.conditional)
    os() << ' ' << s << ".conditional=0";
}

void SgmlFOTBuilder::inlineNIC(const InlineNIC &nic)
{
  if (nic.breakBeforePriority)
    os() << " break-before-priority=" << nic.breakBeforePriority;
  if (nic.breakAfterPriority)
    os() << " break-after-priority=" << nic.breakAfterPriority;
}

void SgmlFOTBuilder::startParagraph(const ParagraphNIC &nic)
{
  os() << "<paragraph";
  displayNIC(nic);
  outputIcs();
  os() << ">" << RE;
}

void SgmlFOTBuilder::endParagraph()
{
  endFlow("paragraph");
}

void SgmlFOTBuilder::paragraphBreak(const ParagraphNIC &nic)
{
  os() << "<paragraph-break";
  displayNIC(nic);
  outputIcs();
  os() << '>' << RE;
}

void SgmlFOTBuilder::startDisplayGroup(const DisplayGroupNIC &nic)
{
  os() << "<display-group";
  if (nic.hasCoalesceId)
    os() << " coalesce-id=\"" << nic.coalesceId << '"';
  displayNIC(nic);
  outputIcs();
  os() << ">" << RE;
}

void SgmlFOTBuilder::endDisplayGroup()
{
  endFlow("display-group");
}

void SgmlFOTBuilder::simpleFlowObj(const char *name)
{
  os() << '<' << name;
  outputIcs();
  os() << '>' << RE;
}

void SgmlFOTBuilder::startSimpleFlowObj(const char *name)
{
  os() << '<' << name;
  outputIcs();
  os() << ">" << RE;
}

void SgmlFOTBuilder::startPortFlow(const char *name)
{
  os() << "<" << name << ">" << RE;
}

void SgmlFOTBuilder::endFlow(const char *name)
{
  os() << "</" << name << ">" << RE;
}

void SgmlFOTBuilder::startSequence()
{
  startSimpleFlowObj("sequence");
}
 
void SgmlFOTBuilder::endSequence()
{
  endFlow("sequence");
}

void SgmlFOTBuilder::startLineField(const LineFieldNIC &)
{
  startSimpleFlowObj("line-field");
}
 
void SgmlFOTBuilder::endLineField()
{
  endFlow("line-field");
}

void SgmlFOTBuilder::startScroll()
{
  startSimpleFlowObj("scroll");
}

void SgmlFOTBuilder::endScroll()
{
  endFlow("scroll");
}

void SgmlFOTBuilder::startLink(const Address &addr)
{
  os() << "<link";
  outputIcs();
  switch (addr.type) {
  case Address::sgmlDocument:
    os() << " destination.system-id=\"" << addr.params[0] << '"';
    os() << " destination.idref=\"" << addr.params[1] << '"';
    break;
  case Address::resolvedNode:
    {
      unsigned long n;
      if (addr.node->elementIndex(n) == accessOK)
	os() << " destination.element-index=" << n;
      break;
    }
  case Address::idref:
    os() << " destination.idref=\"" << addr.params[0] << '"';
    break;
  case Address::entity:
    os() << " destination.entity=\"" << addr.params[0] << '"';
    break;
  default:
    break;
  }
  os() << '>' << RE;
}

void SgmlFOTBuilder::endLink()
{
  endFlow("link");
}

void SgmlFOTBuilder::startMarginalia()
{
  startSimpleFlowObj("marginalia");
}

void SgmlFOTBuilder::endMarginalia()
{
  endFlow("marginalia");
}

void SgmlFOTBuilder::startMultiModeSerial(const MultiMode *principalMode)
{
  startSimpleFlowObj("multi-mode");
  if (principalMode) {
    os() << "<multi-mode.mode";
    if (principalMode->hasDesc)
      os() << " desc=\"" << principalMode->desc << '"';
    os() << '>' << RE;
  }
  multiModeHasPrincipalMode_.push_back(principalMode != 0);
}

void SgmlFOTBuilder::endMultiModeSerial()
{
  if (multiModeHasPrincipalMode_.back())
    endFlow("multi-mode.mode");
  multiModeHasPrincipalMode_.resize(multiModeHasPrincipalMode_.size() - 1);
  endFlow("multi-mode");
}

void SgmlFOTBuilder::startMultiModeMode(const MultiMode &mode)
{
  if (multiModeHasPrincipalMode_.back()) {
    endFlow("multi-mode.mode");
    multiModeHasPrincipalMode_.back() = 0;
  }
  os() << "<multi-mode.mode name=\"" << mode.name << '"';
  if (mode.hasDesc)
    os() << " desc=\"" << mode.desc << '"';
  os () << '>' << RE;
}

void SgmlFOTBuilder::endMultiModeMode()
{
  endFlow("multi-mode.mode");
}

void SgmlFOTBuilder::startScore(Symbol type)
{
  const char *s = symbolName(type);
  if (s) {
    os() << "<score " << s;
    outputIcs();
    os() << '>' << RE;
  }
}

void SgmlFOTBuilder::startScore(const LengthSpec &ls)
{
  os() << "<score type.length-spec=" << ls;
  outputIcs();
  os() << '>' << RE;
}

void SgmlFOTBuilder::startScore(Char c)
{
  os() << "<score type.char=\"";
  os().put(c);
  os() << '"';
  outputIcs();
  os() << '>' << RE;
}

void SgmlFOTBuilder::endScore()
{
  endFlow("score");
}

void SgmlFOTBuilder::startSimplePageSequence()
{
  startSimpleFlowObj("simple-page-sequence");
}
 
void SgmlFOTBuilder::endSimplePageSequence()
{
  endFlow("simple-page-sequence");
}

void SgmlFOTBuilder::startSimplePageSequenceHeaderFooter(unsigned flags)
{
  os() << "<simple-page-sequence.header-footer";
  if ((flags & (headerHF|footerHF)) == headerHF)
    os() << " header";
  else
    os() << " footer";
  switch (flags & (leftHF|centerHF|rightHF)) {
  case leftHF:
    os() << " left";
    break;
  case rightHF:
    os() << " right";
    break;
  case centerHF:
    os() << " center";
    break;
  }
  if ((flags & (firstHF|otherHF)) == otherHF)
    os() << " first";
  else
    os() << " other";
  if ((flags & (frontHF|backHF)) == frontHF)
    os() << " front";
  else
    os() << " back";
  os() << '>' << RE;
}

void SgmlFOTBuilder::endSimplePageSequenceHeaderFooter(unsigned)
{
  endFlow("simple-page-sequence.header-footer");
}

void SgmlFOTBuilder::pageNumber()
{
  os() << "<page-number>";
}

void SgmlFOTBuilder::startMathSequence()
{
  startSimpleFlowObj("math-sequence");
}

void SgmlFOTBuilder::endMathSequence()
{
  endFlow("math-sequence");
}

void SgmlFOTBuilder::startFractionSerial()
{
  startSimpleFlowObj("fraction");
}

void SgmlFOTBuilder::endFractionSerial()
{
  endFlow("fraction");
}

void SgmlFOTBuilder::startFractionNumerator()
{
  startPortFlow("fraction.numerator");
}

void SgmlFOTBuilder::endFractionNumerator()
{
  endFlow("fraction.numerator");
}

void SgmlFOTBuilder::startFractionDenominator()
{
  startPortFlow("fraction.denominator");
}

void SgmlFOTBuilder::endFractionDenominator()
{
  endFlow("fraction.denominator");
}

void SgmlFOTBuilder::fractionBar()
{
  simpleFlowObj("fraction.fraction-bar");
}

void SgmlFOTBuilder::startUnmath()
{
  startSimpleFlowObj("unmath");
}

void SgmlFOTBuilder::endUnmath()
{
  endFlow("unmath");
}

void SgmlFOTBuilder::startSuperscript()
{
  startSimpleFlowObj("superscript");
}

void SgmlFOTBuilder::endSuperscript()
{
  endFlow("superscript");
}

void SgmlFOTBuilder::startSubscript()
{
  startSimpleFlowObj("subscript");
}

void SgmlFOTBuilder::endSubscript()
{
  endFlow("subscript");
}

void SgmlFOTBuilder::startScriptSerial()
{
  startSimpleFlowObj("script");
  startPortFlow("script.principal");
}

void SgmlFOTBuilder::endScriptSerial()
{
  endFlow("script");
}

void SgmlFOTBuilder::startScriptPreSup()
{
  endFlow("script.principal");
  startPortFlow("script.pre-sup");
}

void SgmlFOTBuilder::endScriptPreSup()
{
  endFlow("script.pre-sup");
}

void SgmlFOTBuilder::startScriptPreSub()
{
  startPortFlow("script.pre-sub");
}

void SgmlFOTBuilder::endScriptPreSub()
{
  endFlow("script.pre-sub");
}

void SgmlFOTBuilder::startScriptPostSup()
{
  startPortFlow("script.post-sup");
}

void SgmlFOTBuilder::endScriptPostSup()
{
  endFlow("script.post-sup");
}

void SgmlFOTBuilder::startScriptPostSub()
{
  startPortFlow("script.post-sub");
}

void SgmlFOTBuilder::endScriptPostSub()
{
  endFlow("script.post-sub");
}

void SgmlFOTBuilder::startScriptMidSup()
{
  startPortFlow("script.mid-sup");
}

void SgmlFOTBuilder::endScriptMidSup()
{
  endFlow("script.mid-sup");
}

void SgmlFOTBuilder::startScriptMidSub()
{
  startPortFlow("script.mid-sub");
}

void SgmlFOTBuilder::endScriptMidSub()
{
  endFlow("script.mid-sub");
}

void SgmlFOTBuilder::startMarkSerial()
{
  startSimpleFlowObj("mark");
  startPortFlow("mark.principal");
}

void SgmlFOTBuilder::endMarkSerial()
{
  endFlow("mark");
}

void SgmlFOTBuilder::startMarkOver()
{
  endFlow("mark.principal");
  startPortFlow("mark.over-mark");
}

void SgmlFOTBuilder::endMarkOver()
{
  endFlow("mark.over-mark");
}

void SgmlFOTBuilder::startMarkUnder()
{
  startPortFlow("mark.under-mark");
}

void SgmlFOTBuilder::endMarkUnder()
{
  endFlow("mark.under-mark");
}

void SgmlFOTBuilder::startFenceSerial()
{
  startSimpleFlowObj("fence");
  startPortFlow("fence.principal");
}

void SgmlFOTBuilder::endFenceSerial()
{
  endFlow("fence");
}

void SgmlFOTBuilder::startFenceOpen()
{
  endFlow("fence.principal");
  startPortFlow("fence.open");
}

void SgmlFOTBuilder::endFenceOpen()
{
  endFlow("fence.open");
}

void SgmlFOTBuilder::startFenceClose()
{
  startPortFlow("fence.close");
}

void SgmlFOTBuilder::endFenceClose()
{
  endFlow("fence.close");
}

void SgmlFOTBuilder::startRadicalSerial()
{
  startSimpleFlowObj("radical");
}

void SgmlFOTBuilder::endRadicalSerial()
{
  endFlow("radical");
}

void SgmlFOTBuilder::startRadicalDegree()
{
  endFlow("radical.principal");
  startPortFlow("radical.degree");
}

void SgmlFOTBuilder::endRadicalDegree()
{
  endFlow("radical.degree");
}

void SgmlFOTBuilder::radicalRadical(const CharacterNIC &nic)
{
  os() << "<radical.radical";
  characterNIC(nic);
  outputIcs();
  os() << '>' << RE;
  startPortFlow("radical.principal");
}

void SgmlFOTBuilder::radicalRadicalDefaulted()
{
  startPortFlow("radical.principal");
}

void SgmlFOTBuilder::startMathOperatorSerial()
{
  startSimpleFlowObj("math-operator");
  startPortFlow("math-operator.principal");
}

void SgmlFOTBuilder::endMathOperatorSerial()
{
  endFlow("math-operator");
}

void SgmlFOTBuilder::startMathOperatorOperator()
{
  endFlow("math-operator.principal");
  startPortFlow("math-operator.operator");
}

void SgmlFOTBuilder::endMathOperatorOperator()
{
  endFlow("math-operator.operator");
}

void SgmlFOTBuilder::startMathOperatorLowerLimit()
{
  startPortFlow("math-operator.lower-limit");
}

void SgmlFOTBuilder::endMathOperatorLowerLimit()
{
  endFlow("math-operator.lower-limit");
}

void SgmlFOTBuilder::startMathOperatorUpperLimit()
{
  startPortFlow("math-operator.upper-limit");
}

void SgmlFOTBuilder::endMathOperatorUpperLimit()
{
  endFlow("math-operator.upper-limit");
}

void SgmlFOTBuilder::startGrid(const GridNIC &nic)
{
  os() << "<grid";
  if (nic.nColumns)
    os() << " grid-n-columns=" << (unsigned long)nic.nColumns;
  if (nic.nRows)
    os() << " grid-n-rows=" << (unsigned long)nic.nRows;
  outputIcs();
  os() << '>' << RE;
}

void SgmlFOTBuilder::endGrid()
{
  endFlow("grid");
}

void SgmlFOTBuilder::startGridCell(const GridCellNIC &nic)
{
  os() << "<grid-cell";
  if (nic.columnNumber)
    os() << " column-number=" << (unsigned long)nic.columnNumber;
  if (nic.rowNumber)
    os() << " row-number=" << (unsigned long)nic.rowNumber;
  outputIcs();
  os() << '>' << RE;
}

void SgmlFOTBuilder::endGridCell()
{
  endFlow("grid-cell");
}

void SgmlFOTBuilder::externalGraphic(const ExternalGraphicNIC &nic)
{
  // FIXME quote as necessary
  os() << "<external-graphic entity-system-id=\""
       << nic.entitySystemId
       << '"'
       << " notation-system-id=\""
       << nic.notationSystemId
       << '"';
  if (nic.scaleType != symbolFalse)
    os() << " scale=" << nic.scaleType;
  else
    os() << " scale.x=" << nic.scale[0] << " scale.y=" << nic.scale[1];
  if (nic.hasMaxWidth)
    os() << " max-width=" << nic.maxWidth;
  if (nic.hasMaxHeight)
    os() << " max-height=" << nic.maxHeight;
  if (nic.isDisplay) {
    os() << " display";
    displayNIC(nic);
  }
  else {
    if (nic.escapementDirection != symbolFalse)
      os() << " escapement-direction=" << nic.escapementDirection;
    if (nic.positionPointX)
      os() << " position-point-x=" << nic.positionPointX;
    if (nic.positionPointY)
      os() << " position-point-y=" << nic.positionPointY;
    inlineNIC(nic);
  }
  outputIcs();
  os() << '>' << RE;
}

void SgmlFOTBuilder::rule(const RuleNIC &nic)
{
  const char *s = symbolName(nic.orientation);
  if (!s)
    return;
  os() << "<rule " << s;
  switch (nic.orientation) {
  case symbolHorizontal:
  case symbolVertical:
    displayNIC(nic);
    break;
  default:
    inlineNIC(nic);
    break;
  }
  if (nic.hasLength)
    os() << " length=" << nic.length;
  outputIcs();
  os() << '>' << RE;
}

void SgmlFOTBuilder::startLeader(const LeaderNIC &nic)
{
  os() << "<leader";
  if (nic.hasLength)
    os() << " length=" << nic.length;
  inlineNIC(nic);
  outputIcs();
  os() << '>' << RE;
}

void SgmlFOTBuilder::endLeader()
{
  endFlow("leader");
}

void SgmlFOTBuilder::startSideline()
{
  startSimpleFlowObj("sideline");
}

void SgmlFOTBuilder::endSideline()
{
  endFlow("sideline");
}

void SgmlFOTBuilder::startBox(const BoxNIC &nic)
{
  os() << "<box";
  if (nic.isDisplay) {
    os() << " display";
    displayNIC(nic);
  }
  else
    inlineNIC(nic);
  outputIcs();
  os() << '>' << RE;
}

void SgmlFOTBuilder::endBox()
{
  endFlow("box");
}

void SgmlFOTBuilder::alignmentPoint()
{
  simpleFlowObj("alignment-point");
}

void SgmlFOTBuilder::startTable(const TableNIC &nic)
{
  os() << "<table";
  switch (nic.widthType) {
  case TableNIC::widthExplicit:
    os() << " width=" << nic.width;
    break;
  case TableNIC::widthMinimum:
    os() << " minimum-width";
    break;
  default:
    break;
  }
  displayNIC(nic);
  outputIcs();
  os() << '>' << RE;
}

void SgmlFOTBuilder::endTable()
{
  endFlow("table");
}

void SgmlFOTBuilder::tableBeforeRowBorder()
{
  simpleFlowObj("table.before-row-border");
}

void SgmlFOTBuilder::tableAfterRowBorder()
{
  simpleFlowObj("table.after-row-border");
}

void SgmlFOTBuilder::tableBeforeColumnBorder()
{
  simpleFlowObj("table.before-column-border");
}

void SgmlFOTBuilder::tableAfterColumnBorder()
{
  simpleFlowObj("table.after-column-border");
}

void SgmlFOTBuilder::tableColumn(const TableColumnNIC &nic)
{
  os() << "<table-column column-number="
       << (unsigned long)nic.columnIndex + 1;
  if (nic.nColumnsSpanned != 1)
    os() << " n-columns-spanned=" << (unsigned long)nic.nColumnsSpanned;
  if (nic.hasWidth)
    os() << " width=" << nic.width;
  outputIcs();
  os() << '>' << RE;
}

void SgmlFOTBuilder::startTablePartSerial(const TablePartNIC &nic)
{
  os() << "<table-part";
  displayNIC(nic);
  outputIcs();
  os() << '>' << RE;
  startPortFlow("table-part.principal");
}

void SgmlFOTBuilder::endTablePartSerial()
{
  endFlow("table-part");
}

void SgmlFOTBuilder::startTablePartHeader()
{
  endFlow("table-part.principal");
  os() << "<table-part.header>" << RE;
}

void SgmlFOTBuilder::endTablePartHeader()
{
  endFlow("table-part.header");
}

void SgmlFOTBuilder::startTablePartFooter()
{
  os() << "<table-part.footer>" << RE;
}

void SgmlFOTBuilder::endTablePartFooter()
{
  endFlow("table-part.footer");
}

void SgmlFOTBuilder::startTableRow()
{
  startSimpleFlowObj("table-row");
}

void SgmlFOTBuilder::endTableRow()
{
  endFlow("table-row");
}

void SgmlFOTBuilder::startTableCell(const TableCellNIC &nic)
{
  if (nic.missing)
    os() << "<table-cell column-number=0";
  else {
    os() << "<table-cell column-number="
         << (unsigned long)nic.columnIndex + 1;
    if (nic.nColumnsSpanned != 1)
      os() << " n-columns-spanned=" << (unsigned long)nic.nColumnsSpanned;
    if (nic.nRowsSpanned != 1)
      os() << " n-rows-spanned=" << (unsigned long)nic.nRowsSpanned;
  }
  outputIcs();
  os() << '>' << RE;
}

void SgmlFOTBuilder::endTableCell()
{
  endFlow("table-cell");
}

void SgmlFOTBuilder::tableCellBeforeRowBorder()
{
  simpleFlowObj("table-cell.before-row-border");
}

void SgmlFOTBuilder::tableCellAfterRowBorder()
{
  simpleFlowObj("table-cell.after-row-border");
}

void SgmlFOTBuilder::tableCellBeforeColumnBorder()
{
  simpleFlowObj("table-cell.before-column-border");
}

void SgmlFOTBuilder::tableCellAfterColumnBorder()
{
  simpleFlowObj("table-cell.after-column-border");
}

void SgmlFOTBuilder::startNode(const NodePtr &node, const StringC &mode,
			       RuleType ruleType)
{
  nodeDepth_++;
  unsigned long n;
  if (node->elementIndex(n) == accessOK) {
    os() << "<element index=" << n;
    GroveString id; 
    if (node->getId(id) == accessOK)
      os() << " id=\"" << id << '"'; // FIXME handle possibility of quotes/non-SGML characters
    if (mode.size())
      os() << " mode=" << mode;
    switch (ruleType) {
    case ruleNone:
      break;
    case ruleQuery:
      os() << " rule-query";
      break;
    case ruleDefault:
      os() << " rule-default";
      break;
    case ruleRoot:
      os() << " rule-root";
      break;
    case ruleId:
      os() << " rule-id";
      break;
    default:
      ASSERT(ruleType >= ruleElement1);
      os() << " rule-element";
      if (ruleType > ruleElement1)
	os() << " n-qual-gi=" << (unsigned long)(ruleType - ruleElement1);
    }
    os() << '>' << RE;
    elementNodeDepth_ = nodeDepth_;
  }
}

void SgmlFOTBuilder::endNode()
{
  if (nodeDepth_ == elementNodeDepth_) {
    endFlow("element");
    if (--elementNodeDepth_ == 1)
      elementNodeDepth_ = 0;
  }
  --nodeDepth_;
}

void SgmlFOTBuilder::currentNodePageNumber(const NodePtr &node)
{
  os() << "<node-page-number";
  unsigned long n;
  if (node->elementIndex(n) == accessOK)
    os() << " element-index=" << n;
  os() << ">";
}

#ifdef DSSSL_NAMESPACE
}
#endif
