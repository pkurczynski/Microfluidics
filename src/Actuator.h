//-------------------------------------------------------------------------
// Actuator.h
//
// Definition of the Actuator class that contains information about
// a single mirror Actuator (pixel)
//
// ErrorCode explanations:
// Actuator will not actuate unless error code is zero
//
//         Value                Error Condition
//         -----                ---------------
//           0                  No error; Actuator OK to actuate
//           1                  DAC Value is outside allowed range
//           8                  DAC Voltage is outside allowed range
//
// revisions:
//
// 1/8/03       Changed method names SetVoltage() --> Set() etc. plus
//              some other cosmetic changes
//
// plk 10/31/00
//-------------------------------------------------------------------------
#ifndef ACTUATOR
#define ACTUATOR

#include <iostream.h>
#include <vcl.h>
#include "TypeDefs.h"
#include "WireListData.h"
#include "Unit5.h"

class Actuator
{
   private:

        Int32           ActuatorID;
        Int32           Row;
        Int32           Column;
        Int32           ChpPad;
        Int32           PkgPad;
        AnsiString      Socket;
        AnsiString      Connector;
        long            DAC;
        long            Chip;
        long            Module;
        AnsiString      Type;

        DACInput        DACValue;
        ActuatorVoltage Voltage;

        AnsiString      Status;
        bool            Initialized;

        static DACInput        MaxDACValue;
        static DACInput        MinDACValue;
        static ActuatorVoltage MaxVoltage;
        static ActuatorVoltage MinVoltage;


        void     Update();

   public:



        Actuator();
        ~Actuator();
        ActuatorVoltage GetVoltage() const { return Voltage; }
        DACInput GetDACValue() const { return DACValue; }


        void Set(DACInput inNewDACValue);
        void Set(WireListData inWireListData);

        void Reset();

        void Add(DACInput inAddDACValue);

        void Show(TStaticText *ioRowDisplay,
                  TStaticText *ioColumnDisplay,
                  TStaticText *ioVoltageDisplay,
                  TStaticText *ioTypeDisplay,
                  TStaticText *ioStatusDisplay,
                  TStaticText *ioActuatorIDDisplay,
                  TStaticText *ioDACValueDisplay);

        void Actuate();

        void SetStatus();
        AnsiString GetStatus();

        friend ostream& operator<<(ostream& ioData, Actuator& inActuator);
        friend istream& operator>>(istream& ioData, Actuator& outActuator);
};


#endif
//-------------------------------------------------------------------------

