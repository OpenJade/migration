// Copyright (c) 1994 James Clark
// See the file COPYING for copying permission.

#ifndef Recognizer_INCLUDED
#define Recognizer_INCLUDED 1
#ifdef __GNUG__
#pragma interface
#endif

#include "Resource.h"
#include "Owner.h"
#include "XcharMap.h"
#include "types.h"
#include <vector>
#include "Trie.h"

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

class Messenger;
class InputSource;

class Recognizer : public Resource {
public:
  Recognizer(Trie *, const XcharMap<EquivCode> &);
  Recognizer(Trie *, const XcharMap<EquivCode> &, std::vector<Token> &);
  ~Recognizer();
  Token recognize(InputSource *, Messenger &) const;
private:
  Recognizer(const Recognizer &); // undefined
  void operator=(const Recognizer &); // undefined
  Boolean multicode_;
  Owner<Trie> trie_;
  XcharMap<EquivCode> map_;
  std::vector<Token> suppressTokens_;
};

#ifdef SP_NAMESPACE
}
#endif

#endif /* not Recognizer_INCLUDED */
