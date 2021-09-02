#pragma once

#include "CoreLib/File.h"
#include "CoreLib/MetroBoyCPU.h"
#include "CoreLib/Constants.h"
#include "CoreLib/Log.h"

#include "GateBoyLib/Probe.h"

#include "GateBoyLib/GateBoyTimer.h"
#include "GateBoyLib/GateBoyLCD.h"
#include "GateBoyLib/GateBoyDMA.h"
#include "GateBoyLib/GateBoySpriteStore.h"
#include "GateBoyLib/GateBoyClocks.h"
#include "GateBoyLib/GateBoyPixPipe.h"
#include "GateBoyLib/GateBoyJoypad.h"
#include "GateBoyLib/GateBoySerial.h"
#include "GateBoyLib/GateBoyInterrupts.h"
#include "GateBoyLib/GateBoySpriteFetcher.h"
#include "GateBoyLib/GateBoyTileFetcher.h"
#include "GateBoyLib/GateBoySpriteScanner.h"
#include "GateBoyLib/GateBoyExtBus.h"
#include "GateBoyLib/GateBoyOamBus.h"
#include "GateBoyLib/GateBoyVramBus.h"
#include "GateBoyLib/GateBoyZramBus.h"
#include "GateBoyLib/GateBoyResetDebug.h"
#include "GateBoyLib/GateBoyCpuBus.h"
#include "GateBoyLib/GateBoySPU.h"

#include <cstring>

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct GateBoy {
  void reset_to_bootrom(const blob& cart_blob, bool fastboot);
  void reset_to_cart(const blob& cart_blob);

  //----------------------------------------

  static bool check_sentinel(const blob& b) {
    if (b.size() < sizeof(GateBoy)) return false;

    GateBoy* gb = (GateBoy*)b.data();
    if (gb->sentinel1 != SENTINEL1) return false;
    if (gb->sentinel2 != SENTINEL2) return false;
    if (gb->sentinel3 != SENTINEL3) return false;
    if (gb->sentinel4 != SENTINEL4) return false;
    return true;
  }

  int from_blob(const blob& b) {
    ASSERT_P(b.size() >= sizeof(*this));
    memcpy(this, b.data(), sizeof(*this));
    ASSERT_P(sentinel1 == SENTINEL1);
    ASSERT_P(sentinel2 == SENTINEL2);
    ASSERT_P(sentinel3 == SENTINEL3);
    ASSERT_P(sentinel4 == SENTINEL4);
    return (int)sizeof(*this);
  }

  void to_blob(blob& b) {
    uint8_t* bytes = (uint8_t*)this;
    b.insert(b.end(), bytes, bytes + sizeof(*this));
  }

  //----------------------------------------

  void check_div() const {
    int div_val = div.get_div();
    if (div_val != BOOT_DIV) {
      LOG_R("div fail!\n");
      *reinterpret_cast<int*>(SENTINEL4) = 1;
    }
  }

  uint8_t dbg_read (const blob& cart_blob, int addr);
  void dbg_write(const blob& cart_blob, int addr, uint8_t data);

  void set_boot_bit(const blob& cart_blob) {
    dbg_write(cart_blob, 0xFF50, 0xFF);
  }

  //-----------------------------------------------------------------------------

  void run_phases(const blob& cart_blob, int phase_count) {
    for (int i = 0; i < phase_count; i++) {
      next_phase(cart_blob);
    }
  }

  void next_phase(const blob& cart_blob);

  void tock_slow(const blob& cart_blob, int pass_index);

  void update_framebuffer();

  uint8_t* reg_begin() { return (uint8_t*)(&sentinel1) + sizeof(sentinel1); }
  uint8_t* reg_end()   { return (uint8_t*)(&sentinel2); }

  int64_t commit_and_hash() {
    int64_t hash = 0;
#ifdef YES_HASH
    {
      uint8_t* a = reg_begin();
      uint8_t* b = reg_end();
      hash = hash_blob2(a, b - a);
    }
#endif

#ifdef USE_OLDNEW_FLAGS
    {
      uint8_t* a = reg_begin();
      uint8_t* b = reg_end();
      commit_blob(a, b - a);
    }
#endif
    return hash;
  }

  void check_state_old_and_driven_or_pulled() {
    auto s = reg_end() - reg_begin();
    for (auto i = 0; i < s; i++) {
      auto r = reg_begin()[i];
      (void)r;
      CHECK_P((r & 0xF0) == BIT_OLD);
      CHECK_P(bool(r & BIT_DRIVEN) != bool(r & BIT_PULLED));
    }
  }

  //-----------------------------------------------------------------------------

  void tock_lcdc();


  void tock_lyc();
  void tock_lcd();

  void tock_dma_gates();
  void tock_dma_logic();

  void set_pins();

  void tock_joypad();
  void tock_interrupts();
  void tock_clocks_gates();
  void tock_clocks_logic();
  void tock_vid_clocks_gates();
  void tock_vid_clocks_logic();
  void tock_div();
  void tock_timer();
  void tock_reset(DFF17 UPOF_DIV15p);
  void tock_ext(const blob& cart_blob);
  void tock_oam_bus();
  void tock_serial();
  void tock_vram_bus(wire TEVO_WIN_FETCH_TRIGp);
  void tock_zram_gates();
  void tock_zram_logic();
  void tock_pix_pipes(wire SACU_CLKPIPE_evn, wire NYXU_BFETCH_RSTn);

  void tock_bootrom_gates();
  void tock_bootrom_logic();

  void tock_window(wire SEGU_CLKPIPE_evn, wire REPU_VBLANKp);
  void tock_spu();

  void update_sprite_reset_flags(
    const Gate WUTY_SFETCH_DONE_TRIGp,
    const wire BYVA_LINE_RSTn,
    const SpriteMatchFlags& sprite_get_flag,
    SpriteResetFlags& sprite_reset_flags);

  void update_sprite_store_flags(
    const SpriteCounter& sprite_counter,
    const wire DYTY_COUNT_CLKp,
    SpriteStoreFlags& sprite_store_flags);

  void store_sprite(
    const SpriteStoreFlags& sprite_store_flags,
    const SpriteResetFlags& sprite_reset_flags,
    const wire BYVA_LINE_RSTn,
    const SpriteBus& sprite_bus,
    const OamTempB& oam_temp_b,
    GateBoySpriteStore& sprite_store);

  static void get_sprite_match_flags(
    const PixCount& pix_count,
    const wire AROR_MATCH_ENp,
    GateBoySpriteStore& sprite_store,
    SpriteMatchFlags& sprite_get_flag,
    SigIn SIG_GND);

  static void sprite_match_to_bus(
    const GateBoySpriteStore& sprite_store,
    const SpriteMatchFlags& sprite_get_flag,
    SpriteBus& sprite_bus);

  void sprite_scan_to_bus(SpriteDeltaY delta, NorLatch XYMU_RENDERINGn, Gate FEPO_STORE_MATCHp);

  void set_lcd_pins(wire SACU_CLKPIPE_evn);

  SpriteDeltaY sub_sprite_y();
  static void oam_latch_to_temp_a(wire COTA_OAM_CLKn, const OamLatchA& old_oam_latch_a, OamTempA& oam_temp_a);
  static void oam_latch_to_temp_b(wire COTA_OAM_CLKn, const OamLatchB& old_oam_latch_b, OamTempB& oam_temp_b);

  //----------------------------------------

  /*#p01.AVOR*/ wire AVOR_SYS_RSTp() const { return or2(rst.AFER_SYS_RSTp.qp_new(), rst.ASOL_POR_DONEn.qp_new()); }
  /*#p01.ALUR*/ wire ALUR_SYS_RSTn() const { return not1(AVOR_SYS_RSTp()); }
  /*#p01.DULA*/ wire DULA_SYS_RSTp() const { return not1(ALUR_SYS_RSTn()); }
  /*#p01.CUNU*/ wire CUNU_SYS_RSTn() const { return not1(DULA_SYS_RSTp()); }
  /*#p01.XORE*/ wire XORE_SYS_RSTp() const { return not1(CUNU_SYS_RSTn()); }
  /*_p01.XEBE*/ wire XEBE_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /*#p01.WALU*/ wire WALU_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /*_p01.WESY*/ wire WESY_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /*_p01.XARE*/ wire XARE_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /*_p03.MULO*/ wire MULO_SYS_RSTn() const { return not1(ALUR_SYS_RSTn()); }

  /*_p01.XODO*/ wire XODO_VID_RSTp() const { return nand2(XEBE_SYS_RSTn(), reg_lcdc.XONA_LCDC_LCDENn.qn_new()); }
  /*_p01.XAPO*/ wire XAPO_VID_RSTn() const { return not1(XODO_VID_RSTp()); }
  /*_p01.LYHA*/ wire LYHA_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*_p01.LYFE*/ wire LYFE_VID_RSTn() const { return not1(LYHA_VID_RSTp()); }
  /*_p01.TOFU*/ wire TOFU_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*_p01.ROSY*/ wire ROSY_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*#p01.ATAR*/ wire ATAR_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*#p01.ABEZ*/ wire ABEZ_VID_RSTn() const { return not1(ATAR_VID_RSTp()); }
  /*_p01.PYRY*/ wire PYRY_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*_p01.AMYG*/ wire AMYG_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }

  /*_p07.UBET*/ wire UBETp()           const { return not1(rst.PIN_77_T1.qp_int_new()); }
  /*_p07.UVAR*/ wire UVARp()           const { return not1(rst.PIN_76_T2.qp_int_new()); }
  /*_p07.UMUT*/ wire UMUT_MODE_DBG1p() const { return and2(rst.PIN_77_T1.qp_int_new(), UVARp()); }
  /*_p07.UNOR*/ wire UNOR_MODE_DBG2p() const { return and2(rst.PIN_76_T2.qp_int_new(), UBETp()); }
  /*_p07.UPOJ*/ wire UPOJ_MODE_PRODn() const { return nand3(UBETp(), UVARp(), rst.PIN_71_RST.qp_int_new()); }
  /*_p08.RYCA*/ wire RYCA_MODE_DBG2n() const { return not1(UNOR_MODE_DBG2p()); }
  /*_p08.TOVA*/ wire TOVA_MODE_DBG2n() const { return not1(UNOR_MODE_DBG2p()); }
  /*_p08.MULE*/ wire MULE_MODE_DBG1n() const { return not1(UMUT_MODE_DBG1p()); }
  /*_p25.TUTO*/ wire TUTO_VRAM_DBGp()  const { return and2(UNOR_MODE_DBG2p(), rst.SOTO_DBG_VRAMp.qn_new()); }

  /*_p01.UCOB*/ wire UCOB_CLKBADp() const { return not1(clk.PIN_74_CLK.clkgood()); }
  /*_p01.ATEZ*/ wire ATEZ_CLKBADp() const { return not1(clk.PIN_74_CLK.clkgood()); }

  /*_p01.ABOL*/ wire ABOL_CLKREQn() const { return not1(clk.SIG_CPU_CLKREQ.out_new()); }
  /*#p01.BUTY*/ wire BUTY_CLKREQp() const { return not1(ABOL_CLKREQn()); }

  wire AZOF_AxCxExGx() const {
    /*_p01.ATAL*/ wire ATAL_xBxDxFxH = not1(clk.AVET_DEGLITCH.out_new());
    /*_p01.AZOF*/ wire AZOF_AxCxExGx = not1(ATAL_xBxDxFxH);
    return AZOF_AxCxExGx;
  }

  /*_p01.ZAXY*/ wire ZAXY_xBxDxFxH() const { return not1(AZOF_AxCxExGx()); }
  /*#p01.ZEME*/ wire ZEME_AxCxExGx() const { return not1(ZAXY_xBxDxFxH()); }
  /*_p29.XYVA*/ wire XYVA_xBxDxFxH() const { return not1(ZEME_AxCxExGx()); }
  /*_p29.XOTA*/ wire XOTA_AxCxExGx() const { return not1(XYVA_xBxDxFxH()); }
  /*_p29.XYFY*/ wire XYFY_xBxDxFxH() const { return not1(XOTA_AxCxExGx()); }
  /*#p01.ALET*/ wire ALET_xBxDxFxH() const { return not1(ZEME_AxCxExGx()); }
  /*_p01.LAPE*/ wire LAPE_AxCxExGx() const { return not1(ALET_xBxDxFxH()); }
  /*#p27.MEHE*/ wire MEHE_AxCxExGx() const { return not1(ALET_xBxDxFxH()); }
  /*#p27.MYVO*/ wire MYVO_AxCxExGx() const { return not1(ALET_xBxDxFxH()); }
  /*_p27.MOXE*/ wire MOXE_AxCxExGx() const { return not1(ALET_xBxDxFxH()); }
  /*_p27.TAVA*/ wire TAVA_xBxDxFxH() const { return not1(LAPE_AxCxExGx()); }

  /*#p01.ATYP*/ wire ATYP_ABCDxxxx() const { return not1(clk.AFUR_xxxxEFGH.qp_new()); }
  /*#p01.AFEP*/ wire AFEP_AxxxxFGH() const { return not1(clk.ALEF_AxxxxFGH.qn_new()); }
  /*#p01.AROV*/ wire AROV_xxCDEFxx() const { return not1(clk.APUK_ABxxxxGH.qp_new()); }
  /*#p01.ADAR*/ wire ADAR_ABCxxxxH() const { return not1(clk.ADYK_ABCxxxxH.qn_new()); }

  /*#p01.BEKO*/ wire BEKO_ABCDxxxx() const { return not1(BUDE_xxxxEFGH()); } // BEKO+BAVY parallel
  /*#p01.BAPY*/ wire BAPY_xxxxxxGH() const { return nor3(ABOL_CLKREQn(), AROV_xxCDEFxx(), ATYP_ABCDxxxx()); }
  /*#p01.BERU*/ wire BERU_ABCDEFxx() const { return not1(BAPY_xxxxxxGH()); }
  /*#p01.BUFA*/ wire BUFA_xxxxxxGH() const { return not1(BERU_ABCDEFxx()); }
  /*#p01.BOLO*/ wire BOLO_ABCDEFxx() const { return not1(BUFA_xxxxxxGH()); }
  /*#p01.BEJA*/ wire BEJA_xxxxEFGH() const { return nand4(BOLO_ABCDEFxx(), BOLO_ABCDEFxx(), BEKO_ABCDxxxx(), BEKO_ABCDxxxx()); }
  /*#p01.BANE*/ wire BANE_ABCDxxxx() const { return not1(BEJA_xxxxEFGH()); }
  /*#p01.BELO*/ wire BELO_xxxxEFGH() const { return not1(BANE_ABCDxxxx()); }
  /*#p01.BAZE*/ wire BAZE_ABCDxxxx() const { return not1(BELO_xxxxEFGH()); }
  /*#p01.BUTO*/ wire BUTO_xBCDEFGH() const { return nand3(AFEP_AxxxxFGH(), ATYP_ABCDxxxx(), BAZE_ABCDxxxx()); }
  /*#p01.BELE*/ wire BELE_Axxxxxxx() const { return not1(BUTO_xBCDEFGH()); }
  /*#p01.BYJU*/ wire BYJU_Axxxxxxx() const { return or2(BELE_Axxxxxxx(), ATEZ_CLKBADp()); }
  /*#p01.BALY*/ wire BALY_xBCDEFGH() const { return not1(BYJU_Axxxxxxx()); }
  /*_p01.BOGA*/ wire BOGA_Axxxxxxx() const { return not1(BALY_xBCDEFGH()); }
  /*#p01.BUVU*/ wire BUVU_Axxxxxxx() const { return and2(BALY_xBCDEFGH(), BUTY_CLKREQp()); }
  /*#p01.BYXO*/ wire BYXO_xBCDEFGH() const { return not1(BUVU_Axxxxxxx()); }
  /*#p01.BEDO*/ wire BEDO_Axxxxxxx() const { return not1(BYXO_xBCDEFGH()); }
  /*#p01.BOWA*/ wire BOWA_xBCDEFGH() const { return not1(BEDO_Axxxxxxx()); }
  /*#p01.BUGO*/ wire BUGO_xBCDExxx() const { return not1(AFEP_AxxxxFGH()); }
  /*#p01.BATE*/ wire BATE_AxxxxxGH() const { return nor3(BUGO_xBCDExxx(), AROV_xxCDEFxx(), ABOL_CLKREQn()); }
  /*#p01.BASU*/ wire BASU_xBCDEFxx() const { return not1(BATE_AxxxxxGH()); }
  /*#p01.BUKE*/ wire BUKE_AxxxxxGH() const { return not1(BASU_xBCDEFxx()); }
  /*#p01.BOMA*/ wire BOMA_xBCDEFGH() const { return not1(BOGA_Axxxxxxx()); }
  /*#p01.BELU*/ wire BELU_xxxxEFGH() const { return nor2(ATYP_ABCDxxxx(), ABOL_CLKREQn()); }
  /*#p01.BYRY*/ wire BYRY_ABCDxxxx() const { return not1(BELU_xxxxEFGH()); }
  /*#p01.BUDE*/ wire BUDE_xxxxEFGH() const { return not1(BYRY_ABCDxxxx()); }
  /*_p01.UVYT*/ wire UVYT_ABCDxxxx() const { return not1(BUDE_xxxxEFGH()); }
  /*_p04.MOPA*/ wire MOPA_xxxxEFGH() const { return not1(UVYT_ABCDxxxx()); }
  /*_p28.XYNY*/ wire XYNY_ABCDxxxx() const { return not1(MOPA_xxxxEFGH()); }

  /*#p21.TALU*/ wire TALU_xxCDEFxx() const { return not1(clk.VENA_xxCDEFxx.qn_new()); }
  /*#p29.XUPY*/ wire XUPY_ABxxEFxx() const { return not1(clk.WUVU_ABxxEFxx.qn_new()); }
  /*#p29.XOCE*/ wire XOCE_xBCxxFGx() const { return not1(clk.WOSU_AxxDExxH.qp_new()); }
  /*#p29.WOJO*/ wire WOJO_AxxxExxx() const { return nor2(clk.WOSU_AxxDExxH.qn_new(), clk.WUVU_ABxxEFxx.qn_new()); }
  /*#p21.SONO*/ wire SONO_ABxxxxGH() const { return not1(TALU_xxCDEFxx()); }
  /*_p29.XYSO*/ wire XYSO_xBCDxFGH() const { return not1(WOJO_AxxxExxx()); }
  /*#p30.CYKE*/ wire CYKE_ABxxEFxx() const { return not1(XUPY_ABxxEFxx()); }
  /*#p30.WUDA*/ wire WUDA_xxCDxxGH() const { return not1(CYKE_ABxxEFxx()); }
  /*#p28.AWOH*/ wire AWOH_xxCDxxGH() const { return not1(XUPY_ABxxEFxx()); }
  /*#p01.AJAX*/ wire AJAX_xxxxEFGH() const { return not1(ATYP_ABCDxxxx()); }

  //-----------------------------------------------------------------------------

  /*_p07.AJAS*/ wire AJAS_CPU_RDn      () const { return not1(cpu_signals.TEDO_CPU_RDp.out_new()); }
  /*_p07.DYKY*/ wire DYKY_CPU_WRn      () const { return not1(cpu_signals.TAPU_CPU_WRp.out_new()); }
  /*_p07.ASOT*/ wire ASOT_CPU_RDp      () const { return not1(AJAS_CPU_RDn()); }
  /*_p28.MYNU*/ wire MYNU_CPU_RDn      () const { return nand2(ASOT_CPU_RDp(), CATY_LATCH_EXTp()); }
  /*_p28.LEKO*/ wire LEKO_CPU_RDp      () const { return not1(MYNU_CPU_RDn()); }
  /*_p07.CUPA*/ wire CUPA_CPU_WRp      () const { return not1(DYKY_CPU_WRn()); }
  /*_p08.REDU*/ wire REDU_CPU_RDn      () const { return not1(cpu_signals.TEDO_CPU_RDp.out_new()); }
  /*_p08.MEXO*/ wire MEXO_CPU_WRn      () const { return not1(cpu_signals.APOV_CPU_WRp.out_new()); }

  /*_p04.DECY*/ wire DECY_LATCH_EXTn   () const { return not1(cpu_signals.SIG_IN_CPU_LATCH_EXT.out_new()); }
  /*_p04.CATY*/ wire CATY_LATCH_EXTp   () const { return not1(DECY_LATCH_EXTn()); }
  /*#p28.BOFE*/ wire BOFE_LATCH_EXTn   () const { return not1(CATY_LATCH_EXTp()); }

  /*#p08.TEXO*/ wire TEXO_ADDR_VRAMn   () const { return and2(cpu_signals.SIG_IN_CPU_EXT_BUSp.out_new(), new_bus.TEVY_ADDR_VRAMn()); }
  /*#p25.TEFA*/ wire TEFA_ADDR_VRAMp   () const { return nor2(new_bus.SYRO_FE00_FFFF(), TEXO_ADDR_VRAMn()); }
  /*#p25.SOSE*/ wire SOSE_ADDR_VRAMp   () const { return and2(TEFA_ADDR_VRAMp(), new_bus.BUS_CPU_A15p.out_new()); }
  /*_p08.LEVO*/ wire LEVO_ADDR_VRAMn   () const { return not1(TEXO_ADDR_VRAMn()); }
  /*_p25.TUJA*/ wire TUJA_CPU_VRAM_WRp () const { return and2(SOSE_ADDR_VRAMp(), cpu_signals.APOV_CPU_WRp.out_new()); }

  wire TOLE_CPU_VRAM_RDp() const
  {
   // Ignoring debug for now
    ///*#p25.TUCA*/ wire TUCA_CPU_VRAM_RDp =  and2(SOSE_ADDR_VRAMp(), ABUZ_EXT_RAM_CS_CLK);
    ///*#p25.TAVY*/ wire TAVY_MOEp         = not1(vram_bus.PIN_45_VRAM_OEn.qn_new());
    ///*#p25.TEFY*/ wire TEFY_VRAM_MCSp    = not1(vram_bus.PIN_43_VRAM_CSn.qn_new());
    ///*#p25.TOLE*/ wire TOLE_CPU_VRAM_RDp = mux2p(TEFY_VRAM_MCSp, TUTO_DBG_VRAMp, TUCA_CPU_VRAM_RDp);

    /*#p25.TUCA*/ wire TUCA_CPU_VRAM_RDp = nand2(SOSE_ADDR_VRAMp(), cpu_signals.ABUZ_EXT_RAM_CS_CLK.out_new());
    /*#p25.TOLE*/ wire TOLE_CPU_VRAM_RDp = not1(TUCA_CPU_VRAM_RDp);

    return TOLE_CPU_VRAM_RDp;
  }

  wire SALE_CPU_VRAM_WRn() const
  {
    // Ignoring debug for now
    ///*#p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = nand2(SOSE_ADDR_VRAMp(), SIG_IN_CPU_WRp.qp_new());  // Schematic wrong, second input is SIG_IN_CPU_WRp
    ///*#p25.TAVY*/ wire TAVY_MOEp         = not1(vram_bus.PIN_45_VRAM_OEn.qn_new());
    ///*#p25.TEFY*/ wire TEFY_VRAM_MCSp    = not1(vram_bus.PIN_43_VRAM_CSn.qn_new());
    ///*#p25.SALE*/ wire SALE_CPU_VRAM_WRn = mux2p(TUTO_DBG_VRAMp, TAVY_MOEp, TEGU_CPU_VRAM_WRn);

    /*#p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = and2(SOSE_ADDR_VRAMp(), cpu_signals.SIG_IN_CPU_WRp.out_new());  // Schematic wrong, second input is SIG_IN_CPU_WRp
    /*#p25.SALE*/ wire SALE_CPU_VRAM_WRn = not1(TEGU_CPU_VRAM_WRn);

    return SALE_CPU_VRAM_WRn;
  }

  //-----------------------------------------------------------------------------

  void reset_sprite_store();

  void dump_sys(Dumper& d) const;

  void dump_cpu_bus(Dumper& d);
  void dump_ext_bus(Dumper& d);
  void dump_vram_bus(Dumper& d);
  void dump_oam_bus(Dumper& d);

  void dump_sprite_scanner(Dumper& d);

  void dump_tile_fetcher(Dumper& d);
  void dump_sprite_fetcher(Dumper& d);

  void dump_resets(Dumper& d);

  void dump_timer(Dumper& d);
  void dump_clocks(Dumper& d);
  void dump_interrupts(Dumper& d);
  void dump_joypad(Dumper& d);
  void dump_lcd(Dumper& d);
  void dump_sprite_store(Dumper& d);
  void dump_mbc1(Dumper& d);
  void dump_dma(Dumper& d);
  void dump_serial(Dumper& d);
  void dump_ppu(Dumper& d);
  void dump_spu(Dumper& d);

  //-----------------------------------------------------------------------------
  // All the SOC registers, pins, buses. Everything between sentinel 1 and
  // sentinel 2 _must_ derive from BitBase.

  uint64_t sentinel1 = SENTINEL1;

  //----------

  /*_SIG_VCC*/ SigIn SIG_VCC;
  /*_SIG_GND*/ SigIn SIG_GND;

  GateBoyCpuBus  old_bus;

  GateBoyCpuBus  new_bus;
  VramBus        vram_bus;
  SpriteBus      sprite_bus;
  GateBoyOamBus  oam_bus;


  /*#p21.VOGA*/ DFF17 VOGA_HBLANKp;                   // ABxDxFxH Clocked on odd, reset on A
  /*#p21.XYMU*/ NorLatch XYMU_RENDERINGn;             // ABxDxFxH Cleared on A, set on BDFH

  GateBoyCpuSignals cpu_signals;
  GateBoyExtPins    ext_pins;
  GateBoyVramPins   vram_pins;
  GateBoyOam        oam;
  GateBoyZram       zram_bus;

  OamLatchA oam_latch_a;
  OamLatchB oam_latch_b;
  OamTempA oam_temp_a;
  OamTempB oam_temp_b;

  ExtDataLatch ext_data_latch;
  ExtAddrLatch ext_addr_latch;

  GateBoyResetDebug rst;
  GateBoyClock      clk;
  GateBoyDiv        div;
  GateBoyTimer      timer;
  GateBoyDMA        dma;
  GateBoyInterrupts interrupts;
  GateBoyJoypad     joy;
  GateBoySerial     serial;

  GateBoySpriteStore   sprite_store;

  SpriteCounter sprite_counter;

  SpriteMatchFlags sprite_match_flags;
  SpriteResetFlags sprite_reset_flags;
  SpriteStoreFlags sprite_store_flags;

  SpriteScanner sprite_scanner;

  SpriteFetcher sprite_fetcher;
  SpritePixA sprite_pix_a;
  SpritePixB sprite_pix_b;

  TileFetcher   tile_fetcher;
  TileTempA tile_temp_a;
  TileTempB tile_temp_b;

  RegLCDC reg_lcdc;
  RegStat reg_stat;
  RegSCX  reg_scx;
  RegSCY  reg_scy;
  RegWY   reg_wy;
  RegWX   reg_wx;

  WinCoords win_coords;

  WindowRegisters win_reg;
  FineScroll      fine_scroll;

  PixCount     pix_count;
  PixelPipes   pix_pipes;
  GateBoyLCD   lcd;

  RegLX  reg_lx;
  RegLY  reg_ly;
  RegLYC reg_lyc;

  RegBGP  reg_bgp;
  RegOBP0 reg_obp0;
  RegOBP1 reg_obp1;

  /*#p21.WODU*/ Gate WODU_HBLANKp;
  /*_p07.SATO*/ Gate SATO_BOOT_BITn;
  /*_p28.ATEJ*/ Gate ATEJ_LINE_RSTp;

  SpritePix flipped_sprite;

  //NR10 reg_NR10;
  //NR11 reg_NR11;
  //NR12 reg_NR12;
  //NR14 reg_NR14;

  //NR21 reg_NR21;
  //NR22 reg_NR22;
  //NR24 reg_NR24;

  //NR30 reg_NR30;
  //NR31 reg_NR31;
  //NR32 reg_NR32;
  //NR34 reg_NR34;

  //NR41 reg_NR41;
  //NR42 reg_NR42;
  //NR43 reg_NR43;
  //NR44 reg_NR44;

  NR50 reg_NR50;
  NR51 reg_NR51;
  NR52 reg_NR52;

  //-----------------------------------------------------------------------------
  // Everything between sentinel 2 and 3 is checked in test_reset_cart_vs_dump

  uint64_t sentinel2 = SENTINEL2;

  uint8_t sys_rst = 0;
  uint8_t sys_t1 = 0;
  uint8_t sys_t2 = 0;
  uint8_t sys_clken = 0;
  uint8_t sys_clkgood = 0;
  uint8_t sys_clkreq = 0;
  uint8_t sys_cpu_en = 0;
  uint8_t sys_fastboot = 0;
  uint8_t sys_buttons = 0;

  //-----------------------------------------------------------------------------
  // CPU

  MetroBoyCPU gb_cpu;
  Req      bus_req_new = {0};
  uint8_t  cpu_data_latch = 0;
  uint8_t  imask_latch = 0;
  uint8_t  intf_latch = 0;
  uint8_t  intf_latch_delay = 0;
  uint8_t  intf_halt_latch = 0;

  //-----------------------------------------------------------------------------
  // Memory

  uint8_t vid_ram [8192];
  uint8_t cart_ram[32768];
  uint8_t int_ram [8192];
  uint8_t oam_ram [256];
  uint8_t zero_ram[128];

  uint64_t sentinel3 = SENTINEL3;

  //-----------------------------------------------------------------------------
  // Everything below here is "external" state not visible to the gameboy itself.

  int line_phase_x = 0; // Position in the current scanline, in phases since ATEJ_LINE_RSTp fired. For debugging.
  int old_lcd_x = 0;
  int old_lcd_y = 0;

  uint8_t framebuffer[160*144];

  //-----------------------------------------------------------------------------
  // Bookkeeping

#ifdef YES_LOGIC_VS_GATES
  bool logic_mode = false;
#else
  #ifdef FAST_MODE
    const bool logic_mode = true;
  #else
    const bool logic_mode = false;
  #endif
#endif

  double   sim_time = 0;
  uint64_t phase_total = 0;
  uint64_t phase_origin = 0;

  uint8_t phase_mask_old = 0;
  uint8_t phase_mask_new = 0;

  uint64_t phase_hash = 0;
  uint64_t cumulative_hash = 0;

  Probes   probes;

  uint64_t sentinel4 = SENTINEL4;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------
