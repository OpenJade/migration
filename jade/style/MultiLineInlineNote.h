//creado : Cristian Tornador
//22-10-2002 - Universidad Politecnica Catalunya (Barcelona)

#include "stylelib.h"
#include "ProcessContext.h"
#include "Interpreter.h"
#include "InterpreterMessages.h"
#include "SosofoObj.h"
#include "macros.h"


//multi-line-inline-note
/**
 *@class MultiLineInlineNoteFlowObj
 *@brief Class which implements the "multi-line-inline-note" flow object
 */
#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

class MultiLineInlineNoteFlowObj : public CompoundFlowObj {
public:
  void *operator new(size_t, Collector &c) {
    return c.allocateObject(1);
  }

  struct NIC{
      NIC();
      SosofoObj *openclose[2];
      FOTBuilder::MultiLineInlineNoteNIC *nic_;
  };
  
  MultiLineInlineNoteFlowObj();

  MultiLineInlineNoteFlowObj(const MultiLineInlineNoteFlowObj &fo);

  void traceSubObjects(Collector &) const;
  void processInner(ProcessContext &);
  FlowObj *copy(Collector &) const;
  bool hasNonInheritedC(const Identifier *) const;
  void setNonInheritedC(const Identifier *, ELObj *,
                        const Location &, Interpreter &);
private:
  Owner<MultiLineInlineNoteFlowObj::NIC> openclose_; 
};

#ifdef DSSSL_NAMESPACE
}
#endif
