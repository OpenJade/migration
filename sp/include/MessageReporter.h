// Copyright (c) 1994 James Clark
// See the file COPYING for copying permission.

#ifndef MessageReporter_INCLUDED
#define MessageReporter_INCLUDED 1

#ifdef __GNUG__
#pragma interface
#endif

#include "types.h"
#include "MessageFormatter.h"
#include "Boolean.h"
#include "OutputCharStream.h"
#include "Message.h"
#include "Location.h"
#include "StringC.h"

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

class SP_API MessageReporter : public MessageFormatter, public Messenger {
public:
  enum Option {
    openElements = 01,
    openEntities = 02,
    messageNumbers = 04,
    clauses = 010 
    };
  // The OutputCharStream will be deleted by the MessageReporter
  MessageReporter(OutputCharStream *);
  virtual ~MessageReporter();
  void setMessageStream(OutputCharStream *);
  OutputCharStream *releaseMessageStream();
  virtual void dispatchMessage(const Message &);
  virtual Boolean getMessageText(const MessageFragment &, StringC &);
  void addOption(Option);
  void setProgramName(const StringC &);
protected:
  MessageReporter(const MessageReporter &); // undefined
  void operator=(const MessageReporter &);  // undefined
  
  virtual const ExternalInfo *locationHeader(const Location &, Offset &off);
  virtual const ExternalInfo *locationHeader(const Origin *, Index, Offset &off);
  virtual void printLocation(const ExternalInfo *info, Offset off);
  OutputCharStream &os();

  OutputCharStream *os_;
  unsigned options_;
  StringC programName_;
};
class SP_API XMLMessageReporter : public MessageReporter {
protected:
  virtual void printLocation(const ExternalInfo *info, Offset off);
public:
  XMLMessageReporter(OutputCharStream * o) : MessageReporter(o) {}
  virtual void dispatchMessage(const Message &);
  virtual Boolean XMLformatFragment(const MessageFragment &, OutputCharStream &);
  virtual void formatMessage(const MessageFragment &,
		     const Vector<CopyOwner<MessageArg> > &args,
		     OutputCharStream &, bool noquote = 0);

  virtual void formatOpenElements(const Vector<OpenElementInfo> &openElementInfo,
			  OutputCharStream &os);
  virtual const ExternalInfo *locationHeader(const Origin *, Index, Offset &off);
  virtual void showOpenEntities(const Origin *, Index, Offset &off) ;
  virtual void showOpenEntities(const Location &loc, Offset &off) {
	showOpenEntities(loc.origin().pointer(), loc.index(), off) ;
  }
  virtual const ExternalInfo *locationHeader(const Location &loc, Offset &off) {
	return locationHeader(loc.origin().pointer(), loc.index(), off) ;
  }

} ;

inline
OutputCharStream &MessageReporter::os()
{
  return *os_;
}

inline
void MessageReporter::setProgramName(const StringC &programName)
{
  programName_ = programName;
}

inline
OutputCharStream *MessageReporter::releaseMessageStream()
{
  OutputCharStream *tem = os_;
  os_ = 0;
  return tem;
}

#ifdef SP_NAMESPACE
}
#endif

#endif /* not MessageReporter_INCLUDED */
