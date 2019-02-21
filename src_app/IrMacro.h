
#include "Define.h"

#ifndef _IRMACRO_H_
#define _IRMACRO_H_


#define IR_MACRO_TBL_MAX        (16)
#define IR_MACRO_KEY_MAX        (20)

#define MACRO_NVRAM_SIZE        (128)

/* variables */

/* functions  */
GLOBAL void IrMacoKeyScan(void);
GLOBAL void StartMacro(void);
GLOBAL void StopMacro(void);
GLOBAL void Macrotest(void);
GLOBAL void IrMacroRAMIni(Byte bIndex);
GLOBAL void Ir_MacroProc(void);

#endif

