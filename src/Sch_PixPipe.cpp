#include "Sch_PixPipe.h"

#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void PixelPipeRegisters::tick(SchematicTop& top) {

  //----------------------------------------

  {
    /*p35.RAJY*/ wire _PIX_BG0  = and(top.VYXE_LCDC_BGEN, BG_PIPE_A7);
    /*p35.TADE*/ wire _PIX_BG1  = and(top.VYXE_LCDC_BGEN, BG_PIPE_B7);

    /*p35.WOXA*/ wire _PIX_SP0  = and(top.XYLO_LCDC_SPEN, SPR_PIPE_A7);
    /*p35.XULA*/ wire _PIX_SP1  = and(top.XYLO_LCDC_SPEN, SPR_PIPE_B7);

    /*p35.NULY*/ wire _PIX_SPn  = nor(_PIX_SP0, _PIX_SP1);

    /*p35.RYFU*/ wire _MASK_BG0 = and(_PIX_BG0, MASK_PIPE_7);
    /*p35.RUTA*/ wire _MASK_BG1 = and(_PIX_BG1, MASK_PIPE_7);
    /*p35.POKA*/ wire _BGPIXELn = nor(_PIX_SPn, _MASK_BG0, _MASK_BG1);

    /*p34.LOME*/ wire _SPRITE_PAL_PIPE_7n = not(PAL_PIPE_7);
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

    /*p35.WUFU*/ wire _COL_OBP00 = amux4(OBP07, _PAL_OBP0D, OBP05, _PAL_OBP0C, OBP03, _PAL_OBP0B, OBP01, _PAL_OBP0A);
    /*p35.WALY*/ wire _COL_OBP01 = amux4(OBP06, _PAL_OBP0D, OBP04, _PAL_OBP0C, OBP02, _PAL_OBP0B, OBP00, _PAL_OBP0A);

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

    /*p35.MOKA*/ wire _COL_OBP10 = amux4(OBP17, _PAL_OBP1D, OBP15, _PAL_OBP1C, OBP13, _PAL_OBP1B, OBP11, _PAL_OBP1A);
    /*p35.MUFA*/ wire _COL_OBP11 = amux4(OBP16, _PAL_OBP1D, OBP14, _PAL_OBP1C, OBP12, _PAL_OBP1B, OBP10, _PAL_OBP1A);

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

    /*p35.NURA*/ wire _COL_BGP1 = amux4(BGP7, _PAL_BGPD, BGP5, _PAL_BGPC, BGP3, _PAL_BGPB, BGP1, _PAL_BGPA);
    /*p35.NELO*/ wire _COL_BGP0 = amux4(BGP6, _PAL_BGPD, BGP4, _PAL_BGPC, BGP2, _PAL_BGPB, BGP0, _PAL_BGPA);

    //----------
    // Pixel merge and send

    // bits 0 and 1 swapped somewhere...

    /*p35.PATY*/ wire PATY_PIX_OUT_LO = or(_COL_BGP1, _COL_OBP00, _COL_OBP10);
    /*p35.PERO*/ wire PERO_PIX_OUT_HI = or(_COL_BGP0, _COL_OBP01, _COL_OBP11);

    /*p35.REMY*/ wire REMY_LD0n = not(PATY_PIX_OUT_LO);
    /*p35.RAVO*/ wire RAVO_LD1n = not(PERO_PIX_OUT_HI);
    top.LCD_PIN_LD0.set(not(REMY_LD0n));
    top.LCD_PIN_LD1.set(not(RAVO_LD1n));
  }

  {
    /*p32.LOZE*/ wire LOZE_PIPE_A_LOAD = not(top.NYXU_TILE_FETCHER_RSTn());

    /*p32.LUHE*/ wire BG_PIX_A0n = not(top.LEGU_BG_PIX_A0);
    /*p32.NOLY*/ wire BG_PIX_A1n = not(top.NUDU_BG_PIX_A1);
    /*p32.LEKE*/ wire BG_PIX_A2n = not(top.MUKU_BG_PIX_A2);
    /*p32.LOMY*/ wire BG_PIX_A3n = not(top.LUZO_BG_PIX_A3);
    /*p32.LALA*/ wire BG_PIX_A4n = not(top.MEGU_BG_PIX_A4);
    /*p32.LOXA*/ wire BG_PIX_A5n = not(top.MYJY_BG_PIX_A5);
    /*p32.NEZE*/ wire BG_PIX_A6n = not(top.NASA_BG_PIX_A6);
    /*p32.NOBO*/ wire BG_PIX_A7n = not(top.NEFO_BG_PIX_A7);

    /*p32.LAKY*/ wire BG_PIPE_A_SET0 = nand(LOZE_PIPE_A_LOAD, top.LEGU_BG_PIX_A0);
    /*p32.NYXO*/ wire BG_PIPE_A_SET1 = nand(LOZE_PIPE_A_LOAD, top.NUDU_BG_PIX_A1);
    /*p32.LOTO*/ wire BG_PIPE_A_SET2 = nand(LOZE_PIPE_A_LOAD, top.MUKU_BG_PIX_A2);
    /*p32.LYDU*/ wire BG_PIPE_A_SET3 = nand(LOZE_PIPE_A_LOAD, top.LUZO_BG_PIX_A3);
    /*p32.MYVY*/ wire BG_PIPE_A_SET4 = nand(LOZE_PIPE_A_LOAD, top.MEGU_BG_PIX_A4);
    /*p32.LODO*/ wire BG_PIPE_A_SET5 = nand(LOZE_PIPE_A_LOAD, top.MYJY_BG_PIX_A5);
    /*p32.NUTE*/ wire BG_PIPE_A_SET6 = nand(LOZE_PIPE_A_LOAD, top.NASA_BG_PIX_A6);
    /*p32.NAJA*/ wire BG_PIPE_A_SET7 = nand(LOZE_PIPE_A_LOAD, top.NEFO_BG_PIX_A7);

    /*p32.LOTY*/ wire BG_PIPE_A_RST0 = nand(LOZE_PIPE_A_LOAD, BG_PIX_A0n);
    /*p32.NEXA*/ wire BG_PIPE_A_RST1 = nand(LOZE_PIPE_A_LOAD, BG_PIX_A1n);
    /*p32.LUTU*/ wire BG_PIPE_A_RST2 = nand(LOZE_PIPE_A_LOAD, BG_PIX_A2n);
    /*p32.LUJA*/ wire BG_PIPE_A_RST3 = nand(LOZE_PIPE_A_LOAD, BG_PIX_A3n);
    /*p32.MOSY*/ wire BG_PIPE_A_RST4 = nand(LOZE_PIPE_A_LOAD, BG_PIX_A4n);
    /*p32.LERU*/ wire BG_PIPE_A_RST5 = nand(LOZE_PIPE_A_LOAD, BG_PIX_A5n);
    /*p32.NYHA*/ wire BG_PIPE_A_RST6 = nand(LOZE_PIPE_A_LOAD, BG_PIX_A6n);
    /*p32.NADY*/ wire BG_PIPE_A_RST7 = nand(LOZE_PIPE_A_LOAD, BG_PIX_A7n);

    /*p32.MYDE*/ BG_PIPE_A0.set(top.SACU_CLKPIPEp(), BG_PIPE_A_SET0, BG_PIPE_A_RST0, top.JOY_PIN_P10_B);
    /*p32.NOZO*/ BG_PIPE_A1.set(top.SACU_CLKPIPEp(), BG_PIPE_A_SET1, BG_PIPE_A_RST1, BG_PIPE_A0);
    /*p32.MOJU*/ BG_PIPE_A2.set(top.SACU_CLKPIPEp(), BG_PIPE_A_SET2, BG_PIPE_A_RST2, BG_PIPE_A1);
    /*p32.MACU*/ BG_PIPE_A3.set(top.SACU_CLKPIPEp(), BG_PIPE_A_SET3, BG_PIPE_A_RST3, BG_PIPE_A2);
    /*p32.NEPO*/ BG_PIPE_A4.set(top.SACU_CLKPIPEp(), BG_PIPE_A_SET4, BG_PIPE_A_RST4, BG_PIPE_A3);
    /*p32.MODU*/ BG_PIPE_A5.set(top.SACU_CLKPIPEp(), BG_PIPE_A_SET5, BG_PIPE_A_RST5, BG_PIPE_A4);
    /*p32.NEDA*/ BG_PIPE_A6.set(top.SACU_CLKPIPEp(), BG_PIPE_A_SET6, BG_PIPE_A_RST6, BG_PIPE_A5);
    /*p32.PYBO*/ BG_PIPE_A7.set(top.SACU_CLKPIPEp(), BG_PIPE_A_SET7, BG_PIPE_A_RST7, BG_PIPE_A6);
  }

  {
    /*p32.LUXA*/ wire LUXA_PIPE_B_LOAD = not(top.NYXU_TILE_FETCHER_RSTn());

    /*p32.TOSA*/ wire BG_PIX_B0n = not(top.RAWU_BG_PIX_B0);
    /*p32.RUCO*/ wire BG_PIX_B1n = not(top.POZO_BG_PIX_B1);
    /*p32.TYCE*/ wire BG_PIX_B2n = not(top.PYZO_BG_PIX_B2);
    /*p32.REVY*/ wire BG_PIX_B3n = not(top.POXA_BG_PIX_B3);
    /*p32.RYGA*/ wire BG_PIX_B4n = not(top.PULO_BG_PIX_B4);
    /*p32.RYLE*/ wire BG_PIX_B5n = not(top.POJU_BG_PIX_B5);
    /*p32.RAPU*/ wire BG_PIX_B6n = not(top.POWY_BG_PIX_B6);
    /*p32.SOJA*/ wire BG_PIX_B7n = not(top.PYJU_BG_PIX_B7);

    /*p32.TUXE*/ wire BG_PIPE_B_SET0 = nand(LUXA_PIPE_B_LOAD, top.RAWU_BG_PIX_B0);
    /*p32.SOLY*/ wire BG_PIPE_B_SET1 = nand(LUXA_PIPE_B_LOAD, top.POZO_BG_PIX_B1);
    /*p32.RUCE*/ wire BG_PIPE_B_SET2 = nand(LUXA_PIPE_B_LOAD, top.PYZO_BG_PIX_B2);
    /*p32.RYJA*/ wire BG_PIPE_B_SET3 = nand(LUXA_PIPE_B_LOAD, top.POXA_BG_PIX_B3);
    /*p32.RUTO*/ wire BG_PIPE_B_SET4 = nand(LUXA_PIPE_B_LOAD, top.PULO_BG_PIX_B4);
    /*p32.RAJA*/ wire BG_PIPE_B_SET5 = nand(LUXA_PIPE_B_LOAD, top.POJU_BG_PIX_B5);
    /*p32.RAJO*/ wire BG_PIPE_B_SET6 = nand(LUXA_PIPE_B_LOAD, top.POWY_BG_PIX_B6);
    /*p32.RAGA*/ wire BG_PIPE_B_SET7 = nand(LUXA_PIPE_B_LOAD, top.PYJU_BG_PIX_B7);

    /*p32.SEJA*/ wire BG_PIPE_B_RST0 = nand(LUXA_PIPE_B_LOAD, BG_PIX_B0n);
    /*p32.SENO*/ wire BG_PIPE_B_RST1 = nand(LUXA_PIPE_B_LOAD, BG_PIX_B1n);
    /*p32.SURE*/ wire BG_PIPE_B_RST2 = nand(LUXA_PIPE_B_LOAD, BG_PIX_B2n);
    /*p32.SEBO*/ wire BG_PIPE_B_RST3 = nand(LUXA_PIPE_B_LOAD, BG_PIX_B3n);
    /*p32.SUCA*/ wire BG_PIPE_B_RST4 = nand(LUXA_PIPE_B_LOAD, BG_PIX_B4n);
    /*p32.SYWE*/ wire BG_PIPE_B_RST5 = nand(LUXA_PIPE_B_LOAD, BG_PIX_B5n);
    /*p32.SUPU*/ wire BG_PIPE_B_RST6 = nand(LUXA_PIPE_B_LOAD, BG_PIX_B6n);
    /*p32.RYJY*/ wire BG_PIPE_B_RST7 = nand(LUXA_PIPE_B_LOAD, BG_PIX_B7n);

    /*p32.TOMY*/ BG_PIPE_B0.set(top.SACU_CLKPIPEp(), BG_PIPE_B_SET0, BG_PIPE_B_RST0, top.JOY_PIN_P10_B);
    /*p32.TACA*/ BG_PIPE_B1.set(top.SACU_CLKPIPEp(), BG_PIPE_B_SET1, BG_PIPE_B_RST1, BG_PIPE_B0);
    /*p32.SADY*/ BG_PIPE_B2.set(top.SACU_CLKPIPEp(), BG_PIPE_B_SET2, BG_PIPE_B_RST2, BG_PIPE_B1);
    /*p32.RYSA*/ BG_PIPE_B3.set(top.SACU_CLKPIPEp(), BG_PIPE_B_SET3, BG_PIPE_B_RST3, BG_PIPE_B2);
    /*p32.SOBO*/ BG_PIPE_B4.set(top.SACU_CLKPIPEp(), BG_PIPE_B_SET4, BG_PIPE_B_RST4, BG_PIPE_B3);
    /*p32.SETU*/ BG_PIPE_B5.set(top.SACU_CLKPIPEp(), BG_PIPE_B_SET5, BG_PIPE_B_RST5, BG_PIPE_B4);
    /*p32.RALU*/ BG_PIPE_B6.set(top.SACU_CLKPIPEp(), BG_PIPE_B_SET6, BG_PIPE_B_RST6, BG_PIPE_B5);
    /*p32.SOHU*/ BG_PIPE_B7.set(top.SACU_CLKPIPEp(), BG_PIPE_B_SET7, BG_PIPE_B_RST7, BG_PIPE_B6);
  }

  {
    /*p29.WUTY*/ wire WUTY_PIPE_LOAD_SPRITEp = not(top.VUSA_PIPE_LOAD_SPRITEn());
    /*p29.XEFY*/ wire XEPY_PIPE_LOAD_SPRITEn = not(WUTY_PIPE_LOAD_SPRITEp);

    /*p34.MEFU*/ wire SPRITE_MASK0p = or(XEPY_PIPE_LOAD_SPRITEn, SPR_PIPE_A0, SPR_PIPE_B0); // def or
    /*p34.MEVE*/ wire SPRITE_MASK1p = or(XEPY_PIPE_LOAD_SPRITEn, SPR_PIPE_A1, SPR_PIPE_B1);
    /*p34.MYZO*/ wire SPRITE_MASK2p = or(XEPY_PIPE_LOAD_SPRITEn, SPR_PIPE_A2, SPR_PIPE_B2);
    /*p34.RUDA*/ wire SPRITE_MASK3p = or(XEPY_PIPE_LOAD_SPRITEn, SPR_PIPE_A3, SPR_PIPE_B3);
    /*p34.VOTO*/ wire SPRITE_MASK4p = or(XEPY_PIPE_LOAD_SPRITEn, SPR_PIPE_A4, SPR_PIPE_B4);
    /*p34.VYSA*/ wire SPRITE_MASK5p = or(XEPY_PIPE_LOAD_SPRITEn, SPR_PIPE_A5, SPR_PIPE_B5);
    /*p34.TORY*/ wire SPRITE_MASK6p = or(XEPY_PIPE_LOAD_SPRITEn, SPR_PIPE_A6, SPR_PIPE_B6);
    /*p34.WOPE*/ wire SPRITE_MASK7p = or(XEPY_PIPE_LOAD_SPRITEn, SPR_PIPE_A7, SPR_PIPE_B7);

    /*p34.LESY*/ wire SPRITE_MASK0n = not(SPRITE_MASK0p);
    /*p34.LOTA*/ wire SPRITE_MASK1n = not(SPRITE_MASK1p);
    /*p34.LYKU*/ wire SPRITE_MASK2n = not(SPRITE_MASK2p);
    /*p34.ROBY*/ wire SPRITE_MASK3n = not(SPRITE_MASK3p);
    /*p34.TYTA*/ wire SPRITE_MASK4n = not(SPRITE_MASK4p);
    /*p34.TYCO*/ wire SPRITE_MASK5n = not(SPRITE_MASK5p);
    /*p34.SOKA*/ wire SPRITE_MASK6n = not(SPRITE_MASK6p);
    /*p34.XOVU*/ wire SPRITE_MASK7n = not(SPRITE_MASK7p);

    // Sprite pipe A
    {
#if 0
      if (!XEPY_PIPE_LOAD_SPRITEn && !SPR_PIPE_A0 && !SPR_PIPE_B0) {
        /*p33.NYLU*/ SPR_PIPE_A0 = SPR_PIX_A0;
      }
      else {
        /*p33.NYLU*/ SPR_PIPE_A0.set(ppu_sig.SACU_CLKPIPEp, 1, 1, JOY_PIN_P10_B);
      }
#endif

      /*p33.LOZA*/ wire SPR_PIX_A0n = not(top.SPR_PIX_A0());
      /*p33.SYBO*/ wire SPR_PIX_A1n = not(top.SPR_PIX_A1());
      /*p33.LUMO*/ wire SPR_PIX_A2n = not(top.SPR_PIX_A2());
      /*p33.SOLO*/ wire SPR_PIX_A3n = not(top.SPR_PIX_A3());
      /*p33.VOBY*/ wire SPR_PIX_A4n = not(top.SPR_PIX_A4());
      /*p33.WYCO*/ wire SPR_PIX_A5n = not(top.SPR_PIX_A5());
      /*p33.SERY*/ wire SPR_PIX_A6n = not(top.SPR_PIX_A6());
      /*p33.SELU*/ wire SPR_PIX_A7n = not(top.SPR_PIX_A7());

      /*p33.MEZU*/ wire SPR_PIX_A_SET0 = nand(SPRITE_MASK0n, top.SPR_PIX_A0());
      /*p33.RUSY*/ wire SPR_PIX_A_SET1 = nand(SPRITE_MASK1n, top.SPR_PIX_A1());
      /*p33.MYXA*/ wire SPR_PIX_A_SET2 = nand(SPRITE_MASK2n, top.SPR_PIX_A2());
      /*p33.RANO*/ wire SPR_PIX_A_SET3 = nand(SPRITE_MASK3n, top.SPR_PIX_A3());
      /*p33.TYGA*/ wire SPR_PIX_A_SET4 = nand(SPRITE_MASK4n, top.SPR_PIX_A4());
      /*p33.VUME*/ wire SPR_PIX_A_SET5 = nand(SPRITE_MASK5n, top.SPR_PIX_A5());
      /*p33.TAPO*/ wire SPR_PIX_A_SET6 = nand(SPRITE_MASK6n, top.SPR_PIX_A6());
      /*p33.TESO*/ wire SPR_PIX_A_SET7 = nand(SPRITE_MASK7n, top.SPR_PIX_A7());

      /*p33.MOFY*/ wire SPR_PIX_A_RST0 = nand(SPRITE_MASK0n, SPR_PIX_A0n);
      /*p33.RUCA*/ wire SPR_PIX_A_RST1 = nand(SPRITE_MASK1n, SPR_PIX_A1n);
      /*p33.MAJO*/ wire SPR_PIX_A_RST2 = nand(SPRITE_MASK2n, SPR_PIX_A2n);
      /*p33.REHU*/ wire SPR_PIX_A_RST3 = nand(SPRITE_MASK3n, SPR_PIX_A3n);
      /*p33.WAXO*/ wire SPR_PIX_A_RST4 = nand(SPRITE_MASK4n, SPR_PIX_A4n);
      /*p33.XOLE*/ wire SPR_PIX_A_RST5 = nand(SPRITE_MASK5n, SPR_PIX_A5n);
      /*p33.TABY*/ wire SPR_PIX_A_RST6 = nand(SPRITE_MASK6n, SPR_PIX_A6n);
      /*p33.TULA*/ wire SPR_PIX_A_RST7 = nand(SPRITE_MASK7n, SPR_PIX_A7n);

      /*p33.NYLU*/ SPR_PIPE_A0.set(top.SACU_CLKPIPEp(), SPR_PIX_A_SET0, SPR_PIX_A_RST0, top.JOY_PIN_P10_B);
      /*p33.PEFU*/ SPR_PIPE_A1.set(top.SACU_CLKPIPEp(), SPR_PIX_A_SET1, SPR_PIX_A_RST1, SPR_PIPE_A0);
      /*p33.NATY*/ SPR_PIPE_A2.set(top.SACU_CLKPIPEp(), SPR_PIX_A_SET2, SPR_PIX_A_RST2, SPR_PIPE_A1);
      /*p33.PYJO*/ SPR_PIPE_A3.set(top.SACU_CLKPIPEp(), SPR_PIX_A_SET3, SPR_PIX_A_RST3, SPR_PIPE_A2);
      /*p33.VARE*/ SPR_PIPE_A4.set(top.SACU_CLKPIPEp(), SPR_PIX_A_SET4, SPR_PIX_A_RST4, SPR_PIPE_A3);
      /*p33.WEBA*/ SPR_PIPE_A5.set(top.SACU_CLKPIPEp(), SPR_PIX_A_SET5, SPR_PIX_A_RST5, SPR_PIPE_A4);
      /*p33.VANU*/ SPR_PIPE_A6.set(top.SACU_CLKPIPEp(), SPR_PIX_A_SET6, SPR_PIX_A_RST6, SPR_PIPE_A5);
      /*p33.VUPY*/ SPR_PIPE_A7.set(top.SACU_CLKPIPEp(), SPR_PIX_A_SET7, SPR_PIX_A_RST7, SPR_PIPE_A6);
    }

    // Sprite pipe B
    {
      /*p33.RATA*/ wire SPR_PIX_B0n = not(top.SPR_PIX_B0());
      /*p33.NUCA*/ wire SPR_PIX_B1n = not(top.SPR_PIX_B1());
      /*p33.LASE*/ wire SPR_PIX_B2n = not(top.SPR_PIX_B2());
      /*p33.LUBO*/ wire SPR_PIX_B3n = not(top.SPR_PIX_B3());
      /*p33.WERY*/ wire SPR_PIX_B4n = not(top.SPR_PIX_B4());
      /*p33.WURA*/ wire SPR_PIX_B5n = not(top.SPR_PIX_B5());
      /*p33.SULU*/ wire SPR_PIX_B6n = not(top.SPR_PIX_B6());
      /*p33.WAMY*/ wire SPR_PIX_B7n = not(top.SPR_PIX_B7());

      /*p33.PABE*/ wire SPR_PIX_B_SET0 = nand(SPRITE_MASK0n, top.SPR_PIX_B0());
      /*p33.MYTO*/ wire SPR_PIX_B_SET1 = nand(SPRITE_MASK1n, top.SPR_PIX_B1());
      /*p33.LELA*/ wire SPR_PIX_B_SET2 = nand(SPRITE_MASK2n, top.SPR_PIX_B2());
      /*p33.MAME*/ wire SPR_PIX_B_SET3 = nand(SPRITE_MASK3n, top.SPR_PIX_B3());
      /*p33.VEXU*/ wire SPR_PIX_B_SET4 = nand(SPRITE_MASK4n, top.SPR_PIX_B4());
      /*p33.VABY*/ wire SPR_PIX_B_SET5 = nand(SPRITE_MASK5n, top.SPR_PIX_B5());
      /*p33.TUXA*/ wire SPR_PIX_B_SET6 = nand(SPRITE_MASK6n, top.SPR_PIX_B6());
      /*p33.VUNE*/ wire SPR_PIX_B_SET7 = nand(SPRITE_MASK7n, top.SPR_PIX_B7());

      /*p33.PYZU*/ wire SPR_PIX_B_RST0 = nand(SPRITE_MASK0n, SPR_PIX_B0n);
      /*p33.MADA*/ wire SPR_PIX_B_RST1 = nand(SPRITE_MASK1n, SPR_PIX_B1n);
      /*p33.LYDE*/ wire SPR_PIX_B_RST2 = nand(SPRITE_MASK2n, SPR_PIX_B2n);
      /*p33.LUFY*/ wire SPR_PIX_B_RST3 = nand(SPRITE_MASK3n, SPR_PIX_B3n);
      /*p33.XATO*/ wire SPR_PIX_B_RST4 = nand(SPRITE_MASK4n, SPR_PIX_B4n);
      /*p33.XEXU*/ wire SPR_PIX_B_RST5 = nand(SPRITE_MASK5n, SPR_PIX_B5n);
      /*p33.TUPE*/ wire SPR_PIX_B_RST6 = nand(SPRITE_MASK6n, SPR_PIX_B6n);
      /*p33.XYVE*/ wire SPR_PIX_B_RST7 = nand(SPRITE_MASK7n, SPR_PIX_B7n);

      /*p33.NURO*/ SPR_PIPE_B0.set(top.SACU_CLKPIPEp(), SPR_PIX_B_SET0, SPR_PIX_B_RST0, top.JOY_PIN_P10_B);
      /*p33.MASO*/ SPR_PIPE_B1.set(top.SACU_CLKPIPEp(), SPR_PIX_B_SET1, SPR_PIX_B_RST1, SPR_PIPE_B0);
      /*p33.LEFE*/ SPR_PIPE_B2.set(top.SACU_CLKPIPEp(), SPR_PIX_B_SET2, SPR_PIX_B_RST2, SPR_PIPE_B1);
      /*p33.LESU*/ SPR_PIPE_B3.set(top.SACU_CLKPIPEp(), SPR_PIX_B_SET3, SPR_PIX_B_RST3, SPR_PIPE_B2);
      /*p33.WYHO*/ SPR_PIPE_B4.set(top.SACU_CLKPIPEp(), SPR_PIX_B_SET4, SPR_PIX_B_RST4, SPR_PIPE_B3);
      /*p33.WORA*/ SPR_PIPE_B5.set(top.SACU_CLKPIPEp(), SPR_PIX_B_SET5, SPR_PIX_B_RST5, SPR_PIPE_B4);
      /*p33.VAFO*/ SPR_PIPE_B6.set(top.SACU_CLKPIPEp(), SPR_PIX_B_SET6, SPR_PIX_B_RST6, SPR_PIPE_B5);
      /*p33.WUFY*/ SPR_PIPE_B7.set(top.SACU_CLKPIPEp(), SPR_PIX_B_SET7, SPR_PIX_B_RST7, SPR_PIPE_B6);
    }

    // Palette pipe
    {
      /*p34.SYPY*/ wire SYPY = not(top.GOMO_SPRITE_X4());
      /*p34.TOTU*/ wire TOTU = not(top.GOMO_SPRITE_X4());
      /*p34.NARO*/ wire NARO = not(top.GOMO_SPRITE_X4());
      /*p34.WEXY*/ wire WEXY = not(top.GOMO_SPRITE_X4());
      /*p34.RYZY*/ wire RYZY = not(top.GOMO_SPRITE_X4());
      /*p34.RYFE*/ wire RYFE = not(top.GOMO_SPRITE_X4());
      /*p34.LADY*/ wire LADY = not(top.GOMO_SPRITE_X4());
      /*p34.LAFY*/ wire LAFY = not(top.GOMO_SPRITE_X4());

      /*p34.PUME*/ wire SPRITE_PAL_PIPE_SET0n = nand(SPRITE_MASK0n, top.GOMO_SPRITE_X4());
      /*p34.SORO*/ wire SPRITE_PAL_PIPE_SET1n = nand(SPRITE_MASK1n, top.GOMO_SPRITE_X4());
      /*p34.PAMO*/ wire SPRITE_PAL_PIPE_SET2n = nand(SPRITE_MASK2n, top.GOMO_SPRITE_X4());
      /*p34.SUKY*/ wire SPRITE_PAL_PIPE_SET3n = nand(SPRITE_MASK3n, top.GOMO_SPRITE_X4());
      /*p34.RORA*/ wire SPRITE_PAL_PIPE_SET4n = nand(SPRITE_MASK4n, top.GOMO_SPRITE_X4());
      /*p34.MENE*/ wire SPRITE_PAL_PIPE_SET5n = nand(SPRITE_MASK5n, top.GOMO_SPRITE_X4());
      /*p34.LUKE*/ wire SPRITE_PAL_PIPE_SET6n = nand(SPRITE_MASK6n, top.GOMO_SPRITE_X4());
      /*p34.LAMY*/ wire SPRITE_PAL_PIPE_SET7n = nand(SPRITE_MASK7n, top.GOMO_SPRITE_X4());

      /*p34.SUCO*/ wire SPRITE_PAL_PIPE_RST0n = nand(SPRITE_MASK0n, SYPY);
      /*p34.TAFA*/ wire SPRITE_PAL_PIPE_RST1n = nand(SPRITE_MASK1n, TOTU);
      /*p34.PYZY*/ wire SPRITE_PAL_PIPE_RST2n = nand(SPRITE_MASK2n, NARO);
      /*p34.TOWA*/ wire SPRITE_PAL_PIPE_RST3n = nand(SPRITE_MASK3n, WEXY);
      /*p34.RUDU*/ wire SPRITE_PAL_PIPE_RST4n = nand(SPRITE_MASK4n, RYZY);
      /*p34.PAZO*/ wire SPRITE_PAL_PIPE_RST5n = nand(SPRITE_MASK5n, RYFE);
      /*p34.LOWA*/ wire SPRITE_PAL_PIPE_RST6n = nand(SPRITE_MASK6n, LADY);
      /*p34.LUNU*/ wire SPRITE_PAL_PIPE_RST7n = nand(SPRITE_MASK7n, LAFY);

      /*p34.RUGO*/ PAL_PIPE_0.set(top.SACU_CLKPIPEp(), SPRITE_PAL_PIPE_SET0n, SPRITE_PAL_PIPE_RST0n, top.JOY_PIN_P10_B);
      /*p34.SATA*/ PAL_PIPE_1.set(top.SACU_CLKPIPEp(), SPRITE_PAL_PIPE_SET1n, SPRITE_PAL_PIPE_RST1n, PAL_PIPE_0);
      /*p34.ROSA*/ PAL_PIPE_2.set(top.SACU_CLKPIPEp(), SPRITE_PAL_PIPE_SET2n, SPRITE_PAL_PIPE_RST2n, PAL_PIPE_1);
      /*p34.SOMY*/ PAL_PIPE_3.set(top.SACU_CLKPIPEp(), SPRITE_PAL_PIPE_SET3n, SPRITE_PAL_PIPE_RST3n, PAL_PIPE_2);
      /*p34.PALU*/ PAL_PIPE_4.set(top.SACU_CLKPIPEp(), SPRITE_PAL_PIPE_SET4n, SPRITE_PAL_PIPE_RST4n, PAL_PIPE_3);
      /*p34.NUKE*/ PAL_PIPE_5.set(top.SACU_CLKPIPEp(), SPRITE_PAL_PIPE_SET5n, SPRITE_PAL_PIPE_RST5n, PAL_PIPE_4);
      /*p34.MODA*/ PAL_PIPE_6.set(top.SACU_CLKPIPEp(), SPRITE_PAL_PIPE_SET6n, SPRITE_PAL_PIPE_RST6n, PAL_PIPE_5);
      /*p34.LYME*/ PAL_PIPE_7.set(top.SACU_CLKPIPEp(), SPRITE_PAL_PIPE_SET7n, SPRITE_PAL_PIPE_RST7n, PAL_PIPE_6);
    }

    // Background mask pipe
    {
      /*p26.XOGA*/ wire XOGA = not(top.DEPO_SPRITE_X7());
      /*p26.XURA*/ wire XURA = not(top.DEPO_SPRITE_X7());
      /*p26.TAJO*/ wire TAJO = not(top.DEPO_SPRITE_X7());
      /*p26.XENU*/ wire XENU = not(top.DEPO_SPRITE_X7());
      /*p26.XYKE*/ wire XYKE = not(top.DEPO_SPRITE_X7());
      /*p26.XABA*/ wire XABA = not(top.DEPO_SPRITE_X7());
      /*p26.TAFU*/ wire TAFU = not(top.DEPO_SPRITE_X7());
      /*p26.XUHO*/ wire XUHO = not(top.DEPO_SPRITE_X7());

      /*p26.TEDE*/ wire MASK_PIPE_SET0 = nand(SPRITE_MASK0n, top.DEPO_SPRITE_X7());
      /*p26.XALA*/ wire MASK_PIPE_SET1 = nand(SPRITE_MASK1n, top.DEPO_SPRITE_X7());
      /*p26.TYRA*/ wire MASK_PIPE_SET2 = nand(SPRITE_MASK2n, top.DEPO_SPRITE_X7());
      /*p26.XYRU*/ wire MASK_PIPE_SET3 = nand(SPRITE_MASK3n, top.DEPO_SPRITE_X7());
      /*p26.XUKU*/ wire MASK_PIPE_SET4 = nand(SPRITE_MASK4n, top.DEPO_SPRITE_X7());
      /*p26.XELY*/ wire MASK_PIPE_SET5 = nand(SPRITE_MASK5n, top.DEPO_SPRITE_X7());
      /*p26.TYKO*/ wire MASK_PIPE_SET6 = nand(SPRITE_MASK6n, top.DEPO_SPRITE_X7());
      /*p26.TUWU*/ wire MASK_PIPE_SET7 = nand(SPRITE_MASK7n, top.DEPO_SPRITE_X7());

      /*p26.WOKA*/ wire MASK_PIPE_RST0 = nand(SPRITE_MASK0n, XOGA);
      /*p26.WEDE*/ wire MASK_PIPE_RST1 = nand(SPRITE_MASK1n, XURA);
      /*p26.TUFO*/ wire MASK_PIPE_RST2 = nand(SPRITE_MASK2n, TAJO);
      /*p26.WEVO*/ wire MASK_PIPE_RST3 = nand(SPRITE_MASK3n, XENU);
      /*p26.WEDY*/ wire MASK_PIPE_RST4 = nand(SPRITE_MASK4n, XYKE);
      /*p26.WUJA*/ wire MASK_PIPE_RST5 = nand(SPRITE_MASK5n, XABA);
      /*p26.TENA*/ wire MASK_PIPE_RST6 = nand(SPRITE_MASK6n, TAFU);
      /*p26.WUBU*/ wire MASK_PIPE_RST7 = nand(SPRITE_MASK7n, XUHO);

      /*p26.VEZO*/ MASK_PIPE_0.set(top.SACU_CLKPIPEp(), MASK_PIPE_SET0, MASK_PIPE_RST0, top.JOY_PIN_P10_B);
      /*p26.WURU*/ MASK_PIPE_1.set(top.SACU_CLKPIPEp(), MASK_PIPE_SET1, MASK_PIPE_RST1, MASK_PIPE_0);
      /*p26.VOSA*/ MASK_PIPE_2.set(top.SACU_CLKPIPEp(), MASK_PIPE_SET2, MASK_PIPE_RST2, MASK_PIPE_1);
      /*p26.WYFU*/ MASK_PIPE_3.set(top.SACU_CLKPIPEp(), MASK_PIPE_SET3, MASK_PIPE_RST3, MASK_PIPE_2);
      /*p26.XETE*/ MASK_PIPE_4.set(top.SACU_CLKPIPEp(), MASK_PIPE_SET4, MASK_PIPE_RST4, MASK_PIPE_3);
      /*p26.WODA*/ MASK_PIPE_5.set(top.SACU_CLKPIPEp(), MASK_PIPE_SET5, MASK_PIPE_RST5, MASK_PIPE_4);
      /*p26.VUMO*/ MASK_PIPE_6.set(top.SACU_CLKPIPEp(), MASK_PIPE_SET6, MASK_PIPE_RST6, MASK_PIPE_5);
      /*p26.VAVA*/ MASK_PIPE_7.set(top.SACU_CLKPIPEp(), MASK_PIPE_SET7, MASK_PIPE_RST7, MASK_PIPE_6);
    }
  }

  //----------------------------------------

  /*p07.TEDO*/ wire TEDO_CPU_RD = not(top.UJYV_CPU_RD());
  /*p07.AJAS*/ wire AJAS_CPU_RD = not(TEDO_CPU_RD);
  /*p07.ASOT*/ wire ASOT_CPU_RD = not(AJAS_CPU_RD);

  /*p07.TAPU*/ wire TAPU_CPU_WR_xxxxxFGH = not(top.UBAL_CPU_WRp_ABCDExxx());
  /*p07.DYKY*/ wire DYKY_CPU_WR_ABCDExxx = not(TAPU_CPU_WR_xxxxxFGH);
  /*p07.CUPA*/ wire CUPA_CPU_WR_xxxxxFGH = not(DYKY_CPU_WR_ABCDExxx);

  /*p22.XOLA*/ wire XOLA_A00n = not(top.CPU_PIN_A00);
  /*p22.XENO*/ wire XENO_A01n = not(top.CPU_PIN_A01);
  /*p22.XUSY*/ wire XUSY_A02n = not(top.CPU_PIN_A02);
  /*p22.XERA*/ wire XERA_A03n = not(top.CPU_PIN_A03);

  /*p22.WADO*/ wire WADO_A00p = not(XOLA_A00n);
  /*p22.WESA*/ wire WESA_A01p = not(XENO_A01n);
  /*p22.WALO*/ wire WALO_A02p = not(XUSY_A02n);
  /*p22.WEPO*/ wire WEPO_A03p = not(XERA_A03n);

  // FF47 BGP
  {
    /*p22.WYBO*/ wire FF47n = nand(top.WERO_FF40_FF4Fp(), WADO_A00p, WESA_A01p, WALO_A02p, XERA_A03n);
    /*p22.WERA*/ wire FF47 = not(FF47n);
    /*p36.VELY*/ wire FF47_WR = and (CUPA_CPU_WR_xxxxxFGH, FF47);
    /*p36.TEPO*/ wire FF47_WRn = not(FF47_WR);

    /*p36.PAVO*/ BGP0.set(FF47_WRn, 1, top.CPU_TRI_D0);
    /*p36.NUSY*/ BGP1.set(FF47_WRn, 1, top.CPU_TRI_D1);
    /*p36.PYLU*/ BGP2.set(FF47_WRn, 1, top.CPU_TRI_D2);
    /*p36.MAXY*/ BGP3.set(FF47_WRn, 1, top.CPU_TRI_D3);
    /*p36.MUKE*/ BGP4.set(FF47_WRn, 1, top.CPU_TRI_D4);
    /*p36.MORU*/ BGP5.set(FF47_WRn, 1, top.CPU_TRI_D5);
    /*p36.MOGY*/ BGP6.set(FF47_WRn, 1, top.CPU_TRI_D6);
    /*p36.MENA*/ BGP7.set(FF47_WRn, 1, top.CPU_TRI_D7);

    /*p36.VUSO*/ wire FF47_RD = and (ASOT_CPU_RD, FF47);
    /*p36.TEPY*/ wire FF47_RDn = not(FF47_RD);

    /*p36.RARO*/ top.CPU_TRI_D0.set_tribuf(!FF47_RDn, BGP0.q());
    /*p36.PABA*/ top.CPU_TRI_D1.set_tribuf(!FF47_RDn, BGP1.q());
    /*p36.REDO*/ top.CPU_TRI_D2.set_tribuf(!FF47_RDn, BGP2.q());
    /*p36.LOBE*/ top.CPU_TRI_D3.set_tribuf(!FF47_RDn, BGP3.q());
    /*p36.LACE*/ top.CPU_TRI_D4.set_tribuf(!FF47_RDn, BGP4.q());
    /*p36.LYKA*/ top.CPU_TRI_D5.set_tribuf(!FF47_RDn, BGP5.q());
    /*p36.LODY*/ top.CPU_TRI_D6.set_tribuf(!FF47_RDn, BGP6.q());
    /*p36.LARY*/ top.CPU_TRI_D7.set_tribuf(!FF47_RDn, BGP7.q());
  }

  // FF48 OBP0
  {
    /*p22.WETA*/ wire FF48n = nand(top.WERO_FF40_FF4Fp(), XOLA_A00n, XENO_A01n, XUSY_A02n, WEPO_A03p);
    /*p22.XAYO*/ wire FF48 = not(FF48n);
    /*p36.XOMA*/ wire FF48_WR = and (CUPA_CPU_WR_xxxxxFGH, FF48);
    /*p36.XELO*/ wire FF48_WRn = not(FF48_WR);

    /*p36.XUFU*/ OBP00.set(FF48_WRn, 1, top.CPU_TRI_D0);
    /*p36.XUKY*/ OBP01.set(FF48_WRn, 1, top.CPU_TRI_D1);
    /*p36.XOVA*/ OBP02.set(FF48_WRn, 1, top.CPU_TRI_D2);
    /*p36.XALO*/ OBP03.set(FF48_WRn, 1, top.CPU_TRI_D3);
    /*p36.XERU*/ OBP04.set(FF48_WRn, 1, top.CPU_TRI_D4);
    /*p36.XYZE*/ OBP05.set(FF48_WRn, 1, top.CPU_TRI_D5);
    /*p36.XUPO*/ OBP06.set(FF48_WRn, 1, top.CPU_TRI_D6);
    /*p36.XANA*/ OBP07.set(FF48_WRn, 1, top.CPU_TRI_D7);

    /*p36.XUFY*/ wire FF48_RD = and (ASOT_CPU_RD, FF48);
    /*p36.XOZY*/ wire FF48_RDn = not(FF48_RD);

    /*p36.XARY*/ top.CPU_TRI_D0.set_tribuf(!FF48_RDn, OBP00.q());
    /*p36.XOKE*/ top.CPU_TRI_D1.set_tribuf(!FF48_RDn, OBP01.q());
    /*p36.XUNO*/ top.CPU_TRI_D2.set_tribuf(!FF48_RDn, OBP02.q());
    /*p36.XUBY*/ top.CPU_TRI_D3.set_tribuf(!FF48_RDn, OBP03.q());
    /*p36.XAJU*/ top.CPU_TRI_D4.set_tribuf(!FF48_RDn, OBP04.q());
    /*p36.XOBO*/ top.CPU_TRI_D5.set_tribuf(!FF48_RDn, OBP05.q());
    /*p36.XAXA*/ top.CPU_TRI_D6.set_tribuf(!FF48_RDn, OBP06.q());
    /*p36.XAWO*/ top.CPU_TRI_D7.set_tribuf(!FF48_RDn, OBP07.q());
  }

  // FF49 OBP1
  {
    /*p22.VAMA*/ wire FF49n = nand(top.WERO_FF40_FF4Fp(), WADO_A00p, XENO_A01n, XUSY_A02n, WEPO_A03p);
    /*p22.TEGO*/ wire FF49 = not(FF49n);
    /*p36.MYXE*/ wire FF49_WR = and (CUPA_CPU_WR_xxxxxFGH, FF49);
    /*p36.LEHO*/ wire FF49_WRn = not(FF49_WR);

    /*p36.MOXY*/ OBP10.set(FF49_WRn, 1, top.CPU_TRI_D0);
    /*p36.LAWO*/ OBP11.set(FF49_WRn, 1, top.CPU_TRI_D1);
    /*p36.MOSA*/ OBP12.set(FF49_WRn, 1, top.CPU_TRI_D2);
    /*p36.LOSE*/ OBP13.set(FF49_WRn, 1, top.CPU_TRI_D3);
    /*p36.LUNE*/ OBP14.set(FF49_WRn, 1, top.CPU_TRI_D4);
    /*p36.LUGU*/ OBP15.set(FF49_WRn, 1, top.CPU_TRI_D5);
    /*p36.LEPU*/ OBP16.set(FF49_WRn, 1, top.CPU_TRI_D6);
    /*p36.LUXO*/ OBP17.set(FF49_WRn, 1, top.CPU_TRI_D7);

    /*p36.MUMY*/ wire FF49_RD = and (ASOT_CPU_RD, FF49);
    /*p36.LOTE*/ wire FF49_RDn = not(FF49_RD); // where does this go?

    /*p36.LAJU*/ top.CPU_TRI_D0.set_tribuf(!FF49_RDn, OBP10.q());
    /*p36.LEPA*/ top.CPU_TRI_D1.set_tribuf(!FF49_RDn, OBP11.q());
    /*p36.LODE*/ top.CPU_TRI_D2.set_tribuf(!FF49_RDn, OBP12.q());
    /*p36.LYZA*/ top.CPU_TRI_D3.set_tribuf(!FF49_RDn, OBP13.q());
    /*p36.LUKY*/ top.CPU_TRI_D4.set_tribuf(!FF49_RDn, OBP14.q());
    /*p36.LUGA*/ top.CPU_TRI_D5.set_tribuf(!FF49_RDn, OBP15.q());
    /*p36.LEBA*/ top.CPU_TRI_D6.set_tribuf(!FF49_RDn, OBP16.q());
    /*p36.LELU*/ top.CPU_TRI_D7.set_tribuf(!FF49_RDn, OBP17.q());
  }
}

//------------------------------------------------------------------------------

SignalHash PixelPipeRegisters::commit() {
  SignalHash hash;

  /*p32.MYDE*/ hash << BG_PIPE_A0.commit_reg();
  /*p32.NOZO*/ hash << BG_PIPE_A1.commit_reg();
  /*p32.MOJU*/ hash << BG_PIPE_A2.commit_reg();
  /*p32.MACU*/ hash << BG_PIPE_A3.commit_reg();
  /*p32.NEPO*/ hash << BG_PIPE_A4.commit_reg();
  /*p32.MODU*/ hash << BG_PIPE_A5.commit_reg();
  /*p32.NEDA*/ hash << BG_PIPE_A6.commit_reg();
  /*p32.PYBO*/ hash << BG_PIPE_A7.commit_reg();
  /*p32.TOMY*/ hash << BG_PIPE_B0.commit_reg();
  /*p32.TACA*/ hash << BG_PIPE_B1.commit_reg();
  /*p32.SADY*/ hash << BG_PIPE_B2.commit_reg();
  /*p32.RYSA*/ hash << BG_PIPE_B3.commit_reg();
  /*p32.SOBO*/ hash << BG_PIPE_B4.commit_reg();
  /*p32.SETU*/ hash << BG_PIPE_B5.commit_reg();
  /*p32.RALU*/ hash << BG_PIPE_B6.commit_reg();
  /*p32.SOHU*/ hash << BG_PIPE_B7.commit_reg();
  /*p33.NURO*/ hash << SPR_PIPE_B0.commit_reg();
  /*p33.MASO*/ hash << SPR_PIPE_B1.commit_reg();
  /*p33.LEFE*/ hash << SPR_PIPE_B2.commit_reg();
  /*p33.LESU*/ hash << SPR_PIPE_B3.commit_reg();
  /*p33.WYHO*/ hash << SPR_PIPE_B4.commit_reg();
  /*p33.WORA*/ hash << SPR_PIPE_B5.commit_reg();
  /*p33.VAFO*/ hash << SPR_PIPE_B6.commit_reg();
  /*p33.WUFY*/ hash << SPR_PIPE_B7.commit_reg();
  /*p33.NYLU*/ hash << SPR_PIPE_A0.commit_reg();
  /*p33.PEFU*/ hash << SPR_PIPE_A1.commit_reg();
  /*p33.NATY*/ hash << SPR_PIPE_A2.commit_reg();
  /*p33.PYJO*/ hash << SPR_PIPE_A3.commit_reg();
  /*p33.VARE*/ hash << SPR_PIPE_A4.commit_reg();
  /*p33.WEBA*/ hash << SPR_PIPE_A5.commit_reg();
  /*p33.VANU*/ hash << SPR_PIPE_A6.commit_reg();
  /*p33.VUPY*/ hash << SPR_PIPE_A7.commit_reg();
  /*p34.RUGO*/ hash << PAL_PIPE_0.commit_reg();
  /*p34.SATA*/ hash << PAL_PIPE_1.commit_reg();
  /*p34.ROSA*/ hash << PAL_PIPE_2.commit_reg();
  /*p34.SOMY*/ hash << PAL_PIPE_3.commit_reg();
  /*p34.PALU*/ hash << PAL_PIPE_4.commit_reg();
  /*p34.NUKE*/ hash << PAL_PIPE_5.commit_reg();
  /*p34.MODA*/ hash << PAL_PIPE_6.commit_reg();
  /*p34.LYME*/ hash << PAL_PIPE_7.commit_reg();
  /*p26.VEZO*/ hash << MASK_PIPE_0.commit_reg();
  /*p26.WURU*/ hash << MASK_PIPE_1.commit_reg();
  /*p26.VOSA*/ hash << MASK_PIPE_2.commit_reg();
  /*p26.WYFU*/ hash << MASK_PIPE_3.commit_reg();
  /*p26.XETE*/ hash << MASK_PIPE_4.commit_reg();
  /*p26.WODA*/ hash << MASK_PIPE_5.commit_reg();
  /*p26.VUMO*/ hash << MASK_PIPE_6.commit_reg();
  /*p26.VAVA*/ hash << MASK_PIPE_7.commit_reg();

  /*p36.PAVO*/ hash << BGP0.commit_reg();
  /*p36.NUSY*/ hash << BGP1.commit_reg();
  /*p36.PYLU*/ hash << BGP2.commit_reg();
  /*p36.MAXY*/ hash << BGP3.commit_reg();
  /*p36.MUKE*/ hash << BGP4.commit_reg();
  /*p36.MORU*/ hash << BGP5.commit_reg();
  /*p36.MOGY*/ hash << BGP6.commit_reg();
  /*p36.MENA*/ hash << BGP7.commit_reg();
  /*p36.XUFU*/ hash << OBP00.commit_reg();
  /*p36.XUKY*/ hash << OBP01.commit_reg();
  /*p36.XOVA*/ hash << OBP02.commit_reg();
  /*p36.XALO*/ hash << OBP03.commit_reg();
  /*p36.XERU*/ hash << OBP04.commit_reg();
  /*p36.XYZE*/ hash << OBP05.commit_reg();
  /*p36.XUPO*/ hash << OBP06.commit_reg();
  /*p36.XANA*/ hash << OBP07.commit_reg();
  /*p36.MOXY*/ hash << OBP10.commit_reg();
  /*p36.LAWO*/ hash << OBP11.commit_reg();
  /*p36.MOSA*/ hash << OBP12.commit_reg();
  /*p36.LOSE*/ hash << OBP13.commit_reg();
  /*p36.LUNE*/ hash << OBP14.commit_reg();
  /*p36.LUGU*/ hash << OBP15.commit_reg();
  /*p36.LEPU*/ hash << OBP16.commit_reg();
  /*p36.LUXO*/ hash << OBP17.commit_reg();

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