// Copyright (c) 1994 James Clark
// See the file COPYING for copying permission.

#ifdef __GNUG__
#pragma implementation
#endif
#include "splib.h"
#include "Partition.h"
#include "ISet.h"
#include "ISetIter.h"
#include "SubstTable.h"
#include <list>
#include "Owner.h"
#include "macros.h"
#include "EquivClass.h"
#include "constant.h"
#include "StringC.h"

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

static void refineByChar(std::list<EquivClass *> *, Char);
static void refineBySet(std::list<EquivClass *> *, const ISet<Char> &, unsigned);

#if _MSC_VER == 900
// Work around MSVC 2.0 bug.
typedef SubstTable _msvc_dummy;
#endif

Partition::Partition(const ISet<Char> &chars,
		     const ISet<Char> **sets,
		     int nSets,
		     const SubstTable &subst)
: map_(0)			// eE gets code 0
{
  std::list<EquivClass *> classes;
  classes.push_front(new EquivClass);
  classes.front()->set.addRange(0, charMax);

  {
    ISetIter<Char> iter(chars);
    Char min, max;
    while (iter.next(min, max)) {
      do {
	refineByChar(&classes, subst[min]);
      } while (min++ != max);
    }
  }

  int i;
  for (i = 0; i < nSets; i++)
    refineBySet(&classes, *sets[i], (1 << i));

  maxCode_ = 0;

  setCodes_.resize(nSets);

  for ( std::list<EquivClass*>::iterator listIter = classes.begin();
       listIter != classes.end();
       ++listIter ) {
    ++maxCode_;
    ASSERT(maxCode_ != 0);
    EquivClass *p = *listIter;
    for (i = 0; i < nSets; i++)
      if ((1 << i) & p->inSets)
	setCodes_[i] += maxCode_;
    ISetIter<Char> setIter(p->set);
    Char min, max;
    while (setIter.next(min, max))
      map_.setRange(min, max, maxCode_);
  }

  {
    ISetIter<Char> iter(chars);
    Char min, max;
    while (iter.next(min, max)) {
      do {
	StringC str(subst.inverse(min));
	EquivCode code = map_[min];
	for (size_t i = 0; i < str.size(); i++)
	  map_.setChar(str[i], code);
      } while (min++ != max);
    }
  }
}

static
void refineByChar(std::list<EquivClass *> *classes, Char c)
{
  // Avoid modifying *classes, while there's an active iter on it.
  EquivClass *found = 0;
  {
    for ( std::list<EquivClass *>::iterator iter = classes->begin(); iter != classes->end(); ++iter ) {
      if ( (*iter)->set.contains(c)) {
	found = *iter;
	break;
      }
    }
  }
  if (found && !found->set.isSingleton()) {
    found->set.remove(c);
    classes->push_front(new EquivClass(found->inSets));
    classes->front()->set.add(c);
  }
}

static
void addUpTo(ISet<Char> *to, Char limit, const ISet<Char> &from)
{
  ISetIter<Char> iter(from);
  Char min, max;
  while (iter.next(min, max) && min < limit)
    to->addRange(min, max >= limit ? limit - 1 : max);
}

enum RefineResult { allIn, allOut, someInSomeOut };

static
RefineResult refine(const ISet<Char> &set, const ISet<Char> &refiner,
		    ISet<Char> *inp, ISet<Char> *outp)
{
  Char setMin, setMax, refMin, refMax;
  ISetIter<Char> refIter(refiner);
  ISetIter<Char> setIter(set);
  Boolean oneIn = 0;
  Boolean oneOut = 0;

  if (!refIter.next(refMin, refMax))
    return allOut;
  while (setIter.next(setMin, setMax)) {
    while (setMin <= setMax) {
      while (refMax < setMin && refIter.next(refMin, refMax))
	;
      if (refMax < setMin || setMin < refMin) {
	if (!oneOut) {
	  if (oneIn)
	    addUpTo(inp, setMin, set);
	  oneOut = 1;
	}
	if (refMax < setMin || refMin > setMax) {
	  if (oneIn)
	    outp->addRange(setMin, setMax);
	  break;
	}
	else {
	  if (oneIn)
	    outp->addRange(setMin, refMin - 1);
	  setMin = refMin;
	}
      }
      else {
	if (!oneIn) {
	  if (oneOut)
	    addUpTo(outp, setMin, set);
	  oneIn = 1;
	}
	if (setMax <= refMax) {
	  if (oneOut)
	    inp->addRange(setMin, setMax);
	  break;
	}
	else {
	  // refMax < setMax
	  if (oneOut)
	    inp->addRange(setMin, refMax);
	  // avoid wrapping round
	  if (refMax == charMax)
	    break;
	  setMin = refMax + 1;
	}
      }
    }
  }
  if (oneIn)
    return oneOut ? someInSomeOut : allIn;
  else
    return allOut;
}

static
void refineBySet(std::list<EquivClass *> *classes, const ISet<Char> &set,
		 unsigned setFlag)
{
  Owner<EquivClass> in(new EquivClass), out(new EquivClass);
  std::list<EquivClass *> newClasses;
  for (;;) {
    EquivClass *p = classes->front();
    if (!p)
      break;
    if (!out)
      out = new EquivClass;
    switch (refine(p->set, set, &in->set, &out->set)) {
    case someInSomeOut:
      in->inSets = p->inSets | setFlag;
      newClasses.push_front(in.extract());
      out->inSets = p->inSets;
      newClasses.push_front(out.extract());
      in = classes->front();
      classes->pop_front();
      in->set.clear();
      in->inSets = 0;
      break;
    case allIn:
      p->inSets |= setFlag;
      newClasses.push_front(classes->front());
      classes->pop_front();
      break;
    case allOut:
      newClasses.push_front(classes->front());
      classes->pop_front();
      break;
    }
  }
  classes->swap(newClasses);
}

#ifdef SP_NAMESPACE
}
#endif
