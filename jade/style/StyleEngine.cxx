// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#include "stylelib.h"
#include "StyleEngine.h"
#include "Interpreter.h"
#include "InterpreterMessages.h"
#include "SchemeParser.h"
#include "FOTBuilder.h"
#include "DssslSpecEventHandler.h"
#include <OpenSP/ArcEngine.h>
#include "ProcessContext.h"
#include <OpenSP/macros.h>
#include <OpenSP/InternalInputSource.h>
#include <OpenSP/OutputCharStream.h>
#include <OpenSP/OutputByteStream.h>
#include "DocumentGenerator.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

StyleEngine::StyleEngine(Messenger &mgr,
			 GroveManager &groveManager,
			 int unitsPerInch,
			 bool debugMode,
			 bool dsssl2,
                         bool strictMode,
			 const FOTBuilder::Description &fotbDescr,
                         const CodingSystem *out)
: interpreter_(0),
  mgr_(&mgr), groveManager_(&groveManager), 
  unitsPerInch_(unitsPerInch), debugMode_(debugMode),
  dsssl2_(dsssl2), strictMode_(strictMode), 
  fotbDescr_(&fotbDescr), outputCodingSystem_(out)  
{
  // FIXME: this is super-ugly. We can't construct the Interpreter
  // right away, since we don't know if its a SL/TL context until
  // we have parsed the spec. Thus we have to store all constructor
  // args until then.
}

void StyleEngine::defineVariable(const StringC &str, StringC &cmdline)
{
  // Dk: Interpret "name=value" as a string variable Setting.
  if (str[0] == '(') {
    cmdline += str;
  } 
  else {
    int i;
    for (i = 0; (i < str.size()) && (str[i] != '='); i++)
      ;

    // Dk: Not name=value?
    if (!i || (i >= (str.size()))) {  
      cmdline += interpreter_->makeStringC("(define ");
      cmdline += str;
      cmdline += interpreter_->makeStringC(" #t)");
    }
    else {  
      // Dk: name=value.
      cmdline += interpreter_->makeStringC("(define ");
      cmdline += StringC(str.begin(), i);
      cmdline += interpreter_->makeStringC(" \"");
      if (str.size() - (i + 1) > 0);
        cmdline += StringC(str.begin() + i + 1, str.size() - (i + 1));
      cmdline += interpreter_->makeStringC("\")");
    }
  }
}

void StyleEngine::parseSpec(SgmlParser &specParser,
			    const CharsetInfo &charset,
			    const StringC &id,
			    Messenger &mgr,
			    const Vector<StringC> &defVars)
{
  DssslSpecEventHandler specHandler(mgr);
  Vector<DssslSpecEventHandler::Part *> parts;
  specHandler.load(specParser, charset, id, parts);

  interpreter_ = new Interpreter(groveManager_, mgr_, unitsPerInch_, 
                                 debugMode_, dsssl2_, 
                                 parts.size() > 0 ? parts[0]->style() : 1, 
                                 strictMode_, *fotbDescr_);
  
  for (int phase = 0; phase < 3; phase++) {
    for (size_t i = 0; i < parts.size(); i++) {
      DssslSpecEventHandler::Part::DIter diter(parts[i]->doc()->diter());
      bool local = 0;
      do {
	if (local) 
    	  diter = parts[i]->diter();
	local = !local;
	for (; !diter.done(); diter.next()) {
          // parse in three phases:
          // 1. features
          // 2. char-repertoire, standard-chars, other-chars
          // 3. the rest
	  if (diter.cur()->type() == DssslSpecEventHandler::DeclarationElement::features ? phase == 0 : 
              ((diter.cur()->type() == DssslSpecEventHandler::DeclarationElement::charRepertoire ||
               diter.cur()->type() == DssslSpecEventHandler::DeclarationElement::standardChars)
	      ? phase == 1 
	      : phase == 2)) {
            if (diter.cur()->type() == DssslSpecEventHandler::DeclarationElement::sgmlGrovePlan) {
              Owner<InputSource> in(new InternalInputSource(
                           diter.cur()->modadd(), InputSourceOrigin::make()));
	      SchemeParser scm(*interpreter_, in);
              scm.parseGrovePlan();
            } 
            else {
	      Owner<InputSource> in;
	      diter.cur()->makeInputSource(specHandler, in);
	      SchemeParser scm(*interpreter_, in);
	      switch (diter.cur()->type()) {
              case DssslSpecEventHandler::DeclarationElement::charRepertoire:
                interpreter_->setCharRepertoire(diter.cur()->name());
                break;
              case DssslSpecEventHandler::DeclarationElement::standardChars:
                scm.parseStandardChars(); 
                break;
              case DssslSpecEventHandler::DeclarationElement::mapSdataEntity:
                scm.parseMapSdataEntity(diter.cur()->name(), diter.cur()->text());
                break;
              case DssslSpecEventHandler::DeclarationElement::addNameChars:
                scm.parseNameChars();
                break;
              case DssslSpecEventHandler::DeclarationElement::addSeparatorChars:
                scm.parseSeparatorChars();
                break;
              case DssslSpecEventHandler::DeclarationElement::features:
                scm.parseFeatures(); 
                break;
              default:
                interpreter_->message(
                       InterpreterMessages::unsupportedDeclaration);
               break;
              }
            }   
          }
	}
      } while (local);
      interpreter_->dEndPart();
    }
  }

  if (defVars.size() > 0) {  
    StringC cmdline;
    for (size_t i = 0; i < defVars.size(); i++) 
      defineVariable(defVars[i], cmdline);
    Owner<InputSource> in(new InternalInputSource(cmdline,
                          InputSourceOrigin::make()));
    SchemeParser scm(*interpreter_, in);
    scm.parse();
    interpreter_->endPart();
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
}


StyleEngine::~StyleEngine()
{
  delete interpreter_;
}

void StyleEngine::process(const NodePtr &node, FOTBuilder &fotb)
{
  interpreter_->compile(node);
  if (interpreter_->style()) {
    ProcessContext context(*interpreter_, fotb);
    context.process(node);
  }
  else {
    FileOutputByteStream outputFile_;
    if (outputFile_.open("jade-grove.out")) {
      DocumentGenerator docgen(interpreter_, new RecordOutputCharStream(
                                 new EncodeOutputCharStream(&outputFile_,
                                   outputCodingSystem_)));
      docgen.emit(node);
    }
  }
}

#ifdef DSSSL_NAMESPACE
}
#endif
