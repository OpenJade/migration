// Copyright (c) 1996 James Clark
// See the file COPYING for copying permission.

#ifdef __GNUG__
#pragma implementation
#endif

#include "config.h"
#include "Boolean.h"
#include "Node.h"
#include <string.h>

#ifdef GROVE_NAMESPACE
namespace GROVE_NAMESPACE {
#endif

AccessResult Node::getOrigin(NodePtr &) const
{
  return accessNotInClass;
}

AccessResult Node::getParent(NodePtr &) const
{
  return accessNotInClass;
}

AccessResult Node::getGroveRoot(NodePtr &) const
{
  return accessNotInClass;
}

AccessResult Node::getAttributes(NamedNodeListPtr &) const
{
  return accessNotInClass;
}

AccessResult Node::getName(GroveString &) const
{
  return accessNotInClass;
}

AccessResult Node::getSystemData(GroveString &) const
{
  return accessNotInClass;
}

AccessResult Node::getEntity(NodePtr &) const
{
  return accessNotInClass;
}

AccessResult Node::getEntityName(GroveString &) const
{
  return accessNotInClass;
}

AccessResult Node::getExternalId(NodePtr &) const
{
  return accessNotInClass;
}

AccessResult Node::getNotation(NodePtr &) const
{
  return accessNotInClass;
}
AccessResult Node::getText(GroveString &) const
{
  return accessNotInClass;
}

AccessResult Node::getNotationName(GroveString &) const
{
  return accessNotInClass;
}

AccessResult Node::getEntityType(EntityType &) const
{
  return accessNotInClass;
}

AccessResult Node::getDefaulted(bool &) const
{
  return accessNotInClass;
}

AccessResult Node::getPublicId(GroveString &) const
{
  return accessNotInClass;
}

AccessResult Node::getSystemId(GroveString &) const
{
  return accessNotInClass;
}

AccessResult Node::getGeneratedSystemId(GroveString &) const
{
  return accessNotInClass;
}

AccessResult Node::getValue(NodeListPtr &) const
{
  return accessNotInClass;
}

AccessResult Node::getTokenSep(GroveChar &) const
{
  return accessNotInClass;
}

AccessResult Node::getImplied(bool &) const
{
  return accessNotInClass;
}

AccessResult Node::getGi(GroveString &) const
{
  return accessNotInClass;
}

AccessResult Node::getId(GroveString &) const
{
  return accessNotInClass;
}

AccessResult Node::getContent(NodeListPtr &) const
{
  return accessNotInClass;
}

AccessResult Node::getIncluded(bool &) const
{
  return accessNotInClass;
}

AccessResult Node::getMustOmitEndTag(bool &) const
{
  return accessNotInClass;
}

AccessResult Node::getToken(GroveString &) const
{
  return accessNotInClass;
}

AccessResult Node::getReferent(NodePtr &) const
{
  return accessNotInClass;
}

AccessResult Node::getGoverning(bool &) const
{
  return accessNotInClass;
}

AccessResult Node::getGeneralEntities(NamedNodeListPtr &) const
{
  return accessNotInClass;
}

AccessResult Node::getNotations(NamedNodeListPtr &) const
{
  return accessNotInClass;
}

AccessResult Node::getSgmlConstants(NodePtr &) const
{
  return accessNotInClass;
}

AccessResult Node::getApplicationInfo(GroveString &) const
{
  return accessNotInClass;
}

AccessResult Node::getProlog(NodeListPtr &) const
{
  return accessNotInClass;
}

AccessResult Node::getEpilog(NodeListPtr &) const
{
  return accessNotInClass;
}

AccessResult Node::getDocumentElement(NodePtr &) const
{
  return accessNotInClass;
}

AccessResult Node::getElements(NamedNodeListPtr &) const
{
  return accessNotInClass;
}

AccessResult Node::getEntities(NamedNodeListPtr &) const
{
  return accessNotInClass;
}

AccessResult Node::getDefaultedEntities(NamedNodeListPtr &) const
{
  return accessNotInClass;
}

AccessResult Node::getGoverningDoctype(NodePtr &) const
{
  return accessNotInClass;
}

AccessResult Node::getDoctypesAndLinktypes(NamedNodeListPtr &) const
{
  return accessNotInClass;
}

AccessResult Node::children(NodeListPtr &) const
{
  return accessNotInClass;
}

AccessResult Node::nextSibling(NodePtr &) const
{
  return accessNotInClass;
}

AccessResult Node::nextChunkSibling(NodePtr &) const
{
  return accessNotInClass;
}

AccessResult Node::firstChild(NodePtr &) const
{
  return accessNotInClass;
}

AccessResult Node::charChunk(const SdataMapper &, GroveString &) const
{
  return accessNotInClass;
}

AccessResult Node::firstSibling(NodePtr &) const
{
  return accessNotInClass;
}

AccessResult Node::siblingsIndex(unsigned long &) const
{
  return accessNotInClass;
}

AccessResult Node::tokens(GroveString &) const
{
  return accessNotInClass;
}

unsigned long Node::hash() const
{
  // Hopefully implementations will do better.
  return 0;
}

AccessResult Node::elementIndex(unsigned long &) const
{
  return accessNotInClass;
}

AccessResult Node::attributeRef(unsigned long n, NodePtr &ptr) const
{
  NamedNodeListPtr atts;
  AccessResult ret = getAttributes(atts);
  if (ret != accessOK)
    return ret;
  return atts->nodeList()->ref(n, ptr);
}

AccessResult Node::followSiblingRef(unsigned long n, NodePtr &ptr) const
{
  if (n == 0)
    return nextSibling(ptr);
  NodePtr tem;
  AccessResult ret = nextSibling(tem);
  if (ret != accessOK)
    return ret;
  while (--n > 0) {
    ret = tem.assignNextSibling();
    if (ret != accessOK)
      return ret;
  }
  return tem->nextSibling(ptr);
}

AccessResult NodeList::ref(unsigned long n, NodePtr &ptr) const
{
  if (n == 0)
    return first(ptr);
  NodeListPtr tem;
  AccessResult ret = rest(tem);
  if (ret != accessOK)
    return ret;
  while (--n > 0) {
    ret = tem.assignRest();
    if (ret != accessOK)
      return ret;
  }
  return tem->first(ptr);
}

NodeListPtr NamedNodeList::nodeListNoOrder() const
{
  return nodeList();
}

void NodeVisitor::sgmlDocument(Node &)
{
}

void NodeVisitor::sgmlConstants(Node &)
{
}

void NodeVisitor::dataChar(Node &)
{
}

void NodeVisitor::element(Node &)
{
}

void NodeVisitor::attributeAssignment(Node &)
{
}

void NodeVisitor::attributeValueToken(Node &)
{
}

void NodeVisitor::pi(Node &)
{
}

void NodeVisitor::sdata(Node &)
{
}

void NodeVisitor::documentType(Node &)
{
}

void NodeVisitor::entity(Node &)
{
}

void NodeVisitor::notation(Node &)
{
}

void NodeVisitor::externalId(Node &)
{
}

void NodeVisitor::externalData(Node &)
{
}

void NodeVisitor::subdoc(Node &)
{
}

bool GroveString::operator==(const GroveString &str) const
{
  if (size() != str.size())
    return 0;
  if (size() == 0)
    return 1;
  if (memcmp(data(), str.data(), size() * sizeof(GroveChar)) == 0)
    return 1;
  return 0;
}

SdataMapper::~SdataMapper()
{
}

bool SdataMapper::sdataMap(GroveString name, GroveString text, GroveChar &) const
{
  return 0;
}

#ifdef GROVE_NAMESPACE
}
#endif
