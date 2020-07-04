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


typedef enum {
    MENU_ENC_INIT = 0,
    MENU_ENC_TIEMPO_ENCENDIDO

} menu_enc_state_t;


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
menu_state_t menu_state = MENU_INIT;
menu_enc_state_t menu_enc_state = MENU_ENC_INIT;


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void MENU_Main_Reset (void)
{
    menu_state = MENU_INIT;
}

//funcion de seleccion del menu principal
//devuelve nueva selecci�n o estado anterior
resp_t MENU_Main (mem_bkp_t * configurations)
{
    resp_t resp = resp_continue;
    sw_actions_t actions = selection_none;
    unsigned char onoff = 0;
    unsigned short time = 0;

    switch (menu_state)
    {
    case MENU_INIT:
        resp = LCD_ShowBlink ("  Entrando en   ",
                              "Config Predeterm",
                              1,
                              BLINK_DIRECT);

        if (resp == resp_finish)
        {
            resp = resp_continue;
            if (CheckSET() == SW_NO)
            {
                LCD_ShowSelectv2Reset();
                menu_state++;
            }
        }
        break;

    case MENU_SHOW_TREATMENT_TIME:
        if (CheckSET() > SW_NO)
            actions = selection_enter;

        if (CheckCCW())
            actions = selection_dwn;

        if (CheckCW())
            actions = selection_up;
        
        resp = LCD_ShowSelectv2((const char *) "Tiempo Predeterm",
                                actions);

        if (resp == resp_change)
            menu_state = MENU_SHOW_ALARM;

        if (resp == resp_change_all_up)
            menu_state = MENU_SHOW_END_CONF;
        
        if (resp == resp_selected)
        {
            LCD_EncoderChangeReset();
            menu_state = MENU_CONF_TREATMENT_TIME;
        }

        if (actions != selection_none)    //algo se cambio, aviso
            resp = resp_change;

        break;

    case MENU_SHOW_ALARM:
        if (CheckSET() > SW_NO)
            actions = selection_enter;

        if (CheckCCW())
            actions = selection_dwn;

        if (CheckCW())
            actions = selection_up;

        resp = LCD_ShowSelectv2((const char *) "Alarma          ",
                                actions);

        if (resp == resp_change)
            menu_state = MENU_SHOW_TICKER;

        if (resp == resp_change_all_up)
            menu_state = MENU_SHOW_TREATMENT_TIME;

        if (resp == resp_selected)
        {
            LCD_EncoderOptionsOnOffReset();
            menu_state = MENU_CONF_ALARM;
        }

        if (actions != selection_none)    //algo se cambio, aviso
            resp = resp_change;

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
            LCD_EncoderOptionsOnOffReset();
            menu_state = MENU_CONF_TICKER;
        }

        if (actions != selection_none)    //algo se cambio, aviso
            resp = resp_change;

        break;


    case MENU_SHOW_END_CONF:
        if (CheckSET() > SW_NO)
            actions = selection_enter;

        if (CheckCCW())
            actions = selection_dwn;

        if (CheckCW())
            actions = selection_up;

        resp = LCD_ShowSelectv2((const char *) "Grabar config   ",
                                actions);

        if (resp == resp_change)
            menu_state = MENU_SHOW_TREATMENT_TIME;

        if (resp == resp_change_all_up)
            menu_state = MENU_SHOW_TICKER;

        if (resp == resp_selected)
        {
            LCD_EncoderOptionsOnOffReset();
            menu_state = MENU_END_CONF;
        }

        if (actions != selection_none)    //algo se cambio, aviso
            resp = resp_change;
        
        break;
        

    case MENU_CONF_TREATMENT_TIME:
        if (CheckSET() > SW_NO)
            actions = selection_enter;

        if (CheckCCW())
            actions = selection_dwn;

        if (CheckCW())
            actions = selection_up;

        time = configurations->treatment_time_min;
        
        resp = LCD_EncoderChange("minutos:        ",
                                 &time,
                                 MINIMUN_TIME_ALLOWED,
                                 MAXIMUN_TIME_ALLOWED,
                                 actions);

        if (resp == resp_finish)
        {
            configurations->treatment_time_min = time;
            menu_state = MENU_SHOW_TREATMENT_TIME;
            resp = resp_continue;
        }

        if (actions != selection_none)    //algo se cambio, aviso
            resp = resp_change;

        break;

    case MENU_CONF_ALARM:
        if (CheckSET() > SW_NO)
            actions = selection_enter;

        if (CheckCCW())
            actions = selection_dwn;

        if (CheckCW())
            actions = selection_up;

        onoff = configurations->alarms_onoff;
        resp = LCD_EncoderOptionsOnOff("Alarma          ",
                                       &onoff,
                                       actions);

        if (resp == resp_finish)
        {
            configurations->alarms_onoff = onoff;            
            menu_state = MENU_SHOW_TREATMENT_TIME;
            resp = resp_continue;
        }

        if (actions != selection_none)    //algo se cambio, aviso
            resp = resp_change;

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

        if (actions != selection_none)    //algo se cambio, aviso
            resp = resp_change;
        
        break;


    case MENU_END_CONF:
        if (CheckSET() > SW_NO)
            actions = selection_enter;

        if (CheckCCW())
            actions = selection_dwn;

        if (CheckCW())
            actions = selection_up;

        onoff = 1;
        resp = LCD_EncoderOptionsOnOff("Grabar Salir    ",
                                       &onoff,
                                       actions);

        if (resp == resp_finish)
        {
            if (onoff)
                resp = resp_need_to_save;
            else
                resp = resp_finish;

            menu_state = MENU_INIT;
        }
        else if (actions != selection_none)    //algo se cambio, aviso
            resp = resp_change;

        break;
        
    default:
        menu_state = MENU_INIT;
        break;
    }

    UpdateEncoder();
    
    return resp;
}


void MENU_Encendido_Reset (void)
{
    menu_enc_state = MENU_ENC_INIT;
}


resp_t MENU_Encendido (mem_bkp_t * configurations)
{
    resp_t resp = resp_continue;
    sw_actions_t actions = selection_none;
    unsigned short time = 0;

    switch (menu_enc_state)
    {
    case MENU_ENC_INIT:
        resp = LCD_ShowBlink ("Entrando en conf",
                              "Tiempo Encendido",
                              1,
                              BLINK_DIRECT);

        if (resp == resp_finish)
        {
            resp = resp_continue;
            if (CheckSET() == SW_NO)
            {
                LCD_EncoderChangeReset();
                menu_enc_state++;
            }
        }
        break;
        
    case MENU_ENC_TIEMPO_ENCENDIDO:
        if (CheckSET() > SW_NO)
            actions = selection_enter;

        if (CheckCCW())
            actions = selection_dwn;

        if (CheckCW())
            actions = selection_up;

        time = configurations->treatment_time_min;
        
        resp = LCD_EncoderChange("Encendido:      ",
                                 &time,
                                 MINIMUN_TIME_ALLOWED,
                                 MAXIMUN_TIME_ALLOWED,
                                 actions);

        if (resp == resp_finish)
        {
            configurations->treatment_time_min = time;
            menu_enc_state = MENU_ENC_INIT;
        }
        else if (actions != selection_none)    //algo se cambio, aviso
            resp = resp_change;
                
        break;

    default:
        menu_enc_state = MENU_ENC_INIT;
        break;
    }

    UpdateEncoder();
    
    return resp;
}


//--- end of file ---//
