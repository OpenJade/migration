
//creado : Cristian Tornador
//22-10-2002 - Universidad Politecnica Catalunya (Barcelona)

#include "stylelib.h"
#include "ProcessContext.h"
#include "Interpreter.h"
#include "InterpreterMessages.h"
#include "SosofoObj.h"
#include "macros.h"

/**
 *@class SideBySideItemFlowObj
 *@brief clase que implementa el objeto de flujo side-by-side-item
 */
class SideBySideItemFlowObj : public CompoundFlowObj {
public:
  void *operator new(size_t, Collector &c) {
    return c.allocateObject(1);
  }
  void processInner(ProcessContext &);
  FlowObj *copy(Collector &) const;
};
