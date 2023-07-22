//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### PARAMETERS.H ##########################
//---------------------------------------------

#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

//-- Configuration for Struct --------------------
typedef struct {

    unsigned short treatment_time_min;    //2
    unsigned short ticker_time;    //4
    unsigned char alarms_onoff;    //5
    unsigned char ticker_onoff;    //6

    unsigned char operation_mode;    //7
    unsigned char dummy1;    //8
    
    unsigned int saved_psw;    //12 align 8
    unsigned int new_psw;    //16

    
    // unsigned char dummy1;    //16
    // unsigned char dummy2;    //16
    // unsigned char dummy3;    //17


} parameters_typedef;

typedef enum {
    MODE_UNKNOW = 0,
    NORMAL_MODE,
    CARD_MODE
    
} operation_modes_t;

//-- End of Memory Struct to Save --------------------

#endif    /* _PARAMETERS_H_ */

//--- end of file ---//

