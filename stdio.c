#include "stdio.h"
#include "string.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include "terminal.h"
int printf(const char* format, ...)
{

   va_list arg;
   int args_number = 0;

   va_start (arg, format);

   for(unsigned int i = 0 ; *(format + i); i++)
   {
      if( *(format + i) == '%' )
      {
         switch (*(format + ++i))
         {
         case '%':
            putchar('%');
            continue;
         case 's':
         {
           char *str = va_arg(arg,char*);
            int j = 0;
            while(*(str + j))
               putchar(*(str + j++));
            args_number++;
            continue;
         }   
         case 'd':
         {  char str[11] = {0};
            int number = va_arg(arg,int), j = 0;
            itoa(number, str, 10);         
            while(*(str + j))
               putchar(*(str + j++));
            args_number++;
            continue;
         }
         default:
            break;
         }
      }
      else
         putchar(*(format + i));

      
}
   va_end (arg);

   return args_number;
}


