#include "MultiLineInlineNote.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif


MultiLineInlineNoteFlowObj::MultiLineInlineNoteFlowObj()
: openclose_(new NIC)
{
 hasSubObjects_ = 1;
}

MultiLineInlineNoteFlowObj::MultiLineInlineNoteFlowObj(const MultiLineInlineNoteFlowObj &fo)
: CompoundFlowObj(fo), openclose_(new NIC(*fo.openclose_))
{
}

void MultiLineInlineNoteFlowObj::traceSubObjects(Collector &c) const
{
  c.trace(openclose_->openclose[0]);
  c.trace(openclose_->openclose[1]);
  CompoundFlowObj::traceSubObjects(c);
}

void MultiLineInlineNoteFlowObj::processInner(ProcessContext &context)
{
  //fotb es siempre el nodo FOT actual
  FOTBuilder &fotb = context.currentFOTBuilder();
  //dos apuntadores diferentes para port al FOTBuilder
  FOTBuilder* openclosefotb[2];
  FOTBuilder::MultiLineInlineNoteNIC* tmpnic;
  tmpnic = openclose_->nic_;
  fotb.startMultiLineInlineNote(*tmpnic, openclosefotb);

  //si existe para i=0 entrarara para open sino para close
  for (int i = 0; i<2; i++){
   if (openclose_->openclose[i]) {
      context.pushPrincipalPort(openclosefotb[i]);
      openclose_->openclose[i]->process(context);
      context.popPrincipalPort();
   }
  }
  fotb.endMultiLineInlineNoteOpenClose();
  CompoundFlowObj::processInner(context);
  fotb.endMultiLineInlineNote();
}

FlowObj *MultiLineInlineNoteFlowObj::copy(Collector &c) const
{
  return new (c) MultiLineInlineNoteFlowObj(*this);
}

bool MultiLineInlineNoteFlowObj::hasNonInheritedC(const Identifier *ident) const
{
  Identifier::SyntacticKey key;
  if (ident->syntacticKey(key)) {
    switch (key) {
    case Identifier::keyBreakBeforePriority:
    case Identifier::keyBreakAfterPriority:
    case Identifier::keyOpen:
    case Identifier::keyClose:
     return 1;
    default:
      break;
    }
  }
  return 0;

}

void MultiLineInlineNoteFlowObj::setNonInheritedC(const Identifier *ident,
                                                 ELObj *obj,
                                                 const Location &loc,
                                                 Interpreter &interp)
{
 SosofoObj *sosofo;
 Identifier::SyntacticKey key;
 if (ident->syntacticKey(key)) {
  switch (key) {
   case Identifier::keyBreakBeforePriority:
      interp.convertIntegerC(obj, ident, loc, openclose_->nic_->breakBeforePriority);
      return;
   case Identifier::keyBreakAfterPriority:
      interp.convertIntegerC(obj, ident, loc, openclose_->nic_->breakAfterPriority);
      return;
   case Identifier::keyOpen:
      sosofo = obj->asSosofo();
      if (!sosofo) {
         interp.setNextLocation(loc);
         interp.message(InterpreterMessages::invalidCharacteristicValue,
                        StringMessageArg(ident->name()));
         return;
      }
      openclose_->openclose[0] = sosofo;
      return;
   case Identifier::keyClose:
      sosofo = obj->asSosofo();
      if (!sosofo) {
         interp.setNextLocation(loc);
         interp.message(InterpreterMessages::invalidCharacteristicValue,
                        StringMessageArg(ident->name()));
         return;
      }
      openclose_->openclose[1] = sosofo;
      return;
   default:
    break;
  }
 }
 CANNOT_HAPPEN();
}

MultiLineInlineNoteFlowObj::NIC::NIC()
: nic_(new FOTBuilder::MultiLineInlineNoteNIC())
{
   for (int i = 0; i < 2; i++)
    openclose[i] = 0;
}

#ifdef DSSSL_NAMESPACE
}
#endif
