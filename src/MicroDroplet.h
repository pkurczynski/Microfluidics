//---------------------------------------------------------------------------
// MicroDroplet.h
//
//
// revisions:
//
//
// plk 07/28/2003
//---------------------------------------------------------------------------

#ifndef MicroDropletH
#define MicroDropletH

#include <vcl.h>

#include "TypeDefs.h"
#include "MEMSystem.h"


#define MAXDROPLETSIZE 1024

class MicroDroplet
{
   private:

      int NumberOfActuators;
      int ActuatorIndexList[MAXDROPLETSIZE][2];
      int Diameter;

      void InitializeActuatorList();

   public:

      MicroDroplet(int inDiameter);
      //MicroDroplet(int inNumberOfActuators, int **inActuatorList);
      void Command(TObject *Sender, int inRow, int inColumn, DACInput inDACValue);
      void Reset(int inDiameter);

};

extern MicroDroplet *theMicroDroplet;

#endif


