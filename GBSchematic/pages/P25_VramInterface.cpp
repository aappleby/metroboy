#include "P25_VramInterface.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P25_VramInterface_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // MAXX pin driver

  c.p25.MA00n = not(b.MA00);
  c.p25.MA01n = not(b.MA01);
  c.p25.MA02n = not(b.MA02);
  c.p25.MA03n = not(b.MA03);
  c.p25.MA04n = not(b.MA04);
  c.p25.MA05n = not(b.MA05);
  c.p25.MA06n = not(b.MA06);
  c.p25.MA07n = not(b.MA07);
  c.p25.MA08n = not(b.MA08);
  c.p25.MA09n = not(b.MA09);
  c.p25.MA10n = not(b.MA10);
  c.p25.MA11n = not(b.MA11);
  c.p25.MA12n = not(b.MA12);

  c.p25.MA00b = not(c.p25.MA00n);
  c.p25.MA01b = not(c.p25.MA01n);
  c.p25.MA02b = not(c.p25.MA02n);
  c.p25.MA03b = not(c.p25.MA03n);
  c.p25.MA04b = not(c.p25.MA04n);
  c.p25.MA05b = not(c.p25.MA05n);
  c.p25.MA06b = not(c.p25.MA06n);
  c.p25.MA07b = not(c.p25.MA07n);
  c.p25.MA08b = not(c.p25.MA08n);
  c.p25.MA09b = not(c.p25.MA09n);
  c.p25.MA10b = not(c.p25.MA10n);
  c.p25.MA11b = not(c.p25.MA11n);
  c.p25.MA12b = not(c.p25.MA12n);

  c.chip.MA00 = c.p25.MA00b;
  c.chip.MA01 = c.p25.MA01b;
  c.chip.MA02 = c.p25.MA02b;
  c.chip.MA03 = c.p25.MA03b;
  c.chip.MA04 = c.p25.MA04b;
  c.chip.MA05 = c.p25.MA05b;
  c.chip.MA06 = c.p25.MA06b;
  c.chip.MA07 = c.p25.MA07b;
  c.chip.MA08 = c.p25.MA08b;
  c.chip.MA09 = c.p25.MA09b;
  c.chip.MA10 = c.p25.MA10b;
  c.chip.MA11 = c.p25.MA11b;
  c.chip.MA12 = c.p25.MA12b;

  //----------

  // not sure this makes sense as or(and(a,b),c)...
  c.p25.CUFE = or(and(b.sys.ADDR_OAM, b.sys.DMA_RUNNING_SYNC), b.sys.CPUCLK_ABCDxxxx9);
  c.p25.VAPE = and(b.p29.TACU, b.p29.TUVO);
  c.p25.AVER = and(b.p28.ACYL, b.p29.XYSO);
  c.p25.XUJY = not(b.p25.VAPE);
  c.p25.BYCU = nor(b.p25.CUFE, b.p25.XUJY, b.p25.AVER);
  c.p25.COTA = not(b.p25.BYCU);

  //----------



  /*p25.SYCY*/ c.p25.MODE_DBG2n = not(b.sys.MODE_DBG2);
  /*p25.SOTO*/ c.p25.DBG_SOTO   = tock_pos(a.p25.MODE_DBG2n, b.p25.MODE_DBG2n, b.sys.SYS_RESETn2, b.p25.DBG_SOTO, !b.p25.DBG_SOTO);
  /*p25.TUTO*/ c.p25.DBG_TUTO   = and(b.sys.MODE_DBG2, !b.p25.DBG_SOTO);
  /*p25.RACO*/ c.p25.DBG_TUTOn  = not(b.p25.DBG_TUTO);

  c.p25.ADDR_FE00_FFFF = not(b.sys.ADDR_0000_FE00);
  c.p25.TEFA = nor(b.p25.ADDR_FE00_FFFF, b.sys.ADDR_VALID_AND_NOT_VRAM);
  c.p25.SOSE = and(b.A15, b.p25.TEFA);

  // guess
  /*p25.TUCA*/ c.p25.CPU_VRAM_RD  = and (b.p25.SOSE, b.sys.CPU_RD_SYNC);
  /*p25.TUJA*/ c.p25.CPU_VRAM_WR  = and (b.p25.SOSE, b.sys.CPU_WR_SYNC);
  /*p25.TEGU*/ c.p25.CPU_VRAM_CLK = nand(b.p25.SOSE, b.sys.CLK_xxxDxxxx1);

  c.p25.MCS_Cn = not(b.chip.MCS_C);
  c.p25.MWR_Cn = not(b.chip.MWR_C);
  c.p25.MOE_Cn = not(b.chip.MOE_C);

  c.p25.CPU_VRAM_RD2  = mux2(b.p25.MCS_Cn, b.p25.CPU_VRAM_RD , b.p25.DBG_TUTO);
  c.p25.CPU_VRAM_WR2  = mux2(b.p25.MWR_Cn, b.p25.CPU_VRAM_WR , b.p25.DBG_TUTO);
  c.p25.CPU_VRAM_CLK2 = mux2(b.p25.MOE_Cn, b.p25.CPU_VRAM_CLK, b.p25.DBG_TUTO);

  c.p25.CPU_VRAM_CLK2n = not(b.p25.CPU_VRAM_CLK2);

  c.p25.ROPY = not(b.p21.XYMU);
  c.p25.SERE = and(b.p25.CPU_VRAM_RD2,   b.p25.ROPY);
  c.p25.SAZO = and(b.p25.CPU_VRAM_CLK2n, b.p25.SERE);
  c.p25.RYJE = not(b.p25.SAZO);
  c.p25.REVO = not(b.p25.RYJE);
  c.p25.ROCY = and(b.p25.SAZO, b.p25.REVO);
  c.p25.RAHU = not(b.p25.ROCY);

  /*p25.RELA*/ c.p25.MD_OUTb = or(b.p25.REVO, b.p25.SAZO);
  /*p25.RENA*/ c.p25.MD_IN   = not(b.p25.MD_OUTb);
  /*p25.ROFA*/ c.p25.MD_OUT  = not(b.p25.MD_IN);

  c.chip.MD0_B = b.p25.MD_OUT;
  c.chip.MD1_B = b.p25.MD_OUT;
  c.chip.MD2_B = b.p25.MD_OUT;
  c.chip.MD3_B = b.p25.MD_OUT;
  c.chip.MD4_B = b.p25.MD_OUT;
  c.chip.MD5_B = b.p25.MD_OUT;
  c.chip.MD6_B = b.p25.MD_OUT;
  c.chip.MD7_B = b.p25.MD_OUT;

  /*p25.RYLU*/ c.p25.RYLU      = nand(b.p25.CPU_VRAM_CLK2, b.p25.ROPY);
  /*p25.SOHO*/ c.p25.SOHO      = and(b.p29.TACU, b.p29.TEXY);
  /*p25.RAWA*/ c.p25.RAWA      = not(b.p25.SOHO);
  /*p25.APAM*/ c.p25.APAM      = not(b.sys.VRAM_TO_OAMn);

  /*p25.SUTU*/ c.p25.MCS       = nor(b.p27.LENA, b.sys.VRAM_TO_OAMn, b.p29.TEXY, b.p25.SERE);
  /*p25.SOHY*/ c.p25.MWR       = nand(b.p25.CPU_VRAM_WR2, b.p25.SERE);
  /*p25.RACU*/ c.p25.MOE       = and(b.p25.RYLU, b.p25.RAWA, b.p27.MYMA, b.p25.APAM);


  /*p25.TODE*/ c.p25.MCS_An    = and(b.p25.MCS, b.p25.DBG_TUTOn);
  /*p25.SEWO*/ c.p25.MCS_Dn    = or(b.p25.DBG_TUTO, b.p25.MCS);

  /*p25.TAXY*/ c.p25.MWR_An    = and(b.p25.MWR, b.p25.DBG_TUTOn);
  /*p25.SOFY*/ c.p25.MWR_Dn    = or(b.p25.DBG_TUTO, b.p25.MWR);

  /*p25.SEMA*/ c.p25.MOE_An    = and(b.p25.MOE, b.p25.DBG_TUTOn);
  /*p25.RUTE*/ c.p25.MOE_Dn    = or(b.p25.DBG_TUTO, b.p25.MOE); // schematic wrong, second input is RACU

  /*p25.SOKY*/ c.p25.MCS_A     = not(b.p25.MCS_An);
  /*p25.SETY*/ c.p25.MCS_D     = not(b.p25.MCS_Dn);
  /*p25.SYSY*/ c.p25.MWR_A     = not(b.p25.MWR_An);
  /*p25.RAGU*/ c.p25.MWR_D     = not(b.p25.MWR_Dn);
  /*p25.REFO*/ c.p25.MOE_A     = not(b.p25.MOE_An);
  /*p25.SAHA*/ c.p25.MOE_D     = not(b.p25.MOE_Dn);

  c.chip.MCS_A = b.p25.MCS_A;
  c.chip.MCS_D = b.p25.MCS_D;
  c.chip.MWR_A = b.p25.MWR_A;
  c.chip.MWR_D = b.p25.MWR_D;
  c.chip.MOE_A = b.p25.MOE_A;
  c.chip.MOE_D = b.p25.MOE_D;

  //----------

  /*p25.RODY*/ if (b.p25.MD_IN) c.MD0 = b.chip.MD0_C;
  /*p25.REBA*/ if (b.p25.MD_IN) c.MD1 = b.chip.MD1_C;
  /*p25.RYDO*/ if (b.p25.MD_IN) c.MD2 = b.chip.MD2_C;
  /*p25.REMO*/ if (b.p25.MD_IN) c.MD3 = b.chip.MD3_C;
  /*p25.ROCE*/ if (b.p25.MD_IN) c.MD4 = b.chip.MD4_C;
  /*p25.ROPU*/ if (b.p25.MD_IN) c.MD5 = b.chip.MD5_C;
  /*p25.RETA*/ if (b.p25.MD_IN) c.MD6 = b.chip.MD6_C;
  /*p25.RAKU*/ if (b.p25.MD_IN) c.MD7 = b.chip.MD7_C;

  //----------
  // more debug stuff

  c.p25.TUSO = nor(b.sys.MODE_DBG2, b.sys.CPUCLK_xxxxxFGH2);
  c.p25.SOLE = not(b.p25.TUSO);
  c.p25.P10_Bn = not(b.chip.P10_B);

  /*p25.TOVU*/ if (b.p25.P10_Bn) c.D0 = b.p25.SOLE;
  /*p25.SOSA*/ if (b.p25.P10_Bn) c.D1 = b.p25.SOLE;
  /*p25.SEDU*/ if (b.p25.P10_Bn) c.D2 = b.p25.SOLE;
  /*p25.TAXO*/ if (b.p25.P10_Bn) c.D3 = b.p25.SOLE;
  /*p25.TAHY*/ if (b.p25.P10_Bn) c.D4 = b.p25.SOLE;
  /*p25.TESU*/ if (b.p25.P10_Bn) c.D5 = b.p25.SOLE;
  /*p25.TAZU*/ if (b.p25.P10_Bn) c.D6 = b.p25.SOLE;
  /*p25.TEWA*/ if (b.p25.P10_Bn) c.D7 = b.p25.SOLE;

  //----------

  /*p25.TEME*/ if (b.p25.RAHU) c.MD0 = b.D0;
  /*p25.TEWU*/ if (b.p25.RAHU) c.MD1 = b.D1;
  /*p25.TYGO*/ if (b.p25.RAHU) c.MD2 = b.D2;
  /*p25.SOTE*/ if (b.p25.RAHU) c.MD3 = b.D3;
  /*p25.SEKE*/ if (b.p25.RAHU) c.MD4 = b.D4;
  /*p25.RUJO*/ if (b.p25.RAHU) c.MD5 = b.D5;
  /*p25.TOFA*/ if (b.p25.RAHU) c.MD6 = b.D6;
  /*p25.SUZA*/ if (b.p25.RAHU) c.MD7 = b.D7;

  c.p25.SYNU = or(b.p25.RAHU, b.MD0);
  c.p25.SYMA = or(b.p25.RAHU, b.MD1);
  c.p25.ROKO = or(b.p25.RAHU, b.MD2);
  c.p25.SYBU = or(b.p25.RAHU, b.MD3);
  c.p25.SAKO = or(b.p25.RAHU, b.MD4);
  c.p25.SEJY = or(b.p25.RAHU, b.MD5);
  c.p25.SEDO = or(b.p25.RAHU, b.MD6);
  c.p25.SAWU = or(b.p25.RAHU, b.MD7);

  c.p25.RURA = not(b.p25.SYNU);
  c.p25.RULY = not(b.p25.SYMA);
  c.p25.RARE = not(b.p25.ROKO);
  c.p25.RODU = not(b.p25.SYBU);
  c.p25.RUBE = not(b.p25.SAKO);
  c.p25.RUMU = not(b.p25.SEJY);
  c.p25.RYTY = not(b.p25.SEDO);
  c.p25.RADY = not(b.p25.SAWU);

  c.chip.MD0_D = b.p25.RURA;
  c.chip.MD1_D = b.p25.RULY;
  c.chip.MD2_D = b.p25.RARE;
  c.chip.MD3_D = b.p25.RODU;
  c.chip.MD4_D = b.p25.RUBE;
  c.chip.MD5_D = b.p25.RUMU;
  c.chip.MD6_D = b.p25.RYTY;
  c.chip.MD7_D = b.p25.RADY;

  //----------

  c.p25.XANE = nor(b.sys.VRAM_TO_OAMn, b.p21.XYMU);
  c.p25.XEDU = not(b.p25.XANE);

  c.p25.XAKY = b.A00;
  c.p25.XUXU = b.A01;
  c.p25.XYNE = b.A02;
  c.p25.XODY = b.A03;
  c.p25.XECA = b.A04;
  c.p25.XOBA = b.A05;
  c.p25.XOPO = b.A06;
  c.p25.XYBO = b.A07;
  c.p25.RYSU = b.A08;
  c.p25.RESE = b.A09;
  c.p25.RUSE = b.A10;
  c.p25.RYNA = b.A11;
  c.p25.RUMO = b.A12;

  if (b.p25.XEDU) {
    c.chip.MA00 = b.p25.XAKY;
    c.chip.MA01 = b.p25.XUXU;
    c.chip.MA02 = b.p25.XYNE;
    c.chip.MA03 = b.p25.XODY;
    c.chip.MA04 = b.p25.XECA;
    c.chip.MA05 = b.p25.XOBA;
    c.chip.MA06 = b.p25.XOPO;
    c.chip.MA07 = b.p25.XYBO;
    c.chip.MA08 = b.p25.RYSU;
    c.chip.MA09 = b.p25.RESE;
    c.chip.MA10 = b.p25.RUSE;
    c.chip.MA11 = b.p25.RYNA;
    c.chip.MA12 = b.p25.RUMO;
  }

  //----------

  c.p25.ROVE = not(b.p25.RAHU);

  c.p25.SEFA_00 = and(b.MD0, b.p25.ROVE);
  c.p25.SOGO_01 = and(b.MD1, b.p25.ROVE);
  c.p25.SEFU_02 = and(b.MD2, b.p25.ROVE);
  c.p25.SUNA_03 = and(b.MD3, b.p25.ROVE);
  c.p25.SUMO_04 = and(b.MD4, b.p25.ROVE);
  c.p25.SAZU_05 = and(b.MD5, b.p25.ROVE);
  c.p25.SAMO_06 = and(b.MD6, b.p25.ROVE);
  c.p25.SUKE_07 = and(b.MD7, b.p25.ROVE);

  c.p25.REGE_00 = not(b.p25.SEFA_00);
  c.p25.RYKY_01 = not(b.p25.SOGO_01);
  c.p25.RAZO_02 = not(b.p25.SEFU_02);
  c.p25.RADA_03 = not(b.p25.SUNA_03);
  c.p25.RYRO_04 = not(b.p25.SUMO_04);
  c.p25.REVU_05 = not(b.p25.SAZU_05);
  c.p25.REKU_06 = not(b.p25.SAMO_06);
  c.p25.RYZE_07 = not(b.p25.SUKE_07);

  c.chip.MD0_A = b.p25.REGE_00;
  c.chip.MD1_A = b.p25.RYKY_01;
  c.chip.MD2_A = b.p25.RAZO_02;
  c.chip.MD3_A = b.p25.RADA_03;
  c.chip.MD4_A = b.p25.RYRO_04;
  c.chip.MD5_A = b.p25.REVU_05;
  c.chip.MD6_A = b.p25.REKU_06;
  c.chip.MD7_A = b.p25.RYZE_07;

  //----------


  c.p25.RALO_00 = not(b.chip.D0_C);
  c.p25.TUNE_01 = not(b.chip.D1_C);
  c.p25.SERA_02 = not(b.chip.D2_C);
  c.p25.TENU_03 = not(b.chip.D3_C);
  c.p25.SYSA_04 = not(b.chip.D4_C);
  c.p25.SUGY_05 = not(b.chip.D5_C);
  c.p25.TUBE_06 = not(b.chip.D6_C);
  c.p25.SYZO_07 = not(b.chip.D7_C);

  c.p25.WEJO_00 = not(b.p25.RALO_00);
  c.p25.BUBO_01 = not(b.p25.TUNE_01);
  c.p25.BETU_02 = not(b.p25.SERA_02);
  c.p25.CYME_03 = not(b.p25.TENU_03);
  c.p25.BAXU_04 = not(b.p25.SYSA_04);
  c.p25.BUHU_05 = not(b.p25.SUGY_05);
  c.p25.BYNY_06 = not(b.p25.TUBE_06);
  c.p25.BYPY_07 = not(b.p25.SYZO_07);

  c.p25.WASA_00 = not(b.p25.RALO_00);
  c.p25.BOMO_01 = not(b.p25.TUNE_01);
  c.p25.BASA_02 = not(b.p25.SERA_02);
  c.p25.CAKO_03 = not(b.p25.TENU_03);
  c.p25.BUMA_04 = not(b.p25.SYSA_04);
  c.p25.BUPY_05 = not(b.p25.SUGY_05);
  c.p25.BASY_06 = not(b.p25.TUBE_06);
  c.p25.BAPE_07 = not(b.p25.SYZO_07);

  c.p25.CEDE = not(b.sys.DO_DMA);
  if (b.p25.CEDE) {
    c.OAM_A_D0 = b.p25.WEJO_00;
    c.OAM_A_D1 = b.p25.BUBO_01;
    c.OAM_A_D2 = b.p25.BETU_02;
    c.OAM_A_D3 = b.p25.CYME_03;
    c.OAM_A_D4 = b.p25.BAXU_04;
    c.OAM_A_D5 = b.p25.BUHU_05;
    c.OAM_A_D6 = b.p25.BYNY_06;
    c.OAM_A_D7 = b.p25.BYPY_07;

    c.OAM_B_D0 = b.p25.WASA_00;
    c.OAM_B_D1 = b.p25.BOMO_01;
    c.OAM_B_D2 = b.p25.BASA_02;
    c.OAM_B_D3 = b.p25.CAKO_03;
    c.OAM_B_D4 = b.p25.BUMA_04;
    c.OAM_B_D5 = b.p25.BUPY_05;
    c.OAM_B_D6 = b.p25.BASY_06;
    c.OAM_B_D7 = b.p25.BAPE_07;
  }

  //----------

  c.p25.TYVY = nand(b.p25.SERE, b.p28.LEKO);
  c.p25.SEBY = not(b.p25.TYVY);

  c.p25.RERY_00 = not(b.MD0);
  c.p25.RUNA_01 = not(b.MD1);
  c.p25.RONA_02 = not(b.MD2);
  c.p25.RUNO_03 = not(b.MD3);
  c.p25.SANA_04 = not(b.MD4);
  c.p25.RORO_05 = not(b.MD5);
  c.p25.RABO_06 = not(b.MD6);
  c.p25.SAME_07 = not(b.MD7);

  c.p25.RUGA_00 = not(b.p25.RERY_00);
  c.p25.ROTA_01 = not(b.p25.RUNA_01);
  c.p25.RYBU_02 = not(b.p25.RONA_02);
  c.p25.RAJU_03 = not(b.p25.RUNO_03);
  c.p25.TYJA_04 = not(b.p25.SANA_04);
  c.p25.REXU_05 = not(b.p25.RORO_05);
  c.p25.RUPY_06 = not(b.p25.RABO_06);
  c.p25.TOKU_07 = not(b.p25.SAME_07);

  if (b.p25.SEBY) {
    c.D0 = b.p25.RUGA_00;
    c.D1 = b.p25.ROTA_01;
    c.D2 = b.p25.RYBU_02;
    c.D3 = b.p25.RAJU_03;
    c.D4 = b.p25.TYJA_04;
    c.D5 = b.p25.REXU_05;
    c.D6 = b.p25.RUPY_06;
    c.D7 = b.p25.TOKU_07;
  }

  //----------

  c.p25.XUCY = nand(b.p27.NETA, b.p27.PORE);
  c.p25.XEZE = nand(b.p27.POTU, b.p27.PORE);
  c.p25.WUKO = not(b.p25.XEZE);

  //----------

  c.p25.XONU_00 = not(b.p27.XUHA);
  c.p25.WUDO_01 = not(b.p27.VYNO);
  c.p25.WAWE_02 = not(b.p27.VUJO);
  c.p25.WOLU_03 = not(b.p27.VYMU);

  if (b.p25.XUCY) {
    c.MA00 = b.p25.XONU_00;
    c.MA01 = b.p25.WUDO_01;
    c.MA02 = b.p25.WAWE_02;
    c.MA03 = b.p25.WOLU_03;
  }

  //----------

  c.p25.VAPY_04 = not(b.p32.RAWU);
  c.p25.SEZU_05 = not(b.p32.POZO);
  c.p25.VEJY_06 = not(b.p32.PYZO);
  c.p25.RUSA_07 = not(b.p32.POXA);
  c.p25.ROHA_08 = not(b.p32.PULO);
  c.p25.RESO_09 = not(b.p32.POJU);
  c.p25.SUVO_10 = not(b.p32.POWY);
  c.p25.TOBO_11 = not(b.p32.PYJU);

  c.p25.VUZA = nor(b.p23.BG_TILE_SEL, b.p32.PYJU);
  c.p25.VURY_12 = not(b.p25.VUZA);

  if (b.p27.NETA) {
    c.MA04 = b.p25.VAPY_04;
    c.MA05 = b.p25.SEZU_05;
    c.MA06 = b.p25.VEJY_06;
    c.MA07 = b.p25.RUSA_07;
    c.MA08 = b.p25.ROHA_08;
    c.MA09 = b.p25.RESO_09;
    c.MA10 = b.p25.SUVO_10;
    c.MA11 = b.p25.TOBO_11;
    c.MA12 = b.p25.VURY_12;
  }
}