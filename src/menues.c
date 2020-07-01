//-----------------------------------------------
// #### PROJECT: O3 TREATMENT - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### MENUES.C ################################
//-----------------------------------------------

// Includes --------------------------------------------------------------------
#include "menues.h"
// #include "hard.h"
// #include "tim.h"
#include "lcd_utils.h"
// #include "gpio.h"


// Module Private Types & Macros -----------------------------------------------
typedef enum {
    MENU_INIT = 0,
    MENU_SHOW_TREATMENT_TIME,
    MENU_SHOW_ALARM,
    MENU_SHOW_TICKER,
    MENU_SHOW_END_CONF,
    MENU_CONF_TREATMENT_TIME,
    MENU_CONF_ALARM,
    MENU_CONF_TICKER,
    MENU_END_CONF

} menu_state_t;


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
menu_state_t menu_state = MENU_INIT;


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------

//funcion de seleccion del menu principal
//devuelve nueva selección o estado anterior
resp_t MENU_Main (mem_bkp_t * configurations)
{
    resp_t resp = resp_continue;
    sw_actions_t actions = selection_none;
    unsigned char onoff = 0;
    unsigned short time = 0;

    switch (menu_state)
    {
    case MENU_INIT:
        resp = LCD_ShowBlink ("    Entering    ",
                              "  Config Menu   ",
                              1,
                              BLINK_DIRECT);

        if (resp == resp_finish)
        {
            resp = resp_continue;
            if (CheckSET() == SW_NO)
                menu_state++;
        }
        break;

    case MENU_SHOW_TREATMENT_TIME:
        if (CheckSET() > SW_NO)
            actions = selection_enter;

        if (CheckCCW())
            actions = selection_dwn;

        if (CheckCW())
            actions = selection_up;
        
        resp = LCD_ShowSelectv2((const char *) "Treatment Time  ",
                                actions);

        if (resp == resp_change)
            menu_state = MENU_SHOW_ALARM;

        if (resp == resp_change_all_up)
            menu_state = MENU_SHOW_END_CONF;
        
        if (resp == resp_selected)
            menu_state = MENU_CONF_TREATMENT_TIME;

        break;

    case MENU_SHOW_ALARM:
        if (CheckSET() > SW_NO)
            actions = selection_enter;

        if (CheckCCW())
            actions = selection_dwn;

        if (CheckCW())
            actions = selection_up;

        resp = LCD_ShowSelectv2((const char *) "Alarm           ",
                                actions);

        if (resp == resp_change)
            menu_state = MENU_SHOW_TICKER;

        if (resp == resp_change_all_up)
            menu_state = MENU_SHOW_TREATMENT_TIME;

        if (resp == resp_selected)
            menu_state = MENU_CONF_ALARM;

        break;

    case MENU_SHOW_TICKER:
        if (CheckSET() > SW_NO)
            actions = selection_enter;

        if (CheckCCW())
            actions = selection_dwn;

        if (CheckCW())
            actions = selection_up;
        
        resp = LCD_ShowSelectv2((const char *) "Ticker          ",
                                actions);

        if (resp == resp_change)
            menu_state = MENU_SHOW_END_CONF;

        if (resp == resp_change_all_up)
            menu_state = MENU_SHOW_ALARM;

        if (resp == resp_selected)
        {
            onoff = configurations->ticker_onoff;
            menu_state = MENU_CONF_TICKER;
        }
        break;


    case MENU_SHOW_END_CONF:
        if (CheckSET() > SW_NO)
            actions = selection_enter;

        if (CheckCCW())
            actions = selection_dwn;

        if (CheckCW())
            actions = selection_up;

        resp = LCD_ShowSelectv2((const char *) "End Configuration",
                                actions);

        if (resp == resp_change)
            menu_state = MENU_SHOW_TREATMENT_TIME;

        if (resp == resp_change_all_up)
            menu_state = MENU_SHOW_TICKER;

        if (resp == resp_selected)
            menu_state = MENU_END_CONF;

        break;
        

    case MENU_CONF_TREATMENT_TIME:
        if (CheckSET() > SW_NO)
            actions = selection_enter;

        if (CheckCCW())
            actions = selection_dwn;

        if (CheckCW())
            actions = selection_up;

        time = configurations->treatment_time_min;
        //TODO: mejorar lectura de memoria y cargar todo en el main, cambiar el chequeo que sigue
        if ((time < 1) || (time > 30))
            time = 30;
        
        resp = LCD_EncoderChange("minutos:        ",
                                 &time,
                                 1,
                                 30,
                                 actions);

        if (resp == resp_finish)
        {
            configurations->treatment_time_min = time;
            menu_state = MENU_SHOW_TREATMENT_TIME;
            resp = resp_continue;
        }
        break;

    case MENU_CONF_ALARM:
        if (CheckSET() > SW_NO)
            actions = selection_enter;

        if (CheckCCW())
            actions = selection_dwn;

        if (CheckCW())
            actions = selection_up;

        onoff = configurations->alarms_onoff;
        resp = LCD_EncoderOptionsOnOff("Alarm           ",
                                       &onoff,
                                       actions);

        if (resp == resp_finish)
        {
            configurations->alarms_onoff = onoff;            
            menu_state = MENU_SHOW_TREATMENT_TIME;
            resp = resp_continue;
        }
        break;

    case MENU_CONF_TICKER:
        if (CheckSET() > SW_NO)
            actions = selection_enter;

        if (CheckCCW())
            actions = selection_dwn;

        if (CheckCW())
            actions = selection_up;

        onoff = configurations->ticker_onoff;
        resp = LCD_EncoderOptionsOnOff("Ticker          ",
                                       &onoff,
                                       actions);

        if (resp == resp_finish)
        {
            configurations->ticker_onoff = onoff;
            menu_state = MENU_SHOW_TREATMENT_TIME;
            resp = resp_continue;
        }
        break;


    case MENU_END_CONF:
        menu_state = MENU_INIT;
        resp = resp_finish;
        break;
        
    default:
        menu_state = MENU_INIT;
        break;
    }

    UpdateEncoder();
    
    return resp;
}


//--- end of file ---//
