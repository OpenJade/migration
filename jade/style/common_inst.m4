// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

// Copyright (c) 1996 James Clark
// See the file COPYING for copying permission.

#include "stylelib.h"

#ifdef SP_MANUAL_INST

#define SP_DEFINE_TEMPLATES
#include <OpenSP/StringOf.h>
#include <OpenSP/Vector.h>
#include <OpenSP/XcharMap.h>
#include <OpenSP/CharMap.h>
#include <OpenSP/Ptr.h>
#include <OpenSP/PointerTable.h>
#include <OpenSP/OwnerTable.h>
#include <OpenSP/HashTable.h>
#include <OpenSP/HashTableItemBase.h>
#include <OpenSP/SubstTable.h>
#include <OpenSP/ISet.h>
#include <OpenSP/Owner.h>
#undef SP_DEFINE_TEMPLATES

#include <OpenSP/types.h>
#include <OpenSP/Entity.h>
#include <OpenSP/Syntax.h>
#include <OpenSP/ExtendEntityManager.h>
#include <OpenSP/Event.h>
#include <OpenSP/CmdLineApp.h>

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

__instantiate(Vector<const CmdLineApp::AppChar *>)
__instantiate(Ptr<CodingSystemKit>)
__instantiate(Ptr<Sd>)
__instantiate(Ptr<InputSourceOrigin>)
__instantiate(Vector<ParsedSystemId::Map>)
__instantiate(Vector<size_t>)
__instantiate(String<Char>)
__instantiate(Vector<String<Char> >)
__instantiate(Ptr<StringResource<Char> >)
__instantiate(String<char>)
__instantiate(`OwnerTable<Named,StringC,Hash,NamedTableKeyFunction>')
__instantiate(`OwnerTableIter<Named,StringC,Hash,NamedTableKeyFunction>')
__instantiate(XcharMap<PackedBoolean>)
__instantiate(SharedXcharMap<PackedBoolean>)
__instantiate(Ptr<SharedXcharMap<PackedBoolean> >)
__instantiate(ConstPtr<SharedXcharMap<PackedBoolean> >)
__instantiate(`PointerTableIter<Named*,StringC,Hash,NamedTableKeyFunction>')
__instantiate(`PointerTable<Named*,StringC,Hash,NamedTableKeyFunction>')
__instantiate(HashTableItemBase<String<Char> >)
__instantiate(`OwnerTable<HashTableItemBase<String<Char> >,String<Char>,Hash,HashTableKeyFunction<String<Char> > >')
__instantiate(`CopyOwnerTable<HashTableItemBase<String<Char> >,String<Char>,Hash,HashTableKeyFunction<String<Char> > >')
__instantiate(`OwnerTableIter<HashTableItemBase<String<Char> >, String<Char>, Hash, HashTableKeyFunction<String<Char> > >')
__instantiate(`PointerTable<HashTableItemBase<String<Char> >*,String<Char>,Hash,HashTableKeyFunction<String<Char> > >')
__instantiate(`PointerTableIter<HashTableItemBase<String<Char> > *, String<Char>, Hash, HashTableKeyFunction<String<Char> > >')
__instantiate(Vector<HashTableItemBase<String<Char> >*>)
__instantiate(Ptr<Origin>)
__instantiate(`HashTable<StringC,Char>')
__instantiate(ConstPtr<Entity>)
__instantiate(Ptr<Entity>)
__instantiate(SubstTable<Char>)
__instantiate(Ptr<Syntax>)
__instantiate(ConstPtr<Syntax>)
__instantiate(ISet<Char>)
__instantiate(Vector<TextItem>)
__instantiate(`HashTableItem<StringC, int>')
__instantiate(`HashTable<StringC,int>')
__instantiate(Vector<PackedBoolean>)
__instantiate(Owner<EventHandler>)
__instantiate(Ptr<EntityManager>)
__instantiate(Ptr<ExtendEntityManager>)
__instantiate(Vector<StorageObjectSpec>)
__instantiate(Ptr<EntityCatalog>)
__instantiate(ConstPtr<EntityCatalog>)
__instantiate(CharMap<Char>)
__instantiate(CharMapPage<Char>)
#ifdef SP_NAMESPACE
}
#endif

#endif /* SP_MANUAL_INST */
