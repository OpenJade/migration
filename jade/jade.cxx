// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#include "config.h"
#include "DssslApp.h"
#include "SgmlFOTBuilder.h"
#include "RtfFOTBuilder.h"
#include "HtmlFOTBuilder.h"
#include "TeXFOTBuilder.h"
#include "OutputCharStream.h"
#include "macros.h"
#include "sptchar.h"
#include "jade_version.h"
#include "JadeMessages.h"
#include "ErrnoMessageArg.h"
#include <iostream.h>
#include <fstream.h>
#include <errno.h>

#ifdef DSSSL_NAMESPACE
using namespace DSSSL_NAMESPACE;
#endif

class DfrontApp : public DssslApp {
public:
  enum { u = 72000 };
  DfrontApp();
  void processOption(AppChar opt, const AppChar *arg);
  FOTBuilder *makeFOTBuilder(const FOTBuilder::Extension *&);
private:
  enum OutputType { fotType, rtfType, htmlType, texType };
  static const AppChar *const outputTypeNames[];
  OutputType outputType_;
  String<AppChar> outputFilename_;
  filebuf outputFile_;
};

SP_DEFINE_APP(DfrontApp)

const DfrontApp::AppChar *const DfrontApp::outputTypeNames[] = {
  SP_T("fot"),
  SP_T("rtf"),
  SP_T("html"),
  SP_T("tex")
};

DfrontApp::DfrontApp()
: DssslApp(u), outputType_(fotType)
{
  registerOption('t', SP_T("(fot|rtf|html|tex)"));
  registerOption('o', SP_T("output_file"));
}

void DfrontApp::processOption(AppChar opt, const AppChar *arg)
{
  switch (opt) {
  case 't':
    {
      for (size_t i = 0;; i++) {
        if (i >= SIZEOF(outputTypeNames)) {
	  message(JadeMessages::unknownType, StringMessageArg(convertInput(arg)));
	  break;
	}
	if (tcscmp(arg, outputTypeNames[i]) == 0) {
	  outputType_ = OutputType(i);
	  break;
	}
      }
    }
    break;
  case 'o':
    if (*arg == 0)
      message(JadeMessages::emptyOutputFilename);
    else
      outputFilename_.assign(arg, tcslen(arg));
    break;
  case 'v':
    message(JadeMessages::versionInfo,
	    StringMessageArg(convertInput(JADE_VERSION)));
    // fall through
  default:
    DssslApp::processOption(opt, arg);
    break;
  }
}

FOTBuilder *DfrontApp::makeFOTBuilder(const FOTBuilder::Extension *&exts)
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
  if (outputType_ != htmlType) {
    outputFilename_ += 0;
    if (!CmdLineApp::openFilebufWrite(outputFile_, outputFilename_.data())) {
      message(JadeMessages::cannotOpenOutputError,
	      StringMessageArg(CmdLineApp::convertInput(outputFilename_.data())),
	      ErrnoMessageArg(errno));
      return 0;
    }
  }
  switch (outputType_) {
  case rtfType:
    unitsPerInch_ = 20*72; // twips
    return makeRtfFOTBuilder(&outputFile_, entityManager(), systemCharset_, this, exts);
  case texType:
    return makeTeXFOTBuilder(&outputFile_);
  case htmlType:
    return makeHtmlFOTBuilder(outputFilename_, outputCodingSystem_, this, exts);
  case fotType:
    return makeSgmlFOTBuilder(new RecordOutputCharStream(new IosOutputCharStream(&outputFile_,
                                                                                 outputCodingSystem_)));
  default:
    break;
  }
  CANNOT_HAPPEN();
  return 0;
}
