#include "PixelPipe.h"
//#include <stdint.h>

typedef unsigned short uint16_t;

void PixelPipe::reset() {
  BGP.reset();
  OBP0.reset();
  OBP1.reset();
}

void PixelPipe::tick(Gameboy& gb, bool CPU_RD, bool CPU_WR) {
  (void)gb;
  uint16_t addr = gb.bus.A;

  if (CPU_RD && addr == 0xFF47) gb.bus.D = BGP;
  if (CPU_RD && addr == 0xFF48) gb.bus.D = OBP0;
  if (CPU_RD && addr == 0xFF49) gb.bus.D = OBP1;
}

void PixelPipe::tock(Gameboy& gb, bool CPU_RD, bool CPU_WR) {
  (void)gb;
  uint16_t addr = gb.bus.A;

  bool RENDERING;
  bool BG_SEQ_x1x3x5x7;
  bool BG_SEQ_01xx45xx;
  bool BG_SEQ_x1x3x5x7;
  bool BG_SEQ_xx23xx67;
  bool BG_SEQ_0123xxxx;
  bool BG_SEQ_x1x3x5x7_DELAY;

  wire VRAM_A_CLK = _nand(RENDERING, BG_SEQ_x1x3x5x7, BG_SEQ_xx23xx67, BG_SEQ_0123xxxx, _not(BG_SEQ_x1x3x5x7_DELAY));
  wire VRAM_B_CLK = _nand(RENDERING, BG_SEQ_x1x3x5x7, BG_SEQ_01xx45xx, _not(BG_SEQ_x1x3x5x7_DELAY));
  VRAM_A.tock(VRAM_A_CLK, 1, gb.vbus.D);
  VRAM_B.tock(VRAM_B_CLK, 1, gb.vbus.D);

  {

#if 0

    if (BG_SEQ_RST) {
      BG_PIPE_A.load(VRAM_A);
      BG_PIPE_B.load(VRAM_B);
    }
    else {
      BG_PIPE_A.shift(CLKPIPE);
      BG_PIPE_B.shift(CLKPIPE);
    }

    //----------
    // Sprite pix & mask

    SPRITE_PIX_LATCH_Ao = and(TULY1,      b.spr.SYCU);
    SPRITE_PIX_LATCH_Bo = and(SPRITE_ABn, b.spr.SYCU);

    wire FLIP_X = and(!b.spr.OAM_A_D5, b.spr.SPRITE_READn);

    uint8_t flipped = FLIP_X ? flip(b.MD) : b.MD;

    SPR_PIX_A0 = latch_pos(b.spr.SPRITE_PIX_LATCH_A, flipped);
    SPR_PIX_B0 = latch_pos(b.spr.SPRITE_PIX_LATCH_B, flipped);
    SPR_MASK0  = or(b.spr.XEFY, SPR_PIX_A_0, SPR_PIX_B_0);

    // XEFY is "SPR_PIPE_LOADn"

    SPR_PAL_PIPE_SET    = nand(SPRITE_MASK0, OAM_A_D4);
    SPR_MSK_PIPE_SET    = nand(SPRITE_MASK0, OAM_A_D7);
    SPR_PIX_PIPE_A_SET0 = nand(SPRITE_MASK0, SPR_PIX_A0);
    SPR_PIX_PIPE_B_SET0 = nand(SPRITE_MASK0, SPR_PIX_B0);

    SPR_PIX_PIPE_A = srtock_pos(CLKPIPE);
    SPR_PIX_PIPE_B = srtock_pos(CLKPIPE);
    SPR_PAL_PIPE   = srtock_pos(CLKPIPE);
    SPR_MASK_PIPE  = srtock_pos(CLKPIPE);
#endif
  }

#if 0
  /*p35.RAJY*/ PIX_BG0    = and(b.vid.LCDC_BGEN, BG_PIPE_A7);
  /*p35.TADE*/ PIX_BG1    = and(b.vid.LCDC_BGEN, BG_PIPE_B7);
  /*p35.WOXA*/ PIX_SP0    = and(b.vid.LCDC_SPEN, SPR_PIX_A_7);
  /*p35.XULA*/ PIX_SP1    = and(b.vid.LCDC_SPEN, SPR_PIX_B_7);

  /*p35.POKA*/ MASK_BGP   = nor(nor(PIX_SP0, PIX_SP1),
                                and(PIX_BG0, MASK_PIPE_7),
                                and(PIX_BG1, MASK_PIPE_7));
  
  /*p35.LAVA*/ MASK_OPB0  = and(!SPRITE_PAL_PIPE_7, !MASK_BGP);
  /*p35.LUKU*/ MASK_OBP1  = and(!SPRITE_PAL_PIPE_7, !MASK_BGP);

  //----------
  // Sprite palette 0 lookup

  /*p35.VYRO*/ PAL_OBP0D = and( PIX_SP0,  PIX_SP1, MASK_OPB0);
  /*p35.VATA*/ PAL_OBP0C = and( PIX_SP0, !PIX_SP1, MASK_OPB0);
  /*p35.VOLO*/ PAL_OBP0B = and(!PIX_SP0,  PIX_SP1, MASK_OPB0);
  /*p35.VUGO*/ PAL_OBP0A = and(!PIX_SP0, !PIX_SP1, MASK_OPB0);

  /*p35.LEDO*/ PAL_OBP0D = and( PIX_SP1,  PIX_SP0, MASK_OBP1);
  /*p35.LYKY*/ PAL_OBP0C = and( PIX_SP1, !PIX_SP0, MASK_OBP1);
  /*p35.LARU*/ PAL_OBP0B = and(!PIX_SP1,  PIX_SP0, MASK_OBP1);
  /*p35.LOPU*/ PAL_OBP0A = and(!PIX_SP1, !PIX_SP0, MASK_OBP1);

  /*p35.POBU*/ PAL_BGPA  = and(!PIX_BG1, !PIX_BG0, MASK_BGP);
  /*p35.NUMA*/ PAL_BGPB  = and( PIX_BG1, !PIX_BG0, MASK_BGP);
  /*p35.NUXO*/ PAL_BGPC  = and(!PIX_BG1,  PIX_BG0, MASK_BGP);
  /*p35.NYPO*/ PAL_BGPD  = and( PIX_BG1,  PIX_BG0, MASK_BGP);

  /*p35.WUFU*/ COL_OBP00 = amux4(OBP0_D7, PAL_OBP0D, OBP0_D5, PAL_OBP0C, OBP0_D3, PAL_OBP0B, OBP0_D1, PAL_OBP0A);
  /*p35.WALY*/ COL_OBP01 = amux4(OBP0_D6, PAL_OBP0D, OBP0_D4, PAL_OBP0C, OBP0_D2, PAL_OBP0B, OBP0_D0, PAL_OBP0A);

  /*p35.MOKA*/ COL_OBP10 = amux4(OBP1_D7, PAL_OBP0D, OBP1_D5, PAL_OBP0C, OBP1_D3, PAL_OBP0B, OBP1_D1, PAL_OBP0A);
  /*p35.MUFA*/ COL_OBP11 = amux4(OBP1_D6, PAL_OBP0D, OBP1_D4, PAL_OBP0C, OBP1_D2, PAL_OBP0B, OBP1_D0, PAL_OBP0A);

  /*p35.NURA*/ COL_BGP1  = amux4(BGP_D7, PAL_BGPD, BGP_D5, PAL_BGPC, BGP_D3, PAL_BGPB, BGP_D1, PAL_BGPA);
  /*p35.NELO*/ COL_BGP0  = amux4(BGP_D6, PAL_BGPD, BGP_D4, PAL_BGPC, BGP_D2, PAL_BGPB, BGP_D0, PAL_BGPA);

  c.chip_out.LD0 = or(COL_BGP1, COL_OBP00, COL_OBP10);
  c.chip_out.LD1 = or(COL_BGP0, COL_OBP01, COL_OBP11);
#endif

  //----------
  // Palettes

  BGP.tock (_nand(CPU_WR, addr == 0xFF47), 0, gb.bus.D);
  OBP0.tock(_nand(CPU_WR, addr == 0xFF48), 0, gb.bus.D);
  OBP1.tock(_nand(CPU_WR, addr == 0xFF49), 0, gb.bus.D);
}
