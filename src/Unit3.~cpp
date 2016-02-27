//----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit3.h"
#include "Unit1.h"
#include "MEMSystem.h"

//----------------------------------------------------------------------------
#pragma resource "*.dfm"
TDataModule3 *DataModule3;

//----------------------------------------------------------------------------
__fastcall TDataModule3::TDataModule3(TComponent *Owner)
	: TDataModule(Owner)
{
}


//----------------------------------------------------------------------------
// DataModuleCreate()
//
// Creates a data module to hold WireList information.
// The Wire List database is set at design time (file name: WireList.dbf)
// The Table and DataSource that contain this information are set
// at design time.  This method calls the initialization method in the
// DeformableMirror class which sets each actuator's DAC card & channel
// address and other parameters stored in the Wire List.
//
// called by:  (system, on program execution)
//----------------------------------------------------------------------------
void __fastcall TDataModule3::DataModuleCreate(TObject *Sender)
{
   theSystem->InitializeWireList(Table1);
}
//----------------------------------------------------------------------------

