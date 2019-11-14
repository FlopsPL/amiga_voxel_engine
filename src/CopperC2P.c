#include "CopperC2P.h"

unsigned short* build_c2p_copper_list(unsigned short pxInLine, unsigned short howManyLines) {
  unsigned short* copper_list_address = AllocMem(4+8*howManyLines+(4*pxInLine*howManyLines));
  unsigned short* memoryAddress = copper_list_address; 
  unsigned short wait_command_head = 0x2637; /// 26 horizontal line and 37 vertical point
  unsigned short wait_command_tail = 0xFFFE;
  unsigned short color_change_head = 0x0180; /// Color pallete nr 0 (background)
  unsigned short color_change_tail = 0x0000; /// Black color 0RGB

  BYTE* horizontal_line = (BYTE*)(&wait_command_head);
  BYTE horizontal_line_number = 26;
  *memoryAddress++ = 0x0100; *memoryAddress++ = 0x0000; /// DAMCON turning off all bitplanes
  //*memoryAddress++ = 0x2637; *memoryAddress++ = 0xFFFE; /// Waiting for screen starting point.
  for (unsigned short line=0; line<howManyLines;++line) {
    *memoryAddress++ = wait_command_head;
    *memoryAddress++ = wait_command_tail;
    *horizontal_line = ++horizontal_line_number;
    for (unsigned short pixel=0; pixel<pxInLine; ++pixel) {
      *memoryAddress++ = color_change_head;
      *memoryAddress++ = color_change_tail;
    }
    *memoryAddress++ = color_change_head; /// One more to have back black border.
    *memoryAddress++ = color_change_tail;
  }
  return copper_list_address;
}
