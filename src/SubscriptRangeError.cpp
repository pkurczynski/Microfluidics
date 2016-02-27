#include <vcl.h>
#include "SubscriptRangeError.h"
#include "TypeDefs.h"

SubscriptRangeError::SubscriptRangeError(Int32 inRow, Int32 inColumn, Int32 inWhich)
{
   Row = inRow;
   Column = inColumn;
   Which = inWhich;

   switch(Which)
   {
      case 0:
         theMessage = "Bad Row Index";
         break;
      case 1:
         theMessage = "Bad Column Index";
         break;
      case 2:
         theMessage = "Bad Row, Column Index";
         break;
      default:
         theMessage = "Unknown Error";
   }


}


Int32 SubscriptRangeError::BadRow()
{
   return Row;
}


Int32 SubscriptRangeError::BadColumn()
{
   return Column;
}

MyString SubscriptRangeError::Message()
{
   return theMessage;
}
