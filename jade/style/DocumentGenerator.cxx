// Copyright (c) 1999 Matthias Clasen
// See the file copying.txt for copying permission.

#include "stylelib.h"
#include <OpenSP/macros.h>
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

void DocumentGenerator::emit(const GroveStringListPtr &strs, 
                             const char *a, 
                             const char *sep,
                             const char *o)
{
  ConstGroveStringListIter iter(ConstGroveStringListIter(strs->iter()));
  if (!iter.done()) {
    os() << a << iter.cur();
    iter.next();
    for (;!iter.done();iter.next()) 
      os() << sep << iter.cur();
    os() << o;
  }
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
    if (ptr->getProlog(prolog) == accessOK)
      emit(prolog, sep);
    NodePtr docelem;
    if (ptr->getDocumentElement(docelem) == accessOK)
      emit(docelem);
    NodeListPtr epilog;
    if (ptr->getProlog(epilog) == accessOK)
      emit(epilog, sep);
    break;
  }
  case ComponentName::idElement: {
    GroveString gi;
    ptr->getGi(gi);
    os() << "<" << gi;
    NamedNodeListPtr atts;
    GroveChar ch = ' ';
    GroveString sep(&ch, 1);
    NodePtr nd;
    if (ptr->getAttributes(atts) == accessOK
        && atts->nodeList()->first(nd) == accessOK) {
      os() << " ";
      emit(atts->nodeList(), sep);
    }
    os() << ">"; 
    NodeListPtr content;
    sep.assign(0, 0);
    if (ptr->getContent(content) == accessOK)
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
      os() << name << "='";
      GroveChar ch;
      GroveString sep;
      if (ptr->getTokenSep(ch) != accessNull) 
        sep.assign(&ch, 1);
      NodeListPtr value;
      if (ptr->getValue(value) == accessOK)
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
  case ComponentName::idSdata: 
  case ComponentName::idSubdocument:
  case ComponentName::idExternalData: {
    GroveString str;
    ptr->getEntityName(str);
    os() << "&" << str << ";";
    break;
  }
  case ComponentName::idDocumentType: {
    GroveString name;
    GroveString sep;
    ptr->getName(name);
    os() << "<!DOCTYPE " << name << " [" << RE;
    NamedNodeListPtr nnl;
    if (ptr->getNotations(nnl) == accessOK)
      emit(nnl->nodeList(), sep);
    if (ptr->getParameterEntities(nnl) == accessOK)
      emit(nnl->nodeList(), sep);
    if (ptr->getGeneralEntities(nnl) == accessOK)
      emit(nnl->nodeList(), sep);
    NodePtr dent;
    if (ptr->getDefaultEntity(dent) == accessOK) 
      emit(dent);
    if (ptr->getElementTypes(nnl) == accessOK)
      emit(nnl->nodeList(), sep);
    os() << "]>" << RE;
    break;
  } 
  case ComponentName::idNotation: {
    GroveString name;
    ptr->getName(name);
    os() << "<!NOTATION " << name << " ";
    NodePtr extid;
    if (ptr->getExternalId(extid) == accessOK)
      emit(extid);
    os() << ">" << RE;
    NamedNodeListPtr atts;
    NodePtr first;
    if (ptr->getAttributeDefs(atts) == accessOK
	&& atts->nodeList()->first(first) == accessOK) {
      os() << "<!ATTLIST #NOTATION " << name << RE;
      GroveChar ch = RE;
      GroveString sep(&ch, 1);
      emit(atts->nodeList(), sep);
      os() << " >" << RE;
    }
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
	os() << " SUBDOC";
	break;
      case Node::EntityType::cdata:
	os() << " CDATA";
	break;
      case Node::EntityType::sdata:
	os() << " SDATA";
	break;
      case Node::EntityType::ndata: 
	os()  << " NDATA";
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
	os() << " " << notname << " ";
        NamedNodeListPtr atts;
        NodePtr first;
        if (ptr->getAttributes(atts) == accessOK 
          && atts->nodeList()->first(first) == accessOK) {
          os() << "[";
	  GroveChar ch = ' ';
          GroveString sep(&ch, 1);
          emit(atts->nodeList(), sep);
	  os() << "] ";
        }
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
      os() << "'" << text << "' "; 
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
      if (ptr->getModelGroup(model) == accessOK)
	emit(model);
      break;
    }
    default:
      CANNOT_HAPPEN();
    }
    GroveStringListPtr cl;
    if (ptr->getExclusions(cl) == accessOK) 
      emit(cl, " -(", "|", ")");  
    if (ptr->getInclusions(cl) == accessOK) 
      emit(cl, " +(", "|", ")");  
    os() << " >" << RE;
    NamedNodeListPtr atts;
    NodePtr first;
    if (ptr->getAttributeDefs(atts) == accessOK
	&& atts->nodeList()->first(first) == accessOK) {
      os() << "<!ATTLIST " << gi << RE;
      GroveChar ch = RE;
      GroveString sep(&ch, 1);
      emit(atts->nodeList(), sep);
      os() << " >" << RE;
    }
     break;
  }
  case ComponentName::idAttributeDef: {
    GroveString name;
    ptr->getName(name);
    os() << "  " << name << " ";
    Node::DeclValueType::Enum decl;
    ptr->getDeclValueType(decl); 
    switch (decl) {
    case Node::DeclValueType::cdata:
      os() << "CDATA ";
      break; 
    case Node::DeclValueType::entity:
      os() << "ENTITY ";
      break; 
    case Node::DeclValueType::entities:
      os() << "ENTITIES ";
      break; 
    case Node::DeclValueType::id:
      os() << "ID ";
      break; 
    case Node::DeclValueType::idref:
      os() << "IDREF ";
      break; 
    case Node::DeclValueType::idrefs:
      os() << "IDREFS ";
      break; 
    case Node::DeclValueType::name:
      os() << "NAME ";
      break; 
    case Node::DeclValueType::names:
      os() << "NAMES ";
      break; 
    case Node::DeclValueType::nmtoken:
      os() << "NMTOKEN ";
      break; 
    case Node::DeclValueType::nmtokens:
      os() << "NMTOKENS ";
      break; 
    case Node::DeclValueType::number:
      os() << "NUMBER ";
      break; 
    case Node::DeclValueType::numbers:
      os() << "NUMBERS ";
      break; 
    case Node::DeclValueType::nutoken:
      os() << "NUTOKEN ";
      break; 
    case Node::DeclValueType::nutokens:
      os() << "NUTOKENS ";
      break; 
    case Node::DeclValueType::notation: 
      os() << "NOTATION ";
      // fall through
    case Node::DeclValueType::nmtkgrp: {
      GroveStringListPtr toks;
      ptr->getTokens(toks);
      emit(toks, "(", "|", ") "); 
      break; 
    } 
    default: 
      CANNOT_HAPPEN();
    }
    Node::DefaultValueType::Enum deflt;
    ptr->getDefaultValueType(deflt);
    switch (deflt) {
    case Node::DefaultValueType::fixed:
      os() << "#FIXED ";
      // fall through
    case Node::DefaultValueType::value: {
      NodeListPtr val;
      GroveString sep;
      if (ptr->getDefaultValue(val) == accessOK)
	emit(val, sep); 
      break; 
    }
    case Node::DefaultValueType::required:
      os() << "#REQUIRED";
      break;
    case Node::DefaultValueType::current:
      os() << "#CURRENT";
      break;
    case Node::DefaultValueType::conref:
      os() << "#CONREF";
      break;
    case Node::DefaultValueType::implied:
      os() << "#IMPLIED";
      break;
    default:
      CANNOT_HAPPEN();
    }
    break;
  }
  case ComponentName::idExternalId: {
    GroveString pubid, sysid;
    if (ptr->getPublicId(pubid) == accessOK) 
      os() << "PUBLIC '" << pubid << "'";
    if (ptr->getSystemId(sysid) == accessOK) 
      os() << (pubid.size() > 0 ? " " : "") << "SYSTEM '" << sysid << "'";
    break;
  }
  case ComponentName::idModelGroup: {
    //FIXME: fails for (#pcdata)
    os() << "(";
    Node::Connector::Enum conn;
    ptr->getConnector(conn); 
    GroveChar ch;
    switch (conn) {
    case Node::Connector::and_: 
      ch = '&';
      break;
    case Node::Connector::or_: 
      ch = '|';
      break;
    case Node::Connector::seq: 
      ch = ',';
      break;
    default:
      CANNOT_HAPPEN();
    }
    GroveString sep(&ch, 1);
    NodeListPtr toks;
    if (ptr->getContentTokens(toks) == accessOK)
      emit(toks, sep);
    os() << ")";
    Node::OccurIndicator::Enum occur;
    if (ptr->getOccurIndicator(occur) == accessOK) {
      switch (occur) {
      case Node::OccurIndicator::opt:
        os() << '?';
        break;
      case Node::OccurIndicator::plus:
        os() << '+';
        break;
      case Node::OccurIndicator::rep:
        os() << '*';
        break;
      default:
        CANNOT_HAPPEN();
      } 
    }
    break;
  }
  case ComponentName::idElementToken: {
    GroveString gi;
    ptr->getGi(gi);
    os() << gi; 
    Node::OccurIndicator::Enum occur;
    if (ptr->getOccurIndicator(occur) == accessOK) {
      switch (occur) {
      case Node::OccurIndicator::opt:
        os() << '?';
        break;
      case Node::OccurIndicator::plus:
        os() << '+';
        break;
      case Node::OccurIndicator::rep:
        os() << '*';
        break;
      default:
        CANNOT_HAPPEN();
      } 
    }
    break;
  }
  case ComponentName::idPcdataToken: {
    os() << "#PCDATA";
    break;
  }
  case ComponentName::idSgmlConstants: 
  case ComponentName::noId: // message and nonSgml 
    break;
  default: 
    CANNOT_HAPPEN(); 
  }
}

void DocumentGenerator::emit(const NodeListPtr &ptr, const GroveString &sep) 
{
   NodeListPtr nl(ptr);
   NodePtr nd;
   for (;;) {
     if (nl->first(nd) != accessOK)
       return;
     emit(nd);
     if (nl->rest(nl) != accessOK)
       return;
     os() << sep;
   }  
} 

#ifdef DSSSL_NAMESPACE
}
#endif
