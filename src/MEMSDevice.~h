//---------------------------------------------------------------------------
// MEMSDevice.h
//
// Class that implements all of the components of a system that are "on chip."
// The MEMSDevice includes an array of actuators that can be set to specified
// voltages.  The state of a MEMSDevice can be stored to a file, and recalled
// from disk to re-create a specific hardware configuration.  A MEMSDevice is
// owned and controlled by a MEMSystem.
//
// revisions:
//
// 1/8/03       Changed the names of several methods, and made other
//              cosmetic improvements.
//
// 7/28/03      Added methods for control of microdroplets:
//                     CommandActuator, CommandToZero
//
// plk 10/31/00
//---------------------------------------------------------------------------
#ifndef MEMSDeviceH
#define MEMSDeviceH

#include <iostream.h>
#include <vcl.h>

#include "TypeDefs.h"
#include "Actuator.h"
#include "Array2d.h"
#include "WireListData.h"

class MEMSDevice
{
   private:

      ArrayIndex       NumRows;
      ArrayIndex       NumColumns;

      ActuatorVoltage  BiasVoltage;

      void             CalibrateVoltsToDAC();

   public:

      Array2d<Actuator> theActuatorArray;

      Int32 gFocusColumn;
      Int32 gFocusRow;
      Float32 VoltsToDAC[2];
      Float32 DACToVolts[2];

      MEMSDevice();
      ~MEMSDevice();

      void Reset();
      void CommandToZero(TObject *Sender);

      void DisplayFocusActuator(TStaticText *inRowDisplay,
                                TStaticText *inColumnDisplay,
                                TStaticText *inVoltageDisplay,
                                TStaticText *inTypeDisplay,
                                TStaticText *inStatusDisplay,
                                TStaticText *inActuatorIDDisplay,
                                TStaticText *inDACValueDisplay);

      ActuatorVoltage GetBias();
      void SetBias(DACInput inDACValue);

      DACInput GetActuatorDACValue(Int32 inRow, Int32 inColumn);
      void SetActuatorDACValue(Int32 inRow,
                               Int32 inColumn,
                               DACInput inDACValue);


      void CommandActuator(TObject *Sender,
                           Int32 inRow,
                           Int32 inColumn,
                           DACInput inDACValue);


      ArrayIndex GetNumRows(){return NumRows;}
      void SetNumRows(ArrayIndex inNumRows){NumRows = inNumRows;}

      ArrayIndex GetNumColumns(){return NumColumns;}
      void SetNumColumns(ArrayIndex inNumColumns){NumColumns = inNumColumns;}

      void ZernikeShape();

      void ActuateDevice();

      void RotateCCW_90();

      ActuatorVoltage  ConvertDACValueToVolts(DACInput inDACValue);

      bool ActuatorIsAnElectrode(Int32 inRow, Int32 inColumn);

      friend ostream& operator<<(ostream& ioData, MEMSDevice& inMEMSDevice);
      friend istream& operator>>(istream& ioData, MEMSDevice& outMEMSDevice);

};

#endif









