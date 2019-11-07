#pragma once

struct Gameboy;

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P06_SerialLink {
  // FF01 SB
  bool SARE,SEFY,SANO,URYS,DAKU,EPYT,DEHO,DAWE,CAGE,UFEG;
  bool COHY_00,DUMO_01,DYBO_02,DAJU_03,DYLY_04,EHUJ_05,EFAK_06,EGUV_07;
  bool CUFU_00,DOCU_01,DELA_02,DYGE_03,DOLA_04,ELOK_05,EDEL_06,EFEF_07;
  bool CUBA_00,DEGU_01,DYRA_02,DOJO_03,DOVU_04,EJAB_05,EROD_06,EDER_07;
  bool CUGY_00,DUDE_01,DETU_02,DASO_03,DAME_04,EVOK_05,EFAB_06,ETAK_07;

  // FF02 SC
  bool CULY_00, ETAF_07;
  bool CORE_00, ELUV_07;

  // clock divider
  bool COTY;

  // output reg
  bool ELYS;

  // counter that triggers INT_SERIAL
  bool CAFA_00,CYLO_01,CYDE_02,CALY_03;

  bool UWAM,UCOM,CAVE,DAWA,CARO,JAGO,EDYL,KEXU,KUJO,COBA,CABY;

  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);
};
