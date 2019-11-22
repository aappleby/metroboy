#include "P25_VramInterface.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P25_VramInterface_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // MAXX pin driver

  /*p25.MYFU*/ c.p25.MA00n = not(b.MA00);
  /*p25.MASA*/ c.p25.MA01n = not(b.MA01);
  /*p25.MYRE*/ c.p25.MA02n = not(b.MA02);
  /*p25.MAVU*/ c.p25.MA03n = not(b.MA03);
  /*p25.MEPA*/ c.p25.MA04n = not(b.MA04);
  /*p25.MYSA*/ c.p25.MA05n = not(b.MA05);
  /*p25.MEWY*/ c.p25.MA06n = not(b.MA06);
  /*p25.MUME*/ c.p25.MA07n = not(b.MA07);
  /*p25.VOVA*/ c.p25.MA08n = not(b.MA08);
  /*p25.VODE*/ c.p25.MA09n = not(b.MA09);
  /*p25.RUKY*/ c.p25.MA10n = not(b.MA10);
  /*p25.RUMA*/ c.p25.MA11n = not(b.MA11);
  /*p25.REHO*/ c.p25.MA12n = not(b.MA12);

  /*p25.LEXE*/ c.p25.MA00b = not(c.p25.MA00n);
  /*p25.LOZU*/ c.p25.MA01b = not(c.p25.MA01n);
  /*p25.LACA*/ c.p25.MA02b = not(c.p25.MA02n);
  /*p25.LUVO*/ c.p25.MA03b = not(c.p25.MA03n);
  /*p25.LOLY*/ c.p25.MA04b = not(c.p25.MA04n);
  /*p25.LALO*/ c.p25.MA05b = not(c.p25.MA05n);
  /*p25.LEFA*/ c.p25.MA06b = not(c.p25.MA06n);
  /*p25.LUBY*/ c.p25.MA07b = not(c.p25.MA07n);
  /*p25.TUJY*/ c.p25.MA08b = not(c.p25.MA08n);
  /*p25.TAGO*/ c.p25.MA09b = not(c.p25.MA09n);
  /*p25.NUVA*/ c.p25.MA10b = not(c.p25.MA10n);
  /*p25.PEDU*/ c.p25.MA11b = not(c.p25.MA11n);
  /*p25.PONY*/ c.p25.MA12b = not(c.p25.MA12n);

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

  /*p25.SYRO*/ c.p25.ADDR_FE00_FFFF = not(b.sys.ADDR_0000_FE00);
  /*p25.TEFA*/ c.p25.TEFA = nor(b.p25.ADDR_FE00_FFFF, b.sys.ADDR_VALID_AND_NOT_VRAM);\

  // guess
  /*p25.SOSE*/ c.p25.ADDR_VRAM = and(b.A15, b.p25.TEFA);

  // guess
  /*p25.TUCA*/ c.p25.CPU_VRAM_RD  = and (b.p25.ADDR_VRAM, b.sys.CPU_RD_SYNC);
  /*p25.TUJA*/ c.p25.CPU_VRAM_WR  = and (b.p25.ADDR_VRAM, b.sys.CPU_WR_SYNC);
  /*p25.TEGU*/ c.p25.CPU_VRAM_CLK = nand(b.p25.ADDR_VRAM, b.sys.CLK_xxxDxxxx1);

  /*p25.TEFY*/ c.p25.MCS_Cn = not(b.chip.MCS_C);
  /*p25.SUDO*/ c.p25.MWR_Cn = not(b.chip.MWR_C);
  /*p25.TAVY*/ c.p25.MOE_Cn = not(b.chip.MOE_C);

  /*p25.TOLE*/ c.p25.CPU_VRAM_RD2  = mux2(b.p25.MCS_Cn, b.p25.CPU_VRAM_RD , b.p25.DBG_TUTO);
  /*p25.TYJY*/ c.p25.CPU_VRAM_WR2  = mux2(b.p25.MWR_Cn, b.p25.CPU_VRAM_WR , b.p25.DBG_TUTO);
  /*p25.SALE*/ c.p25.CPU_VRAM_CLK2 = mux2(b.p25.MOE_Cn, b.p25.CPU_VRAM_CLK, b.p25.DBG_TUTO);

  /*p25.RUVY*/ c.p25.CPU_VRAM_CLK2n = not(b.p25.CPU_VRAM_CLK2);

  /*p25.SERE*/ c.p25.SERE = and(b.p25.CPU_VRAM_RD2,   b.p25.RENDERINGn);
  /*p25.ROPY*/   c.p25.RENDERINGn = not(b.p21.RENDERING);

  /*p25.SAZO*/ c.p25.MD_OUTd = and(b.p25.CPU_VRAM_CLK2n, b.p25.SERE);
  /*p25.RYJE*/ c.p25.MD_INb  = not(b.p25.MD_OUTd);
  /*p25.REVO*/ c.p25.MD_OUTc = not(b.p25.MD_INb);
  /*p25.ROCY*/ c.p25.MD_OUTe = and(b.p25.MD_OUTd, b.p25.MD_OUTc);
  /*p25.RAHU*/ c.p25.D_TO_MD = not(b.p25.MD_OUTe);

  /*p25.RELA*/ c.p25.MD_OUTb = or(b.p25.MD_OUTc, b.p25.MD_OUTd);
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





  /*p25.SOKY*/ c.chip.MCS_A = not(b.p25.MCS_An);
  /*p25.SETY*/ c.chip.MCS_D = not(b.p25.MCS_Dn);
  /*p25.TODE*/   c.p25.MCS_An = and(b.p25.MCS, b.p25.DBG_TUTOn);
  /*p25.SEWO*/   c.p25.MCS_Dn = or(b.p25.MCS, b.p25.DBG_TUTO);
  /*p25.SUTU*/     c.p25.MCS = nor(b.p27.LENA, b.sys.VRAM_TO_OAMn, b.spr.TEXY, b.p25.SERE);

  /*p25.SYSY*/ c.chip.MWR_A = not(b.p25.MWR_An);
  /*p25.RAGU*/ c.chip.MWR_D = not(b.p25.MWR_Dn);
  /*p25.TAXY*/   c.p25.MWR_An = and(b.p25.MWR, b.p25.DBG_TUTOn);
  /*p25.SOFY*/   c.p25.MWR_Dn = or(b.p25.MWR, b.p25.DBG_TUTO);
  /*p25.SOHY*/     c.p25.MWR = nand(b.p25.CPU_VRAM_WR2, b.p25.SERE);

  /*p25.REFO*/ c.chip.MOE_A = not(b.p25.MOE_An);
  /*p25.SAHA*/ c.chip.MOE_D = not(b.p25.MOE_Dn);
  /*p25.SEMA*/   c.p25.MOE_An    = and(b.p25.MOE, b.p25.DBG_TUTOn);
  /*p25.RUTE*/   c.p25.MOE_Dn    = or(b.p25.MOE, b.p25.DBG_TUTO); // schematic wrong, second input is RACU
  /*p25.RACU*/     c.p25.MOE = and(b.p25.RYLU, b.p25.RAWA, b.p27.MYMA, b.p25.VRAM_TO_OAM);
  /*p25.RYLU*/       c.p25.RYLU = nand(b.p25.CPU_VRAM_CLK2, b.p25.RENDERINGn);
  /*p25.RAWA*/       c.p25.RAWA = not(b.p25.SOHO);
  /*p25.SOHO*/         c.p25.SOHO = and(b.spr.TACU, b.spr.TEXY);
  /*p27.MYMA*/       c.p27.MYMA = not(b.p27.LONY);
  /*p25.APAM*/       c.p25.VRAM_TO_OAM = not(b.sys.VRAM_TO_OAMn);

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

  /*p25.SYCY*/ c.p25.MODE_DBG2n = not(b.sys.MODE_DBG2);
  /*p25.SOTO*/ c.p25.DBG_SOTO   = tock_pos(a.p25.MODE_DBG2n, b.p25.MODE_DBG2n, b.sys.SYS_RESETn2, b.p25.DBG_SOTO, !b.p25.DBG_SOTO);
  /*p25.TUTO*/ c.p25.DBG_TUTO   = and(b.sys.MODE_DBG2, !b.p25.DBG_SOTO);
  /*p25.RACO*/ c.p25.DBG_TUTOn  = not(b.p25.DBG_TUTO);

  /*p25.TUSO*/ c.p25.TUSO = nor(b.sys.MODE_DBG2, b.sys.CPU_CLK1n);
  /*p25.SOLE*/ c.p25.SOLE = not(b.p25.TUSO);
  /*p25.????*/ c.p25.P10_Bn = not(b.chip.P10_B);

  /*p25.TOVU*/ if (b.p25.P10_Bn) c.D0 = b.p25.SOLE;
  /*p25.SOSA*/ if (b.p25.P10_Bn) c.D1 = b.p25.SOLE;
  /*p25.SEDU*/ if (b.p25.P10_Bn) c.D2 = b.p25.SOLE;
  /*p25.TAXO*/ if (b.p25.P10_Bn) c.D3 = b.p25.SOLE;
  /*p25.TAHY*/ if (b.p25.P10_Bn) c.D4 = b.p25.SOLE;
  /*p25.TESU*/ if (b.p25.P10_Bn) c.D5 = b.p25.SOLE;
  /*p25.TAZU*/ if (b.p25.P10_Bn) c.D6 = b.p25.SOLE;
  /*p25.TEWA*/ if (b.p25.P10_Bn) c.D7 = b.p25.SOLE;

  //----------

  /*p25.TEME*/ if (b.p25.D_TO_MD) c.MD0 = b.D0; // well this is clearly backwards
  /*p25.TEWU*/ if (b.p25.D_TO_MD) c.MD1 = b.D1;
  /*p25.TYGO*/ if (b.p25.D_TO_MD) c.MD2 = b.D2;
  /*p25.SOTE*/ if (b.p25.D_TO_MD) c.MD3 = b.D3;
  /*p25.SEKE*/ if (b.p25.D_TO_MD) c.MD4 = b.D4;
  /*p25.RUJO*/ if (b.p25.D_TO_MD) c.MD5 = b.D5;
  /*p25.TOFA*/ if (b.p25.D_TO_MD) c.MD6 = b.D6;
  /*p25.SUZA*/ if (b.p25.D_TO_MD) c.MD7 = b.D7;

  /*p25.SYNU*/ c.p25.SYNU = or(b.p25.D_TO_MD, b.MD0);
  /*p25.SYMA*/ c.p25.SYMA = or(b.p25.D_TO_MD, b.MD1);
  /*p25.ROKO*/ c.p25.ROKO = or(b.p25.D_TO_MD, b.MD2);
  /*p25.SYBU*/ c.p25.SYBU = or(b.p25.D_TO_MD, b.MD3);
  /*p25.SAKO*/ c.p25.SAKO = or(b.p25.D_TO_MD, b.MD4);
  /*p25.SEJY*/ c.p25.SEJY = or(b.p25.D_TO_MD, b.MD5);
  /*p25.SEDO*/ c.p25.SEDO = or(b.p25.D_TO_MD, b.MD6);
  /*p25.SAWU*/ c.p25.SAWU = or(b.p25.D_TO_MD, b.MD7);

  /*p25.RURA*/ c.p25.RURA = not(b.p25.SYNU);
  /*p25.RULY*/ c.p25.RULY = not(b.p25.SYMA);
  /*p25.RARE*/ c.p25.RARE = not(b.p25.ROKO);
  /*p25.RODU*/ c.p25.RODU = not(b.p25.SYBU);
  /*p25.RUBE*/ c.p25.RUBE = not(b.p25.SAKO);
  /*p25.RUMU*/ c.p25.RUMU = not(b.p25.SEJY);
  /*p25.RYTY*/ c.p25.RYTY = not(b.p25.SEDO);
  /*p25.RADY*/ c.p25.RADY = not(b.p25.SAWU);

  c.chip.MD0_D = b.p25.RURA;
  c.chip.MD1_D = b.p25.RULY;
  c.chip.MD2_D = b.p25.RARE;
  c.chip.MD3_D = b.p25.RODU;
  c.chip.MD4_D = b.p25.RUBE;
  c.chip.MD5_D = b.p25.RUMU;
  c.chip.MD6_D = b.p25.RYTY;
  c.chip.MD7_D = b.p25.RADY;

  //----------

  /*p25.XANE*/ c.p25.A_TO_MAn = nor(b.sys.VRAM_TO_OAMn, b.p21.RENDERING);
  /*p25.XEDU*/ c.p25.A_TO_MA = not(b.p25.A_TO_MAn);

  /*p25.XAKY*/ if (b.p25.A_TO_MA) c.chip.MA00 = b.A00;
  /*p25.XUXU*/ if (b.p25.A_TO_MA) c.chip.MA01 = b.A01;
  /*p25.XYNE*/ if (b.p25.A_TO_MA) c.chip.MA02 = b.A02;
  /*p25.XODY*/ if (b.p25.A_TO_MA) c.chip.MA03 = b.A03;
  /*p25.XECA*/ if (b.p25.A_TO_MA) c.chip.MA04 = b.A04;
  /*p25.XOBA*/ if (b.p25.A_TO_MA) c.chip.MA05 = b.A05;
  /*p25.XOPO*/ if (b.p25.A_TO_MA) c.chip.MA06 = b.A06;
  /*p25.XYBO*/ if (b.p25.A_TO_MA) c.chip.MA07 = b.A07;
  /*p25.RYSU*/ if (b.p25.A_TO_MA) c.chip.MA08 = b.A08;
  /*p25.RESE*/ if (b.p25.A_TO_MA) c.chip.MA09 = b.A09;
  /*p25.RUSE*/ if (b.p25.A_TO_MA) c.chip.MA10 = b.A10;
  /*p25.RYNA*/ if (b.p25.A_TO_MA) c.chip.MA11 = b.A11;
  /*p25.RUMO*/ if (b.p25.A_TO_MA) c.chip.MA12 = b.A12;

  //----------

  /*p25.ROVE*/ c.p25.D_TO_MDn = not(b.p25.D_TO_MD);

  /*p25.SEFA*/ c.p25.SEFA_00 = and(b.MD0, b.p25.D_TO_MDn);
  /*p25.SOGO*/ c.p25.SOGO_01 = and(b.MD1, b.p25.D_TO_MDn);
  /*p25.SEFU*/ c.p25.SEFU_02 = and(b.MD2, b.p25.D_TO_MDn);
  /*p25.SUNA*/ c.p25.SUNA_03 = and(b.MD3, b.p25.D_TO_MDn);
  /*p25.SUMO*/ c.p25.SUMO_04 = and(b.MD4, b.p25.D_TO_MDn);
  /*p25.SAZU*/ c.p25.SAZU_05 = and(b.MD5, b.p25.D_TO_MDn);
  /*p25.SAMO*/ c.p25.SAMO_06 = and(b.MD6, b.p25.D_TO_MDn);
  /*p25.SUKE*/ c.p25.SUKE_07 = and(b.MD7, b.p25.D_TO_MDn);

  /*p25.REGE*/ c.p25.REGE_00 = not(b.p25.SEFA_00);
  /*p25.RYKY*/ c.p25.RYKY_01 = not(b.p25.SOGO_01);
  /*p25.RAZO*/ c.p25.RAZO_02 = not(b.p25.SEFU_02);
  /*p25.RADA*/ c.p25.RADA_03 = not(b.p25.SUNA_03);
  /*p25.RYRO*/ c.p25.RYRO_04 = not(b.p25.SUMO_04);
  /*p25.REVU*/ c.p25.REVU_05 = not(b.p25.SAZU_05);
  /*p25.REKU*/ c.p25.REKU_06 = not(b.p25.SAMO_06);
  /*p25.RYZE*/ c.p25.RYZE_07 = not(b.p25.SUKE_07);

  c.chip.MD0_A = b.p25.REGE_00;
  c.chip.MD1_A = b.p25.RYKY_01;
  c.chip.MD2_A = b.p25.RAZO_02;
  c.chip.MD3_A = b.p25.RADA_03;
  c.chip.MD4_A = b.p25.RYRO_04;
  c.chip.MD5_A = b.p25.REVU_05;
  c.chip.MD6_A = b.p25.REKU_06;
  c.chip.MD7_A = b.p25.RYZE_07;

  //----------

  /*p25.RALO*/ c.p25.D0n = not(b.chip.D0_C);
  /*p25.TUNE*/ c.p25.D1n = not(b.chip.D1_C);
  /*p25.SERA*/ c.p25.D2n = not(b.chip.D2_C);
  /*p25.TENU*/ c.p25.D3n = not(b.chip.D3_C);
  /*p25.SYSA*/ c.p25.D4n = not(b.chip.D4_C);
  /*p25.SUGY*/ c.p25.D5n = not(b.chip.D5_C);
  /*p25.TUBE*/ c.p25.D6n = not(b.chip.D6_C);
  /*p25.SYZO*/ c.p25.D7n = not(b.chip.D7_C);

  //----------

  /*p25.TYVY*/ c.p25.MD_TO_Dn = nand(b.p25.SERE, b.spr.CPU_READ_MYSTERY);
  /*p25.SEBY*/ c.p25.MD_TO_D = not(b.p25.MD_TO_Dn);

  /*p25.RERY*/ c.p25.RERY_00 = not(b.MD0);
  /*p25.RUNA*/ c.p25.RUNA_01 = not(b.MD1);
  /*p25.RONA*/ c.p25.RONA_02 = not(b.MD2);
  /*p25.RUNO*/ c.p25.RUNO_03 = not(b.MD3);
  /*p25.SANA*/ c.p25.SANA_04 = not(b.MD4);
  /*p25.RORO*/ c.p25.RORO_05 = not(b.MD5);
  /*p25.RABO*/ c.p25.RABO_06 = not(b.MD6);
  /*p25.SAME*/ c.p25.SAME_07 = not(b.MD7);

  /*p25.RUGA*/ if (b.p25.MD_TO_D) c.D0 = b.p25.RERY_00;
  /*p25.ROTA*/ if (b.p25.MD_TO_D) c.D1 = b.p25.RUNA_01;
  /*p25.RYBU*/ if (b.p25.MD_TO_D) c.D2 = b.p25.RONA_02;
  /*p25.RAJU*/ if (b.p25.MD_TO_D) c.D3 = b.p25.RUNO_03;
  /*p25.TYJA*/ if (b.p25.MD_TO_D) c.D4 = b.p25.SANA_04;
  /*p25.REXU*/ if (b.p25.MD_TO_D) c.D5 = b.p25.RORO_05;
  /*p25.RUPY*/ if (b.p25.MD_TO_D) c.D6 = b.p25.RABO_06;
  /*p25.TOKU*/ if (b.p25.MD_TO_D) c.D7 = b.p25.SAME_07;

  //----------

  //----------
  // window something address gen
}