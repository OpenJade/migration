// Copyright (c) 1996 James Clark
// See the file COPYING for copying permission.

// Need option registration method that allows derived class to change
// option names.

#ifdef __GNUG__
#pragma implementation
#endif

#include "splib.h"
#include "CmdLineApp.h"
#include "CmdLineAppMessages.h"
#include "MessageArg.h"
#include "ErrnoMessageArg.h"
#include "Options.h"
#include "version.h"
#include "xnew.h"
#include "macros.h"
#include "sptchar.h"
#include "MessageTable.h"
#include "CodingSystemKit.h"

#include "ConsoleOutput.h"

#include <iostream.h>
#include <fstream.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef SP_HAVE_LOCALE
#include <locale.h>
#endif
#ifdef SP_HAVE_SETMODE
#include <fcntl.h>
#include <io.h>
#endif
#ifdef SP_HAVE_IOS_BINARY
#define IOS_BINARY ios::binary
#else
#define IOS_BINARY 0
#endif
#ifdef SP_WIDE_SYSTEM
#include <sys/stat.h>
#endif
#ifdef SP_WIDE_SYSTEM

#include <stdio.h>

#else /* not SP_WIDE_SYSTEM */

#include <sys/types.h>
#ifdef SP_INCLUDE_UNISTD_H
#include <unistd.h>
#endif
#ifdef SP_INCLUDE_IO_H
#include <io.h>
#endif

#endif /* not SP_WIDE_SYSTEM */

#ifndef SP_DEFAULT_ENCODING
#ifdef WIN32
#define SP_DEFAULT_ENCODING SP_T("WINDOWS")
#else
#define SP_DEFAULT_ENCODING  SP_T("IS8859-1")
#endif
#endif /* not SP_DEFAULT_ENCODING */

#ifdef SP_WIDE_SYSTEM
#include <windows.h>
#endif

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

static const SP_TCHAR *progName = 0;

static const SP_TCHAR versionString[] = SP_VERSION;

CmdLineApp::CmdLineApp(const char *requiredInternalCode)
: errorFile_(0),
  outputCodingSystem_(0),
  // Colon at beginning is Posix.2ism that says to return : rather than ? for
  // missing option argument.
  optstr_(SP_T(":"), 1),
  MessageReporter(0),
  internalCharsetIsDocCharset_(1),
  codingSystem_(0)
{
  initCodingSystem(requiredInternalCode);
  setMessageStream(makeStdErr());
  registerOption('b', internalCharsetIsDocCharset_ ? SP_T("bctf") : SP_T("encoding"));
  registerOption('f', SP_T("error_file"));
  registerOption('v');
}

void CmdLineApp::resetCodingSystemKit()
{
  codingSystemKit_ = codingSystemKit_->copy();
}

void CmdLineApp::registerOption(AppChar c, const AppChar *argName)
{
  optstr_ += c;
  if (argName) {
    optstr_ += SP_T(':');
    optArgNames_.push_back(argName);
  }
}

StringC CmdLineApp::usageString()
{
  String<AppChar> result;
  if (progName)
    result.assign(progName, tcslen(progName));
  PackedBoolean hadOption[128];
  for (int i = 0; i < 128; i++)
    hadOption[i] = 0;
  Boolean hadNoArgOption = 0;
  for (size_t i = 1; i < optstr_.size(); i++) {
    if (optstr_[i] == 0)
      break;
    if (i + 1 < optstr_.size() && optstr_[i + 1] == ':')
      i++;
    else if (!hadOption[optstr_[i]]) {
      hadOption[optstr_[i]] = 1;
      if (!hadNoArgOption) {
	hadNoArgOption = 1;
	result.append(SP_T(" [-"), 3);
      }
      result += optstr_[i];
    }
  }
  if (hadNoArgOption)
    result += SP_T(']');
  size_t j = 0;
  for (size_t i = 1; i < optstr_.size(); i++) {
    if (i + 1 < optstr_.size() && optstr_[i + 1] == ':') {
      if (!hadOption[optstr_[i]]) {
	hadOption[optstr_[i]] = 1;
	result += SP_T(' ');
	result += SP_T('[');
	result += SP_T('-');
	result += optstr_[i];
	result += SP_T(' ');
	result.append(optArgNames_[j], tcslen(optArgNames_[j]));
	result += SP_T(']');
      }
      i++;
      j++;
    }
  }
  result.append(SP_T(" sysid..."), tcslen(SP_T(" sysid...")));
  result += 0;
  return convertInput(result.data());
}

static
void ewrite(const CmdLineApp::AppChar *s)
{
#ifdef SP_WIDE_SYSTEM
  fputts(s, stderr);
#else
  int n = (int)strlen(s);
  while (n > 0) {
    int nw = write(2, s, n);
    if (nw < 0)
      break;
    n -= nw;
    s += nw;
  }
#endif
}

static
#ifdef SP_FANCY_NEW_HANDLER
int outOfMemory(size_t)
#else
void outOfMemory()
#endif
{
  if (progName) {
    ewrite(progName);
    ewrite(SP_T(": "));
  }
  ewrite(SP_T(": out of memory\n"));
  exit(1);
#ifdef SP_FANCY_NEW_HANDLER
  return 0;
#endif  
}

int CmdLineApp::init(int, AppChar **argv)
{
  set_new_handler(outOfMemory);
#ifdef SP_HAVE_LOCALE
  setlocale(LC_ALL, "");
#endif
#ifdef SP_HAVE_SETMODE
  _setmode(1, _O_BINARY);
  _setmode(2, _O_BINARY);
#endif
  progName = argv[0];
  if (progName)
    setProgramName(convertInput(progName));
#ifdef __GNUG__
  // cout is a performance disaster in libg++ unless we do this.
  ios::sync_with_stdio(0);
#endif
  return 0;
}

int CmdLineApp::run(int argc, AppChar **argv)
{
  int ret = init(argc, argv);
  if (ret)
    return ret;
  int firstArg;
  ret = processOptions(argc, argv, firstArg);
  if (ret)
    return ret;
  ret = processArguments(argc - firstArg, argv + firstArg);
  progName = 0;
  return ret;
}    

Boolean CmdLineApp::openFilebufWrite(filebuf &file,
				     const AppChar *filename)
{
#ifdef SP_WIDE_SYSTEM
  int fd = _wopen(filename, _O_CREAT|_O_WRONLY|_O_TRUNC|_O_BINARY,
                  _S_IREAD | _S_IWRITE);
  if (fd < 0) {
    // _wopen will always fail on Windows 95
    String<char> buf;
    int len = WideCharToMultiByte(CP_ACP, 0, filename, -1, 0, 0, 0, 0);
    buf.resize(len + 1);
    WideCharToMultiByte(CP_ACP, 0, filename, -1, buf.begin(), len, 0, 0);
    buf[len] = '\0';
    return file.open(buf.data(), ios::out|ios::trunc|IOS_BINARY) != 0;
  }
  return file.attach(fd) != 0;
#else
  return file.open(filename, ios::out|ios::trunc|IOS_BINARY) != 0;
#endif
}

int CmdLineApp::processOptions(int argc, AppChar **argv, int &nextArg)
{
  AppChar ostr[2];
  optstr_ += SP_T('\0');
  Options<AppChar> options(argc, argv, optstr_.data());
  AppChar opt;
  while (options.get(opt)) {
    switch (opt) {
    case ':':
      ostr[0] = options.opt();
      ostr[1] = SP_T('\0');
      message(CmdLineAppMessages::missingOptionArgError,
	      StringMessageArg(convertInput(ostr)));
      message(CmdLineAppMessages::usage,
	      StringMessageArg(usageString()));
      return 1;
    case '?':
      ostr[0] = options.opt();
      ostr[1] = SP_T('\0');
      message(CmdLineAppMessages::invalidOptionError,
	      StringMessageArg(convertInput(ostr)));
      message(CmdLineAppMessages::usage,
	      StringMessageArg(usageString()));
      return 1;
    default:
      processOption(opt, options.arg());
      break;
    }
  }
  nextArg = options.ind();
  if (errorFile_) {
    static filebuf file;
    if (!openFilebufWrite(file, errorFile_)) {
      message(CmdLineAppMessages::cannotOpenOutputError,
	      StringMessageArg(convertInput(errorFile_)),
	      ErrnoMessageArg(errno));
      return 1;
    }
    setMessageStream(new IosOutputCharStream(&file, codingSystem()));
  }
  if (!outputCodingSystem_)
    outputCodingSystem_ = codingSystem();
  return 0;
}

void CmdLineApp::processOption(AppChar opt, const AppChar *arg)
{
  switch (opt) {
  case 'b':
    outputCodingSystem_ = lookupCodingSystem(arg);
    if (!outputCodingSystem_)
      message(internalCharsetIsDocCharset_
	      ? CmdLineAppMessages::unknownBctf
	      : CmdLineAppMessages::unknownEncoding,
	      StringMessageArg(convertInput(arg)));
    break;
  case 'f':
    errorFile_ = arg;
    break;
  case 'v':
    // print the version number
    message(CmdLineAppMessages::versionInfo,
	    StringMessageArg(convertInput(versionString)));
    break;
  default:
    CANNOT_HAPPEN();
  }
}

Boolean CmdLineApp::getMessageText(const MessageFragment &frag,
				   StringC &text)
{
  String<SP_TCHAR> str;
  if (!MessageTable::instance()->getText(frag, str))
    return 0;
#ifdef SP_WIDE_SYSTEM
  text.assign((const Char *)str.data(), str.size());
#else
  str += 0;
  text = codingSystem()->convertIn(str.data());
#endif
  return 1;
}

Boolean stringMatches(const SP_TCHAR *s, const char *key)
{
  for (; *key != '\0'; s++, key++) {
    if (*s != tolower(*key) && *s != toupper(*key))
      return 0;
  }
  return *s == '\0';
}

void CmdLineApp::initCodingSystem(const char *requiredInternalCode)
{
  const char *name = requiredInternalCode;
#ifdef SP_MULTI_BYTE
  char buf[256];
  if (!name) {
    const SP_TCHAR *internalCode = tgetenv(SP_T("SP_SYSTEM_CHARSET"));
    if (internalCode) {
      buf[255] = '\0';
      for (size_t i = 0; i < 255; i++) {
	buf[i] = internalCode[i];
	if (buf[i] == '\0')
	  break;
      }
      name = buf;
    }
  }
  if (requiredInternalCode)
    internalCharsetIsDocCharset_ = 0;
  else {
    const SP_TCHAR *useInternal = tgetenv(SP_T("SP_CHARSET_FIXED"));
    if (useInternal
        && (stringMatches(useInternal, "YES")
	    || stringMatches(useInternal, "1")))
      internalCharsetIsDocCharset_ = 0;
  }
#endif /* SP_MULTI_BYTE */
  codingSystemKit_ = CodingSystemKit::make(name);
  const SP_TCHAR *codingName = tgetenv(internalCharsetIsDocCharset_
				       ? SP_T("SP_BCTF")
				       : SP_T("SP_ENCODING"));
  if (codingName)
    codingSystem_ = lookupCodingSystem(codingName);
#ifdef SP_MULTI_BYTE
  if (!codingSystem_ && !internalCharsetIsDocCharset_)
    codingSystem_ = lookupCodingSystem(SP_DEFAULT_ENCODING);
#endif
  if (!codingSystem_
#ifndef SP_WIDE_SYSTEM
      || codingSystem_->fixedBytesPerChar() > 1
#endif
    )
    codingSystem_ = codingSystemKit_->identityCodingSystem();
}

const CodingSystem *
CmdLineApp::lookupCodingSystem(const AppChar *codingName)
{
#define MAX_CS_NAME 50
  if (tcslen(codingName) < MAX_CS_NAME) {
    char buf[MAX_CS_NAME];
    int i;
    for (i = 0; codingName[i] != SP_T('\0'); i++) {
      SP_TUCHAR c = codingName[i];
#ifdef SP_WIDE_SYSTEM
      if (c > (unsigned char)-1)
	return 0;
#endif
      buf[i] = char(c);
    }
    buf[i] = '\0';
    return codingSystemKit_->makeCodingSystem(buf, internalCharsetIsDocCharset_);
  }
  return 0;
}

StringC CmdLineApp::convertInput(const SP_TCHAR *s)
{
#ifdef SP_WIDE_SYSTEM
  StringC str(s, wcslen(s));
#else
  StringC str(codingSystem()->convertIn(s));
#endif
  for (size_t i = 0; i < str.size(); i++)
    if (str[i] == '\n')
      str[i] = '\r';
  return str;
}

OutputCharStream *CmdLineApp::makeStdErr()
{
  OutputCharStream *os = ConsoleOutput::makeOutputCharStream(2);
  if (os)
    return os;
  return new IosOutputCharStream(cerr.rdbuf(), codingSystem());
}

OutputCharStream *CmdLineApp::makeStdOut()
{
  OutputCharStream *os = ConsoleOutput::makeOutputCharStream(1);
  if (os)
    return os;
  return new IosOutputCharStream(cout.rdbuf(), outputCodingSystem_);
}

#ifdef SP_NAMESPACE
}
#endif
