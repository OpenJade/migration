// Copyright (c) 1994 James Clark
// See the file COPYING for copying permission.

#include "splib.h"

#ifdef SP_MANUAL_INST

#define SP_DEFINE_TEMPLATES
#include "Owner.h"
#include "CopyOwner.h"
#include <vector>
#include "Ptr.h"
#include <deque>
#include <list>
#include "OwnerTable.h"
#include "PointerTable.h"
#include "HashTable.h"
#include "HashTableItemBase.h"
#include "StringOf.h"
#include "IListIter.h"
#include "ListIter.h"
#include "NamedResourceTable.h"
#undef SP_DEFINE_TEMPLATES

#include <stddef.h>
#include "Attribute.h"
#include "Attributed.h"
#include "CharsetDecl.h"
#include "ContentToken.h"
#include "Dtd.h"
#include "ElementType.h"
#include "Entity.h"
#include "EntityCatalog.h"
#include "EntityManager.h"
#include "EquivClass.h"
#include "Event.h"
#include "Hash.h"
#include "HashTable.h"
#include "HashTableItemBase.h"
#include "InputSource.h"
#include "LinkProcess.h"
#include "Lpd.h"
#include "LpdEntityRef.h"
#include "Markup.h"
#include "NamedResource.h"
#include "NamedTable.h"
#include "Named.h"
#include "NameToken.h"
#include "OpenElement.h"
#include "OutputState.h"
#include "Recognizer.h"
#include "Sd.h"
#include "SdText.h"
#include "SdFormalError.h"
#include "SrInfo.h"
#include "StringResource.h"
#include "Syntax.h"
#include "Text.h"
#include "Trie.h"
#include "Undo.h"
#include "Boolean.h"
#include "types.h"
#include "Id.h"

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

__instantiate(Ptr<Entity>)
__instantiate(ConstPtr<Entity>)
__instantiate(Ptr<Notation>)
__instantiate(ConstPtr<Notation>)
__instantiate(Ptr<Recognizer>)
__instantiate(ConstPtr<Recognizer>)
__instantiate(Ptr<EntityOrigin>)
__instantiate(ConstPtr<EntityOrigin>)
__instantiate(Ptr<AttributeValue>)
__instantiate(ConstPtr<AttributeValue>)
__instantiate(Ptr<AttributeDefinitionList>)
__instantiate(ConstPtr<AttributeDefinitionList>)
__instantiate(Ptr<ElementDefinition>)
__instantiate(ConstPtr<ElementDefinition>)
__instantiate(Ptr<Dtd>)
__instantiate(ConstPtr<Dtd>)
__instantiate(Ptr<Sd>)
__instantiate(ConstPtr<Sd>)
__instantiate(Ptr<Syntax>)
__instantiate(ConstPtr<Syntax>)
__instantiate(Ptr<SourceLinkRuleResource>)
__instantiate(ConstPtr<SourceLinkRuleResource>)
__instantiate(Ptr<Lpd>)
__instantiate(ConstPtr<Lpd>)
__instantiate(Ptr<ComplexLpd>)
__instantiate(ConstPtr<ComplexLpd>)
__instantiate(CopyOwner<BlankTrie>)
__instantiate(CopyOwner<AttributeSemantics>)
__instantiate(CopyOwner<AttributeDefinition>)
__instantiate(CopyOwner<DeclaredValue>)
__instantiate(std::vector<Owner<ContentToken> >)
__instantiate(std::vector<CopyOwner<AttributeDefinition> >)
__instantiate(std::vector<Owner<AttributeList> >)
__instantiate(std::vector<AttributeList>)
__instantiate(`HashTableItem<StringC, int>')
__instantiate(`HashTable<StringC,int>')
__instantiate(`HashTableItem<StringC,Char>')
__instantiate(`HashTable<StringC,Char>')
__instantiate(std::list<EquivClass>)
__instantiate(std::list<InputSource>)
__instantiate(std::list<OpenElement>)
__instantiate(std::list<Event>)
__instantiate(std::list<OutputStateLevel>)
__instantiate(std::list<SdFormalError>)
__instantiate(std::list<Undo>)
__instantiate(IQueue<MessageEvent>)
__instantiate(IQueue<Event>)
__instantiate(`OwnerTable<Named,StringC,Hash,NamedTableKeyFunction>')
__instantiate(`OwnerTableIter<Named,StringC,Hash,NamedTableKeyFunction>')
__instantiate(Owner<AndInfo>)
__instantiate(Owner<BlankTrie>)
__instantiate(Owner<EquivClass>)
__instantiate(Owner<ModelGroup>)
__instantiate(Owner<OpenElement>)
__instantiate(Owner<ContentToken>)
__instantiate(Owner<DeclaredValue>)
__instantiate(Owner<LeafContentToken>)
__instantiate(Owner<AttributeSemantics>)
__instantiate(Owner<CompiledModelGroup>)
__instantiate(Owner<AttributeDefinition>)
__instantiate(Owner<AttributeList>)
__instantiate(Owner<Trie>)
__instantiate(Owner<Markup>)
__instantiate(`PointerTableIter<Named*,StringC,Hash,NamedTableKeyFunction>')
__instantiate(`PointerTable<Named*,StringC,Hash,NamedTableKeyFunction>')
__instantiate(`PointerTable<Ptr<NamedResource>,StringC,Hash,NamedResourceKeyFunction>')
__instantiate(`PointerTableIter<Ptr<NamedResource>,StringC,Hash,NamedResourceKeyFunction>')
__instantiate(std::vector<ConstPtr<Entity> >)
__instantiate(std::vector<NameToken>)
__instantiate(std::vector<CharsetDeclRange>)
__instantiate(std::vector<CharsetDeclSection>)
__instantiate(std::vector<Ptr<Dtd> >)
__instantiate(std::vector<Attribute>)
__instantiate(std::vector<TextItem>)
__instantiate(std::vector<SdTextItem>)
__instantiate(std::vector<MarkupItem>)
__instantiate(std::vector<RankStem*>)
__instantiate(std::vector<const RankStem*>)
__instantiate(std::vector<Attributed*>)
__instantiate(std::vector<ElementType*>)
__instantiate(std::vector<const ElementType*>)
__instantiate(std::vector<ConstPtr<Notation> >)
__instantiate(std::vector<Ptr<NamedResource> >)
__instantiate(std::vector<LastSet>)
__instantiate(std::vector<ConstPtr<AttributeValue> >)
__instantiate(std::vector<SrInfo>)
__instantiate(std::vector<Named*>)
__instantiate(std::vector<FirstSet>)
__instantiate(std::vector<const AttributeList *>)
__instantiate(std::vector<ConstPtr<ElementDefinition> >)
__instantiate(std::vector<Text>)
__instantiate(std::vector<Location>)
__instantiate(std::vector<NamedResourceTable<Entity> >)
__instantiate(std::vector<ContentModelAmbiguity>)
__instantiate(std::vector<Transition>)
__instantiate(std::vector<LeafContentToken*>)
__instantiate(std::vector<size_t>)
 // we really just want to test if size_t == unsigned int
#if !defined(SIZEOF_SIZE_T) || !defined(SIZEOF_UNSIGNED_INT) || (SIZEOF_SIZE_T != SIZEOF_UNSIGNED_INT)
__instantiate(std::vector<unsigned int>)
#endif

__instantiate(NamedTable<Id>)
__instantiate(NamedTableIter<Id>)
__instantiate(IListIter<EquivClass>)
__instantiate(IListIter<InputSource>)

__instantiate(NamedResourceTable<Notation>)
__instantiate(NamedResourceTable<Entity>)
__instantiate(NamedResourceTableIter<Entity>)
__instantiate(NamedResourceTableIter<Notation>)
__instantiate(ConstNamedResourceTableIter<Entity>)
__instantiate(ConstNamedResourceTableIter<Notation>)

__instantiate(std::vector<IdLinkRule>)
__instantiate(std::vector<ConstPtr<SourceLinkRuleResource> >)
__instantiate(std::vector<std::vector<ConstPtr<SourceLinkRuleResource> > >)
__instantiate(std::vector<ResultElementSpec>)
__instantiate(std::vector<ConstPtr<AttributeDefinitionList> >)
__instantiate(std::vector<ConstPtr<Lpd> >)
__instantiate(std::vector<LpdEntityRef *>)
__instantiate(`PointerTable<LpdEntityRef *, LpdEntityRef, LpdEntityRef, LpdEntityRef>')
__instantiate(`PointerTableIter<LpdEntityRef *, LpdEntityRef, LpdEntityRef, LpdEntityRef>')
__instantiate(`OwnerTable<LpdEntityRef, LpdEntityRef, LpdEntityRef, LpdEntityRef>')
__instantiate(`OwnerTableIter<LpdEntityRef, LpdEntityRef, LpdEntityRef, LpdEntityRef>')
__instantiate(std::list<LinkProcessOpenElement>)

#ifdef SP_NAMESPACE
}
#endif

#endif /* SP_MANUAL_INST */
