//---------------------------------------------------------------------------
#ifndef VMEmethodsH
#define VMEmethodsH
//---------------------------------------------------------------------------
//#include <Classes.hpp>
//#include <Controls.hpp>
//#include <StdCtrls.hpp>
//#include <Forms.hpp>
//#include <ComCtrls.hpp>
#include <stdlib.h>
#include <stdio.h>

#define BT983               // Used in SBS-Bit3 code
#define T_HVDAC16       // Type of HVDAC used


#include "Btapi.h"   // Bit3 Header file
#include "Btdef.h"   // Bit3 Header file
//#include "decl-32.h"

#define BT_DEF_UNIT    0    // Default Unit
#define BT_DEF_ADDR    0    // Default address to be used

// HVDAC Defines
#define HVDAC16         16           // HVDAC that uses 16 bit address
#define HVDAC24         24           // HVDAC that uses 24 bit address
#define DAC_OUT_MOD     0x0800      // Output DAC Module offset  high addr (lowmodule+16)&0x1F)<<7)
#define	LOAD_REG		0x0         // Load Data Register
#define	SET_DAC 		0x1000      // Activate the DACs
#define	SET_ALL_DACS    0x2000      // Activate all DACs
#define	SET_CLEAR_LOW	0x3000      // Set DAC Outputs to zero, CLR Low
#define	SET_CLEAR_HIGH  0x4000      // Set DAC Outputs to High, CLR High
#define GREEN_LED       0x01        // DAC Green LED ON data bit
#define RED_LED         0x00        // DAC Red LED ON data bit
#define HVDAC_SEL       0x100000    // HVDAC Select code

// VME-DAC short A16 address space - OpCodes
const int   LED_OP          = 0xE000;       // DAC LED Opcode
const int   XSN_OP          = 0x8000;       // DAC OpCode:Transfer of Serial Numbe from EEProm to Register
const int   SNDUMP_OP       = 0xA000;       // DAC OpCode: Dump Serial Number Byte
const int   SNVIEW_OP       = 0xB000;       // DAC OpCode: Change Serial Number Byte View
const int   LEDVIEW_OP      = 0x9000;       // DAC OpCode: LED View
const int   ADC3123_IO_ADDR = 0xF000;     // VMIC 3123 short IO address
const int   DB0_H   = 0x0001;       // DAC Data Bit-0  High
const int   DB0_L   = 0x0000;       // DAC Data Bit-0  Low

// Old HVDAC constants
const int MDAC_MODS  =   32;        // Maximum number of modules (boards)
const int MDAC_CHIPS =   8;         // Maximum number of chips (DACs / Board)
const int MDAC_DACS  =   8;         // Maximum number of dacs (channels / DAC)

// New HVDAC constants
const int MHVDAC_CRATES =   2;        // Maximum number of crates
const int MHVDAC_MODS  =   18;        // Maximum number of modules (boards)
const int MHVDAC_CHIPS =   36;         // Maximum number of chips (DACs / Board)
const int MHVDAC_DACS  =   8;         // Maximum number of dacs (channels / DAC)

// VME Logical Addresses
const bt_devaddr_t  SBSBT_LA    = 0x0;          // Bit 3 VME Logical Address
const size_t        SBSBT_ML    = 0x10000;      // Bit 3 VME Map Length
const bt_devaddr_t  HVDAC_LA    = 0x0;          // Bit 3 VME Logical Address
const size_t        HVDAC_ML    = 0x1000000;      // Bit 3 VME Map Length
const bt_devaddr_t  VMIC_ADCLA  = 0xc00000;     // VMIC ADC VME Logical Address
const size_t        VMIC_ML     = 0x100000;     // VMIC ADC Map Length
const bt_devaddr_t  PAS_ADCLA   = 0xB00000;     // PAS ADC VME Logical Address
const size_t        PAS_ML      = 0x100;        // PAS ADC Map Length
const bt_devaddr_t  ADC3123_LA  = 0xF000;       // VMIC 3123 short IO address
const size_t        ADC3123_ML  = 0x100;        // Bit 3 VME Map Length

const float BL_DACVolts     =   180.0;
const float BL_DAC_VPB  =   BL_DACVolts/8192;      // 13 bit DAC

//const int hvdac = HVDAC16;
#ifdef T_HVDAC16
 int hvdac = HVDAC16;
#else
 int hvdac = HVDAC24;
#endif

#if 0
extern int __fastcall VMECtrl(void);
extern void bt_cleanup(void);
extern int __fastcall DACReset(void);
extern int __fastcall SetDAC(long crate,long mod,long chip,long dac,bt_data16_t dacdata,int delay);
extern int msdelay(long int tcnt);
#endif

int __fastcall VMECtrl(void);
void bt_cleanup(void);
int __fastcall DACReset(void);
int __fastcall SetDAC(long crate,long mod,long chip,long dac,bt_data16_t dacdata,int delay);
int msdelay(long int tcnt);

#endif

