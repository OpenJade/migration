// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

PRIMITIVE(Cons, "cons", 2, 0, 0, expression)
PRIMITIVE(List, "list", 0, 0, 1, noFeature)
PRIMITIVE(IsNull, "null?", 1, 0, 0, noFeature)
PRIMITIVE(IsList, "list?", 1, 0, 0, noFeature)
PRIMITIVE(IsPair, "pair?", 1, 0, 0, expression)
PRIMITIVE(IsEqual, "equal?", 2, 0, 0, noFeature)
PRIMITIVE(Car, "car", 1, 0, 0, expression)
PRIMITIVE(Cdr, "cdr", 1, 0, 0, expression)
PRIMITIVE(Length, "length", 1, 0, 0, noFeature)
PRIMITIVE(Append, "append", 0, 0, 1, noFeature)
PRIMITIVE(Reverse, "reverse", 1, 0, 0, noFeature)
PRIMITIVE(ListTail, "list-tail", 2, 0, 0, noFeature)
PRIMITIVE(ListRef, "list-ref", 2, 0, 0, noFeature)
PRIMITIVE(Member, "member", 2, 0, 0, noFeature)
PRIMITIVE(Not, "not", 1, 0, 0, noFeature)
PRIMITIVE(IsSymbol, "symbol?", 1, 0, 0, noFeature)
PRIMITIVE(IsKeyword, "keyword?", 1, 0, 0, noFeature)
PRIMITIVE(IsBoolean, "boolean?", 1, 0, 0, noFeature)
PRIMITIVE(IsProcedure, "procedure?", 1, 0, 0, noFeature)
PRIMITIVE(IsString, "string?", 1, 0, 0, noFeature)
PRIMITIVE(IsInteger, "integer?", 1, 0, 0, noFeature)
PRIMITIVE(IsReal, "real?", 1, 0, 0, noFeature)
PRIMITIVE(IsNumber, "number?", 1, 0, 0, noFeature)
PRIMITIVE(IsQuantity, "quantity?", 1, 0, 0, noFeature)
PRIMITIVE(IsChar, "char?", 1, 0, 0, noFeature)
PRIMITIVE(IsCharEqual, "char=?", 2, 0, 0, noFeature)
PRIMITIVE(String, "string", 0, 0, 1, noFeature)
PRIMITIVE(SymbolToString, "symbol->string", 1, 0, 0, expression)
PRIMITIVE(StringToSymbol, "string->symbol", 1, 0, 0, expression)
PRIMITIVE(StringLength, "string-length", 1, 0, 0, noFeature)
PRIMITIVE(IsStringEqual, "string=?", 2, 0, 0, noFeature)
PRIMITIVE(StringAppend, "string-append", 0, 0, 1, noFeature)
PRIMITIVE(StringRef, "string-ref", 2, 0, 0, noFeature)
PRIMITIVE(Substring, "substring", 3, 0, 0, noFeature)
PRIMITIVE(Equal, "=", 0, 0, 1, noFeature)
PRIMITIVE(Plus, "+", 0, 0, 1, noFeature)
PRIMITIVE(Minus, "-", 1, 0, 1, noFeature)
PRIMITIVE(Multiply, "*", 0, 0, 1, noFeature)
PRIMITIVE(Divide, "/", 1, 0, 1, noFeature)
PRIMITIVE(Quotient, "quotient", 2, 0, 0, noFeature)
PRIMITIVE(Remainder, "remainder", 2, 0, 0, noFeature)
PRIMITIVE(Modulo, "modulo", 2, 0, 0, noFeature)
PRIMITIVE(Less, "<", 0, 0, 1, noFeature)
PRIMITIVE(Greater, ">", 0, 0, 1, noFeature)
PRIMITIVE(LessEqual, "<=", 0, 0, 1, noFeature)
PRIMITIVE(GreaterEqual, ">=", 0, 0, 1, noFeature)
PRIMITIVE(Min, "min", 1, 0, 1, noFeature)
PRIMITIVE(Max, "max", 1, 0, 1, noFeature)
PRIMITIVE(Floor, "floor", 1, 0, 0, noFeature)
PRIMITIVE(Ceiling, "ceiling", 1, 0, 0, noFeature)
PRIMITIVE(Truncate, "truncate", 1, 0, 0, noFeature)
PRIMITIVE(Round, "round", 1, 0, 0, noFeature)
PRIMITIVE(Abs, "abs", 1, 0, 0, noFeature)
PRIMITIVE(Sqrt, "sqrt", 1, 0, 0, noFeature)
PRIMITIVE(Time, "time", 0, 0, 0, noFeature)
PRIMITIVE(TimeToString, "time->string", 1, 1, 0, noFeature)
PRIMITIVE(CharProperty, "char-property", 2, 1, 0, noFeature)
SPRIMITIVE(Literal, "literal", 0, 0, 1, noFeature)
SPRIMITIVE(ProcessChildren, "process-children", 0, 0, 0, noFeature)
SPRIMITIVE(ProcessChildrenTrim, "process-children-trim", 0, 0, 0, noFeature)
SPRIMITIVE(SosofoAppend, "sosofo-append", 0, 0, 1, noFeature)
SPRIMITIVE(NextMatch, "next-match", 0, 1, 0, noFeature)
SPRIMITIVE(EmptySosofo, "empty-sosofo", 0, 0, 0, noFeature)
SPRIMITIVE(SosofoLabel, "sosofo-label", 2, 0, 0, noFeature)
SPRIMITIVE(SosofoDiscardLabeled, "sosofo-discard-labeled", 2, 0, 0, noFeature)
SPRIMITIVE(IsSosofo, "sosofo?", 1, 0, 0, noFeature)
SPRIMITIVE(IsStyle, "style?", 1, 0, 0, noFeature)
SPRIMITIVE(MergeStyle, "merge-style", 0, 0, 1, noFeature)
SPRIMITIVE(ProcessElementWithId, "process-element-with-id", 1, 0, 0, crossReference)
SPRIMITIVE(ProcessFirstDescendant, "process-first-descendant", 0, 0, 1, noFeature)
SPRIMITIVE(ProcessMatchingChildren, "process-matching-children", 0, 0, 1, noFeature)
SPRIMITIVE(ProcessNodeList, "process-node-list", 1, 0, 0, query)
SPRIMITIVE(CurrentNodePageNumberSosofo, "current-node-page-number-sosofo",
	  0, 0, 0, simplePage)
SPRIMITIVE(PageNumberSosofo, "page-number-sosofo", 0, 0, 0, simplePage)
PRIMITIVE(FormatNumber, "format-number", 2, 0, 0, noFeature)
PRIMITIVE(FormatNumberList, "format-number-list", 3, 0, 0, noFeature)
PRIMITIVE(Error, "error", 1, 0, 0, noFeature)
PRIMITIVE(ExternalProcedure, "external-procedure", 1, 0, 0, noFeature)
PRIMITIVE(StringToNumber, "string->number", 1, 1, 0, noFeature)
PRIMITIVE(NumberToString, "number->string", 1, 1, 0, noFeature)
SPRIMITIVE(DisplaySize, "display-size", 0, 0, 0, noFeature)
SPRIMITIVE(TableUnit, "table-unit", 1, 0, 0, table)
SPRIMITIVE(IsDisplaySpace, "display-space?", 1, 0, 0, noFeature)
SPRIMITIVE(DisplaySpace, "display-space", 1, 0, 1, noFeature)
SPRIMITIVE(IsInlineSpace, "inline-space?", 1, 0, 0, noFeature)
SPRIMITIVE(InlineSpace, "inline-space", 1, 0, 1, noFeature)
SPRIMITIVE(IsColor, "color?", 1, 0, 0, noFeature)
SPRIMITIVE(IsColorSpace, "color-space?", 1, 0, 0, noFeature)
SPRIMITIVE(Color, "color", 1, 0, 1, noFeature)
SPRIMITIVE(ColorSpace, "color-space", 1, 0, 1, noFeature)
SPRIMITIVE(IsAddress, "address?", 1, 0, 0, noFeature)
SPRIMITIVE(IsAddressLocal, "address-local?", 1, 0, 0, noFeature)
SPRIMITIVE(IsAddressVisited, "address-visited?", 1, 0, 0, noFeature)
SPRIMITIVE(CurrentNodeAddress, "current-node-address", 0, 0, 0, noFeature)
SPRIMITIVE(IdrefAddress, "idref-address", 1, 0, 0, noFeature)
SPRIMITIVE(EntityAddress, "entity-address", 1, 0, 0, noFeature)
SPRIMITIVE(SgmlDocumentAddress, "sgml-document-address", 2, 0, 0, noFeature)
SPRIMITIVE(NodeListAddress, "node-list-address", 1, 0, 0, noFeature)
SPRIMITIVE(HytimeLinkend, "hytime-linkend", 0, 0, 0, noFeature)
SPRIMITIVE(CharScriptCase, "char-script-case", 1, 0, 1, noFeature)
SPRIMITIVE(IsGlyphId, "glyph-id?", 1, 0, 0, noFeature)
SPRIMITIVE(GlyphId, "glyph-id", 1, 0, 0, noFeature)
SPRIMITIVE(IsGlyphSubstTable, "glyph-subst-table?", 1, 0, 0, noFeature)
SPRIMITIVE(GlyphSubstTable, "glyph-subst-table", 1, 0, 0, noFeature)
SPRIMITIVE(GlyphSubst, "glyph-subst", 2, 0, 0, noFeature)
// Core query language
PRIMITIVE(CurrentNode, "current-node", 0, 0, 0, noFeature)
PRIMITIVE(IsNodeListEmpty, "node-list-empty?", 1, 0, 0, noFeature)
PRIMITIVE(IsNodeList, "node-list?", 1, 0, 0, noFeature)
PRIMITIVE(NodeListError, "node-list-error", 2, 0, 0, noFeature)
PRIMITIVE(Gi, "gi", 0, 1, 0, noFeature)
PRIMITIVE(FirstChildGi, "first-child-gi", 0, 1, 0, noFeature)
PRIMITIVE(Id, "id", 0, 1, 0, noFeature)
PRIMITIVE(Parent, "parent", 0, 1, 0, noFeature)
PRIMITIVE(Ancestor, "ancestor", 1, 1, 0, noFeature)
PRIMITIVE(AttributeString, "attribute-string", 1, 1, 0, noFeature)
PRIMITIVE(InheritedAttributeString, "inherited-attribute-string", 1, 1, 0, noFeature)
PRIMITIVE(InheritedElementAttributeString, "inherited-element-attribute-string", 2, 1, 0, noFeature)
PRIMITIVE(IsFirstSibling, "first-sibling?", 0, 1, 0, noFeature)
PRIMITIVE(IsAbsoluteFirstSibling, "absolute-first-sibling?", 0, 1, 0, noFeature)
PRIMITIVE(IsLastSibling, "last-sibling?", 0, 1, 0, noFeature)
PRIMITIVE(IsAbsoluteLastSibling, "absolute-last-sibling?", 0, 1, 0, noFeature)
PRIMITIVE(IsHaveAncestor, "have-ancestor?", 1, 1, 0, noFeature)
PRIMITIVE(ChildNumber, "child-number", 0, 1, 0, noFeature)
PRIMITIVE(AncestorChildNumber, "ancestor-child-number", 1, 1, 0, noFeature)
PRIMITIVE(HierarchicalNumber, "hierarchical-number", 1, 1, 0, noFeature)
PRIMITIVE(HierarchicalNumberRecursive, "hierarchical-number-recursive", 1, 1, 0, noFeature)
PRIMITIVE(ElementNumber, "element-number", 0, 1, 0, noFeature)
PRIMITIVE(ElementNumberList, "element-number-list", 1, 1, 0, noFeature)
PRIMITIVE(EntitySystemId, "entity-system-id", 1, 1, 0, noFeature)
PRIMITIVE(EntityPublicId, "entity-public-id", 1, 1, 0, noFeature)
PRIMITIVE(EntityGeneratedSystemId, "entity-generated-system-id", 1, 1, 0, noFeature)
PRIMITIVE(EntityText, "entity-text", 1, 1, 0, noFeature)
PRIMITIVE(EntityNotation, "entity-notation", 1, 1, 0, noFeature)
PRIMITIVE(EntityType, "entity-type", 1, 1, 0, noFeature)
PRIMITIVE(EntityAttributeString, "entity-attribute-string", 2, 1, 0, noFeature)
PRIMITIVE(NotationSystemId, "notation-system-id", 1, 1, 0, noFeature)
PRIMITIVE(NotationPublicId, "notation-public-id", 1, 1, 0, noFeature)
PRIMITIVE(NotationGeneratedSystemId, "notation-generated-system-id", 1, 1, 0, noFeature)
PRIMITIVE(GeneralNameNormalize, "general-name-normalize", 1, 1, 0, noFeature)
PRIMITIVE(EntityNameNormalize, "entity-name-normalize", 1, 1, 0, noFeature)
// Some non-core query language
PRIMITIVE(NodeListFirst, "node-list-first", 1, 0, 0, query)
PRIMITIVE(NodeListRest, "node-list-rest", 1, 0, 0, query)
PRIMITIVE(NodeList, "node-list", 0, 0, 1, query)
PRIMITIVE(Children, "children", 1, 0, 0, query)
PRIMITIVE(Follow, "follow", 1, 0, 0, query)
PRIMITIVE(Preced, "preced", 1, 0, 0, query)
PRIMITIVE(Descendants, "descendants", 1, 0, 0, query)
PRIMITIVE(Attributes, "attributes", 1, 0, 0, query)
PRIMITIVE(NodeListNoOrder, "node-list-no-order", 1, 0, 0, query)
PRIMITIVE(IsNodeListEqual, "node-list=?", 2, 0, 0, query)
PRIMITIVE(IsNamedNodeList, "named-node-list?", 1, 0, 0, query)
PRIMITIVE(NamedNode, "named-node", 2, 0, 0, query)
PRIMITIVE(NamedNodeListNames, "named-node-list-names", 1, 0, 0, query)
PRIMITIVE(NamedNodeListNormalize, "named-node-list-normalize", 3, 0, 0, query)
PRIMITIVE(SelectElements, "select-elements", 2, 0, 0, query)
PRIMITIVE(Data, "data", 1, 0, 0, query)
PRIMITIVE(ElementWithId, "element-with-id", 1, 1, 0, query)
PRIMITIVE(EmptyNodeList, "empty-node-list", 0, 0, 0, query)
PRIMITIVE(NodeProperty, "node-property", 2, 0, 1, query)
PRIMITIVE(SelectByClass, "select-by-class", 2, 0, 0, query)
PRIMITIVE(NodeListMap, "node-list-map", 2, 0, 0, query)
PRIMITIVE(NodeListRef, "node-list-ref", 2, 0, 0, query)
PRIMITIVE(NodeListReverse, "node-list-reverse", 1, 0, 0, query)
PRIMITIVE(NodeListLength, "node-list-length", 1, 0, 0, query)
PRIMITIVE(SgmlParse, "sgml-parse", 1, 0, 1, query)
PRIMITIVE(IsMatchElement, "match-element?", 2, 0, 0, query)
// External procedures
XPRIMITIVE(ReadEntity, "read-entity", 1, 0, 0)
XPRIMITIVE(Debug, "debug", 1, 0, 0)
XPRIMITIVE(IfFirstPage, "if-first-page", 2, 0, 0)
XPRIMITIVE(IfFrontPage, "if-front-page", 2, 0, 0)
XPRIMITIVE(AllElementNumber, "all-element-number", 0, 1, 0)
// DSSSL2 procedures
PRIMITIVE2(IsVector, "vector?", 1, 0, 0)
PRIMITIVE2(Vector, "vector", 0, 0, 1)
PRIMITIVE2(VectorRef, "vector-ref", 2, 0, 0)
PRIMITIVE2(VectorSet, "vector-set!", 3, 0, 0)
PRIMITIVE2(MakeVector, "make-vector", 1, 1, 0)
PRIMITIVE2(VectorToList, "vector->list", 1, 0, 0)
PRIMITIVE2(ListToVector, "list->vector", 1, 0, 0)
PRIMITIVE2(VectorFill, "vector-fill!", 2, 0, 0)
PRIMITIVE2(IsEqv, "eqv?", 2, 0, 0)
PRIMITIVE2(Memv, "memv", 2, 0, 0)
PRIMITIVE2(QuantityToString, "quantity->string", 1, 1, 0)
//more non-core expression language
PRIMITIVE(Assoc, "assoc", 2, 0, 0, expression)
PRIMITIVE(KeywordToString, "keyword->string", 1, 0, 0, expression)		   
PRIMITIVE(StringToKeyword, "string->keyword", 1, 0, 0, expression)		   
PRIMITIVE(IsExact, "exact?", 1, 0, 0, expression)
PRIMITIVE(IsInexact, "inexact?", 1, 0, 0, expression)
PRIMITIVE(IsZero, "zero?", 1, 0, 0, expression)
PRIMITIVE(IsPositive, "positive?", 1, 0, 0, expression)
PRIMITIVE(IsNegative, "negative?", 1, 0, 0, expression)
PRIMITIVE(IsOdd, "odd?", 1, 0, 0, expression)
PRIMITIVE(IsEven, "even?", 1, 0, 0, expression)
PRIMITIVE(Exp, "exp", 1, 0, 0, expression)
PRIMITIVE(Log, "log", 1, 0, 0, expression)
PRIMITIVE(Sin, "sin", 1, 0, 0, expression)
PRIMITIVE(Cos, "cos", 1, 0, 0, expression)
PRIMITIVE(Tan, "tan", 1, 0, 0, expression)
PRIMITIVE(Asin, "asin", 1, 0, 0, expression)
PRIMITIVE(Acos, "acos", 1, 0, 0, expression)
PRIMITIVE(Atan, "atan", 1, 1, 0, expression)
PRIMITIVE(Expt, "expt", 2, 0, 0, expression)
PRIMITIVE(ExactToInexact, "exact->inexact", 1, 0, 0, expression)
PRIMITIVE(InexactToExact, "inexact->exact", 1, 0, 0, expression)
PRIMITIVE(QuantityToNumber, "quantity->number", 1, 0, 0, expression)
PRIMITIVE(StringToList, "string->list", 1, 0, 0, expression)
PRIMITIVE(ListToString, "list->string", 1, 0, 0, expression)
PRIMITIVE(TimeLess, "time<?", 2, 0, 0, expression)
PRIMITIVE(TimeGreater, "time>?", 2, 0, 0, expression)
PRIMITIVE(TimeLessOrEqual, "time<=?", 2, 0, 0, expression)
PRIMITIVE(TimeGreaterOrEqual, "time>=?", 2, 0, 0, expression)
PRIMITIVE(IsLanguage, "language?", 1, 0, 0, expression)
PRIMITIVE(CurrentLanguage, "current-language", 0, 0, 0, expression)
PRIMITIVE(WithLanguage, "with-language", 2, 0, 0, expression)
PRIMITIVE(CharLess, "char<?", 2, 0, 0, expression)
PRIMITIVE(CharLessOrEqual, "char<=?", 2, 0, 0, expression)
PRIMITIVE(CharUpcase, "char-upcase", 1, 0, 0, expression)
PRIMITIVE(CharDowncase, "char-downcase", 1, 0, 0, expression)
PRIMITIVE(StringEquiv, "string-equiv?", 3, 0, 0, expression)
PRIMITIVE(StringLess, "string<?", 2, 0, 0, expression)
PRIMITIVE(StringLessOrEqual, "string<=?", 2, 0, 0, expression)
// more style language
SPRIMITIVE(MapConstructor, "map-constructor", 2, 0, 0, query)
// more external procedures
XXPRIMITIVE(Language, "language", 2, 0, 0)
XXPRIMITIVE(XSgmlParse, "sgml-parse", 1, 0, 1)
XXPRIMITIVE(XExpt, "expt", 2, 0, 0)
// transformation language
TPRIMITIVE(SubgroveSpec, "subgrove-spec", 0, 0, 1, noFeature)
TPRIMITIVE(IsCreateSpec, "create-spec?", 1, 0, 0, noFeature)
TPRIMITIVE(CreateRoot, "create-root", 2, 0 , 0, noFeature)
TPRIMITIVE(CreateSub, "create-sub", 2, 0, 1, noFeature)
TPRIMITIVE(CreatePreced, "create-preced", 2, 0, 1, noFeature)
TPRIMITIVE(CreateFollow, "create-follow", 2, 0, 1, noFeature)
