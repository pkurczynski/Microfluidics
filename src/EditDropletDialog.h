//----------------------------------------------------------------------------
#ifndef EditDropletDialogH
#define EditDropletDialogH
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
class TEditDropletDialogBox : public TForm
{
__published:        
	TButton *OKBtn;
	TButton *CancelBtn;
	TBevel *Bevel1;
        TStaticText *DropletDiameterStaticText;
        TEdit *DropletDiameterEditBox;
private:
public:
	virtual __fastcall TEditDropletDialogBox(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TEditDropletDialogBox *EditDropletDialogBox;
//----------------------------------------------------------------------------
#endif    
