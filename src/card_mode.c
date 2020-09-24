//-----------------------------------------------
// #### PROJECT: O3 TREATMENT - Custom Board ####
// ## Internal Test Functions Module
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CARD_MODE.C #############################
//-----------------------------------------------

// Includes --------------------------------------------------------------------
#include "card_mode.h"
#include "card_utils.h"
#include "lcd_utils.h"
#include "lcd.h"
#include "mfrc522.h"
#include "tim.h"
#include "uart.h"

// #include "flash_program.h"
// #include "menues.h"

#include <stdio.h>

// Module Private Types & Macros -----------------------------------------------
typedef enum {
    CARD_INIT_MFRC522 = 0,
    CARD_INIT,
    CARD_STANDBY,
    CARD_SHOW_SESSIONS,
    CARD_SHOW_SESSIONS_1,
    CARD_SHOW_SESSIONS_2,
    CARD_DISCOUNT,
    CARD_TO_INIT_SESSION,
    CARD_TO_INIT_SESSION_1,    
    CARD_ERROR_INVALID,
    CARD_ERROR_NO_CREDIT,
    CARD_ERROR_WRITING,
    CARD_ERROR_WAIT_TO_INIT,
    CARD_NO_RFID_DETECTED
    
} card_mode_t;


// Externals -------------------------------------------------------------------
extern char s_blank [];


// Globals ---------------------------------------------------------------------
unsigned char card_internal = 0;
unsigned char card_disc = 0;
card_mode_t card_mode_state = CARD_INIT_MFRC522;
volatile unsigned short tt_card_internal = 0;

// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void Card_Mode_Standby_Reset (void)
{
    card_mode_state = CARD_INIT_MFRC522;
}


void Card_Mode_Standby_Init (void)
{
    card_mode_state = CARD_INIT;
}


uint8_t id [20] = { 0 };
#define BLOCK_TO_UNLOCK    4    //sector 1 bloque 0
card_data_t card_data;
void Card_Mode_Standby (mem_bkp_t * configurations)
{
    unsigned char status = 0;
    unsigned char size = 0;
    char s_lcd [40] = { 0 };
    
    switch (card_mode_state)
    {
    case CARD_INIT_MFRC522:
        RFID_CS_ON;
        RFID_RST_ON;
        LCD_Writel1(" Inicializando  ");
        Wait_ms(500);
        MFRC522_Init();
        Wait_ms(500);
        if (MFRC522_Verify_Version() != 0x92)
        {
            LCD_Writel2("No RFID detector");
            card_mode_state = CARD_NO_RFID_DETECTED;
            ChangeLed(LED_NO_RFID);
            tt_card_internal = 2000;
        }
        else
            card_mode_state++;
        
        break;

    case CARD_INIT:
        LCD_Writel1("WURTH ARGENTINA ");
        card_internal = 0;
        ChangeLed(LED_TREATMENT_STANDBY);
        card_mode_state++;
        break;

        
    case CARD_STANDBY:
        switch (card_internal)
        {
        case 0:
            if (!tt_card_internal)
            {
                LCD_Writel2("Acerque Tarjeta*");
                tt_card_internal = 1200;
                card_internal++;
            }
            break;

        case 1:
            if (!tt_card_internal)
            {
                LCD_Writel2(s_blank);
                tt_card_internal = 1200;
                card_internal = 0;
            }
            break;

        default:
            card_internal = 0;
            break;
        }

        //si acerca tarjeta
        status = MFRC522_Check_NoHalt(id);
        // status = MFRC522_Check(id);        
        if (status == MI_OK)
        {
            sprintf(s_lcd, "carduid: 0x%02x 0x%02x 0x%02x 0x%02x\n",
                    *(id+0),
                    *(id+1),
                    *(id+2),
                    *(id+3));                        
                        
            Usart1Send(s_lcd);
            Wait_ms(30);

            // show card bcc
            sprintf(s_lcd, "cardbcc: 0x%02x\n", *(id+4));
            Usart1Send(s_lcd);
            Wait_ms(30);
            card_mode_state++;
        }
        break;

    case CARD_SHOW_SESSIONS:
        size = MFRC522_SelectTag(id);        
        if (size)
        {
            // select tag and show card type
            sprintf(s_lcd, "cardtype: 0x%02x\n", size);
            Usart1Send(s_lcd);
            Wait_ms(30);
            
            unsigned char s_key [] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
            status = MFRC522_Auth(PICC_AUTHENT1A, BLOCK_TO_UNLOCK, s_key, id);
            if (status == MI_OK)
            {
                unsigned char readblock[18] = { 0 };
                status = MFRC522_ReadBlock(BLOCK_TO_UNLOCK, readblock);
                if (status == MI_OK)
                {
                    status = Card_ProcessDataString(readblock, &card_data);
                    if (status == MI_OK)
                    {
                        // sprintf(s_lcd, "Sesiones: %d    ", card_data.sessions_left);
                        // LCD_Writel1(s_lcd);
                        // sprintf(s_lcd, "Totales:  %d    ", card_data.sessions_orig);
                        // LCD_Writel2(s_lcd);
                        sprintf(s_lcd, "Sesiones %d/%d  ",
                                card_data.sessions_left,
                                card_data.sessions_orig);
                        
                        LCD_Writel1(s_lcd);
                        LCD_Writel2("Acerque Tarjeta*");
                        tt_card_internal = 1500;
                        card_mode_state++;
                    }
                    else
                    {
                        sprintf(s_lcd, "invalid data %s\n", readblock);
                        Usart1Send(s_lcd);
                    }
                }
                else
                    Usart1Send("cant read card data!\n");

                //desbloqueo rfid
                if (card_mode_state == CARD_SHOW_SESSIONS)
                    MFRC522_StopCrypto1();
            }
            else
                Usart1Send("cant authorize card!\n");
        }
        else
        {
            Usart1Send("cant select card!\n");
            status = MI_ERR;
        }
        
        if (status != MI_OK)
            card_mode_state = CARD_ERROR_INVALID;

        break;

    case CARD_SHOW_SESSIONS_1:
        if (!tt_card_internal)
        {
            if (card_data.sessions_left > 0)
            {
                LCD_Writel1(" Mantenga cerca ");
                LCD_Writel2(" para la sesion ");
                tt_card_internal = 1500;
                card_mode_state++;
            }
            else
            {
                card_mode_state = CARD_ERROR_NO_CREDIT;
                MFRC522_StopCrypto1();
            }
        }
        break;

    case CARD_SHOW_SESSIONS_2:
        if (!tt_card_internal)
        {
            LCD_Writel2(s_blank);            
            card_mode_state++;
            card_disc = 0;
            LCD_2DO_RENGLON;
        }
        break;

    case CARD_DISCOUNT:
        if (!tt_card_internal)
        {
            tt_card_internal = 100;

            if (card_disc < 16)
            {
                Lcd_TransmitStr(".");
                card_disc++;
            }
            else
            {
                // reviso si puedo grabar
                card_data.sessions_left--;
                Card_CreateDataString((unsigned char *)s_lcd, &card_data);
                if (MFRC522_WriteBlock(BLOCK_TO_UNLOCK, (uint8_t *)s_lcd) == MI_OK)
                {
                    if (MFRC522_ReadBlock(BLOCK_TO_UNLOCK, (uint8_t *)s_lcd) == MI_OK)
                    {
                        if (Card_CompareDataString((unsigned char *)s_lcd, &card_data) == MI_OK)
                            card_mode_state = CARD_TO_INIT_SESSION;
                        else
                            Usart1Send("card verify error!\n");
                    }
                    else
                        Usart1Send("cant re-read card data!\n");
                }
                else
                    Usart1Send("cant write card data!\n");

                if (card_mode_state == CARD_DISCOUNT)    //hubo algunos de los errores
                    card_mode_state = CARD_ERROR_WRITING;

                MFRC522_StopCrypto1();
            }
        }
        break;

    case CARD_TO_INIT_SESSION:
        if (!tt_card_internal)
        {
            configurations->dummy1 = 1;
            LCD_Writel1("Listo para      ");
            LCD_Writel2(" iniciar sesion ");
            tt_card_internal = 1000;
            card_mode_state++;
        }
        break;

    case CARD_TO_INIT_SESSION_1:
        if (!tt_card_internal)
        {
            LCD_Writel1("Presione O3 para");
            // LCD_Writel1(" iniciar sesion ");
            tt_card_internal = 1000;
            card_mode_state--;
        }
        break;
        
    case CARD_ERROR_INVALID:
        LCD_Writel2("Tarjeta invalida");
        tt_card_internal = 2000;
        card_mode_state = CARD_ERROR_WAIT_TO_INIT;
        break;

    case CARD_ERROR_NO_CREDIT:
        LCD_Writel1("Tarjeta agotada ");
        LCD_Writel2(s_blank);            
        tt_card_internal = 2000;
        card_mode_state = CARD_ERROR_WAIT_TO_INIT;
        break;
        
    case CARD_ERROR_WRITING:
        LCD_Writel1("Error al grabar ");
        LCD_Writel2(" la Tarjeta!!!  ");
        tt_card_internal = 2000;
        card_mode_state = CARD_ERROR_WAIT_TO_INIT;
        break;

    case CARD_ERROR_WAIT_TO_INIT:
        if (!tt_card_internal)
            card_mode_state = CARD_INIT;

        break;
        
    case CARD_NO_RFID_DETECTED:
        if (!tt_card_internal)
            card_mode_state = CARD_INIT_MFRC522;
        
        break;
        
    default:
        card_mode_state = CARD_INIT_MFRC522;
        break;
    }
}


void Card_Mode_Timeouts (void)
{
    if (tt_card_internal)
        tt_card_internal--;
}

//--- end of file ---//
