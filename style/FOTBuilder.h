// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#ifndef FOTBuilder_INCLUDED
#define FOTBuilder_INCLUDED 1

#include "types.h"
#include "StringC.h"
#include "Node.h"
#include "Link.h"
#include "IList.h"
#include "Vector.h"
#include "Resource.h"
#include "Ptr.h"
#include <stddef.h>
#include <string.h>
#include "dsssl_ns.h"

// This is a work around for a bug in Sun C++ 4.1.
// The class local typedef of PublicId will cause it to reject a
// subsequent declaration of the class PublicId, but it's happy
// if it sees the class first.
#ifdef __SUNPRO_CC
#include "ExternalId.h"
#endif

#ifdef SP_USE_DLL
#ifdef BUILD_LIBSTYLE
#define STYLE_API SP_DLLEXPORT
#else
#define STYLE_API SP_DLLIMPORT
#endif
#else /* not SP_USE_DLL */
#define STYLE_API /* as nothing */
#endif /* not SP_USE_DLL */

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

class SaveFOTBuilder;

class STYLE_API FOTBuilder {
public:
  enum Symbol {
    symbolFalse,
    symbolTrue,
    symbolNotApplicable,
    // These are guaranteed to be in increasing order of weight,
    // and expansion.
    symbolUltraCondensed,
    symbolExtraCondensed,
    symbolCondensed,
    symbolSemiCondensed,
    symbolUltraLight,
    symbolExtraLight,
    symbolLight,
    symbolSemiLight,
    symbolMedium,
    symbolSemiExpanded,
    symbolExpanded,
    symbolExtraExpanded,
    symbolUltraExpanded,
    symbolSemiBold,
    symbolBold,
    symbolExtraBold,
    symbolUltraBold,
    symbolUpright,
    symbolOblique,
    symbolBackSlantedOblique,
    symbolItalic,
    symbolBackSlantedItalic,
    symbolStart,
    symbolEnd,
    symbolCenter,
    symbolJustify,
    symbolSpreadInside,
    symbolSpreadOutside,
    symbolPageInside,
    symbolPageOutside,
    symbolWrap,
    symbolAsis,
    symbolAsisWrap,
    symbolAsisTruncate,
    symbolNone,
    symbolBefore,
    symbolThrough,
    symbolAfter,
    symbolTopToBottom,
    symbolLeftToRight,
    symbolBottomToTop,
    symbolRightToLeft,
    symbolInside,
    symbolOutside,
    symbolHorizontal,
    symbolVertical,
    symbolEscapement,
    symbolLineProgression,
    symbolMath,
    symbolOrdinary,
    symbolOperator,
    symbolBinary,
    symbolRelation,
    symbolOpening,
    symbolClosing,
    symbolPunctuation,
    symbolInner,
    symbolSpace,
    symbolPage,
    symbolPageRegion,
    symbolColumnSet,
    symbolColumn,
    symbolMax,
    symbolMaxUniform,
    symbolMiter,
    symbolRound,
    symbolJoin,
    symbolButt,
    symbolSquare,
    symbolLoose,
    symbolNormal,
    symbolKern,
    symbolTight,
    symbolTouch,
    symbolPreserve,
    symbolCollapse,
    symbolIgnore,
    symbolRelative,
    symbolDisplay,
    symbolInline,
    symbolBorder,
    symbolBackground,
    symbolBoth,
    symbolBase,
    symbolFont,
    symbolTop,
    symbolBottom,
    symbolSpread,
    symbolSolid,
    symbolOutline,
    symbolWith,
    symbolAgainst,
    symbolForce
  };
  enum { nSymbols = symbolForce + 1 };
  typedef const char *PublicId;
  struct GlyphId {
    GlyphId() : publicId(0), suffix(0) { }
    GlyphId(const char *s, unsigned long n = 0) : publicId(s), suffix(n) { }
    PublicId publicId;
    // If suffix is non-zero, then add a double-colon plus
    // the suffix in decimal onto publicId to get the complete
    // public identifier of the glyph id.
    unsigned long suffix;
  };
  struct GlyphSubstTable : public Resource {
    unsigned uniqueId;
    Vector<GlyphId> pairs;
    GlyphId subst(const GlyphId &) const;
  };
  // SP_LETTER2('U', 'K')
#define SP_LETTER2(c1, c2) (((c1) << 8) | (c2))
  typedef unsigned Letter2;
  typedef long Length;
  struct LengthSpec {
    LengthSpec(long len = 0) : length(len), displaySizeFactor(0.0) { }
    long length;
    double displaySizeFactor;
    operator bool() const { return length != 0 || displaySizeFactor != 0.0; }
  };
  struct TableLengthSpec : LengthSpec {
    TableLengthSpec() : tableUnitFactor(0.0) { }
    double tableUnitFactor;
  };
  struct Extension {
    const char *pubid;
    void (FOTBuilder::*boolSetter)(bool);
    void (FOTBuilder::*stringSetter)(const StringC &);
    void (FOTBuilder::*integerSetter)(long);
    void (FOTBuilder::*lengthSetter)(Length);
  };
  struct OptLengthSpec {
    OptLengthSpec() : hasLength(0) { }
    bool hasLength;
    LengthSpec length;
  };
  struct DisplaySpace {
    DisplaySpace() : priority(0), conditional(1), force(0) { }
    LengthSpec nominal;
    LengthSpec min;
    LengthSpec max;
    long priority;
    bool conditional;
    bool force;
  };
  struct InlineSpace {
    LengthSpec nominal;
    LengthSpec min;
    LengthSpec max;
  };
  // non-inherited characteristics for all displayed flow objects
  struct DisplayNIC {
    DisplayNIC();
    DisplaySpace spaceBefore;
    DisplaySpace spaceAfter;
    Symbol keep;
    Symbol breakBefore;
    Symbol breakAfter;
    bool keepWithPrevious;
    bool keepWithNext;
    bool mayViolateKeepBefore;
    bool mayViolateKeepAfter;
  };
  struct InlineNIC {
    InlineNIC();
    long breakBeforePriority;
    long breakAfterPriority;
  };
  struct DisplayGroupNIC : DisplayNIC {
    DisplayGroupNIC();
    bool hasCoalesceId;
    StringC coalesceId;
  };
  struct ExternalGraphicNIC : DisplayNIC, InlineNIC {
    ExternalGraphicNIC();
    bool isDisplay;
    Symbol scaleType; // symbolFalse if not a symbol
    double scale[2]; // if scaleType == symbolFalse
    StringC entitySystemId;
    StringC notationSystemId;
    bool hasMaxWidth;
    LengthSpec maxWidth;
    bool hasMaxHeight;
    LengthSpec maxHeight;
    // These apply only if !isDisplay
    // symbolFalse if no escapementDirection
    Symbol escapementDirection;
    LengthSpec positionPointX;
    LengthSpec positionPointY;
  };
  struct BoxNIC : DisplayNIC, InlineNIC {
    BoxNIC();
    bool isDisplay;
  };
  struct RuleNIC : DisplayNIC, InlineNIC {
    RuleNIC();
    Symbol orientation;
    bool hasLength;
    LengthSpec length;
  };
  struct LeaderNIC : InlineNIC {
    LeaderNIC();
    bool hasLength;
    LengthSpec length;
  };
  // A paragraph has the same non-inherited characteristics
  // as a display-group.
  typedef DisplayNIC ParagraphNIC;
  struct CharacterNIC {
    CharacterNIC();
    enum {
      cIsDropAfterLineBreak,
      cIsDropUnlessBeforeLineBreak,
      cIsPunct,
      cIsInputWhitespace,
      cIsInputTab,
      cIsRecordEnd,
      cIsSpace,
      cChar,
      cGlyphId,
      cScript,
      cMathClass,
      cMathFontPosture,
      cBreakBeforePriority,
      cBreakAfterPriority
    };
    unsigned specifiedC;
    Char ch;
    GlyphId glyphId;
    long breakBeforePriority;
    long breakAfterPriority;
    Symbol mathClass;
    Symbol mathFontPosture;
    PublicId script;
    bool isDropAfterLineBreak;
    bool isDropUnlessBeforeLineBreak;
    bool isPunct;
    bool isInputWhitespace;
    bool isInputTab;
    bool isRecordEnd;
    bool isSpace;
    // This doesn't have a flag in specifiedC,
    // because it has a fixed default.
    double stretchFactor;
  };
  typedef InlineNIC LineFieldNIC;
  struct TableNIC : public DisplayNIC {
    TableNIC();
    enum WidthType {
      widthFull,
      widthMinimum,
      widthExplicit
    };
    WidthType widthType;
    LengthSpec width;
  };
  typedef DisplayNIC TablePartNIC;
  struct TableColumnNIC {
    TableColumnNIC();
    unsigned columnIndex;  // zero-based
    unsigned nColumnsSpanned;
    bool hasWidth;
    TableLengthSpec width;
  };
  struct TableCellNIC {
    TableCellNIC();
    // If true, this isn't a real table cell.
    // Rather it's one that can be used to fill in cells missing in this row.
    bool missing;
    unsigned columnIndex;  // zero-based
    unsigned nColumnsSpanned;
    unsigned nRowsSpanned;
  };
  struct DeviceRGBColor {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
  };
  struct MultiMode {
    MultiMode();
    bool hasDesc;
    StringC name;
    StringC desc;
  };
  struct Address {
    enum Type {
      // An address of #f
      none,
      // An address that was resolved by the front-end to a node
      // Only the node member is valid.
      resolvedNode,
      // node contains current node, params[0] is string.
      idref,
      entity,
      sgmlDocument,
      hytimeLinkend,
      tei,
      html
    };
    Type type;
    // Gives current node or resolved node.
    NodePtr node;
    StringC params[3];
  };
  virtual SaveFOTBuilder *asSaveFOTBuilder();
  virtual ~FOTBuilder();
  // Default for compound flow objects
  virtual void start();
  virtual void end();
  // Default for atomic flow objects: defaults to start(); end();
  virtual void atomic();
  // Atomic flow objects
  virtual void characters(const Char *, size_t);
  // Like characters(), but data comes from charChunk() on a NodePtr.
  // If you are building a tree, then the NodePtr can be saved instead
  // of the data.  The default implementation calls characters().
  virtual void charactersFromNode(const NodePtr &, const Char *, size_t);
  virtual void character(const CharacterNIC &);
  virtual void paragraphBreak(const ParagraphNIC &);
  virtual void externalGraphic(const ExternalGraphicNIC &);
  virtual void rule(const RuleNIC &);
  virtual void alignmentPoint();
  // For flow object class public id
  // "UNREGISTERED::James Clark//Flow Object Class::formatting-instruction"
  virtual void formattingInstruction(const StringC &);
  // Non-atomic flow objects
  virtual void startSequence();
  virtual void endSequence();
  virtual void startLineField(const LineFieldNIC &);
  virtual void endLineField();
  virtual void startParagraph(const ParagraphNIC &);
  virtual void endParagraph();
  virtual void startDisplayGroup(const DisplayGroupNIC &);
  virtual void endDisplayGroup();
  virtual void startScroll();
  virtual void endScroll();
  virtual void startLink(const Address &);
  virtual void endLink();
  virtual void startMarginalia();
  virtual void endMarginalia();
  virtual void startMultiMode(const MultiMode *principalPort,  // null if no principal port
			      const Vector<MultiMode> &namedPorts,
			      Vector<FOTBuilder *> &);
  virtual void endMultiMode();
  virtual void startScore(Char);
  virtual void startScore(const LengthSpec &);
  virtual void startScore(Symbol);
  virtual void endScore();
  virtual void startLeader(const LeaderNIC &);
  virtual void endLeader();
  virtual void startSideline();
  virtual void endSideline();
  virtual void startBox(const BoxNIC &);
  virtual void endBox();
  // Tables
  virtual void startTable(const TableNIC &);
  virtual void endTable();
  // A call for each border is made immediately
  // after startTable(), each preceded by any appropriate set*() calls.
  virtual void tableBeforeRowBorder();
  virtual void tableAfterRowBorder();
  virtual void tableBeforeColumnBorder();
  virtual void tableAfterColumnBorder();
  virtual void startTablePart(const TablePartNIC &, FOTBuilder *&header, FOTBuilder *&footer);
  virtual void endTablePart();
  virtual void tableColumn(const TableColumnNIC &);
  virtual void startTableRow();
  virtual void endTableRow();
  virtual void startTableCell(const TableCellNIC &);
  virtual void endTableCell();
  virtual void tableCellBeforeRowBorder();
  virtual void tableCellAfterRowBorder();
  virtual void tableCellBeforeColumnBorder();
  virtual void tableCellAfterColumnBorder();
  // Implementation must set numerator and denominator
  // must be set to non-null values.
  virtual void startFraction(FOTBuilder *&numerator, FOTBuilder *&denominator);
  virtual void endFraction();
  virtual void startSimplePageSequence();
  virtual void endSimplePageSequence();
  // Headers and footers are treated like a separate port.
  // There are 24 different parts to the headers and footers
  // numbered 0 to 027; the numbers are computed by or'ing the
  // following flags.
  enum HF {
    // first page or other page
    firstHF = 01,
    otherHF = 0,
    // front page or back page
    frontHF = 02,
    backHF = 0,
    // header or footer
    headerHF = 04,
    footerHF = 0,
    // left, center or right part of header
    leftHF = 0,
    centerHF = 010,
    rightHF = 020,
    // number of different HF calls
    nHF = 030
  };
  virtual void startSimplePageSequenceHeaderFooter(unsigned);
  virtual void endSimplePageSequenceHeaderFooter(unsigned);
  virtual void endAllSimplePageSequenceHeaderFooter();
  // page-number sosofo
  virtual void pageNumber();
  // Inherited characteristics
  // Set the value of the characteristic for the next flow object.
  // Inherited characteristics that are not explicitly set have
  // the same value as the parent flow object.
  virtual void setFontSize(Length);
  virtual void setFontFamilyName(const StringC &);
  virtual void setFontWeight(Symbol);
  virtual void setFontPosture(Symbol);
  virtual void setStartIndent(const LengthSpec &);
  virtual void setEndIndent(const LengthSpec &);
  virtual void setFirstLineStartIndent(const LengthSpec &);
  virtual void setLastLineEndIndent(const LengthSpec &);
  virtual void setLineSpacing(const LengthSpec &);
  virtual void setFieldWidth(const LengthSpec &);
  virtual void setMarginaliaSep(const LengthSpec &);
  virtual void setLines(Symbol);
  virtual void setQuadding(Symbol);
  virtual void setDisplayAlignment(Symbol);
  virtual void setFieldAlign(Symbol);
  virtual void setColor(const DeviceRGBColor &);
  virtual void setBackgroundColor(); // background of #f
  virtual void setBackgroundColor(const DeviceRGBColor &);
  virtual void setBorderPresent(bool);
  virtual void setLineThickness(Length);
  virtual void setCellBeforeRowMargin(Length);
  virtual void setCellAfterRowMargin(Length);
  virtual void setCellBeforeColumnMargin(Length);
  virtual void setCellAfterColumnMargin(Length);
  virtual void setLineSep(Length);
  virtual void setBoxSizeBefore(Length);
  virtual void setBoxSizeAfter(Length);
  virtual void setPositionPointShift(const LengthSpec &);
  virtual void setStartMargin(const LengthSpec &);
  virtual void setEndMargin(const LengthSpec &);
  virtual void setSidelineSep(const LengthSpec &);
  virtual void setAsisWrapIndent(const LengthSpec &);
  virtual void setLineNumberSep(const LengthSpec &);
  virtual void setLastLineJustifyLimit(const LengthSpec &);
  virtual void setJustifyGlyphSpaceMaxAdd(const LengthSpec &);
  virtual void setJustifyGlyphSpaceMaxRemove(const LengthSpec &);
  virtual void setTableCornerRadius(const LengthSpec &);
  virtual void setBoxCornerRadius(const LengthSpec &);
  virtual void setMinPreLineSpacing(const OptLengthSpec &);
  virtual void setMinPostLineSpacing(const OptLengthSpec &);
  virtual void setMinLeading(const OptLengthSpec &);
  virtual void setInhibitLineBreaks(bool);
  virtual void setHyphenate(bool);
  virtual void setKern(bool);
  virtual void setLigature(bool);
  virtual void setScoreSpaces(bool);
  virtual void setFloatOutMarginalia(bool);
  virtual void setFloatOutSidelines(bool);
  virtual void setFloatOutLineNumbers(bool);
  virtual void setCellBackground(bool);
  virtual void setSpanWeak(bool);
  virtual void setIgnoreRecordEnd(bool);
  virtual void setNumberedLines(bool);
  virtual void setHangingPunct(bool);
  virtual void setBoxOpenEnd(bool);
  virtual void setTruncateLeader(bool);
  virtual void setAlignLeader(bool);
  virtual void setTablePartOmitMiddleHeader(bool);
  virtual void setTablePartOmitMiddleFooter(bool);
  virtual void setBorderOmitAtBreak(bool);
  virtual void setPrincipalModeSimultaneous(bool);
  virtual void setMarginaliaKeepWithPrevious(bool);
  virtual void setLineJoin(Symbol);
  virtual void setLineCap(Symbol);
  virtual void setLineNumberSide(Symbol);
  virtual void setKernMode(Symbol);
  virtual void setInputWhitespaceTreatment(Symbol);
  virtual void setFillingDirection(Symbol);
  virtual void setWritingMode(Symbol);
  virtual void setLastLineQuadding(Symbol);
  virtual void setMathDisplayMode(Symbol);
  virtual void setBoxType(Symbol);
  virtual void setGlyphAlignmentMode(Symbol);
  virtual void setBoxBorderAlignment(Symbol);
  virtual void setCellRowAlignment(Symbol);
  virtual void setBorderAlignment(Symbol);
  virtual void setSidelineSide(Symbol);
  virtual void setHyphenationKeep(Symbol);
  virtual void setPositionPreference(Symbol);
  virtual void setFontStructure(Symbol);
  virtual void setFontProportionateWidth(Symbol);
  virtual void setCellCrossed(Symbol);
  virtual void setMarginaliaSide(Symbol);
  virtual void setLayer(long);
  virtual void setBackgroundLayer(long);
  virtual void setBorderPriority(long);
  virtual void setLineRepeat(long);
  virtual void setSpan(long);
  virtual void setMinLeaderRepeat(long);
  virtual void setHyphenationRemainCharCount(long);
  virtual void setHyphenationPushCharCount(long);
  virtual void setWidowCount(long);
  virtual void setOrphanCount(long);
  // 0 means #f
  virtual void setExpandTabs(long);
  virtual void setHyphenationLadderCount(long);
  // public id or #f
  virtual void setBackgroundTile(PublicId);
  virtual void setLineBreakingMethod(PublicId);
  virtual void setLineCompositionMethod(PublicId);
  virtual void setImplicitBidiMethod(PublicId);
  virtual void setGlyphSubstMethod(PublicId);
  virtual void setGlyphReorderMethod(PublicId);
  virtual void setHyphenationMethod(PublicId);
  virtual void setTableAutoWidthMethod(PublicId);
  virtual void setFontName(PublicId);
  // Two-letter code
  virtual void setLanguage(Letter2);
  virtual void setCountry(Letter2);
  virtual void setEscapementSpaceBefore(const InlineSpace &);
  virtual void setEscapementSpaceAfter(const InlineSpace &);
  // For simple page sequence
  virtual void setPageWidth(Length);
  virtual void setPageHeight(Length);
  virtual void setLeftMargin(Length);
  virtual void setRightMargin(Length);
  virtual void setTopMargin(Length);
  virtual void setBottomMargin(Length);
  virtual void setHeaderMargin(Length);
  virtual void setFooterMargin(Length);
  // Others
  virtual void setGlyphSubstTable(const Vector<ConstPtr<GlyphSubstTable> > &);
  // Backlinks
  // processingMode is empty for initial processing mode.
  enum RuleType {
    ruleNone,
    ruleRoot,
    ruleDefault,
    ruleQuery,
    ruleId,
    ruleElement1,		// an element rule with 1 GI
    ruleElement2,		// an element rule with 2 GI
    ruleElement3,		// an element rule with 3 GIs
    ruleElement30000 = ruleElement1 + 29999
    };
  virtual void startNode(const NodePtr &, const StringC &processingMode,
			 RuleType ruleType);
  virtual void endNode();
  virtual void currentNodePageNumber(const NodePtr &);

  virtual void extensionSet(void (FOTBuilder::*)(bool), bool);
  virtual void extensionSet(void (FOTBuilder::*)(const StringC &), const StringC &);
  virtual void extensionSet(void (FOTBuilder::*)(long), long);
  
  static const char *symbolName(Symbol);
};

class STYLE_API SaveFOTBuilder : public Link, public FOTBuilder {
public:
  SaveFOTBuilder();
  SaveFOTBuilder(const NodePtr &, const StringC &processingMode,
		 RuleType ruleType);
 ~SaveFOTBuilder();
  SaveFOTBuilder *asSaveFOTBuilder();
  void emit(FOTBuilder &);
  void characters(const Char *, size_t);
  void charactersFromNode(const NodePtr &, const Char *, size_t);
  void character(const CharacterNIC &);
  void paragraphBreak(const ParagraphNIC &);
  void externalGraphic(const ExternalGraphicNIC &);
  void rule(const RuleNIC &);
  void alignmentPoint();
  void formattingInstruction(const StringC &);
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
  void startMultiMode(const MultiMode *principalPort,
                      const Vector<MultiMode> &namedPorts,
		      Vector<FOTBuilder *> &);
  void endMultiMode();
  void startMarginalia();
  void endMarginalia();
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
  void startSimplePageSequence();
  void endSimplePageSequence();
  // Headers and footers are treated like a separate port.
  void startSimplePageSequenceHeaderFooter(unsigned);
  void endSimplePageSequenceHeaderFooter(unsigned);
  void endAllSimplePageSequenceHeaderFooter();
  // page-number sosofo
  void pageNumber();
  void startFraction(FOTBuilder *&numerator, FOTBuilder *&denominator);
  void endFraction();
  void startTable(const TableNIC &);
  void endTable();
  void tableBeforeRowBorder();
  void tableAfterRowBorder();
  void tableBeforeColumnBorder();
  void tableAfterColumnBorder();
  void startTablePart(const TablePartNIC &, FOTBuilder *&header, FOTBuilder *&footer);
  void endTablePart();
  void tableColumn(const TableColumnNIC &);
  void startTableRow();
  void endTableRow();
  void startTableCell(const TableCellNIC &);
  void endTableCell();
  void tableCellBeforeRowBorder();
  void tableCellAfterRowBorder();
  void tableCellBeforeColumnBorder();
  void tableCellAfterColumnBorder();
  // Inherited characteristics
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
  void setLines(Symbol);
  void setQuadding(Symbol);
  void setDisplayAlignment(Symbol);
  void setFieldAlign(Symbol);
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
  void setMarginaliaSep(const LengthSpec &);
  void setMinPreLineSpacing(const OptLengthSpec &);
  void setMinPostLineSpacing(const OptLengthSpec &);
  void setMinLeading(const OptLengthSpec &);
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
  void setExpandTabs(long);
  void setHyphenationLadderCount(long);
  void setBackgroundTile(PublicId);
  void setLineBreakingMethod(PublicId);
  void setLineCompositionMethod(PublicId);
  void setImplicitBidiMethod(PublicId);
  void setGlyphSubstMethod(PublicId);
  void setGlyphReorderMethod(PublicId);
  void setHyphenationMethod(PublicId);
  void setTableAutoWidthMethod(PublicId);
  void setFontName(PublicId);
  void setLanguage(Letter2);
  void setCountry(Letter2);
  void setEscapementSpaceBefore(const InlineSpace &);
  void setEscapementSpaceAfter(const InlineSpace &);
  void setGlyphSubstTable(const Vector<ConstPtr<GlyphSubstTable> > &);
  void startNode(const NodePtr &, const StringC &processingMode, RuleType);
  void endNode();
  void currentNodePageNumber(const NodePtr &);
  void extensionSet(void (FOTBuilder::*)(bool), bool);
  void extensionSet(void (FOTBuilder::*)(const StringC &), const StringC &);
  void extensionSet(void (FOTBuilder::*)(long), long);

  struct Call {
    virtual ~Call();
    virtual void emit(FOTBuilder &) = 0;
    Call *next;
  };
private:
  SaveFOTBuilder(const SaveFOTBuilder &); // undefined
  void operator=(const SaveFOTBuilder &); // undefined
  struct NoArgCall : Call {
    typedef void (FOTBuilder::*FuncPtr)();
    NoArgCall(FuncPtr f) : func(f) { }
    void emit(FOTBuilder &);
    FuncPtr func;
  };
  struct LongArgCall : Call {
    typedef void (FOTBuilder::*FuncPtr)(long);
    LongArgCall(FuncPtr f, long n) : func(f), arg(n) { }
    void emit(FOTBuilder &);
    FuncPtr func;
    long arg;
  };
  struct ExtensionLongArgCall : Call {
    typedef void (FOTBuilder::*FuncPtr)(long);
    ExtensionLongArgCall(FuncPtr f, long n) : func(f), arg(n) { }
    void emit(FOTBuilder &);
    FuncPtr func;
    long arg;
  };
  struct LengthSpecArgCall : Call {
    typedef void (FOTBuilder::*FuncPtr)(const LengthSpec &);
    LengthSpecArgCall(FuncPtr f, const LengthSpec &ls) : func(f), arg(ls) { }
    void emit(FOTBuilder &);
    FuncPtr func;
    LengthSpec arg;
  };
  struct OptLengthSpecArgCall : Call {
    typedef void (FOTBuilder::*FuncPtr)(const OptLengthSpec &);
    OptLengthSpecArgCall(FuncPtr f, const OptLengthSpec &ols) : func(f), arg(ols) { }
    void emit(FOTBuilder &);
    FuncPtr func;
    OptLengthSpec arg;
  };
  struct NodePtrArgCall : Call {
    typedef void (FOTBuilder::*FuncPtr)(const NodePtr &);
    NodePtrArgCall(FuncPtr f, const NodePtr &nd) : func(f), arg(nd) { }
    void emit(FOTBuilder &);
    FuncPtr func;
    NodePtr arg;
  };
  struct StringArgCall : Call {
    typedef void (FOTBuilder::*FuncPtr)(const StringC &);
    StringArgCall(FuncPtr f, const StringC &s) : func(f), arg(s) { }
    void emit(FOTBuilder &);
    FuncPtr func;
    StringC arg;
  };
  struct ExtensionStringArgCall : Call {
    typedef void (FOTBuilder::*FuncPtr)(const StringC &);
    ExtensionStringArgCall(FuncPtr f, const StringC &s) : func(f), arg(s) { }
    void emit(FOTBuilder &);
    FuncPtr func;
    StringC arg;
  };
   struct CharArgCall : Call {
    typedef void (FOTBuilder::*FuncPtr)(Char);
    CharArgCall(FuncPtr f, Char c) : func(f), arg(c) { }
    void emit(FOTBuilder &);
    FuncPtr func;
    Char arg;
  };
  struct BoolArgCall : Call {
    typedef void (FOTBuilder::*FuncPtr)(bool);
    BoolArgCall(FuncPtr f, bool b) : func(f), arg(b) { }
    void emit(FOTBuilder &);
    FuncPtr func;
    bool arg;
  };
  struct ExtensionBoolArgCall : Call {
    typedef void (FOTBuilder::*FuncPtr)(bool);
    ExtensionBoolArgCall(FuncPtr f, bool b) : func(f), arg(b) { }
    void emit(FOTBuilder &);
    FuncPtr func;
    bool arg;
  };
  struct SymbolArgCall : Call {
    typedef void (FOTBuilder::*FuncPtr)(Symbol);
    SymbolArgCall(FuncPtr f, Symbol sym) : func(f), arg(sym) { }
    void emit(FOTBuilder &);
    FuncPtr func;
    Symbol arg;
  };
  struct PublicIdArgCall : Call {
    typedef void (FOTBuilder::*FuncPtr)(PublicId);
    PublicIdArgCall(FuncPtr f, PublicId pubid) : func(f), arg(pubid) { }
    void emit(FOTBuilder &);
    FuncPtr func;
    PublicId arg;
  };
  struct UnsignedArgCall : Call {
    typedef void (FOTBuilder::*FuncPtr)(unsigned);
    UnsignedArgCall(FuncPtr f, unsigned n) : func(f), arg(n) { }
    void emit(FOTBuilder &);
    FuncPtr func;
    unsigned arg;
  };
  struct DeviceRGBColorArgCall : Call {
    typedef void (FOTBuilder::*FuncPtr)(const DeviceRGBColor &);
    DeviceRGBColorArgCall(FuncPtr f, const DeviceRGBColor &color) : func(f), arg(color) { }
    void emit(FOTBuilder &);
    FuncPtr func;
    DeviceRGBColor arg;
  };
  struct InlineSpaceArgCall : Call {
    typedef void (FOTBuilder::*FuncPtr)(const InlineSpace &);
    InlineSpaceArgCall(FuncPtr f, InlineSpace is) : func(f), arg(is) { }
    void emit(FOTBuilder &);
    FuncPtr func;
    InlineSpace arg;
  };
  struct CharactersFromNodeCall : Call {
    CharactersFromNodeCall(const NodePtr &, const Char *, size_t);
    void emit(FOTBuilder &);
    const Char *data;
    size_t size;
    NodePtr node;
  };
  struct CharactersCall : Call {
    CharactersCall(const Char *, size_t);
    void emit(FOTBuilder &);
    StringC str;
  };
  struct CharacterCall : Call {
    CharacterCall(const CharacterNIC &nic) : arg(nic) { }
    void emit(FOTBuilder &);
    CharacterNIC arg;
  };
  struct StartNodeCall : Call {
    StartNodeCall(const NodePtr &, const StringC &, RuleType);
    void emit(FOTBuilder &);
    NodePtr node;
    StringC mode;
    RuleType rule;
  };
  struct StartParagraphCall : Call {
    StartParagraphCall(const ParagraphNIC &nic);
    void emit(FOTBuilder &);
    ParagraphNIC arg;
  };
  struct ParagraphBreakCall : Call {
    ParagraphBreakCall(const ParagraphNIC &nic);
    void emit(FOTBuilder &);
    ParagraphNIC arg;
  };
  struct StartDisplayGroupCall : Call {
    StartDisplayGroupCall(const DisplayGroupNIC &nic);
    void emit(FOTBuilder &);
    DisplayGroupNIC arg;
  };
  struct StartBoxCall : Call {
    StartBoxCall(const BoxNIC &nic) : arg(nic) { }
    void emit(FOTBuilder &);
    BoxNIC arg;
  };
  struct StartLineFieldCall : Call {
    StartLineFieldCall(const LineFieldNIC &nic) : arg(nic) { }
    void emit(FOTBuilder &);
    LineFieldNIC arg;
  };
  struct ExternalGraphicCall : Call {
    ExternalGraphicCall(const ExternalGraphicNIC &nic) : arg(nic) { }
    void emit(FOTBuilder &);
    ExternalGraphicNIC arg;
  };
  struct RuleCall : Call {
    RuleCall(const RuleNIC &nic) : arg(nic) { }
    void emit(FOTBuilder &);
    RuleNIC arg;
  };
  struct StartLeaderCall : Call {
    StartLeaderCall(const LeaderNIC &nic) : arg(nic) { }
    void emit(FOTBuilder &);
    LeaderNIC arg;
  };
  struct StartTableCall : Call {
    StartTableCall(const TableNIC &nic) : arg(nic) { }
    void emit(FOTBuilder &);
    TableNIC arg;
  };
  struct TableColumnCall : Call {
    TableColumnCall(const TableColumnNIC &nic) : arg(nic) { }
    void emit(FOTBuilder &);
    TableColumnNIC arg;
  };
  struct StartTableCellCall : Call {
    StartTableCellCall(const TableCellNIC &nic) : arg(nic) { }
    void emit(FOTBuilder &);
    TableCellNIC arg;
  };
  struct StartLinkCall : Call {
    StartLinkCall(const Address &addr) : arg(addr) { }
    void emit(FOTBuilder &);
    Address arg;
  };
  struct SetGlyphSubstTableCall : Call {
    SetGlyphSubstTableCall(const Vector<ConstPtr<GlyphSubstTable> > &tables) : arg(tables) { }
    void emit(FOTBuilder &);
    Vector<ConstPtr<GlyphSubstTable> > arg;
  };
  Call *calls_;
  Call **tail_;
  NodePtr currentNode_;
  StringC processingMode_;
  RuleType ruleType_;
};

// Would like to make this a member of SaveFOTBuilder, but can't because
// it has members that are instances of SaveFOTBuilder.

struct StartFractionCall : SaveFOTBuilder::Call {
  StartFractionCall(FOTBuilder *&numerator, FOTBuilder *&denominator);
  void emit(FOTBuilder &);
  SaveFOTBuilder numerator;
  SaveFOTBuilder denominator;
};

struct StartTablePartCall : SaveFOTBuilder::Call {
  StartTablePartCall(const FOTBuilder::TablePartNIC &nic, FOTBuilder *&header, FOTBuilder *&footer);
  void emit(FOTBuilder &);
  FOTBuilder::TablePartNIC arg;
  SaveFOTBuilder header;
  SaveFOTBuilder footer;
};

struct StartMultiModeCall : SaveFOTBuilder::Call {
  StartMultiModeCall(const FOTBuilder::MultiMode *principalMode,
	             const Vector<FOTBuilder::MultiMode> &namedModes,
	             Vector<FOTBuilder *> &ports);
  void emit(FOTBuilder &);
  FOTBuilder::MultiMode principalMode;
  bool hasPrincipalMode;
  Vector<FOTBuilder::MultiMode> namedModes;
  IList<SaveFOTBuilder> ports;
};

// This uses SaveFOTBuilder to provide a serial view of multi-port objects.

class STYLE_API SerialFOTBuilder : public FOTBuilder {
public:
  SerialFOTBuilder();
  // Instead of overriding these
  void startFraction(FOTBuilder *&numerator, FOTBuilder *&denominator);
  void endFraction();
  void startTablePart(const TablePartNIC &, FOTBuilder *&header, FOTBuilder *&footer);
  void endTablePart();
  void startMultiMode(const MultiMode *,
                      const Vector<MultiMode> &,
		      Vector<FOTBuilder *> &);
  void endMultiMode();
  // Override these
  virtual void startFractionSerial();
  virtual void endFractionSerial();
  virtual void startFractionNumerator();
  virtual void endFractionNumerator();
  virtual void startFractionDenominator();
  virtual void endFractionDenominator();
  virtual void startTablePartSerial(const TablePartNIC &);
  virtual void endTablePartSerial();
  virtual void startTablePartHeader();
  virtual void endTablePartHeader();
  virtual void startTablePartFooter();
  virtual void endTablePartFooter();
  virtual void startMultiModeSerial(const MultiMode *);
  virtual void endMultiModeSerial();
  virtual void startMultiModeMode(const MultiMode &);
  virtual void endMultiModeMode();
private:
  IList<SaveFOTBuilder> save_;
  Vector<Vector<MultiMode> > multiModeStack_;
};

inline
bool operator==(const FOTBuilder::GlyphId &g1, const FOTBuilder::GlyphId &g2)
{
  return g1.publicId == g2.publicId && g1.suffix == g2.suffix;
}

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* not FOTBuilder_INCLUDED */

