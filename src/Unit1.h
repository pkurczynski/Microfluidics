//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ImgList.hpp>
#include <ComCtrls.hpp>
#include <DBGrids.hpp>
#include <Menus.hpp>
#include <ActnList.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TDrawGrid *DrawGrid1;
        TStaticText *StaticText3;
        TStaticText *StaticText4;
        TStaticText *StaticText5;
        TStaticText *StaticText6;
        TButton *Button1;
        TStatusBar *StatusBar1;
        TMainMenu *MainMenu1;
        TMenuItem *File1;
        TMenuItem *New1;
        TMenuItem *Open1;
        TMenuItem *Save1;
        TMenuItem *SaveAs1;
        TMenuItem *Exit1;
        TActionList *ActionList1;
        TAction *FileNew;
        TAction *FileOpen;
        TAction *FileSave;
        TAction *FileSaveAs;
        TAction *FileExit;
        TOpenDialog *OpenDialog1;
        TSaveDialog *SaveDialog1;
        TStaticText *StaticText7;
        TStaticText *StaticText8;
        TAction *ViewWireList;
        TMenuItem *View1;
        TMenuItem *WireList1;
        TScrollBar *ScrollBar1;
        TMemo *Memo1;
        TMenuItem *Zernike1;
        TAction *ShapeZernike;
        TGroupBox *GroupBox1;
        TGroupBox *GroupBox2;
        TStaticText *StaticText1;
        TScrollBar *ScrollBar2;
        TGroupBox *GroupBox3;
        TStaticText *StaticText2;
        TStaticText *StaticText9;
        TStaticText *StaticText10;
        TStaticText *StaticText11;
        TStaticText *StaticText12;
        TStaticText *StaticText13;
        TStaticText *StaticText14;
        TMenuItem *Edit1;
        TStaticText *StaticText15;
        TStaticText *StaticText16;
        TMenuItem *Droplet1;
        TAction *EditMicroDropletAction;

        void __fastcall DrawGrid1DrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall ScrollBar1Change(TObject *Sender);
        void __fastcall FileNewExecute(TObject *Sender);
        void __fastcall FileExitExecute(TObject *Sender);
        void __fastcall FileOpenExecute(TObject *Sender);
        void __fastcall FileSaveExecute(TObject *Sender);
        void __fastcall FileSaveAsExecute(TObject *Sender);
        void __fastcall ViewWireListExecute(TObject *Sender);

        void __fastcall Button1OnClick(TObject *Sender);
        void __fastcall ShapeZernikeExecute(TObject *Sender);
        void __fastcall ScrollBar2Change(TObject *Sender);
        void __fastcall DrawGrid1_OnMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall DrawGrid1_OnSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
        void __fastcall EditMicroDropletActionExecute(TObject *Sender);




private:	// User declarations
       TColor GetCellColor(int inColumn, int inRow);


public:		// User declarations
        int theMicroDropletDiameter;

        __fastcall TForm1(TComponent* Owner);

        void UpdateDrawGridCell(TObject *Sender,
                                int inGridCellColumn,
                                int inGridCellRow);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
