//creado : Cristian Tornador
//22-10-2002 - Universidad Politecnica Catalunya (Barcelona)

#include "stylelib.h"
#include "ProcessContext.h"
#include "Interpreter.h"
#include "InterpreterMessages.h"
#include "SosofoObj.h"
#include "macros.h"

//anchor flow object
/**
 *@class AnchorFlowObj
 *@brief clase que crea el objeto de flujo Anchor
 */
class AnchorFlowObj : public CompoundFlowObj {
public:
  void *operator new(size_t, Collector &c) {
    return c.allocateObject(1);
  }
  AnchorFlowObj()
   : nic_(new FOTBuilder::AnchorNIC) { }
  AnchorFlowObj(const AnchorFlowObj &fo)
   : CompoundFlowObj(fo), nic_(new FOTBuilder::AnchorNIC(*fo.nic_)) { }
  void processInner(ProcessContext &);
  FlowObj *copy(Collector &) const;
  bool hasNonInheritedC(const Identifier *) const;
  void setNonInheritedC(const Identifier *, ELObj *,
                        const Location &, Interpreter &);
private:
  Owner<FOTBuilder::AnchorNIC> nic_;
};

