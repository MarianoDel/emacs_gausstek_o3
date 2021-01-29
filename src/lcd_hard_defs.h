//------------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### LCD_HARD_DEFS.H ##########################
//------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef _LCD_HARD_DEFS_H_
#define _LCD_HARD_DEFS_H_


// Exported Types Constants and Macros -----------------------------------------
// LCD Hardware defs
//#define WITH_STATE_MACHINE		//rutinas SM hay que ir llamando a un callback
					//y se envia desde un buffer con interrupcion

// #define PORT_INVERTED				//bit bajo con alto PA0->LCD_D7
#define PORT_DIRECT				//pin de puerto bajo dato bajo (ej. PA0 -> LCD_D4 en 4 bits)

#define PMASK       0x000F    //ver clear data

// #define LCDClearData GPIOA->BSRR = 0x000F0000	//reset PA3..PA0
//#define LCDClearData GPIOA->BSRR = 0x0F000000	//reset PA8..PA11
// #define LCDClearData	GPIOC->BRR = PMASK;    //reset PC0 - PC3
#define LCDClearData	(GPIOA->BRR = PMASK)    //reset PA0 - PA3

/***  Declarations of ports of a specified MCU  ***/
/* Choose the output control pins accord your MCU */
#define lcd4bit		1		/* 4 bit interface; comment this line if  */
							/*        is 8 bit interface              */
#define lcdPort2	GPIOA	/* Port of 4 data bits to lcd connection  */
// #define lcdPort2	GPIOC	/* Port of 4 data bits to lcd connection  */
#define lcdDataPinOffset	0	//offset al primer pin de los datos
					//PA0 = 0; PA4 = 4; PA8 = 8


// Exported Functions ----------------------------------------------------------

#endif    /* _LCD_HARD_DEFS_H_ */

//--- end of file ---//

