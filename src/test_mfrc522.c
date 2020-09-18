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
#include "stm32f0xx.h"
#include "uart.h"
#include "hard.h"
#include "tim.h"

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
    uint8_t id [20] = { 0 };
    char s_buf [20] = { 0 };
    RFID_CS_ON;
    RFID_RST_ON;
    Wait_ms(1000);
    MFRC522_Init();

    Wait_ms(2000);
    status = MFRC522_Verify_Version();
    sprintf(s_buf, "version: 0x%x\n", status);
    Usart1Send(s_buf);

    // if (INA4)
    //     Usart1Send("INA4 Enabled\n");

    while (1)
    {
        Wait_ms(2000);
        memset(id, '\0', sizeof(id));

        // doing a request        
        status = MFRC522_Request(PICC_REQIDL, id);
        if (status == MI_OK)
        {
            Usart1Send("MI_OK\n");            
            for (unsigned char i = 0; i < 4; i++)
            {                
                sprintf(s_buf, "i: %d id: 0x%02x\n",
                        i,
                        *(id+i));
                Usart1Send(s_buf);
                Wait_ms(20);
            }
        }
        
        // status = MFRC522_Check(id);
        // if (status == MI_OK)
        // {
        //     Usart1Send("MI_OK\n");            
        //     for (unsigned char i = 0; i < sizeof(id); i++)
        //     {                
        //         sprintf(s_buf, "i: %d id: 0x%x\n",
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
    
