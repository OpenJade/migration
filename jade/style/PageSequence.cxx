
#include "PageSequence.h"

PageSequenceFlowObj::PageSequenceFlowObj()
{
  hasSubObjects_ = 1;
}

PageSequenceFlowObj::PageSequenceFlowObj(const PageSequenceFlowObj &fo)
: CompoundFlowObj(fo)
{
}

void PageSequenceFlowObj::traceSubObjects(Collector &c) const
{
  CompoundFlowObj::traceSubObjects(c);
}

void PageSequenceFlowObj::processInner(ProcessContext &context)
{
  FOTBuilder &fotb = context.currentFOTBuilder();
  fotb.startPageSequence();
  CompoundFlowObj::processInner(context);
  fotb.endPageSequence();
} 

FlowObj *PageSequenceFlowObj::copy(Collector &c) const
{
  return new (c) PageSequenceFlowObj(*this);
}

