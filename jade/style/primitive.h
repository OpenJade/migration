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
PRIMITIVE(AncestorChildNumber, "ancestor-child-number", 0, 1, 0)
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
PRIMITIVE(Children, "children", 1, 0, 0)
PRIMITIVE(Descendants, "descendants", 1, 0, 0)
PRIMITIVE(Attributes, "attributes", 1, 0, 0)
PRIMITIVE(NodeListNoOrder, "node-list-no-order", 1, 0, 0)
PRIMITIVE(IsNodeListEqual, "node-list=?", 2, 0, 0)
PRIMITIVE(IsNamedNodeList, "named-node-list?", 1, 0, 0)
PRIMITIVE(NamedNode, "named-node", 2, 0, 0)
PRIMITIVE(SelectElements, "select-elements", 2, 0, 0)
PRIMITIVE(Data, "data", 1, 0, 0)
// External procedures
XPRIMITIVE(Debug, "debug", 1, 0, 0)
XPRIMITIVE(IfFirstPage, "if-first-page", 2, 0, 0)
XPRIMITIVE(IfFrontPage, "if-front-page", 2, 0, 0)
