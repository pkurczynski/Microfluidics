//----------------------------------------------------------------------------
#ifndef Unit2H
#define Unit2H
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
//----------------------------------------------------------------------------
class TOKBottomDlg : public TForm
{
__published:        
	TButton *OKBtn;
	TButton *CancelBtn;
	TBevel *Bevel1;
        TStaticText *StaticText1;
        TStaticText *StaticText2;
        TStaticText *StaticText3;
        TEdit *Edit1;
        TEdit *Edit2;
        TEdit *Edit3;
private:
public:
	virtual __fastcall TOKBottomDlg(TComponent* AOwner);
        bool __fastcall TOKBottomDlg::Execute();


};
//----------------------------------------------------------------------------
extern PACKAGE TOKBottomDlg *OKBottomDlg;
//----------------------------------------------------------------------------
#endif    
