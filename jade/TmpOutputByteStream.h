#ifndef TmpOutputByteStream_INCLUDED
#define TmpOutputByteStream_INCLUDED 1

#include "OutputByteStream.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

#ifdef SP_NAMESPACE
using namespace SP_NAMESPACE;
#endif

class TmpOutputByteStream2 : public OutputByteStream {
public:
  struct Block;
  class Iter {
  public:
    Iter(const TmpOutputByteStream2 &sb) : block_(sb.head_), lastBlockUsed_(sb.lastBlockUsed()) { }
    bool next(const char *&p, size_t &n) {
      if (block_) {
	p = block_->buf;
	n = block_->next ? (size_t)TmpOutputByteStream2::bufSize : lastBlockUsed_;
	block_ = block_->next;
	return 1;
      }
      else
	return 0;
    }
  private:
    Block *block_;
    size_t lastBlockUsed_;
  };
  TmpOutputByteStream2() : head_(0), last_(0), nFullBlocks_(0) {}
  ~TmpOutputByteStream2()
  {
    while (head_) {
      Block *tem = head_;
      head_ = head_->next;
      delete tem;
    }
  }
  bool isEmpty() { return ( head_ == NULL ) ? true : false; }
  void flush() {}
  void flushBuf(char ch)
  {
    Block *tem = new Block;
    char *p = tem->buf;
    *p++ = ch;
    ptr_ = p;
    end_ = tem->buf + bufSize;
    tem->next = 0;
    if (last_) {
      nFullBlocks_++;
      last_->next = tem;
    }
    else
      head_ = tem;
      last_ = tem;
  }  
  enum { bufSize = 1024 };
private:
  friend class Iter;
  struct Block {
    Block *next;
    char buf[bufSize];
  };
  size_t lastBlockUsed() const {
    return last_ ? (ptr_ - last_->buf) : 0;
  }
  unsigned nFullBlocks_;
  Block *head_;
  Block *last_;
};

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* not OutputByteStream_INCLUDED */
