// Copyright (c) 1996 James Clark
// See the file copying.txt for copying permission.

#ifndef ProcessContext_INCLUDED
#define ProcessContext_INCLUDED 1

#include <OpenSP/Resource.h>
#include <OpenSP/Ptr.h>
#include <OpenSP/Vector.h>
#include <OpenSP/NCVector.h>
#include <OpenSP/Owner.h>
#include "Collector.h"
#include "Style.h"
#include "FOTBuilder.h"
#include "ELObj.h"
#include "SosofoObj.h"
#include "VM.h"
#include "ProcessingMode.h"
#include <OpenSP/Link.h>
#include <OpenSP/IList.h>
#include <OpenSP/IQueue.h>

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

class Expression;

class ProcessContext : public Collector::DynamicRoot {
public:
  ProcessContext(Interpreter &, FOTBuilder &);
  ~ProcessContext();
  FOTBuilder &currentFOTBuilder();
  StyleStack &currentStyleStack();
  void process(const NodePtr &);
  void processNode(const NodePtr &, const ProcessingMode *,
		   const Location &, bool chunk = 1);
  void processNodeSafe(const NodePtr &, const ProcessingMode *,
		       const Location &, bool chunk = 1);
  void nextMatch(StyleObj *, const Location &loc);
  void processChildren(const ProcessingMode *, const Location &);
  void processChildrenTrim(const ProcessingMode *, const Location &);
  void characters(const Char *, size_t, const Location &);
  void charactersFromNode(const NodePtr &, const Char *, size_t,
			  const Location &);
  void trace(Collector &) const;
  void startFlowObj();
  void endFlowObj();
  // Uses of label: do this
  void startConnection(SymbolObj *, const Location &);
  void endConnection();

  // Contains information about which flow object classes a port shall accept
  // and can validate flow objects according to this information.
  class Validator : public Resource {
  public:
    // If the flow object specified by the argument is valid in the current
    // position in this stream, this function returns true and updates
    // state information as if the flow object was added to the stream.
    // If the FO isn't valid, this method reports it with a message.
    // Default implementation returns true.
    virtual bool isValid(const FlowObj &, ProcessContext &);
    virtual bool charsValid(size_t, const Location &, ProcessContext &);
  };

  void validate(const FlowObj &);
  void endValidate();
  // happens only for object with a non-principal port
  void pushPorts(bool hasPrincipalPort,
		 const Vector<SymbolObj *> &ports,
		 const Vector<FOTBuilder *> &fotbs,
		 const Vector<Validator *> validators,
		 Validator *principalPortValidator = 0);
  void popPorts();
  void pushPrincipalPort(Validator *);
  void popPrincipalPort();
  // Used by SpS headers and footers.
  void pushPseudoPort(FOTBuilder *principalPort,
			 Validator *);
  void popPseudoPort();
  // happens inside pushPorts() (if any)
  void startMapContent(ELObj *, const Location &);
  void endMapContent();
  void startDiscardLabeled(SymbolObj *);
  void endDiscardLabeled();
  // table support
  void startTable();
  void endTable();
  void startTablePart();
  void endTablePart();
  void addTableColumn(unsigned columnIndex, unsigned span, StyleObj *);
  unsigned currentTableColumn();
  void noteTableCell(unsigned colIndex, unsigned colSpan, unsigned rowSpan);
  StyleObj *tableColumnStyle(unsigned columnIndex, unsigned span);
  StyleObj *tableRowStyle();
  void startTableRow(StyleObj *);
  bool inTable() const;
  bool inTableRow();
  void endTableRow();
  void clearPageType();
  void setPageType(unsigned);
  bool getPageType(unsigned &) const;
  void startParagraph();
  void endParagraph();
  bool paragraphAncestor() const;

  VM &vm();
 private:
  ProcessContext(const ProcessContext &); // undefined
  void operator=(const ProcessContext &); // undefined
  void badContentMap(bool &, const Location &);
  void coverSpannedRows();
  void restoreConnection(unsigned connectableLevel, size_t portIndex);
  struct Port {
    Port();
    FOTBuilder *fotb;
    IQueue<NodeSaveFOTBuilder> saveQueue;
    Vector<SymbolObj *> labels;
    Ptr<Validator> validator;
    unsigned connected;
  };
  // A flow object with a port that can be connected to.
  struct Connectable;
  friend struct Connectable;
  struct Connectable : public Link {
    Connectable(int nPorts, const StyleStack &, unsigned);
    NCVector<Port> ports;
    StyleStack styleStack;
    unsigned flowObjLevel;
    Vector<SymbolObj *> principalPortLabels;
    Ptr<Validator> principalPortValidator;
  };
  // A connection between a flow object and its flow parent
  // made with label:.
  struct Connection;
  friend struct Connection;
  struct Connection : public Link {
    Connection(FOTBuilder *, const StyleStack & = StyleStack());
    Connection(const StyleStack &, Port *, unsigned connectableLevel);
    FOTBuilder *fotb;
    StyleStack styleStack;
    Port *port;
    unsigned connectableLevel;
    unsigned nBadFollow;
  };
  struct Table : public Link {
    Table();
    unsigned currentColumn;
    // first index is column (zero-based)
    // second is span - 1.
    Vector<Vector<StyleObj *> > columnStyles;
    // for each column, how many rows are covered
    // starting with the current row
    Vector<unsigned> covered;
    unsigned nColumns;
    StyleObj *rowStyle;
    bool inTableRow;
    unsigned rowConnectableLevel;
  };
  struct NodeStackEntry {
    unsigned long elementIndex;
    unsigned groveIndex;
    const ProcessingMode *processingMode;
  };
  FOTBuilder ignoreFotb_;
  IList<Connection> connectionStack_;
  IList<Connectable> connectableStack_;
  unsigned connectableStackLevel_;
  IList<Table> tableStack_;
  NCVector<IQueue<NodeSaveFOTBuilder> > principalPortSaveQueues_;
  VM vm_;
  ProcessingMode::Specificity matchSpecificity_;
  unsigned flowObjLevel_;
  bool havePageType_;
  unsigned pageType_;
  unsigned paragraphLevel_;
  Vector<NodeStackEntry> nodeStack_;
  Vector<unsigned> invalidLevels_;
  Vector<Ptr<Validator> > validatorStack_;
  friend class CurrentNodeSetter;
  friend struct Table;
};

inline
FOTBuilder &ProcessContext::currentFOTBuilder()
{
  return *connectionStack_.head()->fotb;
}

inline
StyleStack &ProcessContext::currentStyleStack()
{
  return connectionStack_.head()->styleStack;
}

inline
VM &ProcessContext::vm()
{
  return vm_;
}

inline
void ProcessContext::startFlowObj()
{
  flowObjLevel_++;
}

inline
void ProcessContext::setPageType(unsigned n)
{
  havePageType_ = 1;
  pageType_ = n;
}

inline
void ProcessContext::clearPageType()
{
  havePageType_ = 0;
}

inline
bool ProcessContext::getPageType(unsigned &n) const
{
  if (!havePageType_)
    return 0;
  n = pageType_;
  return 1;
}

inline
bool ProcessContext::inTable() const
{
  return !tableStack_.empty();
}

inline
void ProcessContext::startParagraph()
{
  paragraphLevel_++;
}

inline
void ProcessContext::endParagraph()
{
  paragraphLevel_--;
}

inline
bool ProcessContext::paragraphAncestor() const
{
  return paragraphLevel_ > 0;
}

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* not ProcessContext_INCLUDED */

