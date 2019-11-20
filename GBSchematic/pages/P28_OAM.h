#pragma once

struct P28_OAM {
public:

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  /*p28.BODE*/ union { bool CLK3; };
  /*p28.ZODO*/ union { bool OAM_CLK; };
  /*p28.BETE*/ union { bool OAM_ADDR_RENDER; };
  /*p28.APAR*/ union { bool OAM_ADDR_PARSE; };
  /*p28.ASAM*/ union { bool OAM_ADDR_CPU; };
  /*p28.ZONE*/ union { bool OAM_A_CS; };
  /*p28.ZOFE*/ union { bool OAM_B_CS; };
  /*p28.YZET*/ union { bool OAM_A7; };
  /*p28.XEMU*/ union { bool OAM_A6; };
  /*p28.YMEV*/ union { bool OAM_A5; };
  /*p28.YVOM*/ union { bool OAM_A4; };
  /*p28.YFOC*/ union { bool OAM_A3; };
  /*p28.YFOT*/ union { bool OAM_A2; };
  /*p28.ZYFO*/ union { bool OAM_A1; };
  /*p28.AMAB*/ bool AMAB;
  /*p28.LEKO*/ bool LEKO;
  /*p28.ATEJ*/ bool ATEJ;
  /*p28.ACYL*/ bool ACYL;
  /*p28.WEFE*/ bool P10_Bn;
  /*p28.WEWU*/ bool OAM_A_CPU_RD;

  /*p28.WUME*/ bool OAM_B_LATCH_TO_D;
  /*p28.BESU*/ bool BESU;
  /*p28.ANOM*/ bool SCAN_RSTn;
  /*p28.FETO*/ bool SCAN_DONE;
  /*p28.BYVA*/ bool BYVA;
  /*p28.AZYB*/ bool AZYB;

  /*p28.XUTO*/ bool OAM_WR;
  
  /*p28.YVAL*/ bool YVAL;
  /*p28.YRYV*/ bool YRYV;

  /*p28.AWOH*/ bool CLK_2Mn;
  /*p28.ABAF*/ bool IN_FRAMEn;
  /*p28.ANEL*/ bool ANEL;
  /*p28.BYHA*/ bool BYHA;
  /*p28.ABAK*/ bool ABAK;
  /*p28.GAVA*/ bool SCAN_CLK;

  /*p28.YFEL*/ bool SCAN0;
  /*p28.WEWY*/ bool SCAN1;
  /*p28.GOSO*/ bool SCAN2;
  /*p28.ELYN*/ bool SCAN3;
  /*p28.FAHA*/ bool SCAN4;
  /*p28.FONY*/ bool SCAN5;

  /*p28.ASEN*/ bool ASEN;
  /*p28.BOGE*/ bool DMA_RUNNING_SYNCn;
  /*p28.AJON*/ bool AJON;
  /*p28.AJUJ*/ bool AJUJ;
  /*p28.XYNY*/ bool XYNY;
  /*p28.ADAH*/ bool ADDR_OAMn;
  /*p28.WUJE*/ bool WUJE;
  /*p28.XUPA*/ bool XUPA;
  
  /*p28.APAG*/ bool D_TO_OAMDn;
  /*p28.WEFY*/ bool WEFY;
  /*p28.BOFE*/ bool BOFE;
  /*p28.AJEP*/ bool AJEP;
  /*p28.XUJA*/ bool XUJA;
  /*p28.BOTA*/ bool BOTA;
  /*p28.ASYT*/ bool ASYT;
  /*p28.WARU*/ bool WARU;
  /*p28.AZUL*/ bool D_TO_OAMD;

  /*p28.AZAR*/ bool VRAM_TO_OAMc;

  /*p28.GARO*/ bool A0n;
  /*p28.WACU*/ bool A1n;
  /*p28.GOSE*/ bool A2n;
  /*p28.WAPE*/ bool A3n;
  /*p28.FEVU*/ bool A4n;
  /*p28.GERA*/ bool A5n;
  /*p28.WAXA*/ bool A6n;
  /*p28.FOBY*/ bool A7n;

  /*p28.FYKE*/ bool IDX_0n;
  /*p28.FUGU*/ bool IDX_1n;
  /*p28.FACO*/ bool IDX_2n;
  /*p28.FABY*/ bool IDX_3n;
  /*p28.GYKA*/ bool IDX_4n;
  /*p28.GYBU*/ bool IDX_5n;

  /*p28.WYDU*/ bool WYDU;
  /*p28.GECA*/ bool GECA;
  
  /*p28.WUWE*/ bool SCANZn;
  /*p28.GUSE*/ bool SCAN0n;
  /*p28.GEMA*/ bool SCAN1n;
  /*p28.FUTO*/ bool SCAN2n;
  /*p28.FAKU*/ bool SCAN3n;
  /*p28.GAMA*/ bool SCAN4n;
  /*p28.GOBY*/ bool SCAN5n;



  /*p28.GEFY*/ bool GEFY;

  /*p28.FETU*/ bool DMA_A07n;
  /*p28.FYDU*/ bool DMA_A06n;
  /*p28.EDOL*/ bool DMA_A05n;
  /*p28.ELUG*/ bool DMA_A04n;
  /*p28.FYKY*/ bool DMA_A03n;
  /*p28.FAGO*/ bool DMA_A02n;
  /*p28.FESA*/ bool DMA_A01n;
  /*p28.FODO*/ bool DMA_A00n;

  /*p28.GEKA*/ bool OAM_A0;
  /*p28.MYNU*/ bool MYNU;
  /*p28.WAFO*/ bool OAM_A0n;
  /*p28.GUKO*/ bool OAM_B_LATCH_TO_Dn;
  /*p28.WUKU*/ bool OAM_A_CPU_RDn;
  /*p28.YLYC*/ bool OAM_A_CSn;
  /*p28.YNYC*/ bool OAM_B_CSn;

  // unused
  //bool XUCA,XECY,XUVA;
};
