// TextFOTBuilder - a formatter for plain text output.
// Copyright (c) 1999 Avi Kivity

#include "config.h"
#include "TextFOTBuilder.h"

#undef  DBG_TEXT_BACKEND
#ifndef DBG_TEXT_BACKEND
#   define DBG(x) void(0)
#else
#   define DBG(x) x
#   include <iostream.h>
#endif

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
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

class TextFOTBuilder
: Uncopyable, public FOTBuilder
{
public:
  TextFOTBuilder(OutputByteStream* os);
private:
  virtual void startSimplePageSequence(FOTBuilder* headerFooter[nHF]);
  virtual void endSimplePageSequenceHeaderFooter();
  virtual void endSimplePageSequence();
  void startParagraph(const ParagraphNIC &);
  void endParagraph();
  void characters(const Char *, size_t);
public:
  typedef long Ordinate;
    struct BoundBox {
	    BoundBox() : left(0), right(0), top(0), bottom(0) {}
	    Ordinate left, right, top, bottom;
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
      virtual void emitLine(const Char* chars, unsigned n) = 0;
      virtual void verticalSpace(Ordinate height) = 0;
    };
    class ParaBuilder : Abstract {
    public:
      virtual void chars(const Char* chars, unsigned n) = 0;
    };
    class Area : Abstract {
    public:
      virtual Ordinate width() const = 0;
      virtual Ordinate height() const = 0;
      virtual void chars(Ordinate x, Ordinate y, const Char* chars, unsigned n) = 0;
    };
    class Page : public Area {
    public:
      Page();
      virtual Ordinate width() const;
      virtual Ordinate height() const;
      virtual void chars(Ordinate x, Ordinate y, const Char* chars, unsigned n);
      void flush(OutputByteStream& os);
    private:
      Char page_[66][80];
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
    class FlowParaBuilder : public ParaBuilder {
    public:
      FlowParaBuilder(FlowPort& flow);
      ~FlowParaBuilder();
      virtual void chars(const Char* chars, unsigned n);
    private:
      void allocLine();
      void flushLine();
    private:
      FlowPort& flow_;
      Ordinate x_;
      Char* line_;
    };
    class AreaFlowPort : public FlowPort {
    public:
      AreaFlowPort(Area& area);
      virtual Ordinate currentWidth() const;
      virtual void emitLine(const Char* chars, unsigned n);
      virtual void verticalSpace(Ordinate height);
    private:
      Area& area_;
      Ordinate y_;
    };
    class PageSeqFlowPort : public FlowPort {
    public:
      PageSeqFlowPort(TextFOTBuilder& fotb, Model::PageSequence model);
      ~PageSeqFlowPort();
      virtual Ordinate currentWidth() const;
      virtual void emitLine(const Char* chars, unsigned n);
      virtual void verticalSpace(Ordinate height);
    private:
      const Area* getArea() const;
      Area* getArea();
      Area* createPrinicipalArea(Area& page);
    private:
      TextFOTBuilder& fotb_;
      OutputByteStream& os_;
      Vector<Page> output_;
      Area* currentArea_;
      Model::PageSequence model_;
      Ordinate y_;
    };
  };
public:
  Ordinate measureGenerated(const Model::Generated& gen, const Output::Area& area);
public:
  FOTBuilder bitBucket_;
  Output::FlowPort* principalPort_;
  Output::ParaBuilder* para_;
  OutputByteStream& os_;
  SaveFOTBuilder* spsHF;
};

TextFOTBuilder::TextFOTBuilder(OutputByteStream* os)
: os_(*os)
, para_(0)
, principalPort_(0)
{
}
 
void 
TextFOTBuilder::startSimplePageSequence(FOTBuilder* headerFooter[nHF]) 
{
  DBG(cout << "TextFOTBuilder::startSimplePageSequence()" << endl);
  // FIXME: check PagePointer != 0
  spsHF = new SaveFOTBuilder[nHF];
  for (int i = 0; i < nHF; ++i)
    headerFooter[i] = &spsHF[i];
}

void 
TextFOTBuilder::endSimplePageSequenceHeaderFooter()
{
  DBG(cout << "TextFOTBuilder::endSimplePageSequenceHeaderFooter()" << endl);
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
  DBG(cout << "TextFOTBuilder::endSimplePageSequence()" << endl);
  delete principalPort_;
  principalPort_ = 0;
}

void 
TextFOTBuilder::startParagraph(const ParagraphNIC &)
{
  DBG(cout << "TextFOTBuilder::startParagraph()" << endl);
  // FIXME: nested paragraphs
  para_ = new Output::FlowParaBuilder(*principalPort_);
}
 
void 
TextFOTBuilder::endParagraph()
{
  DBG(cout << "TextFOTBuilder::endParagraph()" << endl);
  // FIXME: nested paragraphs
  delete para_;
  para_ = 0;
}
  
void 
TextFOTBuilder::characters(const Char* chars, size_t n)
{
  DBG(cout << "TextFOTBuilder::characters()" << endl);
	para_->chars(chars, n);
}


TextFOTBuilder::Output::Page::Page()
{
  for (unsigned y = 0; y < height(); ++y)
    for (unsigned x = 0; x < width(); ++x)
      page_[y][x] = ' ';
}

TextFOTBuilder::Ordinate 
TextFOTBuilder::Output::Page::width() const
{
  return 80;
}

TextFOTBuilder::Ordinate 
TextFOTBuilder::Output::Page::height() const
{
  return 66;
}

void 
TextFOTBuilder::Output::Page::chars(Ordinate x, Ordinate y, const Char* chars, unsigned n)
{
  for (unsigned i = 0; i < n; ++i) 
    page_[y][x+i] = chars[i];
}

void 
TextFOTBuilder::Output::Page::flush(OutputByteStream& os)
{
  for (Ordinate y = 0; y < height(); ++y) {
    Ordinate end = width();
    while (end > 0 && page_[y][end-1] == ' ')
      --end;
    for (Ordinate x = 0; x < end; ++x)
      os.sputc(char(page_[y][x])); // hope for ascii
    os.sputc('\n');
  }
}

TextFOTBuilder::Output::SubArea::SubArea(Area& container, BoundBox bbox)
: bbox_(bbox)
, area_(container)
{
}

TextFOTBuilder::Ordinate 
TextFOTBuilder::Output::SubArea::width() const
{
  return bbox_.right - bbox_.left;
}

TextFOTBuilder::Ordinate 
TextFOTBuilder::Output::SubArea::height() const
{
  return bbox_.bottom - bbox_.top;
}

void 
TextFOTBuilder::Output::SubArea::chars(Ordinate x, Ordinate y, const Char* chars, unsigned n)
{
  area_.chars(x+bbox_.left, y+bbox_.top, chars, n);
}

TextFOTBuilder::Output::MeasuredArea::MeasuredArea(Ordinate width, Ordinate max_height)
: width_(width)
, height_(max_height)
, measuredHeight_(0)
, measuredWidth_(0)
{
}

TextFOTBuilder::Ordinate 
TextFOTBuilder::Output::MeasuredArea::measuredHeight() const
{
  return measuredHeight_;
}

TextFOTBuilder::Ordinate 
TextFOTBuilder::Output::MeasuredArea::measuredWidth() const
{
  return measuredWidth_;
}

TextFOTBuilder::Ordinate 
TextFOTBuilder::Output::MeasuredArea::height() const
{
  return height_;
}

TextFOTBuilder::Ordinate 
TextFOTBuilder::Output::MeasuredArea::width() const
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
TextFOTBuilder::Output::MeasuredArea::chars(Ordinate x, Ordinate y, const Char* chars, unsigned n)
{
  measuredHeight_ = max(Ordinate(y+1), measuredHeight_);
  measuredWidth_ = max(Ordinate(x+n), measuredWidth_);
}

TextFOTBuilder::Output::FlowParaBuilder::FlowParaBuilder(FlowPort& flow)
: flow_(flow)
, x_(0)
, line_(0)
{
}

TextFOTBuilder::Output::FlowParaBuilder::~FlowParaBuilder()
{
  flushLine();
}

void
TextFOTBuilder::Output::FlowParaBuilder::allocLine()
{
  line_ = new Char[flow_.currentWidth()];
  for (Ordinate x = 0; x < flow_.currentWidth(); ++x) 
    line_[x] = ' ';
  x_ = 0;
}

void
TextFOTBuilder::Output::FlowParaBuilder::flushLine()
{
  if (line_) {
    flow_.emitLine(line_, x_);
    delete[] line_;
    line_ = 0;
  }
}

void
TextFOTBuilder::Output::FlowParaBuilder::chars(const Char* chars, unsigned n)
{
  // break lines rather tastelessly.
  while (n > 0) {
    if (!line_)
      allocLine();
    if (x_ + n > flow_.currentWidth()) {
      while (n > 0, x_ < flow_.currentWidth())
        --n, line_[x_++] = *chars++;
      flushLine();
    } else {
      while (n > 0)
        --n, line_[x_++] = *chars++;
    }
  }
}

TextFOTBuilder::Output::AreaFlowPort::AreaFlowPort(Area& area)
: area_ ( area )
, y_ ( 0 )
{
}

TextFOTBuilder::Ordinate 
TextFOTBuilder::Output::AreaFlowPort::currentWidth() const
{
  return area_.width();
}

void 
TextFOTBuilder::Output::AreaFlowPort::emitLine(const Char* chars, unsigned n)
{
  area_.chars(0, y_++, chars, n);
}

void 
TextFOTBuilder::Output::AreaFlowPort::verticalSpace(Ordinate height)
{   
}

TextFOTBuilder::Output::PageSeqFlowPort::PageSeqFlowPort
  (TextFOTBuilder& fotb, Model::PageSequence model)
: fotb_(fotb)
, os_(fotb.os_)
, model_(model)
, currentArea_(0)
, y_(0)
{
}

TextFOTBuilder::Output::PageSeqFlowPort::~PageSeqFlowPort()
{
  for (unsigned i = 0; i < output_.size(); ++i)
    output_[i].flush(os_);
}

TextFOTBuilder::Output::Area*
TextFOTBuilder::Output::PageSeqFlowPort::getArea()
{
  if (!currentArea_ || (y_+1 >= currentArea_->height())) {
    output_.push_back(Output::Page());
    currentArea_ = createPrinicipalArea(output_.back());
    y_ = 0;
  }
  return currentArea_;
}

const TextFOTBuilder::Output::Area*
TextFOTBuilder::Output::PageSeqFlowPort::getArea() const
{
  return const_cast<TextFOTBuilder::Output::PageSeqFlowPort*>(this)->getArea();
}


TextFOTBuilder::Output::Area*
TextFOTBuilder::Output::PageSeqFlowPort::createPrinicipalArea(Area& page)
{
  FlowPort* save = fotb_.principalPort_;
  Ordinate headerHeight = fotb_.measureGenerated(model_.repeat[0].regions[0].header, page);
  Ordinate footerHeight = fotb_.measureGenerated(model_.repeat[0].regions[0].footer, page);

  BoundBox bbox;
  bbox.left = 0;
  bbox.right = page.width();

  bbox.top = 0;
  bbox.bottom = headerHeight;
  Output::SubArea header(page, bbox);
  Output::AreaFlowPort fp_h(header);
  fotb_.principalPort_ = &fp_h;
  model_.repeat[0].regions[0].header.content.emit(fotb_);
  
  bbox.top = page.height() - footerHeight;
  bbox.bottom = page.height();
  Output::SubArea footer(page, bbox);
  Output::AreaFlowPort fp_f(footer);
  fotb_.principalPort_ = &fp_f;
  model_.repeat[0].regions[0].footer.content.emit(fotb_);
  
  fotb_.principalPort_ = save;
  bbox.top = headerHeight;
  bbox.bottom = page.height() - footerHeight;
  return new Output::SubArea(page, bbox);
}

TextFOTBuilder::Ordinate
TextFOTBuilder::measureGenerated(const Model::Generated& g, const Output::Area& p)
{
  Output::FlowPort* save = principalPort_;
  Output::MeasuredArea ma ( p.width(), p.height() );
  Output::AreaFlowPort fp ( ma );
  principalPort_ = &fp;
  g.content.emit(*this);
  Ordinate height = ma.measuredHeight();
  principalPort_ = save;
  return height;
}

TextFOTBuilder::Ordinate 
TextFOTBuilder::Output::PageSeqFlowPort::currentWidth() const
{
  return getArea()->width();
}

void 
TextFOTBuilder::Output::PageSeqFlowPort::emitLine(const Char* chars, unsigned n)
{
  getArea()->chars(0, y_, chars, n);
  ++y_;
}

void  
TextFOTBuilder::Output::PageSeqFlowPort::verticalSpace(Ordinate height)
{
  ++y_; 
}

FOTBuilder *makeTextFOTBuilder(OutputByteStream* os, Messenger *,
			      const FOTBuilder::Extension *&)
{
  return new TextFOTBuilder(os);
}

#ifdef DSSSL_NAMESPACE
}
#endif

