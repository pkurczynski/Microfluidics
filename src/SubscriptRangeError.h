//---------------------------------------------------------------------
// SubscriptRangeError.h
//
// which = 0 ... bad row subscript
//         1 ... bad column subscript
//         2 ... both subscripts bad!
//
// plk 10/31/00
//---------------------------------------------------------------------
#ifndef SUBSCRIPTRANGEERROR
#define SUBSCRIPTRANGEERROR 1
#include "TypeDefs.h"

class SubscriptRangeError
{
   private:
      Int32 Row;
      Int32 Column;
      Int32 Which;
      MyString theMessage;

   public:
      SubscriptRangeError(Int32 inRow, Int32 inColumn, Int32 inWhich);
      Int32 BadRow();
      Int32 BadColumn();
      MyString Message();
};

#endif
//---------------------------------------------------------------------
