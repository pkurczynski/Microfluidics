//---------------------------------------------------------------------------

#ifndef Unit5H
#define Unit5H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TViewActuatorForm : public TForm
{
__published:	// IDE-managed Components
        TStaticText *StaticText1;
        TStaticText *StaticText2;
        TStaticText *StaticText3;
        TStaticText *StaticText4;
        TStaticText *StaticText5;
        TStaticText *StaticText6;
        TEdit *Edit1;
        TEdit *Edit2;
        TEdit *Edit3;
        TEdit *Edit4;
        TEdit *Edit5;
        TEdit *Edit6;
        TEdit *Edit7;
        TEdit *Edit8;
        TEdit *Edit9;
        TStaticText *StaticText7;
        TStaticText *StaticText8;
        TStaticText *StaticText9;
        TEdit *Edit10;
        TEdit *Edit11;
        TEdit *Edit12;
        TEdit *Edit13;
        TStaticText *StaticText10;
        TStaticText *StaticText11;
        TStaticText *StaticText12;
        TStaticText *StaticText13;
private:	// User declarations
public:		// User declarations
        __fastcall TViewActuatorForm(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TViewActuatorForm *ViewActuatorForm;
//---------------------------------------------------------------------------
#endif
