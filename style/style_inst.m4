// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#include "stylelib.h"

#ifdef SP_MANUAL_INST

#define SP_DEFINE_TEMPLATES
#include <OpenSP/Vector.h>
#include <OpenSP/NCVector.h>
#include <OpenSP/Ptr.h>
#include <OpenSP/Owner.h>
#include <OpenSP/PointerTable.h>
#include <OpenSP/OwnerTable.h>
#include <OpenSP/NamedTable.h>
#include <OpenSP/IList.h>
#include <OpenSP/IListIter.h>
#include <OpenSP/IQueue.h>
#include <OpenSP/HashTable.h>
#include <OpenSP/CharMap.h>
#ifdef SP_SIZEOF_BOOL_1
#include <OpenSP/XcharMap.h>
#endif
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
#include <OpenSP/Boolean.h>
#include "Node.h"
#include "DssslApp.h"
#include "Pattern.h"
#include "MacroFlowObj.h"

#ifdef DSSSL_NAMESPACE
class DSSSL_NAMESPACE::ProcessingMode;
class DSSSL_NAMESPACE::SosofoObj;
#else
class ProcessingMode;
class SosofoObj;
#endif

#if _MSC_VER >= 1100

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

#ifdef DSSSL_NAMESPACE
using namespace DSSSL_NAMESPACE;
#endif

#ifdef GROVE_NAMESPACE
using namespace GROVE_NAMESPACE;
#endif

#else

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

#endif

__instantiate(Ptr<Insn>)
__instantiate(Owner<Expression>)
__instantiate(Vector<const Identifier *>)
__instantiate(NCVector<Owner<Expression> >)
__instantiate(Vector<BoundVar>)
__instantiate(Owner<FOTBuilder>)
__instantiate(Owner<SaveFOTBuilder>)
__instantiate(Owner<InputSource>)
__instantiate(Vector<SymbolObj *>)
__instantiate(Owner<OutputCharStream>)
__instantiate(Ptr<Environment::FrameVarList>)
__instantiate(ConstPtr<Environment::FrameVarList>)
__instantiate(`PointerTable<SymbolObj *, StringC, Hash, SymbolObj>')
__instantiate(NamedTableIter<Identifier>)
__instantiate(Ptr<StyleSpec>)
__instantiate(Ptr<InheritedC>)
__instantiate(ConstPtr<InheritedC>)
__instantiate(Vector<ConstPtr<InheritedC> >)
__instantiate(Vector<const Vector<ConstPtr<InheritedC> > *>)
__instantiate(Vector<const VarStyleObj *>)
__instantiate(Ptr<InheritedCInfo>)
__instantiate(NamedTable<ProcessingMode::ElementRules>)
__instantiate(NamedTableIter<ProcessingMode::ElementRules>)
__instantiate(NamedTableIter<ProcessingMode>)
__instantiate(NCVector<ProcessingMode::GroveRules>)
__instantiate(Vector<ProcessingMode::Rule>)
__instantiate(Ptr<ProcessingMode::Action>)
__instantiate(IList<ProcessingMode::ElementRule>)
__instantiate(IListIter<ProcessingMode::ElementRule>)
__instantiate(Vector<const ProcessingMode::ElementRule *>)
__instantiate(IList<SaveFOTBuilder>)
__instantiate(Vector<const ProcessingMode *>)
__instantiate(Owner<FOTBuilder::DisplayGroupNIC>)
__instantiate(Vector<SosofoObj *>)
__instantiate(Vector<FOTBuilder *>)
__instantiate(IQueue<NodeSaveFOTBuilder>)
__instantiate(NCVector<IQueue<NodeSaveFOTBuilder> >)
__instantiate(IList<ProcessContext::Connection>)
__instantiate(IListIter<ProcessContext::Connection>)
__instantiate(IList<ProcessContext::Connectable>)
__instantiate(IListIter<ProcessContext::Connectable>)
__instantiate(Ptr<PopList>)
__instantiate(Vector<Ptr<InheritedCInfo> >)
__instantiate(NCVector<ProcessContext::Port>)
__instantiate(NCVector<CaseExpression::Case>)
__instantiate(NamedTable<NumberCache::Entry>)
__instantiate(NCVector<NamedTable<NumberCache::Entry> >)
__instantiate(NamedTable<NumberCache::ElementEntry>)
__instantiate(NCVector<NamedTable<NumberCache::ElementEntry> >)
__instantiate(Vector<ELObj *>)
__instantiate(Vector<ConstPtr<Insn> >)
__instantiate(Vector<StyleObj *>)
__instantiate(Vector<Vector<StyleObj *> >)
__instantiate(IList<ProcessContext::Table>)
__instantiate(IListIter<ProcessContext::Table>)
__instantiate(Vector<String<char> >)
__instantiate(Owner<LengthSpec>)
__instantiate(Vector<FOTBuilder::MultiMode>)
__instantiate(Vector<Vector<FOTBuilder::MultiMode> >)
__instantiate(Vector<FOTBuilder::CharacterNIC>)
__instantiate(`HashTable<StringC,FunctionObj *>')
__instantiate(`HashTableItem<StringC,FunctionObj *>')
__instantiate(Vector<DssslSpecEventHandler::Part *>)
__instantiate(Vector<DssslSpecEventHandler::PartHeader *>)
__instantiate(IListIter<DssslSpecEventHandler::BodyElement>)
__instantiate(IList<DssslSpecEventHandler::BodyElement>)
__instantiate(IListIter<DssslSpecEventHandler::PartHeader>)
__instantiate(IList<DssslSpecEventHandler::PartHeader>)
__instantiate(IListIter<DssslSpecEventHandler::Doc>)
__instantiate(IList<DssslSpecEventHandler::Doc>)
__instantiate(Owner<DssslSpecEventHandler::SpecPart>)
__instantiate(Owner<Location>)
__instantiate(Owner<FOTBuilder::DisplaySpace>)
__instantiate(Owner<FOTBuilder::InlineSpace>)
__instantiate(Owner<FOTBuilder::Address>)
__instantiate(Vector<String<char> *>)
__instantiate(`PointerTable<String<char> *, String<char>, Interpreter::StringSet, Interpreter::StringSet>')
__instantiate(`OwnerTable<String<char>, String<char>, Interpreter::StringSet, Interpreter::StringSet>')
__instantiate(Vector<FOTBuilder::GlyphId>)
__instantiate(Ptr<FOTBuilder::GlyphSubstTable>)
__instantiate(ConstPtr<FOTBuilder::GlyphSubstTable>)
__instantiate(Vector<ConstPtr<FOTBuilder::GlyphSubstTable> >)
__instantiate(Owner<FOTBuilder::ExtensionFlowObj>)
__instantiate(Owner<FOTBuilder::CompoundExtensionFlowObj>)
__instantiate(`HashTable<StringC,NodePtr>')
__instantiate(`HashTableItem<StringC,NodePtr>')
__instantiate(Vector<ProcessContext::NodeStackEntry>)
__instantiate(Vector<const Pattern::Element *>)
__instantiate(NCVector<Pattern>)
__instantiate(IList<Pattern::Element>)
__instantiate(IListIter<Pattern::Element>)
__instantiate(IList<Pattern::Qualifier>)
__instantiate(IListIter<Pattern::Qualifier>)
__instantiate(Ptr<MacroFlowObj::Definition>)
__instantiate(Owner<MacroFlowObj::S>)
__instantiate(`HashTable<StringC,CharPart>')
__instantiate(`HashTableItem<StringC,CharPart>')
__instantiate(`HashTable<StringC,StringC>')
__instantiate(`HashTableItem<StringC,StringC>')
#ifdef SP_SIZEOF_BOOL_1
__instantiate(XcharMap<char>)
__instantiate(SharedXcharMap<char>)
__instantiate(Ptr<SharedXcharMap<char> >)
__instantiate(ConstPtr<SharedXcharMap<char> >)
#endif
__instantiate(CharMap<ELObjPart>)
__instantiate(CharMapPage<ELObjPart>)
__instantiate(CharMapColumn<ELObjPart>)
__instantiate(CharMapResource<ELObjPart>)
__instantiate(Ptr<CharMapResource<ELObjPart> >)
__instantiate(ConstPtr<CharMapResource<ELObjPart> >)
#ifndef _MSC_VER
__instantiate(`OneRefArgCall<FOTBuilder::Address>')
__instantiate(`OneArgCall<char const *>')
__instantiate(`OneRefArgCall<FOTBuilder::TableCellNIC>')
__instantiate(`OneArgCall<FOTBuilder::Symbol>')
__instantiate(`OneArgCall<unsigned int>')
__instantiate(`OneArgCall<unsigned short>')
__instantiate(`OneRefArgCall<FOTBuilder::CharacterNIC>')
__instantiate(`OneRefArgCall<NodePtr>')
__instantiate(`OneRefArgCall<FOTBuilder::LeaderNIC>')
__instantiate(`OneArgCall<long>')
__instantiate(`OneRefArgCall<FOTBuilder::GridNIC>')
__instantiate(`OneRefArgCall<FOTBuilder::TableColumnNIC>')
__instantiate(`OneRefArgCall<FOTBuilder::RuleNIC>')
__instantiate(`OneRefArgCall<FOTBuilder::DisplayGroupNIC>')
__instantiate(`OneRefArgCall<String<unsigned short> >')
__instantiate(`OneRefArgCall<FOTBuilder::LengthSpec>')
__instantiate(`OneRefArgCall<FOTBuilder::InlineNIC>')
__instantiate(`OneRefArgCall<FOTBuilder::DisplayNIC>')
__instantiate(`OneRefArgCall<FOTBuilder::TableNIC>')
__instantiate(`OneRefArgCall<Vector<ConstPtr<FOTBuilder::GlyphSubstTable> > >')
__instantiate(`OneRefArgCall<FOTBuilder::BoxNIC>')
__instantiate(`OneRefArgCall<FOTBuilder::GridCellNIC>')
__instantiate(`OneRefArgCall<FOTBuilder::DeviceRGBColor>')
__instantiate(`OneRefArgCall<FOTBuilder::OptLengthSpec>')
__instantiate(`OneArgCall<bool>')
__instantiate(`OneRefArgCall<FOTBuilder::InlineSpace>')
__instantiate(`OneRefArgCall<FOTBuilder::ExternalGraphicNIC>') 
__instantiate(`Owner<CharMap<NCVector<CharProp::addedProp_>::size_type> >')
__instantiate(`CharMap<NCVector<CharProp::addedProp_>::size_type>')
__instantiate(`CharMapColumn<NCVector<CharProp::addedProp_>::size_type>')
__instantiate(`CharMapPage<NCVector<CharProp::addedProp_>::size_type>')
__instantiate(`NCVector<CharProp::addedProp_>')
__instantiate(`Vector<Ptr<ProcessContext::Validator> >')
__instantiate(`Ptr<ProcessContext::Validator>')
__instantiate(`OneRefArgCall<FOTBuilder::OptInlineSpace>') 
__instantiate(`NCVector<TransformationMode::Association>') 
__instantiate(`CharMap<MaybeIntegerCharPropValues::ValT_>')
__instantiate(`CharMapColumn<MaybeIntegerCharPropValues::ValT_>')
__instantiate(`CharMapPage<MaybeIntegerCharPropValues::ValT_>')
__instantiate(`Vector<ProcessContext::Validator *>')
__instantiate(`CharMap<PublicIdCharPropValues::ValT_>')
__instantiate(`CharMapColumn<PublicIdCharPropValues::ValT_>')
__instantiate(`CharMapPage<PublicIdCharPropValues::ValT_>')
__instantiate(`Ptr<TransformationMode::Action>')
__instantiate(`CharMap<BooleanCharPropValues::ValT_>')
__instantiate(`CharMapColumn<BooleanCharPropValues::ValT_>')
__instantiate(`CharMapPage<BooleanCharPropValues::ValT_>')
__instantiate(`CharMap<IntegerCharPropValues::ValT_>')
__instantiate(`CharMapColumn<IntegerCharPropValues::ValT_>')
__instantiate(`CharMapPage<IntegerCharPropValues::ValT_>')
__instantiate(`CharMap<ELObj *>')
__instantiate(`CharMapColumn<ELObj *>')
__instantiate(`CharMapPage<ELObj *>')
__instantiate(`CharMap<SymbolCharPropValues::ValT_>')
__instantiate(`CharMapColumn<SymbolCharPropValues::ValT_>')
__instantiate(`CharMapPage<SymbolCharPropValues::ValT_>')
__instantiate(`Vector<CreateSpecObj *>')
__instantiate(`Owner<String<unsigned short> >')
__instantiate(`String<unsigned short>')
__instantiate(`Vector<CharProp *>')
__instantiate(`OneRefArgCall<Vector<FOTBuilder::CharacterNIC> >')
__instantiate(`Vector<FOTBuilder::CharacterNIC>')
#endif

#ifdef SP_NAMESPACE
}
#endif

#endif /* SP_MANUAL_INST */
