// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

PRIMITIVE(Cons, "cons", 2, 0, 0)
PRIMITIVE(List, "list", 0, 0, 1)
PRIMITIVE(IsNull, "null?", 1, 0, 0)
PRIMITIVE(IsList, "list?", 1, 0, 0)
PRIMITIVE(IsPair, "pair?", 1, 0, 0)
PRIMITIVE(IsEqual, "equal?", 2, 0, 0)
PRIMITIVE(Car, "car", 1, 0, 0)
PRIMITIVE(Cdr, "cdr", 1, 0, 0)
PRIMITIVE(Length, "length", 1, 0, 0)
PRIMITIVE(Append, "append", 0, 0, 1)
PRIMITIVE(Reverse, "reverse", 1, 0, 0)
PRIMITIVE(ListTail, "list-tail", 2, 0, 0)
PRIMITIVE(ListRef, "list-ref", 2, 0, 0)
PRIMITIVE(Member, "member", 2, 0, 0)
PRIMITIVE(Not, "not", 1, 0, 0)
PRIMITIVE(IsSymbol, "symbol?", 1, 0, 0)
PRIMITIVE(IsKeyword, "keyword?", 1, 0, 0)
PRIMITIVE(IsBoolean, "boolean?", 1, 0, 0)
PRIMITIVE(IsProcedure, "procedure?", 1, 0, 0)
PRIMITIVE(IsString, "string?", 1, 0, 0)
PRIMITIVE(IsInteger, "integer?", 1, 0, 0)
PRIMITIVE(IsReal, "real?", 1, 0, 0)
PRIMITIVE(IsNumber, "number?", 1, 0, 0)
PRIMITIVE(IsQuantity, "quantity?", 1, 0, 0)
PRIMITIVE(IsChar, "char?", 1, 0, 0)
PRIMITIVE(IsCharEqual, "char=?", 2, 0, 0)
PRIMITIVE(String, "string", 0, 0, 1)
PRIMITIVE(SymbolToString, "symbol->string", 1, 0, 0)
PRIMITIVE(StringToSymbol, "string->symbol", 1, 0, 0)
PRIMITIVE(StringLength, "string-length", 1, 0, 0)
PRIMITIVE(IsStringEqual, "string=?", 2, 0, 0)
PRIMITIVE(StringAppend, "string-append", 0, 0, 1)
PRIMITIVE(StringRef, "string-ref", 2, 0, 0)
PRIMITIVE(Substring, "substring", 3, 0, 0)
PRIMITIVE(Equal, "=", 0, 0, 1)
PRIMITIVE(Plus, "+", 0, 0, 1)
PRIMITIVE(Minus, "-", 1, 0, 1)
PRIMITIVE(Multiply, "*", 0, 0, 1)
PRIMITIVE(Divide, "/", 1, 0, 1)
PRIMITIVE(Quotient, "quotient", 2, 0, 0)
PRIMITIVE(Remainder, "remainder", 2, 0, 0)
PRIMITIVE(Modulo, "modulo", 2, 0, 0)
PRIMITIVE(Less, "<", 0, 0, 1)
PRIMITIVE(Greater, ">", 0, 0, 1)
PRIMITIVE(LessEqual, "<=", 0, 0, 1)
PRIMITIVE(GreaterEqual, ">=", 0, 0, 1)
PRIMITIVE(Min, "min", 1, 0, 1)
PRIMITIVE(Max, "max", 1, 0, 1)
PRIMITIVE(Floor, "floor", 1, 0, 0)
PRIMITIVE(Ceiling, "ceiling", 1, 0, 0)
PRIMITIVE(Truncate, "truncate", 1, 0, 0)
PRIMITIVE(Round, "round", 1, 0, 0)
PRIMITIVE(Abs, "abs", 1, 0, 0)
PRIMITIVE(Sqrt, "sqrt", 1, 0, 0)
PRIMITIVE(Time, "time", 0, 0, 0)
PRIMITIVE(TimeToString, "time->string", 1, 1, 0)
PRIMITIVE(CharProperty, "char-property", 2, 1, 0)
PRIMITIVE(Literal, "literal", 0, 0, 1)
PRIMITIVE(ProcessChildren, "process-children", 0, 0, 0)
PRIMITIVE(ProcessChildrenTrim, "process-children-trim", 0, 0, 0)
PRIMITIVE(SosofoAppend, "sosofo-append", 0, 0, 1)
PRIMITIVE(NextMatch, "next-match", 0, 1, 0)
PRIMITIVE(EmptySosofo, "empty-sosofo", 0, 0, 0)
PRIMITIVE(SosofoLabel, "sosofo-label", 2, 0, 0)
PRIMITIVE(SosofoDiscardLabeled, "sosofo-discard-labeled", 2, 0, 0)
PRIMITIVE(IsSosofo, "sosofo?", 1, 0, 0)
PRIMITIVE(IsStyle, "style?", 1, 0, 0)
PRIMITIVE(MergeStyle, "merge-style", 0, 0, 1)
PRIMITIVE(ProcessElementWithId, "process-element-with-id", 1, 0, 0)
PRIMITIVE(ProcessFirstDescendant, "process-first-descendant", 0, 0, 1)
PRIMITIVE(ProcessMatchingChildren, "process-matching-children", 0, 0, 1)
PRIMITIVE(ProcessNodeList, "process-node-list", 1, 0, 0)
PRIMITIVE(CurrentNodePageNumberSosofo, "current-node-page-number-sosofo",
	  0, 0, 0)
PRIMITIVE(PageNumberSosofo, "page-number-sosofo", 0, 0, 0)
PRIMITIVE(FormatNumber, "format-number", 2, 0, 0)
PRIMITIVE(FormatNumberList, "format-number-list", 3, 0, 0)
PRIMITIVE(Error, "error", 1, 0, 0)
PRIMITIVE(ExternalProcedure, "external-procedure", 1, 0, 0)
PRIMITIVE(StringToNumber, "string->number", 1, 1, 0)
PRIMITIVE(NumberToString, "number->string", 1, 1, 0)
PRIMITIVE(DisplaySize, "display-size", 0, 0, 0)
PRIMITIVE(TableUnit, "table-unit", 1, 0, 0)
PRIMITIVE(IsDisplaySpace, "display-space?", 1, 0, 0)
PRIMITIVE(DisplaySpace, "display-space", 1, 0, 1)
PRIMITIVE(IsInlineSpace, "inline-space?", 1, 0, 0)
PRIMITIVE(InlineSpace, "inline-space", 1, 0, 1)
PRIMITIVE(IsColor, "color?", 1, 0, 0)
PRIMITIVE(IsColorSpace, "color-space?", 1, 0, 0)
PRIMITIVE(Color, "color", 1, 0, 1)
PRIMITIVE(ColorSpace, "color-space", 1, 0, 1)
PRIMITIVE(IsAddress, "address?", 1, 0, 0)
PRIMITIVE(IsAddressLocal, "address-local?", 1, 0, 0)
PRIMITIVE(IsAddressVisited, "address-visited?", 1, 0, 0)
PRIMITIVE(CurrentNodeAddress, "current-node-address", 0, 0, 0)
PRIMITIVE(IdrefAddress, "idref-address", 1, 0, 0)
PRIMITIVE(EntityAddress, "entity-address", 1, 0, 0)
PRIMITIVE(SgmlDocumentAddress, "sgml-document-address", 2, 0, 0)
PRIMITIVE(NodeListAddress, "node-list-address", 1, 0, 0)
PRIMITIVE(HytimeLinkend, "hytime-linkend", 0, 0, 0)
PRIMITIVE(CharScriptCase, "char-script-case", 1, 0, 1)
PRIMITIVE(IsGlyphId, "glyph-id?", 1, 0, 0)
PRIMITIVE(GlyphId, "glyph-id", 1, 0, 0)
PRIMITIVE(IsGlyphSubstTable, "glyph-subst-table?", 1, 0, 0)
PRIMITIVE(GlyphSubstTable, "glyph-subst-table", 1, 0, 0)
PRIMITIVE(GlyphSubst, "glyph-subst", 2, 0, 0)
// Core query language
PRIMITIVE(CurrentNode, "current-node", 0, 0, 0)
PRIMITIVE(IsNodeListEmpty, "node-list-empty?", 1, 0, 0)
PRIMITIVE(IsNodeList, "node-list?", 1, 0, 0)
PRIMITIVE(NodeListError, "node-list-error", 2, 0, 0)
PRIMITIVE(Gi, "gi", 0, 1, 0)
PRIMITIVE(FirstChildGi, "first-child-gi", 0, 1, 0)
PRIMITIVE(Id, "id", 0, 1, 0)
PRIMITIVE(Parent, "parent", 0, 1, 0)
PRIMITIVE(Ancestor, "ancestor", 1, 1, 0)
PRIMITIVE(AttributeString, "attribute-string", 1, 1, 0)
PRIMITIVE(InheritedAttributeString, "inherited-attribute-string", 1, 1, 0)
PRIMITIVE(InheritedElementAttributeString, "inherited-element-attribute-string", 2, 1, 0)
PRIMITIVE(IsFirstSibling, "first-sibling?", 0, 1, 0)
PRIMITIVE(IsAbsoluteFirstSibling, "absolute-first-sibling?", 0, 1, 0)
PRIMITIVE(IsLastSibling, "last-sibling?", 0, 1, 0)
PRIMITIVE(IsAbsoluteLastSibling, "absolute-last-sibling?", 0, 1, 0)
PRIMITIVE(IsHaveAncestor, "have-ancestor?", 1, 1, 0)
PRIMITIVE(ChildNumber, "child-number", 0, 1, 0)
PRIMITIVE(AncestorChildNumber, "ancestor-child-number", 1, 1, 0)
PRIMITIVE(HierarchicalNumber, "hierarchical-number", 1, 1, 0)
PRIMITIVE(HierarchicalNumberRecursive, "hierarchical-number-recursive", 1, 1, 0)
PRIMITIVE(ElementNumber, "element-number", 0, 1, 0)
PRIMITIVE(ElementNumberList, "element-number-list", 1, 1, 0)
PRIMITIVE(EntitySystemId, "entity-system-id", 1, 1, 0)
PRIMITIVE(EntityPublicId, "entity-public-id", 1, 1, 0)
PRIMITIVE(EntityGeneratedSystemId, "entity-generated-system-id", 1, 1, 0)
PRIMITIVE(EntityText, "entity-text", 1, 1, 0)
PRIMITIVE(EntityNotation, "entity-notation", 1, 1, 0)
PRIMITIVE(EntityType, "entity-type", 1, 1, 0)
PRIMITIVE(EntityAttributeString, "entity-attribute-string", 2, 1, 0)
PRIMITIVE(NotationSystemId, "notation-system-id", 1, 1, 0)
PRIMITIVE(NotationPublicId, "notation-public-id", 1, 1, 0)
PRIMITIVE(NotationGeneratedSystemId, "notation-generated-system-id", 1, 1, 0)
PRIMITIVE(GeneralNameNormalize, "general-name-normalize", 1, 1, 0)
PRIMITIVE(EntityNameNormalize, "entity-name-normalize", 1, 1, 0)
// Some non-core query language
PRIMITIVE(NodeListFirst, "node-list-first", 1, 0, 0)
PRIMITIVE(NodeListRest, "node-list-rest", 1, 0, 0)
PRIMITIVE(NodeList, "node-list", 0, 0, 1)
PRIMITIVE(Children, "children", 1, 0, 0)
PRIMITIVE(Follow, "follow", 1, 0, 0)
PRIMITIVE(Preced, "preced", 1, 0, 0)
PRIMITIVE(Descendants, "descendants", 1, 0, 0)
PRIMITIVE(Attributes, "attributes", 1, 0, 0)
PRIMITIVE(NodeListNoOrder, "node-list-no-order", 1, 0, 0)
PRIMITIVE(IsNodeListEqual, "node-list=?", 2, 0, 0)
PRIMITIVE(IsNamedNodeList, "named-node-list?", 1, 0, 0)
PRIMITIVE(NamedNode, "named-node", 2, 0, 0)
PRIMITIVE(NamedNodeListNames, "named-node-list-names", 1, 0, 0)
PRIMITIVE(NamedNodeListNormalize, "named-node-list-normalize", 3, 0, 0)
PRIMITIVE(SelectElements, "select-elements", 2, 0, 0)
PRIMITIVE(Data, "data", 1, 0, 0)
PRIMITIVE(ElementWithId, "element-with-id", 1, 1, 0)
PRIMITIVE(EmptyNodeList, "empty-node-list", 0, 0, 0)
PRIMITIVE(NodeProperty, "node-property", 2, 0, 1)
PRIMITIVE(SelectByClass, "select-by-class", 2, 0, 0)
PRIMITIVE(NodeListMap, "node-list-map", 2, 0, 0)
PRIMITIVE(NodeListRef, "node-list-ref", 2, 0, 0)
PRIMITIVE(NodeListReverse, "node-list-reverse", 1, 0, 0)
PRIMITIVE(NodeListLength, "node-list-length", 1, 0, 0)
PRIMITIVE(SgmlParse, "sgml-parse", 1, 0, 1)
PRIMITIVE(IsMatchElement, "match-element?", 2, 0, 0)
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
PRIMITIVE(Assoc, "assoc", 2, 0, 0)
PRIMITIVE(KeywordToString, "keyword->string", 1, 0, 0)		   
PRIMITIVE(StringToKeyword, "string->keyword", 1, 0, 0)		   
PRIMITIVE(IsExact, "exact?", 1, 0, 0)
PRIMITIVE(IsInexact, "inexact?", 1, 0, 0)
PRIMITIVE(IsZero, "zero?", 1, 0, 0)
PRIMITIVE(IsPositive, "positive?", 1, 0, 0)
PRIMITIVE(IsNegative, "negative?", 1, 0, 0)
PRIMITIVE(IsOdd, "odd?", 1, 0, 0)
PRIMITIVE(IsEven, "even?", 1, 0, 0)
PRIMITIVE(Exp, "exp", 1, 0, 0)
PRIMITIVE(Log, "log", 1, 0, 0)
PRIMITIVE(Sin, "sin", 1, 0, 0)
PRIMITIVE(Cos, "cos", 1, 0, 0)
PRIMITIVE(Tan, "tan", 1, 0, 0)
PRIMITIVE(Asin, "asin", 1, 0, 0)
PRIMITIVE(Acos, "acos", 1, 0, 0)
PRIMITIVE(Atan, "atan", 1, 1, 0)
PRIMITIVE(Expt, "expt", 2, 0, 0)
PRIMITIVE(ExactToInexact, "exact->inexact", 1, 0, 0)
PRIMITIVE(InexactToExact, "inexact->exact", 1, 0, 0)
PRIMITIVE(QuantityToNumber, "quantity->number", 1, 0, 0)
PRIMITIVE(StringToList, "string->list", 1, 0, 0)
PRIMITIVE(ListToString, "list->string", 1, 0, 0)
PRIMITIVE(TimeLess, "time<?", 2, 0, 0)
PRIMITIVE(TimeGreater, "time>?", 2, 0, 0)
PRIMITIVE(TimeLessOrEqual, "time<=?", 2, 0, 0)
PRIMITIVE(TimeGreaterOrEqual, "time>=?", 2, 0, 0)
PRIMITIVE(IsLanguage, "language?", 1, 0, 0)
PRIMITIVE(CurrentLanguage, "current-language", 0, 0, 0)
PRIMITIVE(WithLanguage, "with-language", 2, 0, 0)
PRIMITIVE(CharLess, "char<?", 2, 0, 0)
PRIMITIVE(CharLessOrEqual, "char<=?", 2, 0, 0)
PRIMITIVE(CharUpcase, "char-upcase", 1, 0, 0)
PRIMITIVE(CharDowncase, "char-downcase", 1, 0, 0)
PRIMITIVE(StringEquiv, "string-equiv?", 3, 0, 0)
PRIMITIVE(StringLess, "string<?", 2, 0, 0)
PRIMITIVE(StringLessOrEqual, "string<=?", 2, 0, 0)
// more style language
PRIMITIVE(MapConstructor, "map-constructor", 2, 0, 0)
// more external procedures
XXPRIMITIVE(Language, "language", 2, 0, 0)
