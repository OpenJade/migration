//creado : Cristian Tornador
//22-10-2002 - Universidad Politecnica Catalunya (Barcelona)

#include "stylelib.h"
#include "ProcessContext.h"
#include "Interpreter.h"
#include "InterpreterMessages.h"
#include "SosofoObj.h"
#include "macros.h"

//page-sequence
/**
 *@class PageSequenceFlowObj
 *@brief Clase que crea el objeto de flujo page-sequence
 */
class PageSequenceFlowObj : public CompoundFlowObj {
public:
  void *operator new(size_t, Collector &c) {
    return c.allocateObject(1);
  }
  PageSequenceFlowObj();
  PageSequenceFlowObj(const PageSequenceFlowObj &);
  void processInner(ProcessContext &);
  FlowObj *copy(Collector &) const;
  void traceSubObjects(Collector &) const;
};

