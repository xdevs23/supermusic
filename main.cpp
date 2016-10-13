#include <stdio.h>

#ifndef SAMPLE_RATE
#define SAMPLE_RATE 48000
#endif
#ifndef BIT_DEPTH
#define BIT_DEPTH       8
#endif

int smpmult = 1, bitmult = 1;

static void putcharc(int c) {
  if(bitmult > 1)
    for ( int i = bitmult; i > 0; i-- )
      putchar( (i << (2^bitmult)) );
  else putchar(c);
}

static void playaud(int c) {
  if ( smpmult <= 0)
    printf("Warning: smpmult is invalid!\n");
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
  for (int t=0;t<100*100*100;t++) {
    if(t < 100*100*10)
      plaud(
        (t/4 >> 3) & (t/6 >> 9) * t - 4
      );
    else if(t < 100*100*15)
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
  }

  playp(8, 8, 1);
  playp(4, -2, -1);
}
