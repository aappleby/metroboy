#pragma once

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
#include "GateBoyLib/GateBoyWaveBus.h"
#include "GateBoyLib/GateBoyZramBus.h"
#include "GateBoyLib/GateBoyReset.h"
#include "GateBoyLib/GateBoyCpuBus.h"
#include "GateBoyLib/GateBoySPU.h"
#include "GateBoyLib/GateBoyPins.h"
#include "GateBoyLib/GateBoyRegisters.h"

//-----------------------------------------------------------------------------
// All the SOC registers, pins, buses. Everything in GateBoyState _must_
// derive from BitBase.

#pragma pack(push, 1)
struct GateBoyState {

  //----------------------------------------

  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  int64_t hash_regression();
  int64_t hash_all();

  GBResult peek(int addr) const;
  GBResult poke(int addr, uint8_t data_in);

  void commit();
  void check_state_old_and_driven_or_pulled();
  bool diff(const GateBoyState& gbb, uint8_t mask) const;

  static FieldInfo fields[];

  /*#p08.TEXO*/ wire TEXO_ADDR_VRAMn_new   () const;
  /*#p25.TEFA*/ wire TEFA_ADDR_VRAMp_new   () const;
  /*#p25.SOSE*/ wire SOSE_ADDR_VRAMp_new   () const;
  /*_p08.LEVO*/ wire LEVO_ADDR_VRAMn_new   () const;
  /*_p25.TUJA*/ wire TUJA_CPU_VRAM_WRp_new () const;

  /*_p01.XODO*/ wire XODO_VID_RSTp_new() const;
  /*_p01.XAPO*/ wire XAPO_VID_RSTn_new() const;
  /*_p01.LYHA*/ wire LYHA_VID_RSTp_new() const;
  /*_p01.LYFE*/ wire LYFE_VID_RSTn_new() const;
  /*_p01.TOFU*/ wire TOFU_VID_RSTp_new() const;
  /*_p01.ROSY*/ wire ROSY_VID_RSTp_new() const;
  /*#p01.ATAR*/ wire ATAR_VID_RSTp_new() const;
  /*#p01.ABEZ*/ wire ABEZ_VID_RSTn_new() const;
  /*_p01.PYRY*/ wire PYRY_VID_RSTp_new() const;
  /*_p01.AMYG*/ wire AMYG_VID_RSTp_new() const;

  /*#p25.TOLE*/ wire TOLE_CPU_VRAM_RDp_new() const;
  /*#p25.SALE*/ wire SALE_CPU_VRAM_WRn_new() const;

  /*_p09.HAPO*/ wire HAPO_SYS_RSTp() const { return not1(sys_rst.ALUR_SYS_RSTn_new()); }
  /*_p09.GUFO*/ wire GUFO_SYS_RSTn() const { return not1(HAPO_SYS_RSTp()); }

  /*_p09.JYRO*/ wire JYRO_APU_RSTp() const { return  or2(HAPO_SYS_RSTp(), spu.HADA_NR52_ALL_SOUND_ON.qn_new()); }
  /*_p09.KEPY*/ wire KEPY_APU_RSTn() const { return not1(JYRO_APU_RSTp()); }
  /*_p09.KUBY*/ wire KUBY_APU_RSTn() const { return not1(JYRO_APU_RSTp()); }
  /*_p09.KEBA*/ wire KEBA_APU_RSTp() const { return not1(KUBY_APU_RSTn()); }

  /*_p01.ATUS*/ wire ATUS_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p01.BELA*/ wire BELA_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p01.BOPO*/ wire BOPO_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p09.ATYV*/ wire ATYV_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p09.KAME*/ wire KAME_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p14.HUDE*/ wire HUDE_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p14.JYBU*/ wire JYBU_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p15.BUWE*/ wire BUWE_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p15.CEXE*/ wire CEXE_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p15.CYWU*/ wire CYWU_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p09.AFAT*/ wire AFAT_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p09.AGUR*/ wire AGUR_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p11.CAMY*/ wire CAMY_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p11.CEPO*/ wire CEPO_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p19.CABE*/ wire CABE_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p14.KYPU*/ wire KYPU_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p14.FAZO*/ wire FAZO_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p16.GOVE*/ wire GOVE_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p11.HATO*/ wire HATO_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p16.KOPY*/ wire KOPY_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p16.HEKY*/ wire HEKY_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p16.GAZE*/ wire GAZE_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p18.CALU*/ wire CALU_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p17.BAMA*/ wire BAMA_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p17.ACOR*/ wire ACOR_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p09.DAPA*/ wire DAPA_APU_RSTn() const { return not1(KEBA_APU_RSTp()); } // bavu crosses over this? why is it 3-rung?
  /*#p19.FEXO*/ wire FEXO_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p19.HYNE*/ wire HYNE_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p20.GASO*/ wire GASO_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p20.FEBY*/ wire FEBY_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p20.BOKY*/ wire BOKY_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p13.DUKA*/ wire DUKA_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p13.ERUM*/ wire ERUM_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p13.KADO*/ wire KADO_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p15.KATY*/ wire KATY_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p16.GURO*/ wire GURO_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p16.KUHA*/ wire KUHA_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p16.GOMA*/ wire GOMA_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }


  /*_p16.ANUJ*/ wire ANUJ_CPU_WR_WEIRD()  const { return and2(cpu_signals.SIG_IN_CPU_DBUS_FREE.qp_new(), cpu_signals.BOGY_CPU_WRp()); }
  /*_p09.EDEK*/ wire EDEK_NR52_DBG_APUp() const { return not1(spu.FERO_NR52_DBG_APUn.qn_new()); }
  /*_p14.FAPE*/ wire FAPE_CPU_RDp_DBGp()  const { return and2(cpu_signals.FOGE_CPU_RDp(), EDEK_NR52_DBG_APUp()); }
  /*_p16.EGAD*/ wire EGAD_CPU_RDn_DBGn()  const { return nand2(cpu_signals.DOVO_CPU_RDp(), EDEK_NR52_DBG_APUp()); }
  /*#p11.DAXA*/ wire DAXA_CPU_RDn_DBGn()  const { return nand2(cpu_signals.CEGE_CPU_RDp(), EDEK_NR52_DBG_APUp()); }

  /*_p10.TACE*/ wire TACE_AMP_ENn() const {
    return and4(ch1.HOCA_CH1_AMP_ENn(),
                ch2.FUTE_CH2_AMP_ENn(),
                ch3.GUXE_CH3_AMP_ENn.qp_new(),
                ch4.GEVY_CH4_AMP_ENn());
  }

  //----------------------------------------

  /*_SIG_VCC*/ SigIn SIG_VCC;
  /*_SIG_GND*/ SigIn SIG_GND;
  
  RegJoy  reg_joy;  // 0xFF00
  //RegSB   reg_sb;   // 0xFF01
  //RegSC   reg_sc;   // 0xFF02
  RegDIV  reg_div;  // 0xFF04
  RegTIMA reg_tima; // 0xFF05
  RegTMA  reg_tma;  // 0xFF06
  RegTAC  reg_tac;  // 0xFF07
  RegIF   reg_if;   // 0xFF0F

  RegLCDC reg_lcdc; // 0xFF40
  RegStat reg_stat; // 0xFF41
  RegSCY  reg_scy;  // 0xFF42
  RegSCX  reg_scx;  // 0xFF43
  RegLY   reg_ly;   // 0xFF44
  RegLYC  reg_lyc;  // 0xFF45
  RegDmaHi  reg_dma;  // 0xFF46
  RegBGP  reg_bgp;  // 0xFF47
  RegOBP0 reg_obp0; // 0xFF48
  RegOBP1 reg_obp1; // 0xFF49
  RegWY   reg_wy;   // 0xFF4A
  RegWX   reg_wx;   // 0xFF4B
  RegLX   reg_lx;   // Not mapped
  RegIE   reg_ie;   // 0xFFFF

  GateBoyReset sys_rst;
  GateBoyClock sys_clk;

  /*#p21.VOGA*/ DFF17    VOGA_HBLANKp_evn;
  /*#p21.XYMU*/ NorLatch XYMU_RENDERING_LATCHn;
  /*#p04.MATU*/ DFF17    MATU_DMA_RUNNINGp_odd;
  /*#p28.ACYL*/ Gate     ACYL_SCANNINGp_odd;
  /*#p21.WODU*/ Gate     WODU_HBLANK_GATEp_odd;
  /*_p28.ATEJ*/ Gate     ATEJ_LINE_RST_TRIGp_odd;
  /*_p29.FEPO*/ Gate     FEPO_STORE_MATCHp_odd;

  GateBoyCpuSignals cpu_signals;
  GateBoyCpuABus    cpu_abus;
  GateBoyCpuDBus    cpu_dbus;

  InterruptControl int_ctrl;
  InterruptLatch   int_latch;

  struct { VramABusLo lo; VramABusHi hi; } vram_abus;
  VramDBus       vram_dbus;
                
  OamControl     oam_ctrl;
  OamABus        oam_abus;
  OamDBusA       oam_dbus_a;
  OamDBusB       oam_dbus_b;
  OamLatchA      oam_latch_a;
  OamLatchB      oam_latch_b;
  OamTempA       oam_temp_a;
  OamTempB       oam_temp_b;

  WaveBus        wave_bus;

  ExtDataLatch   ext_data_latch;
  ExtAddrLatch   ext_addr_latch;

  GateBoyMBC     ext_mbc;

  GateBoyZram    zram_bus;


  DmaControl dma_ctrl;
  RegDmaLo   dma_lo;
  

  CpuInt  cpu_int;
  CpuAck  cpu_ack;

  JoyInt   joy_int;
  JoyLatch joy_latch;

  //----------

  SpriteScanner    sprite_scanner;
  ScanCounter      scan_counter;
  SpriteCounter    sprite_counter;
  SpriteIndex      sprite_index;
  SpriteMatchFlags sprite_match_flags_odd;
  SpriteResetFlags sprite_reset_flags_odd;
  SpriteStoreFlags sprite_store_flags_evn;

  SpriteIBus sprite_ibus;
  SpriteLBus sprite_lbus;

  StoreI0 store_i0;
  StoreI1 store_i1;
  StoreI2 store_i2;
  StoreI3 store_i3;
  StoreI4 store_i4;
  StoreI5 store_i5;
  StoreI6 store_i6;
  StoreI7 store_i7;
  StoreI8 store_i8;
  StoreI9 store_i9;

  StoreL0 store_l0;
  StoreL1 store_l1;
  StoreL2 store_l2;
  StoreL3 store_l3;
  StoreL4 store_l4;
  StoreL5 store_l5;
  StoreL6 store_l6;
  StoreL7 store_l7;
  StoreL8 store_l8;
  StoreL9 store_l9;

  StoreX0 store_x0;
  StoreX1 store_x1;
  StoreX2 store_x2;
  StoreX3 store_x3;
  StoreX4 store_x4;
  StoreX5 store_x5;
  StoreX6 store_x6;
  StoreX7 store_x7;
  StoreX8 store_x8;
  StoreX9 store_x9;

  //----------


  SpriteFetchCounter sfetch_counter_evn;
  SpriteFetchControl sfetch_control;

  //----------

  TileFetchCounter tfetch_counter;
  TileFetchControl tfetch_control;
  TileTempA tile_temp_a;
  TileTempB tile_temp_b;

  //----------

  WinControl win_ctrl;

  struct {
    WinMapX    map;
  } win_x;

  struct {
    WinTileY   tile;
    WinMapY    map;
  } win_y;

  FineCount  fine_count_odd;
  FineScroll fine_scroll;

  //----------

  SpritePix flipped_sprite;
  SpritePixA sprite_pix_a;
  SpritePixB sprite_pix_b;

  PixCount pix_count;   // odd
  MaskPipe mask_pipe;
  BgwPipeA bgw_pipe_a;
  BgwPipeB bgw_pipe_b;
  SprPipeA spr_pipe_a;
  SprPipeB spr_pipe_b;
  PalPipe  pal_pipe;

  LCDControl lcd;

  GateBoySPU  spu;
  SpuChannel1 ch1;
  SpuChannel2 ch2;
  SpuChannel3 ch3;
  SpuChannel4 ch4;
};
#pragma pack(pop)
