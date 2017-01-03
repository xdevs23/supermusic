#ifndef _STRCONV_H_
#define _STRCONV_H

#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <algorithm>


typedef unsigned char  byte;
typedef   signed char sbyte;
typedef byte          ubyte;

extern int cstrtoi(char* string);
extern int cstrtoi(std::string string);
extern int string_to_int(char* string);
extern int string_to_int(std::string string);
/*extern char* itostr(int integer);
extern char* int_to_string(int integer);*/

#endif
