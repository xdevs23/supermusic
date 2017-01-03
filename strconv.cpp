#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <unistd.h>
#include <cmath>

using namespace std;


void p(string msg) {
  cerr << msg << "\n";
}


/**
 * Convert a string to a number.
 * Returns -1 if string is not representable.
 *
 * strptr: string to convert
 */
int cstrtoi(char* str) {
  int number = 0, n;
  char c;
  bool istobeneg = false;
  for (int _i = 0; _i < strlen(str); _i++) {
    int i = strlen(str) - _i - 1;
    n = 0;
    c = str[_i];
#ifdef SUPERDEBUG
    cerr << "Converting " << c << " to a number (i " << i << ")" << endl;
#endif
    if ( _i == 0 ) {
      switch(c) {
        case '-': istobeneg = true;
#ifdef SUPERDEBUG
          cerr << "Is negative!" << endl;
#endif
        default: break;
      }
    }
    if ( ! (_i == 0 && istobeneg) ) switch(c) {
      case '\0': break;
      case '0': n = 0; break;
      case '1': n = 1; break;
      case '2': n = 2; break;
      case '3': n = 3; break;
      case '4': n = 4; break;
      case '5': n = 5; break;
      case '6': n = 6; break;
      case '7': n = 7; break;
      case '8': n = 8; break;
      case '9': n = 9; break;
#ifdef SUPERDEBUG
      default: p("string not representable"); return -1; break;
#else
      default: return -1; break;
#endif
    }
#ifdef SUPERDEBUG
    cerr << "n is " << n << endl;
#endif

#ifdef SUPERDEBUG
    cerr << n << "*" << "(10^" << i << ")" << " = " << n*pow(10,i) << endl;
#endif
    if (istobeneg)
      number -= n*pow(10,i);
    else
      number += n*pow(10,i);
  }
#ifdef SUPERDEBUG
  cerr << "Number is " << number << endl;
#endif
  return number;
}

int string_to_int(char* string) {
  return cstrtoi(string);
}

int cstrtoi(std::string string) {
  return cstrtoi(string.c_str());
}

int string_to_int(std::string string) {
  return cstrtoi(string.c_str());
}

