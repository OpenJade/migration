// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#ifndef VM_INCLUDED
#define VM_INCLUDED 1

#include "Collector.h"
#include "EvalContext.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

class ELObj;
class Interpreter;
class Insn;

class VM : public EvalContext, private Collector::DynamicRoot  {
public:
  VM(Interpreter &);
  VM(EvalContext &, Interpreter &);
  virtual ~VM();
  ELObj **sp;
  Interpreter *interp;
  ELObj **closure;
  ELObj *protectClosure;
  ELObj **frame;
  int nActualArgs;
  ELObj *eval(const Insn *, ELObj **display = 0, ELObj *arg = 0);
  void initStack();
  void needStack(int);
  void pushFrame(const Insn *next, int argsPushed);
  const Insn *popFrame();
  void trace(Collector &) const;
  Vector<const ProcessingMode *> modeStack;
private:
  void growStack(int);
  void init();

  ELObj **slim;
  ELObj **sbase;

  struct ControlStackEntry {
    int frameSize;		// before pushing args
    ELObj **closure;
    ELObj *protectClosure;
    const Insn *next;
  };

  ControlStackEntry *csp;
  ControlStackEntry *csbase;
  ControlStackEntry *cslim;
};

inline
void VM::needStack(int n)
{
  if (slim - sp < n)
    growStack(n);
}

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* not VM_INCLUDED */
