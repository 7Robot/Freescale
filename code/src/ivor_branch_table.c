/*
   File: ivor_branch_table_p1.c - for use with MPC560x only
   Description:  Branch table for 16 e200z0h core interrupts
   Copyright Freescale 2008.  All Rights Reserved
   Rev 1.0 Jul  6 2007 S.M. - Initial version 
   Rev 1.1 May 13 2008 D.F. - Adapted 551x version for e200z0h processors
*/

#pragma push

#define SIXTEEN_BYTES 16

/* IVOR4 will call this handler */
extern void INTC_INTCInterruptHandler(void);

#pragma section code_type ".ivor_branch_table"
#pragma force_active on

#pragma function_align 16  

__declspec(vle_on) asm void ivor_branch_table_p1(void) {
  nofralloc
 
				.align SIXTEEN_BYTES       
IVOR0trap:  b	IVOR0trap	 // IVOR 0 interrupt handler

				.align SIXTEEN_BYTES       
IVOR1trap:  b	IVOR1trap	 // IVOR 1 interrupt handler
						
				.align SIXTEEN_BYTES       
IVOR2trap:  b	IVOR2trap	 // IVOR 2 interrupt handler

				.align SIXTEEN_BYTES       
IVOR3trap:  b	IVOR3trap	 // IVOR 3 interrupt handler

				.align SIXTEEN_BYTES       
IVOR4trap:  b   INTC_INTCInterruptHandler // External Interrupt

				.align SIXTEEN_BYTES       
IVOR5trap:  b	IVOR5trap    // IVOR 5 interrupt handler

				.align SIXTEEN_BYTES       
IVOR6trap:  b	IVOR6trap	 // IVOR 6 interrupt handler

				.align SIXTEEN_BYTES       
IVOR7trap:  b	IVOR7trap	 // IVOR 7 interrupt handler

				.align SIXTEEN_BYTES       
IVOR8trap:  b	IVOR8trap	 // IVOR 8 interrupt handler

				.align SIXTEEN_BYTES       
IVOR9trap:  b	IVOR9trap	 // IVOR 9 interrupt handler

				.align SIXTEEN_BYTES       
IVOR10trap: b   IVOR10trap   // IVOR 10 interrupt handler

				.align SIXTEEN_BYTES       
IVOR11trap:  b	IVOR11trap	 // IVOR 11 interrupt handler

				.align SIXTEEN_BYTES       
IVOR12trap:  b	IVOR12trap	 // IVOR 12 interrupt handler

				.align SIXTEEN_BYTES       
IVOR13trap:  b	IVOR13trap	 // IVOR 13 interrupt handler

				.align SIXTEEN_BYTES       
IVOR14trap:  b	IVOR14trap	 // IVOR 14 interrupt handler

				.align SIXTEEN_BYTES       
IVOR15trap:  b	IVOR15trap	 // IVOR15 interrupt handler

        nop  // fill to 0x100 bytes
        nop
        nop
        
}

#pragma pop
