//---------------------------------------------------------------------------
// MEMSDevice.cpp
//
// Implementation of the MEMSDevice class.
//
// plk 10/31/00
//---------------------------------------------------------------------------
#include "Unit1.h"
#include "TypeDefs.h"
#include "MEMSDevice.h"
#include "Array2d.h"
#include "Actuator.h"
#include "MEMSystem.h"
#include "Unit5.h"  // for TViewActuatorForm
#include <math.h>

#define TINY 0.00001

MEMSDevice::MEMSDevice() : theActuatorArray(37, 37)
{

   //CAUTION:  NumRows and NumColumns must be set to the
   // same numerical values as the dimension of theActuatorArray
   // in the initializer list above.

   NumRows = 37;
   NumColumns = 37;

   BiasVoltage = 0;

   gFocusColumn = 0;
   gFocusRow = 0;

   CalibrateVoltsToDAC();

   RotateCCW_90();

}


MEMSDevice::~MEMSDevice()
{
   // destroy the MEMSDevice
}


//---------------------------------------------------------------------------
// Reset()
//
// Resets Actuators in a MEMSDevice to their default values.  Calls
// Individual Actuator Reset Methods.
//
// called by:  MEMSystem::Reset
//---------------------------------------------------------------------------
void MEMSDevice::Reset()
{

   for (ArrayIndex i = 0; i<NumRows; i++)
   {
      for (ArrayIndex j=0;j<NumColumns; j++)
      {
           theActuatorArray(i,j).Reset();
      }
   }

}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// CommandToZero()
//
// Resets Actuators in a MEMSDevice to zero voltage.  Calls
// Individual Actuator  Methods.
//
// called by: TForm1::DrawGrid1_OnSelectCell
//---------------------------------------------------------------------------
void MEMSDevice::CommandToZero(TObject *Sender)
{

   for (ArrayIndex i = 0; i<NumRows; i++)
   {
      for (ArrayIndex j=0;j<NumColumns; j++)
      {
           CommandActuator(Sender,i,j,0);
      }
   }

}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// CalibrateVoltsToDAC()
//
// Sets the appropriate coefficiencts of a linear function that
// the converts Voltages to DAC values, i.e.:
//
//              DACvalue = c[0]  + c[0] * Voltage
//
// Also sets the coefficients for the reciprocal conversion, from
// DAC values to Voltages, i.e.
//
//              Voltage = DACtoVolts[0] + DACToVolts[1] * DACValue
//
// called by:  MEMSDevice::MEMSDevice()
//
//---------------------------------------------------------------------------
void MEMSDevice::CalibrateVoltsToDAC()
{

   // these values obtained from measurements of DAC input (with s'ware) and
   // Voltage output.  The following data values were obtained:
   //     "Y"     =    f( "X" )
   //    DAC input      Negative Voltage (measured with 3478A Multimeter)
   //       0               0   ( roughly 9 mV )
   //      50              1.13
   //     100              2.26
   //     312              7.00
   //
   // Rough calibration values were obtained using a two point fit
   // to the first and last data points, and checking for consistency
   // with the middle two data points.

   VoltsToDAC[0] = 0.0;
   VoltsToDAC[1] = 44.57;

   DACToVolts[0] = -1 * (VoltsToDAC[0]/VoltsToDAC[1]);
   DACToVolts[1] = 1/VoltsToDAC[1];


}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// GetActuatorDACValue()
//
// Returns the DACValue of an Actuator specified by (inRow, inColumn)
//
// arguments: inRow, inColumn   Row and Column of the Actuator in question
//
// called by:  TForm1::DrawGrid1DrawCell()
//             TForm1::ScrollBar1Change()
//             TForm1::GetCellColor()
//---------------------------------------------------------------------------
DACInput MEMSDevice::GetActuatorDACValue(Int32 inRow, Int32 inColumn)
{

   return theActuatorArray(inRow,inColumn).GetDACValue();
}







//---------------------------------------------------------------------------
// SetActuatorDACvalue()
//
// Sets a mirror Actuator specified by (inRow,inColumn) to a specified
// DAC Value.  Does not actuate the actuator (command the hardware).
//
// arguments:  inRow, inColumn         Row and Column of the Current Actuator
//             inDACValue              Desired voltage (piston) for the
//                                     actuator.
//
// called by:  TForm1::ScrollBar1Change()
//---------------------------------------------------------------------------
void MEMSDevice::SetActuatorDACValue(Int32 inRow,
                                     Int32 inColumn,
                                     DACInput inDACValue)
{

   theActuatorArray(inRow,inColumn).Set(inDACValue);


   theActuatorArray(inRow,inColumn).Set(inDACValue);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// CommandActuator()
//
// Sets a mirror Actuator specified by (inRow,inColumn) to a specified
// DAC Value.  Sets the corresponding cell of the drawgrid display to
// the appropriate color.  Sends command to the VME to actuate the array
// if hardware is enabled.
//
// called by: MicroDroplet::Command()
//---------------------------------------------------------------------------
void MEMSDevice::CommandActuator(TObject *Sender,
                                 Int32 inRow,
                                 Int32 inColumn,
                                 DACInput inDACValue)
{

   

   // set the actuator to the desired DACValue,
   // but do not command the VME hardware
   theActuatorArray(inRow,inColumn).Set(inDACValue);

   // command the VME hardware to set the actuator's
   // channel to the set voltage
   theActuatorArray(inRow,inColumn).Actuate();

   // update the graphics display so that the actuator's
   // GridCell reflects the voltage currently being applied
   // to the real world electrode.
   Form1->UpdateDrawGridCell(Sender, inColumn, inRow);


}



//---------------------------------------------------------------------------
// GetBias()
//
// Returns the current device bias as a voltage.
//
// Called by: TForm1::ScrollBar2Change()
//---------------------------------------------------------------------------
ActuatorVoltage MEMSDevice::GetBias()
{
   return BiasVoltage;
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// SetBias()                                      UNDER CONSTRUCTION
//
// Sets all actuators to a common bias value.
//
// Called by: TForm1::ScrollBar2Change()
//---------------------------------------------------------------------------
void MEMSDevice::SetBias(DACInput inBias)
{
   BiasVoltage = ConvertDACValueToVolts(inBias);

   for(ArrayIndex i=0;i<NumRows;i++)
   {
        for(ArrayIndex j=0;j<NumColumns;j++)
        {
             theActuatorArray(i,j).Set(inBias);
        }

   }


}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// DisplayFocusActuator()
//
// Calls the current Actuator's Display method to display its state.
//
// arguments:  inColumnDisplay, inRowDisplay, inPistonDisplay
//              pointers to TStaticText objects where the column, row,
//              piston will be displayed on the graphics window.
//
// called by:   TForm1::DrawGrid1DrawCell
//---------------------------------------------------------------------------
void MEMSDevice::DisplayFocusActuator(  TStaticText *inRowDisplay,
                                        TStaticText *inColumnDisplay,
                                        TStaticText *inVoltageDisplay,
                                        TStaticText *inTypeDisplay,
                                        TStaticText *inStatusDisplay,
                                        TStaticText *inActuatorIDDisplay,
                                        TStaticText *inDACValueDisplay)
{
   theActuatorArray(gFocusRow,gFocusColumn).Show(inRowDisplay,
                                                 inColumnDisplay,
                                                 inVoltageDisplay,
                                                 inTypeDisplay,
                                                 inStatusDisplay,
                                                 inActuatorIDDisplay,
                                                 inDACValueDisplay);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ActuateDevice()
//
// Actuates the MEMSDevice by calling each Actuator's Actuate method,
// which sends commands to the VME bus.
//
// called by: Form1::Button?OnClick()
//---------------------------------------------------------------------------
void MEMSDevice::ActuateDevice()
{

  for(ArrayIndex i=0;i<NumRows;i++)
   {
        for(ArrayIndex j=0;j<NumColumns;j++)
        {
                theActuatorArray(i,j).Actuate();
        }

   }


}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// RotateCCW_90()
//
// Rotates the MEMSDevice Counterclockwise 90 degrees.  Applies
// shift and rotation transformation to all actuators in the device.
//
//---------------------------------------------------------------------------
void MEMSDevice::RotateCCW_90()
{

#if 0
   Int32 theOldRow[19];
   Int32 theOldColumn[19];

   Int32 theNewRow[19];
   Int32 theNewColumn[19];


   // this algorithm assumes a square array.
   for(ArrayIndex i=0;i<NumRows;i++)
   {
        for(ArrayIndex j=0;j<NumColumns;j++)
        {
                theNewRow = theOldColumn;
                theNew

                theNewIndices[i][j]=theOldIndices[j][-i+19];
        }

   }

   // change the Actuator row, column indices to their
   // new values.

   int teststophere = 3;

#endif

}
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// ZernikeShape()                                  UNDER CONSTRUCTION
//
// Sets the actuators to voltages appropriate to simulate a Zernike polynomial
//
//
// called by:  TForm1::ShapeZernikeExecute
//
// NOTE:  Accesses TForm1 public data members:  TDrawGrid1 (properties)
//---------------------------------------------------------------------------
void MEMSDevice::ZernikeShape()
{

   Float32 Xmax = NumColumns*Form1->DrawGrid1->DefaultColWidth;
   Float32 Ymax = NumRows*Form1->DrawGrid1->DefaultRowHeight;

   Float32 Xtrans = Xmax/2;
   Float32 Ytrans = Ymax/2;

   Float32 StretchX = 2.0/Xmax;
   Float32 StretchY = -2.0/Ymax;

   Float32 NewX;
   Float32 NewY;
   Float32 rho;
   Float32 theta;
   Float32 Zernike;
   Float32 NewDACValue;

   Float32 ColumnWidth = Form1->DrawGrid1->DefaultColWidth;
   Float32 RowHeight = Form1->DrawGrid1->DefaultRowHeight;
   Float32 NumDACValues = theSystem->GetNumDACValues();

   Float32 ExtraScale = 0.6;
   for(ArrayIndex i=0;i<NumRows;i++)
   {
        for(ArrayIndex j=0;j<NumColumns;j++)
        {
             NewX = StretchX*(i*ColumnWidth - Xtrans);
             NewY = StretchY*(j*RowHeight - Ytrans);

             rho = sqrt(NewX*NewX + NewY*NewY);

             if (NewX !=0)
             {
                theta = atan2(NewY,NewX);
             }
             else
             {
                theta = atan2(NewY,TINY);
             }

             Zernike = rho;

             NewDACValue = Zernike*NumDACValues*ExtraScale;

             theActuatorArray(i,j).Set(NewDACValue);
        }

   }

}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ConvertDACValueToVolts()
//
// called by: SetBias(), Actuator::Update()
//
//---------------------------------------------------------------------------
ActuatorVoltage MEMSDevice::ConvertDACValueToVolts(DACInput inDACValue)
{

   ActuatorVoltage outVoltage;

   outVoltage = DACToVolts[0] + DACToVolts[1] * inDACValue;

   return outVoltage;


}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ActuatorIsAnElectrode()
//
// Returns a TRUE value if the actuator is an electrode, as opposed
// to an electrode spacer, ground connection etc.
//
// called by:  Form1::GetCellColor()
//
//---------------------------------------------------------------------------
bool MEMSDevice::ActuatorIsAnElectrode(Int32 inRow, Int32 inColumn)
{
     bool theActuatorIsAnElectrode;

     if (theActuatorArray(inRow, inColumn).GetStatus() == "OK")
     {
        theActuatorIsAnElectrode = true;
     } else
     {
        theActuatorIsAnElectrode = false;
     }

     return theActuatorIsAnElectrode;
}



//-------------------------------------------------------------------------
// operator<<
//
// Writes a MEMSDevice object to a stream.  For saving data to a file.
//
// called by: MEMSystem::SaveConfiguration
//
//-------------------------------------------------------------------------
ostream& operator<<(ostream& ioData, MEMSDevice& inMEMSDevice)
{

   for (ArrayIndex i=0;i<inMEMSDevice.NumRows;i++)
   {
      for (ArrayIndex j=0;j<inMEMSDevice.NumColumns;j++)
      {
         // this syntax may not work if theActuatorArray
         // is declared private within MEMSDevice
         ioData << inMEMSDevice.theActuatorArray(i,j) << endl;
      }

      ioData << endl;
   }

   return ioData;
}



//-------------------------------------------------------------------------
// operator>>
//
// Writes data from a stream to a MEMSDevice.  For retrieving data from
// a file.
//
// called by:  MEMSystem::OpenConfiguration
//
//-------------------------------------------------------------------------
istream& operator>>(istream& ioData, MEMSDevice& outMEMSDevice)
{

   // read in Actuator array data
   for (ArrayIndex i=0;i<outMEMSDevice.NumRows;i++)
   {
      for (ArrayIndex j=0;j<outMEMSDevice.NumColumns;j++)
      {
         // this syntax may not work if theActuatorArray
         // is declared private within MEMSDevice plk 11/24/00
         ioData >> outMEMSDevice.theActuatorArray(i,j);
      }
   }

   return ioData;

}



