// Copyright (c) 2000 Matthias Clasen
// See the file COPYING for copying permission.

#ifndef Fixed4CodingSystem_INCLUDED
#define Fixed4CodingSystem_INCLUDED 1

#include "CodingSystem.h"

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

class SP_API Fixed4CodingSystem : public CodingSystem {
public:
  Decoder *makeDecoder() const;
  Encoder *makeEncoder() const;
  unsigned fixedBytesPerChar() const;
};

#ifdef SP_NAMESPACE
}
#endif

#endif /* not Fixed4CodingSystem_INCLUDED */
