// Copyright (c) 1996 James Clark
// See the file COPYING for copying permission.

#ifndef GroveBuilder_INCLUDED
#define GroveBuilder_INCLUDED 1

#include "Boolean.h"
#include "Node.h"

#ifdef SP_USE_DLL
#ifdef BUILD_LIBSPGROVE
#define SPGROVE_API SP_DLLEXPORT
#else
#define SPGROVE_API SP_DLLIMPORT
#endif
#else /* not SP_USE_DLL */
#define SPGROVE_API /* as nothing */
#endif /* not SP_USE_DLL */

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

class Messenger;
class ErrorCountEventHandler;

class SPGROVE_API GroveBuilder {
public:
  static bool setBlocking(bool);
  static ErrorCountEventHandler *make(Messenger *,
#ifdef GROVE_NAMESPACE
				      GROVE_NAMESPACE::
#endif
				      NodePtr &root);
private:
  GroveBuilder();
};

#ifdef SP_NAMESPACE
}
#endif

#endif /* not GroveBuilder_INCLUDED */
