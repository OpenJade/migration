// Copyright (c) 1997 James Clark
// See the file copying.txt for copying permission.

#include "config.h"
#include "TransformFOTBuilder.h"
#include "FOTBuilder.h"
#include "OutputCharStream.h"
#include "MessageArg.h"
#include "ErrnoMessageArg.h"

#include <errno.h>

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

const char RE = '\r';

class TransformFOTBuilder : public SerialFOTBuilder {
public:
  // SGML Transformations
  struct DocumentTypeNIC {
    ~DocumentTypeNIC();
    StringC name;
    StringC publicId;
    StringC systemId;
  };
  struct ElementNIC {
    ~ElementNIC();
    StringC gi;
    Vector<StringC> attributes;
  };
  class TransformExtensionFlowObj : public FOTBuilder::ExtensionFlowObj {
  public:
    virtual void atomic(TransformFOTBuilder &, const NodePtr &) const = 0;
  };
  class TransformCompoundExtensionFlowObj : public FOTBuilder::CompoundExtensionFlowObj {
  public:
    virtual void start(TransformFOTBuilder &, const NodePtr &) const = 0;
    virtual void end(TransformFOTBuilder &) const = 0;
  };
  class EntityRefFlowObj : public TransformExtensionFlowObj {
  public:
    void atomic(TransformFOTBuilder &fotb, const NodePtr &) const {
      fotb.entityRef(name_);
    }
    bool hasNIC(const StringC &name) const {
      return name == "name";
    }
    void setNIC(const StringC &name, const Value &value) {
      value.convertString(name_);
    }
    ExtensionFlowObj *copy() const { return new EntityRefFlowObj(*this); }
  private:
    StringC name_;
  };
  class ProcessingInstructionFlowObj : public TransformExtensionFlowObj {
  public:
    void atomic(TransformFOTBuilder &fotb, const NodePtr &) const {
      fotb.processingInstruction(data_);
    }
    bool hasNIC(const StringC &name) const {
      return name.size() == 4 && name[0] == 'd' && name[1] == 'a' && name[2] == 't' && name[3] == 'a';
    }
    void setNIC(const StringC &name, const Value &value) {
      value.convertString(data_);
    }
    ExtensionFlowObj *copy() const { return new ProcessingInstructionFlowObj(*this); }
  private:
    StringC data_;
  };
  class EmptyElementFlowObj : public TransformExtensionFlowObj {
    void atomic(TransformFOTBuilder &fotb, const NodePtr &nd) const {
      if (nic_.gi.size() > 0)
	fotb.emptyElement(nic_);
      else {
	GroveString str;
	if (nd && nd->getGi(str) == accessOK) {
	  ElementNIC tem(nic_);
	  tem.gi.assign(str.data(), str.size());
	  fotb.emptyElement(tem);
	}
	else
	  fotb.emptyElement(nic_);
      }
    }
    bool hasNIC(const StringC &name) const {
      return name == "gi" || name == "attributes";
    }
    void setNIC(const StringC &name, const Value &value) {
      switch (name[0]) {
      case 'g':
	value.convertString(nic_.gi);
	break;
      case 'a':
	value.convertStringPairList(nic_.attributes);
	break;
      }
    }
    ExtensionFlowObj *copy() const { return new EmptyElementFlowObj(*this); }
  private:
    ElementNIC nic_;
  };
  class ElementFlowObj : public TransformCompoundExtensionFlowObj {
    void start(TransformFOTBuilder &fotb, const NodePtr &nd) const {
      if (nic_.gi.size() > 0)
	fotb.startElement(nic_);
      else {
	GroveString str;
	if (nd && nd->getGi(str) == accessOK) {
	  ElementNIC tem(nic_);
	  tem.gi.assign(str.data(), str.size());
	  fotb.startElement(tem);
	}
	else
	  fotb.startElement(nic_);
      }
    }
    void end(TransformFOTBuilder &fotb) const {
      fotb.endElement();
    }
    bool hasNIC(const StringC &name) const {
      return name == "gi" || name == "attributes";
    }
    void setNIC(const StringC &name, const Value &value) {
      switch (name[0]) {
      case 'g':
	value.convertString(nic_.gi);
	break;
      case 'a':
	value.convertStringPairList(nic_.attributes);
	break;
      }
    }
    ExtensionFlowObj *copy() const { return new ElementFlowObj(*this); }
  private:
    ElementNIC nic_;
  };
  class EntityFlowObj : public TransformCompoundExtensionFlowObj {
    void start(TransformFOTBuilder &fotb, const NodePtr &) const {
      fotb.startEntity(systemId_);
    }
    void end(TransformFOTBuilder &fotb) const {
      fotb.endEntity();
    }
    bool hasNIC(const StringC &name) const {
      return name == "system-id";
    }
    void setNIC(const StringC &name, const Value &value) {
      value.convertString(systemId_);
    }
    ExtensionFlowObj *copy() const { return new EntityFlowObj(*this); }
  private:
    StringC systemId_;
  };
  class DocumentTypeFlowObj : public TransformExtensionFlowObj {
    void atomic(TransformFOTBuilder &fotb, const NodePtr &nd) const {
      fotb.documentType(nic_);
    }
    bool hasNIC(const StringC &name) const {
      return name == "system-id" || name == "public-id" || name == "name";
    }
    void setNIC(const StringC &name, const Value &value) {
      switch (name[0]) {
      case 's':
	value.convertString(nic_.systemId);
	break;
      case 'p':
	value.convertString(nic_.publicId);
	break;
      case 'n':
	value.convertString(nic_.name);
	break;
      }
    }
    ExtensionFlowObj *copy() const { return new DocumentTypeFlowObj(*this); }
  private:
    DocumentTypeNIC nic_;
  };
  TransformFOTBuilder(CmdLineApp *, bool xml);
  ~TransformFOTBuilder();
  void startElement(const ElementNIC &);
  void endElement();
  void emptyElement(const ElementNIC &);
  void characters(const Char *s, size_t n);
  void processingInstruction(const StringC &);
  void documentType(const DocumentTypeNIC &);
  void formattingInstruction(const StringC &);
  void entityRef(const StringC &);
  void startEntity(const StringC &);
  void endEntity();
  void extension(const ExtensionFlowObj &fo, const NodePtr &);
  void startExtensionSerial(const CompoundExtensionFlowObj &fo, const NodePtr &nd);
  void endExtensionSerial(const CompoundExtensionFlowObj &fo);
private:
  TransformFOTBuilder(const TransformFOTBuilder &);
  void operator=(const TransformFOTBuilder &);

  OutputCharStream &os() { return *os_; }
  void attributes(const Vector<StringC> &atts);

  CmdLineApp *app_;
  OutputCharStream *os_;
  Owner<OutputCharStream> topOs_;
  Vector<StringC> openElements_;
  StringC undefGi_;
  struct OpenFile : Link {
    ~OpenFile();
    OutputCharStream *saveOs;
    // fb must be before os so it gets destroyed afterwards
    FileOutputByteStream fb;
    Owner<OutputCharStream> os;
    StringC systemId;
  };
  IList<OpenFile> openFileStack_;
  bool xml_;
};

FOTBuilder *makeTransformFOTBuilder(CmdLineApp *app,
				    bool xml,
				    const FOTBuilder::Extension *&ext)
{
  static const TransformFOTBuilder::ProcessingInstructionFlowObj pi;
  static const TransformFOTBuilder::ElementFlowObj element;
  static const TransformFOTBuilder::EmptyElementFlowObj emptyElement;
  static const TransformFOTBuilder::EntityFlowObj entity;
  static const TransformFOTBuilder::EntityRefFlowObj entityRef;
  static const TransformFOTBuilder::DocumentTypeFlowObj documentType;
  static const FOTBuilder::Extension extensions[] = {
    {
      "UNREGISTERED::James Clark//Flow Object Class::processing-instruction",
      0,
      0,
      0,
      0,
      &pi
    },
    {
      "UNREGISTERED::James Clark//Flow Object Class::element",
      0,
      0,
      0,
      0,
      &element
    },
    {
      "UNREGISTERED::James Clark//Flow Object Class::empty-element",
      0,
      0,
      0,
      0,
      &emptyElement
    },
    {
      "UNREGISTERED::James Clark//Flow Object Class::entity",
      0,
      0,
      0,
      0,
      &entity
    },
    {
      "UNREGISTERED::James Clark//Flow Object Class::entity-ref",
      0,
      0,
      0,
      0,
      &entityRef
    },
    {
      "UNREGISTERED::James Clark//Flow Object Class::document-type",
      0,
      0,
      0,
      0,
      &documentType
    },
    { 0 }
  };
  ext = extensions;
  return new TransformFOTBuilder(app, xml);
}

static
void outputNumericCharRef(OutputCharStream &os, Char c)
{
  os << "&#" << (unsigned long)c << ';';
}

TransformFOTBuilder::TransformFOTBuilder(CmdLineApp *app, bool xml)
: app_(app),
  xml_(xml),
  topOs_(new RecordOutputCharStream(app->makeStdOut()))
{
  undefGi_ = app_->systemCharset().execToDesc("#UNDEF");
  topOs_->setEscaper(outputNumericCharRef);
  os_ = topOs_.pointer();
}

TransformFOTBuilder::~TransformFOTBuilder()
{
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
    os() << RE << atts[i] << '=';
    const StringC &s = atts[i + 1];
    if (!contains(s, '"'))
      os() << '"' << s << '"';
    else if (!contains(s, '\''))
      os() << '\'' << s << '\'';
    else {
      os() << '"';
      for (size_t j = 0; j < s.size(); j++) {
        if (s[j] == '"') {
	  if (xml_)
	    os() << "&quot;";
	  else
	    outputNumericCharRef(os(), '"');
	}
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
  if (xml_)
    os() << RE << '>';
  else
    os() << '>' << RE;
  openElements_.push_back(s);
  start();
}

void TransformFOTBuilder::emptyElement(const ElementNIC &nic)
{
  os() << "<";
  const StringC &s = nic.gi.size() == 0 ? undefGi_ : nic.gi;
  os() << s;
  attributes(nic.attributes);
  if (xml_)
    os() << "/>";
  else
    os() << '>';
  atomic();
}
 
void TransformFOTBuilder::endElement()
{
  if (!xml_)
    os() << RE;
  os() << "</" << openElements_.back();
  if (xml_)
    os() << RE;
  os() << '>';
  openElements_.resize(openElements_.size() - 1);
  end();
}

void TransformFOTBuilder::processingInstruction(const StringC &s)
{
  os() << "<?" << s;
  if (xml_)
    os() << "?>";
  else
    os() << '>';
  atomic();
}

void TransformFOTBuilder::formattingInstruction(const StringC &s)
{
  os() << s;
}

void TransformFOTBuilder::entityRef(const StringC &s)
{
  os() << "&" << s << ";";
}

void TransformFOTBuilder::startEntity(const StringC &systemId)
{
  OpenFile *ofp = new OpenFile;
  openFileStack_.insert(ofp);
  ofp->systemId = systemId;
  ofp->saveOs = os_;
  String<CmdLineApp::AppChar> filename;
#ifdef SP_WIDE_SYSTEM
  filename = systemId;
#else
  filename = app_->codingSystem()->convertOut(systemId);
#endif
  if (filename.size()) {
    filename += 0;
    if (!ofp->fb.open(filename.data())) {
      app_->message(CmdLineApp::openFileErrorMessage(),
		    StringMessageArg(systemId),
		    ErrnoMessageArg(errno));
    }
    else {
      ofp->os
	= new RecordOutputCharStream(
	      new EncodeOutputCharStream(&ofp->fb,
				         app_->outputCodingSystem()));
      ofp->os->setEscaper(outputNumericCharRef);
      os_ = ofp->os.pointer();
    }
  }
}

void TransformFOTBuilder::endEntity()
{
  OpenFile &of = *openFileStack_.head();
  if (of.os) {
    errno = 0;
    of.os->flush();
    if (!of.fb.close())
      app_->message(CmdLineApp::closeFileErrorMessage(),
		    StringMessageArg(of.systemId),
		    ErrnoMessageArg(errno));
  }
  os_ = of.saveOs;
  delete openFileStack_.get();
}

void TransformFOTBuilder::characters(const Char *s, size_t n)
{
  for (; n > 0; n--, s++) {
    switch (*s) {
    case '&':
      if (xml_)
	os() << "&amp;";
      else
	outputNumericCharRef(os(), *s);
      break;
    case '<':
      if (xml_)
	os() << "&lt;";
      else
	outputNumericCharRef(os(), *s);
      break;
    case '>':
      if (xml_)
	os() << "&gt;";
      else
	outputNumericCharRef(os(), *s);
      break;
    default:
      os().put(*s);
      break;
    }
  }
}

void TransformFOTBuilder::extension(const ExtensionFlowObj &fo, const NodePtr &nd)
{
  ((const TransformExtensionFlowObj &)fo).atomic(*this, nd);
}

void TransformFOTBuilder::startExtensionSerial(const CompoundExtensionFlowObj &fo, const NodePtr &nd)
{
  ((const TransformCompoundExtensionFlowObj &)fo).start(*this, nd);
}

void TransformFOTBuilder::endExtensionSerial(const CompoundExtensionFlowObj &fo)
{
  ((const TransformCompoundExtensionFlowObj &)fo).end(*this);
}

TransformFOTBuilder::OpenFile::~OpenFile()
{
}

TransformFOTBuilder::DocumentTypeNIC::~DocumentTypeNIC()
{
}

TransformFOTBuilder::ElementNIC::~ElementNIC()
{
}

#ifdef DSSSL_NAMESPACE
}
#endif

#include "TransformFOTBuilder_inst.cxx"
