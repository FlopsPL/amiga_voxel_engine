#include "Copper8x8_C2P.h"

unsigned short* build_8x8_c2p_copper_list() {
  unsigned short* copper_list_start_address = AllocMem(4+(24+(4*40))*32+12); /// Less than 6kB whole copper list.
  unsigned short* memoryAddress = copper_list_start_address; 
  unsigned short wait_command_head = 0x2601; /// 0x26 horizontal line
  unsigned short wait_command_tail = 0xFF00; /// Compare only line number, do not compare px in line
  unsigned short wait_command_head_px = 0x0037; /// 0x37 px
  unsigned short wait_command_tail_px = 0x80fe; /// Compare only px number, ignore line number comparision
  unsigned short skip_command_head = 0x2e01; /// 0x2e (0x26+8=0x2e)
  unsigned short skip_command_tail = 0x7f01; /// Compare only line number, ignore px comparision. Skip next command
                                             /// if equal or grater;
  unsigned short color_change_head = 0x0180; /// Color pallete nr 0 (background)
  unsigned short color_change_tail = 0x0000; /// Black color 0RGB
  unsigned short copper_list_address_high_head = 0x0080;
  unsigned short copper_list_address_low_head = 0x0082;
  unsigned short copper_list_address_tail = 0x0000;

  unsigned short copper_list_reset_head = 0x0088;
  unsigned short copper_list_reset_tail = 0x0000;

  //*memoryAddress++ = 0x0100; *memoryAddress++ = 0x0000; /// DAMCON turning off all bitplanes
  //unsigned char line_on_which_skip = 0x26+0x08; /// Initial value for skip command
  unsigned char* skip_on_witch_line = (unsigned char*)(&skip_command_head);
  *memoryAddress++ = wait_command_head;*memoryAddress++ = wait_command_tail; /// Waiting for first visible line.
  unsigned int copper_list_insertion_address;/// = (unsigned int)memoryAddress;
  unsigned short high_address;/// = (unsigned short)copper_list_insertion_address;
  unsigned short low_address;/// = (unsigned short)(copper_list_insertion_address >> 16); /// In asm will be only one command SWAP.
  for(unsigned short line = 0; line < 32; ++line)
  {
    /// Set coppper list starting address.
    copper_list_insertion_address = (unsigned int)memoryAddress+8; /// This address + 8, because it need to be set only once and loop can be done, without it.
    high_address = (unsigned short)copper_list_insertion_address;
    low_address = (unsigned short)(copper_list_insertion_address >> 16); /// In asm will be only one command SWAP.

    *memoryAddress++ = copper_list_address_high_head; /// New start address for copperlist (for 8 times line draw loop).
    *memoryAddress++ = high_address;
    *memoryAddress++ = copper_list_address_low_head;
    *memoryAddress++ = low_address;
    *memoryAddress++ = wait_command_head_px; /// Wait for 0x37px. Here starts min loop (8 times same line).
    *memoryAddress++ = wait_command_tail_px;
    for(unsigned short pixel = 0; pixel < 40; ++pixel) /// Start drawing line
    {
      *memoryAddress++ = color_change_head;
      *memoryAddress++ = color_change_tail; /// To draw we need to overwrite only this two bytes. 0RGB
    }   /// 12 bit color, 4 bit per color, word. For 320x256 (8x8) its 40x32 big pixels it's olny 1280 writes for whole screen. 2kB of Ram for raw data.128
    *memoryAddress++ = color_change_head; /// One more to have back black border.
    *memoryAddress++ = color_change_tail;
    /// Skip instruction here.
    *skip_on_witch_line = *skip_on_witch_line+0x08; /// Changing only first byte of skip command.
    *memoryAddress++ = skip_command_head;
    *memoryAddress++ = skip_command_tail;
    *memoryAddress++ = copper_list_reset_head; /// Reseting copper internal address counter.
    *memoryAddress++ = copper_list_reset_tail; /// It is skiped if raster line is after line set in above skip command.
    /// Streching to 8 lines same line ends.
  }
  copper_list_insertion_address = (unsigned int)copper_list_start_address;
  high_address = (unsigned short)copper_list_insertion_address;
  low_address = (unsigned short)(copper_list_insertion_address >> 16);
  *memoryAddress++ = copper_list_address_high_head;
  *memoryAddress++ = high_address;
  *memoryAddress++ = copper_list_address_low_head;
  *memoryAddress++ = low_address;
  *memoryAddress++ = 0xFFFF;
  *memoryAddress++ = 0xFFFE; /// End coppper list command.
  return copper_list_start_address;
}