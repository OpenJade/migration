// Copyright (c) 1994 James Clark
// See the file COPYING for copying permission.

#ifndef EventQueue_INCLUDED
#define EventQueue_INCLUDED 1

#include <deque>
#include "Event.h"

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

class EventQueue : public EventHandler {
public:
  EventQueue();

  bool empty() const
  { return _queue.empty(); }
  
  Event *front() const
  { return _queue.front(); }

  void pop_front()
  { _queue.pop_front(); }

  void swap( std::deque<Event *>& q )
  { _queue.swap( q ); }

  void clear()
  { _queue.clear(); }

  void append(Event *);

private:
#define EVENT(c, f) void f(c *);
#include "events.h"
#undef EVENT
  
  std::deque<Event *> _queue;
};

class Pass1EventHandler : public EventQueue {
public:
  Pass1EventHandler();
  void init(EventHandler *origHandler);
  void message(MessageEvent *);
  Boolean hadError() const;
  EventHandler *origHandler() const;
private:
  Boolean hadError_;
  EventHandler *origHandler_;
};

inline
void EventQueue::append(Event *event)
{
  _queue.push_back(event);
}

inline
Boolean Pass1EventHandler::hadError() const
{
  return hadError_;
}

inline
EventHandler *Pass1EventHandler::origHandler() const
{
  return origHandler_;
}

#ifdef SP_NAMESPACE
}
#endif

#endif /* not EventQueue_INCLUDED */
