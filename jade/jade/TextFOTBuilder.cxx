// TextFOTBuilder - a formatter for plain text output.
// Copyright (c) 1999 Avi Kivity

#include "config.h"
#include "TextFOTBuilder.h"
#include "macros.h"
#include <stack>

#undef   DBG_TEXT_BACKEND
#ifndef  DBG_TEXT_BACKEND
#   define DBG(x) void(0)
#else
#   define DBG(x) x
#   include <iostream.h>
#endif

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

#ifdef DSSSL_NAMESPACE
    namespace TextBackend {
#   define TEXTBACKEND_SCOPE TextBackend::
#else
#   define TEXTBACKEND_SCOPE
#endif

#ifdef DBG_TEXT_BACKEND
class Debugger {
public:
  Debugger(const char *func) : context_(func) { indent() << ">> " << context_ << endl; ++level_; }
  ~Debugger() { --level_; indent() << "<< " << context_ << endl; }
  ostream& indent() { for (int i = 0; i < level_; ++i) os() << "   "; return os(); }
  ostream& os() { return cout; }
private:
  static int level_;
  const char* context_;
};

int Debugger::level_ = 0;

template<class T> ostream& operator << (Debugger& d, const T& v) {
  return d.indent() << "-- " << v;
}
#endif


class Uncopyable {
public:
  Uncopyable() {}
private:
  Uncopyable(const Uncopyable&);     // undefined
  void operator=(const Uncopyable&); // undefined
};

class Abstract {
public:
  virtual ~Abstract() {}
};

typedef long Ordinate;
struct BoundBox {
  BoundBox() : left(0), right(0), top(0), bottom(0) {}
  Ordinate left, right, top, bottom;
};

class LineWidthProvider : Abstract
{
public:
  virtual int& numberOfSpaces() = 0;
  virtual Ordinate& freeLineWidth() = 0;
  virtual void nextLine() = 0;
};

struct TextArea {
  class Area : Abstract {
  public:
    virtual Ordinate width() const = 0;
    virtual Ordinate height() const = 0;
    virtual void chars(Ordinate x, Ordinate y, const Char* chars, unsigned n) = 0;
  };
  class Page : public Area {
  public:
    virtual void flush() = 0;
  };
  class RealPage : public Page {
  public:
    RealPage(OutputByteStream& os);
    virtual Ordinate width() const;
    virtual Ordinate height() const;
    virtual void chars(Ordinate x, Ordinate y, const Char* chars, unsigned n);
    virtual void flush();
  private:
    Char page_[66][80];
    OutputByteStream& os_;
  };
  class DummyPage : public Page {
  public:
    virtual Ordinate width() const;
    virtual Ordinate height() const;
    virtual void chars(Ordinate x, Ordinate y, const Char* chars, unsigned n);
    virtual void flush();
  };
  class SubArea : public Area {
  public:
    SubArea(Area& container, BoundBox bbox);
    virtual Ordinate width() const;
    virtual Ordinate height() const;
    virtual void chars(Ordinate x, Ordinate y, const Char* chars, unsigned n);
  private:
    BoundBox bbox_;
    Area& area_;
  };
  class MeasuredArea : public Area {
  public:
    MeasuredArea(Ordinate width, Ordinate max_height = 66);
    Ordinate measuredWidth() const;
    Ordinate measuredHeight() const;
    virtual Ordinate width() const;
    virtual Ordinate height() const;
    virtual void chars(Ordinate x, Ordinate y, const Char* chars, unsigned n);
  private:
    Ordinate width_;
    Ordinate height_;
    Ordinate measuredHeight_;
    Ordinate measuredWidth_;
  };
  class PageFactory : Abstract {
  public:
    virtual Page* create() const = 0;
  };
  class RealPageFactory : public PageFactory {
  public:
    RealPageFactory(OutputByteStream* os) : os_(os) {}
    virtual Page* create() const { return new RealPage(*os_); }
  private:
    OutputByteStream* os_;
  };
  class DummyPageFactory : public PageFactory {
  public:
    virtual Page* create() const { return new DummyPage; }
  };
};

class TextFOTBuilder
: Uncopyable, public FOTBuilder
{
public:
  TextFOTBuilder(TextArea::PageFactory& pageFactory, LineWidthProvider& lineWidthProvider);
  ~TextFOTBuilder();
private:
  virtual void startSimplePageSequence(FOTBuilder* headerFooter[nHF]);
  virtual void endSimplePageSequenceHeaderFooter();
  virtual void endSimplePageSequence();
  virtual void startParagraph(const ParagraphNIC &);
  virtual void endParagraph();
  virtual void characters(const Char *, size_t);
  virtual void setQuadding(Symbol);
public:
  struct Style {
    Style();
    Symbol quadding;
  };
  struct Model {
    struct Generated {
      Generated() : height(0), height_specified(0) {}
      SaveFOTBuilder content;
      Ordinate height;
      bool height_specified;
    };
    struct Region {
      Generated header;
      Generated footer;
      BoundBox  boundBox;  
    };
    struct PageModel {
      PageModel() : principalRegion(0) {}
      Vector<Region> regions;
      size_t principalRegion;
    };
    struct PageSequence {
      Vector<PageModel> initial;
      Vector<PageModel> repeat;
    };
  };
  struct Output {
    class FlowPort : Abstract {
    public:
      virtual Ordinate currentWidth() const = 0;
      virtual void emitLine(Ordinate x, const Char* chars, unsigned n) = 0;
      virtual void verticalSpace(Ordinate height) = 0;
    };
    class ParaBuilder : Abstract {
    public:
      virtual void chars(const Char* chars, unsigned n) = 0;
      virtual void pbreak() = 0;
    };
    class NestedParaBuilder : public ParaBuilder {
    public:  
      virtual void chars(const Char* chars, unsigned n);
      virtual void pbreak();
      void push(ParaBuilder* inner);
      void pop();
      ParaBuilder* top();
    private:
      Vector<ParaBuilder*> stack_;
    };
    class FlowParaBuilder : public ParaBuilder {
    public:
      FlowParaBuilder(FlowPort& flow, TextFOTBuilder& fotb);
      ~FlowParaBuilder();
      virtual void chars(const Char* chars, unsigned n);
      virtual void pbreak();
    private:
      void allocLine();
      void flushLine();
    private:
      FlowPort& flow_;
      TextFOTBuilder& fotb_;
      Ordinate x_;
      Char* line_;
    };
    class AreaFlowPort : public FlowPort {
    public:
      AreaFlowPort(TextArea::Area& area);
      virtual Ordinate currentWidth() const;
      virtual void emitLine(Ordinate x, const Char* chars, unsigned n);
      virtual void verticalSpace(Ordinate height);
    private:
      TextArea::Area& area_;
      Ordinate y_;
    };
    class PageSeqFlowPort : public FlowPort {
    public:
      PageSeqFlowPort(TextFOTBuilder& fotb, Model::PageSequence model);
      ~PageSeqFlowPort();
      virtual Ordinate currentWidth() const;
      virtual void emitLine(Ordinate x, const Char* chars, unsigned n);
      virtual void verticalSpace(Ordinate height);
    private:
      const TextArea::Area* getArea() const;
      TextArea::Area* getArea();
      TextArea::Area* createPrinicipalArea(TextArea::Area& page);
    private:
      TextFOTBuilder& fotb_;
      TextArea::PageFactory& pageFactory_;
      Vector<TextArea::Page*> output_;
      TextArea::Area* currentArea_;
      Model::PageSequence model_;
      Ordinate y_;
    };
  };
public:
  Ordinate measureGenerated(const Model::Generated& gen, const TextArea::Area& area);
  Ordinate quaddingOffset(Symbol quadding, Ordinate freeLineWidth);
  const Style& style() const { return styleStack_.top(); }
  Style& style()             { return styleStack_.top(); }
  virtual void start();
  virtual void end();
public:
  TextArea::PageFactory& pageFactory_;
  LineWidthProvider& lineWidthProvider_;
  FOTBuilder bitBucket_;
  Output::FlowPort* principalPort_;
  Output::NestedParaBuilder* para_;
  SaveFOTBuilder* spsHF;
  std::stack<Style> styleStack_;
  Style style_;
};

TextFOTBuilder::Style::Style()
: quadding ( symbolStart )
{
}

TextFOTBuilder::TextFOTBuilder(TextArea::PageFactory& pageFactory, LineWidthProvider& lineWidthProvider)
: pageFactory_(pageFactory)
, lineWidthProvider_(lineWidthProvider)
, para_(new Output::NestedParaBuilder)
, principalPort_(0)
{
  styleStack_.push(Style());
}

TextFOTBuilder::~TextFOTBuilder()
{
  delete para_;
}
 
void 
TextFOTBuilder::startSimplePageSequence(FOTBuilder* headerFooter[nHF]) 
{
  DBG(Debugger dbg("TextFOTBuilder::startSimplePageSequence()"));
  start();
  // FIXME: check PagePointer != 0
  spsHF = new SaveFOTBuilder[nHF];
  for (int i = 0; i < nHF; ++i)
    headerFooter[i] = &spsHF[i];
}

void 
TextFOTBuilder::endSimplePageSequenceHeaderFooter()
{
  DBG(Debugger dbg("TextFOTBuilder::endSimplePageSequenceHeaderFooter()"));
  Model::PageSequence ps;
  Model::PageModel pm;
  Model::Region r;
  r.boundBox.left = 0;
  r.boundBox.right = 80;
  r.boundBox.top = 0;
  r.boundBox.bottom = 66;
  pm.regions.push_back(r);
  ps.repeat.push_back(pm);
  HF lcr[3] = { leftHF, centerHF, rightHF };
  HF hf[2] = { headerHF, footerHF };
  Symbol quadding[3] = { symbolStart, symbolCenter, symbolEnd };
  Model::Generated (Model::Region::*gen[2]) 
    = { &Model::Region::header, &Model::Region::footer };
  for (int i = 0; i < 2; ++i) {
    Model::Generated& g = ps.repeat[0].regions[0].*(gen[i]);
    g.content.startSideBySide(DisplayNIC());
    for ( unsigned j = 0; j < 3; ++j ) {
      g.content.startSideBySideItem();
      g.content.setQuadding(quadding[j]);
      g.content.startParagraph(ParagraphNIC());
      spsHF[hf[i] | lcr[j] | firstHF | otherHF].emit(g.content);
      g.content.endParagraph();
      g.content.endSideBySideItem();
    }
    g.content.endSideBySide();
  }
  principalPort_ = new Output::PageSeqFlowPort(*this, ps);
}

void 
TextFOTBuilder::endSimplePageSequence()
{
  DBG(Debugger dbg("TextFOTBuilder::endSimplePageSequence()"));
  delete principalPort_;
  principalPort_ = 0;
  end();
}

void 
TextFOTBuilder::startParagraph(const ParagraphNIC &)
{
  DBG(Debugger dbg("TextFOTBuilder::startParagraph()"));
  start();
  para_->push(new Output::FlowParaBuilder(*principalPort_, *this));
}
 
void 
TextFOTBuilder::endParagraph()
{
  DBG(Debugger dbg("TextFOTBuilder::endParagraph()"));
  delete para_->top();
  para_->pop();
  end();
}
  
void 
TextFOTBuilder::characters(const Char* chars, size_t n)
{
  atomic();
  DBG(Debugger dbg("TextFOTBuilder::characters()"));
  DBG(dbg << "data = \"");
  DBG(for (size_t i = 0; i<n; ++i) dbg.os() << char(chars[i]));
  DBG(dbg.os() << "\"" << endl);
  para_->chars(chars, n);
}

TextArea::RealPage::RealPage(OutputByteStream& os)
: os_(os)
{
  DBG(Debugger dbg("TextArea::RealPage::RealPage()"));
  for (unsigned y = 0; y < height(); ++y)
    for (unsigned x = 0; x < width(); ++x)
      page_[y][x] = ' ';
}

Ordinate 
TextArea::RealPage::width() const
{
  return 80;
}

Ordinate 
TextArea::RealPage::height() const
{
  return 66;
}

void 
TextArea::RealPage::chars(Ordinate x, Ordinate y, const Char* chars, unsigned n)
{
  DBG(Debugger dbg("TextArea::RealPage::chars()"));
  DBG(dbg << "x = " << x << ", y = " << y << endl);
  for (unsigned i = 0; i < n; ++i) 
    page_[y][x+i] = chars[i];
}

void 
TextArea::RealPage::flush()
{
  DBG(Debugger dbg("TextArea::RealPage::flush()"));
  for (Ordinate y = 0; y < height(); ++y) {
    Ordinate end = width();
    while (end > 0 && page_[y][end-1] == ' ')
      --end;
    for (Ordinate x = 0; x < end; ++x)
      os_.sputc(char(page_[y][x])); // hope for ascii
    os_.sputc('\n');
  }
  os_.sputc('\f');
}

Ordinate 
TextArea::DummyPage::width() const
{
  return 80;
}

Ordinate 
TextArea::DummyPage::height() const
{
  return 66;
}

void 
TextArea::DummyPage::chars(Ordinate x, Ordinate y, const Char* chars, unsigned n)
{
  DBG(Debugger dbg("TextArea::DummyPage::chars()"));
  DBG(dbg << "x = " << x << ", y = " << y << endl);
}

void 
TextArea::DummyPage::flush()
{
}

TextArea::SubArea::SubArea(Area& container, BoundBox bbox)
: bbox_(bbox)
, area_(container)
{
}

Ordinate 
TextArea::SubArea::width() const
{
  return bbox_.right - bbox_.left;
}

Ordinate 
TextArea::SubArea::height() const
{
  return bbox_.bottom - bbox_.top;
}

void 
TextArea::SubArea::chars(Ordinate x, Ordinate y, const Char* chars, unsigned n)
{
  area_.chars(x+bbox_.left, y+bbox_.top, chars, n);
}

TextArea::MeasuredArea::MeasuredArea(Ordinate width, Ordinate max_height)
: width_(width)
, height_(max_height)
, measuredHeight_(0)
, measuredWidth_(0)
{
}

Ordinate 
TextArea::MeasuredArea::measuredHeight() const
{
  return measuredHeight_;
}

Ordinate 
TextArea::MeasuredArea::measuredWidth() const
{
  return measuredWidth_;
}

Ordinate 
TextArea::MeasuredArea::height() const
{
  return height_;
}

Ordinate 
TextArea::MeasuredArea::width() const
{
  return width_;
}

template <class T>
inline
const T&
max(const T& v1, const T& v2)
{
  return v1 > v2 ? v1 : v2;
}

void 
TextArea::MeasuredArea::chars(Ordinate x, Ordinate y, const Char* chars, unsigned n)
{
  measuredHeight_ = max(Ordinate(y+1), measuredHeight_);
  measuredWidth_ = max(Ordinate(x+n), measuredWidth_);
}

TextFOTBuilder::Output::FlowParaBuilder::FlowParaBuilder(FlowPort& flow, TextFOTBuilder& fotb)
: flow_(flow)
, fotb_(fotb)
, x_(0)
, line_(0)
{
  DBG(Debugger dbg("TextFOTBuilder::Output::FlowParaBuilder::FlowParaBuilder()"));
}

TextFOTBuilder::Output::FlowParaBuilder::~FlowParaBuilder()
{
  DBG(Debugger dbg("TextFOTBuilder::Output::FlowParaBuilder::~FlowParaBuilder()"));
  flushLine();
}

void
TextFOTBuilder::Output::FlowParaBuilder::allocLine()
{
  DBG(Debugger dbg("TextFOTBuilder::Output::FlowParaBuilder::allocLine()"));
  fotb_.lineWidthProvider_.nextLine();
  line_ = new Char[flow_.currentWidth()];
  for (Ordinate x = 0; x < flow_.currentWidth(); ++x) 
    line_[x] = ' ';
  x_ = 0;
}

void
TextFOTBuilder::Output::FlowParaBuilder::flushLine()
{
  DBG(Debugger dbg("TextFOTBuilder::Output::FlowParaBuilder::flushLine()"));
  if (line_) {
    if (x_ > 0) {
      Ordinate& freeLineWidth = fotb_.lineWidthProvider_.freeLineWidth();
      Ordinate offset = fotb_.quaddingOffset(fotb_.style().quadding, freeLineWidth);
      flow_.emitLine(offset, line_, x_);
      freeLineWidth = flow_.currentWidth() - x_;
    }
    delete[] line_;
    line_ = 0;
  }
}

void
TextFOTBuilder::Output::FlowParaBuilder::chars(const Char* chars, unsigned n)
{
  DBG(Debugger dbg("TextFOTBuilder::Output::FlowParaBuilder::chars()"));
  // break lines rather tastelessly.
  while (n > 0) {
    if (!line_)
      allocLine();
    if (x_ + n > flow_.currentWidth() - fotb_.lineWidthProvider_.freeLineWidth()) {
      while (n > 0, x_ < flow_.currentWidth() - fotb_.lineWidthProvider_.freeLineWidth())
        --n, line_[x_++] = *chars++;
      flushLine();
    } else {
      while (n > 0)
        --n, line_[x_++] = *chars++;
    }
  }
}

void
TextFOTBuilder::Output::FlowParaBuilder::pbreak()
{
  flushLine();
}

void
TextFOTBuilder::Output::NestedParaBuilder::chars(const Char* chars, unsigned n)
{
  if (stack_.size() == 0)
      CANNOT_HAPPEN();
  top()->chars(chars, n);
}

void
TextFOTBuilder::Output::NestedParaBuilder::pbreak()
{
  if (stack_.size() == 0)
      CANNOT_HAPPEN();
  top()->pbreak();
}

void
TextFOTBuilder::Output::NestedParaBuilder::push(ParaBuilder* inner)
{
  if (stack_.size())
    top()->pbreak();
  stack_.push_back(inner);
}

void 
TextFOTBuilder::Output::NestedParaBuilder::pop()
{
  stack_.erase(stack_.begin() + stack_.size()-1, stack_.begin() + stack_.size());
}

TextFOTBuilder::Output::ParaBuilder*
TextFOTBuilder::Output::NestedParaBuilder::top()
{
  return stack_.back();
}

TextFOTBuilder::Output::AreaFlowPort::AreaFlowPort(TextArea::Area& area)
: area_ ( area )
, y_ ( 0 )
{
}

Ordinate 
TextFOTBuilder::Output::AreaFlowPort::currentWidth() const
{
  return area_.width();
}

void 
TextFOTBuilder::Output::AreaFlowPort::emitLine(Ordinate x, const Char* chars, unsigned n)
{
  DBG(Debugger dbg("TextFOTBuilder::Output::AreaFlowPort::emitLine"));
  area_.chars(x, y_++, chars, n);
}

void 
TextFOTBuilder::Output::AreaFlowPort::verticalSpace(Ordinate height)
{   
}

TextFOTBuilder::Output::PageSeqFlowPort::PageSeqFlowPort
  (TextFOTBuilder& fotb, Model::PageSequence model)
: fotb_(fotb)
, pageFactory_(fotb.pageFactory_)
, model_(model)
, currentArea_(0)
, y_(0)
{
}

TextFOTBuilder::Output::PageSeqFlowPort::~PageSeqFlowPort()
{
  for (unsigned i = 0; i < output_.size(); ++i) {
    output_[i]->flush();
    delete output_[i];
  }
}

TextArea::Area*
TextFOTBuilder::Output::PageSeqFlowPort::getArea()
{
  if (!currentArea_ || (y_+1 >= currentArea_->height())) {
    DBG(Debugger dbg("TextFOTBuilder::Output::PageSeqFlowPort::getArea()"));
    DBG(dbg << "Creating a new page." << endl);
    output_.push_back(pageFactory_.create());
    currentArea_ = createPrinicipalArea(*output_.back());
    y_ = 0;
  }
  return currentArea_;
}

const TextArea::Area*
TextFOTBuilder::Output::PageSeqFlowPort::getArea() const
{
  return const_cast<PageSeqFlowPort*>(this)->getArea();
}


TextArea::Area*
TextFOTBuilder::Output::PageSeqFlowPort::createPrinicipalArea(TextArea::Area& page)
{
  DBG(Debugger dbg("TextFOTBuilder::Output::PageSeqFlowPort::createPrinicipalArea()"));
  FlowPort* save = fotb_.principalPort_;
  TextFOTBuilder::Output::NestedParaBuilder* save_para = fotb_.para_;
  fotb_.para_ = new NestedParaBuilder;
  DBG(dbg << "Measuring header" << endl);
  Ordinate headerHeight = fotb_.measureGenerated(model_.repeat[0].regions[0].header, page);
  DBG(dbg << "Measuring footer" << endl);
  Ordinate footerHeight = fotb_.measureGenerated(model_.repeat[0].regions[0].footer, page);
  DBG(dbg << "header = " << headerHeight << ", footer = " << footerHeight << endl);

  BoundBox bbox;
  bbox.left = 0;
  bbox.right = page.width();

  DBG(dbg << "Emitting header" << endl);
  bbox.top = 0;
  bbox.bottom = headerHeight;
  TextArea::SubArea header(page, bbox);
  Output::AreaFlowPort fp_h(header);
  fotb_.principalPort_ = &fp_h;
  model_.repeat[0].regions[0].header.content.emit(fotb_);
  
  DBG(dbg << "Emitting footer" << endl);
  bbox.top = page.height() - footerHeight;
  bbox.bottom = page.height();
  TextArea::SubArea footer(page, bbox);
  Output::AreaFlowPort fp_f(footer);
  fotb_.principalPort_ = &fp_f;
  model_.repeat[0].regions[0].footer.content.emit(fotb_);
  
  DBG(dbg << "Creating principal port subarea" << endl);
  delete fotb_.para_;
  fotb_.para_ = save_para;
  fotb_.principalPort_ = save;
  bbox.top = headerHeight;
  bbox.bottom = page.height() - footerHeight;
  return new TextArea::SubArea(page, bbox);
}

Ordinate
TextFOTBuilder::measureGenerated(const Model::Generated& g, const TextArea::Area& p)
{
  Output::FlowPort* save = principalPort_;
  TextArea::MeasuredArea ma ( p.width(), p.height() );
  Output::AreaFlowPort fp ( ma );
  principalPort_ = &fp;
  g.content.emit(*this);
  Ordinate height = ma.measuredHeight();
  principalPort_ = save;
  return height;
}

Ordinate 
TextFOTBuilder::Output::PageSeqFlowPort::currentWidth() const
{
  return getArea()->width();
}

void 
TextFOTBuilder::Output::PageSeqFlowPort::emitLine(Ordinate x, const Char* chars, unsigned n)
{
  DBG(Debugger dbg("TextFOTBuilder::Output::PageSeqFlowPort::emitLine()"));
  getArea()->chars(x, y_, chars, n);
  ++y_;
}

void  
TextFOTBuilder::Output::PageSeqFlowPort::verticalSpace(Ordinate height)
{
  ++y_; 
}

void 
TextFOTBuilder::setQuadding(Symbol quadding)
{
  style_.quadding = quadding;
}

void 
TextFOTBuilder::start()
{
  styleStack_.push(style_);
}

void 
TextFOTBuilder::end()
{
  styleStack_.pop();
  style_ = styleStack_.top();
}

Ordinate
TextFOTBuilder::quaddingOffset(Symbol quadding, long freeLineWidth)
{
  switch (quadding)
  {
  case symbolStart: return 0;
  case symbolCenter: return freeLineWidth/2;
  case symbolEnd: return freeLineWidth;
  }
  return 0;
}

class LineWidthCache
{
public:
  LineWidthCache() : fill_(cache_), use_() {}
  LineWidthProvider& fill() { return fill_; }
  LineWidthProvider& use()  { use_.reset(cache_); return use_; }
private:
  struct Entry {
    Entry() : nspaces(0), width(0) {}
    int nspaces;
    Ordinate width;
  };
  class Fill : public LineWidthProvider {
  public:
    Fill(Vector<Entry>& cache) : cache_(cache) { nextLine(); }
    virtual int& numberOfSpaces() { return cache_.back().nspaces; }
    virtual Ordinate& freeLineWidth() { return cache_.back().width; }
    virtual void nextLine() { cache_.push_back(Entry()); }
  private:
    Vector<Entry>& cache_;
  };
  class Use : public LineWidthProvider {
  public:
    virtual int& numberOfSpaces() { return (*line_).nspaces; }
    virtual Ordinate& freeLineWidth() { return (*line_).width; }
    virtual void nextLine() { ++line_; }
    void reset(Vector<Entry>& cache) { line_ = cache.begin(); }
  private:
    Vector<Entry>::iterator line_;
  };
private:
  Vector<Entry> cache_;
  Fill fill_;
  Use  use_;
  friend class Fill;
  friend class Use;
};

class MultipassTextFOTBuilder
: Uncopyable, public SaveFOTBuilder
{
public:
  MultipassTextFOTBuilder(OutputByteStream* os);
  ~MultipassTextFOTBuilder();
private:
  void process();
private:
  OutputByteStream* os_;
};

MultipassTextFOTBuilder::MultipassTextFOTBuilder(OutputByteStream* os)
: os_(os) 
{
}

MultipassTextFOTBuilder::~MultipassTextFOTBuilder()
{
  process();
}

void 
MultipassTextFOTBuilder::process()
{
  LineWidthCache lwc;
  TextArea::DummyPageFactory nooutput;
  TextFOTBuilder pass1(nooutput, lwc.fill());
  emit(pass1);
  TextArea::RealPageFactory output(os_);
  TextFOTBuilder pass2(output, lwc.use());
  emit(pass2);
}

#ifdef DSSSL_NAMESPACE
} // namespace TextBackend
#endif

FOTBuilder *makeTextFOTBuilder(OutputByteStream* os, Messenger *,
			      const FOTBuilder::Extension *&)
{
  return new TEXTBACKEND_SCOPE MultipassTextFOTBuilder(os);
}

#ifdef DSSSL_NAMESPACE
}
#endif

