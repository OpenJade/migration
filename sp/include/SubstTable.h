// Copyright (c) 1994 James Clark, 2000 Matthias Clasen
// See the file COPYING for copying permission.

#ifndef SubstTable_INCLUDED
#define SubstTable_INCLUDED

#include <limits.h>
#include "StringC.h"
#include "Boolean.h"

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

class SubstTable {
public:
  SubstTable();
  void addSubst(Char from, Char to);
  void subst(Char &c) const { if (table_.size() > 0) c = table_[c]; }
  void subst(StringC &) const;
  Char operator[](Char c) const { return table_.size() > 0 ? table_[c] : c; }
  StringC inverse(Char c) const;
  void inverseTable(SubstTable &) const;
private:
  StringC table_;
  mutable StringC pairs_;
  mutable Boolean pairsValid_;
};

#ifdef SP_NAMESPACE
}
#endif

#endif /* SubstTable_INCLUDED */
