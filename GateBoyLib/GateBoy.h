#pragma once

#include "CoreLib/File.h"
#include "CoreLib/CPU.h"
#include "CoreLib/Constants.h"

#include "GateBoyLib/Probe.h"
#include "GateBoyLib/Sch_Timer.h"
#include "GateBoyLib/RegLCD.h"
#include "GateBoyLib/Sch_DMA.h"
#include "GateBoyLib/RegSpriteStore.h"
#include "GateBoyLib/Sch_Clocks.h"
#include "GateBoyLib/Sch_PixPipe.h"
#include "GateBoyLib/Sch_Joypad.h"
#include "GateBoyLib/Sch_Serial.h"
#include "GateBoyLib/Sch_Interrupts.h"
#include "GateBoyLib/Sch_SpriteFetcher.h"
#include "GateBoyLib/Sch_TileFetcher.h"
#include "GateBoyLib/Sch_SpriteScanner.h"
#include "GateBoyLib/Sch_ExtBus.h"
#include "GateBoyLib/Sch_OamBus.h"
#include "GateBoyLib/Sch_VramBus.h"

//-----------------------------------------------------------------------------

#if 0

/* p22.WADO*/ wire WADO_A00p_ext = not1(XOLA_A00n_ext);
/* p22.WESA*/ wire WESA_A01p_ext = not1(XENO_A01n_ext);
/* p22.WALO*/ wire WALO_A02p_ext = not1(XUSY_A02n_ext);
/* p22.WEPO*/ wire WEPO_A03p_ext = not1(XERA_A03n_ext);

/* p07.SYKE*/ wire SYKE_ADDR_HIp_ext = nor2(TUNA_0000_FDFF_ext, TONA_A08n_ext);
/* p07.SOHA*/ wire SOHA_ADDR_HIn_ext = not1(SYKE_ADDR_HIp_ext);

/* p07.ROPE*/ wire ROPE_ADDR_OAMn_ext  = nand2(SOHA_ADDR_HIn_ext, RYCU_FE00_FFFF_ext);
/* p07.SARO*/ wire SARO_ADDR_OAMp_ext = not1(ROPE_ADDR_OAMn_ext);

/* p22.XALY*/ wire XALY_0x00xxxx_ext = nor3(BUS_CPU_A[ 7], BUS_CPU_A[ 5], BUS_CPU_A[ 4]);
/* p22.WUTU*/ wire WUTU_ADDR_PPUn_ext  = nand3(SYKE_ADDR_HIp_ext, BUS_CPU_A[ 6], XALY_0x00xxxx_ext);
/* p22.WERO*/ wire WERO_ADDR_PPUp_ext  = not1(WUTU_ADDR_PPUn_ext);
#endif

//-----------------------------------------------------------------------------

struct SpriteDeltaY {

  wire GESE_SCAN_MATCH_Yp(wire XYMO_LCDC_SPSIZEn) {
    /*#p29.GOVU*/ wire _GOVU_SPSIZE_MATCH_new = or2(XYMO_LCDC_SPSIZEn, GYKY_YDIFF_S3);
    /* p29.WOTA*/ wire _WOTA_SCAN_MATCH_Yn_new = nand6(GACE_SPRITE_DELTA4, GUVU_SPRITE_DELTA5, GYDA_SPRITE_DELTA6, GEWY_SPRITE_DELTA7, WUHU_YDIFF_C7, _GOVU_SPSIZE_MATCH_new);
    /* p29.GESE*/ wire _GESE_SCAN_MATCH_Yp_new = not1(_WOTA_SCAN_MATCH_Yn_new);
    return _GESE_SCAN_MATCH_Yp_new;
  }

  /* p29.DEGE*/ wire DEGE_SPRITE_DELTA0;
  /* p29.DABY*/ wire DABY_SPRITE_DELTA1;
  /* p29.DABU*/ wire DABU_SPRITE_DELTA2;
  /* p29.GYSA*/ wire GYSA_SPRITE_DELTA3;
  /* p29.GACE*/ wire GACE_SPRITE_DELTA4;
  /* p29.GUVU*/ wire GUVU_SPRITE_DELTA5;
  /* p29.GYDA*/ wire GYDA_SPRITE_DELTA6;
  /* p29.GEWY*/ wire GEWY_SPRITE_DELTA7;

  /* p29.GYKY*/ wire GYKY_YDIFF_S3;
  /* p29.WUHU*/ wire WUHU_YDIFF_C7;
};

inline SpriteDeltaY sprite_delta_y(const OamTempA& oam_temp_a, const RegLY& reg_ly) {
  wire GND = 0;

  /*#p29.EBOS*/ wire EBOS_LY0n_new_evn = not1(reg_ly.MUWY_LY0p_evn.qp_new());
  /* p29.DASA*/ wire DASA_LY1n_new_evn = not1(reg_ly.MYRO_LY1p_evn.qp_new());
  /* p29.FUKY*/ wire FUKY_LY2n_new_evn = not1(reg_ly.LEXA_LY2p_evn.qp_new());
  /* p29.FUVE*/ wire FUVE_LY3n_new_evn = not1(reg_ly.LYDO_LY3p_evn.qp_new());
  /* p29.FEPU*/ wire FEPU_LY4n_new_evn = not1(reg_ly.LOVU_LY4p_evn.qp_new());
  /* p29.FOFA*/ wire FOFA_LY5n_new_evn = not1(reg_ly.LEMA_LY5p_evn.qp_new());
  /* p29.FEMO*/ wire FEMO_LY6n_new_evn = not1(reg_ly.MATO_LY6p_evn.qp_new());
  /* p29.GUSU*/ wire GUSU_LY7n_new_evn = not1(reg_ly.LAFO_LY7p_evn.qp_new());

  /* p29.ERUC*/ wire _ERUC_YDIFF_S0_new = add_s(EBOS_LY0n_new_evn, oam_temp_a.XUSO_OAM_DA0p_evn.qp_new(), GND);
  /* p29.ERUC*/ wire _ERUC_YDIFF_C0_new = add_c(EBOS_LY0n_new_evn, oam_temp_a.XUSO_OAM_DA0p_evn.qp_new(), GND);
  /* p29.ENEF*/ wire _ENEF_YDIFF_S1_new = add_s(DASA_LY1n_new_evn, oam_temp_a.XEGU_OAM_DA1p_evn.qp_new(), _ERUC_YDIFF_C0_new);
  /* p29.ENEF*/ wire _ENEF_YDIFF_C1_new = add_c(DASA_LY1n_new_evn, oam_temp_a.XEGU_OAM_DA1p_evn.qp_new(), _ERUC_YDIFF_C0_new);
  /* p29.FECO*/ wire _FECO_YDIFF_S2_new = add_s(FUKY_LY2n_new_evn, oam_temp_a.YJEX_OAM_DA2p_evn.qp_new(), _ENEF_YDIFF_C1_new);
  /* p29.FECO*/ wire _FECO_YDIFF_C2_new = add_c(FUKY_LY2n_new_evn, oam_temp_a.YJEX_OAM_DA2p_evn.qp_new(), _ENEF_YDIFF_C1_new);
  /* p29.GYKY*/ wire _GYKY_YDIFF_S3_new = add_s(FUVE_LY3n_new_evn, oam_temp_a.XYJU_OAM_DA3p_evn.qp_new(), _FECO_YDIFF_C2_new);
  /* p29.GYKY*/ wire _GYKY_YDIFF_C3_new = add_c(FUVE_LY3n_new_evn, oam_temp_a.XYJU_OAM_DA3p_evn.qp_new(), _FECO_YDIFF_C2_new);
  /* p29.GOPU*/ wire _GOPU_YDIFF_S4_new = add_s(FEPU_LY4n_new_evn, oam_temp_a.YBOG_OAM_DA4p_evn.qp_new(), _GYKY_YDIFF_C3_new);
  /* p29.GOPU*/ wire _GOPU_YDIFF_C4_new = add_c(FEPU_LY4n_new_evn, oam_temp_a.YBOG_OAM_DA4p_evn.qp_new(), _GYKY_YDIFF_C3_new);
  /* p29.FUWA*/ wire _FUWA_YDIFF_S5_new = add_s(FOFA_LY5n_new_evn, oam_temp_a.WYSO_OAM_DA5p_evn.qp_new(), _GOPU_YDIFF_C4_new);
  /* p29.FUWA*/ wire _FUWA_YDIFF_C5_new = add_c(FOFA_LY5n_new_evn, oam_temp_a.WYSO_OAM_DA5p_evn.qp_new(), _GOPU_YDIFF_C4_new);
  /* p29.GOJU*/ wire _GOJU_YDIFF_S6_new = add_s(FEMO_LY6n_new_evn, oam_temp_a.XOTE_OAM_DA6p_evn.qp_new(), _FUWA_YDIFF_C5_new);
  /* p29.GOJU*/ wire _GOJU_YDIFF_C6_new = add_c(FEMO_LY6n_new_evn, oam_temp_a.XOTE_OAM_DA6p_evn.qp_new(), _FUWA_YDIFF_C5_new);
  /* p29.WUHU*/ wire _WUHU_YDIFF_S7_new = add_s(GUSU_LY7n_new_evn, oam_temp_a.YZAB_OAM_DA7p_evn.qp_new(), _GOJU_YDIFF_C6_new);
  /* p29.WUHU*/ wire _WUHU_YDIFF_C7_new = add_c(GUSU_LY7n_new_evn, oam_temp_a.YZAB_OAM_DA7p_evn.qp_new(), _GOJU_YDIFF_C6_new);

  /* p29.DEGE*/ wire _DEGE_SPRITE_DELTA0_new = not1(_ERUC_YDIFF_S0_new);
  /* p29.DABY*/ wire _DABY_SPRITE_DELTA1_new = not1(_ENEF_YDIFF_S1_new);
  /* p29.DABU*/ wire _DABU_SPRITE_DELTA2_new = not1(_FECO_YDIFF_S2_new);
  /* p29.GYSA*/ wire _GYSA_SPRITE_DELTA3_new = not1(_GYKY_YDIFF_S3_new);
  /* p29.GACE*/ wire _GACE_SPRITE_DELTA4_new = not1(_GOPU_YDIFF_S4_new);
  /* p29.GUVU*/ wire _GUVU_SPRITE_DELTA5_new = not1(_FUWA_YDIFF_S5_new);
  /* p29.GYDA*/ wire _GYDA_SPRITE_DELTA6_new = not1(_GOJU_YDIFF_S6_new);
  /* p29.GEWY*/ wire _GEWY_SPRITE_DELTA7_new = not1(_WUHU_YDIFF_S7_new);

  return {
    _DEGE_SPRITE_DELTA0_new,
    _DABY_SPRITE_DELTA1_new,
    _DABU_SPRITE_DELTA2_new,
    _GYSA_SPRITE_DELTA3_new,
    _GACE_SPRITE_DELTA4_new,
    _GUVU_SPRITE_DELTA5_new,
    _GYDA_SPRITE_DELTA6_new,
    _GEWY_SPRITE_DELTA7_new,
    _GYKY_YDIFF_S3_new,
    _WUHU_YDIFF_C7_new
  };
}

//-----------------------------------------------------------------------------

struct Bootrom {
  void reset_cart() {
    BOOT_BITn_h.reset(REG_D1C1);
  }

  void tock(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],
    wire UMUT_MODE_DBG1p_ext,
    wire AVOR_SYS_RSTp,
    wire TEDO_CPU_RDp,
    wire TAPU_CPU_WRp,
    uint8_t* boot_buf,
    BusOut BUS_CPU_D_out[8])
  {
    {
      /* p07.TYRO*/ wire _TYRO_XX_0x0x0000p_ext = nor6(BUS_CPU_A[ 7], BUS_CPU_A[ 5], BUS_CPU_A[ 3], BUS_CPU_A[ 2], BUS_CPU_A[ 1], BUS_CPU_A[ 0]);
      /* p07.TUFA*/ wire _TUFA_XX_x1x1xxxxp_ext = and2(BUS_CPU_A[ 4], BUS_CPU_A[ 6]);
      /*#p01.ALUR*/ wire _ALUR_SYS_RSTn_new =  not1(AVOR_SYS_RSTp);
      /* p07.TEXE*/ wire _TEXE_FF50_RDp_ext =  and4(TEDO_CPU_RDp,    SYKE_ADDR_HIp_ext(BUS_CPU_A), _TYRO_XX_0x0x0000p_ext, _TUFA_XX_x1x1xxxxp_ext);
      /* p07.TUGE*/ wire _TUGE_FF50_WRn_clk = nand4(TAPU_CPU_WRp, SYKE_ADDR_HIp_ext(BUS_CPU_A), _TYRO_XX_0x0x0000p_ext, _TUFA_XX_x1x1xxxxp_ext);
      // FF50 - disable bootrom bit
      /* p07.SATO*/ wire _SATO_BOOT_BITn_old = or2(BUS_CPU_D[0], BOOT_BITn_h.qp_old());
      /* p07.TEPU*/ BOOT_BITn_h.dff17(_TUGE_FF50_WRn_clk, _ALUR_SYS_RSTn_new, _SATO_BOOT_BITn_old);
      /* p07.SYPU*/ BUS_CPU_D_out[0].tri6_pn(_TEXE_FF50_RDp_ext, BOOT_BITn_h.qp_new());
    }

    /* BOOT -> CBD */
#if 0
  /* p07.ZYBA*/ wire ZYBA_ADDR_00n = not1(BUS_CPU_A[ 0]);
  /* p07.ZUVY*/ wire ZUVY_ADDR_01n = not1(BUS_CPU_A[ 1]);
  /* p07.ZUFY*/ wire ZUFY_ADDR_04n = not1(BUS_CPU_A[ 4]);
  /* p07.ZERA*/ wire ZERA_ADDR_05n = not1(BUS_CPU_A[ 5]);
  /* p07.ZOLE*/ wire ZOLE_ADDR_00  = and2(ZUVY_ADDR_01n, ZYBA_ADDR_00n);
  /* p07.ZAJE*/ wire ZAJE_ADDR_01  = and2(ZUVY_ADDR_01n, BUS_CPU_A[ 0]);
  /* p07.ZUBU*/ wire ZUBU_ADDR_10  = and2(BUS_CPU_A[ 1], ZYBA_ADDR_00n);
  /* p07.ZAPY*/ wire ZAPY_ADDR_11  = and2(BUS_CPU_A[ 1], BUS_CPU_A[ 0]);

  /* p07.ZETE*/ wire BOOTROM_A1nA0n = not1(ZOLE_ADDR_00);
  /* p07.ZEFU*/ wire BOOTROM_A1nA0p  = not1(ZAJE_ADDR_01);
  /* p07.ZYRO*/ wire BOOTROM_A1pA0n  = not1(ZUBU_ADDR_10);
  /* p07.ZAPA*/ wire BOOTROM_A1pA0p   = not1(ZAPY_ADDR_11);
  /* p07.ZYGA*/ wire BOOTROM_A2n    = not1(BUS_CPU_A[ 2]);
  /* p07.ZYKY*/ wire BOOTROM_A3n    = not1(BUS_CPU_A[ 3]);
  /* p07.ZYKY*/ wire BOOTROM_A5nA4n = and2(ZERA_ADDR_05n, ZUFY_ADDR_04n);
  /* p07.ZYGA*/ wire BOOTROM_A5nA4p  = and2(ZERA_ADDR_05n, BUS_CPU_A[ 4]);
  /* p07.ZOVY*/ wire BOOTROM_A5pA4n  = and2(BUS_CPU_A[ 5], ZUFY_ADDR_04n);
  /* p07.ZUKO*/ wire BOOTROM_A5pA4p   = and2(BUS_CPU_A[ 5], BUS_CPU_A[ 4]);
  /* p07.ZAGE*/ wire BOOTROM_A6n    = not1(BUS_CPU_A[ 6]);
  /* p07.ZYRA*/ wire BOOTROM_A7n    = not1(BUS_CPU_A[ 7]);
#endif

    // this is kind of a hack
    uint16_t cpu_addr = pack_u16p(16, BUS_CPU_A);
    uint8_t bootrom_data = boot_buf[cpu_addr & 0xFF];

    /* p07.TULO*/ wire _TULO_ADDR_BOOTROMp_new = nor8(BUS_CPU_A[15], BUS_CPU_A[14], BUS_CPU_A[13], BUS_CPU_A[12],
                                                      BUS_CPU_A[11], BUS_CPU_A[10], BUS_CPU_A[ 9], BUS_CPU_A[ 8]);
    /* p07.TERA*/ wire _TERA_BOOT_BITp_new  = not1(BOOT_BITn_h.qp_new());
    /* p07.TUTU*/ wire _TUTU_READ_BOOTROMp_new = and2(_TERA_BOOT_BITp_new, _TULO_ADDR_BOOTROMp_new);

    /* p07.ZORO*/ wire _ZORO_0000xxxx_XXp_ext = nor4(BUS_CPU_A[15], BUS_CPU_A[14], BUS_CPU_A[13], BUS_CPU_A[12]);
    /* p07.ZADU*/ wire _ZADU_xxxx0000_XXp_ext = nor4(BUS_CPU_A[11], BUS_CPU_A[10], BUS_CPU_A[ 9], BUS_CPU_A[ 8]);
    /* p07.ZUFA*/ wire _ZUFA_0000_00FFp_ext  = and2(_ZORO_0000xxxx_XXp_ext, _ZADU_xxxx0000_XXp_ext);
    /* p07.YAZA*/ wire _YAZA_MODE_DBG1n_ext = not1(UMUT_MODE_DBG1p_ext);
    /* p07.YULA*/ wire _YULA_BOOT_RDp_new   = and3(TEDO_CPU_RDp, _YAZA_MODE_DBG1n_ext, _TUTU_READ_BOOTROMp_new); // def AND
    /* p07.ZADO*/ wire _ZADO_BOOT_CSn_new   = nand2(_YULA_BOOT_RDp_new, _ZUFA_0000_00FFp_ext);
    /* p07.ZERY*/ wire _ZERY_BOOT_CSp_new   = not1(_ZADO_BOOT_CSn_new);
    BUS_CPU_D_out[0].tri6_pn(_ZERY_BOOT_CSp_new, !bool(bootrom_data & 0x01));
    BUS_CPU_D_out[1].tri6_pn(_ZERY_BOOT_CSp_new, !bool(bootrom_data & 0x02));
    BUS_CPU_D_out[2].tri6_pn(_ZERY_BOOT_CSp_new, !bool(bootrom_data & 0x04));
    BUS_CPU_D_out[3].tri6_pn(_ZERY_BOOT_CSp_new, !bool(bootrom_data & 0x08));
    BUS_CPU_D_out[4].tri6_pn(_ZERY_BOOT_CSp_new, !bool(bootrom_data & 0x10));
    BUS_CPU_D_out[5].tri6_pn(_ZERY_BOOT_CSp_new, !bool(bootrom_data & 0x20));
    BUS_CPU_D_out[6].tri6_pn(_ZERY_BOOT_CSp_new, !bool(bootrom_data & 0x40));
    BUS_CPU_D_out[7].tri6_pn(_ZERY_BOOT_CSp_new, !bool(bootrom_data & 0x80));
  }

  wire TUTU_READ_BOOTROMp_new(wire BUS_CPU_A[16])
  {
    /* p07.TULO*/ wire _TULO_ADDR_BOOTROMp_new = nor8(BUS_CPU_A[15], BUS_CPU_A[14], BUS_CPU_A[13], BUS_CPU_A[12],
                                                      BUS_CPU_A[11], BUS_CPU_A[10], BUS_CPU_A[ 9], BUS_CPU_A[ 8]);
    /* p07.TERA*/ wire _TERA_BOOT_BITp_new  = not1(BOOT_BITn_h.qp_new());
    /* p07.TUTU*/ wire _TUTU_READ_BOOTROMp_new = and2(_TERA_BOOT_BITp_new, _TULO_ADDR_BOOTROMp_new);
    return _TUTU_READ_BOOTROMp_new;
  }

  /*p07.TEPU*/ DFF17 BOOT_BITn_h;
};

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct GateBoy {

  void dump(Dumper& d) const;

  void reset_boot(uint8_t* _boot_buf, size_t _boot_size,
                     uint8_t* _cart_buf, size_t _cart_size, bool fastboot);

  void reset_cart(uint8_t* _boot_buf, size_t _boot_size,
                   uint8_t* _cart_buf, size_t _cart_size);

  void set_cart(uint8_t* _boot_buf, size_t _boot_size,
                uint8_t* _cart_buf, size_t _cart_size);


  void load_post_bootrom_state();

  //----------------------------------------

  void load_dump(const char* filename) {
    load_obj(filename, *this);
    check_sentinel();
  }

  void save_dump(const char* filename) {
    save_obj(filename, *this);
  }

  //----------------------------------------

  void check_div() const {
    int div = div_reg.get_div();
    if (div != BOOT_DIV) {
      printf("div fail!\n");
      *reinterpret_cast<int*>(SENTINEL4) = 1;
    }
  }

  void check_sentinel() const {
    if (sentinel1 != SENTINEL1) {
      printf("sentinel1 fail!\n");
      *reinterpret_cast<int*>(SENTINEL4) = 1;
    }
    if (sentinel2 != SENTINEL2) {
      printf("sentinel2 fail!\n");
      *reinterpret_cast<int*>(SENTINEL4) = 1;
    }
    if (sentinel3 != SENTINEL3) {
      printf("sentinel3 fail!\n");
      *reinterpret_cast<int*>(SENTINEL4) = 1;
    }
    if (sentinel4 != SENTINEL4) {
      printf("sentinel4 fail!\n");
      *reinterpret_cast<int*>(SENTINEL4) = 1;
    }
  }

  uint8_t dbg_read (int addr);
  void dbg_write(int addr, uint8_t data);

  void set_boot_bit() {
    dbg_write(0xFF50, 0xFF);
  }

  //-----------------------------------------------------------------------------

  /* p07.UBET*/ wire UBETp_ext()           const { return not1(wire(sys_t1)); }
  /* p07.UVAR*/ wire UVARp_ext()           const { return not1(wire(sys_t2)); }
  /* p07.UMUT*/ wire UMUT_MODE_DBG1p_ext() const { return and2(wire(sys_t1), UVARp_ext()); }
  /* p07.UNOR*/ wire UNOR_MODE_DBG2p() const { return and2(wire(sys_t2), UBETp_ext()); }
  /* p07.UPOJ*/ wire UPOJ_MODE_PRODn_ext() const { return nand3(UBETp_ext(), UVARp_ext(), wire(sys_rst)); }
  /* p08.RYCA*/ wire RYCA_MODE_DBG2n_ext() const { return not1(UNOR_MODE_DBG2p()); }
  /* p08.TOVA*/ wire TOVA_MODE_DBG2n_ext() const { return not1(UNOR_MODE_DBG2p()); }
  /* p08.MULE*/ wire MULE_MODE_DBG1n_ext() const { return not1(UMUT_MODE_DBG1p_ext()); }

  //-----------------------------------------------------------------------------

  void run(int phase_count) {
    for (int i = 0; i < phase_count; i++) {
      next_phase();
    }
  }

  void next_phase();

  void tock_slow(int pass_index);

  uint8_t* reg_begin() { return (uint8_t*)(&sentinel1) + sizeof(sentinel1); }
  uint8_t* reg_end()   { return (uint8_t*)(&sentinel2); }
  int64_t commit_and_hash() {
    return ::commit_and_hash(reg_begin(), reg_end() - reg_begin());
  }

  void tock_joypad(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],
    wire AVOR_SYS_RSTp,
    wire BOGA_Axxxxxxx_clkevn,
    wire TEDO_CPU_RDp,
    wire TAPU_CPU_WRp
  );

  void tock_serial(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],
    wire AVOR_SYS_RSTp,
    wire TEDO_CPU_RDp,
    wire TAPU_CPU_WRp
  );

  void tock_interrupts(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],
    wire AVOR_SYS_RSTp,
    wire TEDO_CPU_RDp,
    wire TAPU_CPU_WRp,

    wire FEPO_STORE_MATCHp_new_evn,
    wire PARU_VBLANKp_new_evn,
    wire PURE_LINE_ENDn_new_evn,
    wire MOBA_TIMER_OVERFLOWp_evn_new,
    wire WODU_HBLANKp_new_odd);

  void tock_reg_lyc(
    wire BUS_CPU_D[8],
    wire AVOR_SYS_RSTp,
    wire TEDO_CPU_RDp,
    wire TAPU_CPU_WRp,

    wire WERO_ADDR_PPUp_ext,
    wire XOLA_A00n_ext, wire WADO_A00p_ext,
    wire XENO_A01n_ext, wire WESA_A01p_ext,
    wire XUSY_A02n_ext, wire WALO_A02p_ext,
    wire XERA_A03n_ext, wire WEPO_A03p_ext
  );

  void tock_reg_stat(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],
    wire AVOR_SYS_RSTp,
    wire TEDO_CPU_RDp,
    wire TAPU_CPU_WRp,

    wire ACYL_SCANNINGp_new_evn,
    wire XYMU_RENDERINGp_new_xxx,
    wire PARU_VBLANKp_new_evn
  );

  void tock_pix_pipe(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],
    wire XODO_VID_RSTp_new_h,

    wire TEDO_CPU_RDp,
    wire TAPU_CPU_WRp,

    wire XYMU_RENDERINGp_new_xxx,
    wire SACU_CLKPIPE_AxCxExGx_clknew_evn,
    wire NYXU_BFETCH_RSTn_new_xxx,

    wire &REMY_LD0n_new,
    wire &RAVO_LD1n_new
  );

  void tock_lcd(
    wire XODO_VID_RSTp_new_h,
    wire TALU_xxCDEFxx_clkevn,
    wire TYFA_CLKPIPE_xBxDxFxH_clknew_odd,
    wire SACU_CLKPIPE_AxCxExGx_clknew_evn,
    wire XONA_LCDC_LCDENp_h_new,
    wire XYMU_RENDERINGp_new_xxx,
    wire AVAP_SCAN_DONE_TRIGp_new_xxx,
    wire WEGO_HBLANKp_new_any,
    wire PURE_LINE_ENDn_new_evn,
    wire REMY_LD0n_new,
    wire RAVO_LD1n_new
  );

  void tock_ext(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],
    wire ABUZ_xxCDEFGH_clk_evn,
    wire TEDO_CPU_RDp,
    wire APOV_CPU_WRp_clkevn,
    wire TEXO_ADDR_VRAMn_ext,
    wire TUTU_READ_BOOTROMp_new
  );

  void tock_vram(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],

    wire AVOR_SYS_RSTp,
    wire XODO_VID_RSTp_new_h,

    wire ATAL_xBxDxFxH_clk_odd,
    wire ABUZ_xxCDEFGH_clk_evn,

    wire CATY_LATCH_EXTp_ext,
    wire SOSE_ADDR_VRAMp_ext,

    wire TEDO_CPU_RDp,
    wire TAPU_CPU_WRp,
    wire APOV_CPU_WRp_clkevn,

    wire TUTO_DBG_VRAMp_new,

    wire ATEJ_LINE_RSTp,
    wire TEVO_FETCH_TRIGp_new,
    wire NYXU_BFETCH_RSTn_new_xxx,
    wire PARU_VBLANKp_new_evn,
    wire PORE_WIN_MODEp_new_evn,
    wire WYMO_LCDC_WINENn_new,
    wire XYMU_RENDERINGp_new_xxx
  );

  void tock_oam(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],
    wire AVOR_SYS_RSTp,
    wire ATAL_xBxDxFxH_clk_odd,
    wire UVYT_ABCDxxxx_clkevn,
    wire XOCE_xBCxxFGx_clkodd,
    wire XYSO_xBCDxFGH_clkodd,
    wire TAPU_CPU_WRp,
    wire TEDO_CPU_RDp,
    wire CATY_LATCH_EXTp_ext,
    wire MATU_DMA_RUNNINGp_new_evn,
    wire XYMU_RENDERINGp_new_xxx,
    wire ACYL_SCANNINGp_new_evn
  );

  void tock_zram(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],
    wire TEDO_CPU_RDp,
    wire TAPU_CPU_WRp
  );

  //-----------------------------------------------------------------------------
  // All the SOC registers, pins, buses. Everything in this section should derive
  // from BitBase.

  uint64_t sentinel1 = SENTINEL1;

  //----------
  // CPU interface

  BusOut BUS_CPU_D_out[8];

  PinOut PIN_CPU_WAKE;          // top right wire by itself <- P02.AWOB

  PinIn  PIN_CPU_RDp;           // top right port PORTA_00: -> LAGU, LAVO, TEDO
  PinIn  PIN_CPU_WRp;           // top right port PORTA_01: ->
  PinOut PIN_CPU_UNOR_DBG;      // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  PinOut PIN_CPU_ADDR_HIp;      // top right port PORTA_03: <- P25.SYRO_FE00_FFFFp
  PinOut PIN_CPU_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
  PinOut PIN_CPU_UMUT_DBG;      // top right port PORTA_05: <- P07.UMUT_MODE_DBG1
  PinIn  PIN_CPU_EXT_BUSp;      // top right port PORTA_06: -> TEXO, APAP

  PinIn  PIN_CPU_ACK_VBLANK;    // bottom right port PORTB_01: -> P02.LETY, vblank int ack
  PinOut PIN_CPU_INT_VBLANK;    // bottom right port PORTB_03: <- P02.LOPE, vblank int
  PinIn  PIN_CPU_ACK_STAT  ;    // bottom right port PORTB_05: -> P02.LEJA, stat int ack
  PinOut PIN_CPU_INT_STAT  ;    // bottom right port PORTB_07: <- P02.LALU, stat int
  PinIn  PIN_CPU_ACK_TIMER ;    // bottom right port PORTB_09: -> P02.LESA, timer int ack
  PinOut PIN_CPU_INT_TIMER ;    // bottom right port PORTB_11: <- P02.NYBO, timer int
  PinIn  PIN_CPU_ACK_SERIAL;    // bottom right port PORTB_13: -> P02.LUFE, serial int ack
  PinOut PIN_CPU_INT_SERIAL;    // bottom right port PORTB_15: <- P02.UBUL, serial int
  PinIn  PIN_CPU_ACK_JOYPAD;    // bottom right port PORTB_17: -> P02.LAMO, joypad int ack
  PinOut PIN_CPU_INT_JOYPAD;    // bottom right port PORTB_19: <- P02.ULAK, joypad int

  PinIn  PIN_CPU_6;             // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6?
  PinOut PIN_CPU_BOWA_Axxxxxxx; // top left port PORTD_01: <- this is the "put address on bus" clock
  PinOut PIN_CPU_BEDO_xBCDEFGH; // top left port PORTD_02: <-
  PinOut PIN_CPU_BEKO_ABCDxxxx; // top left port PORTD_03: <- this is the "reset for next cycle" clock
  PinOut PIN_CPU_BUDE_xxxxEFGH; // top left port PORTD_04: <- this is the "put write data on bus" clock
  PinOut PIN_CPU_BOLO_ABCDEFxx; // top left port PORTD_05: <-
  PinIn  PIN_CPU_LATCH_EXT;     // top left port PORTD_06: -> ANUJ, DECY, LAVO, MUZU
  PinOut PIN_CPU_BUKE_AxxxxxGH; // top left port PORTD_07: <- this is probably the "latch bus data" clock
  PinOut PIN_CPU_BOMA_xBCDEFGH; // top left port PORTD_08: <- (RESET_CLK) // These two clocks are the only ones that run before PIN_CPU_READYp is asserted.
  PinOut PIN_CPU_BOGA_Axxxxxxx; // top left port PORTD_09: <- test pad 3

  //----------

  OamTempA oam_temp_a;
  OamTempB oam_temp_b;
  OamLatchA oam_latch_a;
  OamLatchB oam_latch_b;
  OamBus oam_bus;

  //----------

  ExtDataLatch ext_data_latch;
  ExtAddrLatch ext_addr_latch;
  PinOut PIN_EXT_CLK;    // PIN_75
  PinOut PIN_EXT_CSn;    // PIN_80 // CS changes on phase C if addr in [A000,FDFF]
  PinOut PIN_EXT_RDn;    // PIN_79 // RDn idles low, goes high on phase B for an external write
  PinOut PIN_EXT_WRn;    // PIN_78 // WRn idles high, goes low during EFG if there's a write
  PinOut PIN_EXT_A[16];
  PinIO  PIN_EXT_D[8];

  //----------

  SpriteTempA sprite_temp_a;
  SpriteTempB sprite_temp_b;
  BusOut BUS_VRAM_An[13];
  BusIO  BUS_VRAM_Dp[8];
  PinOut PIN_VRAM_CSn; // PIN_43
  PinOut PIN_VRAM_OEn; // PIN_45
  PinOut PIN_VRAM_WRn; // PIN_49
  PinOut PIN_VRAM_Ap[13];
  PinIO  PIN_VRAM_Dp[8];

  //----------

  ResetRegisters      rst_reg;
  PhaseClockRegisters pclk_reg;
  VideoClockRegisters vclk_reg;
  DebugRegisters      dbg_reg;

  //----------

  DivRegister    div_reg;
  TimerRegisters tim_reg;
  Bootrom        bootrom;

  //----------

  DmaRegisters dma_reg;

  //----------

  InterruptRegisters int_reg;
  DFF IE_D0;
  DFF IE_D1;
  DFF IE_D2;
  DFF IE_D3;
  DFF IE_D4;

  //----------

  JoypadRegisters joypad;
  PinIn  PIN_JOY_P10; // PIN_67   Pressing a button pulls the corresponding pin _down_.
  PinIn  PIN_JOY_P11; // PIN_66
  PinIn  PIN_JOY_P12; // PIN_65
  PinIn  PIN_JOY_P13; // PIN_64
  PinOut PIN_JOY_P14; // PIN_63
  PinOut PIN_JOY_P15; // PIN_62

  //----------

  SerialRegisters ser_reg;
  PinIO  PIN_SCK;  // PIN_68
  PinIn  PIN_SIN;  // PIN_69
  PinOut PIN_SOUT; // PIN_70

  //----------

  SpriteCounter sprite_counter;
  SpriteStore        sprite_store;
  BusIO SPR_TRI_I[6]; // AxCxExGx
  BusIO SPR_TRI_L[4]; // AxCxExGx

  //----------

  SpriteScanner sprite_scanner;
  ScanCounter scan_counter;

  //----------

  TileFetcher tile_fetcher;
  TileTempA tile_temp_a;
  TileTempB tile_temp_b;
  RegSCX reg_scx;
  RegSCY reg_scy;
  WinMapX win_map_x;
  WinLineY win_line_y;

  //----------

  SpriteFetcher sprite_fetcher;

  //----------

  PixCounter pix_count;
  RegStat reg_stat;
  RegBGP  reg_bgp;
  RegOBP0 reg_obp0;
  RegOBP1 reg_obp1;
  WindowRegisters win_reg;
  FineScroll fine_scroll;
  PPURegisters ppu_reg;
  PixelPipes pix_pipes;
  RegWY reg_wy;
  RegWX reg_wx;

  //----------

  RegLCDC reg_lcdc;
  RegLX   reg_lx;
  RegLY   reg_ly;
  RegLYC  reg_lyc;
  LcdRegisters lcd;

  //----------

  uint64_t sentinel2 = SENTINEL2;

  bool oam_clk_old = 0;
  bool zram_clk_old = 0;

  //-----------------------------------------------------------------------------
  // Control stuff

  bool sys_rst = 0;
  bool sys_t1 = 0;
  bool sys_t2 = 0;
  bool sys_clken = 0;
  bool sys_clkgood = 0;
  bool sys_cpuready = 0;
  bool sys_cpu_en = 0;
  bool sys_fastboot = 0;
  bool sys_cpu_start = 0;
  uint8_t sys_buttons = 0;

  //-----------------------------------------------------------------------------
  // CPU

  CPU      cpu;
  Req      cpu_req = {0};
  Req      dbg_req = {0};
  Req      bus_req = {0};
  uint8_t  cpu_data_latch = 0;
  uint8_t  imask_latch = 0;

  bool int_vblank = 0;
  bool int_vblank_halt = 0;

  bool int_stat = 0;
  bool int_stat_halt = 0;

  bool int_timer = 0;
  bool int_timer_halt = 0;

  bool int_serial = 0;
  bool int_serial_halt = 0;

  bool int_joypad = 0;
  bool int_joypad_halt = 0;

  //-----------------------------------------------------------------------------
  // Memory

  uint8_t vid_ram [8192];
  uint8_t cart_ram[8192];
  uint8_t ext_ram [8192];
  uint8_t oam_ram [256];
  uint8_t zero_ram[128];

  //-----------------------------------------------------------------------------
  // LCD and framebuffer

  uint8_t framebuffer[160*144];
  uint8_t gb_screen_x = 0;
  uint8_t gb_screen_y = 0;
  bool old_lcd_clock = 0;
  bool old_lcd_latch = 0;
  uint8_t lcd_data_latch = 0;

  //-----------------------------------------------------------------------------

  uint64_t sentinel3 = SENTINEL3;

  double   sim_time = 0;
  int32_t  phase_total = 0;
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
