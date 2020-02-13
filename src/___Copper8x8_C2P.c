#include "Copper8x8_C2P.h"

#define wait_less 0x003f
#define wait_greater 0x803f

#define wait_less_end_line 0x00df
#define wait_greater_end_line 0x80df

#define skip_less 0x0701;
#define skip_greater 0x8701;

unsigned short* build_8x8_c2p_copper_list() {
  unsigned short* copper_list_start_address = memAllocChip((4+(24+(4*40))*32+12)*3); /// Less than 6kB whole copper list. Chip alloc.
  unsigned short* memoryAddress = copper_list_start_address; 
  unsigned short wait_command_head = 0x2801; /// 0x28 horizontal line
  unsigned short wait_command_tail = 0xFF80; /// Compare only line number, do not compare px in line
  unsigned short wait_command_head_px = wait_less; /// 0x37 px
  unsigned short wait_command_tail_px = 0x80fe; /// Compare only px number, ignore line number comparision
  
  unsigned short wait_command_head_end_line = wait_less_end_line;
  unsigned short wait_command_tail_end_line = 0x80fe;
  
  unsigned short skip_command_head = 0x0701; /// 0x2e (0x28+8=0x30)
  unsigned short skip_command_tail = 0x8701; /// Compare only line number, ignore px comparision. Skip next command
                                             /// if equal or grater;
  unsigned short color_change_head = 0x0180; /// Color pallete nr 0 (background)
  unsigned short color_change_black_tail = 0x0000; /// Black color 0RGB
  unsigned short color_change_tail = 0x0fff; /// Black color 0RGB
  unsigned short copper_list_address_high_head = 0x0080;
  unsigned short copper_list_address_low_head = 0x0082;
  unsigned short copper_list_address_tail = 0x0000;

  unsigned short copper_list_reset_head = 0x0088;
  unsigned short copper_list_reset_tail = 0x0000;

  *memoryAddress++ = wait_command_head;*memoryAddress++ = wait_command_tail; /// Waiting for first visible line.

  unsigned int copper_list_insertion_address;
  unsigned short* high_address;
  unsigned short* low_address;

  high_address = &copper_list_insertion_address;
  low_address = high_address;
  ++low_address;

  for(unsigned short line = 0; line < 32; ++line)
  { 
    copper_list_insertion_address = (unsigned int)memoryAddress;
    //copper_list_insertion_address += 8;  /// Skip copper insertion address in next step.
    /// Set coppper list starting address.
   
    
    //*memoryAddress++ = color_change_head;
    //*memoryAddress++ = color_change_black_tail;

    *memoryAddress++ = wait_command_head_px; /// Wait for 0x37px. Here starts min loop (8 times same line).
    *memoryAddress++ = wait_command_tail_px;
    for(unsigned short pixel = 0; pixel < 40; ++pixel) /// Start drawing line
    {
      color_change_tail = color_change_tail - 0x0111;
      *memoryAddress++ = color_change_head;
      *memoryAddress++ = color_change_tail; /// To draw we need to overwrite only this two bytes. 0RGB
      if (color_change_tail == 0) color_change_tail = 0x0fff;

    }   /// 12 bit color, 4 bit per color, word. For 320x256 (8x8) its 40x32 big pixels it's olny 1280 writes for whole screen. 2kB of Ram for raw data.128
   *memoryAddress++ = color_change_head; /// One more to have back black border.
   *memoryAddress++ = color_change_black_tail;
     // *memoryAddress++ = wait_command_head_end_line;
   //*memoryAddress++ = wait_command_tail_end_line;
    *memoryAddress++ = copper_list_address_high_head; /// New start address for copperlist (for 8 times line draw loop).
    *memoryAddress++ = *high_address;
    *memoryAddress++ = copper_list_address_low_head;
    *memoryAddress++ = *low_address;

    // *memoryAddress++ = color_change_head; /// One more to have back black border.
    // *memoryAddress++ = color_change_black_tail;
    // *memoryAddress++ = color_change_head; /// One more to have back black border.
    // *memoryAddress++ = color_change_black_tail;
    // *memoryAddress++ = color_change_head; /// One more to have back black border.
    // *memoryAddress++ = color_change_black_tail;
    // *memoryAddress++ = color_change_head; /// One more to have back black border.
    // *memoryAddress++ = color_change_black_tail;
    // *memoryAddress++ = color_change_head; /// One more to have back black border.
    // *memoryAddress++ = color_change_black_tail;
    // *memoryAddress++ = color_change_head; /// One more to have back black border.
    // *memoryAddress++ = color_change_black_tail;
    // *memoryAddress++ = color_change_head; /// One more to have back black border.
    // *memoryAddress++ = color_change_black_tail;
    // *memoryAddress++ = color_change_head; /// One more to have back black border.
    // *memoryAddress++ = color_change_black_tail;


    
    /// Skip instruction here.
    *memoryAddress++ = skip_command_head;
    *memoryAddress++ = skip_command_tail;
    if (line > 9 && line < 26) {
      wait_command_head_px = wait_greater;
      skip_command_head = skip_greater;
      wait_command_head_end_line = wait_greater_end_line;
    } else {
      wait_command_head_px = wait_less;
      skip_command_head = skip_less
      wait_command_head_end_line = wait_less_end_line;
    }
    
    *memoryAddress++ = copper_list_reset_head; /// Reseting copper internal address counter.
    *memoryAddress++ = copper_list_reset_tail; /// It is skiped if raster line is after line set in above skip command.
    // *memoryAddress++ = 0x00e3; /// less greater need to be added
    // *memoryAddress++ = 0x80fe;
    /// Streching to 8 lines same line ends.

    // if (*skip_on_witch_line < 0x09 && *skip_on_witch_line > 0x00) {
    //   *memoryAddress++ = 0x00e3;
    //   *memoryAddress++ = 0xfffe;
    // }
  }
  copper_list_insertion_address = (unsigned int)copper_list_start_address;
  *memoryAddress++ = copper_list_address_high_head;
  *memoryAddress++ = *high_address;
  *memoryAddress++ = copper_list_address_low_head;
  *memoryAddress++ = *low_address;
  *memoryAddress++ = 0xFFFF;
  *memoryAddress++ = 0xFFFE; /// End coppper list command.
  return copper_list_start_address;
}

/// Wiadomo co jest nie tak. Nie da się zrobić pętli, z przeskokiem na końcu (chyba nie da), bo
/// brakuje ostani skip musi być bez 8, a pierwzy wait z 8