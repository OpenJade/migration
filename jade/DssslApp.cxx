// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#include "config.h"
#include "DssslApp.h"
#include "StorageManager.h"
#include "DssslAppMessages.h"
#include "StyleEngine.h"
#include "sptchar.h"
#include "macros.h"

#ifdef SP_HAVE_LOCALE
#include <locale.h>
#endif

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

DssslApp::DssslApp(int unitsPerInch)
: GroveApp("unicode"), unitsPerInch_(unitsPerInch)
{
  registerOption('d', SP_T("dsssl_spec"));
  registerOption('V', SP_T("variable"));
}

int DssslApp::init(int argc, AppChar **argv)
{
  int ret = GroveApp::init(argc, argv);
#ifdef SP_HAVE_LOCALE
  // Since we use strtod(), must have C numeric
  setlocale(LC_NUMERIC, "C");
#endif
  return ret;
}

int DssslApp::processSysid(const StringC &sysid)
{
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
      if (dssslSpecSysid_.size() == 0) {
	static const Char ext[] = { '.', 'd', 's', 'l' };
        s.append(ext, SIZEOF(ext));
	specId.unparse(systemCharset(), 0, dssslSpecSysid_);
      }
      break;
    }
  if (dssslSpecSysid_.size() == 0) {
    message(DssslAppMessages::noSpec);
    return 0;
  }
  initParser(dssslSpecSysid_);
  // Since the thread parsing the DSSSL spec is a different thread
  // from the thread parsing the document, we can't share an
  // entity manager.
  clearEntityManager();
  parser_.swap(specParser_);
  return GroveApp::processSysid(sysid);
}

void DssslApp::processOption(AppChar opt, const AppChar *arg)
{
  switch (opt) {
  case 'd':
    dssslSpecId_.resize(0);
    dssslSpecSysid_ = convertInput(arg);
    for (size_t i = dssslSpecSysid_.size(); i > 0; i--) {
      if (dssslSpecSysid_[i - 1] == '#') {
        dssslSpecId_.assign(dssslSpecSysid_.data() + i,
	                    dssslSpecSysid_.size() - i);
	dssslSpecSysid_.resize(i - 1);
	break;
      }
    }
    break;
  case 'V':
    defineVars_.push_back(convertInput(arg));
    break;
  default:
    GroveApp::processOption(opt, arg);
  }
}

void DssslApp::processGrove()
{
  const FOTBuilder::Extension *extensions = 0;
  Owner<FOTBuilder> fotb(makeFOTBuilder(extensions));
  if (!fotb)
    return;
  StyleEngine se(*this, rootNode_, unitsPerInch_, extensions);
  for (size_t i = 0; i < defineVars_.size(); i++)
    se.defineVariable(defineVars_[i]);
  se.parseSpec(specParser_, systemCharset(), dssslSpecId_, *this);
  se.process(*fotb);
}

#ifdef DSSSL_NAMESPACE
}
#endif
