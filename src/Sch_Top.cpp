#include "Sch_Top.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

void SchematicTop::tick_everything() {
  clk_reg.tick(*this);
  dbg_reg.tick(*this);
  rst_reg.tick(*this);
  tim_reg.tick(*this);


  CPU_PIN_BOOTp.set(TUTU_BOOTp());


  /*
  dma_reg.tick(*this);
  tim_reg.tick(*this);
  ser_reg.tick(*this);

  joy_reg.tick(rst_sig, clk_reg, cpu_bus, cpu_sig);

  ppu_reg.tick(*this);
  sst_reg.tick(*this);
  lcd_reg.tick(*this);
  pxp_reg.tick(*this);
  //cpu_sig.tick(*this);
  cpu_pins_out.tick(*this);
  vram_pins.tick(*this);
  */
}

//-----------------------------------------------------------------------------

SignalHash SchematicTop::commit_everything() {
  SignalHash hash;

  /* PIN_01 */ hash << EXT_PIN_A00_C.clear_preset();      // -> KOVA
  /* PIN_02 */ hash << EXT_PIN_A01_C.clear_preset();      // -> CAMU
  /* PIN_03 */ hash << EXT_PIN_A02_C.clear_preset();      // -> BUXU
  /* PIN_04 */ hash << EXT_PIN_A03_C.clear_preset();      // -> BASE
  /* PIN_05 */ hash << EXT_PIN_A04_C.clear_preset();      // -> AFEC
  /* PIN_06 */ hash << EXT_PIN_A05_C.clear_preset();      // -> ABUP
  /* PIN_07 */ hash << EXT_PIN_A06_C.clear_preset();      // -> CYGU
  /* PIN_08 */ hash << EXT_PIN_A07_C.clear_preset();      // -> COGO
  /* PIN_09 */ hash << EXT_PIN_A08_C.clear_preset();      // -> MUJY
  /* PIN_10 */ hash << EXT_PIN_A09_C.clear_preset();      // -> NENA
  /* PIN_11 */ hash << EXT_PIN_A10_C.clear_preset();      // -> SURA
  /* PIN_12 */ hash << EXT_PIN_A11_C.clear_preset();      // -> MADY
  /* PIN_13 */ hash << EXT_PIN_A12_C.clear_preset();      // -> LAHE
  /* PIN_14 */ hash << EXT_PIN_A13_C.clear_preset();      // -> LURA
  /* PIN_15 */ hash << EXT_PIN_A14_C.clear_preset();      // -> PEVO
  /* PIN_16 */ hash << EXT_PIN_A15_C.clear_preset();      // -> RAZA
  /* PIN_71 */ hash << EXT_PIN_RST.clear_preset();
  /* PIN_74 */ hash << EXT_PIN_CLK_GOOD.clear_preset();
  /* PIN_76 */ hash << EXT_PIN_T2.clear_preset();
  /* PIN_77 */ hash << EXT_PIN_T1.clear_preset();
  /* PIN_78 */ hash << EXT_PIN_WRn_C.clear_preset();     // -> UBAL
  /* PIN_79 */ hash << EXT_PIN_RD_C.clear_preset();     // -> UJYV

  //----------

  hash << CPU_PIN_RD.clear_preset();     // PORTA_00: -> UJYV, LAGU, LAVO
  hash << CPU_PIN_WR.clear_preset();     // PORTA_01: -> AREV, LAGU.
  hash << CPU_PIN_ADDR_VALID.clear_preset();     // PORTA_06: -> APAP, TEXO
  hash << CPU_PIN5.clear_preset();     // PORTD_05: -> FROM_CPU5
  hash << CPU_PIN6.clear_preset();      // PORTD_00: -> LEXY, doesn't do anything
  hash << CPU_PIN_CLKREQ.clear_preset();         // PORTC_00: -> ABOL

  hash << CPU_PIN_A00.clear_preset();
  hash << CPU_PIN_A01.clear_preset();
  hash << CPU_PIN_A02.clear_preset();
  hash << CPU_PIN_A03.clear_preset();
  hash << CPU_PIN_A04.clear_preset();
  hash << CPU_PIN_A05.clear_preset();
  hash << CPU_PIN_A06.clear_preset();
  hash << CPU_PIN_A07.clear_preset();
  hash << CPU_PIN_A08.clear_preset();
  hash << CPU_PIN_A09.clear_preset();
  hash << CPU_PIN_A10.clear_preset();
  hash << CPU_PIN_A11.clear_preset();
  hash << CPU_PIN_A12.clear_preset();
  hash << CPU_PIN_A13.clear_preset();
  hash << CPU_PIN_A14.clear_preset();
  hash << CPU_PIN_A15.clear_preset();

  hash << CPU_TRI_D0.commit_tribuf();
  hash << CPU_TRI_D1.commit_tribuf();
  hash << CPU_TRI_D2.commit_tribuf();
  hash << CPU_TRI_D3.commit_tribuf();
  hash << CPU_TRI_D4.commit_tribuf();
  hash << CPU_TRI_D5.commit_tribuf();
  hash << CPU_TRI_D6.commit_tribuf();
  hash << CPU_TRI_D7.commit_tribuf();

  hash << CPU_PIN_BOOTp.commit_pinout();  // PORTA_04: <- TUTU

  //----------

  hash << VRM_TRI_A00.commit_tribuf();
  hash << VRM_TRI_A01.commit_tribuf();
  hash << VRM_TRI_A02.commit_tribuf();
  hash << VRM_TRI_A03.commit_tribuf();
  hash << VRM_TRI_A04.commit_tribuf();
  hash << VRM_TRI_A05.commit_tribuf();
  hash << VRM_TRI_A06.commit_tribuf();
  hash << VRM_TRI_A07.commit_tribuf();
  hash << VRM_TRI_A08.commit_tribuf();
  hash << VRM_TRI_A09.commit_tribuf();
  hash << VRM_TRI_A10.commit_tribuf();
  hash << VRM_TRI_A11.commit_tribuf();
  hash << VRM_TRI_A12.commit_tribuf();

  hash << VRM_TRI_D0.commit_tribuf();
  hash << VRM_TRI_D1.commit_tribuf();
  hash << VRM_TRI_D2.commit_tribuf();
  hash << VRM_TRI_D3.commit_tribuf();
  hash << VRM_TRI_D4.commit_tribuf();
  hash << VRM_TRI_D5.commit_tribuf();
  hash << VRM_TRI_D6.commit_tribuf();
  hash << VRM_TRI_D7.commit_tribuf();

  //----------

  hash << clk_reg.commit();
  hash << dbg_reg.commit();
  hash << rst_reg.commit();
  hash << tim_reg.commit();

#if 0
  hash << lcd_reg.commit();
  hash << pxp_reg.commit();
  hash << sst_reg.commit();
  hash << ppu_reg.commit();
  hash << win_reg.commit();
  hash << lcd_reg.commit();
  hash << ser_reg.commit();

  /* PIN_17 */ hash << EXT_PIN_D0_C.clear_preset();       // -> TOVO,SOMA
  /* PIN_18 */ hash << EXT_PIN_D1_C.clear_preset();       // -> RUZY,RONY
  /* PIN_19 */ hash << EXT_PIN_D2_C.clear_preset();       // -> ROME,RAXY
  /* PIN_20 */ hash << EXT_PIN_D3_C.clear_preset();       // -> SAZA,SELO
  /* PIN_21 */ hash << EXT_PIN_D4_C.clear_preset();       // -> TEHE,SODY
  /* PIN_22 */ hash << EXT_PIN_D5_C.clear_preset();       // -> RATU,SAGO
  /* PIN_23 */ hash << EXT_PIN_D6_C.clear_preset();       // -> SOCA,RUPA
  /* PIN_24 */ hash << EXT_PIN_D7_C.clear_preset();       // -> RYBA,SAZY
#endif

  return hash;
}

//-----------------------------------------------------------------------------
// Debug signals

wire SchematicTop::UMUT_MODE_DBG1p() const {
  /*p07.UVAR*/ wire UVAR_T2n = not(EXT_PIN_T2);
  /*p07.UMUT*/ wire UMUT_MODE_DBG1p = and (EXT_PIN_T1, UVAR_T2n);
  return UMUT_MODE_DBG1p;
}

wire SchematicTop::UNOR_MODE_DBG2p() const {
  /*p07.UBET*/ wire UBET_T1n = not(EXT_PIN_T1);
  /*p07.UNOR*/ wire UNOR_MODE_DBG2p = and (EXT_PIN_T2, UBET_T1n); // Must be UNORp, see UJYV/UBAL
  return UNOR_MODE_DBG2p;
}

wire SchematicTop::UPOJ_MODE_PROD() const {
  /*p07.UBET*/ wire UBET_T1n = not(EXT_PIN_T1);
  /*p07.UVAR*/ wire UVAR_T2n = not(EXT_PIN_T2);
  /*p07.UPOJ*/ wire UPOJ_MODE_PROD = nand(UBET_T1n, UVAR_T2n, EXT_PIN_RST);
  return UPOJ_MODE_PROD;
}

wire SchematicTop::TOVA_MODE_DBG2n() const {
  /*p08.TOVA*/ wire TOVA_MODE_DBG2n = not(UNOR_MODE_DBG2p());
  return TOVA_MODE_DBG2n;
}

wire SchematicTop::TUTO_DBG_VRAMp() const {
  /*p25.TUTO*/ wire TUTO_DBG_VRAMp = and (UNOR_MODE_DBG2p(), !dbg_reg.SOTO_DBG);
  return TUTO_DBG_VRAMp;
}

// still not sure what this is...
wire SchematicTop::ABUZ() const {
  /*p??.APAP*/ wire APAP = not(CPU_PIN_ADDR_VALID); // Missing from schematic
  /*p01.AWOD*/ wire AWOD = nor(UNOR_MODE_DBG2p(), APAP);
  /*p01.ABUZ*/ wire ABUZ = not(AWOD);
  return ABUZ;
}

void SchematicTop::preset_t1t2(bool t1, bool t2) {
  EXT_PIN_T1.preset(true, t1);
  EXT_PIN_T2.preset(true, t2);
}

//-----------------------------------------------------------------------------
// Clock signals

wire SchematicTop::AFUR_xBCDExxx() const { return clk_reg.AFUR_xBCDExxx; }
wire SchematicTop::ALEF_xxCDEFxx() const { return clk_reg.ALEF_xxCDEFxx; }
wire SchematicTop::APUK_xxxDEFGx() const { return clk_reg.APUK_xxxDEFGx; }
wire SchematicTop::ADYK_xxxxEFGH() const { return clk_reg.ADYK_xxxxEFGH; }
wire SchematicTop::WUVU_AxxDExxH() const { return clk_reg.WUVU_AxxDExxH; }
wire SchematicTop::VENA_xBCDExxx() const { return clk_reg.VENA_xBCDExxx; }
wire SchematicTop::WOSU_xxCDxxGH() const { return clk_reg.WOSU_xxCDxxGH; }

wire SchematicTop::BYJU_AxCDEFGH() const {
  /*p01.ABOL*/ wire ABOL_CLKREQn  = not(CPU_PIN_CLKREQ);
  /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!AFUR_xBCDExxx());
  /*p01.NULE*/ wire NULE_AxxxxFGH = nor(ABOL_CLKREQn,  ATYP_xBCDExxx);
  /*p01.AROV*/ wire AROV_xxxDEFGx = not(!APUK_xxxDEFGx());
  /*p01.BAPY*/ wire BAPY_AxxxxxxH = nor(ABOL_CLKREQn,  AROV_xxxDEFGx, ATYP_xBCDExxx);
  /*p01.AFEP*/ wire AFEP_ABxxxxGH = not(ALEF_xxCDEFxx());
  /*p01.BYRY*/ wire BYRY_xBCDExxx = not(NULE_AxxxxFGH);
  /*p01.BUDE*/ wire BUDE_AxxxxFGH = not(BYRY_xBCDExxx);
  /*p01.BERU*/ wire BERU_xBCDEFGx = not(BAPY_AxxxxxxH);
  /*p01.BUFA*/ wire BUFA_AxxxxxxH = not(BERU_xBCDEFGx);
  /*p01.BOLO*/ wire BOLO_xBCDEFGx = not(BUFA_AxxxxxxH);
  /*p01.BEKO*/ wire BEKO_xBCDExxx = not(BUDE_AxxxxFGH);
  /*p01.BEJA*/ wire BEJA_AxxxxFGH = nand(BOLO_xBCDEFGx, BEKO_xBCDExxx);
  /*p01.BANE*/ wire BANE_xBCDExxx = not(BEJA_AxxxxFGH);
  /*p01.BELO*/ wire BELO_AxxxxFGH = not(BANE_xBCDExxx);
  /*p01.BAZE*/ wire BAZE_xBCDExxx = not(BELO_AxxxxFGH);
  /*p01.BUTO*/ wire BUTO_AxCDEFGH = nand(AFEP_ABxxxxGH, ATYP_xBCDExxx, BAZE_xBCDExxx);
  /*p01.BELE*/ wire BELE_xBxxxxxx = not(BUTO_AxCDEFGH);
  /*p01.ATEZ*/ wire ATEZ_CLKBAD   = not(EXT_PIN_CLK_GOOD);
  /*p01.BYJU*/ wire BYJU_AxCDEFGH = nor(BELE_xBxxxxxx, ATEZ_CLKBAD);
  return BYJU_AxCDEFGH;
}

//-----------------------------------------------------------------------------
// Timer signals

wire SchematicTop::UVYN_DIV_05n() const    { return not(tim_reg.TAMA_DIV_05); }
wire SchematicTop::UMEK_DIV_06n() const    { return not(tim_reg.UGOT_DIV_06); }
wire SchematicTop::UREK_DIV_07n() const    { return not(tim_reg.TULU_DIV_07); }
wire SchematicTop::UPOF_DIV_15() const     { return tim_reg.UPOF_DIV_15; }
wire SchematicTop::MOBA_INT_TIMERp() const { return tim_reg.MOBA_INT_TIMERp; }

//-----------------------------------------------------------------------------
// Reset signals

wire SchematicTop::AVOR_RSTp() const {
  /*p01.AVOR*/ wire AVOR_RSTp = or(rst_reg.AFER_RSTp.q(), rst_reg.ASOL_RST_LATCHp.q());   
  return AVOR_RSTp;
}

wire SchematicTop::XAPO_VID_RSTn() const {
  /*p01.AVOR*/ wire AVOR_RSTp = or(rst_reg.AFER_RSTp.q(), rst_reg.ASOL_RST_LATCHp.q());   
  /*p01.ALUR*/ wire ALUR_RSTn = not(AVOR_RSTp);   // this goes all over the place
  /*p01.DULA*/ wire DULA_RSTp = not(ALUR_RSTn);
  /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RSTp);
  /*p01.XORE*/ wire XORE_RSTp = not(CUNU_RSTn);
  /*p01.XEBE*/ wire XEBE_RSTn = not(XORE_RSTp);
  /*p01.XODO*/ wire XODO_VID_RSTp = nand(XEBE_RSTn, ppu_config.XONA_LCDC_EN);
  /*p01.XAPO*/ wire XAPO_VID_RSTn = not(XODO_VID_RSTp);
  return XAPO_VID_RSTn;
}


//-----------------------------------------------------------------------------
// CPU signals

wire SchematicTop::UJYV_CPU_RD() const {
  /*p07.UJYV*/ wire UJYV_CPU_RD = mux2_n(EXT_PIN_RD_C, CPU_PIN_RD, UNOR_MODE_DBG2p());
  return UJYV_CPU_RD;
}

wire SchematicTop::UBAL_CPU_WRp_ABCDExxx() const {
  /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!AFUR_xBCDExxx());
  /*p01.ADAR*/ wire ADAR_ABCDxxxx = not(ADYK_xxxxEFGH());
  /*p01.AFAS*/ wire AFAS_xxxxxFGH = nor(ADAR_ABCDxxxx, ATYP_xBCDExxx);
  /*p01.AREV*/ wire AREV_CPU_WRn_ABCDExxx = nand(CPU_PIN_WR, AFAS_xxxxxFGH);
  /*p01.APOV*/ wire APOV_CPU_WRp_xxxxxFGH    = not(AREV_CPU_WRn_ABCDExxx);
  /*p07.UBAL*/ wire UBAL_CPU_WRp_ABCDExxx = mux2_n(EXT_PIN_WRn_C, APOV_CPU_WRp_xxxxxFGH, UNOR_MODE_DBG2p());
  return UBAL_CPU_WRp_ABCDExxx;
}

int SchematicTop::get_addr() const {
  return pack(CPU_PIN_A00, CPU_PIN_A01, CPU_PIN_A02, CPU_PIN_A03, CPU_PIN_A04, CPU_PIN_A05, CPU_PIN_A06, CPU_PIN_A07,
    CPU_PIN_A08, CPU_PIN_A09, CPU_PIN_A10, CPU_PIN_A11, CPU_PIN_A12, CPU_PIN_A13, CPU_PIN_A14, CPU_PIN_A15);
}

void SchematicTop::preset_addr(bool oe, uint16_t addr) {
  CPU_PIN_A00.preset(oe, addr & 0x0001);
  CPU_PIN_A01.preset(oe, addr & 0x0002);
  CPU_PIN_A02.preset(oe, addr & 0x0004);
  CPU_PIN_A03.preset(oe, addr & 0x0008);
  CPU_PIN_A04.preset(oe, addr & 0x0010);
  CPU_PIN_A05.preset(oe, addr & 0x0020);
  CPU_PIN_A06.preset(oe, addr & 0x0040);
  CPU_PIN_A07.preset(oe, addr & 0x0080);
  CPU_PIN_A08.preset(oe, addr & 0x0100);
  CPU_PIN_A09.preset(oe, addr & 0x0200);
  CPU_PIN_A10.preset(oe, addr & 0x0400);
  CPU_PIN_A11.preset(oe, addr & 0x0800);
  CPU_PIN_A12.preset(oe, addr & 0x1000);
  CPU_PIN_A13.preset(oe, addr & 0x2000);
  CPU_PIN_A14.preset(oe, addr & 0x4000);
  CPU_PIN_A15.preset(oe, addr & 0x8000);
}

int SchematicTop::get_data() const {
  return pack(CPU_TRI_D0, CPU_TRI_D1, CPU_TRI_D2, CPU_TRI_D3, CPU_TRI_D4, CPU_TRI_D5, CPU_TRI_D6, CPU_TRI_D7);
}

void SchematicTop::set_data(bool oe, uint8_t data) {
  CPU_TRI_D0.set_tribuf(oe, data & 0x01);
  CPU_TRI_D1.set_tribuf(oe, data & 0x02);
  CPU_TRI_D2.set_tribuf(oe, data & 0x04);
  CPU_TRI_D3.set_tribuf(oe, data & 0x08);
  CPU_TRI_D4.set_tribuf(oe, data & 0x10);
  CPU_TRI_D5.set_tribuf(oe, data & 0x20);
  CPU_TRI_D6.set_tribuf(oe, data & 0x40);
  CPU_TRI_D7.set_tribuf(oe, data & 0x80);
}

void SchematicTop::preset_rd(bool rd)            { CPU_PIN_RD.preset(true, rd); }
void SchematicTop::preset_wr(bool wr)            { CPU_PIN_WR.preset(true, wr); }
void SchematicTop::preset_addr_valid(bool valid) { CPU_PIN_ADDR_VALID.preset(true, valid); }

//-----------------------------------------------------------------------------
// Bootrom signals

wire SchematicTop::BOOT_BITn() const {
  /*p07.TEPU*/ return bootrom.BOOT_BITn;
}

wire SchematicTop::TUTU_BOOTp() const {
  /*p07.TERA*/ wire TERA_BOOT_BITp  = not(BOOT_BITn());
  /*p07.TULO*/ wire TULO_ADDR_00XXp = nor(CPU_PIN_A15, CPU_PIN_A14, CPU_PIN_A13, CPU_PIN_A12, CPU_PIN_A11, CPU_PIN_A10, CPU_PIN_A09, CPU_PIN_A08);
  /*p07.TUTU*/ wire TUTU_ADDR_BOOTp = and (TERA_BOOT_BITp, TULO_ADDR_00XXp);
  /*p07.TUTU*/ wire TUTU_BOOTp = TUTU_ADDR_BOOTp;
  return TUTU_BOOTp;
}

//-----------------------------------------------------------------------------
// DMA signals

wire SchematicTop::MATU_DMA_RUNNINGp() const {
  /*p04.MATU*/ return dma_reg.MATU_DMA_RUNNINGp;
}

wire SchematicTop::MUDA_DMA_SRC_VRAMp() const {
  /*p04.LEBU*/ wire LEBU_DMA_ADDR_A15n  = not(dma_reg.DMA_A15.q());
  /*p04.MUDA*/ wire MUDA_DMA_SRC_VRAMp = nor(dma_reg.DMA_A13.q(), dma_reg.DMA_A14.q(), LEBU_DMA_ADDR_A15n);
  return MUDA_DMA_SRC_VRAMp;
}

wire SchematicTop::DMA_A00() const { return dma_reg.DMA_A00; }
wire SchematicTop::DMA_A01() const { return dma_reg.DMA_A01; }
wire SchematicTop::DMA_A02() const { return dma_reg.DMA_A02; }
wire SchematicTop::DMA_A03() const { return dma_reg.DMA_A03; }
wire SchematicTop::DMA_A04() const { return dma_reg.DMA_A04; }
wire SchematicTop::DMA_A05() const { return dma_reg.DMA_A05; }
wire SchematicTop::DMA_A06() const { return dma_reg.DMA_A06; }
wire SchematicTop::DMA_A07() const { return dma_reg.DMA_A07; }
wire SchematicTop::DMA_A08() const { return dma_reg.DMA_A08; }
wire SchematicTop::DMA_A09() const { return dma_reg.DMA_A09; }
wire SchematicTop::DMA_A10() const { return dma_reg.DMA_A10; }
wire SchematicTop::DMA_A11() const { return dma_reg.DMA_A11; }
wire SchematicTop::DMA_A12() const { return dma_reg.DMA_A12; }
wire SchematicTop::DMA_A13() const { return dma_reg.DMA_A13; }
wire SchematicTop::DMA_A14() const { return dma_reg.DMA_A14; }
wire SchematicTop::DMA_A15() const { return dma_reg.DMA_A15; }

//------------------------------------------------------------------------------
// Joypad signals

wire SchematicTop::ASOK_INT_JPp() const {
  /*p02.ASOK*/ wire ASOK_INT_JPp = and (joy_reg.JP_GLITCH3, joy_reg.JP_GLITCH0);
  return ASOK_INT_JPp;
}

//------------------------------------------------------------------------------

//void preset(bool EXT_PIN_RD_A, bool EXT_PIN_CS_A);

/*
void preset_d(bool oe, uint8_t d) {
PIN_D0_C.preset(oe, d & 0x01);
PIN_D1_C.preset(oe, d & 0x02);
PIN_D2_C.preset(oe, d & 0x04);
PIN_D3_C.preset(oe, d & 0x08);
PIN_D4_C.preset(oe, d & 0x10);
PIN_D5_C.preset(oe, d & 0x20);
PIN_D6_C.preset(oe, d & 0x40);
PIN_D7_C.preset(oe, d & 0x80);
}
*/


#if 0
void ExtPinsOut::preset() {
  EXT_PIN_WRn_C.preset(true, 0);   // -> P07.UBAL
  EXT_PIN_RD_C.preset(true, 0);   // -> P07.UJYV
  EXT_PIN_A00_C.preset(true, 0);   // -> P08.KOVA
  EXT_PIN_A01_C.preset(true, 0);   // -> P08.CAMU
  EXT_PIN_A02_C.preset(true, 0);   // -> P08.BUXU
  EXT_PIN_A03_C.preset(true, 0);   // -> P08.BASE
  EXT_PIN_A04_C.preset(true, 0);   // -> P08.AFEC
  EXT_PIN_A05_C.preset(true, 0);   // -> P08.ABUP
  EXT_PIN_A06_C.preset(true, 0);   // -> P08.CYGU
  EXT_PIN_A07_C.preset(true, 0);   // -> P08.COGO
  EXT_PIN_A08_C.preset(true, 0);   // -> P08.MUJY
  EXT_PIN_A09_C.preset(true, 0);   // -> P08.NENA
  EXT_PIN_A10_C.preset(true, 0);   // -> P08.SURA
  EXT_PIN_A11_C.preset(true, 0);   // -> P08.MADY
  EXT_PIN_A12_C.preset(true, 0);   // -> P08.LAHE
  EXT_PIN_A13_C.preset(true, 0);   // -> P08.LURA
  EXT_PIN_A14_C.preset(true, 0);   // -> P08.PEVO
  EXT_PIN_A15_C.preset(true, 0);   // -> P08.RAZA

  if (!EXT_PIN_RD_A && !EXT_PIN_CS_A) {
    uint16_t ext_addr = get_addr();

    if (0x0000 <= ext_addr && ext_addr <= 0x7FFF) {
      // Cart rom
      //uint8_t d = rom[ext_addr];
      uint8_t d = 0;
      preset_d(true, d);
    }
    else if (0xC000 <= ext_addr && ext_addr <= 0xDFFF) {
      // Main ram
      //uint8_t d = ram[ext_addr - 0xC000];
      uint8_t d = 0;
      preset_d(true, d);
    }
    else if (0xE000 <= ext_addr && ext_addr <= 0xFFFF) {
      // Echo ram
      //uint8_t d = ram[ext_addr - 0xE000];
      uint8_t d = 0;
      preset_d(true, d);
    }
    else {
      printf("Bad address?\n");
      __debugbreak();
    }
  }
  else {
    preset_d(false, 0);
  }
}
#endif





#if 0
inline void dump_pins(TextPainter& text_painter, const PinOut& a, const PinOut& d) {
  dump_pin(text_painter, a.a, d.a);
}

inline void dump_pins(TextPainter& text_painter, const PinOut& a, const PinOut& b, const PinIn& c, const PinOut& d) {
  dump_pin(text_painter, a.a, b.a, c.a, d.a);
}

void dump_regs(TextPainter& /*text_painter*/) {
  /*
  text_painter.dprintf(" ----- EXT_BUS -----\n");

  text_painter.dprintf("ADDR_LATCH ");
  dump2(text_painter, EXT_ADDR_LATCH_14.a);
  dump2(text_painter, EXT_ADDR_LATCH_13.a);
  dump2(text_painter, EXT_ADDR_LATCH_12.a);
  text_painter.add_char(':');
  dump2(text_painter, EXT_ADDR_LATCH_11.a);
  dump2(text_painter, EXT_ADDR_LATCH_10.a);
  dump2(text_painter, EXT_ADDR_LATCH_09.a);
  dump2(text_painter, EXT_ADDR_LATCH_08.a);
  text_painter.add_char(':');
  dump2(text_painter, EXT_ADDR_LATCH_07.a);
  dump2(text_painter, EXT_ADDR_LATCH_06.a);
  dump2(text_painter, EXT_ADDR_LATCH_05.a);
  dump2(text_painter, EXT_ADDR_LATCH_04.a);
  text_painter.add_char(':');
  dump2(text_painter, EXT_ADDR_LATCH_03.a);
  dump2(text_painter, EXT_ADDR_LATCH_02.a);
  dump2(text_painter, EXT_ADDR_LATCH_01.a);
  dump2(text_painter, EXT_ADDR_LATCH_00.a);
  text_painter.newline();

  text_painter.dprintf("DATA_LATCH ");
  dump2(text_painter, CART_DATA_LATCH_07.a);
  dump2(text_painter, CART_DATA_LATCH_06.a);
  dump2(text_painter, CART_DATA_LATCH_05.a);
  dump2(text_painter, CART_DATA_LATCH_04.a);
  text_painter.add_char(':');
  dump2(text_painter, CART_DATA_LATCH_03.a);
  dump2(text_painter, CART_DATA_LATCH_02.a);
  dump2(text_painter, CART_DATA_LATCH_01.a);
  dump2(text_painter, CART_DATA_LATCH_00.a);
  text_painter.newline();
  */
}

void dump_pins(TextPainter& /*text_painter*/) {
  /*
  text_painter.dprintf("----- EXT_PINS -----\n");

  text_painter.dprintf("PIN_PHI %d\n", PIN_PHI.a.val);
  text_painter.dprintf("WR  %d\n", EXT_PIN_WR_A.a.val);
  text_painter.dprintf("RD  %d\n", EXT_PIN_RD_A.a.val);
  text_painter.dprintf("CS  %d\n", EXT_PIN_CS_A.a.val);

  text_painter.add_text("Axx ");
  dump_pins(text_painter, EXT_PIN_A15_A, EXT_PIN_A15_D);
  dump_pins(text_painter, EXT_PIN_A14_A, EXT_PIN_A14_D);
  dump_pins(text_painter, EXT_PIN_A13_A, EXT_PIN_A13_D);
  dump_pins(text_painter, EXT_PIN_A12_A, EXT_PIN_A12_D);
  text_painter.add_char(':');
  dump_pins(text_painter, EXT_PIN_A11_A, EXT_PIN_A11_D);
  dump_pins(text_painter, EXT_PIN_A10_A, EXT_PIN_A10_D);
  dump_pins(text_painter, EXT_PIN_A09_A, EXT_PIN_A09_D);
  dump_pins(text_painter, EXT_PIN_A08_A, EXT_PIN_A08_D);
  text_painter.add_char(':');
  dump_pins(text_painter, EXT_PIN_A07_A, EXT_PIN_A07_D);
  dump_pins(text_painter, EXT_PIN_A06_A, EXT_PIN_A06_D);
  dump_pins(text_painter, EXT_PIN_A05_A, EXT_PIN_A05_D);
  dump_pins(text_painter, EXT_PIN_A04_A, EXT_PIN_A04_D);
  text_painter.add_char(':');
  dump_pins(text_painter, EXT_PIN_A03_A, EXT_PIN_A03_D);
  dump_pins(text_painter, EXT_PIN_A02_A, EXT_PIN_A02_D);
  dump_pins(text_painter, EXT_PIN_A01_A, EXT_PIN_A01_D);
  dump_pins(text_painter, EXT_PIN_A00_A, EXT_PIN_A00_D);
  text_painter.newline();

  text_painter.add_text("Dxx ");
  dump_pins(text_painter, EXT_PIN_D7_A, EXT_PIN_D7_B, PIN_D7_C, EXT_PIN_D7_D);
  dump_pins(text_painter, EXT_PIN_D6_A, EXT_PIN_D6_B, PIN_D6_C, EXT_PIN_D6_D);
  dump_pins(text_painter, EXT_PIN_D5_A, EXT_PIN_D5_B, PIN_D5_C, EXT_PIN_D5_D);
  dump_pins(text_painter, EXT_PIN_D4_A, EXT_PIN_D4_B, PIN_D4_C, EXT_PIN_D4_D);
  dump_pins(text_painter, EXT_PIN_D3_A, EXT_PIN_D3_B, PIN_D3_C, EXT_PIN_D3_D);
  dump_pins(text_painter, EXT_PIN_D2_A, EXT_PIN_D2_B, PIN_D2_C, EXT_PIN_D2_D);
  dump_pins(text_painter, EXT_PIN_D1_A, EXT_PIN_D1_B, PIN_D1_C, EXT_PIN_D1_D);
  dump_pins(text_painter, EXT_PIN_D0_A, EXT_PIN_D0_B, PIN_D0_C, EXT_PIN_D0_D);
  text_painter.newline();
  */
}

uint16_t get_addr() const {
  uint16_t ext_addr = (uint16_t)pack(
    EXT_PIN_A00_A, EXT_PIN_A01_A, EXT_PIN_A02_A, EXT_PIN_A03_A,
    EXT_PIN_A04_A, EXT_PIN_A05_A, EXT_PIN_A06_A, EXT_PIN_A07_A,
    EXT_PIN_A08_A, EXT_PIN_A09_A, EXT_PIN_A10_A, EXT_PIN_A11_A,
    EXT_PIN_A12_A, EXT_PIN_A13_A, EXT_PIN_A14_A, EXT_PIN_A15_A);

  return ext_addr;
}

/*
void dump_pins(TextPainter& text_painter) {
text_painter.dprintf("----- SYS_PINS -----\n");
text_painter.dprintf("PIN_RST      %d\n", PIN_RST.a.val);
text_painter.dprintf("PIN_CLK_GOOD %d\n", PIN_CLK_GOOD.a.val);
text_painter.dprintf("PIN_CLK_IN_xBxDxFxH   %d\n", PIN_CLK_IN_xBxDxFxH.a.val);
text_painter.dprintf("EXT_PIN_T1       %d\n", EXT_PIN_T1.a.val);
text_painter.dprintf("EXT_PIN_T2       %d\n", EXT_PIN_T2.a.val);
text_painter.newline();
}
*/

#endif