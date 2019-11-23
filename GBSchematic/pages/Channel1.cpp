#include "Channel1.h"
#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P11_Ch1Regs_tick(const Gameboy& qa, const Gameboy& qb, Gameboy& qc) {

  const Channel1& pa = qa.ch1;
  const Channel1& pb = qb.ch1;
  Channel1& pc = qc.ch1;

  //----------
  // Inputs

  wire ZERO     = qb.chip.P10_B;
  wire RESETn   = qb.apu.APU_RESETn;
  wire RESETo   = qb.apu.APU_RESETn3;
  wire RESET1   = qb.apu.APU_RESET1;
  wire CLKREQn  = qb.sys.CPUCLK_REQn;
  wire aCLK_2M  = qa.sys.AJER_2M;
  wire aCLK_1M  = qa.sys.CPUCLK_xxxxEFGH9;
  wire aCLK_512 = qa.sys.CLK_512a;
  wire bCLK_2M  = qb.sys.AJER_2M;
  wire bCLK_1M  = qb.sys.CPUCLK_xxxxEFGH9;
  wire bCLK_512 = qb.sys.CLK_512a;
  wire bCLK_256 = qb.sys.CLK_256a;
  wire bCLK_128 = qb.sys.CLK_128a;

  bool CPU_RD   = qb.sys.CPU_RD;
  bool CPU_RDn  = qb.apu.CPU_RDn;
  wire APU_WR   = qb.apu.APU_WR;
  bool CPU_WR_WEIRD = qb.apu.CPU_WR_WEIRD;

  wire ADDR_FF10 = qb.apu.ADDR_FF10;
  wire ADDR_FF11 = qb.apu.ADDR_FF11;
  wire ADDR_FF12 = qb.apu.ADDR_FF12;
  wire ADDR_FF13 = qb.apu.ADDR_FF13;
  wire ADDR_FF14 = qb.apu.ADDR_FF14;

  wire NR41_LEN3b = qb.ch4.NR41_LEN3b; // wat?

  wire D0 = qb.D0;
  wire D1 = qb.D1;
  wire D2 = qb.D2;
  wire D3 = qb.D3;
  wire D4 = qb.D4;
  wire D5 = qb.D5;
  wire D6 = qb.D6;
  wire D7 = qb.D7;

  bool DBG_APU = qb.apu.DBG_APU;
  wire DBG_SWEEP = qb.apu.DBG_SWEEP;

  //----------
  // Outputs

  bool cD0 = false;
  bool cD1 = false;
  bool cD2 = false;
  bool cD3 = false;
  bool cD4 = false;
  bool cD5 = false;
  bool cD6 = false;
  bool cD7 = false;

  /*p01.BATA*/ wire aCLK_2Mn = not(aCLK_2M);
  /*p01.BATA*/ wire bCLK_2Mn = not(bCLK_2M);
  /*p11.CEPO*/ wire RESET1n = not(RESET1);
  /*p11.BUWA*/ wire CPU_RDa = not(CPU_RDn);
  /*p11.BUDA*/ wire CPU_RDo = not(CPU_RD);

  /*p11.BUZE*/ wire ADDR_FF10n = not(ADDR_FF10);
  /*p11.HAXE*/ wire ADDR_FF12n = not(ADDR_FF12);
  /*p11.GAGO*/ wire ADDR_FF12o = not(ADDR_FF12);
  /*p11.CACA*/ wire ADDR_FF13n = not(ADDR_FF13);
  /*p11.CURE*/ wire ADDR_FF14n = not(ADDR_FF14);

  //----------
  // FF10 NR10

  /*p11.BANY*/ pc.NR10_SHIFT0    = tock_pos(pa.FF10_WR, pb.FF10_WR, RESETn, pb.NR10_SHIFT0,    D0);
  /*p11.ARAX*/ pc.NR10_SHIFT1    = tock_pos(pa.FF10_WR, pb.FF10_WR, RESETn, pb.NR10_SHIFT1,    D1);
  /*p11.ANAZ*/ pc.NR10_SHIFT2    = tock_pos(pa.FF10_WR, pb.FF10_WR, RESETn, pb.NR10_SHIFT2,    D2);
  /*p11.AVAF*/ pc.NR10_SWEEP_DIR = tock_pos(pa.FF10_WR, pb.FF10_WR, RESETn, pb.NR10_SWEEP_DIR, D3);
  /*p11.ADEK*/ pc.NR10_DELAY0    = tock_pos(pa.FF10_WR, pb.FF10_WR, RESETn, pb.NR10_DELAY0,    D4);
  /*p11.BANA*/ pc.NR10_DELAY1    = tock_pos(pa.FF10_WR, pb.FF10_WR, RESETn, pb.NR10_DELAY1,    D5);
  /*p11.BOTU*/ pc.NR10_DELAY2    = tock_pos(pa.FF10_WR, pb.FF10_WR, RESETn, pb.NR10_DELAY2,    D6);
  /*p11.CENU*/   pc.FF10_WR = and(APU_WR, ADDR_FF10);

  /*p12.ARYL*/ pc.NR10_SWEEP_DIRn = not(pb.NR10_SWEEP_DIR);
  /*p13.BAVE*/ pc.NR10_NO_SWEEP   = and(pb.NR10_DELAY0, pb.NR10_DELAY1, pb.NR10_DELAY2); // polarity

  /*p11.AMYD*/ if (pb.FF10_RD) cD0 = pb.NR10_SHIFT0;
  /*p11.ATAX*/ if (pb.FF10_RD) cD1 = pb.NR10_SHIFT1;
  /*p11.AZYP*/ if (pb.FF10_RD) cD2 = pb.NR10_SHIFT2;
  /*p11.AFOX*/ if (pb.FF10_RD) cD3 = pb.NR10_SWEEP_DIR;
  /*p11.AVEK*/ if (pb.FF10_RD) cD4 = pb.NR10_DELAY0;
  /*p11.AKUX*/ if (pb.FF10_RD) cD5 = pb.NR10_DELAY1;
  /*p11.AWOS*/ if (pb.FF10_RD) cD6 = pb.NR10_DELAY2;
  /*p11.ASOP*/   pc.FF10_RD  = not(pb.FF10_RDn);
  /*p11.ATYN*/     pc.FF10_RDn = nor(CPU_RDn, ADDR_FF10n);

  //----------
  // FF11 NR11

  /*p11.CENA*/ pc.NR11_DUTY_0 = tock_pos(pa.FF11_WRn, pb.FF11_WRn, RESET1n, pb.NR11_DUTY_0, D6);
  /*p11.DYCA*/ pc.NR11_DUTY_1 = tock_pos(pa.FF11_WRn, pb.FF11_WRn, RESET1n, pb.NR11_DUTY_1, D7);
  /*p11.DAFO*/   pc.FF11_WRn = not(pb.FF11_WR);
  /*p11.COVU*/     pc.FF11_WR = and(APU_WR, ADDR_FF11);

  /*p11.BOWO*/ if (pb.FF11_RD) cD6 = pb.NR11_DUTY_0;
  /*p11.CUDA*/ if (pb.FF11_RD) cD7 = pb.NR11_DUTY_1;
  /*p11.BEXU*/   pc.FF11_RD = nor(CPU_RDa, ADDR_FF11); // polarity?

  //----------
  // FF12 NR12

  /*p11.JUSA*/ pc.NR12_DELAY0  = tock_pos(pa.FF12_WRo, pb.FF12_WRo, pb.FF12_RSTn, pb.NR12_DELAY0,  D0);
  /*p11.JUZY*/ pc.NR12_DELAY1  = tock_pos(pa.FF12_WRo, pb.FF12_WRo, pb.FF12_RSTn, pb.NR12_DELAY1,  D1);
  /*p11.JOMA*/ pc.NR12_DELAY2  = tock_pos(pa.FF12_WRo, pb.FF12_WRo, pb.FF12_RSTn, pb.NR12_DELAY2,  D2);
  /*p11.JAFY*/ pc.NR12_ENV_DIR = tock_pos(pa.FF12_WRn, pb.FF12_WRn, pb.FF12_RSTn, pb.NR12_ENV_DIR, D3);
  /*p11.JATY*/ pc.NR12_VOL0    = tock_pos(pa.FF12_WRn, pb.FF12_WRn, pb.FF12_RSTn, pb.NR12_VOL0,    D4);
  /*p11.JAXO*/ pc.NR12_VOL1    = tock_pos(pa.FF12_WRn, pb.FF12_WRn, pb.FF12_RSTn, pb.NR12_VOL1,    D5);
  /*p11.JENA*/ pc.NR12_VOL2    = tock_pos(pa.FF12_WRn, pb.FF12_WRn, pb.FF12_RSTn, pb.NR12_VOL2,    D6);
  /*p11.JOPU*/ pc.NR12_VOL3    = tock_pos(pa.FF12_WRn, pb.FF12_WRn, pb.FF12_RSTn, pb.NR12_VOL3,    D7);
  /*p11.KYGY*/   pc.FF12_WRo   = not(pb.FF12_WRa);
  /*p11.HAFU*/     pc.FF12_WRa   = and(APU_WR, ADDR_FF12);
  /*p11.GAXU*/   pc.FF12_WRn   = nand(APU_WR, ADDR_FF12);
  /*p11.HATO*/   pc.FF12_RSTn = not(RESET1);
  /*p11.KAGY*/   pc.FF12_WRb   = not(pb.FF12_WRn); // unused?

  /*p11.JYNE*/ if (pb.FF12_RDn) cD0 = pb.NR12_DELAY0; // polarity?
  /*p11.JACA*/ if (pb.FF12_RDn) cD1 = pb.NR12_DELAY1;
  /*p11.JOKU*/ if (pb.FF12_RDn) cD2 = pb.NR12_DELAY2;
  /*p11.HONO*/ if (pb.FF12_RDo) cD3 = pb.NR12_ENV_DIR;
  /*p11.HOWU*/ if (pb.FF12_RDo) cD4 = pb.NR12_VOL0;
  /*p11.HEWA*/ if (pb.FF12_RDo) cD5 = pb.NR12_VOL1;
  /*p11.HEVE*/ if (pb.FF12_RDo) cD6 = pb.NR12_VOL2;
  /*p11.JYSE*/ if (pb.FF12_RDo) cD7 = pb.NR12_VOL3;
  /*p11.HAMY*/   pc.FF12_RDn   = or(ADDR_FF12n, CPU_RDn);
  /*p11.HOCU*/   pc.FF12_RDo   = or(ADDR_FF12o, CPU_RDn);

  //----------
  // Frequency can be read from FF13/FF14 in debug mode

  /*p11.EVAJ*/ pc.FF13_RDa = not(pb.FF13_RDn);
  /*p11.DYPU*/   pc.FF13_RDn = nor(pb.CPU_RDnb, ADDR_FF13n);
  /*p11.DAXA*/     pc.CPU_RDnb = nand(pb.CPU_RDb, DBG_APU);
  /*p11.CEGE*/       pc.CPU_RDb  = not(CPU_RDn);

  /*p11.DUPY*/ pc.FF14_RDa = or(pb.CPU_RDnb, ADDR_FF14n); // polarity?

  /*p11.FORU*/ if (pb.FF13_RDa) cD0 = pb.FREQ_CNT_00;
  /*p11.GEFU*/ if (pb.FF13_RDa) cD1 = pb.FREQ_CNT_01;
  /*p11.KYVU*/ if (pb.FF13_RDa) cD2 = pb.FREQ_CNT_02;
  /*p11.KUMO*/ if (pb.FF13_RDa) cD3 = pb.FREQ_CNT_03;
  /*p11.KARY*/ if (pb.FF13_RDa) cD4 = pb.FREQ_CNT_04;
  /*p11.GODE*/ if (pb.FF13_RDa) cD5 = pb.FREQ_CNT_05;
  /*p11.GOJE*/ if (pb.FF13_RDa) cD6 = pb.FREQ_CNT_06;
  /*p11.FOZE*/ if (pb.FF13_RDa) cD7 = pb.FREQ_CNT_07;
  /*p11.DOPA*/ if (pb.FF14_RDa) cD0 = pb.FREQ_CNT_08;
  /*p11.DEMU*/ if (pb.FF14_RDa) cD1 = pb.FREQ_CNT_09;
  /*p11.DEXO*/ if (pb.FF14_RDa) cD2 = pb.FREQ_CNT_10;

  //----------
  // FF14 NR14

  /*p11.BOKO*/ pc.NR14_STOP = tock_pos(pa.FF14_WRn, pb.FF14_WRn, pb.NR14_STOP_RST, pc.NR14_STOP, D6);
  /*p11.BAGE*/   pc.FF14_WRn   = nand(CPU_WR_WEIRD, ADDR_FF14); 
  /*p11.CAMY*/   pc.NR14_STOP_RST = not(RESET1);

  /*p13.DUPE*/ pc.NR14_START = tock_pos(pa.FF14_WRnb, pb.FF14_WRnb, pb.NR14_START_RST, pb.NR14_START, D7);
  /*p13.DOGE*/   pc.FF14_WRnb = nand(APU_WR, ADDR_FF14);
  /*p13.DADO*/   pc.NR14_START_RST = nor(RESET1, pb.START);

  /*p11.BYTU*/ if (pb.FF14_RDb) cD6 = pb.NR14_STOP;
  /*p11.BALE*/   pc.FF14_RDb = nand(CPU_RDo, ADDR_FF14);

  //----------
  // Sweep timer

  /*p09.CATE*/ pc.SWEEP_DELAY_CLK = not(pb.SWEEP_CLKn);
  /*p09.CONE*/   pc.SWEEP_CLKn = not(pb.SWEEP_CLKb);
  /*p09.CELY*/     pc.SWEEP_CLKb = mux2(pb.DBG_SWEEP_CLK, bCLK_128, DBG_APU);
  /*p09.BAZA*/       pc.DBG_SWEEP_CLK = tock_pos(aCLK_2M, bCLK_2M, RESETo, pb.DBG_SWEEP_CLK, DBG_SWEEP);

  /*p13.CUPO*/ pc.SWEEP_DELAY0 = count_pos(pa.SWEEP_DELAY_CLK, pb.SWEEP_DELAY_CLK, pb.SWEEP_DELAY_RSTn, pb.SWEEP_DELAY0, pb.NR10_DELAY0);
  /*p13.CYPU*/ pc.SWEEP_DELAY1 = count_pos(pa.SWEEP_DELAY0,    pb.SWEEP_DELAY0,    pb.SWEEP_DELAY_RSTn, pb.SWEEP_DELAY1, pb.NR10_DELAY1);
  /*p13.CAXY*/ pc.SWEEP_DELAY2 = count_pos(pa.SWEEP_DELAY1,    pb.SWEEP_DELAY1,    pb.SWEEP_DELAY_RSTn, pb.SWEEP_DELAY2, pb.NR10_DELAY2);
  /*p13.CYMU*/   pc.SWEEP_DELAY_RSTn = not(pb.SWEEP_DELAY_RSTb);
  /*p13.DAFA*/     pc.SWEEP_DELAY_RSTb = nor(pb.SWEEP_TRIGGER, pb.SEQ0);

  /*p13.BEXA*/ pc.SWEEP_TRIGGER = tock_pos(aCLK_2M, bCLK_2M, pb.SWEEP_TRIGGER_RSTn, pb.SWEEP_TRIGGER, pb.SWEEP_MAX);
  /*p13.BURY*/   pc.SWEEP_TRIGGER_RSTn = nor(pb.NR10_NO_SWEEP, RESET1);
  /*p13.COZE*/   pc.SWEEP_MAX = and(pb.SWEEP_DELAY0, pb.SWEEP_DELAY1, pb.SWEEP_DELAY2);

  //----------
  // Frequency summer

  /*p12.GALO*/ pc.SUM_A00 = tock_pos(pa.SHIFT_DONE_SYNCn, pa.SHIFT_DONE_SYNCn, pa.SUMMER_RSTn, pb.SUM_A00, pb.SUMMER00);
  /*p12.JODE*/ pc.SUM_A01 = tock_pos(pa.SHIFT_DONE_SYNCn, pa.SHIFT_DONE_SYNCn, pa.SUMMER_RSTn, pb.SUM_A01, pb.SUMMER01);
  /*p12.KARE*/ pc.SUM_A02 = tock_pos(pa.SHIFT_DONE_SYNCn, pa.SHIFT_DONE_SYNCn, pa.SUMMER_RSTn, pb.SUM_A02, pb.SUMMER02);
  /*p12.JYME*/ pc.SUM_A03 = tock_pos(pa.SHIFT_DONE_SYNCn, pa.SHIFT_DONE_SYNCn, pa.SUMMER_RSTn, pb.SUM_A03, pb.SUMMER03);
  /*p12.GYME*/ pc.SUM_A04 = tock_pos(pa.SHIFT_DONE_SYNCn, pa.SHIFT_DONE_SYNCn, pa.SUMMER_RSTn, pb.SUM_A04, pb.SUMMER04);
  /*p12.FAXO*/ pc.SUM_A05 = tock_pos(pa.SHIFT_DONE_SYNCn, pa.SHIFT_DONE_SYNCn, pa.SUMMER_RSTn, pb.SUM_A05, pb.SUMMER05);
  /*p12.EXAP*/ pc.SUM_A06 = tock_pos(pa.SHIFT_DONE_SYNCn, pa.SHIFT_DONE_SYNCn, pa.SUMMER_RSTn, pb.SUM_A06, pb.SUMMER06);
  /*p12.DELE*/ pc.SUM_A07 = tock_pos(pa.SHIFT_DONE_SYNCn, pa.SHIFT_DONE_SYNCn, pa.SUMMER_RSTn, pb.SUM_A07, pb.SUMMER07);
  /*p12.DEXE*/ pc.SUM_A08 = tock_pos(pa.SHIFT_DONE_SYNCn, pa.SHIFT_DONE_SYNCn, pa.SUMMER_RSTn, pb.SUM_A08, pb.SUMMER08);
  /*p12.DOFY*/ pc.SUM_A09 = tock_pos(pa.SHIFT_DONE_SYNCn, pa.SHIFT_DONE_SYNCn, pa.SUMMER_RSTn, pb.SUM_A09, pb.SUMMER09);
  /*p12.DOLY*/ pc.SUM_A10 = tock_pos(pa.SHIFT_DONE_SYNCn, pa.SHIFT_DONE_SYNCn, pa.SUMMER_RSTn, pb.SUM_A10, pb.SUMMER10);

  /*p12.HORA*/ pc.SUM_B00 = tock_pos(pb.SHIFT_DONE_SYNCn, pb.SHIFT_DONE_SYNCn, pb.SUMMER_RSTn, pc.SUM_B00, pb.SHIFTER_FLIP_00);
  /*p12.HOPA*/ pc.SUM_B01 = tock_pos(pb.SHIFT_DONE_SYNCn, pb.SHIFT_DONE_SYNCn, pb.SUMMER_RSTn, pc.SUM_B01, pb.SHIFTER_FLIP_01);
  /*p12.HELE*/ pc.SUM_B02 = tock_pos(pb.SHIFT_DONE_SYNCn, pb.SHIFT_DONE_SYNCn, pb.SUMMER_RSTn, pc.SUM_B02, pb.SHIFTER_FLIP_02);
  /*p12.JAPE*/ pc.SUM_B03 = tock_pos(pb.SHIFT_DONE_SYNCn, pb.SHIFT_DONE_SYNCn, pb.SUMMER_RSTn, pc.SUM_B03, pb.SHIFTER_FLIP_03);
  /*p12.JETE*/ pc.SUM_B04 = tock_pos(pb.SHIFT_DONE_SYNCn, pb.SHIFT_DONE_SYNCn, pb.SUMMER_RSTn, pc.SUM_B04, pb.SHIFTER_FLIP_04);
  /*p12.GELE*/ pc.SUM_B05 = tock_pos(pb.SHIFT_DONE_SYNCn, pb.SHIFT_DONE_SYNCn, pb.SUMMER_RSTn, pc.SUM_B05, pb.SHIFTER_FLIP_05);
  /*p12.EPYR*/ pc.SUM_B06 = tock_pos(pb.SHIFT_DONE_SYNCn, pb.SHIFT_DONE_SYNCn, pb.SUMMER_RSTn, pc.SUM_B06, pb.SHIFTER_FLIP_06);
  /*p12.EDOK*/ pc.SUM_B07 = tock_pos(pb.SHIFT_DONE_SYNCn, pb.SHIFT_DONE_SYNCn, pb.SUMMER_RSTn, pc.SUM_B07, pb.SHIFTER_FLIP_07);
  /*p12.DEFA*/ pc.SUM_B08 = tock_pos(pb.SHIFT_DONE_SYNCn, pb.SHIFT_DONE_SYNCn, pb.SUMMER_RSTn, pc.SUM_B08, pb.SHIFTER_FLIP_08);
  /*p12.ETER*/ pc.SUM_B09 = tock_pos(pb.SHIFT_DONE_SYNCn, pb.SHIFT_DONE_SYNCn, pb.SUMMER_RSTn, pc.SUM_B09, pb.SHIFTER_FLIP_09);
  /*p12.DEVA*/ pc.SUM_B10 = tock_pos(pb.SHIFT_DONE_SYNCn, pb.SHIFT_DONE_SYNCn, pb.SUMMER_RSTn, pc.SUM_B10, pb.SHIFTER_FLIP_10);
  /*p13.KYLY*/   pc.SUMMER_RSTn = not(pb.SUMMER_RST);
  /*p13.KABA*/     pc.SUMMER_RST = or(RESET1, pb.SEQ0);
  /*p12.GYLO*/   pc.SHIFTER_FLIP_00 = xor(pb.NR10_SWEEP_DIRn, pb.SHIFTER_00);
  /*p12.GELA*/   pc.SHIFTER_FLIP_01 = xor(pb.NR10_SWEEP_DIRn, pb.SHIFTER_01);
  /*p12.GOPO*/   pc.SHIFTER_FLIP_02 = xor(pb.NR10_SWEEP_DIRn, pb.SHIFTER_02);
  /*p12.HEFY*/   pc.SHIFTER_FLIP_03 = xor(pb.NR10_SWEEP_DIRn, pb.SHIFTER_03);
  /*p12.KEFE*/   pc.SHIFTER_FLIP_04 = xor(pb.NR10_SWEEP_DIRn, pb.SHIFTER_04);
  /*p12.GOLY*/   pc.SHIFTER_FLIP_05 = xor(pb.NR10_SWEEP_DIRn, pb.SHIFTER_05);
  /*p12.FURE*/   pc.SHIFTER_FLIP_06 = xor(pb.NR10_SWEEP_DIRn, pb.SHIFTER_06);
  /*p12.DYME*/   pc.SHIFTER_FLIP_07 = xor(pb.NR10_SWEEP_DIRn, pb.SHIFTER_07);
  /*p12.CALE*/   pc.SHIFTER_FLIP_08 = xor(pb.NR10_SWEEP_DIRn, pb.SHIFTER_08);
  /*p12.DOZY*/   pc.SHIFTER_FLIP_09 = xor(pb.NR10_SWEEP_DIRn, pb.SHIFTER_09);
  /*p12.CULU*/   pc.SHIFTER_FLIP_10 = xor(pb.NR10_SWEEP_DIRn, pb.SHIFTER_10);

  /*p12.GUXA*/ wire SUMMER00_C = add_c(!pb.SUM_A00, !pb.SUM_B00, pb.NR10_SWEEP_DIRn);
  /*p12.HALU*/ wire SUMMER01_C = add_c(!pb.SUM_A01, !pb.SUM_B01, SUMMER00_C);
  /*p12.JULE*/ wire SUMMER02_C = add_c(!pb.SUM_A02, !pb.SUM_B02, SUMMER01_C);
  /*p12.JORY*/ wire SUMMER03_C = add_c(!pb.SUM_A03, !pb.SUM_B03, SUMMER02_C);
  /*p12.HEXO*/ wire SUMMER04_C = add_c(!pb.SUM_A04, !pb.SUM_B04, SUMMER03_C);
  /*p12.GEVA*/ wire SUMMER05_C = add_c(!pb.SUM_A05, !pb.SUM_B05, SUMMER04_C);
  /*p12.FEGO*/ wire SUMMER06_C = add_c(!pb.SUM_A06, !pb.SUM_B06, SUMMER05_C);
  /*p12.ETEK*/ wire SUMMER07_C = add_c(!pb.SUM_A07, !pb.SUM_B07, SUMMER06_C);
  /*p12.DYXE*/ wire SUMMER08_C = add_c(!pb.SUM_A08, !pb.SUM_B08, SUMMER07_C);
  /*p12.DULE*/ wire SUMMER09_C = add_c(!pb.SUM_A09, !pb.SUM_B09, SUMMER08_C);
  /*p12.CORU*/ wire SUMMER10_C = add_c(!pb.SUM_A10, !pb.SUM_B10, SUMMER09_C);

  /*p12.GUXA*/ wire SUMMER00_S = add_s(!pb.SUM_A00, !pb.SUM_B00, pb.NR10_SWEEP_DIRn);
  /*p12.HALU*/ wire SUMMER01_S = add_s(!pb.SUM_A01, !pb.SUM_B01, SUMMER00_C);
  /*p12.JULE*/ wire SUMMER02_S = add_s(!pb.SUM_A02, !pb.SUM_B02, SUMMER01_C);
  /*p12.JORY*/ wire SUMMER03_S = add_s(!pb.SUM_A03, !pb.SUM_B03, SUMMER02_C);
  /*p12.HEXO*/ wire SUMMER04_S = add_s(!pb.SUM_A04, !pb.SUM_B04, SUMMER03_C);
  /*p12.GEVA*/ wire SUMMER05_S = add_s(!pb.SUM_A05, !pb.SUM_B05, SUMMER04_C);
  /*p12.FEGO*/ wire SUMMER06_S = add_s(!pb.SUM_A06, !pb.SUM_B06, SUMMER05_C);
  /*p12.ETEK*/ wire SUMMER07_S = add_s(!pb.SUM_A07, !pb.SUM_B07, SUMMER06_C);
  /*p12.DYXE*/ wire SUMMER08_S = add_s(!pb.SUM_A08, !pb.SUM_B08, SUMMER07_C);
  /*p12.DULE*/ wire SUMMER09_S = add_s(!pb.SUM_A09, !pb.SUM_B09, SUMMER08_C);
  /*p12.CORU*/ wire SUMMER10_S = add_s(!pb.SUM_A10, !pb.SUM_B10, SUMMER09_C);

  /*p12.ATYS*/ pc.FREQ_OVERFLOW = or(/*p12.BYLE*/ nor(pb.NR10_SWEEP_DIRn, SUMMER10_C), pb.NR10_SWEEP_DIRn);  // polarity

  /*p12.HYKA*/ pc.SUMMER00 = srtock_pos(pa.SUMMER_CLKa, pa.SUMMER_CLKa, pb.SUMMER00_SET, pb.SUMMER00_RST, pc.SUMMER00, SUMMER00_S);
  /*p12.JYKA*/ pc.SUMMER01 = srtock_pos(pa.SUMMER_CLKa, pa.SUMMER_CLKa, pb.SUMMER01_SET, pb.SUMMER01_RST, pc.SUMMER01, SUMMER01_S);
  /*p12.HAVO*/ pc.SUMMER02 = srtock_pos(pa.SUMMER_CLKa, pa.SUMMER_CLKa, pb.SUMMER02_SET, pb.SUMMER02_RST, pc.SUMMER02, SUMMER02_S);
  /*p12.EDUL*/ pc.SUMMER03 = srtock_pos(pa.SUMMER_CLKa, pa.SUMMER_CLKa, pb.SUMMER03_SET, pb.SUMMER03_RST, pc.SUMMER03, SUMMER03_S);
  /*p12.FELY*/ pc.SUMMER04 = srtock_pos(pa.SUMMER_CLKa, pa.SUMMER_CLKa, pb.SUMMER04_SET, pb.SUMMER04_RST, pc.SUMMER04, SUMMER04_S);
  /*p12.HOLU*/ pc.SUMMER05 = srtock_pos(pa.SUMMER_CLKa, pa.SUMMER_CLKa, pb.SUMMER05_SET, pb.SUMMER05_RST, pc.SUMMER05, SUMMER05_S);
  /*p12.HYXU*/ pc.SUMMER06 = srtock_pos(pa.SUMMER_CLKa, pa.SUMMER_CLKa, pb.SUMMER06_SET, pb.SUMMER06_RST, pc.SUMMER06, SUMMER06_S);
  /*p12.HOPO*/ pc.SUMMER07 = srtock_pos(pa.SUMMER_CLKa, pa.SUMMER_CLKa, pb.SUMMER07_SET, pb.SUMMER07_RST, pc.SUMMER07, SUMMER07_S);
  /*p12.DYGY*/ pc.SUMMER08 = srtock_pos(pa.SUMMER_CLKb, pa.SUMMER_CLKb, pb.SUMMER08_SET, pb.SUMMER08_RST, pc.SUMMER08, SUMMER08_S);
  /*p12.EVAB*/ pc.SUMMER09 = srtock_pos(pa.SUMMER_CLKb, pa.SUMMER_CLKb, pb.SUMMER09_SET, pb.SUMMER09_RST, pc.SUMMER09, SUMMER09_S);
  /*p12.AXAN*/ pc.SUMMER10 = srtock_pos(pa.SUMMER_CLKb, pa.SUMMER_CLKb, pb.SUMMER10_SET, pb.SUMMER10_RST, pc.SUMMER10, SUMMER10_S);
  /*p13.BUSO*/   pc.SUMMER_CLKb = or(pb.SHIFTER_EN, pb.FREQ_OVERFLOW, pb.SWEEP_TRIGGER);
  /*p13.BOJE*/   pc.SUMMER_CLKa = and(pb.SHIFTER_EN, /*p13.ATUV*/ and(pb.SWEEP_TRIGGER, pb.FREQ_OVERFLOW));
  /*p12.GYLU*/   pc.SUMMER00_SET = nand(pb.FREQ_WRc, D0);
  /*p12.GETA*/   pc.SUMMER01_SET = nand(pb.FREQ_WRc, D1);
  /*p12.GOLO*/   pc.SUMMER02_SET = nand(pb.FREQ_WRc, D2);
  /*p12.GOPE*/   pc.SUMMER03_SET = nand(pb.FREQ_WRc, D3);
  /*p12.KOVU*/   pc.SUMMER04_SET = nand(pb.FREQ_WRc, D4);
  /*p12.KYPA*/   pc.SUMMER05_SET = nand(pb.FREQ_WRc, D5);
  /*p12.ELER*/   pc.SUMMER06_SET = nand(pb.FREQ_WRc, D6);
  /*p12.ETOL*/   pc.SUMMER07_SET = nand(pb.FREQ_WRc, D7);
  /*p12.BUGU*/   pc.SUMMER08_SET = nand(pb.FREQ_WRb, D0);
  /*p12.BUDO*/   pc.SUMMER09_SET = nand(pb.FREQ_WRb, D1);
  /*p12.AFEG*/   pc.SUMMER10_SET = nand(pb.FREQ_WRb, D2);
  /*p12.EFOR*/   pc.SUMMER00_RST = nor(pb.FREQ_WR_00, RESET1);
  /*p12.GATO*/   pc.SUMMER01_RST = nor(pb.FREQ_WR_01, RESET1);
  /*p12.GYFU*/   pc.SUMMER02_RST = nor(pb.FREQ_WR_02, RESET1);
  /*p12.GAMO*/   pc.SUMMER03_RST = nor(pb.FREQ_WR_03, RESET1);
  /*p12.KAPO*/   pc.SUMMER04_RST = nor(pb.FREQ_WR_04, RESET1);
  /*p12.KAJU*/   pc.SUMMER05_RST = nor(pb.FREQ_WR_05, RESET1);
  /*p12.ELUF*/   pc.SUMMER06_RST = nor(pb.FREQ_WR_06, RESET1);
  /*p12.ESEL*/   pc.SUMMER07_RST = nor(pb.FREQ_WR_07, RESET1);
  /*p12.BOXU*/   pc.SUMMER08_RST = nor(pb.FREQ_WR_08, RESET1);
  /*p12.BOVU*/   pc.SUMMER09_RST = nor(pb.FREQ_WR_09, RESET1);
  /*p12.APAJ*/   pc.SUMMER10_RST = nor(pb.FREQ_WR_10, RESET1);
  /*p12.EJYF*/     pc.FREQ_WR_00 = and(pb.FREQ_WRc, pb.FREQ_D00n);
  /*p12.FOPU*/     pc.FREQ_WR_01 = and(pb.FREQ_WRc, pb.FREQ_D01n);
  /*p12.FOKE*/     pc.FREQ_WR_02 = and(pb.FREQ_WRc, pb.FREQ_D02n);
  /*p12.FEGA*/     pc.FREQ_WR_03 = and(pb.FREQ_WRc, pb.FREQ_D03n);
  /*p12.KAVO*/     pc.FREQ_WR_04 = and(pb.FREQ_WRc, pb.FREQ_D04n);
  /*p12.KYFU*/     pc.FREQ_WR_05 = and(pb.FREQ_WRc, pb.FREQ_D05n);
  /*p12.ETOK*/     pc.FREQ_WR_06 = and(pb.FREQ_WRc, pb.FREQ_D06n);
  /*p12.EMAR*/     pc.FREQ_WR_07 = and(pb.FREQ_WRc, pb.FREQ_D07n);
  /*p12.BASO*/     pc.FREQ_WR_08 = and(pb.FREQ_WRb, pb.FREQ_D08n);
  /*p12.AMAC*/     pc.FREQ_WR_09 = and(pb.FREQ_WRb, pb.FREQ_D09n);
  /*p12.AJUX*/     pc.FREQ_WR_10 = and(pb.FREQ_WRb, pb.FREQ_D10n);
  /*p12.BYFU*/       pc.FREQ_D10n = not(D2);
  /*p12.BOFU*/       pc.FREQ_D09n = not(D1);
  /*p12.BYSU*/       pc.FREQ_D08n = not(D0);
  /*p12.DULO*/       pc.FREQ_D07n = not(D7);
  /*p12.DYLU*/       pc.FREQ_D06n = not(D6);
  /*p12.JULO*/       pc.FREQ_D05n = not(D5);
  /*p12.KOPU*/       pc.FREQ_D04n = not(D4);
  /*p12.ETUV*/       pc.FREQ_D03n = not(D3);
  /*p12.FULE*/       pc.FREQ_D02n = not(D2);
  /*p12.GULU*/       pc.FREQ_D01n = not(D1);
  /*p12.DEKE*/       pc.FREQ_D00n = not(D0);
  /*p12.DEBY*/   pc.FREQ_WRb = and(APU_WR, ADDR_FF14); // polarity?
  /*p12.DYLA*/   pc.FREQ_WRc = not(/*p12.DEPU*/ nand(APU_WR, ADDR_FF13));  // polarity?

  //----------
  // Frequency shifter

  /*p13.BUGE*/ pc.SHIFTER_EN = nand(!pb.NR10_SHIFT0, !pb.NR10_SHIFT1, !pb.NR10_SHIFT2);

  /*p13.EGOR*/ pc.SHIFTER_CLKn = and(NR41_LEN3b, pb.DODY);
  /*p13.DODY*/   pc.DODY = nor(pb.EGYP, /*p13.CELE*/ not(pb.SHIFTER_EN));
  /*p13.EGYP*/     pc.EGYP = nor(pb.SEQ_CLKn, pb.FEMU);
  /*p13.FEMU*/       pc.FEMU = unk2(pb.EPUK, pb.EVOL);
  /*p13.EPUK*/         pc.EPUK = nor(pb.SHIFT_DONE_SYNC, RESET1);
  /*p13.EVOL*/         pc.EVOL = nor(pb.SWEEP_TRIGGER, pb.SEQ2);

  /*p12.EJYB*/ pc.SHIFTER_CLKo = not(/*p12.FAJA*/ not(pb.SHIFTER_CLKn));
  /*p12.BECY*/ pc.SHIFTER_CLKp = not(/*p12.CYBE*/ not(pb.SHIFTER_CLKo));

  /*p13.COPY*/ pc.SHIFT_DONE = and(pb.SHIFTER_CNT0, pb.SHIFTER_CNT1, pb.SHIFTER_CNT2);
  /*p13.COPA*/   pc.SHIFTER_CNT0 = count_pos(pa.SHIFTER_CNT_CLK,  pb.SHIFTER_CNT_CLK,  pb.SHIFTER_CNT_LD, pb.SHIFTER_CNT0, !pb.NR10_SHIFT0);
  /*p13.CAJA*/   pc.SHIFTER_CNT1 = count_pos(pa.SHIFTER_CNT0,     pb.SHIFTER_CNT0,     pb.SHIFTER_CNT_LD, pb.SHIFTER_CNT1, !pb.NR10_SHIFT1);
  /*p13.BYRA*/   pc.SHIFTER_CNT2 = count_pos(pa.SHIFTER_CNT1,     pb.SHIFTER_CNT1,     pb.SHIFTER_CNT_LD, pb.SHIFTER_CNT2, !pb.NR10_SHIFT2);
  /*p13.DAPU*/     pc.SHIFTER_CNT_CLK = not(pb.SHIFTER_CLKn);
  /*p13.CYLU*/     pc.SHIFTER_CNT_LD = not(pb.SHIFTER_CNT_LDn);
  /*p13.DACU*/       pc.SHIFTER_CNT_LDn = nor(pb.SEQ0, pb.SWEEP_TRIGGER);

  /*p13.ADAD*/ pc.SHIFT_DONE_SYNCn = not(!pb.SHIFT_DONE_SYNC);
  /*p13.BYTE*/   pc.SHIFT_DONE_SYNC = tock_pos(aCLK_2M, bCLK_2M, pb.SHIFT_DONE_SYNC_RST, pb.SHIFT_DONE_SYNC, pb.SHIFT_DONE);
  /*p13.ATAT*/     pc.SHIFT_DONE_SYNC_RST = nor(RESET1, pb.SWEEP_TRIGGER);

  {
    /*p13.KALA*/ wire SHIFTER_LOAD  = nor(pb.SWEEP_TRIGGER, pb.SEQ0);
    /*p12.KAPE*/ wire SHIFTER_LOADn = not(SHIFTER_LOAD);
    /*p12.JUJU*/ wire SHIFTER_LOADo = not(SHIFTER_LOAD);
    /*p12.KEDO*/ wire SHIFTER_LOADp = not(SHIFTER_LOAD);

    /*p12.JUTA*/ wire SUMMER00n = not(pb.SUMMER00);
    /*p12.HOXE*/ wire SUMMER01n = not(pb.SUMMER01);
    /*p12.HUNY*/ wire SUMMER02n = not(pb.SUMMER02);
    /*p12.KEKE*/ wire SUMMER03n = not(pb.SUMMER03);
    /*p12.KOVO*/ wire SUMMER04n = not(pb.SUMMER04);
    /*p12.FOHY*/ wire SUMMER05n = not(pb.SUMMER05);
    /*p12.DEBO*/ wire SUMMER06n = not(pb.SUMMER06);
    /*p12.CYKY*/ wire SUMMER07n = not(pb.SUMMER07);
    /*p12.BYRU*/ wire SUMMER08n = not(pb.SUMMER08);
    /*p12.APAT*/ wire SUMMER09n = not(pb.SUMMER09);
    /*p12.BOJO*/ wire SUMMER10n = not(pb.SUMMER10);

    /*p12.HOZU*/ wire SHIFTER_SET_00 = nand(pb.SUMMER00, SHIFTER_LOADn);
    /*p12.HOLA*/ wire SHIFTER_SET_01 = nand(pb.SUMMER01, SHIFTER_LOADn);
    /*p12.HAWY*/ wire SHIFTER_SET_02 = nand(pb.SUMMER02, SHIFTER_LOADn);
    /*p12.KYRY*/ wire SHIFTER_SET_03 = nand(pb.SUMMER03, SHIFTER_LOADo);
    /*p12.KOLA*/ wire SHIFTER_SET_04 = nand(pb.SUMMER04, SHIFTER_LOADo);
    /*p12.GOVO*/ wire SHIFTER_SET_05 = nand(pb.SUMMER05, SHIFTER_LOADo);
    /*p12.EKEM*/ wire SHIFTER_SET_06 = nand(pb.SUMMER06, SHIFTER_LOADo);
    /*p12.DACE*/ wire SHIFTER_SET_07 = nand(pb.SUMMER07, SHIFTER_LOADp);
    /*p12.BEGE*/ wire SHIFTER_SET_08 = nand(pb.SUMMER08, SHIFTER_LOADp);
    /*p12.BESO*/ wire SHIFTER_SET_09 = nand(pb.SUMMER09, SHIFTER_LOADp);
    /*p12.BEJU*/ wire SHIFTER_SET_10 = nand(pb.SUMMER10, SHIFTER_LOADp);

    /*p12.JADO*/ wire SHIFTER_RST_00 = nor(RESET1, /*p12.KOKO*/ and(SUMMER00n, SHIFTER_LOADn));
    /*p12.HOBU*/ wire SHIFTER_RST_01 = nor(RESET1, /*p12.JOCY*/ and(SUMMER01n, SHIFTER_LOADn));
    /*p12.HYVU*/ wire SHIFTER_RST_02 = nor(RESET1, /*p12.JEHY*/ and(SUMMER02n, SHIFTER_LOADn));
    /*p12.KETO*/ wire SHIFTER_RST_03 = nor(RESET1, /*p12.KAXY*/ and(SUMMER03n, SHIFTER_LOADo));
    /*p12.KYBO*/ wire SHIFTER_RST_04 = nor(RESET1, /*p12.KEVY*/ and(SUMMER04n, SHIFTER_LOADo));
    /*p12.EZUK*/ wire SHIFTER_RST_05 = nor(RESET1, /*p12.EVOF*/ and(SUMMER05n, SHIFTER_LOADo));
    /*p12.ENOK*/ wire SHIFTER_RST_06 = nor(RESET1, /*p12.EREG*/ and(SUMMER06n, SHIFTER_LOADo));
    /*p12.BEWO*/ wire SHIFTER_RST_07 = nor(RESET1, /*p12.BAPU*/ and(SUMMER07n, SHIFTER_LOADp));
    /*p12.AGOR*/ wire SHIFTER_RST_08 = nor(RESET1, /*p12.AFUG*/ and(SUMMER08n, SHIFTER_LOADp));
    /*p12.AFUX*/ wire SHIFTER_RST_09 = nor(RESET1, /*p12.BUVO*/ and(SUMMER09n, SHIFTER_LOADp));
    /*p12.AVUF*/ wire SHIFTER_RST_10 = nor(RESET1, /*p12.AFYR*/ and(SUMMER10n, SHIFTER_LOADp));

    /*p12.FABU*/ pc.SHIFTER_00 = srtock_pos(pb.SHIFTER_CLKn, pb.SHIFTER_CLKn, SHIFTER_SET_00, SHIFTER_RST_00, pb.SHIFTER_00, pb.SHIFTER_01);
    /*p12.JEFA*/ pc.SHIFTER_01 = srtock_pos(pb.SHIFTER_CLKn, pb.SHIFTER_CLKn, SHIFTER_SET_01, SHIFTER_RST_01, pb.SHIFTER_01, pb.SHIFTER_02);
    /*p12.GOGA*/ pc.SHIFTER_02 = srtock_pos(pb.SHIFTER_CLKn, pb.SHIFTER_CLKn, SHIFTER_SET_02, SHIFTER_RST_02, pb.SHIFTER_02, pb.SHIFTER_03);
    /*p12.JOLU*/ pc.SHIFTER_03 = srtock_pos(pa.SHIFTER_CLKo, pb.SHIFTER_CLKo, SHIFTER_SET_03, SHIFTER_RST_03, pb.SHIFTER_03, pb.SHIFTER_04);
    /*p12.JOTA*/ pc.SHIFTER_04 = srtock_pos(pa.SHIFTER_CLKo, pb.SHIFTER_CLKo, SHIFTER_SET_04, SHIFTER_RST_04, pb.SHIFTER_04, pb.SHIFTER_05);
    /*p12.FUDE*/ pc.SHIFTER_05 = srtock_pos(pa.SHIFTER_CLKo, pb.SHIFTER_CLKo, SHIFTER_SET_05, SHIFTER_RST_05, pb.SHIFTER_05, pb.SHIFTER_06);
    /*p12.FEDO*/ pc.SHIFTER_06 = srtock_pos(pa.SHIFTER_CLKo, pb.SHIFTER_CLKo, SHIFTER_SET_06, SHIFTER_RST_06, pb.SHIFTER_06, pb.SHIFTER_07);
    /*p12.EXAC*/ pc.SHIFTER_07 = srtock_pos(pa.SHIFTER_CLKp, pb.SHIFTER_CLKp, SHIFTER_SET_07, SHIFTER_RST_07, pb.SHIFTER_07, pb.SHIFTER_08);
    /*p12.ELUX*/ pc.SHIFTER_08 = srtock_pos(pa.SHIFTER_CLKp, pb.SHIFTER_CLKp, SHIFTER_SET_08, SHIFTER_RST_08, pb.SHIFTER_08, pb.SHIFTER_09);
    /*p12.AGEZ*/ pc.SHIFTER_09 = srtock_pos(pa.SHIFTER_CLKp, pb.SHIFTER_CLKp, SHIFTER_SET_09, SHIFTER_RST_09, pb.SHIFTER_09, pb.SHIFTER_10);
    /*p12.BEKU*/ pc.SHIFTER_10 = srtock_pos(pa.SHIFTER_CLKp, pb.SHIFTER_CLKp, SHIFTER_SET_10, SHIFTER_RST_10, pb.SHIFTER_10, ZERO);
  }

  //----------
  // Length timer

  /*p13.BACY*/ pc.NR11_LEN0 = count_pos(pa.CH1_LEN_CLK, pb.CH1_LEN_CLK, pb.FF11_WRc, pb.NR11_LEN0, D0);
  /*p13.CAVY*/ pc.NR11_LEN1 = count_pos(pa.NR11_LEN0,   pb.NR11_LEN0,   pb.FF11_WRc, pb.NR11_LEN1, D1);
  /*p13.BOVY*/ pc.NR11_LEN2 = count_pos(pa.NR11_LEN1,   pb.NR11_LEN1,   pb.FF11_WRc, pb.NR11_LEN2, D2);
  /*p13.CUNO*/ pc.NR11_LEN3 = count_pos(pa.NR11_LEN2,   pb.NR11_LEN2,   pb.FF11_WRc, pb.NR11_LEN3, D3);
  /*p13.CURA*/ pc.NR11_LEN4 = count_pos(pa.NR11_LEN3n,  pb.NR11_LEN3n,  pb.FF11_WRa, pb.NR11_LEN4, D4);
  /*p13.ERAM*/ pc.NR11_LEN5 = count_pos(pa.NR11_LEN4,   pb.NR11_LEN4,   pb.FF11_WRa, pb.NR11_LEN5, D5);
  /*p13.CANU*/   pc.CH1_LEN_CLK = not(pb.CH1_LEN_CLKn);
  /*p13.CAPY*/     pc.CH1_LEN_CLKn = nor(bCLK_256, !pb.NR14_STOP, pb.CH1_LEN_DONE); // use_len polarity?
  /*p13.CUSO*/   pc.NR11_LEN3n = not(!pb.NR11_LEN3);
  /*p13.BUGY*/   pc.FF11_WRc = not(pb.FF11_WRna);
  /*p13.BEPE*/   pc.FF11_WRa = not(pb.FF11_WRna);
  /*p13.BORO*/     pc.FF11_WRna = nand(APU_WR, ADDR_FF11);

  /*p13.CERO*/ pc.CH1_LEN_DONE = tock_pos(!pa.NR11_LEN5, !pb.NR11_LEN5, pb.CH1_LEN_DONE_RST, pc.CH1_LEN_DONE, !pc.CH1_LEN_DONE);
  /*p13.CORY*/   pc.CH1_LEN_DONE_RST = nor(pb.SEQ0, RESET1, pb.FF11_WRb);
  /*p13.BOKA*/     pc.FF11_WRb = not(pb.FF11_WRna);




















  //----------
  // Restart sequencer

  {
    /*p13.EZEC*/ pc.START = tock_pos(aCLK_1M, bCLK_1M, pb.START_RST, pb.START, pb.NR14_START);
    /*p13.DUKA*/   pc.START_RST = not(RESET1);

    /*p09.CALO*/ pc.SEQ_CLK = tock_pos(aCLK_2Mn, bCLK_2Mn, RESETn, pb.SEQ_CLK, !pb.SEQ_CLK);
    /*p09.DYFA*/ pc.SEQ_CLKn = not(!pb.SEQ_CLK);

    /*p13.EGET*/ pc.SEQ0_RST  = nor(RESET1, pb.SEQ1);
    /*p13.GEFE*/ pc.SEQ0_RSTn = not(pb.SEQ0_RST);

    /*p13.ERUM*/ pc.SEQ_RSTo = not(RESET1);

    /*p13.FYFO*/ pc.SEQ_D    = or(pb.SEQ0_RSTn, pb.START); // unk2

    /*p13.FEKU*/ pc.SEQ0 = tock_pos(pa.SEQ_CLKn, pb.SEQ_CLKn, pb.SEQ0_RSTn, pb.SEQ0, pb.SEQ_D);
    /*p13.FARE*/ pc.SEQ1 = tock_pos(pa.SEQ_CLKn, pb.SEQ_CLKn, pb.SEQ_RSTo,  pb.SEQ1, pb.SEQ0);
    /*p13.FYTE*/ pc.SEQ2 = tock_pos(pa.SEQ_CLKn, pb.SEQ_CLKn, pb.SEQ_RSTo,  pb.SEQ2, pb.SEQ1);

    /*p13.GEPU*/ pc.SEQ2n = not(pb.SEQ2);
  }

  //----------
  // Waveform generator

  /*p13.GEXU*/ pc.FREQ_GATE = or(/*p13.FEMY*/ nor(pb.CH1_AMP_ENn, RESET1), pb.SEQ2n);

  /*p11.GEKU*/ pc.CH1_FREQ_CLKa = not(/*p11.FULO*/ nor(pb.SEQ_CLKn, pb.FREQ_GATE));
  /*p11.EPYK*/ pc.CH1_FREQ_RSTn = nor(pb.SEQ0, pb.PHASE_CLKna);

  /*p11.KYPE*/ pc.CH1_FREQ_CLKb = not(pb.FREQ_CNT_03);
  /*p11.DERU*/ pc.CH1_FREQ_CLKc = not(pb.FREQ_CNT_07);
  /*p11.FUME*/ pc.CH1_FREQ_RSTa = not(pb.CH1_FREQ_RSTn);
  /*p11.DEGA*/ pc.CH1_FREQ_RSTb = not(pb.CH1_FREQ_RSTn);
  /*p11.DAKO*/ pc.CH1_FREQ_RSTc = not(pb.CH1_FREQ_RSTn);

  /*p11.GAXE*/ pc.FREQ_CNT_00 = count_pos(pa.CH1_FREQ_CLKa, pb.CH1_FREQ_CLKa, pb.CH1_FREQ_RSTa, pb.FREQ_CNT_00, pb.SUMMER00);
  /*p11.HYFE*/ pc.FREQ_CNT_01 = count_pos(pa.FREQ_CNT_00,   pb.FREQ_CNT_00,   pb.CH1_FREQ_RSTa, pb.FREQ_CNT_01, pb.SUMMER01);
  /*p11.JYTY*/ pc.FREQ_CNT_02 = count_pos(pa.FREQ_CNT_01,   pb.FREQ_CNT_01,   pb.CH1_FREQ_RSTa, pb.FREQ_CNT_02, pb.SUMMER02);
  /*p11.KYNA*/ pc.FREQ_CNT_03 = count_pos(pa.FREQ_CNT_02,   pb.FREQ_CNT_02,   pb.CH1_FREQ_RSTa, pb.FREQ_CNT_03, pb.SUMMER03);
  /*p11.JEMA*/ pc.FREQ_CNT_04 = count_pos(pa.CH1_FREQ_CLKb, pb.CH1_FREQ_CLKb, pb.CH1_FREQ_RSTb, pb.FREQ_CNT_04, pb.SUMMER04);
  /*p11.HYKE*/ pc.FREQ_CNT_05 = count_pos(pa.FREQ_CNT_04,   pb.FREQ_CNT_04,   pb.CH1_FREQ_RSTb, pb.FREQ_CNT_05, pb.SUMMER05);
  /*p11.FEVA*/ pc.FREQ_CNT_06 = count_pos(pa.FREQ_CNT_05,   pb.FREQ_CNT_05,   pb.CH1_FREQ_RSTb, pb.FREQ_CNT_06, pb.SUMMER06);
  /*p11.EKOV*/ pc.FREQ_CNT_07 = count_pos(pa.FREQ_CNT_06,   pb.FREQ_CNT_06,   pb.CH1_FREQ_RSTb, pb.FREQ_CNT_07, pb.SUMMER07);
  /*p11.EMUS*/ pc.FREQ_CNT_08 = count_pos(pa.CH1_FREQ_CLKc, pb.CH1_FREQ_CLKc, pb.CH1_FREQ_RSTc, pb.FREQ_CNT_08, pb.SUMMER08);
  /*p11.EVAK*/ pc.FREQ_CNT_09 = count_pos(pa.FREQ_CNT_09,   pb.FREQ_CNT_09,   pb.CH1_FREQ_RSTc, pb.FREQ_CNT_09, pb.SUMMER09);
  /*p11.COPU*/ pc.FREQ_CNT_10 = count_pos(pa.FREQ_CNT_10,   pb.FREQ_CNT_10,   pb.CH1_FREQ_RSTc, pb.FREQ_CNT_10, pb.SUMMER10);

  /*p13.COMY*/ pc.PHASE_CLKnb = tock_pos(pa.PHASE_CLK_CLK, pb.PHASE_CLK_CLK, pb.PHASE_CLK_RST, pb.PHASE_CLKnb, !pb.PHASE_CLKnb);
  /*p13.CALA*/   pc.PHASE_CLK_CLK = not(pb.FREQ_CNT_10);
  /*p13.DYRU*/   pc.PHASE_CLK_RST = nor(RESET1, pb.SEQ0, pb.DOKA);
  /*p13.DOKA*/     pc.DOKA = and(pb.PHASE_CLKnb, pb.SEQ_CLKn);

  /*p13.DAJO*/ pc.PHASE_CLKa = not(pb.PHASE_CLKna);
  /*p13.COPE*/   pc.PHASE_CLKna = not(pb.PHASE_CLKb);
  /*p13.CYTE*/     pc.PHASE_CLKb = not(pb.PHASE_CLKnb);

  /*p13.ESUT*/ pc.PHASE_xBxDxFxH = tock_pos( pa.PHASE_CLKa,     pb.PHASE_CLKa,      RESET1n, pc.PHASE_xBxDxFxH, !pc.PHASE_xBxDxFxH);
  /*p13.EROS*/ pc.PHASE_xxCDxxGH = tock_pos(!pa.PHASE_xBxDxFxH, !pb.PHASE_xBxDxFxH, RESET1n, pc.PHASE_xxCDxxGH, !pc.PHASE_xxCDxxGH);
  /*p13.DAPE*/ pc.PHASE_xxxxEFGH = tock_pos(!pa.PHASE_xxCDxxGH, !pb.PHASE_xxCDxxGH, RESET1n, pc.PHASE_xxxxEFGH, !pc.PHASE_xxxxEFGH);

  /*p13.DUVO*/ pc.PHASE_AxCxExGx = not(pb.PHASE_xBxDxFxH);
  /*p13.ENEK*/ pc.PHASE_xxxxxxGx = and(pb.PHASE_AxCxExGx, pb.PHASE_xxxxxxGH);
  /*p13.EZOZ*/ pc.PHASE_xxxxxxGH = and(pb.PHASE_xxCDxxGH, pb.PHASE_xxxxEFGH);
  /*p13.CODO*/ pc.PHASE_ABCDEFxx = not(pb.PHASE_xxxxxxGH);

  /*p13.DUNA*/ pc.RAW_BIT = amux4(pb.PHASE_xxxxxxGx, /*p13.COSO*/ nor( pb.NR11_DUTY_0,  pb.NR11_DUTY_1),
                                  pb.PHASE_xxxxxxGH, /*p13.CAVA*/ nor(!pb.NR11_DUTY_0,  pb.NR11_DUTY_1),
                                  pb.PHASE_xxxxEFGH, /*p13.CEVU*/ nor( pb.NR11_DUTY_0, !pb.NR11_DUTY_1),
                                  pb.PHASE_ABCDEFxx, /*p13.CAXO*/ nor(!pb.NR11_DUTY_0, !pb.NR11_DUTY_1));
  /*p13.DUWO*/ pc.RAW_BIT_SYNC = tock_pos(pa.PHASE_CLKna, pb.PHASE_CLKna, RESET1n, pb.RAW_BIT_SYNC, pb.RAW_BIT);


  


  /*p13.BERY*/ pc.CH1_STOP = or(/*p13.BONE*/ not(pb.FREQ_OVERFLOW),
                                RESET1,
                                /*p13.CYFA*/ and(pb.CH1_LEN_DONE, pb.NR14_STOP),
                                pb.CH1_AMP_ENn); // polarity
  
  
  
  /*p13.HOCA*/ pc.CH1_AMP_ENn = nor(pb.NR12_ENV_DIR, pb.NR12_VOL0, pb.NR12_VOL1, pb.NR12_VOL2, pb.NR12_VOL3);

  //----------
  // Envelope generator. I'm not entirely sure how this works...

  /*p13.KOMA*/ pc.ENV_EN = nor(pb.NR12_DELAY0, pb.NR12_DELAY1, pb.NR12_DELAY2);

  /*p13.JOVA*/ pc.ENV_DELAY0 = count_pos(pa.CLK_64b,    pb.CLK_64b,    pb.ENV_DELAY_LOAD, pb.ENV_DELAY0, !pb.NR12_DELAY0);
  /*p13.KENU*/ pc.ENV_DELAY1 = count_pos(pa.ENV_DELAY0, pb.ENV_DELAY0, pb.ENV_DELAY_LOAD, pb.ENV_DELAY1, !pb.NR12_DELAY1);
  /*p13.KERA*/ pc.ENV_DELAY2 = count_pos(pa.ENV_DELAY1, pb.ENV_DELAY1, pb.ENV_DELAY_LOAD, pb.ENV_DELAY2, !pb.NR12_DELAY2);
  /*p13.JOLA*/   pc.CLK_64b  = not(pb.CLK_64nb);
  /*p13.KERE*/     pc.CLK_64nb = not(pb.CLK_64a);
  /*p13.KALY*/       pc.CLK_64a  = tock_pos(pa.CLK_128n, pb.CLK_128n, pb.CH1_RSTd, pb.CLK_64a, !pb.CLK_64a);
  /*p13.JONE*/         pc.CLK_128n = not(bCLK_128);
  /*p13.KADO*/         pc.CH1_RSTd = not(RESET1);
  /*p13.KUXU*/   pc.ENV_DELAY_LOAD = not(pb.ENV_DELAY_LOADn);
  /*p13.KAZA*/     pc.ENV_DELAY_LOADn = nor(pb.SEQ0, pb.ENV_CLK);

  /*p13.KOTE*/ pc.ENV_TRIGGER = and(pb.ENV_DELAY0, pb.ENV_DELAY1, pb.ENV_DELAY2);

  /*p13.KOZY*/ pc.ENV_CLK = tock_pos(aCLK_512, bCLK_512, pb.SWEEP_CNT_MAX_SYNC_RST, pb.ENV_CLK, pb.ENV_TRIGGER);
  /*p13.KORO*/   pc.SWEEP_CNT_MAX_SYNC_RST = nor(pb.KUKU, pb.ENV_EN);
  /*p13.KUKU*/     pc.KUKU = nor(CLKREQn, pb.SWEEP_CNT_MAX_SYNCn);
  /*p13.KURY*/       pc.SWEEP_CNT_MAX_SYNCn = not(pb.ENV_CLK);

  /*p13.HEVO*/ pc.ENV0 = count_pos(pa.ENV_DELTA0, pb.ENV_DELTA0, pb.SEQ0, pb.ENV0, pb.NR12_VOL0);
  /*p13.HOKO*/ pc.ENV1 = count_pos(pa.ENV_DELTA1, pb.ENV_DELTA1, pb.SEQ0, pb.ENV1, pb.NR12_VOL1);
  /*p13.HEMY*/ pc.ENV2 = count_pos(pa.ENV_DELTA2, pb.ENV_DELTA2, pb.SEQ0, pb.ENV2, pb.NR12_VOL2);
  /*p13.HAFO*/ pc.ENV3 = count_pos(pa.ENV_DELTA3, pb.ENV_DELTA3, pb.SEQ0, pb.ENV3, pb.NR12_VOL3);
  /*p13.HESU*/   pc.ENV_DELTA0 = amux2(pb.NR12_ENV_DIR, pb.ENV1, !pb.ENV1, !pb.NR12_ENV_DIR);
  /*p13.HETO*/   pc.ENV_DELTA1 = amux2(pb.NR12_ENV_DIR, pb.ENV2, !pb.ENV2, !pb.NR12_ENV_DIR);
  /*p13.HYTO*/   pc.ENV_DELTA2 = amux2(pb.NR12_ENV_DIR, pb.ENV3, !pb.ENV3, !pb.NR12_ENV_DIR);
  /*p13.JUFY*/   pc.ENV_DELTA3 = amux2(pb.NR12_ENV_DIR, pb.KAKE,     pb.KAKE,      !pb.NR12_ENV_DIR);
  /*p13.KAKE*/     pc.KAKE = and(pb.ENV_CLK, pb.ENV_EN, pb.KEZU);
  /*p13.KEZU*/       pc.KEZU = or(pb.ENV_STOP, pb.KEKO); // unk2
  /*p13.KEKO*/         pc.KEKO = or(RESET1, pb.SEQ0);

  /*p13.KYNO*/ pc.ENV_STOP = tock_pos(pa.ENV_CLK, pb.ENV_CLK, pb.ENV_STOP_RST, pb.ENV_STOP, pb.ENV_MAX);
  /*p13.KORU*/   pc.ENV_STOP_RST = nor(pb.SEQ0, RESET1);
  /*p13.JADE*/   pc.ENV_MAX = or(pb.ENV_TOP, pb.ENV_BOT);
  /*p13.HAKE*/     pc.ENV_TOP = not(pb.ENV_TOPn);
  /*p13.HUFU*/       pc.ENV_TOPn = nand(pb.NR12_ENV_DIR, pb.ENV0, pb.ENV1, pb.ENV2, pb.ENV3);
  /*p13.HANO*/     pc.ENV_BOT = nor(pb.NR12_ENV_DIR, pb.ENV0, pb.ENV1, pb.ENV2, pb.ENV3);

  //----------
  // Audio output

  /*p13.CYTO*/ pc.CH1_ACTIVE = or(pb.SEQ0, pb.CH1_STOP);
  /*p13.BOTO*/ pc.BIT_OUT = or(/*p13.COWE*/ and(pb.CH1_ACTIVE, pb.RAW_BIT_SYNC), DBG_APU);

  /*p13.AMOP*/ pc.CH1_OUT0 = and(pb.ENV0, pb.BIT_OUT);
  /*p13.ASON*/ pc.CH1_OUT1 = and(pb.ENV1, pb.BIT_OUT);
  /*p13.AGOF*/ pc.CH1_OUT2 = and(pb.ENV2, pb.BIT_OUT);
  /*p13.ACEG*/ pc.CH1_OUT3 = and(pb.ENV3, pb.BIT_OUT);
}
