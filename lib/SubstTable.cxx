// Copyright (c) 1994 James Clark, 2000 Matthias Clasen
// See the file COPYING for copying permission.

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

SubstTable::SubstTable()
: pairsValid_(1)
{
}

void SubstTable::addSubst(Char from, Char to)
{
  if (table_.size() == 0) {
    table_.resize(Char(-1) + 1);
#if _MSC_VER == 1100
    // Workaround for Visual C++ 5.0 bug.
    Char n = 0;
    int i = 0;
    while (i < Char(-1) + 1)
      table_[i++] = n++;
#else
    for (int i = 0; i < Char(-1) + 1; i++)
      table_[i] = i;
#endif
  }
  if (table_[from] != to)
    pairsValid_ = 0;
  table_[from] = to;
}

StringC SubstTable::inverse(Char ch) const
{
  if (!pairsValid_) {
    const Char *p = table_.data();
    size_t length = table_.size();
    for (size_t i = 0; i < length; i++)
      if (p[i] != i) {
	// FIXME use mutable if available
#ifndef HAVE_MUTABLE
	((SubstTable *)this)->
#endif
	  pairs_ += Char(i);
#ifndef HAVE_MUTABLE
	((SubstTable *)this)->
#endif
	  pairs_ += p[i];
      }
#ifndef HAVE_MUTABLE
    ((SubstTable *)this)->
#endif
      pairsValid_ = 1;
  }
  const Char *p = pairs_.data();
  if (!p)
    return StringC(&ch, 1);
  StringC result;
  if (table_[ch] == ch)
    result += ch;
  for (size_t n = pairs_.size(); n > 0; n -= 2, p += 2)
    if (p[1] == ch)
      result += p[0];
  return result;
}

void SubstTable::inverseTable(SubstTable &inv) const
{
  if (table_.size() == 0) {
    inv.table_.resize(0);
    inv.pairs_.resize(0);
    inv.pairsValid_ = 1;
  }
  else {
    if (inv.table_.size() == 0)
      inv.table_.resize(Char(-1) + 1);
    int i;
    for (i = 0; i < Char(-1) + 1; i++)
      inv.table_[i] = i;
    inv.pairs_.resize(0);
    inv.pairsValid_ = 0;
    for (i = 0; i < Char(-1) + 1; i++)
      if (table_[i] != i)
	inv.table_[table_[i]] = i;
  }
}

void SubstTable::subst(StringC &str) const
{
  for (size_t i = 0; i < str.size(); i++)
    subst(str[i]);
}

#ifdef SP_NAMESPACE
}
#endif

