#ifndef _ENGINE_H_
#define _ENGINE_H_


#include <stdlib.h>
#include <ace/managers/viewport/simplebuffer.h>
//#define WIDTH 320
#define HEIGHT 256
#define DEPTH 4
#define COLORS 32
#define PLANEWIDTH 40
#define TERRAINDEPTH 30
#define XSIZE 150
#define YSIZE 128
#define LINEHEIGHT 4


UBYTE heightMap0[256][256];//64k
UBYTE heightMap1[128][128];//16k
UBYTE heightMap2[64][64];//4k
UBYTE heightMap3[32][32];//1k
UBYTE heightMap4[16][16];
UBYTE colorMap0[256][256];//64
UBYTE colorMap1[128][128];//16
UBYTE colorMap2[64][64];//4
UBYTE colorMap3[32][32];//1
UBYTE colorMap4[16][16];
UBYTE screenP1[XSIZE*YSIZE];//3,6k
UBYTE screenP2[XSIZE*YSIZE];//3,6k
UBYTE screenP1depth[XSIZE*YSIZE];//3,6k
UBYTE screenP2depth[XSIZE*YSIZE];//3,6k

UWORD fastPlane1W[PLANEWIDTH*HEIGHT];//20k
UWORD fastPlane2W[PLANEWIDTH*HEIGHT];//20k
UWORD fastPlane3W[PLANEWIDTH*HEIGHT];//20k
UWORD fastPlane4W[PLANEWIDTH*HEIGHT];//20k

WORD rayCastX[XSIZE][TERRAINDEPTH];//3,6k
WORD rayCastY[YSIZE][TERRAINDEPTH];//3,6k

UBYTE enemyPlacementPerspective[XSIZE/2][128];

UBYTE modulo2[XSIZE];

UBYTE debugValue;

//sand dunes theme
UWORD kolory[COLORS] =
{
	0x101,0x212,0x323,0x424,0x523,0x633,0x743,0x854,
	0xa65,0xb75,0xc85,0xd96,0xeb8,0xfdb,0xbcc,0x8be
};

UBYTE colorByteDitherP1EvenHigh[COLORS*COLORS*COLORS];//32k
UBYTE colorByteDitherP2EvenHigh[COLORS*COLORS*COLORS];//32k
UBYTE colorByteDitherP3EvenHigh[COLORS*COLORS*COLORS];//32k
UBYTE colorByteDitherP4EvenHigh[COLORS*COLORS*COLORS];//32k

UBYTE colorByteDitherP1OddHigh[COLORS*COLORS*COLORS];//32k
UBYTE colorByteDitherP2OddHigh[COLORS*COLORS*COLORS];//32k
UBYTE colorByteDitherP3OddHigh[COLORS*COLORS*COLORS];//32k
UBYTE colorByteDitherP4OddHigh[COLORS*COLORS*COLORS];//32k

UBYTE colorByteDither8x8P1[8*8*8*8*8];//32k
UBYTE colorByteDither8x8P2[8*8*8*8*8];//32k
UBYTE colorByteDither8x8P3[8*8*8*8*8];//32k
UBYTE colorByteDither8x8P4[8*8*8*8*8];//32k

UWORD p1x,p1y,p1h,p2x,p2y,p2h;
int interlace;
UBYTE renderingDepth = TERRAINDEPTH;

void engineGsCreate(void);

void engineGsLoop(void);

void engineGsDestroy(void);

#endif // _ENGINE_H_
