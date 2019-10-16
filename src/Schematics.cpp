#include "Types.h"

// mystery signals 

bool ANAP;
bool FF41;
bool TOFU;
bool XEHO;
bool WESY;
bool POVA;
bool TADY;

bool SAVY;
bool XODU;
bool TUKY;
bool SYBE;
bool RUTU;

bool T1;
bool T2;
bool CPU_WR_RAW;

bool ABOL_1MHZ;
bool CLKIN_A;
bool CLKIN_B;

bool CLK_16k;
bool CLK_64k;
bool CLK_256k;

bool ATAL_4MHZ;
bool CYBO_4MHZ;
bool AJER_2MHZ;
bool APU_RESET;

bool FERO_Q;
bool UMER;
bool APU_RESET5n;

bool CPU_WR_WAT;
bool CPU_RAW_RD;
bool WR_IN;
bool RD_B;

bool A15_C;

bool FF00RD;
bool FF00WR;
bool FF46;
bool FF47;
bool FF48;
bool FF49;
bool FF04_FF07;

bool FF0F;

bool CPU; // wat?

bool FROM_CPU;
bool FROM_CPU3;
bool FROM_CPU4;
bool FROM_CPU5;
bool FROM_CPU6;
bool FROM_CPU7;
bool FROM_CPU8;
bool FROM_CPU9;
bool FROM_CPU10;

bool SCK_IN;
bool SCK_DIR;
bool SER_OUT;
bool SIN_IN;

bool OAM_A_CS;
bool OAM_ADDR_CPU;
bool OAM_ADDR_PARSE;
bool OAM_ADDR_RENDER;
bool OAM_ADR_CPU;
bool OAM_B_CS;
bool OAM_CLK;

bool RESET;
bool RESET7;
bool RESET_VIDEO2n;

bool MATU_Q;
bool MCS_IN;
bool MOE_IN;
bool MWR_IN;
bool NET02;

bool VAVA3;
bool RUTU_OUT;
bool BURO_Q;

bool MATE;
bool ROXY;
bool PURE;
bool LYRY;
bool MYVO;
bool SYLO;
bool WODU;
bool XYDO;
bool ABEZ;
bool ABUZ;
bool ACYL;
bool AFAS;
bool ANOM;
bool AZYB;
bool BEDO;
bool BESU;
bool BYVA;
bool CEHA;
bool CLK3;
bool COTA;
bool FETO;
bool LEKO;
bool LENA;
bool MOSU;
bool MYMA;
bool NETA;
bool POJU;
bool PORE;
bool POTU;
bool POWY;
bool POXA;
bool POZO;
bool PULO;
bool PYJU;
bool PYZO;
bool RAWU;
bool SELA;
bool TACU;
bool TAVA;
bool TEXO;
bool TEXY;
bool TULY;
bool TUVO;
bool TYVY;
bool VUJO;
bool VYMU;
bool VYNO;
bool WEZA;
bool WUCO;
bool WUME;
bool WUTY;
bool WUZY;
bool WYDA;
bool WYJA;
bool WYSE;
bool XUHA;
bool XUPY;
bool XYMU;
bool ZYSU;

// buses

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

bool OBP0PIXELn;
bool OBP1PIXELn;

bool A0;
bool A1;
bool A2;
bool A3;
bool A4;
bool A5;
bool A6;
bool A7;
bool A8;
bool A9;
bool A10;
bool A11;
bool A12;
bool A13;
bool A14;

bool D0;
bool D1;
bool D2;
bool D3;
bool D4;
bool D5;
bool D6;
bool D7;

bool D0_A;
bool D1_A;
bool D2_A;
bool D3_A;
bool D4_A;
bool D5_A;
bool D6_A;
//bool D7_A; // extcpubuses_sch

bool D0_IN;
bool D1_IN;
bool D2_IN;
bool D3_IN;
bool D4_IN;
bool D5_IN;
bool D6_IN;
bool D7_IN;

bool MD0_A;
bool MD3_A;
bool MD4_A;
bool MD7_A;
bool MD6_A;
bool MD1_A;
bool MD5_A;
bool MD2_A;

bool MD0_IN;
bool MD1_IN;
bool MD2_IN;
bool MD3_IN;
bool MD4_IN;
bool MD5_IN;
bool MD6_IN;
bool MD7_IN;

bool MD0_OUT;
bool MD1_OUT;
bool MD2_OUT;
bool MD3_OUT;
bool MD4_OUT;
bool MD5_OUT;
bool MD6_OUT;
bool MD7_OUT;

bool MD0;
bool MD1;
bool MD2;
bool MD3;
bool MD4;
bool MD5;
bool MD6;
bool MD7;

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

bool OAM_A_D0;
bool OAM_A_D1;
bool OAM_A_D2;
bool OAM_A_D3;
bool OAM_A_D4;
bool OAM_A_D5;
bool OAM_A_D6;
bool OAM_A_D7;

bool OAM_B_D0;
bool OAM_B_D1;
bool OAM_B_D2;
bool OAM_B_D3;
bool OAM_B_D4;
bool OAM_B_D5;
bool OAM_B_D6;
bool OAM_B_D7;

bool DMA_A0;
bool DMA_A1;
bool DMA_A2;
bool DMA_A3;
bool DMA_A4;
bool DMA_A5;
bool DMA_A6;
bool DMA_A7;

bool OAM_A1;
bool OAM_A2;
bool OAM_A3;
bool OAM_A4;
bool OAM_A5;
bool OAM_A6;
bool OAM_A7;
