
/*
A simple "hello world" example.
Set the screen background color and palette colors.
Then write a message to the nametable.
Finally, turn on the PPU to display video.
*/

#include "neslib.h"
#include <peekpoke.h>
#include <stdlib.h>
#include <string.h>

// link the pattern table into CHR ROM
//#link "chr_generic.s"



// bank-switching configuration
#define NES_MAPPER 4		// Mapper 4 (MMC3)
#define NES_PRG_BANKS 4		// # of 16KB PRG banks
#define NES_CHR_BANKS 1		// # of 8KB CHR banks
//#define HEAP_START 0x0900
//#resource "nesbanked.cfg"
#define CFGFILE nesbanked.cfg

#define MMC_MODE 0x00

#define MMC3_SET_REG(r,n)\
	POKE(0x8000, MMC_MODE|(r));\
	POKE(0x8001, (n));

#define MMC3_CHR_0000(n) MMC3_SET_REG(0,n)
#define MMC3_CHR_0800(n) MMC3_SET_REG(1,n)
#define MMC3_CHR_1000(n) MMC3_SET_REG(2,n)
#define MMC3_CHR_1400(n) MMC3_SET_REG(3,n)
#define MMC3_CHR_1800(n) MMC3_SET_REG(4,n)
#define MMC3_CHR_1C00(n) MMC3_SET_REG(5,n)
#define MMC3_PRG_8000(n) MMC3_SET_REG(6,n)
#define MMC3_PRG_A000(n) MMC3_SET_REG(7,n)

#define MMC3_MIRROR(n) POKE(0xa000, (n))

#define MMC3_WRAM_DISABLE() POKE(0xA001, 0x40)
#define MMC3_WRAM_ENABLE() POKE(0xA001, 0x80)
#define MMC3_WRAM_READ_ONLY() POKE(0xA001, 0xC0)

void testmalloc()
{
  char *stringA = calloc(1, 8);
  byte i;
  for (i = 0; i < 8; ++i)
  {
    stringA[i] = 0x88;
  }
  
  vram_adr(NTADR_A(2,4));		// set address
  vram_write(stringA, 8);	// write bytes to video RAM
  free(stringA);
  
  stringA = malloc(256);
  for (i = 0; i < 255; ++i)
  {
    stringA[i] = i;
  }
  vram_adr(NTADR_A(2,8));		// set address
  vram_write(stringA, 256);	// write bytes to video RAM
  free(stringA);
  
}

// main function, run after console reset
void main(void) {
  MMC3_WRAM_ENABLE();
  // set palette colors
  pal_col(0,0x02);	// set screen to dark blue
  pal_col(1,0x14);	// fuchsia
  pal_col(2,0x20);	// grey
  pal_col(3,0x30);	// white

  // write text to name table
  vram_adr(NTADR_A(2,2));		// set address
  vram_write("HELLO, WORLD!", 13);	// write bytes to video RAM
  
  testmalloc();
  // enable PPU rendering (turn on screen)
  ppu_on_all();

  // infinite loop
  while (1) ;
}
