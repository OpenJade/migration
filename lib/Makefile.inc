# -*- Makefile -*- Time-stamp: <07/08/27 21:12:00 ptr>

LIBNAME = osp
MAJOR = 1
MINOR = 5
PATCH = 2
SRC_CXX = Allocator.cxx ArcEngine.cxx \
	Attribute.cxx \
	Big5CodingSystem.cxx \
	CharsetDecl.cxx CharsetInfo.cxx CharsetRegistry.cxx CmdLineApp.cxx \
	CodingSystem.cxx CodingSystemKit.cxx \
	ConsoleOutput.cxx ContentState.cxx ContentToken.cxx \
	DescriptorManager.cxx Dtd.cxx DtdDeclEventHandler.cxx \
	EUCJPCodingSystem.cxx ElementType.cxx \
	Entity.cxx EntityApp.cxx EntityCatalog.cxx EntityDecl.cxx \
	EntityManager.cxx \
	ErrnoMessageArg.cxx ErrorCountEventHandler.cxx Event.cxx \
	EventGenerator.cxx ExtendEntityManager.cxx \
	ExternalId.cxx Fixed2CodingSystem.cxx GenericEventHandler.cxx \
	Group.cxx Hash.cxx IListBase.cxx Id.cxx \
	IdentityCodingSystem.cxx InputSource.cxx InternalInputSource.cxx \
	Link.cxx LinkProcess.cxx LiteralStorage.cxx Location.cxx Lpd.cxx \
	Markup.cxx Message.cxx MessageArg.cxx \
	MessageEventHandler.cxx MessageFormatter.cxx \
	MessageReporter.cxx \
	MessageTable.cxx ModeInfo.cxx \
	Notation.cxx NotationStorage.cxx \
	NumericCharRefOrigin.cxx OffsetOrderedList.cxx \
	OpenElement.cxx OutputByteStream.cxx \
	OutputCharStream.cxx OutputState.cxx Param.cxx \
	Parser.cxx ParserApp.cxx \
	ParserEventGeneratorKit.cxx ParserMessages.cxx \
	ParserOptions.cxx ParserState.cxx Partition.cxx \
	PosixStorage.cxx \
	Recognizer.cxx RewindStorageObject.cxx \
	SGMLApplication.cxx SJISCodingSystem.cxx SOEntityCatalog.cxx Sd.cxx \
	SdText.cxx SearchResultMessageArg.cxx SgmlParser.cxx \
	ShortReferenceMap.cxx StdioStorage.cxx \
	StorageManager.cxx Syntax.cxx \
	Text.cxx TokenMessageArg.cxx \
	TranslateCodingSystem.cxx TrieBuilder.cxx \
	TypeId.cxx URLStorage.cxx \
	UTF8CodingSystem.cxx Undo.cxx UnicodeCodingSystem.cxx \
	UnivCharsetDesc.cxx XMLCodingSystem.cxx \
	assert.cxx \
	parseAttribute.cxx parseCommon.cxx parseDecl.cxx parseInstance.cxx \
	parseMode.cxx parseParam.cxx parseSd.cxx splib.cxx \
	app_inst.cxx arc_inst.cxx entmgr_inst.cxx \
	parser_inst.cxx xentmgr_inst.cxx SubstTable.cxx \
	UTF16CodingSystem.cxx Fixed4CodingSystem.cxx \
        app_inst.cxx arc_inst.cxx entmgr_inst.cxx parser_inst.cxx xentmgr_inst.cxx

SRC_C =	memcmp.c memmove.c strerror.c
