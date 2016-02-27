//#include <vcl.h>  // for MessageDlg
//#include "Unit1.h"
#include "lucentVMEmethodsxfer.h"


// VME descriptors
bt_desc_t btd;                      // Bit 3 descriptor (pointer to struct) returned from bt_open
bt_desc_t hvdacbtd;                      // Bit 3 descriptor (pointer to struct) returned from bt_open
bt_desc_t vmicbtd;                  // VMIC Bit 3 descriptor returned from btopen
bt_desc_t pasbtd;                   // PAS Bit 3 descriptor returned from btopen
bt_desc_t adc3123btd;                   // PAS Bit 3 descriptor returned from btopen

// VME base addresses
void * base_a16;                    // Void pointer to VME space
void * base_a24;                    // Void pointer to VME space, used for standard addressing of VMIC
void * hvdac_base;                    // Void pointer to VME space, used for standard addressing of VMIC
void * pas_base;                    // Void pointer to VME space, used for standard addressing of VMIC
void * adc3123_base;                    // Void pointer to VME space, used for standard addressing of VMIC
bt_error_t hw_status = BT_SUCCESS;  // Bit 3 enum return value
int btunit = BT_DEF_UNIT;           // BT Unit Number

//int hvdac;    // defines the type of HVDAC used




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



int VMECtrl(void)
{
char devname[BT_MAX_DEV_NAME];      // place for device name
bt_dev_t bttype = BT_DEV_A16;       // BT Logical device type

bt_info_t    paramSwap       = BT_INFO_SWAP;   /* parameter to get/set */
const char   *infoname_pSwap = "BT_INFO_SWAP"; /* name of the info parameter */
bt_info_t    paramDma       = BT_INFO_DMA_THRESHOLD;   /* parameter to get/set */
const char   *infoname_pDma = "BT_INFO_DMA_THRESHOLD"; /* name of the info parameter */
bt_devdata_t value;                        /* parameters' value */

    msdelay(0);    // or else bt_gen_name fails with devname = nonsense

    // Open the Controller, establish descriptor and ck status
    hw_status = bt_open(&btd,bt_gen_name(btunit,bttype,devname,BT_MAX_DEV_NAME),BT_RDWR);
    if (hw_status != BT_SUCCESS)
        printf("Bt_Open Error = ");
    // Memory Map the VME adapter
    hw_status = bt_mmap(btd,&base_a16,SBSBT_LA,SBSBT_ML,BT_RDWR,BT_SWAP_DEFAULT);
    if (hw_status != BT_SUCCESS)
    {
        if (hw_status==BT_EUNKNOWN_REMID)
        {
            printf("Crate is OFF!  Turn on and Retry");
        }
        printf("bt_mmap Error = Controller");
    }

    // Open the HVDAC, establish descriptor and ck status
    hw_status = bt_open(&hvdacbtd,bt_gen_name(btunit,BT_DEV_A24,devname,BT_MAX_DEV_NAME),BT_RDWR);
    if (hw_status != BT_SUCCESS)
        printf("Bt_Open Error = ");
    // Memory Map the VME adapter
//const bt_devaddr_t  HVDAC_LA    = 0x0;          // Bit 3 VME Logical Address
//const size_t        HVDAC_ML    = 0x1000000;      // Bit 3 VME Map Length
    //hw_status = bt_mmap(hvdacbtd,&hvdac_base,HVDAC_LA,HVDAC_ML,BT_RDWR,BT_SWAP_DEFAULT);
	//hw_status = bt_mmap(hvdacbtd,&hvdac_base,HVDAC_LA,0x0100000,BT_RDWR,BT_SWAP_DEFAULT);

/*****************************************************************************
**
**  Swapping values.
**
**  Not all are supported on each platform.
**
**  Use with SWAP_BITS parameter (BIOC_PARAM & BIOC_DEV_ATTRIB)
**

typedef enum BT_SWAP_BITS_VALUE {
    BT_SWAP_NONE = 0,   /* No swapping 
    BT_SWAP_BSNBD,      /* Byte swap, non byte data 
    BT_SWAP_WS,         /* Word swap 
    BT_SWAP_WS_BSNBD,   /* Word swap and byte swap, non btye data 
    BT_SWAP_BSBD,       /* Byte swap, btye data 
    BT_SWAP_BSBD_BSNBD, /* Btye swap, byte data and byte swap, non byte data 
    BT_SWAP_BSBD_WS,    /* Byte swap, byte data and word swap 
    BT_SWAP_BSBD_WS_BSNBD,  /* All possible swapping 
    BT_SWAP_DEFAULT,    /* Driver default swapping 
    BT_MAX_SWAP
} bt_swap_t;

*****************************************************************************/

	hw_status = bt_mmap(hvdacbtd,&hvdac_base,HVDAC_LA,0x0100000,BT_RDWR,BT_SWAP_NONE);

    if (hw_status != BT_SUCCESS)
    {
        if (hw_status==BT_EUNKNOWN_REMID)
        {
            printf("Crate is OFF!  Turn on and Retry");
        }
        printf("\n\n********* bt_mmap Error = HVDAC\n\n");
    }




/******************************/
		//// set the info parameter
		value = BT_SWAP_NONE;

        hw_status = bt_set_info(hvdacbtd, paramSwap, value);		
        if (BT_SUCCESS != hw_status) {
            bt_perror(hvdacbtd, hw_status, "Could not set info for the device");
			//don't exit here- we still need to close the device 
			//main_ret = EXIT_FAILURE;
        } else {
            //printf ("%s was set to 0x%lX.\n",  infoname_pSwap, (unsigned long) value);
        }   

		/************
		value = (bt_devdata_t)0x40;

        hw_status = bt_set_info(hvdacbtd, paramDma, value);		
        if (BT_SUCCESS != hw_status) {
            bt_perror(hvdacbtd, hw_status, "Could not set info for the device");
			//don't exit here- we still need to close the device 
			//main_ret = EXIT_FAILURE;
        } else {
            printf ("%s was set to 0x%lX.\n",  infoname_pDma, (unsigned long) value);
        }
		**********/
/***************************/

/****************************/
        ///// get the info parameter 
        hw_status = bt_get_info(hvdacbtd, paramSwap, &value);
        if (BT_SUCCESS != hw_status) {
            bt_perror(hvdacbtd, hw_status, "Could not get info for the device");
            //don't exit here- we still need to close the device 
			//main_ret = EXIT_FAILURE;
        } else {
            //printf ("Value of %s is %lu (0x%lX)\n", infoname_pSwap,
            //(unsigned long) value, (unsigned long) value);
        } 

        hw_status = bt_get_info(hvdacbtd, paramDma, &value);
        if (BT_SUCCESS != hw_status) {
            bt_perror(hvdacbtd, hw_status, "Could not get info for the device");
            //don't exit here- we still need to close the device 
			//main_ret = EXIT_FAILURE;
        } else {
            //printf ("Value of %s is %lu (0x%lX)\n", infoname_pDma,
            //(unsigned long) value, (unsigned long) value);
        } 
/********************/




//BT_ENOMEM
/*******************
    //// These are not used in the test program
    // Open up the 24 bit VMIC address device
    hw_status = bt_open(&vmicbtd,bt_gen_name(btunit,BT_DEV_A24,devname,BT_MAX_DEV_NAME),BT_RDWR);
    if (hw_status != BT_SUCCESS)
        printf("Bt_Open Error = ");

    // Get Memory Mapped Pointer to the 24 bit VMIC-ADC address device
    hw_status = bt_mmap(vmicbtd,&base_a24,VMIC_ADCLA,VMIC_ML,BT_RDWR,BT_SWAP_DEFAULT);
    if (hw_status != BT_SUCCESS)
        printf("bt_mmap Error = ");

    // Open up the 24 bit PAS address device
    hw_status = bt_open(&pasbtd,bt_gen_name(btunit,BT_DEV_A24,devname,BT_MAX_DEV_NAME),BT_RDWR);
    if (hw_status != BT_SUCCESS)
        printf("Bt_Open Error = ");

    // Get Memory Mapped Pointer to the 24 bit PAS-ADC address device
    hw_status = bt_mmap(pasbtd,&pas_base,PAS_ADCLA,PAS_ML,BT_RDWR,BT_SWAP_DEFAULT);
    if (hw_status != BT_SUCCESS)
        printf("bt_mmap Error = ");

    // Open up the 24 bit 3123 address device
    hw_status = bt_open(&adc3123btd,bt_gen_name(btunit,BT_DEV_A16,devname,BT_MAX_DEV_NAME),BT_RDWR);
    if (hw_status != BT_SUCCESS)
        printf("Bt_Open Error = ");
    // Get Memory Mapped Pointer to the 16 bit VMIC3123-ADC address device
    hw_status = bt_mmap(adc3123btd,&adc3123_base,ADC3123_LA,ADC3123_ML,BT_RDWR,BT_SWAP_DEFAULT);
    if (hw_status != BT_SUCCESS)
        printf("bt_mmap Error = ");
*************/
    return(0);
}
//---------------------------------------------------------------------------


/* ============================== bt_cleanup ==================================
bt_cleanup
    Unmap the VME memory mapped locations for
    SBS-Bit3, VMIC, PAS_ADC
=========================================================================== */
void bt_cleanup(void)
{
    bt_unmmap(btd,base_a16,SBSBT_ML);       // SBS controller
    if (hw_status != BT_SUCCESS)
        printf("Bt_ Error = ");
    bt_unmmap(hvdacbtd,hvdac_base,HVDAC_ML);       // HVDACS controller
    if (hw_status != BT_SUCCESS)
        printf("Bt_ Error = ");
/*************
    bt_unmmap(vmicbtd,base_a24,VMIC_ML);    // VMIC ADC
    if (hw_status != BT_SUCCESS)
        printf("Bt_ Error = ");
    bt_unmmap(pasbtd,pas_base,PAS_ML);      // PAS ADC
    if (hw_status != BT_SUCCESS)
        printf("Bt_ Error = ");
    bt_unmmap(adc3123btd,adc3123_base,ADC3123_ML);      // PAS ADC
    if (hw_status != BT_SUCCESS)
        printf("Bt_ Error = ");
***********/
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
int DACReset(void)
{

long crate,mods,chips,dacs,clow,chi;
bt_devaddr_t dacaddr,ledaddr;
size_t num_w, anum_w;   // unsigned int
bt_data16_t dacdata,led;
unsigned int lstatus,sn,sn1;
int cnt = 0;
DWORD startTime, endTime;
float elapsedTime;
unsigned char buff[16];
int i;
bt_devaddr_t newdac;
volatile bt_data16_t *pdacaddr;     // unsigned short
bt_data8_t *vmebase;                   // unsigned char


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
		vmebase = (bt_data8_t *)hvdac_base;  // need character pointer from void pointer - needed for mmap

		for(i=0; i<16; i++) {
			buff[i] = 0x10;
		}

        // HVDACs
        bt_clrerr(hvdacbtd);
        
		for(crate=0;crate<1;crate++)
        {
		
            for (mods=0;mods<MHVDAC_MODS;++mods)
            //for (mods=2;mods<3;++mods)   
            {
                led = (bt_data16_t )GREEN_LED;
				//led = (bt_data16_t )0x0001;
                ledaddr = HVDAC_SEL|(crate<<19)|(mods<<7)|LED_OP;                       // green led
                lstatus = bt_write(hvdacbtd,&led,ledaddr,num_w,&anum_w);   // set LED
				if (lstatus != BT_SUCCESS)
					printf("\n*********Bt_Write Error*******\n");
				else
					//printf("\n* LED GREEN *\n");
				
				//printf("\n* LED GREEN SET *\n");

				startTime = GetTickCount();
		
				i = 0;
                for (chips=0;chips<MHVDAC_CHIPS;++chips)
                {
                    for (dacs=0;dacs<MHVDAC_DACS;++dacs)
                    {
                        clow = chips&0x07;
                        chi = chips&(0x038);
                        dacaddr = HVDAC_SEL|(crate<<19)|(chi<<13)|(mods<<7)|(clow<<4)|(dacs<<1);
						/***********
						lstatus = bt_write(hvdacbtd,(char *)buff,dacaddr,sizeof(buff),&anum_w); 
						*************/
                        //lstatus = bt_write(hvdacbtd,&dacdata,dacaddr,num_w,&anum_w); // set DAC ADDR (LOAD_REG)
                        //printf("\n%d\tLOAD_REG address = %ld", i++, (long)dacaddr);
						//lstatus |= bt_write(hvdacbtd,&dacdata,dacaddr|SET_DAC,num_w,&anum_w);    // Set DAC

                        //printf("\tSET_DAC address = %ld", (long)(dacaddr|SET_DAC));
						//lstatus |= bt_write(hvdacbtd,&dacdata,dacaddr|SET_CLEAR_HIGH,num_w,&anum_w); //
						//if (lstatus != BT_SUCCESS)
							//printf("\n*********Bt_Write Error*******\n");
						// Load the Data Register
						//newdac = dacaddr|LOAD_REG;
						//pdacaddr = (volatile bt_data16_t *)(vmebase+newdac);
						//printf("\n  \tLOAD_REG address = %ld", (long)pdacaddr);
						//*pdacaddr = (bt_data16_t )dacdata;
						bt_write(hvdacbtd,&dacdata,dacaddr|LOAD_REG,num_w,&anum_w);

						//newdac = dacaddr|SET_DAC;
						//pdacaddr = (volatile bt_data16_t *)(vmebase+newdac);
						//printf("\tSET_DAC address = %ld", (long)pdacaddr);
						//*pdacaddr = (bt_data16_t )dacdata;
						
						//bt_write(hvdacbtd,&dacdata,dacaddr|SET_CLEAR_HIGH,num_w,&anum_w);
						
                    }
                }
                //for(mods=0; mods<4; mods++)
                SetAllDACS(mods);   //transfer all Dacs in module and enable outputs //jao 

				//endTime = GetTickCount();
				//elapsedTime = (float)((endTime - startTime) * 0.001);
				//printf("\nelapsedTime = %f seconds  ", elapsedTime);

/*********************************/
				//getchar();
				//printf("\n* LED RED SET *\n");
                led = RED_LED;      // Red LED data   // ** this is accomplished in DAC_Ctrl
                lstatus |= bt_write(hvdacbtd,&led,ledaddr,num_w,&anum_w);     // Turn the Red LED on

/***********************************/
            }
        }
    }

    if (lstatus != BT_SUCCESS)
        printf("Bt_Write Error = ");

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

/********************************************/
int SetDAC(long crate,long mod,long chip,long dac,bt_data16_t dacdata,int delay)
{
bt_devaddr_t dacaddr,newdac;
volatile bt_data16_t *pdacaddr;     // unsigned short
bt_data8_t *vmebase;                   // unsigned char
char str[64];
long i,chi,clow;
size_t num_w, anum_w;   // unsigned int
bt_data16_t tdacdata;


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
        //pdacaddr = (volatile bt_data16_t *)(vmebase+newdac);
        //*pdacaddr = (bt_data16_t )GREEN_LED;
        tdacdata = (bt_data16_t )GREEN_LED;
        bt_write(hvdacbtd,&tdacdata,dacaddr|LED_OP,num_w,&anum_w);

       // Load the Data Register
        newdac = dacaddr|LOAD_REG;
        //pdacaddr = (volatile bt_data16_t *)(vmebase+newdac);
        //*pdacaddr = (bt_data16_t )dacdata;
        bt_write(hvdacbtd,&dacdata,dacaddr|LOAD_REG,num_w,&anum_w);
   


        newdac = dacaddr|SET_DAC;
        //pdacaddr = (volatile bt_data16_t *)(vmebase+newdac);
        //*pdacaddr = (bt_data16_t )dacdata;
        bt_write(hvdacbtd,&dacdata,dacaddr|SET_DAC,num_w,&anum_w);
        


        newdac = dacaddr|LED_OP;
        //pdacaddr = (volatile bt_data16_t *)(vmebase+newdac);
        if(delay>0) msdelay(delay);
        //*pdacaddr = (bt_data16_t )RED_LED;
        tdacdata = (bt_data16_t )RED_LED;
        bt_write(hvdacbtd,&tdacdata,dacaddr|LED_OP,num_w,&anum_w);

        // Check the Status  ***********  Calls need to return DTACK **************
        hw_status = bt_chkerr(hvdacbtd);
        if (hw_status != BT_SUCCESS)
        {
            bt_clrerr(hvdacbtd);
            printf("Bt_Write Error = ");
        }
        //hw_status = BT_SUCCESS; // Until hardware is fixed

        //LRFineTMainF->AddrLab->Caption = IntToHex((bt_data16_t )*pdacaddr,6);
        //LRFineTMainF->Refresh();
    //return(dacaddr);
    return(hw_status);
}

/********************************************/


int SetAllDACS(long mod)
{
	bt_devaddr_t dacaddr,newdac;
	volatile bt_data16_t *pdacaddr;     // unsigned short
	bt_data8_t *vmebase;                   // unsigned char
	char str[64];
	long chi,clow;
	size_t num_w, anum_w;   // unsigned int
	bt_data16_t tdacdata;
	
	long crate,chip,dac;
	int delay;
	unsigned short dacdata;
	
	crate = 0;
	chip = 0;
	dac = 0;
	dacdata = 0;
	delay = 8;

      // HVDAC
        //bt_clrerr(hvdacbtd);
        // Set up the DAC channel address
        num_w = 2;
        clow = (chip&0x7)<<4;
        chi = (chip&0x38)<<13;
        crate = crate<<19;
        dacaddr = (crate)|(chi)|(mod<<7)|(clow)|(dac<<1)|(HVDAC_SEL);
		   
        //newdac = dacaddr|LED_OP;
 
        tdacdata = (bt_data16_t )GREEN_LED;
        bt_write(hvdacbtd,&tdacdata,dacaddr|LED_OP,num_w,&anum_w);

       // Xfer all dac data to outputs
        //newdac = dacaddr|LOAD_REG;
        bt_write(hvdacbtd,&dacdata,dacaddr|SET_ALL_DACS,num_w,&anum_w);
        bt_write(hvdacbtd,&dacdata,dacaddr|SET_CLEAR_HIGH,num_w,&anum_w);
   

        //newdac = dacaddr|LED_OP;
     
        if(delay>0) msdelay(delay);
        
        tdacdata = (bt_data16_t )RED_LED;
        bt_write(hvdacbtd,&tdacdata,dacaddr|LED_OP,num_w,&anum_w);

        // Check the Status  ***********  Calls need to return DTACK **************
        hw_status = bt_chkerr(hvdacbtd);
        if (hw_status != BT_SUCCESS)
        {
            bt_clrerr(hvdacbtd);
            printf("Bt_Write Error = ");
        }

    return(hw_status);
}

int SetDacMap(long crate,long mod,long chip,long dac,unsigned short dacdata,int delay, int actuator)
{

extern  short int modarray[];
extern  chiparray[];
extern  dacchannel[];
extern  dacdataarray[];
long i,chi,clow;
size_t num_w, anum_w;
bt_devaddr_t dacaddr;
bt_data16_t tdacdata;

			bt_clrerr(hvdacbtd);

			//for(actuator=0; actuator <1024; actuator++){
				mod = modarray[actuator];
                chip = chiparray[actuator];
                dac = dacchannel[actuator];
				//dacdata = dacdataarray[actuator];
				//}

        num_w = 2;
        clow = (chip&0x7)<<4;
        chi = (chip&0x38)<<13;
        crate = crate<<19;
        dacaddr = (crate)|(chi)|(mod<<7)|(clow)|(dac<<1)|(HVDAC_SEL);
        //vmebase = (bt_data8_t *)hvdac_base;  // need character pointer from void pointer - needed for mmap     
        
		tdacdata = (bt_data16_t )GREEN_LED;
        bt_write(hvdacbtd,&tdacdata,dacaddr|LED_OP,num_w,&anum_w);	   //set LED
        bt_write(hvdacbtd,&dacdata,dacaddr|LOAD_REG,num_w,&anum_w);		//load reg
        bt_write(hvdacbtd,&dacdata,dacaddr|SET_DAC,num_w,&anum_w);		// set to output
        
        if(delay>0) msdelay(delay);
        
        tdacdata = (bt_data16_t )RED_LED;
        bt_write(hvdacbtd,&tdacdata,dacaddr|LED_OP,num_w,&anum_w);

        // Check the Status  ***********  Calls need to return DTACK **************
        hw_status = bt_chkerr(hvdacbtd);
        if (hw_status != BT_SUCCESS)
        {
            bt_clrerr(hvdacbtd);
            printf("Bt_Write Error = ");
        }
		
				
	return(hw_status);
}
//---------------------------------------------------------------------------



/* ============================== msdelay ==================================
Delay function
    tcnt: in msec

=========================================================================== */
int msdelay(long int tcnt)
{
long int tic1,tic2;
long it,ft,ct;

/*
    it = ct = K3140F->Get32Cnt();
    ft = it+(tcnt*10);
    while(ft>ct)
    {
        ct = K3140F->Get32Cnt();
        Application->ProcessMessages();
    }
    return(0);

*/
    tic2 = 0;
    tic1 = GetTickCount()+tcnt;   // in msec
    while(tic2<tic1)
    {
        tic2 = GetTickCount();
    }
    return(0);

}
//---------------------------------------------------------------------------


/***************
int main(
    int argc, 
    char **argv

    )
{
	printf("\nVMECtrl**********\n");
	VMECtrl();

	printf("\nDACReset**********\n");
	DACReset();

	printf("\nbt_cleanup**********\n");
	bt_cleanup();

}
*************/

 
