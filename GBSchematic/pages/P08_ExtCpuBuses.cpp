#include "P08_ExtCpuBuses.h"
#include "../Schematics.h"
#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P08_ExtCpuBuses::tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc) {
  const P08_ExtCpuBuses& pa = ga.p08;
  const P08_ExtCpuBuses& pb = gb.p08;
  P08_ExtCpuBuses& pc = gc.p08;

  (void)ga;
  (void)pa;

  pc.TOVA = not(gb.T1nT2);
  pc.RYCA = not(gb.T1nT2);
  pc.MULE = not(gb.T1T2n);
  gc.NET01 = pb.TOVA;

  //----------
  // center right, generating the external read/write signals to the cart

  pc.SORE = not(gb.cpu.A15);

  // TEVY = not_vram
  pc.TEVY = or(gb.cpu.A13, gb.cpu.A14, pb.SORE); // BUG - pretty sure this is an OR and not an AND, the dots are on the opposite side compared to TOZA

  pc.TEXO = and(gb.cpu.FROM_CPU4, pb.TEVY);
  pc.LEVO = not(pb.TEXO);
  pc.LAGU = unk3(gb.cpu.CPU_RAW_RD, pb.LEVO, gb.cpu.FROM_CPU3);
  pc.LYWE = not(pb.LAGU);

  pc.MOCA = nor(pb.TEXO, gb.T1T2n);
  pc.MEXO = not(gb.CPU_RD_SYNC);
  pc.NEVY = or(pb.MEXO, pb.MOCA);
  pc.MOTY = or(pb.MOCA, pb.LYWE);
  pc.PUVA = or(pb.NEVY, gb.LUMA);
  pc.TYMU = nor(gb.LUMA, pb.MOTY);

  pc.UVER = nand(pb.PUVA, gb.NET01);
  pc.USUF = nor (pb.PUVA, gb.T1nT2);
  pc.UGAC = nand(pb.TYMU, gb.NET01);
  pc.URUN = nor (pb.TYMU, gb.T1nT2);

  gc.chip.WR_A = pb.UVER;
  gc.chip.WR_D = pb.USUF;
  gc.chip.RD_A = pb.UGAC;
  gc.chip.RD_D = pb.URUN;

  //----------
  // top center

  pc.TOLA = not(gb.cpu.A01);
  pc.LOXO = unk3(pb.MULE, pb.TEXO, gb.T1T2n);
  pc.LASY = not(pb.LOXO);
  pc.MATE = not(pb.LASY);

  gc.TOLA_A1n = pb.TOLA;

  //----------
  // left center, CS and A15 drivers

  pc.SOGY = not(gb.cpu.A14);
  pc.TUMA = and(gb.cpu.A13, pb.SOGY, gb.cpu.A15); // selects A000-BFFF, cart ram
  
  //pc.TYNU = unk3(gb.cpu.A15, gb.cpu.A14, pb.TUMA);
  
  // this is a guess, it selects addr < 0xC00 || cart_ram, which seems reasonable
  pc.TYNU = nor(nand(gb.cpu.A15, gb.cpu.A14), pb.TUMA);
  
  pc.TOZA = and(pb.TYNU, gb.ABUZ, gb.FEXXFFXXn);

  // TUTU is the "use bootrom" signal, so this is saying "if our address is
  // not in the high half and we're not running the bootrom, read from the
  // cart
  pc.SOBY_15 = nor(gb.cpu.A15, gb.TUTU);
  pc.TYHO = mux2(gb.DMA_A15, pb.TOZA, gb.LUMA); // polarity?

  gc.chip.CS_A   = pb.TYHO;

  //----------
  // Low half of the address pin output driver

  pc.ALOR_00 = latch_pos(pb.MATE, pc.ALOR_00, gb.cpu.A00);
  pc.APUR_01 = latch_pos(pb.MATE, pc.APUR_01, gb.cpu.A01);
  pc.ALYR_02 = latch_pos(pb.MATE, pc.ALYR_02, gb.cpu.A02);
  pc.ARET_03 = latch_pos(pb.MATE, pc.ARET_03, gb.cpu.A03);
  pc.AVYS_04 = latch_pos(pb.MATE, pc.AVYS_04, gb.cpu.A04);
  pc.ATEV_05 = latch_pos(pb.MATE, pc.ATEV_05, gb.cpu.A05);
  pc.AROS_06 = latch_pos(pb.MATE, pc.AROS_06, gb.cpu.A06);
  pc.ARYM_07 = latch_pos(pb.MATE, pc.ARYM_07, gb.cpu.A07);
  pc.LUNO_08 = latch_pos(pb.MATE, pc.LUNO_08, gb.cpu.A08);
  pc.LYSA_09 = latch_pos(pb.MATE, pc.LYSA_09, gb.cpu.A09);
  pc.PATE_10 = latch_pos(pb.MATE, pc.PATE_10, gb.cpu.A10);
  pc.LUMY_11 = latch_pos(pb.MATE, pc.LUMY_11, gb.cpu.A11);
  pc.LOBU_12 = latch_pos(pb.MATE, pc.LOBU_12, gb.cpu.A12);
  pc.LONU_13 = latch_pos(pb.MATE, pc.LONU_13, gb.cpu.A13);
  pc.NYRE_14 = latch_pos(pb.MATE, pc.NYRE_14, gb.cpu.A14);
  pc.SEPY_15 = nand(gb.ABUZ, pb.SOBY_15);

  pc.AMET_00 = mux2(gb.DMA_A00, pb.ALOR_00, gb.LUMA);
  pc.ATOL_01 = mux2(gb.DMA_A01, pb.APUR_01, gb.LUMA);
  pc.APOK_02 = mux2(gb.DMA_A02, pb.ALYR_02, gb.LUMA);
  pc.AMER_03 = mux2(gb.DMA_A03, pb.ARET_03, gb.LUMA);
  pc.ATEM_04 = mux2(gb.DMA_A04, pb.AVYS_04, gb.LUMA);
  pc.ATOV_05 = mux2(gb.DMA_A05, pb.ATEV_05, gb.LUMA);
  pc.ATYR_06 = mux2(gb.DMA_A06, pb.AROS_06, gb.LUMA);
  pc.ASUR_07 = mux2(gb.DMA_A07, pb.ARYM_07, gb.LUMA);
  pc.MANO_08 = mux2(gb.DMA_A08, pb.LUNO_08, gb.LUMA);
  pc.MASU_09 = mux2(gb.DMA_A09, pb.LYSA_09, gb.LUMA);
  pc.PAMY_10 = mux2(gb.DMA_A10, pb.PATE_10, gb.LUMA);
  pc.MALE_11 = mux2(gb.DMA_A11, pb.LUMY_11, gb.LUMA);
  pc.MOJY_12 = mux2(gb.DMA_A12, pb.LOBU_12, gb.LUMA);
  pc.MUCE_13 = mux2(gb.DMA_A13, pb.LONU_13, gb.LUMA);
  pc.PEGE_14 = mux2(gb.DMA_A14, pb.NYRE_14, gb.LUMA);
  pc.TAZY_15 = mux2(gb.DMA_A15, pb.SEPY_15, gb.LUMA);

  pc.KUPO_00 = nand(pb.AMET_00, pb.TOVA);
  pc.CABA_01 = nand(pb.ATOL_01, pb.TOVA);
  pc.BOKU_02 = nand(pb.APOK_02, pb.TOVA);
  pc.BOTY_03 = nand(pb.AMER_03, pb.TOVA);
  pc.BYLA_04 = nand(pb.ATEM_04, pb.TOVA);
  pc.BADU_05 = nand(pb.ATOV_05, pb.TOVA);
  pc.CEPU_06 = nand(pb.ATYR_06, pb.TOVA);
  pc.DEFY_07 = nand(pb.ASUR_07, pb.TOVA);
  pc.MYNY_08 = nand(pb.MANO_08, pb.TOVA);
  pc.MUNE_09 = nand(pb.MASU_09, pb.TOVA);
  pc.ROXU_10 = nand(pb.PAMY_10, pb.TOVA);
  pc.LEPY_11 = nand(pb.MALE_11, pb.TOVA);
  pc.LUCE_12 = nand(pb.MOJY_12, pb.TOVA);
  pc.LABE_13 = nand(pb.MUCE_13, pb.TOVA);
  pc.PUHE_14 = nand(pb.PEGE_14, pb.TOVA);
  pc.SUZE_15 = nand(pb.TAZY_15, pb.RYCA);

  pc.KOTY_00 = nor(pb.AMET_00, gb.T1nT2);
  pc.COTU_01 = nor(pb.ATOL_01, gb.T1nT2);
  pc.BAJO_02 = nor(pb.APOK_02, gb.T1nT2);
  pc.BOLA_03 = nor(pb.AMER_03, gb.T1nT2);
  pc.BEVO_04 = nor(pb.ATEM_04, gb.T1nT2);
  pc.AJAV_05 = nor(pb.ATOV_05, gb.T1nT2);
  pc.CYKA_06 = nor(pb.ATYR_06, gb.T1nT2);
  pc.COLO_07 = nor(pb.ASUR_07, gb.T1nT2);
  pc.MEGO_08 = nor(pb.MANO_08, gb.T1nT2);
  pc.MENY_09 = nor(pb.MASU_09, gb.T1nT2);
  pc.RORE_10 = nor(pb.PAMY_10, gb.T1nT2);
  pc.LYNY_11 = nor(pb.MALE_11, gb.T1nT2);
  pc.LOSO_12 = nor(pb.MOJY_12, gb.T1nT2);
  pc.LEVA_13 = nor(pb.MUCE_13, gb.T1nT2);
  pc.PAHY_14 = nor(pb.PEGE_14, gb.T1nT2);
  pc.RULO_15 = nor(pb.TAZY_15, gb.T1nT2);

  gc.chip.A00_A = pb.KUPO_00;
  gc.chip.A01_A = pb.CABA_01;
  gc.chip.A02_A = pb.BOKU_02;
  gc.chip.A03_A = pb.BOTY_03;
  gc.chip.A04_A = pb.BYLA_04;
  gc.chip.A05_A = pb.BADU_05;
  gc.chip.A06_A = pb.CEPU_06;
  gc.chip.A07_A = pb.DEFY_07;
  gc.chip.A08_A = pb.MYNY_08;
  gc.chip.A09_A = pb.MUNE_09;
  gc.chip.A10_A = pb.ROXU_10;
  gc.chip.A11_A = pb.LEPY_11;
  gc.chip.A12_A = pb.LUCE_12;
  gc.chip.A13_A = pb.LABE_13;
  gc.chip.A14_A = pb.PUHE_14;
  gc.chip.A15_A = pb.SUZE_15;

  gc.chip.A00_D = pb.KOTY_00;
  gc.chip.A01_D = pb.COTU_01;
  gc.chip.A02_D = pb.BAJO_02;
  gc.chip.A03_D = pb.BOLA_03;
  gc.chip.A04_D = pb.BEVO_04;
  gc.chip.A05_D = pb.AJAV_05;
  gc.chip.A06_D = pb.CYKA_06;
  gc.chip.A07_D = pb.COLO_07;
  gc.chip.A08_D = pb.MEGO_08;
  gc.chip.A09_D = pb.MENY_09;
  gc.chip.A10_D = pb.RORE_10;
  gc.chip.A11_D = pb.LYNY_11;
  gc.chip.A12_D = pb.LOSO_12;
  gc.chip.A13_D = pb.LEVA_13;
  gc.chip.A14_D = pb.PAHY_14;
  gc.chip.A15_D = pb.RULO_15;

  //----------
  // Chip data pin output driver. Some of this was on P25, but it should _not_ be there.

  pc.REDU = not(gb.CPU_RD);
  pc.RORU = mux2(pb.REDU, pb.MOTY, gb.T1nT2);
  pc.LULA = not(pb.RORU);
  pc.LYRA = nand(gb.T1nT2, pb.RORU);
  pc.LAVO = nand(gb.cpu.CPU_RAW_RD, pb.TEXO, gb.cpu.FROM_CPU5);

  pc.RUNE_00 = nor(pb.RORU, gb.cpu.D0);
  pc.RYPU_01 = nor(pb.RORU, gb.cpu.D1);
  pc.SULY_02 = nor(pb.RORU, gb.cpu.D2);
  pc.SEZE_03 = nor(pb.RORU, gb.cpu.D3);
  pc.RESY_04 = nor(pb.RORU, gb.cpu.D4);
  pc.TAMU_05 = nor(pb.RORU, gb.cpu.D5);
  pc.ROGY_06 = nor(pb.RORU, gb.cpu.D6);
  pc.RYDA_07 = nor(pb.RORU, gb.cpu.D7);

  pc.RUXA_00 = nand(gb.cpu.D0, pb.LULA);
  pc.RUJA_01 = nand(gb.cpu.D1, pb.LULA);
  pc.RABY_02 = nand(gb.cpu.D2, pb.LULA);
  pc.RERA_03 = nand(gb.cpu.D3, pb.LULA);
  pc.RORY_04 = nand(gb.cpu.D4, pb.LULA);
  pc.RYVO_05 = nand(gb.cpu.D5, pb.LULA);
  pc.RAFY_06 = nand(gb.cpu.D6, pb.LULA);
  pc.RAVU_07 = nand(gb.cpu.D7, pb.LULA);

  gc.chip.D0_A = pb.RUXA_00;
  gc.chip.D1_A = pb.RUJA_01;
  gc.chip.D2_A = pb.RABY_02;
  gc.chip.D3_A = pb.RERA_03;
  gc.chip.D4_A = pb.RORY_04;
  gc.chip.D5_A = pb.RYVO_05;
  gc.chip.D6_A = pb.RAFY_06;
  gc.chip.D7_A = pb.RAVU_07;

  gc.chip.D0_B = pb.LULA;
  gc.chip.D1_B = pb.LULA;
  gc.chip.D2_B = pb.LULA;
  gc.chip.D3_B = pb.LULA;
  gc.chip.D4_B = pb.LULA;
  gc.chip.D5_B = pb.LULA;
  gc.chip.D6_B = pb.LULA;
  gc.chip.D7_B = pb.LULA;

  gc.chip.D0_D = pb.RUNE_00;
  gc.chip.D1_D = pb.RYPU_01;
  gc.chip.D2_D = pb.SULY_02;
  gc.chip.D3_D = pb.SEZE_03;
  gc.chip.D4_D = pb.RESY_04;
  gc.chip.D5_D = pb.TAMU_05;
  gc.chip.D6_D = pb.ROGY_06;
  gc.chip.D7_D = pb.RYDA_07;

  //----------
  // Chip data pin input driver. Some of this was on P25, but it should _not_ be there.
  // Not sure why there's two of these

  pc.TOVO_00 = not(gb.chip.D0_C);
  pc.RUZY_01 = not(gb.chip.D1_C);
  pc.ROME_02 = not(gb.chip.D2_C);
  pc.SAZA_03 = not(gb.chip.D3_C);
  pc.TEHE_04 = not(gb.chip.D4_C);
  pc.RATU_05 = not(gb.chip.D5_C);
  pc.SOCA_06 = not(gb.chip.D6_C);
  pc.RYBA_07 = not(gb.chip.D7_C);

  pc.TUTY_00 = not(pb.TOVO_00);
  pc.SYWA_01 = not(pb.RUZY_01);
  pc.SUGU_02 = not(pb.ROME_02);
  pc.TAWO_03 = not(pb.SAZA_03);
  pc.TUTE_04 = not(pb.TEHE_04);
  pc.SAJO_05 = not(pb.RATU_05);
  pc.TEMY_06 = not(pb.SOCA_06);
  pc.ROPA_07 = not(pb.RYBA_07);

  if (pb.LYRA) {
    gc.cpu.D0 = pb.TUTY_00;
    gc.cpu.D1 = pb.SYWA_01;
    gc.cpu.D2 = pb.SUGU_02;
    gc.cpu.D3 = pb.TAWO_03;
    gc.cpu.D4 = pb.TUTE_04;
    gc.cpu.D5 = pb.SAJO_05;
    gc.cpu.D6 = pb.TEMY_06;
    gc.cpu.D7 = pb.ROPA_07;
  }

  pc.SOMA_00 = latch_pos(pb.LAVO, pb.SOMA_00, gb.chip.D0_C);
  pc.RONY_01 = latch_pos(pb.LAVO, pb.RONY_01, gb.chip.D1_C);
  pc.RAXY_02 = latch_pos(pb.LAVO, pb.RAXY_02, gb.chip.D2_C);
  pc.SELO_03 = latch_pos(pb.LAVO, pb.SELO_03, gb.chip.D3_C);
  pc.SODY_04 = latch_pos(pb.LAVO, pb.SODY_04, gb.chip.D4_C);
  pc.SAGO_05 = latch_pos(pb.LAVO, pb.SAGO_05, gb.chip.D5_C);
  pc.RUPA_06 = latch_pos(pb.LAVO, pb.RUPA_06, gb.chip.D6_C);
  pc.SAZY_07 = latch_pos(pb.LAVO, pb.SAZY_07, gb.chip.D7_C);

  pc.RYMA_00 = pb.SOMA_00;
  pc.RUVO_01 = pb.RONY_01;
  pc.RYKO_02 = pb.RAXY_02;
  pc.TAVO_03 = pb.SELO_03;
  pc.TEPE_04 = pb.SODY_04;
  pc.SAFO_05 = pb.SAGO_05;
  pc.SEVU_06 = pb.RUPA_06;
  pc.TAJU_07 = pb.SAZY_07;

  if (pb.LAVO) {
    gc.cpu.D0 = pb.RYMA_00;
    gc.cpu.D1 = pb.RUVO_01;
    gc.cpu.D2 = pb.RYKO_02;
    gc.cpu.D3 = pb.TAVO_03;
    gc.cpu.D4 = pb.TEPE_04;
    gc.cpu.D5 = pb.SAFO_05;
    gc.cpu.D6 = pb.SEVU_06;
    gc.cpu.D7 = pb.TAJU_07;
  }

  //----------
  // if NET01 high, drive external address bus onto internal address
  // probably not inverting...

  pc.KOVA_00 = not(gb.chip.A00_C); pc.KEJO_00 = pb.KOVA_00; 
  pc.CAMU_01 = not(gb.chip.A01_C); pc.BYXE_01 = pb.CAMU_01; 
  pc.BUXU_02 = not(gb.chip.A02_C); pc.AKAN_02 = pb.BUXU_02; 
  pc.BASE_03 = not(gb.chip.A03_C); pc.ANAR_03 = pb.BASE_03; 
  pc.AFEC_04 = not(gb.chip.A04_C); pc.AZUV_04 = pb.AFEC_04; 
  pc.ABUP_05 = not(gb.chip.A05_C); pc.AJOV_05 = pb.ABUP_05; 
  pc.CYGU_06 = not(gb.chip.A06_C); pc.BYNE_06 = pb.CYGU_06; 
  pc.COGO_07 = not(gb.chip.A07_C); pc.BYNA_07 = pb.COGO_07; 
  pc.MUJY_08 = not(gb.chip.A08_C); pc.LOFA_08 = pb.MUJY_08; 
  pc.NENA_09 = not(gb.chip.A09_C); pc.MAPU_09 = pb.NENA_09; 
  pc.SURA_10 = not(gb.chip.A10_C); pc.RALA_10 = pb.SURA_10; 
  pc.MADY_11 = not(gb.chip.A11_C); pc.LORA_11 = pb.MADY_11; 
  pc.LAHE_12 = not(gb.chip.A12_C); pc.LYNA_12 = pb.LAHE_12; 
  pc.LURA_13 = not(gb.chip.A13_C); pc.LEFY_13 = pb.LURA_13; 
  pc.PEVO_14 = not(gb.chip.A14_C); pc.NEFE_14 = pb.PEVO_14; 
  pc.RAZA_15 = not(gb.chip.A15_C); pc.SYZU_15 = pb.RAZA_15;

  // wat
  //gc.chip.A15_C  = pb.RAZA_15;


  // these may be backwards, probably don't want to drive external address onto bus normally...

#if 0
  if (gb.NET01) {
    gc.cpu.A00 = pb.KEJO_00;
    gc.cpu.A01 = pb.BYXE_01;
    gc.cpu.A02 = pb.AKAN_02;
    gc.cpu.A03 = pb.ANAR_03;
    gc.cpu.A04 = pb.AZUV_04;
    gc.cpu.A05 = pb.AJOV_05;
    gc.cpu.A06 = pb.BYNE_06;
    gc.cpu.A07 = pb.BYNA_07;
    gc.cpu.A08 = pb.LOFA_08;
    gc.cpu.A09 = pb.MAPU_09;
    gc.cpu.A10 = pb.RALA_10;
    gc.cpu.A11 = pb.LORA_11;
    gc.cpu.A12 = pb.LYNA_12;
    gc.cpu.A13 = pb.LEFY_13;
    gc.cpu.A14 = pb.NEFE_14;
  }                 

  if (pb.RYCA) {
    gc.cpu.A15 = pb.SYZU_15;
  }
#endif

}
