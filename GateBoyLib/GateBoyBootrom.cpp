#include "GateBoyLib/GateBoyBootrom.h"

//--------------------------------------------------------------------------------

void GateBoyBootrom::reset_cart() {
  BOOT_BITn_h.reset(REG_D1C1);
}

//--------------------------------------------------------------------------------

void GateBoyBootrom::tock(
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

//--------------------------------------------------------------------------------

wire GateBoyBootrom::TUTU_READ_BOOTROMp_new(wire BUS_CPU_A[16])
{
  /* p07.TULO*/ wire _TULO_ADDR_BOOTROMp_new = nor8(BUS_CPU_A[15], BUS_CPU_A[14], BUS_CPU_A[13], BUS_CPU_A[12],
                                                    BUS_CPU_A[11], BUS_CPU_A[10], BUS_CPU_A[ 9], BUS_CPU_A[ 8]);
  /* p07.TERA*/ wire _TERA_BOOT_BITp_new  = not1(BOOT_BITn_h.qp_new());
  /* p07.TUTU*/ wire _TUTU_READ_BOOTROMp_new = and2(_TERA_BOOT_BITp_new, _TULO_ADDR_BOOTROMp_new);
  return _TUTU_READ_BOOTROMp_new;
}

//--------------------------------------------------------------------------------
