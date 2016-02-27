//----------------------------------------------------------------------------
#ifndef Unit3H
#define Unit3H
//----------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Windows.hpp>
#include <Classes.hpp>
#include <Graphics.hpp>
#include <Controls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <DB.hpp>
#include <DBTables.hpp>
#include <Db.hpp>
//----------------------------------------------------------------------------
class TDataModule3 : public TDataModule
{
__published:
        TTable *Table1;
        TDataSource *DataSource1;
	void __fastcall DataModuleCreate(TObject *Sender);
        
private:
	// private declarations
public:
	// public declarations
	__fastcall TDataModule3(TComponent *Owner);
};
//----------------------------------------------------------------------------
extern TDataModule3 *DataModule3;
//----------------------------------------------------------------------------
#endif
