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

// FF40 LCDC
extern reg XYLO,XAFO,XYMO,XONA,WYMO,WEXU,WOKY,VYXE;

// FF42 SCY
extern reg FEZU,FUNY,DEDE,FOTY,FOHA,FYMO,GAVE,FUJO;

// FF43 SCX
extern reg CYXU,BAKE,BEMY,CUZY,CABU,DUZU,DATY,GUBO;

// FF45 LYC
extern reg SOTA,SYRY,SALO,SEDY,VEVO,VAFA,VUCE,RAHA;

// FF4A WY
extern reg NAFU,MELA,NYRO,NAGA,NULO,NUKA,NENE,NESO;

// FF4B WX
extern reg MYCE,MYPA,NOFE,NUKU,MYPU,MUVO,MEBY,NOKE;


// FF40 LCDC
extern bool FF40_D0;
extern bool FF40_D1;
extern bool FF40_D2;
extern bool FF40_D3;
extern bool FF40_D4;
extern bool FF40_D5;
extern bool FF40_D6;
extern bool FF40_D7;

// FF42 SCY
extern bool FF42_D0;
extern bool FF42_D1;
extern bool FF42_D2;
extern bool FF42_D3;
extern bool FF42_D4;
extern bool FF42_D5;
extern bool FF42_D6;
extern bool FF42_D7;

// FF43 SCX
extern bool FF43_D0;
extern bool FF43_D1;
extern bool FF43_D2;
extern bool FF43_D3;
extern bool FF43_D4;
extern bool FF43_D5;
extern bool FF43_D6;
extern bool FF43_D7;

// FF45 LYC
extern bool FF45_D0;
extern bool FF45_D1;
extern bool FF45_D2;
extern bool FF45_D3;
extern bool FF45_D4;
extern bool FF45_D5;
extern bool FF45_D6;
extern bool FF45_D7;

// FF4A WY
extern bool FF4A_D0;
extern bool FF4A_D1;
extern bool FF4A_D2;
extern bool FF4A_D3;
extern bool FF4A_D4;
extern bool FF4A_D5;
extern bool FF4A_D6;
extern bool FF4A_D7;

// FF4B WX
extern bool FF4B_D0;
extern bool FF4B_D1;
extern bool FF4B_D2;
extern bool FF4B_D3;
extern bool FF4B_D4;
extern bool FF4B_D5;
extern bool FF4B_D6;
extern bool FF4B_D7;



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
