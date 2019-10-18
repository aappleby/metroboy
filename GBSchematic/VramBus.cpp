#include "VramBus.h"

bool MCS_A, MCS_D, MCS_IN;
bool MOE_A, MOE_D, MOE_IN;
bool MWR_A, MWR_D, MWR_IN;

// not gpio, i guess?
bool MA0;
bool MA1;
bool MA2;
bool MA3;
bool MA4;
bool MA5;
bool MA7;
bool MA8;
bool MA9;
bool MA6;
bool MA10;
bool MA11;
bool MA12;

// chip-to-vram data bus
bool MD0_A, MD0_OUT, MD0_IN, MD0;
bool MD1_A, MD1_OUT, MD1_IN, MD1;
bool MD2_A, MD2_OUT, MD2_IN, MD2;
bool MD3_A, MD3_OUT, MD3_IN, MD3;
bool MD4_A, MD4_OUT, MD4_IN, MD4;
bool MD5_A, MD5_OUT, MD5_IN, MD5;
bool MD6_A, MD6_OUT, MD6_IN, MD6;
bool MD7_A, MD7_OUT, MD7_IN, MD7;
