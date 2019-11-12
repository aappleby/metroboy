#pragma once

struct Gameboy;

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P08_ExtCpuBuses {
  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { bool TOLA; bool TOLA_A1n; };
  union { bool TOVA; bool NET01; };

  bool LULA; // -> p25.ryvo...

  bool TEXO;

private:

  // Address latch
  bool ALOR_00,APUR_01,ALYR_02,ARET_03,AVYS_04,ATEV_05,AROS_06,ARYM_07;
  bool AMET_00,ATOL_01,APOK_02,AMER_03,ATEM_04,ATOV_05,ATYR_06,ASUR_07;
  bool KOTY_00,COTU_01,BAJO_02,BOLA_03,BEVO_04,AJAV_05,CYKA_06,COLO_07;
  bool KUPO_00,CABA_01,BOKU_02,BOTY_03,BYLA_04,BADU_05,CEPU_06,DEFY_07;

  bool LUNO_08,LYSA_09,PATE_10,LUMY_11,LOBU_12,LONU_13,NYRE_14;
  bool MANO_08,MASU_09,PAMY_10,MALE_11,MOJY_12,MUCE_13,PEGE_14;
  bool MEGO_08,MENY_09,RORE_10,LYNY_11,LOSO_12,LEVA_13,PAHY_14;
  bool MYNY_08,MUNE_09,ROXU_10,LEPY_11,LUCE_12,LABE_13,PUHE_14;

  // Data output driver
  bool RUXA_00,RUJA_01,RABY_02,RERA_03,RORY_04,RYVO_05,RAFY_06,RAVU_07; // DX_A
  bool TOVO_00,RUZY_01,ROME_02,SAZA_03,TEHE_04,RATU_05,SOCA_06,RYBA_07; // DX_C
  bool RUNE_00,RYPU_01,SULY_02,SEZE_03,RESY_04,TAMU_05,ROGY_06,RYDA_07; // DX_D
  bool TUTY_00,SYWA_01,SUGU_02,TAWO_03,TUTE_04,SAJO_05,TEMY_06,ROPA_07;

  // Data input latch
  bool SOMA_00,RONY_01,RAXY_02,SELO_03,SODY_04,SAGO_05,RUPA_06,SAZY_07;
  bool RYMA_00,RUVO_01,RYKO_02,TAVO_03,TEPE_04,SAFO_05,SEVU_06,TAJU_07;

  // Debug, drives external address onto internal address
  bool KOVA_00,CAMU_01,BUXU_02,BASE_03,AFEC_04,ABUP_05,CYGU_06,COGO_07;
  bool KEJO_00,BYXE_01,AKAN_02,ANAR_03,AZUV_04,AJOV_05,BYNE_06,BYNA_07;

  bool MUJY_08,NENA_09,SURA_10,MADY_11,LAHE_12,LURA_13,PEVO_14,RAZA_15;
  bool LOFA_08,MAPU_09,RALA_10,LORA_11,LYNA_12,LEFY_13,NEFE_14,SYZU_15;

  union { bool SORE; bool ADDR_0000_7FFF; };
  union { bool TEVY; bool ADDR_NOT_VRAM; };

  bool LEVO,LAGU,LYWE,MOCA,MEXO,NEVY,MOTY,PUVA,TYMU,USUF,UVER,UGAC,URUN;
  bool MULE,LOXO,LASY,MATE;
  bool SOGY,TUMA,TYNU,TOZA;
  bool SOBY_15,SEPY_15,TYHO,TAZY_15,RYCA,RULO_15,SUZE_15;
  bool LAVO;
  bool REDU,RORU,LYRA;
};
