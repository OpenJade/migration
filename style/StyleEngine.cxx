// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#include "stylelib.h"
#include "StyleEngine.h"
#include "Interpreter.h"
#include "InterpreterMessages.h"
#include "SchemeParser.h"
#include "FOTBuilder.h"
#include "DssslSpecEventHandler.h"
#include "ArcEngine.h"
#include "ProcessContext.h"
#include "macros.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

StyleEngine::StyleEngine(Messenger &mgr,
			 GroveManager &groveManager,
			 int unitsPerInch,
			 bool debugMode,
			 bool dsssl2,
                         bool strictMode,
			 const FOTBuilder::Extension *extensionTable)
: interpreter_(new Interpreter(&groveManager, &mgr, unitsPerInch, 
                               debugMode, dsssl2, strictMode, extensionTable))
{
}

void StyleEngine::parseSpec(SgmlParser &specParser,
			    const CharsetInfo &charset,
			    const StringC &id,
			    Messenger &mgr)
{
  DssslSpecEventHandler specHandler(mgr);
  Vector<DssslSpecEventHandler::Part *> parts;
  specHandler.load(specParser, charset, id, parts);

  for (int phase = 0; phase < 2; phase++) {
    for (size_t i = 0; i < parts.size(); i++) {
      DssslSpecEventHandler::Part::DIter diter(parts[i]->doc()->diter());
      bool local = 0;
      do {
	if (local) 
    	  diter = parts[i]->diter();
	local = !local;
	for (; !diter.done(); diter.next()) {
	  if ((diter.cur()->type() == DssslSpecEventHandler::DeclarationElement::charRepertoire ||
               diter.cur()->type() == DssslSpecEventHandler::DeclarationElement::standardChars)
	      ? phase == 0
	      : phase == 1) {
	    Owner<InputSource> in;
	    diter.cur()->makeInputSource(specHandler, in);
	    switch (diter.cur()->type()) {
            case DssslSpecEventHandler::DeclarationElement::charRepertoire:
              interpreter_->setCharRepertoire(diter.cur()->name());
              break;
            case DssslSpecEventHandler::DeclarationElement::addNameChars:
              interpreter_->addNameChars(in);
              break;
            case DssslSpecEventHandler::DeclarationElement::addSeparatorChars:
              interpreter_->addSeparatorChars(in);
              break;
	    case DssslSpecEventHandler::DeclarationElement::standardChars:
	      interpreter_->addStandardChars(in);
	      break;
	    case DssslSpecEventHandler::DeclarationElement::mapSdataEntity:
	      interpreter_->addSdataEntity(diter.cur()->name(), 
				           diter.cur()->text(),
				           in);
	      break;
	    default:
	      interpreter_->message(InterpreterMessages::unsupportedDeclaration);
	      break;
	    }
	  }
	}
      } while (local);
    }
    interpreter_->dEndPart();
  }

  for (size_t i = 0; i < parts.size(); i++) {
    for (DssslSpecEventHandler::Part::Iter iter(parts[i]->iter());
         !iter.done();
	 iter.next()) {
      Owner<InputSource> in;
      iter.cur()->makeInputSource(specHandler, in);
      if (in) {
	SchemeParser scm(*interpreter_, in);
	scm.parse();
      }
    }
    interpreter_->endPart();
  }
  interpreter_->compile();
}

void StyleEngine::defineVariable(const StringC &ident)
{
  interpreter_->defineVariable(ident);
}

StyleEngine::~StyleEngine()
{
  delete interpreter_;
}

void StyleEngine::process(const NodePtr &node, FOTBuilder &fotb)
{
  ProcessContext context(*interpreter_, fotb);
  context.process(node);
}

#ifdef DSSSL_NAMESPACE
}
#endif
