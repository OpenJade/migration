// TeXFOTBuilder.cxx: a Generic TeX backend for Jade
// Written by David Megginson <dmeggins@microstar.com>

#include "config.h"
#include "TeXFOTBuilder.h"

#ifdef SP_SHORT_HEADERS
#include <strstrea.h>
#else
#include <strstream.h>
#endif

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

class TeXFOTBuilder : public SerialFOTBuilder {

public:

  //////////////////////////////////////////////////////////////////////
  // Constructor and destructor.
  //////////////////////////////////////////////////////////////////////

  TeXFOTBuilder(streambuf *);
  ~TeXFOTBuilder();

  //////////////////////////////////////////////////////////////////////
  // Atomic flow objects
  //////////////////////////////////////////////////////////////////////

  void characters(const Char *, size_t);
  void character(const CharacterNIC &);
  void paragraphBreak(const ParagraphNIC &);
  void externalGraphic(const ExternalGraphicNIC &);
  void rule(const RuleNIC &);
  void alignmentPoint();
  void formattingInstruction(const StringC &);

  //////////////////////////////////////////////////////////////////////
  // Non-atomic flow objects
  //////////////////////////////////////////////////////////////////////

  void startSequence();
  void endSequence();
  void startLineField(const LineFieldNIC &);
  void endLineField();
  void startParagraph(const ParagraphNIC &);
  void endParagraph();
  void startDisplayGroup(const DisplayGroupNIC &);
  void endDisplayGroup();
  void startScroll();
  void endScroll();
  void startLink(const Address &);
  void endLink();
  void startMarginalia();
  void endMarginalia();
#if 0
  void startMultiMode(const MultiMode *principalPort,  // null if no principal port
			      const Vector<MultiMode> &namedPorts,
			      Vector<FOTBuilder *> &);
  void endMultiMode();
#endif
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
  // Tables
  void startTable(const TableNIC &);
  void endTable();
  // A call for each border is made immediately
  // after startTable(), each preceded by any appropriate set*() calls.
  void tableBeforeRowBorder();
  void tableAfterRowBorder();
  void tableBeforeColumnBorder();
  void tableAfterColumnBorder();
  void startTablePartSerial(const TablePartNIC &);
  void endTablePartSerial();
  void tableColumn(const TableColumnNIC &);
  void startTableRow();
  void endTableRow();
  void startTableCell(const TableCellNIC &);
  void endTableCell();
  void tableCellBeforeRowBorder();
  void tableCellAfterRowBorder();
  void tableCellBeforeColumnBorder();
  void tableCellAfterColumnBorder();
  // Implementation must set numerator and denominator
  // must be set to non-null values.
  void startFraction(FOTBuilder *&numerator, FOTBuilder *&denominator);
  void endFraction();
  void startSimplePageSequence();
  void endSimplePageSequence();
  // Headers and footers are treated like a separate port.
  void startSimplePageSequenceHeaderFooter(unsigned);
  void endSimplePageSequenceHeaderFooter(unsigned);
  // page-number sosofo
  void pageNumber();

  //////////////////////////////////////////////////////////////////////
  // Inherited characteristics
  // Set the value of the characteristic for the next flow object.
  // Inherited characteristics that are not explicitly set have
  // the same value as the parent flow object.
  //////////////////////////////////////////////////////////////////////

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
  void setMarginaliaSep(const LengthSpec &);
  void setLines(Symbol);
  void setQuadding(Symbol);
  void setDisplayAlignment(Symbol);
  void setFieldAlign(Symbol);
  void setColor(const DeviceRGBColor &);
  void setBackgroundColor(); // background of #f
  void setBackgroundColor(const DeviceRGBColor &);
  void setBorderPresent(bool);
  void setLineThickness(Length);
  void setCellBeforeRowMargin(Length);
  void setCellAfterRowMargin(Length);
  void setCellBeforeColumnMargin(Length);
  void setCellAfterColumnMargin(Length);
  void setLineSep(Length);
  void setBoxSizeBefore(Length);
  void setBoxSizeAfter(Length);
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
  void setLineJoin(Symbol);
  void setLineCap(Symbol);
  void setLineNumberSide(Symbol);
  void setKernMode(Symbol);
  void setInputWhitespaceTreatment(Symbol);
  void setFillingDirection(Symbol);
  void setWritingMode(Symbol);
  void setLastLineQuadding(Symbol);
  void setMathDisplayMode(Symbol);
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
  // 0 means #f
  void setExpandTabs(long);
  void setHyphenationLadderCount(long);
  // public id or #f
  void setBackgroundTile(PublicId);
  void setLineBreakingMethod(PublicId);
  void setLineCompositionMethod(PublicId);
  void setImplicitBidiMethod(PublicId);
  void setGlyphSubstMethod(PublicId);
  void setGlyphReorderMethod(PublicId);
  void setHyphenationMethod(PublicId);
  void setTableAutoWidthMethod(PublicId);
  void setFontName(PublicId);
  // Two-letter code
  void setLanguage(Letter2);
  void setCountry(Letter2);
  // For simple page sequence
  void setPageWidth(Length);
  void setPageHeight(Length);
  void setLeftMargin(Length);
  void setRightMargin(Length);
  void setTopMargin(Length);
  void setBottomMargin(Length);
  void setHeaderMargin(Length);
  void setFooterMargin(Length);


private:
				// Variables.
  ostream fileout_;
  ostrstream * stringout_;

				// Functions.
  ostream &os();
  void insertAtomic(const char *name);
  void startGroup(const char *name);
  void endGroup(const char *name);
  void setlength(const char *,Length);
  void set(const char *,const StringC &);
  void set(const char *,Symbol);
  void set(const char *,const LengthSpec &);
  void set(const char *,double);
  void set(const char *,const DeviceRGBColor &);
  void set(const char *,bool);
  void set(const char *,long);
  void set(const char *,long unsigned int);
  void set(const char *,PublicId);
  void set(const char *,Letter2);
  void set(const char *,const DisplaySpace &);
  void set(const char *,const GlyphId &);

  void setInlineNIC(const InlineNIC &);
  void setDisplayNIC(const DisplayNIC &);

  void dumpInherited();
};

#define MAYBESET(name,value,default) (value!=default?(set(name,value),0):0)


//
// Get the current output stream.
//
inline
ostream &TeXFOTBuilder::os()
{
  return fileout_;
}


//
// Define an output operator for StringC
//
static ostream &operator<<(ostream &os, const StringC &s)
{
  for (size_t i = 0; i < s.size(); i++)
    os << char(s[i]);
  return os;
}


FOTBuilder *makeTeXFOTBuilder(streambuf *sbuf)
{
  return new TeXFOTBuilder(sbuf);
}

////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
////////////////////////////////////////////////////////////////////////

TeXFOTBuilder::TeXFOTBuilder(streambuf *sbuf)
: fileout_(sbuf), stringout_(new ostrstream)
{
  os() << "\\startFOT{}\n";
}

TeXFOTBuilder::~TeXFOTBuilder()
{
  os() << "\\endFOT{}\n";
}

//////////////////////////////////////////////////////////////////////
// Atomic flow objects
//////////////////////////////////////////////////////////////////////

void TeXFOTBuilder::characters(const Char *s, size_t n)
{
  for (; n > 0; n--, s++) {
				// Since TeX has only 256 characters
				// by default, two-byte characters
				// will need special treatment.
    if (*s > 255) {
      set("Ch",(unsigned long)(*s));
      insertAtomic("Character");
    } else {
				// Otherwise, check for special
				// TeX escapes.
      switch(*s) {
      default:
	os() << char(*s);
	break;
      case '\\':
      case '{':
      case '}':
      case '$':
      case '&':
      case '#':
      case '^':
      case '_':
      case '~':
      case '%':
	os() << "\\char" << int(*s) << "{}";
	break;
      case '\r':
	os() << '\n';
	break;
      }
    }
  }
}

void TeXFOTBuilder::character(const CharacterNIC &nic)
{
  if (nic.specifiedC & nic.cChar)
    set("Ch",(unsigned long)nic.ch);
  if (nic.specifiedC & nic.cGlyphId)
    set("GlyphId",nic.glyphId);
  if (nic.specifiedC & nic.cBreakBeforePriority)
    set("BreakBeforePriority",nic.breakBeforePriority);
  if (nic.specifiedC & nic.cBreakAfterPriority)
    set("BreakAfterPriority",nic.breakAfterPriority);
  if (nic.specifiedC & nic.cMathClass)
    set("MathClass",nic.mathClass);
  if (nic.specifiedC & nic.cMathFontPosture)
    set("MathFontPosture",nic.mathFontPosture);
  if (nic.specifiedC & nic.cScript)
    set("Script",(long unsigned int)nic.script);
  if (nic.specifiedC & nic.cIsDropAfterLineBreak)
    set("IsDropAfterLineBreak",nic.isDropAfterLineBreak);
  if (nic.specifiedC & nic.cIsDropUnlessBeforeLineBreak)
    set("IsDropUnlessBeforeLineBreak",nic.isDropUnlessBeforeLineBreak);
  if (nic.specifiedC & nic.cIsPunct)
    set("IsPunct",nic.isPunct);
  if (nic.specifiedC & nic.cIsInputWhitespace)
    set("IsInputWhiteSpace",nic.isInputWhitespace);
  if (nic.specifiedC & nic.cIsInputTab)
    set("IsInputTab",nic.isInputTab);
  if (nic.specifiedC & nic.cIsRecordEnd)
    set("IsRecordEnd",nic.isRecordEnd);
  if (nic.specifiedC & nic.cIsSpace)
    set("IsSpace",nic.isSpace);
  MAYBESET("StretchFactor",nic.stretchFactor,0L);
  insertAtomic("Character");
}

void TeXFOTBuilder::paragraphBreak(const ParagraphNIC &nic)
{
  setDisplayNIC(nic);
  insertAtomic("ParagraphBreak");
}

void TeXFOTBuilder::externalGraphic(const ExternalGraphicNIC &nic)
{
  setDisplayNIC(nic);
  setInlineNIC(nic);
				// ExternalGraphicNIC
  MAYBESET("IsDisplay",nic.isDisplay,symbolFalse);
  MAYBESET("ScaleType",nic.scaleType,symbolMaxUniform);
  if (nic.scaleType == symbolFalse) {
    set("ScaleX",nic.scale[0]);
    set("ScaleY",nic.scale[1]);
  }
  set("EntitySystemId",nic.entitySystemId);
  set("NotationSystemId",nic.notationSystemId);
  if(nic.hasMaxWidth)
    set("MaxWidth",nic.maxWidth);
  if (nic.hasMaxHeight)
    set("MaxHeight",nic.maxHeight);
  if (!nic.isDisplay) {
    set("EscapementDirection",nic.escapementDirection);
    set("PositionPointX",nic.positionPointX);
    set("PositionPointY",nic.positionPointY);
  }
  insertAtomic("ExternalGraphic");
}

void TeXFOTBuilder::rule(const RuleNIC &nic)
{
  setDisplayNIC(nic);
  setInlineNIC(nic);
				// Rule NIC
  set("Orientation",nic.orientation);
  if (nic.hasLength)
    set("Length",nic.length);
  insertAtomic("Rule");
}

void TeXFOTBuilder::alignmentPoint()
{
  insertAtomic("AlignmentPoint");
}

void TeXFOTBuilder::formattingInstruction (const StringC &instr)
{
  os() << instr;
}
  

//////////////////////////////////////////////////////////////////////
// Non-atomic flow objects
//////////////////////////////////////////////////////////////////////

void TeXFOTBuilder::startSequence()
{
  startGroup("Sequence");
}

void TeXFOTBuilder::endSequence()
{
  endGroup("Sequence");
}

void TeXFOTBuilder::startLineField(const LineFieldNIC &nic)
{
  setInlineNIC(nic);
  startGroup("LineField");
}

void TeXFOTBuilder::endLineField()
{
  endGroup("LineField");
}

void TeXFOTBuilder::startParagraph(const ParagraphNIC &nic)
{
  setDisplayNIC(nic);
  startGroup("Paragraph");
}

void TeXFOTBuilder::endParagraph()
{
  endGroup("Paragraph");
}

void TeXFOTBuilder::startDisplayGroup(const DisplayGroupNIC &nic)
{
  setDisplayNIC(nic);
				// DisplayGroupNIC
  if (nic.hasCoalesceId) 
    set("CoalesceId",nic.coalesceId);
  startGroup("DisplayGroup");
}

void TeXFOTBuilder::endDisplayGroup()
{
  endGroup("DisplayGroup");
}

void TeXFOTBuilder::startScroll()
{
  startGroup("Scroll");
}

void TeXFOTBuilder::endScroll()
{
  endGroup("Scroll");
}

void TeXFOTBuilder::startLink(const Address &)
{
  // Incomplete
  startGroup("Link");
}

void TeXFOTBuilder::endLink()
{
  endGroup("Link");
}

void TeXFOTBuilder::startMarginalia()
{
  startGroup("Marginalia");
}

void TeXFOTBuilder::endMarginalia()
{
  endGroup("Marginalia");
}

void TeXFOTBuilder::startScore(Char score)
{
  startGroup("Score");
}

void TeXFOTBuilder::startScore(const LengthSpec &len)
{
  set("ScoreLength",len);
  startGroup("Score");
}

void TeXFOTBuilder::startScore(Symbol type)
{
  set("ScoreType",type);
  startGroup("Score");
}

void TeXFOTBuilder::endScore()
{
  endGroup("Score");
}

void TeXFOTBuilder::startLeader(const LeaderNIC &nic)
{
  setInlineNIC(nic);
  if (nic.hasLength)
    set("Length",nic.length);
  startGroup("Leader");
}

void TeXFOTBuilder::endLeader()
{
  endGroup("Leader");
}

void TeXFOTBuilder::startSideline()
{
  startGroup("SideLine");
}

void TeXFOTBuilder::endSideline()
{
  endGroup("SideLine");
}

void TeXFOTBuilder::startBox(const BoxNIC &nic)
{
  setDisplayNIC(nic);
  setInlineNIC(nic);
				// BoxNIC
  MAYBESET("IsDisplay",nic.isDisplay,symbolFalse);
  startGroup("Box");
}

void TeXFOTBuilder::endBox()
{
  endGroup("Box");
}

// Tables
void TeXFOTBuilder::startTable(const TableNIC &nic)
{
  setDisplayNIC(nic);
				// TableNIC
  switch (nic.widthType) {
  case TableNIC::widthFull:
    set("TableWidth","full");
    break;
  case TableNIC::widthMinimum:
    set("TableWidth","minimum");
    break;
  case TableNIC::widthExplicit:
    set("TableWidth",nic.width);
    break;
  }
  startGroup("Table");
}

void TeXFOTBuilder::endTable()
{
  endGroup("Table");
}

// A call for each border is made immediately
// after startTable(), each preceded by any appropriate set*() calls.
void TeXFOTBuilder::tableBeforeRowBorder()
{
  insertAtomic("TableBeforeRowBorder");
}

void TeXFOTBuilder::tableAfterRowBorder()
{
  insertAtomic("TableAfterRowBorder");
}

void TeXFOTBuilder::tableBeforeColumnBorder()
{
  insertAtomic("TableBeforeColumnBorder");
}

void TeXFOTBuilder::tableAfterColumnBorder()
{
  insertAtomic("TableAfterColumnBorder");
}

void TeXFOTBuilder::startTablePartSerial(const TablePartNIC &nic)
{
  setDisplayNIC(nic);
  startGroup("TablePart");
}

void TeXFOTBuilder::endTablePartSerial()
{
  endGroup("TablePart");
}

void TeXFOTBuilder::tableColumn(const TableColumnNIC &nic)
{
  MAYBESET("ColumnIndex",long(nic.columnIndex),0);
  MAYBESET("NColumnsSpanned",long(nic.nColumnsSpanned),1);
  if (nic.hasWidth)
    set("Width",nic.width);
  insertAtomic("TableColumn");
}

void TeXFOTBuilder::startTableRow()
{
  startGroup("TableRow");
}

void TeXFOTBuilder::endTableRow()
{
  endGroup("TableRow");
}

void TeXFOTBuilder::startTableCell(const TableCellNIC &nic)
{
  MAYBESET("ColumnIndex",long(nic.columnIndex),0);
  MAYBESET("NColumnsSpanned",long(nic.nColumnsSpanned),1);
  MAYBESET("NRowsSpanned",long(nic.nRowsSpanned),1);
  startGroup("TableCell");
}

void TeXFOTBuilder::endTableCell()
{
  endGroup("TableCell");
}

void TeXFOTBuilder::tableCellBeforeRowBorder()
{
  insertAtomic("TableCellBeforeRowBorder");
}

void TeXFOTBuilder::tableCellAfterRowBorder()
{
  insertAtomic("TableCellAfterRowBorder");
}

void TeXFOTBuilder::tableCellBeforeColumnBorder()
{
  insertAtomic("TableCellBeforeColumnBorder");
}

void TeXFOTBuilder::tableCellAfterColumnBorder()
{
  insertAtomic("TableCellAfterColumnBorder");
}

// Implementation must set numerator and denominator
// must be set to non-null values.
void TeXFOTBuilder::startFraction(FOTBuilder *&numerator,
				  FOTBuilder *&denominator)
{
  startGroup("Fraction");
}

void TeXFOTBuilder::endFraction()
{
  endGroup("Fraction");
}

void TeXFOTBuilder::startSimplePageSequence()
{
  startGroup("SimplePageSequence");
}

void TeXFOTBuilder::endSimplePageSequence()
{
  endGroup("SimplePageSequence");
}

// These aren't real flow objects, so handle them a little
// differently.
void TeXFOTBuilder::startSimplePageSequenceHeaderFooter(unsigned flags)
{
  os() << "  \\SimplePageSequence";
  if ((flags & (firstHF|otherHF)) == firstHF)
    os() << "First";
  else
    os() << "Other";
  if ((flags & (frontHF|backHF)) == frontHF)
    os() << "Front";
  else
    os() << "Back";
  switch (flags & (leftHF|centerHF|rightHF)) {
  case leftHF:
    os() << "Left";
    break;
  case centerHF:
    os() << "Center";
    break;
  case rightHF:
    os() << "Right";
    break;
  }
  if ((flags & (headerHF|footerHF)) == headerHF)
    os() << "Header";
  else
    os() << "Footer";
  os() << '{';
}

void TeXFOTBuilder::endSimplePageSequenceHeaderFooter(unsigned)
{
  os() << "}%\n";
}

// page-number sosofo
void TeXFOTBuilder::pageNumber()
{
  insertAtomic("PageNumber");
}


//////////////////////////////////////////////////////////////////////
// Inherited characteristics
// Set the value of the characteristic for the next flow object.
// Inherited characteristics that are not explicitly set have
// the same value as the parent flow object.
//////////////////////////////////////////////////////////////////////

void TeXFOTBuilder::setFontSize(Length size)
{
  setlength("FontSize",size);
}

void TeXFOTBuilder::setFontFamilyName(const StringC &name)
{
  set("FontFamilyName",name);
}

void TeXFOTBuilder::setFontWeight(Symbol weight)
{
  set("FontWeight",weight);
}

void TeXFOTBuilder::setFontPosture(Symbol posture)
{
  set("FontPosture",posture);
}

void TeXFOTBuilder::setStartIndent(const LengthSpec &indent)
{
  set("StartIndent",indent);
}

void TeXFOTBuilder::setEndIndent(const LengthSpec &indent)
{
  set("EndIndent",indent);
}

void TeXFOTBuilder::setFirstLineStartIndent(const LengthSpec &indent)
{
  set("FirstLineStartIndent",indent);
}

void TeXFOTBuilder::setLastLineEndIndent(const LengthSpec &indent)
{
  set("LastLineEndIndent",indent);
}

void TeXFOTBuilder::setLineSpacing(const LengthSpec &spacing)
{
  set("LineSpacing",spacing);
}

void TeXFOTBuilder::setFieldWidth(const LengthSpec &width)
{
  set("FieldWidth",width);
}

void TeXFOTBuilder::setMarginaliaSep(const LengthSpec &sep)
{
  set("MarginaliaSep",sep);
}

void TeXFOTBuilder::setLines(Symbol lines)
{
  set("Lines",lines);
}

void TeXFOTBuilder::setQuadding(Symbol quadding)
{
  set("Quadding",quadding);
}

void TeXFOTBuilder::setDisplayAlignment(Symbol align)
{
  set("DisplayAlignment",align);
}

void TeXFOTBuilder::setFieldAlign(Symbol align)
{
  set("FieldAlign",align);
}

void TeXFOTBuilder::setColor(const DeviceRGBColor &color)
{
  set("Color",color);
}

void TeXFOTBuilder::setBackgroundColor()
{
  set("BackgroundColor",symbolFalse);
}

void TeXFOTBuilder::setBackgroundColor(const DeviceRGBColor &color)
{
  set("BackgroundColor",color);
}

void TeXFOTBuilder::setBorderPresent(bool flag)
{
  set("BorderPresent",flag);
}

void TeXFOTBuilder::setLineThickness(Length thickness)
{
  setlength("LineThickness",thickness);
}

void TeXFOTBuilder::setCellBeforeRowMargin(Length margin)
{
  setlength("CellBeforeRowMargin",margin);
}

void TeXFOTBuilder::setCellAfterRowMargin(Length margin)
{
  setlength("CellAfterRowMargin",margin);
}

void TeXFOTBuilder::setCellBeforeColumnMargin(Length margin)
{
  setlength("CellBeforeColumnMargin",margin);
}

void TeXFOTBuilder::setCellAfterColumnMargin(Length margin)
{
  setlength("CellAfterColumnMargin",margin);
}

void TeXFOTBuilder::setLineSep(Length sep)
{
  setlength("LineSep",sep);
}

void TeXFOTBuilder::setBoxSizeBefore(Length size)
{
  setlength("BoxSizeBefore",size);
}

void TeXFOTBuilder::setBoxSizeAfter(Length size)
{
  setlength("BoxSizeAfter",size);
}

void TeXFOTBuilder::setPositionPointShift(const LengthSpec &shift)
{
  set("PositionPointShift",shift);
}

void TeXFOTBuilder::setStartMargin(const LengthSpec &margin)
{
  set("StartMargin",margin);
}

void TeXFOTBuilder::setEndMargin(const LengthSpec &margin)
{
  set("EndMargin",margin);
}

void TeXFOTBuilder::setSidelineSep(const LengthSpec &sep)
{
  set("SidelineSep",sep);
}

void TeXFOTBuilder::setAsisWrapIndent(const LengthSpec &indent)
{
  set("AsisWrapIndent",indent);
}

void TeXFOTBuilder::setLineNumberSep(const LengthSpec &sep)
{
  set("LineNumberSep",sep);
}

void TeXFOTBuilder::setLastLineJustifyLimit(const LengthSpec &limit)
{
  set("LastLineJustifyLimit",limit);
}

void TeXFOTBuilder::setJustifyGlyphSpaceMaxAdd(const LengthSpec &max)
{
  set("JustifyGlyphSpaceMaxAdd",max);
}

void TeXFOTBuilder::setJustifyGlyphSpaceMaxRemove(const LengthSpec &max)
{
  set("JustifyGlyphSpaceMaxRemove",max);
}

void TeXFOTBuilder::setTableCornerRadius(const LengthSpec &radius)
{
  set("TableCornerRadius",radius);
}

void TeXFOTBuilder::setBoxCornerRadius(const LengthSpec &radius)
{
  set("BoxCornerRadius",radius);
}

void TeXFOTBuilder::setInhibitLineBreaks(bool flag)
{
  set("InhibitLineBreaks",flag);
}

void TeXFOTBuilder::setHyphenate(bool flag)
{
  set("Hyphenate",flag);
}

void TeXFOTBuilder::setKern(bool flag)
{
  set("Kern",flag);
}

void TeXFOTBuilder::setLigature(bool flag)
{
  set("Ligature",flag);
}

void TeXFOTBuilder::setScoreSpaces(bool flag)
{
  set("ScoreSpaces",flag);
}

void TeXFOTBuilder::setFloatOutMarginalia(bool flag)
{
  set("FloatOutMarginalia",flag);
}

void TeXFOTBuilder::setFloatOutSidelines(bool flag)
{
  set("FloatOutSidelines",flag);
}

void TeXFOTBuilder::setFloatOutLineNumbers(bool flag)
{
  set("FloatOutLineNumbers",flag);
}

void TeXFOTBuilder::setCellBackground(bool flag)
{
  set("CellBackground",flag);
}

void TeXFOTBuilder::setSpanWeak(bool flag)
{
  set("SpanWeak",flag);
}

void TeXFOTBuilder::setIgnoreRecordEnd(bool flag)
{
  set("IgnoreRecordEnd",flag);
}

void TeXFOTBuilder::setNumberedLines(bool flag)
{
  set("NumberedLines",flag);
}

void TeXFOTBuilder::setHangingPunct(bool flag)
{
  set("HangingPunct",flag);
}

void TeXFOTBuilder::setBoxOpenEnd(bool flag)
{
  set("BoxOpenEnd",flag);
}

void TeXFOTBuilder::setTruncateLeader(bool flag)
{
  set("TruncateLeader",flag);
}

void TeXFOTBuilder::setAlignLeader(bool flag)
{
  set("AlignLeader",flag);
}

void TeXFOTBuilder::setTablePartOmitMiddleHeader(bool flag)
{
  set("TablePartOmitMiddleHeader",flag);
}

void TeXFOTBuilder::setTablePartOmitMiddleFooter(bool flag)
{
  set("TablePartOmitMiddleFooter",flag);
}

void TeXFOTBuilder::setBorderOmitAtBreak(bool flag)
{
  set("BorderOmitAtBreak",flag);
}

void TeXFOTBuilder::setPrincipalModeSimultaneous(bool flag)
{
  set("PrincipalModeSimultaneous",flag);
}

void TeXFOTBuilder::setMarginaliaKeepWithPrevious(bool flag)
{
  set("MarginaliaKeepWithPrevious",flag);
}

void TeXFOTBuilder::setLineJoin(Symbol join)
{
  set("LineJoin",join);
}

void TeXFOTBuilder::setLineCap(Symbol cap)
{
  set("LineCap",cap);
}

void TeXFOTBuilder::setLineNumberSide(Symbol side)
{
  set("LineNumberSide",side);
}

void TeXFOTBuilder::setKernMode(Symbol mode)
{
  set("KernMode",mode);
}

void TeXFOTBuilder::setInputWhitespaceTreatment(Symbol treatment)
{
  set("InputWhitespaceTreatment",treatment);
}

void TeXFOTBuilder::setFillingDirection(Symbol direction)
{
  set("FillingDirection",direction);
}

void TeXFOTBuilder::setWritingMode(Symbol mode)
{
  set("WritingMode",mode);
}

void TeXFOTBuilder::setLastLineQuadding(Symbol quadding)
{
  set("LastLineQuadding",quadding);
}

void TeXFOTBuilder::setMathDisplayMode(Symbol mode)
{
  set("MathDisplayMode",mode);
}

void TeXFOTBuilder::setBoxType(Symbol type)
{
  set("BoxType",type);
}

void TeXFOTBuilder::setGlyphAlignmentMode(Symbol mode)
{
  set("GlyphAlignmentMode",mode);
}

void TeXFOTBuilder::setBoxBorderAlignment(Symbol align)
{
  set("BoxBorderAlignment",align);
}

void TeXFOTBuilder::setCellRowAlignment(Symbol align)
{
  set("CellRowAlignment",align);
}

void TeXFOTBuilder::setBorderAlignment(Symbol align)
{
  set("BorderAlignment",align);
}

void TeXFOTBuilder::setSidelineSide(Symbol side)
{
  set("SidelineSide",side);
}

void TeXFOTBuilder::setHyphenationKeep(Symbol keep)
{
  set("HyphenationKeep",keep);
}

void TeXFOTBuilder::setPositionPreference(Symbol pref)
{
  set("PositionPreference",pref);
}

void TeXFOTBuilder::setFontStructure(Symbol structure)
{
  set("FontStructure",structure);
}

void TeXFOTBuilder::setFontProportionateWidth(Symbol width)
{
  set("FontProportionateWidth",width);
}

void TeXFOTBuilder::setCellCrossed(Symbol crossed)
{
  set("CellCrossed",crossed);
}

void TeXFOTBuilder::setMarginaliaSide(Symbol side)
{
  set("MarginaliaSide",side);
}

void TeXFOTBuilder::setLayer(long n)
{
  set("Layer",n);
}

void TeXFOTBuilder::setBackgroundLayer(long n)
{
  set("BackgroundLayer",n);
}

void TeXFOTBuilder::setBorderPriority(long n)
{
  set("BorderPriority",n);
}

void TeXFOTBuilder::setLineRepeat(long n)
{
  set("LineRepeat",n);
}

void TeXFOTBuilder::setSpan(long n)
{
  set("Span",n);
}

void TeXFOTBuilder::setMinLeaderRepeat(long n)
{
  set("MinLeaderRepeat",n);
}

void TeXFOTBuilder::setHyphenationRemainCharCount(long n)
{
  set("HyphenationRemainCharCount",n);
}

void TeXFOTBuilder::setHyphenationPushCharCount(long n)
{
  set("HyphenationPushCharCount",n);
}

void TeXFOTBuilder::setWidowCount(long n)
{
  set("WidowCount",n);
}

void TeXFOTBuilder::setOrphanCount(long n)
{
  set("OrphanCount",n);
}

// 0 means #f
void TeXFOTBuilder::setExpandTabs(long n)
{
  set("ExpandTabs",n);
}

void TeXFOTBuilder::setHyphenationLadderCount(long n)
{
  set("HyphenationLadderCount",n);
}

// public id or #f
void TeXFOTBuilder::setBackgroundTile(PublicId id)
{
  set("BackgroundTile",id);
}

void TeXFOTBuilder::setLineBreakingMethod(PublicId id)
{
  set("LineBreakingMethod",id);
}

void TeXFOTBuilder::setLineCompositionMethod(PublicId id)
{
  set("LineCompositionMethod",id);
}

void TeXFOTBuilder::setImplicitBidiMethod(PublicId id)
{
  set("ImplicitBidiMethod",id);
}

void TeXFOTBuilder::setGlyphSubstMethod(PublicId id)
{
  set("GlyphSubstMethod",id);
}

void TeXFOTBuilder::setGlyphReorderMethod(PublicId id)
{
  set("GlyphReorderMethod",id);
}

void TeXFOTBuilder::setHyphenationMethod(PublicId id)
{
  set("HyphenationMethod",id);
}

void TeXFOTBuilder::setTableAutoWidthMethod(PublicId id)
{
  set("TableAutoWidthMethod",id);
}

void TeXFOTBuilder::setFontName(PublicId id)
{
  set("FontName",id);
}

// Two-letter code
void TeXFOTBuilder::setLanguage(Letter2 language)
{
  set("Language",language);
}

void TeXFOTBuilder::setCountry(Letter2 country)
{
  set("Country",country);
}

// For simple page sequence
void TeXFOTBuilder::setPageWidth(Length width)
{
  setlength("PageWidth",width);
}

void TeXFOTBuilder::setPageHeight(Length height)
{
  setlength("PageHeight",height);
}

void TeXFOTBuilder::setLeftMargin(Length margin)
{
  setlength("LeftMargin",margin);
}

void TeXFOTBuilder::setRightMargin(Length margin)
{
  setlength("RightMargin",margin);
}

void TeXFOTBuilder::setTopMargin(Length margin)
{
  setlength("TopMargin",margin);
}

void TeXFOTBuilder::setBottomMargin(Length margin)
{
  setlength("BottomMargin",margin);
}

void TeXFOTBuilder::setHeaderMargin(Length margin)
{
  setlength("HeaderMargin",margin);
}

void TeXFOTBuilder::setFooterMargin(Length margin)
{
  setlength("FooterMargin",margin);
}

////////////////////////////////////////////////////////////////////////
// Private member functions.
////////////////////////////////////////////////////////////////////////

//
// Insert an atomic flow object.
//
void TeXFOTBuilder::insertAtomic(const char *name)
{
  os() << "\\insert" << name << '{';
  dumpInherited();
  os() << "}%\n";
}

//
// Start a non-atomic flow object.
//
void TeXFOTBuilder::startGroup(const char *name)
{
  os() << "\\start" << name << '{';
  dumpInherited();
  os() << "}%\n";
}

//
// End a non-atomic flow object.
//
void TeXFOTBuilder::endGroup(const char *name)
{
  os() << "\\end" << name << "%\n";
}

//
// Set a Length (needs a different name to avoid conflict
// with long.
//
void TeXFOTBuilder::setlength(const char *name,Length size)
{
  *stringout_ << "\\def\\" << name << '{'
	      << float(size/1000.0)
	      << "pt}%\n";
}

//
// Set a StringC.
//
void TeXFOTBuilder::set(const char *name,const StringC &value)
{
  *stringout_ << "\\def\\" << name << '{'
	      << value
	      << "}%\n";
}

//
// Set a Symbol.
//
void TeXFOTBuilder::set(const char *name,Symbol symbol)
{
  const char * symbolName;

  switch (symbol) {
  case symbolFalse:
    symbolName = "false";
    break;
  case symbolTrue:
    symbolName = "true";
    break;
  case symbolNotApplicable:
    symbolName = "notapplicable";
    break;
  case symbolUltraCondensed:
    symbolName = "ultracondensed";
    break;
  case symbolExtraCondensed:
    symbolName = "extracondensed";
    break;
  case symbolCondensed:
    symbolName = "condensed";
    break;
  case symbolSemiCondensed:
    symbolName = "semicondensed";
    break;
  case symbolUltraLight:
    symbolName = "ultralight";
    break;
  case symbolExtraLight:
    symbolName = "extralight";
    break;
  case symbolLight:
    symbolName = "light";
    break;
  case symbolSemiLight:
    symbolName = "semilight";
    break;
  case symbolMedium:
    symbolName = "medium";
    break;
  case symbolSemiExpanded:
    symbolName = "semiexpanded";
    break;
  case symbolExpanded:
    symbolName = "expanded";
    break;
  case symbolExtraExpanded:
    symbolName = "extraexpanded";
    break;
  case symbolUltraExpanded:
    symbolName = "ultraexpanded";
    break;
  case symbolSemiBold:
    symbolName = "semibold";
    break;
  case symbolBold:
    symbolName = "bold";
    break;
  case symbolExtraBold:
    symbolName = "extrabold";
    break;
  case symbolUltraBold:
    symbolName = "ultrabold";
    break;
  case symbolUpright:
    symbolName = "upright";
    break;
  case symbolOblique:
    symbolName = "oblique";
    break;
  case symbolBackSlantedOblique:
    symbolName = "backslantedoblique";
    break;
  case symbolItalic:
    symbolName = "italic";
    break;
  case symbolBackSlantedItalic:
    symbolName = "backslanteditalic";
    break;
  case symbolStart:
    symbolName = "start";
    break;
  case symbolEnd:
    symbolName = "end";
    break;
  case symbolCenter:
    symbolName = "center";
    break;
  case symbolJustify:
    symbolName = "justify";
    break;
  case symbolSpreadInside:
    symbolName = "spreadinside";
    break;
  case symbolSpreadOutside:
    symbolName = "spreadoutside";
    break;
  case symbolPageInside:
    symbolName = "pageinside";
    break;
  case symbolPageOutside:
    symbolName = "pageoutside";
    break;
  case symbolWrap:
    symbolName = "wrap";
    break;
  case symbolAsis:
    symbolName = "asis";
    break;
  case symbolAsisWrap:
    symbolName = "asiswrap";
    break;
  case symbolAsisTruncate:
    symbolName = "asistruncate";
    break;
  case symbolNone:
    symbolName = "none";
    break;
  case symbolBefore:
    symbolName = "before";
    break;
  case symbolThrough:
    symbolName = "through";
    break;
  case symbolAfter:
    symbolName = "after";
    break;
  case symbolTopToBottom:
    symbolName = "toptobottom";
    break;
  case symbolLeftToRight:
    symbolName = "lefttoright";
    break;
  case symbolBottomToTop:
    symbolName = "bottomtotop";
    break;
  case symbolRightToLeft:
    symbolName = "righttoleft";
    break;
  case symbolInside:
    symbolName = "inside";
    break;
  case symbolOutside:
    symbolName = "outside";
    break;
  case symbolHorizontal:
    symbolName = "horizontal";
    break;
  case symbolVertical:
    symbolName = "vertical";
    break;
  case symbolEscapement:
    symbolName = "escapement";
    break;
  case symbolLineProgression:
    symbolName = "lineprogression";
    break;
  case symbolMath:
    symbolName = "math";
    break;
  case symbolOrdinary:
    symbolName = "ordinary";
    break;
  case symbolOperator:
    symbolName = "operator";
    break;
  case symbolBinary:
    symbolName = "binary";
    break;
  case symbolRelation:
    symbolName = "relation";
    break;
  case symbolOpening:
    symbolName = "opening";
    break;
  case symbolClosing:
    symbolName = "closing";
    break;
  case symbolPunctuation:
    symbolName = "punctuation";
    break;
  case symbolInner:
    symbolName = "inner";
    break;
  case symbolSpace:
    symbolName = "space";
    break;
  case symbolPage:
    symbolName = "page";
    break;
  case symbolColumnSet:
    symbolName = "columnset";
    break;
  case symbolColumn:
    symbolName = "column";
    break;
  case symbolMax:
    symbolName = "max";
    break;
  case symbolMaxUniform:
    symbolName = "maxuniform";
    break;
  case symbolMiter:
    symbolName = "miter";
    break;
  case symbolRound:
    symbolName = "round";
    break;
  case symbolJoin:
    symbolName = "join";
    break;
  case symbolButt:
    symbolName = "butt";
    break;
  case symbolSquare:
    symbolName = "square";
    break;
  case symbolLoose:
    symbolName = "loose";
    break;
  case symbolNormal:
    symbolName = "normal";
    break;
  case symbolKern:
    symbolName = "kern";
    break;
  case symbolTight:
    symbolName = "tight";
    break;
  case symbolTouch:
    symbolName = "touch";
    break;
  case symbolPreserve:
    symbolName = "preserve";
    break;
  case symbolCollapse:
    symbolName = "collapse";
    break;
  case symbolIgnore:
    symbolName = "ignore";
    break;
  case symbolRelative:
    symbolName = "relative";
    break;
  case symbolDisplay:
    symbolName = "display";
    break;
  case symbolInline:
    symbolName = "inline";
    break;
  case symbolBorder:
    symbolName = "border";
    break;
  case symbolBackground:
    symbolName = "background";
    break;
  case symbolBoth:
    symbolName = "both";
    break;
  case symbolBase:
    symbolName = "base";
    break;
  case symbolFont:
    symbolName = "font";
    break;
  case symbolTop:
    symbolName = "top";
    break;
  case symbolBottom:
    symbolName = "bottom";
    break;
  case symbolSpread:
    symbolName = "spread";
    break;
  case symbolSolid:
    symbolName = "solid";
    break;
  case symbolOutline:
    symbolName = "outline";
    break;
  case symbolWith:
    symbolName = "with";
    break;
  case symbolAgainst:
    symbolName = "against";
    break;
  case symbolForce:
    symbolName = "force";
    break;
  }

  *stringout_ << "\\def\\" << name << '{' << symbolName << "}%\n";
}

//
// Set a LengthSpec.
//
void TeXFOTBuilder::set(const char *name,const LengthSpec &spec)
{
  *stringout_ << "\\def\\" << name << '{'
	      << float(spec.length/1000.0)
	      << "pt}";
  *stringout_ << "\\def\\" << name << "Factor{"
	      << spec.displaySizeFactor
	      << "}%\n";
}

//
// Set a double.
//
void TeXFOTBuilder::set(const char *name,double n)
{
  *stringout_ << "\\def\\" << name << '{' << n << "}%\n";
}

//
// Set a DeviceRGBColor.
//
void TeXFOTBuilder::set(const char *name, const DeviceRGBColor &color)
{
  *stringout_ << "\\def\\" << name << "Red{" << color.red << "}%\n";
  *stringout_ << "\\def\\" << name << "Green{" << color.green << "}%\n";
  *stringout_ << "\\def\\" << name << "Blue{" << color.blue << "}%\n";
}

//
// Set a bool.
//
void TeXFOTBuilder::set(const char *name,bool flag)
{
  *stringout_ << "\\def\\" << name << '{'
	      << (flag ? 1 : 0)
	      << "}%\n";
}

//
// Set a long.
//
void TeXFOTBuilder::set(const char *name,long n)
{
  *stringout_ << "\\def\\" << name << '{' << n << "}%\n";
}

//
// Set an unsigned long.
//
void TeXFOTBuilder::set(const char *name,long unsigned int n)
{
  *stringout_ << "\\def\\" << name << '{' << n << "}%\n";
}

//
// Set a PublicId.
//
void TeXFOTBuilder::set(const char *name,PublicId id)
{
  *stringout_ << "\\def\\" << name << '{' << id << "}%\n";
}

//
// Set a Letter2.
//
void TeXFOTBuilder::set(const char *name,Letter2 code)
{
  char letter1 = (code & 0xff00) >> 8;
  char letter2 = (code & 0xff);
  *stringout_ << "\\def\\" << name << '{' << letter1 << letter2 << "}%\n";
}

//
// This one is a problem because it duplications functionality from
// above.
//
void TeXFOTBuilder::set(const char *name,const DisplaySpace &space)
{
  if (space.nominal.length != 0 || space.min.length != 0
      || space.max.length != 0) {
    *stringout_ << "\\def\\" << name << "Nominal{"
		<< (space.nominal.length/1000.0) << "pt}";
    if (space.nominal.displaySizeFactor != 0)
      *stringout_ << "\\def\\" << name << "NominalFactor{"
		  << space.nominal.displaySizeFactor << "}%\n";
    if (space.min.length != 0)
      *stringout_ << "\\def\\" << name << "Min{"
		  << (space.min.length/1000.0) << "pt}";
    if (space.min.displaySizeFactor != 0)
      *stringout_ << "\\def\\" << name << "MinFactor{"
		  << space.min.displaySizeFactor << "}%\n";
    if (space.max != 0)
      *stringout_ << "\\def\\" << name << "Max{"
		  << (space.max.length/1000.0) << "pt}";
    if (space.max.displaySizeFactor != 0)
      *stringout_ << "\\def\\" << name << "MaxFactor{"
		  << space.max.displaySizeFactor << "}%\n";
    if (space.priority != 0)
      *stringout_ << "\\def\\" << name << "Priority{"
		  << space.priority << "}%\n";
    if (space.conditional)
      *stringout_ << "\\def\\" << name << "Conditional{"
		  << (space.conditional ? 1 : 0) << "}%\n";
    if (space.force)
      *stringout_ << "\\def\\" << name << "Force{"
		  << (space.force ? 1 : 0) << "}%\n";
  }
}

void TeXFOTBuilder::set(const char *name,const GlyphId &glyphId)
{
  *stringout_ << "\\def\\" << name << '{';
  if (glyphId.publicId) {
    *stringout_ << glyphId.publicId;
    if (glyphId.suffix)
      *stringout_ << "::" << glyphId.suffix;
  }
  *stringout_ << "}%\n";
}

//
// NIC classes which are inherited by others.
//

void TeXFOTBuilder::setInlineNIC(const InlineNIC &nic)
{
  MAYBESET("BreakBeforePriority",nic.breakBeforePriority,0);
  MAYBESET("BreakAfterPriority",nic.breakAfterPriority,0);
}

void TeXFOTBuilder::setDisplayNIC(const DisplayNIC &nic)
{
  set("SpaceBefore",nic.spaceBefore);
  set("SpaceAfter",nic.spaceAfter);
  MAYBESET("Keep",nic.keep,symbolFalse);
  MAYBESET("BreakBefore",nic.breakBefore,symbolFalse);
  MAYBESET("BreakAfter",nic.breakAfter,symbolFalse);
  MAYBESET("KeepWithPrevious",nic.keepWithPrevious,symbolFalse);
  MAYBESET("KeepWithNext",nic.keepWithNext,symbolFalse);
  MAYBESET("MayViolateKeepBefore",nic.mayViolateKeepBefore,symbolFalse);
  MAYBESET("MayViolateKeepAfter",nic.mayViolateKeepAfter,symbolFalse);
}

//
// Dump all accumulated inherited characteristics.
//
void TeXFOTBuilder::dumpInherited()
{
  *stringout_ << '\0';
  os() << stringout_->str();
  stringout_->rdbuf()->freeze(0);
  delete stringout_;
  stringout_ = new ostrstream;	// should probably check...
}

#ifdef DSSSL_NAMESPACE
}
#endif
