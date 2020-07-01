//----------------------------------------------
// #### PROYECTO GAUSSTEK O3 - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### HARD.H #################################
//----------------------------------------------
#ifndef _HARD_H_
#define _HARD_H_

#include "stm32f0xx.h"


//-- Defines For Configuration -------------------
//---- Configuration for Hardware Versions -------
// #define HARDWARE_VERSION_1_1
#define HARDWARE_VERSION_1_0


// #define SOFTWARE_VERSION_1_1
#define SOFTWARE_VERSION_1_0


//---- Features Configuration -----------------
//---- End of Features Configuration ----------

// Exported Pinout Names -------------------------------------------------------
//GPIOA pin0    
//GPIOA pin1    
//GPIOA pin2    
//GPIOA pin3    Lcd interface

//GPIOA pin4
#define LCD_E    ((GPIOA->ODR & 0x0010) != 0)
#define LCD_E_ON    (GPIOA->BSRR = 0x00000010)
#define LCD_E_OFF    (GPIOA->BSRR = 0x00100000)

//GPIOA pin5    
#define LCD_RS    ((GPIOA->ODR & 0x0020) != 0)
#define LCD_RS_ON    (GPIOA->BSRR = 0x00000020)
#define LCD_RS_OFF    (GPIOA->BSRR = 0x00200000)

//GPIOA pin6
#define AC_SYNC ((GPIOA->IDR & 0x0040) == 0)

//GPIOA pin7
//GPIOB pin0
//GPIOB pin1    

//GPIOA pin8
#define RELAY    ((GPIOA->ODR & 0x0100) != 0)
#define RELAY_ON    (GPIOA->BSRR = 0x00000100)
#define RELAY_OFF    (GPIOA->BSRR = 0x01000000)

//GPIOA pin9    
//GPIOA pin10    
//GPIOA pin11    

//GPIOA pin12
#define LED ((GPIOA->ODR & 0x1000) != 0)
#define LED_ON GPIOA->BSRR = 0x00001000
#define LED_OFF GPIOA->BSRR = 0x10000000

//GPIOA pin13
//GPIOA pin14

//GPIOA pin15    
#define BUZZER ((GPIOA->ODR & 0x8000) != 0)
#define BUZZER_ON GPIOA->BSRR = 0x00008000
#define BUZZER_OFF GPIOA->BSRR = 0x80000000

//GPIOB pin3
#define SWITCH_O3 ((GPIOB->IDR & 0x0008) == 0)

//GPIOB pin4
#define ENC_CLK ((GPIOB->IDR & 0x0010) == 0)

//GPIOB pin5
#define ENC_DT ((GPIOB->IDR & 0x0020) == 0)

//GPIOB pin6
#define ENC_SW ((GPIOB->IDR & 0x0040) == 0)
#define SWITCH_SET    ENC_SW

//GPIOB pin7
#define CTRL_BKL    ((GPIOB->ODR & 0x0080) != 0)
#define CTRL_BKL_ON    (GPIOB->BSRR = 0x00000080)
#define CTRL_BKL_OFF    (GPIOB->BSRR = 0x00800000)


// Exported Types & Macros -----------------------------------------------------
// Main Program States
typedef enum
{    
    MAIN_INIT = 0,
    MAIN_WELCOME,
    MAIN_STAND_BY_0,
    MAIN_STAND_BY_1,
    MAIN_START_TREATMENT,
    MAIN_IN_TREATMENT,
    MAIN_GO_PAUSE,
    MAIN_PAUSED,    
    MAIN_RESUMING,
    MAIN_ENDING_TREATMENT,
    MAIN_IN_MAIN_MENU
    
} main_state_t;


// Buzzer Commands
#define BUZZER_STOP_CMD        0
#define BUZZER_LONG_CMD        1
#define BUZZER_HALF_CMD        2
#define BUZZER_SHORT_CMD        3

// Buzzer Timeouts
#define TT_BUZZER_BIP_SHORT		300
#define TT_BUZZER_BIP_SHORT_WAIT	500
#define TT_BUZZER_BIP_HALF		600
#define TT_BUZZER_BIP_HALF_WAIT    	800
#define TT_BUZZER_BIP_LONG		2000
#define TT_BUZZER_BIP_LONG_WAIT        2000


//Estados Externos de LED BLINKING
#define LED_NO_BLINKING    0
#define LED_TREATMENT_STANDBY    1
#define LED_TREATMENT_GENERATING    2
#define LED_TREATMENT_ERROR    6

typedef enum {
    resp_ok = 0,
    resp_continue,
    resp_selected,
    resp_change,
    resp_change_all_up,
    resp_working,
    resp_error,
    resp_need_to_save,
    resp_finish,
    resp_nok,
    resp_timeout,
    resp_ready,
    resp_no_answer,
    resp_save

} resp_t;

typedef enum {
    SW_NO = 0,
    SW_MIN,
    SW_HALF,
    SW_FULL
    
} resp_sw_t;

#define SWITCHES_TIMER_RELOAD    5
#define SWITCHES_THRESHOLD_FULL	300		//3 segundos
#define SWITCHES_THRESHOLD_HALF	100		//1 segundo
#define SWITCHES_THRESHOLD_MIN	5		//50 ms

#define ENCODER_COUNTER_ROOF    10
#define ENCODER_COUNTER_THRESHOLD    3

//--- Stringtify Utils -----------------------
#define str_macro(s) #s


// Module Exported Functions ---------------------------------------------------
void HARD_Timeouts (void);

void ChangeLed (unsigned char);
void UpdateLed (void);

void UpdateBuzzer (void);
void BuzzerCommands(unsigned char, unsigned char);

resp_sw_t CheckSET (void);
resp_sw_t CheckO3 (void);

void UpdateSwitches (void);

void UpdateEncoder (void);
unsigned char CheckCCW (void);
unsigned char CheckCW (void);

void RelayOn (void);
void RelayOff (void);
unsigned char RelayIsOn (void);
unsigned char RelayIsOff (void);
void UpdateRelay (void);



#endif /* _HARD_H_ */

//--- end of file ---//

