#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <cmath>

#include "strconv.h"

using namespace std;

typedef unsigned char byte;

//#define SUPERDEBUG

#ifndef SAMPLE_RATE
#define SAMPLE_RATE 48000
#endif
#ifndef BIT_DEPTH
#define BIT_DEPTH       8
#endif

int smpmult = 1, bitmult = 1;
static void putchar_(int c) {
  fprintf(stderr, "\033[K\r%i", c);
  cerr.flush();
  putchar(c);
  usleep(1);
}

static void putcharc(int c) {
  if(bitmult > 1)
    for ( int i = bitmult; i > 0; i-- )
      switch(i) {
         case 4:
           putchar_( (c >> 24) ); break;
         case 3:
           putchar_( (c >> 16) ); break;
         case 2:
           putchar_( (c >>  8) ); break;
         case 1:
           putchar_(  c        ); break;
     }
  else putchar(c);
}

static void playaud(int c) {
  if ( smpmult <= 0)
    fprintf(stderr, "Warning: smpmult is invalid!\n");
  for ( int i = 0; i < smpmult; i++ )
    putcharc(c);
}

static void playaudl(int c) {
  putchar(c);
}

// 0: legacy
// 1: new
#define PLAY_MODE 1

static void plaud(int c) {
#ifdef SUPERDEBUG
  cerr << c << endl;
#endif
#if PLAY_MODE == 0
  playaudl(c);
#else
  playaud(c);
#endif
}

static void plaudt(int c) {
  for ( int i = 0; i < 10; i++ )
    plaud(c);
}

static void plaud_ht(int c, int mult) {
  for ( int i = 0; i < mult; i++ )
    plaud(c);
}

static void plaud_ht_dyn(int c, int mult) {
  for ( int i = 0; i < mult; i++)
    if(i < mult/2)
      plaud(c - i);
    else if(i == mult/2)
      plaud(c);
    else
      plaud(c + i);
}

void playp(int times, int shift, int direction) {
  if(direction == -1)
    for ( int t = 0; t < 100*100*times; t++ )
      plaud( t & (t << shift) );
  else
    for ( int t = 0; t < 100*100*times; t++ )
      plaud( t & (t >> shift) );
}

void playp2(int times, int shift) {
  for ( int t = 0; t < 100*100*times; t++ )
    plaud( t & (t << shift) | shift << times + (times*t-shift));
}

void playexp(int a, int b, int c, int d) {
  for ( int i = 0; i < c*100*100; i++ ) {
    plaud( ( a & ( i << i % 8 ) + b - c & (i >> i/2) ) * d );
  }
}

void p(string msg) {
  cerr << msg << "\n";
}

#define __INT32_MAX 2147483647
#define __INT32_MIN -2147483648


#define argoffset 1

int main(int argc, char** argv) {
  smpmult = SAMPLE_RATE / 16000;
  bitmult = BIT_DEPTH   /     8;
  fprintf(stderr, "bitmult: %i, smpmult: %i\n", bitmult, smpmult);
  int a1 = 0, a2 = 0, a3 = 0, a4 = 0;
  if(argc > 1 + argoffset)
    a1 = cstrtoi(argv[1 + argoffset]);
  if(argc > 2 + argoffset)
    a2 = cstrtoi(argv[2 + argoffset]);
  if(argc > 3 + argoffset)
    a3 = cstrtoi(argv[3 + argoffset]);
  if(argc > 4 + argoffset)
    a4 = cstrtoi(argv[4 + argoffset]);
  fprintf(stderr, "a1 %i, a2 %i, a3 %i, a4 %i\n", a1, a2, a3, a4);
  if ( a1 == -1 || a2 == -1 || a3 == -1 || a4 == -1) {
    fprintf(stderr, "WARNING: one of these numbers (%i %i %i %i) is -1!\n", a1, a2, a3, a4);
  }
  const char* tp = (argc > 1 ? argv[1] : "\0");
    if (!strncmp(tp, "classic", 7)) {
      fprintf(stderr, "Playing with args %i %i %i\n", a1, a2, a3);
      playp(a1, a2, a3);
    } else if (!strncmp(tp, "single", strlen("single"))) for(;;)plaud(a1);
      else if (!strncmp(tp, "incr", strlen("incr"))) for(int i=0;i<255*100;i++)plaud(i/100);
      else if (!strncmp(tp, "_1", 2)) for(int i=0;i<100*100*100;i++)plaud(i << (i & 3) >> 2 << 3 >> 1);
      else if (!strncmp(tp, "_2", 2)) for(int i=0;i<100*100*100;i++){plaud(i << (i & 4) << 3 + (i/1000) * 2);plaud(i >> (i & 3) << 5 - (i/100));}
      else if (!strncmp(tp, "_3", 2)) for(int i=0;i<100*100*100*2;i++){/*plaud(i << (i & 6) << 4);plaud(i << 4 & (i << 3) + i/100);*/plaud(i >> (i << 8 - (i/20)));/*plaud((i << (i & 7) >> 2) * 3 - 40);*/}
      else if (!strncmp(tp, "_4", 2)) for(int i=0,t=7;i<123*123*90*2||t<123*123*90/2;i++,t++){plaud(i << 3 & t >> 4 + (t & i << 6 + i - t/3 + 7 - 3 & t >> 2 | i << 4));}
      else if (!strncmp(tp, "_5", 2)) playp2(a1, a2);
      else if (!strncmp(tp, "rainb", strlen("rainb"))) {
      bool offsgoingup = true;
      p("RAINBOW PLAY");
      #define RAINBOW_MAX_OFFSET 55
      byte curoffs = 0;
      for(;;) {
        for ( int s = 0; s < 400; s++ ) {
        for ( int i = 0; i < 200; i++ ) {
          for ( int sd = 0; sd < s; sd++)
            plaud(i + curoffs);
        }
        for ( int i = 200; i != 0; i-- ) {
          for ( int sd = 0; sd < s; sd++)
            plaud(i + curoffs);
        }
        }
        //if(offsgoingup) curoffs++;
        //else curoffs--;
        //if(curoffs == RAINBOW_MAX_OFFSET) offsgoingup = false;
        //else if(curoffs == 0) offsgoingup = true;
      }
    } else if (!strncmp(tp, "exp", 3)) {
      fprintf(stderr, "Experimental play!\n");
      playexp(a1, a2, a3, a4);
    } else {
  /* for (int t=0;t<100*100*100;t++) {
    if(t < 100*100*25)
      plaud(
        (t/4 >> 3) & (t/6 >> 9) * t - 4
      );
    /*else if(t < 100*100*15)
      plaud(
        (t << 2) & (t/2 >> 7) * t
      );
    else if(t < 100*100*20)
      plaud_ht_dyn(
        (t/6 >> 4) & (t/2 >> 6) * t/2 - 12
      , 3);
    else if(t < 100*100*25)
      plaud(
        (t/4 >> 6) & (t/5 >> 12) * t - 8
      );
    else if(t < 100*100*30)
      plaud(
        (t >> 2) & (t >> 4) + t%100
      );
    else if(t < 100*100*35)
      plaud( t & (t >> 4) & (t/6 >> 4) * t);
    else if(t < 100*100*40)
      plaud(t & t*(t >> 8) & (t/3 >> 9) * t/3);
    else if(t < 100*100*45)
      plaud(t/2 & (t >> 5) + 16/t - 2^3);
  * /}*/
  playp(12, 8, 2);
  playp(12, 8, 1);
  }
  return 0;
}
