// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#ifndef threads_INCLUDED
#define threads_INCLUDED 1

// Thread primitives.

#ifdef _MSC_VER
#ifdef _MT
#define WIN32_THREADS
// If you don't want blocking, define SP_NO_BLOCK
#ifndef SP_NO_BLOCK
#define WIN32_BLOCKING
#define SP_ThreadLocalStorage __declspec(thread)
#endif /* not SP_NO_BLOCK */
#endif /* _MT */
#endif /* _MSC_VER */

#ifdef WIN32_THREADS

#define STRICT 1
#include "windows.h"
#include "process.h"
// <windows.h> appears to turn these warnings back on
#ifdef _MSC_VER
#pragma warning ( disable : 4237 )
#endif

#define THREAD_CALLING_CONVENTION __stdcall

class Thread {
public:
  typedef unsigned ReturnType;
  Thread(ReturnType (THREAD_CALLING_CONVENTION *func)(void *), void *args) {
    unsigned threadId;
    handle_ = HANDLE(_beginthreadex(NULL, 0, func, args, 0, &threadId));
  }
  int wait() {
    if (WaitForSingleObject(handle_, INFINITE) != WAIT_OBJECT_0)
      abort();
    unsigned long ret;
    if (!GetExitCodeThread(handle_, &ret))
      abort();
    if (!CloseHandle(handle_))
      abort();
    return int(ret);
  }
private:
  HANDLE handle_;
};


class Mutex {
public:
  class Lock {
  public:
    Lock(Mutex *mp) : mp_(mp) {
      if (mp) EnterCriticalSection(&mp->cs_);
    }
    ~Lock() {
      if (mp_) LeaveCriticalSection(&mp_->cs_);
    }
  private:
    Mutex *mp_;
  };
  Mutex() {
    InitializeCriticalSection(&cs_);
  }
  ~Mutex() {
    DeleteCriticalSection(&cs_);
  }
  friend class Lock;
private:
  CRITICAL_SECTION cs_;
};

// Thread-safe reference counts.

class RefCount {
public:
  RefCount() : count_(0) { }
  void operator++() { InterlockedIncrement(&count_); }
  bool operator--() { return InterlockedDecrement(&count_) != 0; }
  operator unsigned long() const { return count_; }
private:
  long count_;
};

#else /* not WIN32_THREADS */

#define THREAD_CALLING_CONVENTION

class Thread {
public:
  typedef int ReturnType;
  Thread(ReturnType (THREAD_CALLING_CONVENTION *func)(void *), void *args) {
    ret_ = (*func)(args);
  }
  ReturnType wait() { return ret_; }
private:
  ReturnType ret_;
};

class RefCount {
public:
  RefCount() : count_(0) { }
  void operator++() { ++count_; }
  bool operator--() { return --count_ != 0; }
  operator unsigned long() const { return count_; }
private:
  unsigned long count_;
};

class Mutex {
public:
  class Lock {
  public:
    Lock(Mutex *) { }
  };
  Mutex() { }
};

#endif /* not WIN32_THREADS */

#ifdef WIN32_BLOCKING

class Condition {
public:
  // Manual reset event, initially non-signalled
  Condition() : handle_(CreateEvent(NULL, 1, 0, NULL)), nWaiters_(0) { }
  ~Condition() { CloseHandle(handle_); }
  // Set the event to signalled, releasing all threads waiting on it,
  // then set it back to non-signalled.
  // For performance reasons, we want to avoid calling PulseEvent unless we have
  // some thread waiting on this condition.
  // The obvious solution of a boolean variable that's set by wait() and
  // cleared by pulse() fails in the following scenario:
  // reader: needPulse_ = 1;
  // writer: needPulse_ = 0;
  // writer: PulseEvent()
  // reader: WaitForSingleObject()
  void pulse() {
    if (nWaiters_) {
      PulseEvent(handle_);
    }
  }
  // Wait for the event to become signalled.
  bool wait() const {
    InterlockedIncrement((long *)&nWaiters_);
    bool ret = (WaitForSingleObject(handle_, INFINITE) == WAIT_OBJECT_0);
    InterlockedDecrement((long *)&nWaiters_);
    return ret;
  }
  // Set the event to signalled, releasing all threads waiting on it.
  // Don't set it back to non-signalled.  Any thread that subsequently
  // waits will return immediately.
  void set() { SetEvent(handle_); }
private:
  long nWaiters_;
  HANDLE handle_;
};

#else /* not WIN32_BLOCKING */

class Condition {
public:
  Condition() { }
  void pulse() { }
  void set() { }
  bool wait() const { return 0; }
};

#endif /* not WIN32_BLOCKING */

#endif /* not threads_INCLUDED */
