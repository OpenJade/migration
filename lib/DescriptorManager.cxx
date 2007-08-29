// Copyright (c) 1994 James Clark
// See the file COPYING for copying permission.

#include "splib.h"
#include "DescriptorManager.h"
#include <list>

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

DescriptorUser::DescriptorUser(DescriptorManager *manager)
: manager_(manager)
{
  if (manager_)
    manager_->addUser(this);
}

DescriptorUser::~DescriptorUser()
{
  if (manager_)
    manager_->removeUser(this);
}

void DescriptorUser::managerDeleted()
{
  manager_ = 0;
}

Boolean DescriptorUser::suspend()
{
  return 0;
}

void DescriptorUser::acquireD()
{
  if (manager_)
    manager_->acquireD();
}

void DescriptorUser::releaseD()
{
  if (manager_)
    manager_->releaseD();
}

DescriptorManager::DescriptorManager(int maxD)
: maxD_(maxD), usedD_(0)
{
}

DescriptorManager::~DescriptorManager()
{
  for ( std::list<DescriptorUser *>::iterator iter = users_.begin();
       iter != users_.end();
       ++iter )
    (*iter)->managerDeleted();
}

void DescriptorManager::addUser(DescriptorUser *p)
{
  users_.push_front(p);
}

void DescriptorManager::removeUser(DescriptorUser *p)
{
  users_.remove(p);
}

void DescriptorManager::acquireD()
{
  if (usedD_ >= maxD_) {
    for ( std::list<DescriptorUser *>::iterator iter = users_.begin();
	 iter != users_.end();
	 ++iter ) {
      if ( (*iter)->suspend())
	break;
    }
  }
  usedD_++;
}

void DescriptorManager::releaseD()
{
  usedD_--;
}

#ifdef SP_NAMESPACE
}
#endif
