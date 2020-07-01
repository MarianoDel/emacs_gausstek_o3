//-----------------------------------------------
// #### PROJECT: O3 TREATMENT - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### MENUES.H ################################
//-----------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef _MENUES_H_
#define _MENUES_H_

#include "hard.h"
#include "flash_program.h"

// Exported Types --------------------------------------------------------------
#define mem_bkp_t parameters_typedef

// Exported Constants or Macros ------------------------------------------------


// Exported Functions ----------------------------------------------------------
resp_t MENU_Main (mem_bkp_t *);

    
#endif    /* _MENUES_H_ */

//--- end of file ---//

