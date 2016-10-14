#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <unistd.h>

using namespace std;

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
#if PLAY_MODE == 0
  playaudl(c);
#else
  playaud(c);
#endif
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

int main(int argc, char** argv) {
  smpmult = SAMPLE_RATE / 16000;
  bitmult = BIT_DEPTH   /     8;
  fprintf(stderr, "bitmult: %i, smpmult: %i\n", bitmult, smpmult);
  for ( int t = 0, t2 = 0;;t += 10, t2++) {
    plaud((t2 & (t2 >> 8)));
  }
}
