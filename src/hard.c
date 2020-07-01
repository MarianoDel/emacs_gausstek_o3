//----------------------------------------------
// #### PROYECTO GAUSSTEK O3 - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### HARD.H #################################
//----------------------------------------------
#include "hard.h"
#include "stm32f0xx.h"


// Module Private Types & Macros -----------------------------------------------
#define LED_BLINK_ON    LED_ON
#define LED_BLINK_OFF    LED_OFF

// Led Blinking States
typedef enum
{    
    START_BLINKING = 0,
    WAIT_TO_OFF,
    WAIT_TO_ON,
    WAIT_NEW_CYCLE
    
} led_state_t;


// Buzzer Bips States
typedef enum
{    
    BUZZER_WAIT_COMMANDS = 0,
    BUZZER_MARK,
    BUZZER_SPACE,
    BUZZER_TO_STOP
    
} buzzer_state_t;

// Externals -------------------------------------------------------------------



// Globals ---------------------------------------------------------------------
//for timers or timeouts
volatile unsigned short timer_led = 0;
volatile unsigned char switches_timer = 0;
volatile unsigned short buzzer_timeout = 0;

//para el led
led_state_t led_state = START_BLINKING;
unsigned char blink = 0;
unsigned char how_many_blinks = 0;

//para el buzzer
buzzer_state_t buzzer_state = BUZZER_WAIT_COMMANDS;
unsigned char buzzer_multiple = 0;
unsigned short buzzer_timer_reload_mark = 0;
unsigned short buzzer_timer_reload_space = 0;


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
//cambia configuracion de bips del LED
void ChangeLed (unsigned char how_many)
{
    how_many_blinks = how_many;
    led_state = START_BLINKING;
}


void UpdateLed (void)
{
    switch (led_state)
    {
        case START_BLINKING:
            blink = how_many_blinks;
            
            if (blink)
            {
                LED_BLINK_ON;
                timer_led = 200;
                led_state++;
                blink--;
            }
            break;

        case WAIT_TO_OFF:
            if (!timer_led)
            {
                LED_BLINK_OFF;
                timer_led = 200;
                led_state++;
            }
            break;

        case WAIT_TO_ON:
            if (!timer_led)
            {
                if (blink)
                {
                    blink--;
                    timer_led = 200;
                    led_state = WAIT_TO_OFF;
                    LED_BLINK_ON;
                }
                else
                {
                    led_state = WAIT_NEW_CYCLE;
                    timer_led = 2000;
                }
            }
            break;

        case WAIT_NEW_CYCLE:
            if (!timer_led)
                led_state = START_BLINKING;

            break;

        default:
            led_state = START_BLINKING;
            break;
    }
}


void BuzzerCommands(unsigned char command, unsigned char multiple)
{
    if (command == BUZZER_STOP_CMD)
        buzzer_state = BUZZER_TO_STOP;
    else 
    {
        if (command == BUZZER_LONG_CMD)
        {
            buzzer_timer_reload_mark = TT_BUZZER_BIP_LONG;
            buzzer_timer_reload_space = TT_BUZZER_BIP_LONG_WAIT;
        }
        else if (command == BUZZER_HALF_CMD)
        {
            buzzer_timer_reload_mark = TT_BUZZER_BIP_HALF;
            buzzer_timer_reload_space = TT_BUZZER_BIP_HALF_WAIT;
        }
        else
        {
            buzzer_timer_reload_mark = TT_BUZZER_BIP_SHORT;
            buzzer_timer_reload_space = TT_BUZZER_BIP_SHORT_WAIT;
        }

        buzzer_state = BUZZER_MARK;
        buzzer_timeout = 0;
        buzzer_multiple = multiple;
    }
}


void UpdateBuzzer (void)
{
    switch (buzzer_state)
    {
        case BUZZER_WAIT_COMMANDS:
            break;

        case BUZZER_MARK:
            if (!buzzer_timeout)
            {
                BUZZER_ON;
                buzzer_state++;
                buzzer_timeout = buzzer_timer_reload_mark;
            }
            break;

        case BUZZER_SPACE:
            if (!buzzer_timeout)
            {
                if (buzzer_multiple > 1)
                {
                    buzzer_multiple--;
                    BUZZER_OFF;
                    buzzer_state = BUZZER_MARK;
                    buzzer_timeout = buzzer_timer_reload_space;
                }
                else
                    buzzer_state = BUZZER_TO_STOP;
            }
            break;

        case BUZZER_TO_STOP:
        default:
            BUZZER_OFF;
            buzzer_state = BUZZER_WAIT_COMMANDS;
            break;
    }
}


volatile unsigned char enc_clk_cntr = 0;
volatile unsigned char enc_dt_cntr = 0;
void UpdateEncoderFilters (void)
{
    if (ENC_CLK)
    {
        if (enc_clk_cntr < ENCODER_COUNTER_ROOF)
            enc_clk_cntr++;
    }
    else
    {
        if (enc_clk_cntr)
            enc_clk_cntr--;        
    }

    if (ENC_DT)
    {
        if (enc_dt_cntr < ENCODER_COUNTER_ROOF)
            enc_dt_cntr++;
    }
    else
    {
        if (enc_dt_cntr)
            enc_dt_cntr--;        
    }
}


void HARD_Timeouts (void)
{
    if (timer_led)
        timer_led--;

    if (switches_timer)
        switches_timer--;
    
    if (buzzer_timeout)
        buzzer_timeout--;

    UpdateEncoderFilters();
    
}





unsigned short sw_o3_cntr = 0;
unsigned short sw_set_cntr = 0;

resp_sw_t CheckO3 (void)
{
    resp_sw_t sw = SW_NO;
    
    if (sw_o3_cntr > SWITCHES_THRESHOLD_FULL)
        sw = SW_FULL;
    else if (sw_o3_cntr > SWITCHES_THRESHOLD_HALF)
        sw = SW_HALF;
    else if (sw_o3_cntr > SWITCHES_THRESHOLD_MIN)
        sw = SW_MIN;

    return sw;    
}


resp_sw_t CheckSET (void)
{
    resp_sw_t sw = SW_NO;
    
    if (sw_set_cntr > SWITCHES_THRESHOLD_FULL)
        sw = SW_FULL;
    else if (sw_set_cntr > SWITCHES_THRESHOLD_HALF)
        sw = SW_HALF;
    else if (sw_set_cntr > SWITCHES_THRESHOLD_MIN)
        sw = SW_MIN;

    return sw;    
}


void UpdateSwitches (void)
{
    if (!switches_timer)
    {
        if (SWITCH_O3)
            sw_o3_cntr++;
        else if (sw_o3_cntr > 50)
            sw_o3_cntr -= 50;
        else if (sw_o3_cntr > 10)
            sw_o3_cntr -= 5;
        else if (sw_o3_cntr)
            sw_o3_cntr--;

        if (SWITCH_SET)
            sw_set_cntr++;
        else if (sw_set_cntr > 50)
            sw_set_cntr -= 50;
        else if (sw_set_cntr > 10)
            sw_set_cntr -= 5;
        else if (sw_set_cntr)
            sw_set_cntr--;
        
        switches_timer = SWITCHES_TIMER_RELOAD;
    }       
}


unsigned char last_clk = 0;
unsigned char encoder_ccw = 0;
unsigned char encoder_cw = 0;
void UpdateEncoder (void)
{
    unsigned char current_clk = 0;

    //check if we have rising edge on clk
    if (enc_clk_cntr > ENCODER_COUNTER_THRESHOLD)
        current_clk = 1;
    else
        current_clk = 0;
    
    if ((last_clk == 0) && (current_clk == 1))    //rising edge
    {
        //have a new clock edge
        if (enc_dt_cntr > ENCODER_COUNTER_THRESHOLD)
        {
            //CW
            if (encoder_cw < 1)
                encoder_cw++;
        }
        else
        {
            //CCW
            if (encoder_ccw < 1)
                encoder_ccw++;
        }
    }

    if (last_clk != current_clk)
        last_clk = current_clk;
}




unsigned char CheckCCW (void)
{
    unsigned char a = 0;
    
    if (encoder_ccw)
    {
        encoder_ccw--;
        a = 1;
    }
    
    return a;
}


unsigned char CheckCW (void)
{
    unsigned char a = 0;
    
    if (encoder_cw)
    {
        encoder_cw--;
        a = 1;
    }
    
    return a;
}


#define TT_DELAYED_OFF		3600		//para relay placa redonda
#define TT_DELAYED_ON		4560		//para relay placa redonda
#define TT_RELAY			60		//timeout de espera antes de pegar o despegar el relay

enum Relay_State {

	ST_OFF = 0,
	ST_WAIT_ON,
	ST_DELAYED_ON,
	ST_ON,
	ST_WAIT_OFF,
	ST_DELAYED_OFF

};

volatile unsigned short timer_relay = 0;
enum Relay_State relay_state = ST_OFF;
unsigned char last_edge;

//Pide conectar el relay
void RelayOn (void)
{
    if (!RelayIsOn())
    {
        relay_state = ST_WAIT_ON;
        timer_relay = TT_RELAY;
    }
}

//Pide desconectar el relay
void RelayOff (void)
{
    if (!RelayIsOff())
    {
        relay_state = ST_WAIT_OFF;
        timer_relay = TT_RELAY;
    }
}

//Revisa el estado del relay
unsigned char RelayIsOn (void)
{
    if ((relay_state == ST_WAIT_ON) ||
        (relay_state == ST_DELAYED_ON) ||
        (relay_state == ST_ON))
        return 1;
    else
        return 0;
}

//Revisa el estado del relay
unsigned char RelayIsOff (void)
{
    if ((relay_state == ST_WAIT_OFF) ||
        (relay_state == ST_DELAYED_OFF) ||
        (relay_state == ST_OFF))
        return 1;
    else
        return 0;
}


void UpdateRelayTimeout (void)
{
    if (timer_relay)
        timer_relay--;
}

//chequeo continuo del estado del relay
void UpdateRelay (void)
{
    unsigned char edge = 0;

    // if ((!last_edge) && (SYNC))		//flanco ascendente detector
    // {									//senoidal arriba
    //     last_edge = 1;
    //     SYNCP_ON;
    // }

    // if ((last_edge) && (!SYNC))		//flanco descendente detector
    // {									//senoidal abajo
    //     edge = 1;
    //     last_edge = 0;
    //     SYNCP_OFF;
    // }

    // switch (relay_state)
    // {
    // case ST_OFF:

    //     break;

    // case ST_WAIT_ON:
    //     if (edge)
    //     {
    //         edge = 0;
    //         relay_state = ST_DELAYED_ON;
    //         TIM16->CNT = 0;
    //     }

    //     if (!timer_relay)		//agoto el timer y no encontro sincro, pega igual
    //     {
    //         RELAY_ON;
    //         relay_state = ST_ON;
    //     }
    //     break;

    // case ST_DELAYED_ON:
    //     if (TIM16->CNT > TT_DELAYED_ON)
    //     {
    //         RELAY_ON;
    //         relay_state = ST_ON;
    //     }
    //     break;

    // case ST_ON:

    //     break;

    // case ST_WAIT_OFF:
    //     if (edge)
    //     {
    //         edge = 0;
    //         relay_state = ST_DELAYED_OFF;
    //         TIM16->CNT = 0;
    //     }

    //     if (!timer_relay)		//agoto el timer y no encontro sincro, despega igual
    //     {
    //         RELAY_OFF;
    //         relay_state = ST_OFF;
    //     }

    //     break;

    // case ST_DELAYED_OFF:
    //     if (TIM16->CNT > TT_DELAYED_OFF)
    //     {
    //         RELAY_OFF;
    //         relay_state = ST_OFF;
    //     }
    //     break;

    // default:
    //     RELAY_OFF;
    //     relay_state = ST_OFF;
    //     break;
    // }
}

//--- end of file ---//


