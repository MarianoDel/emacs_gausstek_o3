//------------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CARD_UTILS.C #############################
//------------------------------------------------

// Includes --------------------------------------------------------------------
#include "card_utils.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Module Private Types & Macros -----------------------------------------------


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------

// procesa un bloque de 16bytes
unsigned char Card_ProcessDataString (unsigned char * str, card_data_t * data)
{
    unsigned char status = MI_ERR;
    char * wstr;
    wstr = (char *) str;

    if ((*(wstr+0) == 'S') &&
        (*(wstr+1) == ':') &&
        (*(wstr+5) == ':') &&
        (*(wstr+9) == ':'))
    {
        char left [4] = { 0 };
        char orig [4] = { 0 };        
        strncpy(left, (wstr+2), 3);
        strncpy(orig, (wstr+6), 3);
        data->sessions_left = atoi(left);
        data->sessions_orig = atoi(orig);

        //sanity checks
        if ((data->sessions_left >= 0) &&
            (data->sessions_left < 1000) &&
            (data->sessions_orig > 0) &&
            (data->sessions_orig < 1000))
        {
            if (data->sessions_left <= data->sessions_orig)
                status = MI_OK;
        }
    }

    return status;
}


unsigned char Card_CreateDataString (unsigned char * str, card_data_t * data)
{
    unsigned char status = MI_ERR;

    char * wstr;
    wstr = (char *) str;
    
    //sanity checks
    if ((data->sessions_left >= 0) &&
        (data->sessions_left < 1000) &&
        (data->sessions_orig > 0) &&
        (data->sessions_orig < 1000))
    {
        if (data->sessions_left <= data->sessions_orig)
        {
            sprintf(wstr, "S:%03d:%03d:", data->sessions_left, data->sessions_orig);
            status = MI_OK;
        }
    }

    return status;
}


unsigned char Card_CompareDataString (unsigned char * str, card_data_t * data)
{
    unsigned char status = MI_ERR;

    card_data_t card_to_cmp;

    status = Card_ProcessDataString(str, &card_to_cmp);
    if (status == MI_OK)
    {
        if ((data->sessions_orig != card_to_cmp.sessions_orig) ||
            (data->sessions_left != card_to_cmp.sessions_left))
            status = MI_ERR;
    }

    return status;
}

//--- end of file ---//
    
