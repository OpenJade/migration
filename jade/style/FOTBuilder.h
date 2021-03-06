// Copyright (c) 1996, 1997 James Clark
// See the file copying.txt for copying permission.

#ifndef FOTBuilder_INCLUDED
#define FOTBuilder_INCLUDED 1

#include <OpenSP/types.h>
#include <OpenSP/StringC.h>
#include <OpenSP/Boolean.h>
#include "Node.h"
#include <OpenSP/Link.h>
#include <OpenSP/IList.h>
#include <OpenSP/Vector.h>
#include <OpenSP/Resource.h>
#include <OpenSP/Ptr.h>
#include <OpenSP/Owner.h>
#include <stddef.h>
#include <string.h>
#include "dsssl_ns.h"

// This is a work around for a bug in Sun C++ 4.1.
// The class local typedef of PublicId will cause it to reject a
// subsequent declaration of the class PublicId, but it's happy
// if it sees the class first.
#ifdef __SUNPRO_CC
#include <OpenSP/ExternalId.h>
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
    symbolInitial,
    symbolFinal,
    symbolWrap,
    symbolAsis,
    symbolAsisWrap,
    symbolAsisTruncate,
    symbolNone,
    symbolIndent,
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
    symbolBevel,
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
    symbolForce,
    symbolIndependent,
    symbolPile,
    symbolSupOut,
    symbolSubOut,
    symbolLeadEdge,
    symbolTrailEdge,
    symbolExplicit,
    symbolRowMajor,
    symbolColumnMajor,
    symbolUppercase,
    symbolLowercase,
    symbolCapitalize
  };
  enum { nSymbols = symbolCapitalize + 1 };
  typedef const char *PublicId;
  struct STYLE_API GlyphId {
    GlyphId() : publicId(0), suffix(0) { }
    GlyphId(const char *s, unsigned long n = 0) : publicId(s), suffix(n) { }
    PublicId publicId;
    // If suffix is non-zero, then add a double-colon plus
    // the suffix in decimal onto publicId to get the complete
    // public identifier of the glyph id.
    unsigned long suffix;
  };
  struct STYLE_API GlyphSubstTable : public Resource {
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
  struct STYLE_API TableLengthSpec : LengthSpec {
    TableLengthSpec() : tableUnitFactor(0.0) { }
    double tableUnitFactor;
  };
  struct STYLE_API OptLengthSpec {
    OptLengthSpec() : hasLength(0) { }
    bool hasLength;
    LengthSpec length;
  };
  struct STYLE_API DisplaySpace {
    DisplaySpace() : priority(0), conditional(1), force(0) { }
    LengthSpec nominal;
    LengthSpec min;
    LengthSpec max;
    long priority;
    bool conditional;
    bool force;
  };
  struct STYLE_API InlineSpace {
    LengthSpec nominal;
    LengthSpec min;
    LengthSpec max;
  };
  struct STYLE_API OptInlineSpace {
    OptInlineSpace() : hasSpace(0) { }
    bool hasSpace;
    InlineSpace space;
  };
  // non-inherited characteristics for all displayed flow objects
  struct STYLE_API DisplayNIC {
    DisplayNIC();
    DisplaySpace spaceBefore;
    DisplaySpace spaceAfter;
    Symbol positionPreference;
    Symbol keep;
    Symbol breakBefore;
    Symbol breakAfter;
    bool keepWithPrevious;
    bool keepWithNext;
    bool mayViolateKeepBefore;
    bool mayViolateKeepAfter;
  };
  struct STYLE_API InlineNIC {
    InlineNIC();
    long breakBeforePriority;
    long breakAfterPriority;
  };
  struct STYLE_API DisplayGroupNIC : DisplayNIC {
    DisplayGroupNIC();
    bool hasCoalesceId;
    StringC coalesceId;
  };
  struct STYLE_API ExternalGraphicNIC : DisplayNIC, InlineNIC {
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
  struct STYLE_API BoxNIC : DisplayNIC, InlineNIC {
    BoxNIC();
    bool isDisplay;
  };
  struct STYLE_API RuleNIC : DisplayNIC, InlineNIC {
    RuleNIC();
    Symbol orientation;
    bool hasLength;
    LengthSpec length;
  };
  struct STYLE_API LeaderNIC : InlineNIC {
    LeaderNIC();
    bool hasLength;
    LengthSpec length;
  };
  // A paragraph has the same non-inherited characteristics
  // as a display-group.
  typedef DisplayNIC ParagraphNIC;
  struct STYLE_API CharacterNIC {
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
  struct STYLE_API TableNIC : public DisplayNIC {
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
  struct STYLE_API TableColumnNIC {
    TableColumnNIC();
    unsigned columnIndex;  // zero-based
    unsigned nColumnsSpanned;
    bool hasWidth;
    TableLengthSpec width;
  };
  struct STYLE_API TableCellNIC {
    TableCellNIC();
    // If true, this isn't a real table cell.
    // Rather it's one that can be used to fill in cells missing in this row.
    bool missing;
    unsigned columnIndex;  // zero-based
    unsigned nColumnsSpanned;
    unsigned nRowsSpanned;
  };
  struct STYLE_API DeviceRGBColor {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
  };
  struct STYLE_API MultiMode {
    MultiMode();
    bool hasDesc;
    StringC name;
    StringC desc;
  };
  struct STYLE_API Address {
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
  virtual void characters(const Vector<CharacterNIC> &);
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
  virtual void startSideBySide(const DisplayNIC&);
  virtual void endSideBySide();
  virtual void startSideBySideItem();
  virtual void endSideBySideItem();
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
  virtual void startMathSequence();
  virtual void endMathSequence();
  virtual void startFraction(FOTBuilder *&numerator, FOTBuilder *&denominator);
  // startFraction is followed by call to fractionBar with set...() calls
  // intervening
  virtual void fractionBar();
  virtual void endFraction();
  virtual void startUnmath();
  virtual void endUnmath();
  virtual void startSuperscript();
  virtual void endSuperscript();
  virtual void startSubscript();
  virtual void endSubscript();
  virtual void startScript(FOTBuilder *&preSup,
                           FOTBuilder *&preSub,
                           FOTBuilder *&postSup,
                           FOTBuilder *&postSub,
                           FOTBuilder *&midSup,
                           FOTBuilder *&midSub);
  virtual void endScript();
  virtual void startMark(FOTBuilder *&overMark, FOTBuilder *&underMark);
  virtual void endMark();
  virtual void startFence(FOTBuilder *&open, FOTBuilder *&close);
  virtual void endFence();
  virtual void startRadical(FOTBuilder *&degree);
  // startRadical is followed by one of the following
  virtual void radicalRadical(const CharacterNIC &);
  virtual void radicalRadicalDefaulted();
  virtual void endRadical();
  virtual void startMathOperator(FOTBuilder *&oper,
                                 FOTBuilder *&lowerLimit,
                                 FOTBuilder *&upperLimit);
  virtual void endMathOperator();
  
  struct STYLE_API GridNIC {
    GridNIC();
    unsigned nColumns;
    unsigned nRows;
  };
  virtual void startGrid(const GridNIC &);
  virtual void endGrid();

  struct STYLE_API GridCellNIC {
    GridCellNIC();
    unsigned columnNumber;
    unsigned rowNumber;
  };
  virtual void startGridCell(const GridCellNIC &);
  virtual void endGridCell();

  // Simple page
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
  virtual void startSimplePageSequence(FOTBuilder* headerFooter[nHF]);
  virtual void endSimplePageSequenceHeaderFooter();
  virtual void endSimplePageSequence();

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
  virtual void setSideBySideOverlapControl(Symbol);
  virtual void setSideBySidePreAlign(Symbol);
  virtual void setSideBySidePostAlign(Symbol);
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
  virtual void setGridEquidistantRows(bool);
  virtual void setGridEquidistantColumns(bool);
  virtual void setLineJoin(Symbol);
  virtual void setLineCap(Symbol);
  virtual void setLineNumberSide(Symbol);
  virtual void setKernMode(Symbol);
  virtual void setInputWhitespaceTreatment(Symbol);
  virtual void setFillingDirection(Symbol);
  virtual void setWritingMode(Symbol);
  virtual void setLastLineQuadding(Symbol);
  virtual void setMathDisplayMode(Symbol);
  virtual void setScriptPreAlign(Symbol);
  virtual void setScriptPostAlign(Symbol);
  virtual void setScriptMidSupAlign(Symbol);
  virtual void setScriptMidSubAlign(Symbol);
  virtual void setNumeratorAlign(Symbol);
  virtual void setDenominatorAlign(Symbol);
  virtual void setGridPositionCellType(Symbol);
  virtual void setGridColumnAlignment(Symbol);
  virtual void setGridRowAlignment(Symbol);
  virtual void setBoxType(Symbol);
  virtual void setGlyphAlignmentMode(Symbol);
  virtual void setBoxBorderAlignment(Symbol);
  virtual void setCellRowAlignment(Symbol);
  virtual void setBorderAlignment(Symbol);
  virtual void setSidelineSide(Symbol);
  virtual void setHyphenationKeep(Symbol);
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
  virtual void setInlineSpaceSpace(const OptInlineSpace &);
  // If a symbol or #f. (Set to #f if a procedure, since this case is handled
  // by the engine.)
  virtual void setCharMap(Symbol);
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
  virtual void startNode(const NodePtr &, const StringC &processingMode);
  virtual void endNode();
  virtual void currentNodePageNumber(const NodePtr &);

  class CompoundExtensionFlowObj;
  class STYLE_API ExtensionFlowObj {
  public:
    virtual ~ExtensionFlowObj();
    class Value {
    public:
      virtual bool convertString(StringC &) const = 0;
      virtual bool convertStringPairList(Vector<StringC> &) const = 0;
      virtual bool convertStringList(Vector<StringC> &) const = 0;
      virtual bool convertBoolean(bool &) const = 0;
    };
    virtual CompoundExtensionFlowObj *asCompoundExtensionFlowObj();
    virtual const CompoundExtensionFlowObj *asCompoundExtensionFlowObj() const;
    virtual bool hasNIC(const StringC &) const;
    virtual void setNIC(const StringC &, const Value &);
    virtual ExtensionFlowObj *copy() const = 0;
  };
  class STYLE_API CompoundExtensionFlowObj : public ExtensionFlowObj {
  public:
    CompoundExtensionFlowObj *asCompoundExtensionFlowObj();
    const CompoundExtensionFlowObj *asCompoundExtensionFlowObj() const;
    virtual bool hasPrincipalPort() const;
    virtual void portNames(Vector<StringC> &) const;
  };

  // extension inherited characteristics

  virtual void extensionSet(void (FOTBuilder::*)(bool), bool);
  virtual void extensionSet(void (FOTBuilder::*)(const StringC &), const StringC &);
  virtual void extensionSet(void (FOTBuilder::*)(long), long);
  
  // extension flow objects
  virtual void extension(const ExtensionFlowObj &, const NodePtr &);
  virtual void startExtension(const CompoundExtensionFlowObj &,
			      const NodePtr &,
			      Vector<FOTBuilder *> &ports);
  virtual void endExtension(const CompoundExtensionFlowObj &);

  struct STYLE_API Extension {
    const char *pubid;
    void (FOTBuilder::*boolSetter)(bool);
    void (FOTBuilder::*stringSetter)(const StringC &);
    void (FOTBuilder::*integerSetter)(long);
    void (FOTBuilder::*lengthSetter)(Length);
    const ExtensionFlowObj *flowObj;
  };

  struct STYLE_API Feature {
    const char *name;
    bool partial;
  };

  struct STYLE_API Description {
    // These must be zero-terminated lists
    const Extension *extensions;
    const Feature *features;
    // Set this if you want character NICs whose default values are the values
    // of character properties to be included correctly. This degrades per-
    // formance, so if you don't care (i.e. the formatter has its own
    // values), set to false.
    bool wantCharPropertyNICs;
    // A zero-terminated list of media type names according to
    // HTML 4.0 Specification. 
    // Instead of putting "all" in there, leave the list empty 
    // to match any media type.
    const char **media;
  };

  static const char *symbolName(Symbol);
};

class STYLE_API SaveFOTBuilder : public Link, public FOTBuilder {
public:
  SaveFOTBuilder();
  SaveFOTBuilder(const SaveFOTBuilder&);
 ~SaveFOTBuilder();
  SaveFOTBuilder& operator = ( const SaveFOTBuilder& );
  void clear();
  SaveFOTBuilder *asSaveFOTBuilder();
  virtual void emit(FOTBuilder &) const;
  class Emitter;

  void characters(const Char *, size_t);
  void characters(const Vector<CharacterNIC> &);
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
  void startSideBySide(const DisplayNIC&);
  void endSideBySide();
  void startSideBySideItem();
  void endSideBySideItem();
  void startSimplePageSequence(FOTBuilder* headerFooter[nHF]);
  void endSimplePageSequenceHeaderFooter();
  void endSimplePageSequence();
  // page-number sosofo
  void pageNumber();
  // math
  void startMathSequence();
  void endMathSequence();
  void startFraction(FOTBuilder *&numerator, FOTBuilder *&denominator);
  void fractionBar();
  void endFraction();
  void startUnmath();
  void endUnmath();
  void startSuperscript();
  void endSuperscript();
  void startSubscript();
  void endSubscript();
  void startScript(FOTBuilder *&preSup,
                   FOTBuilder *&preSub,
                   FOTBuilder *&postSup,
                   FOTBuilder *&postSub,
                   FOTBuilder *&midSup,
                   FOTBuilder *&midSub);
  void endScript();
  void startMark(FOTBuilder *&overMark, FOTBuilder *&underMark);
  void endMark();
  void startFence(FOTBuilder *&open, FOTBuilder *&close);
  void endFence();
  void startRadical(FOTBuilder *&degree);
  void endRadical();
  void radicalRadical(const CharacterNIC &);
  void radicalRadicalDefaulted();
  void startMathOperator(FOTBuilder *&oper,
                         FOTBuilder *&lowerLimit,
                         FOTBuilder *&upperLimit);
  void endMathOperator();
  void startGrid(const GridNIC &);
  void endGrid();
  void startGridCell(const GridCellNIC &);
  void endGridCell();

  // Tables
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
  void setSideBySideOverlapControl(Symbol);
  void setSideBySidePreAlign(Symbol);
  void setSideBySidePostAlign(Symbol);
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
  void setGridEquidistantRows(bool);
  void setGridEquidistantColumns(bool);
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
  void setInlineSpaceSpace(const OptInlineSpace &);
  void setCharMap(Symbol);
  void setGlyphSubstTable(const Vector<ConstPtr<GlyphSubstTable> > &);
  void startNode(const NodePtr &, const StringC &processingMode);
  void endNode();
  void currentNodePageNumber(const NodePtr &);
  void extensionSet(void (FOTBuilder::*)(bool), bool);
  void extensionSet(void (FOTBuilder::*)(const StringC &), const StringC &);
  void extensionSet(void (FOTBuilder::*)(long), long);
  void extension(const ExtensionFlowObj &, const NodePtr &);
  void startExtension(const CompoundExtensionFlowObj &,
		      const NodePtr &,
		      Vector<FOTBuilder *> &ports);
  void endExtension(const CompoundExtensionFlowObj &);

  struct Call {
    Call() : next(0) {}
    virtual ~Call();
    virtual Call* clone() const = 0;
    virtual void emit(FOTBuilder &) const = 0;
    Call *next;
  };
private:
  struct NoArgCall : Call {
    virtual Call* clone() const { return new NoArgCall(*this); }
    typedef void (FOTBuilder::*FuncPtr)();
    NoArgCall(FuncPtr f) : func(f) { }
    void emit(FOTBuilder &) const;
    FuncPtr func;
  };
  struct CharactersFromNodeCall : Call {
    CharactersFromNodeCall(const NodePtr &, const Char *, size_t);
    virtual Call* clone() const { return new CharactersFromNodeCall(*this); }
    void emit(FOTBuilder &) const;
    const Char *data;
    size_t size;
    NodePtr node;
  };
  struct CharactersCall : Call {
    CharactersCall(const Char *, size_t);
    virtual Call* clone() const { return new CharactersCall(*this); }
    void emit(FOTBuilder &) const;
    StringC str;
  };
  struct StartNodeCall : Call {
    StartNodeCall(const NodePtr &, const StringC &);
    virtual Call* clone() const { return new StartNodeCall(*this); }
    void emit(FOTBuilder &) const;
    NodePtr node;
    StringC mode;
  };
  struct ExtensionCall : Call {
    ExtensionCall(const ExtensionFlowObj &fo, const NodePtr &nd)
      : arg(fo.copy()), node(nd) { }
    ExtensionCall(const ExtensionCall& other);
    virtual Call* clone() const { return new ExtensionCall(*this); }
    void emit(FOTBuilder &) const;
    Owner<ExtensionFlowObj> arg;
    NodePtr node;
  };
  struct EndExtensionCall : Call {
    EndExtensionCall(const CompoundExtensionFlowObj &fo)
      : arg(fo.copy()->asCompoundExtensionFlowObj()) { }
    EndExtensionCall(const EndExtensionCall& other);
    virtual Call* clone() const { return new EndExtensionCall(*this); }
    void emit(FOTBuilder &) const;
    Owner<CompoundExtensionFlowObj> arg;
  };
  Call *calls_;
  Call **tail_;
};

class STYLE_API NodeSaveFOTBuilder : public SaveFOTBuilder {
public:
  NodeSaveFOTBuilder(const NodePtr &, const StringC &processingMode);
  virtual void emit(FOTBuilder&) const;
private:
  NodePtr currentNode_;
  StringC processingMode_;
};

class SaveFOTBuilder::Emitter {
public:
  class Mark;
  Emitter(SaveFOTBuilder& source, FOTBuilder& sink);
  Emitter(Emitter& source, Mark from, Mark to, FOTBuilder& sink);
  void emit();
  Mark mark() const;
private:
  SaveFOTBuilder& source_;
  FOTBuilder& sink_;
  Call* from_;
  Call* to_;
  Call* where_;
};

class SaveFOTBuilder::Emitter::Mark {
public:
private:
  typedef SaveFOTBuilder::Call Call;
  Mark(Call* where) : where_(where) {}
  Call* where_;
  friend class SaveFOTBuilder::Emitter;
};

// Would like to make this a member of SaveFOTBuilder, but can't because
// they would be member templates which are poorly supported under most 
// compilers.

template <class Arg1> 
class OneArgCall : public SaveFOTBuilder::Call {
public:
  typedef void (FOTBuilder::*FuncPtr)(Arg1);
  OneArgCall(FuncPtr f, Arg1 arg1) : func_(f), arg1_(arg1) {}
  virtual SaveFOTBuilder::Call* clone() const { return new OneArgCall(*this); }
  virtual void emit(FOTBuilder& fotb) const { (fotb.*func_)(arg1_); }
private:
  FuncPtr func_;
  Arg1 arg1_;
};

template <class Arg1> 
class OneRefArgCall : public SaveFOTBuilder::Call {
public:
  typedef void (FOTBuilder::*FuncPtr)(const Arg1&);
  OneRefArgCall(FuncPtr f, const Arg1& arg1) : func_(f), arg1_(arg1) {}
  virtual SaveFOTBuilder::Call* clone() const { return new OneRefArgCall(*this); }
  virtual void emit(FOTBuilder& fotb) const { (fotb.*func_)(arg1_); }
private:
  FuncPtr func_;
  Arg1 arg1_;
};

// Would like to make this a member of SaveFOTBuilder, but can't because
// it has members that are instances of SaveFOTBuilder.

struct StartSimplePageSequenceCall : SaveFOTBuilder::Call {
  StartSimplePageSequenceCall(FOTBuilder* headerFooter[FOTBuilder::nHF]);
  virtual SaveFOTBuilder::Call* clone() const { return new StartSimplePageSequenceCall(*this); }
  void emit(FOTBuilder&) const;
  SaveFOTBuilder headerFooter[FOTBuilder::nHF];
};

struct StartFractionCall : SaveFOTBuilder::Call {
  StartFractionCall(FOTBuilder *&numerator, FOTBuilder *&denominator);
  virtual SaveFOTBuilder::Call* clone() const { return new StartFractionCall(*this); }
  void emit(FOTBuilder &) const;
  SaveFOTBuilder numerator;
  SaveFOTBuilder denominator;
};

struct StartScriptCall : SaveFOTBuilder::Call {
  StartScriptCall(FOTBuilder *&preSup,
                  FOTBuilder *&preSub,
                  FOTBuilder *&postSup,
                  FOTBuilder *&postSub,
                  FOTBuilder *&midSup,
                  FOTBuilder *&midSub);
  virtual SaveFOTBuilder::Call* clone() const { return new StartScriptCall(*this); }
  void emit(FOTBuilder &) const;
  SaveFOTBuilder preSup;
  SaveFOTBuilder preSub;
  SaveFOTBuilder postSup;
  SaveFOTBuilder postSub;
  SaveFOTBuilder midSup;
  SaveFOTBuilder midSub;
};

struct StartMarkCall : public SaveFOTBuilder::Call {
  StartMarkCall(FOTBuilder *&overMark, FOTBuilder *&underMark);
  virtual SaveFOTBuilder::Call* clone() const { return new StartMarkCall(*this); }
  void emit(FOTBuilder &) const;
  SaveFOTBuilder overMark;
  SaveFOTBuilder underMark;
};

struct StartFenceCall : public SaveFOTBuilder::Call {
  StartFenceCall(FOTBuilder *&open, FOTBuilder *&close);
  virtual SaveFOTBuilder::Call* clone() const { return new StartFenceCall(*this); }
  void emit(FOTBuilder &) const;
  SaveFOTBuilder open;
  SaveFOTBuilder close;
};

struct StartRadicalCall : public SaveFOTBuilder::Call {
  StartRadicalCall(FOTBuilder *&degree);
  virtual SaveFOTBuilder::Call* clone() const { return new StartRadicalCall(*this); }
  void emit(FOTBuilder &) const;
  SaveFOTBuilder degree;
};

struct StartMathOperatorCall : public SaveFOTBuilder::Call {
  StartMathOperatorCall(FOTBuilder *&oper,
                        FOTBuilder *&lowerLimit,
                        FOTBuilder *&upperLimit);
  virtual SaveFOTBuilder::Call* clone() const { return new StartMathOperatorCall(*this); }
  void emit(FOTBuilder &) const;
  SaveFOTBuilder oper;
  SaveFOTBuilder lowerLimit;
  SaveFOTBuilder upperLimit;
};

struct StartTablePartCall : SaveFOTBuilder::Call {
  StartTablePartCall(const FOTBuilder::TablePartNIC &nic, FOTBuilder *&header, FOTBuilder *&footer);
  virtual SaveFOTBuilder::Call* clone() const { return new StartTablePartCall(*this); }
  void emit(FOTBuilder &) const;
  FOTBuilder::TablePartNIC arg;
  SaveFOTBuilder header;
  SaveFOTBuilder footer;
};

struct StartMultiModeCall : SaveFOTBuilder::Call {
  StartMultiModeCall(const FOTBuilder::MultiMode *principalMode,
	             const Vector<FOTBuilder::MultiMode> &namedModes,
	             Vector<FOTBuilder *> &ports);
  StartMultiModeCall(const StartMultiModeCall& other);
  virtual SaveFOTBuilder::Call* clone() const { return new StartMultiModeCall(*this); }
  void emit(FOTBuilder &) const;
  FOTBuilder::MultiMode principalMode;
  bool hasPrincipalMode;
  Vector<FOTBuilder::MultiMode> namedModes;
  IList<SaveFOTBuilder> ports;
};

struct StartExtensionCall : SaveFOTBuilder::Call {
  StartExtensionCall(const FOTBuilder::CompoundExtensionFlowObj &,
		     const NodePtr &,
		     Vector<FOTBuilder *> &);
  StartExtensionCall(const StartExtensionCall& other);
  virtual SaveFOTBuilder::Call* clone() const { return new StartExtensionCall(*this); }
  void emit(FOTBuilder &) const;
  IList<SaveFOTBuilder> ports;
  NodePtr node;
  Owner<FOTBuilder::CompoundExtensionFlowObj> flowObj;
};

// This uses SaveFOTBuilder to provide a serial view of multi-port objects.

class STYLE_API SerialFOTBuilder : public FOTBuilder {
public:
  SerialFOTBuilder();
  // Instead of overriding these
  void startSimplePageSequence(FOTBuilder* headerFooter[nHF]);
  void endSimplePageSequenceHeaderFooter();
  void endSimplePageSequence();
  void startTablePart(const TablePartNIC &,
                      FOTBuilder *&header, FOTBuilder *&footer);
  void endTablePart();
  void startFraction(FOTBuilder *&numerator, FOTBuilder *&denominator);
  void endFraction();
  void startScript(FOTBuilder *&preSup,
                   FOTBuilder *&preSub,
                   FOTBuilder *&postSup,
                   FOTBuilder *&postSub,
                   FOTBuilder *&midSup,
                   FOTBuilder *&midSub);
  void endScript();
  void startMark(FOTBuilder *&overMark, FOTBuilder *&underMark);
  void endMark();
  void startFence(FOTBuilder *&open, FOTBuilder *&close);
  void endFence();
  void startRadical(FOTBuilder *&degree);
  void endRadical();
  void startMathOperator(FOTBuilder *&oper,
                         FOTBuilder *&lowerLimit,
                         FOTBuilder *&upperLimit);
  void endMathOperator();
  void startMultiMode(const MultiMode *,
                      const Vector<MultiMode> &,
		      Vector<FOTBuilder *> &);
  void endMultiMode();
  void startExtension(const CompoundExtensionFlowObj &,
		      const NodePtr &,
		      Vector<FOTBuilder *> &ports);
  void endExtension(const CompoundExtensionFlowObj &);
  // Override these
  virtual void startSimplePageSequenceSerial();
  virtual void endSimplePageSequenceSerial();
  virtual void startSimplePageSequenceHeaderFooter(unsigned);
  virtual void endSimplePageSequenceHeaderFooter(unsigned);
  virtual void endAllSimplePageSequenceHeaderFooter();
  virtual void startFractionSerial();
  virtual void endFractionSerial();
  virtual void startFractionNumerator();
  virtual void endFractionNumerator();
  virtual void startFractionDenominator();
  virtual void endFractionDenominator();
  virtual void startScriptSerial();
  virtual void endScriptSerial();
  virtual void startScriptPreSup();
  virtual void endScriptPreSup();
  virtual void startScriptPreSub();
  virtual void endScriptPreSub();
  virtual void startScriptPostSup();
  virtual void endScriptPostSup();
  virtual void startScriptPostSub();
  virtual void endScriptPostSub();
  virtual void startScriptMidSup();
  virtual void endScriptMidSup();
  virtual void startScriptMidSub();
  virtual void endScriptMidSub();
  virtual void startMarkSerial();
  virtual void endMarkSerial();
  virtual void startMarkOver();
  virtual void endMarkOver();
  virtual void startMarkUnder();
  virtual void endMarkUnder();
  virtual void startFenceSerial();
  virtual void endFenceSerial();
  virtual void startFenceOpen();
  virtual void endFenceOpen();
  virtual void startFenceClose();
  virtual void endFenceClose();
  virtual void startRadicalSerial();
  virtual void endRadicalSerial();
  virtual void startRadicalDegree();
  virtual void endRadicalDegree();
  virtual void startMathOperatorSerial();
  virtual void endMathOperatorSerial();
  virtual void startMathOperatorOperator();
  virtual void endMathOperatorOperator();
  virtual void startMathOperatorLowerLimit();
  virtual void endMathOperatorLowerLimit();
  virtual void startMathOperatorUpperLimit();
  virtual void endMathOperatorUpperLimit();
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
  virtual void startExtensionSerial(const CompoundExtensionFlowObj &, const NodePtr &);
  virtual void endExtensionSerial(const CompoundExtensionFlowObj &);
  virtual void startExtensionStream(const StringC &);
  virtual void endExtensionStream(const StringC &);
private:
  IList<SaveFOTBuilder> save_;
  Vector<Vector<MultiMode> > multiModeStack_;
};

inline
bool operator==(const FOTBuilder::GlyphId &g1, const FOTBuilder::GlyphId &g2)
{
  return g1.publicId == g2.publicId && g1.suffix == g2.suffix;
}

bool STYLE_API operator==(const StringC &, const char *);

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* not FOTBuilder_INCLUDED */

