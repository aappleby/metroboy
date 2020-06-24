#include "TestGB.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

void TestGB::ext_preset() {
  ext_pins.WR_C.preset(true, 0);   // -> P07.UBAL
  ext_pins.RD_C.preset(true, 0);   // -> P07.UJYV
  ext_pins.A00_C.preset(true, 0);   // -> P08.KOVA
  ext_pins.A01_C.preset(true, 0);   // -> P08.CAMU
  ext_pins.A02_C.preset(true, 0);   // -> P08.BUXU
  ext_pins.A03_C.preset(true, 0);   // -> P08.BASE
  ext_pins.A04_C.preset(true, 0);   // -> P08.AFEC
  ext_pins.A05_C.preset(true, 0);   // -> P08.ABUP
  ext_pins.A06_C.preset(true, 0);   // -> P08.CYGU
  ext_pins.A07_C.preset(true, 0);   // -> P08.COGO
  ext_pins.A08_C.preset(true, 0);   // -> P08.MUJY
  ext_pins.A09_C.preset(true, 0);   // -> P08.NENA
  ext_pins.A10_C.preset(true, 0);   // -> P08.SURA
  ext_pins.A11_C.preset(true, 0);   // -> P08.MADY
  ext_pins.A12_C.preset(true, 0);   // -> P08.LAHE
  ext_pins.A13_C.preset(true, 0);   // -> P08.LURA
  ext_pins.A14_C.preset(true, 0);   // -> P08.PEVO
  ext_pins.A15_C.preset(true, 0);   // -> P08.RAZA

  if (!ext_pins.RD_A && !ext_pins.CS_A) {
    uint16_t ext_addr = (uint16_t)pack(ext_pins.A00_A, ext_pins.A01_A, ext_pins.A02_A, ext_pins.A03_A,
                                       ext_pins.A04_A, ext_pins.A05_A, ext_pins.A06_A, ext_pins.A07_A,
                                       ext_pins.A08_A, ext_pins.A09_A, ext_pins.A10_A, ext_pins.A11_A,
                                       ext_pins.A12_A, ext_pins.A13_A, ext_pins.A14_A, ext_pins.A15_A);

    if (0x0000 <= ext_addr && ext_addr <= 0x7FFF) {
      // Cart rom
      //uint8_t d = rom[ext_addr];
      uint8_t d = 0;
      ext_pins.D0_C.preset(true, d & 0x01);
      ext_pins.D1_C.preset(true, d & 0x02);
      ext_pins.D2_C.preset(true, d & 0x04);
      ext_pins.D3_C.preset(true, d & 0x08);
      ext_pins.D4_C.preset(true, d & 0x10);
      ext_pins.D5_C.preset(true, d & 0x20);
      ext_pins.D6_C.preset(true, d & 0x40);
      ext_pins.D7_C.preset(true, d & 0x80);
    }
    else if (0xC000 <= ext_addr && ext_addr <= 0xDFFF) {
      // Main ram
      //uint8_t d = ram[ext_addr - 0xC000];
      uint8_t d = 0;
      ext_pins.D0_C.preset(true, d & 0x01);
      ext_pins.D1_C.preset(true, d & 0x02);
      ext_pins.D2_C.preset(true, d & 0x04);
      ext_pins.D3_C.preset(true, d & 0x08);
      ext_pins.D4_C.preset(true, d & 0x10);
      ext_pins.D5_C.preset(true, d & 0x20);
      ext_pins.D6_C.preset(true, d & 0x40);
      ext_pins.D7_C.preset(true, d & 0x80);
    }
    else if (0xE000 <= ext_addr && ext_addr <= 0xFFFF) {
      // Echo ram
      //uint8_t d = ram[ext_addr - 0xE000];
      uint8_t d = 0;
      ext_pins.D0_C.preset(true, d & 0x01);
      ext_pins.D1_C.preset(true, d & 0x02);
      ext_pins.D2_C.preset(true, d & 0x04);
      ext_pins.D3_C.preset(true, d & 0x08);
      ext_pins.D4_C.preset(true, d & 0x10);
      ext_pins.D5_C.preset(true, d & 0x20);
      ext_pins.D6_C.preset(true, d & 0x40);
      ext_pins.D7_C.preset(true, d & 0x80);
    }
    else {
      printf("Bad address?\n");
      __debugbreak();
    }
  }
  else {
    ext_pins.D0_C.preset(false, 0);
    ext_pins.D1_C.preset(false, 0);
    ext_pins.D2_C.preset(false, 0);
    ext_pins.D3_C.preset(false, 0);
    ext_pins.D4_C.preset(false, 0);
    ext_pins.D5_C.preset(false, 0);
    ext_pins.D6_C.preset(false, 0);
    ext_pins.D7_C.preset(false, 0);
  }
}

//-----------------------------------------------------------------------------

bool TestGB::commit_everything() {
  bool changed = false;

  /*p04.MAKA*/ changed |= FROM_CPU5_SYNC.commit_reg();

  /*p01.AFUR*/ changed |= clk_reg.PHAZ_xBCDExxx.commit_duo();
  /*p01.ALEF*/ changed |= clk_reg.PHAZ_xxCDEFxx.commit_duo();
  /*p01.APUK*/ changed |= clk_reg.PHAZ_xxxDEFGx.commit_duo();
  /*p01.ADYK*/ changed |= clk_reg.PHAZ_xxxxEFGH.commit_duo();

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

  /*p02.LOPE*/ changed |= int_reg.FF0F_0.commit_reg();
  /*p02.UBUL*/ changed |= int_reg.FF0F_1.commit_reg();
  /*p02.ULAK*/ changed |= int_reg.FF0F_2.commit_reg();
  /*p02.LALU*/ changed |= int_reg.FF0F_3.commit_reg();
  /*p02.NYBO*/ changed |= int_reg.FF0F_4.commit_reg();
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
  /*p31.YLOR*/ changed |= oam_reg.REG_OAM_A0.commit_reg();
  /*p31.ZYTY*/ changed |= oam_reg.REG_OAM_A1.commit_reg();
  /*p31.ZYVE*/ changed |= oam_reg.REG_OAM_A2.commit_reg();
  /*p31.ZEZY*/ changed |= oam_reg.REG_OAM_A3.commit_reg();
  /*p31.GOMO*/ changed |= oam_reg.REG_OAM_A4.commit_reg();
  /*p31.BAXO*/ changed |= oam_reg.REG_OAM_A5.commit_reg();
  /*p31.YZOS*/ changed |= oam_reg.REG_OAM_A6.commit_reg();
  /*p31.DEPO*/ changed |= oam_reg.REG_OAM_A7.commit_reg();
  /*p29.XUSO*/ changed |= oam_reg.REG_OAM_B0.commit_reg();
  /*p29.XEGU*/ changed |= oam_reg.REG_OAM_B1.commit_reg();
  /*p29.YJEX*/ changed |= oam_reg.REG_OAM_B2.commit_reg();
  /*p29.XYJU*/ changed |= oam_reg.REG_OAM_B3.commit_reg();
  /*p29.YBOG*/ changed |= oam_reg.REG_OAM_B4.commit_reg();
  /*p29.WYSO*/ changed |= oam_reg.REG_OAM_B5.commit_reg();
  /*p29.XOTE*/ changed |= oam_reg.REG_OAM_B6.commit_reg();
  /*p29.YZAB*/ changed |= oam_reg.REG_OAM_B7.commit_reg();

  /*p32.LEGU*/ changed |= pxp_reg.BG_PIX_A0.commit_latch();
  /*p32.NUDU*/ changed |= pxp_reg.BG_PIX_A1.commit_latch();
  /*p32.MUKU*/ changed |= pxp_reg.BG_PIX_A2.commit_latch();
  /*p32.LUZO*/ changed |= pxp_reg.BG_PIX_A3.commit_latch();
  /*p32.MEGU*/ changed |= pxp_reg.BG_PIX_A4.commit_latch();
  /*p32.MYJY*/ changed |= pxp_reg.BG_PIX_A5.commit_latch();
  /*p32.NASA*/ changed |= pxp_reg.BG_PIX_A6.commit_latch();
  /*p32.NEFO*/ changed |= pxp_reg.BG_PIX_A7.commit_latch();
  /*p32.RAWU*/ changed |= pxp_reg.BG_PIX_B0.commit_reg();
  /*p32.POZO*/ changed |= pxp_reg.BG_PIX_B1.commit_reg();
  /*p32.PYZO*/ changed |= pxp_reg.BG_PIX_B2.commit_reg();
  /*p32.POXA*/ changed |= pxp_reg.BG_PIX_B3.commit_reg();
  /*p32.PULO*/ changed |= pxp_reg.BG_PIX_B4.commit_reg();
  /*p32.POJU*/ changed |= pxp_reg.BG_PIX_B5.commit_reg();
  /*p32.POWY*/ changed |= pxp_reg.BG_PIX_B6.commit_reg();
  /*p32.PYJU*/ changed |= pxp_reg.BG_PIX_B7.commit_reg();
  /*p33.PEFO*/ changed |= pxp_reg.SPR_PIX_A0.commit_latch();
  /*p33.ROKA*/ changed |= pxp_reg.SPR_PIX_A1.commit_latch();
  /*p33.MYTU*/ changed |= pxp_reg.SPR_PIX_A2.commit_latch();
  /*p33.RAMU*/ changed |= pxp_reg.SPR_PIX_A3.commit_latch();
  /*p33.SELE*/ changed |= pxp_reg.SPR_PIX_A4.commit_latch();
  /*p33.SUTO*/ changed |= pxp_reg.SPR_PIX_A5.commit_latch();
  /*p33.RAMA*/ changed |= pxp_reg.SPR_PIX_A6.commit_latch();
  /*p33.RYDU*/ changed |= pxp_reg.SPR_PIX_A7.commit_latch();
  /*p33.REWO*/ changed |= pxp_reg.SPR_PIX_B0.commit_latch();
  /*p33.PEBA*/ changed |= pxp_reg.SPR_PIX_B1.commit_latch();
  /*p33.MOFO*/ changed |= pxp_reg.SPR_PIX_B2.commit_latch();
  /*p33.PUDU*/ changed |= pxp_reg.SPR_PIX_B3.commit_latch();
  /*p33.SAJA*/ changed |= pxp_reg.SPR_PIX_B4.commit_latch();
  /*p33.SUNY*/ changed |= pxp_reg.SPR_PIX_B5.commit_latch();
  /*p33.SEMO*/ changed |= pxp_reg.SPR_PIX_B6.commit_latch();
  /*p33.SEGA*/ changed |= pxp_reg.SPR_PIX_B7.commit_latch();
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

  /*p01.TUBO*/ changed |= rst_reg.TUBO_CLKREQn_LATCH.commit_latch();
  /*p01.AFER*/ changed |= rst_reg.RESET_REGp.commit_reg();

  /*p06.ETAF*/ changed |= ser_reg.XFER_START.commit_reg();
  /*p06.CULY*/ changed |= ser_reg.XFER_DIR.commit_reg();
  /*p06.COTY*/ changed |= ser_reg.SER_CLK.commit_reg();
  /*p06.CAFA*/ changed |= ser_reg.SER_CNT0.commit_reg();
  /*p06.CYLO*/ changed |= ser_reg.SER_CNT1.commit_reg();
  /*p06.CYDE*/ changed |= ser_reg.SER_CNT2.commit_reg();
  /*p06.CALY*/ changed |= ser_reg.SER_CNT3.commit_reg();
  /*p06.CUBA*/ changed |= ser_reg.SER_DATA0.commit_reg();
  /*p06.DEGU*/ changed |= ser_reg.SER_DATA1.commit_reg();
  /*p06.DYRA*/ changed |= ser_reg.SER_DATA2.commit_reg();
  /*p06.DOJO*/ changed |= ser_reg.SER_DATA3.commit_reg();
  /*p06.DOVU*/ changed |= ser_reg.SER_DATA4.commit_reg();
  /*p06.EJAB*/ changed |= ser_reg.SER_DATA5.commit_reg();
  /*p06.EROD*/ changed |= ser_reg.SER_DATA6.commit_reg();
  /*p06.EDER*/ changed |= ser_reg.SER_DATA7.commit_reg();
  /*p06.ELYS*/ changed |= ser_reg.SER_OUT.commit_reg();

  /*p29.TOXE*/ changed |= spr_reg.TOXE_SFETCH_S0_D0.commit_reg();
  /*p29.TULY*/ changed |= spr_reg.TULY_SFETCH_S1.commit_reg();
  /*p29.TESE*/ changed |= spr_reg.TESE_SFETCH_S2.commit_reg();
  /*p29.TOBU*/ changed |= spr_reg.TOBU_SFETCH_S1_D2.commit_reg();
  /*p29.VONU*/ changed |= spr_reg.VONU_SFETCH_S1_D4.commit_reg();
  /*p29.SEBA*/ changed |= spr_reg.SEBA_SFETCH_S1_D5.commit_reg();
  /*p29.TYFO*/ changed |= spr_reg.TYFO_SFETCH_S0_D1.commit_reg();

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
  /*p03.MOBA*/ changed |= tim_reg.INT_TIMER.commit_reg();
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

  /*p29.WUVU*/ changed |= vck_reg.WUVU_AxxDExxH.commit_reg();
  /*p21.VENA*/ changed |= vck_reg.VENA_xBCDExxx.commit_reg();
  /*p29.WOSU*/ changed |= vck_reg.WOSU_xxCDxxGH.commit_reg();

  /*p??.ROXY*/ changed |= vid_reg.ROXY_FINE_MATCH_LATCH.commit_latch();
  /*p??.PUXA*/ changed |= vid_reg.PUXA_FINE_MATCH_SYNC1.commit_reg();
  /*p27.NYZE*/ changed |= vid_reg.NYZE_FINE_MATCH_SYNC2.commit_reg();
  /*p27.RYKU*/ changed |= vid_reg.FINE_CNT0.commit_reg();
  /*p27.ROGA*/ changed |= vid_reg.FINE_CNT1.commit_reg();
  /*p27.RUBU*/ changed |= vid_reg.FINE_CNT2.commit_reg();
  /*p21.XEHO*/ changed |= vid_reg.X0.commit_reg();
  /*p21.SAVY*/ changed |= vid_reg.X1.commit_reg();
  /*p21.XODU*/ changed |= vid_reg.X2.commit_reg();
  /*p21.XYDO*/ changed |= vid_reg.X3.commit_reg();
  /*p21.TUHU*/ changed |= vid_reg.X4.commit_reg();
  /*p21.TUKY*/ changed |= vid_reg.X5.commit_reg();
  /*p21.TAKO*/ changed |= vid_reg.X6.commit_reg();
  /*p21.SYBE*/ changed |= vid_reg.X7.commit_reg();
  /*p27.NOPA*/ changed |= vid_reg.NOPA_WIN_MODE_SYNC.commit_reg();
  /*p27.SOVY*/ changed |= vid_reg.SOVY_WIN_HIT_SYNC.commit_reg();
  /*p21.XYMU*/ changed |= vid_reg.XYMU_RENDERINGp.commit_latch();
  /*p21.VOGA*/ changed |= vid_reg.RENDER_DONE_SYNC.commit_reg();
  /*p27.PYNU*/ changed |= vid_reg.PYNU_WIN_MODE_LATCH.commit_latch();
  /*p27.RYDY*/ changed |= vid_reg.RYDY_WIN_HIT_LATCH.commit_latch();
  /*p21.ROXE*/ changed |= vid_reg.INT_HBL_EN.commit_reg();
  /*p21.RUFO*/ changed |= vid_reg.INT_VBL_EN.commit_reg();
  /*p21.REFE*/ changed |= vid_reg.INT_OAM_EN.commit_reg();
  /*p21.RUGU*/ changed |= vid_reg.INT_LYC_EN.commit_reg();
  /*p27.SARY*/ changed |= vid_reg.WY_MATCH_SYNC.commit_reg();
  /*p27.RYFA*/ changed |= vid_reg.WIN_MATCH_ONSCREEN_SYNC1.commit_reg();
  /*p27.RENE*/ changed |= vid_reg.WIN_MATCH_ONSCREEN_SYNC2.commit_reg();
  /*p27.PYCO*/ changed |= vid_reg.WIN_MATCH_SYNC1.commit_reg();
  /*p27.NUNU*/ changed |= vid_reg.WIN_MATCH_SYNC2.commit_reg();
  /*p27.LONY*/ changed |= vid_reg.BG_READ_VRAM_LATCHp.commit_latch();
  /*p27.LAXU*/ changed |= vid_reg.BFETCH_S0.commit_reg();
  /*p27.MESU*/ changed |= vid_reg.BFETCH_S1.commit_reg();
  /*p27.NYVA*/ changed |= vid_reg.BFETCH_S2.commit_reg();
  /*p27.LOVY*/ changed |= vid_reg.LOVY_BG_SEQ5_SYNC.commit_reg();
  /*p24.NYKA*/ changed |= vid_reg.BFETCH_DONE_SYNC.commit_reg();
  /*p24.PORY*/ changed |= vid_reg.BFETCH_DONE_SYNC_DELAY.commit_reg();
  /*p27.LYZU*/ changed |= vid_reg.BFETCH_S0_DELAY.commit_reg();
  /*p27.WYKA*/ changed |= vid_reg.WIN_X3.commit_reg();
  /*p27.WODY*/ changed |= vid_reg.WIN_X4.commit_reg();
  /*p27.WOBO*/ changed |= vid_reg.WIN_X5.commit_reg();
  /*p27.WYKO*/ changed |= vid_reg.WIN_X6.commit_reg();
  /*p27.XOLO*/ changed |= vid_reg.WIN_X7.commit_reg();
  /*p27.VYNO*/ changed |= vid_reg.WIN_Y0.commit_reg();
  /*p27.VUJO*/ changed |= vid_reg.WIN_Y1.commit_reg();
  /*p27.VYMU*/ changed |= vid_reg.WIN_Y2.commit_reg();
  /*p27.TUFU*/ changed |= vid_reg.WIN_Y3.commit_reg();
  /*p27.TAXA*/ changed |= vid_reg.WIN_Y4.commit_reg();
  /*p27.TOZO*/ changed |= vid_reg.WIN_Y5.commit_reg();
  /*p27.TATE*/ changed |= vid_reg.WIN_Y6.commit_reg();
  /*p27.TEKE*/ changed |= vid_reg.WIN_Y7.commit_reg();
  /*p24.PYGO*/ changed |= vid_reg.PYGO_TILE_DONE.commit_reg();
  /*p24.POKY*/ changed |= vid_reg.POKY_FRONT_PORCH_LATCH.commit_latch();
  /*p27.REJO*/ changed |= vid_reg.WY_MATCH_LATCH.commit_latch();

  /*p27.TAKA*/ changed |= vid_reg.SFETCH_RUN_LATCH.commit_latch();
  /*p27.SOBU*/ changed |= vid_reg.SOBU_SPRITE_FETCH_SYNC1.commit_reg();
  /*p27.SUDA*/ changed |= vid_reg.SUDA_SPRITE_FETCH_SYNC2.commit_reg();
  changed |= vid_reg.MA00.commit_tribuf();
  changed |= vid_reg.MA01.commit_tribuf();
  changed |= vid_reg.MA02.commit_tribuf();
  changed |= vid_reg.MA03.commit_tribuf();
  changed |= vid_reg.MA04.commit_tribuf();
  changed |= vid_reg.MA05.commit_tribuf();
  changed |= vid_reg.MA06.commit_tribuf();
  changed |= vid_reg.MA07.commit_tribuf();
  changed |= vid_reg.MA08.commit_tribuf();
  changed |= vid_reg.MA09.commit_tribuf();
  changed |= vid_reg.MA10.commit_tribuf();
  changed |= vid_reg.MA11.commit_tribuf();
  changed |= vid_reg.MA12.commit_tribuf();
  changed |= vid_reg.MD0.commit_tribuf();
  changed |= vid_reg.MD1.commit_tribuf();
  changed |= vid_reg.MD2.commit_tribuf();
  changed |= vid_reg.MD3.commit_tribuf();
  changed |= vid_reg.MD4.commit_tribuf();
  changed |= vid_reg.MD5.commit_tribuf();
  changed |= vid_reg.MD6.commit_tribuf();
  changed |= vid_reg.MD7.commit_tribuf();

  /*p07.TEPU*/ changed |= bus_reg.BOOT_BIT.commit_reg();
  /*p25.SOTO*/ changed |= bus_reg.SOTO_DBG.commit_reg();

  /*p08.ALOR*/ changed |= bus_reg.CPU_ADDR_LATCH_00.commit_latch();
  /*p08.APUR*/ changed |= bus_reg.CPU_ADDR_LATCH_01.commit_latch();
  /*p08.ALYR*/ changed |= bus_reg.CPU_ADDR_LATCH_02.commit_latch();
  /*p08.ARET*/ changed |= bus_reg.CPU_ADDR_LATCH_03.commit_latch();
  /*p08.AVYS*/ changed |= bus_reg.CPU_ADDR_LATCH_04.commit_latch();
  /*p08.ATEV*/ changed |= bus_reg.CPU_ADDR_LATCH_05.commit_latch();
  /*p08.AROS*/ changed |= bus_reg.CPU_ADDR_LATCH_06.commit_latch();
  /*p08.ARYM*/ changed |= bus_reg.CPU_ADDR_LATCH_07.commit_latch();
  /*p08.LUNO*/ changed |= bus_reg.CPU_ADDR_LATCH_08.commit_latch();
  /*p08.LYSA*/ changed |= bus_reg.CPU_ADDR_LATCH_09.commit_latch();
  /*p08.PATE*/ changed |= bus_reg.CPU_ADDR_LATCH_10.commit_latch();
  /*p08.LUMY*/ changed |= bus_reg.CPU_ADDR_LATCH_11.commit_latch();
  /*p08.LOBU*/ changed |= bus_reg.CPU_ADDR_LATCH_12.commit_latch();
  /*p08.LONU*/ changed |= bus_reg.CPU_ADDR_LATCH_13.commit_latch();
  /*p08.NYRE*/ changed |= bus_reg.CPU_ADDR_LATCH_14.commit_latch();

  /*p08.SOMA*/ changed |= bus_reg.CART_DATA_LATCH_00.commit_latch();
  /*p08.RONY*/ changed |= bus_reg.CART_DATA_LATCH_01.commit_latch();
  /*p08.RAXY*/ changed |= bus_reg.CART_DATA_LATCH_02.commit_latch();
  /*p08.SELO*/ changed |= bus_reg.CART_DATA_LATCH_03.commit_latch();
  /*p08.SODY*/ changed |= bus_reg.CART_DATA_LATCH_04.commit_latch();
  /*p08.SAGO*/ changed |= bus_reg.CART_DATA_LATCH_05.commit_latch();
  /*p08.RUPA*/ changed |= bus_reg.CART_DATA_LATCH_06.commit_latch();
  /*p08.SAZY*/ changed |= bus_reg.CART_DATA_LATCH_07.commit_latch();

  //----------------------------------------
  // PINS

  changed |= cpu_pins.TO_CPU2.commit_pinout();       // <- P02.AWOB
  changed |= cpu_pins.CPU_RAW_RD.clear_preset();     // PORTA_00: -> P07.UJYV, P08.LAGU, P08.LAVO
  changed |= cpu_pins.CPU_RAW_WR.clear_preset();     // PORTA_01: -> P01.AREV, P08.LAGU. This is almost definitely "raw write"
  changed |= cpu_pins.UNOR_MODE_DBG2.commit_pinout();         // PORTA_02: <- P07.UNOR_MODE_DBG2
  changed |= cpu_pins.SYRO.commit_pinout();          // PORTA_03: <- P25.SYRO
  changed |= cpu_pins.READ_BOOTROM.commit_pinout();  // PORTA_04: <- P07.READ_BOOTROM
  changed |= cpu_pins.UMUT_MODE_DBG1.commit_pinout();         // PORTA_05: <- P07.UMUT_MODE_DBG1
  changed |= cpu_pins.ADDR_VALIDx.clear_preset();    // PORTA_06: -> APAP, TEXO. This is almost definitely "address valid"
  changed |= cpu_pins.ACK_VBLANK.clear_preset();     // PORTB_01: -> P02.LETY, vblank int ack
  changed |= cpu_pins.INT_VBLANK.commit_pinout();    // PORTB_03: <- P02.LOPE, vblank int
  changed |= cpu_pins.ACK_STAT.clear_preset();       // PORTB_05: -> P02.LEJA, stat int ack
  changed |= cpu_pins.INT_STAT.commit_pinout();      // PORTB_07: <- P02.LALU, stat int
  changed |= cpu_pins.ACK_TIMER.clear_preset();      // PORTB_09: -> P02.LESA, timer int ack
  changed |= cpu_pins.INT_TIMER.commit_pinout();     // PORTB_11: <- P02.NYBO, timer int
  changed |= cpu_pins.ACK_SERIAL.clear_preset();     // PORTB_13: -> P02.LUFE, serial int ack
  changed |= cpu_pins.INT_SERIAL.commit_pinout();    // PORTB_15: <- P02.UBUL, serial int
  changed |= cpu_pins.ACK_JOYPAD.clear_preset();     // PORTB_17: -> P02.LAMO, joypad int ack
  changed |= cpu_pins.INT_JOYPAD.commit_pinout();    // PORTB_19: <- P02.ULAK, joypad int
  changed |= cpu_pins.CLKREQ.clear_preset();         // PORTC_00: -> ABOL (an inverter) -> BATE. Something about "cpu ready". clock request?
  changed |= cpu_pins.AFER.commit_pinout();          // PORTC_01: <- P01.AFER , reset related reg
  changed |= cpu_pins.PIN_RESET.commit_pinout();     // PORTC_02: <- PIN_RESET directly connected to the pad
  changed |= cpu_pins.CLK_GOOD.commit_pinout();      // PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,
  changed |= cpu_pins.CPU_RESET.commit_pinout();     // PORTC_04: <- P01.CPU_RESET
  changed |= cpu_pins.FROM_CPU6.clear_preset();      // PORTD_00: -> P07.LEXY, doesn't do anything

  changed |= cpu_pins.BOWA_AxCDEFGH.commit_pinout(); // PORTD_01: <- P01.BOWA
  changed |= cpu_pins.BEDO_xBxxxxxx.commit_pinout(); // PORTD_02: <- P01.BEDO _____fgh

  changed |= cpu_pins.BEKO_xBCDExxx.commit_pinout(); // PORTD_03: <- P01.BEKO ____efgh connection not indicated on P01
  changed |= cpu_pins.BUDE_AxxxxFGH.commit_pinout(); // PORTD_04: <- P01.BUDE abcd____
  
  changed |= cpu_pins.BOLO_xBCDEFGx.commit_pinout(); // PORTD_05: <- BOLO_xBCDEFGx + BYDA?                                - test pad 2
  changed |= cpu_pins.FROM_CPU5.clear_preset();      // PORTD_05: -> FROM_CPU5
  changed |= cpu_pins.BUKE_ABxxxxxH.commit_pinout(); // PORTD_06: <- P01.BUKE _____f__

  changed |= cpu_pins.BOMA_xBxxxxxx.commit_pinout(); // PORTD_07: <- P01.RESET_CLK _____fgh
  changed |= cpu_pins.BOGA_AxCDEFGH.commit_pinout(); // PORTD_08: <- P01.BOGA abcde___

  changed |= cpu_pins.A00.clear_preset();
  changed |= cpu_pins.A01.clear_preset();
  changed |= cpu_pins.A02.clear_preset();
  changed |= cpu_pins.A03.clear_preset();
  changed |= cpu_pins.A04.clear_preset();
  changed |= cpu_pins.A05.clear_preset();
  changed |= cpu_pins.A06.clear_preset();
  changed |= cpu_pins.A07.clear_preset();
  changed |= cpu_pins.A08.clear_preset();
  changed |= cpu_pins.A09.clear_preset();
  changed |= cpu_pins.A10.clear_preset();
  changed |= cpu_pins.A11.clear_preset();
  changed |= cpu_pins.A12.clear_preset();
  changed |= cpu_pins.A13.clear_preset();
  changed |= cpu_pins.A14.clear_preset();
  changed |= cpu_pins.A15.clear_preset();
  changed |= cpu_pins.D0.commit_tribuf();
  changed |= cpu_pins.D1.commit_tribuf();
  changed |= cpu_pins.D2.commit_tribuf();
  changed |= cpu_pins.D3.commit_tribuf();
  changed |= cpu_pins.D4.commit_tribuf();
  changed |= cpu_pins.D5.commit_tribuf();
  changed |= cpu_pins.D6.commit_tribuf();
  changed |= cpu_pins.D7.commit_tribuf();

  changed |= oam_pins.COTA_CLK.commit_pinout();
  changed |= oam_pins.ZODO_OE.commit_pinout();
  changed |= oam_pins.WR_A.commit_pinout(); // definitely write
  changed |= oam_pins.WR_B.commit_pinout(); // definitely write
  changed |= oam_pins.A0.commit_pinout();
  changed |= oam_pins.A1.commit_pinout();
  changed |= oam_pins.A2.commit_pinout();
  changed |= oam_pins.A3.commit_pinout();
  changed |= oam_pins.A4.commit_pinout();
  changed |= oam_pins.A5.commit_pinout();
  changed |= oam_pins.A6.commit_pinout();
  changed |= oam_pins.A7.commit_pinout();
  changed |= oam_pins.A_D0.commit_tribuf();
  changed |= oam_pins.A_D1.commit_tribuf();
  changed |= oam_pins.A_D2.commit_tribuf();
  changed |= oam_pins.A_D3.commit_tribuf();
  changed |= oam_pins.A_D4.commit_tribuf();
  changed |= oam_pins.A_D5.commit_tribuf();
  changed |= oam_pins.A_D6.commit_tribuf();
  changed |= oam_pins.A_D7.commit_tribuf();
  changed |= oam_pins.B_D0.commit_tribuf();
  changed |= oam_pins.B_D1.commit_tribuf();
  changed |= oam_pins.B_D2.commit_tribuf();
  changed |= oam_pins.B_D3.commit_tribuf();
  changed |= oam_pins.B_D4.commit_tribuf();
  changed |= oam_pins.B_D5.commit_tribuf();
  changed |= oam_pins.B_D6.commit_tribuf();
  changed |= oam_pins.B_D7.commit_tribuf();

  changed |= wave_pins.BORY_ABxxxxxH.commit_pinout();

  // Package pins

  /* PIN_01 */ changed |= ext_pins.A00_A.commit_pinout();   // <- P08.KUPO
  /* PIN_01 */ changed |= ext_pins.A00_C.clear_preset();   // -> P08.KOVA
  /* PIN_01 */ changed |= ext_pins.A00_D.commit_pinout();   // <- P08.KOTY
  /* PIN_02 */ changed |= ext_pins.A01_A.commit_pinout();   // <- P08.CABA
  /* PIN_02 */ changed |= ext_pins.A01_C.clear_preset();   // -> P08.CAMU
  /* PIN_02 */ changed |= ext_pins.A01_D.commit_pinout();   // <- P08.COTU
  /* PIN_03 */ changed |= ext_pins.A02_A.commit_pinout();   // <- P08.BOKU
  /* PIN_03 */ changed |= ext_pins.A02_C.clear_preset();   // -> P08.BUXU
  /* PIN_03 */ changed |= ext_pins.A02_D.commit_pinout();   // <- P08.BAJO
  /* PIN_04 */ changed |= ext_pins.A03_A.commit_pinout();   // <- P08.BOTY
  /* PIN_04 */ changed |= ext_pins.A03_C.clear_preset();   // -> P08.BASE
  /* PIN_04 */ changed |= ext_pins.A03_D.commit_pinout();   // <- P08.BOLA
  /* PIN_05 */ changed |= ext_pins.A04_A.commit_pinout();   // <- P08.BYLA
  /* PIN_05 */ changed |= ext_pins.A04_C.clear_preset();   // -> P08.AFEC
  /* PIN_05 */ changed |= ext_pins.A04_D.commit_pinout();   // <- P08.BEVO
  /* PIN_06 */ changed |= ext_pins.A05_A.commit_pinout();   // <- P08.BADU
  /* PIN_06 */ changed |= ext_pins.A05_C.clear_preset();   // -> P08.ABUP
  /* PIN_06 */ changed |= ext_pins.A05_D.commit_pinout();   // <- P08.AJAV
  /* PIN_07 */ changed |= ext_pins.A06_A.commit_pinout();   // <- P08.CEPU
  /* PIN_07 */ changed |= ext_pins.A06_C.clear_preset();   // -> P08.CYGU
  /* PIN_07 */ changed |= ext_pins.A06_D.commit_pinout();   // <- P08.CYKA
  /* PIN_08 */ changed |= ext_pins.A07_A.commit_pinout();   // <- P08.DEFY
  /* PIN_08 */ changed |= ext_pins.A07_C.clear_preset();   // -> P08.COGO
  /* PIN_08 */ changed |= ext_pins.A07_D.commit_pinout();   // <- P08.COLO
  /* PIN_09 */ changed |= ext_pins.A08_A.commit_pinout();   // <- P08.MYNY
  /* PIN_09 */ changed |= ext_pins.A08_C.clear_preset();   // -> P08.MUJY
  /* PIN_09 */ changed |= ext_pins.A08_D.commit_pinout();   // <- P08.MEGO
  /* PIN_10 */ changed |= ext_pins.A09_A.commit_pinout();   // <- P08.MUNE
  /* PIN_10 */ changed |= ext_pins.A09_C.clear_preset();   // -> P08.NENA
  /* PIN_10 */ changed |= ext_pins.A09_D.commit_pinout();   // <- P08.MENY
  /* PIN_11 */ changed |= ext_pins.A10_A.commit_pinout();   // <- P08.ROXU
  /* PIN_11 */ changed |= ext_pins.A10_C.clear_preset();   // -> P08.SURA
  /* PIN_11 */ changed |= ext_pins.A10_D.commit_pinout();   // <- P08.RORE
  /* PIN_12 */ changed |= ext_pins.A11_A.commit_pinout();   // <- P08.LEPY
  /* PIN_12 */ changed |= ext_pins.A11_C.clear_preset();   // -> P08.MADY
  /* PIN_12 */ changed |= ext_pins.A11_D.commit_pinout();   // <- P08.LYNY
  /* PIN_13 */ changed |= ext_pins.A12_A.commit_pinout();   // <- P08.LUCE
  /* PIN_13 */ changed |= ext_pins.A12_C.clear_preset();   // -> P08.LAHE
  /* PIN_13 */ changed |= ext_pins.A12_D.commit_pinout();   // <- P08.LOSO
  /* PIN_14 */ changed |= ext_pins.A13_A.commit_pinout();   // <- P08.LABE
  /* PIN_14 */ changed |= ext_pins.A13_C.clear_preset();   // -> P08.LURA
  /* PIN_14 */ changed |= ext_pins.A13_D.commit_pinout();   // <- P08.LEVA
  /* PIN_15 */ changed |= ext_pins.A14_A.commit_pinout();   // <- P08.PUHE
  /* PIN_15 */ changed |= ext_pins.A14_C.clear_preset();   // -> P08.PEVO
  /* PIN_15 */ changed |= ext_pins.A14_D.commit_pinout();   // <- P08.PAHY
  /* PIN_16 */ changed |= ext_pins.A15_A.commit_pinout();   // <- P08.SUZE
  /* PIN_16 */ changed |= ext_pins.A15_C.clear_preset();   // -> P08.RAZA
  /* PIN_16 */ changed |= ext_pins.A15_D.commit_pinout();   // <- P08.RULO
  /* PIN_17 */ changed |= ext_pins.D0_A.commit_pinout();    // <- P08.RUXA
  /* PIN_17 */ changed |= ext_pins.D0_B.commit_pinout();    // <- P08.LULA
  /* PIN_17 */ changed |= ext_pins.D0_C.clear_preset();    // -> P08.TOVO,SOMA
  /* PIN_17 */ changed |= ext_pins.D0_D.commit_pinout();    // <- P08.RUNE
  /* PIN_18 */ changed |= ext_pins.D1_A.commit_pinout();    // <- P08.RUJA
  /* PIN_18 */ changed |= ext_pins.D1_B.commit_pinout();    // <- P08.LULA
  /* PIN_18 */ changed |= ext_pins.D1_C.clear_preset();    // -> P08.RUZY,RONY
  /* PIN_18 */ changed |= ext_pins.D1_D.commit_pinout();    // <- P08.RYPU
  /* PIN_19 */ changed |= ext_pins.D2_A.commit_pinout();    // <- P08.RABY
  /* PIN_19 */ changed |= ext_pins.D2_B.commit_pinout();    // <- P08.LULA
  /* PIN_19 */ changed |= ext_pins.D2_C.clear_preset();    // -> P08.ROME,RAXY
  /* PIN_19 */ changed |= ext_pins.D2_D.commit_pinout();    // <- P08.SULY
  /* PIN_20 */ changed |= ext_pins.D3_A.commit_pinout();    // <- P08.RERA
  /* PIN_20 */ changed |= ext_pins.D3_B.commit_pinout();    // <- P08.LULA
  /* PIN_20 */ changed |= ext_pins.D3_C.clear_preset();    // -> P08.SAZA,SELO
  /* PIN_20 */ changed |= ext_pins.D3_D.commit_pinout();    // <- P08.SEZE
  /* PIN_21 */ changed |= ext_pins.D4_A.commit_pinout();    // <- P08.RORY
  /* PIN_21 */ changed |= ext_pins.D4_B.commit_pinout();    // <- P08.LULA
  /* PIN_21 */ changed |= ext_pins.D4_C.clear_preset();    // -> P08.TEHE,SODY
  /* PIN_21 */ changed |= ext_pins.D4_D.commit_pinout();    // <- P08.RESY
  /* PIN_22 */ changed |= ext_pins.D5_A.commit_pinout();    // <- P08.RYVO
  /* PIN_22 */ changed |= ext_pins.D5_B.commit_pinout();    // <- P08.LULA
  /* PIN_22 */ changed |= ext_pins.D5_C.clear_preset();    // -> P08.RATU,SAGO
  /* PIN_22 */ changed |= ext_pins.D5_D.commit_pinout();    // <- P08.TAMU
  /* PIN_23 */ changed |= ext_pins.D6_A.commit_pinout();    // <- P08.RAFY
  /* PIN_23 */ changed |= ext_pins.D6_B.commit_pinout();    // <- P08.LULA
  /* PIN_23 */ changed |= ext_pins.D6_C.clear_preset();    // -> P08.SOCA,RUPA
  /* PIN_23 */ changed |= ext_pins.D6_D.commit_pinout();    // <- P08.ROGY
  /* PIN_24 */ changed |= ext_pins.D7_A.commit_pinout();    // <- P08.RAVU
  /* PIN_24 */ changed |= ext_pins.D7_B.commit_pinout();    // <- P08.LULA
  /* PIN_24 */ changed |= ext_pins.D7_C.clear_preset();    // -> P08.RYBA,SAZY
  /* PIN_24 */ changed |= ext_pins.D7_D.commit_pinout();    // <- P08.RYDA
  /* PIN_25 */ changed |= vram_pins.MD7_A.commit_pinout();   // <- P25.RYZE
  /* PIN_25 */ changed |= vram_pins.MD7_B.commit_pinout();   // <- P25.ROFA
  /* PIN_25 */ changed |= vram_pins.MD7_C.clear_preset();   // -> P25.RAKU
  /* PIN_25 */ changed |= vram_pins.MD7_D.commit_pinout();   // <- P25.RADY
  /* PIN_26 */ changed |= vram_pins.MD6_A.commit_pinout();   // <- P25.REKU
  /* PIN_26 */ changed |= vram_pins.MD6_B.commit_pinout();   // <- P25.ROFA
  /* PIN_26 */ changed |= vram_pins.MD6_C.clear_preset();   // -> P25.RETA
  /* PIN_26 */ changed |= vram_pins.MD6_D.commit_pinout();   // <- P25.RYTY
  /* PIN_27 */ changed |= vram_pins.MD5_A.commit_pinout();   // <- P25.REVU
  /* PIN_27 */ changed |= vram_pins.MD5_B.commit_pinout();   // <- P25.ROFA
  /* PIN_27 */ changed |= vram_pins.MD5_C.clear_preset();   // -> P25.ROPU
  /* PIN_27 */ changed |= vram_pins.MD5_D.commit_pinout();   // <- P25.RUMU
  /* PIN_28 */ changed |= vram_pins.MD4_A.commit_pinout();   // <- P25.RYRO
  /* PIN_28 */ changed |= vram_pins.MD4_B.commit_pinout();   // <- P25.ROFA
  /* PIN_28 */ changed |= vram_pins.MD4_C.clear_preset();   // -> P25.ROCE
  /* PIN_28 */ changed |= vram_pins.MD4_D.commit_pinout();   // <- P25.RUBE
  /* PIN_29 */ changed |= vram_pins.MD3_A.commit_pinout();   // <- P25.RADA
  /* PIN_29 */ changed |= vram_pins.MD3_B.commit_pinout();   // <- P25.ROFA
  /* PIN_29 */ changed |= vram_pins.MD3_C.clear_preset();   // -> P25.REMO
  /* PIN_29 */ changed |= vram_pins.MD3_D.commit_pinout();   // <- P25.RODU
  /* PIN_30 */ changed |= vram_pins.MD2_A.commit_pinout();   // <- P25.RAZO
  /* PIN_30 */ changed |= vram_pins.MD2_B.commit_pinout();   // <- P25.ROFA
  /* PIN_30 */ changed |= vram_pins.MD2_C.clear_preset();   // -> P25.RYDO
  /* PIN_30 */ changed |= vram_pins.MD2_D.commit_pinout();   // <- P25.RARE
  /* PIN_31 */ changed |= vram_pins.MD1_A.commit_pinout();   // <- P25.RYKY
  /* PIN_31 */ changed |= vram_pins.MD1_B.commit_pinout();   // <- P25.ROFA
  /* PIN_31 */ changed |= vram_pins.MD1_C.clear_preset();   // -> P25.REBA
  /* PIN_31 */ changed |= vram_pins.MD1_D.commit_pinout();   // <- P25.RULY
  /* PIN_32 */ /*GND*/
  /* PIN_33 */ changed |= vram_pins.MD0_A.commit_pinout();   // <- P25.REGE
  /* PIN_33 */ changed |= vram_pins.MD0_B.commit_pinout();   // <- P25.ROFA
  /* PIN_33 */ changed |= vram_pins.MD0_C.clear_preset();   // -> P25.RODY
  /* PIN_33 */ changed |= vram_pins.MD0_D.commit_pinout();   // <- P25.RURA
  /* PIN_34 */ changed |= vram_pins.MA00_AD.commit_pinout();  // <- P04.ECAL
  /* PIN_35 */ changed |= vram_pins.MA01_AD.commit_pinout();  // <- P04.EGEZ
  /* PIN_36 */ changed |= vram_pins.MA02_AD.commit_pinout();  // <- P04.FUHE
  /* PIN_37 */ changed |= vram_pins.MA03_AD.commit_pinout();  // <- P04.FYZY
  /* PIN_38 */ changed |= vram_pins.MA04_AD.commit_pinout();  // <- P04.DAMU
  /* PIN_39 */ changed |= vram_pins.MA05_AD.commit_pinout();  // <- P04.DAVA
  /* PIN_40 */ changed |= vram_pins.MA06_AD.commit_pinout();  // <- P04.ETEG
  /* PIN_41 */ changed |= vram_pins.MA07_AD.commit_pinout();  // <- P04.EREW
  /* PIN_42 */ changed |= vram_pins.MA12_AD.commit_pinout();  // <- P04.EXYF
  /* PIN_43 */ changed |= vram_pins.MCSn_A.commit_pinout();   // <- P25.SOKY
  /* PIN_43 */ changed |= vram_pins.MCS_C.clear_preset();   // -> P25.TEFY
  /* PIN_43 */ changed |= vram_pins.MCSn_D.commit_pinout();   // <- P25.SETY
  /* PIN_44 */ changed |= vram_pins.MA10_AD.commit_pinout();  // <- P04.ERAF
  /* PIN_45 */ changed |= vram_pins.MOEn_A.commit_pinout();   // <- P25.REFO
  /* PIN_45 */ changed |= vram_pins.MOE_C.clear_preset();   // -> P25.TAVY
  /* PIN_45 */ changed |= vram_pins.MOEn_D.commit_pinout();   // <- P25.SAHA
  /* PIN_46 */ changed |= vram_pins.MA11_AD.commit_pinout();  // <- P04.FUSY
  /* PIN_47 */ changed |= vram_pins.MA09_AD.commit_pinout();  // <- P04.DUVE
  /* PIN_48 */ changed |= vram_pins.MA08_AD.commit_pinout();  // <- P04.EVAX
  /* PIN_49 */ changed |= vram_pins.MWRn_A.commit_pinout();   // <- P25.SYSY
  /* PIN_49 */ changed |= vram_pins.MWR_C.clear_preset();   // -> P25.SUDO
  /* PIN_49 */ changed |= vram_pins.MWRn_D.commit_pinout();   // <- P25.RAGU
  
  changed |= lcd_reg.commit_pins();
  
  /* PIN_58 */ /*VCC*/
  /* PIN_59 */ /*ROUT*/
  /* PIN_60 */ /*LOUT*/
  /* PIN_61 */ /*VIN*/
  /* PIN_62 */ changed |= joy_pin.P15_A.commit_pinout();   // <- p05.CELA
  /* PIN_62 */ changed |= joy_pin.P15_D.commit_pinout();   // <- p05.COFY
  /* PIN_63 */ changed |= joy_pin.P14_A.commit_pinout();   // <- p05.KARU
  /* PIN_63 */ changed |= joy_pin.P14_D.commit_pinout();   // <- p05.KELY
  /* PIN_64 */ changed |= joy_pin.P13_A.commit_pinout();   // <- P05.KORY
  /* PIN_64 */ changed |= joy_pin.P13_B.commit_pinout();   
  /* PIN_64 */ changed |= joy_pin.P13_C.clear_preset();   // -> P02.KERY, P05.KOLO
  /* PIN_64 */ changed |= joy_pin.P13_D.commit_pinout();   // <- P05.KALE
  /* PIN_65 */ changed |= joy_pin.P12_A.commit_pinout();   // <- P05.KYHU
  /* PIN_65 */ changed |= joy_pin.P12_B.commit_pinout();   
  /* PIN_65 */ changed |= joy_pin.P12_C.clear_preset();   // -> P02.KERY, P05.KEJA
  /* PIN_65 */ changed |= joy_pin.P12_D.commit_pinout();   // <- P05.KASY
  /* PIN_66 */ changed |= joy_pin.P11_A.commit_pinout();   // <- P05.KYTO
  /* PIN_66 */ changed |= joy_pin.P11_B.commit_pinout();   
  /* PIN_66 */ changed |= joy_pin.P11_C.clear_preset();   // -> P02.KERY, P05.KAPA
  /* PIN_66 */ changed |= joy_pin.P11_D.commit_pinout();   // <- P05.KABU
  /* PIN_67 */ changed |= joy_pin.P10_A.commit_pinout();   // <- P05.KOLE
  /* PIN_67 */ changed |= joy_pin.P10_B.commit_pinout();   
  /* PIN_67 */ changed |= joy_pin.P10_C.clear_preset();   // -> P02.KERY, P05.KEVU
  /* PIN_67 */ changed |= joy_pin.P10_D.commit_pinout();   // <- P05.KYBU

  changed |= ser_reg.commit_pins();

  /* PIN_71 */ changed |= sys_pins.RST.clear_preset();
  /* PIN_72 */ /*GND*/
  /* PIN_73 */ /*CLKOUT*/
  /* PIN_74 */ changed |= sys_pins.CLK_GOOD.clear_preset();
  /* PIN_74 */ changed |= sys_pins.CLK_IN_xBxDxFxH.clear_preset();
  /* PIN_75 */ changed |= ext_pins.PHI.commit_pinout();     // <- P01.BUDE/BEVA
  /* PIN_76 */ changed |= sys_pins.T2.clear_preset();
  /* PIN_77 */ changed |= sys_pins.T1.clear_preset();
  /* PIN_78 */ changed |= ext_pins.WR_A.commit_pinout();   // <- P08.UVER
  /* PIN_78 */ changed |= ext_pins.WR_C.clear_preset();   // -> P07.UBAL
  /* PIN_78 */ changed |= ext_pins.WR_D.commit_pinout();   // <- P08.USUF
  /* PIN_79 */ changed |= ext_pins.RD_A.commit_pinout();   // <- P08.UGAC
  /* PIN_79 */ changed |= ext_pins.RD_C.clear_preset();   // -> P07.UJYV
  /* PIN_79 */ changed |= ext_pins.RD_D.commit_pinout();   // <- P08.URUN
  /* PIN_80 */ changed |= ext_pins.CS_A.commit_pinout();   // <- P08.TYHO

  return changed;
}

//-----------------------------------------------------------------------------
