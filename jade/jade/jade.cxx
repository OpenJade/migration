// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#include "config.h"
#include "DssslApp.h"
#include "SgmlFOTBuilder.h"
#include "RtfFOTBuilder.h"
#include "TeXFOTBuilder.h"
#include "TransformFOTBuilder.h"
#ifdef JADE_HTML
#include "HtmlFOTBuilder.h"
#endif
#ifdef JADE_MIF
#include "MifFOTBuilder.h"
#endif
#ifdef JADE_TEXT
#include "TextFOTBuilder.h"
#endif
#include <OpenSP/OutputCharStream.h>
#include <OpenSP/macros.h>
#include <OpenSP/sptchar.h>
#include "JadeMessages.h"
#include <OpenSP/ErrnoMessageArg.h>
#include <OpenSP/OutputByteStream.h>
#include "dsssl_ns.h"

#include <errno.h>

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

class JadeApp : public DssslApp {
public:
  enum { u = 72000 };
  JadeApp();
  void processOption(AppChar opt, const AppChar *arg);
  FOTBuilder *makeFOTBuilder(const FOTBuilder::Description *&);
private:
  enum OutputType { fotType, 
                    rtfType,
#ifdef JADE_HTML
                    htmlType,
#endif
		    texType,
#ifdef JADE_MIF
                    mifType,
#endif
                    sgmlType, 
                    xmlType, 
#ifdef JADE_TEXT
                    txtType 
#endif 
                   };
  static const AppChar *const outputTypeNames[];
  OutputType outputType_;
  String<AppChar> outputFilename_;
  Vector<StringC> outputOptions_;
  FileOutputByteStream outputFile_;
};

const JadeApp::AppChar *const JadeApp::outputTypeNames[] = {
  SP_T("fot"),
  SP_T("rtf"),
#ifdef JADE_HTML
  SP_T("html"),
#endif
  SP_T("tex"),
#ifdef JADE_MIF
  SP_T("mif"),
#endif
  SP_T("sgml"),
  SP_T("xml"),
#ifdef JADE_TEXT
  SP_T("txt"),
#endif
};

JadeApp::JadeApp()
: DssslApp(u), outputType_(fotType)
{
  registerOption('t', SP_T("output-type"), JadeMessages::outputType,
                 JadeMessages::tHelp);
  registerOption('o', SP_T("output-file"), JadeMessages::file,
                 JadeMessages::oHelp);
}

void JadeApp::processOption(AppChar opt, const AppChar *arg)
{
  switch (opt) {
  case 't':
    {
      const AppChar *sub = tcschr(arg, SP_T('-'));
      size_t len = sub ? sub - arg : tcslen(arg);
      for (size_t i = 0;; i++) {
        if (i >= SIZEOF(outputTypeNames)) {
	  message(JadeMessages::unknownType, StringMessageArg(convertInput(arg)));
	  break;
	}
	if (tcsncmp(arg, outputTypeNames[i], len) == 0) {
	  outputType_ = OutputType(i);
	  break;
	}
      }
      if (sub) {
	StringC tem(convertInput(sub));
	StringC arg;
	for (size_t i = 0; i < tem.size(); i++) {
	  if (tem[i] == '-') {
	    if (arg.size())
	      outputOptions_.push_back(arg);
	    arg.resize(0);
	  }
	  else
	    arg += tem[i];
	}
	if (arg.size())
	  outputOptions_.push_back(arg);
      }
    }
    break;
  case 'o':
    if (*arg == 0)
      message(JadeMessages::emptyOutputFilename);
    else
      outputFilename_.assign(arg, tcslen(arg));
    break;
  default:
    DssslApp::processOption(opt, arg);
    break;
  }
}

FOTBuilder *JadeApp::makeFOTBuilder(const FOTBuilder::Description *&descr)
{
  if (outputFilename_.size() == 0) {
    if (defaultOutputBasename_.size() != 0) {
#ifdef SP_WIDE_SYSTEM
      outputFilename_ = defaultOutputBasename_;
#else
      outputFilename_ = codingSystem()->convertOut(defaultOutputBasename_);
      // convertOut adds a nul
      outputFilename_.resize(outputFilename_.size() - 1);
#endif
    }
    else
      outputFilename_.assign(SP_T("jade-out"), 8);
    outputFilename_ += SP_T('.');
    const AppChar *ext = outputTypeNames[outputType_];
    outputFilename_.append(ext, tcslen(ext));
  }
  switch (outputType_) {
#ifdef JADE_HTML
  case htmlType:
#endif
  case sgmlType:
  case xmlType:
    break;
  default:
    outputFilename_ += 0;
    if (!outputFile_.open(outputFilename_.data())) {
      message(JadeMessages::cannotOpenOutputError,
	      StringMessageArg(CmdLineApp::convertInput(outputFilename_.data())),
	      ErrnoMessageArg(errno));
      return 0;
    }
    break;
  }
  switch (outputType_) {
  case rtfType:
    unitsPerInch_ = 20*72; // twips
    return makeRtfFOTBuilder(&outputFile_, outputOptions_, entityManager(), systemCharset(), this, descr);
  case texType:
    return makeTeXFOTBuilder(&outputFile_, this, descr);
#ifdef JADE_HTML
  case htmlType:
    return makeHtmlFOTBuilder(outputFilename_, this, descr);
#endif /* JADE_HTML */
#ifdef JADE_MIF
  case mifType:
    return makeMifFOTBuilder(outputFilename_, entityManager(), systemCharset(), this, descr);
#endif /* JADE_MIF */
  case fotType:
    return makeSgmlFOTBuilder(new RecordOutputCharStream(
                                  new EncodeOutputCharStream(&outputFile_,
                                      outputCodingSystem_)), descr);
  case sgmlType:
  case xmlType:
    return makeTransformFOTBuilder(this, outputType_ == xmlType, outputOptions_, descr);
#ifdef JADE_TEXT
  case txtType:
    return makeTextFOTBuilder(&outputFile_, this, descr);
#endif /* JADE_TEXT */
  default:
    break;
  }
  CANNOT_HAPPEN();
  return 0;
}

#ifdef DSSSL_NAMESPACE
}
#endif

#ifdef DSSSL_NAMESPACE
SP_DEFINE_APP(DSSSL_NAMESPACE::JadeApp)
#else
SP_DEFINE_APP(JadeApp)
#endif
