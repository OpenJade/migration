// Copyright (c) 1994 James Clark, 2000 Matthias Clasen
// See the file COPYING for copying permission.

#ifndef XcharMap_DEF_INCLUDED
#define XcharMap_DEF_INCLUDED 1

#include <stddef.h>

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

template<class T>
SharedXcharMap<T>::SharedXcharMap()
{
}

template<class T>
SharedXcharMap<T>::SharedXcharMap(T defaultValue)
{
  for (size_t i = 0; i < sizeof(v)/sizeof(v[0]); i++)
    v[i] = defaultValue;
}

template<class T>
XcharMap<T>::XcharMap()
: ptr_(0)
{
}

template<class T>
XcharMap<T>::XcharMap(T defaultValue)
: sharedMap_(new SharedXcharMap<T>(defaultValue))
#ifdef SP_MULTI_BYTE
,hiMap_(new CharMapResource<T>(defaultValue))
#endif
{
  ptr_ = sharedMap_->ptr();
}

template<class T>
void XcharMap<T>::setRange(Char min, Char max, T val)
{
  if (min <= max) {
    do {
#ifdef SP_MULTI_BYTE
      if (min == 0x10000) {
	hiMap_->setRange(min, max, val);
	return;
      }
#endif
      ptr_[min] = val;
    } while (min++ != max);
  }
}

#ifdef SP_NAMESPACE
}
#endif

#endif /* not XcharMap_DEF_INCLUDED */
