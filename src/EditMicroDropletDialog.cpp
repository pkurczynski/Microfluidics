//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Unit1.h"
#include "EditMicroDropletDialog.h"
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TEditMicroDropletDialogBox *EditMicroDropletDialogBox;
//---------------------------------------------------------------------
__fastcall TEditMicroDropletDialogBox::TEditMicroDropletDialogBox(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// TEditMicroDropletDialogBox::Execute()
//
// Executes the MicroDroplet Dialog box.  Allows user to change micro-
// droplet parameters.
//
// called by:  TForm1::EditMicroDropletActionExecute()
// plk 3/5/2004
//---------------------------------------------------------------------
bool __fastcall TEditMicroDropletDialogBox::Execute()
{

   EditMicroDropletDialogBox = new TEditMicroDropletDialogBox(Application);
   bool Result;

   try
   {
      EditMicroDropletDialogBox->DiameterEditBox->Text =
         Form1->theMicroDropletDiameter;

      Result = (EditMicroDropletDialogBox->ShowModal() == IDOK );

      Form1->theMicroDropletDiameter =
         EditMicroDropletDialogBox->DiameterEditBox->Text.ToInt();

   }
   catch(...)
   {
      Result = false;
   }
   EditMicroDropletDialogBox->Free();

   return Result;
}








