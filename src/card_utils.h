//------------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CARD_UTILS.H #############################
//------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef _CARD_UTILS_H_
#define _CARD_UTILS_H_


// Exported Types Constants and Macros -----------------------------------------
typedef struct {
    union {
        unsigned char uid_bytes [4];
        unsigned int uid;
    };
    unsigned char bcc;
    unsigned char type;
    int sessions_left;
    int sessions_orig;
    
} card_data_t;

// Status enumeration, Used with most functions
#define MI_OK			0
#define MI_NOTAGERR		1
#define MI_ERR			2


// Exported Functions ----------------------------------------------------------
unsigned char Card_ProcessDataString (unsigned char *, card_data_t *);
unsigned char Card_CreateDataString (unsigned char *, card_data_t *);
unsigned char Card_CompareDataString (unsigned char *, card_data_t *);

void Card_ShowCardIdent (card_data_t *);
void Card_ShowCardData (card_data_t *);

#endif    /* _CARD_UTILS_H_ */

//--- end of file ---//

