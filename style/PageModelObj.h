// Copyright (c) 1996 James Clark
//created by Cristian Tornador Antolin, barcelona 01/09/2003 cristxoc@opencoders.org
//www.opencoders.org

#ifndef PageModelObj_INCLUDED
#define PageModelObj_INCLUDED 1

#include "types.h"
#include "Collector.h"
#include "OutputCharStream.h"
#include "Boolean.h"
#include "Location.h"
#include "Node.h"
#include "FOTBuilder.h"
#include <string.h>
#include "ELObj.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

//prueba para declarar PageModelObj se encarga de guardar los datos de la hoja de estilo
//del define-page-model
//y pasarselos al FOTBuilder en la forma correspondiente.
class PageModelObj : public ELObj{
public:
  void *operator new(size_t, Collector &c) {
    return c.allocateObject(1);
  }
  void operator=(PageModelObj &);
//----------------------------------------CONSTRUCTORES
  PageModelObj();
  //construye el objeto a partir de una estructura FOTBuilder::StModel
  PageModelObj(const FOTBuilder::StModel &);
  PageModelObj(const FOTBuilder::StModel &, const StringC &);
  PageModelObj(PageModelObj& pm): mystmodel_(new FOTBuilder::StModel(pm.mystmodel())) { } 

  //dev. la referencia de este objeto.
  PageModelObj *asPageModel();
  //void whatID(Interpreter &, _IO_ostream_withassign &);
  void whatID();
  bool isEqual(ELObj &);
  //muestra los valores de este objeto.
  void print(Interpreter &, OutputCharStream &);
  //void print(Interpreter &, _IO_ostream_withassign &);
  void print();
//----------------------------------------MODIF ATRIBUTOS
  void setWidth(long);
  void setHeight(long);
  void setFillingDirection(FOTBuilder::Symbol);
  void setRegion_X_Origin(long);
  void setRegion_Y_Origin(long);
  void setRegion_Width(long);
  void setRegion_Height(long);
  void setRegion_Header_Height(long);
  void setRegion_Header_Width(long);
  void setRegion_Header_FillingDirection(FOTBuilder::Symbol);
  void setRegion_Header_ContentsAlignment(FOTBuilder::Symbol);
  void setRegion_Footer_Height(long);
  void setRegion_Footer_Width(long);
  void setRegion_Footer_FillingDirection(FOTBuilder::Symbol);
  void setRegion_Footer_ContentsAlignment(FOTBuilder::Symbol);
  void setRegion_FillingDirection(FOTBuilder::Symbol);
  void setID(StringC &);
  //extrae los valores en forma de la estructura FOTBuilder::StModel, que es la solicitada.
  bool pageModelData(FOTBuilder::StModel &);
//----------------------------------------CONSULTORES ATRIBUTOS
  long getWidth();
  long getHeight();
  FOTBuilder::Symbol getFillingDirection();
  long getRegion_X_Origin();
  long getRegion_Y_Origin();
  long getRegion_Width();
  long getRegion_Height();
  long getRegion_Header_Height();
  long getRegion_Header_Width();
  FOTBuilder::Symbol getRegion_Header_FillingDirection();
  FOTBuilder::Symbol getRegion_Header_ContentsAlignment();
  long getRegion_Footer_Height();
  long getRegion_Footer_Width();
  FOTBuilder::Symbol getRegion_Footer_FillingDirection();
  FOTBuilder::Symbol getRegion_Footer_ContentsAlignment();
  FOTBuilder::Symbol getRegion_FillingDirection();
  StringC getID();
  FOTBuilder::StModel mystmodel();
//----------------------------------------ATRIBUTOS
private:
 FOTBuilder::StModel *mystmodel_;
 StringC idstr;
};

#ifdef DSSSL_NAMESPACE
}
#endif

#endif /* not PageModelObj_INCLUDED */
