// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#include "stylelib.h"
#include "VM.h"
#include "Insn.h"
#include "Interpreter.h"
#include "InterpreterMessages.h"
#include "ELObjMessageArg.h"
#include "Expression.h"
#include "macros.h"
#include "Insn2.h"
#include "SosofoObj.h"
#include <string.h>
#include <stddef.h>
#include <stdio.h>

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif


VM::VM(Interpreter &interpreter)
: Collector::DynamicRoot(interpreter), interp(&interpreter),
  slim(0), sbase(0), sp(0), closure(0), frame(0), protectClosure(0),
  csp(0), cslim(0), csbase(0)
{
}

VM::~VM()
{
  delete [] sbase;
  delete [] csbase;
}

void VM::initStack()
{
  sp = sbase;
  frame = sbase;
  csp = csbase;
  modeStack.resize(0);
}

void VM::growStack(int n)
{
  size_t newSize = sp - sbase;
  if (n > newSize)
    newSize += (n + 15) & ~15;
  else
    newSize += newSize;
  ELObj **newStack = new ELObj *[newSize];
  slim = newStack + newSize;
  memcpy(newStack, sbase, (sp - sbase)*sizeof(ELObj *));
  sp = newStack + (sp - sbase);
  frame = newStack + (frame - sbase);
  if (sbase)
    delete [] sbase;
  sbase = newStack;
}

void VM::trace(Collector &c) const
{
  if (sp) {
    for (ELObj **p = sbase; p != sp; p++)
      c.trace(*p);
  }
  for (ControlStackEntry *p = csbase; p != csp; p++)
    c.trace(p->protectClosure);
  c.trace(protectClosure);
}

void VM::pushFrame(const Insn *next, int argsPushed)
{
  if (csp >= cslim) {
    size_t newSize = csbase ? (cslim - csbase)*2 : 8;
    ControlStackEntry *newBase = new ControlStackEntry[newSize];
    cslim = newBase + newSize;
    memcpy(newBase, csbase, (csp - csbase)*sizeof(ControlStackEntry));
    csp = newBase + (csp - csbase);
    if (csbase)
      delete [] csbase;
    csbase = newBase;
  }
  csp->closure = closure;
  csp->protectClosure = protectClosure;
  csp->next = next;
  csp->frameSize = sp - frame - argsPushed;
  csp++;
}

const Insn *VM::popFrame()
{
  ASSERT(csp > csbase);
  --csp;
  closure = csp->closure;
  protectClosure = csp->protectClosure;
  frame = sp - csp->frameSize;
  return csp->next;
}

ELObj *VM::eval(const Insn *insn, ELObj **display, ELObj *arg)
{
  initStack();
  if (arg) {
    needStack(1);
    *sp++ = arg;
  }
  closure = display;
  protectClosure = 0;
  // The inner loop.
  while (insn)
    insn = insn->execute(*this);
  ELObj *result;
  if (sp) {
    sp--;
    ASSERT(sp == sbase);
    ASSERT(csp == csbase);
    result = *sp;
    ASSERT(result != 0);
  }
  else
    result = interp->makeError();
  return result;
}

Insn::~Insn()
{
}

bool Insn::isReturn(int &) const
{
  return false;
}

bool Insn::isPopBindings(int &, InsnPtr &) const
{
  return false;
}

const Insn *ErrorInsn::execute(VM &vm) const
{
  vm.sp = 0;
  return 0;
}

CondFailInsn::CondFailInsn(const Location &loc)
: loc_(loc)
{
}

const Insn *CondFailInsn::execute(VM &vm) const
{
  vm.interp->setNextLocation(loc_);
  vm.interp->message(InterpreterMessages::condFail);
  return ErrorInsn::execute(vm);
}

CaseFailInsn::CaseFailInsn(const Location &loc)
: loc_(loc)
{
}

const Insn *CaseFailInsn::execute(VM &vm) const
{
  vm.interp->setNextLocation(loc_);
  vm.interp->message(InterpreterMessages::caseFail, ELObjMessageArg(vm.sp[-1], *vm.interp));
  return ErrorInsn::execute(vm);
}

ConstantInsn::ConstantInsn(ELObj *obj, InsnPtr next)
: value_(obj), next_(next)
{
}

const Insn *ConstantInsn::execute(VM &vm) const
{
  vm.needStack(1);
  *vm.sp++ = value_;
  return next_.pointer();
}

ResolveQuantitiesInsn::ResolveQuantitiesInsn(const Location &loc, InsnPtr next)
: loc_(loc), next_(next)
{
}

const Insn *ResolveQuantitiesInsn::execute(VM &vm) const
{
  ELObj *tem = vm.sp[-1]->resolveQuantities(1,
					    *vm.interp,
					    loc_);
  ASSERT(tem != 0);
  if (vm.interp->isError(tem)) {
    vm.sp = 0;
    return 0;
  }
  vm.sp[-1] = tem;
  return next_.pointer();
}

TestInsn::TestInsn(InsnPtr consequent, InsnPtr alternative)
: consequent_(consequent), alternative_(alternative)
{
}

const Insn *TestInsn::execute(VM &vm) const
{
  return (*--vm.sp)->isTrue() ? consequent_.pointer() : alternative_.pointer();
}

OrInsn::OrInsn(InsnPtr nextTest, InsnPtr next)
: nextTest_(nextTest), next_(next)
{
}

const Insn *OrInsn::execute(VM &vm) const
{
  if (vm.sp[-1]->isTrue())
    return next_.pointer();
  --vm.sp;
  return nextTest_.pointer();
}

AndInsn::AndInsn(InsnPtr nextTest, InsnPtr next)
: nextTest_(nextTest), next_(next)
{
}

const Insn *AndInsn::execute(VM &vm) const
{
  if (!vm.sp[-1]->isTrue())
    return next_.pointer();
  --vm.sp;
  return nextTest_.pointer();
}

CaseInsn::CaseInsn(ELObj *obj, InsnPtr match, InsnPtr fail)
: obj_(obj), match_(match), fail_(fail)
{
}

const Insn *CaseInsn::execute(VM &vm) const
{
  if (*vm.sp[-1] == *obj_) {
    --vm.sp;
    return match_.pointer();
  }
  return fail_.pointer();
}

PopInsn::PopInsn(InsnPtr next)
: next_(next)
{
}

const Insn *PopInsn::execute(VM &vm) const
{
  --vm.sp;
  return next_.pointer();
}

ApplyBaseInsn::ApplyBaseInsn(int nArgs, const Location &loc)
: nArgs_(nArgs), loc_(loc)
{
}

ApplyInsn::ApplyInsn(int nArgs, const Location &loc, InsnPtr next)
: ApplyBaseInsn(nArgs, loc), next_(next)
{
}

TailApplyInsn::TailApplyInsn(int nCallerArgs, int nArgs, const Location &loc)
: ApplyBaseInsn(nArgs, loc), nCallerArgs_(nCallerArgs)
{
}

// top of stack is operand; next down is last operand and so on

FunctionObj *ApplyBaseInsn::decodeArgs(VM &vm) const
{
  FunctionObj *func = (*--vm.sp)->asFunction();
  if (!func) {
    vm.interp->setNextLocation(loc_);
    vm.interp->message(InterpreterMessages::callNonFunction,
		       ELObjMessageArg(*vm.sp, *vm.interp));
    vm.sp = 0;
    return 0;
  }
  int nReq = func->nRequiredArgs();
  if (nArgs_ < nReq) {
    vm.interp->setNextLocation(loc_);
    vm.interp->message(InterpreterMessages::missingArg);
    vm.sp = 0;
    return 0;
  }
  if (nArgs_ - nReq > func->nOptionalArgs()) {
    if (func->nKeyArgs()) {
      // Keyword args can be specified more than once
      // so we can only check there are an even number.
      if ((nArgs_ - nReq - func->nOptionalArgs()) & 1) {
	vm.interp->setNextLocation(loc_);
	vm.interp->message(InterpreterMessages::oddKeyArgs);
	vm.sp -= (nArgs_ - nReq) - func->nOptionalArgs();
      }
    }
    else if (!func->restArg()) {
      vm.interp->setNextLocation(loc_);
      vm.interp->message(InterpreterMessages::tooManyArgs);
      vm.sp -= (nArgs_ - nReq) - func->nOptionalArgs();
    }
  }
  return func;
}

const Insn *ApplyInsn::execute(VM &vm) const
{
  FunctionObj *func = decodeArgs(vm);
  vm.nActualArgs = nArgs_;
  if (func)
    return func->call(vm, loc_, next_.pointer());
  else
    return 0;
}

const Insn *TailApplyInsn::execute(VM &vm) const
{
  FunctionObj *func = decodeArgs(vm);
  vm.nActualArgs = nArgs_;
  if (func)
    return func->tailCall(vm, loc_, nCallerArgs_);
  else
    return 0;
}

ApplyPrimitiveObj::ApplyPrimitiveObj()
: FunctionObj(&signature_)
{
}

const Signature ApplyPrimitiveObj::signature_ = { 2, 0, 1 };

const Insn *ApplyPrimitiveObj::call(VM &vm, const Location &loc,
				    const Insn *next)
{
  if (!shuffle(vm, loc))
    return 0;
  ApplyInsn insn(vm.nActualArgs, loc, (Insn *)next);
  return insn.execute(vm);
}

bool ApplyPrimitiveObj::shuffle(VM &vm, const Location &loc)
{
  int nArgs = vm.nActualArgs;
  ELObj *func = vm.sp[-nArgs];
  for (int i = nArgs - 2; i > 0; i--)
    vm.sp[-i - 2] = vm.sp[-i - 1];
  vm.nActualArgs = nArgs - 2;
  ELObj *list = *--vm.sp;
  --vm.sp;
  while (!list->isNil()) {
    PairObj *tem = list->asPair();
    if (!tem) {
      vm.interp->setNextLocation(loc);
      vm.interp->message(InterpreterMessages::notAList,
			 StringMessageArg(vm.interp->makeStringC("apply")),
			 OrdinalMessageArg(nArgs),
			 ELObjMessageArg(list, *vm.interp));
      vm.sp = 0;
      return 0;
    }
    vm.needStack(1);
    vm.nActualArgs++;
    *vm.sp++ = tem->car();
    list = tem->cdr();
  }
  vm.needStack(1);
  *vm.sp++ = func;
  return 1;
}

const Insn *ApplyPrimitiveObj::tailCall(VM &vm, const Location &loc,
					int nCallerArgs)
{
  if (!shuffle(vm, loc))
    return 0;
  TailApplyInsn insn(nCallerArgs, vm.nActualArgs, loc);
  return insn.execute(vm);
}

PrimitiveCallInsn::PrimitiveCallInsn(int nArgs, PrimitiveObj *prim,
				     const Location &loc,
				     InsnPtr next)
: nArgs_(nArgs), prim_(prim), loc_(loc), next_(next)
{
}

const Insn *PrimitiveCallInsn::execute(VM &vm) const
{
  if (nArgs_ == 0)
    vm.needStack(1);
  ELObj **argp = vm.sp - nArgs_;
  *argp = prim_->primitiveCall(nArgs_, argp, vm, *vm.interp, loc_);
  vm.sp = argp + 1;
  if (vm.interp->isError(*argp)) {
    vm.sp = 0;
    return 0;
  }
  else
    return next_.pointer();
}

InsnPtr FunctionObj::makeCallInsn(int nArgs, Interpreter &,
				  const Location &loc,
				  InsnPtr next)
{
  return new FunctionCallInsn(nArgs, this, loc, next);
}

InsnPtr FunctionObj::makeTailCallInsn(int nArgs, Interpreter &,
				      const Location &loc, int nCallerArgs)
{
  return new FunctionTailCallInsn(nArgs, this, loc, nCallerArgs);
}

FunctionObj *FunctionObj::asFunction()
{
  return this;
}
  
const Insn *PrimitiveObj::call(VM &vm, const Location &loc,
			       const Insn *next)
{
  if (vm.nActualArgs == 0)
    vm.needStack(1);
  ELObj **argp = vm.sp - vm.nActualArgs;
  *argp = primitiveCall(vm.nActualArgs, argp, vm, *vm.interp, loc);
  vm.sp = argp + 1;
  if (vm.interp->isError(*argp)) {
    vm.sp = 0;
    return 0;
  }
  else
    return next;
}

const Insn *PrimitiveObj::tailCall(VM &vm, const Location &loc,
				   int nCallerArgs)
{
  ELObj **argp = vm.sp - vm.nActualArgs;
  ELObj *result = primitiveCall(vm.nActualArgs, argp, vm, *vm.interp, loc);
  if (vm.interp->isError(result)) {
    vm.sp = 0;
    return 0;
  }
  else {
    vm.sp = argp - nCallerArgs;
    const Insn *next = vm.popFrame();
    vm.needStack(1);
    *vm.sp++ = result;
    return next;
  }
}

InsnPtr PrimitiveObj::makeCallInsn(int nArgs, Interpreter &interp, const Location &loc,
				   InsnPtr next)
{
  return new PrimitiveCallInsn(nArgs, this, loc, next);
}

ELObj *PrimitiveObj::argError(Interpreter &interp,
			      const Location &loc,
			      const MessageType3 &msg,
			      unsigned index,
			      ELObj *obj) const
{
  interp.setNextLocation(loc);
  interp.message(msg,
		 StringMessageArg(ident_->name()),
		 OrdinalMessageArg(index + 1),
		 ELObjMessageArg(obj, interp));
  return interp.makeError();
}

ELObj *PrimitiveObj::noCurrentNodeError(Interpreter &interp,
					const Location &loc) const
{
  interp.setNextLocation(loc);
  interp.message(InterpreterMessages::noCurrentNode);
  return interp.makeError();
}

ClosureInsn::ClosureInsn(const Signature *sig, InsnPtr code, int displayLength,
			 InsnPtr next)
: sig_(sig), code_(code), displayLength_(displayLength), next_(next)
{
}

const Insn *ClosureInsn::execute(VM &vm) const
{
  ELObj **display
    = displayLength_ ? new ELObj *[displayLength_ + 1] : 0;
  ELObj **tem = vm.sp - displayLength_;
  for (int i = 0; i < displayLength_; i++)
    display[i] = tem[i];
  if (displayLength_ == 0) {
    vm.needStack(1);
    tem = vm.sp;
  }
  else
    display[displayLength_] = 0;
  // Make sure objects in display are still visible on the stack
  // to the garbage collector.
  *tem++ = new (*vm.interp) ClosureObj(sig_, code_, display);
  vm.sp = tem;
  return next_.pointer();
}

FunctionCallInsn:: FunctionCallInsn(int nArgs, FunctionObj *function,
				    const Location &loc, InsnPtr next)
: nArgs_(nArgs), function_(function), loc_(loc), next_(next)
{
}

const Insn *FunctionCallInsn::execute(VM &vm) const
{
  vm.nActualArgs = nArgs_;
  return function_->call(vm, loc_, next_.pointer());
}

FunctionTailCallInsn:: FunctionTailCallInsn(int nArgs, FunctionObj *function,
					    const Location &loc,
					    int nCallerArgs)
: nArgs_(nArgs), function_(function), loc_(loc), nCallerArgs_(nCallerArgs)
{
}

const Insn *FunctionTailCallInsn::execute(VM &vm) const
{
  vm.nActualArgs = nArgs_;
  return function_->tailCall(vm, loc_, nCallerArgs_);
}

VarargsInsn::VarargsInsn(const Signature &sig,
			 Vector<InsnPtr> &entryPoints,
			 const Location &loc)
: sig_(&sig), loc_(loc)
{
  entryPoints.swap(entryPoints_);
}

const Insn *VarargsInsn::execute(VM &vm) const
{
  int n = vm.nActualArgs - sig_->nRequiredArgs;
  if ((sig_->restArg || sig_->nKeyArgs)
      && n > entryPoints_.size() - 2) {
    // cons up the rest args
    ELObjDynamicRoot protect(*vm.interp, vm.interp->makeNil());
    for (int i = n - (entryPoints_.size() - 2); i > 0; i--) {
      protect = new (*vm.interp) PairObj(vm.sp[-1], protect);
      --vm.sp;
    }
    vm.needStack(sig_->nKeyArgs + sig_->restArg);
    if (sig_->restArg)
      *vm.sp++ = protect;
    if (sig_->nKeyArgs) {
      for (int i = 0; i < sig_->nKeyArgs; i++)
	vm.sp[i] = vm.interp->makeUnspecified();
      ELObj *tem = protect;
      for (int i = n - (entryPoints_.size() - 2); i > 0; i -= 2) {
	KeywordObj *k = ((PairObj *)tem)->car()->asKeyword();
	tem = ((PairObj *)tem)->cdr();
	if (k) {
	  for (int j = 0; j < sig_->nKeyArgs; j++)
	    if (sig_->keys[j] == k->identifier()) {
	      if (vm.sp[j] == vm.interp->makeUnspecified())
		vm.sp[j] = ((PairObj *)tem)->car();
	      k = 0;
	      break;
	    }
	  if (k && !sig_->restArg) {
	    vm.interp->setNextLocation(loc_);
	    vm.interp->message(InterpreterMessages::invalidKeyArg,
			       StringMessageArg(k->identifier()->name()));
	  }
	}
	else {
	  vm.interp->setNextLocation(loc_);
	  vm.interp->message(InterpreterMessages::keyArgsNotKey);
	}
	tem = ((PairObj *)tem)->cdr();
      }
      vm.sp += sig_->nKeyArgs;
    }
    return entryPoints_.back().pointer();
  }
  return entryPoints_[n].pointer();
}

SetKeyArgInsn::SetKeyArgInsn(int offset, InsnPtr next)
: offset_(offset), next_(next)
{
}

const Insn *SetKeyArgInsn::execute(VM &vm) const
{
  ELObj *val = *--vm.sp;
  vm.sp[offset_] = val;
  return next_.pointer();
}

TestUnspecifiedInsn::TestUnspecifiedInsn(int offset,
					 InsnPtr ifUnspecified,
					 InsnPtr ifNotUnspecified)
: offset_(offset), ifUnspecified_(ifUnspecified),
  ifNotUnspecified_(ifNotUnspecified)
{
}
     
const Insn *TestUnspecifiedInsn::execute(VM &vm) const
{
  if (vm.sp[offset_] == vm.interp->makeUnspecified())
    return ifUnspecified_.pointer();
  else
    return ifNotUnspecified_.pointer();
}

ClosureObj::ClosureObj(const Signature *sig, InsnPtr code, ELObj **display)
: FunctionObj(sig), code_(code), display_(display)
{
  hasSubObjects_ = 1;
}

const Insn *ClosureObj::call(VM &vm, const Location &, const Insn *next)
{
  vm.needStack(1);
  vm.pushFrame(next, vm.nActualArgs);
  vm.frame = vm.sp - vm.nActualArgs;
  vm.closure = display_;
  vm.protectClosure = this;
  return code_.pointer();
}

const Insn *ClosureObj::tailCall(VM &vm, const Location &, int nCallerArgs)
{
  vm.needStack(1);
  int nArgs = vm.nActualArgs;
  if (nCallerArgs) {
    ELObj **oldFrame = vm.sp - nArgs;
    ELObj **newFrame = oldFrame - nCallerArgs;
    for (int i = 0; i < nArgs; i++)
      newFrame[i] = oldFrame[i];
    vm.frame = newFrame;
    vm.sp = newFrame + nArgs;
  }
  else
    vm.frame = vm.sp - nArgs;
  vm.closure = display_;
  vm.protectClosure = this;
  return code_.pointer();
}

void ClosureObj::traceSubObjects(Collector &c) const
{
  if (display_) {
    for (ELObj **p = display_; *p; p++)
      c.trace(*p);
  }
}

ReturnInsn::ReturnInsn(int totalArgs)
: totalArgs_(totalArgs)
{
}

bool ReturnInsn::isReturn(int &nArgs) const
{
  nArgs = totalArgs_;
  return true;
}

const Insn *ReturnInsn::execute(VM &vm) const
{
  ELObj *result = *--vm.sp;
  vm.sp -= totalArgs_;
  const Insn *next = vm.popFrame();
  *vm.sp++ = result;
  return next;
}

FrameRefInsn::FrameRefInsn(int index, InsnPtr next)
: index_(index), next_(next)
{
}

const Insn *FrameRefInsn::execute(VM &vm) const
{
  vm.needStack(1);
  *vm.sp++ = vm.frame[index_];
  return next_.pointer();
}

StackRefInsn::StackRefInsn(int index, int frameIndex, InsnPtr next)
: index_(index), frameIndex_(frameIndex), next_(next)
{
}

const Insn *StackRefInsn::execute(VM &vm) const
{
  vm.needStack(1);
  ASSERT(vm.sp - vm.frame == frameIndex_ - index_);
  *vm.sp = vm.sp[index_];
  vm.sp += 1;
  return next_.pointer();
}

ClosureRefInsn::ClosureRefInsn(int index, InsnPtr next)
: index_(index), next_(next)
{
}

const Insn *ClosureRefInsn::execute(VM &vm) const
{
  vm.needStack(1);
  *vm.sp++ = vm.closure[index_];
  return next_.pointer();
}

TopRefInsn::TopRefInsn(const Identifier *var, InsnPtr next)
: var_(var), next_(next)
{
}

const Insn *TopRefInsn::execute(VM &vm) const
{
  ELObj *tem = var_->computeValue(1, *vm.interp);
  if (vm.interp->isError(tem)) {
    vm.sp = 0;
    return 0;
  }
  else {
    vm.needStack(1);
    *vm.sp++ = tem;
    return next_.pointer();
  }
}

InsnPtr PopBindingsInsn::make(int n, InsnPtr next)
{
  if (!next.isNull()) {
    int i;
    if (next->isReturn(i))
      return new ReturnInsn(n + i);
    if (next->isPopBindings(i, next))
      return new PopBindingsInsn(n + i, next);
  }
  return new PopBindingsInsn(n, next);
}

PopBindingsInsn::PopBindingsInsn(int n, InsnPtr next)
: n_(n), next_(next)
{
}

const Insn *PopBindingsInsn::execute(VM &vm) const
{
  vm.sp -= n_;
  vm.sp[-1] = vm.sp[n_ - 1];
  return next_.pointer();
}

bool PopBindingsInsn::isPopBindings(int &n, InsnPtr &next) const
{
  n = n_;
  next = next_;
  return true;
}

MakeBoxesInsn::MakeBoxesInsn(int n, InsnPtr next)
: n_(n), next_(next)
{
}

const Insn *MakeBoxesInsn::execute(VM &vm) const
{
  vm.needStack(n_);
  for (int i = 0; i < n_; i++)
    *vm.sp++ = new (*vm.interp) BoxObj;
  return next_.pointer();
}

SetBoxesInsn::SetBoxesInsn(int n, InsnPtr next)
: n_(n), next_(next)
{
}

// On top of the stack there are n values, next n boxes.
// Put the values in the boxes, and replace the boxes by the values.
// Then pop the values.

const Insn *SetBoxesInsn::execute(VM &vm) const
{
  ELObj **boxes = vm.sp - n_ - n_;
  ELObj **values = vm.sp - n_;
  for (int i = 0; i < n_; i++) {
    BoxObj *box = boxes[i]->asBox();
    ASSERT(box != 0);
    boxes[i] = box->value = values[i];
  }
  vm.sp -= n_;
  return next_.pointer();
}

UnboxInsn::UnboxInsn(const Identifier *ident, const Location &loc, InsnPtr next)
: ident_(ident), loc_(loc), next_(next)
{
}

const Insn *UnboxInsn::execute(VM &vm) const
{
  BoxObj *box = vm.sp[-1]->asBox();
  ASSERT(box != 0);
  if (!box->value) {
    vm.interp->setNextLocation(loc_);
    vm.interp->message(InterpreterMessages::undefinedVariableReference,
                       StringMessageArg(ident_->name()));
    vm.sp = 0;
    return 0;
  }
  vm.sp[-1] = box->value;
  return next_.pointer();
}

const Insn *CheckSosofoInsn::execute(VM &vm) const
{
  if (!vm.sp[-1]->asSosofo()) {
    vm.sp = 0;
    vm.interp->setNextLocation(loc_);
    vm.interp->message(InterpreterMessages::sosofoContext);
    return 0;
  }
  return next_.pointer();
}

const Insn *PushModeInsn::execute(VM &vm) const
{
  vm.modeStack.push_back(vm.processingMode);
  vm.processingMode = mode_;
  return next_.pointer();
}

const Insn *PopModeInsn::execute(VM &vm) const
{
  vm.processingMode = vm.modeStack.back();
  vm.modeStack.resize(vm.modeStack.size() - 1);
  return next_.pointer();
}

MaybeOverrideStyleInsn::MaybeOverrideStyleInsn(InsnPtr next)
: next_(next)
{
}

const Insn *MaybeOverrideStyleInsn::execute(VM &vm) const
{
  if (vm.overridingStyle)
    vm.sp[-1] = new (*vm.interp) OverriddenStyleObj((BasicStyleObj *)vm.sp[-1],
						    vm.overridingStyle);
  return next_.pointer();
}

VarStyleInsn::VarStyleInsn(const ConstPtr<StyleSpec> &styleSpec, unsigned displayLength,
			   bool hasUse, InsnPtr next)
: styleSpec_(styleSpec), displayLength_(displayLength), hasUse_(hasUse), next_(next)
{
}

const Insn *VarStyleInsn::execute(VM &vm) const
{
  ELObj **display
    = displayLength_ ? new ELObj *[displayLength_ + 1] : 0;
  ELObj **tem = vm.sp - displayLength_;
  for (int i = 0; i < displayLength_; i++)
    display[i] = tem[i];
  if (displayLength_ == 0) {
    vm.needStack(1);
    tem = vm.sp;
  }
  else
    display[displayLength_] = 0;
  // Make sure objects in display are still visible on the stack
  // to the garbage collector.
  StyleObj *use;
  if (hasUse_)
    use = (StyleObj *)*--tem;
  else
    use = 0;
  *tem++ = new (*vm.interp) VarStyleObj(styleSpec_, use, display, vm.currentNode);
  vm.sp = tem;
  return next_.pointer();
}

SetStyleInsn::SetStyleInsn(InsnPtr next)
: next_(next)
{
}

const Insn *SetStyleInsn::execute(VM &vm) const
{
  ((FlowObj *)vm.sp[-2])->setStyle((StyleObj *)vm.sp[-1]);
  vm.sp--;
  return next_.pointer();
}

SosofoAppendInsn::SosofoAppendInsn(size_t n, InsnPtr next)
: n_(n), next_(next)
{
}

const Insn *SosofoAppendInsn::execute(VM &vm) const
{
  AppendSosofoObj *obj = new (*vm.interp) AppendSosofoObj;
  ELObj **tem = vm.sp - n_;
  for (size_t i = 0; i < n_; i++) {
    ASSERT(tem[i]->asSosofo() != 0);
    obj->append((SosofoObj *)tem[i]);
  }
  vm.sp -= n_ - 1;
  vm.sp[-1] = obj;
  return next_.pointer();
}

CopyFlowObjInsn::CopyFlowObjInsn(FlowObj *flowObj, InsnPtr next)
: flowObj_(flowObj), next_(next)
{
}

const Insn *CopyFlowObjInsn::execute(VM &vm) const
{
  vm.needStack(1);
  *vm.sp++ = flowObj_->copy(*vm.interp);
  return next_.pointer();
}

SetNonInheritedCsSosofoInsn
::SetNonInheritedCsSosofoInsn(InsnPtr code, int displayLength, InsnPtr next)
: code_(code), displayLength_(displayLength), next_(next)
{
}

const Insn *SetNonInheritedCsSosofoInsn::execute(VM &vm) const
{
  ELObj **display
    = displayLength_ ? new ELObj *[displayLength_ + 1] : 0;
  ELObj **tem = vm.sp - displayLength_;
  for (int i = 0; i < displayLength_; i++) {
    display[i] = tem[i];
    ASSERT(display[i] != 0);
  }
  if (displayLength_)
    display[displayLength_] = 0;
  // Make sure objects in display are still visible on the stack
  // to the garbage collector.
  FlowObj *flowObj = (FlowObj *)*--tem;
  ASSERT((*tem)->asSosofo() != 0);
  *tem++ = new (*vm.interp) SetNonInheritedCsSosofoObj(flowObj, code_, display);
  vm.sp = tem;
  return next_.pointer();
}

SetNonInheritedCInsn::SetNonInheritedCInsn(const Identifier *nic, const Location &loc,
					   InsnPtr next)
: nic_(nic), loc_(loc), next_(next)
{
}

const Insn *SetNonInheritedCInsn::execute(VM &vm) const
{
  ASSERT(vm.sp[-2]->asSosofo() != 0);
  ((FlowObj *)vm.sp[-2])->setNonInheritedC(nic_, vm.sp[-1], loc_, *vm.interp);
  vm.sp--;
  return next_.pointer();
}

SetContentInsn::SetContentInsn(const CompoundFlowObj *flowObj, InsnPtr next)
: flowObj_(flowObj), next_(next)
{
}

const Insn *SetContentInsn::execute(VM &vm) const
{
  CompoundFlowObj *copy = (CompoundFlowObj *)flowObj_->copy(*vm.interp);
  copy->setContent((SosofoObj *)vm.sp[-1]);
  vm.sp[-1] = copy;
  return next_.pointer();
}

SetDefaultContentInsn::SetDefaultContentInsn(const CompoundFlowObj *flowObj, InsnPtr next)
: flowObj_(flowObj), next_(next)
{
}

const Insn *SetDefaultContentInsn::execute(VM &vm) const
{
  vm.needStack(1);
  *vm.sp++ = flowObj_->copy(*vm.interp);
  ((CompoundFlowObj *)vm.sp[-1])
    ->setContent(new (*vm.interp) ProcessChildrenSosofoObj(vm.processingMode));
  return next_.pointer();
}

MakeDefaultContentInsn::MakeDefaultContentInsn(InsnPtr next)
: next_(next)
{
}

const Insn *MakeDefaultContentInsn::execute(VM &vm) const
{
  vm.needStack(1);
  *vm.sp++ = new (*vm.interp) ProcessChildrenSosofoObj(vm.processingMode);
  return next_.pointer();
}

LabelSosofoInsn::LabelSosofoInsn(const Location &loc, InsnPtr next)
: loc_(loc), next_(next)
{
}

const Insn *LabelSosofoInsn::execute(VM &vm) const
{
  SymbolObj *sym = vm.sp[-1]->asSymbol();
  if (!sym) {
    vm.interp->setNextLocation(loc_);
    vm.interp->message(InterpreterMessages::labelNotASymbol);
    vm.sp = 0;
    return 0;
  }
  ASSERT(vm.sp[-2]->asSosofo() != 0);
  vm.sp[-2] = new (*vm.interp) LabelSosofoObj(sym, loc_, (SosofoObj *)vm.sp[-2]);
  vm.sp--;
  return next_.pointer();
}

ContentMapSosofoInsn::ContentMapSosofoInsn(const Location &loc, InsnPtr next)
: loc_(loc), next_(next)
{
}

const Insn *ContentMapSosofoInsn::execute(VM &vm) const
{
  ASSERT(vm.sp[-2]->asSosofo() != 0);
  vm.sp[-2] = new (*vm.interp) ContentMapSosofoObj(vm.sp[-1], &loc_, (SosofoObj *)vm.sp[-2]);
  vm.sp--;
  return next_.pointer();
}

BoxObj::BoxObj()
: value(0)
{
  hasSubObjects_ = 1;
}

BoxObj *BoxObj::asBox()
{
  return this;
}

void BoxObj::traceSubObjects(Collector &c) const
{
  c.trace(value);
}

#ifdef DSSSL_NAMESPACE
}
#endif

