#pragma once

struct Clocks {
};

extern Clocks clk;


struct Resets {
  bool RESET2;
  bool RESET7;
  bool RESET7n;
  bool RESET6;
  bool RESET8;
  bool RESET9;

  bool RESET_DIVn;

  bool RESET_VIDEO;
  bool RESET_VIDEOn;
  bool RESET_VIDEO2n;

  bool APU_RESET;
  bool APU_RESETn;
  bool APU_RESET2n;
  bool APU_RESET3n;
  bool APU_RESET4n;
  bool APU_RESET5n;
};

extern Resets rst;

extern bool CLKPIPE;

extern bool CLK1;
extern bool CLK2;
extern bool CLK3; // sprites use this clock

extern bool BOGA1MHZ;
extern bool CLKIN_An;

extern bool CERY_2MHZ;
extern bool BAVU_1MHZ;
extern bool JESO_512K;
extern bool HAMA_512Kn;

extern bool CLK_16k;
extern bool CLK_64k;
extern bool CLK_256k;

extern bool HORU_512;
extern bool BUFY_256;
extern bool BYFE_128;

extern bool ABOL_1MHZ;
extern bool AJER_2MHZ;
extern bool CYBO_4MHZ;

extern bool APUV_4MHZ;


extern bool AJER_2MHZn;
extern bool DYFA_1MHZ;
