//creado : Cristian Tornador
//22-10-2002 - Universidad Politecnica Catalunya (Barcelona) 

#include "stylelib.h"
#include "ProcessContext.h"
#include "Interpreter.h"
#include "InterpreterMessages.h"
#include "SosofoObj.h"
#include "macros.h"

//emphasizing-mark
/**
 *@class EmphasizingMarkFlowObj
 *@brief clase que implementa el objeto de flujo emphasizing-mark
 */
class EmphasizingMarkFlowObj : public CompoundFlowObj {
public:
  void *operator new(size_t, Collector &c) {
    return c.allocateObject(1);
  }
  EmphasizingMarkFlowObj()
   : nic_(new FOTBuilder::EmphasizingMarkNIC), emphmark_(0) { }
  EmphasizingMarkFlowObj(const EmphasizingMarkFlowObj &fo)
   : CompoundFlowObj(fo), nic_(new FOTBuilder::EmphasizingMarkNIC(*fo.nic_)) {
 }
  void traceSubObjects(Collector &c) const; 
  void processInner(ProcessContext &);
  FlowObj *copy(Collector &) const;
  bool hasNonInheritedC(const Identifier *) const;
  void setNonInheritedC(const Identifier *, ELObj *,
                        const Location &, Interpreter &);
private:
  Owner<FOTBuilder::EmphasizingMarkNIC> nic_;
  SosofoObj* emphmark_;
};



