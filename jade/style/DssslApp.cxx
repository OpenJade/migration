// Copyright (c) 1996, 1997 James Clark
// See the file copying.txt for copying permission.

#include "stylelib.h"
#include "DssslApp.h"
#include <OpenSP/StorageManager.h>
#include "DssslAppMessages.h"
#include "StyleEngine.h"
#include <OpenSP/sptchar.h>
#include <OpenSP/macros.h>
#include "LocNode.h"
#include "SdNode.h"
#include <OpenSP/InputSource.h>
#include <OpenSP/ArcEngine.h>
#include <OpenSP/Entity.h>
#include <OpenSP/MessageTable.h>

#include <ctype.h>
#include <string.h>

#ifdef SP_HAVE_LOCALE
#include <locale.h>
#endif

#ifndef OPENJADE_MESSAGE_DOMAIN
#define OPENJADE_MESSAGE_DOMAIN ""
#endif /* not OPENJADE_MESSAGE_DOMAIN */

#ifndef OPENJADE_LOCALE_DIR
#define OPENJADE_LOCALE_DIR ""
#endif /* not OPENJADE_LOCALE_DIR */


#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

/* shift a-z far enough to be outside C locale alphanumeric */
const char docOffset = 40;
const char specOffset = 80;
 
DssslApp::DssslApp(int unitsPerInch)
: GroveApp("unicode"), unitsPerInch_(unitsPerInch),
  dssslSpecOption_(0), debugMode_(0), dsssl2_(0),
  strictMode_(0),
  specTitleOption_(0), fotbDescr_(0),
  fotb_(0), se_(0)
{
  registerOption('2', 0, DssslAppMessages::help2);
  registerOption('G', SP_T("debug"), DssslAppMessages::GHelp);
  registerOption('d', SP_T("specification"), DssslAppMessages::sysid,
                 DssslAppMessages::dHelp);
  registerOption('V', SP_T("define"), DssslAppMessages::vardef,
                 DssslAppMessages::VHelp);
  registerOption('s', SP_T("strict"), DssslAppMessages::sHelp);
  registerOption('T', SP_T("spec-title"), DssslAppMessages::name, 
                 DssslAppMessages::THelp);
  registerOption('e' + docOffset, SP_T("doc-open-entities"), 
                 DssslAppMessages::eHelp);
  registerOption('g' + docOffset, SP_T("doc-open-elements"), 
                 DssslAppMessages::gHelp);
  registerOption('n' + docOffset, SP_T("doc-error-numbers"), 
                 DssslAppMessages::nHelp);
  registerOption('x' + docOffset, SP_T("doc-references"), 
                 DssslAppMessages::xHelp);
  registerOption('i' + docOffset, SP_T("doc-include"), DssslAppMessages::name,
                 DssslAppMessages::iHelp);
  registerOption('w' + docOffset, SP_T("doc-warning"), DssslAppMessages::type,
                 DssslAppMessages::wHelp);
  registerOption('e' + specOffset, SP_T("spec-open-entities"), 
                 DssslAppMessages::eHelp); 
  registerOption('g' + specOffset, SP_T("spec-open-elements"), 
                 DssslAppMessages::gHelp);
  registerOption('n' + specOffset, SP_T("spec-error-numbers"), 
                 DssslAppMessages::nHelp);
  registerOption('x' + specOffset, SP_T("spec-references"), 
                 DssslAppMessages::xHelp);
  registerOption('i' + specOffset, SP_T("spec-include"), DssslAppMessages::name,
                 DssslAppMessages::iHelp);
  registerOption('w' + specOffset, SP_T("spec-warning"), DssslAppMessages::type,
                 DssslAppMessages::wHelp);
  registerInfo(DssslAppMessages::prefixInfo, 1);
}

int DssslApp::init(int argc, AppChar **argv)
{
  int ret = GroveApp::init(argc, argv);
#ifdef SP_HAVE_LOCALE
  // Since we use strtod(), must have C numeric
  setlocale(LC_NUMERIC, "C");
#endif
  MessageTable::instance()->registerMessageDomain(jstyleModule,
                                                  OPENJADE_MESSAGE_DOMAIN,
						  OPENJADE_LOCALE_DIR);
  return ret;
}

int DssslApp::processSysid(const StringC &sysid)
{
  rootSystemId_ = sysid;
  ParsedSystemId v;
  if (!entityManager()->parseSystemId(sysid, systemCharset(), 0, 0,
				      *this, v))
    return 0;
  for (size_t i = v.size(); i > 0; i--)
    if (v[i - 1].storageManager->inheritable()) {
      ParsedSystemId specId;
      specId.resize(1);
      StorageObjectSpec &spec = specId[0];
      spec = v[i - 1];
      StringC &s = spec.specId;
      // replace an up to 5 character extension with .dsl
      for (size_t j = 0; j < 5; j++) {
	if (s.size() < j + 1)
	  break;
	if (s[s.size() - j - 1] == '.') {
	  s.resize(s.size() - j - 1);
	  break;
	}
      }
      if (strcmp(v[i - 1].storageManager->type(), "OSFILE") == 0)
	defaultOutputBasename_ = s;
      if (!dssslSpecOption_) {
	static const Char ext[] = { '.', 'd', 's', 'l' };
        s.append(ext, SIZEOF(ext));
	specId.unparse(systemCharset(), 0, dssslSpecSysid_);
      }
      break;
    }

  int ret = 1;
  fotb_ = makeFOTBuilder(fotbDescr_);
  if (fotb_) {
    if (initSpecParser()) {
      se_ = new StyleEngine(*this, *this, unitsPerInch_, debugMode_, 
  			    dsssl2_, strictMode_, *fotbDescr_, 
			    outputCodingSystem());  
      se_->parseSpec(specParser_, systemCharset(), dssslSpecId_, *this, 
                     defineVars_);
      ret = GroveApp::processSysid(sysid);
      delete se_;
    }
    delete fotb_; 
  }
  return ret;
}

void DssslApp::processOption(AppChar opt, const AppChar *arg)
{
  switch (opt) {
  case AppChar('e' + docOffset):
  case AppChar('g' + docOffset):
  case AppChar('n' + docOffset):
  case AppChar('x' + docOffset):
  case AppChar('i' + docOffset):
  case AppChar('w' + docOffset):
    GroveApp::processOption(opt - docOffset, arg);
    break;
  case AppChar('e' + specOffset):
  case AppChar('g' + specOffset):
  case AppChar('n' + specOffset):
  case AppChar('x' + specOffset):
  case AppChar('i' + specOffset):
  case AppChar('w' + specOffset):
    docOptions_ = options_;
    options_ = specOptions_;
    GroveApp::processOption(opt - specOffset, arg);
    specOptions_ = options_;
    options_ = docOptions_;
    break;
  case 'e':
  case 'g':
  case 'n':
  case 'x':
  case 'i':
  case 'w':
    processOption(opt + docOffset, arg);
    processOption(opt + specOffset, arg);
    break;
  case 'G':
    debugMode_ = 1;
    break;
  case '2':
    dsssl2_ = 1;
    break;
  case 'd':
    dssslSpecId_.resize(0);
    dssslSpecSysid_ = convertInput(arg);
    dssslSpecOption_ = 1;
    splitOffId(dssslSpecSysid_, dssslSpecId_);
    break;
  case 'T':
    specTitleOption_ = 1;
    specTitle_ = convertInput(arg);
    break;
  case 'V':
    defineVars_.push_back(convertInput(arg));
    break;
  case 's':
    strictMode_ = 1;
    break;
  case 'v':
    message(DssslAppMessages::versionInfo,
	    StringMessageArg(convertInput(SP_T(OPENJADE_PACKAGE))),
	    StringMessageArg(convertInput(SP_T(OPENJADE_VERSION))));
    // fall through
  default:
    GroveApp::processOption(opt, arg);
  }
}

void DssslApp::splitOffId(StringC &sysid, StringC &id)
{
  id.resize(0);
  for (size_t i = sysid.size(); i > 0; i--) {
    if (sysid[i - 1] == '#') {
      id.assign(sysid.data() + i,
		sysid.size() - i);
      sysid.resize(i - 1);
      break;
    }
  }
}

int DssslApp::generateEvents(ErrorCountEventHandler *eceh)
{
  groveTable_.insert(rootSystemId_, rootNode_);
  // Since the thread parsing the DSSSL spec is a different thread
  // from the thread parsing the document, we can't share an
  // entity manager.
  // The document parser has already been inited and so will
  // use the current entity manager.
  // The spec parser hasn't yet been inited and so will use
  // a new entity manager.
  // The parser thread is started in GroveApp::generateEvents
  // which hasn't happened yet.
  clearEntityManager();
  return GroveApp::generateEvents(eceh);
}

class PrologPiEventHandler : public ErrorCountEventHandler {
 public:
  PrologPiEventHandler(DssslApp &app);
  void pi(PiEvent *);
  void endProlog(EndPrologEvent *);
  Boolean gotSpec() { return gotSpec_; }
 private:
  DssslApp *app_;
  Boolean gotSpec_;
};

PrologPiEventHandler::PrologPiEventHandler(DssslApp &app)
: app_(&app), gotSpec_(0)
{
}

void PrologPiEventHandler::pi(PiEvent *event)
{
  if (app_->getDssslSpecFromPi(event->data(),
                               event->dataLength(),
                               event->location())) {
    gotSpec_ = 1;
    cancel();
  }
  delete event;
}

void PrologPiEventHandler::endProlog(EndPrologEvent *event)
{
  cancel();
  delete event;
}

Boolean DssslApp::getDssslSpecFromProlog()
{
  initParser(rootSystemId_);
  PrologPiEventHandler eh(*this);
  parser_.parseAll(eh, eh.cancelPtr());
  return eh.gotSpec();
}

Boolean DssslApp::getDssslSpecFromPi(const Char *s, size_t n,
				     const Location &loc)
{
  static struct {
    const char *key;
    Boolean (DssslApp::*handler)(const Char *s, size_t, const Location &);
  } pis[] = {
    { "xml-stylesheet", &DssslApp::handleAttlistPi },
    { "xml:stylesheet", &DssslApp::handleAttlistPi },
    { "stylesheet", &DssslApp::handleAttlistPi },
    { "dsssl", &DssslApp::handleSimplePi },
  };
  for (size_t i = 0; i < SIZEOF(pis); i++) {
    size_t len = strlen(pis[i].key);
    if (n >= len
        && matchCi(s, len, pis[i].key)
	&& (n == len || isS(s[len]))) {
      s += len;
      n -= len;
      return (this->*pis[i].handler)(s, n, loc);
    }
  }
  return 0;
}

Boolean DssslApp::handleSimplePi(const Char *s, size_t n,
				 const Location &loc)
{
  skipS(s, n);
  if (n == 0)
    return 0;
  StringC sysid(s, n);
  splitOffId(sysid, dssslSpecId_);
  return entityManager()->expandSystemId(sysid, loc, 0, systemCharset(), 0, *this,
				         dssslSpecSysid_);
}

Boolean DssslApp::handleAttlistPi(const Char *s, size_t n,
				  const Location &loc)
{
  // FIXME maybe give warnings if syntax is wrong
  Boolean hadHref = 0;
  StringC href;
  Boolean hadTitle = 0;
  Boolean isDsssl = 0;
  Boolean isAlternate = 0;
  StringC name;
  StringC value;
  while (getAttribute(s, n, name, value)) {
    if (matchCi(name, "type")) {
      static const char *types[] = {
	"text/dsssl",
	"text/x-dsssl",
	"application/dsssl",
	"application/x-dsssl"
      };
      for (size_t i = 0; i < SIZEOF(types); i++)
	if (matchCi(value, types[i])) {
	  isDsssl = 1;
	  break;
	}
      if (!isDsssl)
	return 0;
    }
    else if (matchCi(name, "href")) {
      hadHref = 1;
      value.swap(href);
    }
    else if (matchCi(name, "title")) {
      if (specTitleOption_ && specTitle_ != value) { 
	availableSpecTitles_.push_back(value);
	return 0;
      }
      hadTitle = 1;
    }
    else if (matchCi(name, "alternate")) {
      isAlternate = matchCi(value, "yes");
    }
    else if (matchCi(name, "media")) { 
      /* There is no point in doing detailed comparisons if either
         the stylesheet is suitable for all media types or the
         backend handles all media types. This doesn't work for
         redundant values like "all, screen". */ 
      Boolean match = matchCi(value, "all") || (fotbDescr_->media[0] == 0);
      const Char *c = value.data();
      const Char *end = c + value.size();
      while (c < end && !match) {
        while (c < end && isS(*c))
          c++;
        for (int i = 0; fotbDescr_->media[i] && !match; i++) {
          match = 1;
          const Char *d;
          const char *d1;
          for (d = c, d1 = fotbDescr_->media[i];; d++, d1++) { 
            if (d == end) {
              match = (*d1 == '\0');
              break;
            }
            if (*d1 == '\0') { 
              /* This is not entirely correct. We should check
                 that *d is outside of [a-zA-Z0-9-]. */
              match = (*d == Char(' ') || *d == Char(','));
              break;
            }
            if (*d != Char(*d1)) {
              match = 0;  
              break;
            }
          }           
        }
        while (c < end && *c != Char(','))
          c++;
        c++;
      }
      if (!match)
        return 0;
    }
  }
  // PI doesn't contain useful information
  if (!isDsssl || !hadHref)
    return 0;
  // 
  if (!(specTitleOption_ && hadTitle) && isAlternate && dssslSpecSysid_.size() > 0)
    return 0;
  splitOffId(href, dssslSpecId_);
  // FIXME should use location of attribute value rather than location of PI
  return entityManager()->expandSystemId(href, loc, 0, systemCharset(), 0, *this,
				         dssslSpecSysid_)
    && ((specTitleOption_ && hadTitle) || !isAlternate);
}

void DssslApp::skipS(const Char *&s, size_t &n)
{
  while (n > 0 && isS(*s))
    s++, n--;
}

Boolean DssslApp::isS(Char c)
{
  return c <= CHAR_MAX && isspace((unsigned char)c);
}

Boolean DssslApp::matchCi(const StringC &s, const char *key)
{
  return matchCi(s.data(), s.size(), key);
}

Boolean DssslApp::matchCi(const Char *s, size_t n, const char *key)
{
  for (; *key; key++, s++, n--) {
    if (!n)
      return 0;
    if (*s != tolower(*key) && *s != toupper(*key))
      return 0;
  }
  return n == 0;
}

Boolean DssslApp::getAttribute(const Char *&s, size_t &n,
			       StringC &name, StringC &value)
{
  name.resize(0);
  value.resize(0);
  skipS(s, n);
  for (;;) {
    if (n == 0)
      return 0;
    if (*s == '=' || isS(*s))
      break;
    name += *s;
    s++, n--;
  }
  skipS(s, n);
  if (n == 0 || *s != '=')
    return 0;
  s++, n--;
  skipS(s, n);
  Char quote = 0;
  if (n > 0 && (*s == '"' || *s == '\'')) {
    quote = *s;
    s++, n--;
  }
  for (;;) {
    if (n == 0) {
      if (quote)
	return 0;
      break;
    }
    if (quote) {
      if (*s == quote) {
	s++, n--;
	break;
      }
    }
    else if (isS(*s))
      break;
    value += *s;
    s++, n--;
  }
  // FIXME resolve numeric character references
  return 1;
}

Boolean DssslApp::initSpecParser()
{
  if (!dssslSpecOption_ && !getDssslSpecFromProlog()) {
    if (specTitleOption_ && availableSpecTitles_.size() > 0)
      message(DssslAppMessages::noSpecTitle, 
              StringMessageArg(specTitle_),
              StringVectorMessageArg(availableSpecTitles_));
    if (dssslSpecSysid_.size() == 0) {
      message(DssslAppMessages::noSpec);
      return 0;
    }
  }
  SgmlParser::Params params;
  params.sysid = dssslSpecSysid_;
  params.entityManager = entityManager().pointer();
  params.options = &specOptions_;
  specParser_.init(params);
  specParser_.allLinkTypesActivated();
  return 1;
}

void DssslApp::processGrove()
{
  se_->process(rootNode_, *fotb_);
}

bool DssslApp::load(const StringC &sysid, const Vector<StringC> &active,
		    const NodePtr &parent, NodePtr &rootNode, const Vector<StringC> &architecture)
{
  SgmlParser::Params params;
  params.sysid = sysid;
  const NodePtr *ndp = groveTable_.lookup(params.sysid);
  if (ndp) {
    rootNode = *ndp;
    return 1;
  }
  ErrorCountEventHandler *eceh;
  const SdNode *sdNode;
  NodePtr parentRoot;
  if (parent
      && parent->getGroveRoot(parentRoot) == accessOK
      && (sdNode = SdNode::convert(parentRoot)) != 0
      && sdNode->getSd(params.sd, params.prologSyntax, params.instanceSyntax) == accessOK) {
    params.entityType = SgmlParser::Params::subdoc;
    eceh = GroveBuilder::make(groveTable_.count() + 1, this, this, 0,
			      params.sd, params.prologSyntax, params.instanceSyntax,
			      rootNode);
  }
  else
    eceh = GroveBuilder::make(groveTable_.count() + 1, this, this, 0, rootNode);
  Owner<EventHandler> eh(eceh);
  groveTable_.insert(params.sysid, rootNode);
  params.entityManager = entityManager().pointer();
  params.options = &options_;
  SgmlParser parser;
  parser.init(params);

  for (size_t i = 0; i < active.size(); i++)
    parser.activateLinkType(active[i]);
  parser.allLinkTypesActivated();

  if (architecture.size() > 0) {
    SelectOneArcDirector director(architecture, *eh);
    ArcEngine::parseAll(parser, director, director, eceh->cancelPtr());
  }
  else
    parser.parseAll(*eh, eceh->cancelPtr());
  return 1;
}

void DssslApp::mapSysid(StringC &id)
{
 // find the sysid for id by looking through the catalogs. 
  ConstPtr<EntityCatalog> 
     catalog(entityManager()->makeCatalog(id, systemCharset(), *this));
  catalog->lookupPublic(id, systemCharset(), *this, id); 
}

bool DssslApp::readEntity(const StringC &sysid, StringC &contents) 
{
  Owner<InputSource> in(entityManager()->open(sysid,
					      systemCharset(),
					      InputSourceOrigin::make(),
					      0,
					      *this));
  if (!in)
    return 0;
  for (;;) {
    Xchar c = in->get(*this);
    if (c == InputSource::eE)
      break;
    in->extendToBufferEnd();
    contents.append(in->currentTokenStart(), in->currentTokenLength());
  }
  return !in->accessError();
}

#ifdef DSSSL_NAMESPACE
}
#endif
