// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#include "config.h"
#include "HtmlFOTBuilder.h"
#include "FOTBuilder.h"
#include "Link.h"
#include "IList.h"
#include "IListIter.h"
#include "OutputCharStream.h"
#include "Ptr.h"
#include "Resource.h"
#include "NCVector.h"
#include "macros.h"
#include "HtmlMessages.h"
#include "MessageArg.h"
#include "ErrnoMessageArg.h"
#include "StringResource.h"
#include <fstream.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

class DiscardOutputCharStream : public OutputCharStream {
public:
  DiscardOutputCharStream() { }
  void flush() { }
  void flushBuf(Char) { }
};

const char RE = '\r';

class HtmlFOTBuilder : public SerialFOTBuilder {
public:
  class OutputState;
  // An address that has been referenced.
  class AddressRef {
  public:
    virtual void outputRef(bool end, OutputCharStream &, OutputState &) = 0;
    virtual ~AddressRef();
  };
  struct CharStyle : public Resource {
    CharStyle() : aref(0), isBold(0), isItalic(0), color(0)
      { }
    unsigned color;
    bool isBold;
    bool isItalic;
    AddressRef *aref;
  };
  struct ParaStyle : public Resource {
    ParaStyle() : quadding(symbolStart) { }
    Symbol quadding;
  };
  class OutputState {
  public:
    OutputState(const String<CmdLineApp::AppChar> *outputFilename,
                CmdLineApp *app)
     : outputFilename_(outputFilename),
       app_(app),
       outputDocIndex_(unsigned(-1)) { }
    void setCharStyle(const ConstPtr<CharStyle> &, OutputCharStream &);
    unsigned setOutputDocIndex(unsigned i) {
      unsigned tem = outputDocIndex_;
      outputDocIndex_ = i;
      return tem;
    }
    unsigned outputDocIndex() const { return outputDocIndex_; }
    String<CmdLineApp::AppChar> outputFilename(unsigned i) const;
    CmdLineApp &app() { return *app_; }
    const OutputCodingSystem *codingSystem() const { return app_->outputCodingSystem(); }
    Messenger &messenger() { return *app_; }
  private:
    const String<CmdLineApp::AppChar> *outputFilename_;

    CmdLineApp *app_;
    ConstPtr<CharStyle> style_;
    unsigned outputDocIndex_;
  };
  class Item : public Link {
  public:
    Item() { }
    virtual ~Item();
    virtual void output(OutputCharStream &, OutputState &) { }
  private:
    void operator=(const Item &);
    Item(const Item &);
  };
  // Corresponds to <A NAME="..."></A>
  class Addressable : public Item, public AddressRef {
  public:
    Addressable(size_t i)
      : elementIndex_(i), referenced_(0), docIndex_(unsigned(-1)) { }
    void outputRef(bool end, OutputCharStream &, OutputState &);
    void output(OutputCharStream &, OutputState &);
    bool defined() const { return docIndex_ != unsigned(-1); }
    bool referenced() const { return referenced_; }
    void setDefined(unsigned docIndex, bool wholeDocument = 0) {
      docIndex_ = docIndex;
      if (wholeDocument)
	elementIndex_ = (unsigned)-1;
    }
    void setReferenced() { referenced_ = 1; }
  private:
    // -1 if the whole document
    size_t elementIndex_;
    // unsigned(-1) if not defined
    unsigned docIndex_;
    bool referenced_;
  };
  class Markup : public Item {
  public:
    Markup(const StringC &str, AddressRef *aref) : str_(str), aref_(aref) { }
    void output(OutputCharStream &, OutputState &);
  private:
    StringC str_;
    AddressRef *aref_;
  };
  class Pcdata : public Item {
  public:
    Pcdata(const Ptr<CharStyle> &style) : style_(style) { }
    void output(OutputCharStream &, OutputState &);
  private:
    virtual const Char *data(size_t &) const = 0;
    ConstPtr<CharStyle> style_;
  };
  class ImmediatePcdata : public Pcdata {
  public:
    ImmediatePcdata(const Char *s, size_t n, const Ptr<CharStyle> &style)
      : Pcdata(style), str_(s, n) { }
  private:
    const Char *data(size_t &n) const { n = str_.size(); return str_.data(); }
    StringC str_;
  };
  class NodePcdata : public Pcdata {
  public:
    NodePcdata(const NodePtr &node, const Char *s, size_t n, const Ptr<CharStyle> &style)
      : Pcdata(style), node_(node), s_(s), n_(n) { }
  private:
    const Char *data(size_t &n) const { n = n_; return s_; }
    NodePtr node_;
    const Char *s_;
    size_t n_;
  };
  class Container : public Item {
  public:
    Container() { }
    IList<Item> *contentPtr() { return &content_; }
    void output(OutputCharStream &, OutputState &);
  protected:
    void reverse();
    // In reverse order while being built.
    IList<Item> content_;
  private:
    Container(const Container &);
    void operator=(const Container &);
  };
  class Block : public Container {
  public:
    Block(const Ptr<ParaStyle> &style) : style_(style) { }
    void output(OutputCharStream &, OutputState &);
  private:
    ConstPtr<ParaStyle> style_;
  };
  class Document : public Container {
  public:
    Document(unsigned index, const ConstPtr<StringResource<Char> > &title)
      : index_(index), title_(title) { }
    ~Document();
    void output(OutputCharStream &, OutputState &);
  private:
    ConstPtr<StringResource<Char> > title_;
    unsigned index_;
  };
  HtmlFOTBuilder(const String<CmdLineApp::AppChar> &, CmdLineApp *);
  ~HtmlFOTBuilder();
  void setFontWeight(Symbol);
  void setFontPosture(Symbol);
  void setColor(const DeviceRGBColor &);
  void setQuadding(Symbol);
  void setScrollTitle(const StringC &);
  void formattingInstruction(const StringC &);
  void start();
  void end();
  void atomic();
  void characters(const Char *s, size_t n);
  void charactersFromNode(const NodePtr &, const Char *s, size_t n);
  void startParagraph(const ParagraphNIC &nic);
  void endParagraph();
  void startLink(const Address &);
  void startScroll();
  void endScroll();
  void startNode(const NodePtr &, const StringC &, RuleType);
  void endNode();
  void flushPendingAddresses();
  static void outputCdata(const Char *, size_t, OutputCharStream &);
  static void outputCdata(const StringC &s, OutputCharStream &os) {
    outputCdata(s.data(), s.size(), os);
  }
private:
  HtmlFOTBuilder(const HtmlFOTBuilder &);
  void operator=(const HtmlFOTBuilder &);
  struct FlowObjectInfo : public Link {
    FlowObjectInfo(const Ptr<CharStyle> &cs,
                   const Ptr<ParaStyle> &ps,
		   unsigned i,
		   const ConstPtr<StringResource<Char> > &st)
     : charStyle(cs), paraStyle(ps), docIndex(i), scrollTitle(st) { }
    Ptr<CharStyle> charStyle;
    Ptr<ParaStyle> paraStyle;
    unsigned docIndex;
    ConstPtr<StringResource<Char> > scrollTitle;
  };
  struct DestInfo : public Link {
    DestInfo(IList<Item> *p) : list(p) { }
    IList<Item> *list;
  };

  // These implement copy-on-write
  CharStyle &charStyle() {
    if (charStyle_->count() > 1)
      charStyle_ = new CharStyle(*charStyle_);
    return *charStyle_;
  }
  ParaStyle &paraStyle() {
    if (paraStyle_->count() > 1)
      paraStyle_ = new ParaStyle(*paraStyle_);
    return *paraStyle_;
  }
  void insertAddr(size_t);
  Addressable *elementAddress(size_t n);

  Ptr<CharStyle> charStyle_;
  Ptr<ParaStyle> paraStyle_;
  Ptr<CharStyle> parentCharStyle_;
  Ptr<ParaStyle> parentParaStyle_;
  IList<DestInfo> destStack_;
  IList<Item> *dest_;
  Container root_;
  // Index of the current HTML document
  unsigned docIndex_;
  // Number of HTML documents
  unsigned nDocuments_;
  ConstPtr<StringResource<Char> > scrollTitle_;
  ConstPtr<StringResource<Char> > parentScrollTitle_;
  IList<FlowObjectInfo> flowObjectStack_;
  Vector<Addressable *> elements_;
  Vector<Vector<size_t> > pendingAddr_;
  int nOpenElements_;
  String<CmdLineApp::AppChar> outputFilename_;
  CmdLineApp *app_;
  enum { basePointSize = 12, sizeIncPercent = 20 };
};

FOTBuilder *makeHtmlFOTBuilder(const String<CmdLineApp::AppChar> &outputFilename,
			       CmdLineApp *app,
			       const FOTBuilder::Extension *&ext)
{
  static const FOTBuilder::Extension extensions[] = {
    { "UNREGISTERED::James Clark//Characteristic::scroll-title", 0,
      (void (FOTBuilder::*)(const StringC &))&HtmlFOTBuilder::setScrollTitle },
    { 0, 0, 0}
  };
  ext = extensions;
  return new HtmlFOTBuilder(outputFilename, app);
}

static
void reverse(IList<HtmlFOTBuilder::Item> &list)
{
  IList<HtmlFOTBuilder::Item> tem;
  while (!list.empty())
    tem.insert(list.get());
  tem.swap(list);
}

HtmlFOTBuilder::HtmlFOTBuilder(const String<CmdLineApp::AppChar> &outputFilename,
			       CmdLineApp *app)
: outputFilename_(outputFilename),
  app_(app),
  nDocuments_(0),
  docIndex_(unsigned(-1)),
  charStyle_(new CharStyle),
  paraStyle_(new ParaStyle),
  nOpenElements_(0)
{
  parentCharStyle_ = charStyle_;
  parentParaStyle_ = paraStyle_;
  dest_ = root_.contentPtr();
}

HtmlFOTBuilder::~HtmlFOTBuilder()
{
  reverse(*dest_);
  while (!destStack_.empty()) {
    reverse(*destStack_.head()->list);
    delete destStack_.get();
  }
  OutputState state(&outputFilename_, app_);
  DiscardOutputCharStream os;
  root_.output(os, state);
  for (size_t i = 0; i < elements_.size(); i++)
    if (elements_[i] && !elements_[i]->defined())
     delete elements_[i];
}

void HtmlFOTBuilder::start()
{
  flowObjectStack_.insert(new FlowObjectInfo(parentCharStyle_, parentParaStyle_,
                                             docIndex_, parentScrollTitle_));
  parentCharStyle_ = charStyle_;
  parentParaStyle_ = paraStyle_;
  parentScrollTitle_ = scrollTitle_;
}


void HtmlFOTBuilder::end()
{
  parentCharStyle_ = charStyle_ = flowObjectStack_.head()->charStyle;
  parentParaStyle_ = paraStyle_ = flowObjectStack_.head()->paraStyle;
  parentScrollTitle_ = scrollTitle_ = flowObjectStack_.head()->scrollTitle;
  docIndex_ = flowObjectStack_.head()->docIndex;
  delete flowObjectStack_.get();
}

void HtmlFOTBuilder::atomic()
{
  charStyle_ = parentCharStyle_;
  paraStyle_ = parentParaStyle_;
  scrollTitle_ = parentScrollTitle_;
}

void HtmlFOTBuilder::formattingInstruction(const StringC &s)
{
  dest_->insert(new Markup(s, charStyle_->aref));
  atomic();
}

void HtmlFOTBuilder::charactersFromNode(const NodePtr &node, const Char *s, size_t n)
{
  flushPendingAddresses();
  dest_->insert(new NodePcdata(node, s, n, charStyle_));
}

void HtmlFOTBuilder::characters(const Char *s, size_t n)
{
  flushPendingAddresses();
  dest_->insert(new ImmediatePcdata(s, n, charStyle_));
}

void HtmlFOTBuilder::startParagraph(const ParagraphNIC &)
{
  // handle space-before and after
  start();
  Block *block = new Block(paraStyle_);
  dest_->insert(block);
  destStack_.insert(new DestInfo(dest_));
  dest_ = block->contentPtr();
}

void HtmlFOTBuilder::endParagraph()
{
  reverse(*dest_);
  dest_ = destStack_.head()->list;
  delete destStack_.get();
  end();
}

void HtmlFOTBuilder::startScroll()
{
  start();
  docIndex_ = nDocuments_++;
  Document *doc = new Document(docIndex_, scrollTitle_);
  dest_->insert(doc);
  destStack_.insert(new DestInfo(dest_));
  dest_ = doc->contentPtr();
  if (pendingAddr_.size()) {
    for (size_t i = 0; i < pendingAddr_.back().size(); i++) {
      Addressable *tem = elementAddress(pendingAddr_.back()[i]);
      if (!tem->defined() && docIndex_ != (unsigned)-1) {
        dest_->insert(tem);
	tem->setDefined(docIndex_, 1);
      }
    }
    pendingAddr_.back().resize(0);
  }
}

void HtmlFOTBuilder::endScroll()
{
  reverse(*dest_);
  dest_ = destStack_.head()->list;
  delete destStack_.get();
  end();
}

void HtmlFOTBuilder::startLink(const Address &addr)
{
  charStyle().aref = 0;
  switch (addr.type) {
  case Address::resolvedNode:
    {
      unsigned long n;
      if (addr.node->elementIndex(n) == accessOK) {
	Addressable *tem = elementAddress(n);
	tem->setReferenced();
	charStyle().aref = tem;
      }
      break;
    }
  case Address::idref:
    {
      const StringC &id = addr.params[0];
      size_t i;
      for (i = 0; i < id.size(); i++)
	if (id[i] == ' ')
	  break;
      NodePtr node;
      NamedNodeListPtr elements;
      unsigned long n;
      if (addr.node->getGroveRoot(node) == accessOK
	  && node->getElements(elements) == accessOK
	  && elements->namedNode(GroveString(id.data(), i), node) == accessOK
	  && node->elementIndex(n) == accessOK) {
	Addressable *tem = elementAddress(n);
	tem->setReferenced();
	charStyle().aref = tem;
      }
      break;
    }
  default:
    break;
  }
  start();
}

HtmlFOTBuilder::Addressable *HtmlFOTBuilder::elementAddress(size_t n)
{
  for (size_t i = elements_.size(); i <= n; i++)
    elements_.push_back((Addressable *)0);
  if (!elements_[n])
    elements_[n] = new Addressable(n);
  return elements_[n];
}

void HtmlFOTBuilder::startNode(const NodePtr &node, const StringC &mode,
			       RuleType)
{
  pendingAddr_.resize(pendingAddr_.size() + 1);
  if (mode.size() == 0) {
    if (pendingAddr_.size() > 1)
      pendingAddr_.back() = pendingAddr_[pendingAddr_.size() - 2];
    unsigned long n;
    if (node->elementIndex(n) == accessOK && mode.size() == 0)
      pendingAddr_.back().push_back(size_t(n));
  }
}

void HtmlFOTBuilder::endNode()
{
  pendingAddr_.resize(pendingAddr_.size() - 1);
}

void HtmlFOTBuilder::insertAddr(size_t n)
{
  Addressable *tem = elementAddress(n);
  if (!tem->defined() && docIndex_ != (unsigned)-1) {
    dest_->insert(tem);
    tem->setDefined(docIndex_);
  }
}

void HtmlFOTBuilder::setQuadding(Symbol sym)
{
  if (sym == symbolJustify)
    sym = symbolStart;
  paraStyle().quadding = sym;
}

void HtmlFOTBuilder::setFontWeight(Symbol weight)
{
  charStyle().isBold = (weight > symbolMedium);
}

void HtmlFOTBuilder::setFontPosture(Symbol posture)
{
  switch (posture) {
  case symbolOblique:
  case symbolItalic:
    charStyle().isItalic = 1;
    break;
  default:
    charStyle().isItalic = 0;
    break;
  }
}

void HtmlFOTBuilder::setColor(const DeviceRGBColor &color)
{
  charStyle().color = (color.red << 16) | (color.green << 8) | color.blue;
}

void HtmlFOTBuilder::setScrollTitle(const StringC &s)
{
  scrollTitle_ = new StringResource<Char>(s);
}

void HtmlFOTBuilder::flushPendingAddresses()
{
  if (pendingAddr_.size()) {
    for (size_t i = 0; i < pendingAddr_.back().size(); i++)
      insertAddr(pendingAddr_.back()[i]);
    pendingAddr_.back().resize(0);
  }
}

void HtmlFOTBuilder::outputCdata(const Char *s, size_t n, OutputCharStream &os)
{
  for (; n > 0; n--, s++) {
    switch (*s) {
    case '\n':
      break;
    case '&':
      os << "&amp;";
      break;
    case '<':
      os << "&lt;";
      break;
    case '>':
      os << "&gt;";
      break;
    case '\r':
      os << RE;
      break;
    default:
      if (*s >= 255)
	os << "&#" << (unsigned long)*s << ';';
      else
	os.put(*s);
      break;
    }
  }
}

void HtmlFOTBuilder::Document::output(OutputCharStream &, OutputState &state)
{
  filebuf file;
  String<CmdLineApp::AppChar> filename(state.outputFilename(index_));
  filename += 0;
  if (!state.app().openFilebufWrite(file, filename.data())) {
    state.messenger()
      .message(HtmlMessages::cannotOpenOutputError,
               StringMessageArg(state.app().convertInput(filename.data())),
	       ErrnoMessageArg(errno));
    return;
  }
  RecordOutputCharStream os(new IosOutputCharStream(&file,
						    state.app().outputCodingSystem()));
  unsigned oldDocIndex = state.setOutputDocIndex(index_);
  os << "<HTML>" << RE;
  if (!title_.isNull()) {
    os << "<TITLE>";
    HtmlFOTBuilder::outputCdata(*title_, os);
    os << "</TITLE>";
  }
  os << "<BODY>" << RE;
  Container::output(os, state);
  os << "</BODY>" << RE;
  os << "</HTML>" << RE;
  state.setOutputDocIndex(oldDocIndex);
}

void HtmlFOTBuilder::Container::output(OutputCharStream &os, OutputState &state)
{
  for (IListIter<Item> iter(content_); !iter.done(); iter.next())
    iter.cur()->output(os, state);
}

void HtmlFOTBuilder::Block::output(OutputCharStream &os, OutputState &state)
{
  switch (style_->quadding) {
  case symbolCenter:
    os << "<center>";
    break;
  case symbolEnd:
    os << "<div align=right>";
    break;
  default:
    os << "<div>";
    break;
  }
  Container::output(os, state);
  state.setCharStyle(ConstPtr<CharStyle>(0), os);
  switch (style_->quadding) {
  case symbolCenter:
    os << "</center>";
    break;
  default:
    os << "</div>";
    break;
  }
}

void HtmlFOTBuilder::Pcdata::output(OutputCharStream &os, OutputState &state)
{
  state.setCharStyle(style_, os);
  size_t n;
  const Char *s = data(n);
  HtmlFOTBuilder::outputCdata(s, n, os);
}

void HtmlFOTBuilder::Markup::output(OutputCharStream &os, OutputState &state)
{
  if (!aref_)
    state.setCharStyle(ConstPtr<CharStyle>(0), os);
  else {
    Ptr<CharStyle> style(new CharStyle);
    style->aref = aref_;
    state.setCharStyle(style, os);
  }
  os << str_;
}

void HtmlFOTBuilder::Addressable::output(OutputCharStream &os, OutputState &)
{
  ASSERT(defined());
  if (referenced_ && elementIndex_ != (unsigned)-1)
    os << "<A NAME=" << (unsigned long)elementIndex_ << "></A>";
}

void HtmlFOTBuilder::Addressable::outputRef(bool end, OutputCharStream &os,
					    OutputState &state)
{
  ASSERT(referenced_);
  if (defined()) {
    if (end)
      os << "</A>";
    else {
      os << "<A HREF=\"";
      if (state.outputDocIndex() != docIndex_ || elementIndex_ == (unsigned)-1) {
	const String<CmdLineApp::AppChar> &outputFilename = state.outputFilename(docIndex_);
	size_t i = outputFilename.size();
	for (; i > 0; i--)
	  if (outputFilename[i - 1] == '\\' || outputFilename[i - 1] == '/')
	     break;
	for (; i < outputFilename.size(); i++)
	  os.put(outputFilename[i]);
      }
      if (elementIndex_ != (unsigned)-1)
	os << "#" << (unsigned long)elementIndex_;
      os << "\">";
    }
  }
}

String<CmdLineApp::AppChar> HtmlFOTBuilder::OutputState::outputFilename(unsigned index) const
{
  if (index == 0)
    return *outputFilename_;
  String<CmdLineApp::AppChar> base(*outputFilename_);
  String<CmdLineApp::AppChar> ext;
  for (size_t j = 0; j < 5; j++) {
    if (base.size() < j + 1)
      break;
    Char c = base[base.size() - j - 1];
    if (c == '/' || c == '\\')
      break;
    if (c == '.') {
      ext.assign(base.data() + (base.size() - j - 1), j + 1);
      base.resize(base.size() - j - 1);
      break;
    }
  }
  char buf[32];
  sprintf(buf, "%u", index);
  for (const char *p = buf; *p; p++)
    base += CmdLineApp::AppChar(*p);
  base += ext;
  return base;
}

void HtmlFOTBuilder
::OutputState::setCharStyle(const ConstPtr<CharStyle> &style, OutputCharStream &os)
{
  // We nest like this <A><FONT><I><B></B></I></FONT></A>
  if (style == style_)
    return;
  if (!style_.isNull()) {
    if (style_->isBold)
      os << "</B>";
    if (style_->isItalic)
      os << "</I>";
    if (style_->color != 0)
      os << "</FONT>";
  }
  AddressRef *oldRef;
  if (style_.isNull())
    oldRef = 0;
  else
    oldRef = style_->aref;
  AddressRef *newRef;
  if (style.isNull())
    newRef = 0;
  else
    newRef = style->aref;
  if (oldRef != newRef) {
    if (oldRef)
      oldRef->outputRef(1, os, *this);
    if (newRef)
      newRef->outputRef(0, os, *this);
  }
  if (!style.isNull()) {
    if (style->color != 0) {
      os << "<FONT COLOR=\"#";
      for (int i = 20; i >= 0; i -= 4)
	os << "0123456789abcdef"[(style->color >> i) & 0xf];
      os << '"' << '>';
    }
    if (style->isItalic)
      os << "<I>";
    if (style->isBold)
      os << "<B>";
  }
  style_ = style;
}

HtmlFOTBuilder::AddressRef::~AddressRef()
{
}

HtmlFOTBuilder::Item::~Item()
{
}

HtmlFOTBuilder::Document::~Document()
{
}

#ifdef DSSSL_NAMESPACE
}
#endif

#include "HtmlFOTBuilder_inst.cxx"
