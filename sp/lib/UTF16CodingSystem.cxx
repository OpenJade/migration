// Copyright (c) 2000 Matthias Clasen
// See the file COPYING for copying permission.

#include "splib.h"

#ifdef SP_MULTI_BYTE

#include "UTF16CodingSystem.h"
#include "constant.h"

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

class UTF16Decoder : public Decoder {
public:
  UTF16Decoder();
  size_t decode(Char *, const char *, size_t, const char **);
private:
  // value for encoding error
  enum { invalid = 0xfffd };
  Boolean recovering_;
};

class UTF16Encoder : public Encoder {
public:
  UTF16Encoder();
  void output(const Char *, size_t, OutputByteStream *);
private:
  // value for encoding error
  enum { invalid = 0xfffd };
};

Decoder *UTF16CodingSystem::makeDecoder() const
{
  return new UTF16Decoder;
}

Encoder *UTF16CodingSystem::makeEncoder() const
{
  return new UTF16Encoder;
}


UTF16Decoder::UTF16Decoder()
: recovering_(0)
{
}

size_t UTF16Decoder::decode(Char *to, const char *from,
			  size_t fromLen, const char **rest)
{
  Char *start = to;
  const unsigned char *us = (const unsigned char *)from;
  for (;;) {
    if (fromLen < 2) 
      break;
    Unsigned32 x = (us[0] << 8) + us[1];
    if (x < 0xd800 || x > 0xdfff) {
      *to++ = x;
      us += 2;
      fromLen -= 2;
      continue;
    } 
    if (x > 0xdbff) {
      // FIXME: unpaired RC element
      *to++ = invalid;
      us += 2;
      fromLen -= 2;
      continue;
    }
    if (fromLen < 4)
      break;
    Unsigned32 y = (us[2] << 8) + us[3];
    if (y < 0xd800 || y > 0xdfff) {
      // FIXME: unpaired RC element
      *to++ = invalid;
      *to++ = y;
      us += 4;
      fromLen -= 4;
      continue;
    }
    if (y < 0xdc00) {
      // FIXME: two unpaired RC elements
      *to++ = invalid;
      *to++ = invalid;
      us += 4;
      fromLen -= 4;
      continue;
    } 
    *to++ = ((x - 0xd800) * 0x400 + (y - 0xdc00)) + 0x10000; 
    us += 2;
    fromLen -= 2;
  } 
  *rest = (char *)us;
  return to - start;
}

UTF16Encoder::UTF16Encoder()
{
}

void UTF16Encoder::output(const Char *s, size_t n, OutputByteStream *sb)
{
  for (;n > 0;n--, s++) {
    if (*s < 0x10000) {
      sb->sputc((*s >> 8) && 0xff);
      sb->sputc(*s && 0xff);
    } 
    else if (*s < 0x110000) {
      Unsigned32 y = ((*s - 0x10000) / 0x400) + 0xd800;
      Unsigned32 z = ((*s - 0x10000) % 0x400) + 0xdc00;
      sb->sputc((y >> 8) && 0xff);
      sb->sputc(y && 0xff);
      sb->sputc((z >> 8) && 0xff);
      sb->sputc(z && 0xff);
    } else {
      sb->sputc((invalid >> 8) && 0xff);
      sb->sputc(invalid && 0xff);
    }
  }
}

#ifdef SP_NAMESPACE
}
#endif

#else /* not SP_MULTI_BYTE */

#ifndef __GNUG__
static char non_empty_translation_unit;	// sigh
#endif

#endif /* not SP_MULTI_BYTE */
