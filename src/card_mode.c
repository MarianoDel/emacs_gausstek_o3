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
#include "lcd_utils.h"
#include "lcd.h"
// #include "flash_program.h"
// #include "menues.h"

#include <stdio.h>

// Module Private Types & Macros -----------------------------------------------
typedef enum {
    CARD_INIT = 0,
    CARD_STANDBY,
    CARD_SHOW_SESSIONS,
    CARD_SHOW_SESSIONS_1,
    CARD_SHOW_SESSIONS_2,
    CARD_DISCOUNT,
    CARD_WRITING_ERROR
    
} card_mode_t;


// Externals -------------------------------------------------------------------
extern char s_blank [];


// Globals ---------------------------------------------------------------------
unsigned char card_internal = 0;
unsigned char card_disc = 0;
card_mode_t card_mode_state = CARD_INIT;
volatile unsigned short tt_card_internal = 0;

// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void Card_Mode_Standby_Reset (void)
{
    card_mode_state = CARD_INIT;
}

void Card_Mode_Standby (mem_bkp_t * configurations)
{
    char s_lcd [20] = { 0 };
    
    switch (card_mode_state)
    {
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
        if (configurations->dummy1 == 1)
        {
            configurations->dummy1 = 0;
            card_mode_state++;
        }
        break;

    case CARD_SHOW_SESSIONS:
        //TODO: ojo con el tamanio de las variables aca
        sprintf(s_lcd, "Sesiones: %d/%d",
                configurations->dummy2,
                300);

        LCD_Writel2(s_lcd);
        tt_card_internal = 1500;
        card_mode_state++;
        break;

    case CARD_SHOW_SESSIONS_1:
        if (!tt_card_internal)
        {
            LCD_Writel1("Aleje y Acerque ");
            LCD_Writel2("Tarjeta confirma");
            tt_card_internal = 5000;
            card_mode_state++;
            configurations->dummy1 = 0;
        }
        break;

    case CARD_SHOW_SESSIONS_2:
        if (configurations->dummy1 == 1)
        {
            configurations->dummy1 = 0;
            LCD_Writel1(" Mantenga cerca ");
            LCD_Writel2(s_blank);            
            card_mode_state++;
            configurations->dummy1 = 0;
            tt_card_internal = 0;
            card_disc = 0;
            LCD_2DO_RENGLON;
        }
        else if (!tt_card_internal)
            card_mode_state = CARD_INIT;

        break;

    case CARD_DISCOUNT:
        if (!tt_card_internal)
        {
            tt_card_internal = 500;

            if (card_disc < 16)
            {
                Lcd_TransmitStr(".");
                card_disc++;
            }
            else
            {
                LCD_Writel1("Error al grabar ");
                LCD_Writel2(" la Tarjeta!!!  ");
                tt_card_internal = 2000;
                card_mode_state++;
            }
        }

        //TODO: si sale ok aca empieza tratamiento
        
        break;

    case CARD_WRITING_ERROR:
        if (!tt_card_internal)
            card_mode_state = CARD_INIT;

        break;
        
        
    default:
        card_mode_state = CARD_INIT;        
        break;
    }
}


void Card_Mode_Timeouts (void)
{
    if (tt_card_internal)
        tt_card_internal--;
}

//--- end of file ---//
