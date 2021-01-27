#pragma once

#include "CoreLib/File.h"
#include "CoreLib/MetroBoyCPU.h"
#include "CoreLib/Constants.h"

#include "GateBoyLib/Probe.h"

#include "GateBoyLib/GateBoyBootrom.h"
#include "GateBoyLib/GateBoyBuses.h"
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
#include "GateBoyLib/GateBoyPins.h"

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct GateBoy {

  void dump(Dumper& d) const;

  void reset_to_bootrom(bool fastboot);
  void reset_to_cart();
  void load_cart(uint8_t* _boot_buf, size_t _boot_size,
                uint8_t* _cart_buf, size_t _cart_size);

  //----------------------------------------

  void from_blob(const blob& b) {
    ASSERT_P(b.size() == sizeof(*this));
    memcpy(this, b.data(), sizeof(*this));
    check_sentinel();
  }

  void to_blob(blob& b) {
    uint8_t* bytes = (uint8_t*)this;
    b.insert(b.end(), bytes, bytes + sizeof(*this));
  }

  //----------------------------------------

  void check_div() const {
    int div_val = div.get_div();
    if (div_val != BOOT_DIV) {
      printf("div fail!\n");
      *reinterpret_cast<int*>(SENTINEL4) = 1;
    }
  }

  static bool check_sentinel(const blob& b) {
    if (b.size() != sizeof(GateBoy)) return false;

    GateBoy* gb = (GateBoy*)b.data();
    if (gb->sentinel1 != SENTINEL1) return false;
    if (gb->sentinel2 != SENTINEL2) return false;
    if (gb->sentinel3 != SENTINEL3) return false;
    if (gb->sentinel4 != SENTINEL4) return false;
    return true;
  }

  void check_sentinel() const {
    ASSERT_P(sentinel1 == SENTINEL1);
    ASSERT_P(sentinel2 == SENTINEL2);
    ASSERT_P(sentinel3 == SENTINEL3);
    ASSERT_P(sentinel4 == SENTINEL4);
  }

  uint8_t dbg_read (int addr);
  void dbg_write(int addr, uint8_t data);

  void set_boot_bit() {
    dbg_write(0xFF50, 0xFF);
  }

  //-----------------------------------------------------------------------------

  void run_phases(int phase_count) {
    for (int i = 0; i < phase_count; i++) {
      next_phase();
    }
  }

  void next_phase();

  void tock_slow(int pass_index);

  void update_framebuffer();

  uint8_t* reg_begin() { return (uint8_t*)(&sentinel1) + sizeof(sentinel1); }
  uint8_t* reg_end()   { return (uint8_t*)(&sentinel2); }

  int64_t commit_and_hash() {
    int64_t hash = 0;
#ifdef USE_HASH
    {
      uint8_t* a = (uint8_t*)(&sentinel1);
      uint8_t* b = (uint8_t*)(&sentinel2);
      hash = hash_blob2(a, b - a);
    }
#endif

#ifdef USE_COMMIT
    {
      uint8_t* a = reg_begin();
      uint8_t* b = reg_end();
      commit_blob(a, b - a);
    }
#endif
    return hash;
  }

  void check_state_old_and_driven_or_pullup() {
    auto s = reg_end() - reg_begin();
    for (auto i = 0; i < s; i++) {
      auto r = reg_begin()[i];
      (void)r;
      CHECK_P((r & 0xF0) == BIT_OLD);
      CHECK_P((r & BIT_DRIVEN) || (r & BIT_PULLUP));
    }
  }

  static wire ROGE_WY_MATCHp_old(const RegWY& reg_wy_old, const RegLY& reg_ly_old, DFF9 WYMO_LCDC_WINENn_old) {
    /*#p27.NAZE*/ wire _NAZE_WY_MATCH0p = xnor2(reg_wy_old.NESO_WY0n.qn_old(), reg_ly_old.MUWY_LY0p.qp_old());
    /* p27.PEBO*/ wire _PEBO_WY_MATCH1p = xnor2(reg_wy_old.NYRO_WY1n.qn_old(), reg_ly_old.MYRO_LY1p.qp_old());
    /* p27.POMO*/ wire _POMO_WY_MATCH2p = xnor2(reg_wy_old.NAGA_WY2n.qn_old(), reg_ly_old.LEXA_LY2p.qp_old());
    /* p27.NEVU*/ wire _NEVU_WY_MATCH3p = xnor2(reg_wy_old.MELA_WY3n.qn_old(), reg_ly_old.LYDO_LY3p.qp_old());
    /* p27.NOJO*/ wire _NOJO_WY_MATCH4p = xnor2(reg_wy_old.NULO_WY4n.qn_old(), reg_ly_old.LOVU_LY4p.qp_old());
    /* p27.PAGA*/ wire _PAGA_WY_MATCH5p = xnor2(reg_wy_old.NENE_WY5n.qn_old(), reg_ly_old.LEMA_LY5p.qp_old());
    /* p27.PEZO*/ wire _PEZO_WY_MATCH6p = xnor2(reg_wy_old.NUKA_WY6n.qn_old(), reg_ly_old.MATO_LY6p.qp_old());
    /* p27.NUPA*/ wire _NUPA_WY_MATCH7p = xnor2(reg_wy_old.NAFU_WY7n.qn_old(), reg_ly_old.LAFO_LY7p.qp_old());

    /*#p27.PALO*/ wire _PALO_WY_MATCHn  = nand5(WYMO_LCDC_WINENn_old.qn_old(), _NOJO_WY_MATCH4p, _PAGA_WY_MATCH5p, _PEZO_WY_MATCH6p, _NUPA_WY_MATCH7p);
    /* p27.NELE*/ wire _NELE_WY_MATCHp  = not1(_PALO_WY_MATCHn);
    /* p27.PAFU*/ wire _PAFU_WY_MATCHn  = nand5(_NELE_WY_MATCHp, _NAZE_WY_MATCH0p, _PEBO_WY_MATCH1p, _POMO_WY_MATCH2p, _NEVU_WY_MATCH3p);
    /* p27.ROGE*/ wire _ROGE_WY_MATCHp  = not1(_PAFU_WY_MATCHn);
    return _ROGE_WY_MATCHp;
  }

  static wire NUKO_WX_MATCHp_old(const PixCount& pix_count_old, const RegWX& reg_wx_old, NorLatch REJO_WY_MATCH_LATCHp_old) {
    /*#p27.MYLO*/ wire _MYLO_WX_MATCH0p = xnor2(pix_count_old.XEHO_PX0p.qp_old(), reg_wx_old.MYPA_WX0n.qn_old());
    /* p27.PUWU*/ wire _PUWU_WX_MATCH1p = xnor2(pix_count_old.SAVY_PX1p.qp_old(), reg_wx_old.NOFE_WX1n.qn_old());
    /* p27.PUHO*/ wire _PUHO_WX_MATCH2p = xnor2(pix_count_old.XODU_PX2p.qp_old(), reg_wx_old.NOKE_WX2n.qn_old());
    /* p27.NYTU*/ wire _NYTU_WX_MATCH3p = xnor2(pix_count_old.XYDO_PX3p.qp_old(), reg_wx_old.MEBY_WX3n.qn_old());
    /* p27.NEZO*/ wire _NEZO_WX_MATCH4p = xnor2(pix_count_old.TUHU_PX4p.qp_old(), reg_wx_old.MYPU_WX4n.qn_old());
    /* p27.NORY*/ wire _NORY_WX_MATCH5p = xnor2(pix_count_old.TUKY_PX5p.qp_old(), reg_wx_old.MYCE_WX5n.qn_old());
    /* p27.NONO*/ wire _NONO_WX_MATCH6p = xnor2(pix_count_old.TAKO_PX6p.qp_old(), reg_wx_old.MUVO_WX6n.qn_old());
    /* p27.PASE*/ wire _PASE_WX_MATCH7p = xnor2(pix_count_old.SYBE_PX7p.qp_old(), reg_wx_old.NUKU_WX7n.qn_old());

    /* p27.PUKY*/ wire _PUKY_WX_MATCHn  = nand5(REJO_WY_MATCH_LATCHp_old.qp_old(), _NEZO_WX_MATCH4p, _NORY_WX_MATCH5p, _NONO_WX_MATCH6p, _PASE_WX_MATCH7p);
    /* p27.NUFA*/ wire _NUFA_WX_MATCHp  = not1(_PUKY_WX_MATCHn);
    /* p27.NOGY*/ wire _NOGY_WX_MATCHn  = nand5(_NUFA_WX_MATCHp, _MYLO_WX_MATCH0p, _PUWU_WX_MATCH1p, _PUHO_WX_MATCH2p, _NYTU_WX_MATCH3p);
    /* p27.NUKO*/ wire _NUKO_WX_MATCHp  = not1(_NOGY_WX_MATCHn);
    return _NUKO_WX_MATCHp;
  }

  //-----------------------------------------------------------------------------

  void read_boot_bit();
  void write_boot_bit_sync();
  void read_bootrom();

  void reg_lcdc_read();
  void reg_lcdc_write();

  void tock_joypad();

  /* p02.ASOK*/ wire ASOK_INT_JOYp() const { return and2(joy.APUG_JP_GLITCH3.qp_new(), joy.BATU_JP_GLITCH0.qp_new()); }

  void reg_stat_read();
  void reg_stat_write();
  void reg_stat_tock();

  void tock_interrupts(wire WODU_HBLANKp);
  void read_intf();
  void read_ie();
  void write_ie();

  void tock_clocks();
  void tock_vid_clocks();

  SpritePix flip_sprite_pix(wire TEXY_SFETCHINGp, DFF8n BAXO_OAM_DB5p);

  SpriteDeltaY sub_sprite_y();

  void tock_sprite_fetcher(wire TEKY_SFETCH_REQp_old);

  void set_cpu_pins();

  void reg_wy_read();
  void reg_wy_write();

  void reg_wx_read();
  void reg_wx_write();

  void reg_bgp_read();
  void reg_bgp_write();

  void reg_obp0_read();
  void reg_obp0_write();

  void reg_obp1_read();
  void reg_obp1_write();

  void reg_div_tock();
  void reg_div_read();
  void reg_tima_read();
  void reg_tma_read();
  void reg_tma_write();
  void reg_tac_read();
  void reg_tac_write();
  void tock_timer();

  void tock_pix_counter(wire SACU_CLKPIPE_evn);

  BGScrollX add_scx();
  BGScrollY add_scy();

  void reg_scy_read();
  void reg_scy_write();

  void reg_scx_read();
  void reg_scx_write();

  void reg_ly_read();
  void reg_ly_write();
  void reg_ly_tock2();

  wire TEGY_STROBE() const;
  void reg_lx_tock();
  void reg_lyc_read();
  void reg_lyc_write();
  void reg_lyc_tock2();

  void store_tile_temp_a();
  void store_tile_temp_b();

  void store_sprite_pix_a(SpritePix sprite_pix_old);
  void store_sprite_pix_b(SpritePix sprite_pix_old);

  void tock_win_map_x(wire TEVO_WIN_FETCH_TRIGp);
  void tock_win_map_y();

  void tock_tile_fetcher(wire NYXU_BFETCH_RSTn, wire MOCE_BFETCH_DONEn_old);

  wire LOMA_LATCH_TILE_DAn() const;
  wire LABU_LATCH_TILE_DBn() const;

  void tock_reset(DFF17 UPOF_DIV15p);

  void set_lcd_pin_ctrl();

  void copy_cpu_addr_to_addr_latch();
  void copy_addr_latch_to_pins();
  void copy_pins_to_data_latch();
  void copy_data_latch_to_cpu_bus();
  void copy_cpu_data_to_pins();
  void set_ext_control_pins();
  void read_ext_to_pins();
  void write_pins_to_ext();

  wire BOGE_DMA_RUNNINGn() const;
  wire DUGA_DMA_RUNNINGn() const;
  wire LUMA_DMA_CARTp() const;
  wire LUFA_DMA_VRAMp() const;
  void reg_dma_write();
  void reg_dma_tock();
  void reg_dma_read();

  wire ACYL_SCANNINGp();
  wire AVAP_SCAN_DONE_TRIGp();
  void tock_sprite_scanner();
  void ext_to_oam_data_bus();
  void vram_to_oam_data_bus();

  void dma_to_oam_addr_bus();
  void sprite_index_to_oam_addr_bus();
  void scan_index_to_oam_addr_bus();
  void cpu_to_oam_addr_bus();
  void cpu_to_oam_data_bus();
  void set_oam_pin_clk();
  void set_oam_pin_wr();
  void set_oam_pin_oe();
  void tock_oam_bus();
  void latch_oam_data_bus();
  void oam_latch_to_cpu();

  void oam_latch_to_temp_a();
  void oam_latch_to_temp_b();

  void tock_serial();

  void cpu_addr_to_vram_addr();
  void dma_addr_to_vram_addr();
  void scroll_to_vram_addr(BGScrollX scroll_x, BGScrollY scroll_y);
  void win_to_vram_addr();
  void tile_to_vram_addr(const BGScrollY scroll_y);
  void sprite_to_vram_addr();
  void vram_addr_to_pins();
  void cpu_data_to_vram_bus_data();
  void vram_bus_data_to_pins();
  void set_vram_pin_cs();
  void set_vram_pin_wr();
  void set_vram_pin_oe();
  void read_vram();
  void write_vram();
  void vram_pins_to_data_bus();
  void vram_data_bus_to_cpu_bus();

  void read_zram();
  void write_zram();

  void update_count(wire _DYTY_STORE_CLKp);
  SpriteStoreFlag get_store_flags(wire _DYTY_STORE_CLKp);
  SpriteMatchFlag get_match_flags_old(wire _AROR_MATCH_ENp_old);
  SpriteMatchFlag get_match_flags_new(wire _AROR_MATCH_ENp_new);
  SpriteFirstMatch get_first_match(SpriteMatchFlag match_flag) const;
  void get_sprite(SpriteFirstMatch first_match);
  void store_sprite_x(SpriteStoreFlag store_flag, SpriteFirstMatch sprite_flag);
  void store_sprite_index(SpriteStoreFlag store_flag);
  void store_sprite_line(SpriteStoreFlag store_flag);
  void ly_to_sprite_line(wire FEPO_STORE_MATCHp);

  void tock_lcd(wire XYVO_y144p_old, DFF17 RUTU_x113p_old);
  void set_lcd_pin_flip(DFF17  TULU_DIV07p, DFF9 XONA_LCDC_LCDENn);
  void set_lcd_pin_vsync();
  void set_lcd_pin_hsync(wire TYFA_CLKPIPE_odd, DFF17 XYDO_PX3p_old);
  void set_lcd_pin_data(wire REMY_LD0n, wire RAVO_LD1n);
  void set_lcd_pin_latch();
  void set_lcd_pin_clock(wire SACU_CLKPIPE_evn);
  void update_lcd_pipe();

  void tock_bgw_pipe(wire SACU_CLKPIPE_evn, wire NYXU_BFETCH_RSTn);
  void tock_sprite_pipe(wire SACU_CLKPIPE_evn);
  void tock_mask_pipe(wire SACU_CLKPIPE_evn);
  void tock_pal_pipe(wire SACU_CLKPIPE_evn);
  void tock_pix_output();

  void tock_fine_scroll(wire TYFA_CLKPIPE_odd, wire TEVO_WIN_FETCH_TRIGp);

  /*#p25.ROPY*/ wire ROPY_RENDERINGn() const { return not1(XYMU_RENDERINGn.qn_new()); }
  /*#p25.SERE*/ wire SERE_CPU_VRAM_RDp() const { return and2(TOLE_CPU_VRAM_RDp(), ROPY_RENDERINGn()); }
  /*#p21.WEGO*/ wire WEGO_HBLANKp() const { return or2(TOFU_VID_RSTp(), VOGA_HBLANKp.qp_new()); }

  //----------------------------------------

  /*#p01.AVOR*/ wire AVOR_SYS_RSTp() const { return or2(rst.AFER_SYS_RSTp.qp_new(), rst.ASOL_POR_DONEn.qp_new()); }
  /*#p01.ALUR*/ wire ALUR_SYS_RSTn() const { return not1(AVOR_SYS_RSTp()); }
  /*#p01.DULA*/ wire DULA_SYS_RSTp() const { return not1(ALUR_SYS_RSTn()); }
  /*#p01.CUNU*/ wire CUNU_SYS_RSTn() const { return not1(DULA_SYS_RSTp()); }
  /*#p01.XORE*/ wire XORE_SYS_RSTp() const { return not1(CUNU_SYS_RSTn()); }
  /* p01.XEBE*/ wire XEBE_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /*#p01.WALU*/ wire WALU_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /* p01.WESY*/ wire WESY_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /* p01.XARE*/ wire XARE_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /* p03.MULO*/ wire MULO_SYS_RSTn() const { return not1(ALUR_SYS_RSTn()); }

  /* p01.XODO*/ wire XODO_VID_RSTp() const { return nand2(XEBE_SYS_RSTn(), rst._XONA_LCDC_LCDENn.qn_new()); }
  /* p01.XAPO*/ wire XAPO_VID_RSTn() const { return not1(XODO_VID_RSTp()); }
  /* p01.LYHA*/ wire LYHA_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /* p01.LYFE*/ wire LYFE_VID_RSTn() const { return not1(LYHA_VID_RSTp()); }
  /* p01.TOFU*/ wire TOFU_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /* p01.ROSY*/ wire ROSY_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*#p01.ATAR*/ wire ATAR_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*#p01.ABEZ*/ wire ABEZ_VID_RSTn() const { return not1(ATAR_VID_RSTp()); }
  /* p01.PYRY*/ wire PYRY_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /* p01.AMYG*/ wire AMYG_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }

  /* p07.UBET*/ wire UBETp()           const { return not1(rst.PIN_77_T1.qp_new()); }
  /* p07.UVAR*/ wire UVARp()           const { return not1(rst.PIN_76_T2.qp_new()); }
  /* p07.UMUT*/ wire UMUT_MODE_DBG1p() const { return and2(rst.PIN_77_T1.qp_new(), UVARp()); }
  /* p07.UNOR*/ wire UNOR_MODE_DBG2p() const { return and2(rst.PIN_76_T2.qp_new(), UBETp()); }
  /* p07.UPOJ*/ wire UPOJ_MODE_PRODn() const { return nand3(UBETp(), UVARp(), rst.PIN_71_RST.qp_new()); }
  /* p08.RYCA*/ wire RYCA_MODE_DBG2n() const { return not1(UNOR_MODE_DBG2p()); }
  /* p08.TOVA*/ wire TOVA_MODE_DBG2n() const { return not1(UNOR_MODE_DBG2p()); }
  /* p08.MULE*/ wire MULE_MODE_DBG1n() const { return not1(UMUT_MODE_DBG1p()); }
  /* p25.TUTO*/ wire TUTO_VRAM_DBGp()  const { return and2(UNOR_MODE_DBG2p(), rst.SOTO_DBG_VRAMp.qn_new()); }

  //----------------------------------------

  /* p28.ATEJ*/ wire ATEJ_LINE_RSTp_old() const;
  /* p27.XAHY*/ wire XAHY_LINE_RSTn_old() const { return not1(ATEJ_LINE_RSTp_old()); }
  /*#p28.ANOM*/ wire ANOM_LINE_RSTn_old() const { return nor2(ATEJ_LINE_RSTp_old(), ATAR_VID_RSTp_old()); }
  /* p28.ABAK*/ wire ABAK_LINE_RSTp_old() const { return  or2(ATEJ_LINE_RSTp_old(), AMYG_VID_RSTp_old()); }
  /*#p29.BALU*/ wire BALU_LINE_RSTp_old() const { return not1(ANOM_LINE_RSTn_old()); }
  /* p28.BYVA*/ wire BYVA_LINE_RSTn_old() const { return not1(ABAK_LINE_RSTp_old()); }
  /* p29.DYBA*/ wire DYBA_LINE_RSTp_old() const { return not1(BYVA_LINE_RSTn_old()); }
  /*#p29.BAGY*/ wire BAGY_LINE_RSTn_old() const { return not1(BALU_LINE_RSTp_old()); }
  /* p21.TADY*/ wire TADY_LINE_RSTn_old() const { return nor2(ATEJ_LINE_RSTp_old(), TOFU_VID_RSTp_old()); }

  /* p28.ATEJ*/ wire ATEJ_LINE_RSTp_new() const;
  /* p27.XAHY*/ wire XAHY_LINE_RSTn_new() const { return not1(ATEJ_LINE_RSTp_new()); }
  /*#p28.ANOM*/ wire ANOM_LINE_RSTn_new() const { return nor2(ATEJ_LINE_RSTp_new(), ATAR_VID_RSTp()); }
  /* p28.ABAK*/ wire ABAK_LINE_RSTp_new() const { return  or2(ATEJ_LINE_RSTp_new(), AMYG_VID_RSTp()); }
  /*#p29.BALU*/ wire BALU_LINE_RSTp_new() const { return not1(ANOM_LINE_RSTn_new()); }
  /* p28.BYVA*/ wire BYVA_LINE_RSTn_new() const { return not1(ABAK_LINE_RSTp_new()); }
  /* p29.DYBA*/ wire DYBA_LINE_RSTp_new() const { return not1(BYVA_LINE_RSTn_new()); }
  /*#p29.BAGY*/ wire BAGY_LINE_RSTn_new() const { return not1(BALU_LINE_RSTp_new()); }
  /* p21.TADY*/ wire TADY_LINE_RSTn_new() const { return nor2(ATEJ_LINE_RSTp_new(), TOFU_VID_RSTp()); }

  /*#p21.PARU*/ wire PARU_VBLANKp() const { return not1(lcd.POPU_VBLANKp.qn_new()); }

  ///* p01.XAPO*/ wire XAPO_VID_RSTn() const { return not1(XODO_VID_RSTp()); }
  ///* p01.TOFU*/ wire TOFU_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  ///*#p01.ATAR*/ wire ATAR_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  ///* p01.AMYG*/ wire AMYG_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  ///*#p01.ABEZ*/ wire ABEZ_VID_RSTn() const { return not1(ATAR_VID_RSTp()); }

  /*#p01.AVOR*/ wire AVOR_SYS_RSTp_old() const { return or2(rst.AFER_SYS_RSTp.qp_old(), rst.ASOL_POR_DONEn.qp_old()); }
  /*#p01.ALUR*/ wire ALUR_SYS_RSTn_old() const { return not1(AVOR_SYS_RSTp_old()); }
  /*#p01.DULA*/ wire DULA_SYS_RSTp_old() const { return not1(ALUR_SYS_RSTn_old()); }
  /*#p01.CUNU*/ wire CUNU_SYS_RSTn_old() const { return not1(DULA_SYS_RSTp_old()); }
  /*#p01.XORE*/ wire XORE_SYS_RSTp_old() const { return not1(CUNU_SYS_RSTn_old()); }
  /* p01.XEBE*/ wire XEBE_SYS_RSTn_old() const { return not1(XORE_SYS_RSTp_old()); }
  /* p01.XODO*/ wire XODO_VID_RSTp_old() const { return nand2(XEBE_SYS_RSTn_old(), rst._XONA_LCDC_LCDENn.qn_old()); }
  /* p01.XAPO*/ wire XAPO_VID_RSTn_old() const { return not1(XODO_VID_RSTp_old()); }
  /* p01.TOFU*/ wire TOFU_VID_RSTp_old() const { return not1(XAPO_VID_RSTn_old()); }
  /*#p01.ATAR*/ wire ATAR_VID_RSTp_old() const { return not1(XAPO_VID_RSTn_old()); }
  /* p01.AMYG*/ wire AMYG_VID_RSTp_old() const { return not1(XAPO_VID_RSTn_old()); }
  /*#p01.ABEZ*/ wire ABEZ_VID_RSTn_old() const { return not1(ATAR_VID_RSTp_old()); }

  //----------------------------------------


  /* p01.UCOB*/ wire UCOB_CLKBADp() const { return not1(pins.PIN_74_CLK.clock_good()); }
  /* p01.ATEZ*/ wire ATEZ_CLKBADp() const { return not1(pins.PIN_74_CLK.clock_good()); }

  /* p01.ABOL*/ wire ABOL_CLKREQn() const { return not1(clk.SIG_CPU_CLKREQ); }
  /*#p01.BUTY*/ wire BUTY_CLKREQp() const { return not1(ABOL_CLKREQn()); }

  wire AZOF_AxCxExGx() const {
    /* p01.ATAL*/ wire _ATAL_xBxDxFxH = not1(clk.AVET);
    /* p01.AZOF*/ wire _AZOF_AxCxExGx = not1(_ATAL_xBxDxFxH);
    return _AZOF_AxCxExGx;
  }

  /* p01.ZAXY*/ wire ZAXY_xBxDxFxH() const { return not1(AZOF_AxCxExGx()); }
  /*#p01.ZEME*/ wire ZEME_AxCxExGx() const { return not1(ZAXY_xBxDxFxH()); }
  /* p29.XYVA*/ wire XYVA_xBxDxFxH() const { return not1(ZEME_AxCxExGx()); }
  /* p29.XOTA*/ wire XOTA_AxCxExGx() const { return not1(XYVA_xBxDxFxH()); }
  /* p29.XYFY*/ wire XYFY_xBxDxFxH() const { return not1(XOTA_AxCxExGx()); }
  /*#p01.ALET*/ wire ALET_xBxDxFxH() const { return not1(ZEME_AxCxExGx()); }
  /* p01.LAPE*/ wire LAPE_AxCxExGx() const { return not1(ALET_xBxDxFxH()); }
  /*#p27.MEHE*/ wire MEHE_AxCxExGx() const { return not1(ALET_xBxDxFxH()); }
  /*#p27.MYVO*/ wire MYVO_AxCxExGx() const { return not1(ALET_xBxDxFxH()); }
  /* p27.MOXE*/ wire MOXE_AxCxExGx() const { return not1(ALET_xBxDxFxH()); }
  /* p27.TAVA*/ wire TAVA_xBxDxFxH() const { return not1(LAPE_AxCxExGx()); }

  /*#p01.ATYP*/ wire ATYP_ABCDxxxx() const { return not1(clk.AFUR_xxxxEFGHp.qp_new()); }
  /*#p01.AFEP*/ wire AFEP_AxxxxFGH() const { return not1(clk.ALEF_AxxxxFGHp.qn_new()); }
  /*#p01.AROV*/ wire AROV_xxCDEFxx() const { return not1(clk.APUK_ABxxxxGHp.qp_new()); }
  /*#p01.ADAR*/ wire ADAR_ABCxxxxH() const { return not1(clk.ADYK_ABCxxxxHp.qn_new()); }

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
  /* p01.BOGA*/ wire BOGA_Axxxxxxx() const { return not1(BALY_xBCDEFGH()); }
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
  /* p01.UVYT*/ wire UVYT_ABCDxxxx() const { return not1(BUDE_xxxxEFGH()); }
  /* p04.MOPA*/ wire MOPA_xxxxEFGH() const { return not1(UVYT_ABCDxxxx()); }
  /* p28.XYNY*/ wire XYNY_ABCDxxxx() const { return not1(MOPA_xxxxEFGH()); }

  /*#p21.TALU*/ wire TALU_xxCDEFxx() const { return not1(clk.VENA_xxCDEFxx.qn_new()); }
  /*#p29.XUPY*/ wire XUPY_ABxxEFxx() const { return not1(clk.WUVU_ABxxEFxx.qn_new()); }
  /*#p29.XOCE*/ wire XOCE_xBCxxFGx() const { return not1(clk.WOSU_AxxDExxH.qp_new()); }
  /*#p29.WOJO*/ wire WOJO_AxxxExxx() const { return nor2(clk.WOSU_AxxDExxH.qn_new(), clk.WUVU_ABxxEFxx.qn_new()); }
  /*#p21.SONO*/ wire SONO_ABxxxxGH() const { return not1(TALU_xxCDEFxx()); }
  /* p29.XYSO*/ wire XYSO_xBCDxFGH() const { return not1(WOJO_AxxxExxx()); }
  /*#p30.CYKE*/ wire CYKE_ABxxEFxx() const { return not1(XUPY_ABxxEFxx()); }
  /*#p30.WUDA*/ wire WUDA_xxCDxxGH() const { return not1(CYKE_ABxxEFxx()); }
  /*#p28.AWOH*/ wire AWOH_xxCDxxGH() const { return not1(XUPY_ABxxEFxx()); }
  /*#p01.AJAX*/ wire AJAX_xxxxEFGH() const { return not1(ATYP_ABCDxxxx()); }

  //-----------------------------------------------------------------------------

  /* p07.AJAS*/ wire AJAS_CPU_RDn      () const { return not1(cpu_bus.TEDO_CPU_RDp); }
  /* p07.DYKY*/ wire DYKY_CPU_WRn      () const { return not1(cpu_bus.TAPU_CPU_WRp); }
  /* p07.ASOT*/ wire ASOT_CPU_RDp      () const { return not1(AJAS_CPU_RDn()); }
  /* p28.LEKO*/ wire LEKO_CPU_RDp      () const { return not1(MYNU_CPU_RDn()); }
  /* p07.CUPA*/ wire CUPA_CPU_WRp      () const { return not1(DYKY_CPU_WRn()); }
  /* p28.MYNU*/ wire MYNU_CPU_RDn      () const { return nand2(ASOT_CPU_RDp(), CATY_LATCH_EXTp()); }
  /* p08.REDU*/ wire REDU_CPU_RDn      () const { return not1(cpu_bus.TEDO_CPU_RDp); }
  /* p08.MEXO*/ wire MEXO_CPU_WRn      () const { return not1(cpu_bus.APOV_CPU_WRp); }


  /* p04.DECY*/ wire DECY_LATCH_EXTn   () const { return not1(cpu_bus.SIG_CPU_LATCH_EXT); }
  /* p04.CATY*/ wire CATY_LATCH_EXTp   () const { return not1(DECY_LATCH_EXTn()); }
  /*#p28.BOFE*/ wire BOFE_LATCH_EXTn   () const { return not1(CATY_LATCH_EXTp()); }

  /* p07.TERA*/ wire TERA_BOOT_BITp    () const { return not1(cpu_bus.TEPU_BOOT_BITn_h.qp_new()); }
  /* p07.TUTU*/ wire TUTU_READ_BOOTROMp() const { return and2(TERA_BOOT_BITp(), new_bus.TULO_ADDR_BOOTROMp()); }

  /*#p08.TEXO*/ wire TEXO_ADDR_VRAMn   () const { return and2(cpu_bus.SIG_CPU_EXT_BUSp, new_bus.TEVY_ADDR_VRAMn()); }
  /*#p25.TEFA*/ wire TEFA_ADDR_VRAMp   () const { return nor2(new_bus.SYRO_FE00_FFFF(), TEXO_ADDR_VRAMn()); }
  /*#p25.SOSE*/ wire SOSE_ADDR_VRAMp   () const { return and2(TEFA_ADDR_VRAMp(), new_bus.BUS_CPU_A15p.qp_new()); }
  /* p08.LEVO*/ wire LEVO_ADDR_VRAMn   () const { return not1(TEXO_ADDR_VRAMn()); }
  /* p25.TUJA*/ wire TUJA_CPU_VRAM_WRp () const { return and2(SOSE_ADDR_VRAMp(), cpu_bus.APOV_CPU_WRp); }

  wire TOLE_CPU_VRAM_RDp() const
  {
   // Ignoring debug for now
    ///*#p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp =  and2(SOSE_ADDR_VRAMp(), ABUZ_EXT_RAM_CS_CLK);
    ///*#p25.TAVY*/ wire _TAVY_MOEp         = not1(vram_bus.PIN_45_VRAM_OEn.qn_new());
    ///*#p25.TEFY*/ wire _TEFY_VRAM_MCSp    = not1(vram_bus.PIN_43_VRAM_CSn.qn_new());
    ///*#p25.TOLE*/ wire _TOLE_CPU_VRAM_RDp = mux2p(_TEFY_VRAM_MCSp, _TUTO_DBG_VRAMp, _TUCA_CPU_VRAM_RDp);

    /*#p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp = nand2(SOSE_ADDR_VRAMp(), cpu_bus.ABUZ_EXT_RAM_CS_CLK);
    /*#p25.TOLE*/ wire _TOLE_CPU_VRAM_RDp = not1(_TUCA_CPU_VRAM_RDp);

    return _TOLE_CPU_VRAM_RDp;
  }

  wire SALE_CPU_VRAM_WRn() const
  {
    // Ignoring debug for now
    ///*#p25.TEGU*/ wire _TEGU_CPU_VRAM_WRn = nand2(SOSE_ADDR_VRAMp(), SIG_CPU_WRp.qp_new());  // Schematic wrong, second input is SIG_CPU_WRp
    ///*#p25.TAVY*/ wire _TAVY_MOEp         = not1(vram_bus.PIN_45_VRAM_OEn.qn_new());
    ///*#p25.TEFY*/ wire _TEFY_VRAM_MCSp    = not1(vram_bus.PIN_43_VRAM_CSn.qn_new());
    ///*#p25.SALE*/ wire _SALE_CPU_VRAM_WRn = mux2p(_TUTO_DBG_VRAMp, _TAVY_MOEp, _TEGU_CPU_VRAM_WRn);

    /*#p25.TEGU*/ wire _TEGU_CPU_VRAM_WRn = and2(SOSE_ADDR_VRAMp(), cpu_bus.SIG_CPU_WRp);  // Schematic wrong, second input is SIG_CPU_WRp
    /*#p25.SALE*/ wire _SALE_CPU_VRAM_WRn = not1(_TEGU_CPU_VRAM_WRn);

    return _SALE_CPU_VRAM_WRn;
  }

  //-----------------------------------------------------------------------------

  wire NUNY_WIN_MODE_TRIGp_old() const {
    /*#p27.NUNY*/ wire _NUNY_WIN_MODE_TRIGp_old = and2(win_reg.PYNU_WIN_MODE_Ap.qp_old(), win_reg.NOPA_WIN_MODE_Bp.qn_old());
    return _NUNY_WIN_MODE_TRIGp_old;
  }
  /* p27.NYFO*/ wire NYFO_WIN_MODE_TRIGn_old() const { return not1(NUNY_WIN_MODE_TRIGp_old()); }
  /* p27.MOSU*/ wire MOSU_WIN_MODE_TRIGp_old() const { return not1(NYFO_WIN_MODE_TRIGn_old()); }

  wire NUNY_WIN_MODE_TRIGp_new() const {
    /*#p27.NUNY*/ wire _NUNY_WIN_MODE_TRIGp_new = and2(win_reg.PYNU_WIN_MODE_Ap.qp_new(), win_reg.NOPA_WIN_MODE_Bp.qn_new());
    return _NUNY_WIN_MODE_TRIGp_new;
  }
  /* p27.NYFO*/ wire NYFO_WIN_MODE_TRIGn_new() const { return not1(NUNY_WIN_MODE_TRIGp_new()); }
  /* p27.MOSU*/ wire MOSU_WIN_MODE_TRIGp_new() const { return not1(NYFO_WIN_MODE_TRIGn_new()); }


  wire SUZU_WIN_FIRST_TILEne_old() const {
    /*#p27.SYLO*/ wire _SYLO_WIN_HITn_old = not1(win_reg.RYDY_WIN_HITp);
    /* p27.TUXY*/ wire _TUXY_WIN_FIRST_TILEne_old = nand2(_SYLO_WIN_HITn_old, win_reg.SOVY_WIN_HITp.qp_old());
    /* p27.SUZU*/ wire _SUZU_WIN_FIRST_TILEne_old = not1(_TUXY_WIN_FIRST_TILEne_old);
    return _SUZU_WIN_FIRST_TILEne_old;
  }

  wire SUZU_WIN_FIRST_TILEne_new() const {
    /*#p27.SYLO*/ wire _SYLO_WIN_HITn_new = not1(win_reg.RYDY_WIN_HITp);
    /* p27.TUXY*/ wire _TUXY_WIN_FIRST_TILEne_new = nand2(_SYLO_WIN_HITn_new, win_reg.SOVY_WIN_HITp.qp_new());
    /* p27.SUZU*/ wire _SUZU_WIN_FIRST_TILEne_new = not1(_TUXY_WIN_FIRST_TILEne_new);
    return _SUZU_WIN_FIRST_TILEne_new;
  }

  wire SEKO_WIN_FETCH_TRIGp_old() const {
    /* p27.SEKO*/ wire _SEKO_WIN_FETCH_TRIGp_old = nor2(win_reg.RYFA_WIN_FETCHn_A.qn_old(), win_reg.RENE_WIN_FETCHn_B.qp_old());
    return _SEKO_WIN_FETCH_TRIGp_old;
  }

  wire SEKO_WIN_FETCH_TRIGp_new() const {
    /* p27.SEKO*/ wire _SEKO_WIN_FETCH_TRIGp_new = nor2(win_reg.RYFA_WIN_FETCHn_A.qn_new(), win_reg.RENE_WIN_FETCHn_B.qp_new());
    return _SEKO_WIN_FETCH_TRIGp_new;
  }

  /*#p27.SYLO*/ wire SYLO_WIN_HITn_old() const { return not1(win_reg.RYDY_WIN_HITp); }
  /*#p24.TOMU*/ wire TOMU_WIN_HITp_old() const { return not1(SYLO_WIN_HITn_old()); }
  /* p27.TUKU*/ wire TUKU_WIN_HITn_old() const { return not1(TOMU_WIN_HITp_old()); }
  /*#p24.SOCY*/ wire SOCY_WIN_HITn_old() const { return not1(TOMU_WIN_HITp_old()); }

  /*#p27.SYLO*/ wire SYLO_WIN_HITn_new() const { return not1(win_reg.RYDY_WIN_HITp); }
  /*#p24.TOMU*/ wire TOMU_WIN_HITp_new() const { return not1(SYLO_WIN_HITn_new()); }
  /* p27.TUKU*/ wire TUKU_WIN_HITn_new() const { return not1(TOMU_WIN_HITp_new()); }
  /*#p24.SOCY*/ wire SOCY_WIN_HITn_new() const { return not1(TOMU_WIN_HITp_new()); }

  /*#p27.NOCU*/ wire NOCU_WIN_MODEn() const { return not1(win_reg.PYNU_WIN_MODE_Ap.qp_new()); }
  /* p27.PORE*/ wire PORE_WIN_MODEp() const { return not1(NOCU_WIN_MODEn()); }
  /* p26.AXAD*/ wire AXAD_WIN_MODEn() const { return not1(PORE_WIN_MODEp()); }

  //-----------------------------------------------------------------------------

  wire ROZE_FINE_COUNT_7n_old() {
    /*#p27.ROZE*/ wire _ROZE_FINE_COUNT_7n_old = nand3(fine_scroll.RUBU_FINE_CNT2.qp_old(), fine_scroll.ROGA_FINE_CNT1.qp_old(), fine_scroll.RYKU_FINE_CNT0.qp_old());
    return _ROZE_FINE_COUNT_7n_old;
  }

  wire POVA_FINE_MATCH_TRIGp() {
    /*#p27.POVA*/ wire _POVA_FINE_MATCH_TRIGp = and2(fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_new(), fine_scroll.NYZE_SCX_FINE_MATCH_B.qn_new());
    return _POVA_FINE_MATCH_TRIGp;
  }


  wire GESE_SCAN_MATCH_Yp(SpriteDeltaY& delta, DFF9 XYMO_LCDC_SPSIZEn) {
    /*#p29.GOVU*/ wire _GOVU_SPSIZE_MATCH_new = or2(XYMO_LCDC_SPSIZEn.qn_new(), delta.GYKY_YDIFF3.sum);
    /* p29.WOTA*/ wire _WOTA_SCAN_MATCH_Yn_new = nand6(delta.GACE_SPRITE_DELTA4, delta.GUVU_SPRITE_DELTA5, delta.GYDA_SPRITE_DELTA6, delta.GEWY_SPRITE_DELTA7, delta.WUHU_YDIFF7.carry, _GOVU_SPSIZE_MATCH_new);
    /* p29.GESE*/ wire _GESE_SCAN_MATCH_Yp_new = not1(_WOTA_SCAN_MATCH_Yn_new);
    return _GESE_SCAN_MATCH_Yp_new;
  }

  wire XANO_PX167p_old() const {
    /*#p21.XUGU*/ wire _XUGU_PX167n_old = nand5(pix_count.XEHO_PX0p.qp_old(), pix_count.SAVY_PX1p.qp_old(), pix_count.XODU_PX2p.qp_old(), pix_count.TUKY_PX5p.qp_old(), pix_count.SYBE_PX7p.qp_old()); // 128 + 32 + 4 + 2 + 1 = 167
    /*#p21.XANO*/ wire _XANO_PX167p_old = not1(_XUGU_PX167n_old);
    return _XANO_PX167p_old;
  }

  wire XANO_PX167p_new() const {
    /*#p21.XUGU*/ wire _XUGU_PX167n = nand5(pix_count.XEHO_PX0p.qp_new(), pix_count.SAVY_PX1p.qp_new(), pix_count.XODU_PX2p.qp_new(), pix_count.TUKY_PX5p.qp_new(), pix_count.SYBE_PX7p.qp_new()); // 128 + 32 + 4 + 2 + 1 = 167
    /*#p21.XANO*/ wire _XANO_PX167p = not1(_XUGU_PX167n);
    return _XANO_PX167p;
  }

  wire NERU_VSYNCp() const {
    /*#p24.NERU*/ wire _NERU_VSYNCp = nor8(lcd.reg_ly.LAFO_LY7p.qp_new(), lcd.reg_ly.LOVU_LY4p.qp_new(), lcd.reg_ly.LYDO_LY3p.qp_new(), lcd.reg_ly.MUWY_LY0p.qp_new(), lcd.reg_ly.MYRO_LY1p.qp_new(), lcd.reg_ly.LEXA_LY2p.qp_new(), lcd.reg_ly.LEMA_LY5p.qp_new(), lcd.reg_ly.MATO_LY6p.qp_new());
    return _NERU_VSYNCp;
  }

  /*#p21.XYVO*/ wire XYVO_y144p_old() const { return and2(lcd.reg_ly.LOVU_LY4p.qp_old(), lcd.reg_ly.LAFO_LY7p.qp_old()); } // 128 + 16 = 144
  /*#p21.XYVO*/ wire XYVO_y144p_new() const { return and2(lcd.reg_ly.LOVU_LY4p.qp_new(), lcd.reg_ly.LAFO_LY7p.qp_new()); } // 128 + 16 = 144

  wire PURE_LINE_ENDn() const {
    /*#p21.PURE*/ wire _PURE_LINE_ENDn = not1(lcd.reg_lx.RUTU_x113p.qp_new());
    return _PURE_LINE_ENDn;
  }

  //-----------------------------------------------------------------------------

  //wire NYPE_x113p_new() const { return NYPE_x113p.qp_new(); }
  //wire NYPE_x113n_new() const { return NYPE_x113p.qn_new(); }
  //
  //wire RUTU_x113p_old() const { return RUTU_x113p.qp_old(); }
  //wire RUTU_x113n_old() const { return RUTU_x113p.qn_old(); }
  //
  //wire RUTU_x113p_new() const { return RUTU_x113p.qp_new(); }
  //wire RUTU_x113n_new() const { return RUTU_x113p.qn_new(); }

  /*#p24.LOFU*/ wire LOFU_x113n_old() const { return not1(lcd.reg_lx.RUTU_x113p.qp_old()); }
  /*#p24.LOFU*/ wire LOFU_x113n_new() const { return not1(lcd.reg_lx.RUTU_x113p.qp_new()); }


  /* p29.SAKY*/ wire SAKY_SFETCHn_old() const { return nor2(sprite_fetcher.TULY_SFETCH_S1p.qp_old(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp_old()); }

  /* p29.SAKY*/ wire SAKY_SFETCHn() const { return nor2(sprite_fetcher.TULY_SFETCH_S1p.qp_new(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp_new()); }

  wire TEXY_SFETCHINGp_old() const {
    /* p29.TEPA*/ wire _TEPA_RENDERINGp = not1(XYMU_RENDERINGn.qn_old());
    /* p29.TYSO*/ wire _TYSO_SFETCHINGn = or2(SAKY_SFETCHn_old(), _TEPA_RENDERINGp); // def or
    /* p29.TEXY*/ wire _TEXY_SFETCHINGp = not1(_TYSO_SFETCHINGn);
    return _TEXY_SFETCHINGp;
  }

  wire TEXY_SFETCHINGp() const {
    /* p29.TEPA*/ wire _TEPA_RENDERINGp = not1(XYMU_RENDERINGn.qn_new());
    /* p29.TYSO*/ wire _TYSO_SFETCHINGn = or2(SAKY_SFETCHn(), _TEPA_RENDERINGp); // def or
    /* p29.TEXY*/ wire _TEXY_SFETCHINGp = not1(_TYSO_SFETCHINGn);
    return _TEXY_SFETCHINGp;
  }

  /* p27.SOWO*/ wire SOWO_SFETCH_RUNNINGn_old() const { return not1(sprite_fetcher.TAKA_SFETCH_RUNNINGp.qp_old()); }
  /* p27.SOWO*/ wire SOWO_SFETCH_RUNNINGn_new() const { return not1(sprite_fetcher.TAKA_SFETCH_RUNNINGp.qp_new()); }

  /* p29.TYTU*/ wire TYTU_SFETCH_S0n() const { return not1(sprite_fetcher.TOXE_SFETCH_S0p.qp_new()); }

  wire TUVO_PPU_OAM_RDp() const {
    /* p29.TEPA*/ wire _TEPA_RENDERINGp = not1(XYMU_RENDERINGn.qn_new());
    /* p29.TUVO*/ wire _TUVO_PPU_OAM_RDp = nor3(_TEPA_RENDERINGp, sprite_fetcher.TULY_SFETCH_S1p.qp_new(), sprite_fetcher.TESE_SFETCH_S2p.qp_new());
    return _TUVO_PPU_OAM_RDp;
  }

  wire TACU_SPR_SEQ_5_TRIG() const {
    /* p29.TACU*/ wire _TACU_SPR_SEQ_5_TRIG = nand2(sprite_fetcher.TYFO_SFETCH_S0p_D1.qp_new(), TYTU_SFETCH_S0n());
    return _TACU_SPR_SEQ_5_TRIG;
  }

  wire XUJY_OAM_CLKENp() const {
    /* p25.VAPE*/ wire _VAPE_OAM_CLKENn = and2(TUVO_PPU_OAM_RDp(), TACU_SPR_SEQ_5_TRIG());
    /* p25.XUJY*/ wire _XUJY_OAM_CLKENp = not1(_VAPE_OAM_CLKENn);
    return _XUJY_OAM_CLKENp;
  }


  wire XUJA_SPR_OAM_LATCHn() const {
    /* p28.WEFY*/ wire _WEFY_SPR_READp = and2(TUVO_PPU_OAM_RDp(), sprite_fetcher.TYFO_SFETCH_S0p_D1.qp_new());
    /*#p28.XUJA*/ wire _XUJA_SPR_OAM_LATCHn  = not1(_WEFY_SPR_READp);
    return _XUJA_SPR_OAM_LATCHn;
  }

  wire WUTY_SFETCH_DONE_TRIGp() const {
    /* p29.TYNO*/ wire _TYNO = nand3(sprite_fetcher.TOXE_SFETCH_S0p.qp_new(), sprite_fetcher.SEBA_SFETCH_S1p_D5.qp_new(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp_new());
    /* p29.VUSA*/ wire _VUSA_SPRITE_DONEn = or2(sprite_fetcher.TYFO_SFETCH_S0p_D1.qn_new(), _TYNO);
    /* p29.WUTY*/ wire _WUTY_SFETCH_DONE_TRIGp = not1(_VUSA_SPRITE_DONEn);
    return _WUTY_SFETCH_DONE_TRIGp;
  }

  wire SYCU_SFETCH_S0pe() const {
    /* p24.LOBY*/ wire _LOBY_RENDERINGn = not1(XYMU_RENDERINGn.qn_new());
    /* p29.SYCU*/ wire _SYCU_SFETCH_S0pe = nor3(TYTU_SFETCH_S0n(), _LOBY_RENDERINGn, sprite_fetcher.TYFO_SFETCH_S0p_D1.qp_new());
    return _SYCU_SFETCH_S0pe;
  }

  wire XADO_STORE_SPRITE_An() const {
    /*#p29.TOPU*/ wire _TOPU_STORE_SPRITE_Ap = and2(sprite_fetcher.TULY_SFETCH_S1p.qp_new(), SYCU_SFETCH_S0pe());
    /*#p29.VYWA*/ wire _VYWA_STORE_SPRITE_An = not1(_TOPU_STORE_SPRITE_Ap);
    /*#p29.WENY*/ wire _WENY_STORE_SPRITE_Ap = not1(_VYWA_STORE_SPRITE_An);
    /*#p29.XADO*/ wire _XADO_STORE_SPRITE_An = not1(_WENY_STORE_SPRITE_Ap);
    return _XADO_STORE_SPRITE_An;
  }

  wire PUCO_STORE_SPRITE_Bn() const {
    /*#p29.RACA*/ wire _RACA_STORE_SPRITE_Bp = and2(sprite_fetcher.VONU_SFETCH_S1p_D4.qp_new(), SYCU_SFETCH_S0pe());
    /*#p29.PEBY*/ wire _PEBY_STORE_SPRITE_Bn = not1(_RACA_STORE_SPRITE_Bp);
    /*#p29.NYBE*/ wire _NYBE_STORE_SPRITE_Bp = not1(_PEBY_STORE_SPRITE_Bn);
    /*#p29.PUCO*/ wire _PUCO_STORE_SPRITE_Bn = not1(_NYBE_STORE_SPRITE_Bp);
    return _PUCO_STORE_SPRITE_Bn;
  }

  /*#p29.XUQU*/ wire XUQU_SPRITE_AB() const { return not1(sprite_fetcher.VONU_SFETCH_S1p_D4.qn_new()); }

  wire SOHO_SPR_VRAM_RDp() const {
    /* p25.SOHO*/ wire _SOHO_SPR_VRAM_RDp = and2(TACU_SPR_SEQ_5_TRIG(), TEXY_SFETCHINGp());
    return _SOHO_SPR_VRAM_RDp;
  }

  //-----------------------------------------------------------------------------

  wire FETO_SCAN_DONEp_old() const {
    /*#p28.FETO*/ wire _FETO_SCAN_DONEp_old = and4(sprite_scanner.YFEL_SCAN0.qp_old(), sprite_scanner.WEWY_SCAN1.qp_old(), sprite_scanner.GOSO_SCAN2.qp_old(), sprite_scanner.FONY_SCAN5.qp_old()); // 32 + 4 + 2 + 1 = 39
    return _FETO_SCAN_DONEp_old;
  }

  wire FETO_SCAN_DONEp_new() const {
    /*#p28.FETO*/ wire _FETO_SCAN_DONEp_new = and4(sprite_scanner.YFEL_SCAN0.qp_new(), sprite_scanner.WEWY_SCAN1.qp_new(), sprite_scanner.GOSO_SCAN2.qp_new(), sprite_scanner.FONY_SCAN5.qp_new()); // 32 + 4 + 2 + 1 = 39
    return _FETO_SCAN_DONEp_new;
  }

  wire FETO_SCAN_DONEp_any() const {
    /*#p28.FETO*/ wire _FETO_SCAN_DONEp_any = and4(sprite_scanner.YFEL_SCAN0.qp_any(), sprite_scanner.WEWY_SCAN1.qp_any(), sprite_scanner.GOSO_SCAN2.qp_any(), sprite_scanner.FONY_SCAN5.qp_any()); // 32 + 4 + 2 + 1 = 39
    return _FETO_SCAN_DONEp_any;
  }

  wire AVAP_SCAN_DONE_TRIGp_old() const {
    /*#p29.BEBU*/ wire _BEBU_SCAN_DONE_TRIGn_old = or3(sprite_scanner.DOBA_SCAN_DONE_Bp.qp_old(), BALU_LINE_RSTp_old(), sprite_scanner.BYBA_SCAN_DONE_Ap.qn_old());
    /*#p29.AVAP*/ wire _AVAP_SCAN_DONE_TRIGp_old = not1(_BEBU_SCAN_DONE_TRIGn_old);
    return _AVAP_SCAN_DONE_TRIGp_old;
  }

  wire AVAP_SCAN_DONE_TRIGp_new() const {
    /*#p29.BEBU*/ wire _BEBU_SCAN_DONE_TRIGn_new = or3(sprite_scanner.DOBA_SCAN_DONE_Bp.qp_new(), BALU_LINE_RSTp_new(), sprite_scanner.BYBA_SCAN_DONE_Ap.qn_new());
    /*#p29.AVAP*/ wire _AVAP_SCAN_DONE_TRIGp_new = not1(_BEBU_SCAN_DONE_TRIGn_new);
    return _AVAP_SCAN_DONE_TRIGp_new;
  }

  /* p29.CEHA*/ wire CEHA_SCANNINGp() const { return not1(sprite_scanner.CENO_SCANNINGn.qn_new()); }
  /*#p29.BYJO*/ wire BYJO_SCANNINGn() const { return not1(CEHA_SCANNINGp()); }


  wire FEPO_STORE_MATCHp(SpriteMatchFlag& flag) const {
    /* p29.FEFY*/ wire _FEFY_STORE_MATCHp = nand5(flag.XAGE_STORE4_MATCHn, flag.YLOZ_STORE3_MATCHn, flag.DEGO_STORE2_MATCHn, flag.DYDU_STORE1_MATCHn, flag.YDUG_STORE0_MATCHn);
    /* p29.FOVE*/ wire _FOVE_STORE_MATCHp = nand5(flag.YGEM_STORE9_MATCHn, flag.EFYL_STORE8_MATCHn, flag.DYKA_STORE7_MATCHn, flag.YBEZ_STORE6_MATCHn, flag.EGOM_STORE5_MATCHn);
    /* p29.FEPO*/ wire _FEPO_STORE_MATCHp = or2(_FEFY_STORE_MATCHp, _FOVE_STORE_MATCHp);
    return _FEPO_STORE_MATCHp;
  }

  //-----------------------------------------------------------------------------

  /*#p21.XENA*/ wire XENA_STORE_MATCHn(SpriteMatchFlag& flag) const { return not1(FEPO_STORE_MATCHp(flag)); }

  /* p27.ROMO*/ wire ROMO_PRELOAD_DONEn() const { return not1(tile_fetcher.POKY_PRELOAD_LATCHp.qp_new()); }

  wire TAVE_PRELOAD_DONE_TRIGp_old() const {
    /* p27.ROMO*/ wire _ROMO_PRELOAD_DONEn_old      = not1(tile_fetcher.POKY_PRELOAD_LATCHp.qp_old());
    /* p27.SUVU*/ wire _SUVU_PRELOAD_DONE_TRIGn_old = nand4(XYMU_RENDERINGn.qn_old(), _ROMO_PRELOAD_DONEn_old, tile_fetcher.NYKA_FETCH_DONEp.qp_old(), tile_fetcher.PORY_FETCH_DONEp.qp_old());
    /* p27.TAVE*/ wire _TAVE_PRELOAD_DONE_TRIGp_old = not1(_SUVU_PRELOAD_DONE_TRIGn_old);
    return _TAVE_PRELOAD_DONE_TRIGp_old;
  }

  wire TAVE_PRELOAD_DONE_TRIGp_new() const {
    /* p27.ROMO*/ wire _ROMO_PRELOAD_DONEn_new      = not1(tile_fetcher.POKY_PRELOAD_LATCHp.qp_new());
    /* p27.SUVU*/ wire _SUVU_PRELOAD_DONE_TRIGn_new = nand4(XYMU_RENDERINGn.qn_new(), _ROMO_PRELOAD_DONEn_new, tile_fetcher.NYKA_FETCH_DONEp.qp_new(), tile_fetcher.PORY_FETCH_DONEp.qp_new());
    /* p27.TAVE*/ wire _TAVE_PRELOAD_DONE_TRIGp_new = not1(_SUVU_PRELOAD_DONE_TRIGn_new);
    return _TAVE_PRELOAD_DONE_TRIGp_new;
  }

  ///* p27.MOCE*/ wire MOCE_BFETCH_DONEn_old(wire NYXU_BFETCH_RSTn_old) const { return nand3(_LAXU_BFETCH_S0p.qp_old(), _NYVA_BFETCH_S2p.qp_old(), NYXU_BFETCH_RSTn_old); }
  ///* p27.LYRY*/ wire LYRY_BFETCH_DONEp_old(wire NYXU_BFETCH_RSTn_old) const { return not1(MOCE_BFETCH_DONEn_old(NYXU_BFETCH_RSTn_old)); }

  ///* p27.MOCE*/ wire MOCE_BFETCH_DONEn_new(wire NYXU_BFETCH_RSTn_new) const { return nand3(_LAXU_BFETCH_S0p.qp_new(), _NYVA_BFETCH_S2p.qp_new(), NYXU_BFETCH_RSTn_new); }
  ///* p27.LYRY*/ wire LYRY_BFETCH_DONEp_new(wire NYXU_BFETCH_RSTn_new) const { return not1(MOCE_BFETCH_DONEn_new(NYXU_BFETCH_RSTn_new)); }

  ///* p27.MOCE*/ wire MOCE_BFETCH_DONEn_any(wire NYXU_BFETCH_RSTn_new) const { return nand3(_LAXU_BFETCH_S0p.qp_any(), _NYVA_BFETCH_S2p.qp_any(), NYXU_BFETCH_RSTn_new); }
  ///* p27.LYRY*/ wire LYRY_BFETCH_DONEp_any(wire NYXU_BFETCH_RSTn_new) const { return not1(MOCE_BFETCH_DONEn_any(NYXU_BFETCH_RSTn_new)); }

  /* p27.LUSU*/ wire LUSU_FETCHINGn()      const { return not1(tile_fetcher.LONY_FETCHINGp.qp_new()); }
  /* p27.LENA*/ wire LENA_BFETCHINGp()     const { return not1(LUSU_FETCHINGn()); }
  /*#p27.NAKO*/ wire NAKO_BFETCH_S1n()     const { return not1(tile_fetcher._MESU_BFETCH_S1p.qp_new()); }
  /*#p27.NOFU*/ wire NOFU_BFETCH_S2n()     const { return not1(tile_fetcher._NYVA_BFETCH_S2p.qp_new()); }
  /* p27.NOGU*/ wire NOGU_BFETCH_01p()     const { return nand2(NAKO_BFETCH_S1n(), NOFU_BFETCH_S2n()); }
  /* p27.NENY*/ wire NENY_BFETCH_01n()     const { return not1(NOGU_BFETCH_01p()); }
  /* p27.POTU*/ wire POTU_BGW_MAP_READp()  const { return and2(LENA_BFETCHINGp(), NENY_BFETCH_01n()); }
  /* p27.NETA*/ wire NETA_BGW_TILE_READp() const { return and2(LENA_BFETCHINGp(), NOGU_BFETCH_01p()); }
  /*#p27.XUHA*/ wire XUHA_FETCH_HILOp()    const { return not1(NOFU_BFETCH_S2n()); }


  //-----------------------------------------------------------------------------
  // All the SOC registers, pins, buses. Everything in this section should derive
  // from BitBase.

  uint64_t sentinel1 = SENTINEL1;

  //----------

  GateBoyPins pins;
  GateBoyBuses old_bus;
  GateBoyBuses new_bus;

  /*p21.VOGA*/ DFF17 VOGA_HBLANKp;                   // ABxDxFxH Clocked on odd, reset on A
  /*p21.XYMU*/ NorLatch XYMU_RENDERINGn;             // ABxDxFxH Cleared on A, set on BDFH

  GateBoyCpuBus  cpu_bus;
  GateBoyExtBus  ext_bus;
  GateBoyVramBus vram_bus;
  GateBoyOamBus  oam_bus;
  GateBoyZramBus zram_bus;

  GateBoyResetDebug rst;
  GateBoyClock      clk;
  GateBoyDiv        div;
  GateBoyTimer      timer;
  GateBoyDMA        dma;
  GateBoyInterrupts interrupts;
  GateBoyJoypad     joy;
  GateBoySerial     serial;

  GateBoySpriteStore   sprite_store;
  SpriteScanner sprite_scanner;
  SpriteFetcher sprite_fetcher;
  TileFetcher   tile_fetcher;

  RegLCDC reg_lcdc;
  RegStat reg_stat;
  RegSCX  reg_scx;
  RegSCY  reg_scy;
  RegWY   reg_wy;
  RegWX   reg_wx;

  WinMapX         win_map_x;
  WinLineY        win_map_y;
  WindowRegisters win_reg;
  FineScroll      fine_scroll;

  PixCount     pix_count;
  PixelPipes   pix_pipes;
  GateBoyLCD   lcd;

  //----------

  uint64_t sentinel2 = SENTINEL2;


  //-----------------------------------------------------------------------------
  // Control stuff

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

  MetroBoyCPU      cpu;

  Req      bus_req_old = {0};
  Req      bus_req_new = {0};
  uint8_t  cpu_data_latch = 0;
  uint8_t  int_ack_latch = 0;
  uint8_t  intf_latch = 0;
  uint8_t  intf_latch_delay = 0;
  uint8_t  intf_halt_latch = 0;

  //-----------------------------------------------------------------------------
  // Memory

  uint8_t vid_ram [8192];
  uint8_t cart_ram[8192];
  uint8_t int_ram [8192];
  uint8_t oam_ram [256];
  uint8_t zero_ram[128];

  uint64_t sentinel3 = SENTINEL3;

  //-----------------------------------------------------------------------------
  // LCD and framebuffer

  int old_lcd_x = 0;
  int old_lcd_y = 0;

  uint8_t framebuffer[160*144];
  //uint8_t gb_screen_x = 0;
  //uint8_t gb_screen_y = 0;

  //-----------------------------------------------------------------------------

  double   sim_time = 0;
  uint64_t phase_total = 0;
  uint64_t phase_origin = 0;

  uint64_t phase_hash = 0;
  uint64_t cumulative_hash = 0;

  Probes   probes;

  uint8_t* boot_buf = nullptr;
  size_t   boot_size = 0;

  uint8_t* cart_buf = nullptr;
  size_t   cart_size = 0;

  uint64_t sentinel4 = SENTINEL4;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------
