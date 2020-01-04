#include "BusDump.h"
#include "TestGB.h"

using namespace Schematics;

void test_bus_dump(BusDump* dump_log, int len) {

  TestGB gb;
  gb.reset();

  // Preload the rom reads into the gameboy.
  for (int i = 0; i < len; i++) {
    BusDump dump = dump_log[i];
    if (dump.cpu_addr <= 0x7FFF) {
      gb.rom[dump.cpu_addr] = dump.bus_data;
    }
  }

  for (int i = 0; i < len; i++) {
    BusDump dump = dump_log[i];

    gb.bus_tri.set_addr(dump.cpu_addr);
    gb.bus_tri.set_data(dump.cpu_data);
    gb.sys_reg.CPU_RAW_RD = dump.cpu_rd;
    gb.sys_reg.CPU_RAW_WR = dump.cpu_wr;

    gb.sim_phase(8);

    int new_addr = gb.bus_tri.get_addr();
    int new_data = gb.bus_tri.get_data();

    if (new_addr != dump.bus_addr) printf("bus_addr mismatch\n");
    if (new_data != dump.bus_data) printf("bus_data mismatch\n");
  }

}





BusDump poweron_000_div[] = {
  {.phase=     4, .cpu_addr=0x0100, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0100, .bus_data=0x00, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    12, .cpu_addr=0x0101, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0101, .bus_data=0xc3, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    20, .cpu_addr=0x0102, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0102, .bus_data=0x50, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    28, .cpu_addr=0x0103, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0103, .bus_data=0x01, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    36, .cpu_addr=0x0104, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0104, .bus_data=0xce, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    44, .cpu_addr=0x0150, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0150, .bus_data=0xfa, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    52, .cpu_addr=0x0151, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0151, .bus_data=0x04, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    60, .cpu_addr=0x0152, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0152, .bus_data=0xff, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    68, .cpu_addr=0xff04, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0xff04, .bus_data=0xab, .bus_rd=1, .bus_wr=0, .bus_ack=1},
};
