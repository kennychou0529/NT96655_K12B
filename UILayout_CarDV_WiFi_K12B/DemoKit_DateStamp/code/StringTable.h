/*//////////////////////////////////////////////////////////////////////////////

 ---------- Document Info ---------------

        Author : Anonymous
       Created : 16:44:11, Jul  9 2007
 Last modified : 10:44:25, Mar 16 2010
   Making tool : UI Designer Studio
   File format : C language header file

 ----------- Object Info ----------------

         Class : STRING DB
          Name : StringTable

*///////////////////////////////////////////////////////////////////////////////

#ifndef _STRINGTABLE_H_
#define _STRINGTABLE_H_

#include "draw_lib.h"

// STRING DB count
#define STRINGTABLE_LANG_NUM    1

// STRING ID count
#define STRINGTABLE_STR_NUM    2

// STRING DB
extern const unsigned short *gStringTable_EN[];

// STRING ID
//#define name id
#define STRID_NULL    0x0000
#define STRID_DUMMY    0x0001

#endif //_STRINGTABLE_H_
