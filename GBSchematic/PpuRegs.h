#pragma once
#include "Schematics.h"

extern bool CLKPIPE;

// screen y coordinate
extern bool V0, V1, V2, V3, V4, V5, V6, V7;

// screen x coordinate
extern bool ACAM, AZUB, AMEL, AHAL, APUX, ABEF, ADAZ, ASAH;

// sprite pixels
extern bool LESY, LOTA, LYKU, ROBY, TYTA, TYCO, SOKA, XOVU; 

// background pixel pipe?
extern reg VEZO, WURU, VOSA, WYFU, XETE, WODA, VUMO, VAVA;

// tile reg?
extern reg RAWU, POZO, PYZO, POXA, PULO, POJU, POWY, PYJU;

// tile latch?
extern reg LEGU, NUDU, MUKU, LUZO, MEGU, MYJY, NASA, NEFO;

// bg_pix_b
extern reg TOMY, TACA, SADY, RYSA, SOBO, SETU, RALU, SOHU;

// bg_pix_a
extern reg MYDE, NOZO, MOJU, MACU, NEPO, MODU, NEDA, PYBO;

// FF47 BGP
extern reg MENA,MORU,MAXY,NUSY,MOGY,MUKE,PYLU,PAVO;

// FF48 OBP0
extern reg XANA,XYZE,XALO,XUKY,XUPO,XERU,XOVA,XUFU;

// FF49 OBP1
extern reg LUXO,LUGU,LOSE,LAWO,LEPU,LUNE,MOSA,MOXY;

extern bool BGPIXELn;

extern bool OBP0PIXELn;
extern bool OBP1PIXELn;

extern bool SPR_PIX_A_0;
extern bool SPR_PIX_A_1;
extern bool SPR_PIX_A_2;
extern bool SPR_PIX_A_3;
extern bool SPR_PIX_A_4;
extern bool SPR_PIX_A_5;
extern bool SPR_PIX_A_6;
extern bool SPR_PIX_A_7;

extern bool SPR_PIX_B_0;
extern bool SPR_PIX_B_1;
extern bool SPR_PIX_B_2;
extern bool SPR_PIX_B_3;
extern bool SPR_PIX_B_4;
extern bool SPR_PIX_B_5;
extern bool SPR_PIX_B_6;
extern bool SPR_PIX_B_7;

extern bool BG_PIX_A_0;
extern bool BG_PIX_A_1;
extern bool BG_PIX_A_2;
extern bool BG_PIX_A_3;
extern bool BG_PIX_A_4;
extern bool BG_PIX_A_5;
extern bool BG_PIX_A_6;
extern bool BG_PIX_A_7;

extern bool BG_PIX_B_0;
extern bool BG_PIX_B_1;
extern bool BG_PIX_B_2;
extern bool BG_PIX_B_3;
extern bool BG_PIX_B_4;
extern bool BG_PIX_B_5;
extern bool BG_PIX_B_6;
extern bool BG_PIX_B_7;

extern bool BGP_D0;
extern bool BGP_D1;
extern bool BGP_D2;
extern bool BGP_D3;
extern bool BGP_D4;
extern bool BGP_D5;
extern bool BGP_D6;
extern bool BGP_D7;

extern bool OBP0_D0;
extern bool OBP0_D1;
extern bool OBP0_D2;
extern bool OBP0_D3;
extern bool OBP0_D4;
extern bool OBP0_D5;
extern bool OBP0_D6;
extern bool OBP0_D7;

extern bool OBP1_D0;
extern bool OBP1_D1;
extern bool OBP1_D2;
extern bool OBP1_D3;
extern bool OBP1_D4;
extern bool OBP1_D5;
extern bool OBP1_D6;
extern bool OBP1_D7;
