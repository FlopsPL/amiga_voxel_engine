#include <ace/types.h>
#include "Copper8x8_C2P.h"


unsigned int cop[] = {
	0x01800000, 0x2637FFFE, 0x01800F00, 0x018000F0,
	0x0180000F,	0x01800F00, 0x018000F0, 0x0180000F,
	0x01800F00, 0x018000F0, 0x0180000F, 0x01800F00,
	0x018000F0,	0x0180000F, 0xFFFFFFFE
	};

	unsigned int cop2[] = {
	0x01800000, 0x2637FFFE, 0x01800FF0, 0x018000F0,
	0x0180000F,	0x01800F00, 0x01800FF0, 0x0180000F,
	0x01800F00, 0x018000F0, 0x0180000F, 0x01800F00,
	0x018000F0,	0x01800FFF, 0xFFFFFFFE
	};


volatile unsigned short old_irq;
volatile unsigned short old_dma;
volatile unsigned int old_copper_addr;

void main() {
	volatile unsigned short* inter_read = 0x0dff01c;
	volatile unsigned short* inter = 0x0dff09a;
	volatile unsigned short* dmacon_read = 0x0dff002;
	volatile unsigned short* dmacon = 0x0DFF096;
	volatile unsigned int* coppaddr = 0x0DFF080;
	volatile unsigned short* copper_reset_reg = 0x0DFF088;

	unsigned int* exec_addr = 0x00000004;
	unsigned int a6 = *(exec_addr);
	unsigned int a6_156 = a6+156;
	unsigned int a1 = *((unsigned int*)a6_156);
	unsigned int a1_38 = a1 + 38;
	old_copper_addr = *((unsigned int*)a1_38);
	// move.l 4.w,a6
	// move.l 156(a6),a1
	// move.l 38(a1),OldCopper
	// move.l a7,OldStack


	old_irq = (*inter_read) | 0x8000;
	old_dma = (*dmacon_read) | 0x8000;


	*inter = 0x7fff;
	*dmacon = 0x7fff;

	
	//*copper_reg = copper_list_addr;
	
	// unsigned int* pointer = memAllocChip(15*4);
	// unsigned int copper_list_addr = (unsigned int)pointer;
	// for (int i=0; i<15;++i) {
	// 	*pointer++ = cop[i];
 	// }

 	// *coppaddr = copper_list_addr;

 	// *copper_reset_reg = 0;
	
	
	
	unsigned short* copper_list_c2p = build_8x8_c2p_copper_list();
	*coppaddr = copper_list_c2p;
 	*dmacon = 0x8280;

	

	// unsigned short* pCopper_list_word = copper_list_c2p;
	// pCopper_list_word += 9; /// Firts big pixel
	// unsigned short color = 0x0f00;
	// for (int i=0;i<40;++i) {
	// 	*pCopper_list_word = color;
	// 	color = color >> 1;
	// 	if (color == 0) {
	// 		color = 0x0f00;
	// 	}
	// 	pCopper_list_word += 2;
	// }
// 	while (1) {
// 		pointer = copper_list_addr;
// 	for (int i=0; i<15;++i) {
// 		*pointer++ = cop2[i];
//  	}	
// 	}

	unsigned char loop = 1;
	unsigned char *CIA_A_port = 0x00BFE001;
	while (loop)
	{
		volatile unsigned char value = (*CIA_A_port) & 0x40;
		if (value != 0x40)
		{
			loop = 0;
			break;
		}
	}
	// 	/// Need to add old copper list address.
	// pointer = copper_list_addr;
	// for (int i = 0; i < 15; ++i)
	// {
	// 	*pointer++ = cop2[i];
	// }

	// while (loop)
	// {
	// 	volatile unsigned char value = (*CIA_A_port) & 0x40;
	// 	if (value != 0x40)
	// 	{
	// 		loop = 0;
	// 		break;
	// 	}
	// }



	*coppaddr = old_copper_addr;
	*copper_reset_reg = 0;

	*inter = old_irq;
	*dmacon = old_dma;
	free(copper_list_c2p);

	//*coppaddr = old_copper_addr;


	return 0;
}
