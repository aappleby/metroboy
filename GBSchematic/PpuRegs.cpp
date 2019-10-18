#include "PpuRegs.h"

bool CLKPIPE;

// screen y coordinate
bool V0, V1, V2, V3, V4, V5, V6, V7;

// screen x coordinate
bool ACAM, AZUB, AMEL, AHAL, APUX, ABEF, ADAZ, ASAH;

// sprite pixels
bool LESY, LOTA, LYKU, ROBY, TYTA, TYCO, SOKA, XOVU; 

// background pixel pipe?
reg VEZO, WURU, VOSA, WYFU, XETE, WODA, VUMO, VAVA;

// tile reg?
reg RAWU, POZO, PYZO, POXA, PULO, POJU, POWY, PYJU;

// tile latch?
reg LEGU, NUDU, MUKU, LUZO, MEGU, MYJY, NASA, NEFO;

// bg_pix_b
reg TOMY, TACA, SADY, RYSA, SOBO, SETU, RALU, SOHU;

// bg_pix_a
reg MYDE, NOZO, MOJU, MACU, NEPO, MODU, NEDA, PYBO;

// FF47 BGP
reg MENA,MORU,MAXY,NUSY,MOGY,MUKE,PYLU,PAVO;

// FF48 OBP0
reg XANA,XYZE,XALO,XUKY,XUPO,XERU,XOVA,XUFU;

// FF49 OBP1
reg LUXO,LUGU,LOSE,LAWO,LEPU,LUNE,MOSA,MOXY;

bool BGPIXELn;

bool OBP0PIXELn;
bool OBP1PIXELn;

bool SPR_PIX_A_0;
bool SPR_PIX_A_1;
bool SPR_PIX_A_2;
bool SPR_PIX_A_3;
bool SPR_PIX_A_4;
bool SPR_PIX_A_5;
bool SPR_PIX_A_6;
bool SPR_PIX_A_7;

bool SPR_PIX_B_0;
bool SPR_PIX_B_1;
bool SPR_PIX_B_2;
bool SPR_PIX_B_3;
bool SPR_PIX_B_4;
bool SPR_PIX_B_5;
bool SPR_PIX_B_6;
bool SPR_PIX_B_7;

bool BG_PIX_A_0;
bool BG_PIX_A_1;
bool BG_PIX_A_2;
bool BG_PIX_A_3;
bool BG_PIX_A_4;
bool BG_PIX_A_5;
bool BG_PIX_A_6;
bool BG_PIX_A_7;

bool BG_PIX_B_0;
bool BG_PIX_B_1;
bool BG_PIX_B_2;
bool BG_PIX_B_3;
bool BG_PIX_B_4;
bool BG_PIX_B_5;
bool BG_PIX_B_6;
bool BG_PIX_B_7;

bool BGP_D0;
bool BGP_D1;
bool BGP_D2;
bool BGP_D3;
bool BGP_D4;
bool BGP_D5;
bool BGP_D6;
bool BGP_D7;

bool OBP0_D0;
bool OBP0_D1;
bool OBP0_D2;
bool OBP0_D3;
bool OBP0_D4;
bool OBP0_D5;
bool OBP0_D6;
bool OBP0_D7;

bool OBP1_D0;
bool OBP1_D1;
bool OBP1_D2;
bool OBP1_D3;
bool OBP1_D4;
bool OBP1_D5;
bool OBP1_D6;
bool OBP1_D7;
