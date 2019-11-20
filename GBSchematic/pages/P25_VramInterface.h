#pragma once

struct P25_VramInterface {

  /*p25.SYRO*/ bool ADDR_FE00_FFFF;


  /*p25.WUKO*/ bool WIN_TILE_READ;
  /*p25.COTA*/ bool OAM_IN_CLKn;

  /*p25.MYFU*/ bool MA00n;
  /*p25.MASA*/ bool MA01n;
  /*p25.MYRE*/ bool MA02n;
  /*p25.MAVU*/ bool MA03n;
  /*p25.MEPA*/ bool MA04n;
  /*p25.MYSA*/ bool MA05n;
  /*p25.MEWY*/ bool MA06n;
  /*p25.MUME*/ bool MA07n;
  /*p25.VOVA*/ bool MA08n;
  /*p25.VODE*/ bool MA09n;
  /*p25.RUKY*/ bool MA10n;
  /*p25.RUMA*/ bool MA11n;
  /*p25.REHO*/ bool MA12n;

  /*p25.LEXE*/ bool MA00b;
  /*p25.LOZU*/ bool MA01b;
  /*p25.LACA*/ bool MA02b;
  /*p25.LUVO*/ bool MA03b;
  /*p25.LOLY*/ bool MA04b;
  /*p25.LALO*/ bool MA05b;
  /*p25.LEFA*/ bool MA06b;
  /*p25.LUBY*/ bool MA07b;
  /*p25.TUJY*/ bool MA08b;
  /*p25.TAGO*/ bool MA09b;
  /*p25.NUVA*/ bool MA10b;
  /*p25.PEDU*/ bool MA11b;
  /*p25.PONY*/ bool MA12b;

  // guess
  /*p25.CUFE*/ bool OAM_WR;

  /*p25.VAPE*/ bool VAPE;
  /*p25.AVER*/ bool AVER;
  /*p25.XUJY*/ bool XUJY;
  /*p25.BYCU*/ bool OAM_IN_CLK;
  /*p25.TEFA*/ bool TEFA;
  /*p25.SOSE*/ bool ADDR_VRAM;
  /*p25.TUCA*/ bool CPU_VRAM_RD;
  /*p25.TUJA*/ bool CPU_VRAM_WR;
  /*p25.TEGU*/ bool CPU_VRAM_CLK;
  /*p25.TAVY*/ bool MOE_Cn;
  /*p25.SYCY*/ bool MODE_DBG2n;
  /*p25.SOTO*/ bool DBG_SOTO;
  /*p25.TUTO*/ bool DBG_TUTO;
  /*p25.SUDO*/ bool MWR_Cn;
  /*p25.TEFY*/ bool MCS_Cn;
  
  /*p25.TOLE*/ bool CPU_VRAM_RD2;
  /*p25.TYJY*/ bool CPU_VRAM_WR2;
  /*p25.SALE*/ bool CPU_VRAM_CLK2;

  /*p25.RENA*/ bool MD_IN;
  /*p25.RYJE*/ bool MD_INb;
  /*p25.RAHU*/ bool D_TO_MD;

  /*p25.ROFA*/ bool MD_OUT;
  /*p25.RELA*/ bool MD_OUTb;
  /*p25.REVO*/ bool MD_OUTc;
  /*p25.SAZO*/ bool MD_OUTd;
  /*p25.ROCY*/ bool MD_OUTe;

  /*p25.ROPY*/ bool ROPY;
  /*p25.SERE*/ bool SERE;
  /*p25.RUVY*/ bool CPU_VRAM_CLK2n;
  /*p25.SOHY*/ bool MWR;

  /*p25.RYLU*/ bool RYLU;
  /*p25.SOHO*/ bool SOHO;
  /*p25.RAWA*/ bool RAWA;
  /*p25.APAM*/ bool APAM;
  /*p25.SUTU*/ bool MCS;
  /*p25.RACU*/ bool MOE;
  /*p25.RACO*/ bool DBG_TUTOn;
  /*p25.RUTE*/ bool MOE_Dn;
  /*p25.SEWO*/ bool MCS_Dn;
  /*p25.TODE*/ bool MCS_An;
  /*p25.SEMA*/ bool MOE_An;
  /*p25.TAXY*/ bool MWR_An;
  /*p25.SOFY*/ bool MWR_Dn;
  /*p25.SAHA*/ bool MOE_D;
  /*p25.SETY*/ bool MCS_D;
  /*p25.SOKY*/ bool MCS_A;
  /*p25.REFO*/ bool MOE_A;
  /*p25.SYSY*/ bool MWR_A;
  /*p25.RAGU*/ bool MWR_D;

  // something debug
  /*p25.TUSO*/ bool TUSO;
  /*p25.SOLE*/ bool SOLE;
  /*p25.????*/ bool P10_Bn;

  /*p25.SYNU*/ bool SYNU;
  /*p25.SYMA*/ bool SYMA;
  /*p25.ROKO*/ bool ROKO;
  /*p25.SYBU*/ bool SYBU;
  /*p25.SAKO*/ bool SAKO;
  /*p25.SEJY*/ bool SEJY;
  /*p25.SEDO*/ bool SEDO;
  /*p25.SAWU*/ bool SAWU;
  /*p25.RURA*/ bool RURA;
  /*p25.RULY*/ bool RULY;
  /*p25.RARE*/ bool RARE;
  /*p25.RODU*/ bool RODU;
  /*p25.RUBE*/ bool RUBE;
  /*p25.RUMU*/ bool RUMU;
  /*p25.RYTY*/ bool RYTY;
  /*p25.RADY*/ bool RADY;

  /*p25.ROVE*/ bool D_TO_MDn;

  /*p25.SEFA*/ bool SEFA_00;
  /*p25.SOGO*/ bool SOGO_01;
  /*p25.SEFU*/ bool SEFU_02;
  /*p25.SUNA*/ bool SUNA_03;
  /*p25.SUMO*/ bool SUMO_04;
  /*p25.SAZU*/ bool SAZU_05;
  /*p25.SAMO*/ bool SAMO_06;
  /*p25.SUKE*/ bool SUKE_07;

  /*p25.REGE*/ bool REGE_00;
  /*p25.RYKY*/ bool RYKY_01;
  /*p25.RAZO*/ bool RAZO_02;
  /*p25.RADA*/ bool RADA_03;
  /*p25.RYRO*/ bool RYRO_04;
  /*p25.REVU*/ bool REVU_05;
  /*p25.REKU*/ bool REKU_06;
  /*p25.RYZE*/ bool RYZE_07;

  /*p25.CEDE*/ bool DO_DMAn;

  /*p25.RALO*/ bool D0n;
  /*p25.TUNE*/ bool D1n;
  /*p25.SERA*/ bool D2n;
  /*p25.TENU*/ bool D3n;
  /*p25.SYSA*/ bool D4n;
  /*p25.SUGY*/ bool D5n;
  /*p25.TUBE*/ bool D6n;
  /*p25.SYZO*/ bool D7n;

  /*p25.XANE*/ bool A_TO_MAn;
  /*p25.XEDU*/ bool A_TO_MA;

  /*p25.TYVY*/ bool MD_TO_Dn;
  /*p25.SEBY*/ bool MD_TO_D;

  /*p25.RERY*/ bool RERY_00;
  /*p25.RUNA*/ bool RUNA_01;
  /*p25.RONA*/ bool RONA_02;
  /*p25.RUNO*/ bool RUNO_03;
  /*p25.SANA*/ bool SANA_04;
  /*p25.RORO*/ bool RORO_05;
  /*p25.RABO*/ bool RABO_06;
  /*p25.SAME*/ bool SAME_07;

  /*p25.XUCY*/ bool XUCY;
  /*p25.XEZE*/ bool WIN_TILE_READn;
  /*p25.VUZA*/ bool VUZA;
};


