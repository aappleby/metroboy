#include "BusDump.h"
#include "Sch_Top.h"

using namespace Schematics;

#if 0

void test_bus_dump(BusDump* dump_log, int len) {

  SchematicTop gb;
  //gb.reset();

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
    gb.sys_reg.CPU_PIN_RDp = dump.cpu_rd;
    gb.sys_reg.CPU_PIN_WRp = dump.cpu_wr;

    //gb.sim_phase(8);

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

BusDump poweron_004_div[] = {
  {.phase=     0, .cpu_addr=0x0100, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0000, .bus_data=0xff, .bus_rd=0, .bus_wr=0, .bus_ack=0},
  {.phase=     2, .cpu_addr=0x0100, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0100, .bus_data=0x00, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=     4, .cpu_addr=0x0100, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0100, .bus_data=0x00, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=     6, .cpu_addr=0x0101, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0100, .bus_data=0x00, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=     8, .cpu_addr=0x0101, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0101, .bus_data=0xc3, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    10, .cpu_addr=0x0101, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0101, .bus_data=0xc3, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    12, .cpu_addr=0x0101, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0101, .bus_data=0xc3, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    14, .cpu_addr=0x0102, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0101, .bus_data=0xc3, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    16, .cpu_addr=0x0102, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0102, .bus_data=0x50, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    18, .cpu_addr=0x0102, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0102, .bus_data=0x50, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    20, .cpu_addr=0x0102, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0102, .bus_data=0x50, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    22, .cpu_addr=0x0103, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0102, .bus_data=0x50, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    24, .cpu_addr=0x0103, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0103, .bus_data=0x01, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    26, .cpu_addr=0x0103, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0103, .bus_data=0x01, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    28, .cpu_addr=0x0103, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0103, .bus_data=0x01, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    30, .cpu_addr=0x0104, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0103, .bus_data=0x01, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    32, .cpu_addr=0x0104, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0104, .bus_data=0xce, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    34, .cpu_addr=0x0104, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0104, .bus_data=0xce, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    36, .cpu_addr=0x0104, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0104, .bus_data=0xce, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    38, .cpu_addr=0x0150, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0104, .bus_data=0xce, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    40, .cpu_addr=0x0150, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0150, .bus_data=0x00, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    42, .cpu_addr=0x0150, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0150, .bus_data=0x00, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    44, .cpu_addr=0x0150, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0150, .bus_data=0x00, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    46, .cpu_addr=0x0151, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0150, .bus_data=0x00, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    48, .cpu_addr=0x0151, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0151, .bus_data=0x00, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    50, .cpu_addr=0x0151, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0151, .bus_data=0x00, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    52, .cpu_addr=0x0151, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0151, .bus_data=0x00, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    54, .cpu_addr=0x0152, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0151, .bus_data=0x00, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    56, .cpu_addr=0x0152, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0152, .bus_data=0x00, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    58, .cpu_addr=0x0152, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0152, .bus_data=0x00, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    60, .cpu_addr=0x0152, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0152, .bus_data=0x00, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    62, .cpu_addr=0x0153, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0152, .bus_data=0x00, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    64, .cpu_addr=0x0153, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0153, .bus_data=0x00, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    66, .cpu_addr=0x0153, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0153, .bus_data=0x00, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    68, .cpu_addr=0x0153, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0153, .bus_data=0x00, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    70, .cpu_addr=0x0154, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0153, .bus_data=0x00, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    72, .cpu_addr=0x0154, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0154, .bus_data=0xfa, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    74, .cpu_addr=0x0154, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0154, .bus_data=0xfa, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    76, .cpu_addr=0x0154, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0154, .bus_data=0xfa, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    78, .cpu_addr=0x0155, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0154, .bus_data=0xfa, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    80, .cpu_addr=0x0155, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0155, .bus_data=0x04, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    82, .cpu_addr=0x0155, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0155, .bus_data=0x04, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    84, .cpu_addr=0x0155, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0155, .bus_data=0x04, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    86, .cpu_addr=0x0156, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0155, .bus_data=0x04, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    88, .cpu_addr=0x0156, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0156, .bus_data=0xff, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    90, .cpu_addr=0x0156, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0156, .bus_data=0xff, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    92, .cpu_addr=0x0156, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0156, .bus_data=0xff, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    94, .cpu_addr=0xff04, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0x0156, .bus_data=0xff, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    96, .cpu_addr=0xff04, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0xff04, .bus_data=0xab, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=    98, .cpu_addr=0xff04, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0xff04, .bus_data=0xab, .bus_rd=1, .bus_wr=0, .bus_ack=1},
  {.phase=   100, .cpu_addr=0xff04, .cpu_data=0x00, .cpu_rd=1, .cpu_wr=0, .bus_addr=0xff04, .bus_data=0x00, .bus_rd=1, .bus_wr=0, .bus_ack=1},
};

void test_bus_dumps() {
  test_bus_dump(poweron_000_div, sizeof(poweron_000_div) / sizeof(poweron_000_div[0]));
}

#endif