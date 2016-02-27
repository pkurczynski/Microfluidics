//--------------------------------------------------------------------- 
// WireListData.h
//
// Class for transferring data from the Wire List database to an
// individual actuator in a deformable mirror.  This class is used
// in DataModuleCreate() and  Actuator::Initialize()
//
// plk 11/19/00
//
// v.6  added extra data elements to accomodate Bob Boie's wire list
//---------------------------------------------------------------------

#ifndef WIRELISTDATA
#define WIRELISTDATA

#include <Classes.hpp>

class WireListData
{
   public:
        Int32 ActuatorID;
        int Row;
        int Column;
        int ChpPad;
        int PkgPad;
        AnsiString PkgPin;
        AnsiString Socket;
        AnsiString Connector;
        int DAC;
        int Chip;
        int Module;
        AnsiString Type;

};

#endif
