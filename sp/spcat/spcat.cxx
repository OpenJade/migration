// Copyright (c) 1998, 1999 Matthias Clasen 
// See the file COPYING for copying permission.

#include "config.h"

#include "EntityApp.h"
#include "Entity.h"
#include "Text.h"
#include "Sd.h"
#include "sptchar.h"
#include "OutputCharStream.h"

#ifdef SP_NAMESPACE
using namespace SP_NAMESPACE;
#endif

class SpcatApp : public EntityApp {
public:
  SpcatApp();
  void processOption(AppChar opt, const AppChar *arg);
  int processArguments(int argc, AppChar **argv);
  int processSysid(const StringC &);
private:
  Boolean lastFound_;
};

SP_DEFINE_APP(SpcatApp)

SpcatApp::SpcatApp()
 : lastFound_(true)
{
  registerOption('P', SP_T("public_id"));
  registerOption('S', SP_T("system_id"));
  registerOption('p', SP_T("parameter_entity"));
  registerOption('d', SP_T("doctype"));
  registerOption('l', SP_T("linktype"));
  registerOption('e', SP_T("entity"));
  registerOption('n', SP_T("notation"));
  registerOption('s', SP_T("doctype"));
  registerLongOption(SP_T("public-id"), 'P', SP_T("literal"));
  registerLongOption(SP_T("system-id"), 'S', SP_T("literal"));
  registerLongOption(SP_T("parameter-entity"), 'p', SP_T("name"));
  registerLongOption(SP_T("doctype"), 'd', SP_T("name"));
  registerLongOption(SP_T("linktype"), 'l', SP_T("name"));
  registerLongOption(SP_T("entity"), 'e', SP_T("name"));
  registerLongOption(SP_T("notation"), 'n', SP_T("name"));
  registerLongOption(SP_T("declaration"), 's', SP_T("doctype"));
}

void SpcatApp::processOption(AppChar opt, const AppChar *arg)
{
  //FIXME This is to avoid calling entityManager() for 
  //EntityApp options, since entityManager() fixes the set 
  //of catalogs and clearEntityManager() didn't work for me.
  switch (opt) {
  case 'P':
  case 'S':
  case 'e':
  case 'p':
  case 'd':
  case 'l':
  case 'n':
  case 's':
    break;
  default: 
    EntityApp::processOption(opt, arg);
    return ;
  }

  ExternalId exId;
  StringC name;
  EntityDecl::DeclType type = EntityDecl::generalEntity; 
  Ptr<EntityManager> ema(entityManager().pointer());
  Sd sd(ema);
  Syntax syntax(sd); 
  Text txt;
  Location loc;

  switch (opt) {
  case 'P':
    for (int i = 0; arg[i] != '\0'; i++) 
      txt.addChar(arg[i], loc);
    const MessageType1 *err, *err1;
    exId.setPublic(txt, systemCharset(), syntax.space(), err, err1);
    break;
  case 'S':
    for (int i = 0; arg[i] != '\0'; i++) 
      txt.addChar(arg[i], loc);
    exId.setSystem(txt);
    break;
  case 'e':
    type = EntityDecl::generalEntity;
    for (int i = 0; arg[i] != '\0'; i++) 
      name += arg[i]; 
    break;
  case 'p':
    type = EntityDecl::parameterEntity;
    //FIXME get a complete syntax from somewhere
    //so that we can say
    //name += syntax.peroDelim();
    name += '%';
    for (int i = 0; arg[i] != '\0'; i++) 
      name += arg[i]; 
    break;
  case 'd':
    type = EntityDecl::doctype;
    for (int i = 0; arg[i] != '\0'; i++) 
      name += arg[i]; 
    break;
  case 'l':
    type = EntityDecl::linktype;
    for (int i = 0; arg[i] != '\0'; i++) 
      name += arg[i]; 
    break;
  case 'n':
    type = EntityDecl::notation;
    for (int i = 0; arg[i] != '\0'; i++) 
      name += arg[i]; 
    break;
  case 's':
    type = EntityDecl::sgml;
    break;
  default:
    EntityApp::processOption(opt, arg);
    return ;
  }

  ExternalTextEntity ent(name, type, loc, exId);
  StringC ret;
  ConstPtr<EntityCatalog> cat = 
         entityManager()->makeCatalog(ret, systemCharset(), *this);
  StringC sysid;

  if ((type == EntityDecl::sgml && cat->sgmlDecl(systemCharset(), *this, sysid, ret)) 
      || cat->lookup(ent, syntax, systemCharset(), *this, ret)) {
    lastFound_ = true;
    //FIXME should use makeStdOut(), but it didn't work for me.
    //OutputCharStream *os = makeStdOut();
    //*os << ret << '\n';	
    //delete os;
    for (int i = 0; i < ret.size(); i++) 
      printf("%c", ret[i]);
    printf("\n");
  } 
  else
    lastFound_ = false;
}

int SpcatApp::processArguments(int argc, AppChar **argv)
{
  if (lastFound_) 
    return 0;
  else 
    return 1;
}

int SpcatApp::processSysid(const StringC &)
{
  return 0;
}
