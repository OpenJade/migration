// This file was automatically generated from style/InterpreterMessages.msg by msggen.pl.
#ifndef InterpreterMessages_INCLUDED
#define InterpreterMessages_INCLUDED 1

#ifdef __GNUG__
#pragma interface
#endif
#include "Message.h"

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

struct InterpreterMessages {
  // 0
  static const MessageType0 unexpectedEof;
  // 1
  static const MessageType0 invalidChar;
  // 2
  static const MessageType0 unknownHash;
  // 3
  static const MessageType1 unknownNamedConstant;
  // 4
  static const MessageType1 unexpectedToken;
  // 5
  static const MessageType0 unterminatedString;
  // 6
  static const MessageType0 missingCloseParen;
  // 7
  static const MessageType1 invalidNumber;
  // 8
  static const MessageType1 invalidAfiiGlyphId;
  // 9
  static const MessageType1 callNonFunction;
  // 10
  static const MessageType0 tooManyArgs;
  // 11
  static const MessageType0 oddKeyArgs;
  // 12
  static const MessageType0 missingArg;
  // 13
  static const MessageType1 syntacticKeywordAsVariable;
  // 14
  static const MessageType1 undefinedVariableReference;
  // 15
  static const MessageType0 badCharName;
  // 16
  static const MessageType1 keywordNotAllowedCall;
  // 17
  static const MessageType1 symbolRequired;
  // 18
  static const MessageType1 unknownTopLevelForm;
  // 19
  static const MessageType1 badModeForm;
  // 20
  static const MessageType1L duplicateDefinition;
  // 22
  static const MessageType1 identifierLoop;
  // 23
  static const MessageType0 outOfRange;
  // 24
  static const MessageType1 unitLoop;
  // 25
  static const MessageType1 badUnitDefinition;
  // 26
  static const MessageType1L duplicateUnitDefinition;
  // 28
  static const MessageType1 undefinedQuantity;
  // 29
  static const MessageType0 incompatibleDimensions;
  // 30
  static const MessageType3 notABoolean;
  // 31
  static const MessageType3 notAPair;
  // 32
  static const MessageType3 notAList;
  // 33
  static const MessageType3 notASymbol;
  // 34
  static const MessageType3 notAString;
  // 35
  static const MessageType3 notAStringOrSymbol;
  // 36
  static const MessageType3 notASymbolOrString;
  // 37
  static const MessageType3 notAChar;
  // 38
  static const MessageType3 notAStyle;
  // 39
  static const MessageType3 notAnExactInteger;
  // 40
  static const MessageType3 notAQuantity;
  // 41
  static const MessageType3 notAColorSpace;
  // 42
  static const MessageType3 notANumber;
  // 43
  static const MessageType3 notASosofo;
  // 44
  static const MessageType3 notAnOptSingletonNode;
  // 45
  static const MessageType3 notASingletonNode;
  // 46
  static const MessageType3 notANodeList;
  // 47
  static const MessageType3 notANamedNodeList;
  // 48
  static const MessageType3 notALengthSpec;
  // 49
  static const MessageType3 notAQuantityOrLengthSpec;
  // 50
  static const MessageType3 notAPriority;
  // 51
  static const MessageType3 notAPattern;
  // 52
  static const MessageType3 notAnAddress;
  // 53
  static const MessageType3 notAGlyphId;
  // 54
  static const MessageType3 notAGlyphSubstTable;
  // 55
  static const MessageType3 notAGlyphIdPairList;
  // 56
  static const MessageType3 notAProcedure;
  // 57
  static const MessageType0L duplicateDefaultRule;
  // 59
  static const MessageType0L duplicateRootRule;
  // 61
  static const MessageType1L duplicateIdRule;
  // 63
  static const MessageType0L duplicateElementRule;
  // 65
  static const MessageType1L duplicateInitialValue;
  // 67
  static const MessageType1 invalidStyleKeyword;
  // 68
  static const MessageType2 invalidMakeKeyword;
  // 69
  static const MessageType1 unknownFlowObjectClass;
  // 70
  static const MessageType1 atomicContent;
  // 71
  static const MessageType0 labelNotASymbol;
  // 72
  static const MessageType1 badConnection;
  // 73
  static const MessageType0 badContentMap;
  // 74
  static const MessageType1 contentMapBadPort;
  // 75
  static const MessageType1 invalidCharacteristicValue;
  // 76
  static const MessageType0 condFail;
  // 77
  static const MessageType1 caseFail;
  // 78
  static const MessageType1 caseElse;
  // 79
  static const MessageType0 caseUnresolvedQuantities;
  // 80
  static const MessageType1 errorProc;
  // 81
  static const MessageType0 divideBy0;
  // 82
  static const MessageType1 invalidKeyArg;
  // 83
  static const MessageType0 keyArgsNotKey;
  // 84
  static const MessageType0 oneBody;
  // 85
  static const MessageType0 specNotArc;
  // 86
  static const MessageType0 noStyleSpec;
  // 87
  static const MessageType1 unknownCharName;
  // 88
  static const MessageType0 noCurrentNode;
  // 89
  static const MessageType0 noCurrentProcessingMode;
  // 90
  static const MessageType0 invalidRadix;
  // 91
  static const MessageType0 sosofoContext;
  // 92
  static const MessageType0 styleContext;
  // 93
  static const MessageType0 notInCharacteristicValue;
  // 94
  static const MessageType1 expressionNotImplemented;
  // 95
  static const MessageType0 RGBColorArgCount;
  // 96
  static const MessageType0 RGBColorArgType;
  // 97
  static const MessageType0 RGBColorArgRange;
  // 98
  static const MessageType1 unknownColorSpaceFamily;
  // 99
  static const MessageType0 deviceRGBColorSpaceNoArgs;
  // 100
  static const MessageType1 notABuiltinInheritedC;
  // 101
  static const MessageType1 invalidNumberFormat;
  // 102
  static const MessageType1 invalidPublicIdChar;
  // 103
  static const MessageType1 debug;
  // 104
  static const MessageType0 useLoop;
  // 105
  static const MessageType1 missingPart;
  // 106
  static const MessageType0 noParts;
  // 107
  static const MessageType0 tableCellOutsideTable;
  // 108
  static const MessageType0 tableRowOutsideTable;
  // 109
  static const MessageType1 noNodePropertyValue;
  // 110
  static const MessageType0 returnNotNodeList;
};

#ifdef SP_NAMESPACE
}
#endif

#endif /* not InterpreterMessages_INCLUDED */
