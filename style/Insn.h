// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#ifndef Insn_INCLUDED
#define Insn_INCLUDED 1

#include "ELObj.h"
#include "Resource.h"
#include "Ptr.h"
#include "Location.h"
#include "Message.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

class VM;
class EvalContext;
class Interpreter;

class Insn : public Resource {
public:
  virtual ~Insn();
  virtual const Insn *execute(VM &) const = 0;
  virtual bool isReturn(int &nArgs) const;
  virtual bool isPopBindings(int &n, ConstPtr<Insn> &) const;
};

typedef ConstPtr<Insn> InsnPtr;

class ErrorInsn : public Insn {
public:
  const Insn *execute(VM &) const;
};

class CondFailInsn : public ErrorInsn {
public:
  CondFailInsn(const Location &loc);
  const Insn *execute(VM &) const;
private:
  Location loc_;
};

class CaseFailInsn : public ErrorInsn {
public:
  CaseFailInsn(const Location &loc);
  const Insn *execute(VM &) const;
private:
  Location loc_;
};

class ConstantInsn : public Insn {
public:
  ConstantInsn(ELObj *, InsnPtr);
  const Insn *execute(VM &) const;
private:
  ELObj *value_;
  InsnPtr next_;
};

class ResolveQuantitiesInsn : public Insn {
public:
  ResolveQuantitiesInsn(const Location &, InsnPtr);
  const Insn *execute(VM &) const;
private:
  Location loc_;
  InsnPtr next_;
};

class TestInsn : public Insn {
public:
  TestInsn(InsnPtr, InsnPtr);
  const Insn *execute(VM &) const;
private:
  InsnPtr consequent_;
  InsnPtr alternative_;
};

class OrInsn : public Insn {
public:
  OrInsn(InsnPtr nextTest, InsnPtr next);
  const Insn *execute(VM &) const;
private:
  InsnPtr nextTest_;
  InsnPtr next_;
};

class AndInsn : public Insn {
public:
  AndInsn(InsnPtr nextTest, InsnPtr next);
  const Insn *execute(VM &) const;
private:
  InsnPtr nextTest_;
  InsnPtr next_;
};

class CaseInsn : public Insn {
public:
  CaseInsn(ELObj *, InsnPtr match, InsnPtr fail);
  const Insn *execute(VM &) const;
private:
  ELObj *obj_;
  InsnPtr match_;
  InsnPtr fail_;
};

class PopInsn : public Insn {
public:
  PopInsn(InsnPtr next);
  const Insn *execute(VM &) const;
private:
  InsnPtr next_;
};

class ConsInsn : public Insn {
public:
  ConsInsn(InsnPtr next);
  const Insn *execute(VM &) const;
private:
  InsnPtr next_;
};

class AppendInsn : public Insn {
public:
  AppendInsn(const Location &, InsnPtr next);
  const Insn *execute(VM &) const;
private:
  Location loc_;
  InsnPtr next_;
};

class ApplyBaseInsn : public Insn {
public:
  ApplyBaseInsn(int nArgs, const Location &);
protected:
  FunctionObj *decodeArgs(VM &) const;
  Location loc_;
  int nArgs_;
};

class ApplyInsn : public ApplyBaseInsn {
public:
  ApplyInsn(int nArgs, const Location &, InsnPtr);
  const Insn *execute(VM &) const;
private:
  InsnPtr next_;
};

class TailApplyInsn : public ApplyBaseInsn {
public:
  TailApplyInsn(int nCallerArgs, int nArgs, const Location &);
  const Insn *execute(VM &) const;
private:
  int nCallerArgs_;
};

class FrameRefInsn : public Insn {
public:
  FrameRefInsn(int index, InsnPtr next);
  const Insn *execute(VM &vm) const;
private:
  int index_;
  InsnPtr next_;
};

class StackRefInsn : public Insn {
public:
  StackRefInsn(int index, int frameIndex, InsnPtr next);
  const Insn *execute(VM &vm) const;
private:
  int index_;			// always negative
  int frameIndex_;
  InsnPtr next_;
};

class ClosureRefInsn : public Insn {
public:
  ClosureRefInsn(int index, InsnPtr next);
  const Insn *execute(VM &vm) const;
private:
  int index_;
  InsnPtr next_;
};

class TopRefInsn : public Insn {
public:
  TopRefInsn(const Identifier *var, InsnPtr next);
  const Insn *execute(VM &vm) const;
private:
  const Identifier *var_;
  InsnPtr next_;
  Location loc_;
};

class PopBindingsInsn : public Insn {
public:
  const Insn *execute(VM &vm) const;
  bool isPopBindings(int &n, InsnPtr &) const;
  static InsnPtr make(int n, InsnPtr next);
private:
  PopBindingsInsn(int n, InsnPtr next);
  int n_;
  InsnPtr next_;
};

class PrimitiveObj;

class PrimitiveCallInsn : public Insn {
public:
  PrimitiveCallInsn(int nArgs, PrimitiveObj *, const Location &, InsnPtr);
  const Insn *execute(VM &) const;
private:
  int nArgs_;
  PrimitiveObj *prim_;
  Location loc_;
  InsnPtr next_;
};

struct Signature {
  int nRequiredArgs;
  int nOptionalArgs;
  bool restArg;
  int nKeyArgs;
  const Identifier *const *keys;
};

// This Insn constructs a ClosureObj.

class ClosureInsn : public Insn {
public:
  ClosureInsn(const Signature *, InsnPtr code, int displayLength, InsnPtr next);
  const Insn *execute(VM &) const;
private:
  const Signature *sig_;
  InsnPtr code_;
  int displayLength_;
  InsnPtr next_;
};

class ClosureObj;

class FunctionCallInsn : public Insn {
public:
  FunctionCallInsn(int nArgs, FunctionObj *, const Location &, InsnPtr);
  const Insn *execute(VM &) const;
private:
  int nArgs_;
  FunctionObj *function_;		// must be permanent
  Location loc_;
  InsnPtr next_;
};

class FunctionTailCallInsn : public Insn {
public:
  FunctionTailCallInsn(int nArgs, FunctionObj *, const Location &,
		       int nCallerArgs);
  const Insn *execute(VM &) const;
private:
  int nArgs_;
  FunctionObj *function_;		// must be permanent
  Location loc_;
  int nCallerArgs_;
};

class VarargsInsn : public Insn {
public:
  VarargsInsn(const Signature &, Vector<InsnPtr> &entryPoints,
	      const Location &);
  const Insn *execute(VM &) const;
private:
  const Signature *sig_;
  Vector<InsnPtr> entryPoints_;
  Location loc_;
};

class SetKeyArgInsn : public Insn {
public:
  SetKeyArgInsn(int offset, InsnPtr);
  const Insn *execute(VM &) const;
private:
  int offset_;
  InsnPtr next_;
};

class TestUnspecifiedInsn : public Insn {
public:
  TestUnspecifiedInsn(int offset,
		      InsnPtr ifUnspecified, InsnPtr ifNotUnspecified);
  const Insn *execute(VM &) const;
private:
  int offset_;
  InsnPtr ifUnspecified_;
  InsnPtr ifNotUnspecified_;
  InsnPtr next_;
};

class ReturnInsn : public Insn {
public:
  ReturnInsn(int totalArgs);
  const Insn *execute(VM &) const;
  bool isReturn(int &nArgs) const;
private:
  int totalArgs_;
};

class FunctionObj : public ELObj {
public:
  FunctionObj(const Signature *sig) : sig_(sig) { }
  int totalArgs();
  int nRequiredArgs();
  int nOptionalArgs();
  int nKeyArgs();
  bool restArg();
  virtual const Insn *call(VM &vm, const Location &, const Insn *next) = 0;
  virtual const Insn *tailCall(VM &vm, const Location &, int nCallerArgs) = 0;
  virtual InsnPtr makeCallInsn(int nArgs, Interpreter &, const Location &,
			       InsnPtr next);
  virtual InsnPtr makeTailCallInsn(int nArgs, Interpreter &,
				   const Location &, int nCallerArgs);
  const Signature &signature() const { return *sig_; }
  FunctionObj *asFunction();
private:
  const Signature *sig_;
};

class PrimitiveObj : public FunctionObj {
public:
  PrimitiveObj(const Signature *sig) : FunctionObj(sig) { }
  const Insn *call(VM &vm, const Location &, const Insn *next);
  const Insn *tailCall(VM &vm, const Location &, int nCallerArgs);
  InsnPtr makeCallInsn(int nArgs, Interpreter &, const Location &, InsnPtr next);
  virtual ELObj *primitiveCall(int nArgs, ELObj **args, EvalContext &, Interpreter &,
			       const Location &) = 0;
  void setIdentifier(const Identifier *ident);
protected:
  ELObj *argError(Interpreter &, const Location &,
		  const MessageType3 &, unsigned, ELObj *) const;
  ELObj *noCurrentNodeError(Interpreter &, const Location &) const;
private:
  const Identifier *ident_;
};

class ApplyPrimitiveObj : public FunctionObj {
public:
  ApplyPrimitiveObj();
  const Insn *call(VM &vm, const Location &, const Insn *next);
  const Insn *tailCall(VM &vm, const Location &, int nCallerArgs);
private:
  bool shuffle(VM &vm, const Location &loc);
  static const Signature signature_;
};

class ClosureObj : public FunctionObj {
public:
  void *operator new(size_t, Collector &c) {
    return c.allocateObject(1);
  }
  ClosureObj(const Signature *, InsnPtr, ELObj **display);
  ~ClosureObj() { delete [] display_; }
  ELObj **display();
  const Insn *call(VM &, const Location &, const Insn *);
  const Insn *tailCall(VM &, const Location &, int nCallerArgs);
  void traceSubObjects(Collector &) const;
  ELObj *display(int) const;
private:
  InsnPtr code_;
  // terminated by null pointer.
  // null if empty.
  ELObj **display_;
};

class BoxObj : public ELObj {
public:
  BoxObj();
  BoxObj *asBox();
  void traceSubObjects(Collector &) const;
  ELObj *value;
};

class MakeBoxesInsn : public Insn {
public:
  MakeBoxesInsn(int n, InsnPtr next);
  const Insn *execute(VM &) const;
private:
  int n_;
  InsnPtr next_;
};

class SetBoxesInsn : public Insn {
public:
  SetBoxesInsn(int n, InsnPtr next);
  const Insn *execute(VM &vm) const;
private:
  int n_;
  InsnPtr next_;
};

class UnboxInsn : public Insn {
public:
  UnboxInsn(const Identifier *ident, const Location &, InsnPtr next);
  const Insn *execute(VM &vm) const;
private:
  const Identifier *ident_;
  Location loc_;
  InsnPtr next_;
};

inline
int FunctionObj::nRequiredArgs()
{
  return signature().nRequiredArgs;
}

inline
int FunctionObj::nOptionalArgs()
{
  return signature().nOptionalArgs;
}

inline
bool FunctionObj::restArg()
{
  return signature().restArg;
}

inline
int FunctionObj::nKeyArgs()
{
  return signature().nKeyArgs;
}

inline
int FunctionObj::totalArgs()
{
  const Signature &sig = signature();
  return sig.nRequiredArgs + sig.nOptionalArgs + sig.nKeyArgs + sig.restArg;
}

inline
void PrimitiveObj::setIdentifier(const Identifier *ident)
{
  ident_ = ident;
}

inline
ELObj *ClosureObj::display(int i) const
{
  return display_[i];
}

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* not Insn_INCLUDED */
