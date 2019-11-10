#include "P08_ExtCpuBuses.h"
#include "../Schematics.h"
#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P08_ExtCpuBuses::tick(const Gameboy& /*a*/, const Gameboy& b, Gameboy& c) {

  c.p08.TOVA = not(b.p07.T1nT2);
  c.p08.RYCA = not(b.p07.T1nT2);
  c.p08.MULE = not(b.p07.T1T2n);

  //----------
  // center right, generating the external read/write signals to the cart

  c.p08.SORE = not(b.A15);

  // TEVY = not_vram
  c.p08.TEVY = or(b.A13, b.A14, b.p08.SORE); // BUG - pretty sure this is an OR and not an AND, the dots are on the opposite side compared to TOZA

  c.p08.TEXO = and(b.cpu.FROM_CPU4, b.p08.TEVY);
  c.p08.LEVO = not(b.p08.TEXO);
  c.p08.LAGU = unk3(b.cpu.CPU_RAW_RD, b.p08.LEVO, b.cpu.FROM_CPU3);
  c.p08.LYWE = not(b.p08.LAGU);

  c.p08.MOCA = nor(b.p08.TEXO, b.p07.T1T2n);
  c.p08.MEXO = not(b.p01.CPU_WR_SYNC);
  c.p08.NEVY = or(b.p08.MEXO, b.p08.MOCA);
  c.p08.MOTY = or(b.p08.MOCA, b.p08.LYWE);
  c.p08.PUVA = or(b.p08.NEVY, b.p04.LUMA);
  c.p08.TYMU = nor(b.p04.LUMA, b.p08.MOTY);

  c.p08.UVER = nand(b.p08.PUVA, b.p08.NET01);
  c.p08.USUF = nor (b.p08.PUVA, b.p07.T1nT2);
  c.p08.UGAC = nand(b.p08.TYMU, b.p08.NET01);
  c.p08.URUN = nor (b.p08.TYMU, b.p07.T1nT2);

  //----------
  // top center

  c.p08.TOLA = not(b.A01);
  c.p08.LOXO = unk3(b.p08.MULE, b.p08.TEXO, b.p07.T1T2n);
  c.p08.LASY = not(b.p08.LOXO);
  c.p08.MATE = not(b.p08.LASY);

  //----------
  // left center, CS and A15 drivers

  c.p08.SOGY = not(b.A14);
  c.p08.TUMA = and(b.A13, b.p08.SOGY, b.A15); // selects A000-BFFF, cart ram
  
  //c.p08.TYNU = unk3(b.A15, b.A14, b.p08.TUMA);
  
  // this is a guess, it selects addr < 0xC00 || cart_ram, which seems reasonable
  c.p08.TYNU = nor(nand(b.A15, b.A14), b.p08.TUMA);
  
  c.p08.TOZA = and(b.p08.TYNU, b.p01.CPU_RD_SYNC, b.p07.FEXXFFXXn);

  // TUTU is the "use bootrom" signal, so this is saying "if our address is
  // not in the high half and we're not running the bootrom, read from the
  // cart
  c.p08.SOBY_15 = nor(b.A15, b.p07.TUTU);
  c.p08.TYHO = mux2(b.p04.DMA_A15, b.p08.TOZA, b.p04.LUMA); // polarity?

  //----------
  // Low half of the address pin output driver

  c.p08.ALOR_00 = latch_pos(b.p08.MATE, b.p08.ALOR_00, b.A00);
  c.p08.APUR_01 = latch_pos(b.p08.MATE, b.p08.APUR_01, b.A01);
  c.p08.ALYR_02 = latch_pos(b.p08.MATE, b.p08.ALYR_02, b.A02);
  c.p08.ARET_03 = latch_pos(b.p08.MATE, b.p08.ARET_03, b.A03);
  c.p08.AVYS_04 = latch_pos(b.p08.MATE, b.p08.AVYS_04, b.A04);
  c.p08.ATEV_05 = latch_pos(b.p08.MATE, b.p08.ATEV_05, b.A05);
  c.p08.AROS_06 = latch_pos(b.p08.MATE, b.p08.AROS_06, b.A06);
  c.p08.ARYM_07 = latch_pos(b.p08.MATE, b.p08.ARYM_07, b.A07);
  c.p08.LUNO_08 = latch_pos(b.p08.MATE, b.p08.LUNO_08, b.A08);
  c.p08.LYSA_09 = latch_pos(b.p08.MATE, b.p08.LYSA_09, b.A09);
  c.p08.PATE_10 = latch_pos(b.p08.MATE, b.p08.PATE_10, b.A10);
  c.p08.LUMY_11 = latch_pos(b.p08.MATE, b.p08.LUMY_11, b.A11);
  c.p08.LOBU_12 = latch_pos(b.p08.MATE, b.p08.LOBU_12, b.A12);
  c.p08.LONU_13 = latch_pos(b.p08.MATE, b.p08.LONU_13, b.A13);
  c.p08.NYRE_14 = latch_pos(b.p08.MATE, b.p08.NYRE_14, b.A14);
  c.p08.SEPY_15 = nand(b.p01.CPU_RD_SYNC, b.p08.SOBY_15);

  c.p08.AMET_00 = mux2(b.p04.DMA_A00, b.p08.ALOR_00, b.p04.LUMA);
  c.p08.ATOL_01 = mux2(b.p04.DMA_A01, b.p08.APUR_01, b.p04.LUMA);
  c.p08.APOK_02 = mux2(b.p04.DMA_A02, b.p08.ALYR_02, b.p04.LUMA);
  c.p08.AMER_03 = mux2(b.p04.DMA_A03, b.p08.ARET_03, b.p04.LUMA);
  c.p08.ATEM_04 = mux2(b.p04.DMA_A04, b.p08.AVYS_04, b.p04.LUMA);
  c.p08.ATOV_05 = mux2(b.p04.DMA_A05, b.p08.ATEV_05, b.p04.LUMA);
  c.p08.ATYR_06 = mux2(b.p04.DMA_A06, b.p08.AROS_06, b.p04.LUMA);
  c.p08.ASUR_07 = mux2(b.p04.DMA_A07, b.p08.ARYM_07, b.p04.LUMA);
  c.p08.MANO_08 = mux2(b.p04.DMA_A08, b.p08.LUNO_08, b.p04.LUMA);
  c.p08.MASU_09 = mux2(b.p04.DMA_A09, b.p08.LYSA_09, b.p04.LUMA);
  c.p08.PAMY_10 = mux2(b.p04.DMA_A10, b.p08.PATE_10, b.p04.LUMA);
  c.p08.MALE_11 = mux2(b.p04.DMA_A11, b.p08.LUMY_11, b.p04.LUMA);
  c.p08.MOJY_12 = mux2(b.p04.DMA_A12, b.p08.LOBU_12, b.p04.LUMA);
  c.p08.MUCE_13 = mux2(b.p04.DMA_A13, b.p08.LONU_13, b.p04.LUMA);
  c.p08.PEGE_14 = mux2(b.p04.DMA_A14, b.p08.NYRE_14, b.p04.LUMA);
  c.p08.TAZY_15 = mux2(b.p04.DMA_A15, b.p08.SEPY_15, b.p04.LUMA);

  c.p08.KUPO_00 = nand(b.p08.AMET_00, b.p08.TOVA);
  c.p08.CABA_01 = nand(b.p08.ATOL_01, b.p08.TOVA);
  c.p08.BOKU_02 = nand(b.p08.APOK_02, b.p08.TOVA);
  c.p08.BOTY_03 = nand(b.p08.AMER_03, b.p08.TOVA);
  c.p08.BYLA_04 = nand(b.p08.ATEM_04, b.p08.TOVA);
  c.p08.BADU_05 = nand(b.p08.ATOV_05, b.p08.TOVA);
  c.p08.CEPU_06 = nand(b.p08.ATYR_06, b.p08.TOVA);
  c.p08.DEFY_07 = nand(b.p08.ASUR_07, b.p08.TOVA);
  c.p08.MYNY_08 = nand(b.p08.MANO_08, b.p08.TOVA);
  c.p08.MUNE_09 = nand(b.p08.MASU_09, b.p08.TOVA);
  c.p08.ROXU_10 = nand(b.p08.PAMY_10, b.p08.TOVA);
  c.p08.LEPY_11 = nand(b.p08.MALE_11, b.p08.TOVA);
  c.p08.LUCE_12 = nand(b.p08.MOJY_12, b.p08.TOVA);
  c.p08.LABE_13 = nand(b.p08.MUCE_13, b.p08.TOVA);
  c.p08.PUHE_14 = nand(b.p08.PEGE_14, b.p08.TOVA);
  c.p08.SUZE_15 = nand(b.p08.TAZY_15, b.p08.RYCA);

  c.p08.KOTY_00 = nor(b.p08.AMET_00, b.p07.T1nT2);
  c.p08.COTU_01 = nor(b.p08.ATOL_01, b.p07.T1nT2);
  c.p08.BAJO_02 = nor(b.p08.APOK_02, b.p07.T1nT2);
  c.p08.BOLA_03 = nor(b.p08.AMER_03, b.p07.T1nT2);
  c.p08.BEVO_04 = nor(b.p08.ATEM_04, b.p07.T1nT2);
  c.p08.AJAV_05 = nor(b.p08.ATOV_05, b.p07.T1nT2);
  c.p08.CYKA_06 = nor(b.p08.ATYR_06, b.p07.T1nT2);
  c.p08.COLO_07 = nor(b.p08.ASUR_07, b.p07.T1nT2);
  c.p08.MEGO_08 = nor(b.p08.MANO_08, b.p07.T1nT2);
  c.p08.MENY_09 = nor(b.p08.MASU_09, b.p07.T1nT2);
  c.p08.RORE_10 = nor(b.p08.PAMY_10, b.p07.T1nT2);
  c.p08.LYNY_11 = nor(b.p08.MALE_11, b.p07.T1nT2);
  c.p08.LOSO_12 = nor(b.p08.MOJY_12, b.p07.T1nT2);
  c.p08.LEVA_13 = nor(b.p08.MUCE_13, b.p07.T1nT2);
  c.p08.PAHY_14 = nor(b.p08.PEGE_14, b.p07.T1nT2);
  c.p08.RULO_15 = nor(b.p08.TAZY_15, b.p07.T1nT2);

  //----------
  // Chip data pin output driver. Some of this was on P25, but it should _not_ be there.

  c.p08.REDU = not(b.p07.CPU_RD);
  c.p08.RORU = mux2(b.p08.REDU, b.p08.MOTY, b.p07.T1nT2);
  c.p08.LULA = not(b.p08.RORU);
  c.p08.LYRA = nand(b.p07.T1nT2, b.p08.RORU);
  c.p08.LAVO = nand(b.cpu.CPU_RAW_RD, b.p08.TEXO, b.cpu.FROM_CPU5);

  c.p08.RUNE_00 = nor(b.p08.RORU, b.D0);
  c.p08.RYPU_01 = nor(b.p08.RORU, b.D1);
  c.p08.SULY_02 = nor(b.p08.RORU, b.D2);
  c.p08.SEZE_03 = nor(b.p08.RORU, b.D3);
  c.p08.RESY_04 = nor(b.p08.RORU, b.D4);
  c.p08.TAMU_05 = nor(b.p08.RORU, b.D5);
  c.p08.ROGY_06 = nor(b.p08.RORU, b.D6);
  c.p08.RYDA_07 = nor(b.p08.RORU, b.D7);

  c.p08.RUXA_00 = nand(b.D0, b.p08.LULA);
  c.p08.RUJA_01 = nand(b.D1, b.p08.LULA);
  c.p08.RABY_02 = nand(b.D2, b.p08.LULA);
  c.p08.RERA_03 = nand(b.D3, b.p08.LULA);
  c.p08.RORY_04 = nand(b.D4, b.p08.LULA);
  c.p08.RYVO_05 = nand(b.D5, b.p08.LULA);
  c.p08.RAFY_06 = nand(b.D6, b.p08.LULA);
  c.p08.RAVU_07 = nand(b.D7, b.p08.LULA);

  //----------
  // Chip data pin input driver. Some of this was on P25, but it should _not_ be there.
  // Not sure why there's two of these

  c.p08.TOVO_00 = not(b.chip.D0_C);
  c.p08.RUZY_01 = not(b.chip.D1_C);
  c.p08.ROME_02 = not(b.chip.D2_C);
  c.p08.SAZA_03 = not(b.chip.D3_C);
  c.p08.TEHE_04 = not(b.chip.D4_C);
  c.p08.RATU_05 = not(b.chip.D5_C);
  c.p08.SOCA_06 = not(b.chip.D6_C);
  c.p08.RYBA_07 = not(b.chip.D7_C);

  c.p08.TUTY_00 = not(b.p08.TOVO_00);
  c.p08.SYWA_01 = not(b.p08.RUZY_01);
  c.p08.SUGU_02 = not(b.p08.ROME_02);
  c.p08.TAWO_03 = not(b.p08.SAZA_03);
  c.p08.TUTE_04 = not(b.p08.TEHE_04);
  c.p08.SAJO_05 = not(b.p08.RATU_05);
  c.p08.TEMY_06 = not(b.p08.SOCA_06);
  c.p08.ROPA_07 = not(b.p08.RYBA_07);

  c.p08.SOMA_00 = latch_pos(b.p08.LAVO, b.p08.SOMA_00, b.chip.D0_C);
  c.p08.RONY_01 = latch_pos(b.p08.LAVO, b.p08.RONY_01, b.chip.D1_C);
  c.p08.RAXY_02 = latch_pos(b.p08.LAVO, b.p08.RAXY_02, b.chip.D2_C);
  c.p08.SELO_03 = latch_pos(b.p08.LAVO, b.p08.SELO_03, b.chip.D3_C);
  c.p08.SODY_04 = latch_pos(b.p08.LAVO, b.p08.SODY_04, b.chip.D4_C);
  c.p08.SAGO_05 = latch_pos(b.p08.LAVO, b.p08.SAGO_05, b.chip.D5_C);
  c.p08.RUPA_06 = latch_pos(b.p08.LAVO, b.p08.RUPA_06, b.chip.D6_C);
  c.p08.SAZY_07 = latch_pos(b.p08.LAVO, b.p08.SAZY_07, b.chip.D7_C);

  c.p08.RYMA_00 = b.p08.SOMA_00;
  c.p08.RUVO_01 = b.p08.RONY_01;
  c.p08.RYKO_02 = b.p08.RAXY_02;
  c.p08.TAVO_03 = b.p08.SELO_03;
  c.p08.TEPE_04 = b.p08.SODY_04;
  c.p08.SAFO_05 = b.p08.SAGO_05;
  c.p08.SEVU_06 = b.p08.RUPA_06;
  c.p08.TAJU_07 = b.p08.SAZY_07;

  //----------
  // if NET01 high, drive external address bus onto internal address
  // probably not inverting...

  c.p08.KOVA_00 = not(b.chip.A00_C);
  c.p08.CAMU_01 = not(b.chip.A01_C);
  c.p08.BUXU_02 = not(b.chip.A02_C);
  c.p08.BASE_03 = not(b.chip.A03_C);
  c.p08.AFEC_04 = not(b.chip.A04_C);
  c.p08.ABUP_05 = not(b.chip.A05_C);
  c.p08.CYGU_06 = not(b.chip.A06_C);
  c.p08.COGO_07 = not(b.chip.A07_C);
  c.p08.MUJY_08 = not(b.chip.A08_C);
  c.p08.NENA_09 = not(b.chip.A09_C);
  c.p08.SURA_10 = not(b.chip.A10_C);
  c.p08.MADY_11 = not(b.chip.A11_C);
  c.p08.LAHE_12 = not(b.chip.A12_C);
  c.p08.LURA_13 = not(b.chip.A13_C);
  c.p08.PEVO_14 = not(b.chip.A14_C);
  c.p08.RAZA_15 = not(b.chip.A15_C);

  c.p08.KEJO_00 = b.p08.KOVA_00; 
  c.p08.BYXE_01 = b.p08.CAMU_01; 
  c.p08.AKAN_02 = b.p08.BUXU_02; 
  c.p08.ANAR_03 = b.p08.BASE_03; 
  c.p08.AZUV_04 = b.p08.AFEC_04; 
  c.p08.AJOV_05 = b.p08.ABUP_05; 
  c.p08.BYNE_06 = b.p08.CYGU_06; 
  c.p08.BYNA_07 = b.p08.COGO_07; 
  c.p08.LOFA_08 = b.p08.MUJY_08; 
  c.p08.MAPU_09 = b.p08.NENA_09; 
  c.p08.RALA_10 = b.p08.SURA_10; 
  c.p08.LORA_11 = b.p08.MADY_11; 
  c.p08.LYNA_12 = b.p08.LAHE_12; 
  c.p08.LEFY_13 = b.p08.LURA_13; 
  c.p08.NEFE_14 = b.p08.PEVO_14; 
  c.p08.SYZU_15 = b.p08.RAZA_15;

  // wat
  //c.chip.A15_C  = b.p08.RAZA_15;


  // these may be backwards, probably don't want to drive external address onto bus normally...

#if 0
  if (b.NET01) {
    c.cpu.A00 = b.p08.KEJO_00;
    c.cpu.A01 = b.p08.BYXE_01;
    c.cpu.A02 = b.p08.AKAN_02;
    c.cpu.A03 = b.p08.ANAR_03;
    c.cpu.A04 = b.p08.AZUV_04;
    c.cpu.A05 = b.p08.AJOV_05;
    c.cpu.A06 = b.p08.BYNE_06;
    c.cpu.A07 = b.p08.BYNA_07;
    c.cpu.A08 = b.p08.LOFA_08;
    c.cpu.A09 = b.p08.MAPU_09;
    c.cpu.A10 = b.p08.RALA_10;
    c.cpu.A11 = b.p08.LORA_11;
    c.cpu.A12 = b.p08.LYNA_12;
    c.cpu.A13 = b.p08.LEFY_13;
    c.cpu.A14 = b.p08.NEFE_14;
  }                 

  if (b.p08.RYCA) {
    c.cpu.A15 = b.p08.SYZU_15;
  }
#endif

  c.chip.CS_A = b.p08.TYHO;
  c.chip.WR_A = b.p08.UVER;
  c.chip.WR_D = b.p08.USUF;
  c.chip.RD_A = b.p08.UGAC;
  c.chip.RD_D = b.p08.URUN;

  c.chip.A00_A = b.p08.KUPO_00;
  c.chip.A01_A = b.p08.CABA_01;
  c.chip.A02_A = b.p08.BOKU_02;
  c.chip.A03_A = b.p08.BOTY_03;
  c.chip.A04_A = b.p08.BYLA_04;
  c.chip.A05_A = b.p08.BADU_05;
  c.chip.A06_A = b.p08.CEPU_06;
  c.chip.A07_A = b.p08.DEFY_07;
  c.chip.A08_A = b.p08.MYNY_08;
  c.chip.A09_A = b.p08.MUNE_09;
  c.chip.A10_A = b.p08.ROXU_10;
  c.chip.A11_A = b.p08.LEPY_11;
  c.chip.A12_A = b.p08.LUCE_12;
  c.chip.A13_A = b.p08.LABE_13;
  c.chip.A14_A = b.p08.PUHE_14;
  c.chip.A15_A = b.p08.SUZE_15;

  c.chip.A00_D = b.p08.KOTY_00;
  c.chip.A01_D = b.p08.COTU_01;
  c.chip.A02_D = b.p08.BAJO_02;
  c.chip.A03_D = b.p08.BOLA_03;
  c.chip.A04_D = b.p08.BEVO_04;
  c.chip.A05_D = b.p08.AJAV_05;
  c.chip.A06_D = b.p08.CYKA_06;
  c.chip.A07_D = b.p08.COLO_07;
  c.chip.A08_D = b.p08.MEGO_08;
  c.chip.A09_D = b.p08.MENY_09;
  c.chip.A10_D = b.p08.RORE_10;
  c.chip.A11_D = b.p08.LYNY_11;
  c.chip.A12_D = b.p08.LOSO_12;
  c.chip.A13_D = b.p08.LEVA_13;
  c.chip.A14_D = b.p08.PAHY_14;
  c.chip.A15_D = b.p08.RULO_15;

  c.chip.D0_A = b.p08.RUXA_00;
  c.chip.D1_A = b.p08.RUJA_01;
  c.chip.D2_A = b.p08.RABY_02;
  c.chip.D3_A = b.p08.RERA_03;
  c.chip.D4_A = b.p08.RORY_04;
  c.chip.D5_A = b.p08.RYVO_05;
  c.chip.D6_A = b.p08.RAFY_06;
  c.chip.D7_A = b.p08.RAVU_07;

  c.chip.D0_B = b.p08.LULA;
  c.chip.D1_B = b.p08.LULA;
  c.chip.D2_B = b.p08.LULA;
  c.chip.D3_B = b.p08.LULA;
  c.chip.D4_B = b.p08.LULA;
  c.chip.D5_B = b.p08.LULA;
  c.chip.D6_B = b.p08.LULA;
  c.chip.D7_B = b.p08.LULA;

  c.chip.D0_D = b.p08.RUNE_00;
  c.chip.D1_D = b.p08.RYPU_01;
  c.chip.D2_D = b.p08.SULY_02;
  c.chip.D3_D = b.p08.SEZE_03;
  c.chip.D4_D = b.p08.RESY_04;
  c.chip.D5_D = b.p08.TAMU_05;
  c.chip.D6_D = b.p08.ROGY_06;
  c.chip.D7_D = b.p08.RYDA_07;

  if (b.p08.LYRA) {
    c.D0 = b.p08.TUTY_00;
    c.D1 = b.p08.SYWA_01;
    c.D2 = b.p08.SUGU_02;
    c.D3 = b.p08.TAWO_03;
    c.D4 = b.p08.TUTE_04;
    c.D5 = b.p08.SAJO_05;
    c.D6 = b.p08.TEMY_06;
    c.D7 = b.p08.ROPA_07;
  }

  if (b.p08.LAVO) {
    c.D0 = b.p08.RYMA_00;
    c.D1 = b.p08.RUVO_01;
    c.D2 = b.p08.RYKO_02;
    c.D3 = b.p08.TAVO_03;
    c.D4 = b.p08.TEPE_04;
    c.D5 = b.p08.SAFO_05;
    c.D6 = b.p08.SEVU_06;
    c.D7 = b.p08.TAJU_07;
  }
}
