#ifndef _COPPER_C2P_H_
#define _COPPER_C2P_H_

//typedef char BYTE;

/// Building Copper predifined copperlist for fast 4096 color C2P.
/// @param pxInLine - How many pixels in one horizontal line. For Amiga should be 40 without overscann.
/// @param howManyLines - How many lines for screen (normal value is 256).
/// @return pointer on copper list.
unsigned short* build_c2p_copper_list(unsigned short pxInLine, unsigned short howManyLines);

#endif