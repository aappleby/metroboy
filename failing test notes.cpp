poweron_stat_006.gb
poweron_stat_120.gb
lcdon_to_stat1_d.gb
lcdon_to_stat2_a.gb
  // stat should read 84, but it reads 85
  // low bits of stat go from 101 -> 111 -> 110, so there _must_ be some sort of latch sampling the bus
  // stat low nibble goes 5-7-6, but it's supposed to read 4 - SADU cleared too late?
  // stat low nibble goes 4-6-2 but it's supposed to read 0 - RUPO cleared too late?
  // RUPO cleared too late?

poweron_oam_006.gb
poweron_oam_120.gb
poweron_oam_234.gb
lcdon_to_oam_unlock_d.gb
  // should be locked but wasn't - scan started late - oam locked the A after the read
  // so oam should be locked but its not?
  // hblank late
  // SIG_OAM_OEn goes back to 1 on E after the read starts, so we have to latch before that?
  // but the oam address doesn't show up until H?
  // sig_oam_clkn goes low EFGH during read



poweron_vram_026.gb
poweron_vram_140.gb
  // should be locked but wasn't - rendering started late?
  // Address is on the bus for B->A, XYMU goes low on A - should we be latching data on AB?




oam_read_l0_d.gb
  // address does show up on the oam bus for two phases HA but OEn is high

oam_read_l1_a.gb
  // oam OEn goes high on DE, CPU bus sees the value from D? interaction with oam latch?

oam_read_l1_f.gb
  // should be locked but wasn't - scan started late?
  // if it was a data latch issue reading stat, it wouldn't also affect oam read...?

// I should check that these oam tests are writing to 0xFE30 and not 0xFE00 or something

// does the OAM memory latch the read address?


// so we have a cpu data latch... we reset it on FG and GH
// but we're tocking the CPU on AB _before_ tock_gates
// so we need to send the CPU the new CPU bus on AB instead of the old?


oam_write_l0_d.gb
  // should be unlocked but wasn't - hblank started late?

oam_write_l1_c.gb
  // this is the scan/render gap

line_153_lyc0_int_inc_sled.gb
  // the isr seems to fire at a reasonable time, but it interrupts the very last inc
  // ly changes from 153 to 0 on BC, and then there are 8 phases before ROPO latches
  // it, so it's not like that could be later
  // so like the last inc executes anyway somehow?
  // what do the other inc sleds do?
  // oam_int_inc_sled -
  //   inc on AB, int on FG
  // vblank2_int_inc_sled -
  //   inc on AB, int on BC
  // vblank2_int_inc_sled -
  //   inc on AB, int on BC


ppu_sprite0_scx3_b.gb
  // read goes on the bus on AB
  // stat goes to 00 on the _next_ AB    SUGGESTS LATCH AT THE END OF AB
  // WODU goes high on HA
  // VOGA goes high on AB
  // the stat bits are driven off XYMU, which is driven off VOGA

ppu_sprite0_scx7_b.gb
  // pix goes to 167 on HA
  // same as above? looks like it.


int_hblank_halt_bug_b.gb
  // this feels like it's related to the inc sled failure
  // we should do another inc sled where the last instruction is a nop and see
  // if the inc is duplicated like in the halt bug


// haven't looked at these yet
stat_write_glitch_l0_a.gb
stat_write_glitch_l0_b.gb
stat_write_glitch_l1_b.gb
stat_write_glitch_l1_c.gb















//load_rom("tests/mooneye-gb/tests/build/acceptance/" "ppu/lcdon_write_timing-GS.gb"); // dmg pass, gateboy fail
//load_rom("tests/mooneye-gb/tests/build/acceptance/ppu/lcdon_timing-GS.gb"); // dmg pass, gateboy fail
//load_rom("tests/mooneye-gb/tests/build/acceptance/ppu/intr_2_mode0_timing_sprites.gb"); // dmg pass, gateboy fail
//load_rom("tests/gb-test-roms/cpu_instrs/cpu_instrs.gb"); // pass
//load_rom("tests/gb-test-roms/instr_timing/instr_timing.gb"); // pass
//load_rom("tests/gb-test-roms/mem_timing/mem_timing.gb"); // pass
//load_rom("tests/gb-test-roms/mem_timing-2/mem_timing.gb"); // pass
//load_rom("tests/gb-test-roms/halt_bug.gb"); // fail
//load_rom("tests/gb-test-roms/oam_bug/oam_bug.gb"); // fail
//load_rom("tests/scribbltests/lycscx/lycscx.gb"); // pass
//load_rom("tests/scribbltests/lycscy/lycscy.gb"); // pass
//load_rom("tests/scribbltests/palettely/palettely.gb");
//load_rom("tests/scribbltests/scxly/scxly.gb");
//load_rom("tests/scribbltests/statcount/statcount-auto.gb"); // pass

