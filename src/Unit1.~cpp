//---------------------------------------------------------------------------
// Unit1.cpp                                    C++ Program Unit
//
// Main program file for MEMS/AO mirror development static,
// open loop testbed software.
//
// plk 11/19/00
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <stdio.h>

#include "Unit1.h"
#include "Unit2.h"      // File...New Dialog Box (under development)
#include "Unit4.h"      // view Wire List Database Form
#include "MEMSystem.h"
#include "MicroDroplet.h"
#include "EditMicroDropletDialog.h"

#include "TypeDefs.h"

#define myRGB(r,g,b) ( ((long)( (b) << 8|(g)) << 8 ) | (r) )

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
MicroDroplet *theMicroDroplet;


//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{

   theSystem = new MEMSystem;

   theMicroDropletDiameter = 5;
   theMicroDroplet = new MicroDroplet(theMicroDropletDiameter);


   DrawGrid1->Visible = true;
}
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------
//DrawGrid1DrawCell()
//
// Draws a Cell in DrawGrid1, corresponding to a single "pixel" of the
// deformable mirror.  Color of the cell is determined by the pixel voltage.
// Also updates the TrackBar position and the text displays of current,
// focussed row, column and voltage of pixel Actuator.
//
// called by:   Event Handler for DrawCell events of DrawGrid1
//              ScrollBar1OnChange
//
//---------------------------------------------------------------------------
void __fastcall TForm1::DrawGrid1DrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
   // draw the cell with the appropriate color
   TColor theCellColor = GetCellColor(ACol,ARow);
   DrawGrid1->Canvas->Brush->Color = theCellColor;
   DrawGrid1->Canvas->FillRect(Rect);

   

   // draw the focus rectangle if the cell has focus
   if (State.Contains(gdFocused))
   {
        DrawGrid1->Canvas->DrawFocusRect(Rect);

        theSystem->theMirror->gFocusColumn = ACol;
        theSystem->theMirror->gFocusRow = ARow;

        // update the focus'ed actuator parameters in the
        // actuator text window
        theSystem->theMirror->DisplayFocusActuator(StaticText4,
                                                   StaticText5,
                                                   StaticText6,
                                                   StaticText11,
                                                   StaticText9,
                                                   StaticText14,
                                                   StaticText16);
        ScrollBar1->Position = theSystem->theMirror->GetActuatorDACValue(ARow,ACol);


   }
}
//---------------------------------------------------------------------------



void __fastcall TForm1::DrawGrid1_OnMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

#if 0
      // Column and Row are returned from MouseToCell with the
      // X,Y coordinates of the center of the cell in which the
      // mouse pointer currently is placed.       plk 7/25/2003
      long MaxGridPixelColumn, MaxGridPixelRow;
      long CellAndGridColumnWidth, CellAndGridRowWidth;
      long Column, Row;
      DrawGrid1->MouseToCell(X,Y,MaxGridPixelColumn,MaxGridPixelRow);

      CellAndGridColumnWidth = DrawGrid1->ColCount + DrawGrid1->GridLineWidth;
      CellAndGridRowWidth = DrawGrid1->RowCount + DrawGrid1->GridLineWidth;

      Column = ( MaxGridPixelColumn - CellAndGridColumnWidth ) /
                        CellAndGridColumnWidth;
      Row = (MaxGridPixelRow - CellAndGridRowWidth ) /
                        CellAndGridRowWidth;

      Form1->MouseXCoordTextBox->Caption = AnsiString((int)Row);
      Form1->MouseYCoordTextBox->Caption = AnsiString((int)Column);

#endif

}
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------
// GetCellColor()
//
// Computes the color of a pixel at a specified column, row in the
// draw cell.  Uses global variables.
//
// called by:  TForm1::DrawGrid1DrawCell
//
// NOTE:  Accesses MEMSystem property "NumDACValues"  (the number
// of discrete DAC values = (e.g.) 8192 for 13 bit DAC)
//
//---------------------------------------------------------------------------
TColor TForm1::GetCellColor(int inColumn, int inRow)
{
  short index;
  short theBlueValue;
  short theGreenValue;
  short theRedValue;

  TColor theCellColor;

  if ( theSystem->theMirror->ActuatorIsAnElectrode(inRow, inColumn) )
  {
        // GetActuatorDACValue returns a DACInput, which type is defined
        // in TypeDefs.h to be bt_data16_t which type is a 16 Bit integer
        // (i.e. short)
        index = theSystem->theMirror->GetActuatorDACValue(inRow,inColumn);

        // this procedure results in a spectrum of yellow fading to blue
        // at higher DAC values
        theBlueValue = (short) (index*255/theSystem->GetNumDACValues());
        theGreenValue = 255 - (short)(index*255/theSystem->GetNumDACValues());
        theRedValue = 255 - (short)(index*255/theSystem->GetNumDACValues());
        theCellColor = myRGB(theRedValue,theGreenValue,theBlueValue);
  } else
  {
        // if the cell does not represent an electrode, then color it
        // grey
        theCellColor = myRGB(50,30,30);
  }

  return theCellColor;

}
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// ScrollBar1Change()
//
// Changes the voltage of a focused mirror pixel (draw cell) using the
// TrackBar.  Draws the mirror pixel in the appropriate color.
//
//---------------------------------------------------------------------------
void __fastcall TForm1::ScrollBar1Change(TObject *Sender)
{
   theSystem->theMirror->SetActuatorDACValue(theSystem->theMirror->gFocusRow,
                                             theSystem->theMirror->gFocusColumn,
                                             ScrollBar1->Position);

   TRect theCellRect = DrawGrid1->CellRect(theSystem->theMirror->gFocusColumn,
                                           theSystem->theMirror->gFocusRow);

   StaticText6->Caption = theSystem->theMirror->GetActuatorDACValue(
                                theSystem->theMirror->gFocusRow,
                                theSystem->theMirror->gFocusColumn);


   // Draw the focused cell in the new color, to match the updated
   // Actuator value from the TrackBarChange event
   TGridDrawState theState;
   theState << gdFocused;
   DrawGrid1DrawCell(Sender,
                     theSystem->theMirror->gFocusColumn,
                     theSystem->theMirror->gFocusRow,
                     theCellRect,theState);
}
//---------------------------------------------------------------------------





void __fastcall TForm1::FileNewExecute(TObject *Sender)
{

 //  if ( NewMirDlgBoxWrapper->Execute() )
 //  {

        DrawGrid1->Visible = false;
        DrawGrid1->Visible = true;
        ScrollBar1->Position = 0;

        theSystem->Reset();

        //theSystem->theMirror->SetDesigner(OKBottomDlg->Edit1->Text);
        //theSystem->theMirror->SetName(OKBottomDlg->Edit2->Text);
        //theSystem->theMirror->SetConfigFile(OKBottomDlg->Edit3->Text);
        //StatusBar1->Panels->Items[0]->Text = theSystem->theMirror->GetDesigner();
        //StatusBar1->Panels->Items[1]->Text = theSystem->theMirror->GetName();
        StatusBar1->Panels->Items[0]->Text = theSystem->GetConfigFile();
  //  }
}
//---------------------------------------------------------------------------




void __fastcall TForm1::FileExitExecute(TObject *Sender)
{
   delete theSystem;
   Close();
}
//---------------------------------------------------------------------------




void __fastcall TForm1::FileOpenExecute(TObject *Sender)
{
   if (OpenDialog1->Execute())
   {
        theSystem->OpenConfiguration(OpenDialog1->FileName);
        StatusBar1->Panels->Items[0]->Text = theSystem->GetConfigFile();

   }

   DrawGrid1->Visible = false;
   DrawGrid1->Visible = true;

}
//---------------------------------------------------------------------------




void __fastcall TForm1::FileSaveExecute(TObject *Sender)
{

   theSystem->SaveConfiguration("No New File",0);

}
//---------------------------------------------------------------------------




void __fastcall TForm1::FileSaveAsExecute(TObject *Sender)
{
   // Save As ... a mirror configuration (NOT Save)
   if (SaveDialog1->Execute())
   {
        theSystem->SaveConfiguration(SaveDialog1->FileName,1);
        StatusBar1->Panels->Items[0]->Text = theSystem->GetConfigFile();
   }


}
//---------------------------------------------------------------------------




void __fastcall TForm1::ViewWireListExecute(TObject *Sender)
{

   Form4 = new TForm4(this);
   Form4->ShowModal();

   delete Form4;

}
//---------------------------------------------------------------------------




void __fastcall TForm1::Button1OnClick(TObject *Sender)
{
   theSystem->Actuate();
}
//---------------------------------------------------------------------------




void __fastcall TForm1::ShapeZernikeExecute(TObject *Sender)
{
   theSystem->theMirror->ZernikeShape();
   DrawGrid1->Visible = false;
   DrawGrid1->Visible = true;

}
//---------------------------------------------------------------------------




void __fastcall TForm1::ScrollBar2Change(TObject *Sender)
{

   theSystem->theMirror->SetBias(ScrollBar2->Position);

   ActuatorVoltage theBiasVoltage = theSystem->theMirror->GetBias();
   StaticText1->Caption = FormatFloat("0.000",theBiasVoltage);


   // redraw the mirror grid displaying new Actuator DACValue settings
   DrawGrid1->Visible = false;
   DrawGrid1->Visible = true;

}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// DrawGrid1_OnSelectCell()
//
// Event Handler for OnSelectCell event of DrawGrid1.
//
// When user selects a cell, a droplet (subregion of actuators) is
// activated on the display, and the corresponding DAC values are set
// in the hardware system.
//
//---------------------------------------------------------------------------

void __fastcall TForm1::DrawGrid1_OnSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
    int theActuatorDACValue=Form1->ScrollBar1->Position;

    // set voltage of all actuators in the MEMSDevice to zero;
    // erase the corresponding GridCell elements on the display.
    theSystem->theMirror->CommandToZero(Sender);

    // set voltage of actuators in the droplet to their desired
    // values, set the corresponding GridCell elements on the display
    // to the appropriate color.
    theMicroDroplet->Command(Sender,ARow,ACol,theActuatorDACValue);


}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// UpdateDrawGridCell()
//
// Calls the DrawGridCell OnDrawGrid event handler to update the DrawGrid
// with the appropriate cell color, to match the voltage of the corresponding
// actuator.
//
// arguments:  inGridCellColumn, inGridCellRow
//      the column, row of the actuator (also column, row of the drawgrid
//      cell corresponding to the actuator)
//
// called by:  TForm1::DrawGrid1_OnSelectCell()
//---------------------------------------------------------------------------
void TForm1::UpdateDrawGridCell(TObject *Sender,
                                int inGridCellColumn,
                                int inGridCellRow)
{
    long theGridCellLeftPixelColumn, theGridCellTopPixelRow;
    long theGridCellRowSpacing = DrawGrid1->DefaultRowHeight;
    long theGridCellColumnSpacing = DrawGrid1->DefaultColWidth;

    theGridCellTopPixelRow = inGridCellRow*DrawGrid1->DefaultRowHeight
                               + DrawGrid1->GridLineWidth*inGridCellRow;
    theGridCellLeftPixelColumn = inGridCellColumn*DrawGrid1->DefaultColWidth
                               + DrawGrid1->GridLineWidth*inGridCellColumn;

    TRect theNeighborRect(theGridCellLeftPixelColumn,
                          theGridCellTopPixelRow,
                          theGridCellLeftPixelColumn+theGridCellColumnSpacing,
                          theGridCellTopPixelRow+theGridCellRowSpacing);

    TGridDrawState theState;

    DrawGrid1DrawCell(Sender,
                      inGridCellColumn,
                      inGridCellRow,
                      theNeighborRect,
                      theState);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::EditMicroDropletActionExecute(TObject *Sender)
{
   // Execute the Edit...MicroDroplet dialog box.
   if (EditMicroDropletDialogBox->Execute())
   {
        theMicroDroplet->Reset(theMicroDropletDiameter);
   }
}
//---------------------------------------------------------------------------

