// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#include "config.h"
#include "RtfFOTBuilder.h"
#include "Vector.h"
#include "HashTable.h"
#include "InputSource.h"
#include "StorageManager.h"
#include "Location.h"
#include "macros.h"
#ifdef SP_SHORT_HEADERS
#include <strstrea.h>
#else
#include <strstream.h>
#endif
#include <string.h>
#include <limits.h>

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

class tmpstreambuf : public streambuf {
public:
  struct Block;
  class Iter {
  public:
    Iter(const tmpstreambuf &sb) : block_(sb.head_), lastBlockUsed_(sb.lastBlockUsed()) { }
    bool next(const char *&p, size_t &n) {
      if (block_) {
	p = block_->buf;
	n = block_->next ? tmpstreambuf::bufSize : lastBlockUsed_;
	block_ = block_->next;
	return 1;
      }
      else
	return 0;
    }
  private:
    Block *block_;
    size_t lastBlockUsed_;
  };
  tmpstreambuf();
  ~tmpstreambuf();
  int overflow(int ch);
  int sync();
  int underflow();
  streampos seekoff(streamoff, ios::seek_dir, int);
  enum { bufSize = 1024 };
private:
  friend class Iter;
  struct Block {
    Block *next;
    char buf[bufSize];
  };
  size_t lastBlockUsed() const {
  // Sun and IBM's C++ compilers think pptr() is non-const
#if defined(__SUNPRO_CC) || defined(__xlC__)
    return ((tmpstreambuf *)this)->lastBlockUsed();
  }
  size_t lastBlockUsed() {
#endif
    return last_ ? (pptr() - last_->buf) : 0;
  }
  unsigned nFullBlocks_;
  Block *head_;
  Block *last_;
};

class BitVector {
public:
  BitVector();
  void add(unsigned long);
  bool contains(unsigned long) const;
private:
  Vector<char> v_;
};

class RtfFOTBuilder : public SerialFOTBuilder {
public:
  RtfFOTBuilder(streambuf *, const Ptr<ExtendEntityManager> &, const CharsetInfo &, Messenger *);
  ~RtfFOTBuilder();
  void characters(const Char *, size_t);
  void paragraphBreak(const ParagraphNIC &);
  void externalGraphic(const ExternalGraphicNIC &);
  void rule(const RuleNIC &);
  void setFontSize(long n);
  void setFontFamilyName(const StringC &);
  void setFontWeight(Symbol);
  void setFontPosture(Symbol);
  void setGlyphSubstTable(const Vector<ConstPtr<GlyphSubstTable> > &);
  void setPositionPointShift(const LengthSpec &);
  void setLanguage(Letter2);
  void setCountry(Letter2);
  void setStartIndent(const LengthSpec &);
  void setEndIndent(const LengthSpec &);
  void setFirstLineStartIndent(const LengthSpec &);
  void setLineSpacing(const LengthSpec &);
  void setMinLeading(const OptLengthSpec &);
  void setFieldWidth(const LengthSpec &);
  void setLines(Symbol);
  void setInputWhitespaceTreatment(Symbol);
  void setExpandTabs(long);
  void setQuadding(Symbol);
  void setDisplayAlignment(Symbol);
  void setFieldAlign(Symbol);
  void setHyphenate(bool);
  void setKern(bool);
  void setHyphenationLadderCount(long);
  void setWidowCount(long);
  void setOrphanCount(long);
  void setColor(const DeviceRGBColor &);
  void setBackgroundColor(const DeviceRGBColor &);
  void setBackgroundColor();
  void setLineThickness(Length);
  void setLineRepeat(long);
  void setScoreSpaces(bool);
  void setBoxType(Symbol);
  void setBorderPresent(bool);
  void setBorderPriority(long);
  void setBorderOmitAtBreak(bool);
  void setPageWidth(long units);
  void setPageHeight(long units);
  void setLeftMargin(long units);
  void setRightMargin(long units);
  void setTopMargin(long units);
  void setBottomMargin(long units);
  void setHeaderMargin(long units);
  void setFooterMargin(long units);
  void setPageNumberRestart(bool);
  void setPageNumberFormat(const StringC &);
  void setPageNColumns(long);
  void setPageColumnSep(Length);
  void setPageBalanceColumns(bool);
  void startSimplePageSequence();
  void endSimplePageSequence();
  void startSimplePageSequenceHeaderFooter(unsigned);
  void endSimplePageSequenceHeaderFooter(unsigned);
  void endAllSimplePageSequenceHeaderFooter();
  void pageNumber();
  void startLink(const Address &);
  void endLink();
  void startScore(Symbol);
  void startBox(const BoxNIC &);
  void endBox();
  void startParagraph(const ParagraphNIC &);
  void endParagraph();
  void startDisplayGroup(const DisplayGroupNIC &);
  void endDisplayGroup();
  void startLineField(const LineFieldNIC &);
  void endLineField();
  void startLeader(const LeaderNIC &);
  void endLeader();
  void startTable(const TableNIC &);
  void endTable();
  void startTablePartSerial(const TablePartNIC &);
  void endTablePartSerial();
  void startTablePartHeader();
  void endTablePartHeader();
  void startTablePartFooter();
  void endTablePartFooter();
  void tableColumn(const TableColumnNIC &);
  void startTableRow();
  void endTableRow();
  void startTableCell(const TableCellNIC &);
  void endTableCell();
  void tableBeforeRowBorder();
  void tableAfterRowBorder();
  void tableBeforeColumnBorder();
  void tableAfterColumnBorder();
  void tableCellBeforeRowBorder();
  void tableCellAfterRowBorder();
  void tableCellBeforeColumnBorder();
  void tableCellAfterColumnBorder();
  void setCellBeforeRowMargin(Length);
  void setCellAfterRowMargin(Length);
  void setCellBeforeColumnMargin(Length);
  void setCellAfterColumnMargin(Length);
  void setCellBackground(bool);
  void startNode(const NodePtr &, const StringC &, RuleType);
  void endNode();
  void currentNodePageNumber(const NodePtr &);
  struct WinCharset {
    unsigned char charsetCode;
    const char *fontSuffix;
    Char mapping[128];
  };
  enum { nWinCharsets = 6 };
  static const WinCharset winCharsets[nWinCharsets];
  struct SymbolFont {
    const char *name;
    Char mapping[256];
  };
  enum { nSymbolFonts = 3 };
  static const SymbolFont symbolFonts[nSymbolFonts];
  enum {
    topBorder,
    bottomBorder,
    leftBorder,
    rightBorder
  };
  struct Border {
    Border() : priority(0), flags(0) { }
    long priority;
    enum {
      isPresent = 01,
      isDouble = 02,
      omitAtBreak = 04
    };
    unsigned char flags;
    unsigned char thickness;
    unsigned short color;
  };
  enum BreakType {
    breakNone,
    breakPage,
    breakColumn
  };
private:
  ostream &os() { return *osp_; }
  void start();
  void end();
  void atomic();
  void syncCharFormat();
  void setCharset(int);
  void inlinePrepare();
  void flushPendingElements();
  void startDisplay(const DisplayNIC &);
  void endDisplay();
  void newPar(bool allowSpaceBefore = 1);
  bool systemIdFilename(const StringC &systemId, String<char> &filename);
  int makeColor(const DeviceRGBColor &);
  void outputBookmarkName(const Char *, size_t);
  void outputBookmarkName(unsigned long);
  static unsigned convertLanguage(unsigned language, unsigned country,
				  unsigned &langCharsets);
  void idrefButton(const Char *s, size_t n);
  void outputTable();
  void storeBorder(Border &);
  void resolveBorder(Border &, Border &);
  void resolveOutsideBorder(Border &, const Border &);
  void outputTableBorder(const Border &);
  void outputHeaderFooter(const char *suffix, unsigned flags);
  long computeLengthSpec(const LengthSpec &);
  void displaySizeChanged();

  enum InlineState {
    inlineFirst,		// never had an inline FO
    inlineStart,		// must emit \par before next inline FO
    inlineContinue,		// like Start, but will be a continuation line
    inlineField,		// in a line field
    inlineFieldEnd,             // in a line field with align=end
    inlineMiddle,		// had some inline FOs
    inlineTable			// just after \row
  };
  InlineState inlineState_;
  enum UnderlineType {
     noUnderline,
     underlineSingle,
     underlineDouble,
     underlineWords
   };
  // What the RTF reader thinks the format is.
  struct CommonFormat {
    CommonFormat();
    bool isBold;
    bool isItalic;
    bool isSmallCaps;
    // UnderlineType
    char underline;
    bool isStrikethrough;
    int fontFamily;
    int fontSize;
    int color;
    int charBackgroundColor;
    int positionPointShift;
    unsigned language;
    unsigned country;
    bool kern;
  };
  struct OutputFormat : CommonFormat {
    OutputFormat();
    int charset;
    unsigned lang; // the RTF language code
    // bitmap of charsets that can be used for the lang
    unsigned langCharsets;
  };
  enum {
    widowControl = 01,
    orphanControl = 02
  };
  struct ParaFormat {
    ParaFormat();
    int leftIndent;
    int rightIndent;
    int firstLineIndent;
    int lineSpacing;
    bool lineSpacingAtLeast;
    char quadding;		// 'l', 'c', 'r', 'j'
    Symbol lines;
    char widowOrphanControl;
  };
  struct Format : ParaFormat, CommonFormat {
    Format();
    bool hyphenate;
    int fieldWidth;
    Symbol fieldAlign;
    Symbol inputWhitespaceTreatment;
    long expandTabs;
    char displayAlignment;      // 'l', 'c', 'r'
    long lineThickness;
    bool lineDouble;
    bool scoreSpaces;
    bool boxHasBorder;
    bool boxHasBackground;
    int backgroundColor; // 0 for none
    bool borderPresent;
    bool borderOmitAtBreak;
    bool cellBackground;
    long borderPriority;
    long cellTopMargin;
    long cellBottomMargin;
    long cellLeftMargin;
    long cellRightMargin;
    // These are needed for handling LengthSpecs
    LengthSpec positionPointShiftSpec;
    LengthSpec leftIndentSpec;
    LengthSpec rightIndentSpec;
    LengthSpec firstLineIndentSpec;
    LengthSpec lineSpacingSpec;
    LengthSpec fieldWidthSpec;
  };
  struct PageFormat {
    PageFormat();
    long pageWidth;
    long pageHeight;
    long leftMargin;
    long rightMargin;
    long headerMargin;
    long footerMargin;
    long topMargin;
    long bottomMargin;
    bool pageNumberRestart;
    const char *pageNumberFormat;
    long nColumns;
    long columnSep;
    bool balance;
  };
  // This is for a nested displayed flow object
  struct DisplayInfo {
    int spaceAfter;
    bool keepWithNext;
    bool saveKeep;
    BreakType breakAfter;
  };
  OutputFormat outputFormat_;
  Format specFormat_;
  ParaFormat paraFormat_;
  Vector<ParaFormat> paraStack_;
  Vector<DisplayInfo> displayStack_;
  Vector<Format> specFormatStack_;
  Vector<NodePtr> pendingElements_;
  Vector<unsigned> pendingElementLevels_;
  unsigned nPendingElementsNonEmpty_;
  unsigned nodeLevel_;
  Vector<size_t> displayBoxLevels_;
  bool boxFirstPara_;      // not yet had a paragraph in the outermost displayed box
  long boxSep_;
  long accumSpaceBox_;
  HashTable<StringC,int> fontFamilyNameTable_;
  struct FontFamilyCharsets {
    FontFamilyCharsets();
    int rtfFontNumber[nWinCharsets];
  };
  Vector<FontFamilyCharsets> fontFamilyCharsetsTable_;
  int nextRtfFontNumber_;
  int accumSpace_;
  bool keepWithNext_;
  bool hyphenateSuppressed_;
  long maxConsecHyphens_;
  ostream *osp_;
  ostream finalos_;
  tmpstreambuf tempbuf_;
  ostream tempos_;
  ostrstream cellos_;
  long tableWidth_;
  char tableAlignment_;
  long tableDisplaySize_;
  int tableLeftIndent_;
  int tableRightIndent_;
  long addLeftIndent_;
  long addRightIndent_;
  Border tableBorder_[4];
  unsigned cellIndex_;
  struct Cell {
    Cell() : present(0), hasBackground(0), span(1), vspan(1) { }
    bool present;
    bool hasBackground;
    unsigned short backgroundColor;
    String<char> content;
    unsigned span;
    unsigned vspan;
    Border border[4];
  };
  struct Column {
    Column() : hasWidth(0) { }
    bool hasWidth;
    TableLengthSpec width;
  };
  Vector<Vector<Cell> > cells_;
  bool inTableHeader_;
  unsigned nHeaderRows_;
  Vector<Column> columns_;
  bool inTable_;
  ostrstream fieldos_;
  int fieldTabPos_;
  long displaySize_;
  bool hadSection_;
  bool doBalance_;
  BreakType doBreak_;
  bool keep_;
  bool hadParInKeep_;
  unsigned linkDepth_;
  unsigned inSimplePageSequence_;
  PageFormat pageFormat_;
  Vector<PageFormat> pageFormatStack_;
  OutputFormat saveOutputFormat_;
  Vector<unsigned> colorTable_;
  Ptr<ExtendEntityManager> entityManager_;
  Messenger *mgr_;
  const CharsetInfo *systemCharset_;
  bool followWhitespaceChar_;
  unsigned currentColumn_;
  BitVector elementsRefed_;
  // Leaders
  unsigned leaderDepth_;
  ostrstream nullos_;
  ostream *preLeaderOsp_;
  enum { charTableSize = 0x3000 };
  enum { symbolFontFlag = 0x8000 };
  unsigned short charTable_[charTableSize];
  String<char> hfPart_[nHF];
  ostrstream hfos_;
  friend struct OutputFormat;
  friend struct Format;
  friend struct CommonFormat;
  friend struct ParaFormat;
};

inline
long RtfFOTBuilder::computeLengthSpec(const LengthSpec &spec)
{
  if (spec.displaySizeFactor == 0.0)
    return spec.length;
  else {
    double tem = displaySize_ * spec.displaySizeFactor;
    return spec.length + long(tem >= 0.0 ? tem +.5 : tem - .5);
  }
}

// Use a line-spacing of 12pt for the header and footer
// and assume 2.5pt of it occur after the baseline.

const int hfPreSpace = 190;
const int hfPostSpace = 50;
const char BOOKMARK_CHAR = '\0';
const int SYMBOL_FONT_PAGE = 0xf000;
const unsigned DEFAULT_LANG = 0x400;

inline int halfPoints(long n)
{
  return n/10;
}

tmpstreambuf::tmpstreambuf()
: head_(0), last_(0), nFullBlocks_(0)
{
}

tmpstreambuf::~tmpstreambuf()
{
  while (head_) {
    Block *tem = head_;
    head_ = head_->next;
    delete tem;
  }
}

int tmpstreambuf::overflow(int ch)
{
  if (last_ && pptr() < last_->buf + bufSize) {
    sync();
    if (ch != EOF)
      return sputc(ch);
    return 0;
  }
  Block *tem = new Block;
  char *p = tem->buf;
  if (ch != EOF)
    *p++ = ch;
  setp(p, tem->buf + bufSize);
  tem->next = 0;
  if (last_) {
    nFullBlocks_++;
    last_->next = tem;
  }
  else
    head_ = tem;
  last_ = tem;
  return 0;
}

int tmpstreambuf::sync()
{
  setp(pptr(), last_->buf + bufSize);
  return 0;
}

int tmpstreambuf::underflow()
{
  return EOF;
}

// Just enough to give us the current offset.

streampos tmpstreambuf::seekoff(streamoff off, ios::seek_dir dir,
				int mode)
{
  if ((mode & ios::out) && dir == ios::cur && off == 0) {
    if (last_ == 0)
      return 0;
    else
      return nFullBlocks_ * bufSize + (pptr() - last_->buf);
  }
  return EOF;
}

ostream &operator<<(ostream &os, const String<char> &s)
{
  for (size_t i = 0; i < s.size(); i++)
    os << s[i];
  return os;
}

FOTBuilder *makeRtfFOTBuilder(streambuf *sbuf,
			      const Ptr<ExtendEntityManager> &entityManager,
			      const CharsetInfo &systemCharset,
			      Messenger *mgr,
			      const FOTBuilder::Extension *&ext)
{
  static const FOTBuilder::Extension extensions[] = {
    {
      "UNREGISTERED::James Clark//Characteristic::page-number-format",
      0,
      (void (FOTBuilder::*)(const StringC &))&RtfFOTBuilder::setPageNumberFormat,
      0,
      0
    },
    {
      "UNREGISTERED::James Clark//Characteristic::page-number-restart?",
      (void (FOTBuilder::*)(bool))&RtfFOTBuilder::setPageNumberRestart,
      0,
      0
    },
    {
      "UNREGISTERED::James Clark//Characteristic::page-n-columns",
      0,
      0,
      (void (FOTBuilder::*)(long))&RtfFOTBuilder::setPageNColumns,
      0
    },
    {
      "UNREGISTERED::James Clark//Characteristic::page-column-sep",
      0,
      0,
      0,
      (void (FOTBuilder::*)(FOTBuilder::Length))&RtfFOTBuilder::setPageColumnSep,
    },
    {
      "UNREGISTERED::James Clark//Characteristic::page-balance-columns?",
      (void (FOTBuilder::*)(bool))&RtfFOTBuilder::setPageBalanceColumns,
      0,
      0
    },
    { 0, 0, 0}
  };
  ext = extensions;
  return new RtfFOTBuilder(sbuf, entityManager, systemCharset, mgr);
}

RtfFOTBuilder::RtfFOTBuilder(streambuf *sbuf,
			     const Ptr<ExtendEntityManager> &entityManager,
			     const CharsetInfo &systemCharset, Messenger *mgr)
: finalos_(sbuf), entityManager_(entityManager),
  systemCharset_(&systemCharset), mgr_(mgr),
  inlineState_(inlineFirst),
  tempos_(&tempbuf_), osp_(&tempos_),
  accumSpace_(0),
  keepWithNext_(0), hadSection_(0),
  linkDepth_(0),
  inTable_(0),
  nodeLevel_(0),
  nPendingElementsNonEmpty_(0),
  followWhitespaceChar_(0),
  currentColumn_(0),
  boxFirstPara_(0),
  leaderDepth_(0),
  fontFamilyCharsetsTable_(1 + nSymbolFonts),
  nextRtfFontNumber_(1),
  inTableHeader_(0),
  nHeaderRows_(0),
  displaySize_(72*10*17),
  addLeftIndent_(0),
  addRightIndent_(0),
  inSimplePageSequence_(0),
  doBalance_(0),
  hyphenateSuppressed_(0),
  maxConsecHyphens_(0),
  doBreak_(breakNone),
  keep_(0),
  hadParInKeep_(0)
{
  specFormat_.fontSize = 20; // 10 points
  specFormatStack_.push_back(specFormat_);
  pageFormatStack_.push_back(pageFormat_);
  StringC times;
  for (const char *s = "Times New Roman"; *s; s++)
    times += *s;
  fontFamilyNameTable_.insert(times, 0);
  fontFamilyCharsetsTable_[0].rtfFontNumber[0] = 0;
  for (int i = 0; i < charTableSize; i++)
    charTable_[i] = 0;
  for (int i = 0; i < nWinCharsets; i++) {
    for (int j = 0; j < 128; j++) {
      Char c = winCharsets[i].mapping[j];
      if (c) {
	if (!charTable_[c])
	  charTable_[c] = (j | 0x80 | (1 << (8 + i)));
	else if ((charTable_[i] & 0x7f) == j)
	  charTable_[c] |= (1 << (8 + i));
      }
    }
  }
  for (int i = 0; i < nSymbolFonts; i++) {
    for (int j = 0; j < 256; j++) {
      Char c = symbolFonts[i].mapping[j];
      if (c && !charTable_[c])
	charTable_[c] = (j | (i << 8) | symbolFontFlag );
    }
    StringC tem;
    for (const char *s = symbolFonts[i].name; *s; s++)
      tem += *s;
    fontFamilyNameTable_.insert(tem, i + 1);
  }
}

RtfFOTBuilder::~RtfFOTBuilder()
{
  if (doBalance_) {
    os() << "\\sect\\sbknone\\par";
    doBalance_ = 0;
  }
  os() << "}\n";
  os().flush();
  osp_ = &finalos_;
  os() << "{\\rtf1\\ansi\\deff0\n";
  os() << "{\\fonttbl";
  HashTableIter<StringC,int> iter(fontFamilyNameTable_);
  for (;;) {
    const StringC *key;
    const int *ip;
    if (!iter.next(key, ip))
      break;
    const int *charsets = fontFamilyCharsetsTable_[*ip].rtfFontNumber;
    for (int i = 0; i < nWinCharsets; i++) {
      if (charsets[i] >= 0) {
	os() << "{\\f" << charsets[i] << "\\fnil\\fcharset" << int(winCharsets[i].charsetCode) << ' ';
	for (size_t j = 0; j < key->size(); j++)
	  os() << (char)(*key)[j];
	os() << winCharsets[i].fontSuffix;
	os() << ";}\n";
      }
    }
  }
  os() << "}\n";
  os() << "{\\colortbl;";
  for (size_t i = 0; i < colorTable_.size(); i++) {
    unsigned n = colorTable_[i];
    os() << "\\red" << (n >> 16)
         << "\\green" << ((n >> 8) & 0xff)
	 << "\\blue" << (n & 0xff)
	 << ';';
  }
  os() << "}\n";
  if (maxConsecHyphens_ > 0)
    os() << "\\hyphconsec" << maxConsecHyphens_;
  os() << "\\deflang" << DEFAULT_LANG << "\\notabind\\facingp\\hyphauto1\\widowctrl\n";

  tmpstreambuf::Iter sbIter(tempbuf_);
  const char *s;
  size_t n;
  while (sbIter.next(s, n)) {
    while (n > 0) {
      const char *p = (char *)memchr(s, BOOKMARK_CHAR, n);
      if (!p) {
	os().write(s, n);
	break;
      }
      os().write(s, p - s);
      n -= (p - s);
      s = p;
      --n;
      s++;
      unsigned long ei = 0;
      for (int i = 0; i < 4; i++, n--) {
        if (n == 0) {
          bool gotSome = sbIter.next(s, n);
          ASSERT(gotSome);
	}
	ei = (ei << 8) | (unsigned char)*s++;
      }
      if (elementsRefed_.contains(ei)) {
	os() << "{\\*\\bkmkstart ";
        outputBookmarkName(ei);
        os() << '}';
        os() << "{\\*\\bkmkend ";
	outputBookmarkName(ei);
	os() << '}';
      }
    }
  }
}

void RtfFOTBuilder::syncCharFormat()
{
  bool changed = 0;
  if (outputFormat_.isBold != specFormat_.isBold) {
    os() << "\\b";
    if (!specFormat_.isBold)
      os() << '0';
    outputFormat_.isBold = specFormat_.isBold;
    changed = 1;
  }
  if (outputFormat_.isItalic != specFormat_.isItalic) {
    os() << "\\i";
    if (!specFormat_.isItalic)
      os() << '0';
    outputFormat_.isItalic = specFormat_.isItalic;
    changed = 1;
  }
  if (outputFormat_.underline != specFormat_.underline) {
    switch (specFormat_.underline) {
    case noUnderline:
      os() << "\\ul0";
      break;
    case underlineSingle:
      os() << "\\ul";
      break;
    case underlineDouble:
      os() << "\\uldb";
      break;
    case underlineWords:
      os() << "\\ulw";
      break;
    }
    outputFormat_.underline = specFormat_.underline;
    changed = 1;
  }
  if (outputFormat_.isSmallCaps != specFormat_.isSmallCaps) {
    os() << "\\scaps";
    if (!specFormat_.isSmallCaps)
      os() << '0';
    outputFormat_.isSmallCaps = specFormat_.isSmallCaps;
    changed = 1;
  }
  if (outputFormat_.isStrikethrough != specFormat_.isStrikethrough) {
    os() << "\\strike";
    if (!specFormat_.isStrikethrough)
      os() << '0';
    outputFormat_.isStrikethrough = specFormat_.isStrikethrough;
    changed = 1;
  }
  if (outputFormat_.positionPointShift != specFormat_.positionPointShift) {
    if (specFormat_.positionPointShift >= 0)
      os() << "\\up" << specFormat_.positionPointShift;
    else
      os() << "\\dn" << -specFormat_.positionPointShift;
    outputFormat_.positionPointShift = specFormat_.positionPointShift;
    changed = 1;
  }
  if (outputFormat_.fontSize != specFormat_.fontSize) {
    os() << "\\fs" << specFormat_.fontSize;
    outputFormat_.fontSize = specFormat_.fontSize;
    changed = 1;
  }
  if (outputFormat_.country != specFormat_.country
      || outputFormat_.language != specFormat_.language) {
    outputFormat_.country = specFormat_.country;
    outputFormat_.language = specFormat_.language;
    unsigned lang = convertLanguage(outputFormat_.language, outputFormat_.country,
				    outputFormat_.langCharsets);
    if (lang != outputFormat_.lang) {
      outputFormat_.lang = lang;
      os() << "\\lang" << lang;
      changed = 1;
    }
  }
  bool charsetOk = ((1 << outputFormat_.charset) & outputFormat_.langCharsets);
  if (outputFormat_.fontFamily != specFormat_.fontFamily || !charsetOk) {
    outputFormat_.fontFamily = specFormat_.fontFamily;
    // Make sure the charset is compatible with the language
    if (!charsetOk) {
      for (outputFormat_.charset = 0;
           !(outputFormat_.langCharsets & (1 << outputFormat_.charset));
	   outputFormat_.charset++)
	;
    }
    int &n = fontFamilyCharsetsTable_[outputFormat_.fontFamily].rtfFontNumber[outputFormat_.charset];
    if (n < 0)
      n = nextRtfFontNumber_++;
    os() << "\\f" << n;
    changed = 1;
  }
  if (outputFormat_.color != specFormat_.color) {
    os() << "\\cf" << specFormat_.color;
    outputFormat_.color = specFormat_.color;
    changed = 1;
  }
  if (outputFormat_.charBackgroundColor != specFormat_.charBackgroundColor) {
    os() << "\\highlight" << specFormat_.charBackgroundColor;
    outputFormat_.charBackgroundColor = specFormat_.charBackgroundColor;
    changed = 1;
  }
  if (!hyphenateSuppressed_ && !specFormat_.hyphenate) {
    hyphenateSuppressed_ = 1;
    os() << "\\hyphpar0";
    changed = 1;
  }
  if (outputFormat_.kern != specFormat_.kern) {
    os() << "\\kerning" << (specFormat_.kern ? '1' : '0');
    outputFormat_.kern = specFormat_.kern;
    changed = 1;
  }
  if (changed)
    os() << ' ';
}

void RtfFOTBuilder::setCharset(int cs)
{
  outputFormat_.charset = cs;
  int &n = fontFamilyCharsetsTable_[outputFormat_.fontFamily].rtfFontNumber[cs];
  if (n < 0)
    n = nextRtfFontNumber_++;
  os() << "\\f" << n;
}

inline
void hexChar(ostream &os, unsigned code)
{
  static const char hex[] = "0123456789abcdef";
  os << "\\'" << hex[(code >> 4) & 0xf] << hex[code & 0xf];
}

void RtfFOTBuilder::characters(const Char *s, size_t n)
{
  // Ignore record ends at the start of continuation paragraphs.
  if (inlineState_ == inlineContinue && paraFormat_.lines == symbolWrap) {
    for (; n > 0 && *s == '\r'; s++, n--)
      ;
    if (n == 0)
      return;
  }
  // This avoids clearing followWhitespaceChar_.
  if (inlineState_ != inlineMiddle)
    inlinePrepare();
  syncCharFormat();
  for (; n > 0; s++, n--) {
    bool prevWhitespaceChar = followWhitespaceChar_;
    followWhitespaceChar_ = 0;
    currentColumn_++;
    switch (*s) {
    case '\n':
      currentColumn_--;
      followWhitespaceChar_ = prevWhitespaceChar;
      break;
    case '\r':
      followWhitespaceChar_ = 1;
      switch (paraFormat_.lines) {
      case symbolNone:
	// FIXME
      case symbolWrap:
	switch (specFormat_.inputWhitespaceTreatment) {
	case symbolIgnore:
	  currentColumn_--;
	  break;
	case symbolCollapse:
	  if (prevWhitespaceChar) {
	    currentColumn_--;
	    break;
	  }
	  // fall through
	default:
	  os() << ' ';
	}
	break;
      default:
	os() << "\\sa0\\par\\fi0\\sb0\n";
	currentColumn_ = 0;
	break;
      }
      break;
    case '\t':
      if (specFormat_.expandTabs && specFormat_.inputWhitespaceTreatment == symbolPreserve) {
	unsigned col = --currentColumn_ + specFormat_.expandTabs;
	col = (col / specFormat_.expandTabs) * specFormat_.expandTabs;
	for (; currentColumn_ < col; currentColumn_++)
	  os() << ' ';
	followWhitespaceChar_ = 1;
	break;
      }
      // fall through
    case ' ':
      followWhitespaceChar_ = 1;
      switch (specFormat_.inputWhitespaceTreatment) {
      case symbolIgnore:
	currentColumn_--;
	break;
      case symbolCollapse:
	if (prevWhitespaceChar) {
	  currentColumn_--;
	  break;
	}
	// fall through
      default:
	os() << ' ';
      }
      break;
    case 0x2002:
      os() << "\\enspace ";
      break;
    case 0x2003:
      os() << "\\emspace ";
      break;
    case 0x2010:
      os() << '-';
      break;
    case 0x2011:
      os() << "\\_";
      break;
    case 0x200c:
      os() << "\\zwnj ";
      break;
    case 0x200d:
      os() << "\\zwj ";
      break;
    case 0xa0:
      os() << "\\~";
      break;
    case 0xad:
      os() << "\\-";
      break;
    case '\0':
      break;
    case '{':
    case '}':
    case '\\':
      os() << '\\' << char(*s);
      break;
      // fall through
    default:
      if (*s < 0x80)
	os() << char(*s);
      else if (*s < charTableSize) {
	unsigned short code = charTable_[*s];
	if (code & symbolFontFlag) {
	  outputFormat_.fontFamily = ((code >> 8) & 0x7f) + 1;
	  setCharset(nWinCharsets - 1);
	  hexChar(os(), code & 0xff);
	}
        else {
	  if (!(code & (1 << (outputFormat_.charset + 8)))) {
	    if (!code)
	      break;
	    // If possible, choose a charset compatible with the language
	    if (code & (outputFormat_.langCharsets << 8))
	      code &= ((outputFormat_.langCharsets << 8) | 0xff);
	    int i = 0;
	    for (unsigned mask = 0x100; !(code & mask); mask <<= 1, i++)
	      ;
	    setCharset(i);
	  }
	  hexChar(os(), code & 0xff);
	}
      }
      else if (*s >= SYMBOL_FONT_PAGE + 0x20 && *s <= SYMBOL_FONT_PAGE + 0xff) {
	setCharset(nWinCharsets - 1);
	hexChar(os(), *s & 0xff);
      }
      break;
    }
  }
}

void RtfFOTBuilder::setFontSize(long n)
{
  specFormat_.fontSize = halfPoints(n);
}

void RtfFOTBuilder::setFontFamilyName(const StringC &name)
{
  const int *p = fontFamilyNameTable_.lookup(name);
  if (!p) {
    specFormat_.fontFamily = fontFamilyCharsetsTable_.size();
    fontFamilyNameTable_.insert(name, specFormat_.fontFamily);
    fontFamilyCharsetsTable_.resize(fontFamilyCharsetsTable_.size() + 1);
  }
  else
    specFormat_.fontFamily = *p;
}

void RtfFOTBuilder::setFontWeight(Symbol weight)
{
  specFormat_.isBold = (weight > symbolMedium);
}

void RtfFOTBuilder::setFontPosture(Symbol posture)
{
  switch (posture) {
  case symbolOblique:
  case symbolItalic:
    specFormat_.isItalic = 1;
    break;
  default:
    specFormat_.isItalic = 0;
    break;
  }
}

void RtfFOTBuilder::setGlyphSubstTable(const Vector<ConstPtr<GlyphSubstTable> > &tables)
{
  static const int smallCapAfiiSignature = 0375;
  specFormat_.isSmallCaps = 0;
  for (size_t i = 0; i < tables.size(); i++) {
    const Vector<GlyphId> &pairs = tables[i]->pairs;
    for (size_t j = 0; j < pairs.size(); j += 2) {
      if (pairs[j].suffix + (smallCapAfiiSignature << 8) == pairs[j + 1].suffix
	  && pairs[j].suffix >= 'a'
	  && pairs[j].suffix <= 'z'
	  && pairs[j].publicId == pairs[j + 1].publicId
	  && strcmp(pairs[j].publicId, "ISO/IEC 10036/RA//Glyphs") == 0) {
	specFormat_.isSmallCaps = 1;
	break;
      }
    }
  }
}

void RtfFOTBuilder::setHyphenate(bool b)
{
  specFormat_.hyphenate = b;
}

void RtfFOTBuilder::setKern(bool b)
{
  specFormat_.kern = b;
}

void RtfFOTBuilder::setHyphenationLadderCount(long n)
{
  if (n > 0 && (maxConsecHyphens_ == 0 || n < maxConsecHyphens_))
    maxConsecHyphens_ = n;
}

void RtfFOTBuilder::setWidowCount(long n)
{
  if (n > 1)
    specFormat_.widowOrphanControl |= widowControl;
  else
    specFormat_.widowOrphanControl &= orphanControl;
}

void RtfFOTBuilder::setOrphanCount(long n)
{
  if (n > 1)
    specFormat_.widowOrphanControl |= orphanControl;
  else
    specFormat_.widowOrphanControl &= widowControl;
}

void RtfFOTBuilder::setColor(const DeviceRGBColor &color)
{
  specFormat_.color = makeColor(color);
}

void RtfFOTBuilder::setBackgroundColor(const DeviceRGBColor &color)
{
  specFormat_.backgroundColor = makeColor(color);
}

void RtfFOTBuilder::setBackgroundColor()
{
  specFormat_.backgroundColor = 0;
}

int RtfFOTBuilder::makeColor(const DeviceRGBColor &color)
{
  unsigned n = (color.red << 16) | (color.green << 8) | color.blue;
  for (int i = 0; i < colorTable_.size(); i++)
    if (colorTable_[i] == n)
      return i + 1;
  colorTable_.push_back(n);
  return colorTable_.size();
}

void RtfFOTBuilder::setBoxType(Symbol sym)
{
  switch (sym) {
  case symbolBorder:
    specFormat_.boxHasBorder = 1;
    specFormat_.boxHasBackground = 0;
    break;
  case symbolBackground:
    specFormat_.boxHasBorder = 0;
    specFormat_.boxHasBackground = 1;
    break;
  case symbolBoth:
    specFormat_.boxHasBorder = 1;
    specFormat_.boxHasBackground = 1;
    break;
  default:
    break;
  }
}

void RtfFOTBuilder::setLineThickness(Length len)
{
  specFormat_.lineThickness = (len > 150 ? 150 : len);
}

void RtfFOTBuilder::setLineRepeat(long n)
{
  specFormat_.lineDouble = (n > 1);
}

void RtfFOTBuilder::setScoreSpaces(bool b)
{
  specFormat_.scoreSpaces = b;
}

void RtfFOTBuilder::setBorderPresent(bool b)
{
  specFormat_.borderPresent = b;
}

void RtfFOTBuilder::setBorderPriority(long n)
{
  specFormat_.borderPriority = n;
}

void RtfFOTBuilder::setBorderOmitAtBreak(bool b)
{
  specFormat_.borderOmitAtBreak = b;
}

void RtfFOTBuilder::newPar(bool allowSpaceBefore)
{
  long boxExtraTopSep = 0;
  if (boxFirstPara_) {
    boxFirstPara_ = 0;
    long leftSep = paraFormat_.leftIndent;
    long rightSep = paraFormat_.rightIndent;
    for (size_t i = 1; i < displayBoxLevels_.size(); i++) {
      leftSep += specFormatStack_[displayBoxLevels_[i]].leftIndent;
      rightSep += specFormatStack_[displayBoxLevels_[i]].rightIndent;
    }
    boxSep_ = leftSep > rightSep ? leftSep : rightSep;
    // RTF doesn't include space before or after a paragraph inside
    // the box.
    boxExtraTopSep = accumSpace_;
    accumSpace_ = accumSpaceBox_;
    accumSpaceBox_ = 0;
    if (boxExtraTopSep > boxSep_)
      boxExtraTopSep -= boxSep_;
    else
      boxExtraTopSep = 0;
  }
  if (inlineState_ != inlineFirst) {
    if (!allowSpaceBefore) {
      os() << "\\sa" << accumSpace_;
      accumSpace_ = 0;
    }
    if (keep_) {
      if (hadParInKeep_ || inlineState_ == inlineContinue)
	keepWithNext_ = 1;
      hadParInKeep_ = 1;
    }
    if (!doBreak_ && keepWithNext_)
      os() << "\\keepn";
    keepWithNext_ = 0;
    os() << "\\par";
  }
  switch (doBreak_) {
  case breakPage:
    os() << "\\page";
    doBreak_ = breakNone;
    break;
  case breakColumn:
    os() << "\\column";
    doBreak_ = breakNone;
    break;
  default:
    break;
  }
  os() << "\\pard";
  if (inTable_)
    os() << "\\intbl";
  flushPendingElements();
  if (accumSpace_) {
    os() << "\\sb" << accumSpace_;
    accumSpace_ = 0;
  }
  if (keep_)
    os() << "\\keep";
  if (!paraFormat_.widowOrphanControl)
    os() << "\\nowidctlpar";
  if (displayBoxLevels_.size() > 0) {
    const Format &boxFormat = specFormatStack_[displayBoxLevels_[0]];
    os() << "\\box";
    if (boxFormat.lineThickness > 75)
      os() << "\\brdrth\\brdrw" << boxFormat.lineThickness/2;
    else {
      if (boxFormat.lineDouble)
	os() << "\\brdrdb";
      else
	os() << "\\brdrs";
      os() << "\\brdrw" << boxFormat.lineThickness;
    }
    os() << "\\brsp" << boxSep_;
    if (boxFormat.color)
      os() << "\\brdrcf" << boxFormat.color;
    if (boxSep_ + boxFormat.leftIndent + addLeftIndent_)
      os() << "\\li" << boxSep_ + boxFormat.leftIndent + addLeftIndent_;
    if (boxSep_ + boxFormat.rightIndent + addRightIndent_)
      os() << "\\ri" << boxSep_ + boxFormat.rightIndent + addRightIndent_;
    if (boxExtraTopSep) {
      os() << "\\sl-" << boxExtraTopSep;
      keepWithNext_ = 1;
      newPar(0);
    }
  }
  else {
    if (paraFormat_.leftIndent || addLeftIndent_)
      os() << "\\li" << paraFormat_.leftIndent + addLeftIndent_;
    if (paraFormat_.rightIndent || addRightIndent_)
      os() << "\\ri" << paraFormat_.rightIndent + addRightIndent_;
  }
  hyphenateSuppressed_ = 0;
}

// This must be called before any inline flow object.

void RtfFOTBuilder::inlinePrepare()
{
  followWhitespaceChar_ = 0;
  if (inlineState_ == inlineMiddle || inlineState_ == inlineField
      || inlineState_ == inlineFieldEnd)
    return;
  newPar();
  os() << "\\sl" << (paraFormat_.lineSpacingAtLeast ? paraFormat_.lineSpacing : - paraFormat_.lineSpacing);
  int fli = inlineState_ == inlineContinue ? 0 : paraFormat_.firstLineIndent;
  if (fli)
    os() << "\\fi" << fli;
  if (paraFormat_.quadding != 'l')
    os() << "\\q" << paraFormat_.quadding;
  inlineState_ = inlineMiddle;
  os() << ' ';
}

void RtfFOTBuilder::setPositionPointShift(const LengthSpec &val)
{
  specFormat_.positionPointShiftSpec = val;
  specFormat_.positionPointShift = halfPoints(computeLengthSpec(val));
}

void RtfFOTBuilder::setStartIndent(const LengthSpec &val)
{
  specFormat_.leftIndentSpec = val;
  specFormat_.leftIndent = computeLengthSpec(val);
}

void RtfFOTBuilder::setLanguage(Letter2 code)
{
  specFormat_.language = code;
}

void RtfFOTBuilder::setCountry(Letter2 code)
{
  specFormat_.country = code;
}
 
void RtfFOTBuilder::setEndIndent(const LengthSpec &val)
{
  specFormat_.rightIndentSpec = val;
  specFormat_.rightIndent = computeLengthSpec(val);
}

void RtfFOTBuilder::setFirstLineStartIndent(const LengthSpec &val)
{
  specFormat_.firstLineIndentSpec = val;
  specFormat_.firstLineIndent = computeLengthSpec(val);
}

void RtfFOTBuilder::setLineSpacing(const LengthSpec &val)
{
  specFormat_.lineSpacingSpec = val;
  specFormat_.lineSpacing = computeLengthSpec(val);
}

void RtfFOTBuilder::setMinLeading(const OptLengthSpec &ols)
{
  specFormat_.lineSpacingAtLeast = ols.hasLength;
}

void RtfFOTBuilder::setFieldWidth(const LengthSpec &val)
{
  specFormat_.fieldWidthSpec = val;
  specFormat_.fieldWidth = computeLengthSpec(val);
}

void RtfFOTBuilder::displaySizeChanged()
{
  specFormat_.positionPointShift = computeLengthSpec(specFormat_.positionPointShiftSpec);
  specFormat_.leftIndent = computeLengthSpec(specFormat_.leftIndentSpec);
  specFormat_.rightIndent = computeLengthSpec(specFormat_.rightIndentSpec);
  specFormat_.firstLineIndent = computeLengthSpec(specFormat_.firstLineIndentSpec);
  specFormat_.lineSpacing = computeLengthSpec(specFormat_.lineSpacingSpec);
  specFormat_.fieldWidth = computeLengthSpec(specFormat_.fieldWidthSpec);
}

void RtfFOTBuilder::setQuadding(Symbol quadding)
{
  switch (quadding) {
  case symbolStart:
  default:
    specFormat_.quadding = 'l';
    break;
  case symbolEnd:
    specFormat_.quadding = 'r';
    break;
  case symbolCenter:
    specFormat_.quadding = 'c';
    break;
  case symbolJustify:
    specFormat_.quadding = 'j';
    break;
  }
}

void RtfFOTBuilder::setDisplayAlignment(Symbol displayAlignment)
{
  switch (displayAlignment) {
  case symbolStart:
    specFormat_.displayAlignment = 'l';
    break;
  case symbolEnd:
    specFormat_.displayAlignment = 'r';
    break;
  case symbolCenter:
    specFormat_.displayAlignment = 'c';
    break;
  case symbolInside:
  case symbolOutside:
  default:
    specFormat_.displayAlignment = 'l';
    break;
  }
}

void RtfFOTBuilder::setFieldAlign(Symbol fieldAlign)
{
  specFormat_.fieldAlign = fieldAlign;
}

void RtfFOTBuilder::setLines(Symbol lines)
{
  specFormat_.lines = lines;
}

void RtfFOTBuilder::setInputWhitespaceTreatment(Symbol sym)
{
  specFormat_.inputWhitespaceTreatment = sym;
}

void RtfFOTBuilder::setExpandTabs(long n)
{
  specFormat_.expandTabs = n;
}

void RtfFOTBuilder::startParagraph(const ParagraphNIC &nic)
{
  startDisplay(nic);
  start();
  paraStack_.resize(paraStack_.size() + 1);
  paraStack_.back() = paraFormat_;
  paraFormat_ = specFormat_;
}

void RtfFOTBuilder::endParagraph()
{
  paraFormat_ = paraStack_.back();
  paraStack_.resize(paraStack_.size() - 1);
  endDisplayGroup();
}

void RtfFOTBuilder::paragraphBreak(const ParagraphNIC &nic)
{
  if (paraStack_.size() > 0) {
    endDisplay();
    startDisplay(nic);
    paraFormat_ = specFormat_;
    atomic();
  }
}

void RtfFOTBuilder::startBox(const BoxNIC &nic)
{
  if (nic.isDisplay) {
    startDisplay(nic);
    if (displayBoxLevels_.size() == 0) {
      accumSpaceBox_ = accumSpace_;
      accumSpace_ = 0;
      boxFirstPara_ = 1;
    }
    displayBoxLevels_.push_back(specFormatStack_.size());
  }
  else {
    if (specFormat_.boxHasBackground && specFormat_.backgroundColor)
      specFormat_.charBackgroundColor = specFormat_.backgroundColor;
    inlinePrepare();
  }
  start();
}

void RtfFOTBuilder::endBox()
{
  if (displayBoxLevels_.size() > 0
      && displayBoxLevels_.back() + 1 == specFormatStack_.size()) {
    // We're ending a displayed box.
    if (displayBoxLevels_.size() == 1) {
      // It was the outermost box.
      if (boxFirstPara_) {
	boxFirstPara_ = 0;
	boxSep_ = 0;
	long boxHeight = accumSpace_;
	if (boxHeight <= 0)
	  boxHeight = 1;
	accumSpace_ = accumSpaceBox_;
	newPar(1);
	os() << "\\sl-" << boxHeight;
      }
      else if (accumSpace_ > boxSep_) {
	long extra = accumSpace_ - boxSep_;
	accumSpace_ = 0;
	keepWithNext_ = 1;
	newPar(1);
	os() << "\\sl-" << extra;
      }
      else
	accumSpace_ = 0;
      // This prevents an adjacent box being merged by RTF.
      displayBoxLevels_.resize(displayBoxLevels_.size() - 1);
      os() << "\\keepn\\par\\pard\\sl-1";
    }
    end();
    endDisplay();
  }
  else
    end();
}

void RtfFOTBuilder::startDisplayGroup(const DisplayGroupNIC &nic)
{
  startDisplay(nic);
  start();
}

void RtfFOTBuilder::startDisplay(const DisplayNIC &nic)
{
  long spaceBefore = computeLengthSpec(nic.spaceBefore.nominal);
  if (spaceBefore > accumSpace_)
    accumSpace_ = spaceBefore;
  if (nic.keepWithPrevious)
    keepWithNext_ = 1;
  if (inlineState_ != inlineFirst && inlineState_ != inlineTable)
    inlineState_ = inlineStart;
  switch (nic.breakBefore) {
  case symbolPage:
  case symbolPageRegion:
  case symbolColumnSet:
    doBreak_ = breakPage;
    break;
  case symbolColumn:
    if (doBreak_ != breakPage)
      doBreak_ = breakColumn;
    break;
  default:
    break;
  }
  displayStack_.resize(displayStack_.size() + 1);
  long spaceAfter = computeLengthSpec(nic.spaceAfter.nominal);
  displayStack_.back().spaceAfter = spaceAfter;
  displayStack_.back().keepWithNext = nic.keepWithNext;
  displayStack_.back().saveKeep = keep_;
  switch (nic.keep) {
  case symbolTrue:
  case symbolPage:
  case symbolColumnSet:
  case symbolColumn:
    if (!keep_) {
      hadParInKeep_ = 0;
      keep_ = 1;
    }
    break;
  default:
    break;
  }
  switch (nic.breakAfter) {
  case symbolPage:
  case symbolPageRegion:
  case symbolColumnSet:
    displayStack_.back().breakAfter = breakPage;
    break;
  case symbolColumn:
    displayStack_.back().breakAfter = breakColumn;
    break;
  default:
    displayStack_.back().breakAfter = breakNone;
    break;
  }
}

void RtfFOTBuilder::endDisplayGroup()
{
  end();
  endDisplay();
}

void RtfFOTBuilder::endDisplay()
{
  doBreak_ = displayStack_.back().breakAfter;
  keep_ = displayStack_.back().saveKeep;
  if (inlineState_ != inlineTable)
    inlineState_ = inlineContinue;
  if (displayStack_.back().spaceAfter > accumSpace_)
    accumSpace_ = displayStack_.back().spaceAfter;
  if (displayStack_.back().keepWithNext)
    keepWithNext_ = 1;
  displayStack_.resize(displayStack_.size() - 1);
}

void RtfFOTBuilder::startLineField(const LineFieldNIC &)
{
  start();
  if (inlineState_ == inlineStart || inlineState_ == inlineContinue) {
    fieldTabPos_ = specFormat_.fieldWidth + paraFormat_.leftIndent;
    if (inlineState_ == inlineStart)
      fieldTabPos_ += paraFormat_.firstLineIndent;
    inlinePrepare();
    inlineState_ = inlineField;
    // Empirically-determined minimum possible distance between tabs.
    static const int minTabSep = 26;
    switch (specFormat_.fieldAlign) {
    case symbolStart:
    default:
      os() << "\\tx" << fieldTabPos_ <<  ' ';
      break;
    case symbolEnd:
      osp_ = &fieldos_;
      inlineState_ = inlineFieldEnd;
      break;
    case symbolCenter:
      {
	int sep = specFormat_.fieldWidth / 2;
	if (sep < minTabSep)
	  sep = minTabSep;
	os() << "\\tqc\\tx" <<  fieldTabPos_ - sep << "\\tx" << fieldTabPos_ << "\\tab ";
      }
      break;
    }
  }
}

void RtfFOTBuilder::endLineField()
{
  if (inlineState_ == inlineFieldEnd) {
    osp_ = &tempos_;
    // Strip trailing spaces and adjust position of
    // tab stop accordingly.
    char *str = fieldos_.str();
    int i = fieldos_.pcount();
    for (; i > 0; i--)
      if (str[i - 1] != ' ')
        break;
    // Assume each space is .25 em.
    int sep = (fieldos_.pcount() - i)*outputFormat_.fontSize*5/2;
    static const int minTabSep = 26;
    if (sep < minTabSep)
      sep = minTabSep;
    os() << "\\tqr\\tx" <<  fieldTabPos_ - sep << "\\tx" << fieldTabPos_ << "\\tab ";
    os().write(str, i);
    fieldos_.rdbuf()->freeze(0);
    fieldos_.seekp(0, ios::beg);
    os() << "\\tab ";
  }
  else if (inlineState_ == inlineField) {
    os() << "\\tab ";
    inlineState_ = inlineMiddle;
  }
  end();
}

void RtfFOTBuilder::startLeader(const LeaderNIC &)
{
  start();
  inlinePrepare();
  if (leaderDepth_++ == 0) {
    preLeaderOsp_ = osp_;
    osp_ = &nullos_;
  }
}

void RtfFOTBuilder::endLeader()
{
  if (--leaderDepth_ == 0) {
    osp_ = preLeaderOsp_;
    nullos_.seekp(0, ios::beg);
    // MS Word doesn't mind if tabs aren't set at the beginning of the paragraph.
    os() << "\\tqr\\tldot\\tx" << (displaySize_ - paraFormat_.rightIndent) << "\\tab ";
  }
  end();
}

void RtfFOTBuilder::setPageWidth(long units)
{
  if (!inSimplePageSequence_)
    pageFormat_.pageWidth = units;
}

void RtfFOTBuilder::setPageHeight(long units)
{
  if (!inSimplePageSequence_)
    pageFormat_.pageHeight = units;
}

void RtfFOTBuilder::setLeftMargin(long units)
{
  if (!inSimplePageSequence_)
    pageFormat_.leftMargin = units;
}

void RtfFOTBuilder::setRightMargin(long units)
{
  if (!inSimplePageSequence_)
    pageFormat_.rightMargin = units;
}

void RtfFOTBuilder::setTopMargin(long units)
{
  if (!inSimplePageSequence_)
    pageFormat_.topMargin = units;
}

void RtfFOTBuilder::setBottomMargin(long units)
{
  if (!inSimplePageSequence_)
    pageFormat_.bottomMargin = units;
}

void RtfFOTBuilder::setHeaderMargin(long units)
{
  if (!inSimplePageSequence_)
    pageFormat_.headerMargin = units;
}

void RtfFOTBuilder::setFooterMargin(long units)
{
  if (!inSimplePageSequence_)
    pageFormat_.footerMargin = units;
}

void RtfFOTBuilder::setPageNumberRestart(bool b)
{
  if (!inSimplePageSequence_)
    pageFormat_.pageNumberRestart = b;
}

void RtfFOTBuilder::setPageNumberFormat(const StringC &str)
{
  if (inSimplePageSequence_)
    return;
  pageFormat_.pageNumberFormat = "dec";
  if (str.size() == 1) {
    switch (str[0]) {
    case 'A':
      pageFormat_.pageNumberFormat = "ucltr";
      break;
    case 'a':
      pageFormat_.pageNumberFormat = "lcltr";
      break;
    case 'I':
      pageFormat_.pageNumberFormat = "ucrm";
      break;
    case 'i':
      pageFormat_.pageNumberFormat = "lcrm";
      break;
    }
  }
}

void RtfFOTBuilder::setPageNColumns(long n)
{
  if (!inSimplePageSequence_)
    pageFormat_.nColumns = n > 0 ? n : 1;
}

void RtfFOTBuilder::setPageColumnSep(Length n)
{
  if (!inSimplePageSequence_)
    pageFormat_.columnSep = n >= 0 ? n : 0;
}

void RtfFOTBuilder::setPageBalanceColumns(bool b)
{
  if (!inSimplePageSequence_)
    pageFormat_.balance = b;
}

void RtfFOTBuilder::startSimplePageSequence()
{
  inSimplePageSequence_++;
  start();
  if (doBalance_) {
    os() << "\\sect\\sbknone";
    doBalance_ = 0;
  }
  if (hadSection_)
    os() << "\\sect";
  else
    hadSection_ = 1;
  if (pageFormat_.headerMargin < hfPreSpace)
    pageFormat_.headerMargin = hfPreSpace;
  if (pageFormat_.footerMargin < hfPostSpace)
    pageFormat_.footerMargin = hfPostSpace;
  os() << "\\sectd\\plain";
  if (pageFormat_.pageWidth > pageFormat_.pageHeight)
    os() << "\\lndscpsxn";
  os() << "\\pgwsxn" << pageFormat_.pageWidth
       << "\\pghsxn" << pageFormat_.pageHeight
       << "\\marglsxn" << pageFormat_.leftMargin
       << "\\margrsxn" << pageFormat_.rightMargin
       << "\\margtsxn" << pageFormat_.topMargin
       << "\\margbsxn" << pageFormat_.bottomMargin
       << "\\headery" << 0
       << "\\footery" << 0
       << "\\pgn" << pageFormat_.pageNumberFormat;
  if (pageFormat_.pageNumberRestart)
    os() << "\\pgnrestart";
  displaySize_ = pageFormat_.pageWidth - pageFormat_.leftMargin - pageFormat_.rightMargin;
  if (pageFormat_.nColumns > 1) {
    os() << "\\cols" << pageFormat_.nColumns << "\\colsx" << pageFormat_.columnSep;
    displaySize_ -= pageFormat_.columnSep * (pageFormat_.nColumns - 1);
    displaySize_ /= pageFormat_.nColumns;
  }
  if (pageFormat_.balance)
    doBalance_ = 1;
  displaySizeChanged();
  outputFormat_ = OutputFormat();
  doBreak_ = breakNone;
}

void RtfFOTBuilder::endSimplePageSequence()
{
  if (inlineState_ != inlineFirst)
    os() << "\\par";
  inlineState_ = inlineFirst;
  end();
  --inSimplePageSequence_;
  pageFormat_ = pageFormatStack_.back();
  doBreak_ = breakNone;
}

void RtfFOTBuilder::startSimplePageSequenceHeaderFooter(unsigned)
{
  inlineState_ = inlineMiddle;
  saveOutputFormat_ = outputFormat_;
  outputFormat_ = OutputFormat();
  osp_ = &hfos_;
}

void RtfFOTBuilder::endSimplePageSequenceHeaderFooter(unsigned i)
{
  outputFormat_ = saveOutputFormat_;
  char *s = hfos_.str();
  hfPart_[i].assign(s, hfos_.pcount());
  hfos_.rdbuf()->freeze(0);
  hfos_.seekp(0, ios::beg);
  osp_ = &tempos_;
}

void RtfFOTBuilder::endAllSimplePageSequenceHeaderFooter()
{
  bool titlePage = 0;
  for (int i = 0; i < nHF; i += nHF/6) {
    if (hfPart_[i | frontHF | firstHF] != hfPart_[i | frontHF | otherHF]
        || hfPart_[i | backHF | firstHF] != hfPart_[i | backHF | otherHF]) {
      titlePage = 1;
      break;
    }
  }
  if (titlePage) {
    os() << "\\titlepg";
    outputHeaderFooter("f", frontHF|firstHF);
  }
  outputHeaderFooter("l", backHF|otherHF);
  outputHeaderFooter("r", frontHF|otherHF);
  for (size_t i = 0; i < nHF; i++)
    hfPart_[i].resize(0);
  inlineState_ = inlineFirst;
}

void RtfFOTBuilder::outputHeaderFooter(const char *suffix, unsigned flags)
{
  os() << "{\\header" << suffix
       << "\\pard\\sl"
       << -(hfPreSpace + hfPostSpace)
       << "\\sb" << (pageFormat_.headerMargin - hfPreSpace)
       << "\\sa" << (pageFormat_.topMargin  - hfPostSpace - pageFormat_.headerMargin)
       << "\\plain\\tqc\\tx"
       << (pageFormat_.pageWidth - pageFormat_.leftMargin - pageFormat_.rightMargin)/2
       << "\\tqr\\tx"
       << (pageFormat_.pageWidth - pageFormat_.leftMargin - pageFormat_.rightMargin)
       << hfPart_[flags | headerHF | leftHF]
       << "\\tab "
       << hfPart_[flags | headerHF | centerHF]
       << "\\tab "
       << hfPart_[flags | headerHF | rightHF]
       << "\\par}"
       << "{\\footer" << suffix
       << "\\pard\\sl"
       << -(hfPreSpace + hfPostSpace)
       << "\\sb" << (pageFormat_.bottomMargin - hfPreSpace - pageFormat_.footerMargin)
       << "\\sa" << (pageFormat_.footerMargin - hfPostSpace)
       << "\\plain\\tqc\\tx"
       << (pageFormat_.pageWidth - pageFormat_.leftMargin - pageFormat_.rightMargin)/2
       << "\\tqr\\tx"
       << (pageFormat_.pageWidth - pageFormat_.leftMargin - pageFormat_.rightMargin)
       << hfPart_[flags | footerHF | leftHF]
       << "\\tab "
       << hfPart_[flags | footerHF | centerHF]
       << "\\tab "
       << hfPart_[flags | footerHF | rightHF]
       << "\\par}";
}

void RtfFOTBuilder::pageNumber()
{
  inlinePrepare();
  syncCharFormat();
  os() << "\\chpgn ";
}

void RtfFOTBuilder::startScore(Symbol type)
{
  start();
  switch (type) {
  case symbolAfter:
    if (specFormat_.lineDouble)
      specFormat_.underline = underlineDouble;
    else if (!specFormat_.scoreSpaces)
      specFormat_.underline = underlineWords;
    else
      specFormat_.underline = underlineSingle;
    break;
  case symbolThrough:
    specFormat_.isStrikethrough = 1;
    break;
  default:
    break;
  }
}

void RtfFOTBuilder::idrefButton(const Char *s, size_t n)
{
  os() << "{\\field";
  os() << "{\\*\\fldinst   ";  // doesn't work without the trailing spaces!
  os() << "GOTOBUTTON ";
  outputBookmarkName(s, n);
  os() << ' ';
}

void RtfFOTBuilder::startLink(const Address &addr)
{
  start();
  if (linkDepth_++ == 0) {
    switch (addr.type) {
    case Address::resolvedNode:
      {
	GroveString id; 
        if (addr.node->getId(id) == accessOK)
	  idrefButton(id.data(), id.size());
	else {
	  unsigned long n;
	  if (addr.node->elementIndex(n) == accessOK) {
	    os() << "{\\field";
	    os() << "{\\*\\fldinst   ";  // doesn't work without the trailing spaces!
	    os() << "GOTOBUTTON ";
	    outputBookmarkName(n);
	    os() << ' ';
	    elementsRefed_.add(n);
	  }
	  else
	    os() << "{{";
	}
	break;
      }
    case Address::idref:
      {
        const StringC &id = addr.params[0];
	size_t i = 0;
        for (; i < id.size(); i++) {
	  // If they use multiple IDREFs, they'll only get
	  // the first.
	  if (id[i] == ' ')
	    break;
	}
	idrefButton(id.data(), i);
	break;
      }
    default:
      os() << "{{";
      break;
    }
    saveOutputFormat_ = outputFormat_;
  }
}

void RtfFOTBuilder::endLink()
{
  if (--linkDepth_ == 0) {
    os() << "}{\\fldrslt }}";
    outputFormat_ = saveOutputFormat_;
  }
  end();
}

void RtfFOTBuilder::rule(const RuleNIC &nic)
{
  switch (nic.orientation) {
  case symbolHorizontal:
    {
      long lineOffset;
      long lineLength;
      if (nic.hasLength && (lineLength = computeLengthSpec(nic.length)) >= 0) {
	switch (specFormat_.displayAlignment) {
	default:
	case 'l':
	  lineOffset = specFormat_.leftIndent;
	  break;
	case 'c':
	  lineOffset = (displaySize_ + specFormat_.leftIndent - specFormat_.rightIndent
	                - lineLength)/2;
	  break;
	case 'r':
	  lineOffset = displaySize_ - specFormat_.rightIndent - lineLength;
	  break;
	}
      }
      else {
	lineOffset = specFormat_.leftIndent;
	lineLength = displaySize_ - specFormat_.leftIndent - specFormat_.rightIndent;
      }
      startDisplay(nic);
      // We can't allow spaceBefore on this paragraph, because the y position
      // is measured from the start of the space before, but this space will
      // disappear on a page break.
      newPar(0);
      os() << "\\sl-1{\\*\\do\\dobxcolumn\\dobypara\\dodhgt0\\dpline\\dpptx0\\dppty0"
           << "\\dpptx" << lineLength
	   << "\\dppty0"
	   << "\\dpx" << lineOffset
	   << "\\dpy0"
	   << "\\dpxsize" << lineLength
	   << "\\dpysize0";
      if (specFormat_.color) {
	unsigned n = colorTable_[specFormat_.color - 1];
	os() << "\\dplinecor" << (n >> 16)
	     << "\\dplinecog" << ((n >> 8) & 0xff)
	     << "\\dplinecob" << (n & 0xff);
      }
      os() << "\\dplinew" << specFormat_.lineThickness;
      os() << "}";
      endDisplay();
    }
    break;
  case symbolVertical:
    startDisplay(nic);
    newPar();
    if (nic.hasLength) {
      long lineLength = computeLengthSpec(nic.length);
      if (lineLength > 0)
	os() << "\\sl-" << lineLength;
      // FIXME handle other display alignments
    }
    endDisplay();
    break;
  case symbolEscapement:
    // if we don't have a length, can handle this with a leader tab.
    // If we do have a length, perhaps underline a blank space.
    inlinePrepare();
    break;
  case symbolLineProgression:
    inlinePrepare();
    break;
  default:
    break;
  }
  atomic();
}

void RtfFOTBuilder::externalGraphic(const ExternalGraphicNIC &nic)
{
  if (nic.isDisplay) {
    startDisplay(nic);
    newPar();
    if (specFormat_.displayAlignment != 'l')
      os() << "\\q" << specFormat_.displayAlignment;
    os() << "\\sl1000";
  }
  else
    inlinePrepare();
  String<char> filename;
  if (systemIdFilename(nic.entitySystemId, filename)) {
    os() << "{\\field\\flddirty{\\*\\fldinst INCLUDEPICTURE \"";
    for (size_t i = 0; i < filename.size(); i++) {
      if (filename[i] == '\\')
	os() << "\\\\\\\\";
      else
	os() << filename[i];
    }
    os() << "\" }{\\fldrslt }}";
  }
  if (nic.isDisplay)
    endDisplay();
  atomic();
}

bool RtfFOTBuilder::systemIdFilename(const StringC &systemId, String<char> &filename)
{
  if (systemId.size() == 0)
    return 0;
  Owner<InputSource> in
    = entityManager_->open(systemId, *systemCharset_, new InputSourceOrigin, 0, *mgr_);
  if (!in)
    return 0;
  Xchar c = in->get(*mgr_);
  if (c == InputSource::eE)
    return 0;
  const Location &loc = in->currentLocation();
  if (loc.origin().isNull())
    return 0;
  const InputSourceOrigin *tem = loc.origin()->asInputSourceOrigin();
  if (!tem)
    return 0;
  const ParsedSystemId *parsed
    = ExtendEntityManager::externalInfoParsedSystemId(tem->externalInfo());
  if (!parsed)
    return 0;
  if (parsed->size() != 1)
    return 0;
  if (strcmp((*parsed)[0].storageManager->type(), "OSFILE") != 0)
    return 0;
  const StringC &id = (*parsed)[0].id;
  String<char> buf;
  // FIXME use WideCharToMultibyte
  for (size_t i = 0; i < id.size(); i++)
    buf += char(id[i]);
#if 0
  buf += '\0';
  // This is bad because it means you can't move the generated RTF file around.
  char *ptr;
  filename.resize(2048);
  unsigned len = GetFullPathNameA(buf.data(), 2048, filename.begin(), &ptr);
  if (len == 0)
    return 0;
  filename.resize(len);
#else
  filename = buf;
#endif
  return 1;
}

void RtfFOTBuilder::startTable(const TableNIC &nic)
{
  startDisplay(nic);
  start();
  if (nic.widthType == TableNIC::widthExplicit)
    tableWidth_ = computeLengthSpec(nic.width);
  else
    tableWidth_ = (displaySize_
                   - specFormat_.leftIndent
		   - specFormat_.rightIndent);
  tableDisplaySize_ = displaySize_;
  tableLeftIndent_ = specFormat_.leftIndent;
  tableRightIndent_ = specFormat_.rightIndent;
  tableAlignment_ = specFormat_.displayAlignment;
  switch (tableAlignment_) {
  case 'r':
    if (tableRightIndent_ != 0) {
      tableAlignment_ = 'l';
      tableLeftIndent_ = tableDisplaySize_ - tableWidth_ - tableRightIndent_;
    }
    break;
  case 'c':
    if (tableLeftIndent_ != 0 || tableRightIndent_ != 0) {
      tableAlignment_ = 'l';
      tableLeftIndent_ = (tableDisplaySize_ - tableWidth_ + tableLeftIndent_ - tableRightIndent_)/2;
    }
    break;
  default:
    break;
  }
  inTable_ = 1;
}

void RtfFOTBuilder::endTable()
{
  if (cells_.size())
    outputTable();
  if (inlineState_ == inlineTable) {
    outputFormat_ = OutputFormat();
    os() << "\\pard\\plain\\sl-1";
    inlineState_ = inlineStart;
  }
  end();
  endDisplay();
  displaySize_ = tableDisplaySize_;
  inTable_ = 0;
}

void RtfFOTBuilder::startTablePartSerial(const TablePartNIC &nic)
{
  nHeaderRows_ = 0;
  startDisplay(nic);
  start();
}

void RtfFOTBuilder::endTablePartSerial()
{
  if (cells_.size())
    outputTable();
  end();
  endDisplay();
}

void RtfFOTBuilder::startTablePartHeader()
{
  inTableHeader_ = 1;
}

void RtfFOTBuilder::endTablePartHeader()
{
  inTableHeader_ = 0;
}

void RtfFOTBuilder::startTablePartFooter()
{
}

void RtfFOTBuilder::endTablePartFooter()
{
}

void RtfFOTBuilder::tableColumn(const TableColumnNIC &nic)
{
  if (nic.nColumnsSpanned == 1) {
    if (nic.columnIndex >= columns_.size())
      columns_.resize(columns_.size() + 1);
    columns_[nic.columnIndex].hasWidth = nic.hasWidth;
    if (nic.hasWidth)
      columns_[nic.columnIndex].width = nic.width;
  }
  atomic();
}

void RtfFOTBuilder::outputTable()
{
  for (size_t i = 0; i < cells_.size(); i++) {
    size_t j;
    bool lastIsDummy = 0;
    // last cell should be a dummy cell
    for (j = 0; j < cells_[i].size() - 1; j += cells_[i][j].span)
      lastIsDummy = !cells_[i][j].present;
    if (lastIsDummy)
      j--;
    if (j > columns_.size())
      columns_.resize(j);
  }
  for (size_t i = 0; i < cells_.size(); i++) {
    size_t oldSize = cells_[i].size();
    if (oldSize != columns_.size()) {
      cells_[i].resize(columns_.size());
      if (oldSize > 0
	  && oldSize < cells_[i].size()
	  && !cells_[i][oldSize - 1].present) {
	for (size_t j = oldSize; j < cells_[i].size(); j++)
	  cells_[i][j] = cells_[i][oldSize - 1];
      }
    }
  }

  // Deal the best we can with vertical spanning
  for (size_t i = 0; i < cells_.size(); i++) {
    for (size_t j = 0; j < cells_[i].size(); j += cells_[i][j].span) {
      if (cells_[i][j].vspan > 1) {
	if (i + cells_[i][j].vspan > cells_.size())
	  cells_[i][j].vspan = cells_.size() - i;
	for (size_t k = 1; k < cells_[i][j].vspan; k++) {
	  if (!cells_[i + k][j].present) {
	    cells_[i + k][j].present = 1;
	    cells_[i + k][j].span = cells_[i][j].span;
	    cells_[i + k][j].border[leftBorder] = cells_[i][j].border[leftBorder];
	    cells_[i + k][j].border[rightBorder] = cells_[i][j].border[rightBorder];
	  }
	}
	cells_[i + cells_[i][j].vspan - 1][j].border[bottomBorder]
	    = cells_[i][j].border[bottomBorder];
	cells_[i][j].border[bottomBorder].flags = 0;
      }
    }
  }
      
  Vector<long> columnWidths(columns_.size());
  int nExplicitWidth = 0;
  int totalExplicitWidth = 0;
  double totalTableUnits = 0;
  displaySize_ = tableDisplaySize_;
  for (size_t i = 0; i < columns_.size(); i++)
    if (columns_[i].hasWidth) {
      columnWidths[i] = computeLengthSpec(columns_[i].width);
      nExplicitWidth++;
      totalExplicitWidth += columnWidths[i];
      totalTableUnits += columns_[i].width.tableUnitFactor;
    }
  double tableUnitWidth = 0.0;
  totalTableUnits += columns_.size() - nExplicitWidth;
  if (totalTableUnits)
    tableUnitWidth
      = ((tableWidth_ - totalExplicitWidth)/totalTableUnits);
  for (size_t i = 0; i < columns_.size(); i++)
    if (!columns_[i].hasWidth)
      columnWidths[i] = long(tableUnitWidth);
    else if (columns_[i].width.tableUnitFactor > 0.0)
      columnWidths[i] += long(columns_[i].width.tableUnitFactor * tableUnitWidth);
  if (cells_.size() > 0) {
    for (size_t i = 0; i < cells_[0].size(); i += cells_[0][i].span)
      resolveOutsideBorder(cells_[0][i].border[topBorder], tableBorder_[topBorder]);
    for (size_t i = 0; i < cells_.back().size(); i += cells_.back()[i].span)
	resolveOutsideBorder(cells_.back()[i].border[bottomBorder], tableBorder_[bottomBorder]);
  }
  for (size_t i = 0; i < cells_.size(); i++) {
    // Resolve the column borders
    if (cells_[i].size()) {
      resolveOutsideBorder(cells_[i][0].border[leftBorder], tableBorder_[leftBorder]);
      size_t prev = 0;
      size_t j;
      for (j = cells_[i][0].span; j < cells_[i].size(); j += cells_[i][j].span) {
	resolveBorder(cells_[i][prev].border[rightBorder],
	              cells_[i][j].border[leftBorder]);
	prev = j;
      }
      if (j == columns_.size())
	resolveOutsideBorder(cells_[i][prev].border[rightBorder], tableBorder_[rightBorder]);
    }
    // Resolve the borders adjacent to the next row
    if (i + 1 < cells_.size()) {
      size_t j2 = 0;
      for (size_t j1 = 0; j1 < cells_[i].size(); j1 += cells_[i][j1].span) {
	while (j2 < j1 && j2 < cells_[i + 1].size())
	  j2 += cells_[i + 1][j2].span;
	if (j1 == j2
	    && j2 < cells_[i + 1].size()
	    && cells_[i][j1].span == cells_[i + 1][j2].span)
	  resolveBorder(cells_[i][j1].border[bottomBorder], cells_[i + 1][j2].border[topBorder]);
      }
    }
    os() << "\\trowd";
    if (tableAlignment_ == 'l' && tableLeftIndent_ != 0)
      os() << "\\trleft" << tableLeftIndent_;
    else
      os() << "\\trq" << char(tableAlignment_);
    if (keep_)
      os() << "\\trkeep";
    if (i < nHeaderRows_)
      os() << "\\trhdr";
#if 0
    // Including this causes Word to leave out rules seemingly at random
    else if (i > 0) {
      // This is necessary to get borders on table-breaks.
      // Putting it on the first row confuses Word.
      const Border &brdr = tableBorder_[bottomBorder];
      if ((brdr.flags & RtfFOTBuilder::Border::isPresent)
	&& !(brdr.flags & RtfFOTBuilder::Border::omitAtBreak)) {
	os() << "\\trbrdrb";
	outputTableBorder(brdr);
      }
    }
#endif
    os() << ' ';
    long pos = tableLeftIndent_;
    for (unsigned j = 0; j < cells_[i].size(); j += cells_[i][j].span) {
      for (int k = 0; k < 4; k++) {
	const Border &brdr = cells_[i][j].border[k];
	if (brdr.flags & RtfFOTBuilder::Border::isPresent) {
	  os() << "\\clbrdr" << "tblr"[k];
	  outputTableBorder(brdr);
	}
      }
      if (cells_[i][j].hasBackground && cells_[i][j].backgroundColor)
	os() << "\\clshdng10000\\clcfpat" << cells_[i][j].backgroundColor;
      for (int k = 0; k < cells_[i][j].span; k++)
	pos += columnWidths[j + k];
      os() << "\\cellx" << pos;
    }
    os() << ' ';
    for (unsigned j = 0; j < cells_[i].size(); j += cells_[i][j].span) {
      os() << cells_[i][j].content;
      if (keep_)
	os() << "\\keepn";
      os() << "\\cell ";
    }
    os() << "\\row ";
  }
  columns_.resize(0);
  cells_.resize(0);
}

void RtfFOTBuilder::outputTableBorder(const Border &brdr)
{
  if (brdr.thickness > 75)
    os() << "\\brdrth\\brdrw" << long(brdr.thickness)/2;
  else if (brdr.flags & Border::isDouble)
    os() << "\\brdrdb\\brdrw" << long(brdr.thickness);
  else
    os() << "\\brdrs\\brdrw" << long(brdr.thickness);
  if (brdr.color)
     os() << "\\brdrcf" << long(brdr.color);
}

void RtfFOTBuilder::startTableRow()
{
  if (inlineState_ == inlineStart) {
    if (accumSpace_)
      os() << "\\sa" << accumSpace_;
    if (keepWithNext_)
      os() << "\\keepn";
    os() << "\\par";
  }
  // FIXME if state is inlineTable and there's some accumSpace_, must add blank row
  keepWithNext_ = 0;
  accumSpace_ = 0;
  cells_.resize(cells_.size() + 1);
  start();
}

void RtfFOTBuilder::endTableRow()
{
  if (inTableHeader_) {
    Vector<Cell> headerRow;
    cells_.back().swap(headerRow);
    for (size_t i = cells_.size() - 1; i > nHeaderRows_; i--)
      cells_[i - 1].swap(cells_[i]);
    headerRow.swap(cells_[nHeaderRows_++]);
  }
  end();
  inlineState_ = inlineTable;
}

void RtfFOTBuilder::startTableCell(const TableCellNIC &nic)
{
  if (nic.columnIndex >= cells_.back().size())
    cells_.back().resize(nic.columnIndex + 1);
  cellIndex_ = nic.columnIndex;
  cells_.back()[cellIndex_].present = !nic.missing;
  cells_.back()[cellIndex_].span = nic.nColumnsSpanned;
  cells_.back()[cellIndex_].vspan = nic.nRowsSpanned;
  cells_.back()[cellIndex_].hasBackground = specFormat_.cellBackground;
  if (specFormat_.cellBackground)
    cells_.back()[cellIndex_].backgroundColor = specFormat_.backgroundColor;
  // Make a guess at the display-size.
  bool hasWidth = 1;
  long newDisplaySize = 0;
  displaySize_ = tableDisplaySize_;
  for (unsigned i = cellIndex_; i < cellIndex_ + nic.nColumnsSpanned; i++) {
    if (i >= columns_.size() || !columns_[i].hasWidth) {
      hasWidth = 0;
      break;
    }
    newDisplaySize += computeLengthSpec(columns_[i].width);
  }
  if (!hasWidth)
    newDisplaySize
      = (tableWidth_*nic.nColumnsSpanned)
        /(columns_.size() ? columns_.size() : 2);
  osp_ = &cellos_;
  inlineState_ = inlineFirst;
  addLeftIndent_  = specFormat_.cellLeftMargin;
  addRightIndent_ = specFormat_.cellRightMargin;
  accumSpace_ = specFormat_.cellTopMargin;
  displaySize_ = newDisplaySize;
  displaySizeChanged();
  start();
  os() << "\\plain ";
  outputFormat_ = OutputFormat();
}

void RtfFOTBuilder::endTableCell()
{
  accumSpace_ += specFormat_.cellBottomMargin;
  if (accumSpace_ > 0) {
    if (inlineState_ == inlineFirst)
      os() << "\\pard\\intbl\\sl-" << accumSpace_ << "\\par";
    else
      os() << "\\sa" << accumSpace_;
  }
  accumSpace_ = 0;
  char *s = cellos_.str();
  cells_.back()[cellIndex_].content.assign(s, cellos_.pcount());
  cellos_.rdbuf()->freeze(0);
  cellos_.seekp(0, ios::beg);
  end();
  osp_ = &tempos_;
  accumSpace_ = 0;
  addLeftIndent_ = 0;
  addRightIndent_ = 0;
  keepWithNext_ = 0;
}

void RtfFOTBuilder::tableBeforeRowBorder()
{
  storeBorder(tableBorder_[topBorder]);
}

void RtfFOTBuilder::tableAfterRowBorder()
{
  storeBorder(tableBorder_[bottomBorder]);
}

void RtfFOTBuilder::tableBeforeColumnBorder()
{
  storeBorder(tableBorder_[leftBorder]);
}

void RtfFOTBuilder::tableAfterColumnBorder()
{
  storeBorder(tableBorder_[rightBorder]);
}

void RtfFOTBuilder::tableCellBeforeRowBorder()
{
  storeBorder(cells_.back()[cellIndex_].border[topBorder]);
}

void RtfFOTBuilder::tableCellAfterRowBorder()
{
  storeBorder(cells_.back()[cellIndex_].border[bottomBorder]);
}

void RtfFOTBuilder::tableCellBeforeColumnBorder()
{
  storeBorder(cells_.back()[cellIndex_].border[leftBorder]);
}

void RtfFOTBuilder::tableCellAfterColumnBorder()
{
  storeBorder(cells_.back()[cellIndex_].border[rightBorder]);
}

void RtfFOTBuilder::storeBorder(Border &b)
{
  b.flags = 0;
  if (specFormat_.borderPresent) {
    b.flags |= Border::isPresent;
    b.thickness = specFormat_.lineThickness;
    b.color = specFormat_.color;
    if (specFormat_.lineDouble)
      b.flags |= Border::isDouble;
  }
  if (specFormat_.borderOmitAtBreak)
    b.flags |= Border::omitAtBreak;
  b.priority = specFormat_.borderPriority;
  atomic();
}

void RtfFOTBuilder::resolveBorder(Border &b1, Border &b2)
{
  if (b1.priority != b2.priority) {
    if (b1.priority > b2.priority)
      b2 = b1;
    else
      b1 = b2;
  }
  else if ((b1.flags ^ b2.flags) & Border::isPresent) {
    if (b1.flags & Border::isPresent)
      b2 = b1;
    else
      b1 = b2;
  }
}

void RtfFOTBuilder::resolveOutsideBorder(Border &cellBorder, const Border &tableBorder)
{
  // The standard doesn't say this, but otherwise table-border: #t wouldn't work.
  if (tableBorder.priority >= cellBorder.priority)
    cellBorder = tableBorder;
}

void RtfFOTBuilder::setCellBeforeRowMargin(Length n)
{
  specFormat_.cellTopMargin = n;
}

void RtfFOTBuilder::setCellAfterRowMargin(Length n)
{
  specFormat_.cellBottomMargin = n;
}

void RtfFOTBuilder::setCellBeforeColumnMargin(Length n)
{
  specFormat_.cellLeftMargin = n;
}

void RtfFOTBuilder::setCellAfterColumnMargin(Length n)
{
  specFormat_.cellRightMargin = n;
}

void RtfFOTBuilder::setCellBackground(bool b)
{
  specFormat_.cellBackground = b;
}

void RtfFOTBuilder::outputBookmarkName(const Char *s, size_t n)
{
  os() << "ID_";
  for (; n > 0; n--, s++) {
    Char c = *s;
    switch (c) {
    case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
    case '8': case '9':
    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h':
    case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p':
    case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x':
    case 'y': case 'z':
    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H':
    case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P':
    case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
    case 'Y': case 'Z':
      os() << char(c);
      break;
    default:
      os() << '_' << (unsigned long)c << '_';
      break;
    }
  }
}

void RtfFOTBuilder::outputBookmarkName(unsigned long n)
{
  os() << "E_" << n;
}

void RtfFOTBuilder::flushPendingElements()
{
  for (size_t i = 0; i < pendingElements_.size(); i++) {
    const NodePtr &node = pendingElements_[i];
    GroveString id;
    if (node->getId(id) == accessOK) {
      os() << "{\\*\\bkmkstart ";
      outputBookmarkName(id.data(), id.size());
      os() << '}';
      os() << "{\\*\\bkmkend ";
      outputBookmarkName(id.data(), id.size());
      os() << '}';
    }
    else {
      unsigned long n;
      if (node->elementIndex(n) == accessOK) {
	os() << BOOKMARK_CHAR;
	os() << char((n >> 24) & 0xff)
	     << char((n >> 16) & 0xff)
	     << char((n >> 8) & 0xff)
	     << char(n & 0xff);
      }
    }
  }
  nPendingElementsNonEmpty_ = 0;
  pendingElements_.resize(0);
  pendingElementLevels_.resize(0);
}

void RtfFOTBuilder::startNode(const NodePtr &node, const StringC &mode,
			      RuleType)
{
  nodeLevel_++;
  if (mode.size() == 0) {
    pendingElements_.push_back(node);
    pendingElementLevels_.push_back(nodeLevel_);
  }
}

void RtfFOTBuilder::endNode()
{
  // The idea is not to put out a bookmark if there were
  // no flow objects associated with the node.
  // The flow objects might have been labeled, in which case
  // we will get a startNode for the node later.
  if (pendingElements_.size() > 0 && pendingElementLevels_.back() == nodeLevel_
      && nPendingElementsNonEmpty_ < pendingElements_.size()) {
    pendingElementLevels_.resize(pendingElements_.size() - 1);
    pendingElements_.resize(pendingElements_.size() - 1);
  }
  nodeLevel_--;
}

void RtfFOTBuilder::currentNodePageNumber(const NodePtr &node)
{
  GroveString id;
  unsigned long n;
  if (node->getId(id) == accessOK) {
    os() << "{\\field\\flddirty{\\*\\fldinst PAGEREF ";
    outputBookmarkName(id.data(), id.size());
    os() << "}{\\fldrslt 000}}";
  }
  else if (node->elementIndex(n) == accessOK) {
    os() << "{\\field\\flddirty{\\*\\fldinst PAGEREF ";
    outputBookmarkName(n);
    elementsRefed_.add(n);
    os() << "}{\\fldrslt 000}}";
  }
}

void RtfFOTBuilder::start()
{
  nPendingElementsNonEmpty_ = pendingElements_.size();
  specFormatStack_.push_back(specFormat_);
  if (!inSimplePageSequence_)
    pageFormatStack_.push_back(pageFormat_);
}

void RtfFOTBuilder::end()
{
  specFormatStack_.resize(specFormatStack_.size() - 1);
  specFormat_ = specFormatStack_.back();
  if (!inSimplePageSequence_) {
    pageFormatStack_.resize(pageFormatStack_.size() - 1);
    pageFormat_ = pageFormatStack_.back();
  }
}

void RtfFOTBuilder::atomic()
{
  nPendingElementsNonEmpty_ = pendingElements_.size();
  specFormat_ = specFormatStack_.back();
}

RtfFOTBuilder::CommonFormat::CommonFormat()
: isBold(0), isItalic(0), fontSize(24), fontFamily(0),
  language(0), country(0),
  color(0), charBackgroundColor(0),
  underline(noUnderline), isStrikethrough(0), positionPointShift(0),
  kern(0), isSmallCaps(0)
{
}

RtfFOTBuilder::OutputFormat::OutputFormat()
: charset(0), lang(DEFAULT_LANG), langCharsets(0x1f)
{
}

RtfFOTBuilder::Format::Format()
: fieldWidth(0), fieldAlign(symbolStart), displayAlignment('l'),
  inputWhitespaceTreatment(symbolPreserve), expandTabs(8),
  lineThickness(20), lineDouble(0), backgroundColor(0),
  boxHasBorder(1), boxHasBackground(0),
  borderPresent(1), borderPriority(0), borderOmitAtBreak(0),
  cellLeftMargin(0), cellRightMargin(0), cellTopMargin(0), cellBottomMargin(0),
  lineSpacingSpec(240), cellBackground(0), scoreSpaces(1), hyphenate(0)
{
}

RtfFOTBuilder::ParaFormat::ParaFormat()
: leftIndent(0), rightIndent(0), firstLineIndent(0), quadding('l'),
  lineSpacing(240), lines(symbolWrap), lineSpacingAtLeast(0),
  widowOrphanControl(widowControl|orphanControl)
{
}

RtfFOTBuilder::PageFormat::PageFormat()
: leftMargin(0), rightMargin(0),
  topMargin(0), bottomMargin(0),
  headerMargin(0), footerMargin(0),
  pageHeight(72*20*11), pageWidth(72*10*17),
  pageNumberFormat("dec"), pageNumberRestart(0),
  nColumns(1), columnSep(72*10), balance(0)
{
}

RtfFOTBuilder::FontFamilyCharsets::FontFamilyCharsets()
{
  for (int i = 0; i < nWinCharsets; i++)
    rtfFontNumber[i] = -1;
}

BitVector::BitVector()
{
}

void BitVector::add(unsigned long n)
{
  if (n >= v_.size()) {
    for (size_t k = 1 + (n - v_.size()); k > 0; --k)
      v_.push_back(0);
  }
  v_[n] = 1;
}

bool BitVector::contains(unsigned long n) const
{
  return n < v_.size() && v_[n] != 0;
}

// This was mostly automatically generated using GetLocaleInfo().

unsigned RtfFOTBuilder::convertLanguage(unsigned language, unsigned country,
					unsigned &langCharsets)
{
  if (language == 0) {
    langCharsets = 0x1f;
    return DEFAULT_LANG;
  }
  switch (language) {
  case SP_LETTER2('B', 'G'):
    langCharsets = 0x4;
    return 0x402;
  case SP_LETTER2('C', 'A'):
    langCharsets = 0x3;
    return 0x403;
  case SP_LETTER2('C', 'S'):
    langCharsets = 0x13;
    return 0x405;
  case SP_LETTER2('D', 'A'):
    langCharsets = 0x13;
    return 0x406;
  case SP_LETTER2('D', 'E'):
    langCharsets = 0x13;
    switch (country) {
    case SP_LETTER2('D', 'E'):
      return 0x407;
    case SP_LETTER2('C', 'H'):
      return 0x807;
    case SP_LETTER2('A', 'T'):
      return 0xc07;
    case SP_LETTER2('L', 'U'):
      return 0x1007;
    case SP_LETTER2('L', 'I'):
      return 0x1407;
    }
    return 0x407;
  case SP_LETTER2('E', 'L'):
    langCharsets = 0x8;
    return 0x408;
  case SP_LETTER2('E', 'N'):
    langCharsets = 0x1f;
    switch (country) {
    case SP_LETTER2('U', 'S'):
      return 0x409;
    case SP_LETTER2('G', 'B'):
      return 0x809;
    case SP_LETTER2('A', 'U'):
      return 0xc09;
    case SP_LETTER2('C', 'A'):
      return 0x1009;
    case SP_LETTER2('N', 'Z'):
      return 0x1409;
    case SP_LETTER2('I', 'E'):
      return 0x1809;
    case SP_LETTER2('Z', 'A'):
      return 0x1c09;
    case SP_LETTER2('J', 'M'):
      return 0x2009;
    case SP_LETTER2('C', 'B'):
      return 0x2409;
    case SP_LETTER2('B', 'Z'):
      return 0x2809;
    case SP_LETTER2('T', 'T'):
      return 0x2c09;
    }
    return 0x409;
  case SP_LETTER2('E', 'S'):
    langCharsets = 0x13;
    switch (country) {
    case SP_LETTER2('E', 'S'):
      return 0x40a;
    case SP_LETTER2('M', 'X'):
      return 0x80a;
    case SP_LETTER2('G', 'T'):
      return 0x100a;
    case SP_LETTER2('C', 'R'):
      return 0x140a;
    case SP_LETTER2('P', 'A'):
      return 0x180a;
    case SP_LETTER2('D', 'O'):
      return 0x1c0a;
    case SP_LETTER2('V', 'E'):
      return 0x200a;
    case SP_LETTER2('C', 'O'):
      return 0x240a;
    case SP_LETTER2('P', 'E'):
      return 0x280a;
    case SP_LETTER2('A', 'R'):
      return 0x2c0a;
    case SP_LETTER2('E', 'C'):
      return 0x300a;
    case SP_LETTER2('C', 'L'):
      return 0x340a;
    case SP_LETTER2('U', 'Y'):
      return 0x380a;
    case SP_LETTER2('P', 'Y'):
      return 0x3c0a;
    case SP_LETTER2('B', 'O'):
      return 0x400a;
    case SP_LETTER2('S', 'V'):
      return 0x440a;
    case SP_LETTER2('H', 'N'):
      return 0x480a;
    case SP_LETTER2('N', 'I'):
      return 0x4c0a;
    case SP_LETTER2('P', 'R'):
      return 0x500a;
    }
    return 0x40a;
  case SP_LETTER2('F', 'I'):
    langCharsets = 0x13;
    return 0x40b;
  case SP_LETTER2('F', 'R'):
    langCharsets = 0x13;
    switch (country) {
    case SP_LETTER2('F', 'R'):
      return 0x40c;
    case SP_LETTER2('B', 'E'):
      return 0x80c;
    case SP_LETTER2('C', 'A'):
      return 0xc0c;
    case SP_LETTER2('C', 'H'):
      return 0x100c;
    case SP_LETTER2('L', 'U'):
      return 0x140c;
    }
    return 0x40c;
  case SP_LETTER2('H', 'U'):
    langCharsets = 0x12;
    return 0x40e;
  case SP_LETTER2('I', 'S'):
    langCharsets = 0x3;
    return 0x40f;
  case SP_LETTER2('I', 'T'):
    langCharsets = 0x13;
    switch (country) {
    case SP_LETTER2('I', 'T'):
      return 0x410;
    case SP_LETTER2('C', 'H'):
      return 0x810;
    }
    return 0x410;
  case SP_LETTER2('N', 'L'):
    langCharsets = 0x13;
    switch (country) {
    case SP_LETTER2('N', 'L'):
      return 0x413;
    case SP_LETTER2('B', 'E'):
      return 0x813;
    }
    return 0x413;
  case SP_LETTER2('N', 'O'):
    langCharsets = 0x13;
    return 0x414;
  case SP_LETTER2('P', 'L'):
    langCharsets = 0x12;
    return 0x415;
  case SP_LETTER2('P', 'T'):
    langCharsets = 0x13;
    switch (country) {
    case SP_LETTER2('B', 'R'):
      return 0x416;
    case SP_LETTER2('P', 'T'):
      return 0x816;
    }
    return 0x416;
  case SP_LETTER2('R', 'O'):
    langCharsets = 0x12;
    return 0x418;
  case SP_LETTER2('R', 'U'):
    langCharsets = 0x4;
    return 0x419;
  case SP_LETTER2('H', 'R'):
    langCharsets = 0x12;
    // FIXME are langCharsets different here?
    switch (country) {
    case SP_LETTER2('H', 'R'):
      return 0x41a;
    case SP_LETTER2('S', 'P'):
      return 0x81a;
    }
    return 0x41a;
  case SP_LETTER2('S', 'K'):
    langCharsets = 0x12;
    return 0x41b;
  case SP_LETTER2('S', 'Q'):
    langCharsets = 0x12;
    return 0x41c;
  case SP_LETTER2('S', 'V'):
    langCharsets = 0x13;
    switch (country) {
    case SP_LETTER2('S', 'E'):
      return 0x41d;
    case SP_LETTER2('F', 'I'):
      return 0x81d;
    }
    return 0x41d;
  case SP_LETTER2('T', 'R'):
    langCharsets = 0x12;
    return 0x41f;
  case SP_LETTER2('I', 'D'):
    langCharsets = 0x13;
    return 0x421;
  case SP_LETTER2('U', 'K'):
    langCharsets = 0x4;
    return 0x422;
  case SP_LETTER2('B', 'E'):
    langCharsets = 0x4;
    return 0x423;
  case SP_LETTER2('S', 'L'):
    langCharsets = 0x12;
    return 0x424;
  case SP_LETTER2('E', 'T'):
    langCharsets = 0x12;
    return 0x425;
  case SP_LETTER2('L', 'V'):
    langCharsets = 0x12;
    return 0x426;
  case SP_LETTER2('L', 'T'):
    langCharsets = 0x12;
    return 0x427;
  case SP_LETTER2('V', 'I'):
    langCharsets = 0x13;
    return 0x42a;
  case SP_LETTER2('E', 'U'):
    langCharsets = 0x13;
    return 0x42d;
  case SP_LETTER2('A', 'F'):
    langCharsets = 0x1f;
    return 0x436;
  case SP_LETTER2('F', 'O'):
    langCharsets = 0x13;
    return 0x438;
  }
  langCharsets = 0x1f;
  return DEFAULT_LANG;
}

const RtfFOTBuilder::WinCharset
RtfFOTBuilder::winCharsets[RtfFOTBuilder::nWinCharsets] = {
  // CP 1252
  { 0, "", {
    0x0000, 0x0000, 0x201a, 0x0192, 0x201e, 0x2026, 0x2020, 0x2021,
    0x02c6, 0x2030, 0x0160, 0x2039, 0x0152, 0x0000, 0x0000, 0x0000,
    0x0000, 0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014,
    0x02dc, 0x2122, 0x0161, 0x203a, 0x0153, 0x0000, 0x0000, 0x0178,
    0x00a0, 0x00a1, 0x00a2, 0x00a3, 0x00a4, 0x00a5, 0x00a6, 0x00a7,
    0x00a8, 0x00a9, 0x00aa, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00af,
    0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x00b6, 0x00b7,
    0x00b8, 0x00b9, 0x00ba, 0x00bb, 0x00bc, 0x00bd, 0x00be, 0x00bf,
    0x00c0, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x00c7,
    0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf,
    0x00d0, 0x00d1, 0x00d2, 0x00d3, 0x00d4, 0x00d5, 0x00d6, 0x00d7,
    0x00d8, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x00dd, 0x00de, 0x00df,
    0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x00e7,
    0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef,
    0x00f0, 0x00f1, 0x00f2, 0x00f3, 0x00f4, 0x00f5, 0x00f6, 0x00f7,
    0x00f8, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x00fd, 0x00fe, 0x00ff }
  },
  // CP 1250
  { 238, " CE", {
    0x0000, 0x0000, 0x201a, 0x0000, 0x201e, 0x2026, 0x2020, 0x2021,
    0x0000, 0x2030, 0x0160, 0x2039, 0x015a, 0x0164, 0x017d, 0x0179,
    0x0000, 0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014,
    0x0000, 0x2122, 0x0161, 0x203a, 0x015b, 0x0165, 0x017e, 0x017a,
    0x00a0, 0x02c7, 0x02d8, 0x0141, 0x00a4, 0x0104, 0x00a6, 0x00a7,
    0x00a8, 0x00a9, 0x015e, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x017b,
    0x00b0, 0x00b1, 0x02db, 0x0142, 0x00b4, 0x00b5, 0x00b6, 0x00b7,
    0x00b8, 0x0105, 0x015f, 0x00bb, 0x013d, 0x02dd, 0x013e, 0x017c,
    0x0154, 0x00c1, 0x00c2, 0x0102, 0x00c4, 0x0139, 0x0106, 0x00c7,
    0x010c, 0x00c9, 0x0118, 0x00cb, 0x011a, 0x00cd, 0x00ce, 0x010e,
    0x0110, 0x0143, 0x0147, 0x00d3, 0x00d4, 0x0150, 0x00d6, 0x00d7,
    0x0158, 0x016e, 0x00da, 0x0170, 0x00dc, 0x00dd, 0x0162, 0x00df,
    0x0155, 0x00e1, 0x00e2, 0x0103, 0x00e4, 0x013a, 0x0107, 0x00e7,
    0x010d, 0x00e9, 0x0119, 0x00eb, 0x011b, 0x00ed, 0x00ee, 0x010f,
    0x0111, 0x0144, 0x0148, 0x00f3, 0x00f4, 0x0151, 0x00f6, 0x00f7,
    0x0159, 0x016f, 0x00fa, 0x0171, 0x00fc, 0x00fd, 0x0163, 0x02d9 }
  },
  // CP 1251
  { 204, " CYR", {
    0x0402, 0x0403, 0x201a, 0x0453, 0x201e, 0x2026, 0x2020, 0x2021,
    0x0000, 0x2030, 0x0409, 0x2039, 0x040a, 0x040c, 0x040b, 0x040f,
    0x0452, 0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014,
    0x0000, 0x2122, 0x0459, 0x203a, 0x045a, 0x045c, 0x045b, 0x045f,
    0x00a0, 0x040e, 0x045e, 0x0408, 0x00a4, 0x0490, 0x00a6, 0x00a7,
    0x0401, 0x00a9, 0x0404, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x0407,
    0x00b0, 0x00b1, 0x0406, 0x0456, 0x0491, 0x00b5, 0x00b6, 0x00b7,
    0x0451, 0x2116, 0x0454, 0x00bb, 0x0458, 0x0405, 0x0455, 0x0457,
    0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417,
    0x0418, 0x0419, 0x041a, 0x041b, 0x041c, 0x041d, 0x041e, 0x041f,
    0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427,
    0x0428, 0x0429, 0x042a, 0x042b, 0x042c, 0x042d, 0x042e, 0x042f,
    0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437,
    0x0438, 0x0439, 0x043a, 0x043b, 0x043c, 0x043d, 0x043e, 0x043f,
    0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447,
    0x0448, 0x0449, 0x044a, 0x044b, 0x044c, 0x044d, 0x044e, 0x044f }
  },
  // CP 1253
  { 161, " Greek", {
    0x0000, 0x0000, 0x201a, 0x0192, 0x201e, 0x2026, 0x2020, 0x2021,
    0x0000, 0x2030, 0x0000, 0x2039, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014,
    0x0000, 0x2122, 0x0000, 0x203a, 0x0000, 0x0000, 0x0000, 0x0000,
    0x00a0, 0x0385, 0x0386, 0x00a3, 0x00a4, 0x00a5, 0x00a6, 0x00a7,
    0x00a8, 0x00a9, 0x0000, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x2015,
    0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x0384, 0x00b5, 0x00b6, 0x00b7,
    0x0388, 0x0389, 0x038a, 0x00bb, 0x038c, 0x00bd, 0x038e, 0x038f,
    0x0390, 0x0391, 0x0392, 0x0393, 0x0394, 0x0395, 0x0396, 0x0397,
    0x0398, 0x0399, 0x039a, 0x039b, 0x039c, 0x039d, 0x039e, 0x039f,
    0x03a0, 0x03a1, 0x0000, 0x03a3, 0x03a4, 0x03a5, 0x03a6, 0x03a7,
    0x03a8, 0x03a9, 0x03aa, 0x03ab, 0x03ac, 0x03ad, 0x03ae, 0x03af,
    0x03b0, 0x03b1, 0x03b2, 0x03b3, 0x03b4, 0x03b5, 0x03b6, 0x03b7,
    0x03b8, 0x03b9, 0x03ba, 0x03bb, 0x03bc, 0x03bd, 0x03be, 0x03bf,
    0x03c0, 0x03c1, 0x03c2, 0x03c3, 0x03c4, 0x03c5, 0x03c6, 0x03c7,
    0x03c8, 0x03c9, 0x03ca, 0x03cb, 0x03cc, 0x03cd, 0x03ce, 0x0000 }
  },
  // CP 1254
  { 162, " TUR", {
    0x0000, 0x0000, 0x201a, 0x0192, 0x201e, 0x2026, 0x2020, 0x2021,
    0x02c6, 0x2030, 0x0160, 0x2039, 0x0152, 0x0000, 0x0000, 0x0000,
    0x0000, 0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014,
    0x02dc, 0x2122, 0x0161, 0x203a, 0x0153, 0x0000, 0x0000, 0x0178,
    0x00a0, 0x00a1, 0x00a2, 0x00a3, 0x00a4, 0x00a5, 0x00a6, 0x00a7,
    0x00a8, 0x00a9, 0x00aa, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00af,
    0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x00b6, 0x00b7,
    0x00b8, 0x00b9, 0x00ba, 0x00bb, 0x00bc, 0x00bd, 0x00be, 0x00bf,
    0x00c0, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x00c7,
    0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf,
    0x011e, 0x00d1, 0x00d2, 0x00d3, 0x00d4, 0x00d5, 0x00d6, 0x00d7,
    0x00d8, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x0130, 0x015e, 0x00df,
    0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x00e7,
    0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef,
    0x011f, 0x00f1, 0x00f2, 0x00f3, 0x00f4, 0x00f5, 0x00f6, 0x00f7,
    0x00f8, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x0131, 0x015f, 0x00ff }
  },
  // This is the Symbol charset which is treated specially.
  // It must be last in this table.
  { 2, "" }
};

const RtfFOTBuilder::SymbolFont
RtfFOTBuilder::symbolFonts[RtfFOTBuilder::nSymbolFonts] = {
  { "Symbol", {
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0020, 0x0021, 0x2200, 0x0023, 0x2203, 0x0025, 0x0026, 0x220B,
    0x0028, 0x0029, 0x2217, 0x002B, 0x002C, 0x2212, 0x002E, 0x002F,
    0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
    0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
    0x2245, 0x0391, 0x0392, 0x03A7, 0x2206, 0x0395, 0x03A6, 0x0393,
    0x0397, 0x0399, 0x03D1, 0x039A, 0x039B, 0x039C, 0x039D, 0x039F,
    0x03A0, 0x0398, 0x03A1, 0x03A3, 0x03A4, 0x03A5, 0x03C2, 0x2126,
    0x039E, 0x03A8, 0x0396, 0x005B, 0x2234, 0x005D, 0x22A5, 0x005F,
    0x203E, 0x03B1, 0x03B2, 0x03C7, 0x03B4, 0x03B5, 0x03C6, 0x03B3,
    0x03B7, 0x03B9, 0x03D5, 0x03BA, 0x03BB, 0x03BC, 0x03BD, 0x03BF,
    0x03C0, 0x03B8, 0x03C1, 0x03C3, 0x03C4, 0x03C5, 0x03D6, 0x03C9,
    0x03BE, 0x03C8, 0x03B6, 0x007B, 0x007C, 0x007D, 0x223C, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x03D2, 0x2032, 0x2264, 0x2215, 0x221E, 0x0192, 0x2663,
    0x2666, 0x2665, 0x2660, 0x2194, 0x2190, 0x2191, 0x2192, 0x2193,
    0x00B0, 0x00B1, 0x2033, 0x2265, 0x00D7, 0x221D, 0x2202, 0x2022,
    0x00F7, 0x2260, 0x2261, 0x2248, 0x2026, 0x0000, 0x0000, 0x21B5,
    0x2135, 0x2111, 0x211C, 0x2118, 0x2297, 0x2295, 0x2205, 0x2229,
    0x222A, 0x2283, 0x2287, 0x2284, 0x2282, 0x2286, 0x2208, 0x2209,
    0x2220, 0x2207, 0x00AE, 0x00A9, 0x2122, 0x220F, 0x221A, 0x22C5,
    0x00AC, 0x2227, 0x2228, 0x21D4, 0x21D0, 0x21D1, 0x21D2, 0x21D3,
    0x25CA, 0x2329, 0x00AE, 0x00A9, 0x2122, 0x2211, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x232A, 0x222B, 0x2320, 0x0000, 0x2321, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 }
  },
  // I created this table by hand.  It's sometimes not obvious what
  // the appropriate mapping is.
  { "Wingdings", {
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x2702, 0x2701, 0x0000, 0x0000, 0x0000, 0x0000,
    0x260e, 0x2706, 0x2709, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2328,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2707, 0x270d,
    0x0000, 0x270c, 0x0000, 0x0000, 0x0000, 0x261c, 0x261e, 0x261d,
    0x261f, 0x0000, 0x263a, 0x0000, 0x2639, 0x0000, 0x2620, 0x0000,
    0x0000, 0x2708, 0x263c, 0x0000, 0x2744, 0x0000, 0x271e, 0x0000,
    0x2720, 0x2721, 0x262a, 0x262f, 0x0950, 0x2638, 0x2648, 0x2649,
    0x264a, 0x264b, 0x264c, 0x264d, 0x264e, 0x264f, 0x2650, 0x2651,
    0x2652, 0x2653, 0x0000, 0x0000, 0x25cf, 0x274d, 0x25a0, 0x25a1,
    0x0000, 0x2751, 0x2752, 0x0000, 0x0000, 0x25c6, 0x2756, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x2780, 0x2781, 0x2782, 0x2783, 0x2784, 0x2785, 0x2786,
    0x2787, 0x2788, 0x2789, 0x0000, 0x278a, 0x278b, 0x278c, 0x278d,
    0x278e, 0x278f, 0x2790, 0x2791, 0x2792, 0x2793, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x25cb, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x25aa,
    0x0000, 0x0000, 0x2726, 0x2605, 0x2736, 0x0000, 0x2739, 0x0000,
    0x0000, 0x0000, 0x2727, 0x0000, 0x0000, 0x272a, 0x2730, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x232b, 0x2326, 0x0000,
    0x27a2, 0x0000, 0x0000, 0x0000, 0x27b2, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x25ab, 0x2718, 0x2714, 0x2612, 0x2611, 0x0000 }
  },
  { "ZapfDingbats", {
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x2701, 0x2702, 0x2703, 0x2704, 0x260E, 0x2706, 0x2707,
    0x2708, 0x2709, 0x261B, 0x261E, 0x270C, 0x270D, 0x270E, 0x270F,
    0x2710, 0x2711, 0x2712, 0x2713, 0x2714, 0x2715, 0x2716, 0x2717,
    0x2718, 0x2719, 0x271A, 0x271B, 0x271C, 0x271D, 0x271E, 0x271F,
    0x2720, 0x2721, 0x2722, 0x2723, 0x2724, 0x2725, 0x2726, 0x2727,
    0x2605, 0x2729, 0x272A, 0x272B, 0x272C, 0x272D, 0x272E, 0x272F,
    0x2730, 0x2731, 0x2732, 0x2733, 0x2734, 0x2735, 0x2736, 0x2737,
    0x2738, 0x2739, 0x273A, 0x273B, 0x273C, 0x273D, 0x273E, 0x273F,
    0x2740, 0x2741, 0x2742, 0x2743, 0x2744, 0x2745, 0x2746, 0x2747,
    0x2748, 0x2749, 0x274A, 0x274B, 0x0000, 0x274D, 0x25A0, 0x274F,
    0x2750, 0x2751, 0x2752, 0x25B2, 0x25BC, 0x25C6, 0x2756, 0x0000,
    0x2758, 0x2759, 0x275A, 0x275B, 0x275C, 0x275D, 0x275E, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x2761, 0x2762, 0x2763, 0x2764, 0x2765, 0x2766, 0x2767,
    0x2663, 0x2666, 0x2665, 0x2660, 0x2460, 0x2461, 0x2462, 0x2463,
    0x2464, 0x2465, 0x2466, 0x2467, 0x2468, 0x2469, 0x2776, 0x2777,
    0x2778, 0x2779, 0x277A, 0x277B, 0x277C, 0x277D, 0x277E, 0x277F,
    0x2780, 0x2781, 0x2782, 0x2783, 0x2784, 0x2785, 0x2786, 0x2787,
    0x2788, 0x2789, 0x278A, 0x278B, 0x278C, 0x278D, 0x278E, 0x278F,
    0x2790, 0x2791, 0x2792, 0x2793, 0x2794, 0x2192, 0x2194, 0x2195,
    0x2798, 0x2799, 0x279A, 0x279B, 0x279C, 0x279D, 0x279E, 0x279F,
    0x27A0, 0x27A1, 0x27A2, 0x27A3, 0x27A4, 0x27A5, 0x27A6, 0x27A7,
    0x27A8, 0x27A9, 0x27AA, 0x27AB, 0x27AC, 0x27AD, 0x27AE, 0x27AF,
    0x0000, 0x27B1, 0x27B2, 0x27B3, 0x27B4, 0x27B5, 0x27B6, 0x27B7,
    0x27B8, 0x27B9, 0x27BA, 0x27BB, 0x27BC, 0x27BD, 0x27BE, 0x0000 }
    },
};
  
#ifdef DSSSL_NAMESPACE
}
#endif

#include "RtfFOTBuilder_inst.cxx"
