#include "Sch_Channel1.h"

namespace Schematics {

#if 0

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void Channel1_tick(const Pins& pins,
                   const Gameboy& a,
                   const Gameboy& b,
                   const ClkSignals& dff20,
                   Gameboy& next) {

  //----------
  // FF10 NR10

  {
    /*p09.AGUR*/ wire RESETn = not1(b.apu.APU_RESET1);
    /*p10.DUPO*/ wire ADDR_0000an = nand2(b.apu.ADDR_0xxx, b.apu.ADDR_x0xx, b.apu.ADDR_xx0x, b.apu.ADDR_xxx0);
    /*p10.DYVA*/ wire ADDR_FF10  = nor4(b.apu.ADDR_FF1Xn, ADDR_0000an);
    /*p11.BUZE*/ wire ADDR_FF10n = not1(ADDR_FF10);

    /*p10.BAFU*/ wire CPU_WRn = not1(b.ctl.TAPU_CPUWR);
    /*p10.BOGY*/ wire APU_WR  = not1(CPU_WRn);
    /*p11.CENU*/ next.ch1.NR10_CLK       = and2(APU_WR, ADDR_FF10);
    /*p11.BANY*/ next.ch1.NR10_SHIFT0    = tock_pos(a.ch1.NR10_CLK, b.ch1.NR10_CLK, RESETn, b.ch1.NR10_SHIFT0,    b.bus.TS_D0());
    /*p11.ARAX*/ next.ch1.NR10_SHIFT1    = tock_pos(a.ch1.NR10_CLK, b.ch1.NR10_CLK, RESETn, b.ch1.NR10_SHIFT1,    b.bus.TS_D1());
    /*p11.ANAZ*/ next.ch1.NR10_SHIFT2    = tock_pos(a.ch1.NR10_CLK, b.ch1.NR10_CLK, RESETn, b.ch1.NR10_SHIFT2,    b.bus.TS_D2());
    /*p11.AVAF*/ next.ch1.NR10_SWEEP_DIR = tock_pos(a.ch1.NR10_CLK, b.ch1.NR10_CLK, RESETn, b.ch1.NR10_SWEEP_DIR, b.bus.TS_D3());
    /*p11.ADEK*/ next.ch1.NR10_DELAY0    = tock_pos(a.ch1.NR10_CLK, b.ch1.NR10_CLK, RESETn, b.ch1.NR10_DELAY0,    b.bus.TS_D4());
    /*p11.BANA*/ next.ch1.NR10_DELAY1    = tock_pos(a.ch1.NR10_CLK, b.ch1.NR10_CLK, RESETn, b.ch1.NR10_DELAY1,    b.bus.TS_D5());
    /*p11.BOTU*/ next.ch1.NR10_DELAY2    = tock_pos(a.ch1.NR10_CLK, b.ch1.NR10_CLK, RESETn, b.ch1.NR10_DELAY2,    b.bus.TS_D6());

    /*p09.AGUZ*/ wire CPU_RDn = not1(b.ctl.TEDO_CPURD);
    /*p11.ATYN*/ wire NR10_RDn = nor4(CPU_RDn, ADDR_FF10n);
    /*p11.ASOP*/ wire NR10_RD  = not1(NR10_RDn);

    if (NR10_RD) next.bus.set_data(
      /*p11.AMYD*/ b.ch1.NR10_SHIFT0,
      /*p11.ATAX*/ b.ch1.NR10_SHIFT1,
      /*p11.AZYP*/ b.ch1.NR10_SHIFT2,
      /*p11.AFOX*/ b.ch1.NR10_SWEEP_DIR,
      /*p11.AVEK*/ b.ch1.NR10_DELAY0,
      /*p11.AKUX*/ b.ch1.NR10_DELAY1,
      /*p11.AWOS*/ b.ch1.NR10_DELAY2
    );
  }

  //----------
  // FF11 NR11
  {
    /*p11.CEPO*/ wire RESET1n = not1(b.apu.APU_RESET1);
    /*p10.DUNO*/ wire ADDR_0001an = nand2(b.apu.ADDR_0xxx, b.apu.ADDR_x0xx, b.apu.ADDR_xx0x, b.apu.ADDR_xxx1); 
    /*p10.CAXE*/ wire ADDR_FF11  = nor4(b.apu.ADDR_FF1Xn, ADDR_0001an);

    /*p10.BAFU*/ wire CPU_WRn = not1(b.ctl.TAPU_CPUWR);
    /*p10.BOGY*/ wire APU_WR  = not1(CPU_WRn);
    /*p09.AGUZ*/ wire CPU_RDn = not1(b.ctl.TEDO_CPURD);
    /*p11.BUWA*/ wire CPU_RDa = not1(CPU_RDn);
    /*p11.BEXU*/ wire NR11_RD = nor4(ADDR_FF11, CPU_RDa); // polarity?
    /*p11.COVU*/ wire NR11_WR = and2(ADDR_FF11, APU_WR);

    /*p11.DAFO*/ next.ch1.NR11_CLK    = not1(NR11_WR);
    /*p11.CENA*/ next.ch1.NR11_DUTY_0 = tock_pos(a.ch1.NR11_CLK, b.ch1.NR11_CLK, RESET1n, b.ch1.NR11_DUTY_0, b.bus.TS_D0());
    /*p11.DYCA*/ next.ch1.NR11_DUTY_1 = tock_pos(a.ch1.NR11_CLK, b.ch1.NR11_CLK, RESET1n, b.ch1.NR11_DUTY_1, b.bus.TS_D1());

    if (NR11_RD) next.bus.set_data(
      /*p11.BOWO*/ b.ch1.NR11_DUTY_0,
      /*p11.CUDA*/ b.ch1.NR11_DUTY_1
    );
  }

  //----------
  // FF12 NR12

  {
    
    /*p10.DAMY*/ wire ADDR_0010an = nand2(b.apu.ADDR_0xxx, b.apu.ADDR_x0xx, b.apu.ADDR_xx1x, b.apu.ADDR_xxx0);
    /*p10.EDAF*/ wire ADDR_FF12  = nor4(b.apu.ADDR_FF1Xn, ADDR_0010an);
  
    /*p10.BAFU*/ wire CPU_WRn = not1(b.ctl.TAPU_CPUWR);
    /*p10.BOGY*/ wire APU_WR  = not1(CPU_WRn);

    /*p11.HATO*/ wire NR12_RSTn = not1(b.apu.APU_RESET1);
    /*p11.KYGY*/ next.ch1.NR12_CLKa  = not1(/*p11.HAFU*/ and2(APU_WR, ADDR_FF12));
    /*p11.GAXU*/ next.ch1.NR12_CLKb  = nand2(APU_WR, ADDR_FF12);
    /*p11.JUSA*/ next.ch1.NR12_DELAY0  = tock_pos(a.ch1.NR12_CLKa, b.ch1.NR12_CLKa, NR12_RSTn, b.ch1.NR12_DELAY0,  b.bus.TS_D0());
    /*p11.JUZY*/ next.ch1.NR12_DELAY1  = tock_pos(a.ch1.NR12_CLKa, b.ch1.NR12_CLKa, NR12_RSTn, b.ch1.NR12_DELAY1,  b.bus.TS_D1());
    /*p11.JOMA*/ next.ch1.NR12_DELAY2  = tock_pos(a.ch1.NR12_CLKa, b.ch1.NR12_CLKa, NR12_RSTn, b.ch1.NR12_DELAY2,  b.bus.TS_D2());
    /*p11.JAFY*/ next.ch1.NR12_ENV_DIR = tock_pos(a.ch1.NR12_CLKb, b.ch1.NR12_CLKb, NR12_RSTn, b.ch1.NR12_ENV_DIR, b.bus.TS_D3());
    /*p11.JATY*/ next.ch1.NR12_VOL0    = tock_pos(a.ch1.NR12_CLKb, b.ch1.NR12_CLKb, NR12_RSTn, b.ch1.NR12_VOL0,    b.bus.TS_D4());
    /*p11.JAXO*/ next.ch1.NR12_VOL1    = tock_pos(a.ch1.NR12_CLKb, b.ch1.NR12_CLKb, NR12_RSTn, b.ch1.NR12_VOL1,    b.bus.TS_D5());
    /*p11.JENA*/ next.ch1.NR12_VOL2    = tock_pos(a.ch1.NR12_CLKb, b.ch1.NR12_CLKb, NR12_RSTn, b.ch1.NR12_VOL2,    b.bus.TS_D6());
    /*p11.JOPU*/ next.ch1.NR12_VOL3    = tock_pos(a.ch1.NR12_CLKb, b.ch1.NR12_CLKb, NR12_RSTn, b.ch1.NR12_VOL3,    b.bus.TS_D7());

    /*p09.AGUZ*/ wire CPU_RDn = not1(b.ctl.TEDO_CPURD);
    /*p11.HAXE*/ wire ADDR_FF12n = not1(ADDR_FF12);
    /*p11.GAGO*/ wire ADDR_FF12o = not1(ADDR_FF12);
    /*p11.HAMY*/ wire NR12_RDn = or2(ADDR_FF12n, CPU_RDn); // polarity?
    /*p11.HOCU*/ wire NR12_RDo = or2(ADDR_FF12o, CPU_RDn);

    if (!NR12_RDn) next.bus.set_data(
      /*p11.JYNE*/ b.ch1.NR12_DELAY0,
      /*p11.JACA*/ b.ch1.NR12_DELAY1,
      /*p11.JOKU*/ b.ch1.NR12_DELAY2,
      /*p11.HONO*/ b.ch1.NR12_ENV_DIR,
      /*p11.HOWU*/ b.ch1.NR12_VOL0,
      /*p11.HEWA*/ b.ch1.NR12_VOL1,
      /*p11.HEVE*/ b.ch1.NR12_VOL2,
      /*p11.JYSE*/ b.ch1.NR12_VOL3
    );

    /*p13.HOCA*/ next.ch1.CH1_AMP_ENn = nor4(b.ch1.NR12_ENV_DIR, b.ch1.NR12_VOL0, b.ch1.NR12_VOL1, b.ch1.NR12_VOL2, b.ch1.NR12_VOL3);
  }

  //----------
  // Frequency can be read from FF13/FF14 in debug mode

  {
    /*p10.ETUF*/ wire ADDR_0011an = nand2(b.apu.ADDR_0xxx, b.apu.ADDR_x0xx, b.apu.ADDR_xx1x, b.apu.ADDR_xxx1); 
    /*p10.DECO*/ wire ADDR_FF13  = nor4(b.apu.ADDR_FF1Xn, ADDR_0011an);

    /*p10.ESOT*/ wire ADDR_0100an = nand2(b.apu.ADDR_0xxx, b.apu.ADDR_x1xx, b.apu.ADDR_xx0x, b.apu.ADDR_xxx0);
    /*p10.DUJA*/ wire ADDR_FF14  = nor4(b.apu.ADDR_FF1Xn, ADDR_0100an);

    /*p11.CACA*/ wire ADDR_FF13n = not1(ADDR_FF13);
    /*p11.CURE*/ wire ADDR_FF14n = not1(ADDR_FF14);

    /*p09.AGUZ*/ wire CPU_RDn = not1(b.ctl.TEDO_CPURD);
    /*p11.CEGE*/ wire CPU_RDb  = not1(CPU_RDn);
    /*p11.DAXA*/ wire CPU_RDnb = nand2(CPU_RDb, b.apu.NR52_DBG_APU);
    /*p11.DYPU*/ wire FF13_RDn = nor4(CPU_RDnb, ADDR_FF13n);
    /*p11.EVAJ*/ wire FF13_RDa = not1(FF13_RDn);
    /*p11.DUPY*/ wire FF14_RDa = or2(CPU_RDnb, ADDR_FF14n); // polarity?

    if (FF13_RDa) next.bus.set_data(
      /*p11.FORU*/ b.ch1.FREQ_CNT_00,
      /*p11.GEFU*/ b.ch1.FREQ_CNT_01,
      /*p11.KYVU*/ b.ch1.FREQ_CNT_02,
      /*p11.KUMO*/ b.ch1.FREQ_CNT_03,
      /*p11.KARY*/ b.ch1.FREQ_CNT_04,
      /*p11.GODE*/ b.ch1.FREQ_CNT_05,
      /*p11.GOJE*/ b.ch1.FREQ_CNT_06,
      /*p11.FOZE*/ b.ch1.FREQ_CNT_07
    );

    if (FF14_RDa) next.bus.set_data(
      /*p11.DOPA*/ b.ch1.FREQ_CNT_08,
      /*p11.DEMU*/ b.ch1.FREQ_CNT_09,
      /*p11.DEXO*/ b.ch1.FREQ_CNT_10
    );
  }

  //----------
  // FF14 NR14
  {
    /*p10.ESOT*/ wire ADDR_0100an = nand2(b.apu.ADDR_0xxx, b.apu.ADDR_x1xx, b.apu.ADDR_xx0x, b.apu.ADDR_xxx0);
    /*p10.DUJA*/ wire ADDR_FF14  = nor4(b.apu.ADDR_FF1Xn, ADDR_0100an);

    /*p11.CAMY*/ wire NR14_RSTa = not1(b.apu.APU_RESET1);
    /*p13.DADO*/ wire NR14_RSTb = nor4(b.apu.APU_RESET1, b.ch1.START);

#if 0
    NR14_CLKa   = nand2(b.cpu.CPU_PIN_HOLD_MEM, TAPU_CPU_WRp_xxxDxxxx, ADDR_FF14); 
    next.ch1.NR14_STOP = tock_pos(a.ch1.NR14_CLKa, b.ch1.NR14_CLKa, NR14_RSTa, b.ch1.NR14_STOP, b.bus.TS_D6());

#endif

    /*p10.BAFU*/ wire CPU_WRn = not1(b.ctl.TAPU_CPU_WRp_xxxDxxxx);
    /*p10.BOGY*/ wire APU_WR  = not1(CPU_WRn);
    /*p16.ANUJ*/ wire CPU_WR_WEIRD = and2(b.cpu.CPU_PIN_HOLD_MEM, APU_WR);

    /*p11.BAGE*/ next.ch1.NR14_CLKa   = nand2(CPU_WR_WEIRD, ADDR_FF14); 
    /*p11.BOKO*/ next.ch1.NR14_STOP = tock_pos(a.ch1.NR14_CLKa, b.ch1.NR14_CLKa, NR14_RSTa, b.ch1.NR14_STOP, b.bus.TS_D6());

    /*p13.DOGE*/ next.ch1.FF14_WRnb = nand2(APU_WR, ADDR_FF14);
    /*p13.DUPE*/ next.ch1.NR14_START = tock_pos(a.ch1.FF14_WRnb, b.ch1.FF14_WRnb, NR14_RSTb, b.ch1.NR14_START, b.bus.TS_D7());

    /*p11.BUDA*/ wire CPU_RDo = not1(b.ctl.TEDO_CPURD);
    /*p11.BALE*/ wire NR14_RD = nand2(CPU_RDo, ADDR_FF14);

    if (NR14_RD) next.bus.set_data(
      0,
      0,
      0,
      0,
      0,
      0,
      /*p11.BYTU*/ b.ch1.NR14_STOP
    );
  }

  //----------
  // Sweep timer

  {
    wire aCLK_2M  = a.apu.AJER_2M;
    wire bCLK_2M  = b.apu.AJER_2M;

    /*p09.ATYV*/ wire RESETo = not1(b.apu.APU_RESET1);
    /*p09.BAZA*/ next.ch1.DBG_SWEEP_CLK = tock_pos(aCLK_2M, bCLK_2M, RESETo, b.ch1.DBG_SWEEP_CLK, b.apu.NR52_DBG_SWEEP);

    wire bCLK_256 = b.apu.CLK_256a;
    wire bCLK_128 = b.apu.CLK_128a;

    /*p09.CATE*/ next.ch1.SWEEP_DELAY_CLK = not1(/*p09.CONE*/ not1(/*p09.CELY*/ mux2(b.ch1.DBG_SWEEP_CLK, bCLK_128, b.apu.NR52_DBG_APU)));
    /*p13.CYMU*/ wire SWEEP_DELAY_RSTn = not1(/*p13.DAFA*/ nor4(b.ch1.SWEEP_TRIGGER, b.ch1.RESTART0));

    /*p13.CUPO*/ next.ch1.SWEEP_DELAY0 = count_pos(a.ch1.SWEEP_DELAY_CLK, b.ch1.SWEEP_DELAY_CLK, SWEEP_DELAY_RSTn, b.ch1.SWEEP_DELAY0, b.ch1.NR10_DELAY0);
    /*p13.CYPU*/ next.ch1.SWEEP_DELAY1 = count_pos(a.ch1.SWEEP_DELAY0,    b.ch1.SWEEP_DELAY0,    SWEEP_DELAY_RSTn, b.ch1.SWEEP_DELAY1, b.ch1.NR10_DELAY1);
    /*p13.CAXY*/ next.ch1.SWEEP_DELAY2 = count_pos(a.ch1.SWEEP_DELAY1,    b.ch1.SWEEP_DELAY1,    SWEEP_DELAY_RSTn, b.ch1.SWEEP_DELAY2, b.ch1.NR10_DELAY2);

    /*p13.BAVE*/ wire NR10_NO_SWEEP   = and2(b.ch1.NR10_DELAY0, b.ch1.NR10_DELAY1, b.ch1.NR10_DELAY2); // polarity
    /*p13.BURY*/ wire SWEEP_TRIGGER_RSTn = nor4(NR10_NO_SWEEP, b.apu.APU_RESET1);
    /*p13.COZE*/ wire SWEEP_MAX = and2(b.ch1.SWEEP_DELAY0, b.ch1.SWEEP_DELAY1, b.ch1.SWEEP_DELAY2);

    /*p13.BEXA*/ next.ch1.SWEEP_TRIGGER = tock_pos(aCLK_2M, bCLK_2M, SWEEP_TRIGGER_RSTn, b.ch1.SWEEP_TRIGGER, SWEEP_MAX);
  }

  //----------
  // Frequency summer

  {
    /*p13.KABA*/ wire SUMMER_RST = or2(b.apu.APU_RESET1, b.ch1.RESTART0);
    /*p13.KYLY*/ wire SUMMER_RSTn = not1(SUMMER_RST);
    /*p12.ARYL*/ wire NR10_SWEEP_DIRn = not1(b.ch1.NR10_SWEEP_DIR);

    /*p12.GYLO*/ wire SHIFTER_FLIP_00 = xor(NR10_SWEEP_DIRn, b.ch1.SHIFTER_00);
    /*p12.GELA*/ wire SHIFTER_FLIP_01 = xor(NR10_SWEEP_DIRn, b.ch1.SHIFTER_01);
    /*p12.GOPO*/ wire SHIFTER_FLIP_02 = xor(NR10_SWEEP_DIRn, b.ch1.SHIFTER_02);
    /*p12.HEFY*/ wire SHIFTER_FLIP_03 = xor(NR10_SWEEP_DIRn, b.ch1.SHIFTER_03);
    /*p12.KEFE*/ wire SHIFTER_FLIP_04 = xor(NR10_SWEEP_DIRn, b.ch1.SHIFTER_04);
    /*p12.GOLY*/ wire SHIFTER_FLIP_05 = xor(NR10_SWEEP_DIRn, b.ch1.SHIFTER_05);
    /*p12.FURE*/ wire SHIFTER_FLIP_06 = xor(NR10_SWEEP_DIRn, b.ch1.SHIFTER_06);
    /*p12.DYME*/ wire SHIFTER_FLIP_07 = xor(NR10_SWEEP_DIRn, b.ch1.SHIFTER_07);
    /*p12.CALE*/ wire SHIFTER_FLIP_08 = xor(NR10_SWEEP_DIRn, b.ch1.SHIFTER_08);
    /*p12.DOZY*/ wire SHIFTER_FLIP_09 = xor(NR10_SWEEP_DIRn, b.ch1.SHIFTER_09);
    /*p12.CULU*/ wire SHIFTER_FLIP_10 = xor(NR10_SWEEP_DIRn, b.ch1.SHIFTER_10);

    /*p13.ADAD*/ next.ch1.SHIFT_DONE_SYNCn = not1(!b.ch1.SHIFT_DONE_SYNC);

    /*p12.GALO*/ next.ch1.SUM_A00 = tock_pos(a.ch1.SHIFT_DONE_SYNCn, a.ch1.SHIFT_DONE_SYNCn, SUMMER_RSTn, b.ch1.SUM_A00, b.ch1.SUMMER00);
    /*p12.JODE*/ next.ch1.SUM_A01 = tock_pos(a.ch1.SHIFT_DONE_SYNCn, a.ch1.SHIFT_DONE_SYNCn, SUMMER_RSTn, b.ch1.SUM_A01, b.ch1.SUMMER01);
    /*p12.KARE*/ next.ch1.SUM_A02 = tock_pos(a.ch1.SHIFT_DONE_SYNCn, a.ch1.SHIFT_DONE_SYNCn, SUMMER_RSTn, b.ch1.SUM_A02, b.ch1.SUMMER02);
    /*p12.JYME*/ next.ch1.SUM_A03 = tock_pos(a.ch1.SHIFT_DONE_SYNCn, a.ch1.SHIFT_DONE_SYNCn, SUMMER_RSTn, b.ch1.SUM_A03, b.ch1.SUMMER03);
    /*p12.GYME*/ next.ch1.SUM_A04 = tock_pos(a.ch1.SHIFT_DONE_SYNCn, a.ch1.SHIFT_DONE_SYNCn, SUMMER_RSTn, b.ch1.SUM_A04, b.ch1.SUMMER04);
    /*p12.FAXO*/ next.ch1.SUM_A05 = tock_pos(a.ch1.SHIFT_DONE_SYNCn, a.ch1.SHIFT_DONE_SYNCn, SUMMER_RSTn, b.ch1.SUM_A05, b.ch1.SUMMER05);
    /*p12.EXAP*/ next.ch1.SUM_A06 = tock_pos(a.ch1.SHIFT_DONE_SYNCn, a.ch1.SHIFT_DONE_SYNCn, SUMMER_RSTn, b.ch1.SUM_A06, b.ch1.SUMMER06);
    /*p12.DELE*/ next.ch1.SUM_A07 = tock_pos(a.ch1.SHIFT_DONE_SYNCn, a.ch1.SHIFT_DONE_SYNCn, SUMMER_RSTn, b.ch1.SUM_A07, b.ch1.SUMMER07);
    /*p12.DEXE*/ next.ch1.SUM_A08 = tock_pos(a.ch1.SHIFT_DONE_SYNCn, a.ch1.SHIFT_DONE_SYNCn, SUMMER_RSTn, b.ch1.SUM_A08, b.ch1.SUMMER08);
    /*p12.DOFY*/ next.ch1.SUM_A09 = tock_pos(a.ch1.SHIFT_DONE_SYNCn, a.ch1.SHIFT_DONE_SYNCn, SUMMER_RSTn, b.ch1.SUM_A09, b.ch1.SUMMER09);
    /*p12.DOLY*/ next.ch1.SUM_A10 = tock_pos(a.ch1.SHIFT_DONE_SYNCn, a.ch1.SHIFT_DONE_SYNCn, SUMMER_RSTn, b.ch1.SUM_A10, b.ch1.SUMMER10);

    /*p12.HORA*/ next.ch1.SUM_B00 = tock_pos(b.ch1.SHIFT_DONE_SYNCn, b.ch1.SHIFT_DONE_SYNCn, SUMMER_RSTn, b.ch1.SUM_B00, SHIFTER_FLIP_00);
    /*p12.HOPA*/ next.ch1.SUM_B01 = tock_pos(b.ch1.SHIFT_DONE_SYNCn, b.ch1.SHIFT_DONE_SYNCn, SUMMER_RSTn, b.ch1.SUM_B01, SHIFTER_FLIP_01);
    /*p12.HELE*/ next.ch1.SUM_B02 = tock_pos(b.ch1.SHIFT_DONE_SYNCn, b.ch1.SHIFT_DONE_SYNCn, SUMMER_RSTn, b.ch1.SUM_B02, SHIFTER_FLIP_02);
    /*p12.JAPE*/ next.ch1.SUM_B03 = tock_pos(b.ch1.SHIFT_DONE_SYNCn, b.ch1.SHIFT_DONE_SYNCn, SUMMER_RSTn, b.ch1.SUM_B03, SHIFTER_FLIP_03);
    /*p12.JETE*/ next.ch1.SUM_B04 = tock_pos(b.ch1.SHIFT_DONE_SYNCn, b.ch1.SHIFT_DONE_SYNCn, SUMMER_RSTn, b.ch1.SUM_B04, SHIFTER_FLIP_04);
    /*p12.GELE*/ next.ch1.SUM_B05 = tock_pos(b.ch1.SHIFT_DONE_SYNCn, b.ch1.SHIFT_DONE_SYNCn, SUMMER_RSTn, b.ch1.SUM_B05, SHIFTER_FLIP_05);
    /*p12.EPYR*/ next.ch1.SUM_B06 = tock_pos(b.ch1.SHIFT_DONE_SYNCn, b.ch1.SHIFT_DONE_SYNCn, SUMMER_RSTn, b.ch1.SUM_B06, SHIFTER_FLIP_06);
    /*p12.EDOK*/ next.ch1.SUM_B07 = tock_pos(b.ch1.SHIFT_DONE_SYNCn, b.ch1.SHIFT_DONE_SYNCn, SUMMER_RSTn, b.ch1.SUM_B07, SHIFTER_FLIP_07);
    /*p12.DEFA*/ next.ch1.SUM_B08 = tock_pos(b.ch1.SHIFT_DONE_SYNCn, b.ch1.SHIFT_DONE_SYNCn, SUMMER_RSTn, b.ch1.SUM_B08, SHIFTER_FLIP_08);
    /*p12.ETER*/ next.ch1.SUM_B09 = tock_pos(b.ch1.SHIFT_DONE_SYNCn, b.ch1.SHIFT_DONE_SYNCn, SUMMER_RSTn, b.ch1.SUM_B09, SHIFTER_FLIP_09);
    /*p12.DEVA*/ next.ch1.SUM_B10 = tock_pos(b.ch1.SHIFT_DONE_SYNCn, b.ch1.SHIFT_DONE_SYNCn, SUMMER_RSTn, b.ch1.SUM_B10, SHIFTER_FLIP_10);

    /*p12.GUXA*/ wire SUMMER00_C = add_c(!b.ch1.SUM_A00, !b.ch1.SUM_B00, NR10_SWEEP_DIRn);
    /*p12.HALU*/ wire SUMMER01_C = add_c(!b.ch1.SUM_A01, !b.ch1.SUM_B01, SUMMER00_C);
    /*p12.JULE*/ wire SUMMER02_C = add_c(!b.ch1.SUM_A02, !b.ch1.SUM_B02, SUMMER01_C);
    /*p12.JORY*/ wire SUMMER03_C = add_c(!b.ch1.SUM_A03, !b.ch1.SUM_B03, SUMMER02_C);
    /*p12.HEXO*/ wire SUMMER04_C = add_c(!b.ch1.SUM_A04, !b.ch1.SUM_B04, SUMMER03_C);
    /*p12.GEVA*/ wire SUMMER05_C = add_c(!b.ch1.SUM_A05, !b.ch1.SUM_B05, SUMMER04_C);
    /*p12.FEGO*/ wire SUMMER06_C = add_c(!b.ch1.SUM_A06, !b.ch1.SUM_B06, SUMMER05_C);
    /*p12.ETEK*/ wire SUMMER07_C = add_c(!b.ch1.SUM_A07, !b.ch1.SUM_B07, SUMMER06_C);
    /*p12.DYXE*/ wire SUMMER08_C = add_c(!b.ch1.SUM_A08, !b.ch1.SUM_B08, SUMMER07_C);
    /*p12.DULE*/ wire SUMMER09_C = add_c(!b.ch1.SUM_A09, !b.ch1.SUM_B09, SUMMER08_C);
    /*p12.CORU*/ wire SUMMER10_C = add_c(!b.ch1.SUM_A10, !b.ch1.SUM_B10, SUMMER09_C);

    /*p12.GUXA*/ wire SUMMER00_S = add_s(!b.ch1.SUM_A00, !b.ch1.SUM_B00, NR10_SWEEP_DIRn);
    /*p12.HALU*/ wire SUMMER01_S = add_s(!b.ch1.SUM_A01, !b.ch1.SUM_B01, SUMMER00_C);
    /*p12.JULE*/ wire SUMMER02_S = add_s(!b.ch1.SUM_A02, !b.ch1.SUM_B02, SUMMER01_C);
    /*p12.JORY*/ wire SUMMER03_S = add_s(!b.ch1.SUM_A03, !b.ch1.SUM_B03, SUMMER02_C);
    /*p12.HEXO*/ wire SUMMER04_S = add_s(!b.ch1.SUM_A04, !b.ch1.SUM_B04, SUMMER03_C);
    /*p12.GEVA*/ wire SUMMER05_S = add_s(!b.ch1.SUM_A05, !b.ch1.SUM_B05, SUMMER04_C);
    /*p12.FEGO*/ wire SUMMER06_S = add_s(!b.ch1.SUM_A06, !b.ch1.SUM_B06, SUMMER05_C);
    /*p12.ETEK*/ wire SUMMER07_S = add_s(!b.ch1.SUM_A07, !b.ch1.SUM_B07, SUMMER06_C);
    /*p12.DYXE*/ wire SUMMER08_S = add_s(!b.ch1.SUM_A08, !b.ch1.SUM_B08, SUMMER07_C);
    /*p12.DULE*/ wire SUMMER09_S = add_s(!b.ch1.SUM_A09, !b.ch1.SUM_B09, SUMMER08_C);
    /*p12.CORU*/ wire SUMMER10_S = add_s(!b.ch1.SUM_A10, !b.ch1.SUM_B10, SUMMER09_C);

    /*p12.ATYS*/ next.ch1.FREQ_OVERFLOW = or2(/*p12.BYLE*/ nor4(NR10_SWEEP_DIRn, SUMMER10_C), NR10_SWEEP_DIRn);  // polarity

    /*p10.ETUF*/ wire ADDR_0011an = nand2(b.apu.ADDR_0xxx, b.apu.ADDR_x0xx, b.apu.ADDR_xx1x, b.apu.ADDR_xxx1); 
    /*p10.DECO*/ wire ADDR_FF13  = nor4(b.apu.ADDR_FF1Xn, ADDR_0011an);

    /*p10.ESOT*/ wire ADDR_0100an = nand2(b.apu.ADDR_0xxx, b.apu.ADDR_x1xx, b.apu.ADDR_xx0x, b.apu.ADDR_xxx0);
    /*p10.DUJA*/ wire ADDR_FF14  = nor4(b.apu.ADDR_FF1Xn, ADDR_0100an);

    /*p10.BAFU*/ wire CPU_WRn = not1(b.ctl.TAPU_CPUWR);
    /*p10.BOGY*/ wire APU_WR  = not1(CPU_WRn);
    /*p12.DEBY*/ wire FREQ_WRb = and2(APU_WR, ADDR_FF14); // polarity?
    /*p12.DYLA*/ wire FREQ_WRc = not1(/*p12.DEPU*/ nand2(APU_WR, ADDR_FF13));  // polarity?

    /*p12.GYLU*/ wire SUMMER00_SET = nand2(FREQ_WRc, b.bus.TS_D0());
    /*p12.GETA*/ wire SUMMER01_SET = nand2(FREQ_WRc, b.bus.TS_D1());
    /*p12.GOLO*/ wire SUMMER02_SET = nand2(FREQ_WRc, b.bus.TS_D2());
    /*p12.GOPE*/ wire SUMMER03_SET = nand2(FREQ_WRc, b.bus.TS_D3());
    /*p12.KOVU*/ wire SUMMER04_SET = nand2(FREQ_WRc, b.bus.TS_D4());
    /*p12.KYPA*/ wire SUMMER05_SET = nand2(FREQ_WRc, b.bus.TS_D5());
    /*p12.ELER*/ wire SUMMER06_SET = nand2(FREQ_WRc, b.bus.TS_D6());
    /*p12.ETOL*/ wire SUMMER07_SET = nand2(FREQ_WRc, b.bus.TS_D7());
    /*p12.BUGU*/ wire SUMMER08_SET = nand2(FREQ_WRb, b.bus.TS_D0());
    /*p12.BUDO*/ wire SUMMER09_SET = nand2(FREQ_WRb, b.bus.TS_D1());
    /*p12.AFEG*/ wire SUMMER10_SET = nand2(FREQ_WRb, b.bus.TS_D2());

    /*p12.EFOR*/ wire SUMMER00_RST = nor4(b.apu.APU_RESET1, /*p12.EJYF*/ and2(FREQ_WRc, /*p12.DEKE*/ not1(b.bus.TS_D0())));
    /*p12.GATO*/ wire SUMMER01_RST = nor4(b.apu.APU_RESET1, /*p12.FOPU*/ and2(FREQ_WRc, /*p12.GULU*/ not1(b.bus.TS_D1())));
    /*p12.GYFU*/ wire SUMMER02_RST = nor4(b.apu.APU_RESET1, /*p12.FOKE*/ and2(FREQ_WRc, /*p12.FULE*/ not1(b.bus.TS_D2())));
    /*p12.GAMO*/ wire SUMMER03_RST = nor4(b.apu.APU_RESET1, /*p12.FEGA*/ and2(FREQ_WRc, /*p12.ETUV*/ not1(b.bus.TS_D3())));
    /*p12.KAPO*/ wire SUMMER04_RST = nor4(b.apu.APU_RESET1, /*p12.KAVO*/ and2(FREQ_WRc, /*p12.KOPU*/ not1(b.bus.TS_D4())));
    /*p12.KAJU*/ wire SUMMER05_RST = nor4(b.apu.APU_RESET1, /*p12.KYFU*/ and2(FREQ_WRc, /*p12.JULO*/ not1(b.bus.TS_D5())));
    /*p12.ELUF*/ wire SUMMER06_RST = nor4(b.apu.APU_RESET1, /*p12.ETOK*/ and2(FREQ_WRc, /*p12.DYLU*/ not1(b.bus.TS_D6())));
    /*p12.ESEL*/ wire SUMMER07_RST = nor4(b.apu.APU_RESET1, /*p12.EMAR*/ and2(FREQ_WRc, /*p12.DULO*/ not1(b.bus.TS_D7())));
    /*p12.BOXU*/ wire SUMMER08_RST = nor4(b.apu.APU_RESET1, /*p12.BASO*/ and2(FREQ_WRb, /*p12.BYSU*/ not1(b.bus.TS_D0())));
    /*p12.BOVU*/ wire SUMMER09_RST = nor4(b.apu.APU_RESET1, /*p12.AMAC*/ and2(FREQ_WRb, /*p12.BOFU*/ not1(b.bus.TS_D1())));
    /*p12.APAJ*/ wire SUMMER10_RST = nor4(b.apu.APU_RESET1, /*p12.AJUX*/ and2(FREQ_WRb, /*p12.BYFU*/ not1(b.bus.TS_D2())));

    /*p13.BOJE*/ wire SUMMER_CLKa = and2(b.ch1.SHIFTER_EN, /*p13.ATUV*/ and2(b.ch1.SWEEP_TRIGGER, b.ch1.FREQ_OVERFLOW));
    /*p13.BUSO*/ wire SUMMER_CLKb = or2(b.ch1.SHIFTER_EN, b.ch1.FREQ_OVERFLOW, b.ch1.SWEEP_TRIGGER);

    /*p12.HYKA*/ next.ch1.SUMMER00 = srtock_pos(SUMMER_CLKa, SUMMER_CLKa, SUMMER00_SET, SUMMER00_RST, b.ch1.SUMMER00, SUMMER00_S);
    /*p12.JYKA*/ next.ch1.SUMMER01 = srtock_pos(SUMMER_CLKa, SUMMER_CLKa, SUMMER01_SET, SUMMER01_RST, b.ch1.SUMMER01, SUMMER01_S);
    /*p12.HAVO*/ next.ch1.SUMMER02 = srtock_pos(SUMMER_CLKa, SUMMER_CLKa, SUMMER02_SET, SUMMER02_RST, b.ch1.SUMMER02, SUMMER02_S);
    /*p12.EDUL*/ next.ch1.SUMMER03 = srtock_pos(SUMMER_CLKa, SUMMER_CLKa, SUMMER03_SET, SUMMER03_RST, b.ch1.SUMMER03, SUMMER03_S);
    /*p12.FELY*/ next.ch1.SUMMER04 = srtock_pos(SUMMER_CLKa, SUMMER_CLKa, SUMMER04_SET, SUMMER04_RST, b.ch1.SUMMER04, SUMMER04_S);
    /*p12.HOLU*/ next.ch1.SUMMER05 = srtock_pos(SUMMER_CLKa, SUMMER_CLKa, SUMMER05_SET, SUMMER05_RST, b.ch1.SUMMER05, SUMMER05_S);
    /*p12.HYXU*/ next.ch1.SUMMER06 = srtock_pos(SUMMER_CLKa, SUMMER_CLKa, SUMMER06_SET, SUMMER06_RST, b.ch1.SUMMER06, SUMMER06_S);
    /*p12.HOPO*/ next.ch1.SUMMER07 = srtock_pos(SUMMER_CLKa, SUMMER_CLKa, SUMMER07_SET, SUMMER07_RST, b.ch1.SUMMER07, SUMMER07_S);
    /*p12.DYGY*/ next.ch1.SUMMER08 = srtock_pos(SUMMER_CLKb, SUMMER_CLKb, SUMMER08_SET, SUMMER08_RST, b.ch1.SUMMER08, SUMMER08_S);
    /*p12.EVAB*/ next.ch1.SUMMER09 = srtock_pos(SUMMER_CLKb, SUMMER_CLKb, SUMMER09_SET, SUMMER09_RST, b.ch1.SUMMER09, SUMMER09_S);
    /*p12.AXAN*/ next.ch1.SUMMER10 = srtock_pos(SUMMER_CLKb, SUMMER_CLKb, SUMMER10_SET, SUMMER10_RST, b.ch1.SUMMER10, SUMMER10_S);
  }

  //----------
  // Frequency shifter

  {
  
    /*p13.BUGE*/ next.ch1.SHIFTER_EN = nand2(!b.ch1.NR10_SHIFT0, !b.ch1.NR10_SHIFT1, !b.ch1.NR10_SHIFT2);

    /*p13.EPUK*/ wire EPUK = nor4(b.ch1.SHIFT_DONE_SYNC, b.apu.APU_RESET1);
    /*p13.EVOL*/ wire EVOL = nor4(b.ch1.SWEEP_TRIGGER, b.ch1.RESTART2);
    /*p13.FEMU*/ wire FEMU = or2(EPUK, EVOL);
    /*p13.EGYP*/ wire EGYP = nor4(b.ch1.RESTART_CLKn, FEMU);  // wat
    /*p13.DODY*/ wire DODY = nor4(EGYP, /*p13.CELE*/ not1(b.ch1.SHIFTER_EN));

    /*p13.EGOR*/ next.ch1.SHIFTER_CLKn = and2(b.ch4.NR41_LEN3b, DODY); // wat?
    /*p12.EJYB*/ next.ch1.SHIFTER_CLKo = not1(/*p12.FAJA*/ not1(b.ch1.SHIFTER_CLKn));
    /*p12.BECY*/ next.ch1.SHIFTER_CLKp = not1(/*p12.CYBE*/ not1(b.ch1.SHIFTER_CLKo));

    /*p13.DACU*/ next.ch1.SHIFTER_CNT_LDn = nor4(b.ch1.RESTART0, b.ch1.SWEEP_TRIGGER);

    /*p13.DAPU*/ next.ch1.SHIFTER_CNT_CLK = not1(b.ch1.SHIFTER_CLKn);
    /*p13.CYLU*/ next.ch1.SHIFTER_CNT_LD = not1(b.ch1.SHIFTER_CNT_LDn);

    /*p13.COPA*/ next.ch1.SHIFTER_CNT0 = count_pos(a.ch1.SHIFTER_CNT_CLK,  b.ch1.SHIFTER_CNT_CLK,  b.ch1.SHIFTER_CNT_LD, b.ch1.SHIFTER_CNT0, !b.ch1.NR10_SHIFT0);
    /*p13.CAJA*/ next.ch1.SHIFTER_CNT1 = count_pos(a.ch1.SHIFTER_CNT0,     b.ch1.SHIFTER_CNT0,     b.ch1.SHIFTER_CNT_LD, b.ch1.SHIFTER_CNT1, !b.ch1.NR10_SHIFT1);
    /*p13.BYRA*/ next.ch1.SHIFTER_CNT2 = count_pos(a.ch1.SHIFTER_CNT1,     b.ch1.SHIFTER_CNT1,     b.ch1.SHIFTER_CNT_LD, b.ch1.SHIFTER_CNT2, !b.ch1.NR10_SHIFT2);

    /*p13.ATAT*/ next.ch1.SHIFT_DONE_SYNC_RST = nor4(b.apu.APU_RESET1, b.ch1.SWEEP_TRIGGER);
    /*p13.BYTE*/ next.ch1.SHIFT_DONE_SYNC = tock_pos(a.apu.AJER_2M, b.apu.AJER_2M,
                                               b.ch1.SHIFT_DONE_SYNC_RST,
                                               b.ch1.SHIFT_DONE_SYNC,
                                               /*p13.COPY*/ and2(b.ch1.SHIFTER_CNT0, b.ch1.SHIFTER_CNT1, b.ch1.SHIFTER_CNT2));
  }

  {
    /*p13.KALA*/ wire SHIFTER_LOAD  = nor4(b.ch1.SWEEP_TRIGGER, b.ch1.RESTART0);
    /*p12.KAPE*/ wire SHIFTER_LOADn = not1(SHIFTER_LOAD);
    /*p12.JUJU*/ wire SHIFTER_LOADo = not1(SHIFTER_LOAD);
    /*p12.KEDO*/ wire SHIFTER_LOADp = not1(SHIFTER_LOAD);

    /*p12.JUTA*/ wire SUMMER00n = not1(b.ch1.SUMMER00);
    /*p12.HOXE*/ wire SUMMER01n = not1(b.ch1.SUMMER01);
    /*p12.HUNY*/ wire SUMMER02n = not1(b.ch1.SUMMER02);
    /*p12.KEKE*/ wire SUMMER03n = not1(b.ch1.SUMMER03);
    /*p12.KOVO*/ wire SUMMER04n = not1(b.ch1.SUMMER04);
    /*p12.FOHY*/ wire SUMMER05n = not1(b.ch1.SUMMER05);
    /*p12.DEBO*/ wire SUMMER06n = not1(b.ch1.SUMMER06);
    /*p12.CYKY*/ wire SUMMER07n = not1(b.ch1.SUMMER07);
    /*p12.BYRU*/ wire SUMMER08n = not1(b.ch1.SUMMER08);
    /*p12.APAT*/ wire SUMMER09n = not1(b.ch1.SUMMER09);
    /*p12.BOJO*/ wire SUMMER10n = not1(b.ch1.SUMMER10);

    /*p12.KOKO*/ wire KOKO = and2(SUMMER00n, SHIFTER_LOADn);
    /*p12.JOCY*/ wire JOCY = and2(SUMMER01n, SHIFTER_LOADn);
    /*p12.JEHY*/ wire JEHY = and2(SUMMER02n, SHIFTER_LOADn);
    /*p12.KAXY*/ wire KAXY = and2(SUMMER03n, SHIFTER_LOADo);
    /*p12.KEVY*/ wire KEVY = and2(SUMMER04n, SHIFTER_LOADo);
    /*p12.EVOF*/ wire EVOF = and2(SUMMER05n, SHIFTER_LOADo);
    /*p12.EREG*/ wire EREG = and2(SUMMER06n, SHIFTER_LOADo);
    /*p12.BAPU*/ wire BAPU = and2(SUMMER07n, SHIFTER_LOADp);
    /*p12.AFUG*/ wire AFUG = and2(SUMMER08n, SHIFTER_LOADp);
    /*p12.BUVO*/ wire BUVO = and2(SUMMER09n, SHIFTER_LOADp);
    /*p12.AFYR*/ wire AFYR = and2(SUMMER10n, SHIFTER_LOADp);

    /*p12.HOZU*/ wire SHIFTER_SET_00 = nand2(b.ch1.SUMMER00, SHIFTER_LOADn);
    /*p12.HOLA*/ wire SHIFTER_SET_01 = nand2(b.ch1.SUMMER01, SHIFTER_LOADn);
    /*p12.HAWY*/ wire SHIFTER_SET_02 = nand2(b.ch1.SUMMER02, SHIFTER_LOADn);
    /*p12.KYRY*/ wire SHIFTER_SET_03 = nand2(b.ch1.SUMMER03, SHIFTER_LOADo);
    /*p12.KOLA*/ wire SHIFTER_SET_04 = nand2(b.ch1.SUMMER04, SHIFTER_LOADo);
    /*p12.GOVO*/ wire SHIFTER_SET_05 = nand2(b.ch1.SUMMER05, SHIFTER_LOADo);
    /*p12.EKEM*/ wire SHIFTER_SET_06 = nand2(b.ch1.SUMMER06, SHIFTER_LOADo);
    /*p12.DACE*/ wire SHIFTER_SET_07 = nand2(b.ch1.SUMMER07, SHIFTER_LOADp);
    /*p12.BEGE*/ wire SHIFTER_SET_08 = nand2(b.ch1.SUMMER08, SHIFTER_LOADp);
    /*p12.BESO*/ wire SHIFTER_SET_09 = nand2(b.ch1.SUMMER09, SHIFTER_LOADp);
    /*p12.BEJU*/ wire SHIFTER_SET_10 = nand2(b.ch1.SUMMER10, SHIFTER_LOADp);

    /*p12.JADO*/ wire SHIFTER_RST_00 = nor4(b.apu.APU_RESET1, KOKO);
    /*p12.HOBU*/ wire SHIFTER_RST_01 = nor4(b.apu.APU_RESET1, JOCY);
    /*p12.HYVU*/ wire SHIFTER_RST_02 = nor4(b.apu.APU_RESET1, JEHY);
    /*p12.KETO*/ wire SHIFTER_RST_03 = nor4(b.apu.APU_RESET1, KAXY);
    /*p12.KYBO*/ wire SHIFTER_RST_04 = nor4(b.apu.APU_RESET1, KEVY);
    /*p12.EZUK*/ wire SHIFTER_RST_05 = nor4(b.apu.APU_RESET1, EVOF);
    /*p12.ENOK*/ wire SHIFTER_RST_06 = nor4(b.apu.APU_RESET1, EREG);
    /*p12.BEWO*/ wire SHIFTER_RST_07 = nor4(b.apu.APU_RESET1, BAPU);
    /*p12.AGOR*/ wire SHIFTER_RST_08 = nor4(b.apu.APU_RESET1, AFUG);
    /*p12.AFUX*/ wire SHIFTER_RST_09 = nor4(b.apu.APU_RESET1, BUVO);
    /*p12.AVUF*/ wire SHIFTER_RST_10 = nor4(b.apu.APU_RESET1, AFYR);

    /*p12.FABU*/ next.ch1.SHIFTER_00 = srtock_pos(b.ch1.SHIFTER_CLKn, b.ch1.SHIFTER_CLKn, SHIFTER_SET_00, SHIFTER_RST_00, b.ch1.SHIFTER_00, b.ch1.SHIFTER_01);
    /*p12.JEFA*/ next.ch1.SHIFTER_01 = srtock_pos(b.ch1.SHIFTER_CLKn, b.ch1.SHIFTER_CLKn, SHIFTER_SET_01, SHIFTER_RST_01, b.ch1.SHIFTER_01, b.ch1.SHIFTER_02);
    /*p12.GOGA*/ next.ch1.SHIFTER_02 = srtock_pos(b.ch1.SHIFTER_CLKn, b.ch1.SHIFTER_CLKn, SHIFTER_SET_02, SHIFTER_RST_02, b.ch1.SHIFTER_02, b.ch1.SHIFTER_03);
    /*p12.JOLU*/ next.ch1.SHIFTER_03 = srtock_pos(a.ch1.SHIFTER_CLKo, b.ch1.SHIFTER_CLKo, SHIFTER_SET_03, SHIFTER_RST_03, b.ch1.SHIFTER_03, b.ch1.SHIFTER_04);
    /*p12.JOTA*/ next.ch1.SHIFTER_04 = srtock_pos(a.ch1.SHIFTER_CLKo, b.ch1.SHIFTER_CLKo, SHIFTER_SET_04, SHIFTER_RST_04, b.ch1.SHIFTER_04, b.ch1.SHIFTER_05);
    /*p12.FUDE*/ next.ch1.SHIFTER_05 = srtock_pos(a.ch1.SHIFTER_CLKo, b.ch1.SHIFTER_CLKo, SHIFTER_SET_05, SHIFTER_RST_05, b.ch1.SHIFTER_05, b.ch1.SHIFTER_06);
    /*p12.FEDO*/ next.ch1.SHIFTER_06 = srtock_pos(a.ch1.SHIFTER_CLKo, b.ch1.SHIFTER_CLKo, SHIFTER_SET_06, SHIFTER_RST_06, b.ch1.SHIFTER_06, b.ch1.SHIFTER_07);
    /*p12.EXAC*/ next.ch1.SHIFTER_07 = srtock_pos(a.ch1.SHIFTER_CLKp, b.ch1.SHIFTER_CLKp, SHIFTER_SET_07, SHIFTER_RST_07, b.ch1.SHIFTER_07, b.ch1.SHIFTER_08);
    /*p12.ELUX*/ next.ch1.SHIFTER_08 = srtock_pos(a.ch1.SHIFTER_CLKp, b.ch1.SHIFTER_CLKp, SHIFTER_SET_08, SHIFTER_RST_08, b.ch1.SHIFTER_08, b.ch1.SHIFTER_09);
    /*p12.AGEZ*/ next.ch1.SHIFTER_09 = srtock_pos(a.ch1.SHIFTER_CLKp, b.ch1.SHIFTER_CLKp, SHIFTER_SET_09, SHIFTER_RST_09, b.ch1.SHIFTER_09, b.ch1.SHIFTER_10);
    /*p12.BEKU*/ next.ch1.SHIFTER_10 = srtock_pos(a.ch1.SHIFTER_CLKp, b.ch1.SHIFTER_CLKp, SHIFTER_SET_10, SHIFTER_RST_10, b.ch1.SHIFTER_10, top.GND);
  }

  //----------
  // Length timer

  {
    /*p10.DUNO*/ wire ADDR_0001an = nand2(b.apu.ADDR_0xxx, b.apu.ADDR_x0xx, b.apu.ADDR_xx0x, b.apu.ADDR_xxx1); 
    /*p10.CAXE*/ wire ADDR_FF11  = nor4(b.apu.ADDR_FF1Xn, ADDR_0001an);

    /*p10.BAFU*/ wire CPU_WRn = not1(b.ctl.TAPU_CPUWR);
    /*p10.BOGY*/ wire APU_WR  = not1(CPU_WRn);

    wire bCLK_256 = b.apu.CLK_256a;
    wire bCLK_128 = b.apu.CLK_128a;

    /*p13.BACY*/ next.ch1.NR11_LEN0 = count_pos(a.ch1.CH1_LEN_CLK, b.ch1.CH1_LEN_CLK, b.ch1.FF11_WRc, b.ch1.NR11_LEN0, b.bus.TS_D0());
    /*p13.CAVY*/ next.ch1.NR11_LEN1 = count_pos(a.ch1.NR11_LEN0,   b.ch1.NR11_LEN0,   b.ch1.FF11_WRc, b.ch1.NR11_LEN1, b.bus.TS_D1());
    /*p13.BOVY*/ next.ch1.NR11_LEN2 = count_pos(a.ch1.NR11_LEN1,   b.ch1.NR11_LEN1,   b.ch1.FF11_WRc, b.ch1.NR11_LEN2, b.bus.TS_D2());
    /*p13.CUNO*/ next.ch1.NR11_LEN3 = count_pos(a.ch1.NR11_LEN2,   b.ch1.NR11_LEN2,   b.ch1.FF11_WRc, b.ch1.NR11_LEN3, b.bus.TS_D3());
    /*p13.CURA*/ next.ch1.NR11_LEN4 = count_pos(a.ch1.NR11_LEN3n,  b.ch1.NR11_LEN3n,  b.ch1.FF11_WRa, b.ch1.NR11_LEN4, b.bus.TS_D4());
    /*p13.ERAM*/ next.ch1.NR11_LEN5 = count_pos(a.ch1.NR11_LEN4,   b.ch1.NR11_LEN4,   b.ch1.FF11_WRa, b.ch1.NR11_LEN5, b.bus.TS_D5());
    /*p13.CANU*/   next.ch1.CH1_LEN_CLK = not1(b.ch1.CH1_LEN_CLKn);
    /*p13.CAPY*/     next.ch1.CH1_LEN_CLKn = nor4(bCLK_256, !b.ch1.NR14_STOP, b.ch1.CH1_LEN_DONE); // use_len polarity?
    /*p13.CUSO*/   next.ch1.NR11_LEN3n = not1(!b.ch1.NR11_LEN3);
    /*p13.BUGY*/   next.ch1.FF11_WRc = not1(b.ch1.FF11_WRna);
    /*p13.BEPE*/   next.ch1.FF11_WRa = not1(b.ch1.FF11_WRna);
    /*p13.BORO*/     next.ch1.FF11_WRna = nand2(APU_WR, ADDR_FF11);

    /*p13.CERO*/ next.ch1.CH1_LEN_DONE = tock_pos(!a.ch1.NR11_LEN5, !b.ch1.NR11_LEN5, b.ch1.CH1_LEN_DONE_RST, b.ch1.CH1_LEN_DONE, !b.ch1.CH1_LEN_DONE);
    /*p13.CORY*/   next.ch1.CH1_LEN_DONE_RST = nor4(b.ch1.RESTART0, b.apu.APU_RESET1, b.ch1.FF11_WRb);
    /*p13.BOKA*/     next.ch1.FF11_WRb = not1(b.ch1.FF11_WRna);
  }

  //----------
  // Restart sequencer

  {
    // FIXME
    //wire aCLK_1M  = a.clk.DOVA_ABCDxxxx;
    //wire bCLK_1M  = b.clk.DOVA_ABCDxxxx;
    wire aCLK_1M  = dff20.DOVA_ABCDxxxx;
    wire bCLK_1M  = dff20.DOVA_ABCDxxxx;

    /*p13.EZEC*/ next.ch1.START = tock_pos(aCLK_1M, bCLK_1M, /*p13.DUKA*/ not1(b.apu.APU_RESET1), b.ch1.START, b.ch1.NR14_START);


    /*p13.GEFE*/ next.ch1.RESTART0_RSTn = not1(/*p13.EGET*/ nor4(b.apu.APU_RESET1, b.ch1.RESTART1));
    /*p13.ERUM*/ next.ch1.RESTART_RSTo = not1(b.apu.APU_RESET1);

    // weird latch?
    /*p13.FYFO*/ wire FYFO = or2(b.ch1.RESTART0_RSTn, b.ch1.START);

    /*p09.AGUR*/ wire RESETn = not1(b.apu.APU_RESET1);

    // FIX CLOCK HERE
    wire aCLK_2M  = a.apu.AJER_2M;
    wire bCLK_2M  = b.apu.AJER_2M;

    /*p01.BATA*/ wire aCLK_2Mn = not1(aCLK_2M);
    /*p01.BATA*/ wire bCLK_2Mn = not1(bCLK_2M);
    /*p09.CALO*/ next.ch1.RESTART_CLK = tock_pos(aCLK_2Mn, bCLK_2Mn, RESETn, b.ch1.RESTART_CLK, !b.ch1.RESTART_CLK);

    /*p09.DYFA*/ next.ch1.RESTART_CLKn = not1(!b.ch1.RESTART_CLK);
    /*p13.FEKU*/ next.ch1.RESTART0 = tock_pos(a.ch1.RESTART_CLKn, b.ch1.RESTART_CLKn, b.ch1.RESTART0_RSTn, b.ch1.RESTART0, FYFO);
    /*p13.FARE*/ next.ch1.RESTART1 = tock_pos(a.ch1.RESTART_CLKn, b.ch1.RESTART_CLKn, b.ch1.RESTART_RSTo,  b.ch1.RESTART1, b.ch1.RESTART0);
    /*p13.FYTE*/ next.ch1.RESTART2 = tock_pos(a.ch1.RESTART_CLKn, b.ch1.RESTART_CLKn, b.ch1.RESTART_RSTo,  b.ch1.RESTART2, b.ch1.RESTART1);
  }

  //----------
  // Waveform generator

  {
    /*p13.CALA*/ next.ch1.PHASE_CLK_CLK = not1(b.ch1.FREQ_CNT_10);
    /*p13.DYRU*/ wire PHASE_CLK_RST = nor4(b.apu.APU_RESET1, b.ch1.RESTART0, /*p13.DOKA*/ and2(b.ch1.PHASE_CLKnb, b.ch1.RESTART_CLKn));

    /*p13.COMY*/ next.ch1.PHASE_CLKnb = tock_pos(a.ch1.PHASE_CLK_CLK, b.ch1.PHASE_CLK_CLK, PHASE_CLK_RST, b.ch1.PHASE_CLKnb, !b.ch1.PHASE_CLKnb);
    /*p13.COPE*/ next.ch1.PHASE_CLKna = not1(/*p13.CYTE*/ not1(b.ch1.PHASE_CLKnb));

    // weird latch?
    /*p13.GEXU*/ next.ch1.FREQ_GATE = or2(/*p13.FEMY*/ nor4(b.ch1.CH1_AMP_ENn, b.apu.APU_RESET1), /*p13.GEPU*/ not1(b.ch1.RESTART2));

    /*p11.EPYK*/ wire FREQ_RSTn = nor4(b.ch1.RESTART0, b.ch1.PHASE_CLKna);
    /*p11.FUME*/ wire FREQ_RSTa = not1(FREQ_RSTn);
    /*p11.DEGA*/ wire FREQ_RSTb = not1(FREQ_RSTn);
    /*p11.DAKO*/ wire FREQ_RSTc = not1(FREQ_RSTn);

    /*p11.GEKU*/ next.ch1.FREQ_CLKa = not1(/*p11.FULO*/ nor4(b.ch1.RESTART_CLKn, b.ch1.FREQ_GATE));
    /*p11.KYPE*/ next.ch1.FREQ_CLKb = not1(b.ch1.FREQ_CNT_03);
    /*p11.DERU*/ next.ch1.FREQ_CLKc = not1(b.ch1.FREQ_CNT_07);

    /*p11.GAXE*/ next.ch1.FREQ_CNT_00 = count_pos(a.ch1.FREQ_CLKa,   b.ch1.FREQ_CLKa,   FREQ_RSTa, b.ch1.FREQ_CNT_00, b.ch1.SUMMER00);
    /*p11.HYFE*/ next.ch1.FREQ_CNT_01 = count_pos(a.ch1.FREQ_CNT_00, b.ch1.FREQ_CNT_00, FREQ_RSTa, b.ch1.FREQ_CNT_01, b.ch1.SUMMER01);
    /*p11.JYTY*/ next.ch1.FREQ_CNT_02 = count_pos(a.ch1.FREQ_CNT_01, b.ch1.FREQ_CNT_01, FREQ_RSTa, b.ch1.FREQ_CNT_02, b.ch1.SUMMER02);
    /*p11.KYNA*/ next.ch1.FREQ_CNT_03 = count_pos(a.ch1.FREQ_CNT_02, b.ch1.FREQ_CNT_02, FREQ_RSTa, b.ch1.FREQ_CNT_03, b.ch1.SUMMER03);
    /*p11.JEMA*/ next.ch1.FREQ_CNT_04 = count_pos(a.ch1.FREQ_CLKb,   b.ch1.FREQ_CLKb,   FREQ_RSTb, b.ch1.FREQ_CNT_04, b.ch1.SUMMER04);
    /*p11.HYKE*/ next.ch1.FREQ_CNT_05 = count_pos(a.ch1.FREQ_CNT_04, b.ch1.FREQ_CNT_04, FREQ_RSTb, b.ch1.FREQ_CNT_05, b.ch1.SUMMER05);
    /*p11.FEVA*/ next.ch1.FREQ_CNT_06 = count_pos(a.ch1.FREQ_CNT_05, b.ch1.FREQ_CNT_05, FREQ_RSTb, b.ch1.FREQ_CNT_06, b.ch1.SUMMER06);
    /*p11.EKOV*/ next.ch1.FREQ_CNT_07 = count_pos(a.ch1.FREQ_CNT_06, b.ch1.FREQ_CNT_06, FREQ_RSTb, b.ch1.FREQ_CNT_07, b.ch1.SUMMER07);
    /*p11.EMUS*/ next.ch1.FREQ_CNT_08 = count_pos(a.ch1.FREQ_CLKc,   b.ch1.FREQ_CLKc,   FREQ_RSTc, b.ch1.FREQ_CNT_08, b.ch1.SUMMER08);
    /*p11.EVAK*/ next.ch1.FREQ_CNT_09 = count_pos(a.ch1.FREQ_CNT_09, b.ch1.FREQ_CNT_09, FREQ_RSTc, b.ch1.FREQ_CNT_09, b.ch1.SUMMER09);
    /*p11.COPU*/ next.ch1.FREQ_CNT_10 = count_pos(a.ch1.FREQ_CNT_10, b.ch1.FREQ_CNT_10, FREQ_RSTc, b.ch1.FREQ_CNT_10, b.ch1.SUMMER10);

    /*p11.CEPO*/ wire RESET1n = not1(b.apu.APU_RESET1);

    /*p13.DAJO*/ next.ch1.PHASE_CLKa = not1(b.ch1.PHASE_CLKna);
    /*p13.ESUT*/ next.ch1.PHASE_xBxDxFxH = tock_pos( a.ch1.PHASE_CLKa,     b.ch1.PHASE_CLKa,      RESET1n, b.ch1.PHASE_xBxDxFxH, !b.ch1.PHASE_xBxDxFxH);
    /*p13.EROS*/ next.ch1.PHASE_xxCDxxGH = tock_pos(!a.ch1.PHASE_xBxDxFxH, !b.ch1.PHASE_xBxDxFxH, RESET1n, b.ch1.PHASE_xxCDxxGH, !b.ch1.PHASE_xxCDxxGH);
    /*p13.DAPE*/ next.ch1.PHASE_xxxxEFGH = tock_pos(!a.ch1.PHASE_xxCDxxGH, !b.ch1.PHASE_xxCDxxGH, RESET1n, b.ch1.PHASE_xxxxEFGH, !b.ch1.PHASE_xxxxEFGH);

    /*p13.DUVO*/ wire PHASE_AxCxExGx = not1(b.ch1.PHASE_xBxDxFxH);
    /*p13.EZOZ*/ wire PHASE_xxxxxxGH = and2(b.ch1.PHASE_xxCDxxGH, b.ch1.PHASE_xxxxEFGH);
    /*p13.ENEK*/ wire PHASE_xxxxxxGx = and2(PHASE_AxCxExGx, PHASE_xxxxxxGH);
    /*p13.CODO*/ wire PHASE_ABCDEFxx = not1(PHASE_xxxxxxGH);

    /*p13.DUNA*/ wire RAW_BIT = amux4(PHASE_xxxxxxGx,    /*p13.COSO*/ nor4( b.ch1.NR11_DUTY_0,  b.ch1.NR11_DUTY_1),
                                      PHASE_xxxxxxGH,    /*p13.CAVA*/ nor4(!b.ch1.NR11_DUTY_0,  b.ch1.NR11_DUTY_1),
                                      b.ch1.PHASE_xxxxEFGH, /*p13.CEVU*/ nor4( b.ch1.NR11_DUTY_0, !b.ch1.NR11_DUTY_1),
                                      PHASE_ABCDEFxx,    /*p13.CAXO*/ nor4(!b.ch1.NR11_DUTY_0, !b.ch1.NR11_DUTY_1));
    /*p13.DUWO*/ next.ch1.RAW_BIT_SYNC = tock_pos(a.ch1.PHASE_CLKna, b.ch1.PHASE_CLKna, RESET1n, b.ch1.RAW_BIT_SYNC, RAW_BIT);
  }

  //----------
  // Envelope generator. I'm not entirely sure how this works...

  {
    /*p13.KUXU*/ wire ENV_DELAY_LOAD = not1(/*p13.KAZA*/ nor4(b.ch1.RESTART0, b.ch1.ENV_CLK));

    wire bCLK_128 = b.apu.CLK_128a;

    // dis not rite
    /*p13.JONE*/ next.ch1.CLK_128n = not1(bCLK_128);
    /*p13.KALY*/ next.ch1.CLK_64a  = tock_pos(a.ch1.CLK_128n, b.ch1.CLK_128n, /*p13.KADO*/ not1(b.apu.APU_RESET1), b.ch1.CLK_64a, !b.ch1.CLK_64a);
    ///*p13.JOLA*/ wire CLK_64b  = not1(/*p13.KERE*/ not1(b.ch1.CLK_64a));
    /*p13.KERE*/ wire KERE = not1(b.ch1.CLK_64a);
    /*p13.JOLA*/ wire CLK_64b = not1(KERE);

    // fix clock here
    /*p13.JOVA*/ next.ch1.ENV_DELAY0 = count_pos(a.ch1.CLK_64a,    CLK_64b,       ENV_DELAY_LOAD, b.ch1.ENV_DELAY0, !b.ch1.NR12_DELAY0);
    /*p13.KENU*/ next.ch1.ENV_DELAY1 = count_pos(a.ch1.ENV_DELAY0, b.ch1.ENV_DELAY0, ENV_DELAY_LOAD, b.ch1.ENV_DELAY1, !b.ch1.NR12_DELAY1);
    /*p13.KERA*/ next.ch1.ENV_DELAY2 = count_pos(a.ch1.ENV_DELAY1, b.ch1.ENV_DELAY1, ENV_DELAY_LOAD, b.ch1.ENV_DELAY2, !b.ch1.NR12_DELAY2);
    /*p13.KOTE*/ wire ENV_TRIGGER = and2(b.ch1.ENV_DELAY0, b.ch1.ENV_DELAY1, b.ch1.ENV_DELAY2);

    /*p13.HAKE*/ wire ENV_TOP = not1(/*p13.HUFU*/ nand2(b.ch1.NR12_ENV_DIR, b.ch1.ENV0, b.ch1.ENV1, b.ch1.ENV2, b.ch1.ENV3));
    /*p13.HANO*/ wire ENV_BOT = nor4(b.ch1.NR12_ENV_DIR, b.ch1.ENV0, b.ch1.ENV1, b.ch1.ENV2, b.ch1.ENV3);
    /*p13.JADE*/ wire ENV_MAX = or2(ENV_TOP, ENV_BOT);

    /*p13.KOMA*/ wire ENV_EN = nor4(b.ch1.NR12_DELAY0, b.ch1.NR12_DELAY1, b.ch1.NR12_DELAY2);

    wire aCLK_512 = a.apu.CLK_512a;
    wire bCLK_512 = b.apu.CLK_512a;

    /*p13.KORO*/ wire ENV_CLK_RST = nor4(/*p13.KUKU*/ nor4(aCLK_512, /*p13.KURY*/ not1(b.ch1.ENV_CLK)), ENV_EN); // schematic wrong

    /*p13.KOZY*/ next.ch1.ENV_CLK  = tock_pos(aCLK_512, bCLK_512, ENV_CLK_RST, b.ch1.ENV_CLK,  ENV_TRIGGER);
    /*p13.KYNO*/ next.ch1.ENV_STOP = tock_pos(a.ch1.ENV_CLK, b.ch1.ENV_CLK, /*p13.KORU*/ nor4(b.ch1.RESTART0, b.apu.APU_RESET1), b.ch1.ENV_STOP, ENV_MAX);

    /*p13.KEKO*/ wire KEKO = or2(b.apu.APU_RESET1, b.ch1.RESTART0);
    
    // weird latch?
    /*p13.KEZU*/ wire KEZU = or2(b.ch1.ENV_STOP, KEKO);

    /*p13.KAKE*/ wire KAKE = and2(b.ch1.ENV_CLK, ENV_EN, KEZU);

    /*p13.HESU*/ next.ch1.ENV_DELTA0 = amux2(b.ch1.NR12_ENV_DIR, b.ch1.ENV1, !b.ch1.ENV1, !b.ch1.NR12_ENV_DIR);
    /*p13.HETO*/ next.ch1.ENV_DELTA1 = amux2(b.ch1.NR12_ENV_DIR, b.ch1.ENV2, !b.ch1.ENV2, !b.ch1.NR12_ENV_DIR);
    /*p13.HYTO*/ next.ch1.ENV_DELTA2 = amux2(b.ch1.NR12_ENV_DIR, b.ch1.ENV3, !b.ch1.ENV3, !b.ch1.NR12_ENV_DIR);
    /*p13.JUFY*/ next.ch1.ENV_DELTA3 = amux2(b.ch1.NR12_ENV_DIR, KAKE,    KAKE,  !b.ch1.NR12_ENV_DIR);

    /*p13.HEVO*/ next.ch1.ENV0 = count_pos(a.ch1.ENV_DELTA0, b.ch1.ENV_DELTA0, b.ch1.RESTART0, b.ch1.ENV0, b.ch1.NR12_VOL0);
    /*p13.HOKO*/ next.ch1.ENV1 = count_pos(a.ch1.ENV_DELTA1, b.ch1.ENV_DELTA1, b.ch1.RESTART0, b.ch1.ENV1, b.ch1.NR12_VOL1);
    /*p13.HEMY*/ next.ch1.ENV2 = count_pos(a.ch1.ENV_DELTA2, b.ch1.ENV_DELTA2, b.ch1.RESTART0, b.ch1.ENV2, b.ch1.NR12_VOL2);
    /*p13.HAFO*/ next.ch1.ENV3 = count_pos(a.ch1.ENV_DELTA3, b.ch1.ENV_DELTA3, b.ch1.RESTART0, b.ch1.ENV3, b.ch1.NR12_VOL3);
  }

  //----------
  // Audio output

  /*p13.BERY*/ wire CH1_STOP = or2(/*p13.BONE*/ not1(b.ch1.FREQ_OVERFLOW),
                                  b.apu.APU_RESET1,
                                  /*p13.CYFA*/ and2(b.ch1.CH1_LEN_DONE, b.ch1.NR14_STOP),
                                  b.ch1.CH1_AMP_ENn); // polarity

  /*p13.CYTO*/ next.ch1.CH1_ACTIVE = or2(b.ch1.RESTART0, CH1_STOP); // polarity

  /*p13.BOTO*/ wire BIT_OUT = or2(/*p13.COWE*/ and2(b.ch1.CH1_ACTIVE, b.ch1.RAW_BIT_SYNC), b.apu.NR52_DBG_APU);
  /*p13.AMOP*/ next.ch1.CH1_OUT0 = and2(b.ch1.ENV0, BIT_OUT);
  /*p13.ASON*/ next.ch1.CH1_OUT1 = and2(b.ch1.ENV1, BIT_OUT);
  /*p13.AGOF*/ next.ch1.CH1_OUT2 = and2(b.ch1.ENV2, BIT_OUT);
  /*p13.ACEG*/ next.ch1.CH1_OUT3 = and2(b.ch1.ENV3, BIT_OUT);
}

#endif

//-----------------------------------------------------------------------------

};