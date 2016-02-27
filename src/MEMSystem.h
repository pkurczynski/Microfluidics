//---------------------------------------------------------------------------
// MEMSystem.h
//
// Header file that defines a MEMSystem class.
//
// plk 11/25/00
//---------------------------------------------------------------------------
#ifndef MEMSystemH
#define MEMSystemH

#include <vcl.h>
#include "TypeDefs.h"
#include "MEMSDevice.h"
#include "Unit3.h"

#define MAXNAMELENGTH 256

// set this flag to TRUE if program will actually command voltages
// to the VME crate
#define HARDWAREENABLED 1

class MEMSystem
{
   private:


      AnsiString       ConfigFile;


      ArrayIndex       NumWires;
      AnsiString       WireListName;
      AnsiString       WireListPath;
      Int32            NumDACValues;
      Int32            DrawGridRotationState;


   public:

      TMemo            *MEMSystemMessage;


      MEMSDevice       *theMirror;



      MEMSystem();
      ~MEMSystem();

      void Reset();
      void InitializeWireList(TTable *inWireListTable);
      void CreateVME();
      void DeleteVME();
      void Actuate();

      void OpenConfiguration(AnsiString inFileName);
      void SaveConfiguration(AnsiString inFilePath, bool inNewConfig);

      AnsiString GetConfigFile();
      void SetConfigFile(AnsiString inNewConfigFile);

      Int32 GetNumDACValues();
      void SetNumDACValues(Int32 inNewNumDACValues);

      Int32 GetDrawGridRotationState(){return DrawGridRotationState;}
};

extern MEMSystem *theSystem;

#endif
