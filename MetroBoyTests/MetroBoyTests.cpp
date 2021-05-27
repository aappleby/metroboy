#include "CoreLib/File.h"
#include "CoreLib/Tests.h"
#include "CoreLib/Types.h"
#include "MetroBoyLib/MetroBoy.h"

#include <stdio.h>
#include <string>

void run_mooneye_acceptance();
void run_screenshot_tests();
void run_mealybug_tests();
int  run_microtests();
int  run_microtest(std::string name);
void test_codegen();

//------------------------------------------------------------------------------

#if 0
void MetroBoyApp::post() {
  double begin = timestamp();

  printf("\n");
  printf("---------- POST begin =====\001\n");

  int err = 0;

  printf("Reset\n");
  failures +=  run_microtest("micro_cpu/build/dmg/rst_0x00.gb");
  failures +=  run_microtest("micro_cpu/build/dmg/rst_0x08.gb");
  failures +=  run_microtest("micro_cpu/build/dmg/rst_0x10.gb");
  failures +=  run_microtest("micro_cpu/build/dmg/rst_0x18.gb");
  failures +=  run_microtest("micro_cpu/build/dmg/rst_0x20.gb");
  failures +=  run_microtest("micro_cpu/build/dmg/rst_0x28.gb");
  failures +=  run_microtest("micro_cpu/build/dmg/rst_0x30.gb");
  failures +=  run_microtest("micro_cpu/build/dmg/rst_0x38.gb");
  printf("\n");
  if (err) exit(1);

  double end = timestamp();
  printf("---------- POST done in %f seconds ----------\n", (end - begin));
  printf("\n");
}

  //load("roms/gb-test-roms/cpu_instrs/cpu_instrs");
  //load("roms/gb-test-roms/instr_timing/instr_timing");
  //load("roms/gb-test-roms/cpu_instrs/individual", "08-misc instrs");
  //load("roms/gb-test-roms/cpu_instrs/individual", "11-op a,(hl)");

  //load("roms/tetris"); // tetris brokennnn

  /*pass*/ //load("roms/mooneye-gb/tests/build/acceptance", "oam_dma/basic");
  /*pass*/ //load("roms/mooneye-gb/tests/build/acceptance", "oam_dma/reg_read");
  /*pass*/ //load("roms/mooneye-gb/tests/build/acceptance", "oam_dma/sources-dmgABCmgbS");
  /*pass*/ //load("roms/mooneye-gb/tests/build/acceptance", "oam_dma_restart");
  /*pass*/ //load("roms/mooneye-gb/tests/build/acceptance", "oam_dma_start");
  /*pass*/ //load("roms/mooneye-gb/tests/build/acceptance", "oam_dma_timing");

  //load("micro_cpu/build/dmg", "cpu_zeropage");

  //load("microtests/build/dmg", "dma_basic");
  //load("microtests/build/dmg", "dma_0x8000");
  //load("microtests/build/dmg", "dma_0xA000");
  //load("microtests/build/dmg", "dma_0xE000");
  //load("microtests/build/dmg", "dma_0xFF00");

  //load("microtests/build/dmg", "poweron_006_oam");

#endif

//------------------------------------------------------------------------------

int run_microtest(std::string filename) {
  std::string path = "microtests/build/dmg/" + filename;

  blob rom = load_blob(path);
  MetroBoy gb;
  gb.reset_to_cart(rom.data(), rom.size());

  int timeout = 8000; // All our "fast" microtests take under 4000 phases
  int phase = 0;
  for (; phase < timeout; phase++) {
    gb.next_phase();
    if (gb.vram.ram[0]) break;
  }

  uint8_t result_a = gb.zram.ram[0]; // actual
  uint8_t result_b = gb.zram.ram[1]; // expected
  uint8_t result_c = gb.zram.ram[2]; // sanity (should be 0x31)

  if (phase >= timeout) {
    LOG_B("%-30s ", filename.c_str());
    LOG_Y("TIMEOUT\n");
    return 1;
  }
  else if (result_c != 0x31) {
    LOG_B("%-30s ", filename.c_str());
    LOG_Y("0x%02x 0x%02x 0x%02x ERROR @ %d\n", result_a, result_b, result_c, phase);
    return 1;
  }
  else if (result_a == result_b) {
    LOG_B("%-30s ", filename.c_str());
    LOG_G("0x%02x 0x%02x 0x%02x PASS @ %d\n", result_a, result_b, result_c, phase);
    return 0;
  }
  else {
    LOG_B("%-30s ", filename.c_str());
    LOG_R("0x%02x 0x%02x 0x%02x FAIL @ %d\n", result_a, result_b, result_c, phase);
    return 1;
  }
}

//------------------------------------------------------------------------------

int run_microtests() {
  TEST_START();

  double begin = timestamp();

#if 0
  failures +=  run_microtest("000-oam_lock.gb");
  failures +=  run_microtest("000-write_to_x8000.gb");
  failures +=  run_microtest("001-vram_unlocked.gb");
  failures +=  run_microtest("002-vram_locked.gb");
  failures +=  run_microtest("004-tima_boot_phase.gb");
  failures +=  run_microtest("004-tima_cycle_timer.gb");
  failures +=  run_microtest("007-lcd_on_stat.gb");
  failures +=  run_microtest("400-dma.gb");
  failures +=  run_microtest("500-scx-timing.gb");
  failures +=  run_microtest("800-ppu-latch-scx.gb");
  failures +=  run_microtest("801-ppu-latch-scy.gb");
  failures +=  run_microtest("802-ppu-latch-tileselect.gb");
  failures +=  run_microtest("803-ppu-latch-bgdisplay.gb");
  failures +=  run_microtest("cpu_bus_1.gb");
  failures +=  run_microtest("dma_0x1000.gb");
  failures +=  run_microtest("dma_0x9000.gb");
  failures +=  run_microtest("dma_0xA000.gb");
  failures +=  run_microtest("dma_0xE000.gb");
  failures +=  run_microtest("dma_basic.gb");
  failures +=  run_microtest("dma_timing_a.gb");
  failures +=  run_microtest("flood_vram.gb");
  failures +=  run_microtest("halt_op_dupe.gb");
  failures +=  run_microtest("halt_op_dupe_delay.gb");
#endif

#if 0
  LOG_Y("Timer tests\n");
  failures +=  run_microtest("div_inc_timing_a.gb");
  failures +=  run_microtest("div_inc_timing_b.gb");
  failures +=  run_microtest("timer_div_phase_c.gb");
  failures +=  run_microtest("timer_div_phase_d.gb");

  failures +=  run_microtest("timer_int_halt_a.gb");
  failures +=  run_microtest("timer_int_halt_b.gb");

  failures +=  run_microtest("timer_int_inc_sled.gb");
  failures +=  run_microtest("timer_int_inc_sled_a.gb");
  failures +=  run_microtest("timer_int_inc_sled_b.gb");
  failures +=  run_microtest("timer_tima_inc_256k_a.gb");
  failures +=  run_microtest("timer_tima_inc_256k_b.gb");
  failures +=  run_microtest("timer_tima_inc_256k_c.gb");
  failures +=  run_microtest("timer_tima_inc_256k_d.gb");
  failures +=  run_microtest("timer_tima_inc_256k_e.gb");
  failures +=  run_microtest("timer_tima_inc_256k_f.gb");
  failures +=  run_microtest("timer_tima_inc_256k_g.gb");
  failures +=  run_microtest("timer_tima_inc_256k_h.gb");
  failures +=  run_microtest("timer_tima_inc_256k_i.gb");
  failures +=  run_microtest("timer_tima_inc_256k_j.gb");
  failures +=  run_microtest("timer_tima_inc_256k_k.gb");
  failures +=  run_microtest("timer_tima_inc_64k_a.gb");
  failures +=  run_microtest("timer_tima_inc_64k_b.gb");
  failures +=  run_microtest("timer_tima_inc_64k_c.gb");
  failures +=  run_microtest("timer_tima_inc_64k_d.gb");
  failures +=  run_microtest("timer_tima_phase_a.gb");
  failures +=  run_microtest("timer_tima_phase_b.gb");
  failures +=  run_microtest("timer_tima_phase_c.gb");
  failures +=  run_microtest("timer_tima_phase_d.gb");
  failures +=  run_microtest("timer_tima_phase_e.gb");
  failures +=  run_microtest("timer_tima_phase_f.gb");
  failures +=  run_microtest("timer_tima_phase_g.gb");
  failures +=  run_microtest("timer_tima_phase_h.gb");
  failures +=  run_microtest("timer_tima_phase_i.gb");
  failures +=  run_microtest("timer_tima_phase_j.gb");
  failures +=  run_microtest("timer_tima_reload_256k_a.gb");
  failures +=  run_microtest("timer_tima_reload_256k_b.gb");
  failures +=  run_microtest("timer_tima_reload_256k_c.gb");
  failures +=  run_microtest("timer_tima_reload_256k_d.gb");
  failures +=  run_microtest("timer_tima_reload_256k_e.gb");
  failures +=  run_microtest("timer_tima_reload_256k_f.gb");
  failures +=  run_microtest("timer_tima_reload_256k_g.gb");
  failures +=  run_microtest("timer_tima_reload_256k_h.gb");
  failures +=  run_microtest("timer_tima_reload_256k_i.gb");
  failures +=  run_microtest("timer_tima_reload_256k_j.gb");
  failures +=  run_microtest("timer_tima_reload_256k_k.gb");
  failures +=  run_microtest("timer_tima_write_a.gb");
  failures +=  run_microtest("timer_tima_write_b.gb");
  failures +=  run_microtest("timer_tima_write_c.gb");
  failures +=  run_microtest("timer_tima_write_d.gb");
  failures +=  run_microtest("timer_tima_write_e.gb");
  failures +=  run_microtest("timer_tima_write_f.gb");
  failures +=  run_microtest("timer_tma_load_a.gb");
  failures +=  run_microtest("timer_tma_load_b.gb");
  failures +=  run_microtest("timer_tma_load_c.gb");
  failures +=  run_microtest("timer_tma_write_a.gb");
  failures +=  run_microtest("timer_tma_write_b.gb");
#endif

#if 1
  LOG_Y("HBlank int tests\n");
  failures +=  run_microtest("hblank_int_di_timing_a.gb");
  failures +=  run_microtest("hblank_int_di_timing_b.gb");
  failures +=  run_microtest("hblank_int_if_a.gb");
  failures +=  run_microtest("hblank_int_if_b.gb");
  failures +=  run_microtest("hblank_int_l0.gb");
  failures +=  run_microtest("hblank_int_l1.gb");
  failures +=  run_microtest("hblank_int_l2.gb");
  failures +=  run_microtest("hblank_int_scx0.gb");
  failures +=  run_microtest("hblank_int_scx0_if_a.gb");
  failures +=  run_microtest("hblank_int_scx0_if_b.gb");
  failures +=  run_microtest("hblank_int_scx0_if_c.gb");
  failures +=  run_microtest("hblank_int_scx0_if_d.gb");
  failures +=  run_microtest("hblank_int_scx1.gb");
  failures +=  run_microtest("hblank_int_scx1_if_a.gb");
  failures +=  run_microtest("hblank_int_scx1_if_b.gb");
  failures +=  run_microtest("hblank_int_scx1_if_c.gb");
  failures +=  run_microtest("hblank_int_scx1_if_d.gb");
  failures +=  run_microtest("hblank_int_scx1_nops_a.gb");
  failures +=  run_microtest("hblank_int_scx1_nops_b.gb");
  failures +=  run_microtest("hblank_int_scx2.gb");
  failures +=  run_microtest("hblank_int_scx2_if_a.gb");
  failures +=  run_microtest("hblank_int_scx2_if_b.gb");
  failures +=  run_microtest("hblank_int_scx2_if_c.gb");
  failures +=  run_microtest("hblank_int_scx2_if_d.gb");
  failures +=  run_microtest("hblank_int_scx2_nops_a.gb");
  failures +=  run_microtest("hblank_int_scx2_nops_b.gb");
  failures +=  run_microtest("hblank_int_scx3.gb");
  failures +=  run_microtest("hblank_int_scx3_if_a.gb");
  failures +=  run_microtest("hblank_int_scx3_if_b.gb");
  failures +=  run_microtest("hblank_int_scx3_if_c.gb");
  failures +=  run_microtest("hblank_int_scx3_if_d.gb");
  failures +=  run_microtest("hblank_int_scx3_nops_a.gb");
  failures +=  run_microtest("hblank_int_scx3_nops_b.gb");
  failures +=  run_microtest("hblank_int_scx4.gb");
  failures +=  run_microtest("hblank_int_scx4_if_a.gb");
  failures +=  run_microtest("hblank_int_scx4_if_b.gb");
  failures +=  run_microtest("hblank_int_scx4_if_c.gb");
  failures +=  run_microtest("hblank_int_scx4_if_d.gb");
  failures +=  run_microtest("hblank_int_scx4_nops_a.gb");
  failures +=  run_microtest("hblank_int_scx4_nops_b.gb");
  failures +=  run_microtest("hblank_int_scx5.gb");
  failures +=  run_microtest("hblank_int_scx5_if_a.gb");
  failures +=  run_microtest("hblank_int_scx5_if_b.gb");
  failures +=  run_microtest("hblank_int_scx5_if_c.gb");
  failures +=  run_microtest("hblank_int_scx5_if_d.gb");
  failures +=  run_microtest("hblank_int_scx5_nops_a.gb");
  failures +=  run_microtest("hblank_int_scx5_nops_b.gb");
  failures +=  run_microtest("hblank_int_scx6.gb");
  failures +=  run_microtest("hblank_int_scx6_if_a.gb");
  failures +=  run_microtest("hblank_int_scx6_if_b.gb");
  failures +=  run_microtest("hblank_int_scx6_if_c.gb");
  failures +=  run_microtest("hblank_int_scx6_if_d.gb");
  failures +=  run_microtest("hblank_int_scx6_nops_a.gb");
  failures +=  run_microtest("hblank_int_scx6_nops_b.gb");
  failures +=  run_microtest("hblank_int_scx7.gb");
  failures +=  run_microtest("hblank_int_scx7_if_a.gb");
  failures +=  run_microtest("hblank_int_scx7_if_b.gb");
  failures +=  run_microtest("hblank_int_scx7_if_c.gb");
  failures +=  run_microtest("hblank_int_scx7_if_d.gb");
  failures +=  run_microtest("hblank_int_scx7_nops_a.gb");
  failures +=  run_microtest("hblank_int_scx7_nops_b.gb");
  failures +=  run_microtest("hblank_scx2_if_a.gb");
  failures +=  run_microtest("hblank_scx3_if_a.gb");
  failures +=  run_microtest("hblank_scx3_if_b.gb");
  failures +=  run_microtest("hblank_scx3_if_c.gb");
  failures +=  run_microtest("hblank_scx3_if_d.gb");
  failures +=  run_microtest("hblank_scx3_int_a.gb");
  failures +=  run_microtest("hblank_scx3_int_b.gb");

  failures +=  run_microtest("int_hblank_halt_bug_a.gb");
  failures +=  run_microtest("int_hblank_halt_bug_b.gb");
  failures +=  run_microtest("int_hblank_halt_scx0.gb");
  failures +=  run_microtest("int_hblank_halt_scx1.gb");
  failures +=  run_microtest("int_hblank_halt_scx2.gb");
  failures +=  run_microtest("int_hblank_halt_scx3.gb");
  failures +=  run_microtest("int_hblank_halt_scx4.gb");
  failures +=  run_microtest("int_hblank_halt_scx5.gb");
  failures +=  run_microtest("int_hblank_halt_scx6.gb");
  failures +=  run_microtest("int_hblank_halt_scx7.gb");
  failures +=  run_microtest("int_hblank_incs_scx0.gb");
  failures +=  run_microtest("int_hblank_incs_scx1.gb");
  failures +=  run_microtest("int_hblank_incs_scx2.gb");
  failures +=  run_microtest("int_hblank_incs_scx3.gb");
  failures +=  run_microtest("int_hblank_incs_scx4.gb");
  failures +=  run_microtest("int_hblank_incs_scx5.gb");
  failures +=  run_microtest("int_hblank_incs_scx6.gb");
  failures +=  run_microtest("int_hblank_incs_scx7.gb");
#endif

#if 0
  failures +=  run_microtest("is_if_set_during_ime0.gb");
  failures +=  run_microtest("lcdon_halt_to_vblank_int_a.gb");
  failures +=  run_microtest("lcdon_halt_to_vblank_int_b.gb");
  failures +=  run_microtest("lcdon_nops_to_vblank_int_a.gb");
  failures +=  run_microtest("lcdon_nops_to_vblank_int_b.gb");
  failures +=  run_microtest("lcdon_to_if_oam_a.gb");
  failures +=  run_microtest("lcdon_to_if_oam_b.gb");
  failures +=  run_microtest("lcdon_to_ly1_a.gb");
  failures +=  run_microtest("lcdon_to_ly1_b.gb");
  failures +=  run_microtest("lcdon_to_ly2_a.gb");
  failures +=  run_microtest("lcdon_to_ly2_b.gb");
  failures +=  run_microtest("lcdon_to_ly3_a.gb");
  failures +=  run_microtest("lcdon_to_ly3_b.gb");
  failures +=  run_microtest("lcdon_to_lyc1_int.gb");
  failures +=  run_microtest("lcdon_to_lyc2_int.gb");
  failures +=  run_microtest("lcdon_to_lyc3_int.gb");
  failures +=  run_microtest("lcdon_to_oam_int_l0.gb");
  failures +=  run_microtest("lcdon_to_oam_int_l1.gb");
  failures +=  run_microtest("lcdon_to_oam_int_l2.gb");
  failures +=  run_microtest("lcdon_to_oam_unlock_a.gb");
  failures +=  run_microtest("lcdon_to_oam_unlock_b.gb");
  failures +=  run_microtest("lcdon_to_oam_unlock_c.gb");
  failures +=  run_microtest("lcdon_to_oam_unlock_d.gb");
  failures +=  run_microtest("lcdon_to_stat0_a.gb");
  failures +=  run_microtest("lcdon_to_stat0_b.gb");
  failures +=  run_microtest("lcdon_to_stat0_c.gb");
  failures +=  run_microtest("lcdon_to_stat0_d.gb");
  failures +=  run_microtest("lcdon_to_stat1_a.gb");
  failures +=  run_microtest("lcdon_to_stat1_b.gb");
  failures +=  run_microtest("lcdon_to_stat1_c.gb");
  failures +=  run_microtest("lcdon_to_stat1_d.gb");
  failures +=  run_microtest("lcdon_to_stat1_e.gb");
  failures +=  run_microtest("lcdon_to_stat2_a.gb");
  failures +=  run_microtest("lcdon_to_stat2_b.gb");
  failures +=  run_microtest("lcdon_to_stat2_c.gb");
  failures +=  run_microtest("lcdon_to_stat2_d.gb");
  failures +=  run_microtest("lcdon_to_stat3_a.gb");
  failures +=  run_microtest("lcdon_to_stat3_b.gb");
  failures +=  run_microtest("lcdon_to_stat3_c.gb");
  failures +=  run_microtest("lcdon_to_stat3_d.gb");
  failures +=  run_microtest("lcdon_write_timing.gb");
  failures +=  run_microtest("line_144_oam_int_a.gb");
  failures +=  run_microtest("line_144_oam_int_b.gb");
  failures +=  run_microtest("line_144_oam_int_c.gb");
  failures +=  run_microtest("line_144_oam_int_d.gb");
  failures +=  run_microtest("line_153_ly_a.gb");
  failures +=  run_microtest("line_153_ly_b.gb");
  failures +=  run_microtest("line_153_ly_c.gb");
  failures +=  run_microtest("line_153_ly_d.gb");
  failures +=  run_microtest("line_153_ly_e.gb");
  failures +=  run_microtest("line_153_ly_f.gb");
  failures +=  run_microtest("line_153_lyc0_int_inc_sled.gb");
  failures +=  run_microtest("line_153_lyc0_stat_timing_a.gb");
  failures +=  run_microtest("line_153_lyc0_stat_timing_b.gb");
  failures +=  run_microtest("line_153_lyc0_stat_timing_c.gb");
  failures +=  run_microtest("line_153_lyc0_stat_timing_d.gb");
  failures +=  run_microtest("line_153_lyc0_stat_timing_e.gb");
  failures +=  run_microtest("line_153_lyc0_stat_timing_f.gb");
  failures +=  run_microtest("line_153_lyc0_stat_timing_g.gb");
  failures +=  run_microtest("line_153_lyc0_stat_timing_h.gb");
  failures +=  run_microtest("line_153_lyc0_stat_timing_i.gb");
  failures +=  run_microtest("line_153_lyc0_stat_timing_j.gb");
  failures +=  run_microtest("line_153_lyc0_stat_timing_k.gb");
  failures +=  run_microtest("line_153_lyc0_stat_timing_l.gb");
  failures +=  run_microtest("line_153_lyc0_stat_timing_m.gb");
  failures +=  run_microtest("line_153_lyc0_stat_timing_n.gb");
  failures +=  run_microtest("line_153_lyc153_stat_timing_a.gb");
  failures +=  run_microtest("line_153_lyc153_stat_timing_b.gb");
  failures +=  run_microtest("line_153_lyc153_stat_timing_c.gb");
  failures +=  run_microtest("line_153_lyc153_stat_timing_d.gb");
  failures +=  run_microtest("line_153_lyc153_stat_timing_e.gb");
  failures +=  run_microtest("line_153_lyc153_stat_timing_f.gb");
  failures +=  run_microtest("line_153_lyc_a.gb");
  failures +=  run_microtest("line_153_lyc_b.gb");
  failures +=  run_microtest("line_153_lyc_c.gb");
  failures +=  run_microtest("line_153_lyc_int_a.gb");
  failures +=  run_microtest("line_153_lyc_int_b.gb");
  failures +=  run_microtest("line_65_ly.gb");
  failures +=  run_microtest("ly_while_lcd_off.gb");
  failures +=  run_microtest("lyc1_int_halt_a.gb");
  failures +=  run_microtest("lyc1_int_halt_b.gb");
  failures +=  run_microtest("lyc1_int_if_edge_a.gb");
  failures +=  run_microtest("lyc1_int_if_edge_b.gb");
  failures +=  run_microtest("lyc1_int_if_edge_c.gb");
  failures +=  run_microtest("lyc1_int_if_edge_d.gb");
  failures +=  run_microtest("lyc1_int_nops_a.gb");
  failures +=  run_microtest("lyc1_int_nops_b.gb");
  failures +=  run_microtest("lyc1_write_timing_a.gb");
  failures +=  run_microtest("lyc1_write_timing_b.gb");
  failures +=  run_microtest("lyc1_write_timing_c.gb");
  failures +=  run_microtest("lyc1_write_timing_d.gb");
  failures +=  run_microtest("lyc2_int_halt_a.gb");
  failures +=  run_microtest("lyc2_int_halt_b.gb");
  failures +=  run_microtest("minimal.gb");
  failures +=  run_microtest("mode2_stat_int_to_oam_unlock.gb");
  failures +=  run_microtest("oam_int_halt_a.gb");
  failures +=  run_microtest("oam_int_halt_b.gb");
  failures +=  run_microtest("oam_int_if_edge_a.gb");
  failures +=  run_microtest("oam_int_if_edge_b.gb");
  failures +=  run_microtest("oam_int_if_edge_c.gb");
  failures +=  run_microtest("oam_int_if_edge_d.gb");
  failures +=  run_microtest("oam_int_if_level_c.gb");
  failures +=  run_microtest("oam_int_if_level_d.gb");
  failures +=  run_microtest("oam_int_inc_sled.gb");
  failures +=  run_microtest("oam_int_nops_a.gb");
  failures +=  run_microtest("oam_int_nops_b.gb");
  failures +=  run_microtest("oam_read_l0_a.gb");
  failures +=  run_microtest("oam_read_l0_b.gb");
  failures +=  run_microtest("oam_read_l0_c.gb");
  failures +=  run_microtest("oam_read_l0_d.gb");
  failures +=  run_microtest("oam_read_l1_a.gb");
  failures +=  run_microtest("oam_read_l1_b.gb");
  failures +=  run_microtest("oam_read_l1_c.gb");
  failures +=  run_microtest("oam_read_l1_d.gb");
  failures +=  run_microtest("oam_read_l1_e.gb");
  failures +=  run_microtest("oam_read_l1_f.gb");
  failures +=  run_microtest("oam_sprite_trashing.gb");
  failures +=  run_microtest("oam_write_l0_a.gb");
  failures +=  run_microtest("oam_write_l0_b.gb");
  failures +=  run_microtest("oam_write_l0_c.gb");
  failures +=  run_microtest("oam_write_l0_d.gb");
  failures +=  run_microtest("oam_write_l0_e.gb");
  failures +=  run_microtest("oam_write_l1_a.gb");
  failures +=  run_microtest("oam_write_l1_b.gb");
  failures +=  run_microtest("oam_write_l1_c.gb");
  failures +=  run_microtest("oam_write_l1_d.gb");
  failures +=  run_microtest("oam_write_l1_e.gb");
  failures +=  run_microtest("oam_write_l1_f.gb");

  failures +=  run_microtest("poweron_000_oam.gb");
  failures +=  run_microtest("poweron_005_oam.gb");
  failures +=  run_microtest("poweron_006_oam.gb");
  failures +=  run_microtest("poweron_069_oam.gb");
  failures +=  run_microtest("poweron_070_oam.gb");
  failures +=  run_microtest("poweron_119_oam.gb");
  failures +=  run_microtest("poweron_120_oam.gb");
  failures +=  run_microtest("poweron_121_oam.gb");
  failures +=  run_microtest("poweron_183_oam.gb");
  failures +=  run_microtest("poweron_184_oam.gb");
  failures +=  run_microtest("poweron_233_oam.gb");
  failures +=  run_microtest("poweron_234_oam.gb");
  failures +=  run_microtest("poweron_235_oam.gb");

  failures +=  run_microtest("poweron_000_vram.gb");
  failures +=  run_microtest("poweron_026_vram.gb");
  failures +=  run_microtest("poweron_025_vram.gb");
  failures +=  run_microtest("poweron_069_vram.gb");
  failures +=  run_microtest("poweron_070_vram.gb");
  failures +=  run_microtest("poweron_139_vram.gb");
  failures +=  run_microtest("poweron_140_vram.gb");
  failures +=  run_microtest("poweron_183_vram.gb");
  failures +=  run_microtest("poweron_184_vram.gb");
#endif

#if 0
  LOG_Y("Power-on register value tests\n");
  failures +=  run_microtest("poweron_000_bgp.gb");
  failures +=  run_microtest("poweron_000_div.gb");
  failures +=  run_microtest("poweron_000_dma.gb");
  failures +=  run_microtest("poweron_000_if.gb");
  failures +=  run_microtest("poweron_000_joy.gb");
  failures +=  run_microtest("poweron_000_lcdc.gb");
  failures +=  run_microtest("poweron_000_ly.gb");
  failures +=  run_microtest("poweron_000_lyc.gb");
  failures +=  run_microtest("poweron_000_obp0.gb");
  failures +=  run_microtest("poweron_000_obp1.gb");
  failures +=  run_microtest("poweron_000_sb.gb");
  failures +=  run_microtest("poweron_000_sc.gb");
  failures +=  run_microtest("poweron_000_scx.gb");
  failures +=  run_microtest("poweron_000_scy.gb");
  failures +=  run_microtest("poweron_000_stat.gb");
  failures +=  run_microtest("poweron_000_tac.gb");
  failures +=  run_microtest("poweron_000_tima.gb");
  failures +=  run_microtest("poweron_000_tma.gb");
  failures +=  run_microtest("poweron_000_wx.gb");
  failures +=  run_microtest("poweron_000_wy.gb");
  failures +=  run_microtest("poweron_004_div.gb");
  failures +=  run_microtest("poweron_005_div.gb");
  failures +=  run_microtest("poweron_005_stat.gb");
  failures +=  run_microtest("poweron_006_stat.gb");
  failures +=  run_microtest("poweron_007_stat.gb");
  failures +=  run_microtest("poweron_026_stat.gb");
  failures +=  run_microtest("poweron_027_stat.gb");
  failures +=  run_microtest("poweron_069_stat.gb");
  failures +=  run_microtest("poweron_070_stat.gb");
  failures +=  run_microtest("poweron_119_ly.gb");
  failures +=  run_microtest("poweron_119_stat.gb");
  failures +=  run_microtest("poweron_120_ly.gb");
  failures +=  run_microtest("poweron_120_stat.gb");
  failures +=  run_microtest("poweron_121_stat.gb");
  failures +=  run_microtest("poweron_140_stat.gb");
  failures +=  run_microtest("poweron_141_stat.gb");
  failures +=  run_microtest("poweron_183_stat.gb");
  failures +=  run_microtest("poweron_184_stat.gb");
  failures +=  run_microtest("poweron_233_ly.gb");
  failures +=  run_microtest("poweron_234_ly.gb");
  failures +=  run_microtest("poweron_234_stat.gb");
  failures +=  run_microtest("poweron_235_stat.gb");
#endif

#if 0
  failures +=  run_microtest("ppu_scx_vs_bgp.gb");
  failures +=  run_microtest("ppu_sprite0_scx0_b.gb");
  failures +=  run_microtest("ppu_sprite0_scx1_a.gb");
  failures +=  run_microtest("ppu_sprite0_scx1_b.gb");
  failures +=  run_microtest("ppu_sprite0_scx2_a.gb");
  failures +=  run_microtest("ppu_sprite0_scx2_b.gb");
  failures +=  run_microtest("ppu_sprite0_scx3_a.gb");
  failures +=  run_microtest("ppu_sprite0_scx3_b.gb");
  failures +=  run_microtest("ppu_sprite0_scx4_a.gb");
  failures +=  run_microtest("ppu_sprite0_scx4_b.gb");
  failures +=  run_microtest("ppu_sprite0_scx5_a.gb");
  failures +=  run_microtest("ppu_sprite0_scx5_b.gb");
  failures +=  run_microtest("ppu_sprite0_scx6_a.gb");
  failures +=  run_microtest("ppu_sprite0_scx6_b.gb");
  failures +=  run_microtest("ppu_sprite0_scx7_a.gb");
  failures +=  run_microtest("ppu_sprite0_scx7_b.gb");
  failures +=  run_microtest("ppu_sprite_testbench.gb");
  failures +=  run_microtest("ppu_spritex_vs_scx.gb");
  failures +=  run_microtest("ppu_win_vs_wx.gb");
  failures +=  run_microtest("ppu_wx_early.gb");
  failures +=  run_microtest("sprite4_0_a.gb");
  failures +=  run_microtest("sprite4_0_b.gb");
  failures +=  run_microtest("sprite4_1_a.gb");
  failures +=  run_microtest("sprite4_1_b.gb");
  failures +=  run_microtest("sprite4_2_a.gb");
  failures +=  run_microtest("sprite4_2_b.gb");
  failures +=  run_microtest("sprite4_3_a.gb");
  failures +=  run_microtest("sprite4_3_b.gb");
  failures +=  run_microtest("sprite4_4_a.gb");
  failures +=  run_microtest("sprite4_4_b.gb");
  failures +=  run_microtest("sprite4_5_a.gb");
  failures +=  run_microtest("sprite4_5_b.gb");
  failures +=  run_microtest("sprite4_6_a.gb");
  failures +=  run_microtest("sprite4_6_b.gb");
  failures +=  run_microtest("sprite4_7_a.gb");
  failures +=  run_microtest("sprite4_7_b.gb");
  failures +=  run_microtest("sprite_0_a.gb");
  failures +=  run_microtest("sprite_0_b.gb");
  failures +=  run_microtest("sprite_1_a.gb");
  failures +=  run_microtest("sprite_1_b.gb");
  failures +=  run_microtest("stat_write_glitch_l0_a.gb");
  failures +=  run_microtest("stat_write_glitch_l0_b.gb");
  failures +=  run_microtest("stat_write_glitch_l0_c.gb");
  failures +=  run_microtest("stat_write_glitch_l143_a.gb");
  failures +=  run_microtest("stat_write_glitch_l143_b.gb");
  failures +=  run_microtest("stat_write_glitch_l143_c.gb");
  failures +=  run_microtest("stat_write_glitch_l143_d.gb");
  failures +=  run_microtest("stat_write_glitch_l154_a.gb");
  failures +=  run_microtest("stat_write_glitch_l154_b.gb");
  failures +=  run_microtest("stat_write_glitch_l154_c.gb");
  failures +=  run_microtest("stat_write_glitch_l154_d.gb");
  failures +=  run_microtest("stat_write_glitch_l1_a.gb");
  failures +=  run_microtest("stat_write_glitch_l1_b.gb");
  failures +=  run_microtest("stat_write_glitch_l1_c.gb");
  failures +=  run_microtest("stat_write_glitch_l1_d.gb");
#endif

#if 0
  failures +=  run_microtest("toggle_lcdc.gb");
#endif

#if 0 // slow
  LOG_Y("VBlank int tests\n");
  failures +=  run_microtest("vblank2_int_halt_a.gb");
  failures +=  run_microtest("vblank2_int_halt_b.gb");
  failures +=  run_microtest("vblank2_int_if_a.gb");
  failures +=  run_microtest("vblank2_int_if_b.gb");
  failures +=  run_microtest("vblank2_int_if_c.gb");
  failures +=  run_microtest("vblank2_int_if_d.gb");
  failures +=  run_microtest("vblank2_int_inc_sled.gb");
  failures +=  run_microtest("vblank2_int_nops_a.gb");
  failures +=  run_microtest("vblank2_int_nops_b.gb");
  failures +=  run_microtest("vblank_int_halt_a.gb");
  failures +=  run_microtest("vblank_int_halt_b.gb");
  failures +=  run_microtest("vblank_int_if_a.gb");
  failures +=  run_microtest("vblank_int_if_b.gb");
  failures +=  run_microtest("vblank_int_if_c.gb");
  failures +=  run_microtest("vblank_int_if_d.gb");
  failures +=  run_microtest("vblank_int_inc_sled.gb");
  failures +=  run_microtest("vblank_int_nops_a.gb");
  failures +=  run_microtest("vblank_int_nops_b.gb");
#endif

#if 0
  failures +=  run_microtest("vram_read_l0_a.gb");
  failures +=  run_microtest("vram_read_l0_b.gb");
  failures +=  run_microtest("vram_read_l0_c.gb");
  failures +=  run_microtest("vram_read_l0_d.gb");
  failures +=  run_microtest("vram_read_l1_a.gb");
  failures +=  run_microtest("vram_read_l1_b.gb");
  failures +=  run_microtest("vram_read_l1_c.gb");
  failures +=  run_microtest("vram_read_l1_d.gb");
  failures +=  run_microtest("vram_write_l0_a.gb");
  failures +=  run_microtest("vram_write_l0_b.gb");
  failures +=  run_microtest("vram_write_l0_c.gb");
  failures +=  run_microtest("vram_write_l0_d.gb");
  failures +=  run_microtest("vram_write_l1_a.gb");
  failures +=  run_microtest("vram_write_l1_b.gb");
  failures +=  run_microtest("vram_write_l1_c.gb");
  failures +=  run_microtest("vram_write_l1_d.gb");
#endif

  //failures +=  run_microtest("wave_write_to_0xC003.gb");

#if 0
  LOG_Y("Window tests\n");
  failures +=  run_microtest("win0_a.gb");
  failures +=  run_microtest("win0_b.gb");
  failures +=  run_microtest("win0_scx3_a.gb");
  failures +=  run_microtest("win0_scx3_b.gb");
  failures +=  run_microtest("win10_a.gb");
  failures +=  run_microtest("win10_b.gb");
  failures +=  run_microtest("win10_scx3_a.gb");
  failures +=  run_microtest("win10_scx3_b.gb");
  failures +=  run_microtest("win11_a.gb");
  failures +=  run_microtest("win11_b.gb");
  failures +=  run_microtest("win12_a.gb");
  failures +=  run_microtest("win12_b.gb");
  failures +=  run_microtest("win13_a.gb");
  failures +=  run_microtest("win13_b.gb");
  failures +=  run_microtest("win14_a.gb");
  failures +=  run_microtest("win14_b.gb");
  failures +=  run_microtest("win15_a.gb");
  failures +=  run_microtest("win15_b.gb");
  failures +=  run_microtest("win1_a.gb");
  failures +=  run_microtest("win1_b.gb");
  failures +=  run_microtest("win2_a.gb");
  failures +=  run_microtest("win2_b.gb");
  failures +=  run_microtest("win3_a.gb");
  failures +=  run_microtest("win3_b.gb");
  failures +=  run_microtest("win4_a.gb");
  failures +=  run_microtest("win4_b.gb");
  failures +=  run_microtest("win5_a.gb");
  failures +=  run_microtest("win5_b.gb");
  failures +=  run_microtest("win6_a.gb");
  failures +=  run_microtest("win6_b.gb");
  failures +=  run_microtest("win7_a.gb");
  failures +=  run_microtest("win7_b.gb");
  failures +=  run_microtest("win8_a.gb");
  failures +=  run_microtest("win8_b.gb");
  failures +=  run_microtest("win9_a.gb");
  failures +=  run_microtest("win9_b.gb");
#endif

  double end = timestamp();
  LOG_Y("---------- Microtests took %f seconds, %d failures ----------\n", (end - begin), failures);

  TEST_END();
}

//------------------------------------------------------------------------------

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  run_microtests();
  //run_screenshot_tests();
  //run_mooneye_acceptance();
  //run_mealybug_tests();
  //return 0;

  //---------


  return 0;
}

//------------------------------------------------------------------------------
