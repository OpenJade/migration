// This file was automatically generated from lib\ParserMessages.msg by msggen.pl.
#ifndef ParserMessages_INCLUDED
#define ParserMessages_INCLUDED 1

#ifdef __GNUG__
#pragma interface
#endif
#include "Message.h"

#ifdef SP_NAMESPACE
namespace SP_NAMESPACE {
#endif

struct ParserMessages {
  // 0
  static const MessageType1 nameLength;
  // 1
  static const MessageType1 parameterEntityNameLength;
  // 2
  static const MessageType1 numberLength;
  // 3
  static const MessageType1 attributeValueLength;
  // 4
  static const MessageType0 peroGrpoProlog;
  // 5
  static const MessageType0 groupLevel;
  // 6
  static const MessageType2 groupCharacter;
  // 7
  static const MessageType0 psRequired;
  // 8
  static const MessageType2 markupDeclarationCharacter;
  // 9
  static const MessageType0 declarationLevel;
  // 10
  static const MessageType0 groupEntityEnd;
  // 11
  static const MessageType1 invalidToken;
  // 12
  static const MessageType0 groupEntityReference;
  // 13
  static const MessageType1 duplicateGroupToken;
  // 14
  static const MessageType1 groupCount;
  // 15
  static const MessageType0 literalLevel;
  // 16
  static const MessageType1 literalMinimumData;
  // 17
  static const MessageType0 dataTagPatternNonSgml;
  // 18
  static const MessageType0 dataTagPatternFunction;
  // 19
  static const MessageType0 eroGrpoStartTag;
  // 20
  static const MessageType0 eroGrpoProlog;
  // 21
  static const MessageType1 functionName;
  // 22
  static const MessageType1 characterNumber;
  // 23
  static const MessageType1 parameterEntityUndefined;
  // 24
  static const MessageType1 entityUndefined;
  // 25
  static const MessageType0 rniNameStart;
  // 26
  static const MessageType0L commentEntityEnd;
  // 28
  static const MessageType0 mixedConnectors;
  // 29
  static const MessageType1 noSuchReservedName;
  // 30
  static const MessageType1 invalidReservedName;
  // 31
  static const MessageType1 minimumLiteralLength;
  // 32
  static const MessageType1 tokenizedAttributeValueLength;
  // 33
  static const MessageType1 systemIdentifierLength;
  // 34
  static const MessageType1 parameterLiteralLength;
  // 35
  static const MessageType1 dataTagPatternLiteralLength;
  // 36
  static const MessageType0 literalClosingDelimiter;
  // 37
  static const MessageType2 paramInvalidToken;
  // 38
  static const MessageType2 groupTokenInvalidToken;
  // 39
  static const MessageType2 connectorInvalidToken;
  // 40
  static const MessageType1 noSuchDeclarationType;
  // 41
  static const MessageType1 dtdSubsetDeclaration;
  // 42
  static const MessageType1 declSubsetCharacter;
  // 43
  static const MessageType0 documentEndDtdSubset;
  // 44
  static const MessageType1 prologCharacter;
  // 45
  static const MessageType0 documentEndProlog;
  // 46
  static const MessageType1 prologDeclaration;
  // 47
  static const MessageType1 rankStemGenericIdentifier;
  // 48
  static const MessageType0 missingTagMinimization;
  // 49
  static const MessageType1 duplicateElementDefinition;
  // 50
  static const MessageType0 entityApplicableDtd;
  // 51
  static const MessageType1L commentDeclInvalidToken;
  // 53
  static const MessageType1 instanceDeclaration;
  // 54
  static const MessageType0 contentNonSgml;
  // 55
  static const MessageType1 noCurrentRank;
  // 56
  static const MessageType1 duplicateAttlistNotation;
  // 57
  static const MessageType1 duplicateAttlistElement;
  // 58
  static const MessageType0 endTagEntityEnd;
  // 59
  static const MessageType1 endTagCharacter;
  // 60
  static const MessageType1 endTagInvalidToken;
  // 61
  static const MessageType0 pcdataNotAllowed;
  // 62
  static const MessageType1 elementNotAllowed;
  // 63
  static const MessageType2 missingElementMultiple;
  // 64
  static const MessageType2 missingElementInferred;
  // 65
  static const MessageType1 startTagEmptyElement;
  // 66
  static const MessageType1L omitEndTagDeclare;
  // 68
  static const MessageType1L omitEndTagOmittag;
  // 70
  static const MessageType1 omitStartTagDeclaredContent;
  // 71
  static const MessageType1 elementEndTagNotFinished;
  // 72
  static const MessageType1 omitStartTagDeclare;
  // 73
  static const MessageType1 taglvlOpenElements;
  // 74
  static const MessageType1 undefinedElement;
  // 75
  static const MessageType0 emptyEndTagNoOpenElements;
  // 76
  static const MessageType1 elementNotFinished;
  // 77
  static const MessageType1 elementNotOpen;
  // 78
  static const MessageType1 internalParameterDataEntity;
  // 79
  static const MessageType1 attributeSpecCharacter;
  // 80
  static const MessageType0 unquotedAttributeValue;
  // 81
  static const MessageType0 attributeSpecEntityEnd;
  // 82
  static const MessageType1 externalParameterDataSubdocEntity;
  // 83
  static const MessageType1 duplicateEntityDeclaration;
  // 84
  static const MessageType1 duplicateParameterEntityDeclaration;
  // 85
  static const MessageType0 piEntityReference;
  // 86
  static const MessageType0 internalDataEntityReference;
  // 87
  static const MessageType0 externalNonTextEntityReference;
  // 88
  static const MessageType0 externalNonTextEntityRcdata;
  // 89
  static const MessageType0 entlvl;
  // 90
  static const MessageType0 piEntityRcdata;
  // 91
  static const MessageType1 recursiveEntityReference;
  // 92
  static const MessageType1 undefinedShortrefMapInstance;
  // 93
  static const MessageType0 usemapAssociatedElementTypeDtd;
  // 94
  static const MessageType0 usemapAssociatedElementTypeInstance;
  // 95
  static const MessageType2 undefinedShortrefMapDtd;
  // 96
  static const MessageType1 unknownShortrefDelim;
  // 97
  static const MessageType1 delimDuplicateMap;
  // 98
  static const MessageType0 noDocumentElement;
  // 99
  static const MessageType0 processingInstructionEntityEnd;
  // 100
  static const MessageType1 processingInstructionLength;
  // 101
  static const MessageType0 processingInstructionClose;
  // 102
  static const MessageType0 attributeSpecNameTokenExpected;
  // 103
  static const MessageType1 noSuchAttributeToken;
  // 104
  static const MessageType0 attributeNameShorttag;
  // 105
  static const MessageType1 noSuchAttribute;
  // 106
  static const MessageType0 attributeValueExpected;
  // 107
  static const MessageType1 nameTokenLength;
  // 108
  static const MessageType0 attributeSpecLiteral;
  // 109
  static const MessageType1 duplicateAttributeSpec;
  // 110
  static const MessageType1 duplicateAttributeDef;
  // 111
  static const MessageType0 emptyDataAttributeSpec;
  // 112
  static const MessageType0 markedSectionEnd;
  // 113
  static const MessageType1 markedSectionLevel;
  // 114
  static const MessageType0L unclosedMarkedSection;
  // 116
  static const MessageType0 specialParseEntityEnd;
  // 117
  static const MessageType2 normalizedAttributeValueLength;
  // 118
  static const MessageType0 attributeValueSyntax;
  // 119
  static const MessageType2 attributeValueChar;
  // 120
  static const MessageType1 attributeValueMultiple;
  // 121
  static const MessageType2 attributeValueNumberToken;
  // 122
  static const MessageType2 attributeValueName;
  // 123
  static const MessageType1 attributeMissing;
  // 124
  static const MessageType1 requiredAttributeMissing;
  // 125
  static const MessageType1 currentAttributeMissing;
  // 126
  static const MessageType1 invalidNotationAttribute;
  // 127
  static const MessageType1 invalidEntityAttribute;
  // 128
  static const MessageType3 attributeValueNotInGroup;
  // 129
  static const MessageType1 notDataOrSubdocEntity;
  // 130
  static const MessageType3 ambiguousModelInitial;
  // 131
  static const MessageType5 ambiguousModel;
  // 132
  static const MessageType5 ambiguousModelSingleAnd;
  // 133
  static const MessageType6 ambiguousModelMultipleAnd;
  // 134
  static const MessageType1L commentDeclarationCharacter;
  // 136
  static const MessageType1 nonSgmlCharacter;
  // 137
  static const MessageType0 dataMarkedSectionDeclSubset;
  // 138
  static const MessageType1L duplicateId;
  // 140
  static const MessageType1 notFixedValue;
  // 141
  static const MessageType1 sdCommentSignificant;
  // 142
  static const MessageType1 standardVersion;
  // 143
  static const MessageType1 namingBeforeLcnmstrt;
  // 144
  static const MessageType1 sdEntityEnd;
  // 145
  static const MessageType2 sdInvalidNameToken;
  // 146
  static const MessageType1 numberTooBig;
  // 147
  static const MessageType1 sdLiteralSignificant;
  // 148
  static const MessageType1 syntaxCharacterNumber;
  // 149
  static const MessageType0 sdParameterEntity;
  // 150
  static const MessageType2 sdParamInvalidToken;
  // 151
  static const MessageType0 giveUp;
  // 152
  static const MessageType1 sdMissingCharacters;
  // 153
  static const MessageType1 missingMinimumChars;
  // 154
  static const MessageType1 duplicateCharNumbers;
  // 155
  static const MessageType1 codeSetHoles;
  // 156
  static const MessageType1 basesetCharsMissing;
  // 157
  static const MessageType1 documentCharMax;
  // 158
  static const MessageType1 fpiMissingField;
  // 159
  static const MessageType1 fpiMissingTextClassSpace;
  // 160
  static const MessageType1 fpiInvalidTextClass;
  // 161
  static const MessageType1 fpiInvalidLanguage;
  // 162
  static const MessageType1 fpiIllegalDisplayVersion;
  // 163
  static const MessageType1 fpiExtraField;
  // 164
  static const MessageType0 notationIdentifierTextClass;
  // 165
  static const MessageType1 unknownBaseset;
  // 166
  static const MessageType2 lexicalAmbiguity;
  // 167
  static const MessageType1 translateSyntaxChar;
  // 168
  static const MessageType1 missingSignificant;
  // 169
  static const MessageType1 missingSyntaxChar;
  // 170
  static const MessageType1 unknownCapacitySet;
  // 171
  static const MessageType1 duplicateCapacity;
  // 172
  static const MessageType1 capacityExceedsTotalcap;
  // 173
  static const MessageType1 unknownPublicSyntax;
  // 174
  static const MessageType0 nmstrtLength;
  // 175
  static const MessageType0 nmcharLength;
  // 176
  static const MessageType1 subdocLevel;
  // 177
  static const MessageType1 subdocEntity;
  // 178
  static const MessageType0 parameterEntityNotEnded;
  // 179
  static const MessageType1 missingId;
  // 180
  static const MessageType1 dtdUndefinedElement;
  // 181
  static const MessageType1 elementNotFinishedDocumentEnd;
  // 182
  static const MessageType0 subdocGiveUp;
  // 183
  static const MessageType0 noDtd;
  // 184
  static const MessageType0 noDtdSubset;
  // 185
  static const MessageType0 notSgml;
  // 186
  static const MessageType1 taglen;
  // 187
  static const MessageType0 groupParameterEntityNotEnded;
  // 188
  static const MessageType1 invalidSgmlChar;
  // 189
  static const MessageType1 translateDocChar;
  // 190
  static const MessageType1 attributeValueLengthNeg;
  // 191
  static const MessageType1 tokenizedAttributeValueLengthNeg;
  // 192
  static const MessageType1 scopeInstanceQuantity;
  // 193
  static const MessageType1 basesetTextClass;
  // 194
  static const MessageType1 capacityTextClass;
  // 195
  static const MessageType1 syntaxTextClass;
  // 196
  static const MessageType0 msocharRequiresMsichar;
  // 197
  static const MessageType1 switchNotMarkup;
  // 198
  static const MessageType1 switchNotInCharset;
  // 199
  static const MessageType1 ambiguousDocCharacter;
  // 200
  static const MessageType1 oneFunction;
  // 201
  static const MessageType1 duplicateFunctionName;
  // 202
  static const MessageType1 missingSignificant646;
  // 203
  static const MessageType1 generalDelimAllFunction;
  // 204
  static const MessageType1 nmcharLetter;
  // 205
  static const MessageType1 nmcharDigit;
  // 206
  static const MessageType1 nmcharRe;
  // 207
  static const MessageType1 nmcharRs;
  // 208
  static const MessageType1 nmcharSpace;
  // 209
  static const MessageType1 nmcharSepchar;
  // 210
  static const MessageType1 switchLetterDigit;
  // 211
  static const MessageType0 zeroNumberOfCharacters;
  // 212
  static const MessageType1 nameReferenceReservedName;
  // 213
  static const MessageType1 ambiguousReservedName;
  // 214
  static const MessageType1 duplicateReservedName;
  // 215
  static const MessageType1 reservedNameSyntax;
  // 216
  static const MessageType1 multipleBSequence;
  // 217
  static const MessageType1 blankAdjacentBSequence;
  // 218
  static const MessageType2 delimiterLength;
  // 219
  static const MessageType2 reservedNameLength;
  // 220
  static const MessageType1 nmcharNmstrt;
  // 221
  static const MessageType0 scopeInstanceSyntaxCharset;
  // 222
  static const MessageType0 emptyOmitEndTag;
  // 223
  static const MessageType1 conrefOmitEndTag;
  // 224
  static const MessageType1 conrefEmpty;
  // 225
  static const MessageType1 notationEmpty;
  // 226
  static const MessageType0 dataAttributeDeclaredValue;
  // 227
  static const MessageType0 dataAttributeDefaultValue;
  // 228
  static const MessageType2 attcnt;
  // 229
  static const MessageType0 idDeclaredValue;
  // 230
  static const MessageType1 multipleIdAttributes;
  // 231
  static const MessageType1 multipleNotationAttributes;
  // 232
  static const MessageType1 duplicateAttributeToken;
  // 233
  static const MessageType1 notationNoAttributes;
  // 234
  static const MessageType2 entityNotationUndefined;
  // 235
  static const MessageType2 mapEntityUndefined;
  // 236
  static const MessageType1 attlistNotationUndefined;
  // 237
  static const MessageType1 bracketedLitlen;
  // 238
  static const MessageType1 genericIdentifierLength;
  // 239
  static const MessageType0 instanceStartOmittag;
  // 240
  static const MessageType1 grplvl;
  // 241
  static const MessageType1 grpgtcnt;
  // 242
  static const MessageType0 minimizedStartTag;
  // 243
  static const MessageType0 minimizedEndTag;
  // 244
  static const MessageType0 multipleDtds;
  // 245
  static const MessageType0 afterDocumentElementEntityEnd;
  // 246
  static const MessageType1 declarationAfterDocumentElement;
  // 247
  static const MessageType0 characterReferenceAfterDocumentElement;
  // 248
  static const MessageType0 entityReferenceAfterDocumentElement;
  // 249
  static const MessageType0 markedSectionAfterDocumentElement;
  // 250
  static const MessageType3 requiredElementExcluded;
  // 251
  static const MessageType3 invalidExclusion;
  // 252
  static const MessageType0 attributeValueShorttag;
  // 253
  static const MessageType0 conrefNotation;
  // 254
  static const MessageType1 duplicateNotationDeclaration;
  // 255
  static const MessageType1L duplicateShortrefDeclaration;
  // 257
  static const MessageType1 duplicateDelimGeneral;
  // 258
  static const MessageType1 idrefGrpcnt;
  // 259
  static const MessageType1 entityNameGrpcnt;
  // 260
  static const MessageType2 attsplen;
  // 261
  static const MessageType1 duplicateDelimShortref;
  // 262
  static const MessageType1 duplicateDelimShortrefSet;
  // 263
  static const MessageType1 defaultEntityInAttribute;
  // 264
  static const MessageType1 defaultEntityReference;
  // 265
  static const MessageType2 mapDefaultEntity;
  // 266
  static const MessageType1 noSuchDtd;
  // 267
  static const MessageType1 noLpdSubset;
  // 268
  static const MessageType0 assocElementDifferentAtts;
  // 269
  static const MessageType1 duplicateLinkSet;
  // 270
  static const MessageType0 emptyResultAttributeSpec;
  // 271
  static const MessageType1 noSuchSourceElement;
  // 272
  static const MessageType1 noSuchResultElement;
  // 273
  static const MessageType0 documentEndLpdSubset;
  // 274
  static const MessageType1 lpdSubsetDeclaration;
  // 275
  static const MessageType0 idlinkDeclSimple;
  // 276
  static const MessageType0 linkDeclSimple;
  // 277
  static const MessageType1 simpleLinkAttlistElement;
  // 278
  static const MessageType0 shortrefOnlyInBaseDtd;
  // 279
  static const MessageType0 usemapOnlyInBaseDtd;
  // 280
  static const MessageType0 linkAttributeDefaultValue;
  // 281
  static const MessageType0 linkAttributeDeclaredValue;
  // 282
  static const MessageType0 simpleLinkFixedAttribute;
  // 283
  static const MessageType0 duplicateIdLinkSet;
  // 284
  static const MessageType1 noInitialLinkSet;
  // 285
  static const MessageType1 notationUndefinedSourceDtd;
  // 286
  static const MessageType0 simpleLinkResultNotImplied;
  // 287
  static const MessageType0 simpleLinkFeature;
  // 288
  static const MessageType0 implicitLinkFeature;
  // 289
  static const MessageType0 explicitLinkFeature;
  // 290
  static const MessageType0 lpdBeforeBaseDtd;
  // 291
  static const MessageType0 dtdAfterLpd;
  // 292
  static const MessageType1 unstableLpdGeneralEntity;
  // 293
  static const MessageType1 unstableLpdParameterEntity;
  // 294
  static const MessageType1 multipleIdLinkRuleAttribute;
  // 295
  static const MessageType1 multipleLinkRuleAttribute;
  // 296
  static const MessageType2 uselinkBadLinkSet;
  // 297
  static const MessageType1 uselinkSimpleLpd;
  // 298
  static const MessageType1 uselinkBadLinkType;
  // 299
  static const MessageType1 duplicateDtdLpd;
  // 300
  static const MessageType1 duplicateLpd;
  // 301
  static const MessageType1 duplicateDtd;
  // 302
  static const MessageType1 undefinedLinkSet;
  // 303
  static const MessageType1 duplicateImpliedResult;
  // 304
  static const MessageType1 simpleLinkCount;
  // 305
  static const MessageType0 duplicateExplicitChain;
  // 306
  static const MessageType1 explicit1RequiresSourceTypeBase;
  // 307
  static const MessageType0 oneImplicitLink;
  // 308
  static const MessageType1 sorryLink;
  // 309
  static const MessageType0 entityReferenceMissingName;
  // 310
  static const MessageType1 explicitNoRequiresSourceTypeBase;
  // 311
  static const MessageType0 linkActivateTooLate;
  // 312
  static const MessageType0 pass2Ee;
  // 313
  static const MessageType2 idlinkElementType;
  // 314
  static const MessageType0 datatagNotImplemented;
  // 315
  static const MessageType0 startTagMissingName;
  // 316
  static const MessageType0 endTagMissingName;
  // 317
  static const MessageType0 startTagGroupNet;
  // 318
  static const MessageType0 documentElementUndefined;
  // 319
  static const MessageType0 badDefaultSgmlDecl;
  // 320
  static const MessageType1L nonExistentEntityRef;
  // 322
  static const MessageType0 pcdataUnreachable;
  // 323
  static const MessageType0 sdRangeNotSingleChar;
  // 324
  static const MessageType0 sdInvalidRange;
  // 325
  static const MessageType0 sdEmptyDelimiter;
  // 326
  static const MessageType0 tooManyCharsMinimumLiteral;
  // 327
  static const MessageType1 defaultedEntityDefined;
  // 328
  static const MessageType0 unclosedStartTag;
  // 329
  static const MessageType0 unclosedEndTag;
  // 330
  static const MessageType0 emptyStartTag;
  // 331
  static const MessageType0 emptyEndTag;
  // 332
  static const MessageType0 netStartTag;
  // 333
  static const MessageType0 nullEndTag;
  // 334
  static const MessageType1 unusedMap;
  // 335
  static const MessageType1 unusedParamEntity;
  // 336
  static const MessageType1 cannotGenerateSystemIdPublic;
  // 337
  static const MessageType1 cannotGenerateSystemIdGeneral;
  // 338
  static const MessageType1 cannotGenerateSystemIdParameter;
  // 339
  static const MessageType1 cannotGenerateSystemIdDoctype;
  // 340
  static const MessageType1 cannotGenerateSystemIdLinktype;
  // 341
  static const MessageType1 cannotGenerateSystemIdNotation;
  // 342
  static const MessageType1 excludeIncludeSame;
  // 343
  static const MessageType1 implyingDtd;
  // 344
  static const MessageType1 afdrVersion;
  // 345
  static const MessageType0 missingAfdrDecl;
  // 346
  static const MessageType0 enrRequired;
  // 347
  static const MessageType1 numericCharRefLiteralNonSgml;
  // 348
  static const MessageType2 numericCharRefUnknownDesc;
  // 349
  static const MessageType3 numericCharRefUnknownBase;
  // 350
  static const MessageType1 numericCharRefBadInternal;
  // 351
  static const MessageType1 numericCharRefNoInternal;
  // 352
  static const MessageType0 netStartTagEndTag;
  // 353
  static const MessageType0 cdataContent;
  // 354
  static const MessageType0 rcdataContent;
  // 355
  static const MessageType0 inclusion;
  // 356
  static const MessageType0 exclusion;
  // 357
  static const MessageType0 numberDeclaredValue;
  // 358
  static const MessageType0 nameDeclaredValue;
  // 359
  static const MessageType0 nutokenDeclaredValue;
  // 360
  static const MessageType0 conrefAttribute;
  // 361
  static const MessageType0 currentAttribute;
  // 362
  static const MessageType0 tempMarkedSection;
  // 363
  static const MessageType0 instanceIncludeMarkedSection;
  // 364
  static const MessageType0 instanceIgnoreMarkedSection;
  // 365
  static const MessageType0 rcdataMarkedSection;
  // 366
  static const MessageType0 piEntity;
  // 367
  static const MessageType0 bracketEntity;
  // 368
  static const MessageType0 internalCdataEntity;
  // 369
  static const MessageType0 internalSdataEntity;
  // 370
  static const MessageType0 externalCdataEntity;
  // 371
  static const MessageType0 externalSdataEntity;
  // 372
  static const MessageType0 dataAttributes;
  // 373
  static const MessageType0 rank;
  // 374
  static const MessageType0 missingSystemId;
  // 375
  static const MessageType0 psComment;
  // 376
  static const MessageType0 namedCharRef;
  // 377
  static const MessageType0 andGroup;
  // 378
  static const MessageType0 attributeValueNotLiteral;
  // 379
  static const MessageType0 missingAttributeName;
  // 380
  static const MessageType0 elementGroupDecl;
  // 381
  static const MessageType0 attlistGroupDecl;
  // 382
  static const MessageType0 emptyCommentDecl;
  // 383
  static const MessageType0 commentDeclS;
  // 384
  static const MessageType0 commentDeclMultiple;
  // 385
  static const MessageType0 missingStatusKeyword;
  // 386
  static const MessageType0 multipleStatusKeyword;
  // 387
  static const MessageType0 instanceParamEntityRef;
  // 388
  static const MessageType0 current;
  // 389
  static const MessageType0 minimizationParam;
  // 390
  static const MessageType0 refc;
  // 391
  static const MessageType0 pcdataNotFirstInGroup;
  // 392
  static const MessageType0 pcdataInSeqGroup;
  // 393
  static const MessageType0 pcdataInNestedModelGroup;
  // 394
  static const MessageType0 pcdataGroupNotRep;
  // 395
  static const MessageType0 nameGroupNotOr;
  // 396
  static const MessageType0 piMissingName;
  // 397
  static const MessageType0 statusKeywordSpecS;
  // 398
  static const MessageType0 externalDataEntityRef;
  // 399
  static const MessageType0 attributeValueExternalEntityRef;
  // 1000
  static const MessageFragment delimStart;
  // 1001
  static const MessageFragment delimEnd;
  // 1002
  static const MessageFragment digit;
  // 1003
  static const MessageFragment nameStartCharacter;
  // 1004
  static const MessageFragment sepchar;
  // 1005
  static const MessageFragment separator;
  // 1006
  static const MessageFragment nameCharacter;
  // 1007
  static const MessageFragment dataCharacter;
  // 1008
  static const MessageFragment minimumDataCharacter;
  // 1009
  static const MessageFragment significantCharacter;
  // 1010
  static const MessageFragment recordEnd;
  // 1011
  static const MessageFragment recordStart;
  // 1012
  static const MessageFragment space;
  // 1013
  static const MessageFragment listSep;
  // 1014
  static const MessageFragment rangeSep;
  // 1015
  static const MessageFragment parameterLiteral;
  // 1016
  static const MessageFragment dataTagGroup;
  // 1017
  static const MessageFragment modelGroup;
  // 1018
  static const MessageFragment dataTagTemplateGroup;
  // 1019
  static const MessageFragment name;
  // 1020
  static const MessageFragment nameToken;
  // 1021
  static const MessageFragment elementToken;
  // 1022
  static const MessageFragment inclusions;
  // 1023
  static const MessageFragment exclusions;
  // 1024
  static const MessageFragment minimumLiteral;
  // 1025
  static const MessageFragment attributeValueLiteral;
  // 1026
  static const MessageFragment systemIdentifier;
  // 1027
  static const MessageFragment number;
  // 1028
  static const MessageFragment attributeValue;
  // 1029
  static const MessageFragment capacityName;
  // 1030
  static const MessageFragment generalDelimiteRoleName;
  // 1031
  static const MessageFragment referenceReservedName;
  // 1032
  static const MessageFragment quantityName;
  // 1033
  static const MessageFragment entityEnd;
  // 1034
  static const MessageFragment shortrefDelim;
};

#ifdef SP_NAMESPACE
}
#endif

#endif /* not ParserMessages_INCLUDED */
