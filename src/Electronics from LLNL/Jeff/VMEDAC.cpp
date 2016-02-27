//------------------------------------------------------------------------
// VMEDAC.cpp
//
//
//------------------------------------------------------------------------
//#include "Unit1.h"
#include "MEMSystem.h"
#include "VMEDAC.h"

VMEDAC::VMEDAC()
{

#if 0
    hw_status = BT_SUCCESS;         // Bit 3 enum return value
    btunit = BT_DEF_UNIT;           // BT Unit Number
#endif



    Form1->Memo1->Lines->Add("Created a VMEDAC");

#if 0
    int rarg;

    while((rarg=VMECtrl())==mrRetry);
    if (rarg == mrAbort)
        exit(0);
    lpmemo->Lines->Add("VMECtrl Complete");
    lpmemo->Refresh();
    DACReset();
    lpmemo->Lines->Add("DAC Reset Complete");
    lpmemo->Refresh();

    if(hvdac == HVDAC16)
    {
        // these lines disabled b/c new UI plk 11/29/00
        //CrateUD->Visible = false;
        //CrateEB->Visible = false;
        //CrateLab->Visible = false;
    }

#endif

}

VMEDAC::~VMEDAC()
{
}


// EVERYTHING ELSE IN THIS FILE IS DISABLED
#if 0


/* ============================== VmeCtrl ==================================
    Open the VME Device
    Memory Map the device using a void pointer
    *** delay is put in to avoid devname getting trash ??
        bt_gen_name uses (const char *) for devname
    bt_gen_name     Generate Device Name pointer
        btUnit:     BT_DEF_UNIT
        BT_DEV_A24: Logical device type
        devname     storage for device name
        BT_MAX_DEV_NAME storage size
    bt_open         Open the specified device,  returns a descriptor
    bt_mmap     Get a memory mapped pointer to the address space
        vmicbtd:    descriptor from bt_open
        base_a24:   address of the pointer to the logical device. Set by bt_mmap
    Devices Opened:
    SBS-Bit3 Controller
    VME - VMIC3120 12 ADC
    PAS-9731 ADC
=========================================================================== */
int __fastcall VMEDAC::VMECtrl(void)
{
char devname[BT_MAX_DEV_NAME];      // place for device name
bt_dev_t bttype = BT_DEV_A16;       // BT Logical device type

    theSystem->msdelay(0);    // or else bt_gen_name fails with devname = nonsense

    // Open the Controller, establish descriptor and ck status
    hw_status = bt_open(&btd,bt_gen_name(btunit,bttype,devname,BT_MAX_DEV_NAME),BT_RDWR);
    if (hw_status != BT_SUCCESS)
        MessageDlg("Bt_Open Error = "+IntToStr(hw_status)+"  "+devname, mtError,TMsgDlgButtons() << mbOK, 0);
    // Memory Map the VME adapter
    hw_status = bt_mmap(btd,&base_a16,SBSBT_LA,SBSBT_ML,BT_RDWR,BT_SWAP_DEFAULT);
    if (hw_status != BT_SUCCESS)
    {
        if (hw_status==BT_EUNKNOWN_REMID)
        {
            if (MessageDlg("Crate is OFF!  Turn on and Retry", mtConfirmation,TMsgDlgButtons() <<
                mbRetry<<mbAbort, 0)==mrRetry)
                return(mrRetry);
            else    return(mrAbort);
        }
        MessageDlg("bt_mmap Error = "+IntToStr(hw_status), mtError,TMsgDlgButtons() << mbOK, 0);
    }

    // Open the HVDAC, establish descriptor and ck status
    hw_status = bt_open(&hvdacbtd,bt_gen_name(btunit,BT_DEV_A24,devname,BT_MAX_DEV_NAME),BT_RDWR);
    if (hw_status != BT_SUCCESS)
        MessageDlg("Bt_Open Error = "+IntToStr(hw_status)+"  "+devname, mtError,TMsgDlgButtons() << mbOK, 0);
    // Memory Map the VME adapter
    hw_status = bt_mmap(hvdacbtd,&hvdac_base,HVDAC_LA,HVDAC_ML,BT_RDWR,BT_SWAP_DEFAULT);
    if (hw_status != BT_SUCCESS)
    {
        if (hw_status==BT_EUNKNOWN_REMID)
        {
            if (MessageDlg("Crate is OFF!  Turn on and Retry", mtConfirmation,TMsgDlgButtons() <<
                mbRetry<<mbAbort, 0)==mrRetry)
                return(mrRetry);
            else    return(mrAbort);
        }
        MessageDlg("bt_mmap Error = "+IntToStr(hw_status), mtError,TMsgDlgButtons() << mbOK, 0);
    }

    //// These are not used in the test program
    // Open up the 24 bit VMIC address device
    hw_status = bt_open(&vmicbtd,bt_gen_name(btunit,BT_DEV_A24,devname,BT_MAX_DEV_NAME),BT_RDWR);
    if (hw_status != BT_SUCCESS)
        MessageDlg("Bt_Open Error = "+IntToStr(hw_status), mtError,TMsgDlgButtons() << mbOK, 0);

    // Get Memory Mapped Pointer to the 24 bit VMIC-ADC address device
    hw_status = bt_mmap(vmicbtd,&base_a24,VMIC_ADCLA,VMIC_ML,BT_RDWR,BT_SWAP_DEFAULT);
    if (hw_status != BT_SUCCESS)
        MessageDlg("bt_mmap Error = "+IntToStr(hw_status), mtError,TMsgDlgButtons() << mbOK, 0);

    // Open up the 24 bit PAS address device
    hw_status = bt_open(&pasbtd,bt_gen_name(btunit,BT_DEV_A24,devname,BT_MAX_DEV_NAME),BT_RDWR);
    if (hw_status != BT_SUCCESS)
        MessageDlg("Bt_Open Error = "+IntToStr(hw_status), mtError,TMsgDlgButtons() << mbOK, 0);

    // Get Memory Mapped Pointer to the 24 bit PAS-ADC address device
    hw_status = bt_mmap(pasbtd,&pas_base,PAS_ADCLA,PAS_ML,BT_RDWR,BT_SWAP_DEFAULT);
    if (hw_status != BT_SUCCESS)
        MessageDlg("bt_mmap Error = "+IntToStr(hw_status), mtError,TMsgDlgButtons() << mbOK, 0);

    // Open up the 24 bit 3123 address device
    hw_status = bt_open(&adc3123btd,bt_gen_name(btunit,BT_DEV_A16,devname,BT_MAX_DEV_NAME),BT_RDWR);
    if (hw_status != BT_SUCCESS)
        MessageDlg("Bt_Open Error = "+IntToStr(hw_status), mtError,TMsgDlgButtons() << mbOK, 0);
    // Get Memory Mapped Pointer to the 16 bit VMIC3123-ADC address device
    hw_status = bt_mmap(adc3123btd,&adc3123_base,ADC3123_LA,ADC3123_ML,BT_RDWR,BT_SWAP_DEFAULT);
    if (hw_status != BT_SUCCESS)
        MessageDlg("bt_mmap Error = "+IntToStr(hw_status), mtError,TMsgDlgButtons() << mbOK, 0);

    return(0);
}
//---------------------------------------------------------------------------




/* ============================== bt_cleanup ==================================
bt_cleanup
    Unmap the VME memory mapped locations for
    SBS-Bit3, VMIC, PAS_ADC
=========================================================================== */
void VMEDAC::bt_cleanup(void)
{
    bt_unmmap(btd,base_a16,SBSBT_ML);       // SBS controller
    if (hw_status != BT_SUCCESS)
        MessageDlg("Bt_ Error = "+IntToStr(hw_status), mtError,TMsgDlgButtons() << mbOK, 0);
    bt_unmmap(hvdacbtd,hvdac_base,HVDAC_ML);       // HVDACS controller
    if (hw_status != BT_SUCCESS)
        MessageDlg("Bt_ Error = "+IntToStr(hw_status), mtError,TMsgDlgButtons() << mbOK, 0);
    bt_unmmap(vmicbtd,base_a24,VMIC_ML);    // VMIC ADC
    if (hw_status != BT_SUCCESS)
        MessageDlg("Bt_ Error = "+IntToStr(hw_status), mtError,TMsgDlgButtons() << mbOK, 0);
    bt_unmmap(pasbtd,pas_base,PAS_ML);      // PAS ADC
    if (hw_status != BT_SUCCESS)
        MessageDlg("Bt_ Error = "+IntToStr(hw_status), mtError,TMsgDlgButtons() << mbOK, 0);
    bt_unmmap(adc3123btd,adc3123_base,ADC3123_ML);      // PAS ADC
    if (hw_status != BT_SUCCESS)
        MessageDlg("Bt_ Error = "+IntToStr(hw_status), mtError,TMsgDlgButtons() << mbOK, 0);
}
//---------------------------------------------------------------------------


/*================================= VMEDACReset ================================
    Reset all the DAC chips to Output Active
    This is necessary after Power up
    This assumes BT_Open has been done in Form Open
    For all Channels (init and set to 0)
        Load data in DAC's = 0
        Set the DAC outputs
        SET_CLEAR_HIGH

ToDo:
    Handle status errors from modules that do not exist
    This could be done using mmap and not bt_write
============================================================================ */
int __fastcall VMEDAC::DACReset(void)
{
long crate,mods,chips,dacs,clow,chi;
bt_devaddr_t dacaddr,ledaddr;
size_t num_w, anum_w;   // unsigned int
bt_data16_t dacdata,led;
unsigned int lstatus,sn,sn1;
int cnt = 0;

    dacdata = 0;
    num_w = 2;

    ///  Old HVDACs
    if(hvdac == HVDAC16)
    {
        bt_clrerr(btd);
        for (mods=0;mods<MDAC_MODS;++mods)
        {
            led = GREEN_LED;
            ledaddr = (mods<<7)|LED_OP;                       // green led
            lstatus = bt_write(btd,&led,ledaddr,num_w,&anum_w);   // set LED
            for (chips=0;chips<MDAC_CHIPS;++chips)
            {
                for (dacs=0;dacs<MDAC_DACS;++dacs)
                {
                    dacaddr = (mods<<7)|(chips<<4)|(dacs<<1);       // set dac addr
                    lstatus = bt_write(btd,&dacdata,dacaddr,num_w,&anum_w); // set DAC ADDR
                    lstatus |= bt_write(btd,&dacdata,dacaddr|SET_DAC,num_w,&anum_w);    // Set DAC
                    lstatus |= bt_write(btd,&dacdata,dacaddr|SET_CLEAR_HIGH,num_w,&anum_w); //
                }
            }
/*
        if(lstatus==BT_SUCCESS)     // this mod appears to be good
        {
            do
            {
                sn = DAC_SN(mods);
                sn1 = DAC_SN(mods);
                if(sn!=sn1) cnt++;
                if(cnt>10)
                {
                    MessageDlg("DAC Serial No Error", mtError,TMsgDlgButtons() << mbOK, 0);
//                    exit(-1);
                }
            }while(sn!=sn1);
//            if(cnt != 0)
//                ShowMessage("cnt = "+IntToStr(cnt));
            ReadDACCFile(mods,sn);
            cnt = 0;
        }
        else if(lstatus!=BT_SUCCESS)     // this mod appears to be good
        {
            cnt++;
            bt_clrerr(btd);
        }
*/
            led = RED_LED;      // Red LED data   // ** this is accomplished in DAC_Ctrl
            lstatus |= bt_write(btd,&led,ledaddr,num_w,&anum_w);     // Turn the Red LED on
        }
    }

    else if(hvdac == HVDAC24)
    {

        // HVDACs
        bt_clrerr(hvdacbtd);
        for(crate=0;crate<2;crate++)
        {
            for (mods=0;mods<MHVDAC_MODS;++mods)
            {
                led = GREEN_LED;
                ledaddr = HVDAC_SEL|(crate<<19)|(mods<<7)|LED_OP;                       // green led
                lstatus = bt_write(hvdacbtd,&led,ledaddr,num_w,&anum_w);   // set LED
                for (chips=0;chips<MHVDAC_CHIPS;++chips)
                {
                    for (dacs=0;dacs<MHVDAC_DACS;++dacs)
                    {
                        clow = chips&0x07;
                        chi = chips&(0x038);
                        dacaddr = HVDAC_SEL|(crate<<19)|(chi<<13)|(mods<<7)|(clow<<4)|(dacs<<1);

                        lstatus = bt_write(hvdacbtd,&dacdata,dacaddr,num_w,&anum_w); // set DAC ADDR
                        lstatus |= bt_write(hvdacbtd,&dacdata,dacaddr|SET_DAC,num_w,&anum_w);    // Set DAC
                        lstatus |= bt_write(hvdacbtd,&dacdata,dacaddr|SET_CLEAR_HIGH,num_w,&anum_w); //
                    }
                }
                led = RED_LED;      // Red LED data   // ** this is accomplished in DAC_Ctrl
                lstatus |= bt_write(hvdacbtd,&led,ledaddr,num_w,&anum_w);     // Turn the Red LED on
            }
        }
    }

    if (lstatus != BT_SUCCESS)
        MessageDlg("Bt_Write Error = "+IntToStr(lstatus)+" : "+IntToStr(cnt), mtError,TMsgDlgButtons() << mbOK, 0);

    return(lstatus);
}
//---------------------------------------------------------------------------


/* =============================== SetDAC ====================================
    SetDAC: Set a single channel
    Args:
        mod: board number 0-31
        chip: DAC number 0-7
        dac: Channel number 0-7
        data: unsigned int: value to set dac
    Return:
        0: success
        1: error

        THis routine is used in the Maintenance Window
============================================================================ */
int __fastcall VMEDAC::SetDAC(long crate,long mod,long chip,long dac,bt_data16_t dacdata,int delay)
{
bt_devaddr_t dacaddr,newdac;
volatile bt_data16_t *pdacaddr;     // unsigned short
bt_data8_t *vmebase;                   // unsigned char
char str[64];
long i,chi,clow;
size_t num_w, anum_w;   // unsigned int
bt_data16_t tdacdata;

    if(hvdac == HVDAC16)
    {
        bt_clrerr(btd);
        // Set up the DAC channel address
        dacaddr = (mod<<7)|(chip<<4)|(dac<<1);
        vmebase = (bt_data8_t *)base_a16;  // need character pointer from void pointer - needed for mmap

        // Set the Green LED
        newdac = dacaddr|LED_OP;
        pdacaddr = (volatile bt_data16_t *)(vmebase+newdac);
        *pdacaddr = (bt_data16_t )GREEN_LED;

       // Load the Data Register
        newdac = dacaddr|LOAD_REG;
        pdacaddr = (volatile bt_data16_t *)(vmebase+newdac);
        *pdacaddr = (bt_data16_t )dacdata;
        newdac = dacaddr|SET_DAC;
        pdacaddr = (volatile bt_data16_t *)(vmebase+newdac);
        *pdacaddr = dacdata;

        // Set the Red LED
        newdac = dacaddr|LED_OP;
        pdacaddr = (volatile bt_data16_t *)(vmebase+newdac);
//        msdelay(100);
        *pdacaddr = (bt_data16_t )RED_LED;

        // Check the Status  ***********  Calls need to return DTACK **************
    //    hw_status = bt_chkerr(btd);
    //    if (hw_status != BT_SUCCESS)
        {
            bt_clrerr(btd);
    //        MessageDlg("Bt_Write Error = "+IntToStr(hw_status), mtError,TMsgDlgButtons() << mbOK, 0);
        }
    //    hw_status = BT_SUCCESS; // Until hardware is fixed
    //    return(dacaddr);
    }

    else if(hvdac == HVDAC24)
    {

      // HVDAC
        bt_clrerr(hvdacbtd);
        // Set up the DAC channel address
        num_w = 2;
        clow = (chip&0x7)<<4;
        chi = (chip&0x38)<<13;
        crate = crate<<19;
        dacaddr = (crate)|(chi)|(mod<<7)|(clow)|(dac<<1)|(HVDAC_SEL);
        vmebase = (bt_data8_t *)hvdac_base;  // need character pointer from void pointer - needed for mmap

        newdac = dacaddr|LED_OP;
        pdacaddr = (volatile bt_data16_t *)(vmebase+newdac);
        *pdacaddr = (bt_data16_t )GREEN_LED;
    //    tdacdata = (bt_data16_t )GREEN_LED;
    //    bt_write(hvdacbtd,&tdacdata,dacaddr|LED_OP,num_w,&anum_w);

       // Load the Data Register
        newdac = dacaddr|LOAD_REG;
        pdacaddr = (volatile bt_data16_t *)(vmebase+newdac);
        *pdacaddr = (bt_data16_t )dacdata;
    //    bt_write(hvdacbtd,&dacdata,dacaddr|LOAD_REG,num_w,&anum_w);

        newdac = dacaddr|SET_DAC;
        pdacaddr = (volatile bt_data16_t *)(vmebase+newdac);
        *pdacaddr = (bt_data16_t )dacdata;
    //    bt_write(hvdacbtd,&dacdata,dacaddr|SET_DAC,num_w,&anum_w);

        newdac = dacaddr|LED_OP;
        pdacaddr = (volatile bt_data16_t *)(vmebase+newdac);
        if(delay>0) theSystem->msdelay(delay);
        *pdacaddr = (bt_data16_t )RED_LED;
    //    tdacdata = (bt_data16_t )RED_LED;
    //    bt_write(hvdacbtd,&tdacdata,dacaddr|LED_OP,num_w,&anum_w);

        // Check the Status  ***********  Calls need to return DTACK **************
        hw_status = bt_chkerr(hvdacbtd);
        if (hw_status != BT_SUCCESS)
        {
            bt_clrerr(hvdacbtd);
            MessageDlg("Bt_Write Error = "+IntToStr(hw_status), mtError,TMsgDlgButtons() << mbOK, 0);
        }
        hw_status = BT_SUCCESS; // Until hardware is fixed

        // these lines disabled b/c new UI plk 11/29/00
        //LRFineTMainF->AddrLab->Caption = IntToHex((bt_data16_t )*pdacaddr,6);
        //LRFineTMainF->Refresh();
    }
    return(dacaddr);
}
//---------------------------------------------------------------------------


#endif


#if 0

// *************************************************************************
//
//
//                        USER INTERFACE METHODS FROM LRHV.cpp
//
// *************************************************************************


void __fastcall TLRFineTMainF::FormDestroy(TObject *Sender)
{
    bt_cleanup();
}
//---------------------------------------------------------------------------


/* =========================== WrDacSBChange ===============================
Write DAC Scroll bar change
    This gets the new value selected and displays it before it is activated
    by the Write Button

=========================================================================== */
void __fastcall TLRFineTMainF::WrDacSBChange(TObject *Sender)
{
    WrDacEB->Text = FormatFloat("0.0000",WrDacSB->Position*BL_DAC_VPB);
    WrHexDacEB->Text = WrDacSB->Position;
}
//---------------------------------------------------------------------------


/* =========================== WrDACButClick ===============================
Write DAC Button Click
    Read in the data from the Scroll bar and set the DAC

=========================================================================== */
void __fastcall TLRFineTMainF::WrDACButClick(TObject *Sender)
{
bt_data16_t dacdata;
bt_devaddr_t dacaddr;
//size_t num_w, anum_w;

    dacdata = (bt_data16_t)StrToInt(WrDacSB->Position);
    dacaddr = SetDAC(CrateUD->Position,BoardUD->Position,DACUD->Position,ChnlUD->Position,dacdata,0);
    DACAddrLab->Caption = "0x"+IntToHex((int)dacaddr,4);
    return;
}
//---------------------------------------------------------------------------


/* =========================== FormCreate ===============================
Form Create
    VMECtrl()
    Log comment to Memo

=========================================================================== */
void __fastcall TLRFineTMainF::FormCreate(TObject *Sender)
{
TMemo *lpmemo = LRFineTMainF->MainMsgMemo;
int rarg;

    while((rarg=VMECtrl())==mrRetry);
    if (rarg == mrAbort)
        exit(0);
    lpmemo->Lines->Add("VMECtrl Complete");
    lpmemo->Refresh();
    DACReset();
    lpmemo->Lines->Add("DAC Reset Complete");
    lpmemo->Refresh();

    if(hvdac == HVDAC16)
    {
        CrateUD->Visible = false;
        CrateEB->Visible = false;
        CrateLab->Visible = false;
    }
}
//---------------------------------------------------------------------------


/* =========================== WrAllDacButClick ===============================
Write all DAC channels of the selected Crate and Module
    Set all DACs to the current value

=========================================================================== */
void __fastcall TLRFineTMainF::WrAllDacButClick(TObject *Sender)
{
int crate,mod,chip,dac;
bt_data16_t dacdata;
bt_devaddr_t dacaddr;

    dacdata = (bt_data16_t)StrToInt(WrDacSB->Position);
    DACAddrLab->Caption = "0x"+IntToHex((int)dacaddr,4);
    crate = CrateUD->Position;
    mod = BoardUD->Position;
    if(hvdac==HVDAC16)
    {
        for(chip=0;chip<MDAC_CHIPS;chip++)
        {
            for(dac=0;dac<MDAC_DACS;dac++)
                SetDAC(crate,mod,chip,dac,dacdata,0);
        }

    }
    else if(hvdac==HVDAC24)
    {
    //    for(crate=0;crate<MHVDAC_CRATES;crate++)
        {
    //        for(mod=0;mod<MHVDAC_MODS;mod++)
            {
                for(chip=0;chip<MHVDAC_CHIPS;chip++)
                {
                    for(dac=0;dac<MHVDAC_DACS;dac++)
                        SetDAC(crate,mod,chip,dac,dacdata,0);
                }
            }
        }
    }
    MessageDlg("Write All Done ", mtInformation,TMsgDlgButtons() << mbOK, 0);
    return;
}
//---------------------------------------------------------------------------



void __fastcall TLRFineTMainF::AWrDacSBChange(TObject *Sender)
{
bt_data16_t dacdata;
bt_devaddr_t dacaddr;
//size_t num_w, anum_w;

    AWrDacEB->Text = FormatFloat("0.0000",AWrDacSB->Position*BL_DAC_VPB);
    AWrHexDacEB->Text = AWrDacSB->Position;

    dacdata = (bt_data16_t)StrToInt(AWrDacSB->Position);
    dacaddr = SetDAC(CrateUD->Position,BoardUD->Position,DACUD->Position,ChnlUD->Position,dacdata,0);
    DACAddrLab->Caption = "0x"+IntToHex((int)dacaddr,4);
    return;
}
//---------------------------------------------------------------------------

#endif