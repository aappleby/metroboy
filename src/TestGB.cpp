#include "TestGB.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

void TestGB::ext_preset() {
  ext_bus.PIN_WR_C.preset(true, 0);   // -> P07.UBAL
  ext_bus.PIN_RD_C.preset(true, 0);   // -> P07.UJYV
  ext_bus.PIN_A00_C.preset(true, 0);   // -> P08.KOVA
  ext_bus.PIN_A01_C.preset(true, 0);   // -> P08.CAMU
  ext_bus.PIN_A02_C.preset(true, 0);   // -> P08.BUXU
  ext_bus.PIN_A03_C.preset(true, 0);   // -> P08.BASE
  ext_bus.PIN_A04_C.preset(true, 0);   // -> P08.AFEC
  ext_bus.PIN_A05_C.preset(true, 0);   // -> P08.ABUP
  ext_bus.PIN_A06_C.preset(true, 0);   // -> P08.CYGU
  ext_bus.PIN_A07_C.preset(true, 0);   // -> P08.COGO
  ext_bus.PIN_A08_C.preset(true, 0);   // -> P08.MUJY
  ext_bus.PIN_A09_C.preset(true, 0);   // -> P08.NENA
  ext_bus.PIN_A10_C.preset(true, 0);   // -> P08.SURA
  ext_bus.PIN_A11_C.preset(true, 0);   // -> P08.MADY
  ext_bus.PIN_A12_C.preset(true, 0);   // -> P08.LAHE
  ext_bus.PIN_A13_C.preset(true, 0);   // -> P08.LURA
  ext_bus.PIN_A14_C.preset(true, 0);   // -> P08.PEVO
  ext_bus.PIN_A15_C.preset(true, 0);   // -> P08.RAZA

  if (!ext_bus.PIN_RD_A && !ext_bus.PIN_CS_A) {
    uint16_t ext_addr = (uint16_t)pack(ext_bus.PIN_A00_A, ext_bus.PIN_A01_A, ext_bus.PIN_A02_A, ext_bus.PIN_A03_A,
                                       ext_bus.PIN_A04_A, ext_bus.PIN_A05_A, ext_bus.PIN_A06_A, ext_bus.PIN_A07_A,
                                       ext_bus.PIN_A08_A, ext_bus.PIN_A09_A, ext_bus.PIN_A10_A, ext_bus.PIN_A11_A,
                                       ext_bus.PIN_A12_A, ext_bus.PIN_A13_A, ext_bus.PIN_A14_A, ext_bus.PIN_A15_A);

    if (0x0000 <= ext_addr && ext_addr <= 0x7FFF) {
      // Cart rom
      //uint8_t d = rom[ext_addr];
      uint8_t d = 0;
      ext_bus.PIN_D0_C.preset(true, d & 0x01);
      ext_bus.PIN_D1_C.preset(true, d & 0x02);
      ext_bus.PIN_D2_C.preset(true, d & 0x04);
      ext_bus.PIN_D3_C.preset(true, d & 0x08);
      ext_bus.PIN_D4_C.preset(true, d & 0x10);
      ext_bus.PIN_D5_C.preset(true, d & 0x20);
      ext_bus.PIN_D6_C.preset(true, d & 0x40);
      ext_bus.PIN_D7_C.preset(true, d & 0x80);
    }
    else if (0xC000 <= ext_addr && ext_addr <= 0xDFFF) {
      // Main ram
      //uint8_t d = ram[ext_addr - 0xC000];
      uint8_t d = 0;
      ext_bus.PIN_D0_C.preset(true, d & 0x01);
      ext_bus.PIN_D1_C.preset(true, d & 0x02);
      ext_bus.PIN_D2_C.preset(true, d & 0x04);
      ext_bus.PIN_D3_C.preset(true, d & 0x08);
      ext_bus.PIN_D4_C.preset(true, d & 0x10);
      ext_bus.PIN_D5_C.preset(true, d & 0x20);
      ext_bus.PIN_D6_C.preset(true, d & 0x40);
      ext_bus.PIN_D7_C.preset(true, d & 0x80);
    }
    else if (0xE000 <= ext_addr && ext_addr <= 0xFFFF) {
      // Echo ram
      //uint8_t d = ram[ext_addr - 0xE000];
      uint8_t d = 0;
      ext_bus.PIN_D0_C.preset(true, d & 0x01);
      ext_bus.PIN_D1_C.preset(true, d & 0x02);
      ext_bus.PIN_D2_C.preset(true, d & 0x04);
      ext_bus.PIN_D3_C.preset(true, d & 0x08);
      ext_bus.PIN_D4_C.preset(true, d & 0x10);
      ext_bus.PIN_D5_C.preset(true, d & 0x20);
      ext_bus.PIN_D6_C.preset(true, d & 0x40);
      ext_bus.PIN_D7_C.preset(true, d & 0x80);
    }
    else {
      printf("Bad address?\n");
      __debugbreak();
    }
  }
  else {
    ext_bus.PIN_D0_C.preset(false, 0);
    ext_bus.PIN_D1_C.preset(false, 0);
    ext_bus.PIN_D2_C.preset(false, 0);
    ext_bus.PIN_D3_C.preset(false, 0);
    ext_bus.PIN_D4_C.preset(false, 0);
    ext_bus.PIN_D5_C.preset(false, 0);
    ext_bus.PIN_D6_C.preset(false, 0);
    ext_bus.PIN_D7_C.preset(false, 0);
  }
}

//-----------------------------------------------------------------------------

bool TestGB::commit_everything() {
  bool changed = false;

  /*p23.VYXE*/ changed |= cfg_reg.LCDC_BGEN.commit_reg();
  /*p23.XYLO*/ changed |= cfg_reg.LCDC_SPEN.commit_reg();
  /*p23.XYMO*/ changed |= cfg_reg.LCDC_SPSIZE.commit_reg();
  /*p23.XAFO*/ changed |= cfg_reg.LCDC_BGMAP.commit_reg();
  /*p23.WEXU*/ changed |= cfg_reg.LCDC_BGTILE.commit_reg();
  /*p23.WYMO*/ changed |= cfg_reg.LCDC_WINEN.commit_reg();
  /*p23.WOKY*/ changed |= cfg_reg.LCDC_WINMAP.commit_reg();
  /*p23.XONA*/ changed |= cfg_reg.XONA_LCDC_EN.commit_reg();
  /*p23.GAVE*/ changed |= cfg_reg.SCY0.commit_reg();
  /*p23.FYMO*/ changed |= cfg_reg.SCY1.commit_reg();
  /*p23.FEZU*/ changed |= cfg_reg.SCY2.commit_reg();
  /*p23.FUJO*/ changed |= cfg_reg.SCY3.commit_reg();
  /*p23.DEDE*/ changed |= cfg_reg.SCY4.commit_reg();
  /*p23.FOTY*/ changed |= cfg_reg.SCY5.commit_reg();
  /*p23.FOHA*/ changed |= cfg_reg.SCY6.commit_reg();
  /*p23.FUNY*/ changed |= cfg_reg.SCY7.commit_reg();
  /*p23.DATY*/ changed |= cfg_reg.SCX0.commit_reg();
  /*p23.DUZU*/ changed |= cfg_reg.SCX1.commit_reg();
  /*p23.CYXU*/ changed |= cfg_reg.SCX2.commit_reg();
  /*p23.GUBO*/ changed |= cfg_reg.SCX3.commit_reg();
  /*p23.BEMY*/ changed |= cfg_reg.SCX4.commit_reg();
  /*p23.CUZY*/ changed |= cfg_reg.SCX5.commit_reg();
  /*p23.CABU*/ changed |= cfg_reg.SCX6.commit_reg();
  /*p23.BAKE*/ changed |= cfg_reg.SCX7.commit_reg();
  /*p36.PAVO*/ changed |= cfg_reg.BGP0.commit_reg();
  /*p36.NUSY*/ changed |= cfg_reg.BGP1.commit_reg();
  /*p36.PYLU*/ changed |= cfg_reg.BGP2.commit_reg();
  /*p36.MAXY*/ changed |= cfg_reg.BGP3.commit_reg();
  /*p36.MUKE*/ changed |= cfg_reg.BGP4.commit_reg();
  /*p36.MORU*/ changed |= cfg_reg.BGP5.commit_reg();
  /*p36.MOGY*/ changed |= cfg_reg.BGP6.commit_reg();
  /*p36.MENA*/ changed |= cfg_reg.BGP7.commit_reg();
  /*p36.XUFU*/ changed |= cfg_reg.OBP00.commit_reg();
  /*p36.XUKY*/ changed |= cfg_reg.OBP01.commit_reg();
  /*p36.XOVA*/ changed |= cfg_reg.OBP02.commit_reg();
  /*p36.XALO*/ changed |= cfg_reg.OBP03.commit_reg();
  /*p36.XERU*/ changed |= cfg_reg.OBP04.commit_reg();
  /*p36.XYZE*/ changed |= cfg_reg.OBP05.commit_reg();
  /*p36.XUPO*/ changed |= cfg_reg.OBP06.commit_reg();
  /*p36.XANA*/ changed |= cfg_reg.OBP07.commit_reg();
  /*p36.MOXY*/ changed |= cfg_reg.OBP10.commit_reg();
  /*p36.LAWO*/ changed |= cfg_reg.OBP11.commit_reg();
  /*p36.MOSA*/ changed |= cfg_reg.OBP12.commit_reg();
  /*p36.LOSE*/ changed |= cfg_reg.OBP13.commit_reg();
  /*p36.LUNE*/ changed |= cfg_reg.OBP14.commit_reg();
  /*p36.LUGU*/ changed |= cfg_reg.OBP15.commit_reg();
  /*p36.LEPU*/ changed |= cfg_reg.OBP16.commit_reg();
  /*p36.LUXO*/ changed |= cfg_reg.OBP17.commit_reg();
  /*p23.NESO*/ changed |= cfg_reg.WY0.commit_reg();
  /*p23.NYRO*/ changed |= cfg_reg.WY1.commit_reg();
  /*p23.NAGA*/ changed |= cfg_reg.WY2.commit_reg();
  /*p23.MELA*/ changed |= cfg_reg.WY3.commit_reg();
  /*p23.NULO*/ changed |= cfg_reg.WY4.commit_reg();
  /*p23.NENE*/ changed |= cfg_reg.WY5.commit_reg();
  /*p23.NUKA*/ changed |= cfg_reg.WY6.commit_reg();
  /*p23.NAFU*/ changed |= cfg_reg.WY7.commit_reg();
  /*p23.MYPA*/ changed |= cfg_reg.WX0.commit_reg();
  /*p23.NOFE*/ changed |= cfg_reg.WX1.commit_reg();
  /*p23.NOKE*/ changed |= cfg_reg.WX2.commit_reg();
  /*p23.MEBY*/ changed |= cfg_reg.WX3.commit_reg();
  /*p23.MYPU*/ changed |= cfg_reg.WX4.commit_reg();
  /*p23.MYCE*/ changed |= cfg_reg.WX5.commit_reg();
  /*p23.MUVO*/ changed |= cfg_reg.WX6.commit_reg();
  /*p23.NUKU*/ changed |= cfg_reg.WX7.commit_reg();

  /*p07.BURO*/ changed |= dbg_reg.FF60_0.commit_reg();
  /*p07.AMUT*/ changed |= dbg_reg.FF60_1.commit_reg();

  /*p04.MATU*/ changed |= dma_reg.MATU_DMA_OAM_WRp.commit_reg(); // -> p25,p28
  /*p04.MYTE*/ changed |= dma_reg.MYTE_DMA_DONE.commit_reg();
  /*p04.LUVY*/ changed |= dma_reg.LUVY_DMA_TRIG_d0.commit_reg();
  /*p04.LENE*/ changed |= dma_reg.LENE_DMA_TRIG_d4.commit_reg();
  /*p04.NAKY*/ changed |= dma_reg.DMA_A00.commit_reg();
  /*p04.PYRO*/ changed |= dma_reg.DMA_A01.commit_reg(); 
  /*p04.NEFY*/ changed |= dma_reg.DMA_A02.commit_reg(); 
  /*p04.MUTY*/ changed |= dma_reg.DMA_A03.commit_reg(); 
  /*p04.NYKO*/ changed |= dma_reg.DMA_A04.commit_reg(); 
  /*p04.PYLO*/ changed |= dma_reg.DMA_A05.commit_reg(); 
  /*p04.NUTO*/ changed |= dma_reg.DMA_A06.commit_reg(); 
  /*p04.MUGU*/ changed |= dma_reg.DMA_A07.commit_reg(); 
  /*p04.NAFA*/ changed |= dma_reg.DMA_A08.commit_reg(); 
  /*p04.PYNE*/ changed |= dma_reg.DMA_A09.commit_reg(); 
  /*p04.PARA*/ changed |= dma_reg.DMA_A10.commit_reg(); 
  /*p04.NYDO*/ changed |= dma_reg.DMA_A11.commit_reg(); 
  /*p04.NYGY*/ changed |= dma_reg.DMA_A12.commit_reg(); 
  /*p04.PULA*/ changed |= dma_reg.DMA_A13.commit_reg(); 
  /*p04.POKU*/ changed |= dma_reg.DMA_A14.commit_reg(); 
  /*p04.MARU*/ changed |= dma_reg.DMA_A15.commit_reg(); 
  /*p04.LYXE*/ changed |= dma_reg.LYXE_DMA_LATCHn.commit_latch();

  // NAND latch
  /*p04.LARA*/ changed |= dma_reg.LARA_DMA_LATCHn.commit_gate();
  /*p04.LOKY*/ changed |= dma_reg.LOKY_DMA_LATCHp.commit_gate();

  /*p02.LOPE*/ changed |= int_reg.LOPE_FF0F_0.commit_reg();
  /*p02.UBUL*/ changed |= int_reg.UBUL_FF0F_3.commit_reg();
  /*p02.ULAK*/ changed |= int_reg.ULAK_FF0F_4.commit_reg();
  /*p02.LALU*/ changed |= int_reg.LALU_FF0F_1.commit_reg();
  /*p02.NYBO*/ changed |= int_reg.NYBO_FF0F_2.commit_reg();
  /*p02.MATY*/ changed |= int_reg.FF0F_L0.commit_latch();
  /*p02.NEJY*/ changed |= int_reg.FF0F_L1.commit_latch();
  /*p02.NUTY*/ changed |= int_reg.FF0F_L2.commit_latch();
  /*p02.MOPO*/ changed |= int_reg.FF0F_L3.commit_latch();
  /*p02.PAVY*/ changed |= int_reg.FF0F_L4.commit_latch();

  /*p02.BATU*/ changed |= joy_reg.JP_GLITCH0.commit_reg();
  /*p02.ACEF*/ changed |= joy_reg.JP_GLITCH1.commit_reg();
  /*p02.AGEM*/ changed |= joy_reg.JP_GLITCH2.commit_reg();
  /*p02.APUG*/ changed |= joy_reg.JP_GLITCH3.commit_reg();
  /*p05.JUTE*/ changed |= joy_reg.JOYP_RA.commit_reg();
  /*p05.KECY*/ changed |= joy_reg.JOYP_LB.commit_reg();
  /*p05.JALE*/ changed |= joy_reg.JOYP_UC.commit_reg();
  /*p05.KYME*/ changed |= joy_reg.JOYP_DS.commit_reg();
  /*p05.KELY*/ changed |= joy_reg.JOYP_UDLR.commit_reg();
  /*p05.COFY*/ changed |= joy_reg.JOYP_ABCS.commit_reg();
  /*p05.KUKO*/ changed |= joy_reg.DBG_FF00_D6.commit_reg();
  /*p05.KERU*/ changed |= joy_reg.DBG_FF00_D7.commit_reg();
  /*p05.KEVU*/ changed |= joy_reg.JOYP_L0.commit_reg();
  /*p05.KAPA*/ changed |= joy_reg.JOYP_L1.commit_reg();
  /*p05.KEJA*/ changed |= joy_reg.JOYP_L2.commit_reg();
  /*p05.KOLO*/ changed |= joy_reg.JOYP_L3.commit_reg();
  /*p02.AWOB*/ changed |= joy_reg.WAKE_CPU.commit_reg();

  changed |= lcd_reg.commit();

  /*p31.XYKY*/ changed |= oam_reg.LATCH_OAM_A0.commit_latch();
  /*p31.YRUM*/ changed |= oam_reg.LATCH_OAM_A1.commit_latch();
  /*p31.YSEX*/ changed |= oam_reg.LATCH_OAM_A2.commit_latch();
  /*p31.YVEL*/ changed |= oam_reg.LATCH_OAM_A3.commit_latch();
  /*p31.WYNO*/ changed |= oam_reg.LATCH_OAM_A4.commit_latch();
  /*p31.CYRA*/ changed |= oam_reg.LATCH_OAM_A5.commit_latch();
  /*p31.ZUVE*/ changed |= oam_reg.LATCH_OAM_A6.commit_latch();
  /*p31.ECED*/ changed |= oam_reg.LATCH_OAM_A7.commit_latch();
  /*p29.YDYV*/ changed |= oam_reg.LATCH_OAM_B0.commit_latch();
  /*p29.YCEB*/ changed |= oam_reg.LATCH_OAM_B1.commit_latch();
  /*p29.ZUCA*/ changed |= oam_reg.LATCH_OAM_B2.commit_latch();
  /*p29.WONE*/ changed |= oam_reg.LATCH_OAM_B3.commit_latch();
  /*p29.ZAXE*/ changed |= oam_reg.LATCH_OAM_B4.commit_latch();
  /*p29.XAFU*/ changed |= oam_reg.LATCH_OAM_B5.commit_latch();
  /*p29.YSES*/ changed |= oam_reg.LATCH_OAM_B6.commit_latch();
  /*p29.ZECA*/ changed |= oam_reg.LATCH_OAM_B7.commit_latch();
  /*p31.YLOR*/ changed |= oam_reg.YLOR_SPRITE_X0.commit_reg();
  /*p31.ZYTY*/ changed |= oam_reg.ZYTY_SPRITE_X1.commit_reg();
  /*p31.ZYVE*/ changed |= oam_reg.ZYVE_SPRITE_X2.commit_reg();
  /*p31.ZEZY*/ changed |= oam_reg.ZEZY_SPRITE_X3.commit_reg();
  /*p31.GOMO*/ changed |= oam_reg.GOMO_SPRITE_X4.commit_reg();
  /*p31.BAXO*/ changed |= oam_reg.BAXO_SPRITE_X5.commit_reg();
  /*p31.YZOS*/ changed |= oam_reg.YZOS_SPRITE_X6.commit_reg();
  /*p31.DEPO*/ changed |= oam_reg.DEPO_SPRITE_X7.commit_reg();
  /*p29.XUSO*/ changed |= oam_reg.XUSO_SPRITE_Y0.commit_reg();
  /*p29.XEGU*/ changed |= oam_reg.XEGU_SPRITE_Y1.commit_reg();
  /*p29.YJEX*/ changed |= oam_reg.YJEX_SPRITE_Y2.commit_reg();
  /*p29.XYJU*/ changed |= oam_reg.XYJU_SPRITE_Y3.commit_reg();
  /*p29.YBOG*/ changed |= oam_reg.YBOG_SPRITE_Y4.commit_reg();
  /*p29.WYSO*/ changed |= oam_reg.WYSO_SPRITE_Y5.commit_reg();
  /*p29.XOTE*/ changed |= oam_reg.XOTE_SPRITE_Y6.commit_reg();
  /*p29.YZAB*/ changed |= oam_reg.YZAB_SPRITE_Y7.commit_reg();

  /*p32.LEGU*/ changed |= pxp_reg.BG_PIX_A0.commit_reg();
  /*p32.NUDU*/ changed |= pxp_reg.BG_PIX_A1.commit_reg();
  /*p32.MUKU*/ changed |= pxp_reg.BG_PIX_A2.commit_reg();
  /*p32.LUZO*/ changed |= pxp_reg.BG_PIX_A3.commit_reg();
  /*p32.MEGU*/ changed |= pxp_reg.BG_PIX_A4.commit_reg();
  /*p32.MYJY*/ changed |= pxp_reg.BG_PIX_A5.commit_reg();
  /*p32.NASA*/ changed |= pxp_reg.BG_PIX_A6.commit_reg();
  /*p32.NEFO*/ changed |= pxp_reg.BG_PIX_A7.commit_reg();
  /*p32.RAWU*/ changed |= pxp_reg.BG_PIX_B0.commit_reg();
  /*p32.POZO*/ changed |= pxp_reg.BG_PIX_B1.commit_reg();
  /*p32.PYZO*/ changed |= pxp_reg.BG_PIX_B2.commit_reg();
  /*p32.POXA*/ changed |= pxp_reg.BG_PIX_B3.commit_reg();
  /*p32.PULO*/ changed |= pxp_reg.BG_PIX_B4.commit_reg();
  /*p32.POJU*/ changed |= pxp_reg.BG_PIX_B5.commit_reg();
  /*p32.POWY*/ changed |= pxp_reg.BG_PIX_B6.commit_reg();
  /*p32.PYJU*/ changed |= pxp_reg.BG_PIX_B7.commit_reg();
  /*p33.PEFO*/ changed |= pxp_reg.SPR_PIX_A0.commit_reg();
  /*p33.ROKA*/ changed |= pxp_reg.SPR_PIX_A1.commit_reg();
  /*p33.MYTU*/ changed |= pxp_reg.SPR_PIX_A2.commit_reg();
  /*p33.RAMU*/ changed |= pxp_reg.SPR_PIX_A3.commit_reg();
  /*p33.SELE*/ changed |= pxp_reg.SPR_PIX_A4.commit_reg();
  /*p33.SUTO*/ changed |= pxp_reg.SPR_PIX_A5.commit_reg();
  /*p33.RAMA*/ changed |= pxp_reg.SPR_PIX_A6.commit_reg();
  /*p33.RYDU*/ changed |= pxp_reg.SPR_PIX_A7.commit_reg();
  /*p33.REWO*/ changed |= pxp_reg.SPR_PIX_B0.commit_reg();
  /*p33.PEBA*/ changed |= pxp_reg.SPR_PIX_B1.commit_reg();
  /*p33.MOFO*/ changed |= pxp_reg.SPR_PIX_B2.commit_reg();
  /*p33.PUDU*/ changed |= pxp_reg.SPR_PIX_B3.commit_reg();
  /*p33.SAJA*/ changed |= pxp_reg.SPR_PIX_B4.commit_reg();
  /*p33.SUNY*/ changed |= pxp_reg.SPR_PIX_B5.commit_reg();
  /*p33.SEMO*/ changed |= pxp_reg.SPR_PIX_B6.commit_reg();
  /*p33.SEGA*/ changed |= pxp_reg.SPR_PIX_B7.commit_reg();
  /*p32.MYDE*/ changed |= pxp_reg.BG_PIPE_A0.commit_reg();
  /*p32.NOZO*/ changed |= pxp_reg.BG_PIPE_A1.commit_reg();
  /*p32.MOJU*/ changed |= pxp_reg.BG_PIPE_A2.commit_reg();
  /*p32.MACU*/ changed |= pxp_reg.BG_PIPE_A3.commit_reg();
  /*p32.NEPO*/ changed |= pxp_reg.BG_PIPE_A4.commit_reg();
  /*p32.MODU*/ changed |= pxp_reg.BG_PIPE_A5.commit_reg();
  /*p32.NEDA*/ changed |= pxp_reg.BG_PIPE_A6.commit_reg();
  /*p32.PYBO*/ changed |= pxp_reg.BG_PIPE_A7.commit_reg();
  /*p32.TOMY*/ changed |= pxp_reg.BG_PIPE_B0.commit_reg();
  /*p32.TACA*/ changed |= pxp_reg.BG_PIPE_B1.commit_reg();
  /*p32.SADY*/ changed |= pxp_reg.BG_PIPE_B2.commit_reg();
  /*p32.RYSA*/ changed |= pxp_reg.BG_PIPE_B3.commit_reg();
  /*p32.SOBO*/ changed |= pxp_reg.BG_PIPE_B4.commit_reg();
  /*p32.SETU*/ changed |= pxp_reg.BG_PIPE_B5.commit_reg();
  /*p32.RALU*/ changed |= pxp_reg.BG_PIPE_B6.commit_reg();
  /*p32.SOHU*/ changed |= pxp_reg.BG_PIPE_B7.commit_reg();
  /*p33.NURO*/ changed |= pxp_reg.SPR_PIPE_B0.commit_reg();
  /*p33.MASO*/ changed |= pxp_reg.SPR_PIPE_B1.commit_reg();
  /*p33.LEFE*/ changed |= pxp_reg.SPR_PIPE_B2.commit_reg();
  /*p33.LESU*/ changed |= pxp_reg.SPR_PIPE_B3.commit_reg();
  /*p33.WYHO*/ changed |= pxp_reg.SPR_PIPE_B4.commit_reg();
  /*p33.WORA*/ changed |= pxp_reg.SPR_PIPE_B5.commit_reg();
  /*p33.VAFO*/ changed |= pxp_reg.SPR_PIPE_B6.commit_reg();
  /*p33.WUFY*/ changed |= pxp_reg.SPR_PIPE_B7.commit_reg();
  /*p33.NYLU*/ changed |= pxp_reg.SPR_PIPE_A0.commit_reg();
  /*p33.PEFU*/ changed |= pxp_reg.SPR_PIPE_A1.commit_reg();
  /*p33.NATY*/ changed |= pxp_reg.SPR_PIPE_A2.commit_reg();
  /*p33.PYJO*/ changed |= pxp_reg.SPR_PIPE_A3.commit_reg();
  /*p33.VARE*/ changed |= pxp_reg.SPR_PIPE_A4.commit_reg();
  /*p33.WEBA*/ changed |= pxp_reg.SPR_PIPE_A5.commit_reg();
  /*p33.VANU*/ changed |= pxp_reg.SPR_PIPE_A6.commit_reg();
  /*p33.VUPY*/ changed |= pxp_reg.SPR_PIPE_A7.commit_reg();
  /*p34.RUGO*/ changed |= pxp_reg.PAL_PIPE_0.commit_reg();
  /*p34.SATA*/ changed |= pxp_reg.PAL_PIPE_1.commit_reg();
  /*p34.ROSA*/ changed |= pxp_reg.PAL_PIPE_2.commit_reg();
  /*p34.SOMY*/ changed |= pxp_reg.PAL_PIPE_3.commit_reg();
  /*p34.PALU*/ changed |= pxp_reg.PAL_PIPE_4.commit_reg();
  /*p34.NUKE*/ changed |= pxp_reg.PAL_PIPE_5.commit_reg();
  /*p34.MODA*/ changed |= pxp_reg.PAL_PIPE_6.commit_reg();
  /*p34.LYME*/ changed |= pxp_reg.PAL_PIPE_7.commit_reg();
  /*p26.VEZO*/ changed |= pxp_reg.MASK_PIPE_0.commit_reg();
  /*p26.WURU*/ changed |= pxp_reg.MASK_PIPE_1.commit_reg();
  /*p26.VOSA*/ changed |= pxp_reg.MASK_PIPE_2.commit_reg();
  /*p26.WYFU*/ changed |= pxp_reg.MASK_PIPE_3.commit_reg();
  /*p26.XETE*/ changed |= pxp_reg.MASK_PIPE_4.commit_reg();
  /*p26.WODA*/ changed |= pxp_reg.MASK_PIPE_5.commit_reg();
  /*p26.VUMO*/ changed |= pxp_reg.MASK_PIPE_6.commit_reg();
  /*p26.VAVA*/ changed |= pxp_reg.MASK_PIPE_7.commit_reg();

  /*p29.TOXE*/ changed |= ppu_reg.TOXE_SFETCH_S0_D0.commit_reg();
  /*p29.TULY*/ changed |= ppu_reg.TULY_SFETCH_S1.commit_reg();
  /*p29.TESE*/ changed |= ppu_reg.TESE_SFETCH_S2.commit_reg();
  /*p29.TOBU*/ changed |= ppu_reg.TOBU_SFETCH_S1_D2.commit_reg();
  /*p29.VONU*/ changed |= ppu_reg.VONU_SFETCH_S1_D4.commit_reg();
  /*p29.SEBA*/ changed |= ppu_reg.SEBA_SFETCH_S1_D5.commit_reg();
  /*p29.TYFO*/ changed |= ppu_reg.TYFO_SFETCH_S0_D1.commit_reg();

  changed |= sst_reg.commit();

  /*p01.UKUP*/ changed |= tim_reg.UKUP_DIV_00.commit_reg();
  /*p01.UFOR*/ changed |= tim_reg.UFOR_DIV_01.commit_reg();
  /*p01.UNER*/ changed |= tim_reg.UNER_DIV_02.commit_reg();
  /*p01.TERO*/ changed |= tim_reg.TERO_DIV_03.commit_reg();
  /*p01.UNYK*/ changed |= tim_reg.UNYK_DIV_04.commit_reg();
  /*p01.TAMA*/ changed |= tim_reg.TAMA_DIV_05.commit_reg();
  /*p01.UGOT*/ changed |= tim_reg.UGOT_DIV_06.commit_reg();
  /*p01.TULU*/ changed |= tim_reg.TULU_DIV_07.commit_reg();
  /*p01.TUGO*/ changed |= tim_reg.TUGO_DIV_08.commit_reg();
  /*p01.TOFE*/ changed |= tim_reg.TOFE_DIV_09.commit_reg();
  /*p01.TERU*/ changed |= tim_reg.TERU_DIV_10.commit_reg();
  /*p01.SOLA*/ changed |= tim_reg.SOLA_DIV_11.commit_reg();
  /*p01.SUBU*/ changed |= tim_reg.SUBU_DIV_12.commit_reg();
  /*p01.TEKA*/ changed |= tim_reg.TEKA_DIV_13.commit_reg();
  /*p01.UKET*/ changed |= tim_reg.UKET_DIV_14.commit_reg();
  /*p01.UPOF*/ changed |= tim_reg.UPOF_DIV_15.commit_reg();
  /*p03.REGA*/ changed |= tim_reg.TIMA_0.commit_counter();
  /*p03.POVY*/ changed |= tim_reg.TIMA_1.commit_counter();
  /*p03.PERU*/ changed |= tim_reg.TIMA_2.commit_counter();
  /*p03.RATE*/ changed |= tim_reg.TIMA_3.commit_counter();
  /*p03.RUBY*/ changed |= tim_reg.TIMA_4.commit_counter();
  /*p03.RAGE*/ changed |= tim_reg.TIMA_5.commit_counter();
  /*p03.PEDA*/ changed |= tim_reg.TIMA_6.commit_counter();
  /*p03.NUGA*/ changed |= tim_reg.TIMA_7.commit_counter();
  /*p03.NYDU*/ changed |= tim_reg.TIMA_MAX.commit_reg();
  /*p03.MOBA*/ changed |= tim_reg.MOBA_INT_TIMER.commit_reg();
  /*p03.SABU*/ changed |= tim_reg.TMA_0.commit_reg();
  /*p03.NYKE*/ changed |= tim_reg.TMA_1.commit_reg();
  /*p03.MURU*/ changed |= tim_reg.TMA_2.commit_reg();
  /*p03.TYVA*/ changed |= tim_reg.TMA_3.commit_reg();
  /*p03.TYRU*/ changed |= tim_reg.TMA_4.commit_reg();
  /*p03.SUFY*/ changed |= tim_reg.TMA_5.commit_reg();
  /*p03.PETO*/ changed |= tim_reg.TMA_6.commit_reg();
  /*p03.SETA*/ changed |= tim_reg.TMA_7.commit_reg();
  /*p03.SOPU*/ changed |= tim_reg.TAC_0.commit_reg();
  /*p03.SAMY*/ changed |= tim_reg.TAC_1.commit_reg();
  /*p03.SABO*/ changed |= tim_reg.TAC_2.commit_reg();

  changed |= ppu_reg.commit();

  changed |= win_reg.commit();

  changed |= vram_bus.TS_MA00.commit_tribuf();
  changed |= vram_bus.TS_MA01.commit_tribuf();
  changed |= vram_bus.TS_MA02.commit_tribuf();
  changed |= vram_bus.TS_MA03.commit_tribuf();
  changed |= vram_bus.TS_MA04.commit_tribuf();
  changed |= vram_bus.TS_MA05.commit_tribuf();
  changed |= vram_bus.TS_MA06.commit_tribuf();
  changed |= vram_bus.TS_MA07.commit_tribuf();
  changed |= vram_bus.TS_MA08.commit_tribuf();
  changed |= vram_bus.TS_MA09.commit_tribuf();
  changed |= vram_bus.TS_MA10.commit_tribuf();
  changed |= vram_bus.TS_MA11.commit_tribuf();
  changed |= vram_bus.TS_MA12.commit_tribuf();
  changed |= vram_bus.TS_MD0.commit_tribuf();
  changed |= vram_bus.TS_MD1.commit_tribuf();
  changed |= vram_bus.TS_MD2.commit_tribuf();
  changed |= vram_bus.TS_MD3.commit_tribuf();
  changed |= vram_bus.TS_MD4.commit_tribuf();
  changed |= vram_bus.TS_MD5.commit_tribuf();
  changed |= vram_bus.TS_MD6.commit_tribuf();
  changed |= vram_bus.TS_MD7.commit_tribuf();

  /*p07.TEPU*/ changed |= bus_reg.BOOT_BITn.commit_reg();

  /*p08.ALOR*/ changed |= ext_bus.EXT_ADDR_LATCH_00.commit_latch();
  /*p08.APUR*/ changed |= ext_bus.EXT_ADDR_LATCH_01.commit_latch();
  /*p08.ALYR*/ changed |= ext_bus.EXT_ADDR_LATCH_02.commit_latch();
  /*p08.ARET*/ changed |= ext_bus.EXT_ADDR_LATCH_03.commit_latch();
  /*p08.AVYS*/ changed |= ext_bus.EXT_ADDR_LATCH_04.commit_latch();
  /*p08.ATEV*/ changed |= ext_bus.EXT_ADDR_LATCH_05.commit_latch();
  /*p08.AROS*/ changed |= ext_bus.EXT_ADDR_LATCH_06.commit_latch();
  /*p08.ARYM*/ changed |= ext_bus.EXT_ADDR_LATCH_07.commit_latch();
  /*p08.LUNO*/ changed |= ext_bus.EXT_ADDR_LATCH_08.commit_latch();
  /*p08.LYSA*/ changed |= ext_bus.EXT_ADDR_LATCH_09.commit_latch();
  /*p08.PATE*/ changed |= ext_bus.EXT_ADDR_LATCH_10.commit_latch();
  /*p08.LUMY*/ changed |= ext_bus.EXT_ADDR_LATCH_11.commit_latch();
  /*p08.LOBU*/ changed |= ext_bus.EXT_ADDR_LATCH_12.commit_latch();
  /*p08.LONU*/ changed |= ext_bus.EXT_ADDR_LATCH_13.commit_latch();
  /*p08.NYRE*/ changed |= ext_bus.EXT_ADDR_LATCH_14.commit_latch();

  /*p08.SOMA*/ changed |= ext_bus.EXT_DATA_LATCH_00.commit_latch();
  /*p08.RONY*/ changed |= ext_bus.EXT_DATA_LATCH_01.commit_latch();
  /*p08.RAXY*/ changed |= ext_bus.EXT_DATA_LATCH_02.commit_latch();
  /*p08.SELO*/ changed |= ext_bus.EXT_DATA_LATCH_03.commit_latch();
  /*p08.SODY*/ changed |= ext_bus.EXT_DATA_LATCH_04.commit_latch();
  /*p08.SAGO*/ changed |= ext_bus.EXT_DATA_LATCH_05.commit_latch();
  /*p08.RUPA*/ changed |= ext_bus.EXT_DATA_LATCH_06.commit_latch();
  /*p08.SAZY*/ changed |= ext_bus.EXT_DATA_LATCH_07.commit_latch();

  //----------------------------------------
  // PINS

  changed |= cpu_bus.PIN_TO_CPU2.commit_pinout();       // <- P02.AWOB
  changed |= cpu_bus.PIN_CPU_RAW_RD.clear_preset();     // PORTA_00: -> UJYV, LAGU, LAVO
  changed |= cpu_bus.PIN_CPU_RAW_WR.clear_preset();     // PORTA_01: -> AREV, LAGU.
  changed |= cpu_bus.PIN_READ_BOOTROM.commit_pinout();  // PORTA_04: <- TUTU
  changed |= cpu_bus.PIN_ADDR_VALID.clear_preset();     // PORTA_06: -> APAP, TEXO
  changed |= int_reg.PIN_ACK_VBLANK.clear_preset();     // PORTB_01: -> LETY, vblank int ack
  changed |= int_reg.PIN_INT_VBLANK.commit_pinout();    // PORTB_03: <- LOPE, vblank int
  changed |= int_reg.PIN_ACK_STAT.clear_preset();       // PORTB_05: -> LEJA, stat int ack
  changed |= int_reg.PIN_INT_STAT.commit_pinout();      // PORTB_07: <- LALU, stat int
  changed |= int_reg.PIN_ACK_TIMER.clear_preset();      // PORTB_09: -> LESA, timer int ack
  changed |= int_reg.PIN_INT_TIMER.commit_pinout();     // PORTB_11: <- NYBO, timer int
  changed |= int_reg.PIN_ACK_SERIAL.clear_preset();     // PORTB_13: -> LUFE, serial int ack
  changed |= int_reg.PIN_INT_SERIAL.commit_pinout();    // PORTB_15: <- UBUL, serial int
  changed |= int_reg.PIN_ACK_JOYPAD.clear_preset();     // PORTB_17: -> LAMO, joypad int ack
  changed |= int_reg.PIN_INT_JOYPAD.commit_pinout();    // PORTB_19: <- ULAK, joypad int
  changed |= cpu_bus.PIN_CLKREQ.clear_preset();         // PORTC_00: -> ABOL
  changed |= cpu_bus.PIN_AFER.commit_pinout();          // PORTC_01: <- AFER
  changed |= cpu_bus.PIN_RESET.commit_pinout();         // PORTC_02: <- PIN_RESET directly connected to the pad
  changed |= cpu_bus.PIN_CLK_GOOD.commit_pinout();      // PORTC_03: <- CLKIN_A
  changed |= cpu_bus.PIN_CPU_RESET.commit_pinout();     // PORTC_04: <- TABA
  changed |= cpu_bus.PIN_FROM_CPU6.clear_preset();      // PORTD_00: -> LEXY, doesn't do anything

  changed |= cpu_bus.PIN_BOWA_AxCDEFGH.commit_pinout(); // PORTD_01: <- BOWA
  changed |= cpu_bus.PIN_BEDO_xBxxxxxx.commit_pinout(); // PORTD_02: <- BEDO _____fgh

  changed |= cpu_bus.PIN_BEKO_xBCDExxx.commit_pinout(); // PORTD_03: <- BEKO ____efgh connection not indicated on P01
  changed |= cpu_bus.PIN_BUDE_AxxxxFGH.commit_pinout(); // PORTD_04: <- BUDE abcd____
  
  changed |= cpu_bus.PIN_BOLO_xBCDEFGx.commit_pinout(); // PORTD_05: <- BOLO
  changed |= cpu_bus.PIN_FROM_CPU5p.clear_preset();      // PORTD_05: -> FROM_CPU5
  changed |= cpu_bus.PIN_BUKE_ABxxxxxH.commit_pinout(); // PORTD_06: <- BUKE _____f__

  changed |= cpu_bus.PIN_BOMA_xBxxxxxx.commit_pinout(); // PORTD_07: <- BOMA _____fgh
  changed |= cpu_bus.PIN_BOGA_AxCDEFGH.commit_pinout(); // PORTD_08: <- BOGA abcde___

  changed |= cpu_bus.PIN_A00.clear_preset();
  changed |= cpu_bus.PIN_A01.clear_preset();
  changed |= cpu_bus.PIN_A02.clear_preset();
  changed |= cpu_bus.PIN_A03.clear_preset();
  changed |= cpu_bus.PIN_A04.clear_preset();
  changed |= cpu_bus.PIN_A05.clear_preset();
  changed |= cpu_bus.PIN_A06.clear_preset();
  changed |= cpu_bus.PIN_A07.clear_preset();
  changed |= cpu_bus.PIN_A08.clear_preset();
  changed |= cpu_bus.PIN_A09.clear_preset();
  changed |= cpu_bus.PIN_A10.clear_preset();
  changed |= cpu_bus.PIN_A11.clear_preset();
  changed |= cpu_bus.PIN_A12.clear_preset();
  changed |= cpu_bus.PIN_A13.clear_preset();
  changed |= cpu_bus.PIN_A14.clear_preset();
  changed |= cpu_bus.PIN_A15.clear_preset();
  changed |= cpu_bus.TS_D0.commit_tribuf();
  changed |= cpu_bus.TS_D1.commit_tribuf();
  changed |= cpu_bus.TS_D2.commit_tribuf();
  changed |= cpu_bus.TS_D3.commit_tribuf();
  changed |= cpu_bus.TS_D4.commit_tribuf();
  changed |= cpu_bus.TS_D5.commit_tribuf();
  changed |= cpu_bus.TS_D6.commit_tribuf();
  changed |= cpu_bus.TS_D7.commit_tribuf();

  changed |= oam_bus.PIN_CLK.commit_pinout();
  changed |= oam_bus.PIN_OE.commit_pinout();
  changed |= oam_bus.PIN_WR_A.commit_pinout(); // definitely write
  changed |= oam_bus.PIN_WR_B.commit_pinout(); // definitely write
  changed |= oam_bus.PIN_A0.commit_pinout();
  changed |= oam_bus.PIN_A1.commit_pinout();
  changed |= oam_bus.PIN_A2.commit_pinout();
  changed |= oam_bus.PIN_A3.commit_pinout();
  changed |= oam_bus.PIN_A4.commit_pinout();
  changed |= oam_bus.PIN_A5.commit_pinout();
  changed |= oam_bus.PIN_A6.commit_pinout();
  changed |= oam_bus.PIN_A7.commit_pinout();
  changed |= oam_bus.PIN_DA0.commit_tribuf();
  changed |= oam_bus.PIN_DA1.commit_tribuf();
  changed |= oam_bus.PIN_DA2.commit_tribuf();
  changed |= oam_bus.PIN_DA3.commit_tribuf();
  changed |= oam_bus.PIN_DA4.commit_tribuf();
  changed |= oam_bus.PIN_DA5.commit_tribuf();
  changed |= oam_bus.PIN_DA6.commit_tribuf();
  changed |= oam_bus.PIN_DA7.commit_tribuf();
  changed |= oam_bus.PIN_DB0.commit_tribuf();
  changed |= oam_bus.PIN_DB1.commit_tribuf();
  changed |= oam_bus.PIN_DB2.commit_tribuf();
  changed |= oam_bus.PIN_DB3.commit_tribuf();
  changed |= oam_bus.PIN_DB4.commit_tribuf();
  changed |= oam_bus.PIN_DB5.commit_tribuf();
  changed |= oam_bus.PIN_DB6.commit_tribuf();
  changed |= oam_bus.PIN_DB7.commit_tribuf();

  changed |= wave_pins.BORY_ABxxxxxH.commit_pinout();

  // Package pins

  /* PIN_01 */ changed |= ext_bus.PIN_A00_A.commit_pinout();     // <- KUPO
  /* PIN_01 */ changed |= ext_bus.PIN_A00_C.clear_preset();      // -> KOVA
  /* PIN_01 */ changed |= ext_bus.PIN_A00_D.commit_pinout();     // <- KOTY
  /* PIN_02 */ changed |= ext_bus.PIN_A01_A.commit_pinout();     // <- CABA
  /* PIN_02 */ changed |= ext_bus.PIN_A01_C.clear_preset();      // -> CAMU
  /* PIN_02 */ changed |= ext_bus.PIN_A01_D.commit_pinout();     // <- COTU
  /* PIN_03 */ changed |= ext_bus.PIN_A02_A.commit_pinout();     // <- BOKU
  /* PIN_03 */ changed |= ext_bus.PIN_A02_C.clear_preset();      // -> BUXU
  /* PIN_03 */ changed |= ext_bus.PIN_A02_D.commit_pinout();     // <- BAJO
  /* PIN_04 */ changed |= ext_bus.PIN_A03_A.commit_pinout();     // <- BOTY
  /* PIN_04 */ changed |= ext_bus.PIN_A03_C.clear_preset();      // -> BASE
  /* PIN_04 */ changed |= ext_bus.PIN_A03_D.commit_pinout();     // <- BOLA
  /* PIN_05 */ changed |= ext_bus.PIN_A04_A.commit_pinout();     // <- BYLA
  /* PIN_05 */ changed |= ext_bus.PIN_A04_C.clear_preset();      // -> AFEC
  /* PIN_05 */ changed |= ext_bus.PIN_A04_D.commit_pinout();     // <- BEVO
  /* PIN_06 */ changed |= ext_bus.PIN_A05_A.commit_pinout();     // <- BADU
  /* PIN_06 */ changed |= ext_bus.PIN_A05_C.clear_preset();      // -> ABUP
  /* PIN_06 */ changed |= ext_bus.PIN_A05_D.commit_pinout();     // <- AJAV
  /* PIN_07 */ changed |= ext_bus.PIN_A06_A.commit_pinout();     // <- CEPU
  /* PIN_07 */ changed |= ext_bus.PIN_A06_C.clear_preset();      // -> CYGU
  /* PIN_07 */ changed |= ext_bus.PIN_A06_D.commit_pinout();     // <- CYKA
  /* PIN_08 */ changed |= ext_bus.PIN_A07_A.commit_pinout();     // <- DEFY
  /* PIN_08 */ changed |= ext_bus.PIN_A07_C.clear_preset();      // -> COGO
  /* PIN_08 */ changed |= ext_bus.PIN_A07_D.commit_pinout();     // <- COLO
  /* PIN_09 */ changed |= ext_bus.PIN_A08_A.commit_pinout();     // <- MYNY
  /* PIN_09 */ changed |= ext_bus.PIN_A08_C.clear_preset();      // -> MUJY
  /* PIN_09 */ changed |= ext_bus.PIN_A08_D.commit_pinout();     // <- MEGO
  /* PIN_10 */ changed |= ext_bus.PIN_A09_A.commit_pinout();     // <- MUNE
  /* PIN_10 */ changed |= ext_bus.PIN_A09_C.clear_preset();      // -> NENA
  /* PIN_10 */ changed |= ext_bus.PIN_A09_D.commit_pinout();     // <- MENY
  /* PIN_11 */ changed |= ext_bus.PIN_A10_A.commit_pinout();     // <- ROXU
  /* PIN_11 */ changed |= ext_bus.PIN_A10_C.clear_preset();      // -> SURA
  /* PIN_11 */ changed |= ext_bus.PIN_A10_D.commit_pinout();     // <- RORE
  /* PIN_12 */ changed |= ext_bus.PIN_A11_A.commit_pinout();     // <- LEPY
  /* PIN_12 */ changed |= ext_bus.PIN_A11_C.clear_preset();      // -> MADY
  /* PIN_12 */ changed |= ext_bus.PIN_A11_D.commit_pinout();     // <- LYNY
  /* PIN_13 */ changed |= ext_bus.PIN_A12_A.commit_pinout();     // <- LUCE
  /* PIN_13 */ changed |= ext_bus.PIN_A12_C.clear_preset();      // -> LAHE
  /* PIN_13 */ changed |= ext_bus.PIN_A12_D.commit_pinout();     // <- LOSO
  /* PIN_14 */ changed |= ext_bus.PIN_A13_A.commit_pinout();     // <- LABE
  /* PIN_14 */ changed |= ext_bus.PIN_A13_C.clear_preset();      // -> LURA
  /* PIN_14 */ changed |= ext_bus.PIN_A13_D.commit_pinout();     // <- LEVA
  /* PIN_15 */ changed |= ext_bus.PIN_A14_A.commit_pinout();     // <- PUHE
  /* PIN_15 */ changed |= ext_bus.PIN_A14_C.clear_preset();      // -> PEVO
  /* PIN_15 */ changed |= ext_bus.PIN_A14_D.commit_pinout();     // <- PAHY
  /* PIN_16 */ changed |= ext_bus.PIN_A15_A.commit_pinout();     // <- SUZE
  /* PIN_16 */ changed |= ext_bus.PIN_A15_C.clear_preset();      // -> RAZA
  /* PIN_16 */ changed |= ext_bus.PIN_A15_D.commit_pinout();     // <- RULO
  /* PIN_17 */ changed |= ext_bus.PIN_D0_A.commit_pinout();      // <- RUXA
  /* PIN_17 */ changed |= ext_bus.PIN_D0_B.commit_pinout();      // <- LULA
  /* PIN_17 */ changed |= ext_bus.PIN_D0_C.clear_preset();       // -> TOVO,SOMA
  /* PIN_17 */ changed |= ext_bus.PIN_D0_D.commit_pinout();      // <- RUNE
  /* PIN_18 */ changed |= ext_bus.PIN_D1_A.commit_pinout();      // <- RUJA
  /* PIN_18 */ changed |= ext_bus.PIN_D1_B.commit_pinout();      // <- LULA
  /* PIN_18 */ changed |= ext_bus.PIN_D1_C.clear_preset();       // -> RUZY,RONY
  /* PIN_18 */ changed |= ext_bus.PIN_D1_D.commit_pinout();      // <- RYPU
  /* PIN_19 */ changed |= ext_bus.PIN_D2_A.commit_pinout();      // <- RABY
  /* PIN_19 */ changed |= ext_bus.PIN_D2_B.commit_pinout();      // <- LULA
  /* PIN_19 */ changed |= ext_bus.PIN_D2_C.clear_preset();       // -> ROME,RAXY
  /* PIN_19 */ changed |= ext_bus.PIN_D2_D.commit_pinout();      // <- SULY
  /* PIN_20 */ changed |= ext_bus.PIN_D3_A.commit_pinout();      // <- RERA
  /* PIN_20 */ changed |= ext_bus.PIN_D3_B.commit_pinout();      // <- LULA
  /* PIN_20 */ changed |= ext_bus.PIN_D3_C.clear_preset();       // -> SAZA,SELO
  /* PIN_20 */ changed |= ext_bus.PIN_D3_D.commit_pinout();      // <- SEZE
  /* PIN_21 */ changed |= ext_bus.PIN_D4_A.commit_pinout();      // <- RORY
  /* PIN_21 */ changed |= ext_bus.PIN_D4_B.commit_pinout();      // <- LULA
  /* PIN_21 */ changed |= ext_bus.PIN_D4_C.clear_preset();       // -> TEHE,SODY
  /* PIN_21 */ changed |= ext_bus.PIN_D4_D.commit_pinout();      // <- RESY
  /* PIN_22 */ changed |= ext_bus.PIN_D5_A.commit_pinout();      // <- RYVO
  /* PIN_22 */ changed |= ext_bus.PIN_D5_B.commit_pinout();      // <- LULA
  /* PIN_22 */ changed |= ext_bus.PIN_D5_C.clear_preset();       // -> RATU,SAGO
  /* PIN_22 */ changed |= ext_bus.PIN_D5_D.commit_pinout();      // <- TAMU
  /* PIN_23 */ changed |= ext_bus.PIN_D6_A.commit_pinout();      // <- RAFY
  /* PIN_23 */ changed |= ext_bus.PIN_D6_B.commit_pinout();      // <- LULA
  /* PIN_23 */ changed |= ext_bus.PIN_D6_C.clear_preset();       // -> SOCA,RUPA
  /* PIN_23 */ changed |= ext_bus.PIN_D6_D.commit_pinout();      // <- ROGY
  /* PIN_24 */ changed |= ext_bus.PIN_D7_A.commit_pinout();      // <- RAVU
  /* PIN_24 */ changed |= ext_bus.PIN_D7_B.commit_pinout();      // <- LULA
  /* PIN_24 */ changed |= ext_bus.PIN_D7_C.clear_preset();       // -> RYBA,SAZY
  /* PIN_24 */ changed |= ext_bus.PIN_D7_D.commit_pinout();      // <- RYDA
  /* PIN_25 */ changed |= vram_pins.PIN_MD7_A.commit_pinout();    // <- RYZE
  /* PIN_25 */ changed |= vram_pins.PIN_MD7_B.commit_pinout();    // <- ROFA
  /* PIN_25 */ changed |= vram_pins.PIN_MD7_C.clear_preset();     // -> RAKU
  /* PIN_25 */ changed |= vram_pins.PIN_MD7_D.commit_pinout();    // <- RADY
  /* PIN_26 */ changed |= vram_pins.PIN_MD6_A.commit_pinout();    // <- REKU
  /* PIN_26 */ changed |= vram_pins.PIN_MD6_B.commit_pinout();    // <- ROFA
  /* PIN_26 */ changed |= vram_pins.PIN_MD6_C.clear_preset();     // -> RETA
  /* PIN_26 */ changed |= vram_pins.PIN_MD6_D.commit_pinout();    // <- RYTY
  /* PIN_27 */ changed |= vram_pins.PIN_MD5_A.commit_pinout();    // <- REVU
  /* PIN_27 */ changed |= vram_pins.PIN_MD5_B.commit_pinout();    // <- ROFA
  /* PIN_27 */ changed |= vram_pins.PIN_MD5_C.clear_preset();     // -> ROPU
  /* PIN_27 */ changed |= vram_pins.PIN_MD5_D.commit_pinout();    // <- RUMU
  /* PIN_28 */ changed |= vram_pins.PIN_MD4_A.commit_pinout();    // <- RYRO
  /* PIN_28 */ changed |= vram_pins.PIN_MD4_B.commit_pinout();    // <- ROFA
  /* PIN_28 */ changed |= vram_pins.PIN_MD4_C.clear_preset();     // -> ROCE
  /* PIN_28 */ changed |= vram_pins.PIN_MD4_D.commit_pinout();    // <- RUBE
  /* PIN_29 */ changed |= vram_pins.PIN_MD3_A.commit_pinout();    // <- RADA
  /* PIN_29 */ changed |= vram_pins.PIN_MD3_B.commit_pinout();    // <- ROFA
  /* PIN_29 */ changed |= vram_pins.PIN_MD3_C.clear_preset();     // -> REMO
  /* PIN_29 */ changed |= vram_pins.PIN_MD3_D.commit_pinout();    // <- RODU
  /* PIN_30 */ changed |= vram_pins.PIN_MD2_A.commit_pinout();    // <- RAZO
  /* PIN_30 */ changed |= vram_pins.PIN_MD2_B.commit_pinout();    // <- ROFA
  /* PIN_30 */ changed |= vram_pins.PIN_MD2_C.clear_preset();     // -> RYDO
  /* PIN_30 */ changed |= vram_pins.PIN_MD2_D.commit_pinout();    // <- RARE
  /* PIN_31 */ changed |= vram_pins.PIN_MD1_A.commit_pinout();    // <- RYKY
  /* PIN_31 */ changed |= vram_pins.PIN_MD1_B.commit_pinout();    // <- ROFA
  /* PIN_31 */ changed |= vram_pins.PIN_MD1_C.clear_preset();     // -> REBA
  /* PIN_31 */ changed |= vram_pins.PIN_MD1_D.commit_pinout();    // <- RULY
  /* PIN_32 */ /*GND*/
  /* PIN_33 */ changed |= vram_pins.PIN_MD0_A.commit_pinout();    // <- REGE
  /* PIN_33 */ changed |= vram_pins.PIN_MD0_B.commit_pinout();    // <- ROFA
  /* PIN_33 */ changed |= vram_pins.PIN_MD0_C.clear_preset();     // -> RODY
  /* PIN_33 */ changed |= vram_pins.PIN_MD0_D.commit_pinout();    // <- RURA
  /* PIN_34 */ changed |= vram_pins.PIN_MA00_AD.commit_pinout();  // <- ECAL
  /* PIN_35 */ changed |= vram_pins.PIN_MA01_AD.commit_pinout();  // <- EGEZ
  /* PIN_36 */ changed |= vram_pins.PIN_MA02_AD.commit_pinout();  // <- FUHE
  /* PIN_37 */ changed |= vram_pins.PIN_MA03_AD.commit_pinout();  // <- FYZY
  /* PIN_38 */ changed |= vram_pins.PIN_MA04_AD.commit_pinout();  // <- DAMU
  /* PIN_39 */ changed |= vram_pins.PIN_MA05_AD.commit_pinout();  // <- DAVA
  /* PIN_40 */ changed |= vram_pins.PIN_MA06_AD.commit_pinout();  // <- ETEG
  /* PIN_41 */ changed |= vram_pins.PIN_MA07_AD.commit_pinout();  // <- EREW
  /* PIN_42 */ changed |= vram_pins.PIN_MA12_AD.commit_pinout();  // <- EXYF
  /* PIN_43 */ changed |= vram_pins.PIN_MCSn_A.commit_pinout();   // <- SOKY
  /* PIN_43 */ changed |= vram_pins.PIN_MCSn_C.clear_preset();     // -> TEFY
  /* PIN_43 */ changed |= vram_pins.PIN_MCSn_D.commit_pinout();   // <- SETY
  /* PIN_44 */ changed |= vram_pins.PIN_MA10_AD.commit_pinout();  // <- ERAF
  /* PIN_45 */ changed |= vram_pins.PIN_MOEn_A.commit_pinout();   // <- REFO
  /* PIN_45 */ changed |= vram_pins.PIN_MOEn_C.clear_preset();     // -> TAVY
  /* PIN_45 */ changed |= vram_pins.PIN_MOEn_D.commit_pinout();   // <- SAHA
  /* PIN_46 */ changed |= vram_pins.PIN_MA11_AD.commit_pinout();  // <- FUSY
  /* PIN_47 */ changed |= vram_pins.PIN_MA09_AD.commit_pinout();  // <- DUVE
  /* PIN_48 */ changed |= vram_pins.PIN_MA08_AD.commit_pinout();  // <- EVAX
  /* PIN_49 */ changed |= vram_pins.PIN_MWRn_A.commit_pinout();   // <- SYSY
  /* PIN_49 */ changed |= vram_pins.PIN_MWRn_C.clear_preset();     // -> SUDO
  /* PIN_49 */ changed |= vram_pins.PIN_MWRn_D.commit_pinout();   // <- RAGU
  
  changed |= lcd_reg.commit_pins();
  
  /* PIN_58 */ /*VCC*/
  /* PIN_59 */ /*ROUT*/
  /* PIN_60 */ /*LOUT*/
  /* PIN_61 */ /*VIN*/
  /* PIN_62 */ changed |= joy_pin.P15_A.commit_pinout();   // <- CELA
  /* PIN_62 */ changed |= joy_pin.P15_D.commit_pinout();   // <- COFY
  /* PIN_63 */ changed |= joy_pin.P14_A.commit_pinout();   // <- KARU
  /* PIN_63 */ changed |= joy_pin.P14_D.commit_pinout();   // <- KELY
  /* PIN_64 */ changed |= joy_pin.P13_A.commit_pinout();   // <- KORY
  /* PIN_64 */ changed |= joy_pin.P13_B.commit_pinout();   
  /* PIN_64 */ changed |= joy_pin.P13_C.clear_preset();    // -> KERY, P05.KOLO
  /* PIN_64 */ changed |= joy_pin.P13_D.commit_pinout();   // <- KALE
  /* PIN_65 */ changed |= joy_pin.P12_A.commit_pinout();   // <- KYHU
  /* PIN_65 */ changed |= joy_pin.P12_B.commit_pinout();   
  /* PIN_65 */ changed |= joy_pin.P12_C.clear_preset();    // -> KERY, P05.KEJA
  /* PIN_65 */ changed |= joy_pin.P12_D.commit_pinout();   // <- KASY
  /* PIN_66 */ changed |= joy_pin.P11_A.commit_pinout();   // <- KYTO
  /* PIN_66 */ changed |= joy_pin.P11_B.commit_pinout();   
  /* PIN_66 */ changed |= joy_pin.P11_C.clear_preset();    // -> KERY, P05.KAPA
  /* PIN_66 */ changed |= joy_pin.P11_D.commit_pinout();   // <- KABU
  /* PIN_67 */ changed |= joy_pin.P10_A.commit_pinout();   // <- KOLE
  /* PIN_67 */ changed |= joy_pin.P10_B.commit_pinout();   
  /* PIN_67 */ changed |= joy_pin.P10_C.clear_preset();    // -> KERY, KEVU
  /* PIN_67 */ changed |= joy_pin.P10_D.commit_pinout();   // <- KYBU

  changed |= ser_reg.commit();

  /* PIN_71 */ changed |= sys_pins.RST.clear_preset();
  /* PIN_72 */ /*GND*/
  /* PIN_73 */ /*CLKOUT*/
  /* PIN_74 */ changed |= sys_pins.PIN_CLK_GOOD.clear_preset();
  /* PIN_74 */ changed |= sys_pins.CLK_IN_xBxDxFxH.clear_preset();
  /* PIN_75 */ changed |= ext_bus.PIN_PHI.commit_pinout();     // <- BUDE/BEVA
  /* PIN_76 */ changed |= sys_pins.T2.clear_preset();
  /* PIN_77 */ changed |= sys_pins.T1.clear_preset();
  /* PIN_78 */ changed |= ext_bus.PIN_WR_A.commit_pinout();    // <- UVER
  /* PIN_78 */ changed |= ext_bus.PIN_WR_C.clear_preset();     // -> UBAL
  /* PIN_78 */ changed |= ext_bus.PIN_WR_D.commit_pinout();    // <- USUF
  /* PIN_79 */ changed |= ext_bus.PIN_RD_A.commit_pinout();    // <- UGAC
  /* PIN_79 */ changed |= ext_bus.PIN_RD_C.clear_preset();     // -> UJYV
  /* PIN_79 */ changed |= ext_bus.PIN_RD_D.commit_pinout();    // <- URUN
  /* PIN_80 */ changed |= ext_bus.PIN_CS_A.commit_pinout();    // <- TYHO

  return changed;
}

//-----------------------------------------------------------------------------
