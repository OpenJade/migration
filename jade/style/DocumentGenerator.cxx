// Copyright (c) 1999 Matthias Clasen
// See the file copying.txt for copying permission.

#include "stylelib.h"
#include "DocumentGenerator.h"
#include <stdio.h>

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

#ifdef GROVE_NAMESPACE
using namespace GROVE_NAMESPACE;
#endif

inline
OutputCharStream &operator<<(OutputCharStream &os, const GroveString &str)
{
  os << StringC((const Char *)str.data(), str.size());
  return os;
}

inline
OutputCharStream &operator<<(OutputCharStream &os, const GroveChar ch)
{
  os << StringC((const Char *)&ch, 1);
  return os;
};

DocumentGenerator::DocumentGenerator(SdataMapper *smap, OutputCharStream *os)
 : smap_(smap), os_(os)
{
}

void DocumentGenerator::emit(const NodePtr &ptr) 
{
  switch (ptr->classDef().className) {
  case ComponentName::idSgmlDocument: {
    NodePtr docelem;
    ptr->getDocumentElement(docelem);
    emit(docelem);
    break;
  }
  case ComponentName::idElement: {
    GroveString gi;
    ptr->getGi(gi);
    os() << "<" << gi;
    NamedNodeListPtr atts;
    GroveString sep;
    ptr->getAttributes(atts);
    emit(atts->nodeList(), sep);
    os() << ">"; 
    NodeListPtr content;
    ptr->getContent(content);
    emit(content, sep);
    bool mustOmitEndTag;
    ptr->getMustOmitEndTag(mustOmitEndTag);
    if (!mustOmitEndTag) 
      os() << "</" << gi << ">";
    break;
  }
  case ComponentName::idDataChar: {
    GroveChar ch;
    ptr->getChar(*smap_, ch);
    os() << ch;
    break;
  }
  case ComponentName::idAttributeAssignment: {
    bool implied;
    ptr->getImplied(implied);
    if (!implied) {
      GroveString name;
      ptr->getName(name);
      os() << " " << name << "='";
      GroveChar ch;
      GroveString sep;
      if (ptr->getTokenSep(ch) != accessNull) 
        sep.assign(&ch, 1);
      NodeListPtr value;
      ptr->getValue(value); 
      emit(value, sep); 
    }
    break;
  }
  case ComponentName::idAttributeValueToken: {
    GroveString tok;
    ptr->getToken(tok);
    os() << tok;
    break;
  }
  default: {
    printf("unsupported node class skipped\n");
  }
  }
}

void DocumentGenerator::emit(const NodeListPtr &ptr, const GroveString &sep) 
{
   NodeListPtr nl(ptr);
   NodePtr nd;
   if (nl->first(nd) == accessNull)
     return;
   nl->rest(nl);
   for (;;) {
     emit(nd);
     if (nl->first(nd) == accessNull)
       return;
     nl->rest(nl);
     os() << sep;
   }  
} 

#ifdef DSSSL_NAMESPACE
}
#endif
