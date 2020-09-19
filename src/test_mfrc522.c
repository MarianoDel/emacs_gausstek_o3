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
    char s_buf [100] = { 0 };
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

        // doing only a request, get the type of card
        // status = MFRC522_Request(PICC_REQIDL, id);
        // if (status == MI_OK)
        // {
        //     Usart1Send("MI_OK\n");            
        //     for (unsigned char i = 0; i < sizeof(id); i++)
        //     {                
        //         sprintf(s_buf, "i: %d id: 0x%02x\n",
        //                 i,
        //                 *(id+i));
        //         Usart1Send(s_buf);
        //         Wait_ms(20);
        //     }
        // }

        // check for the card id and halt
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

        // select tag
        status = MFRC522_Check_NoHalt(id);
        // status = MFRC522_Check(id);
        if (status == MI_OK)
        {
            // show card id
            Usart1Send("MI_OK\n");            
            for (unsigned char i = 0; i < 4; i++)
            {                
                sprintf(s_buf, "i: %d id: 0x%x\n",
                        i,
                        *(id+i));
                Usart1Send(s_buf);
                Wait_ms(20);
            }

            // select tag
            unsigned char size = MFRC522_SelectTag(id);            
            sprintf(s_buf, "cardtype: 0x%02x\n", size);
            Usart1Send(s_buf);
            Wait_ms(30);

            // autorizo el bloque 1
#define BLOCK_TO_UNLOCK    4
// #define BLOCK_TO_UNLOCK    0            
            unsigned char s_key [] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
            status = MFRC522_Auth(PICC_AUTHENT1A, BLOCK_TO_UNLOCK, s_key, id);
            if (status == MI_OK)
            {
                sprintf(s_buf, "MI_OK AuthA block %d\n", BLOCK_TO_UNLOCK);
                Usart1Send(s_buf);
                Wait_ms(30);

                // for (unsigned char j = BLOCK_TO_UNLOCK; j < (BLOCK_TO_UNLOCK + 4); j++)
                // {
                unsigned char j = BLOCK_TO_UNLOCK;
                    sprintf(s_buf, "\nreading block %d\n", j);
                    Usart1Send(s_buf);
                    Wait_ms(30);
                    
                    unsigned char readblock[16] = { 0 };
                    status = MFRC522_ReadBlock(j, readblock);
                    if (status == MI_OK)
                    {
                        for (unsigned char i = 0; i < sizeof(readblock); i++)
                        {                
                            sprintf(s_buf, "0x%02x ", *(readblock+i));
                            Usart1Send(s_buf);
                            Wait_ms(20);
                        }

                        // unsigned char writedata [16] = {0x55, 0x55, 0x55, 0x55 };
                        // status = MFRC522_WriteBlock(BLOCK_TO_UNLOCK, writedata);
                        // if (status == MI_OK)
                        //     Usart1Send("\nsaved ok!!!\n");
                    }
                    else
                    {
                        if (status == MI_ERR)
                            Usart1Send("MI_ERR\n");
                        else if (status == MI_NOTAGERR)
                            Usart1Send("MI_NOTAGERR\n");
                        else
                            Usart1Send("Unknown\n");

                        break;                        
                    }
                // }

                MFRC522_StopCrypto1();
            }
            else
            {
                if (status == MI_ERR)
                    Usart1Send("MI_ERR\n");
                else if (status == MI_NOTAGERR)
                    Usart1Send("MI_NOTAGERR\n");
                else
                    Usart1Send("Unknown\n");
            }

                
                // status = MFRC522_Auth(PICC_AUTHENT1B, BLOCK_TO_READ, s_key, id);
                // if (status == MI_OK)
                // {
                //     sprintf(s_buf, "MI_OK AuthB block %d\n", BLOCK_TO_READ);
                //     Usart1Send(s_buf);
                //     Wait_ms(100);
                // }
                // else
                // {
                //     if (status == MI_ERR)
                //         Usart1Send("MI_ERR\n");
                //     else if (status == MI_NOTAGERR)
                //         Usart1Send("MI_NOTAGERR\n");
                //     else
                //         Usart1Send("Unknown\n");
                    
                //     sprintf(s_buf, "AuthA & AuthB error block %d\n", BLOCK_TO_READ);
                //     Usart1Send(s_buf);
                //     MFRC522_Halt();
                //     Wait_ms(100);
                // }
            // }
        }
        

        // // read a data block
        // status = MFRC522_Check_NoHalt(id);
        // if (status == MI_OK)
        // {
        //     // show card id
        //     Usart1Send("MI_OK\n");            
        //     for (unsigned char i = 0; i < 4; i++)
        //     {                
        //         sprintf(s_buf, "i: %d id: 0x%x\n",
        //                 i,
        //                 *(id+i));
        //         Usart1Send(s_buf);
        //         Wait_ms(20);
        //     }

        //     //authenticate the card
        //     MFRC522_Auth();
        // }
    }
}


//--- end of file ---//
    
