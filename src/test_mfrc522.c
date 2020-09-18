//------------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F303
// ##
// #### MFRC522.C ################################
//------------------------------------------------

// Includes --------------------------------------------------------------------
#include "test_mfrc522.h"
#include "mfrc522.h"
#include "stm32f3xx.h"
#include "usart.h"
#include "hard.h"
#include "timer.h"

#include <string.h>
#include <stdio.h>

// Externals -------------------------------------------------------------------
extern void MFRC522_CS_Enable(void);
extern void MFRC522_CS_Disable(void);


// Globals ---------------------------------------------------------------------


// Module Private Types & Macros -----------------------------------------------


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void TEST_Mfrc522 (void)
{
    unsigned char status = MI_OK;
    unsigned char id [20] = { 0 };
    char s_buf [20] = { 0 };
    OUTA0_ON;
    OUTA1_ON;
    Wait_ms(1000);
    MFRC522_Init();
    while(1)
    {
        Wait_ms(2000);
        status = MFRC522_Verify_Version();
        sprintf(s_buf, "version: 0x%x\n", status);
        Usart1Send(s_buf);

        if (INA4)
            Usart1Send("INA4 Enabled\n");
        
        // memset(id, '\0', sizeof(id));
        // status = MFRC522_Check(id);
        // if (status == MI_OK)
        // {
        //     Usart1Send("MI_OK\n");            
        //     for (unsigned char i = 0; i < sizeof(id); i++)
        //     {                
        //         sprintf(s_buf, "i: %d id: 0x%x",
        //                 i,
        //                 *(id+i));
        //         Usart1Send(s_buf);
        //         Wait_ms(20);
        //     }
        // }
        // else if (status == MI_ERR)
        //     Usart1Send("MI_ERR\n");
        // else if (status == MI_NOTAGERR)
        //     Usart1Send("MI_NOTAGERR\n");
        // else
        //     Usart1Send("Unknown\n");
    }
}


//--- end of file ---//
    
