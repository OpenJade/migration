#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif


#ifdef SP_NAMESPACE
}
#endif
// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#include "stylelib.h"

#ifdef SP_MANUAL_INST

#define SP_DEFINE_TEMPLATES
#include "Vector.h"
#include "NCVector.h"
#include "Ptr.h"
#include "Owner.h"
#include "PointerTable.h"
#include "OwnerTable.h"
#include "NamedTable.h"
#include "IList.h"
#include "IListIter.h"
#include "IQueue.h"
#include "HashTable.h"
#undef SP_DEFINE_TEMPLATES

#include "Insn.h"
#include "Expression.h"
#include "Interpreter.h"
#include "ELObj.h"
#include "Style.h"
#include "FOTBuilder.h"
#include "ProcessingMode.h"
#include "ProcessContext.h"
#include "SosofoObj.h"
#include "DssslSpecEventHandler.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

class ProcessingMode;
class SosofoObj;

#ifdef __DECCXX
#pragma define_template Ptr<Insn>
#else
#ifdef __xlC__
#pragma define(Ptr<Insn>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Ptr<Insn>;
#else
typedef Ptr<Insn> Dummy_0;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Owner<Expression>
#else
#ifdef __xlC__
#pragma define(Owner<Expression>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Owner<Expression>;
#else
typedef Owner<Expression> Dummy_1;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Vector<const Identifier *>
#else
#ifdef __xlC__
#pragma define(Vector<const Identifier *>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Vector<const Identifier *>;
#else
typedef Vector<const Identifier *> Dummy_2;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template NCVector<Owner<Expression> >
#else
#ifdef __xlC__
#pragma define(NCVector<Owner<Expression> >)
#else
#ifdef SP_ANSI_CLASS_INST
template class NCVector<Owner<Expression> >;
#else
typedef NCVector<Owner<Expression> > Dummy_3;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Vector<BoundVar>
#else
#ifdef __xlC__
#pragma define(Vector<BoundVar>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Vector<BoundVar>;
#else
typedef Vector<BoundVar> Dummy_4;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Owner<FOTBuilder>
#else
#ifdef __xlC__
#pragma define(Owner<FOTBuilder>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Owner<FOTBuilder>;
#else
typedef Owner<FOTBuilder> Dummy_5;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Owner<SaveFOTBuilder>
#else
#ifdef __xlC__
#pragma define(Owner<SaveFOTBuilder>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Owner<SaveFOTBuilder>;
#else
typedef Owner<SaveFOTBuilder> Dummy_6;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Owner<InputSource>
#else
#ifdef __xlC__
#pragma define(Owner<InputSource>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Owner<InputSource>;
#else
typedef Owner<InputSource> Dummy_7;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Vector<SymbolObj *>
#else
#ifdef __xlC__
#pragma define(Vector<SymbolObj *>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Vector<SymbolObj *>;
#else
typedef Vector<SymbolObj *> Dummy_8;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Owner<OutputCharStream>
#else
#ifdef __xlC__
#pragma define(Owner<OutputCharStream>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Owner<OutputCharStream>;
#else
typedef Owner<OutputCharStream> Dummy_9;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Ptr<Environment::FrameVarList>
#else
#ifdef __xlC__
#pragma define(Ptr<Environment::FrameVarList>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Ptr<Environment::FrameVarList>;
#else
typedef Ptr<Environment::FrameVarList> Dummy_10;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template ConstPtr<Environment::FrameVarList>
#else
#ifdef __xlC__
#pragma define(ConstPtr<Environment::FrameVarList>)
#else
#ifdef SP_ANSI_CLASS_INST
template class ConstPtr<Environment::FrameVarList>;
#else
typedef ConstPtr<Environment::FrameVarList> Dummy_11;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template PointerTable<SymbolObj *, StringC, Hash, SymbolObj>
#else
#ifdef __xlC__
#pragma define(PointerTable<SymbolObj *, StringC, Hash, SymbolObj>)
#else
#ifdef SP_ANSI_CLASS_INST
template class PointerTable<SymbolObj *, StringC, Hash, SymbolObj>;
#else
typedef PointerTable<SymbolObj *, StringC, Hash, SymbolObj> Dummy_12;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template NamedTableIter<Identifier>
#else
#ifdef __xlC__
#pragma define(NamedTableIter<Identifier>)
#else
#ifdef SP_ANSI_CLASS_INST
template class NamedTableIter<Identifier>;
#else
typedef NamedTableIter<Identifier> Dummy_13;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Ptr<StyleSpec>
#else
#ifdef __xlC__
#pragma define(Ptr<StyleSpec>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Ptr<StyleSpec>;
#else
typedef Ptr<StyleSpec> Dummy_14;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Ptr<InheritedC>
#else
#ifdef __xlC__
#pragma define(Ptr<InheritedC>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Ptr<InheritedC>;
#else
typedef Ptr<InheritedC> Dummy_15;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template ConstPtr<InheritedC>
#else
#ifdef __xlC__
#pragma define(ConstPtr<InheritedC>)
#else
#ifdef SP_ANSI_CLASS_INST
template class ConstPtr<InheritedC>;
#else
typedef ConstPtr<InheritedC> Dummy_16;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Vector<ConstPtr<InheritedC> >
#else
#ifdef __xlC__
#pragma define(Vector<ConstPtr<InheritedC> >)
#else
#ifdef SP_ANSI_CLASS_INST
template class Vector<ConstPtr<InheritedC> >;
#else
typedef Vector<ConstPtr<InheritedC> > Dummy_17;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Vector<const Vector<ConstPtr<InheritedC> > *>
#else
#ifdef __xlC__
#pragma define(Vector<const Vector<ConstPtr<InheritedC> > *>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Vector<const Vector<ConstPtr<InheritedC> > *>;
#else
typedef Vector<const Vector<ConstPtr<InheritedC> > *> Dummy_18;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Vector<const VarStyleObj *>
#else
#ifdef __xlC__
#pragma define(Vector<const VarStyleObj *>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Vector<const VarStyleObj *>;
#else
typedef Vector<const VarStyleObj *> Dummy_19;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Ptr<InheritedCInfo>
#else
#ifdef __xlC__
#pragma define(Ptr<InheritedCInfo>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Ptr<InheritedCInfo>;
#else
typedef Ptr<InheritedCInfo> Dummy_20;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template NamedTable<ProcessingMode::ElementRules>
#else
#ifdef __xlC__
#pragma define(NamedTable<ProcessingMode::ElementRules>)
#else
#ifdef SP_ANSI_CLASS_INST
template class NamedTable<ProcessingMode::ElementRules>;
#else
typedef NamedTable<ProcessingMode::ElementRules> Dummy_21;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template NamedTableIter<ProcessingMode::ElementRules>
#else
#ifdef __xlC__
#pragma define(NamedTableIter<ProcessingMode::ElementRules>)
#else
#ifdef SP_ANSI_CLASS_INST
template class NamedTableIter<ProcessingMode::ElementRules>;
#else
typedef NamedTableIter<ProcessingMode::ElementRules> Dummy_22;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template NamedTable<ProcessingMode::IdRule>
#else
#ifdef __xlC__
#pragma define(NamedTable<ProcessingMode::IdRule>)
#else
#ifdef SP_ANSI_CLASS_INST
template class NamedTable<ProcessingMode::IdRule>;
#else
typedef NamedTable<ProcessingMode::IdRule> Dummy_23;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template NamedTableIter<ProcessingMode::IdRule>
#else
#ifdef __xlC__
#pragma define(NamedTableIter<ProcessingMode::IdRule>)
#else
#ifdef SP_ANSI_CLASS_INST
template class NamedTableIter<ProcessingMode::IdRule>;
#else
typedef NamedTableIter<ProcessingMode::IdRule> Dummy_24;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template NamedTableIter<ProcessingMode>
#else
#ifdef __xlC__
#pragma define(NamedTableIter<ProcessingMode>)
#else
#ifdef SP_ANSI_CLASS_INST
template class NamedTableIter<ProcessingMode>;
#else
typedef NamedTableIter<ProcessingMode> Dummy_25;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template IList<SaveFOTBuilder>
#else
#ifdef __xlC__
#pragma define(IList<SaveFOTBuilder>)
#else
#ifdef SP_ANSI_CLASS_INST
template class IList<SaveFOTBuilder>;
#else
typedef IList<SaveFOTBuilder> Dummy_26;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Vector<const ProcessingMode *>
#else
#ifdef __xlC__
#pragma define(Vector<const ProcessingMode *>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Vector<const ProcessingMode *>;
#else
typedef Vector<const ProcessingMode *> Dummy_27;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template NCVector<ProcessingMode::Part>
#else
#ifdef __xlC__
#pragma define(NCVector<ProcessingMode::Part>)
#else
#ifdef SP_ANSI_CLASS_INST
template class NCVector<ProcessingMode::Part>;
#else
typedef NCVector<ProcessingMode::Part> Dummy_28;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Owner<ProcessingMode::Rule>
#else
#ifdef __xlC__
#pragma define(Owner<ProcessingMode::Rule>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Owner<ProcessingMode::Rule>;
#else
typedef Owner<ProcessingMode::Rule> Dummy_29;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Owner<FOTBuilder::DisplayGroupNIC>
#else
#ifdef __xlC__
#pragma define(Owner<FOTBuilder::DisplayGroupNIC>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Owner<FOTBuilder::DisplayGroupNIC>;
#else
typedef Owner<FOTBuilder::DisplayGroupNIC> Dummy_30;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Vector<SosofoObj *>
#else
#ifdef __xlC__
#pragma define(Vector<SosofoObj *>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Vector<SosofoObj *>;
#else
typedef Vector<SosofoObj *> Dummy_31;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Vector<FOTBuilder *>
#else
#ifdef __xlC__
#pragma define(Vector<FOTBuilder *>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Vector<FOTBuilder *>;
#else
typedef Vector<FOTBuilder *> Dummy_32;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template IQueue<SaveFOTBuilder>
#else
#ifdef __xlC__
#pragma define(IQueue<SaveFOTBuilder>)
#else
#ifdef SP_ANSI_CLASS_INST
template class IQueue<SaveFOTBuilder>;
#else
typedef IQueue<SaveFOTBuilder> Dummy_33;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template NCVector<IQueue<SaveFOTBuilder> >
#else
#ifdef __xlC__
#pragma define(NCVector<IQueue<SaveFOTBuilder> >)
#else
#ifdef SP_ANSI_CLASS_INST
template class NCVector<IQueue<SaveFOTBuilder> >;
#else
typedef NCVector<IQueue<SaveFOTBuilder> > Dummy_34;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template IList<ProcessContext::Connection>
#else
#ifdef __xlC__
#pragma define(IList<ProcessContext::Connection>)
#else
#ifdef SP_ANSI_CLASS_INST
template class IList<ProcessContext::Connection>;
#else
typedef IList<ProcessContext::Connection> Dummy_35;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template IListIter<ProcessContext::Connection>
#else
#ifdef __xlC__
#pragma define(IListIter<ProcessContext::Connection>)
#else
#ifdef SP_ANSI_CLASS_INST
template class IListIter<ProcessContext::Connection>;
#else
typedef IListIter<ProcessContext::Connection> Dummy_36;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template IList<ProcessContext::Connectable>
#else
#ifdef __xlC__
#pragma define(IList<ProcessContext::Connectable>)
#else
#ifdef SP_ANSI_CLASS_INST
template class IList<ProcessContext::Connectable>;
#else
typedef IList<ProcessContext::Connectable> Dummy_37;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template IListIter<ProcessContext::Connectable>
#else
#ifdef __xlC__
#pragma define(IListIter<ProcessContext::Connectable>)
#else
#ifdef SP_ANSI_CLASS_INST
template class IListIter<ProcessContext::Connectable>;
#else
typedef IListIter<ProcessContext::Connectable> Dummy_38;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Ptr<PopList>
#else
#ifdef __xlC__
#pragma define(Ptr<PopList>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Ptr<PopList>;
#else
typedef Ptr<PopList> Dummy_39;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Vector<Ptr<InheritedCInfo> >
#else
#ifdef __xlC__
#pragma define(Vector<Ptr<InheritedCInfo> >)
#else
#ifdef SP_ANSI_CLASS_INST
template class Vector<Ptr<InheritedCInfo> >;
#else
typedef Vector<Ptr<InheritedCInfo> > Dummy_40;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template NCVector<ProcessContext::Port>
#else
#ifdef __xlC__
#pragma define(NCVector<ProcessContext::Port>)
#else
#ifdef SP_ANSI_CLASS_INST
template class NCVector<ProcessContext::Port>;
#else
typedef NCVector<ProcessContext::Port> Dummy_41;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template NCVector<CaseExpression::Case>
#else
#ifdef __xlC__
#pragma define(NCVector<CaseExpression::Case>)
#else
#ifdef SP_ANSI_CLASS_INST
template class NCVector<CaseExpression::Case>;
#else
typedef NCVector<CaseExpression::Case> Dummy_42;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template NamedTable<NumberCache::Entry>
#else
#ifdef __xlC__
#pragma define(NamedTable<NumberCache::Entry>)
#else
#ifdef SP_ANSI_CLASS_INST
template class NamedTable<NumberCache::Entry>;
#else
typedef NamedTable<NumberCache::Entry> Dummy_43;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template NCVector<NamedTable<NumberCache::Entry> >
#else
#ifdef __xlC__
#pragma define(NCVector<NamedTable<NumberCache::Entry> >)
#else
#ifdef SP_ANSI_CLASS_INST
template class NCVector<NamedTable<NumberCache::Entry> >;
#else
typedef NCVector<NamedTable<NumberCache::Entry> > Dummy_44;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template NamedTable<NumberCache::ElementEntry>
#else
#ifdef __xlC__
#pragma define(NamedTable<NumberCache::ElementEntry>)
#else
#ifdef SP_ANSI_CLASS_INST
template class NamedTable<NumberCache::ElementEntry>;
#else
typedef NamedTable<NumberCache::ElementEntry> Dummy_45;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template NCVector<NamedTable<NumberCache::ElementEntry> >
#else
#ifdef __xlC__
#pragma define(NCVector<NamedTable<NumberCache::ElementEntry> >)
#else
#ifdef SP_ANSI_CLASS_INST
template class NCVector<NamedTable<NumberCache::ElementEntry> >;
#else
typedef NCVector<NamedTable<NumberCache::ElementEntry> > Dummy_46;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Vector<ELObj *>
#else
#ifdef __xlC__
#pragma define(Vector<ELObj *>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Vector<ELObj *>;
#else
typedef Vector<ELObj *> Dummy_47;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Vector<ConstPtr<Insn> >
#else
#ifdef __xlC__
#pragma define(Vector<ConstPtr<Insn> >)
#else
#ifdef SP_ANSI_CLASS_INST
template class Vector<ConstPtr<Insn> >;
#else
typedef Vector<ConstPtr<Insn> > Dummy_48;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Vector<StyleObj *>
#else
#ifdef __xlC__
#pragma define(Vector<StyleObj *>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Vector<StyleObj *>;
#else
typedef Vector<StyleObj *> Dummy_49;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Vector<Vector<StyleObj *> >
#else
#ifdef __xlC__
#pragma define(Vector<Vector<StyleObj *> >)
#else
#ifdef SP_ANSI_CLASS_INST
template class Vector<Vector<StyleObj *> >;
#else
typedef Vector<Vector<StyleObj *> > Dummy_50;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template IList<ProcessContext::Table>
#else
#ifdef __xlC__
#pragma define(IList<ProcessContext::Table>)
#else
#ifdef SP_ANSI_CLASS_INST
template class IList<ProcessContext::Table>;
#else
typedef IList<ProcessContext::Table> Dummy_51;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template IListIter<ProcessContext::Table>
#else
#ifdef __xlC__
#pragma define(IListIter<ProcessContext::Table>)
#else
#ifdef SP_ANSI_CLASS_INST
template class IListIter<ProcessContext::Table>;
#else
typedef IListIter<ProcessContext::Table> Dummy_52;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Vector<String<char> >
#else
#ifdef __xlC__
#pragma define(Vector<String<char> >)
#else
#ifdef SP_ANSI_CLASS_INST
template class Vector<String<char> >;
#else
typedef Vector<String<char> > Dummy_53;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Owner<LengthSpec>
#else
#ifdef __xlC__
#pragma define(Owner<LengthSpec>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Owner<LengthSpec>;
#else
typedef Owner<LengthSpec> Dummy_54;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Vector<FOTBuilder::MultiMode>
#else
#ifdef __xlC__
#pragma define(Vector<FOTBuilder::MultiMode>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Vector<FOTBuilder::MultiMode>;
#else
typedef Vector<FOTBuilder::MultiMode> Dummy_55;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Vector<Vector<FOTBuilder::MultiMode> >
#else
#ifdef __xlC__
#pragma define(Vector<Vector<FOTBuilder::MultiMode> >)
#else
#ifdef SP_ANSI_CLASS_INST
template class Vector<Vector<FOTBuilder::MultiMode> >;
#else
typedef Vector<Vector<FOTBuilder::MultiMode> > Dummy_56;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template HashTable<StringC,FunctionObj *>
#else
#ifdef __xlC__
#pragma define(HashTable<StringC,FunctionObj *>)
#else
#ifdef SP_ANSI_CLASS_INST
template class HashTable<StringC,FunctionObj *>;
#else
typedef HashTable<StringC,FunctionObj *> Dummy_57;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template HashTableItem<StringC,FunctionObj *>
#else
#ifdef __xlC__
#pragma define(HashTableItem<StringC,FunctionObj *>)
#else
#ifdef SP_ANSI_CLASS_INST
template class HashTableItem<StringC,FunctionObj *>;
#else
typedef HashTableItem<StringC,FunctionObj *> Dummy_58;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Vector<DssslSpecEventHandler::Part *>
#else
#ifdef __xlC__
#pragma define(Vector<DssslSpecEventHandler::Part *>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Vector<DssslSpecEventHandler::Part *>;
#else
typedef Vector<DssslSpecEventHandler::Part *> Dummy_59;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Vector<DssslSpecEventHandler::PartHeader *>
#else
#ifdef __xlC__
#pragma define(Vector<DssslSpecEventHandler::PartHeader *>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Vector<DssslSpecEventHandler::PartHeader *>;
#else
typedef Vector<DssslSpecEventHandler::PartHeader *> Dummy_60;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template IListIter<DssslSpecEventHandler::BodyElement>
#else
#ifdef __xlC__
#pragma define(IListIter<DssslSpecEventHandler::BodyElement>)
#else
#ifdef SP_ANSI_CLASS_INST
template class IListIter<DssslSpecEventHandler::BodyElement>;
#else
typedef IListIter<DssslSpecEventHandler::BodyElement> Dummy_61;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template IList<DssslSpecEventHandler::BodyElement>
#else
#ifdef __xlC__
#pragma define(IList<DssslSpecEventHandler::BodyElement>)
#else
#ifdef SP_ANSI_CLASS_INST
template class IList<DssslSpecEventHandler::BodyElement>;
#else
typedef IList<DssslSpecEventHandler::BodyElement> Dummy_62;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template IListIter<DssslSpecEventHandler::PartHeader>
#else
#ifdef __xlC__
#pragma define(IListIter<DssslSpecEventHandler::PartHeader>)
#else
#ifdef SP_ANSI_CLASS_INST
template class IListIter<DssslSpecEventHandler::PartHeader>;
#else
typedef IListIter<DssslSpecEventHandler::PartHeader> Dummy_63;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template IList<DssslSpecEventHandler::PartHeader>
#else
#ifdef __xlC__
#pragma define(IList<DssslSpecEventHandler::PartHeader>)
#else
#ifdef SP_ANSI_CLASS_INST
template class IList<DssslSpecEventHandler::PartHeader>;
#else
typedef IList<DssslSpecEventHandler::PartHeader> Dummy_64;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template IListIter<DssslSpecEventHandler::Doc>
#else
#ifdef __xlC__
#pragma define(IListIter<DssslSpecEventHandler::Doc>)
#else
#ifdef SP_ANSI_CLASS_INST
template class IListIter<DssslSpecEventHandler::Doc>;
#else
typedef IListIter<DssslSpecEventHandler::Doc> Dummy_65;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template IList<DssslSpecEventHandler::Doc>
#else
#ifdef __xlC__
#pragma define(IList<DssslSpecEventHandler::Doc>)
#else
#ifdef SP_ANSI_CLASS_INST
template class IList<DssslSpecEventHandler::Doc>;
#else
typedef IList<DssslSpecEventHandler::Doc> Dummy_66;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Owner<DssslSpecEventHandler::SpecPart>
#else
#ifdef __xlC__
#pragma define(Owner<DssslSpecEventHandler::SpecPart>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Owner<DssslSpecEventHandler::SpecPart>;
#else
typedef Owner<DssslSpecEventHandler::SpecPart> Dummy_67;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Owner<Location>
#else
#ifdef __xlC__
#pragma define(Owner<Location>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Owner<Location>;
#else
typedef Owner<Location> Dummy_68;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Owner<FOTBuilder::DisplaySpace>
#else
#ifdef __xlC__
#pragma define(Owner<FOTBuilder::DisplaySpace>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Owner<FOTBuilder::DisplaySpace>;
#else
typedef Owner<FOTBuilder::DisplaySpace> Dummy_69;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Owner<FOTBuilder::InlineSpace>
#else
#ifdef __xlC__
#pragma define(Owner<FOTBuilder::InlineSpace>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Owner<FOTBuilder::InlineSpace>;
#else
typedef Owner<FOTBuilder::InlineSpace> Dummy_70;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Owner<FOTBuilder::Address>
#else
#ifdef __xlC__
#pragma define(Owner<FOTBuilder::Address>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Owner<FOTBuilder::Address>;
#else
typedef Owner<FOTBuilder::Address> Dummy_71;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Vector<String<char> *>
#else
#ifdef __xlC__
#pragma define(Vector<String<char> *>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Vector<String<char> *>;
#else
typedef Vector<String<char> *> Dummy_72;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template PointerTable<String<char> *, String<char>, Interpreter::StringSet, Interpreter::StringSet>
#else
#ifdef __xlC__
#pragma define(PointerTable<String<char> *, String<char>, Interpreter::StringSet, Interpreter::StringSet>)
#else
#ifdef SP_ANSI_CLASS_INST
template class PointerTable<String<char> *, String<char>, Interpreter::StringSet, Interpreter::StringSet>;
#else
typedef PointerTable<String<char> *, String<char>, Interpreter::StringSet, Interpreter::StringSet> Dummy_73;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template OwnerTable<String<char>, String<char>, Interpreter::StringSet, Interpreter::StringSet>
#else
#ifdef __xlC__
#pragma define(OwnerTable<String<char>, String<char>, Interpreter::StringSet, Interpreter::StringSet>)
#else
#ifdef SP_ANSI_CLASS_INST
template class OwnerTable<String<char>, String<char>, Interpreter::StringSet, Interpreter::StringSet>;
#else
typedef OwnerTable<String<char>, String<char>, Interpreter::StringSet, Interpreter::StringSet> Dummy_74;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Vector<FOTBuilder::GlyphId>
#else
#ifdef __xlC__
#pragma define(Vector<FOTBuilder::GlyphId>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Vector<FOTBuilder::GlyphId>;
#else
typedef Vector<FOTBuilder::GlyphId> Dummy_75;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Ptr<FOTBuilder::GlyphSubstTable>
#else
#ifdef __xlC__
#pragma define(Ptr<FOTBuilder::GlyphSubstTable>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Ptr<FOTBuilder::GlyphSubstTable>;
#else
typedef Ptr<FOTBuilder::GlyphSubstTable> Dummy_76;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template ConstPtr<FOTBuilder::GlyphSubstTable>
#else
#ifdef __xlC__
#pragma define(ConstPtr<FOTBuilder::GlyphSubstTable>)
#else
#ifdef SP_ANSI_CLASS_INST
template class ConstPtr<FOTBuilder::GlyphSubstTable>;
#else
typedef ConstPtr<FOTBuilder::GlyphSubstTable> Dummy_77;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Vector<ConstPtr<FOTBuilder::GlyphSubstTable> >
#else
#ifdef __xlC__
#pragma define(Vector<ConstPtr<FOTBuilder::GlyphSubstTable> >)
#else
#ifdef SP_ANSI_CLASS_INST
template class Vector<ConstPtr<FOTBuilder::GlyphSubstTable> >;
#else
typedef Vector<ConstPtr<FOTBuilder::GlyphSubstTable> > Dummy_78;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Owner<FOTBuilder::ExtensionFlowObj>
#else
#ifdef __xlC__
#pragma define(Owner<FOTBuilder::ExtensionFlowObj>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Owner<FOTBuilder::ExtensionFlowObj>;
#else
typedef Owner<FOTBuilder::ExtensionFlowObj> Dummy_79;
#endif
#endif
#endif
#ifdef __DECCXX
#pragma define_template Owner<FOTBuilder::CompoundExtensionFlowObj>
#else
#ifdef __xlC__
#pragma define(Owner<FOTBuilder::CompoundExtensionFlowObj>)
#else
#ifdef SP_ANSI_CLASS_INST
template class Owner<FOTBuilder::CompoundExtensionFlowObj>;
#else
typedef Owner<FOTBuilder::CompoundExtensionFlowObj> Dummy_80;
#endif
#endif
#endif

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* SP_MANUAL_INST */
