//------------------------------------------------------------------------
// VMEDAC.h
//
//
//------------------------------------------------------------------------
#ifndef VMEDACH
#define VMEDACH

//#include "LRHV.h"

class VMEDAC
{
   private:

#if 0
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


        // these members initialized in constructor
        bt_error_t hw_status;  // Bit 3 enum return value
        int btunit;           // BT Unit Number

        //int hvdac;    // defines the type of HVDAC used


        void bt_cleanup();

#endif

   public:

        VMEDAC();
        ~VMEDAC();

#if 0
        int __fastcall VMECtrl();
        int __fastcall DACReset();
        int __fastcall SetDAC(long crate,long mod,long chip,long dac,bt_data16_t dacdata,int delay);

#endif

};

extern VMEDAC *theDACchain;

#endif