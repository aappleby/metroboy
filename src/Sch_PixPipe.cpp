#include "Sch_PixPipe.h"

#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void PixelPipe::tick(SchematicTop& /*top*/) {
}

//------------------------------------------------------------------------------

void PixelPipe::tock(SchematicTop& top) {

  //----------------------------------------

  {
    /*p35.RAJY*/ wire _PIX_BG0  = and(top.VYXE_LCDC_BGEN.q(), BG_PIPE_A7.q());
    /*p35.TADE*/ wire _PIX_BG1  = and(top.VYXE_LCDC_BGEN.q(), BG_PIPE_B7.q());

    /*p35.WOXA*/ wire _PIX_SP0  = and(top.XYLO_LCDC_SPEN.q(), SPR_PIPE_A7.q());
    /*p35.XULA*/ wire _PIX_SP1  = and(top.XYLO_LCDC_SPEN.q(), SPR_PIPE_B7.q());

    /*p35.NULY*/ wire _PIX_SPn  = nor(_PIX_SP0, _PIX_SP1);

    /*p35.RYFU*/ wire _MASK_BG0 = and(_PIX_BG0, MASK_PIPE_7.q());
    /*p35.RUTA*/ wire _MASK_BG1 = and(_PIX_BG1, MASK_PIPE_7.q());
    /*p35.POKA*/ wire _BGPIXELn = nor(_PIX_SPn, _MASK_BG0, _MASK_BG1);

    /*p34.LOME*/ wire _SPRITE_PAL_PIPE_7n = not(PAL_PIPE_7.q());
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

    /*p35.WUFU*/ wire _COL_OBP00 = amux4(OBP07.q(), _PAL_OBP0D, OBP05.q(), _PAL_OBP0C, OBP03.q(), _PAL_OBP0B, OBP01.q(), _PAL_OBP0A);
    /*p35.WALY*/ wire _COL_OBP01 = amux4(OBP06.q(), _PAL_OBP0D, OBP04.q(), _PAL_OBP0C, OBP02.q(), _PAL_OBP0B, OBP00.q(), _PAL_OBP0A);

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

    /*p35.MOKA*/ wire _COL_OBP10 = amux4(OBP17.q(), _PAL_OBP1D, OBP15.q(), _PAL_OBP1C, OBP13.q(), _PAL_OBP1B, OBP11.q(), _PAL_OBP1A);
    /*p35.MUFA*/ wire _COL_OBP11 = amux4(OBP16.q(), _PAL_OBP1D, OBP14.q(), _PAL_OBP1C, OBP12.q(), _PAL_OBP1B, OBP10.q(), _PAL_OBP1A);

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

    /*p35.NURA*/ wire _COL_BGP1 = amux4(MENA_BGP7.q(), _PAL_BGPD, MORU_BGP5.q(), _PAL_BGPC, MAXY_BGP3.q(), _PAL_BGPB, NUSY_BGP1.q(), _PAL_BGPA);
    /*p35.NELO*/ wire _COL_BGP0 = amux4(MOGY_BGP6.q(), _PAL_BGPD, MUKE_BGP4.q(), _PAL_BGPC, PYLU_BGP2.q(), _PAL_BGPB, PAVO_BGP0.q(), _PAL_BGPA);

    //----------
    // Pixel merge and send

    // bits 0 and 1 swapped somewhere...

    /*p35.PATY*/ wire _PATY_PIX_OUT_LO = or(_COL_BGP1, _COL_OBP00, _COL_OBP10);
    /*p35.PERO*/ wire _PERO_PIX_OUT_HI = or(_COL_BGP0, _COL_OBP01, _COL_OBP11);

    /*p35.REMY*/ wire _REMY_LD0n = not(_PATY_PIX_OUT_LO);
    /*p35.RAVO*/ wire _RAVO_LD1n = not(_PERO_PIX_OUT_HI);
    top.LCD_PIN_LD0.set(not(_REMY_LD0n));
    top.LCD_PIN_LD1.set(not(_RAVO_LD1n));
  }

  {
    /*p32.LOZE*/ wire _LOZE_PIPE_A_LOAD = not(top.NYXU_TILE_FETCHER_RSTn());

    /*p32.LUHE*/ wire BG_PIX_A0n = not(top.tile_fetcher.LEGU_BG_PIX_A0.q());
    /*p32.NOLY*/ wire BG_PIX_A1n = not(top.tile_fetcher.NUDU_BG_PIX_A1.q());
    /*p32.LEKE*/ wire BG_PIX_A2n = not(top.tile_fetcher.MUKU_BG_PIX_A2.q());
    /*p32.LOMY*/ wire BG_PIX_A3n = not(top.tile_fetcher.LUZO_BG_PIX_A3.q());
    /*p32.LALA*/ wire BG_PIX_A4n = not(top.tile_fetcher.MEGU_BG_PIX_A4.q());
    /*p32.LOXA*/ wire BG_PIX_A5n = not(top.tile_fetcher.MYJY_BG_PIX_A5.q());
    /*p32.NEZE*/ wire BG_PIX_A6n = not(top.tile_fetcher.NASA_BG_PIX_A6.q());
    /*p32.NOBO*/ wire BG_PIX_A7n = not(top.tile_fetcher.NEFO_BG_PIX_A7.q());

    /*p32.LAKY*/ wire BG_PIPE_A_SET0 = nand(_LOZE_PIPE_A_LOAD, top.tile_fetcher.LEGU_BG_PIX_A0.q());
    /*p32.NYXO*/ wire BG_PIPE_A_SET1 = nand(_LOZE_PIPE_A_LOAD, top.tile_fetcher.NUDU_BG_PIX_A1.q());
    /*p32.LOTO*/ wire BG_PIPE_A_SET2 = nand(_LOZE_PIPE_A_LOAD, top.tile_fetcher.MUKU_BG_PIX_A2.q());
    /*p32.LYDU*/ wire BG_PIPE_A_SET3 = nand(_LOZE_PIPE_A_LOAD, top.tile_fetcher.LUZO_BG_PIX_A3.q());
    /*p32.MYVY*/ wire BG_PIPE_A_SET4 = nand(_LOZE_PIPE_A_LOAD, top.tile_fetcher.MEGU_BG_PIX_A4.q());
    /*p32.LODO*/ wire BG_PIPE_A_SET5 = nand(_LOZE_PIPE_A_LOAD, top.tile_fetcher.MYJY_BG_PIX_A5.q());
    /*p32.NUTE*/ wire BG_PIPE_A_SET6 = nand(_LOZE_PIPE_A_LOAD, top.tile_fetcher.NASA_BG_PIX_A6.q());
    /*p32.NAJA*/ wire BG_PIPE_A_SET7 = nand(_LOZE_PIPE_A_LOAD, top.tile_fetcher.NEFO_BG_PIX_A7.q());

    /*p32.LOTY*/ wire BG_PIPE_A_RST0 = nand(_LOZE_PIPE_A_LOAD, BG_PIX_A0n);
    /*p32.NEXA*/ wire BG_PIPE_A_RST1 = nand(_LOZE_PIPE_A_LOAD, BG_PIX_A1n);
    /*p32.LUTU*/ wire BG_PIPE_A_RST2 = nand(_LOZE_PIPE_A_LOAD, BG_PIX_A2n);
    /*p32.LUJA*/ wire BG_PIPE_A_RST3 = nand(_LOZE_PIPE_A_LOAD, BG_PIX_A3n);
    /*p32.MOSY*/ wire BG_PIPE_A_RST4 = nand(_LOZE_PIPE_A_LOAD, BG_PIX_A4n);
    /*p32.LERU*/ wire BG_PIPE_A_RST5 = nand(_LOZE_PIPE_A_LOAD, BG_PIX_A5n);
    /*p32.NYHA*/ wire BG_PIPE_A_RST6 = nand(_LOZE_PIPE_A_LOAD, BG_PIX_A6n);
    /*p32.NADY*/ wire BG_PIPE_A_RST7 = nand(_LOZE_PIPE_A_LOAD, BG_PIX_A7n);

    /*p32.MYDE*/ BG_PIPE_A0.set(top.SACU_CLKPIPEp(), BG_PIPE_A_SET0, BG_PIPE_A_RST0, top.GND);
    /*p32.NOZO*/ BG_PIPE_A1.set(top.SACU_CLKPIPEp(), BG_PIPE_A_SET1, BG_PIPE_A_RST1, BG_PIPE_A0.q());
    /*p32.MOJU*/ BG_PIPE_A2.set(top.SACU_CLKPIPEp(), BG_PIPE_A_SET2, BG_PIPE_A_RST2, BG_PIPE_A1.q());
    /*p32.MACU*/ BG_PIPE_A3.set(top.SACU_CLKPIPEp(), BG_PIPE_A_SET3, BG_PIPE_A_RST3, BG_PIPE_A2.q());
    /*p32.NEPO*/ BG_PIPE_A4.set(top.SACU_CLKPIPEp(), BG_PIPE_A_SET4, BG_PIPE_A_RST4, BG_PIPE_A3.q());
    /*p32.MODU*/ BG_PIPE_A5.set(top.SACU_CLKPIPEp(), BG_PIPE_A_SET5, BG_PIPE_A_RST5, BG_PIPE_A4.q());
    /*p32.NEDA*/ BG_PIPE_A6.set(top.SACU_CLKPIPEp(), BG_PIPE_A_SET6, BG_PIPE_A_RST6, BG_PIPE_A5.q());
    /*p32.PYBO*/ BG_PIPE_A7.set(top.SACU_CLKPIPEp(), BG_PIPE_A_SET7, BG_PIPE_A_RST7, BG_PIPE_A6.q());
  }

  {
    /*p32.LUXA*/ wire _LUXA_PIPE_B_LOAD = not(top.NYXU_TILE_FETCHER_RSTn());

    /*p32.TOSA*/ wire BG_PIX_B0n = not(top.tile_fetcher.RAWU_BG_PIX_B0.q());
    /*p32.RUCO*/ wire BG_PIX_B1n = not(top.tile_fetcher.POZO_BG_PIX_B1.q());
    /*p32.TYCE*/ wire BG_PIX_B2n = not(top.tile_fetcher.PYZO_BG_PIX_B2.q());
    /*p32.REVY*/ wire BG_PIX_B3n = not(top.tile_fetcher.POXA_BG_PIX_B3.q());
    /*p32.RYGA*/ wire BG_PIX_B4n = not(top.tile_fetcher.PULO_BG_PIX_B4.q());
    /*p32.RYLE*/ wire BG_PIX_B5n = not(top.tile_fetcher.POJU_BG_PIX_B5.q());
    /*p32.RAPU*/ wire BG_PIX_B6n = not(top.tile_fetcher.POWY_BG_PIX_B6.q());
    /*p32.SOJA*/ wire BG_PIX_B7n = not(top.tile_fetcher.PYJU_BG_PIX_B7.q());

    /*p32.TUXE*/ wire BG_PIPE_B_SET0 = nand(_LUXA_PIPE_B_LOAD, top.tile_fetcher.RAWU_BG_PIX_B0.q());
    /*p32.SOLY*/ wire BG_PIPE_B_SET1 = nand(_LUXA_PIPE_B_LOAD, top.tile_fetcher.POZO_BG_PIX_B1.q());
    /*p32.RUCE*/ wire BG_PIPE_B_SET2 = nand(_LUXA_PIPE_B_LOAD, top.tile_fetcher.PYZO_BG_PIX_B2.q());
    /*p32.RYJA*/ wire BG_PIPE_B_SET3 = nand(_LUXA_PIPE_B_LOAD, top.tile_fetcher.POXA_BG_PIX_B3.q());
    /*p32.RUTO*/ wire BG_PIPE_B_SET4 = nand(_LUXA_PIPE_B_LOAD, top.tile_fetcher.PULO_BG_PIX_B4.q());
    /*p32.RAJA*/ wire BG_PIPE_B_SET5 = nand(_LUXA_PIPE_B_LOAD, top.tile_fetcher.POJU_BG_PIX_B5.q());
    /*p32.RAJO*/ wire BG_PIPE_B_SET6 = nand(_LUXA_PIPE_B_LOAD, top.tile_fetcher.POWY_BG_PIX_B6.q());
    /*p32.RAGA*/ wire BG_PIPE_B_SET7 = nand(_LUXA_PIPE_B_LOAD, top.tile_fetcher.PYJU_BG_PIX_B7.q());

    /*p32.SEJA*/ wire BG_PIPE_B_RST0 = nand(_LUXA_PIPE_B_LOAD, BG_PIX_B0n);
    /*p32.SENO*/ wire BG_PIPE_B_RST1 = nand(_LUXA_PIPE_B_LOAD, BG_PIX_B1n);
    /*p32.SURE*/ wire BG_PIPE_B_RST2 = nand(_LUXA_PIPE_B_LOAD, BG_PIX_B2n);
    /*p32.SEBO*/ wire BG_PIPE_B_RST3 = nand(_LUXA_PIPE_B_LOAD, BG_PIX_B3n);
    /*p32.SUCA*/ wire BG_PIPE_B_RST4 = nand(_LUXA_PIPE_B_LOAD, BG_PIX_B4n);
    /*p32.SYWE*/ wire BG_PIPE_B_RST5 = nand(_LUXA_PIPE_B_LOAD, BG_PIX_B5n);
    /*p32.SUPU*/ wire BG_PIPE_B_RST6 = nand(_LUXA_PIPE_B_LOAD, BG_PIX_B6n);
    /*p32.RYJY*/ wire BG_PIPE_B_RST7 = nand(_LUXA_PIPE_B_LOAD, BG_PIX_B7n);

    /*p32.TOMY*/ BG_PIPE_B0.set(top.SACU_CLKPIPEp(), BG_PIPE_B_SET0, BG_PIPE_B_RST0, top.GND);
    /*p32.TACA*/ BG_PIPE_B1.set(top.SACU_CLKPIPEp(), BG_PIPE_B_SET1, BG_PIPE_B_RST1, BG_PIPE_B0.q());
    /*p32.SADY*/ BG_PIPE_B2.set(top.SACU_CLKPIPEp(), BG_PIPE_B_SET2, BG_PIPE_B_RST2, BG_PIPE_B1.q());
    /*p32.RYSA*/ BG_PIPE_B3.set(top.SACU_CLKPIPEp(), BG_PIPE_B_SET3, BG_PIPE_B_RST3, BG_PIPE_B2.q());
    /*p32.SOBO*/ BG_PIPE_B4.set(top.SACU_CLKPIPEp(), BG_PIPE_B_SET4, BG_PIPE_B_RST4, BG_PIPE_B3.q());
    /*p32.SETU*/ BG_PIPE_B5.set(top.SACU_CLKPIPEp(), BG_PIPE_B_SET5, BG_PIPE_B_RST5, BG_PIPE_B4.q());
    /*p32.RALU*/ BG_PIPE_B6.set(top.SACU_CLKPIPEp(), BG_PIPE_B_SET6, BG_PIPE_B_RST6, BG_PIPE_B5.q());
    /*p32.SOHU*/ BG_PIPE_B7.set(top.SACU_CLKPIPEp(), BG_PIPE_B_SET7, BG_PIPE_B_RST7, BG_PIPE_B6.q());
  }

  {
    /*p34.MEFU*/ wire _MEFU_SPRITE_MASK0p = or(top.sprite_fetcher.XEPY_SPRITE_DONEn(), SPR_PIPE_A0.q(), SPR_PIPE_B0.q()); // def or
    /*p34.MEVE*/ wire _MEVE_SPRITE_MASK1p = or(top.sprite_fetcher.XEPY_SPRITE_DONEn(), SPR_PIPE_A1.q(), SPR_PIPE_B1.q());
    /*p34.MYZO*/ wire _MYZO_SPRITE_MASK2p = or(top.sprite_fetcher.XEPY_SPRITE_DONEn(), SPR_PIPE_A2.q(), SPR_PIPE_B2.q());
    /*p34.RUDA*/ wire _RUDA_SPRITE_MASK3p = or(top.sprite_fetcher.XEPY_SPRITE_DONEn(), SPR_PIPE_A3.q(), SPR_PIPE_B3.q());
    /*p34.VOTO*/ wire _VOTO_SPRITE_MASK4p = or(top.sprite_fetcher.XEPY_SPRITE_DONEn(), SPR_PIPE_A4.q(), SPR_PIPE_B4.q());
    /*p34.VYSA*/ wire _VYSA_SPRITE_MASK5p = or(top.sprite_fetcher.XEPY_SPRITE_DONEn(), SPR_PIPE_A5.q(), SPR_PIPE_B5.q());
    /*p34.TORY*/ wire _TORY_SPRITE_MASK6p = or(top.sprite_fetcher.XEPY_SPRITE_DONEn(), SPR_PIPE_A6.q(), SPR_PIPE_B6.q());
    /*p34.WOPE*/ wire _WOPE_SPRITE_MASK7p = or(top.sprite_fetcher.XEPY_SPRITE_DONEn(), SPR_PIPE_A7.q(), SPR_PIPE_B7.q());

    /*p34.LESY*/ wire _LESY_SPRITE_MASK0n = not(_MEFU_SPRITE_MASK0p);
    /*p34.LOTA*/ wire _LOTA_SPRITE_MASK1n = not(_MEVE_SPRITE_MASK1p);
    /*p34.LYKU*/ wire _LYKU_SPRITE_MASK2n = not(_MYZO_SPRITE_MASK2p);
    /*p34.ROBY*/ wire _ROBY_SPRITE_MASK3n = not(_RUDA_SPRITE_MASK3p);
    /*p34.TYTA*/ wire _TYTA_SPRITE_MASK4n = not(_VOTO_SPRITE_MASK4p);
    /*p34.TYCO*/ wire _TYCO_SPRITE_MASK5n = not(_VYSA_SPRITE_MASK5p);
    /*p34.SOKA*/ wire _SOKA_SPRITE_MASK6n = not(_TORY_SPRITE_MASK6p);
    /*p34.XOVU*/ wire _XOVU_SPRITE_MASK7n = not(_WOPE_SPRITE_MASK7p);

    // Sprite pipe A
    {
#if 0
      if (!_XEPY_PIPE_LOAD_SPRITEn && !SPR_PIPE_A0 && !SPR_PIPE_B0) {
        /*p33.NYLU*/ SPR_PIPE_A0 = SPR_PIX_A0;
      }
      else {
        /*p33.NYLU*/ SPR_PIPE_A0.set(ppu_sig.SACU_CLKPIPEp, 1, 1, top.GND);
      }
#endif

      /*p33.LOZA*/ wire SPR_PIX_A0n = not(top.sprite_fetcher.SPR_PIX_A0.q());
      /*p33.SYBO*/ wire SPR_PIX_A1n = not(top.sprite_fetcher.SPR_PIX_A1.q());
      /*p33.LUMO*/ wire SPR_PIX_A2n = not(top.sprite_fetcher.SPR_PIX_A2.q());
      /*p33.SOLO*/ wire SPR_PIX_A3n = not(top.sprite_fetcher.SPR_PIX_A3.q());
      /*p33.VOBY*/ wire SPR_PIX_A4n = not(top.sprite_fetcher.SPR_PIX_A4.q());
      /*p33.WYCO*/ wire SPR_PIX_A5n = not(top.sprite_fetcher.SPR_PIX_A5.q());
      /*p33.SERY*/ wire SPR_PIX_A6n = not(top.sprite_fetcher.SPR_PIX_A6.q());
      /*p33.SELU*/ wire SPR_PIX_A7n = not(top.sprite_fetcher.SPR_PIX_A7.q());

      /*p33.MEZU*/ wire SPR_PIX_A_SET0 = nand(_LESY_SPRITE_MASK0n, top.sprite_fetcher.SPR_PIX_A0.q());
      /*p33.RUSY*/ wire SPR_PIX_A_SET1 = nand(_LOTA_SPRITE_MASK1n, top.sprite_fetcher.SPR_PIX_A1.q());
      /*p33.MYXA*/ wire SPR_PIX_A_SET2 = nand(_LYKU_SPRITE_MASK2n, top.sprite_fetcher.SPR_PIX_A2.q());
      /*p33.RANO*/ wire SPR_PIX_A_SET3 = nand(_ROBY_SPRITE_MASK3n, top.sprite_fetcher.SPR_PIX_A3.q());
      /*p33.TYGA*/ wire SPR_PIX_A_SET4 = nand(_TYTA_SPRITE_MASK4n, top.sprite_fetcher.SPR_PIX_A4.q());
      /*p33.VUME*/ wire SPR_PIX_A_SET5 = nand(_TYCO_SPRITE_MASK5n, top.sprite_fetcher.SPR_PIX_A5.q());
      /*p33.TAPO*/ wire SPR_PIX_A_SET6 = nand(_SOKA_SPRITE_MASK6n, top.sprite_fetcher.SPR_PIX_A6.q());
      /*p33.TESO*/ wire SPR_PIX_A_SET7 = nand(_XOVU_SPRITE_MASK7n, top.sprite_fetcher.SPR_PIX_A7.q());

      /*p33.MOFY*/ wire SPR_PIX_A_RST0 = nand(_LESY_SPRITE_MASK0n, SPR_PIX_A0n);
      /*p33.RUCA*/ wire SPR_PIX_A_RST1 = nand(_LOTA_SPRITE_MASK1n, SPR_PIX_A1n);
      /*p33.MAJO*/ wire SPR_PIX_A_RST2 = nand(_LYKU_SPRITE_MASK2n, SPR_PIX_A2n);
      /*p33.REHU*/ wire SPR_PIX_A_RST3 = nand(_ROBY_SPRITE_MASK3n, SPR_PIX_A3n);
      /*p33.WAXO*/ wire SPR_PIX_A_RST4 = nand(_TYTA_SPRITE_MASK4n, SPR_PIX_A4n);
      /*p33.XOLE*/ wire SPR_PIX_A_RST5 = nand(_TYCO_SPRITE_MASK5n, SPR_PIX_A5n);
      /*p33.TABY*/ wire SPR_PIX_A_RST6 = nand(_SOKA_SPRITE_MASK6n, SPR_PIX_A6n);
      /*p33.TULA*/ wire SPR_PIX_A_RST7 = nand(_XOVU_SPRITE_MASK7n, SPR_PIX_A7n);

      /*p33.NYLU*/ SPR_PIPE_A0.set(top.SACU_CLKPIPEp(), SPR_PIX_A_SET0, SPR_PIX_A_RST0, top.GND);
      /*p33.PEFU*/ SPR_PIPE_A1.set(top.SACU_CLKPIPEp(), SPR_PIX_A_SET1, SPR_PIX_A_RST1, SPR_PIPE_A0.q());
      /*p33.NATY*/ SPR_PIPE_A2.set(top.SACU_CLKPIPEp(), SPR_PIX_A_SET2, SPR_PIX_A_RST2, SPR_PIPE_A1.q());
      /*p33.PYJO*/ SPR_PIPE_A3.set(top.SACU_CLKPIPEp(), SPR_PIX_A_SET3, SPR_PIX_A_RST3, SPR_PIPE_A2.q());
      /*p33.VARE*/ SPR_PIPE_A4.set(top.SACU_CLKPIPEp(), SPR_PIX_A_SET4, SPR_PIX_A_RST4, SPR_PIPE_A3.q());
      /*p33.WEBA*/ SPR_PIPE_A5.set(top.SACU_CLKPIPEp(), SPR_PIX_A_SET5, SPR_PIX_A_RST5, SPR_PIPE_A4.q());
      /*p33.VANU*/ SPR_PIPE_A6.set(top.SACU_CLKPIPEp(), SPR_PIX_A_SET6, SPR_PIX_A_RST6, SPR_PIPE_A5.q());
      /*p33.VUPY*/ SPR_PIPE_A7.set(top.SACU_CLKPIPEp(), SPR_PIX_A_SET7, SPR_PIX_A_RST7, SPR_PIPE_A6.q());
    }

    // Sprite pipe B
    {
      /*p33.RATA*/ wire SPR_PIX_B0n = not(top.sprite_fetcher.SPR_PIX_B0.q());
      /*p33.NUCA*/ wire SPR_PIX_B1n = not(top.sprite_fetcher.SPR_PIX_B1.q());
      /*p33.LASE*/ wire SPR_PIX_B2n = not(top.sprite_fetcher.SPR_PIX_B2.q());
      /*p33.LUBO*/ wire SPR_PIX_B3n = not(top.sprite_fetcher.SPR_PIX_B3.q());
      /*p33.WERY*/ wire SPR_PIX_B4n = not(top.sprite_fetcher.SPR_PIX_B4.q());
      /*p33.WURA*/ wire SPR_PIX_B5n = not(top.sprite_fetcher.SPR_PIX_B5.q());
      /*p33.SULU*/ wire SPR_PIX_B6n = not(top.sprite_fetcher.SPR_PIX_B6.q());
      /*p33.WAMY*/ wire SPR_PIX_B7n = not(top.sprite_fetcher.SPR_PIX_B7.q());

      /*p33.PABE*/ wire SPR_PIX_B_SET0 = nand(_LESY_SPRITE_MASK0n, top.sprite_fetcher.SPR_PIX_B0.q());
      /*p33.MYTO*/ wire SPR_PIX_B_SET1 = nand(_LOTA_SPRITE_MASK1n, top.sprite_fetcher.SPR_PIX_B1.q());
      /*p33.LELA*/ wire SPR_PIX_B_SET2 = nand(_LYKU_SPRITE_MASK2n, top.sprite_fetcher.SPR_PIX_B2.q());
      /*p33.MAME*/ wire SPR_PIX_B_SET3 = nand(_ROBY_SPRITE_MASK3n, top.sprite_fetcher.SPR_PIX_B3.q());
      /*p33.VEXU*/ wire SPR_PIX_B_SET4 = nand(_TYTA_SPRITE_MASK4n, top.sprite_fetcher.SPR_PIX_B4.q());
      /*p33.VABY*/ wire SPR_PIX_B_SET5 = nand(_TYCO_SPRITE_MASK5n, top.sprite_fetcher.SPR_PIX_B5.q());
      /*p33.TUXA*/ wire SPR_PIX_B_SET6 = nand(_SOKA_SPRITE_MASK6n, top.sprite_fetcher.SPR_PIX_B6.q());
      /*p33.VUNE*/ wire SPR_PIX_B_SET7 = nand(_XOVU_SPRITE_MASK7n, top.sprite_fetcher.SPR_PIX_B7.q());

      /*p33.PYZU*/ wire SPR_PIX_B_RST0 = nand(_LESY_SPRITE_MASK0n, SPR_PIX_B0n);
      /*p33.MADA*/ wire SPR_PIX_B_RST1 = nand(_LOTA_SPRITE_MASK1n, SPR_PIX_B1n);
      /*p33.LYDE*/ wire SPR_PIX_B_RST2 = nand(_LYKU_SPRITE_MASK2n, SPR_PIX_B2n);
      /*p33.LUFY*/ wire SPR_PIX_B_RST3 = nand(_ROBY_SPRITE_MASK3n, SPR_PIX_B3n);
      /*p33.XATO*/ wire SPR_PIX_B_RST4 = nand(_TYTA_SPRITE_MASK4n, SPR_PIX_B4n);
      /*p33.XEXU*/ wire SPR_PIX_B_RST5 = nand(_TYCO_SPRITE_MASK5n, SPR_PIX_B5n);
      /*p33.TUPE*/ wire SPR_PIX_B_RST6 = nand(_SOKA_SPRITE_MASK6n, SPR_PIX_B6n);
      /*p33.XYVE*/ wire SPR_PIX_B_RST7 = nand(_XOVU_SPRITE_MASK7n, SPR_PIX_B7n);

      /*p33.NURO*/ SPR_PIPE_B0.set(top.SACU_CLKPIPEp(), SPR_PIX_B_SET0, SPR_PIX_B_RST0, top.GND);
      /*p33.MASO*/ SPR_PIPE_B1.set(top.SACU_CLKPIPEp(), SPR_PIX_B_SET1, SPR_PIX_B_RST1, SPR_PIPE_B0.q());
      /*p33.LEFE*/ SPR_PIPE_B2.set(top.SACU_CLKPIPEp(), SPR_PIX_B_SET2, SPR_PIX_B_RST2, SPR_PIPE_B1.q());
      /*p33.LESU*/ SPR_PIPE_B3.set(top.SACU_CLKPIPEp(), SPR_PIX_B_SET3, SPR_PIX_B_RST3, SPR_PIPE_B2.q());
      /*p33.WYHO*/ SPR_PIPE_B4.set(top.SACU_CLKPIPEp(), SPR_PIX_B_SET4, SPR_PIX_B_RST4, SPR_PIPE_B3.q());
      /*p33.WORA*/ SPR_PIPE_B5.set(top.SACU_CLKPIPEp(), SPR_PIX_B_SET5, SPR_PIX_B_RST5, SPR_PIPE_B4.q());
      /*p33.VAFO*/ SPR_PIPE_B6.set(top.SACU_CLKPIPEp(), SPR_PIX_B_SET6, SPR_PIX_B_RST6, SPR_PIPE_B5.q());
      /*p33.WUFY*/ SPR_PIPE_B7.set(top.SACU_CLKPIPEp(), SPR_PIX_B_SET7, SPR_PIX_B_RST7, SPR_PIPE_B6.q());
    }

    // Palette pipe
    {
      /*p34.SYPY*/ wire _SYPY = not(top.oam_bus.GOMO_SPRITE_X4.q());
      /*p34.TOTU*/ wire _TOTU = not(top.oam_bus.GOMO_SPRITE_X4.q());
      /*p34.NARO*/ wire _NARO = not(top.oam_bus.GOMO_SPRITE_X4.q());
      /*p34.WEXY*/ wire _WEXY = not(top.oam_bus.GOMO_SPRITE_X4.q());
      /*p34.RYZY*/ wire _RYZY = not(top.oam_bus.GOMO_SPRITE_X4.q());
      /*p34.RYFE*/ wire _RYFE = not(top.oam_bus.GOMO_SPRITE_X4.q());
      /*p34.LADY*/ wire _LADY = not(top.oam_bus.GOMO_SPRITE_X4.q());
      /*p34.LAFY*/ wire _LAFY = not(top.oam_bus.GOMO_SPRITE_X4.q());

      /*p34.PUME*/ wire _PUME_PAL_PIPE_SET0n = nand(_LESY_SPRITE_MASK0n, top.oam_bus.GOMO_SPRITE_X4.q());
      /*p34.SORO*/ wire _SORO_PAL_PIPE_SET1n = nand(_LOTA_SPRITE_MASK1n, top.oam_bus.GOMO_SPRITE_X4.q());
      /*p34.PAMO*/ wire _PAMO_PAL_PIPE_SET2n = nand(_LYKU_SPRITE_MASK2n, top.oam_bus.GOMO_SPRITE_X4.q());
      /*p34.SUKY*/ wire _SUKY_PAL_PIPE_SET3n = nand(_ROBY_SPRITE_MASK3n, top.oam_bus.GOMO_SPRITE_X4.q());
      /*p34.RORA*/ wire _RORA_PAL_PIPE_SET4n = nand(_TYTA_SPRITE_MASK4n, top.oam_bus.GOMO_SPRITE_X4.q());
      /*p34.MENE*/ wire _MENE_PAL_PIPE_SET5n = nand(_TYCO_SPRITE_MASK5n, top.oam_bus.GOMO_SPRITE_X4.q());
      /*p34.LUKE*/ wire _LUKE_PAL_PIPE_SET6n = nand(_SOKA_SPRITE_MASK6n, top.oam_bus.GOMO_SPRITE_X4.q());
      /*p34.LAMY*/ wire _LAMY_PAL_PIPE_SET7n = nand(_XOVU_SPRITE_MASK7n, top.oam_bus.GOMO_SPRITE_X4.q());

      /*p34.SUCO*/ wire _SUCO_PAL_PIPE_RST0n = nand(_LESY_SPRITE_MASK0n, _SYPY);
      /*p34.TAFA*/ wire _TAFA_PAL_PIPE_RST1n = nand(_LOTA_SPRITE_MASK1n, _TOTU);
      /*p34.PYZY*/ wire _PYZY_PAL_PIPE_RST2n = nand(_LYKU_SPRITE_MASK2n, _NARO);
      /*p34.TOWA*/ wire _TOWA_PAL_PIPE_RST3n = nand(_ROBY_SPRITE_MASK3n, _WEXY);
      /*p34.RUDU*/ wire _RUDU_PAL_PIPE_RST4n = nand(_TYTA_SPRITE_MASK4n, _RYZY);
      /*p34.PAZO*/ wire _PAZO_PAL_PIPE_RST5n = nand(_TYCO_SPRITE_MASK5n, _RYFE);
      /*p34.LOWA*/ wire _LOWA_PAL_PIPE_RST6n = nand(_SOKA_SPRITE_MASK6n, _LADY);
      /*p34.LUNU*/ wire _LUNU_PAL_PIPE_RST7n = nand(_XOVU_SPRITE_MASK7n, _LAFY);

      /*p34.RUGO*/ PAL_PIPE_0.set(top.SACU_CLKPIPEp(), _PUME_PAL_PIPE_SET0n, _SUCO_PAL_PIPE_RST0n, top.GND);
      /*p34.SATA*/ PAL_PIPE_1.set(top.SACU_CLKPIPEp(), _SORO_PAL_PIPE_SET1n, _TAFA_PAL_PIPE_RST1n, PAL_PIPE_0.q());
      /*p34.ROSA*/ PAL_PIPE_2.set(top.SACU_CLKPIPEp(), _PAMO_PAL_PIPE_SET2n, _PYZY_PAL_PIPE_RST2n, PAL_PIPE_1.q());
      /*p34.SOMY*/ PAL_PIPE_3.set(top.SACU_CLKPIPEp(), _SUKY_PAL_PIPE_SET3n, _TOWA_PAL_PIPE_RST3n, PAL_PIPE_2.q());
      /*p34.PALU*/ PAL_PIPE_4.set(top.SACU_CLKPIPEp(), _RORA_PAL_PIPE_SET4n, _RUDU_PAL_PIPE_RST4n, PAL_PIPE_3.q());
      /*p34.NUKE*/ PAL_PIPE_5.set(top.SACU_CLKPIPEp(), _MENE_PAL_PIPE_SET5n, _PAZO_PAL_PIPE_RST5n, PAL_PIPE_4.q());
      /*p34.MODA*/ PAL_PIPE_6.set(top.SACU_CLKPIPEp(), _LUKE_PAL_PIPE_SET6n, _LOWA_PAL_PIPE_RST6n, PAL_PIPE_5.q());
      /*p34.LYME*/ PAL_PIPE_7.set(top.SACU_CLKPIPEp(), _LAMY_PAL_PIPE_SET7n, _LUNU_PAL_PIPE_RST7n, PAL_PIPE_6.q());
    }

    // Background mask pipe
    {
      /*p26.XOGA*/ wire _XOGA = not(top.oam_bus.DEPO_SPRITE_X7.q());
      /*p26.XURA*/ wire _XURA = not(top.oam_bus.DEPO_SPRITE_X7.q());
      /*p26.TAJO*/ wire _TAJO = not(top.oam_bus.DEPO_SPRITE_X7.q());
      /*p26.XENU*/ wire _XENU = not(top.oam_bus.DEPO_SPRITE_X7.q());
      /*p26.XYKE*/ wire _XYKE = not(top.oam_bus.DEPO_SPRITE_X7.q());
      /*p26.XABA*/ wire _XABA = not(top.oam_bus.DEPO_SPRITE_X7.q());
      /*p26.TAFU*/ wire _TAFU = not(top.oam_bus.DEPO_SPRITE_X7.q());
      /*p26.XUHO*/ wire _XUHO = not(top.oam_bus.DEPO_SPRITE_X7.q());

      /*p26.TEDE*/ wire _TEDE_MASK_PIPE_SET0 = nand(_LESY_SPRITE_MASK0n, top.oam_bus.DEPO_SPRITE_X7.q());
      /*p26.XALA*/ wire _XALA_MASK_PIPE_SET1 = nand(_LOTA_SPRITE_MASK1n, top.oam_bus.DEPO_SPRITE_X7.q());
      /*p26.TYRA*/ wire _TYRA_MASK_PIPE_SET2 = nand(_LYKU_SPRITE_MASK2n, top.oam_bus.DEPO_SPRITE_X7.q());
      /*p26.XYRU*/ wire _XYRU_MASK_PIPE_SET3 = nand(_ROBY_SPRITE_MASK3n, top.oam_bus.DEPO_SPRITE_X7.q());
      /*p26.XUKU*/ wire _XUKU_MASK_PIPE_SET4 = nand(_TYTA_SPRITE_MASK4n, top.oam_bus.DEPO_SPRITE_X7.q());
      /*p26.XELY*/ wire _XELY_MASK_PIPE_SET5 = nand(_TYCO_SPRITE_MASK5n, top.oam_bus.DEPO_SPRITE_X7.q());
      /*p26.TYKO*/ wire _TYKO_MASK_PIPE_SET6 = nand(_SOKA_SPRITE_MASK6n, top.oam_bus.DEPO_SPRITE_X7.q());
      /*p26.TUWU*/ wire _TUWU_MASK_PIPE_SET7 = nand(_XOVU_SPRITE_MASK7n, top.oam_bus.DEPO_SPRITE_X7.q());

      /*p26.WOKA*/ wire _WOKA_MASK_PIPE_RST0 = nand(_LESY_SPRITE_MASK0n, _XOGA);
      /*p26.WEDE*/ wire _WEDE_MASK_PIPE_RST1 = nand(_LOTA_SPRITE_MASK1n, _XURA);
      /*p26.TUFO*/ wire _TUFO_MASK_PIPE_RST2 = nand(_LYKU_SPRITE_MASK2n, _TAJO);
      /*p26.WEVO*/ wire _WEVO_MASK_PIPE_RST3 = nand(_ROBY_SPRITE_MASK3n, _XENU);
      /*p26.WEDY*/ wire _WEDY_MASK_PIPE_RST4 = nand(_TYTA_SPRITE_MASK4n, _XYKE);
      /*p26.WUJA*/ wire _WUJA_MASK_PIPE_RST5 = nand(_TYCO_SPRITE_MASK5n, _XABA);
      /*p26.TENA*/ wire _TENA_MASK_PIPE_RST6 = nand(_SOKA_SPRITE_MASK6n, _TAFU);
      /*p26.WUBU*/ wire _WUBU_MASK_PIPE_RST7 = nand(_XOVU_SPRITE_MASK7n, _XUHO);

      /*p26.VEZO*/ MASK_PIPE_0.set(top.SACU_CLKPIPEp(), _TEDE_MASK_PIPE_SET0, _WOKA_MASK_PIPE_RST0, top.GND);
      /*p26.WURU*/ MASK_PIPE_1.set(top.SACU_CLKPIPEp(), _XALA_MASK_PIPE_SET1, _WEDE_MASK_PIPE_RST1, MASK_PIPE_0.q());
      /*p26.VOSA*/ MASK_PIPE_2.set(top.SACU_CLKPIPEp(), _TYRA_MASK_PIPE_SET2, _TUFO_MASK_PIPE_RST2, MASK_PIPE_1.q());
      /*p26.WYFU*/ MASK_PIPE_3.set(top.SACU_CLKPIPEp(), _XYRU_MASK_PIPE_SET3, _WEVO_MASK_PIPE_RST3, MASK_PIPE_2.q());
      /*p26.XETE*/ MASK_PIPE_4.set(top.SACU_CLKPIPEp(), _XUKU_MASK_PIPE_SET4, _WEDY_MASK_PIPE_RST4, MASK_PIPE_3.q());
      /*p26.WODA*/ MASK_PIPE_5.set(top.SACU_CLKPIPEp(), _XELY_MASK_PIPE_SET5, _WUJA_MASK_PIPE_RST5, MASK_PIPE_4.q());
      /*p26.VUMO*/ MASK_PIPE_6.set(top.SACU_CLKPIPEp(), _TYKO_MASK_PIPE_SET6, _TENA_MASK_PIPE_RST6, MASK_PIPE_5.q());
      /*p26.VAVA*/ MASK_PIPE_7.set(top.SACU_CLKPIPEp(), _TUWU_MASK_PIPE_SET7, _WUBU_MASK_PIPE_RST7, MASK_PIPE_6.q());
    }
  }

  //----------------------------------------

  // FF47 BGP
  {
    /*p22.WYBO*/ wire _WYBO_FF47n = nand(top.int_bus.WERO_FF4Xp(), top.int_bus.WADO_A00p(), top.int_bus.WESA_A01p(), top.int_bus.WALO_A02p(), top.int_bus.XERA_A03n());
    /*p22.WERA*/ wire _WERA_FF47 = not(_WYBO_FF47n);
    /*p36.VELY*/ wire _VELY_FF47_WR = and (top.CUPA_CPU_WRp_xxxxEFGx(), _WERA_FF47);
    /*p36.TEPO*/ wire _TEPO_FF47_WRn = not(_VELY_FF47_WR);

    /*p36.PAVO*/ PAVO_BGP0.set(_TEPO_FF47_WRn, !_TEPO_FF47_WRn, top.int_bus.INT_TRI_D0.q());
    /*p36.NUSY*/ NUSY_BGP1.set(_TEPO_FF47_WRn, !_TEPO_FF47_WRn, top.int_bus.INT_TRI_D1.q());
    /*p36.PYLU*/ PYLU_BGP2.set(_TEPO_FF47_WRn, !_TEPO_FF47_WRn, top.int_bus.INT_TRI_D2.q());
    /*p36.MAXY*/ MAXY_BGP3.set(_TEPO_FF47_WRn, !_TEPO_FF47_WRn, top.int_bus.INT_TRI_D3.q());
    /*p36.MUKE*/ MUKE_BGP4.set(_TEPO_FF47_WRn, !_TEPO_FF47_WRn, top.int_bus.INT_TRI_D4.q());
    /*p36.MORU*/ MORU_BGP5.set(_TEPO_FF47_WRn, !_TEPO_FF47_WRn, top.int_bus.INT_TRI_D5.q());
    /*p36.MOGY*/ MOGY_BGP6.set(_TEPO_FF47_WRn, !_TEPO_FF47_WRn, top.int_bus.INT_TRI_D6.q());
    /*p36.MENA*/ MENA_BGP7.set(_TEPO_FF47_WRn, !_TEPO_FF47_WRn, top.int_bus.INT_TRI_D7.q());

    /*p36.VUSO*/ wire _VUSO_FF47_RD = and (top.ASOT_CPU_RDp(), _WERA_FF47);
    /*p36.TEPY*/ wire _TEPY_FF47_RDn = not(_VUSO_FF47_RD);

    /*p36.RARO*/ top.int_bus.INT_TRI_D0.set_tribuf_6n(_TEPY_FF47_RDn, PAVO_BGP0.q());
    /*p36.PABA*/ top.int_bus.INT_TRI_D1.set_tribuf_6n(_TEPY_FF47_RDn, NUSY_BGP1.q());
    /*p36.REDO*/ top.int_bus.INT_TRI_D2.set_tribuf_6n(_TEPY_FF47_RDn, PYLU_BGP2.q());
    /*p36.LOBE*/ top.int_bus.INT_TRI_D3.set_tribuf_6n(_TEPY_FF47_RDn, MAXY_BGP3.q());
    /*p36.LACE*/ top.int_bus.INT_TRI_D4.set_tribuf_6n(_TEPY_FF47_RDn, MUKE_BGP4.q());
    /*p36.LYKA*/ top.int_bus.INT_TRI_D5.set_tribuf_6n(_TEPY_FF47_RDn, MORU_BGP5.q());
    /*p36.LODY*/ top.int_bus.INT_TRI_D6.set_tribuf_6n(_TEPY_FF47_RDn, MOGY_BGP6.q());
    /*p36.LARY*/ top.int_bus.INT_TRI_D7.set_tribuf_6n(_TEPY_FF47_RDn, MENA_BGP7.q());
  }

  // FF48 OBP0
  {
    /*p22.WETA*/ wire _WETA_FF48n = nand(top.int_bus.WERO_FF4Xp(), top.int_bus.XOLA_A00n(), top.int_bus.XENO_A01n(), top.int_bus.XUSY_A02n(), top.int_bus.WEPO_A03p());
    /*p22.XAYO*/ wire _XAYO_FF48 = not(_WETA_FF48n);
    /*p36.XOMA*/ wire _XOMA_FF48_WR = and (top.CUPA_CPU_WRp_xxxxEFGx(), _XAYO_FF48);
    /*p36.XELO*/ wire _XELO_FF48_WRn = not(_XOMA_FF48_WR);

    /*p36.XUFU*/ OBP00.set(_XELO_FF48_WRn, !_XELO_FF48_WRn, top.int_bus.INT_TRI_D0.q());
    /*p36.XUKY*/ OBP01.set(_XELO_FF48_WRn, !_XELO_FF48_WRn, top.int_bus.INT_TRI_D1.q());
    /*p36.XOVA*/ OBP02.set(_XELO_FF48_WRn, !_XELO_FF48_WRn, top.int_bus.INT_TRI_D2.q());
    /*p36.XALO*/ OBP03.set(_XELO_FF48_WRn, !_XELO_FF48_WRn, top.int_bus.INT_TRI_D3.q());
    /*p36.XERU*/ OBP04.set(_XELO_FF48_WRn, !_XELO_FF48_WRn, top.int_bus.INT_TRI_D4.q());
    /*p36.XYZE*/ OBP05.set(_XELO_FF48_WRn, !_XELO_FF48_WRn, top.int_bus.INT_TRI_D5.q());
    /*p36.XUPO*/ OBP06.set(_XELO_FF48_WRn, !_XELO_FF48_WRn, top.int_bus.INT_TRI_D6.q());
    /*p36.XANA*/ OBP07.set(_XELO_FF48_WRn, !_XELO_FF48_WRn, top.int_bus.INT_TRI_D7.q());

    /*p36.XUFY*/ wire _XUFY_FF48_RD = and (top.ASOT_CPU_RDp(), _XAYO_FF48);
    /*p36.XOZY*/ wire _XOZY_FF48_RDn = not(_XUFY_FF48_RD);

    /*p36.XARY*/ top.int_bus.INT_TRI_D0.set_tribuf_6n(_XOZY_FF48_RDn, OBP00.q());
    /*p36.XOKE*/ top.int_bus.INT_TRI_D1.set_tribuf_6n(_XOZY_FF48_RDn, OBP01.q());
    /*p36.XUNO*/ top.int_bus.INT_TRI_D2.set_tribuf_6n(_XOZY_FF48_RDn, OBP02.q());
    /*p36.XUBY*/ top.int_bus.INT_TRI_D3.set_tribuf_6n(_XOZY_FF48_RDn, OBP03.q());
    /*p36.XAJU*/ top.int_bus.INT_TRI_D4.set_tribuf_6n(_XOZY_FF48_RDn, OBP04.q());
    /*p36.XOBO*/ top.int_bus.INT_TRI_D5.set_tribuf_6n(_XOZY_FF48_RDn, OBP05.q());
    /*p36.XAXA*/ top.int_bus.INT_TRI_D6.set_tribuf_6n(_XOZY_FF48_RDn, OBP06.q());
    /*p36.XAWO*/ top.int_bus.INT_TRI_D7.set_tribuf_6n(_XOZY_FF48_RDn, OBP07.q());
  }

  // FF49 OBP1
  {
    /*p22.VAMA*/ wire _VAMA_FF49n = nand(top.int_bus.WERO_FF4Xp(), top.int_bus.WADO_A00p(), top.int_bus.XENO_A01n(), top.int_bus.XUSY_A02n(), top.int_bus.WEPO_A03p());
    /*p22.TEGO*/ wire _TEGO_FF49 = not(_VAMA_FF49n);
    /*p36.MYXE*/ wire _MYXE_FF49_WR = and (top.CUPA_CPU_WRp_xxxxEFGx(), _TEGO_FF49);
    /*p36.LEHO*/ wire _LEHO_FF49_WRn = not(_MYXE_FF49_WR);

    /*p36.MOXY*/ OBP10.set(_LEHO_FF49_WRn, !_LEHO_FF49_WRn, top.int_bus.INT_TRI_D0.q());
    /*p36.LAWO*/ OBP11.set(_LEHO_FF49_WRn, !_LEHO_FF49_WRn, top.int_bus.INT_TRI_D1.q());
    /*p36.MOSA*/ OBP12.set(_LEHO_FF49_WRn, !_LEHO_FF49_WRn, top.int_bus.INT_TRI_D2.q());
    /*p36.LOSE*/ OBP13.set(_LEHO_FF49_WRn, !_LEHO_FF49_WRn, top.int_bus.INT_TRI_D3.q());
    /*p36.LUNE*/ OBP14.set(_LEHO_FF49_WRn, !_LEHO_FF49_WRn, top.int_bus.INT_TRI_D4.q());
    /*p36.LUGU*/ OBP15.set(_LEHO_FF49_WRn, !_LEHO_FF49_WRn, top.int_bus.INT_TRI_D5.q());
    /*p36.LEPU*/ OBP16.set(_LEHO_FF49_WRn, !_LEHO_FF49_WRn, top.int_bus.INT_TRI_D6.q());
    /*p36.LUXO*/ OBP17.set(_LEHO_FF49_WRn, !_LEHO_FF49_WRn, top.int_bus.INT_TRI_D7.q());

    /*p36.MUMY*/ wire _MUMY_FF49_RD = and (top.ASOT_CPU_RDp(), _TEGO_FF49);
    /*p36.LOTE*/ wire _LOTE_FF49_RDn = not(_MUMY_FF49_RD);

    /*p36.LAJU*/ top.int_bus.INT_TRI_D0.set_tribuf_6n(_LOTE_FF49_RDn, OBP10.q());
    /*p36.LEPA*/ top.int_bus.INT_TRI_D1.set_tribuf_6n(_LOTE_FF49_RDn, OBP11.q());
    /*p36.LODE*/ top.int_bus.INT_TRI_D2.set_tribuf_6n(_LOTE_FF49_RDn, OBP12.q());
    /*p36.LYZA*/ top.int_bus.INT_TRI_D3.set_tribuf_6n(_LOTE_FF49_RDn, OBP13.q());
    /*p36.LUKY*/ top.int_bus.INT_TRI_D4.set_tribuf_6n(_LOTE_FF49_RDn, OBP14.q());
    /*p36.LUGA*/ top.int_bus.INT_TRI_D5.set_tribuf_6n(_LOTE_FF49_RDn, OBP15.q());
    /*p36.LEBA*/ top.int_bus.INT_TRI_D6.set_tribuf_6n(_LOTE_FF49_RDn, OBP16.q());
    /*p36.LELU*/ top.int_bus.INT_TRI_D7.set_tribuf_6n(_LOTE_FF49_RDn, OBP17.q());
  }
}

//------------------------------------------------------------------------------

SignalHash PixelPipe::commit(SchematicTop& top) {
  SignalHash hash;

  /*p32.MYDE*/ hash << BG_PIPE_A0.commit();
  /*p32.NOZO*/ hash << BG_PIPE_A1.commit();
  /*p32.MOJU*/ hash << BG_PIPE_A2.commit();
  /*p32.MACU*/ hash << BG_PIPE_A3.commit();
  /*p32.NEPO*/ hash << BG_PIPE_A4.commit();
  /*p32.MODU*/ hash << BG_PIPE_A5.commit();
  /*p32.NEDA*/ hash << BG_PIPE_A6.commit();
  /*p32.PYBO*/ hash << BG_PIPE_A7.commit();
  /*p32.TOMY*/ hash << BG_PIPE_B0.commit();
  /*p32.TACA*/ hash << BG_PIPE_B1.commit();
  /*p32.SADY*/ hash << BG_PIPE_B2.commit();
  /*p32.RYSA*/ hash << BG_PIPE_B3.commit();
  /*p32.SOBO*/ hash << BG_PIPE_B4.commit();
  /*p32.SETU*/ hash << BG_PIPE_B5.commit();
  /*p32.RALU*/ hash << BG_PIPE_B6.commit();
  /*p32.SOHU*/ hash << BG_PIPE_B7.commit();
  /*p33.NURO*/ hash << SPR_PIPE_B0.commit();
  /*p33.MASO*/ hash << SPR_PIPE_B1.commit();
  /*p33.LEFE*/ hash << SPR_PIPE_B2.commit();
  /*p33.LESU*/ hash << SPR_PIPE_B3.commit();
  /*p33.WYHO*/ hash << SPR_PIPE_B4.commit();
  /*p33.WORA*/ hash << SPR_PIPE_B5.commit();
  /*p33.VAFO*/ hash << SPR_PIPE_B6.commit();
  /*p33.WUFY*/ hash << SPR_PIPE_B7.commit();
  /*p33.NYLU*/ hash << SPR_PIPE_A0.commit();
  /*p33.PEFU*/ hash << SPR_PIPE_A1.commit();
  /*p33.NATY*/ hash << SPR_PIPE_A2.commit();
  /*p33.PYJO*/ hash << SPR_PIPE_A3.commit();
  /*p33.VARE*/ hash << SPR_PIPE_A4.commit();
  /*p33.WEBA*/ hash << SPR_PIPE_A5.commit();
  /*p33.VANU*/ hash << SPR_PIPE_A6.commit();
  /*p33.VUPY*/ hash << SPR_PIPE_A7.commit();
  /*p34.RUGO*/ hash << PAL_PIPE_0.commit();
  /*p34.SATA*/ hash << PAL_PIPE_1.commit();
  /*p34.ROSA*/ hash << PAL_PIPE_2.commit();
  /*p34.SOMY*/ hash << PAL_PIPE_3.commit();
  /*p34.PALU*/ hash << PAL_PIPE_4.commit();
  /*p34.NUKE*/ hash << PAL_PIPE_5.commit();
  /*p34.MODA*/ hash << PAL_PIPE_6.commit();
  /*p34.LYME*/ hash << PAL_PIPE_7.commit();
  /*p26.VEZO*/ hash << MASK_PIPE_0.commit();
  /*p26.WURU*/ hash << MASK_PIPE_1.commit();
  /*p26.VOSA*/ hash << MASK_PIPE_2.commit();
  /*p26.WYFU*/ hash << MASK_PIPE_3.commit();
  /*p26.XETE*/ hash << MASK_PIPE_4.commit();
  /*p26.WODA*/ hash << MASK_PIPE_5.commit();
  /*p26.VUMO*/ hash << MASK_PIPE_6.commit();
  /*p26.VAVA*/ hash << MASK_PIPE_7.commit();

  /*p36.PAVO*/ hash << PAVO_BGP0.commit();
  /*p36.NUSY*/ hash << NUSY_BGP1.commit();
  /*p36.PYLU*/ hash << PYLU_BGP2.commit();
  /*p36.MAXY*/ hash << MAXY_BGP3.commit();
  /*p36.MUKE*/ hash << MUKE_BGP4.commit();
  /*p36.MORU*/ hash << MORU_BGP5.commit();
  /*p36.MOGY*/ hash << MOGY_BGP6.commit();
  /*p36.MENA*/ hash << MENA_BGP7.commit();
  /*p36.XUFU*/ hash << OBP00.commit();
  /*p36.XUKY*/ hash << OBP01.commit();
  /*p36.XOVA*/ hash << OBP02.commit();
  /*p36.XALO*/ hash << OBP03.commit();
  /*p36.XERU*/ hash << OBP04.commit();
  /*p36.XYZE*/ hash << OBP05.commit();
  /*p36.XUPO*/ hash << OBP06.commit();
  /*p36.XANA*/ hash << OBP07.commit();
  /*p36.MOXY*/ hash << OBP10.commit();
  /*p36.LAWO*/ hash << OBP11.commit();
  /*p36.MOSA*/ hash << OBP12.commit();
  /*p36.LOSE*/ hash << OBP13.commit();
  /*p36.LUNE*/ hash << OBP14.commit();
  /*p36.LUGU*/ hash << OBP15.commit();
  /*p36.LEPU*/ hash << OBP16.commit();
  /*p36.LUXO*/ hash << OBP17.commit();

  hash << top.LCD_PIN_LD1.commit();
  hash << top.LCD_PIN_LD0.commit();

  return hash;
}

//------------------------------------------------------------------------------

#if 0
void dump_regs(TextPainter& text_painter) {
  text_painter.dprintf("----- PXP_REG -----\n");
  text_painter.dprintf("BG_PIX_A   0x%02x\n", bg_pix_a());
  text_painter.dprintf("BG_PIX_B   0x%02x\n", bg_pix_b());
  text_painter.dprintf("SPR_PIX_A  0x%02x\n", spr_pix_a());
  text_painter.dprintf("SPR_PIX_B  0x%02x\n", spr_pix_b());
  text_painter.dprintf("BG_PIPE_A  0x%02x\n", bg_pipe_a());
  text_painter.dprintf("BG_PIPE_B  0x%02x\n", bg_pipe_b());
  text_painter.dprintf("SPR_PIPE_A 0x%02x\n", spr_pipe_a());
  text_painter.dprintf("SPR_PIPE_B 0x%02x\n", spr_pipe_b());
  text_painter.dprintf("PAL_PIPE   0x%02x\n", pal_pipe());
  text_painter.dprintf("MASK_PIPE  0x%02x\n", mask_pipe());
  text_painter.newline();
}

uint8_t bg_pix_a() { return (uint8_t)pack(LEGU_BG_PIX_A0.q(), NUDU_BG_PIX_A1.q(), MUKU_BG_PIX_A2.q(), LUZO_BG_PIX_A3.q(), MEGU_BG_PIX_A4.q(), MYJY_BG_PIX_A5.q(), NASA_BG_PIX_A6.q(), NEFO_BG_PIX_A7.q()); }
uint8_t bg_pix_b() { return (uint8_t)pack(RAWU_BG_PIX_B0.q(), POZO_BG_PIX_B1.q(), PYZO_BG_PIX_B2.q(), POXA_BG_PIX_B3.q(), PULO_BG_PIX_B4.q(), POJU_BG_PIX_B5.q(), POWY_BG_PIX_B6.q(), PYJU_BG_PIX_B7.q()); }
uint8_t spr_pix_a() { return (uint8_t)pack(SPR_PIX_A0.q(), SPR_PIX_A1.q(), SPR_PIX_A2.q(), SPR_PIX_A3.q(), SPR_PIX_A4.q(), SPR_PIX_A5.q(), SPR_PIX_A6.q(), SPR_PIX_A7.q()); }
uint8_t spr_pix_b() { return (uint8_t)pack(SPR_PIX_B0.q(), SPR_PIX_B1.q(), SPR_PIX_B2.q(), SPR_PIX_B3.q(), SPR_PIX_B4.q(), SPR_PIX_B5.q(), SPR_PIX_B6.q(), SPR_PIX_B7.q()); }
uint8_t bg_pipe_a() { return (uint8_t)pack(BG_PIPE_A0.q(), BG_PIPE_A1.q(), BG_PIPE_A2.q(), BG_PIPE_A3.q(), BG_PIPE_A4.q(), BG_PIPE_A5.q(), BG_PIPE_A6.q(), BG_PIPE_A7.q()); }
uint8_t bg_pipe_b() { return (uint8_t)pack(BG_PIPE_B0.q(), BG_PIPE_B1.q(), BG_PIPE_B2.q(), BG_PIPE_B3.q(), BG_PIPE_B4.q(), BG_PIPE_B5.q(), BG_PIPE_B6.q(), BG_PIPE_B7.q()); }
uint8_t spr_pipe_a() { return (uint8_t)pack(SPR_PIPE_A0.q(), SPR_PIPE_A1.q(), SPR_PIPE_A2.q(), SPR_PIPE_A3.q(), SPR_PIPE_A4.q(), SPR_PIPE_A5.q(), SPR_PIPE_A6.q(), SPR_PIPE_A7.q()); }
uint8_t spr_pipe_b() { return (uint8_t)pack(SPR_PIPE_B0.q(), SPR_PIPE_B1.q(), SPR_PIPE_B2.q(), SPR_PIPE_B3.q(), SPR_PIPE_B4.q(), SPR_PIPE_B5.q(), SPR_PIPE_B6.q(), SPR_PIPE_B7.q()); }
uint8_t pal_pipe() { return (uint8_t)pack(PAL_PIPE_0.q(), PAL_PIPE_1.q(), PAL_PIPE_2.q(), PAL_PIPE_3.q(), PAL_PIPE_4.q(), PAL_PIPE_5.q(), PAL_PIPE_6.q(), PAL_PIPE_7.q()); }
uint8_t mask_pipe() { return (uint8_t)pack(MASK_PIPE_0.q(), MASK_PIPE_1.q(), MASK_PIPE_2.q(), MASK_PIPE_3.q(), MASK_PIPE_4.q(), MASK_PIPE_5.q(), MASK_PIPE_6.q(), MASK_PIPE_7.q()); }

#endif