// Copyright (c) 1997 James Clark
// See the file COPYING for copying permission.

#ifdef __GNUG__
#pragma implementation
#endif

#include "config.h"
#include "ExtendEntityManager.h"
#include "ParserApp.h"
#include "XmlOutputEventHandler.h"
#include "XmlOutputMessages.h"
#include "Message.h"
#include "MessageArg.h"
#include "macros.h"
#if 0
#include "UTF8CodingSystem.h"
#endif
#include "InputSource.h"
#include "StorageManager.h"
#include <string.h>
#include <stdlib.h>

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

#define EXT_ENT_FILE "extEntities.dtf"
#define INT_ENT_FILE "intEntities.dtf"

inline
void operator+=(StringC &str, const char *s)
{
  while (*s)
    str += *s++;
}

inline
void XmlOutputEventHandler::closeCdataSection()
{
  useCdata_ = 0;
  if (inCdata_) {
    os() << "]]>";
    inCdata_ = 0;
    nCdataEndMatched_ = 0;
  }
}

const char RE = '\r';

XmlOutputEventHandler::Options::Options()
{
  memset(this, 0, sizeof(*this));
}

static
void escape(OutputCharStream &s, Char c)
{
  s << "&#" << (unsigned long)c << ";";
}

XmlOutputEventHandler::XmlOutputEventHandler(const Options &options,
					     OutputCharStream *os,
					     const StringC &encodingName,
					     const char *outputDir,
					     const char *dtdLoc,
					     const Ptr<ExtendEntityManager> &entityManager,
					     const CharsetInfo &systemCharset,
					     CmdLineApp *app)
: options_(options),
  os_(os),
  entityManager_(entityManager),
  systemCharset_(&systemCharset),
  app_(app),
  outputDir_(outputDir),
  dtdLoc_(dtdLoc),
  inDtd_(0),
  inCdata_(0),
  useCdata_(0),
  nCdataEndMatched_(0),
  namecaseGeneral_(0),
  extEntFile_(0),
  intEntFile_(0),
  extEnts_(0),
  intEnts_(0),
  nullOut_(0)
{
  // Create output stream for main output
  os_->setEscaper(escape);
  *os_ << "<?xml version=\"1.0\"";
  if (encodingName.size())
    *os_ << " encoding=\"" << encodingName << '"';
  *os_ << "?>" << RE;

  // Set directory for output files
  if (outputDir_ == NULL || strlen(outputDir_) == 0)
    outputDir_ = ".";

  // Open file for writing external entity declarations if we are preserving
  // any entities
  if (! options_.expExt) {
    char filePath[strlen(outputDir_) + 17];
    strcpy (filePath, outputDir_);
    strcat (filePath, "/");
    strcat (filePath, EXT_ENT_FILE);
    extEntFile_ = new FileOutputByteStream;

    // Open the file, exiting if we fail to do so.
    if (!extEntFile_->open(filePath)) {
      app_->message(XmlOutputMessages::cannotOpenOutputFile,
		    StringMessageArg
		    (app_->codingSystem()->convertIn(filePath)));
      exit(1);
    }

    extEnts_ =
      new EncodeOutputCharStream(extEntFile_, app_->outputCodingSystem());
  }

  // Open file for writing external entity declarations if we are preserving
  // any entities
  if (! options_.expInt) {
    char filePath[strlen(outputDir_) + 17];
    strcpy (filePath, outputDir_);
    strcat (filePath, "/");
    strcat (filePath, INT_ENT_FILE);
    intEntFile_ = new FileOutputByteStream;

    // Open the file, exiting if we fail to do so.
    if (!intEntFile_->open(filePath)) {
      app_->message(XmlOutputMessages::cannotOpenOutputFile,
		    StringMessageArg
		    (app_->codingSystem()->convertIn(filePath)));
      exit(1);
    }

    intEnts_ =
      new EncodeOutputCharStream(intEntFile_, app_->outputCodingSystem());

    /* Make a NullOutputByteStream to write to when we want to throw
       things away. This is only used when we are not expanding internal
       entities. */
    NullOutputByteStream *nobs = new NullOutputByteStream;
    const CodingSystem *outputCodingSystem = app_->outputCodingSystem();
    nullOut_ = (OutputCharStream *)
      new EncodeOutputCharStream(nobs, outputCodingSystem);
  }

}

XmlOutputEventHandler::~XmlOutputEventHandler()
{
  os() << RE;
  delete os_;

  if (! options_.expExt)
    delete extEntFile_;

  if (! options_.expExt) {
    delete intEntFile_;
    delete nullOut_;
  }
}

void XmlOutputEventHandler::message(MessageEvent *event)
{
  app_->dispatchMessage(event->message());
  ErrorCountEventHandler::message(event);
}

void XmlOutputEventHandler::sgmlDecl(SgmlDeclEvent *event)
{
  const Syntax &syntax = event->instanceSyntax();
  if (syntax.namecaseGeneral()) {
    namecaseGeneral_ = 1;
    syntax.generalSubstTable()->inverseTable(lowerSubst_);
  }
  delete event;
}

void XmlOutputEventHandler::data(DataEvent *event)
{
  const Entity *entity = event->entity();

  if (! options_.expInt && entity != NULL) {
    Boolean firstSeen = checkFirstSeen(entity->name());

    // output entity reference
    os() << "&" << entity->name() << ";";

    // save old output stream
    outputStack_.insert(os_);

    // output beginning of entity declaration
    if (firstSeen) {
      // Point default output stream (os_) to the entities
      // declaration file.
      os_ = intEnts_;

      os() << "<!ENTITY " << entity->name() << " CDATA \"";
    } // end if firstSeen

    else {
      os_ = nullOut_;
    } // end else (not firstSeen)
  } // end if expanding internal entities

  // Now, no matter what, output the entity's data
  if (useCdata_)
    outputCdata(event->data(), event->dataLength());
  else
    outputData(event->data(), event->dataLength(), 0);

  // If necessary, end entity decl and replace old output stream
  if (! options_.expInt && entity != NULL) {
    os() << "\">" << RE;
    os_->flush();
    os_ = outputStack_.get();
  }

  delete event;
}

void XmlOutputEventHandler::startElement(StartElementEvent *event)
{
  // This shouldn't happen (although useCdata_ can be true).
  if (inCdata_) {
    inCdata_ = 0;
    nCdataEndMatched_ = 0;
    os() << "]]>";
  }
  os() << '<' << generalName(event->name(), nameBuf_);
  size_t nAttributes = event->attributes().size();
  for (size_t i = 0; i < nAttributes; i++)
    outputAttribute(event->attributes(), i);
  if (options_.nlInTag)
    os() << RE;
  if (options_.cdata
      && event->elementType()->definition()->declaredContent() == ElementDefinition::cdata)
    useCdata_ = 1;
  if (options_.empty
      && event->elementType()->definition()->declaredContent() == ElementDefinition::empty)
    os() << "/>";
  else
    os() << '>';
  delete event;
}

void XmlOutputEventHandler::outputAttribute(const AttributeList &attributes, size_t i)
{
  const AttributeValue *value = attributes.value(i);
  if (!value)
    return;
  const Text *text;
  const StringC *string;
  AttributeValue::Type type = value->info(text, string);
  if (type == AttributeValue::implied)
    return;
  if (options_.nlInTag)
    os() << RE;
  else
    os() << ' ';
  os() << generalName(attributes.name(i), nameBuf_) << "=\"";
  if (type == AttributeValue::cdata) {
    TextIter iter(*text);
    TextItem::Type type;
    const Char *p;
    size_t length;
    const Location *loc;
    while (iter.next(type, p, length, loc)) {
      switch (type) {
      case TextItem::data:
      case TextItem::cdata:
	outputData(p, length, 1);
	break;
      case TextItem::sdata:
	{
	  app_->setNextLocation(loc->origin()->parent());
	  const Entity *entity = loc->origin()->asEntityOrigin()->entity();

	  if (! options_.expInt) {
	    Boolean firstSeen = checkFirstSeen(entity->name());

	    // output entity reference
	    os() << "&" << entity->name() << ";";

	    // save old output stream
	    outputStack_.insert(os_);

	    if (firstSeen) {
	      // Point default output stream (os_) to the entities
	      // declaration file and output start of declaration
	      os_ = intEnts_;

	      os() << "<!ENTITY " << entity->name()
		   << "  \"<?sdataEntity " << entity->name() << " ";
	    } else { // we've seen it before; throw away expansion data
	      os_ = nullOut_;
	    }
	  }

	  // We are expanding internal entities; expand this one as a PI,
	  // since XML does not have SDATA entities
	  else {
	    os() << "<?sdataEntity " << entity->name() << " ";
	  }

	  // Now, no matter what, output the entity's data
	  outputData(p, length, 1);

	  // If necessary, end entity decl and replace old output stream
	  if (! options_.expInt) {
	    os() << " ?>\">" << RE;
	    os_->flush();
	    os_ = outputStack_.get();
	  } else {
	    os() << " ?>";
	  }
	}
	break;
      case TextItem::nonSgml:
	// FIXME
  	break;
      default:
	break;
      }
    }
  }
  else if (attributes.def()->def(i)->isEntity())
    os() << *string;
  else
    os() << generalName(*string, nameBuf_);
  os() << '"';
}

void XmlOutputEventHandler::endElement(EndElementEvent *event)
{
  closeCdataSection();
  if (options_.empty
      && event->elementType()->definition()->declaredContent() == ElementDefinition::empty)
    ;
  else {
    os() << "</" << generalName(event->name(), nameBuf_);
#if 0
    if (options_.nlInTag)
      os() << RE;
#endif
    os() << '>';
  }
  delete event;
}

static Boolean isXmlS(Char c)
{
  switch (c) {
  case ' ':
  case '\r':
  case '\n':
  case '\t':
    return 1;
  }
  return 0;
}

static Boolean containsQuestionLt(const Char *s, size_t n)
{
  for (; n > 1; n--, s++)
    if (*s == '?' && s[1] == '>')
      return 1;
  return 0;
}

// These do not handle Unicode chars properly.
// They treat all Unicode chars >= 170 as name start characters

static Boolean isXmlNameStartChar(Char c)
{
  switch (c) {
  case ':':
  case '.':
  case '_':
  case '-':
    return 1;
  }
  if (c >= 170)
    return 1;
  if ('a' <= c && c <= 'z')
    return 1;
  if ('A' <= c && c <= 'Z')
    return 1;
  return 0;
}

static Boolean isXmlNameChar(Char c)
{
  if (isXmlNameStartChar(c))
    return 1;
  if ('0' <= c && c <= '9')
    return 1;
  return 0;
}

static Boolean startsWithXmlName(const Char *s, size_t n)
{
  if (n == 0)
    return 0;
  if (!isXmlNameStartChar(*s))
    return 0;
  for (s++, n--; n > 0; n--, s++) {
    if (isXmlS(*s))
      return 1;
    if (!isXmlNameChar(*s))
      return 0;
  }
  return 1;
}

void XmlOutputEventHandler::pi(PiEvent *event)
{
  const Char *s = event->data();
  size_t n = event->dataLength();

  if (n >= 3 && s[0] == 'x' && s[1] == 'm' && s[2] == 'l'
           && (n == 3 || isXmlS(s[3])))
    ; // Probably came from an encoding PI.
  else if (!startsWithXmlName(s, n)) {
    app_->setNextLocation(event->location());
    app_->message(XmlOutputMessages::piNoName);
  }
  else if (options_.piEscape) {
    os() << "<?";
    outputData(s, n, 0);
    os() << "?>";
  }
  else if (containsQuestionLt(s, n)) {
    app_->setNextLocation(event->location());
    app_->message(XmlOutputMessages::piQuestionLt);
  }
  else {
    os() << "<?";
    os().write(s, n);
    os() << "?>";
  }
  delete event;
}

void XmlOutputEventHandler::sdataEntity(SdataEntityEvent *event)
{
  const Entity *entity = event->entity();
  app_->setNextLocation(event->location().origin()->parent());

  if (! options_.expInt) {
    Boolean firstSeen = checkFirstSeen(entity->name());

    // output entity reference
    os() << "&" << entity->name() << ";";

    // save old output stream
    outputStack_.insert(os_);

    if (firstSeen) {
      // Point default output stream (os_) to the entities
      // declaration file and output start of declaration
      os_ = intEnts_;

      os() << "<!ENTITY " << entity->name()
	   << "  \"<?sdataEntity " << entity->name() << " ";
    } else { // we've seen it before; throw away expansion data
      os_ = nullOut_;
    }
  }

  // We are expanding internal entities; expand this one as a PI,
  // since XML does not have SDATA entities
  else {
    os() << "<?sdataEntity " << entity->name() << " ";
  }

  // Now, no matter what, output the entity's data
  outputData(event->data(), event->dataLength(), 0);

  // If necessary, end entity decl and replace old output stream
  if (! options_.expInt) {
    os() << " ?>\">" << RE;
    os_->flush();
    os_ = outputStack_.get();
  } else {
    os() << " ?>";
  }

  delete event;
}

/** External data entities may be referenced in attributes only. If
    one is referenced in content, error and exit. */
void XmlOutputEventHandler::externalDataEntity(ExternalDataEntityEvent *event)
{
  app_->message(XmlOutputMessages::externalDataEntityReference,
                StringMessageArg(event->entity()->name()));
  exit (1);
}

void XmlOutputEventHandler::subdocEntity(SubdocEntityEvent *event)
{
  const SubdocEntity *entity = event->entity();
  app_->setNextLocation(event->location().origin()->parent());

  if (options_.expExt) {
    SgmlParser::Params params;
    params.subdocInheritActiveLinkTypes = 1;
    params.subdocReferenced = 1;
    params.origin = event->entityOrigin()->copy();
    params.parent = & ((ParserApp *)app_)->parser();
    params.sysid = entity->externalId().effectiveSystemId();
    params.entityType = SgmlParser::Params::subdoc;
    SgmlParser parser(params);
    parser.parseAll(*this);
  } else {
    os() << "&" << entity->name() << ";";
  }

  delete event;
}

void XmlOutputEventHandler::startDtd(StartDtdEvent *event)
{
  inDtd_ = 1;
  delete event;
}

void XmlOutputEventHandler::endDtd(EndDtdEvent *event)
{
  inDtd_ = 0;
  delete event;
}

void XmlOutputEventHandler::maybeStartDoctype(Boolean &doctypeStarted, const Dtd &dtd)
{
  if (doctypeStarted)
    return;
  doctypeStarted = 1;
  const StringC &name = dtd.documentElementType()->name();
  StringC buf;

  // if appropriate, lowercase the doctype name -jphekman
  if (options_.lower) {
    for (size_t i = 0; i < name.size(); i++) {
      Char c = lowerSubst_[name[i]];
      if (c != name[i]) {
	buf = name;
	buf[i] = c;
	for (i++; i < name.size(); i++)
	  lowerSubst_.subst(buf[i]);
      }
    }
  } else {
    buf = name;
  }

  /* Output the doctype declaration. If requested, specify a local
     file containing the DTD. */
  os() << "<!DOCTYPE " << buf;
  if (dtdLoc_ != NULL) {
    os() << " SYSTEM \"" << dtdLoc_ << "\"";
  }
  os() << " [" << RE;

  /* If requested, include pointers in the instance's internal subset
     to driver files which define internal/external entities. */
  if (! options_.expExt && options_.extDecl) {
    os() << "<!ENTITY % external-entities SYSTEM \""
	 << EXT_ENT_FILE << "\">"
	 << RE << "%external-entities;" << RE;
  }

  if (! options_.expInt && options_.intDecl) {
    os() << "<!ENTITY % internal-entities SYSTEM \""
	 << INT_ENT_FILE << "\">"
	 << RE << "%internal-entities;" << RE;
  }
}

void XmlOutputEventHandler::endProlog(EndPrologEvent *event)
{
  const Dtd &dtd = event->dtd();
  Boolean doctypeStarted = 0;
  if (options_.notation) {
    Dtd::ConstNotationIter iter(dtd.notationIter());
    for (;;) {
      const Notation *notation = iter.nextTemp();
      if (!notation)
	break;
      maybeStartDoctype(doctypeStarted, dtd);
      os() << "<!NOTATION " << generalName(notation->name(), nameBuf_);
      outputExternalId(*notation);
      os() << ">" << RE;
      if (notation->attributeDefTemp()) {
	app_->setNextLocation(notation->defLocation());
	app_->message(XmlOutputMessages::notationAttributes,
	              StringMessageArg(notation->name()));
      }
    }
  }
  if (options_.ndata) {
    Dtd::ConstEntityIter iter(dtd.generalEntityIter());
    for (;;) {
      const Entity *entity = iter.nextTemp();
      if (!entity)
	break;
      const ExternalDataEntity *extDataEntity = entity->asExternalDataEntity();
      if (extDataEntity) {
	maybeStartDoctype(doctypeStarted, dtd);
	os() << "<!ENTITY " << entity->name();
	outputExternalId(*entity);
	if (extDataEntity->dataType() != EntityDecl::ndata) {
	  app_->setNextLocation(entity->defLocation());
	  app_->message(XmlOutputMessages::externalDataNdata,
	                StringMessageArg(entity->name()));
	}
	os() << " NDATA " << generalName(extDataEntity->notation()->name(), nameBuf_) << ">" << RE;
      }	
    }
  }
  if (options_.id || options_.attlist) {
    Dtd::ConstElementTypeIter iter(dtd.elementTypeIter());
    for (;;) {
      const ElementType *elementType = iter.next();
      if (!elementType)
	break;
      const AttributeDefinitionList *adl = elementType->attributeDefTemp();
      if (adl) {
	if (options_.attlist) {
	    maybeStartDoctype(doctypeStarted, dtd);
	    os() << "<!ATTLIST " << generalName(elementType->name(), nameBuf_);
	    for (size_t i = 0; i < adl->size(); i++) {
	      const AttributeDefinition *def = adl->def(i);
	      os() << RE << generalName(def->name(), nameBuf_);
	      AttributeDefinitionDesc desc;
	      def->getDesc(desc);
	      switch (desc.declaredValue) {
	      case AttributeDefinitionDesc::cdata:
		os() << " CDATA #IMPLIED";
		break;
	      case AttributeDefinitionDesc::name:
	      case AttributeDefinitionDesc::number:
	      case AttributeDefinitionDesc::nmtoken:
	      case AttributeDefinitionDesc::nutoken:
		os() << " NMTOKEN #IMPLIED";
		break;
	      case AttributeDefinitionDesc::entity:
		os() << " ENTITY #IMPLIED";
		break;
	      case AttributeDefinitionDesc::idref:
		os() << " IDREF #IMPLIED";
		break;
	      case AttributeDefinitionDesc::names:
	      case AttributeDefinitionDesc::numbers:
	      case AttributeDefinitionDesc::nmtokens:
	      case AttributeDefinitionDesc::nutokens:
		os() << " NMTOKENS #IMPLIED";
		break;
	      case AttributeDefinitionDesc::entities:
		os() << " ENTITIES #IMPLIED";
		break;
	      case AttributeDefinitionDesc::idrefs:
		os() << " IDREFS #IMPLIED";
		break;
	      case AttributeDefinitionDesc::id:
		os() << " ID #IMPLIED";
		break;
	      case AttributeDefinitionDesc::notation:
		os() << " NOTATION";
		// fall through
	      case AttributeDefinitionDesc::nameTokenGroup:
		{
		  os() << " (";
		  for (size_t j = 0; j < desc.allowedValues.size(); j++) {
		    if (j > 0)
		      os() << '|';
		    os() << desc.allowedValues[j];
		  }
		  os() << ") #IMPLIED";
		}
		break;
	      default:
		CANNOT_HAPPEN();
	      }
	    }
	    os() << '>' << RE;
	}
	else {
	  size_t idIndex = adl->idIndex();
	  if (idIndex != size_t(-1)) {
	    maybeStartDoctype(doctypeStarted, dtd);
	    os() << "<!ATTLIST " << generalName(elementType->name(), nameBuf_);
	    os() << ' ' << generalName(adl->def(idIndex)->name(), nameBuf_)
	        << " ID #IMPLIED>" << RE;
	  }
	}
      }
    }
  }
  if (doctypeStarted)
    os() << "]>" << RE;
  delete event;
}

void XmlOutputEventHandler::outputExternalId(const EntityDecl &decl)

{
  const StringC *pubIdP = decl.publicIdPointer();
  const StringC *sysIdP = decl.effectiveSystemIdPointer();
  if (pubIdP) {
    os() << " PUBLIC \"" << *pubIdP << "\"";
    if (decl.declType() == EntityDecl::notation && !sysIdP)
      return;
    os() << " \"";
  }
  else
    os() << " SYSTEM \"";
 if (sysIdP) {
   StringC url;
   switch(fsiToUrl(*sysIdP, decl.defLocation(), url)) {
   case 1:
     os() << url;
     break;
   case 0:
     break;
   default:
     app_->setNextLocation(decl.defLocation());
     app_->message(XmlOutputMessages::cannotConvertFsiToUrl,
                   StringMessageArg(*sysIdP));
     break;
   }
 }
 os() << "\"";
}

// Return 1 if OK; return -1 to generate generic error; return 0 if error already generated.

int XmlOutputEventHandler::fsiToUrl(const StringC &fsi, const Location &loc, StringC &url)
{
  ParsedSystemId parsedBuf;
  if (!entityManager_->parseSystemId(fsi, *systemCharset_, 0, 0, *app_, parsedBuf))
    return 0;
  if (parsedBuf.size() != 1)
    return -1;
  if (strcmp(parsedBuf[0].storageManager->type(), "URL") == 0) {
    url = parsedBuf[0].specId;
    return 1;
  }
  Owner<InputSource> in(entityManager_->open(fsi,
					     *systemCharset_,
					     InputSourceOrigin::make(),
					     0,
					     *app_));
  if (!in)
    return 0;
  Xchar c = in->get(*app_);
  StorageObjectLocation soLoc;
  if (c == InputSource::eE && in->accessError()) {
    if (parsedBuf[0].baseId.size())
      return 0;
    soLoc.storageObjectSpec = &parsedBuf[0];
    soLoc.actualStorageId = parsedBuf[0].specId;
  }
  else {
    const Location &loc = in->currentLocation();
    if (loc.origin().isNull())
      return -1;
    const InputSourceOrigin *tem = loc.origin()->asInputSourceOrigin();
    if (!tem)
      return -1;
    const ParsedSystemId *psi
      = ExtendEntityManager::externalInfoParsedSystemId(tem->externalInfo());
    if (!psi || psi->size() != 1)
      return -1;
    if (!ExtendEntityManager::externalize(tem->externalInfo(), 0, soLoc))
      return -1;
  }
  if (strcmp(soLoc.storageObjectSpec->storageManager->type(), "OSFILE") != 0)
    return -1;
  return filenameToUrl(soLoc.actualStorageId, loc, url);
}

static
StringC hexEncode(unsigned char c)
{
  static const char hexDigits[] = "0123456789ABCDEF";
  StringC result;
  result.resize(2);
  result[0] = hexDigits[c >> 4];
  result[1] = hexDigits[c & 0xF];
  return result;
}

static
StringC urlEncode(Char c)
{
  if (c >= 0x80) {
    StringC tem;
    tem += c;
#if 0
    // This is what the URN spec says.
    UTF8CodingSystem utf8;
    String<char> bytes(utf8.convertOut(tem));
    tem.resize(0);
    for (size_t i = 0; i < bytes.size(); i++)
      tem += hexEncode(bytes[i]);
#endif
    return tem;
  }
  if (strchr("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ$-_.+!*'(),",
             char(c))) {
    StringC tem;
    tem += c;
    return tem;
  }
  return hexEncode((unsigned char)c);
}

int XmlOutputEventHandler::filenameToUrl(const StringC &filename,
					 const Location &loc,
					 StringC &url)
{
#ifdef SP_MSDOS_FILENAMES
  if (filename.size() >= 3 && filename[1] == ':'
      && (('a' <= filename[0] && filename[0] <= 'z')
	  || ('A' <= filename[0] && filename[0] <= 'Z'))
      && (filename[2] == '/' || filename[2] == '\\')) {
    url += "file://";
    url += filename;
    return 1;
  }
#endif
  for (size_t i = 0; i < filename.size(); i++) {
    if (filename[i] == '/'
#ifdef SP_MSDOS_FILENAMES
        || filename[i] == '\\'
#endif
	) {
      url += '/';
    }
    else
      url += urlEncode(filename[i]);
  }
  return 1;
}

void XmlOutputEventHandler::commentDecl(CommentDeclEvent *event)
{
  if (!inDtd_ && options_.comment) {
    Boolean started = 0;
    for (MarkupIter iter(event->markup()); iter.valid(); iter.advance()) {
      if (iter.type() == Markup::comment) {
	if (!started) {
	  os() << "<!--";
	  started = 1;
	}
	os().write(iter.charsPointer(), iter.charsLength());
      }
    }
    if (started)
      os() << "-->";
  }
  delete event;
}

void XmlOutputEventHandler::markedSectionStart(MarkedSectionStartEvent *event)
{
  if (options_.cdata && event->status() == MarkedSectionEvent::cdata)
    useCdata_ = 1;
  delete event;
}

void XmlOutputEventHandler::markedSectionEnd(MarkedSectionEndEvent *event)
{
  if (event->status() == MarkedSectionEvent::cdata)
    closeCdataSection();
  delete event;
}

void XmlOutputEventHandler::outputCdata(const Char *s, size_t n)
{
  static const char cdataEnd[] = "]]>";
  for (; n > 0; s++, n--) {
    Char c = *s;
    if (!inCdata_) {
      os() << "<![CDATA[";
      inCdata_ = 1;
    }
    if (c == cdataEnd[nCdataEndMatched_]) {
      if (nCdataEndMatched_ == 2) {
	os() << "]]><![CDATA[";
	nCdataEndMatched_ = 0;
      }
      else
	nCdataEndMatched_++;
    }
    else
      nCdataEndMatched_ = 0;
    os().put(c);
  }
}

void XmlOutputEventHandler::outputData(const Char *s, size_t n, Boolean inLit)
{
  for (; n > 0; s++, n--) {
    Char c = *s;
    switch (c) {
    case '&':
      os() << "&amp;";
      break;
    case '<':
      os() << "&lt;";
      break;
    case '>':
      os() << "&gt;";
      break;
    case 9:
    case 10:
    case 13:
      if (inLit)
	os() << "&#" << int(c) << ';';
      else
	os().put(c);
      break;
    case '"':
      if (inLit) {
	os() << "&quot;";
	break;
      }
      // fall through
    default:
      os().put(c);
      break;
    }
  }
}

void XmlOutputEventHandler::entityDefaulted(EntityDefaultedEvent *event)
{
  if (options_.reportEnts) {
    ConstPtr<Entity> entity = event->entityPointer();
    const ExternalEntity *extEntity = entity->asExternalEntity();

    // If we are dealing with an external entity (else it will be null)
    // -jphekman
    if (extEntity != 0) {
      const StringC *systemIdPointer = extEntity->systemIdPointer();
      if (systemIdPointer != 0) {
	os () << "<?entityResolved " << *systemIdPointer << " ?>";
      }
    }
  }
}

void XmlOutputEventHandler::inputOpened(InputSource *in)
{
  if (!inDtd_) {
    const CodingSystem *outputCodingSystem = app_->outputCodingSystem();
    const EntityDecl *entDecl = in->currentLocation().origin()->entityDecl();
    const Entity *ent = in->currentLocation().origin()->entity();
    const StringC *effectiveSystemIdPointer =
      entDecl->effectiveSystemIdPointer();
    const StringC *systemIdPointer = entDecl->systemIdPointer();
    const StringC *publicIdPointer = entDecl->publicIdPointer();

    if (options_.reportIS && ent->asExternalEntity() != NULL) {
      os() << "<?inputOpened effectiveSystemID=\"";

      if (effectiveSystemIdPointer == 0)
	os() << "NULL";
      else
	os () << *effectiveSystemIdPointer;

      os() << "\" systemID=\"";
      if (systemIdPointer == 0)
	os() << "NULL";
     else
	os () << *systemIdPointer;

      os() << "\" publicID=\"";
      if (publicIdPointer == 0)
	os() << "NULL";
      else
	os () << *publicIdPointer;

      os() << "\" ?>";
    }

    /* Output entity declaration and, in the case of external
       entities, write file containing entity replacement text. */

    Boolean firstSeen = checkFirstSeen(ent->name());

    if (ent->asExternalEntity() != NULL) {
      if (! options_.expExt) {
	// output entity reference
	os() << "&" << entDecl->name() << ";";

	// output entity declaration
	if (systemIdPointer == 0) {
	  app_->message(XmlOutputMessages::missingSystemId,
			StringMessageArg(entDecl->name()));
	  exit (1);
	}

	// save old output stream
	outputStack_.insert(os_);

	if (firstSeen) {
	  *extEnts_ << "<!ENTITY " << entDecl->name() << " SYSTEM \""
		    << *systemIdPointer << "\">\n";
	  extEnts_->flush();

	  /* Determine name of file. Allocate space for the length of the
	     output directory's string; the "/"; the file name's string;
	     and ".xml" */

	  char filePath[strlen(outputDir_) + 5 +
		       outputCodingSystem->convertOut(*systemIdPointer).size()];
	  strcpy (filePath, outputDir_);
	  strcat (filePath, "/");
	  strcat(filePath,
		 outputCodingSystem->convertOut(*systemIdPointer).data());
	  // Set the suffix to ".xml"
	  convertSuffix(filePath);

	  // Open the file, exiting if we fail to do so.
	  FileOutputByteStream *file = new FileOutputByteStream;
	  outputFileStack_.insert(file);
	  if (!file->open(filePath)) {
	    app_->message(XmlOutputMessages::cannotOpenOutputFile,
			  StringMessageArg
			  (app_->codingSystem()->convertIn(filePath)));
	    exit (1);
	  }

	  // Create output stream to file and set os_ to it.
	  os_ = (OutputCharStream *)
	    new EncodeOutputCharStream(file, outputCodingSystem);
	} // end if firstSeen
	else {
	  // push null os onto file output stack, set os_ to it
	  NullOutputByteStream *nobs = new NullOutputByteStream;
	  outputFileStack_.insert(nobs);

	  // Create output stream to file and set os_ to it.
	  os_ = (OutputCharStream *)
	    new EncodeOutputCharStream(nobs, outputCodingSystem);
	} // end else (notfirst Seen)

      } // end if not expanding external entities

    } // end if asExternalEntity()

    else if (ent->asInternalEntity() != NULL) {
      if (! options_.expInt) {

	// output entity reference
	os() << "&" << entDecl->name() << ";";

	// save old output stream
	outputStack_.insert(os_);

	// output beginning of entity declaration
	if (firstSeen) {
	  // Point default output stream (os_) to the entities
	  // declaration file and output declaration start
	  os_ = intEnts_;

	  os() << "<!ENTITY " << entDecl->name() << " \"";
	} // end if firstSeen

	else {
	  os_ = nullOut_;
	} // end else (not firstSeen)
      } // end if expanding internal entities
    } // end if this is an internal entity

    else {
      // We should only get InternalText and ExternalText entities here.
      app_->message(XmlOutputMessages::unexpectedEntityType,
		    StringMessageArg (ent->name()));
      exit(1);
    }
  }
}

void XmlOutputEventHandler::inputClosed(InputSource *in)
  {
  if (! inDtd_) {
    const Entity *ent = in->currentLocation().origin()->entity();

    // Close external entity
    if (ent->asExternalEntity() != NULL) {
      if (! options_.expExt) {
	// delete current output stream
	os_->flush();
	delete os_;

	// restore previous output stream
	os_ = outputStack_.get();

	// close file
	OutputByteStream *file = outputFileStack_.get();
	delete file;
      }
    }

    // Close internal entity
    else if (ent->asInternalEntity() != NULL) {
      if (! options_.expInt) {
	// Write end of declaration to output stream
	os() << "\">\n";

	/* Do not delete current output stream: it is to a DTD stub
	 * file which we will be reusing, or possible a null output
	 byte stream which we will also be reusing. */
	os_->flush();

	// restore previous output stream
	os_ = outputStack_.get();
      }
    }

    else {
      // We should only get InternalText and ExternalText entities here.
      app_->message(XmlOutputMessages::unexpectedEntityType,
		    StringMessageArg (ent->name()));
      exit(1);
    }

    if (options_.reportIS && ent->asExternalEntity() != NULL) {
      os() << "<?inputClosed ?>";
    }
  }
}

const StringC &XmlOutputEventHandler::generalName(const StringC &name,
					          StringC &buf)
{
  if (options_.lower && namecaseGeneral_) {
    for (size_t i = 0; i < name.size(); i++) {
      Char c = lowerSubst_[name[i]];
      if (c != name[i]) {
	buf = name;
	buf[i] = c;
	for (i++; i < name.size(); i++)
	  lowerSubst_.subst(buf[i]);
	return buf;
      }
    }
  }
  return name;
}

/** Make this string's suffix ".xml", attempting to do the right thing
    if we encounter ".sgm", ".sgml", or ".xml" as the original
    suffix. */
char *XmlOutputEventHandler::convertSuffix(char *name)
{
  // Get a pointer to the last occurrence of ".":
  char *suffix = strrchr(name, '.');

  // If there is no "." in "name", append ".xml" and return
  if (suffix == NULL) {
    strcat(name, ".xml");
    return name;
  }

  // Suffix is "sgm[l]": subsitite ".xml"
  if (strcmp(suffix, ".sgm") == 0 || strcmp (suffix, ".sgml") == 0) {
    strcpy (suffix, ".xml");
    return name;
  }

  // "xml": do nothing
  if (strcmp(suffix, ".xml") == 0) {
    return name;
  }

  // default: append ".xml"
  strcat (name, ".xml");
  return name;
}

/** If this is the first time we have encountered entity "name",
    return true and add it to a list of entities we've seen. Else,
    return false. */
Boolean XmlOutputEventHandler::checkFirstSeen(const StringC &name)
{
  Named *id = entTable_.lookup(name);

  if (!id) {
    entTable_.insert(new Named(name));
    return true;
  }

  return false;
}


/**
 * NullOutputByteStream
 */

NullOutputByteStream::NullOutputByteStream()
{
}

NullOutputByteStream::~NullOutputByteStream()
{
}

void NullOutputByteStream::flush()
{
}

void NullOutputByteStream::sputc(char c)
{
}

void NullOutputByteStream::sputn(const char *, size_t)
{
}

OutputByteStream &NullOutputByteStream::operator<<(char)
{
  return *this;
}

OutputByteStream &NullOutputByteStream::operator<<(unsigned char)
{
  return *this;
}

OutputByteStream &NullOutputByteStream::operator<<(const char *)
{
  return *this;
}

OutputByteStream &NullOutputByteStream::operator<<(int)
{
  return *this;
}

OutputByteStream &NullOutputByteStream::operator<<(unsigned)
{
  return *this;
}

OutputByteStream &NullOutputByteStream::operator<<(long)
{
  return *this;
}

OutputByteStream &NullOutputByteStream::operator<<(unsigned long)
{
  return *this;
}

OutputByteStream &NullOutputByteStream::operator<<(const String<char> &)
{
  return *this;
}

/* Note: Returning NULL is probably not the best solution here, but as
 * nothing actually uses getBufferPtr(), it is hard to see what sort
 * of no-op behavior would actually be appropriate. */
char *NullOutputByteStream::getBufferPtr() const
{
  return NULL;
}

size_t NullOutputByteStream::getBufferSize() const
{
  return 0;
}

void NullOutputByteStream::usedBuffer(size_t)
{
}

void NullOutputByteStream::flushBuf(char)
{
}

#ifdef SP_NAMESPACE
}
#endif
