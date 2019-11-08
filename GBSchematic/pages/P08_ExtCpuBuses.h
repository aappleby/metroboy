#pragma once

struct Gameboy;

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P08_ExtCpuBuses {
#if 0
  struct Input {
    bool CPU_RD;
    bool CPU_RD_SYNC;
    bool CPU_RAW_RD;

    bool FROM_CPU3;
    bool FROM_CPU4;
    bool FROM_CPU5;

    bool T1T2n;
    bool T1nT2;
    bool FEXXFFXXn;
    bool NET02;

    bool TUTU; // P07, this is the "use bootrom" signal
    bool LUMA;
    bool ABUZ; // P01, controlled by FROM_CPU4

               // wat?
    bool A15_C;

    bool A0_IN,A1_IN,A2_IN,A3_IN,A4_IN,A5_IN,A6_IN,A7_IN,A8_IN,A9_IN,A10_IN,A11_IN,A12_IN,A13_IN,A14_IN,A15_IN;
    bool D0_IN,D1_IN,D2_IN,D3_IN,D4_IN,D5_IN,D6_IN,D7_IN;

    bool A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15;
    bool D0,D1,D2,D3,D4,D5,D6,D7;

    bool DMA_A0,DMA_A1,DMA_A2,DMA_A3,DMA_A4,DMA_A5,DMA_A6,DMA_A7,DMA_A8,DMA_A9,DMA_A10,DMA_A11,DMA_A12,DMA_A13,DMA_A14,DMA_A15;
  };

  struct Output {
    bool CS_OUT;
    bool WR_A;
    bool WR_C;
    bool RD_A;
    bool RD_C;

    bool NET01;  // equal to not(T1nT2)

    bool TOLA_A1n;

    bool A_OE;
    bool A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15;

    bool D_OE;
    bool D0,D1,D2,D3,D4,D5,D6,D7;

    // signals to physical address pins
    bool A0_A,A1_A,A2_A,A3_A,A4_A,A5_A,A6_A,A7_A,A8_A,A9_A,A10_A,A11_A,A12_A,A13_A,A14_A,A15_A;
    bool A0_D,A1_D,A2_D,A3_D,A4_D,A5_D,A6_D,A7_D,A8_D,A9_D,A10_D,A11_D,A12_D,A13_D,A14_D,A15_D;

    // signals to physical data pins
    bool D0_D,D1_D,D2_D,D3_D,D4_D,D5_D,D6_D,D7_D;
  };
#endif

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



  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);

  bool SORE,TEVY,TEXO,LEVO,LAGU,LYWE,MOCA,MEXO,NEVY,MOTY,PUVA,TYMU,USUF,UVER,UGAC,URUN;

  bool TOLA,MULE,LOXO,LASY,MATE;

  bool SOGY,TUMA,TYNU,TOZA;

  bool SOBY_15,SEPY_15,TYHO,TAZY_15,RYCA,RULO_15,SUZE_15;

  bool TOVA;
  bool LAVO;

  bool REDU,RORU,LULA,LYRA;
};
