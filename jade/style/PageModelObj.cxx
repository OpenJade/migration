// Copyright (c) 1996 James Clark
// creted by Cristian Tornador Antolin 01/09/2003. cristxoc@opencoders.org
// wwww.opencoders.org

#include "stylelib.h"
#include "ELObj.h"
#include "Interpreter.h"
#include "InterpreterMessages.h"
#include "macros.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "PageModelObj.h"

#ifdef DSSSL_NAMESPACE
namespace DSSSL_NAMESPACE {
#endif

//prueba para declarar PageModelObj
PageModelObj::PageModelObj()
: mystmodel_(new FOTBuilder::StModel())
{
}

PageModelObj::PageModelObj(const FOTBuilder::StModel& stpm)
: mystmodel_(new FOTBuilder::StModel(stpm)) 
{
}

PageModelObj::PageModelObj(const FOTBuilder::StModel& stpm, const StringC& str)
: mystmodel_(new FOTBuilder::StModel(stpm)), idstr(str)
{
}


PageModelObj* PageModelObj::asPageModel()
{
 return this;
}

//FIXME para rellenar para DEBUG ... 
void PageModelObj::print(Interpreter &interp, OutputCharStream &out)
{
}

//void PageModelObj::print(Interpreter &interp, _IO_ostream_withassign &out)
void PageModelObj::print()
{
}


void PageModelObj::setWidth(long n)
{
 mystmodel_->stwidth_ = n;
}

void PageModelObj::setHeight(long n)
{
 mystmodel_->stheight_ = n;
}

//se pueden asignar directamente dos StringC&, operator= ok!
void PageModelObj::setID(StringC &str)
{
 idstr = str;
}

void PageModelObj::setFillingDirection(FOTBuilder::Symbol sym)
{
 mystmodel_->stfillingdirection_ = sym;
}

void PageModelObj::setRegion_X_Origin(long n)
{
 mystmodel_->StRegion_->stx_origin_ = n;
}

void PageModelObj::setRegion_Y_Origin(long n)
{
 mystmodel_->StRegion_->sty_origin_ = n;
}

void PageModelObj::setRegion_Width(long n)
{
 mystmodel_->StRegion_->stwidth_ = n;
}

void PageModelObj::setRegion_Height(long n)
{
 mystmodel_->StRegion_->stheight_ = n;
}

void PageModelObj::setRegion_Header_Height(long n)
{
 mystmodel_->StRegion_->StHeader_->stheight_ = n;
}

void PageModelObj::setRegion_Header_Width(long n)
{
 mystmodel_->StRegion_->StHeader_->stwidth_ = n;
}

void PageModelObj::setRegion_Header_FillingDirection(FOTBuilder::Symbol sym)
{
 mystmodel_->StRegion_->StHeader_->stfillingdirection_ = sym;
}

void PageModelObj::setRegion_Header_ContentsAlignment(FOTBuilder::Symbol sym)
{
 mystmodel_->StRegion_->StHeader_->stcontentsalignment_ = sym;
}

void PageModelObj::setRegion_Footer_Height(long n)
{
 mystmodel_->StRegion_->StFooter_->stheight_ = n;
}

void PageModelObj::setRegion_Footer_Width(long n)
{
 mystmodel_->StRegion_->StFooter_->stwidth_ = n;
}

void PageModelObj::setRegion_Footer_FillingDirection(FOTBuilder::Symbol sym)
{
 mystmodel_->StRegion_->StFooter_->stfillingdirection_ = sym;
}

void PageModelObj::setRegion_Footer_ContentsAlignment(FOTBuilder::Symbol sym)
{
 mystmodel_->StRegion_->StFooter_->stcontentsalignment_ = sym;
}

void PageModelObj::setRegion_FillingDirection(FOTBuilder::Symbol sym)
{
 mystmodel_->StRegion_->stfillingdirection_ = sym;
}


long PageModelObj::getWidth()
{
 return mystmodel_->stwidth_;
}

long PageModelObj::getHeight()
{
 return mystmodel_->stheight_;
}

FOTBuilder::Symbol PageModelObj::getFillingDirection()
{
 return mystmodel_->stfillingdirection_;
}

StringC PageModelObj::getID()
{
 return idstr;
}

long PageModelObj::getRegion_X_Origin()
{
 return mystmodel_->StRegion_->stx_origin_;
}

long PageModelObj::getRegion_Y_Origin()
{
 return mystmodel_->StRegion_->sty_origin_;
}

long PageModelObj::getRegion_Width()
{
 return mystmodel_->StRegion_->stwidth_;
}

long PageModelObj::getRegion_Height()
{
 return mystmodel_->StRegion_->stheight_;
}

long PageModelObj::getRegion_Header_Height()
{
 return mystmodel_->StRegion_->StHeader_->stheight_;
}

long PageModelObj::getRegion_Header_Width()
{
 return mystmodel_->StRegion_->StHeader_->stwidth_;
}

FOTBuilder::Symbol PageModelObj::getRegion_Header_FillingDirection()
{
 return mystmodel_->StRegion_->StHeader_->stfillingdirection_;
}

FOTBuilder::Symbol PageModelObj::getRegion_Header_ContentsAlignment()
{
 return mystmodel_->StRegion_->StHeader_->stcontentsalignment_;
}

long PageModelObj::getRegion_Footer_Height()
{
 return mystmodel_->StRegion_->StFooter_->stheight_;
}

long PageModelObj::getRegion_Footer_Width()
{
 return mystmodel_->StRegion_->StFooter_->stwidth_;
}

FOTBuilder::Symbol PageModelObj::getRegion_Footer_FillingDirection()
{
 return mystmodel_->StRegion_->StFooter_->stfillingdirection_;
}

FOTBuilder::Symbol PageModelObj::getRegion_Footer_ContentsAlignment()
{
 return mystmodel_->StRegion_->StFooter_->stcontentsalignment_;
}

FOTBuilder::Symbol PageModelObj::getRegion_FillingDirection()
{
 return mystmodel_->StRegion_->stfillingdirection_;
}

bool PageModelObj::pageModelData(FOTBuilder::StModel& stpm)
{
 StringObj res(idstr);
 const Char *s = res.data();
 char ptrid[30];
 size_t i;
 for (i = 0; i < res.size(); i++)
   switch (s[i]) {
       case '\\':
       case '"':
         // fall through
	 break;
       default:
         ptrid[i]=s[i];
         break;
   }
  ptrid[i]='\0';
  stpm = FOTBuilder::StModel(*mystmodel_,ptrid);
  return true;
}

void PageModelObj::operator=(PageModelObj& pm)
{
idstr = pm.getID();
mystmodel_ = new FOTBuilder::StModel(pm.mystmodel());
}

FOTBuilder::StModel PageModelObj::mystmodel()
{
 return *mystmodel_;
}
 
//FALTA ..... 
bool PageModelObj::isEqual(ELObj &obj)
{
  FOTBuilder::StModel stpm;
  //mira que se trate de PageModelObj y dev. los valores de los atributos anchura y altura
  return (obj.pageModelData(stpm)
           && (stpm.stwidth_ == getWidth())
	   && (stpm.stfillingdirection_ == getFillingDirection())
           && (stpm.stheight_ == getHeight()));
}

//void PageModelObj::whatID(Interpreter &interp, _IO_ostream_withassign &out)
//Para DEBUG
void PageModelObj::whatID()
{
/*
 StringObj res(idstr);
 cout << "\"";
 const Char *s = res.data();
 for (size_t i = 0; i < res.size(); i++)
   switch (s[i]) {
       case '\\':
       case '"':
         cout << "\\";
         // fall through
       default:
         cout.put(s[i]);
          break;
   }
 cout << "\"";
*/
}


#ifdef DSSSL_NAMESPACE
}
#endif 
