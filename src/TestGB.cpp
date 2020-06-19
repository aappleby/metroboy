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

  /*p29.FEPO*/ FEPO_STORE_MATCH.reset();
  /*p29.GUVA*/ SPRITE0_GET.reset();
  /*p29.ENUT*/ SPRITE1_GET.reset();
  /*p29.EMOL*/ SPRITE2_GET.reset();
  /*p29.GYFY*/ SPRITE3_GET.reset();
  /*p29.GONO*/ SPRITE4_GET.reset();
  /*p29.GEGA*/ SPRITE5_GET.reset();
  /*p29.XOJA*/ SPRITE6_GET.reset();
  /*p29.GUTU*/ SPRITE7_GET.reset();
  /*p29.FOXA*/ SPRITE8_GET.reset();
  /*p29.GUZE*/ SPRITE9_GET.reset();

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
  /*p23.XONA*/ changed |= cfg_reg.LCDC_EN.commit_reg();
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
  /*p23.SYRY*/ changed |= cfg_reg.LYC0.commit_reg();
  /*p23.VUCE*/ changed |= cfg_reg.LYC1.commit_reg();
  /*p23.SEDY*/ changed |= cfg_reg.LYC2.commit_reg();
  /*p23.SALO*/ changed |= cfg_reg.LYC3.commit_reg();
  /*p23.SOTA*/ changed |= cfg_reg.LYC4.commit_reg();
  /*p23.VAFA*/ changed |= cfg_reg.LYC5.commit_reg();
  /*p23.VEVO*/ changed |= cfg_reg.LYC6.commit_reg();
  /*p23.RAHA*/ changed |= cfg_reg.LYC7.commit_reg();
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

  /*p04.MAKA*/ changed |= dma_reg.FROM_CPU5_SYNC.commit_reg();
  /*p04.MATU*/ changed |= dma_reg.DMA_RUNNINGp.commit_reg(); // -> p25,p28
  /*p04.MYTE*/ changed |= dma_reg.MYTE.commit_reg();
  /*p04.LUVY*/ changed |= dma_reg.LUVY.commit_reg();
  /*p04.LENE*/ changed |= dma_reg.LENE.commit_reg();
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
  /*p04.LYXE*/ changed |= dma_reg.LYXE.commit_latch();

  // NAND latch
  /*p04.LARA*/ changed |= dma_reg.LARA.commit_gate();
  /*p04.LOKY*/ changed |= dma_reg.LOKY.commit_gate();

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

  /*p21.SAXO*/ changed |= lcd_reg.X0.commit_reg(); // increments at phase 1, reset to 0 at p909.
  /*p21.TYPO*/ changed |= lcd_reg.X1.commit_reg();
  /*p21.VYZO*/ changed |= lcd_reg.X2.commit_reg();
  /*p21.TELU*/ changed |= lcd_reg.X3.commit_reg();
  /*p21.SUDE*/ changed |= lcd_reg.X4.commit_reg();
  /*p21.TAHA*/ changed |= lcd_reg.X5.commit_reg();
  /*p21.TYRY*/ changed |= lcd_reg.X6.commit_reg();
  /*p21.MUWY*/ changed |= lcd_reg.Y0.commit_reg(); // increments at p909, reset to 0 at p153:001
  /*p21.MYRO*/ changed |= lcd_reg.Y1.commit_reg();
  /*p21.LEXA*/ changed |= lcd_reg.Y2.commit_reg();
  /*p21.LYDO*/ changed |= lcd_reg.Y3.commit_reg();
  /*p21.LOVU*/ changed |= lcd_reg.Y4.commit_reg();
  /*p21.LEMA*/ changed |= lcd_reg.Y5.commit_reg();
  /*p21.MATO*/ changed |= lcd_reg.Y6.commit_reg();
  /*p21.LAFO*/ changed |= lcd_reg.Y7.commit_reg();
  /*p21.RUTU*/ changed |= lcd_reg.NEW_LINE_d0.commit_reg(); // p909+8
  /*p29.CATU*/ changed |= lcd_reg.VID_LINE_d4.commit_reg();  // p001+8
  /*p21.NYPE*/ changed |= lcd_reg.NEW_LINE_d4.commit_reg(); // p001+8
  /*p28.ANEL*/ changed |= lcd_reg.VID_LINE_d6.commit_reg();  // p003+8
  /*p21.MYTA*/ changed |= lcd_reg.LINE_153_d4.commit_reg();  // p153:001 - p000:001
  /*p21.POPU*/ changed |= lcd_reg.IN_VBLANK_d4.commit_reg();    // p144:001 - p000:001
  /*p21.SYGU*/ changed |= lcd_reg.LINE_STROBE.commit_reg();
  /*p24.PAHO*/ changed |= lcd_reg.X_8_SYNC.commit_reg();
  /*p21.WUSA*/ changed |= lcd_reg.CPEN_LATCH.commit_latch();
  /*p24.POME*/ changed |= lcd_reg.POME.commit_latch(); // nand latch with p24.RUJU
  /*p24.RUJU*/ changed |= lcd_reg.RUJU.commit_latch(); // nand latch with p24.POME
  /*p24.MEDA*/ changed |= lcd_reg.VSYNC_OUTn.commit_reg();
  /*p24.LUCA*/ changed |= lcd_reg.LINE_EVEN.commit_reg();
  /*p21.NAPO*/ changed |= lcd_reg.FRAME_EVEN.commit_reg();

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

  /*p29.CENO*/ changed |= spr_reg.STORE_SPRITE_IDXn.commit_reg();
  /*p28.YFEL*/ changed |= spr_reg.SCAN0.commit_reg();
  /*p28.WEWY*/ changed |= spr_reg.SCAN1.commit_reg();
  /*p28.GOSO*/ changed |= spr_reg.SCAN2.commit_reg();
  /*p28.ELYN*/ changed |= spr_reg.SCAN3.commit_reg();
  /*p28.FAHA*/ changed |= spr_reg.SCAN4.commit_reg();
  /*p28.FONY*/ changed |= spr_reg.SCAN5.commit_reg();
  /*p30.XADU*/ changed |= spr_reg.SPRITE_IDX0.commit_reg();
  /*p30.XEDY*/ changed |= spr_reg.SPRITE_IDX1.commit_reg();
  /*p30.ZUZE*/ changed |= spr_reg.SPRITE_IDX2.commit_reg();
  /*p30.XOBE*/ changed |= spr_reg.SPRITE_IDX3.commit_reg();
  /*p30.YDUF*/ changed |= spr_reg.SPRITE_IDX4.commit_reg();
  /*p30.XECU*/ changed |= spr_reg.SPRITE_IDX5.commit_reg();
  /*p29.BYBA*/ changed |= spr_reg.SCAN_DONE_d4.commit_reg();
  /*p29.DOBA*/ changed |= spr_reg.SCAN_DONE_d5.commit_reg();
  /*p29.TOXE*/ changed |= spr_reg.TOXE_SPR_SEQ0.commit_reg();
  /*p29.TULY*/ changed |= spr_reg.TULY_SPR_SEQ1.commit_reg();
  /*p29.TESE*/ changed |= spr_reg.TESE_SPR_SEQ2.commit_reg();
  /*p29.TOBU*/ changed |= spr_reg.TOBU_SEQ_xx23xx.commit_reg();
  /*p29.VONU*/ changed |= spr_reg.VONU_SEQ_xxx34xn.commit_reg();
  /*p29.SEBA*/ changed |= spr_reg.SEBA_SEQ_xxxx45n.commit_reg();
  /*p29.TYFO*/ changed |= spr_reg.TYFO_SEQ_B0d.commit_reg();
  changed |= spr_reg.TS_IDX_0.commit_tribuf();
  changed |= spr_reg.TS_IDX_1.commit_tribuf();
  changed |= spr_reg.TS_IDX_2.commit_tribuf();
  changed |= spr_reg.TS_IDX_3.commit_tribuf();
  changed |= spr_reg.TS_IDX_4.commit_tribuf();
  changed |= spr_reg.TS_IDX_5.commit_tribuf();
  changed |= spr_reg.TS_LINE_0.commit_tribuf();
  changed |= spr_reg.TS_LINE_1.commit_tribuf();
  changed |= spr_reg.TS_LINE_2.commit_tribuf();
  changed |= spr_reg.TS_LINE_3.commit_tribuf();

  /*p29.DEZY*/ changed |= sst_reg.STORE_EN_SYNC.commit_reg();
  /*p29.BESE*/ changed |= sst_reg.SPRITE_COUNT0.commit_reg();
  /*p29.CUXY*/ changed |= sst_reg.SPRITE_COUNT1.commit_reg();
  /*p29.BEGO*/ changed |= sst_reg.SPRITE_COUNT2.commit_reg();
  /*p29.DYBE*/ changed |= sst_reg.SPRITE_COUNT3.commit_reg();
  /*p29.EBOJ*/ changed |= sst_reg.SPRITE0_GET_SYNC.commit_reg();
  /*p30.YGUS*/ changed |= sst_reg.STORE0_IDX0.commit_reg();
  /*p30.YSOK*/ changed |= sst_reg.STORE0_IDX1.commit_reg();
  /*p30.YZEP*/ changed |= sst_reg.STORE0_IDX2.commit_reg();
  /*p30.WYTE*/ changed |= sst_reg.STORE0_IDX3.commit_reg();
  /*p30.ZONY*/ changed |= sst_reg.STORE0_IDX4.commit_reg();
  /*p30.YWAK*/ changed |= sst_reg.STORE0_IDX5.commit_reg();
  /*p30.FYHY*/ changed |= sst_reg.STORE0_LINE0.commit_reg();
  /*p30.GYHO*/ changed |= sst_reg.STORE0_LINE1.commit_reg();
  /*p30.BOZU*/ changed |= sst_reg.STORE0_LINE2.commit_reg();
  /*p30.CUFO*/ changed |= sst_reg.STORE0_LINE3.commit_reg();
  /*p31.XEPE*/ changed |= sst_reg.STORE0_X0.commit_reg();
  /*p31.YLAH*/ changed |= sst_reg.STORE0_X1.commit_reg();
  /*p31.ZOLA*/ changed |= sst_reg.STORE0_X2.commit_reg();
  /*p31.ZULU*/ changed |= sst_reg.STORE0_X3.commit_reg();
  /*p31.WELO*/ changed |= sst_reg.STORE0_X4.commit_reg();
  /*p31.XUNY*/ changed |= sst_reg.STORE0_X5.commit_reg();
  /*p31.WOTE*/ changed |= sst_reg.STORE0_X6.commit_reg();
  /*p31.XAKO*/ changed |= sst_reg.STORE0_X7.commit_reg();
  /*p29.CEDY*/ changed |= sst_reg.SPRITE1_GET_SYNCn.commit_reg();
  /*p30.CAJY*/ changed |= sst_reg.STORE1_IDX0.commit_reg();
  /*p30.CUZA*/ changed |= sst_reg.STORE1_IDX1.commit_reg();
  /*p30.COMA*/ changed |= sst_reg.STORE1_IDX2.commit_reg();
  /*p30.CUFA*/ changed |= sst_reg.STORE1_IDX3.commit_reg();
  /*p30.CEBO*/ changed |= sst_reg.STORE1_IDX4.commit_reg();
  /*p30.CADU*/ changed |= sst_reg.STORE1_IDX5.commit_reg();
  /*p30.ABUG*/ changed |= sst_reg.STORE1_LINE0.commit_reg();
  /*p30.AMES*/ changed |= sst_reg.STORE1_LINE1.commit_reg();
  /*p30.ABOP*/ changed |= sst_reg.STORE1_LINE2.commit_reg();
  /*p30.AROF*/ changed |= sst_reg.STORE1_LINE3.commit_reg();
  /*p31.DANY*/ changed |= sst_reg.STORE1_X0.commit_reg();
  /*p31.DUKO*/ changed |= sst_reg.STORE1_X1.commit_reg();
  /*p31.DESU*/ changed |= sst_reg.STORE1_X2.commit_reg();
  /*p31.DAZO*/ changed |= sst_reg.STORE1_X3.commit_reg();
  /*p31.DAKE*/ changed |= sst_reg.STORE1_X4.commit_reg();
  /*p31.CESO*/ changed |= sst_reg.STORE1_X5.commit_reg();
  /*p31.DYFU*/ changed |= sst_reg.STORE1_X6.commit_reg();
  /*p31.CUSY*/ changed |= sst_reg.STORE1_X7.commit_reg();
  /*p29.EGAV*/ changed |= sst_reg.SPRITE2_GET_SYNCn.commit_reg();
  /*p30.BOXA*/ changed |= sst_reg.STORE2_IDX0.commit_reg();
  /*p30.BUNA*/ changed |= sst_reg.STORE2_IDX1.commit_reg();
  /*p30.BULU*/ changed |= sst_reg.STORE2_IDX2.commit_reg();
  /*p30.BECA*/ changed |= sst_reg.STORE2_IDX3.commit_reg();
  /*p30.BYHU*/ changed |= sst_reg.STORE2_IDX4.commit_reg();
  /*p30.BUHE*/ changed |= sst_reg.STORE2_IDX5.commit_reg();
  /*p30.YKUK*/ changed |= sst_reg.STORE2_LINE0.commit_reg();
  /*p30.YLOV*/ changed |= sst_reg.STORE2_LINE1.commit_reg();
  /*p30.XAZY*/ changed |= sst_reg.STORE2_LINE2.commit_reg();
  /*p30.XOSY*/ changed |= sst_reg.STORE2_LINE3.commit_reg();
  /*p31.FOKA*/ changed |= sst_reg.STORE2_X0.commit_reg();
  /*p31.FYTY*/ changed |= sst_reg.STORE2_X1.commit_reg();
  /*p31.FUBY*/ changed |= sst_reg.STORE2_X2.commit_reg();
  /*p31.GOXU*/ changed |= sst_reg.STORE2_X3.commit_reg();
  /*p31.DUHY*/ changed |= sst_reg.STORE2_X4.commit_reg();
  /*p31.EJUF*/ changed |= sst_reg.STORE2_X5.commit_reg();
  /*p31.ENOR*/ changed |= sst_reg.STORE2_X6.commit_reg();
  /*p31.DEPY*/ changed |= sst_reg.STORE2_X7.commit_reg();
  /*p29.GOTA*/ changed |= sst_reg.SPRITE3_GET_SYNCn.commit_reg();
  /*p30.DAFU*/ changed |= sst_reg.DAFU.commit_reg();
  /*p30.DEBA*/ changed |= sst_reg.DEBA.commit_reg();
  /*p30.DUHA*/ changed |= sst_reg.DUHA.commit_reg();
  /*p30.DUNY*/ changed |= sst_reg.DUNY.commit_reg();
  /*p30.DESE*/ changed |= sst_reg.DESE.commit_reg();
  /*p30.DEVY*/ changed |= sst_reg.DEVY.commit_reg();
  /*p30.ZURY*/ changed |= sst_reg.ZURY.commit_reg();
  /*p30.ZURO*/ changed |= sst_reg.ZURO.commit_reg();
  /*p30.ZENE*/ changed |= sst_reg.ZENE.commit_reg();
  /*p30.ZYLU*/ changed |= sst_reg.ZYLU.commit_reg();
  /*p31.XOLY*/ changed |= sst_reg.XOLY.commit_reg();
  /*p31.XYBA*/ changed |= sst_reg.XYBA.commit_reg();
  /*p31.XABE*/ changed |= sst_reg.XABE.commit_reg();
  /*p31.XEKA*/ changed |= sst_reg.XEKA.commit_reg();
  /*p31.XOMY*/ changed |= sst_reg.XOMY.commit_reg();
  /*p31.WUHA*/ changed |= sst_reg.WUHA.commit_reg();
  /*p31.WYNA*/ changed |= sst_reg.WYNA.commit_reg();
  /*p31.WECO*/ changed |= sst_reg.WECO.commit_reg();
  /*p29.XUDY*/ changed |= sst_reg.SPRITE4_GET_SYNC.commit_reg();
  /*p31.WEDU*/ changed |= sst_reg.WEDU.commit_reg();
  /*p31.YGAJ*/ changed |= sst_reg.YGAJ.commit_reg();
  /*p31.ZYJO*/ changed |= sst_reg.ZYJO.commit_reg();
  /*p31.XURY*/ changed |= sst_reg.XURY.commit_reg();
  /*p31.YBED*/ changed |= sst_reg.YBED.commit_reg();
  /*p31.ZALA*/ changed |= sst_reg.ZALA.commit_reg();
  /*p31.WYDE*/ changed |= sst_reg.WYDE.commit_reg();
  /*p31.XEPA*/ changed |= sst_reg.XEPA.commit_reg();
  /*p30.XYNU*/ changed |= sst_reg.XYNU.commit_reg();
  /*p30.XEGE*/ changed |= sst_reg.XEGE.commit_reg();
  /*p30.XABO*/ changed |= sst_reg.XABO.commit_reg();
  /*p30.WANU*/ changed |= sst_reg.WANU.commit_reg();
  /*p30.XEFE*/ changed |= sst_reg.XEFE.commit_reg();
  /*p30.XAVE*/ changed |= sst_reg.XAVE.commit_reg();
  /*p30.CUMU*/ changed |= sst_reg.CUMU.commit_reg();
  /*p30.CAPO*/ changed |= sst_reg.CAPO.commit_reg();
  /*p30.CONO*/ changed |= sst_reg.CONO.commit_reg();
  /*p30.CAJU*/ changed |= sst_reg.CAJU.commit_reg();
  /*p29.WAFY*/ changed |= sst_reg.SPRITE5_GET_SYNC.commit_reg();
  /*p31.FUSA*/ changed |= sst_reg.FUSA.commit_reg();
  /*p31.FAXA*/ changed |= sst_reg.FAXA.commit_reg();
  /*p31.FOZY*/ changed |= sst_reg.FOZY.commit_reg();
  /*p31.FESY*/ changed |= sst_reg.FESY.commit_reg();
  /*p31.CYWE*/ changed |= sst_reg.CYWE.commit_reg();
  /*p31.DYBY*/ changed |= sst_reg.DYBY.commit_reg();
  /*p31.DURY*/ changed |= sst_reg.DURY.commit_reg();
  /*p31.CUVY*/ changed |= sst_reg.CUVY.commit_reg();
  /*p30.EKAP*/ changed |= sst_reg.EKAP.commit_reg();
  /*p30.ETAV*/ changed |= sst_reg.ETAV.commit_reg();
  /*p30.EBEX*/ changed |= sst_reg.EBEX.commit_reg();
  /*p30.GORU*/ changed |= sst_reg.GORU.commit_reg();
  /*p30.ETYM*/ changed |= sst_reg.ETYM.commit_reg();
  /*p30.EKOP*/ changed |= sst_reg.EKOP.commit_reg();
  /*p30.ANED*/ changed |= sst_reg.ANED.commit_reg();
  /*p30.ACEP*/ changed |= sst_reg.ACEP.commit_reg();
  /*p30.ABUX*/ changed |= sst_reg.ABUX.commit_reg();
  /*p30.ABEG*/ changed |= sst_reg.ABEG.commit_reg();
  /*p29.WOMY*/ changed |= sst_reg.SPRITE6_GET_SYNCn.commit_reg();
  /*p31.YCOL*/ changed |= sst_reg.YCOL.commit_reg();
  /*p31.YRAC*/ changed |= sst_reg.YRAC.commit_reg();
  /*p31.YMEM*/ changed |= sst_reg.YMEM.commit_reg();
  /*p31.YVAG*/ changed |= sst_reg.YVAG.commit_reg();
  /*p31.ZOLY*/ changed |= sst_reg.ZOLY.commit_reg();
  /*p31.ZOGO*/ changed |= sst_reg.ZOGO.commit_reg();
  /*p31.ZECU*/ changed |= sst_reg.ZECU.commit_reg();
  /*p31.ZESA*/ changed |= sst_reg.ZESA.commit_reg();
  /*p30.GECU*/ changed |= sst_reg.GECU.commit_reg();
  /*p30.FOXY*/ changed |= sst_reg.FOXY.commit_reg();
  /*p30.GOHU*/ changed |= sst_reg.GOHU.commit_reg();
  /*p30.FOGO*/ changed |= sst_reg.FOGO.commit_reg();
  /*p30.GACY*/ changed |= sst_reg.GACY.commit_reg();
  /*p30.GABO*/ changed |= sst_reg.GABO.commit_reg();
  /*p30.ZUBE*/ changed |= sst_reg.ZUBE.commit_reg();
  /*p30.ZUMY*/ changed |= sst_reg.ZUMY.commit_reg();
  /*p30.ZEXO*/ changed |= sst_reg.ZEXO.commit_reg();
  /*p30.ZAFU*/ changed |= sst_reg.ZAFU.commit_reg();
  /*p29.WAPO*/ changed |= sst_reg.SPRITE7_GET_SYNC.commit_reg();
  /*p31.ERAZ*/ changed |= sst_reg.STORE7_X0.commit_reg();
  /*p31.EPUM*/ changed |= sst_reg.STORE7_X1.commit_reg();
  /*p31.EROL*/ changed |= sst_reg.STORE7_X2.commit_reg();
  /*p31.EHYN*/ changed |= sst_reg.STORE7_X3.commit_reg();
  /*p31.FAZU*/ changed |= sst_reg.STORE7_X4.commit_reg();
  /*p31.FAXE*/ changed |= sst_reg.STORE7_X5.commit_reg();
  /*p31.EXUK*/ changed |= sst_reg.STORE7_X6.commit_reg();
  /*p31.FEDE*/ changed |= sst_reg.STORE7_X7.commit_reg();
  /*p30.FUZO*/ changed |= sst_reg.FUZO.commit_reg();
  /*p30.GESY*/ changed |= sst_reg.GESY.commit_reg();
  /*p30.FYSU*/ changed |= sst_reg.FYSU.commit_reg();
  /*p30.FEFA*/ changed |= sst_reg.FEFA.commit_reg();
  /*p30.GYNO*/ changed |= sst_reg.GYNO.commit_reg();
  /*p30.GULE*/ changed |= sst_reg.GULE.commit_reg();
  /*p30.XYGO*/ changed |= sst_reg.XYGO.commit_reg();
  /*p30.XYNA*/ changed |= sst_reg.XYNA.commit_reg();
  /*p30.XAKU*/ changed |= sst_reg.XAKU.commit_reg();
  /*p30.YGUM*/ changed |= sst_reg.YGUM.commit_reg();
  /*p29.EXUQ*/ changed |= sst_reg.SPRITE8_GET_SYNCn.commit_reg();
  /*p31.GAVY*/ changed |= sst_reg.GAVY.commit_reg();
  /*p31.GYPU*/ changed |= sst_reg.GYPU.commit_reg();
  /*p31.GADY*/ changed |= sst_reg.GADY.commit_reg();
  /*p31.GAZA*/ changed |= sst_reg.GAZA.commit_reg();
  /*p31.EZUF*/ changed |= sst_reg.EZUF.commit_reg();
  /*p31.ENAD*/ changed |= sst_reg.ENAD.commit_reg();
  /*p31.EBOW*/ changed |= sst_reg.EBOW.commit_reg();
  /*p31.FYCA*/ changed |= sst_reg.FYCA.commit_reg();
  /*p30.AXUV*/ changed |= sst_reg.AXUV.commit_reg();
  /*p30.BADA*/ changed |= sst_reg.BADA.commit_reg();
  /*p30.APEV*/ changed |= sst_reg.APEV.commit_reg();
  /*p30.BADO*/ changed |= sst_reg.BADO.commit_reg();
  /*p30.BEXY*/ changed |= sst_reg.BEXY.commit_reg();
  /*p30.BYHE*/ changed |= sst_reg.BYHE.commit_reg();
  /*p30.AFYM*/ changed |= sst_reg.AFYM.commit_reg();
  /*p30.AZAP*/ changed |= sst_reg.AZAP.commit_reg();
  /*p30.AFUT*/ changed |= sst_reg.AFUT.commit_reg();
  /*p30.AFYX*/ changed |= sst_reg.AFYX.commit_reg();
  /*p29.FONO*/ changed |= sst_reg.SPRITE9_GET_SYNC.commit_reg();
  /*p31.XUVY*/ changed |= sst_reg.STORE9_X0.commit_reg();
  /*p31.XERE*/ changed |= sst_reg.STORE9_X1.commit_reg();
  /*p31.XUZO*/ changed |= sst_reg.STORE9_X2.commit_reg();
  /*p31.XEXA*/ changed |= sst_reg.STORE9_X3.commit_reg();
  /*p31.YPOD*/ changed |= sst_reg.STORE9_X4.commit_reg();
  /*p31.YROP*/ changed |= sst_reg.STORE9_X5.commit_reg();
  /*p31.YNEP*/ changed |= sst_reg.STORE9_X6.commit_reg();
  /*p31.YZOF*/ changed |= sst_reg.STORE9_X7.commit_reg();
  /*p30.XUFO*/ changed |= sst_reg.STORE9_IDX0.commit_reg();
  /*p30.XUTE*/ changed |= sst_reg.STORE9_IDX1.commit_reg();
  /*p30.XOTU*/ changed |= sst_reg.STORE9_IDX2.commit_reg();
  /*p30.XYFE*/ changed |= sst_reg.STORE9_IDX3.commit_reg();
  /*p30.YZOR*/ changed |= sst_reg.STORE9_IDX4.commit_reg();
  /*p30.YBER*/ changed |= sst_reg.STORE9_IDX5.commit_reg();
  /*p30.DEWU*/ changed |= sst_reg.STORE9_LINE0.commit_reg();
  /*p30.CANA*/ changed |= sst_reg.STORE9_LINE1.commit_reg();
  /*p30.DYSY*/ changed |= sst_reg.STORE9_LINE2.commit_reg();
  /*p30.FOFO*/ changed |= sst_reg.STORE9_LINE3.commit_reg();

  /*p01.UKUP*/ changed |= tim_reg.DIV_00.commit_reg();
  /*p01.UFOR*/ changed |= tim_reg.DIV_01.commit_reg();
  /*p01.UNER*/ changed |= tim_reg.DIV_02.commit_reg();
  /*p01.TERO*/ changed |= tim_reg.DIV_03.commit_reg();
  /*p01.UNYK*/ changed |= tim_reg.DIV_04.commit_reg();
  /*p01.TAMA*/ changed |= tim_reg.DIV_05.commit_reg();
  /*p01.UGOT*/ changed |= tim_reg.DIV_06.commit_reg();
  /*p01.TULU*/ changed |= tim_reg.DIV_07.commit_reg();
  /*p01.TUGO*/ changed |= tim_reg.DIV_08.commit_reg();
  /*p01.TOFE*/ changed |= tim_reg.DIV_09.commit_reg();
  /*p01.TERU*/ changed |= tim_reg.DIV_10.commit_reg();
  /*p01.SOLA*/ changed |= tim_reg.DIV_11.commit_reg();
  /*p01.SUBU*/ changed |= tim_reg.DIV_12.commit_reg();
  /*p01.TEKA*/ changed |= tim_reg.DIV_13.commit_reg();
  /*p01.UKET*/ changed |= tim_reg.DIV_14.commit_reg();
  /*p01.UPOF*/ changed |= tim_reg.DIV_15.commit_reg();
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

  /*p29.WUVU*/ changed |= vclk_reg.WUVU_AxxDExxH.commit_reg();
  /*p21.VENA*/ changed |= vclk_reg.VENA_xBCDExxx.commit_reg();
  /*p29.WOSU*/ changed |= vclk_reg.WOSU_xxCDxxGH.commit_reg();

  /*p??.ROXY*/ changed |= vid_reg.FINE_MATCH_LATCH.commit_latch();
  /*p??.PUXA*/ changed |= vid_reg.FINE_MATCH_SYNC1.commit_reg();
  /*p27.NYZE*/ changed |= vid_reg.FINE_MATCH_SYNC2.commit_reg();
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
  /*p27.SOVY*/ changed |= vid_reg.SOVY_WIN_MODE_SYNC.commit_reg();
  /*p21.XYMU*/ changed |= vid_reg.RENDERING_LATCH.commit_latch();
  /*p21.VOGA*/ changed |= vid_reg.RENDER_DONE_SYNC.commit_reg();
  /*p27.PYNU*/ changed |= vid_reg.PYNU_WIN_MODE_LATCH.commit_latch();
  /*p27.RYDY*/ changed |= vid_reg.RYDY_WIN_MODE_LATCH.commit_latch();
  /*p21.ROXE*/ changed |= vid_reg.INT_HBL_EN.commit_reg();
  /*p21.RUFO*/ changed |= vid_reg.INT_VBL_EN.commit_reg();
  /*p21.REFE*/ changed |= vid_reg.INT_OAM_EN.commit_reg();
  /*p21.RUGU*/ changed |= vid_reg.INT_LYC_EN.commit_reg();
  /*p21.ROPO*/ changed |= vid_reg.LY_MATCH_SYNC.commit_reg();
  /*p21.RUPO*/ changed |= vid_reg.LYC_MATCH_LATCHn.commit_latch();
  /*p27.SARY*/ changed |= vid_reg.WY_MATCH_SYNC.commit_reg();
  /*p27.RYFA*/ changed |= vid_reg.WIN_MATCH_ONSCREEN_SYNC1.commit_reg();
  /*p27.RENE*/ changed |= vid_reg.WIN_MATCH_ONSCREEN_SYNC2.commit_reg();
  /*p27.PYCO*/ changed |= vid_reg.WIN_MATCH_SYNC1.commit_reg();
  /*p27.NUNU*/ changed |= vid_reg.WIN_MATCH_SYNC2.commit_reg();
  /*p27.LONY*/ changed |= vid_reg.BG_READ_VRAM_LATCH.commit_latch();
  /*p27.LAXU*/ changed |= vid_reg.BG_SEQ_x1x3x5x7.commit_reg();
  /*p27.MESU*/ changed |= vid_reg.BG_SEQ_xx23xx67.commit_reg();
  /*p27.NYVA*/ changed |= vid_reg.BG_SEQ_xxxx4567.commit_reg();
  /*p27.LOVY*/ changed |= vid_reg.BG_SEQ5_SYNC.commit_reg();
  /*p24.NYKA*/ changed |= vid_reg.BG_SEQ_6.commit_reg();
  /*p24.PORY*/ changed |= vid_reg.BG_SEQ_7.commit_reg();
  /*p27.LYZU*/ changed |= vid_reg.BG_SEQ_x1x3x5x7_DELAY.commit_reg();
  /*p27.WYKA*/ changed |= vid_reg.MAP_X0.commit_reg();
  /*p27.WODY*/ changed |= vid_reg.MAP_X1.commit_reg();
  /*p27.WOBO*/ changed |= vid_reg.MAP_X2.commit_reg();
  /*p27.WYKO*/ changed |= vid_reg.MAP_X3.commit_reg();
  /*p27.XOLO*/ changed |= vid_reg.MAP_X4.commit_reg();
  /*p27.VYNO*/ changed |= vid_reg.TILE_Y0.commit_reg();
  /*p27.VUJO*/ changed |= vid_reg.TILE_Y1.commit_reg();
  /*p27.VYMU*/ changed |= vid_reg.TILE_Y2.commit_reg();
  /*p27.TUFU*/ changed |= vid_reg.MAP_Y0.commit_reg();
  /*p27.TAXA*/ changed |= vid_reg.MAP_Y1.commit_reg();
  /*p27.TOZO*/ changed |= vid_reg.MAP_Y2.commit_reg();
  /*p27.TATE*/ changed |= vid_reg.MAP_Y3.commit_reg();
  /*p27.TEKE*/ changed |= vid_reg.MAP_Y4.commit_reg();
  /*p24.PYGO*/ changed |= vid_reg.PYGO_TILE_DONE.commit_reg();
  /*p24.POKY*/ changed |= vid_reg.POKY_FRONT_PORCH_LATCHn.commit_latch();
  /*p27.REJO*/ changed |= vid_reg.WY_MATCH_LATCH.commit_latch();

  /*p27.TAKA*/ changed |= vid_reg._SPRITE_FETCH_LATCH.commit_latch();
  /*p27.SOBU*/ changed |= vid_reg._SPRITE_FETCH_SYNC1.commit_reg();
  /*p27.SUDA*/ changed |= vid_reg._SPRITE_FETCH_SYNC2.commit_reg();
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
  changed |= cpu_pins.ADDR_VALID.clear_preset();    // PORTA_06: -> P01.AGUT, P08.TEX0. This is almost definitely "address valid"
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

  changed |= oam_pins.CLK_A.commit_pinout();
  changed |= oam_pins.OE.commit_pinout();
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
  /* PIN_34 */ changed |= vram_pins.MA00_D.commit_pinout();  // <- P04.ECAL
  /* PIN_35 */ changed |= vram_pins.MA01_D.commit_pinout();  // <- P04.EGEZ
  /* PIN_36 */ changed |= vram_pins.MA02_D.commit_pinout();  // <- P04.FUHE
  /* PIN_37 */ changed |= vram_pins.MA03_D.commit_pinout();  // <- P04.FYZY
  /* PIN_38 */ changed |= vram_pins.MA04_D.commit_pinout();  // <- P04.DAMU
  /* PIN_39 */ changed |= vram_pins.MA05_D.commit_pinout();  // <- P04.DAVA
  /* PIN_40 */ changed |= vram_pins.MA06_D.commit_pinout();  // <- P04.ETEG
  /* PIN_41 */ changed |= vram_pins.MA07_D.commit_pinout();  // <- P04.EREW
  /* PIN_42 */ changed |= vram_pins.MA12_D.commit_pinout();  // <- P04.EXYF
  /* PIN_43 */ changed |= vram_pins.MCS_A.commit_pinout();   // <- P25.SOKY
  /* PIN_43 */ changed |= vram_pins.MCS_C.clear_preset();   // -> P25.TEFY
  /* PIN_43 */ changed |= vram_pins.MCS_D.commit_pinout();   // <- P25.SETY
  /* PIN_44 */ changed |= vram_pins.MA10_D.commit_pinout();  // <- P04.ERAF
  /* PIN_45 */ changed |= vram_pins.MOE_A.commit_pinout();   // <- P25.REFO
  /* PIN_45 */ changed |= vram_pins.MOE_C.clear_preset();   // -> P25.TAVY
  /* PIN_45 */ changed |= vram_pins.MOE_D.commit_pinout();   // <- P25.SAHA
  /* PIN_46 */ changed |= vram_pins.MA11_D.commit_pinout();  // <- P04.FUSY
  /* PIN_47 */ changed |= vram_pins.MA09_D.commit_pinout();  // <- P04.DUVE
  /* PIN_48 */ changed |= vram_pins.MA08_D.commit_pinout();  // <- P04.EVAX
  /* PIN_49 */ changed |= vram_pins.MWR_A.commit_pinout();   // <- P25.SYSY
  /* PIN_49 */ changed |= vram_pins.MWR_C.clear_preset();   // -> P25.SUDO
  /* PIN_49 */ changed |= vram_pins.MWR_D.commit_pinout();   // <- P25.RAGU
  /* PIN_50 */ changed |= lcd_pins.LD1.commit_pinout();
  /* PIN_51 */ changed |= lcd_pins.LD0.commit_pinout();
  /* PIN_52 */ changed |= lcd_pins.CPG.commit_pinout();
  /* PIN_53 */ changed |= lcd_pins.CP.commit_pinout();
  /* PIN_54 */ changed |= lcd_pins.ST.commit_pinout();
  /* PIN_55 */ changed |= lcd_pins.CPL.commit_pinout();
  /* PIN_56 */ changed |= lcd_pins.FR.commit_pinout();
  /* PIN_57 */ changed |= lcd_pins.S.commit_pinout();
  /* PIN_58 */ /*VCC*/
  /* PIN_59 */ /*ROUT*/
  /* PIN_60 */ /*LOUT*/
  /* PIN_61 */ /*VIN*/
  /* PIN_62 */ changed |= joy_pins.P15_A.commit_pinout();   // <- p05.CELA
  /* PIN_62 */ changed |= joy_pins.P15_D.commit_pinout();   // <- p05.COFY
  /* PIN_63 */ changed |= joy_pins.P14_A.commit_pinout();   // <- p05.KARU
  /* PIN_63 */ changed |= joy_pins.P14_D.commit_pinout();   // <- p05.KELY
  /* PIN_64 */ changed |= joy_pins.P13_A.commit_pinout();   // <- P05.KORY
  /* PIN_64 */ changed |= joy_pins.P13_B.commit_pinout();   
  /* PIN_64 */ changed |= joy_pins.P13_C.clear_preset();   // -> P02.KERY, P05.KOLO
  /* PIN_64 */ changed |= joy_pins.P13_D.commit_pinout();   // <- P05.KALE
  /* PIN_65 */ changed |= joy_pins.P12_A.commit_pinout();   // <- P05.KYHU
  /* PIN_65 */ changed |= joy_pins.P12_B.commit_pinout();   
  /* PIN_65 */ changed |= joy_pins.P12_C.clear_preset();   // -> P02.KERY, P05.KEJA
  /* PIN_65 */ changed |= joy_pins.P12_D.commit_pinout();   // <- P05.KASY
  /* PIN_66 */ changed |= joy_pins.P11_A.commit_pinout();   // <- P05.KYTO
  /* PIN_66 */ changed |= joy_pins.P11_B.commit_pinout();   
  /* PIN_66 */ changed |= joy_pins.P11_C.clear_preset();   // -> P02.KERY, P05.KAPA
  /* PIN_66 */ changed |= joy_pins.P11_D.commit_pinout();   // <- P05.KABU
  /* PIN_67 */ changed |= joy_pins.P10_A.commit_pinout();   // <- P05.KOLE
  /* PIN_67 */ changed |= joy_pins.P10_B.commit_pinout();   
  /* PIN_67 */ changed |= joy_pins.P10_C.clear_preset();   // -> P02.KERY, P05.KEVU
  /* PIN_67 */ changed |= joy_pins.P10_D.commit_pinout();   // <- P05.KYBU

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
