// This file was automatically generated from jade/RtfMessages.msg by msggen.pl.
#include "Message.h"

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

struct RtfMessages {
  // 4000
  static const MessageType0 nestedTable;
  // 4001
  static const MessageType1 systemIdNotFilename;
};
const MessageType0 RtfMessages::nestedTable(
MessageType::error,
#ifdef BUILD_LIBSP
MessageFragment::libModule,
#else
MessageFragment::appModule,
#endif
4000
#ifndef SP_NO_MESSAGE_TEXT
,"nested tables are not allowed in RTF"
#endif
);
const MessageType1 RtfMessages::systemIdNotFilename(
MessageType::error,
#ifdef BUILD_LIBSP
MessageFragment::libModule,
#else
MessageFragment::appModule,
#endif
4001
#ifndef SP_NO_MESSAGE_TEXT
,"could not convert system identifier %1 to a single filename"
#endif
);
#ifdef SP_NAMESPACE
}
#endif
