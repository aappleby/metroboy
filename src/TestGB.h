#include "Sch_Merged.h"
#include "Sch_Pins.h"

#include "Sch_DMA.h"
#include "Sch_SpriteStore.h"
#include "Sch_OAM.h"
#include "Sch_PPU.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct TestGB {

  void ext_preset();

  void tick_everything();
  bool commit_everything();

  void tick_cpu_addr();
  void tick_cpu_read();
  void tick_cpu_write();
  void tick_cpu_pins();
  void tick_cpu_interrupts();

  void tick_serial(CpuPins& cpu_pins);
  void tick_timer();
  void tick_joypad();
  void tick_lcd();
  void tick_window();

  void tick_vram_addr();

  void tick_bootrom();
  void tick_cart_data();
  void tick_cart_pins();

  void tick_oam();

  void tick_ppu();

  void tick_pixpipe();

  //----------------------------------------

  wire UMUT_MODE_DBG1() const {
    /*p07.UVAR*/ wire UVAR_T2n = not(sys_pins.T2);
    /*p07.UMUT*/ wire UMUT_MODE_DBG1 = and(sys_pins.T1, UVAR_T2n);
    return UMUT_MODE_DBG1;
  }

  // must be N otherwise vram debug control doesn't work
  wire UNOR_MODE_DBG2n() const {
    /*p07.UBET*/ wire UBET_T1n = not(sys_pins.T1);
    /*p07.UNOR*/ wire UNOR_MODE_DBG2n = and(sys_pins.T2, UBET_T1n);
    return UNOR_MODE_DBG2n;
  }

  wire TOVA_MODE_DBG2p() const {
    /*p08.TOVA*/ wire TOVA_MODE_DBG2p = not(UNOR_MODE_DBG2n());
    return TOVA_MODE_DBG2p;
  }

  wire UPOJ_MODE_PRODn() const {
    // Die trace:
    // UVAR = not(T2)
    // UBET = not(T1)
    // UPOJ = nand(UBET, UVAR, RST);

    /*p07.UVAR*/ wire UVAR_T2n = not(sys_pins.T2);
    /*p07.UBET*/ wire UBET_T1n = not(sys_pins.T1);
    /*p07.UPOJ*/ wire UPOJ_MODE_PRODn = nand(UBET_T1n, UVAR_T2n, sys_pins.RST);
    return UPOJ_MODE_PRODn;
  }

  //----------------------------------------
  // Root 4 mhz clocks

  wire ANOS_AxCxExGx() const {
    /*p01.ANOS*/ wire ANOS_AxCxExGx = not(sys_pins.CLK_IN_xBxDxFxH);
    return ANOS_AxCxExGx;
  }

  wire ATAL_xBxDxFxH() const {
    /*p01.ATAL*/ wire ATAL_xBxDxFxH = not(ANOS_AxCxExGx());
    return ATAL_xBxDxFxH;
  }

  wire ALET_xBxDxFxH() const {
    /*p01.AZOF*/ wire AZOF_AxCxExGx = not(ATAL_xBxDxFxH());
    /*p01.ZAXY*/ wire ZAXY_xBxDxFxH = not(AZOF_AxCxExGx);
    /*p01.ZEME*/ wire ZEME_AxCxExGx = not(ZAXY_xBxDxFxH);
    /*p01.ALET*/ wire ALET_xBxDxFxH = not(ZEME_AxCxExGx);
    return ALET_xBxDxFxH;
  }

  wire MYVO_AxCxExGx() const {
    /*p27.MYVO*/ wire MYVO = not(ALET_xBxDxFxH());
    return MYVO;
  }

  //----------------------------------------
// Root 1 mhz clocks

  wire AFAS_xxxxxFGH() const {
    /*p01.ADAR*/ wire ADAR_ABCDxxxx = not(clk_reg.PHAZ_xxxxEFGH.q());
    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(clk_reg.PHAZ_xBCDExxx.qn());
    /*p01.AFAS*/ wire AFAS_xxxxxFGH = nor(ADAR_ABCDxxxx, ATYP_xBCDExxx);
    return AFAS_xxxxxFGH;
  }

  wire BUKE_ABxxxxxH() const {
    /*p01.ABOL*/ wire ABOL_CLKREQn  = not(cpu_pins.CLKREQ);
    /*p01.AFEP*/ wire AFEP_ABxxxxGH = not(clk_reg.PHAZ_xxCDEFxx.q());
    /*p01.BUGO*/ wire BUGO_xxCDEFxx = not(AFEP_ABxxxxGH);
    /*p01.AROV*/ wire AROV_xxxDEFGx = not(clk_reg.PHAZ_xxxDEFGx.qn());
    /*p01.BATE*/ wire BATE_ABxxxxxH = nor(ABOL_CLKREQn, BUGO_xxCDEFxx, AROV_xxxDEFGx);
    /*p01.BASU*/ wire BASU_xxCDEFGx = not(BATE_ABxxxxxH);
    /*p01.BUKE*/ wire BUKE_ABxxxxxH = not(BASU_xxCDEFGx);
    return BUKE_ABxxxxxH;
  }

  wire BUDE_AxxxxFGH() const {
    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(clk_reg.PHAZ_xBCDExxx.qn());
    /*p01.ABOL*/ wire ABOL_CLKREQn = not(cpu_pins.CLKREQ);
    /*p01.NULE*/ wire NULE_AxxxxFGH = nor(ABOL_CLKREQn, ATYP_xBCDExxx);
    /*p01.BYRY*/ wire BYRY_xBCDExxx = not(NULE_AxxxxFGH);
    /*p01.BUDE*/ wire BUDE_AxxxxFGH = not(BYRY_xBCDExxx);
    return BUDE_AxxxxFGH;
  }

  wire BOLO_xBCDEFGx() const {
    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(clk_reg.PHAZ_xBCDExxx.qn());
    /*p01.ABOL*/ wire ABOL_CLKREQn = not(cpu_pins.CLKREQ);
    /*p01.AROV*/ wire AROV_xxxDEFGx = not(clk_reg.PHAZ_xxxDEFGx.qn());
    /*p01.BAPY*/ wire BAPY_AxxxxxxH = nor(ABOL_CLKREQn, AROV_xxxDEFGx, ATYP_xBCDExxx);
    /*p01.BERU*/ wire BERU_xBCDEFGx = not(BAPY_AxxxxxxH);
    /*p01.BUFA*/ wire BUFA_AxxxxxxH = not(BERU_xBCDEFGx);
    /*p01.BOLO*/ wire BOLO_xBCDEFGx = not(BUFA_AxxxxxxH);
    return BOLO_xBCDEFGx;
  }

  wire BALY_xBxxxxxx() const {
    /*p01.BEKO*/ wire BEKO_xBCDExxx = not(BUDE_AxxxxFGH());
    /*p01.BEJA*/ wire BEJA_AxxxxFGH = nand(BOLO_xBCDEFGx(), BEKO_xBCDExxx);
    /*p01.BANE*/ wire BANE_xBCDExxx = not(BEJA_AxxxxFGH);
    /*p01.BELO*/ wire BELO_AxxxxFGH = not(BANE_xBCDExxx);
    /*p01.BAZE*/ wire BAZE_xBCDExxx = not(BELO_AxxxxFGH);
    /*p01.AFEP*/ wire AFEP_ABxxxxGH = not(clk_reg.PHAZ_xxCDEFxx.q());
    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(clk_reg.PHAZ_xBCDExxx.qn());
    /*p01.BUTO*/ wire BUTO_AxCDEFGH = nand(AFEP_ABxxxxGH, ATYP_xBCDExxx, BAZE_xBCDExxx);
    /*p01.BELE*/ wire BELE_xBxxxxxx = not(BUTO_AxCDEFGH);
    /*p01.ATEZ*/ wire ATEZ_CLKBAD = not(sys_pins.CLK_GOOD);
    /*p01.BYJU*/ wire BYJU_AxCDEFGH = nor(BELE_xBxxxxxx, ATEZ_CLKBAD);
    /*p01.BALY*/ wire BALY_xBxxxxxx = not(BYJU_AxCDEFGH);
    return BALY_xBxxxxxx;
  }

  wire BOGA_AxCDEFGH() const {
    /*p01.BOGA*/ wire BOGA_AxCDEFGH = not(BALY_xBxxxxxx());
    return BOGA_AxCDEFGH;
  }

  //----------------------------------------
  // Reset signals

  wire ALUR_RSTn() const {
    /*p01.AVOR*/ wire AVOR_RSTp = or (rst_reg.RESET_REGp.q(), rst_reg.ASOL_RST_LATCHp.q());
    /*p01.ALUR*/ wire ALUR_RSTn = not(AVOR_RSTp);   // this goes all over the place
    return ALUR_RSTn;
  }

  wire CUNU_RSTn() const {
    /*p01.DULA*/ wire DULA_RSTp = not(ALUR_RSTn());
    /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RSTp);
    return CUNU_RSTn;
  }

  wire XAPO_VID_RSTn() const {
    /*p01.XORE*/ wire XORE_RSTp = not(CUNU_RSTn());
    /*p01.XEBE*/ wire XEBE_RSTn = not(XORE_RSTp);
    /*p01.XODO*/ wire XODO_VID_RSTp = nand(XEBE_RSTn, cfg_reg.XONA_LCDC_EN.q());
    /*p01.XAPO*/ wire XAPO_VID_RSTn = not(XODO_VID_RSTp);
    return XAPO_VID_RSTn;
  }

  wire ATAR_VID_RSTp() const {
    /*p01.ATAR*/ wire ATAR_VID_RSTp = not(XAPO_VID_RSTn());
    return ATAR_VID_RSTp;
  }

  wire ABEZ_VID_RSTn() const {
    /*p01.ABEZ*/ wire ABEZ_VID_RSTn = not(ATAR_VID_RSTp());
    return ABEZ_VID_RSTn;
  }

  //----------------------------------------
  // LCD & PPU events

  wire BYHA_VID_LINE_TRIG_d4n() const {
    /*p28.ABAF*/ wire VID_LINE_d4n = not(lcd_reg.CATU_VID_LINE_d4.q());
    /*p28.BYHA*/ wire BYHA_VID_LINE_TRIG_d4n = and(or (lcd_reg.ANEL_VID_LINE_d6.q(), VID_LINE_d4n), ABEZ_VID_RSTn());
    return BYHA_VID_LINE_TRIG_d4n;
  }
    
  wire ATEJ_VID_LINE_TRIG_d4p() const {
    /*p28.ATEJ*/ wire ATEJ_VID_LINE_TRIG_d4p = not(BYHA_VID_LINE_TRIG_d4n());
    return ATEJ_VID_LINE_TRIG_d4p;
  }

  wire ACYL_PPU_USE_OAM1p() const {
    /*p28.BOGE*/ wire DMA_RUNNINGn = not(dma_reg.MATU_DMA_OAM_WRp.q());
    /*p28.ACYL*/ wire ACYL_PPU_USE_OAM1p = and(DMA_RUNNINGn, sst_reg.BESU_SCANNINGp);
    return ACYL_PPU_USE_OAM1p;
  }

  wire AVAP_SCAN_DONE_d0_TRIGp() const {
    /*p28.ANOM*/ wire ANOM_SCAN_RSTn = nor(ATEJ_VID_LINE_TRIG_d4p(), ATAR_VID_RSTp());
    /*p29.BALU*/ wire BALU_SCAN_RST = not(ANOM_SCAN_RSTn);
    /*p29.BEBU*/ wire SCAN_DONE_d0_TRIGn = or (BALU_SCAN_RST, sst_reg.SCAN_DONE_d5.q(), !sst_reg.SCAN_DONE_d4.q());
    /*p29.AVAP*/ wire AVAP_SCAN_DONE_d0_TRIGp = not(SCAN_DONE_d0_TRIGn);
    return AVAP_SCAN_DONE_d0_TRIGp;
  }

  // Polarity?
  wire TEXY_SPRITE_READ() const {
    // Die trace:
    // SAKY = nor(TULY17, VONU17)
    // TEPA = not(XYMU)
    // TYSO = or(SAKY, TEPA)
    // TEXY = not(TYSO)

    /*p29.SAKY*/ wire SAKY = nor(ppu_reg.TULY_SFETCH_S1.q(), ppu_reg.VONU_SFETCH_S1_D4.q());
    /*p29.TEPA*/ wire TEPA_RENDERINGn = not(ppu_reg.XYMU_RENDERINGp);
    /*p29.TYSO*/ wire TYSO_SPRITE_READn = or (SAKY, TEPA_RENDERINGn);
    /*p29.TEXY*/ wire TEXY_SPRITE_READ = not(TYSO_SPRITE_READn);

    // So we only read a sprite if both those regs are... low? what is rung 17's polarity?
#if 0
    if (RENDERING_LATCH) {
      /*p29.TEXY*/ wire TEXY_SPRITE_READ = or(ppu_reg.TULY_SFETCH_S1.q(), ppu_reg.VONU_SFETCH_S1_D4.q());;
    }
    else {
      /*p29.TEXY*/ wire TEXY_SPRITE_READ = 1;
    }
#endif

    return TEXY_SPRITE_READ;
  }

  wire WUTY_SPRITE_DONE() const {
    /*p29.TYNO*/ wire TYNO = nand(ppu_reg.TOXE_SFETCH_S0_D0.q(), ppu_reg.SEBA_SFETCH_S1_D5.q(), ppu_reg.VONU_SFETCH_S1_D4.q());
    /*p29.VUSA*/ wire VUSA = or (!ppu_reg.TYFO_SFETCH_S0_D1.q(), TYNO);
    /*p29.WUTY*/ wire WUTY_SPRITE_DONE = not(VUSA);
    return WUTY_SPRITE_DONE;
  }

  wire WODU_RENDER_DONE() const {
    /*p21.XUGU*/ wire XUGU_X_167n = nand(ppu_reg.SAXO_X0.q(), ppu_reg.TYPO_X1.q(), ppu_reg.VYZO_X2.q(), ppu_reg.TAHA_X5.q(), ppu_reg.SYBE_X7.q()); // 128 + 32 + 4 + 2 + 1 = 167
    /*p21.XANO*/ wire XANO_X_167 = not(XUGU_X_167n);
    /*p21.XENA*/ wire XENA_STORE_MATCHn = not(sst_reg.FEPO_STORE_MATCHp);
    /*p21.WODU*/ wire WODU_RENDER_DONE = and(XENA_STORE_MATCHn, XANO_X_167);
    return WODU_RENDER_DONE;
  }

  //----------------------------------------
  // CPU read/write signals

  // Polarity of this seems inconsistent...
  wire TEDO_CPU_RD() const {
#if 0
    /*p07.UNOR*/ wire UNOR_MODE_DBG2 = and(sys_pins.T2, not(sys_pins.T1));
    if (UNOR_MODE_DBG2) {
      TEDO = not(ext_pins.RD_C);
    }
    else {
      TEDO = not(cpu_pins.CPU_RAW_RD);
    }

#endif

    /*p07.UBET*/ wire UBET_T1n = not(sys_pins.T1);
    /*p07.UNOR*/ wire UNOR_MODE_DBG2 = and(sys_pins.T2, UBET_T1n);
    /*p07.UJYV*/ wire UJYV_BUS_RD_MUX = mux2_n(ext_pins.RD_C, cpu_pins.CPU_RAW_RD, UNOR_MODE_DBG2);
    /*p07.TEDO*/ wire TEDO_CPU_RD = not(UJYV_BUS_RD_MUX);
    return TEDO_CPU_RD;
  }

  wire ASOT_CPU_RD() const {
    /*p07.AJAS*/ wire AJAS_BUS_RDn = not(TEDO_CPU_RD());
    /*p07.ASOT*/ wire ASOT_CPU_RD = not(AJAS_BUS_RDn);
    return ASOT_CPU_RD;
  }

  wire AREV_CPU_WRn_ABCDExxx() const {
    /*p01.AREV*/ wire AREV_CPU_WRn_ABCDExxx = nand(cpu_pins.CPU_RAW_WR, AFAS_xxxxxFGH());
    return AREV_CPU_WRn_ABCDExxx;
  }

  wire APOV_CPU_WR_xxxxxFGH() const {
    /*p01.APOV*/ wire APOV_CPU_WR_xxxxxFGH = not(AREV_CPU_WRn_ABCDExxx());
    return APOV_CPU_WR_xxxxxFGH;
  }

  wire TAPU_CPU_WR_xxxxxFGH() const {
    /*p07.UBAL*/ wire UBAL_BUS_WR_ABCDExxx = mux2_n(ext_pins.WR_C, APOV_CPU_WR_xxxxxFGH(), UNOR_MODE_DBG2n());
    /*p07.TAPU*/ wire TAPU_CPU_WR_xxxxxFGH = not(UBAL_BUS_WR_ABCDExxx);
    return TAPU_CPU_WR_xxxxxFGH;
  }
  wire CUPA_CPU_WR_xxxxxFGH() const {
    /*p07.DYKY*/ wire DYKY_BUS_WR_ABCDExxx = not(TAPU_CPU_WR_xxxxxFGH());
    /*p07.CUPA*/ wire CUPA_CPU_WR_xxxxxFGH = not(DYKY_BUS_WR_ABCDExxx);
    return CUPA_CPU_WR_xxxxxFGH;
  }

  //----------------------------------------
  // Window signals

  wire SYLO_WIN_HITn() const {
    /*p27.SYLO*/ wire SYLO_WIN_HITn = not(ppu_reg.RYDY_WIN_HIT_LATCH.q());
    return SYLO_WIN_HITn;
  }

  wire SOCY_WIN_HITn() const {
    /*p24.TOMU*/ wire TOMU_WIN_HIT = not(SYLO_WIN_HITn());
    /*p24.SOCY*/ wire SOCY_WIN_HITn = not(TOMU_WIN_HIT);
    return SOCY_WIN_HITn;
  }

  wire TUKU_WIN_HITn() const {
    /*p24.TOMU*/ wire TOMU_WIN_HIT = not(SYLO_WIN_HITn());
    /*p27.TUKU*/ wire TUKU_WIN_HITn = not(TOMU_WIN_HIT);
    return TUKU_WIN_HITn;
  }

  wire PORE_WIN_MODE() const {
    /*p27.NOCU*/ wire NOCU_WIN_MODEn = not(ppu_reg.PYNU_WIN_MODE_LATCH.q());
    /*p27.PORE*/ wire PORE_WIN_MODE = not(NOCU_WIN_MODEn);
    return PORE_WIN_MODE;
  }

  wire AXAD_WIN_MODEn() const {
    /*p26.AXAD*/ wire AXAD_WIN_MODEn = not(PORE_WIN_MODE());
    return AXAD_WIN_MODEn;
  }

  wire XOFO_WIN_RST() const {
    /*p28.ABAF*/ wire ABAF_VID_LINE_d4n = not(lcd_reg.CATU_VID_LINE_d4.q());
    /*p28.BYHA*/ wire BYHA_VID_LINE_TRIG_d4n = and(or (lcd_reg.ANEL_VID_LINE_d6.q(), ABAF_VID_LINE_d4n), ABEZ_VID_RSTn());
    /*p28.ATEJ*/ wire ATEJ_VID_LINE_TRIG_d4p = not(BYHA_VID_LINE_TRIG_d4n);
    /*p27.XAHY*/ wire XAHY_VID_LINE_TRIG_d4n = not(ATEJ_VID_LINE_TRIG_d4p);
    /*p27.XOFO*/ wire XOFO_WIN_RST = nand(cfg_reg.LCDC_WINEN.q(), XAHY_VID_LINE_TRIG_d4n, XAPO_VID_RSTn());
    return XOFO_WIN_RST;
  }

  //----------------------------------------
  // DMA signals

  /*p04.MUDA*/ wire MUDA_DMA_ADDR_VRAMp() const { // def p
    // Die trace:
    // LEBU = not(MARU06)
    // MUDA = nor(PULA06, POKU06, LEBU);

#if 0
    // if rung 6 of MARU/PULA/POKU was QN:
    // MUDA = and(A13, A14, !A15);
    // would select last quarter of ROM, which doesn't make sense
    // so rung 6 of MARU must be Q.
#endif

    /*p04.LEBU*/ wire LEBU_DMA_ADDR_A15n = not(dma_reg.DMA_A15.q());
    /*p04.MUDA*/ wire MUDA_DMA_ADDR_VRAMp = nor(dma_reg.DMA_A13.q(), dma_reg.DMA_A14.q(), LEBU_DMA_ADDR_A15n);
    return MUDA_DMA_ADDR_VRAMp;
  }

  //----------------------------------------
  // Address decoders

#if 0
  // Not sure about polarity
  // >> SEPY, TOZA, TUCA
  wire ABUZ_ADDR_VALIDx() const {
    // Die trace:
    // APAP = not(CPU_ADDR_VALID)
    // AWOD = nor(UNOR, APAP)
    // ABUZ = not(AWOD)

    /*p??.APAP*/ wire APAP = not(cpu_pins.ADDR_VALID); // Missing from schematic
    /*p01.ABUZ*/ wire AWOD = nor(UNOR_MODE_DBG2n(), APAP);
    /*p01.ABUZ*/ wire ABUZ = not(AWOD);
    return ABUZ;
  }
#endif

#if 0
  // This one is really weird.
  wire TEXO_ADDR_VALID_AND_NOT_VRAM() const {

    // TEVY box color wrong on die trace, but schematic correct.

    // Die trace:
    // SORE = not(A15)
    // TEVY = or(A13, A13, SORE) // A13 line not fully drawn
    // TEXO = and(ADDR_VALIDx?, TEVY)

    // The polarity of ADDR_VALIDx may be wrong

    /*p08.SORE*/ wire SORE = not(cpu_pins.A15);
    /*p08.TEVY*/ wire TEVY = or (cpu_pins.A13, cpu_pins.A14, SORE);
    /*p08.TEXO*/ wire TEXO = and(cpu_pins.ADDR_VALID, TEVY);
    return TEXO;
  }
#endif

  wire TUNA_CPU_ADDR_0000_FDFF() const {
    /*p07.TUNA*/ wire TUNA_CPU_ADDR_0000_FDFF = nand(cpu_pins.A15, cpu_pins.A14, cpu_pins.A13, cpu_pins.A12, cpu_pins.A11, cpu_pins.A10, cpu_pins.A09);
    return TUNA_CPU_ADDR_0000_FDFF;
  }

  wire SYRO_CPU_ADDR_FE00_FFFFp() const {
    /*p25.SYRO*/ wire SYRO_CPU_ADDR_FE00_FFFFp = not(TUNA_CPU_ADDR_0000_FDFF());
    return SYRO_CPU_ADDR_FE00_FFFFp;
  }

  wire SYKE_CPU_ADDR_FFXXp() const {
    /*p07.TONA*/ wire TONA_ADDR_08n = not(cpu_pins.A08);
    /*p07.SYKE*/ wire SYKE_CPU_ADDR_FFXXp = nor(TUNA_CPU_ADDR_0000_FDFF(), TONA_ADDR_08n);
    return SYKE_CPU_ADDR_FFXXp;
  }

  wire WERO_CPU_ADDR_FF4Xp() const {
    /*p22.XALY*/ wire XALY_ADDR_0x00xxxx = nor(cpu_pins.A07, cpu_pins.A05, cpu_pins.A04);
    /*p22.WUTU*/ wire WUTU_FF4Xn = nand(SYKE_CPU_ADDR_FFXXp(), cpu_pins.A06, XALY_ADDR_0x00xxxx);
    /*p22.WERO*/ wire WERO_CPU_ADDR_FF4Xp = not(WUTU_FF4Xn);
    return WERO_CPU_ADDR_FF4Xp;
  }

  wire XEDA_CPU_ADDR_FF46p() const {
    /*p22.XOLA*/ wire XOLA_A00n = not(cpu_pins.A00);
    /*p22.XENO*/ wire XENO_A01n = not(cpu_pins.A01);
    /*p22.XUSY*/ wire XUSY_A02n = not(cpu_pins.A02);
    /*p22.XERA*/ wire XERA_A03n = not(cpu_pins.A03);

    /*p22.WESA*/ wire WESA_A01p = not(XENO_A01n);
    /*p22.WALO*/ wire WALO_A02p = not(XUSY_A02n);

    /*p22.WATE*/ wire WATE_FF46n = nand(WERO_CPU_ADDR_FF4Xp(), XOLA_A00n, WESA_A01p, WALO_A02p, XERA_A03n);
    /*p22.XEDA*/ wire XEDA_CPU_ADDR_FF46p = not(WATE_FF46n);
    return XEDA_CPU_ADDR_FF46p;
  }

  //-----------------------------------------------------------------------------


  int phase_counter = -32;

  /*
  uint8_t rom[65536];
  uint8_t ram[8192];
  uint8_t vram[8192];
  uint8_t hiram[128];
  */

  ClkRegisters clk_reg; // dumped
  BusRegisters bus_reg;// dumped
  ConfigRegisters cfg_reg;// dumped
  DebugRegisters dbg_reg;// dumped
  DmaRegisters dma_reg;// dumped
  InterruptRegisters int_reg;// dumped
  JoypadRegisters joy_reg;// dumped

  LcdRegisters lcd_reg;// dumped
  PixelPipeRegisters pxp_reg;// dumped
  RstRegisters rst_reg;// dumped
  SerialRegisters ser_reg;// dumped
  SpriteStoreRegisters sst_reg;// dumped
  TimerRegisters tim_reg;// dumped
  VclkRegisters vck_reg;// dumped
  PpuRegisters ppu_reg;// dumped
  OamRegisters oam_reg;// dumped

  SysPins sys_pins; // dumped
  VramPins vram_pins; // dumped
  JoypadPins joy_pin; // dumped
  CpuPins cpu_pins; // dumped
  ExtPins ext_pins; // dumped
  OamPins oam_pins; // dumped
  WavePins wave_pins;

  /*p04.MAKA*/ Reg FROM_CPU5_SYNC;

  Signal NYXU_BG_SEQ_RSTn;

  /*p27.TEVO*/ Signal TEVO_CLK_STOPn;

  /*p35.PATY*/ Signal PATY_PIX_OUT_LO;
  /*p35.PERO*/ Signal PERO_PIX_OUT_HI;
};

//-----------------------------------------------------------------------------

};