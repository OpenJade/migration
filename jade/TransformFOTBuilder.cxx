// Copyright (c) 1997 James Clark
// See the file copying.txt for copying permission.

#include "config.h"
#include "TransformFOTBuilder.h"
#include "FOTBuilder.h"
#include "OutputCharStream.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

const char RE = '\r';

class TransformFOTBuilder : public SerialFOTBuilder {
public:
  TransformFOTBuilder(CmdLineApp *);
  void startElement(const ElementNIC &);
  void endElement();
  void emptyElement(const ElementNIC &);
  void characters(const Char *s, size_t n);
  void processingInstruction(const StringC &);
  void documentType(const DocumentTypeNIC &);
private:
  TransformFOTBuilder(const TransformFOTBuilder &);
  void operator=(const TransformFOTBuilder &);

  OutputCharStream &os() { return *os_; }
  void attributes(const Vector<StringC> &atts);

  CmdLineApp *app_;
  Owner<OutputCharStream> os_;
  Vector<StringC> openElements_;
  StringC undefGi_;
};

FOTBuilder *makeTransformFOTBuilder(CmdLineApp *app,
				    const FOTBuilder::Extension *&ext)
{
  ext = 0;
  return new TransformFOTBuilder(app);
}

static
void outputNumericCharRef(OutputCharStream &os, Char c)
{
  os << "&#" << (unsigned long)c << ';';
}

TransformFOTBuilder::TransformFOTBuilder(CmdLineApp *app)
: app_(app),
  os_(new RecordOutputCharStream(app->makeStdOut()))
{
  undefGi_ = app_->systemCharset().execToDesc("#UNDEF");
  os_->setEscaper(outputNumericCharRef);
}

static bool contains(const StringC &str, Char c)
{
  for (size_t i = 0; i < str.size(); i++)
    if (str[i] == c)
       return 1;
  return 0;
}

void TransformFOTBuilder::documentType(const DocumentTypeNIC &nic)
{
  if (nic.name.size()) {
    os() << "<!DOCTYPE " << nic.name;
    if (nic.publicId.size())
      os() << " PUBLIC \"" << nic.publicId << '"';
    else 
      os() << " SYSTEM";
    if (nic.systemId.size()) {
      char quote = contains(nic.systemId, '"') ? '\'' : '"';
      os() << quote << nic.systemId << quote;
    }
    os() << '>' << RE;
  }
  atomic();
}

void TransformFOTBuilder::attributes(const Vector<StringC> &atts)
{
  for (size_t i = 0; i < atts.size(); i += 2) {
    os() << ' ' << atts[i] << '=';
    const StringC &s = atts[i + 1];
    if (!contains(s, '"'))
      os() << '"' << s << '"';
    else if (!contains(s, '\''))
      os() << '\'' << s << '\'';
    else {
      os() << '"';
      for (size_t j = 0; j < s.size(); j++) {
        if (s[j] == '"')
          os() << "&quot;";
        else
          os().put(s[j]);
      }
      os() << '"';
    }
  }
}

void TransformFOTBuilder::startElement(const ElementNIC &nic)
{
  os() << "<";
  const StringC &s = nic.gi.size() == 0 ? undefGi_ : nic.gi;
  os() << s;
  attributes(nic.attributes);
  os() << ">" << RE;
  openElements_.push_back(s);
  start();
}

void TransformFOTBuilder::emptyElement(const ElementNIC &nic)
{
  os() << "<";
  const StringC &s = nic.gi.size() == 0 ? undefGi_ : nic.gi;
  os() << s;
  attributes(nic.attributes);
  os() << '>';
  atomic();
}
 
void TransformFOTBuilder::endElement()
{
  os() << RE << "</" << openElements_.back() << ">";
  openElements_.resize(openElements_.size() - 1);
  end();
}

void TransformFOTBuilder::processingInstruction(const StringC &s)
{
  os() << "<?" << s << '>';
  atomic();
}

void TransformFOTBuilder::characters(const Char *s, size_t n)
{
  for (; n > 0; n--, s++) {
    switch (*s) {
    case '&':
      os() << "&amp;";
      break;
    case '<':
      os() << "&lt;";
      break;
    case '>':
      os() << "&gt;";
      break;
    default:
      os().put(*s);
      break;
    }
  }
}

#ifdef DSSSL_NAMESPACE
}
#endif
