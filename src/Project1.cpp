//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("Project1.res");
USEFORM("Unit1.cpp", Form1);
USEUNIT("MEMSDevice.cpp");
USEUNIT("Actuator.cpp");
USEUNIT("SubscriptRangeError.cpp");
USEFORM("Unit2.cpp", OKBottomDlg);
USEFORM("Unit3.cpp", DataModule3); /* TDataModule: File Type */
USEFORM("Unit4.cpp", Form4);
USEUNIT("MEMSystem.cpp");
USEUNIT("VMEmethods.cpp");
USELIB("BIT3_API.lib");
USEUNIT("MicroDroplet.cpp");
USEFORM("EditMicroDropletDialog.cpp", EditMicroDropletDialogBox);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TForm1), &Form1);
                 Application->CreateForm(__classid(TOKBottomDlg), &OKBottomDlg);
                 Application->CreateForm(__classid(TDataModule3), &DataModule3);
                 Application->CreateForm(__classid(TEditMicroDropletDialogBox), &EditMicroDropletDialogBox);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
