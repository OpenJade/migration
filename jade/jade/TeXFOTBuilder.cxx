// TeXFOTBuilder.cxx: a Generic TeX backend for Jade
// Written by David Megginson <dmeggins@microstar.com>
// With changes from Sebastian Rahtz <s.rahtz@elsevier.co.uk>

#include "config.h"
#include "TeXFOTBuilder.h"
#include "TeXMessages.h"
#include "MessageArg.h"
#include <stdio.h>

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

class TeXFOTBuilder : public SerialFOTBuilder {
public:
  struct PageFloatNIC {
    ~PageFloatNIC();
    StringC placement;
  };
  class TeXExtensionFlowObj : public FOTBuilder::ExtensionFlowObj {
  public:
    virtual void atomic(TeXFOTBuilder &, const NodePtr &) const = 0;
  };
  class TeXCompoundExtensionFlowObj : public FOTBuilder::CompoundExtensionFlowObj {
  public:
    virtual void start(TeXFOTBuilder &, const NodePtr &) const = 0;
    virtual void end(TeXFOTBuilder &) const = 0;
  };
  class PageFloatFlowObj : public TeXCompoundExtensionFlowObj {
    void start(TeXFOTBuilder &fotb, const NodePtr &nd) const {
      fotb.startPageFloat(nic_);
    }
    void end(TeXFOTBuilder &fotb) const {
      fotb.endPageFloat();
    }
    bool hasNIC(const StringC &name) const {
      return name == "placement" ;
    }
    void setNIC(const StringC &name, const Value &value) {
      value.convertString(nic_.placement);
      }
    ExtensionFlowObj *copy() const { return new PageFloatFlowObj(*this); }
  private:
    PageFloatNIC nic_;
    StringC name_;
    StringC placement;
  };
  class PageFootnoteFlowObj : public TeXCompoundExtensionFlowObj {
    void start(TeXFOTBuilder &fotb, const NodePtr &nd) const {
      fotb.startPageFootnote();
    }
    void end(TeXFOTBuilder &fotb) const {
      fotb.endPageFootnote();
    }
    ExtensionFlowObj *copy() const { return new PageFootnoteFlowObj(*this); }
  private:
  };
  //////////////////////////////////////////////////////////////////////
  // Constructor and destructor.
  //////////////////////////////////////////////////////////////////////

  TeXFOTBuilder(OutputByteStream *, Messenger *mgr);
  ~TeXFOTBuilder();

  //////////////////////////////////////////////////////////////////////
  // Atomic flow objects
  //////////////////////////////////////////////////////////////////////
  void extension(const ExtensionFlowObj &fo, const NodePtr &);
  void startExtensionSerial(const CompoundExtensionFlowObj &fo, const NodePtr &nd);
  void endExtensionSerial(const CompoundExtensionFlowObj &fo);
  void start();
  void end();
  void charactersFromNode(const NodePtr &, const Char *, size_t);
  void setPreserveSdata(bool);
  void startPageFloat(const PageFloatNIC &);
  void endPageFloat();
  void setPageFloatNIC(const PageFloatNIC &);
  void startPageFootnote();
  void endPageFootnote();

  void characters(const Char *, size_t);
  void character(const CharacterNIC &);
  void paragraphBreak(const ParagraphNIC &);
  void externalGraphic(const ExternalGraphicNIC &);
  void rule(const RuleNIC &);
  void alignmentPoint();
  void pageNumber();
  void formattingInstruction(const StringC &);
  void tableColumn(const TableColumnNIC &);
  void tableCellBeforeRowBorder();
  void tableCellAfterRowBorder();
  void tableCellBeforeColumnBorder();
  void tableCellAfterColumnBorder();

  void fractionBar();
  void radicalRadical(const CharacterNIC &);
  void radicalRadicalDefaulted();
  void currentNodePageNumber(const NodePtr &);

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
  void startTableRow();
  void endTableRow();
  void startTableCell(const TableCellNIC &);
  void endTableCell();
  void startSimplePageSequence();
  void endSimplePageSequence();
  // Headers and footers are treated like a separate port.
  void startSimplePageSequenceHeaderFooter(unsigned);
  void endSimplePageSequenceHeaderFooter(unsigned);
  // page-number sosofo

  void startTablePartHeader();
  void endTablePartHeader();
  void startTablePartFooter();
  void endTablePartFooter();
  void startMathSequence();
  void endMathSequence();
  void startFractionSerial();
  void endFractionSerial();
  void startFractionNumerator();
  void endFractionNumerator();
  void startFractionDenominator();
  void endFractionDenominator();
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
  void startLink(const Address &);
  void endLink();


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

  void setPageNumberRestart(bool);
  void setPageNumberFormat(const StringC &);
  void setPageNColumns(long);
  void setPageColumnSep(Length);
  void setPageBalanceColumns(bool);
  void setGridRowSep(Length);
  void setGridColumnSep(Length);
  void setSubscriptDepth(Length);
  void setSuperscriptHeight(Length);
  void setUnderMarkDepth(Length);
  void setOverMarkHeight(Length);
  void setHeadingLevel(long);

  void setMinPreLineSpacing(const OptLengthSpec &);
  void setMinPostLineSpacing(const OptLengthSpec &);
  void setMinLeading(const OptLengthSpec &);
  void setScriptPreAlign(Symbol);
  void setScriptPostAlign(Symbol);
  void setScriptMidSupAlign(Symbol);
  void setScriptMidSubAlign(Symbol);
  void setNumeratorAlign(Symbol);
  void setDenominatorAlign(Symbol);
  void setGridPositionCellType(Symbol);
  void setGridColumnAlignment(Symbol);
  void setGridRowAlignment(Symbol);
  void setGridEquidistantRows(bool);
  void setGridEquidistantColumns(bool);
  void setEscapementSpaceBefore(const InlineSpace &);
  void setEscapementSpaceAfter(const InlineSpace &);
  void setGlyphSubstTable(const Vector<ConstPtr<GlyphSubstTable> > &tables);


private:
				// Variables.
  OutputByteStream *fileout_;
  StrOutputByteStream stringout_;
  Messenger *mgr_;
  bool preserveSdata_;


				// Functions.
  OutputByteStream &os();
  void insertAtomic(const char *name);
  void startGroup(const char *name);
  void startBrace(const char *name);
  void endBrace(const char *name);
  void endGroup(const char *name);

  void setlength(const char *,Length);
  void set(const char *,const StringC &);
  void set(const char *,const GroveString &);
  void set(const char *,Symbol);
  void set(const char *,const LengthSpec &);
  void set(const char *,double);
  void set(const char *,const DeviceRGBColor &);
  void set(const char *,bool);
  void set(const char *,long);
  void set(const char *,long unsigned int);
  void set(const char *name,unsigned int n) {
    set(name, (unsigned long)n);
  }
  void set(const char *,PublicId);
  void setletter2(const char *,Letter2);
  void set(const char *,const DisplaySpace &);
  void set(const char *,const GlyphId &);

  void set(const char *,const OptLengthSpec &);
  void set(const char *,const InlineSpace &);

  // Structures for non-inherited characteristics,
  // in the order specified in style/FOTBuilder.h.

  void setDisplayNIC(const DisplayNIC &);
  void setInlineNIC(const InlineNIC &);
  void setDisplayGroupNIC(const DisplayGroupNIC &);
  void setExternalGraphicNIC(const ExternalGraphicNIC &);
  void setBoxNIC(const BoxNIC &);
  void setRuleNIC(const RuleNIC &);
  void setLeaderNIC(const LeaderNIC &);
  void setParagraphNIC(const ParagraphNIC &);
  void setCharacterNIC(const CharacterNIC &);
  void setLineFieldNIC(const LineFieldNIC &);
  void setTableNIC(const TableNIC &);
  void setTablePartNIC(const TablePartNIC &);
  void setTableColumnNIC(const TableColumnNIC &);
  void setTableCellNIC(const TableCellNIC &);
  void setGridNIC(const GridNIC &);
  void setGridCellNIC(const GridCellNIC &);

  void dumpInherited();

  void message(const MessageType0 &);
};

#define MAYBESET(name,value,default) (value!=default?(set(name,value),0):0)


//
// Get the current output stream.
//
inline
OutputByteStream &TeXFOTBuilder::os()
{
  return *fileout_;
}


//
// Define an output operator for StringC
//
// FIXME This won't work for Unicode characters.

static OutputByteStream &operator<<(OutputByteStream &os, const StringC &s)
{
  for (size_t i = 0; i < s.size(); i++)
    os << char(s[i]);
  return os;
}


//
// Define an output operator for GroveString
//
static OutputByteStream &operator<<(OutputByteStream &os, const GroveString &s)
{
  for (size_t i = 0; i < s.size(); i++)
    os << char(s[i]);
  return os;
}

static OutputByteStream &operator<<(OutputByteStream &os, double d)
{
  char buf[64];
  sprintf(buf, "%g", d);
  return os << buf;
}

FOTBuilder *makeTeXFOTBuilder(OutputByteStream *os, Messenger *mgr,
			      const FOTBuilder::Extension *&ext)
{  
  static const TeXFOTBuilder::PageFloatFlowObj pageFloat;
  static const TeXFOTBuilder::PageFootnoteFlowObj pageFootnote;
  static const FOTBuilder::Extension extensions[] = {
    {
      "UNREGISTERED::Sebastian Rahtz//Flow Object Class::page-float",
      0,
      0,
      0,
      0,
      &pageFloat
    },
    {
      "UNREGISTERED::Sebastian Rahtz//Flow Object Class::page-footnote",
      0,
      0,
      0,
      0,
      &pageFootnote
    },
    {
      "UNREGISTERED::James Clark//Characteristic::page-number-format",
      0,
      (void (FOTBuilder::*)(const StringC &))&TeXFOTBuilder::setPageNumberFormat,
      0,
      0
    },
    {
      "UNREGISTERED::James Clark//Characteristic::page-number-restart?",
      (void (FOTBuilder::*)(bool))&TeXFOTBuilder::setPageNumberRestart,
      0,
      0
    },
    {
      "UNREGISTERED::James Clark//Characteristic::page-n-columns",
      0,
      0,
      (void (FOTBuilder::*)(long))&TeXFOTBuilder::setPageNColumns,
      0
    },
    {
      "UNREGISTERED::James Clark//Characteristic::page-column-sep",
      0,
      0,
      0,
      (void (FOTBuilder::*)(FOTBuilder::Length))&TeXFOTBuilder::setPageColumnSep,
    },
    {
      "UNREGISTERED::James Clark//Characteristic::page-balance-columns?",
      (void (FOTBuilder::*)(bool))&TeXFOTBuilder::setPageBalanceColumns,
      0,
      0
    },
    {
      "UNREGISTERED::James Clark//Characteristic::subscript-depth",
      0,
      0,
      0,
      (void (FOTBuilder::*)(FOTBuilder::Length))&TeXFOTBuilder::setSubscriptDepth,
    },
    {
      "UNREGISTERED::James Clark//Characteristic::over-mark-height",
      0,
      0,
      0,
      (void (FOTBuilder::*)(FOTBuilder::Length))&TeXFOTBuilder::setOverMarkHeight,
    },
    {
      "UNREGISTERED::James Clark//Characteristic::under-mark-depth",
      0,
      0,
      0,
      (void (FOTBuilder::*)(FOTBuilder::Length))&TeXFOTBuilder::setUnderMarkDepth,
    },
    {
      "UNREGISTERED::James Clark//Characteristic::superscript-height",
      0,
      0,
      0,
      (void (FOTBuilder::*)(FOTBuilder::Length))&TeXFOTBuilder::setSuperscriptHeight,
    },
    {
      "UNREGISTERED::James Clark//Characteristic::grid-row-sep",
      0,
      0,
      0,
      (void (FOTBuilder::*)(FOTBuilder::Length))&TeXFOTBuilder::setGridRowSep,
    },
    {
      "UNREGISTERED::James Clark//Characteristic::grid-column-sep",
      0,
      0,
      0,
      (void (FOTBuilder::*)(FOTBuilder::Length))&TeXFOTBuilder::setGridColumnSep,
    },
    {
      "UNREGISTERED::James Clark//Characteristic::heading-level",
      0,
      0,
      (void (FOTBuilder::*)(long))&TeXFOTBuilder::setHeadingLevel,
      0
    },
    {
      "UNREGISTERED::James Clark//Characteristic::preserve-sdata?",
      (void (FOTBuilder::*)(bool))&TeXFOTBuilder::setPreserveSdata,
      0,
      0,
      0,
      0
    },

    { 0, 0, 0}
  };
  ext = extensions;
  return new TeXFOTBuilder(os, mgr);
}

////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
////////////////////////////////////////////////////////////////////////

TeXFOTBuilder::TeXFOTBuilder(OutputByteStream *o, Messenger *mgr)
: fileout_(o), mgr_(mgr)
{
  os() << "\\FOT{}";
}

TeXFOTBuilder::~TeXFOTBuilder()
{
  os() << "\\endFOT{}";
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
      case '^':
      case '_':
	os() << "\\char" << int(*s) << "{}";
	break;
      case '{':
      case '}':
      case '$':
      case '&':
      case '#':
      case '~':
      case '%':
	os() << "\\" << char(*s) << " ";
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
  setCharacterNIC(nic);
  insertAtomic("Character");
}

void TeXFOTBuilder::paragraphBreak(const ParagraphNIC &nic)
{
  setParagraphNIC(nic);
  insertAtomic("ParagraphBreak");
}

void TeXFOTBuilder::externalGraphic(const ExternalGraphicNIC &nic)
{
  setExternalGraphicNIC(nic);
  insertAtomic("ExternalGraphic");
}

void TeXFOTBuilder::rule(const RuleNIC &nic)
{
  setRuleNIC(nic);
  insertAtomic("Rule");
}

void TeXFOTBuilder::alignmentPoint()
{
  insertAtomic("AlignmentPoint");
}

// page-number sosofo
void TeXFOTBuilder::pageNumber()
{
  insertAtomic("PageNumber");
}

void TeXFOTBuilder::formattingInstruction (const StringC &instr)
{
  os() << instr;
}
  
void TeXFOTBuilder::tableColumn(const TableColumnNIC &nic)
{
  setTableColumnNIC(nic);
  insertAtomic("TableColumn");
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

void TeXFOTBuilder::fractionBar()
{
  insertAtomic("FractionBar");
}

void TeXFOTBuilder::radicalRadical(const CharacterNIC &c)
{
  setCharacterNIC(c);
  insertAtomic("RadicalRadical");
}

void TeXFOTBuilder::radicalRadicalDefaulted()
{
  insertAtomic("RadicalRadicalDefaulted");
}

void TeXFOTBuilder::currentNodePageNumber(const NodePtr &node)
{
  GroveString id;
  unsigned long ei;

  // FIX ME!
  // Only PARTIALLY supported -- I currently allow cross-references
  // only to elements.
  if (node->getId(id) == accessOK) {
    set("Label",id);
  } else if (node->elementIndex(ei) == accessOK) {
    set("Element",ei);
  } else {
    message(TeXMessages::unsupportedPageNumberNonElement);
    return;
  }
  unsigned long g = node->groveIndex();
  if (g) {
    set("GroveIndex",g);
  }
  insertAtomic("CurrentNodePageNumber");
}


//////////////////////////////////////////////////////////////////////
// Non-atomic flow objects
//////////////////////////////////////////////////////////////////////

void TeXFOTBuilder::startSequence()
{
  startGroup("Seq");
}

void TeXFOTBuilder::endSequence()
{
  endGroup("Seq");
}

void TeXFOTBuilder::startLineField(const LineFieldNIC &nic)
{
  setLineFieldNIC(nic);
  startGroup("LineField");
}

void TeXFOTBuilder::endLineField()
{
  endGroup("LineField");
}

void TeXFOTBuilder::startParagraph(const ParagraphNIC &nic)
{
  setParagraphNIC(nic);
  startGroup("Par");
}

void TeXFOTBuilder::endParagraph()
{
  endGroup("Par");
}

void TeXFOTBuilder::startDisplayGroup(const DisplayGroupNIC &nic)
{
  setDisplayGroupNIC(nic);
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

void TeXFOTBuilder::startScore(Char ch)
{
  set("ScoreCharacter",(unsigned long)ch);
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
  setLeaderNIC(nic);
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
  setBoxNIC(nic);
  startGroup("BOX");
}

void TeXFOTBuilder::endBox()
{
  endGroup("BOX");
}

// Tables
void TeXFOTBuilder::startTable(const TableNIC &nic)
{
  setTableNIC(nic);
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
  setTablePartNIC(nic);
  startGroup("TablePart");
}

void TeXFOTBuilder::endTablePartSerial()
{
  endGroup("TablePart");
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
  setTableCellNIC(nic);
  startGroup("TableCell");
}

void TeXFOTBuilder::endTableCell()
{
  endGroup("TableCell");
}

void TeXFOTBuilder::startSimplePageSequence()
{
  startGroup("SpS");
}

void TeXFOTBuilder::endSimplePageSequence()
{
  endGroup("SpS");
}

// These aren't real flow objects, so handle them a little
// differently.
void TeXFOTBuilder::startSimplePageSequenceHeaderFooter(unsigned flags)
{
  os() << "\n\\SpS";
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
  os() << "%\n{";
}

void TeXFOTBuilder::endSimplePageSequenceHeaderFooter(unsigned flags)
{
  os() << '}';
}

void TeXFOTBuilder::startTablePartHeader()
{
  startGroup("TablePartHeader");
}

void TeXFOTBuilder::endTablePartHeader()
{
  endGroup("TablePartHeader");
}

void TeXFOTBuilder::startTablePartFooter()
{
  startGroup("TablePartFooter");
}

void TeXFOTBuilder::endTablePartFooter()
{
  endGroup("TablePartFooter");
}

void TeXFOTBuilder::startMathSequence()
{
  startGroup("MathSeq");
}

void TeXFOTBuilder::endMathSequence()
{
  endGroup("MathSeq");
}

void TeXFOTBuilder::startFractionSerial()
{
  startGroup("FractionSerial");
}

void TeXFOTBuilder::endFractionSerial()
{
  endGroup("FractionSerial");
}

void TeXFOTBuilder::startFractionNumerator()
{
  startGroup("FractionNumerator");
}

void TeXFOTBuilder::endFractionNumerator()
{
  endGroup("FractionNumerator");
}

void TeXFOTBuilder::startFractionDenominator()
{
  startGroup("FractionDenominator");
}

void TeXFOTBuilder::endFractionDenominator()
{
  endGroup("FractionDenominator");
}

void TeXFOTBuilder::startUnmath()
{
  startGroup("Unmath");
}

void TeXFOTBuilder::endUnmath()
{
  endGroup("Unmath");
}

void TeXFOTBuilder::startSuperscript()
{
  startGroup("Superscript");
}

void TeXFOTBuilder::endSuperscript()
{
  endGroup("Superscript");
}

void TeXFOTBuilder::startSubscript()
{
  startGroup("Subscript");
}

void TeXFOTBuilder::endSubscript()
{
  endGroup("Subscript");
}

void TeXFOTBuilder::startScriptSerial()
{
  startGroup("ScriptSerial");
}

void TeXFOTBuilder::endScriptSerial()
{
  endGroup("ScriptSerial");
}

void TeXFOTBuilder::startScriptPreSup()
{
  startGroup("ScriptPreSup");
}

void TeXFOTBuilder::endScriptPreSup()
{
  endGroup("ScriptPreSup");
}

void TeXFOTBuilder::startScriptPreSub()
{
  startGroup("ScriptPreSub");
}

void TeXFOTBuilder::endScriptPreSub()
{
  endGroup("ScriptPreSub");
}

void TeXFOTBuilder::startScriptPostSup()
{
  startGroup("ScriptPostSup");
}

void TeXFOTBuilder::endScriptPostSup()
{
  endGroup("ScriptPostSup");
}

void TeXFOTBuilder::startScriptPostSub()
{
  startGroup("ScriptPostSub");
}

void TeXFOTBuilder::endScriptPostSub()
{
  endGroup("ScriptPostSub");
}

void TeXFOTBuilder::startScriptMidSup()
{
  startGroup("ScriptMidSup");
}

void TeXFOTBuilder::endScriptMidSup()
{
  endGroup("ScriptMidSup");
}

void TeXFOTBuilder::startScriptMidSub()
{
  startGroup("ScriptMidSub");
}

void TeXFOTBuilder::endScriptMidSub()
{
  endGroup("ScriptMidSub");
}

void TeXFOTBuilder::startMarkSerial()
{
  startGroup("MarkSerial");
}

void TeXFOTBuilder::endMarkSerial()
{
  endGroup("MarkSerial");
}

void TeXFOTBuilder::startMarkOver()
{
  startGroup("MarkOver");
}

void TeXFOTBuilder::endMarkOver()
{
  endGroup("MarkOver");
}

void TeXFOTBuilder::startMarkUnder()
{
  startGroup("MarkUnder");
}

void TeXFOTBuilder::endMarkUnder()
{
  endGroup("MarkUnder");
}

void TeXFOTBuilder::startFenceSerial()
{
  startGroup("FenceSerial");
  // Extra brace to ensure body of fence is inside 
  // a TeX group. Otherwise nested fences go wrong
  os() << "{";
}

void TeXFOTBuilder::endFenceSerial()
{
    //  endGroup("FenceSerial");
}

void TeXFOTBuilder::startFenceOpen()
{
  // Extra closing brace for end of fence body
  os() << "}{";
  //  startGroup("FenceOpen");
}

void TeXFOTBuilder::endFenceOpen()
{
  os() << "}";
  // endGroup("FenceOpen");
}

void TeXFOTBuilder::startFenceClose()
{
  os() << "{";
  // startGroup("FenceClose");
}

void TeXFOTBuilder::endFenceClose()
{
  os() << "}";
  //  endGroup("FenceClose");
}

void TeXFOTBuilder::startRadicalSerial()
{
  startGroup("RadicalSerial");
}

void TeXFOTBuilder::endRadicalSerial()
{
  endGroup("RadicalSerial");
}

void TeXFOTBuilder::startRadicalDegree()
{
  startGroup("RadicalDegree");
}

void TeXFOTBuilder::endRadicalDegree()
{
  endGroup("RadicalDegree");
}

void TeXFOTBuilder::startMathOperatorSerial()
{
  startGroup("MathOperatorSerial");
}

void TeXFOTBuilder::endMathOperatorSerial()
{
  endGroup("MathOperatorSerial");
}

void TeXFOTBuilder::startMathOperatorOperator()
{
  startGroup("MathOperatorOperator");
}

void TeXFOTBuilder::endMathOperatorOperator()
{
  endGroup("MathOperatorOperator");
}

void TeXFOTBuilder::startMathOperatorLowerLimit()
{
  startGroup("MathOperatorLowerLimit");
}

void TeXFOTBuilder::endMathOperatorLowerLimit()
{
  endGroup("MathOperatorLowerLimit");
}

void TeXFOTBuilder::startMathOperatorUpperLimit()
{
  startGroup("MathOperatorUpperLimit");
}

void TeXFOTBuilder::endMathOperatorUpperLimit()
{
  endGroup("MathOperatorUpperLimit");
}

void TeXFOTBuilder::startGrid(const GridNIC &nic)
{
  setGridNIC(nic);
  startGroup("Grid");
}

void TeXFOTBuilder::endGrid()
{
  endGroup("Grid");
}

void TeXFOTBuilder::startGridCell(const GridCellNIC &nic)
{
  setGridCellNIC(nic);
  startGroup("GridCell");
}

void TeXFOTBuilder::endGridCell()
{
  endGroup("GridCell");
}

void TeXFOTBuilder::startNode(const NodePtr &node,
			      const StringC &processingMode, 
			      RuleType ruleType)
{
  GroveString id;
  unsigned long ei;

  if (node->getId(id) == accessOK) {
    set("Label",id);
  }
  else if (node->elementIndex(ei) == accessOK) {
    set("Element", ei);
  }
  unsigned long g = node->groveIndex();
  if (g) {
    set("GroveIndex", g);
  }
  if (processingMode.size()) {
    set("ProcessingMode", processingMode);
  }
  startGroup("Node");
}

void TeXFOTBuilder::endNode()
{
  endGroup("Node");
}

void TeXFOTBuilder::startLink(const Address &addr)
{
  GroveString id;
  unsigned long ei;

  // FIX ME!
  // This needs a lot of work -- for now, it supports only links to
  // elements.

  switch (addr.type) {
  case Address::none:
    break;
  case Address::resolvedNode:
    if (addr.node->getId(id) == accessOK) {
      set("Label",id);
    } else if (addr.node->elementIndex(ei) == accessOK) {
      set("Element", ei);
    }
    else {
      message(TeXMessages::unsupportedLinkNonElement);
    }
    break;
  case Address::idref:
				// just the first IDREF for now
    set("Label",addr.params[0]);
    break;
  case Address::entity:
    message(TeXMessages::unsupportedLinkEntity);
    break;
  case Address::sgmlDocument:
    message(TeXMessages::unsupportedLinkSgmlDoc);
    break;
  case Address::hytimeLinkend:
    message(TeXMessages::unsupportedLinkHyTime);
    break;
  case Address::tei:
    message(TeXMessages::unsupportedLinkTei);
    break;
  case Address::html:
    message(TeXMessages::unsupportedLinkHtml);
    break;
  }
  if (addr.node) {
    unsigned long g = addr.node->groveIndex();
    if (g) {
      set("GroveIndex",g);
    }
  }
  startGroup("Link");
}

void TeXFOTBuilder::endLink()
{
  endGroup("Link");
}

/////////////////////////////////////////////////////////////////////
// Inherited characteristics
// Set the value of the characteristic for the next flow object.
// Inherited characteristics that are not explicitly set have
// the same value as the parent flow object.
//////////////////////////////////////////////////////////////////////

void TeXFOTBuilder::setFontSize(Length size)
{
  setlength("fSize",size);
}

void TeXFOTBuilder::setFontFamilyName(const StringC &name)
{
  stringout_ << "\\def\\fFamName{";
  for (size_t i = 0; i < name.size(); i++)
    {
      switch(name[i]) {
      case ' ': 
	stringout_ << '-';
        break;
      default:
	stringout_ << char(name[i]);
	break;
       }
      }
      stringout_ << "}";
}

void TeXFOTBuilder::setFontWeight(Symbol weight)
{
  set("fWeight",weight);
}

void TeXFOTBuilder::setFontPosture(Symbol posture)
{
  set("fPosture",posture);
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
  set("fStructure",structure);
}

void TeXFOTBuilder::setFontProportionateWidth(Symbol width)
{
  set("fProportionateWidth",width);
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
  set("fName",id);
}

// Two-letter code
void TeXFOTBuilder::setLanguage(Letter2 language)
{
  setletter2("Language",language);
}

void TeXFOTBuilder::setCountry(Letter2 country)
{
  setletter2("Country",country);
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

				// New inherited characteristics
				// added 1 March/97 with math support.
void TeXFOTBuilder::setMinPreLineSpacing(const OptLengthSpec &len)
{
  set("MinPreLineSpacing",len);
}


void TeXFOTBuilder::setMinPostLineSpacing(const OptLengthSpec &len)
{
  set("MinPostLineSpacing",len);
}


void TeXFOTBuilder::setMinLeading(const OptLengthSpec &len)
{
  set("MinLeading",len);
}


void TeXFOTBuilder::setScriptPreAlign(Symbol sym)
{
  set("ScriptPreAlign",sym);
}


void TeXFOTBuilder::setScriptPostAlign(Symbol sym)
{
  set("ScriptPostAlign",sym);
}


void TeXFOTBuilder::setScriptMidSupAlign(Symbol sym)
{
  set("ScriptMidSupAlign",sym);
}


void TeXFOTBuilder::setScriptMidSubAlign(Symbol sym)
{
  set("ScriptMidSubAlign",sym);
}


void TeXFOTBuilder::setNumeratorAlign(Symbol sym)
{
  set("NumeratorAlign",sym);
}


void TeXFOTBuilder::setDenominatorAlign(Symbol sym)
{
  set("DenominatorAlign",sym);
}


void TeXFOTBuilder::setGridPositionCellType(Symbol sym)
{
  set("GridPositionCellType",sym);
}


void TeXFOTBuilder::setGridColumnAlignment(Symbol sym)
{
  set("GridColumnAlignment",sym);
}


void TeXFOTBuilder::setGridRowAlignment(Symbol sym)
{
  set("GridRowAlignment",sym);
}


void TeXFOTBuilder::setGridEquidistantRows(bool flag)
{
  set("GridEquidistantRows",flag);
}


void TeXFOTBuilder::setGridEquidistantColumns(bool flag)
{
  set("GridEquidistantColumns",flag);
}


void TeXFOTBuilder::setEscapementSpaceBefore(const InlineSpace &space)
{
  set("EscapementSpaceBefore",space);
}


void TeXFOTBuilder::setEscapementSpaceAfter(const InlineSpace &space)
{
  set("EscapementSpaceAfter",space);
}


void TeXFOTBuilder::setGlyphSubstTable(const Vector<ConstPtr<GlyphSubstTable> > &tables)
{
  // FIX ME!
  message(TeXMessages::unsupportedGlyphSubstTable);
  // set("GlyphSubstTable",tables);
}

////////////////////////////////////////////////////////////////////////
// Private member functions.
////////////////////////////////////////////////////////////////////////

//
// Insert an atomic flow object.
//
void TeXFOTBuilder::insertAtomic(const char *name)
{
  os() << "\\insert" << name << "%\n{";
  dumpInherited();
  os() << '}';
}

//
// Start a non-atomic flow object.
//
void TeXFOTBuilder::startGroup(const char *name)
{
  os() << "\\" << name << "%\n{";
  dumpInherited();
  os() << '}';
}

//
// Start a non-atomic flow object, with the content delimited by braces
//
void TeXFOTBuilder::startBrace(const char *name)
{
  os() << "\\" << name << "%\n{";
  dumpInherited();
  os() << "}{";
}

//
// End a non-atomic flow object with just a closing brace
//
void TeXFOTBuilder::endBrace(const char *name)
{
  os() << "}";
}

//
// End a non-atomic flow object.
//
void TeXFOTBuilder::endGroup(const char *name)
{
  os() << "\\end" << name << "{}";
}

//
// Set a Length (needs a different name to avoid conflict
// with long.
//
void TeXFOTBuilder::setlength(const char *name,Length size)
{
  stringout_ << "\\def\\" << name << "%\n{"
	      << float(size/1000.0)
	      << "\\p@}";
}

//
// Set a StringC.
//
void TeXFOTBuilder::set(const char *name,const StringC &value)
{
  stringout_ << "\\def\\" << name << "%\n{"
	      << value
	      << '}';
}

//
// Set a GroveString
//
void TeXFOTBuilder::set(const char *name,const GroveString &value)
{
  stringout_ << "\\def\\" << name << "%\n{"
	      << value
	      << '}';
}

//
// Set a Symbol.
//
void TeXFOTBuilder::set(const char *name,Symbol sym)
{
  const char * symbolName = "";

  switch (sym) {
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
  case symbolPageRegion:
    symbolName = "pageregion";
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
  case symbolIndependent:
    symbolName = "independent";
    break;
  case symbolPile:
    symbolName = "pile";
    break;
  case symbolSupOut:
    symbolName = "supout";
    break;
  case symbolSubOut:
    symbolName = "subout";
    break;
  case symbolLeadEdge:
    symbolName = "leadedge";
    break;
  case symbolTrailEdge:
    symbolName = "trailedge";
    break;
  case symbolExplicit:
    symbolName = "explicit";
    break;
  case symbolRowMajor:
    symbolName = "rowmajor";
    break;
  case symbolColumnMajor:
    symbolName = "columnmajor";
    break;
  }

  stringout_ << "\\def\\" << name << "%\n{" << symbolName << '}';
}

//
// Set a LengthSpec.
//
void TeXFOTBuilder::set(const char *name,const LengthSpec &spec)
{
  stringout_ << "\\def\\" << name << "%\n{"
	      << float(spec.length/1000.0)
	      << "\\p@}";
  stringout_ << "\\def\\" << name << "Factor%\n{"
	      << spec.displaySizeFactor
	      << '}';
}

//
// Set a double.
//
void TeXFOTBuilder::set(const char *name,double n)
{
  stringout_ << "\\def\\" << name << "%\n{" << n << '}';
}

//
// Set a DeviceRGBColor.
//
void TeXFOTBuilder::set(const char *name, const DeviceRGBColor &color)
{
  stringout_ << "\\def\\" << name << "Red%\n{" << int(color.red) << '}';
  stringout_ << "\\def\\" << name << "Green%\n{" << int(color.green) << '}';
  stringout_ << "\\def\\" << name << "Blue%\n{" << int(color.blue) << '}';
}

//
// Set a bool.
//
void TeXFOTBuilder::set(const char *name,bool flag)
{
  stringout_ << "\\def\\" << name << "%\n{"
	      << (flag ? 1 : 0)
	      << '}';
}

//
// Set a long.
//
void TeXFOTBuilder::set(const char *name,long n)
{
  stringout_ << "\\def\\" << name << "%\n{" << n << '}';
}

//
// Set an unsigned long.
//
void TeXFOTBuilder::set(const char *name,long unsigned int n)
{
  stringout_ << "\\def\\" << name << "%\n{" << n << '}';
}

//
// Set a PublicId.
//
void TeXFOTBuilder::set(const char *name,PublicId id)
{
  stringout_ << "\\def\\" << name << "%\n{" << id << '}';
}

//
// Set a Letter2.
//
void TeXFOTBuilder::setletter2(const char *name,Letter2 code)
{
  char letter1 = (code & 0xff00) >> 8;
  char letter2 = (code & 0xff);
  stringout_ << "\\def\\" << name << "%\n{" << letter1 << letter2 << '}';
}

//
// This one is a problem because it duplications functionality from
// above.
//
void TeXFOTBuilder::set(const char *name,const DisplaySpace &space)
{
  if (space.nominal.length != 0 || space.min.length != 0
      || space.max.length != 0) {
    stringout_ << "\\def\\" << name << "Nom%\n{"
		<< (space.nominal.length/1000.0) << "\\p@}";
    if (space.nominal.displaySizeFactor != 0)
      stringout_ << "\\def\\" << name << "NomFactor%\n{"
		  << space.nominal.displaySizeFactor << '}';
    if (space.min.length != 0)
      stringout_ << "\\def\\" << name << "Min%\n{"
		  << (space.min.length/1000.0) << "\\p@}";
    if (space.min.displaySizeFactor != 0)
      stringout_ << "\\def\\" << name << "MinFactor%\n{"
		  << space.min.displaySizeFactor << '}';
    if (space.max.length != 0)
      stringout_ << "\\def\\" << name << "Max%\n{"
		  << (space.max.length/1000.0) << "\\p@}";
    if (space.max.displaySizeFactor != 0)
      stringout_ << "\\def\\" << name << "MaxFactor%\n{"
		  << space.max.displaySizeFactor << '}';
    if (space.priority != 0)
      stringout_ << "\\def\\" << name << "Priority%\n{"
		  << space.priority << '}';
    if (space.conditional)
      stringout_ << "\\def\\" << name << "Conditional%\n{"
		  << (space.conditional ? 1 : 0) << '}';
    if (space.force)
      stringout_ << "\\def\\" << name << "Force%\n{"
		  << (space.force ? 1 : 0) << '}';
  }
}

void TeXFOTBuilder::set(const char *name,const GlyphId &glyphId)
{
  stringout_ << "\\def\\" << name << "%\n{";
  if (glyphId.publicId) {
    stringout_ << glyphId.publicId;
    if (glyphId.suffix)
      stringout_ << "::" << glyphId.suffix;
  }
  stringout_ << '}';
}

void TeXFOTBuilder::set(const char *name,const OptLengthSpec &spec)
{
  if (spec.hasLength) {
    set(name,spec.length);
  }
}

// This one is also a problem because it duplicates functionality.
void TeXFOTBuilder::set(const char *name,const InlineSpace &space)
{
  if (space.nominal.length != 0 || space.min.length != 0
      || space.max.length != 0) {
    stringout_ << "\\def\\" << name << "Nom%\n{"
		<< (space.nominal.length/1000.0) << "\\p@}";
    if (space.nominal.displaySizeFactor != 0)
      stringout_ << "\\def\\" << name << "NomFactor%\n{"
		  << space.nominal.displaySizeFactor << '}';
    if (space.min.length != 0)
      stringout_ << "\\def\\" << name << "Min%\n{"
		  << (space.min.length/1000.0) << "\\p@}";
    if (space.min.displaySizeFactor != 0)
      stringout_ << "\\def\\" << name << "MinFactor%\n{"
		  << space.min.displaySizeFactor << '}';
    if (space.max.length != 0)
      stringout_ << "\\def\\" << name << "Max%\n{"
		  << (space.max.length/1000.0) << "\\p@}";
    if (space.max.displaySizeFactor != 0)
      stringout_ << "\\def\\" << name << "MaxFactor%\n{"
		  << space.max.displaySizeFactor << '}';
  }
}

//
// Structures for non-inherited characters, in the order specified
// in /style/FOTBuilder.h.
//

void TeXFOTBuilder::setDisplayNIC(const DisplayNIC &nic)
{
  set("sb",nic.spaceBefore);
  set("sa",nic.spaceAfter);
  MAYBESET("Keep",nic.keep,symbolFalse);
  MAYBESET("BreakBefore",nic.breakBefore,symbolFalse);
  MAYBESET("BreakAfter",nic.breakAfter,symbolFalse);
  MAYBESET("KeepWithPrevious",nic.keepWithPrevious,symbolFalse);
  MAYBESET("KeepWithNext",nic.keepWithNext,symbolFalse);
  MAYBESET("MayViolateKeepBefore",nic.mayViolateKeepBefore,symbolFalse);
  MAYBESET("MayViolateKeepAfter",nic.mayViolateKeepAfter,symbolFalse);
}

void TeXFOTBuilder::setInlineNIC(const InlineNIC &nic)
{
  MAYBESET("BreakBeforePriority",nic.breakBeforePriority,0);
  MAYBESET("BreakAfterPriority",nic.breakAfterPriority,0);
}

void TeXFOTBuilder::setDisplayGroupNIC(const DisplayGroupNIC &nic)
{
  setDisplayNIC(nic);
  if (nic.hasCoalesceId) 
    set("CoalesceId",nic.coalesceId);
}

void TeXFOTBuilder::setExternalGraphicNIC(const ExternalGraphicNIC &nic)
{
  setDisplayNIC(nic);
  setInlineNIC(nic);

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
}

void TeXFOTBuilder::setBoxNIC(const BoxNIC &nic)
{
  setDisplayNIC(nic);
  setInlineNIC(nic);
				// BoxNIC
  MAYBESET("IsDisplay",nic.isDisplay,symbolFalse);
}

void TeXFOTBuilder::setRuleNIC(const RuleNIC &nic)
{
  setDisplayNIC(nic);
  setInlineNIC(nic);
				// Rule NIC
  set("Orientation",nic.orientation);
  if (nic.hasLength)
    set("Length",nic.length);
}

void TeXFOTBuilder::setLeaderNIC(const LeaderNIC &nic)
{
  setInlineNIC(nic);
  if (nic.hasLength)
    set("Length",nic.length);
}

void TeXFOTBuilder::setParagraphNIC(const ParagraphNIC &nic)
{
  setDisplayNIC(nic);
}

void TeXFOTBuilder::setCharacterNIC(const CharacterNIC &nic)
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
}

void TeXFOTBuilder::setLineFieldNIC(const LineFieldNIC &nic)
{
  setInlineNIC(nic);
}

void TeXFOTBuilder::setTableNIC(const TableNIC &nic)
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
}

void TeXFOTBuilder::setTablePartNIC(const TablePartNIC &nic)
{
  setDisplayNIC(nic);
}

void TeXFOTBuilder::setTableColumnNIC(const TableColumnNIC &nic)
{
  MAYBESET("ColumnIndex",long(nic.columnIndex),0);
  MAYBESET("NColumnsSpanned",long(nic.nColumnsSpanned),1);
  if (nic.hasWidth)
    set("Width",nic.width);
}

void TeXFOTBuilder::setTableCellNIC(const TableCellNIC &nic)
{
  // FIX ME!
  // does not deal with "missing" bool yet.
  MAYBESET("ColumnIndex",long(nic.columnIndex),0);
  MAYBESET("NColumnsSpanned",long(nic.nColumnsSpanned),1);
  MAYBESET("NRowsSpanned",long(nic.nRowsSpanned),1);
}

void TeXFOTBuilder::setGridNIC(const GridNIC &nic)
{
  set("NColumns",nic.nColumns);
  set("NRows",nic.nRows);
}

void TeXFOTBuilder::setGridCellNIC(const GridCellNIC &nic)
{
  set("ColumnNumber",nic.columnNumber);
  set("RowNumber",nic.rowNumber);
}


// Dump all accumulated inherited characteristics.

void TeXFOTBuilder::dumpInherited()
{
  String<char> tem;
  stringout_.extractString(tem);
  os() << tem;
}

void TeXFOTBuilder::message(const MessageType0 &msg)
{
  mgr_->message(msg);
}

void TeXFOTBuilder::setPageNumberFormat(const String<unsigned short> &name)
{
  set("PageNumberFormat",name);
}

void TeXFOTBuilder::setPageNColumns(long n)
{
 set("PageNColumns",n);
}

void TeXFOTBuilder::setPageColumnSep(Length w)
{
 setlength("PageColumnSep",w);
}

void TeXFOTBuilder::setPageBalanceColumns(bool flag)
{
 set("PageBalanceColumns",flag);
}

void TeXFOTBuilder::setSubscriptDepth(Length w)
{
 setlength("SubScriptDepth",w);
}

void TeXFOTBuilder::setOverMarkHeight(Length w)
{
 setlength("OverMarkHeight",w);
}

void TeXFOTBuilder::setUnderMarkDepth(Length w)
{
 setlength("UnderMarkDepth",w);
}

void TeXFOTBuilder::setSuperscriptHeight(Length w)
{
 setlength("SuperscriptHeight",w);
}

void TeXFOTBuilder::setGridRowSep(Length  w)
{
 setlength("GridRowsep",w);
}

void TeXFOTBuilder::setGridColumnSep(Length w)
{
 setlength("GridColumnSep",w);
}

void TeXFOTBuilder::setHeadingLevel(long n)
{
 set("HeadingLevel",n);
}

void TeXFOTBuilder::setPageNumberRestart(bool flag)
{
 set("PageNumberRestart",flag);
}

void TeXFOTBuilder::startPageFloat(const PageFloatNIC &nic)
{
  setPageFloatNIC(nic);
  startGroup("PageFloat");
}

void TeXFOTBuilder::endPageFloat()
{
  endGroup("PageFloat");
}

void TeXFOTBuilder::startPageFootnote()
{
  startBrace("PageFootnote");
}

void TeXFOTBuilder::endPageFootnote()
{
  endBrace("PageFootnote");
}

void TeXFOTBuilder::setPageFloatNIC(const PageFloatNIC &nic)
{
  set("placement",nic.placement);
}

TeXFOTBuilder::PageFloatNIC::~PageFloatNIC()
{
}

void TeXFOTBuilder::extension(const ExtensionFlowObj &fo, const NodePtr &nd)
{
  ((const TeXExtensionFlowObj &)fo).atomic(*this, nd);
}

void TeXFOTBuilder::startExtensionSerial(const CompoundExtensionFlowObj &fo, const NodePtr &nd)
{
  ((const TeXCompoundExtensionFlowObj &)fo).start(*this, nd);
}

void TeXFOTBuilder::endExtensionSerial(const CompoundExtensionFlowObj &fo)
{
  ((const TeXCompoundExtensionFlowObj &)fo).end(*this);
}

void TeXFOTBuilder::start()
{
}

void TeXFOTBuilder::end()
{
}
void TeXFOTBuilder::setPreserveSdata(bool b)
{
  preserveSdata_ = b;
}

void TeXFOTBuilder::charactersFromNode(const NodePtr &nd, const Char *s, size_t n)
{
  GroveString name;
  if (preserveSdata_ && n == 1 && nd->getEntityName(name) == accessOK)
    os() << "\\Entity{" << name << '}';
  else
    TeXFOTBuilder::characters(s, n);
}

#ifdef DSSSL_NAMESPACE
}
#endif
