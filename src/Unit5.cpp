//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit5.h"
#include "MEMSystem.h"
#include "MEMSDevice.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TViewActuatorForm *ViewActuatorForm;
//---------------------------------------------------------------------------
__fastcall TViewActuatorForm::TViewActuatorForm(TComponent* Owner)
        : TForm(Owner)
{
  theSystem->theMirror->DisplayFocusActuatorForm(this);

}
//---------------------------------------------------------------------------

