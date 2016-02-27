//----------------------------------------------------------------------------
#ifndef EditMicroDropletDialogH
#define EditMicroDropletDialogH
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
class TEditMicroDropletDialogBox : public TForm
{
__published:        
	TButton *OKBtn;
	TButton *CancelBtn;
	TBevel *Bevel1;
        TStaticText *DiameterStaticText;
        TEdit *DiameterEditBox;
private:

public:
	virtual __fastcall TEditMicroDropletDialogBox(TComponent* AOwner);
        bool __fastcall Execute();
};
//----------------------------------------------------------------------------
extern PACKAGE TEditMicroDropletDialogBox *EditMicroDropletDialogBox;
//----------------------------------------------------------------------------
#endif    
