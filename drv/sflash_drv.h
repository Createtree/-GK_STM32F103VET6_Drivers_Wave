
#ifndef __SFLASH_DRV_H__
#define __SFLASH_DRV_H__

#include "ws_main.h"



 void SFlash_Init(void);
 void SFlash_Cs_EnUfm(void);
 void SFlash_Cs_EnDataF(void);
 void SFlash_Cs_AllUn(void);
 uint8_t SFlash_SpiRW(uint8_t d);






#endif 

