#include "Sch_Merged.h"

#include "Sch_Common.h"
#include "Sch_Pins.h"
#include "TestGB.h"
#include "Constants.h"

using namespace Schematics;

void TestGB::tick_pixpipe() {
  auto clk_sig = clk_reg.sig(*this);
  auto win_sig = win_reg.sig(*this);
  auto dbg_sig = dbg_reg.sig(*this);
  auto ppu_sig = ppu_reg.sig(*this);
  auto sst_sig = sst_reg.sig(*this);

  /*p24.LOBY*/ wire LOBY_RENDERINGn = not(ppu_reg.XYMU_RENDERINGp.q());

  /*p27.NOFU*/ wire NOFU_FETCH_S2n = not(ppu_reg.NYVA_BFETCH_S2.q());
  /*p29.TYTU*/ wire TYTU_SFETCH_S0_D0n = not(ppu_reg.TOXE_SFETCH_S0_D0);
  /*p27.NAKO*/ wire NAKO_FETCH_S1n = not(ppu_reg.MESU_BFETCH_S1.q());

  //----------------------------------------

  /*p27.LAXE*/ wire _LAXE_FETCH_S0n = not(ppu_reg.LAXU_BFETCH_S0.q());
  /*p27.MYSO*/ wire _MYSO_BG_TRIGn = nor(LOBY_RENDERINGn, _LAXE_FETCH_S0n, ppu_reg.LYZU_BFETCH_S0_DELAY.q());

  //----------
  // PPU VRAM read

  // Background/window tile read B handler
  {
    // Clock polarity wrong? Check regs on die.

    // BFETCH_000 - LABU_BG_PIX_B_CLKp = 1;
    // BFETCH_001 - LABU_BG_PIX_B_CLKp = or (!vid_reg.RENDERING_LATCHp.q(), vid_reg.BFETCH_S0_DELAY.q())
    // BFETCH_010 - LABU_BG_PIX_B_CLKp = 1;
    // BFETCH_011 - LABU_BG_PIX_B_CLKp = 1;
    // BFETCH_100 - LABU_BG_PIX_B_CLKp = 1;
    // BFETCH_101 - LABU_BG_PIX_B_CLKp = or (!vid_reg.RENDERING_LATCHp.q(), vid_reg.BFETCH_S0_DELAY.q())

    /*p27.MOFU*/ wire _MOFU_CLKn = and (_MYSO_BG_TRIGn, NAKO_FETCH_S1n);
    /*p32.LESO*/ wire _LESO_CLKp = not(_MOFU_CLKn);
    /*p32.AJAR*/ wire _AJAR_CLKn = not(_LESO_CLKp);
    /*p32.LABU*/ wire _LABU_CLKp = not(_AJAR_CLKn);

    /*p32.RAWU*/ pxp_reg.BG_PIX_B0.set(_LABU_CLKp, dbg_sig.VYPO_P10_Bn, ppu_reg.MD0);
    /*p32.POZO*/ pxp_reg.BG_PIX_B1.set(_LABU_CLKp, dbg_sig.VYPO_P10_Bn, ppu_reg.MD1);
    /*p32.PYZO*/ pxp_reg.BG_PIX_B2.set(_LABU_CLKp, dbg_sig.VYPO_P10_Bn, ppu_reg.MD2);
    /*p32.POXA*/ pxp_reg.BG_PIX_B3.set(_LABU_CLKp, dbg_sig.VYPO_P10_Bn, ppu_reg.MD3);
    /*p32.PULO*/ pxp_reg.BG_PIX_B4.set(_LABU_CLKp, dbg_sig.VYPO_P10_Bn, ppu_reg.MD4);
    /*p32.POJU*/ pxp_reg.BG_PIX_B5.set(_LABU_CLKp, dbg_sig.VYPO_P10_Bn, ppu_reg.MD5);
    /*p32.POWY*/ pxp_reg.BG_PIX_B6.set(_LABU_CLKp, dbg_sig.VYPO_P10_Bn, ppu_reg.MD6);
    /*p32.PYJU*/ pxp_reg.BG_PIX_B7.set(_LABU_CLKp, dbg_sig.VYPO_P10_Bn, ppu_reg.MD7);
  }

  // Sprite tile read handler
  {
    // SFETCH_000 - TOPU_SPRITE_PIX_LATCH_A = 0, 
    // SFETCH_001
    // SFETCH_010
    // SFETCH_011
    // SFETCH_100
    // SFETCH_101
    // SFETCH_110
    // SFETCH_111

    /*p29.SYCU*/ wire _SYCU = nor(TYTU_SFETCH_S0_D0n, LOBY_RENDERINGn, ppu_reg.TYFO_SFETCH_S0_D1);
    /*p29.TOPU*/ wire _TOPU_SPRITE_PIX_LATCH_A = and (ppu_reg.TULY_SFETCH_S1, _SYCU);
    /*p29.RACA*/ wire _RACA_SPRITE_PIX_LATCH_B = and (ppu_reg.VONU_SFETCH_S1_D4, _SYCU);

    /*p29.XONO*/ wire _XONO_FLIP_X = and (oam_reg.REG_OAM_A5, ppu_sig.TEXY_SPRITE_READ);
    /*p33.POBE*/ wire _SPR_PIX_FLIP0 = mux2_p(ppu_reg.MD7, ppu_reg.MD0, _XONO_FLIP_X);
    /*p33.PACY*/ wire _SPR_PIX_FLIP1 = mux2_p(ppu_reg.MD6, ppu_reg.MD1, _XONO_FLIP_X);
    /*p33.PONO*/ wire _SPR_PIX_FLIP2 = mux2_p(ppu_reg.MD5, ppu_reg.MD2, _XONO_FLIP_X);
    /*p33.PUGU*/ wire _SPR_PIX_FLIP3 = mux2_p(ppu_reg.MD4, ppu_reg.MD3, _XONO_FLIP_X);
    /*p33.PUTE*/ wire _SPR_PIX_FLIP4 = mux2_p(ppu_reg.MD3, ppu_reg.MD4, _XONO_FLIP_X);
    /*p33.PULY*/ wire _SPR_PIX_FLIP5 = mux2_p(ppu_reg.MD2, ppu_reg.MD5, _XONO_FLIP_X);
    /*p33.PELO*/ wire _SPR_PIX_FLIP6 = mux2_p(ppu_reg.MD1, ppu_reg.MD6, _XONO_FLIP_X);
    /*p33.PAWE*/ wire _SPR_PIX_FLIP7 = mux2_p(ppu_reg.MD0, ppu_reg.MD7, _XONO_FLIP_X);

    /*p29.VYWA*/ wire _VYWA_CLKp = not(_TOPU_SPRITE_PIX_LATCH_A);
    /*p29.WENY*/ wire _WENY_CLKn = not(_VYWA_CLKp);
    /*p29.XADO*/ wire _XADO_CLKp = not(_WENY_CLKn);

    /*p33.PEFO*/ pxp_reg.SPR_PIX_A0.set(_XADO_CLKp, _SPR_PIX_FLIP0);
    /*p33.ROKA*/ pxp_reg.SPR_PIX_A1.set(_XADO_CLKp, _SPR_PIX_FLIP1);
    /*p33.MYTU*/ pxp_reg.SPR_PIX_A2.set(_XADO_CLKp, _SPR_PIX_FLIP2);
    /*p33.RAMU*/ pxp_reg.SPR_PIX_A3.set(_XADO_CLKp, _SPR_PIX_FLIP3);
    /*p33.SELE*/ pxp_reg.SPR_PIX_A4.set(_XADO_CLKp, _SPR_PIX_FLIP4);
    /*p33.SUTO*/ pxp_reg.SPR_PIX_A5.set(_XADO_CLKp, _SPR_PIX_FLIP5);
    /*p33.RAMA*/ pxp_reg.SPR_PIX_A6.set(_XADO_CLKp, _SPR_PIX_FLIP6);
    /*p33.RYDU*/ pxp_reg.SPR_PIX_A7.set(_XADO_CLKp, _SPR_PIX_FLIP7);

    /*p29.PEBY*/ wire _PEBY_CLKp = not(_RACA_SPRITE_PIX_LATCH_B);
    /*p29.NYBE*/ wire _NYBE_CLKn = not(_PEBY_CLKp);
    /*p29.PUCO*/ wire _PUCO_CLKb = not(_NYBE_CLKn);

    /*p33.REWO*/ pxp_reg.SPR_PIX_B0.set(_PUCO_CLKb, _SPR_PIX_FLIP0);
    /*p33.PEBA*/ pxp_reg.SPR_PIX_B1.set(_PUCO_CLKb, _SPR_PIX_FLIP1);
    /*p33.MOFO*/ pxp_reg.SPR_PIX_B2.set(_PUCO_CLKb, _SPR_PIX_FLIP2);
    /*p33.PUDU*/ pxp_reg.SPR_PIX_B3.set(_PUCO_CLKb, _SPR_PIX_FLIP3);
    /*p33.SAJA*/ pxp_reg.SPR_PIX_B4.set(_PUCO_CLKb, _SPR_PIX_FLIP4);
    /*p33.SUNY*/ pxp_reg.SPR_PIX_B5.set(_PUCO_CLKb, _SPR_PIX_FLIP5);
    /*p33.SEMO*/ pxp_reg.SPR_PIX_B6.set(_PUCO_CLKb, _SPR_PIX_FLIP6);
    /*p33.SEGA*/ pxp_reg.SPR_PIX_B7.set(_PUCO_CLKb, _SPR_PIX_FLIP7);
  }


  // Background pipe A
  {
    // Hax

    // BFETCH_000 - LOMA_BG_LATCHn = 1;
    // BFETCH_001 - LOMA_BG_LATCHn = 1;
    // BFETCH_010 - LOMA_BG_LATCHn = 1;
    // BFETCH_011 - LOMA_BG_LATCHn = or(!vid_reg.RENDERING_LATCHp.q(), vid_reg.BFETCH_S0_DELAY.q());
    // BFETCH_100 - LOMA_BG_LATCHn = 1;
    // BFETCH_101 - LOMA_BG_LATCHn = 1;

    // Original
    /*p27.NYDY*/ wire _NYDY_BG_LATCHn = nand(_MYSO_BG_TRIGn, ppu_reg.MESU_BFETCH_S1.q(), NOFU_FETCH_S2n);
    /*p32.METE*/ wire _METE_BG_LATCHp = not(_NYDY_BG_LATCHn);
    /*p32.LOMA*/ wire _LOMA_BG_LATCHn = not(_METE_BG_LATCHp);

    // LEGU01 << LOMA02
    // LEGU02 << ????? could be TEME?
    // LEGU03 nc
    // LEGU04 << "comp clock"
    // LEGU05 nc
    // LEGU06 nc
    // LEGU07 >> LAKY02, LUHE01
    // LEGU08 nc

    // "comp clock" == not(LOMA02) - so it's just an inverted clock signal

    /*p32.LEGU*/ pxp_reg.BG_PIX_A0.set(_LOMA_BG_LATCHn, ppu_reg.MD0);
    /*p32.NUDU*/ pxp_reg.BG_PIX_A1.set(_LOMA_BG_LATCHn, ppu_reg.MD1);
    /*p32.MUKU*/ pxp_reg.BG_PIX_A2.set(_LOMA_BG_LATCHn, ppu_reg.MD2);
    /*p32.LUZO*/ pxp_reg.BG_PIX_A3.set(_LOMA_BG_LATCHn, ppu_reg.MD3);
    /*p32.MEGU*/ pxp_reg.BG_PIX_A4.set(_LOMA_BG_LATCHn, ppu_reg.MD4);
    /*p32.MYJY*/ pxp_reg.BG_PIX_A5.set(_LOMA_BG_LATCHn, ppu_reg.MD5);
    /*p32.NASA*/ pxp_reg.BG_PIX_A6.set(_LOMA_BG_LATCHn, ppu_reg.MD6);
    /*p32.NEFO*/ pxp_reg.BG_PIX_A7.set(_LOMA_BG_LATCHn, ppu_reg.MD7);

    /*p32.LOZE*/ wire _LOZE_PIPE_A_LOAD = not(ppu_sig.NYXU_BFETCH_RSTn);

    /*p32.LAKY*/ wire _BG_PIPE_A_SET0 = nand(_LOZE_PIPE_A_LOAD, pxp_reg.BG_PIX_A0);
    /*p32.NYXO*/ wire _BG_PIPE_A_SET1 = nand(_LOZE_PIPE_A_LOAD, pxp_reg.BG_PIX_A1);
    /*p32.LOTO*/ wire _BG_PIPE_A_SET2 = nand(_LOZE_PIPE_A_LOAD, pxp_reg.BG_PIX_A2);
    /*p32.LYDU*/ wire _BG_PIPE_A_SET3 = nand(_LOZE_PIPE_A_LOAD, pxp_reg.BG_PIX_A3);
    /*p32.MYVY*/ wire _BG_PIPE_A_SET4 = nand(_LOZE_PIPE_A_LOAD, pxp_reg.BG_PIX_A4);
    /*p32.LODO*/ wire _BG_PIPE_A_SET5 = nand(_LOZE_PIPE_A_LOAD, pxp_reg.BG_PIX_A5);
    /*p32.NUTE*/ wire _BG_PIPE_A_SET6 = nand(_LOZE_PIPE_A_LOAD, pxp_reg.BG_PIX_A6);
    /*p32.NAJA*/ wire _BG_PIPE_A_SET7 = nand(_LOZE_PIPE_A_LOAD, pxp_reg.BG_PIX_A7);

    /*p32.LUHE*/ wire _BG_PIX_A0n = not(pxp_reg.BG_PIX_A0);
    /*p32.NOLY*/ wire _BG_PIX_A1n = not(pxp_reg.BG_PIX_A1);
    /*p32.LEKE*/ wire _BG_PIX_A2n = not(pxp_reg.BG_PIX_A2);
    /*p32.LOMY*/ wire _BG_PIX_A3n = not(pxp_reg.BG_PIX_A3);
    /*p32.LALA*/ wire _BG_PIX_A4n = not(pxp_reg.BG_PIX_A4);
    /*p32.LOXA*/ wire _BG_PIX_A5n = not(pxp_reg.BG_PIX_A5);
    /*p32.NEZE*/ wire _BG_PIX_A6n = not(pxp_reg.BG_PIX_A6);
    /*p32.NOBO*/ wire _BG_PIX_A7n = not(pxp_reg.BG_PIX_A7);

    /*p32.LOTY*/ wire _BG_PIPE_A_RST0 = nand(_LOZE_PIPE_A_LOAD, _BG_PIX_A0n);
    /*p32.NEXA*/ wire _BG_PIPE_A_RST1 = nand(_LOZE_PIPE_A_LOAD, _BG_PIX_A1n);
    /*p32.LUTU*/ wire _BG_PIPE_A_RST2 = nand(_LOZE_PIPE_A_LOAD, _BG_PIX_A2n);
    /*p32.LUJA*/ wire _BG_PIPE_A_RST3 = nand(_LOZE_PIPE_A_LOAD, _BG_PIX_A3n);
    /*p32.MOSY*/ wire _BG_PIPE_A_RST4 = nand(_LOZE_PIPE_A_LOAD, _BG_PIX_A4n);
    /*p32.LERU*/ wire _BG_PIPE_A_RST5 = nand(_LOZE_PIPE_A_LOAD, _BG_PIX_A5n);
    /*p32.NYHA*/ wire _BG_PIPE_A_RST6 = nand(_LOZE_PIPE_A_LOAD, _BG_PIX_A6n);
    /*p32.NADY*/ wire _BG_PIPE_A_RST7 = nand(_LOZE_PIPE_A_LOAD, _BG_PIX_A7n);

    /*p32.MYDE*/ pxp_reg.BG_PIPE_A0.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _BG_PIPE_A_SET0, _BG_PIPE_A_RST0, joy_pin.P10_B);
    /*p32.NOZO*/ pxp_reg.BG_PIPE_A1.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _BG_PIPE_A_SET1, _BG_PIPE_A_RST1, pxp_reg.BG_PIPE_A0);
    /*p32.MOJU*/ pxp_reg.BG_PIPE_A2.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _BG_PIPE_A_SET2, _BG_PIPE_A_RST2, pxp_reg.BG_PIPE_A1);
    /*p32.MACU*/ pxp_reg.BG_PIPE_A3.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _BG_PIPE_A_SET3, _BG_PIPE_A_RST3, pxp_reg.BG_PIPE_A2);
    /*p32.NEPO*/ pxp_reg.BG_PIPE_A4.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _BG_PIPE_A_SET4, _BG_PIPE_A_RST4, pxp_reg.BG_PIPE_A3);
    /*p32.MODU*/ pxp_reg.BG_PIPE_A5.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _BG_PIPE_A_SET5, _BG_PIPE_A_RST5, pxp_reg.BG_PIPE_A4);
    /*p32.NEDA*/ pxp_reg.BG_PIPE_A6.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _BG_PIPE_A_SET6, _BG_PIPE_A_RST6, pxp_reg.BG_PIPE_A5);
    /*p32.PYBO*/ pxp_reg.BG_PIPE_A7.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _BG_PIPE_A_SET7, _BG_PIPE_A_RST7, pxp_reg.BG_PIPE_A6);
  }

  // Background pipe B
  {
    /*p32.LUXA*/ wire _LUXA_PIPE_B_LOAD = not(ppu_sig.NYXU_BFETCH_RSTn);
    /*p32.TUXE*/ wire _BG_PIPE_B_SET0 = nand(_LUXA_PIPE_B_LOAD, !pxp_reg.BG_PIX_B0);
    /*p32.SOLY*/ wire _BG_PIPE_B_SET1 = nand(_LUXA_PIPE_B_LOAD, !pxp_reg.BG_PIX_B1);
    /*p32.RUCE*/ wire _BG_PIPE_B_SET2 = nand(_LUXA_PIPE_B_LOAD, !pxp_reg.BG_PIX_B2);
    /*p32.RYJA*/ wire _BG_PIPE_B_SET3 = nand(_LUXA_PIPE_B_LOAD, !pxp_reg.BG_PIX_B3);
    /*p32.RUTO*/ wire _BG_PIPE_B_SET4 = nand(_LUXA_PIPE_B_LOAD, !pxp_reg.BG_PIX_B4);
    /*p32.RAJA*/ wire _BG_PIPE_B_SET5 = nand(_LUXA_PIPE_B_LOAD, !pxp_reg.BG_PIX_B5);
    /*p32.RAJO*/ wire _BG_PIPE_B_SET6 = nand(_LUXA_PIPE_B_LOAD, !pxp_reg.BG_PIX_B6);
    /*p32.RAGA*/ wire _BG_PIPE_B_SET7 = nand(_LUXA_PIPE_B_LOAD, !pxp_reg.BG_PIX_B7);

    /*p32.SEJA*/ wire _BG_PIPE_B_RST0 = nand(_LUXA_PIPE_B_LOAD, /*p32.TOSA*/ not(!pxp_reg.BG_PIX_B0));
    /*p32.SENO*/ wire _BG_PIPE_B_RST1 = nand(_LUXA_PIPE_B_LOAD, /*p32.RUCO*/ not(!pxp_reg.BG_PIX_B1));
    /*p32.SURE*/ wire _BG_PIPE_B_RST2 = nand(_LUXA_PIPE_B_LOAD, /*p32.TYCE*/ not(!pxp_reg.BG_PIX_B2));
    /*p32.SEBO*/ wire _BG_PIPE_B_RST3 = nand(_LUXA_PIPE_B_LOAD, /*p32.REVY*/ not(!pxp_reg.BG_PIX_B3));
    /*p32.SUCA*/ wire _BG_PIPE_B_RST4 = nand(_LUXA_PIPE_B_LOAD, /*p32.RYGA*/ not(!pxp_reg.BG_PIX_B4));
    /*p32.SYWE*/ wire _BG_PIPE_B_RST5 = nand(_LUXA_PIPE_B_LOAD, /*p32.RYLE*/ not(!pxp_reg.BG_PIX_B5));
    /*p32.SUPU*/ wire _BG_PIPE_B_RST6 = nand(_LUXA_PIPE_B_LOAD, /*p32.RAPU*/ not(!pxp_reg.BG_PIX_B6));
    /*p32.RYJY*/ wire _BG_PIPE_B_RST7 = nand(_LUXA_PIPE_B_LOAD, /*p32.SOJA*/ not(!pxp_reg.BG_PIX_B7));

    /*p32.TOMY*/ pxp_reg.BG_PIPE_B0.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _BG_PIPE_B_SET0, _BG_PIPE_B_RST0, joy_pin.P10_B);
    /*p32.TACA*/ pxp_reg.BG_PIPE_B1.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _BG_PIPE_B_SET1, _BG_PIPE_B_RST1, pxp_reg.BG_PIPE_B0);
    /*p32.SADY*/ pxp_reg.BG_PIPE_B2.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _BG_PIPE_B_SET2, _BG_PIPE_B_RST2, pxp_reg.BG_PIPE_B1);
    /*p32.RYSA*/ pxp_reg.BG_PIPE_B3.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _BG_PIPE_B_SET3, _BG_PIPE_B_RST3, pxp_reg.BG_PIPE_B2);
    /*p32.SOBO*/ pxp_reg.BG_PIPE_B4.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _BG_PIPE_B_SET4, _BG_PIPE_B_RST4, pxp_reg.BG_PIPE_B3);
    /*p32.SETU*/ pxp_reg.BG_PIPE_B5.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _BG_PIPE_B_SET5, _BG_PIPE_B_RST5, pxp_reg.BG_PIPE_B4);
    /*p32.RALU*/ pxp_reg.BG_PIPE_B6.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _BG_PIPE_B_SET6, _BG_PIPE_B_RST6, pxp_reg.BG_PIPE_B5);
    /*p32.SOHU*/ pxp_reg.BG_PIPE_B7.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _BG_PIPE_B_SET7, _BG_PIPE_B_RST7, pxp_reg.BG_PIPE_B6);
  }

  {
    /*p29.XEFY*/ wire _XEPY_SPRITE_DONEn = not(ppu_sig.WUTY_SPRITE_DONE);
    /*p34.MEFU*/ wire _SPRITE_MASK0 = or (_XEPY_SPRITE_DONEn, pxp_reg.SPR_PIPE_A0, pxp_reg.SPR_PIPE_B0);
    /*p34.MEVE*/ wire _SPRITE_MASK1 = or (_XEPY_SPRITE_DONEn, pxp_reg.SPR_PIPE_A1, pxp_reg.SPR_PIPE_B1);
    /*p34.MYZO*/ wire _SPRITE_MASK2 = or (_XEPY_SPRITE_DONEn, pxp_reg.SPR_PIPE_A2, pxp_reg.SPR_PIPE_B2);
    /*p34.RUDA*/ wire _SPRITE_MASK3 = or (_XEPY_SPRITE_DONEn, pxp_reg.SPR_PIPE_A3, pxp_reg.SPR_PIPE_B3);
    /*p34.VOTO*/ wire _SPRITE_MASK4 = or (_XEPY_SPRITE_DONEn, pxp_reg.SPR_PIPE_A4, pxp_reg.SPR_PIPE_B4);
    /*p34.VYSA*/ wire _SPRITE_MASK5 = or (_XEPY_SPRITE_DONEn, pxp_reg.SPR_PIPE_A5, pxp_reg.SPR_PIPE_B5);
    /*p34.TORY*/ wire _SPRITE_MASK6 = or (_XEPY_SPRITE_DONEn, pxp_reg.SPR_PIPE_A6, pxp_reg.SPR_PIPE_B6);
    /*p34.WOPE*/ wire _SPRITE_MASK7 = or (_XEPY_SPRITE_DONEn, pxp_reg.SPR_PIPE_A7, pxp_reg.SPR_PIPE_B7);

    /*p34.LESY*/ wire _SPRITE_MASK0n = not(_SPRITE_MASK0);
    /*p34.LOTA*/ wire _SPRITE_MASK1n = not(_SPRITE_MASK1);
    /*p34.LYKU*/ wire _SPRITE_MASK2n = not(_SPRITE_MASK2);
    /*p34.ROBY*/ wire _SPRITE_MASK3n = not(_SPRITE_MASK3);
    /*p34.TYTA*/ wire _SPRITE_MASK4n = not(_SPRITE_MASK4);
    /*p34.TYCO*/ wire _SPRITE_MASK5n = not(_SPRITE_MASK5);
    /*p34.SOKA*/ wire _SPRITE_MASK6n = not(_SPRITE_MASK6);
    /*p34.XOVU*/ wire _SPRITE_MASK7n = not(_SPRITE_MASK7);

    // Sprite pipe A
    {
      /*p33.MEZU*/ wire _SPR_PIX_A_SET0 = nand(_SPRITE_MASK0n, pxp_reg.SPR_PIX_A0);
      /*p33.RUSY*/ wire _SPR_PIX_A_SET1 = nand(_SPRITE_MASK1n, pxp_reg.SPR_PIX_A1);
      /*p33.MYXA*/ wire _SPR_PIX_A_SET2 = nand(_SPRITE_MASK2n, pxp_reg.SPR_PIX_A2);
      /*p33.RANO*/ wire _SPR_PIX_A_SET3 = nand(_SPRITE_MASK3n, pxp_reg.SPR_PIX_A3);
      /*p33.TYGA*/ wire _SPR_PIX_A_SET4 = nand(_SPRITE_MASK4n, pxp_reg.SPR_PIX_A4);
      /*p33.VUME*/ wire _SPR_PIX_A_SET5 = nand(_SPRITE_MASK5n, pxp_reg.SPR_PIX_A5);
      /*p33.TAPO*/ wire _SPR_PIX_A_SET6 = nand(_SPRITE_MASK6n, pxp_reg.SPR_PIX_A6);
      /*p33.TESO*/ wire _SPR_PIX_A_SET7 = nand(_SPRITE_MASK7n, pxp_reg.SPR_PIX_A7);

      /*p33.MOFY*/ wire _SPR_PIX_A_RST0 = nand(_SPRITE_MASK0n, /*p33.LOZA*/ not(pxp_reg.SPR_PIX_A0));
      /*p33.RUCA*/ wire _SPR_PIX_A_RST1 = nand(_SPRITE_MASK1n, /*p33.SYBO*/ not(pxp_reg.SPR_PIX_A1));
      /*p33.MAJO*/ wire _SPR_PIX_A_RST2 = nand(_SPRITE_MASK2n, /*p33.LUMO*/ not(pxp_reg.SPR_PIX_A2));
      /*p33.REHU*/ wire _SPR_PIX_A_RST3 = nand(_SPRITE_MASK3n, /*p33.SOLO*/ not(pxp_reg.SPR_PIX_A3));
      /*p33.WAXO*/ wire _SPR_PIX_A_RST4 = nand(_SPRITE_MASK4n, /*p33.VOBY*/ not(pxp_reg.SPR_PIX_A4));
      /*p33.XOLE*/ wire _SPR_PIX_A_RST5 = nand(_SPRITE_MASK5n, /*p33.WYCO*/ not(pxp_reg.SPR_PIX_A5));
      /*p33.TABY*/ wire _SPR_PIX_A_RST6 = nand(_SPRITE_MASK6n, /*p33.SERY*/ not(pxp_reg.SPR_PIX_A6));
      /*p33.TULA*/ wire _SPR_PIX_A_RST7 = nand(_SPRITE_MASK7n, /*p33.SELU*/ not(pxp_reg.SPR_PIX_A7));

      /*p33.NYLU*/ pxp_reg.SPR_PIPE_A0.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _SPR_PIX_A_SET0, _SPR_PIX_A_RST0, joy_pin.P10_B);
      /*p33.PEFU*/ pxp_reg.SPR_PIPE_A1.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _SPR_PIX_A_SET1, _SPR_PIX_A_RST1, pxp_reg.SPR_PIPE_A0);
      /*p33.NATY*/ pxp_reg.SPR_PIPE_A2.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _SPR_PIX_A_SET2, _SPR_PIX_A_RST2, pxp_reg.SPR_PIPE_A1);
      /*p33.PYJO*/ pxp_reg.SPR_PIPE_A3.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _SPR_PIX_A_SET3, _SPR_PIX_A_RST3, pxp_reg.SPR_PIPE_A2);
      /*p33.VARE*/ pxp_reg.SPR_PIPE_A4.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _SPR_PIX_A_SET4, _SPR_PIX_A_RST4, pxp_reg.SPR_PIPE_A3);
      /*p33.WEBA*/ pxp_reg.SPR_PIPE_A5.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _SPR_PIX_A_SET5, _SPR_PIX_A_RST5, pxp_reg.SPR_PIPE_A4);
      /*p33.VANU*/ pxp_reg.SPR_PIPE_A6.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _SPR_PIX_A_SET6, _SPR_PIX_A_RST6, pxp_reg.SPR_PIPE_A5);
      /*p33.VUPY*/ pxp_reg.SPR_PIPE_A7.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _SPR_PIX_A_SET7, _SPR_PIX_A_RST7, pxp_reg.SPR_PIPE_A6);
    }

    // Sprite pipe B
    {
      /*p33.PABE*/ wire _SPR_PIX_B_SET0 = nand(_SPRITE_MASK0n, pxp_reg.SPR_PIX_B0);
      /*p33.MYTO*/ wire _SPR_PIX_B_SET1 = nand(_SPRITE_MASK1n, pxp_reg.SPR_PIX_B1);
      /*p33.LELA*/ wire _SPR_PIX_B_SET2 = nand(_SPRITE_MASK2n, pxp_reg.SPR_PIX_B2);
      /*p33.MAME*/ wire _SPR_PIX_B_SET3 = nand(_SPRITE_MASK3n, pxp_reg.SPR_PIX_B3);
      /*p33.VEXU*/ wire _SPR_PIX_B_SET4 = nand(_SPRITE_MASK4n, pxp_reg.SPR_PIX_B4);
      /*p33.VABY*/ wire _SPR_PIX_B_SET5 = nand(_SPRITE_MASK5n, pxp_reg.SPR_PIX_B5);
      /*p33.TUXA*/ wire _SPR_PIX_B_SET6 = nand(_SPRITE_MASK6n, pxp_reg.SPR_PIX_B6);
      /*p33.VUNE*/ wire _SPR_PIX_B_SET7 = nand(_SPRITE_MASK7n, pxp_reg.SPR_PIX_B7);

      /*p33.PYZU*/ wire _SPR_PIX_B_RST0 = nand(_SPRITE_MASK0n, /*p33.RATA*/ not(pxp_reg.SPR_PIX_B0));
      /*p33.MADA*/ wire _SPR_PIX_B_RST1 = nand(_SPRITE_MASK1n, /*p33.NUCA*/ not(pxp_reg.SPR_PIX_B1));
      /*p33.LYDE*/ wire _SPR_PIX_B_RST2 = nand(_SPRITE_MASK2n, /*p33.LASE*/ not(pxp_reg.SPR_PIX_B2));
      /*p33.LUFY*/ wire _SPR_PIX_B_RST3 = nand(_SPRITE_MASK3n, /*p33.LUBO*/ not(pxp_reg.SPR_PIX_B3));
      /*p33.XATO*/ wire _SPR_PIX_B_RST4 = nand(_SPRITE_MASK4n, /*p33.WERY*/ not(pxp_reg.SPR_PIX_B4));
      /*p33.XEXU*/ wire _SPR_PIX_B_RST5 = nand(_SPRITE_MASK5n, /*p33.WURA*/ not(pxp_reg.SPR_PIX_B5));
      /*p33.TUPE*/ wire _SPR_PIX_B_RST6 = nand(_SPRITE_MASK6n, /*p33.SULU*/ not(pxp_reg.SPR_PIX_B6));
      /*p33.XYVE*/ wire _SPR_PIX_B_RST7 = nand(_SPRITE_MASK7n, /*p33.WAMY*/ not(pxp_reg.SPR_PIX_B7));

      /*p33.NURO*/ pxp_reg.SPR_PIPE_B0.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _SPR_PIX_B_SET0, _SPR_PIX_B_RST0, joy_pin.P10_B);
      /*p33.MASO*/ pxp_reg.SPR_PIPE_B1.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _SPR_PIX_B_SET1, _SPR_PIX_B_RST1, pxp_reg.SPR_PIPE_B0);
      /*p33.LEFE*/ pxp_reg.SPR_PIPE_B2.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _SPR_PIX_B_SET2, _SPR_PIX_B_RST2, pxp_reg.SPR_PIPE_B1);
      /*p33.LESU*/ pxp_reg.SPR_PIPE_B3.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _SPR_PIX_B_SET3, _SPR_PIX_B_RST3, pxp_reg.SPR_PIPE_B2);
      /*p33.WYHO*/ pxp_reg.SPR_PIPE_B4.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _SPR_PIX_B_SET4, _SPR_PIX_B_RST4, pxp_reg.SPR_PIPE_B3);
      /*p33.WORA*/ pxp_reg.SPR_PIPE_B5.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _SPR_PIX_B_SET5, _SPR_PIX_B_RST5, pxp_reg.SPR_PIPE_B4);
      /*p33.VAFO*/ pxp_reg.SPR_PIPE_B6.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _SPR_PIX_B_SET6, _SPR_PIX_B_RST6, pxp_reg.SPR_PIPE_B5);
      /*p33.WUFY*/ pxp_reg.SPR_PIPE_B7.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _SPR_PIX_B_SET7, _SPR_PIX_B_RST7, pxp_reg.SPR_PIPE_B6);
    }

    // Palette pipe
    {
      /*p34.PUME*/ wire _SPRITE_PAL_PIPE_SET0n = nand(_SPRITE_MASK0n, oam_reg.REG_OAM_A4);
      /*p34.SORO*/ wire _SPRITE_PAL_PIPE_SET1n = nand(_SPRITE_MASK1n, oam_reg.REG_OAM_A4);
      /*p34.PAMO*/ wire _SPRITE_PAL_PIPE_SET2n = nand(_SPRITE_MASK2n, oam_reg.REG_OAM_A4);
      /*p34.SUKY*/ wire _SPRITE_PAL_PIPE_SET3n = nand(_SPRITE_MASK3n, oam_reg.REG_OAM_A4);
      /*p34.RORA*/ wire _SPRITE_PAL_PIPE_SET4n = nand(_SPRITE_MASK4n, oam_reg.REG_OAM_A4);
      /*p34.MENE*/ wire _SPRITE_PAL_PIPE_SET5n = nand(_SPRITE_MASK5n, oam_reg.REG_OAM_A4);
      /*p34.LUKE*/ wire _SPRITE_PAL_PIPE_SET6n = nand(_SPRITE_MASK6n, oam_reg.REG_OAM_A4);
      /*p34.LAMY*/ wire _SPRITE_PAL_PIPE_SET7n = nand(_SPRITE_MASK7n, oam_reg.REG_OAM_A4);

      /*p34.SUCO*/ wire _SPRITE_PAL_PIPE_RST0n = nand(_SPRITE_MASK0n, /*p34.SYPY*/ not(oam_reg.REG_OAM_A4));
      /*p34.TAFA*/ wire _SPRITE_PAL_PIPE_RST1n = nand(_SPRITE_MASK1n, /*p34.TOTU*/ not(oam_reg.REG_OAM_A4));
      /*p34.PYZY*/ wire _SPRITE_PAL_PIPE_RST2n = nand(_SPRITE_MASK2n, /*p34.NARO*/ not(oam_reg.REG_OAM_A4));
      /*p34.TOWA*/ wire _SPRITE_PAL_PIPE_RST3n = nand(_SPRITE_MASK3n, /*p34.WEXY*/ not(oam_reg.REG_OAM_A4));
      /*p34.RUDU*/ wire _SPRITE_PAL_PIPE_RST4n = nand(_SPRITE_MASK4n, /*p34.RYZY*/ not(oam_reg.REG_OAM_A4));
      /*p34.PAZO*/ wire _SPRITE_PAL_PIPE_RST5n = nand(_SPRITE_MASK5n, /*p34.RYFE*/ not(oam_reg.REG_OAM_A4));
      /*p34.LOWA*/ wire _SPRITE_PAL_PIPE_RST6n = nand(_SPRITE_MASK6n, /*p34.LADY*/ not(oam_reg.REG_OAM_A4));
      /*p34.LUNU*/ wire _SPRITE_PAL_PIPE_RST7n = nand(_SPRITE_MASK7n, /*p34.LAFY*/ not(oam_reg.REG_OAM_A4));

      /*p34.RUGO*/ pxp_reg.PAL_PIPE_0.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _SPRITE_PAL_PIPE_SET0n, _SPRITE_PAL_PIPE_RST0n, joy_pin.P10_B);
      /*p34.SATA*/ pxp_reg.PAL_PIPE_1.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _SPRITE_PAL_PIPE_SET1n, _SPRITE_PAL_PIPE_RST1n, pxp_reg.PAL_PIPE_0);
      /*p34.ROSA*/ pxp_reg.PAL_PIPE_2.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _SPRITE_PAL_PIPE_SET2n, _SPRITE_PAL_PIPE_RST2n, pxp_reg.PAL_PIPE_1);
      /*p34.SOMY*/ pxp_reg.PAL_PIPE_3.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _SPRITE_PAL_PIPE_SET3n, _SPRITE_PAL_PIPE_RST3n, pxp_reg.PAL_PIPE_2);
      /*p34.PALU*/ pxp_reg.PAL_PIPE_4.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _SPRITE_PAL_PIPE_SET4n, _SPRITE_PAL_PIPE_RST4n, pxp_reg.PAL_PIPE_3);
      /*p34.NUKE*/ pxp_reg.PAL_PIPE_5.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _SPRITE_PAL_PIPE_SET5n, _SPRITE_PAL_PIPE_RST5n, pxp_reg.PAL_PIPE_4);
      /*p34.MODA*/ pxp_reg.PAL_PIPE_6.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _SPRITE_PAL_PIPE_SET6n, _SPRITE_PAL_PIPE_RST6n, pxp_reg.PAL_PIPE_5);
      /*p34.LYME*/ pxp_reg.PAL_PIPE_7.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _SPRITE_PAL_PIPE_SET7n, _SPRITE_PAL_PIPE_RST7n, pxp_reg.PAL_PIPE_6);
    }

    // Background mask pipe
    {
      /*p26.TEDE*/ wire _MASK_PIPE_SET0 = nand(_SPRITE_MASK0n, oam_reg.REG_OAM_A7);
      /*p26.XALA*/ wire _MASK_PIPE_SET1 = nand(_SPRITE_MASK1n, oam_reg.REG_OAM_A7);
      /*p26.TYRA*/ wire _MASK_PIPE_SET2 = nand(_SPRITE_MASK2n, oam_reg.REG_OAM_A7);
      /*p26.XYRU*/ wire _MASK_PIPE_SET3 = nand(_SPRITE_MASK3n, oam_reg.REG_OAM_A7);
      /*p26.XUKU*/ wire _MASK_PIPE_SET4 = nand(_SPRITE_MASK4n, oam_reg.REG_OAM_A7);
      /*p26.XELY*/ wire _MASK_PIPE_SET5 = nand(_SPRITE_MASK5n, oam_reg.REG_OAM_A7);
      /*p26.TYKO*/ wire _MASK_PIPE_SET6 = nand(_SPRITE_MASK6n, oam_reg.REG_OAM_A7);
      /*p26.TUWU*/ wire _MASK_PIPE_SET7 = nand(_SPRITE_MASK7n, oam_reg.REG_OAM_A7);

      /*p26.WOKA*/ wire _MASK_PIPE_RST0 = nand(_SPRITE_MASK0n, /*p26.XOGA*/ not(oam_reg.REG_OAM_A7));
      /*p26.WEDE*/ wire _MASK_PIPE_RST1 = nand(_SPRITE_MASK1n, /*p26.XURA*/ not(oam_reg.REG_OAM_A7));
      /*p26.TUFO*/ wire _MASK_PIPE_RST2 = nand(_SPRITE_MASK2n, /*p26.TAJO*/ not(oam_reg.REG_OAM_A7));
      /*p26.WEVO*/ wire _MASK_PIPE_RST3 = nand(_SPRITE_MASK3n, /*p26.XENU*/ not(oam_reg.REG_OAM_A7));
      /*p26.WEDY*/ wire _MASK_PIPE_RST4 = nand(_SPRITE_MASK4n, /*p26.XYKE*/ not(oam_reg.REG_OAM_A7));
      /*p26.WUJA*/ wire _MASK_PIPE_RST5 = nand(_SPRITE_MASK5n, /*p26.XABA*/ not(oam_reg.REG_OAM_A7));
      /*p26.TENA*/ wire _MASK_PIPE_RST6 = nand(_SPRITE_MASK6n, /*p26.TAFU*/ not(oam_reg.REG_OAM_A7));
      /*p26.WUBU*/ wire _MASK_PIPE_RST7 = nand(_SPRITE_MASK7n, /*p26.XUHO*/ not(oam_reg.REG_OAM_A7));

      /*p26.VEZO*/ pxp_reg.MASK_PIPE_0.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _MASK_PIPE_SET0, _MASK_PIPE_RST0, joy_pin.P10_B);
      /*p26.WURU*/ pxp_reg.MASK_PIPE_1.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _MASK_PIPE_SET1, _MASK_PIPE_RST1, pxp_reg.MASK_PIPE_0);
      /*p26.VOSA*/ pxp_reg.MASK_PIPE_2.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _MASK_PIPE_SET2, _MASK_PIPE_RST2, pxp_reg.MASK_PIPE_1);
      /*p26.WYFU*/ pxp_reg.MASK_PIPE_3.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _MASK_PIPE_SET3, _MASK_PIPE_RST3, pxp_reg.MASK_PIPE_2);
      /*p26.XETE*/ pxp_reg.MASK_PIPE_4.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _MASK_PIPE_SET4, _MASK_PIPE_RST4, pxp_reg.MASK_PIPE_3);
      /*p26.WODA*/ pxp_reg.MASK_PIPE_5.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _MASK_PIPE_SET5, _MASK_PIPE_RST5, pxp_reg.MASK_PIPE_4);
      /*p26.VUMO*/ pxp_reg.MASK_PIPE_6.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _MASK_PIPE_SET6, _MASK_PIPE_RST6, pxp_reg.MASK_PIPE_5);
      /*p26.VAVA*/ pxp_reg.MASK_PIPE_7.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, _MASK_PIPE_SET7, _MASK_PIPE_RST7, pxp_reg.MASK_PIPE_6);
    }
  }

  //----------
  // Pixel merge, this is _all_ combi logic. :/
  
  {
    /*p35.RAJY*/ wire _PIX_BG0  = and(cfg_reg.LCDC_BGEN, pxp_reg.BG_PIPE_A7);
    /*p35.TADE*/ wire _PIX_BG1  = and(cfg_reg.LCDC_BGEN, pxp_reg.BG_PIPE_B7);

    /*p35.WOXA*/ wire _PIX_SP0  = and(cfg_reg.LCDC_SPEN, pxp_reg.SPR_PIPE_A7);
    /*p35.XULA*/ wire _PIX_SP1  = and(cfg_reg.LCDC_SPEN, pxp_reg.SPR_PIPE_B7);

    /*p35.NULY*/ wire _PIX_SPn  = nor(_PIX_SP0, _PIX_SP1);

    /*p35.RYFU*/ wire _MASK_BG0 = and(_PIX_BG0, pxp_reg.MASK_PIPE_7);
    /*p35.RUTA*/ wire _MASK_BG1 = and(_PIX_BG1, pxp_reg.MASK_PIPE_7);
    /*p35.POKA*/ wire _BGPIXELn = nor(_PIX_SPn, _MASK_BG0, _MASK_BG1);

    /*p34.LOME*/ wire _SPRITE_PAL_PIPE_7n = not(pxp_reg.PAL_PIPE_7);
    /*p34.LAFU*/ wire _OBP0PIXELn = nand(_SPRITE_PAL_PIPE_7n, _BGPIXELn);
    /*p34.LEKA*/ wire _OBP1PIXELn = nand(_SPRITE_PAL_PIPE_7n, _BGPIXELn);

    //----------
    // Sprite palette 0 lookup

    /*p35.VUMU*/ wire _PIX_SP0n = not(_PIX_SP0);
    /*p35.WYRU*/ wire _PIX_SP0a = not(_PIX_SP0n);
    /*p35.WELE*/ wire _PIX_SP1n = not(_PIX_SP1);
    /*p35.WOLO*/ wire _PIX_SP1a = not(_PIX_SP1n);

    /*p35.LAVA*/ wire _MASK_OPB0  = not(_OBP0PIXELn);
    /*p35.VYRO*/ wire _PAL_OBP0D = and(_PIX_SP0a, _PIX_SP1a, _MASK_OPB0);
    /*p35.VATA*/ wire _PAL_OBP0C = and(_PIX_SP0a, _PIX_SP1n, _MASK_OPB0);
    /*p35.VOLO*/ wire _PAL_OBP0B = and(_PIX_SP0n, _PIX_SP1a, _MASK_OPB0);
    /*p35.VUGO*/ wire _PAL_OBP0A = and(_PIX_SP0n, _PIX_SP1n, _MASK_OPB0);
    

    /*p35.WUFU*/ wire _COL_OBP00 = amux4(cfg_reg.OBP07, _PAL_OBP0D,
                                         cfg_reg.OBP05, _PAL_OBP0C,
                                         cfg_reg.OBP03, _PAL_OBP0B,
                                         cfg_reg.OBP01, _PAL_OBP0A);

    /*p35.WALY*/ wire _COL_OBP01 = amux4(cfg_reg.OBP06, _PAL_OBP0D,
                                         cfg_reg.OBP04, _PAL_OBP0C,
                                         cfg_reg.OBP02, _PAL_OBP0B,
                                         cfg_reg.OBP00, _PAL_OBP0A);

    //----------
    // Sprite palette 1 lookup

    /*p35.MEXA*/ wire _PIX_SP0o = not(_PIX_SP0);
    /*p35.LOZO*/ wire _PIX_SP0b = not(_PIX_SP0o);
    /*p35.MABY*/ wire _PIX_SP1o = not(_PIX_SP1);
    /*p35.LYLE*/ wire _PIX_SP1b = not(_PIX_SP1o);

    /*p35.LUKU*/ wire _MASK_OBP1  = not(_OBP1PIXELn);
    /*p35.LEDO*/ wire _PAL_OBP1D = and(_PIX_SP1b, _PIX_SP0b, _MASK_OBP1);
    /*p35.LYKY*/ wire _PAL_OBP1C = and(_PIX_SP1b, _PIX_SP0o, _MASK_OBP1);
    /*p35.LARU*/ wire _PAL_OBP1B = and(_PIX_SP1o, _PIX_SP0b, _MASK_OBP1);
    /*p35.LOPU*/ wire _PAL_OBP1A = and(_PIX_SP1o, _PIX_SP0o, _MASK_OBP1);

    /*p35.MOKA*/ wire _COL_OBP10 = amux4(cfg_reg.OBP17, _PAL_OBP1D,
                                         cfg_reg.OBP15, _PAL_OBP1C,
                                         cfg_reg.OBP13, _PAL_OBP1B,
                                         cfg_reg.OBP11, _PAL_OBP1A);

    /*p35.MUFA*/ wire _COL_OBP11 = amux4(cfg_reg.OBP16, _PAL_OBP1D,
                                         cfg_reg.OBP14, _PAL_OBP1C,
                                         cfg_reg.OBP12, _PAL_OBP1B,
                                         cfg_reg.OBP10, _PAL_OBP1A);

    //----------
    // Background/window palette lookup

    /*p35.SOBA*/ wire _PIX_BG0n = not(_PIX_BG0);
    /*p35.VYCO*/ wire _PIX_BG1n = not(_PIX_BG1);
    /*p35.NUPO*/ wire _PIX_BG0a = not(_PIX_BG0n);
    /*p35.NALE*/ wire _PIX_BG1a = not(_PIX_BG1n);

    /*p35.MUVE*/ wire _MASK_BGP = not(_BGPIXELn);
    /*p35.POBU*/ wire _PAL_BGPA = and(_PIX_BG1n, _PIX_BG0n, _MASK_BGP);
    /*p35.NUMA*/ wire _PAL_BGPB = and(_PIX_BG1a, _PIX_BG0n, _MASK_BGP);
    /*p35.NUXO*/ wire _PAL_BGPC = and(_PIX_BG1n, _PIX_BG0a, _MASK_BGP);
    /*p35.NYPO*/ wire _PAL_BGPD = and(_PIX_BG1a, _PIX_BG0a, _MASK_BGP);

    /*p35.NURA*/ wire _COL_BGP1 = amux4(cfg_reg.BGP7, _PAL_BGPD,
                                         cfg_reg.BGP5, _PAL_BGPC,
                                         cfg_reg.BGP3, _PAL_BGPB,
                                         cfg_reg.BGP1, _PAL_BGPA);

    /*p35.NELO*/ wire _COL_BGP0 = amux4(cfg_reg.BGP6, _PAL_BGPD,
                                         cfg_reg.BGP4, _PAL_BGPC,
                                         cfg_reg.BGP2, _PAL_BGPB,
                                         cfg_reg.BGP0, _PAL_BGPA);

    //----------
    // Pixel merge and send

    // bits 0 and 1 swapped somewhere...

    /*p35.PATY*/ PATY_PIX_OUT_LO = or(_COL_BGP1, _COL_OBP00, _COL_OBP10);
    /*p35.PERO*/ PERO_PIX_OUT_HI = or(_COL_BGP0, _COL_OBP01, _COL_OBP11);
  }

}