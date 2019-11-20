#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P27_WindowMapLookup_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  /*p27.REPU*/ c.p27.IN_FRAME_Y  = nor(b.p21.INT_VBL, b.sys.VID_RESET4);   // schematic wrong, this is NOR
  /*p27.SYNY*/ c.p27.IN_FRAME_Yn = not(b.p27.IN_FRAME_Y);

  //----------
  // Window Y match

  /*p27.NAZE*/ c.p27.WY_MATCH0n = xor(b.p23.WY0, b.p21.V0);
  /*p27.PEBO*/ c.p27.WY_MATCH1n = xor(b.p23.WY1, b.p21.V1);
  /*p27.POMO*/ c.p27.WY_MATCH2n = xor(b.p23.WY2, b.p21.V2);
  /*p27.NEVU*/ c.p27.WY_MATCH3n = xor(b.p23.WY3, b.p21.V3);
  /*p27.NOJO*/ c.p27.WY_MATCH4n = xor(b.p23.WY4, b.p21.V4);
  /*p27.PAGA*/ c.p27.WY_MATCH5n = xor(b.p23.WY5, b.p21.V5);
  /*p27.PEZO*/ c.p27.WY_MATCH6n = xor(b.p23.WY6, b.p21.V6);
  /*p27.NUPA*/ c.p27.WY_MATCH7n = xor(b.p23.WY7, b.p21.V7);

  // WIN_EN polarity?
  /*p27.PALO*/ c.p27.WY_MATCH_HI  = nand(b.p23.LCDC_WINEN, b.p27.WY_MATCH4n, b.p27.WY_MATCH5n, b.p27.WY_MATCH6n, b.p27.WY_MATCH7n);
  /*p27.NELE*/ c.p27.WY_MATCH_HIn = not(b.p27.WY_MATCH_HI);
  /*p27.PAFU*/ c.p27.WY_MATCHn    = nand(b.p27.WY_MATCH_HIn, b.p27.WY_MATCH0n, b.p27.WY_MATCH1n, b.p27.WY_MATCH2n, b.p27.WY_MATCH3n);
  /*p27.ROGE*/ c.p27.WY_MATCH     = not(b.p27.WY_MATCHn);

  /*p27.SARY*/ c.p27.WY_MATCH_SYNC = tock_pos(a.p21.CLK_1Mb, b.p21.CLK_1Mb, b.sys.VID_RESETn1, b.p27.WY_MATCH_SYNC, b.p27.WY_MATCH);

  //----------
  // Window X match

  // polarity or gates wrong
  /*p27.REJO*/ c.p27.WIN_CHECK_X = or(b.p27.WY_MATCH_SYNC, b.p27.IN_FRAME_Y); // another weird or gate. should be AND?

  /*p27.MYLO*/ c.p27.WX_MATCH0n = xor(b.p21.X0, b.p23.WX0);
  /*p27.PUWU*/ c.p27.WX_MATCH1n = xor(b.p21.X1, b.p23.WX1);
  /*p27.PUHO*/ c.p27.WX_MATCH2n = xor(b.p21.X2, b.p23.WX2);
  /*p27.NYTU*/ c.p27.WX_MATCH3n = xor(b.p21.X3, b.p23.WX3);
  /*p27.NEZO*/ c.p27.WX_MATCH4n = xor(b.p21.X4, b.p23.WX4);
  /*p27.NORY*/ c.p27.WX_MATCH5n = xor(b.p21.X5, b.p23.WX5);
  /*p27.NONO*/ c.p27.WX_MATCH6n = xor(b.p21.X6, b.p23.WX6);
  /*p27.PASE*/ c.p27.WX_MATCH7n = xor(b.p21.X7, b.p23.WX7);

  // not sure about this
  /*p27.PUKY*/ c.p27.WX_MATCH_HI  = nand(b.p27.WIN_CHECK_X, b.p27.WX_MATCH4n, b.p27.WX_MATCH5n, b.p27.WX_MATCH6n, b.p27.WX_MATCH7n);
  /*p27.NUFA*/ c.p27.WX_MATCH_HIn = not (b.p27.WX_MATCH_HI);
  /*p27.NOGY*/ c.p27.WIN_MATCHn    = nand(b.p27.WX_MATCH_HIn, b.p27.WX_MATCH0n, b.p27.WX_MATCH1n, b.p27.WX_MATCH2n, b.p27.WX_MATCH3n);
  /*p27.NUKO*/ c.p27.WIN_MATCH     = not (b.p27.WIN_MATCHn);

  //----------

  // a guess? probably not
  /*p27.SUHA*/ c.p27.FINE_X0 = xor(b.p23.SCX0, b.p27.RYKU);
  /*p27.SYBY*/ c.p27.FINE_X1 = xor(b.p23.SCX1, b.p27.ROGA);
  /*p27.SOZU*/ c.p27.FINE_X2 = xor(b.p23.SCX2, b.p27.RUBU);

  /*p27.ROZE*/ c.p27.ROZE = nand(b.p27.RYKU, b.p27.ROGA, b.p27.RUBU);
  /*p27.POVA*/ c.p27.POVA = and(!b.p27.NYZE, b.p27.PUXA);
  /*p27.PAHA*/ c.p27.PAHA = not(b.p21.RENDERING);
  /*p27.ROXY*/ c.p27.ROXY = unk2(b.p27.PAHA, b.p27.POVA);
  /*p27.PECU*/ c.p27.PECU = nand(b.p24.ROXO, b.p27.ROZE);
  /*p27.RONE*/ c.p27.RONE = nand(b.p27.ROXY, b.p27.FINE_X0, b.p27.FINE_X1, b.p27.FINE_X2);
  /*p27.POHU*/ c.p27.POHU = not(b.p27.RONE);
  /*p27.PANY*/ c.p27.PANY = nor(b.p27.WIN_MATCH, b.p27.ROZE);
  /*p27.SEKO*/ c.p27.SEKO = nor(b.p27.RENE, !b.p27.RYFA);
  /*p27.ROMO*/ c.p27.ROMO = not(b.p24.POKY);
  /*p27.SUVU*/ c.p27.SUVU = nand(b.p21.RENDERING, b.p27.ROMO, b.p24.NYKA, b.p24.PORY);
  /*p27.TAVE*/ c.p27.TAVE = not(b.p27.SUVU);
  /*p27.XAHY*/ c.p27.XAHY = not(b.p28.SPRITE_COUNT_RST);

  /*p27.XOFO*/ c.p27.XOFO = nand(b.p23.LCDC_WINEN, b.p27.XAHY, b.sys.VID_RESETn1);
  /*p27.XACO*/ c.p27.RST_XACO = not(b.p27.XOFO);
  /*p27.PYNU*/ c.p27.PYNU = or(b.p27.NUNU, b.p27.XOFO);
  /*p27.NUNY*/ c.p27.NUNY = and(!b.p27.NOPA, b.p27.PYNU);
  /*p27.NOCU*/ c.p27.NOCU = not(b.p27.PYNU);
  /*p27.PORE*/ c.p27.PORE = not(b.p27.NOCU);
  /*p27.PUKU*/ c.p27.PUKU = nor(b.p27.NUNY, b.p27.RYDY);
  /*p27.RYDY*/ c.p27.RYDY = nor(b.p27.PUKU, b.sys.VID_RESET4, b.p24.PORY);
  /*p27.SYLO*/ c.p27.SYLO = not(b.p27.RYDY);
  /*p27.TUXY*/ c.p27.TUXY = nand(b.p27.SOVY, b.p27.SYLO);
  /*p27.SUZU*/ c.p27.SUZU = not(b.p27.TUXY);
  /*p27.TEVO*/ c.p27.TEVO = nor(b.p27.SEKO, b.p27.SUZU, b.p27.TAVE);
  /*p27.PASO*/ c.p27.PASO = nor(b.p27.TEVO, b.p27.PAHA);
  /*p27.VETU*/ c.p27.VETU = and(b.p27.TEVO, b.p27.PORE);
  /*p27.ROCO*/ c.p27.ROCO = not(b.p24.SEGU);
  /*p27.NYFO*/ c.p27.NYFO = not(b.p27.NUNY);
  /*p27.MOSU*/ c.p27.MOSU = not(b.p27.NYFO);
  /*p27.NYXU*/ c.p27.NYXU = nor(b.p29.AVAP, b.p27.MOSU, b.p27.TEVO);
  /*p27.WAZY*/ c.p27.WAZY = not(b.p27.PORE);

  /*p27.NOPA*/ c.p27.NOPA = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.sys.VID_RESETn1, b.p27.NOPA, b.p27.PYNU);
  /*p27.PYCO*/ c.p27.PYCO = tock_pos(a.p27.ROCO,          b.p27.ROCO,          b.sys.VID_RESETn1, b.p27.PYCO, b.p27.WIN_MATCH);
  /*p27.NUNU*/ c.p27.NUNU = tock_pos(a.p21.CLK_xBxDxFxHb, b.p21.CLK_xBxDxFxHb, b.sys.VID_RESETn1, b.p27.NUNU, b.p27.PYCO);
  /*p27.RYKU*/ c.p27.RYKU = tock_pos(a.p27.PECU,          b.p27.PECU,          b.p27.PASO,        b.p27.RYKU, !b.p27.RYKU);
  /*p27.ROGA*/ c.p27.ROGA = tock_pos(!a.p27.RYKU,         !b.p27.RYKU,         b.p27.PASO,        b.p27.ROGA, !b.p27.ROGA);
  /*p27.RUBU*/ c.p27.RUBU = tock_pos(!a.p27.ROGA,         !b.p27.ROGA,         b.p27.PASO,        b.p27.RUBU, !b.p27.RUBU);
  /*p27.NYZE*/ c.p27.NYZE = tock_pos(a.p21.CLK_xBxDxFxHa, b.p21.CLK_xBxDxFxHa, b.p21.RENDERING,  b.p27.NYZE, b.p27.PUXA);
  /*p27.PUXA*/ c.p27.PUXA = tock_pos(a.p24.ROXO,          b.p24.ROXO,          b.p21.RENDERING,  b.p27.PUXA, b.p27.POHU);
  /*p27.RYFA*/ c.p27.RYFA = tock_pos(a.p24.SEGU,          b.p24.SEGU,          b.p21.RENDERING,  b.p27.RYFA, b.p27.PANY);
  /*p27.RENE*/ c.p27.RENE = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.p21.RENDERING,  b.p27.RENE, b.p27.RYFA);
  /*p27.SOVY*/ c.p27.SOVY = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.sys.VID_RESETn1, b.p27.SOVY, b.p27.RYDY);

  //----------
  // dunno, top right

  /*p27.MOCE*/ c.p27.MOCE = nand(b.p27.LAXU, b.p27.NYVA, b.p27.NYXU);
  /*p27.LYRY*/ c.p27.LYRY = not(b.p27.MOCE);
  /*p27.LAXE*/ c.p27.LAXE = not(b.p27.LAXU);
  /*p27.MYSO*/ c.p27.MYSO = nor(b.p24.LOBY, b.p27.LAXE, b.p27.LYZU);
  /*p27.NAKO*/ c.p27.NAKO = not(b.p27.MESU);
  /*p27.NOFU*/ c.p27.NOFU = not(b.p27.NYVA);
  /*p27.MOFU*/ c.p27.MOFU = and(b.p27.MYSO, b.p27.NAKO);
  /*p27.NOGU*/ c.p27.NOGU = nand(b.p27.NAKO, b.p27.NOFU);
  /*p27.NYDY*/ c.p27.NYDY = nand(b.p27.MYSO, b.p27.MESU, b.p27.NOFU);


  /*p27.NENY*/ c.p27.NENY = not(b.p27.NOGU);
  /*p27.LURY*/ c.p27.LURY = and(!b.p27.LOVY, b.p21.RENDERING);
  /*p27.LONY*/ c.p27.LONY = unk2(b.p27.LURY, !b.p27.LOVY);
  /*p27.MYMA*/ c.p27.MYMA = not(b.p27.LONY);
  /*p27.LUSU*/ c.p27.LUSU = not(b.p27.LONY);
  /*p27.LENA*/ c.p27.LENA = not(b.p27.LUSU);
  /*p27.POTU*/ c.p27.POTU = and(b.p27.LENA, b.p27.NENY);
  /*p27.NETA*/ c.p27.NETA = and(b.p27.LENA, b.p27.NOGU);

  /*p27.LYZU*/ c.p27.LYZU = tock_pos( a.sys.CLK_AxCxExGx4,  b.sys.CLK_AxCxExGx4, b.p21.RENDERING, b.p27.LYZU,  b.p27.LAXU);

  // weird clock is weird
  /*p27.LEBO*/ c.p27.LEBO = nand(b.sys.CLK_AxCxExGx4, b.p27.MOCE);
  /*p27.LAXU*/ c.p27.LAXU = tock_pos( a.p27.LEBO,  b.p27.LEBO, b.p27.NYXU, b.p27.LAXU, !b.p27.LAXU);
  /*p27.MESU*/ c.p27.MESU = tock_pos(!a.p27.LAXU, !b.p27.LAXU, b.p27.NYXU, b.p27.MESU, !b.p27.MESU);
  /*p27.NYVA*/ c.p27.NYVA = tock_pos(!a.p27.MESU, !b.p27.MESU, b.p27.NYXU, b.p27.NYVA, !b.p27.NYVA);
  /*p27.LOVY*/ c.p27.LOVY = tock_pos( a.p21.CLK_xBxDxFxHc,  b.p21.CLK_xBxDxFxHc, b.p27.NYXU, b.p27.LOVY,  b.p27.LYRY);

  //----------
  // address output bus

  /*p27.WYKA*/ c.p27.MA00 = tock_pos( a.p27.VETU,  b.p27.VETU, b.p27.RST_XACO, b.p27.MA00, !b.p27.MA00);
  /*p27.WODY*/ c.p27.MA01 = tock_pos(!a.p27.MA00, !b.p27.MA00, b.p27.RST_XACO, b.p27.MA01, !b.p27.MA01);
  /*p27.WOBO*/ c.p27.MA02 = tock_pos(!a.p27.MA01, !b.p27.MA01, b.p27.RST_XACO, b.p27.MA02, !b.p27.MA02);
  /*p27.WYKO*/ c.p27.MA03 = tock_pos(!a.p27.MA02, !b.p27.MA02, b.p27.RST_XACO, b.p27.MA03, !b.p27.MA03);
  /*p27.XOLO*/ c.p27.MA04 = tock_pos(!a.p27.MA03, !b.p27.MA03, b.p27.RST_XACO, b.p27.MA04, !b.p27.MA04);

  /*p27.XUHA*/ c.p27.MA00_2 = not(b.p27.NOFU);
  /*p27.VYNO*/ c.p27.MA01_2 = tock_pos( a.p27.WAZY,  b.p27.WAZY,     b.p27.IN_FRAME_Yn, b.p27.MA01_2, !b.p27.MA01_2);
  /*p27.VUJO*/ c.p27.MA02_2 = tock_pos(!a.p27.MA01_2, !b.p27.MA01_2, b.p27.IN_FRAME_Yn, b.p27.MA01_2, !b.p27.MA01_2);
  /*p27.VYMU*/ c.p27.MA03_2 = tock_pos(!a.p27.MA02_2, !b.p27.MA02_2, b.p27.IN_FRAME_Yn, b.p27.MA02_2, !b.p27.MA02_2);

  // wait what? 3 clocking 5?
  /*p27.TUFU*/ c.p27.MA05 = tock_pos(!a.p27.MA03_2, !b.p27.MA03_2, b.p27.IN_FRAME_Yn, b.p27.MA05, !b.p27.MA05);
  /*p27.TAXA*/ c.p27.MA06 = tock_pos(!a.p27.MA05,   !b.p27.MA05,   b.p27.IN_FRAME_Yn, b.p27.MA06, !b.p27.MA06);
  /*p27.TOZO*/ c.p27.MA07 = tock_pos(!a.p27.MA06,   !b.p27.MA06,   b.p27.IN_FRAME_Yn, b.p27.MA07, !b.p27.MA07);
  /*p27.TATE*/ c.p27.MA08 = tock_pos(!a.p27.MA07,   !b.p27.MA07,   b.p27.IN_FRAME_Yn, b.p27.MA08, !b.p27.MA08);
  /*p27.TEKE*/ c.p27.MA09 = tock_pos(!a.p27.MA08,   !b.p27.MA08,   b.p27.IN_FRAME_Yn, b.p27.MA09, !b.p27.MA09);




  // this is not right. where is VYPO/CUBA in the schematic?
  //c.p27.VYPO = not(in.CUBA1);

  // this is what the die looks like
  /*p27.VYPO*/ c.p27.P10_Bn = not(b.chip.P10_B);

  /*p27.XEJA*/ if (b.p25.WIN_TILE_READ) c.MA00 = b.p27.MA00;
  /*p27.XAMO*/ if (b.p25.WIN_TILE_READ) c.MA01 = b.p27.MA01;
  /*p27.XAHE*/ if (b.p25.WIN_TILE_READ) c.MA02 = b.p27.MA02;
  /*p27.XULO*/ if (b.p25.WIN_TILE_READ) c.MA03 = b.p27.MA03;
  /*p27.WUJU*/ if (b.p25.WIN_TILE_READ) c.MA04 = b.p27.MA04;
  /*p27.VYTO*/ if (b.p25.WIN_TILE_READ) c.MA05 = b.p27.MA05;
  /*p27.VEHA*/ if (b.p25.WIN_TILE_READ) c.MA06 = b.p27.MA06;
  /*p27.VACE*/ if (b.p25.WIN_TILE_READ) c.MA07 = b.p27.MA07;
  /*p27.VOVO*/ if (b.p25.WIN_TILE_READ) c.MA08 = b.p27.MA08;
  /*p27.VULO*/ if (b.p25.WIN_TILE_READ) c.MA09 = b.p27.MA09;

  /*p27.VEVY*/ c.p27.MA10 = not(b.p23.WIN_MAP_SEL);
  /*p27.VEZA*/ c.p27.MA11 = not(b.p27.P10_Bn);
  /*p27.VOGU*/ c.p27.MA12 = not(b.p27.P10_Bn);

  /*p27.VEVY*/ if (b.p25.WIN_TILE_READ) c.MA10 = b.p27.MA10;
  /*p27.VEZA*/ if (b.p25.WIN_TILE_READ) c.MA11 = b.p27.MA11;
  /*p27.VOGU*/ if (b.p25.WIN_TILE_READ) c.MA12 = b.p27.MA12;

  //----------
  // computes SECA and TAVA

  /*p27.RYCE*/ c.p27.RYCE = and(b.p27.SOBU, !b.p27.SUDA);
  /*p27.SECA*/ c.p27.SECA = nor(b.p27.RYCE, b.sys.VID_RESET5, b.p28.SPRITE_COUNT_RST);
  /*p27.VEKU*/ c.p27.VEKU = nor(b.p29.WUTY, b.p27.TAVE);
  /*p27.TAKA*/ c.p27.TAKA = unk2(b.p27.VEKU, b.p27.SECA);
  /*p27.TUKU*/ c.p27.TUKU = not(b.p24.TOMU);
  /*p27.SOWO*/ c.p27.SOWO = not(b.p27.TAKA);
  /*p27.TEKY*/ c.p27.TEKY = and(b.p29.OAM_SCAN, b.p27.TUKU, b.p27.LYRY, b.p27.SOWO);

  /*p27.SOBU*/ c.p27.SOBU = tock_pos(a.p21.CLK_AxCxExGxa, b.p21.CLK_AxCxExGxa, b.p27.P10_Bn, b.p27.SOBU, b.p27.TEKY);
  /*p27.SUDA*/ c.p27.SUDA = tock_pos(a.sys.CLK_xBxDxFxH5, b.sys.CLK_xBxDxFxH5, b.p27.P10_Bn, b.p27.SUDA, b.p27.SOBU);
}
