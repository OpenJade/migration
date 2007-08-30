// Copyright (c) 1996 James Clark, 1999 Matthias Clasen
// See the file COPYING for copying permission.

// Include all header files for systems that support pre-compiled headers.

#include "Allocator.h"
#include "ArcEngine.h"
#include "Attribute.h"
#include "Attributed.h"
#include "Boolean.h"
#include "CharMap.h"
#include "CharsetDecl.h"
#include "CharsetInfo.h"
#include "CmdLineApp.h"
#include "CodingSystem.h"
#include "CodingSystemKit.h"
#include "ConsoleOutput.h"
#include "ContentState.h"
#include "ContentToken.h"
#include "CopyOwner.h"
#include "DescriptorManager.h"
#include "Dtd.h"
#include "ElementType.h"
#include "Entity.h"
#include "EntityApp.h"
#include "EntityCatalog.h"
#include "EntityDecl.h"
#include "EntityManager.h"
#include "ErrnoMessageArg.h"
#include "ErrorCountEventHandler.h"
#include "Event.h"
#include "EventsWanted.h"
#include "ExtendEntityManager.h"
#include "ExternalId.h"
#include "GenericEventHandler.h"
#include "Hash.h"
#include "HashTable.h"
#include "HashTableItemBase.h"
#include <list>
#include "IListBase.h"
#include "IListIter.h"
#include "IListIterBase.h"
#include <deque>
#include "ISet.h"
#include "ISetIter.h"
#include "IdentityCodingSystem.h"
#include "InputSource.h"
#include "Link.h"
#include "LinkProcess.h"
#include <list>
#include "ListIter.h"
#include "LiteralStorage.h"
#include "Location.h"
#include "Lpd.h"
#include "Markup.h"
#include "Message.h"
#include "MessageArg.h"
#include "MessageBuilder.h"
#include "MessageEventHandler.h"
#include "MessageFormatter.h"
#include "MessageReporter.h"
#include "Mode.h"
#include "Named.h"
#include "NamedResource.h"
#include "NamedResourceTable.h"
#include "NamedTable.h"
#include "Notation.h"
#include "NotationStorage.h"
#include "OpenElement.h"
#include "Options.h"
#include "OutputByteStream.h"
#include "OutputCharStream.h"
#include "Owner.h"
#include "OwnerTable.h"
#include "ParserApp.h"
#include "ParserOptions.h"
#include "PointerTable.h"
#include "PosixStorage.h"
#include "Ptr.h"
#include "RangeMap.h"
#include "Resource.h"
#include "RewindStorageObject.h"
#include "SOEntityCatalog.h"
#include "Sd.h"
#include "SdText.h"
#include "SearchResultMessageArg.h"
#include "SgmlParser.h"
#include "ShortReferenceMap.h"
#include "StdioStorage.h"
#include "StorageManager.h"
#include "StringC.h"
#include "StringOf.h"
#include "StringResource.h"
#include "SubstTable.h"
#include "Syntax.h"
#include "Text.h"
#include "TypeId.h"
#include "URLStorage.h"
#include "UnivCharsetDesc.h"
#include <vector>
#include "XcharMap.h"
#include "config.h"
#include "constant.h"
#include "macros.h"
#include "rtti.h"
#include "sptchar.h"
#include "types.h"
#include "xnew.h"
#ifdef SP_MULTI_BYTE
#include "EUCJPCodingSystem.h"
#include "Fixed2CodingSystem.h"
#include "Fixed4CodingSystem.h"
#include "SJISCodingSystem.h"
#include "UnicodeCodingSystem.h"
#include "UTF8CodingSystem.h"
#include "UTF16CodingSystem.h"
#ifdef WIN32
#include "Win32CodingSystem.h"
#endif
#endif /* SP_MULTI_BYTE */
#include "EventGenerator.h"
#include "ParserEventGeneratorKit.h"
#include "SGMLApplication.h"
#include "ArcProcessor.h"
#include "CatalogEntry.h"
#include "CharsetRegistry.h"
#include "EquivClass.h"
#include "EventQueue.h"
#include "Group.h"
#include "Id.h"
#include "InternalInputSource.h"
#include "LpdEntityRef.h"
#include "MarkupScan.h"
#include "ModeInfo.h"
#include "NameToken.h"
#include "NumericCharRefOrigin.h"
#include "OutputState.h"
#include "Param.h"
#include "Parser.h"
#include "ParserMessages.h"
#include "ParserState.h"
#include "Partition.h"
#include "Priority.h"
#include "Recognizer.h"
#include "SdFormalError.h"
#include "SrInfo.h"
#include "StorageObjectPosition.h"
#include "TokenMessageArg.h"
#include "Trie.h"
#include "TrieBuilder.h"
#include "Undo.h"
#include "token.h"
