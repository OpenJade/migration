// Copyright (c) 1999 Matthias Clasen
// See the file copying.txt for copying permission.

#include "stylelib.h"
#include "macros.h"
#include "DocumentGenerator.h"
#include <stdio.h>

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

#ifdef GROVE_NAMESPACE
using namespace GROVE_NAMESPACE;
#endif

//FIXME: this is just because OutputCharStream::operator<<(Newline) is broken
const char RE = '\r';

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
    NodePtr gdtd;
    ptr->getGoverningDoctype(gdtd);
    emit(gdtd);
    NodeListPtr prolog;
    GroveString sep;
    ptr->getProlog(prolog);
    emit(prolog, sep);
    NodePtr docelem;
    ptr->getDocumentElement(docelem);
    emit(docelem);
    NodeListPtr epilog;
    ptr->getProlog(epilog);
    emit(epilog, sep);
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
      os() << "'";
    }
    break;
  }
  case ComponentName::idAttributeValueToken: {
    GroveString tok;
    ptr->getToken(tok);
    os() << tok;
    break;
  }
  case ComponentName::idPi: {
    GroveString str;
    if (ptr->getEntityName(str) == accessOK) 
      os() << "&" << str << ";";
    else {
      ptr->getSystemData(str);
      os() << "<?" << str << ">";
    }
    break;
  }
  case ComponentName::idSdata: {
    GroveString str;
    if (ptr->getEntityName(str) == accessOK) 
      os() << "&" << str << ";";
    else {
      printf("can't emit sdata entity reference without name\n");
      GroveChar ch;
      ptr->getChar(*smap_, ch);
      os() << ch;
    }
    break;
  }
  case ComponentName::idSubdocument:
  case ComponentName::idExternalData: {
    GroveString str;
    if (ptr->getEntityName(str) == accessOK) 
      os() << "&" << str << ";";
    else 
      printf("can't emit entity reference without name\n");
    break;
  }
  case ComponentName::idDocumentType: {
    GroveString name;
    GroveString sep;
    ptr->getName(name);
    os() << "<!DOCTYPE " << name << " [" << RE;
    NamedNodeListPtr nnl;
    ptr->getNotations(nnl);
    emit(nnl->nodeList(), sep);
    ptr->getParameterEntities(nnl);
    emit(nnl->nodeList(), sep);
    ptr->getGeneralEntities(nnl);
    emit(nnl->nodeList(), sep);
    NodePtr dent;
    if (ptr->getDefaultEntity(dent) == accessOK) {
      emit(dent);
    }
    ptr->getElementTypes(nnl);
    emit(nnl->nodeList(), sep);
    os() << "]>" << RE;
    break;
  } 
  case ComponentName::idNotation: {
    GroveString name;
    ptr->getName(name);
    os() << "<!notation " << name << " ";
    NodePtr extid;
    ptr->getExternalId(extid);
    emit(extid);
    os() << ">" << RE;
    //FIXME: handle attributes
    break;
  }
  case ComponentName::idDefaultEntity: 
  case ComponentName::idEntity: {
    bool defaulted;
    ptr->getDefaulted(defaulted);
    if (defaulted) 
      break;
    os() << "<!ENTITY ";
    Node::EntityType::Enum type;
    ptr->getEntityType(type);
    GroveString name;
    ptr->getName(name);
    if (name.size() > 0)
      os() << name << " ";
    else 
      os() << "#DEFAULT ";
    NodePtr extid;
    if (ptr->getExternalId(extid) == accessOK) {
      emit(extid);
      switch (type) {
      case Node::EntityType::subdocument:
	os() << "SUBDOC";
	break;
      case Node::EntityType::cdata:
	os() << "CDATA";
	break;
      case Node::EntityType::sdata:
	os() << "SDATA";
	break;
      case Node::EntityType::ndata: 
	os()  << "NDATA";
	break;
      default:
	CANNOT_HAPPEN();
      }
      switch (type) {
      case Node::EntityType::cdata:
      case Node::EntityType::sdata:
      case Node::EntityType::ndata: {
	GroveString notname;
	ptr->getNotationName(notname);
	os() << " " << notname;
	//FIXME: handle data attributes
	break;
      }
      default:
	break;
      }
    } 
    else { // internal entity
      switch (type) {
      case Node::EntityType::text:
	break;
      case Node::EntityType::pi:
	os() << "PI ";
	break;
      case Node::EntityType::cdata:
	os() << "CDATA ";
	break;
      case Node::EntityType::sdata:
	os() << "SDATA ";
	break;
      default:
	CANNOT_HAPPEN();
      }
      GroveString text;
      ptr->getText(text);
      os() << "'" << text << "'"; 
    }
    os() << ">" << RE;
    break;
  }
  case ComponentName::idElementType: {
    GroveString gi;
    ptr->getGi(gi);
    os() << "<!ELEMENT " << gi << " ";
    bool ostart, oend;
    ptr->getOmitStartTag(ostart);
    ptr->getOmitEndTag(oend);
    os() << (ostart ? "O " : "- ") << (oend ? "O " : "- ");
    Node::ContentType::Enum type;
    ptr->getContentType(type);
    switch(type) {
    case Node::ContentType::cdata:
      os() << "CDATA";
      break;
    case Node::ContentType::rcdata:
      os() << "RCDATA";
      break;
    case Node::ContentType::empty:
      os() << "EMPTY";
      break;
    case Node::ContentType::any:
      os() << "ANY";
      break;
    case Node::ContentType::modelgrp: {
      NodePtr model;
      ptr->getModelGroup(model);
      emit(model);
      break;
    }
    default:
      CANNOT_HAPPEN();
    }
    //FIXME: handle inclusions and exclusions
    os() << ">" << RE;
    NamedNodeListPtr atts;
    ptr->getAttributeDefs(atts);
    NodePtr first;
    if (atts->nodeList()->first(first) == accessOK) {
      os() << "<!ATTLIST " << gi << RE;
      GroveChar ch = RE;
      GroveString sep(&ch, 1);
      emit(atts->nodeList(), sep);
      os() << RE << ">" << RE;
    }
    break;
  }
  case ComponentName::idAttributeDef: {
    GroveString name;
    ptr->getName(name);
    os() << "  " << name;
    //FIXME
    break;
  }
  case ComponentName::idExternalId: {
    break;
  }
  case ComponentName::idModelGroup: {
    break;
  }
  case ComponentName::idSgmlConstants: {
  case ComponentName::noId: // message and nonSgml 
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
