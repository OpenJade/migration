// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#include "stylelib.h"
#include "Insn.h"
#include "Insn2.h"
#include "Interpreter.h"
#include "InterpreterMessages.h"
#include "ELObjMessageArg.h"
#include "Expression.h"
#include "Style.h"
#include "macros.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

Expression::Expression(const Location &loc)
: loc_(loc)
{
}

void Expression::optimize(Interpreter &, const Environment &, Owner<Expression> &)
{
}

ELObj *Expression::constantValue() const
{
  return 0;
}

const Identifier *Expression::keyword() const
{
  return 0;
}

void Expression::markBoundVars(BoundVarList &)
{
}

ConstantExpression::ConstantExpression(ELObj *obj, const Location &loc)
: Expression(loc), obj_(obj)
{
}

InsnPtr
ConstantExpression::compile(Interpreter &interp, const Environment &env,
			    int stackPos, const InsnPtr &next)
{
  return new ConstantInsn(obj_, new ResolveQuantitiesInsn(location(), next));
}

void ConstantExpression::optimize(Interpreter &interp, const Environment &,
				  Owner<Expression> &expr)
{
  ELObj *tem = obj_->resolveQuantities(0, interp, location());
  if (tem) {
    interp.makePermanent(tem);
    expr = new ResolvedConstantExpression(tem, location());
  }
}

bool ConstantExpression::canEval(bool) const
{
  return 0;
}

const Identifier *ConstantExpression::keyword() const
{
  const KeywordObj *k = obj_->asKeyword();
  if (k)
    return k->identifier();
  else
    return 0;
}

ResolvedConstantExpression::ResolvedConstantExpression(ELObj *obj, const Location &loc)
: Expression(loc), obj_(obj)
{
}

InsnPtr
ResolvedConstantExpression::compile(Interpreter &interp, const Environment &env,
				    int stackPos, const InsnPtr &next)
{
  return new ConstantInsn(obj_, next);
}

ELObj *ResolvedConstantExpression::constantValue() const
{
  return obj_;
}

bool ResolvedConstantExpression::canEval(bool) const
{
  return 1;
}

CallExpression::CallExpression(Owner<Expression> &op,
			       NCVector<Owner<Expression> > &args,
			       const Location &loc)
: Expression(loc)
{
  op.swap(op_);
  args.swap(args_);
}

bool CallExpression::canEval(bool) const
{
  if (!op_->canEval(1))
    return 0;
  for (size_t i = 0; i < args_.size(); i++)
    if (!args_[i]->canEval(1))
      return 0;
  return 1;
}

int CallExpression::nArgs()
{
  return args_.size();
}

InsnPtr CallExpression::compile(Interpreter &interp, const Environment &env,
				int stackPos, const InsnPtr &next)
{
  op_->optimize(interp, env, op_);
  ELObj *value = op_->constantValue();
  InsnPtr result;
  if (value) {
    FunctionObj *func = value->asFunction();
    if (!func) {
      interp.setNextLocation(location());
      interp.message(InterpreterMessages::callNonFunction,
		     ELObjMessageArg(value, interp));
      return new ErrorInsn;
    }
    if (nArgs() < func->nRequiredArgs()) {
      interp.setNextLocation(location());
      interp.message(InterpreterMessages::missingArg);
      return new ErrorInsn;
    }
    if (nArgs() - func->nRequiredArgs() > func->nOptionalArgs()) {
      if (func->nKeyArgs()) {
	if ((nArgs() - func->nRequiredArgs() - func->nOptionalArgs()) & 1) {
	  interp.setNextLocation(location());
	  interp.message(InterpreterMessages::oddKeyArgs);
	  args_.resize(func->nRequiredArgs() + func->nOptionalArgs());
	}
      }
      else if (!func->restArg()) {
	interp.setNextLocation(location());
	interp.message(InterpreterMessages::tooManyArgs);
	args_.resize(func->nRequiredArgs() + func->nOptionalArgs());
      }
    }

    int callerArgs;
    if (!next.isNull() && next->isReturn(callerArgs))
      result = func->makeTailCallInsn(nArgs(), interp, location(), callerArgs);
    else
      result = func->makeCallInsn(nArgs(), interp, location(), next);
  }
  else {
    int n = nArgs();
    int callerArgs;
    if (!next.isNull() && next->isReturn(callerArgs))
      result = new TailApplyInsn(callerArgs, n, location());
    else
      result = new ApplyInsn(n, location(), next);
    result = op_->compile(interp, env, stackPos + n, result);
  }
  for (size_t i = args_.size(); i > 0; i--)
    result = optimizeCompile(args_[i - 1], interp, env,
                             stackPos + i - 1, result);
  return result;
}

void CallExpression::markBoundVars(BoundVarList &vars)
{
  op_->markBoundVars(vars);
  for (size_t i = 0; i < args_.size(); i++)
    args_[i]->markBoundVars(vars);
}

VariableExpression::VariableExpression(const Identifier *ident,
				       const Location &loc)
: Expression(loc), ident_(ident), isTop_(0)
{
}

InsnPtr VariableExpression::compile(Interpreter &interp,
				    const Environment &env,
				    int stackPos,
				    const InsnPtr &next)
{
  bool isFrame;
  int index;
  bool boxed;
  if (env.lookup(ident_, isFrame, index, boxed)) {
    InsnPtr tem;
    int n;
    if (isFrame
        && !next.isNull()
        && next->isPopBindings(n, tem)
	&& n == 1
	&& index - stackPos == -1) {
      // This happens with named let.
      if (boxed)
      	return new UnboxInsn(ident_, location(), tem);
      else
      	return tem;
    }
    if (boxed)
      tem = new UnboxInsn(ident_, location(), next);
    else
      tem = next;
    if (isFrame)
      return new StackRefInsn(index - stackPos, index, tem);
    else
      return new ClosureRefInsn(index, tem);
  }
  isTop_ = 1;
  Location loc;
  unsigned part;
  if (!ident_->defined(part, loc)) {
    interp.setNextLocation(location());
    interp.message(InterpreterMessages::undefinedVariableReference,
		   StringMessageArg(ident_->name()));
    return new ErrorInsn;
  }
  ELObj *val = ident_->computeValue(0, interp);
  if (!val)
    return new TopRefInsn(ident_, next);
  if (interp.isError(val))
    return new ErrorInsn;
  return new ConstantInsn(val, next);
}

void VariableExpression::optimize(Interpreter &interp, const Environment &env,
				  Owner<Expression> &expr)
{
  bool isFrame;
  int index;
  bool boxed;
  if (env.lookup(ident_, isFrame, index, boxed))
    return;
  isTop_ = 1;
  Location loc;
  unsigned part;
  if (ident_->defined(part, loc)) {
    ELObj *obj = ident_->computeValue(0, interp);
    if (obj && !interp.isError(obj)) {
      interp.makePermanent(obj);
      expr = new ConstantExpression(obj, location());
      expr->optimize(interp, env, expr);
    }
  }
}

bool VariableExpression::canEval(bool) const
{
  return !isTop_ || ident_->evaluated();
}

void VariableExpression::markBoundVars(BoundVarList &vars)
{
  vars.mark(ident_);
}

IfExpression::IfExpression(Owner<Expression> &test,
			   Owner<Expression> &consequent,
			   Owner<Expression> &alternate,
			   const Location &loc)
: Expression(loc)
{
  test.swap(test_);
  consequent.swap(consequent_);
  alternate.swap(alternate_);
}

bool IfExpression::canEval(bool maybeCall) const
{
  return (test_->canEval(maybeCall)
          && consequent_->canEval(maybeCall)
	  && alternate_->canEval(maybeCall));
}

void IfExpression::optimize(Interpreter &interp, const Environment &env,
			    Owner<Expression> &expr)
{
  test_->optimize(interp, env, test_);
  ELObj *obj = test_->constantValue();
  if (obj) {
    if (obj->isTrue()) {
      expr = consequent_.extract();
      expr->optimize(interp, env, expr);
    }
    else {
      expr = alternate_.extract();
      expr->optimize(interp, env, expr);
    }
  }
}

InsnPtr IfExpression::compile(Interpreter &interp, const Environment &env,
			      int stackPos, const InsnPtr &next)
{
  alternate_->optimize(interp, env, alternate_);
  if (alternate_->constantValue() == interp.makeFalse())
    return test_->compile(interp, env, stackPos,
			  new AndInsn(optimizeCompile(consequent_,
						      interp, env,
						      stackPos, next),
				      next));
  else
    return test_->compile(interp, env, stackPos,
			  new TestInsn(optimizeCompile(consequent_,
						       interp, env,
						       stackPos, next),
				       alternate_->compile(interp, env,
							   stackPos, next)));
}

void IfExpression::markBoundVars(BoundVarList &vars)
{
  test_->markBoundVars(vars);
  consequent_->markBoundVars(vars);
  alternate_->markBoundVars(vars);
}

OrExpression::OrExpression(Owner<Expression> &test1,
			   Owner<Expression> &test2,
			   const Location &loc)
: Expression(loc)
{
  test1.swap(test1_);
  test2.swap(test2_);
}

bool OrExpression::canEval(bool maybeCall) const
{
  return (test1_->canEval(maybeCall)
          && test2_->canEval(maybeCall));
}

void OrExpression::optimize(Interpreter &interp, const Environment &env,
			    Owner<Expression> &expr)
{
  test1_->optimize(interp, env, test1_);
  ELObj *obj = test1_->constantValue();
  if (obj) {
    if (obj->isTrue())
      expr = test1_.extract();
    else {
      expr = test2_.extract();
      expr->optimize(interp, env, expr);
    }
  }
}

InsnPtr OrExpression::compile(Interpreter &interp, const Environment &env,
			      int stackPos, const InsnPtr &next)
{
  return test1_->compile(interp, env, stackPos,
			 new OrInsn(optimizeCompile(test2_, interp, env,
						    stackPos, next),
				    next));
}

void OrExpression::markBoundVars(BoundVarList &vars)
{
  test1_->markBoundVars(vars);
  test2_->markBoundVars(vars);
}

CondFailExpression::CondFailExpression(const Location &loc)
: Expression(loc)
{
}

InsnPtr CondFailExpression::compile(Interpreter &, const Environment &,
				    int, const InsnPtr &)
{
  return new CondFailInsn(location());
}

bool CondFailExpression::canEval(bool maybeCall) const
{
  return 1;
}

CaseExpression::CaseExpression(Owner<Expression> &key,
			       NCVector<Case> &cases,
			       Owner<Expression> &elseClause,
			       const Location &loc)
: Expression(loc)
{
  key.swap(key_);
  cases.swap(cases_);
  elseClause.swap(else_);
}

InsnPtr CaseExpression::compile(Interpreter &interp,
				const Environment &env, int stackPos,
				const InsnPtr &next)
{
  InsnPtr finish;
  if (else_)
    finish = new PopInsn(else_->compile(interp, env, stackPos, next));
  else
    finish = new CaseFailInsn(location());
  for (size_t i = 0; i < cases_.size(); i++) {
    InsnPtr match = cases_[i].expr->compile(interp, env, stackPos, next);
    for (size_t j = 0; j < nResolved_[i]; j++)
      finish = new CaseInsn(cases_[i].datums[j], match, finish);
  }
  // FIXME handle unresolved quantities
  return key_->compile(interp, env, stackPos, finish);
}


void CaseExpression::markBoundVars(BoundVarList &vars)
{
  key_->markBoundVars(vars);
  for (size_t i = 0; i < cases_.size(); i++)
    cases_[i].expr->markBoundVars(vars);
  if (else_)
    else_->markBoundVars(vars);
}

bool CaseExpression::canEval(bool maybeCall) const
{
  if (!key_->canEval(maybeCall))
    return 0;
  if (else_ && !else_->canEval(maybeCall))
    return 0;
  for (size_t i = 0; i < cases_.size(); i++) {
    if (!cases_[i].expr->canEval(maybeCall))
      return 0;
    if (nResolved_[i] == cases_[i].datums.size())
      return 0;
  }
  return 1;
}

void CaseExpression::optimize(Interpreter &interp, const Environment &env,
			      Owner<Expression> &expr)
{
  key_->optimize(interp, env, key_);
  ELObj *k = key_->constantValue();
  nResolved_.assign(cases_.size(), 0);
  bool unresolved = 0;
  for (size_t i = 0; i < cases_.size(); i++) {
    cases_[i].expr->optimize(interp, env, cases_[i].expr);
    int nResolved = 0;
    for (size_t j = 0; j < cases_[i].datums.size(); j++) {
      ELObj *tem = cases_[i].datums[j]->resolveQuantities(0, interp,
							  location());
      if (tem) {
	if (k && *k == *tem) {
	  expr = cases_[i].expr.extract();
	  return;
	}
	if (j != nResolved)
	  cases_[i].datums[j] = cases_[i].datums[nResolved];
	cases_[i].datums[nResolved++] = tem;
      }
      else
	unresolved = 1;
    }
    nResolved_[i] = nResolved;
  }
  if (else_) {
    else_->optimize(interp, env, else_);
    if (k && !unresolved)
      expr = else_.extract();
  }
  else if (k && !unresolved) {
    interp.setNextLocation(location());
    interp.message(InterpreterMessages::caseFail, ELObjMessageArg(k, interp));
  }
  if (unresolved) {
    interp.setNextLocation(location());
    interp.message(InterpreterMessages::caseUnresolvedQuantities);
  }
}


LambdaExpression::LambdaExpression(Vector<const Identifier *> &formals,
				   NCVector<Owner<Expression> > &inits,
				   int nOptional,
				   bool hasRest,
				   int nKey,
				   Owner<Expression> &body,
				   const Location &loc)
: Expression(loc)
{
  formals.swap(formals_);
  inits.swap(inits_);
  body.swap(body_);
  sig_.nRequiredArgs = formals_.size() - nOptional - nKey - hasRest;
  sig_.nOptionalArgs = nOptional;
  sig_.restArg = hasRest;
  sig_.nKeyArgs = nKey;
  sig_.keys = formals_.begin() + formals_.size() - nKey;
}

bool LambdaExpression::canEval(bool maybeCall) const
{
  if (!maybeCall)
    return 1;
  if (!body_->canEval(1))
    return 0;
  for (size_t i = 0; i < inits_.size(); i++)
    if (inits_[i] && !inits_[i]->canEval(1))
      return 0;
  return 1;
}

InsnPtr LambdaExpression::compile(Interpreter &interp, const Environment &env,
				  int stackPos, const InsnPtr &next)
{
  BoundVarList boundVars;
  env.boundVars(boundVars);
  markBoundVars(boundVars);
  boundVars.removeUnused();
  InsnPtr code
    = optimizeCompile(body_, interp,
		      Environment(formals_, boundVars), formals_.size(),
		      new ReturnInsn(formals_.size()));
  if (sig_.nOptionalArgs || sig_.restArg || sig_.nKeyArgs) {
    Vector<InsnPtr> entryPoints(sig_.nOptionalArgs
				+ (sig_.restArg || sig_.nKeyArgs)
				+ 1);
    // First entry point is for only required args supplied
    // Next entry is for one optional arg supplied, no rest arg
    // Next entry is for two optional args, no rest arg
    // ..
    // Next entry is for all optional args, no other args
    // Last entry is for all optional args supplied, and other args
    //  (for this entry point all args are pushed; unspecified
    //   keyword args are bound to unspecified values).
    entryPoints.back() = code;
    if (sig_.nKeyArgs) {
      // For each keyword argument test whether it was
      // unspecified, and if so initialize it.
      for (int i = sig_.nOptionalArgs + sig_.nKeyArgs - 1;
	   i >= sig_.nOptionalArgs;
	   i--) {
	int offset = i - (sig_.nOptionalArgs + sig_.nKeyArgs);
	InsnPtr &next = entryPoints.back();
	InsnPtr set(new SetKeyArgInsn(offset, next));
	if (inits_[i]) {
	  Vector<const Identifier *> f(formals_);
	  f.resize(sig_.nRequiredArgs + i + sig_.restArg);
	  set = optimizeCompile(inits_[i], interp,
				Environment(f, boundVars),
				formals_.size(),
				set);
	}
	else
	  set = new ConstantInsn(interp.makeFalse(), set);
	next = new TestUnspecifiedInsn(offset, set, next);
      }
    }
    if (sig_.restArg || sig_.nKeyArgs) {
      for (int i = sig_.nOptionalArgs + sig_.nKeyArgs - 1;
	   i >= sig_.nOptionalArgs;
	   i--) {
	if (inits_[i]) {
	  Vector<const Identifier *> f(formals_);
	  f.resize(sig_.nRequiredArgs + i + sig_.restArg);
	  code = optimizeCompile(inits_[i], interp,
				 Environment(f, boundVars),
				 f.size(),
				 code);
	}
	else
	  code = new ConstantInsn(interp.makeFalse(), code);
      }
      if (sig_.restArg)
	code = new ConstantInsn(interp.makeNil(), code);
      entryPoints[sig_.nOptionalArgs] = code;
    }
    for (int i = sig_.nOptionalArgs - 1; i >= 0; i--) {
      if (inits_[i]) {
	Vector<const Identifier *> f(formals_);
	f.resize(sig_.nRequiredArgs + i);
	entryPoints[i]
	  = optimizeCompile(inits_[i], interp,
			    Environment(f, boundVars),
			    f.size(),
			    entryPoints[i + 1]);
      }
      else
	entryPoints[i] = new ConstantInsn(interp.makeFalse(),
					  entryPoints[i + 1]);
    }
    code = new VarargsInsn(sig_, entryPoints, location());
  }
  return compilePushVars(interp, env, stackPos, boundVars, 0,
			 new ClosureInsn(&sig_, code,
					 boundVars.size(), next));
}

InsnPtr Expression::compilePushVars(Interpreter &interp,
				    const Environment &env,
	    		            int stackPos,
			            const BoundVarList &vars,
				    size_t varIndex,
			            const InsnPtr &next)
{
  if (varIndex >= vars.size())
    return next;
  bool isFrame;
  int index;
  bool boxed;
  if (!env.lookup(vars[varIndex].ident, isFrame, index, boxed))
    CANNOT_HAPPEN();
  if (isFrame)
    return new FrameRefInsn(index,
			    compilePushVars(interp, env, stackPos + 1, vars,
					    varIndex + 1, next));
  else
    return new ClosureRefInsn(index,
			      compilePushVars(interp, env, stackPos + 1, vars,
					      varIndex + 1, next));
}

void LambdaExpression::markBoundVars(BoundVarList &vars)
{
  for (int i = 0; i < sig_.nOptionalArgs + sig_.nKeyArgs; i++)
    if (inits_[i]) {
      Vector<const Identifier *> f(formals_);
      f.resize(sig_.nRequiredArgs + i
	       + (sig_.restArg && i >= sig_.nOptionalArgs));
      vars.rebind(f);
      inits_[i]->markBoundVars(vars);
      vars.unbind(f);
    }
  vars.rebind(formals_);
  body_->markBoundVars(vars);
  vars.unbind(formals_);
}

LetExpression::LetExpression(Vector<const Identifier *> &vars,
			     NCVector<Owner<Expression> > &inits,
			     Owner<Expression> &body,
			     const Location &loc)
: Expression(loc)
{
  vars.swap(vars_);
  inits.swap(inits_);
  body.swap(body_);
}

bool LetExpression::canEval(bool maybeCall) const
{
  if (!body_->canEval(maybeCall))
    return 0;
  for (size_t i = 0; i < inits_.size(); i++)
    if (!inits_[i]->canEval(1))
      return 0;
  return 1;
}

InsnPtr LetExpression::compile(Interpreter &interp, const Environment &env,
			       int stackPos, const InsnPtr &next)
{
  int nVars = vars_.size();
  Environment bodyEnv(env);
  bodyEnv.augmentFrame(vars_, stackPos, false);
  return compileInits(interp, env, 0, stackPos,
		      optimizeCompile(body_, interp, bodyEnv,
				      stackPos + nVars,
				      PopBindingsInsn::make(nVars, next)));
}

InsnPtr LetExpression::compileInits(Interpreter &interp,
				    const Environment &env,
				    size_t initIndex,
				    int stackPos,
				    const InsnPtr &next)
{
  if (initIndex >= inits_.size())
    return next;
  return optimizeCompile(inits_[initIndex], interp, env, stackPos,
				   compileInits(interp, env, initIndex + 1,
						stackPos + 1, next));
}

void LetExpression::markBoundVars(BoundVarList &vars)
{
  for (size_t i = 0; i < inits_.size(); i++)
    inits_[i]->markBoundVars(vars);
  vars.rebind(vars_);
  body_->markBoundVars(vars);
  vars.unbind(vars_);
}

LetStarExpression::LetStarExpression(Vector<const Identifier *> &vars,
				     NCVector<Owner<Expression> > &inits,
				     Owner<Expression> &body,
				     const Location &loc)
: LetExpression(vars, inits, body, loc)
{
}

InsnPtr LetStarExpression::compile(Interpreter &interp,
				   const Environment &env,
				   int stackPos,
				   const InsnPtr &next)
{
  int nVars = vars_.size();
  Environment bodyEnv(env);
  bodyEnv.augmentFrame(vars_, stackPos, false);
  return compileInits(interp, env, 0, stackPos,
		      optimizeCompile(body_, interp, bodyEnv,
				      stackPos + vars_.size(),
				      PopBindingsInsn::make(nVars, next)));
}

InsnPtr LetStarExpression::compileInits(Interpreter &interp,
					const Environment &env,
					size_t initIndex,
					int stackPos,
					const InsnPtr &next)
{
  if (initIndex >= inits_.size())
    return next;
  Environment nextEnv(env);
  Vector<const Identifier *> v(1);
  v[0] = vars_[initIndex];
  nextEnv.augmentFrame(v, stackPos, false);
  return optimizeCompile(inits_[initIndex], interp, env, stackPos,
			 compileInits(interp, nextEnv, initIndex + 1,
				      stackPos + 1, next));
}

LetrecExpression::LetrecExpression(Vector<const Identifier *> &vars,
				   NCVector<Owner<Expression> > &inits,
				   Owner<Expression> &body,
				   const Location &loc)
: Expression(loc)
{
  vars.swap(vars_);
  inits.swap(inits_);
  body.swap(body_);
}

bool LetrecExpression::canEval(bool maybeCall) const
{
  if (!body_->canEval(maybeCall))
    return 0;
  for (size_t i = 0; i < inits_.size(); i++)
    if (!inits_[i]->canEval(1))
      return 0;
  return 1;
}


InsnPtr LetrecExpression::compile(Interpreter &interp, const Environment &env,
				  int stackPos, const InsnPtr &next)
{
  int nVars = vars_.size();
  Environment initEnv(env);
  initEnv.augmentFrame(vars_, stackPos, true);
  Environment bodyEnv(env);
  bodyEnv.augmentFrame(vars_, stackPos, false);
  return new MakeBoxesInsn(nVars,
			   compileInits(interp, initEnv, 0,
					stackPos + nVars,
					new SetBoxesInsn(nVars,
							 optimizeCompile(body_, interp, bodyEnv,
									stackPos + nVars,
									PopBindingsInsn::make(nVars, next)))));
}

InsnPtr LetrecExpression::compileInits(Interpreter &interp,
				       const Environment &env,
				       size_t initIndex,
				       int stackPos,
				       const InsnPtr &next)
{
  if (initIndex >= inits_.size())
    return next;
  return optimizeCompile(inits_[initIndex], interp, env, stackPos,
				    compileInits(interp, env, initIndex + 1,
						 stackPos + 1, next));
}

void LetrecExpression::markBoundVars(BoundVarList &vars)
{
  vars.rebind(vars_);
  for (size_t i = 0; i < inits_.size(); i++)
    inits_[i]->markBoundVars(vars);
  body_->markBoundVars(vars);
  vars.unbind(vars_);
}

WithModeExpression::WithModeExpression(const ProcessingMode *mode, Owner<Expression> &expr,
				       const Location &loc)
: Expression(loc), mode_(mode)
{
  expr.swap(expr_);
}

InsnPtr WithModeExpression::compile(Interpreter &interp, const Environment &env, int stackPos,
				    const InsnPtr &next)
{
  return new PushModeInsn(mode_,
			  optimizeCompile(expr_, interp, env, stackPos,
					  new PopModeInsn(next)));
}

void WithModeExpression::markBoundVars(BoundVarList &vars)
{
  expr_->markBoundVars(vars);
}

bool WithModeExpression::canEval(bool maybeCall) const
{
  return expr_->canEval(maybeCall);
}

StyleExpression::StyleExpression(Vector<const Identifier *> &keys,
				 NCVector<Owner<Expression> > &exprs,
				 const Location &loc)
: Expression(loc)
{
  keys.swap(keys_);
  exprs.swap(exprs_);
}

InsnPtr StyleExpression::compile(Interpreter &interp, const Environment &env,
				 int stackPos, const InsnPtr &next)
{
  // FIXME handle force!
  Vector<ConstPtr<InheritedC> > ics;
  Vector<ConstPtr<InheritedC> > forceIcs;
  bool hasUse = 0;
  size_t useIndex;
  BoundVarList boundVars;
  env.boundVars(boundVars);
  for (size_t i = 0; i < keys_.size(); i++) {
    if (!keys_[i]->inheritedC().isNull()) {
      ics.resize(ics.size() + 1);
      exprs_[i]->markBoundVars(boundVars);
    }
  }
  // FIXME optimize case where ics.size() == 0
  boundVars.removeUnused();
  Vector<const Identifier *> noVars;
  Environment newEnv(noVars, boundVars);
  size_t j = 0;
  for (size_t i = 0; i < keys_.size(); i++) {
    Identifier::SyntacticKey sk;
    if (!keys_[i]->inheritedC().isNull()) {
      exprs_[i]->optimize(interp, newEnv, exprs_[i]);
      ELObj *val = exprs_[i]->constantValue();
      if (val) {
	interp.makePermanent(val);
	ics[j] = keys_[i]->inheritedC()->make(val, exprs_[i]->location(), interp);
	if (ics[j].isNull())
	  ics.resize(ics.size() - 1);
	else
	  j++;
      }
      else {
	ics[j++] = new VarInheritedC(keys_[i]->inheritedC(),
				     exprs_[i]->compile(interp, newEnv, 0, InsnPtr()),
				     exprs_[i]->location());
      }
    }
    else if (keys_[i]->syntacticKey(sk) && sk == Identifier::keyUse && !hasUse) {
      hasUse = 1;
      useIndex = i;
    }
    else
      unknownStyleKeyword(keys_[i], interp, location());
  }
  InsnPtr result
    = compilePushVars(interp, env, stackPos + hasUse, boundVars, 0,
		      new VarStyleInsn(new StyleSpec(forceIcs, ics), 
				       boundVars.size(), hasUse,
				       new MaybeOverrideStyleInsn(next)));
  if (!hasUse)
    return result;
  else
    return optimizeCompile(exprs_[useIndex], interp, env, stackPos, result);
}

void StyleExpression::markBoundVars(BoundVarList &vars)
{
  for (size_t i = 0; i < exprs_.size(); i++)
    exprs_[i]->markBoundVars(vars);
}

bool StyleExpression::canEval(bool maybeCall) const
{
  for (size_t i = 0; i < exprs_.size(); i++)
    if (!exprs_[i]->canEval(maybeCall))
      return 0;
  return 1;
}

void StyleExpression::unknownStyleKeyword(const Identifier *ident, Interpreter &interp,
					  const Location &loc) const
{
  interp.setNextLocation(loc);
  StringC tem(ident->name());
  tem += Char(':');
  interp.message(InterpreterMessages::invalidStyleKeyword,
  		 StringMessageArg(tem));
}

MakeExpression::MakeExpression(const Identifier *foc,
			       Vector<const Identifier *> &keys,
			       NCVector<Owner<Expression> > &exprs,
			       const Location &loc)
: foc_(foc), StyleExpression(keys, exprs, loc)
{
}

InsnPtr MakeExpression::compile(Interpreter &interp, const Environment &env, int stackPos,
				const InsnPtr &next)
{
  FlowObj *flowObj = foc_->flowObj();
  if (!flowObj)  {
    interp.setNextLocation(location());
    interp.message(InterpreterMessages::unknownFlowObjectClass,
		   StringMessageArg(foc_->name()));
    flowObj = new (interp) SequenceFlowObj;
    interp.makePermanent(flowObj);
  }
  Owner<Expression> *contentMapExpr = 0;
  InsnPtr rest(next);
  for (size_t i = 0; i < keys_.size(); i++) {
    Identifier::SyntacticKey syn;
    if (keys_[i]->syntacticKey(syn)) {
      if (syn == Identifier::keyLabel)
	rest = optimizeCompile(exprs_[i],
			       interp,
			       env,
			       stackPos + 1,
			       new LabelSosofoInsn(exprs_[i]->location(), rest));
      else if (syn == Identifier::keyContentMap)
	contentMapExpr = &exprs_[i];
    }
  }
  flowObj = applyConstNonInheritedCs(flowObj, interp, env);
  size_t nContent = exprs_.size() - keys_.size();
  CompoundFlowObj *cFlowObj = flowObj->asCompoundFlowObj();
  if (!cFlowObj && nContent > 0) {
    interp.setNextLocation(location());
    interp.message(InterpreterMessages::atomicContent,
		   StringMessageArg(foc_->name()));
    nContent = 0;
  }
  rest = compileNonInheritedCs(interp, env, stackPos + 1, rest);
  // FIXME optimize case where there are no non-inherited styles.
  rest = StyleExpression::compile(interp, env, stackPos + 1, new SetStyleInsn(rest));
  if (nContent == 0 && !contentMapExpr) {
    if (cFlowObj)
      return new SetDefaultContentInsn(cFlowObj, rest);
    else
      return new CopyFlowObjInsn(flowObj, rest);
  }
  rest = new SetContentInsn(cFlowObj, rest);
  if (contentMapExpr) {
    rest = optimizeCompile(*contentMapExpr, interp, env, stackPos + 1,
			   new ContentMapSosofoInsn((*contentMapExpr)->location(), rest));
    if (nContent == 0)
      return new MakeDefaultContentInsn(rest);
  }
  // FIXME get rid of CheckSosofoInsn if we can guarantee result is a SosofoObj.
  if (nContent == 1)
    return optimizeCompile(exprs_.back(), interp, env, stackPos,
			   new CheckSosofoInsn(exprs_.back()->location(), rest));
  rest = new SosofoAppendInsn(nContent, rest);
  for (size_t i = 1; i <= nContent; i++)
    rest = optimizeCompile(exprs_[exprs_.size() - i], interp, env, stackPos + nContent - i,
                           new CheckSosofoInsn(exprs_[exprs_.size() - i]->location(), rest));
  return rest;
}

FlowObj *MakeExpression::applyConstNonInheritedCs(FlowObj *flowObj, Interpreter &interp,
					          const Environment &env)
{
  FlowObj *result = flowObj;
  for (size_t i = 0; i < keys_.size(); i++)
    if (flowObj->hasNonInheritedC(keys_[i])) {
      exprs_[i]->optimize(interp, env, exprs_[i]);
      ELObj *val = exprs_[i]->constantValue();
      if (val) {
	if (result == flowObj) {
	  result = flowObj->copy(interp);
	  interp.makePermanent(result);
	}
	result->setNonInheritedC(keys_[i], val, exprs_[i]->location(), interp);
      }
    }
  return result;
}

// Have a copied flow object on the stack.
// Generate code to set its non-constant, non-inherited characteristics.

InsnPtr MakeExpression::compileNonInheritedCs(Interpreter &interp, const Environment &env,
					      int stackPos, const InsnPtr &next)
{
  FlowObj *flowObj = foc_->flowObj();
  if (!flowObj)
    return next;
  bool gotOne = 0;
  BoundVarList boundVars;
  env.boundVars(boundVars);
  for (size_t i = 0; i < keys_.size(); i++) {
    if (flowObj->hasNonInheritedC(keys_[i]) && !exprs_[i]->constantValue()) {
      exprs_[i]->markBoundVars(boundVars);
      gotOne = 1;
    }
  }
  if (!gotOne)
    return next;
  boundVars.removeUnused();
  Vector<const Identifier *> noVars;
  Environment newEnv(noVars, boundVars);
  InsnPtr code;
  for (size_t i = 0; i < keys_.size(); i++)
    if (flowObj->hasNonInheritedC(keys_[i]) && !exprs_[i]->constantValue())
     code = exprs_[i]->compile(interp, newEnv, 1,
			          new SetNonInheritedCInsn(keys_[i],
						           exprs_[i]->location(),
						           code));
  return compilePushVars(interp, env, stackPos, boundVars, 0,
			 new SetNonInheritedCsSosofoInsn(code, boundVars.size(), next));
}

void MakeExpression::unknownStyleKeyword(const Identifier *ident, Interpreter &interp,
					  const Location &loc) const
{
  FlowObj *flowObj = foc_->flowObj();
  if (!flowObj)
    return;
  Identifier::SyntacticKey key;
  if (ident->syntacticKey(key)) {
    switch(key) {
    case Identifier::keyLabel:
    case Identifier::keyContentMap:
      return;
    default:
      break;
    }
  }
  if (flowObj->hasNonInheritedC(ident))
    return;
  interp.setNextLocation(loc);
  StringC tem(ident->name());
  tem += Char(':');
  interp.message(InterpreterMessages::invalidMakeKeyword,
  		 StringMessageArg(tem), StringMessageArg(foc_->name()));
}

Environment::Environment()
: closureVars_(0)
{
}

Environment::Environment(const Vector<const Identifier *> &frameVars,
			 const BoundVarList &closureVars)
: closureVars_(&closureVars)
{
  FrameVarList *tem = new FrameVarList;
  frameVarList_ = tem;
  tem->vars = &frameVars;
  tem->stackPos = 0;
  tem->boxed = 0;
}

void Environment::boundVars(BoundVarList &result) const
{
  if (closureVars_) {
    for (size_t i = 0; i < closureVars_->size(); i++)
      result.append((*closureVars_)[i].ident,
  		    (*closureVars_)[i].boxed);
  }
  for (const FrameVarList *f = frameVarList_.pointer();
       f;
       f = f->next.pointer()) {
    for (size_t i = 0; i < f->vars->size(); i++)
      result.append((*f->vars)[i], f->boxed);
  }
}

bool Environment::lookup(const Identifier *ident,
			 bool &isFrame, int &index, bool &boxed)
     const
{
  for (const FrameVarList *p = frameVarList_.pointer();
       p;
       p = p->next.pointer()) {
    for (size_t i = 0; i < p->vars->size(); i++)
      if ((*p->vars)[i] == ident) {
	isFrame = true;
	index = i + p->stackPos;
	boxed = p->boxed;
	return true;
      }
  }
  if (closureVars_) {
    for (size_t i = 0; i < closureVars_->size(); i++)
      if ((*closureVars_)[i].ident == ident) {
	isFrame = false;
	index = i;
	boxed = (*closureVars_)[i].boxed;
	return true;
      }
  }
  return false;
}

void Environment::augmentFrame(const Vector<const Identifier *> &vars,
			       int stackPos, bool boxed)
{
  FrameVarList *tem = new FrameVarList;
  tem->stackPos = stackPos;
  tem->vars = &vars;
  tem->next = frameVarList_;
  tem->boxed = boxed;
  frameVarList_ = tem;
}

void BoundVarList::append(const Identifier *id, bool b)
{
  resize(size() + 1);
  BoundVar &tem = back();
  tem.ident = id;
  tem.boxed = b;
  tem.used = 0;
  tem.reboundCount = 0;
}

void BoundVarList::remove(const Vector<const Identifier *> &idents)
{
  size_t j = 0;
  for (size_t i = 0; i < size(); i++) {
    const Identifier *ident = (*this)[i].ident;
    for (size_t k = 0;; k++) {
      if (k >= idents.size()) {
	if (j != i)
	  (*this)[j] = (*this)[i];
	j++;
	break;
      }
      if (idents[k] == ident)
	break;			// skip it
    }
  }
  resize(j);
}

void BoundVarList::removeUnused()
{
  size_t j = 0;
  for (size_t i = 0; i < size(); i++) {
    if ((*this)[i].used) {
      if (j != i)
	(*this)[j] = (*this)[i];
      j++;
    }
  }
  resize(j);
}

void BoundVarList::mark(const Identifier *ident)
{
  BoundVar *bv = find(ident);
  if (bv && !bv->reboundCount)
    bv->used = 1;
}

void BoundVarList::rebind(const Vector<const Identifier *> &idents)
{
  for (size_t i = 0; i < idents.size(); i++) {
    BoundVar *bv = find(idents[i]);
    if (bv)
      bv->reboundCount += 1;
  }
}

void BoundVarList::unbind(const Vector<const Identifier *> &idents)
{
  for (size_t i = 0; i < idents.size(); i++) {
    BoundVar *bv = find(idents[i]);
    if (bv)
      bv->reboundCount -= 1;
  }
}

BoundVar *BoundVarList::find(const Identifier *ident)
{
  for (size_t i = 0; i < size(); i++)
    if ((*this)[i].ident == ident)
      return &(*this)[i];
  return 0;
}

#ifdef DSSSL_NAMESPACE
}
#endif
