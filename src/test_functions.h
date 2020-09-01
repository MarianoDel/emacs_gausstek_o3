//------------------------------------------------
// #### PROJECT: O3 TREATMENT - Custom Board #####
// ## Internal Test Functions Module
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### TEST_FUNCTIONS.H #########################
//------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef _TEST_FUNTIONS_H_
#define _TEST_FUNTIONS_H_

#include "hard.h"

// Exported Types --------------------------------------------------------------


// Exported Constants or Macros ------------------------------------------------


// Exported Functions ----------------------------------------------------------
void TF_Led (void);
void TF_Buzzer (void);
void TF_Buzzer_Functions (void);
void TF_lcdE (void);
void TF_lcdRS (void);
void TF_lcdBklight (void);
void TF_lcdData (void);
void TF_lcdBlink (void);
void TF_lcdScroll (void);
void TF_lcdBigNumbers (void);
void TF_switchO3 (void);
void TF_switchSET (void);
void TF_zcd (void);
void TF_zcd_by_int (void);
void TF_Relay (void);
void TF_RelayBoardOutputs (void);

#endif    /* _TEST_FUNTIONS_H_ */

//--- end of file ---//

