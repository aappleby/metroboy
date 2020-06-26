#include "Sch_Channel4.h"

namespace Schematics {

//-----------------------------------------------------------------------------

#if 0

void Channel4_tick(const Gameboy& a, const Gameboy& b, Gameboy& next) {

  //----------
  // FF20 NR41 - the length register is also the length timer

  {
    /*p10.DONA*/ wire ADDR_0000bn = nand(b.apu.ADDR_0xxx, b.apu.ADDR_x0xx, b.apu.ADDR_xx0x, b.apu.ADDR_xxx0);
    /*p10.DANU*/ wire FF20a    = nor(b.apu.ADDR_FF2Xn, ADDR_0000bn);
    /*p19.CAZE*/ wire FF20_WRn = nand(b.apu.APU_WR, FF20a);
    /*p19.FURU*/ wire FF20_WRa = not(FF20_WRn);
    /*p19.DOTU*/ wire FF20_WRb = not(FF20_WRn);
    /*p19.EPEK*/ wire FF20_WRc = not(FF20_WRn);

    /*p09.KUBY*/ wire RSTn = not(b.apu.APU_RST);
    /*p09.KEBA*/ wire RSTa = not(RSTn);
    /*p19.GAPY*/ wire LEN_RSTn = nor(FF20_WRa, RSTa, b.ch4.RESTART1);

    /*p19.DODA*/ wire LEN_CLKn = nor(b.apu.CLK_256a, b.ch4.LEN_STOP, b.ch4.NR43_FREQ2); // this doesn't seem right
    
    /*p19.CUWA*/ next.ch4.LEN_CLKa   = not(LEN_CLKn);
    /*p19.DOPU*/ next.ch4.NR41_LEN3b = not(!b.ch4.NR41_LEN3);                     
    /*p19.DANO*/ next.ch4.NR41_LEN0  = count_pos(a.ch4.LEN_CLKa,   b.ch4.LEN_CLKa,   FF20_WRb, b.ch4.NR41_LEN0, b.bus.D0());
    /*p19.FAVY*/ next.ch4.NR41_LEN1  = count_pos(a.ch4.NR41_LEN0,  b.ch4.NR41_LEN0,  FF20_WRb, b.ch4.NR41_LEN1, b.bus.D1());
    /*p19.DENA*/ next.ch4.NR41_LEN2  = count_pos(a.ch4.NR41_LEN1,  b.ch4.NR41_LEN1,  FF20_WRb, b.ch4.NR41_LEN2, b.bus.D2());
    /*p19.CEDO*/ next.ch4.NR41_LEN3  = count_pos(a.ch4.NR41_LEN2,  b.ch4.NR41_LEN2,  FF20_WRb, b.ch4.NR41_LEN3, b.bus.D3());
    /*p19.FYLO*/ next.ch4.NR41_LEN4  = count_pos(a.ch4.NR41_LEN3b, b.ch4.NR41_LEN3b, FF20_WRc, b.ch4.NR41_LEN4, b.bus.D4());
    /*p19.EDOP*/ next.ch4.NR41_LEN5  = count_pos(a.ch4.NR41_LEN4,  b.ch4.NR41_LEN4,  FF20_WRc, b.ch4.NR41_LEN5, b.bus.D5());
    /*p19.FUGO*/ next.ch4.LEN_STOP   = tock_pos(!a.ch4.NR41_LEN5,  !b.ch4.NR41_LEN5, LEN_RSTn, b.ch4.LEN_STOP,  !b.ch4.LEN_STOP);

    // No read circuit for FF20?
  }

  //----------
  // FF21

  {
    /*p10.DEWA*/ wire ADDR_0001bn = nand(b.apu.ADDR_0xxx, b.apu.ADDR_x0xx, b.apu.ADDR_xx0x, b.apu.ADDR_xxx1); 
    /*p10.COVO*/ wire FF21a    = nor(b.apu.ADDR_FF2Xn, ADDR_0001bn);
    /*p19.BOFY*/ wire FF21n    = not(FF21a);
    /*p19.GONY*/ wire FF21o    = not(FF21a);
    /*p19.DACO*/ wire FF21_WRa = and(b.apu.APU_WR, FF21a);
    /*p19.GOKO*/ wire FF21_WRb = and(b.apu.APU_WR, FF21a);
    /*p09.AGUZ*/ wire CPU_RDn = not(b.ctl.TEDO_CPURD);
    /*p19.BOXE*/ wire FF21_RDa = or(CPU_RDn, FF21n); // polarity?
    /*p19.HASU*/ wire FF21_RDb = or(CPU_RDn, FF21o);

    /*p09.KUBY*/ wire RSTn = not(b.apu.APU_RST);
    /*p09.KEBA*/ wire RSTa = not(RSTn);
    /*p19.FEXO*/ wire RSTq = not(RSTa);

    /*p19.DYKE*/ next.ch4.FF21_WRn = not(FF21_WRa);
    /*p19.FUPA*/ next.ch4.FF21_WRo = not(FF21_WRb);
    /*p19.EMOK*/ next.ch4.NR42_ENV_TIMER0 = tock_pos(a.ch4.FF21_WRn, b.ch4.FF21_WRn, RSTq, b.ch4.NR42_ENV_TIMER0, b.bus.D0());
    /*p19.ETYJ*/ next.ch4.NR42_ENV_TIMER1 = tock_pos(a.ch4.FF21_WRn, b.ch4.FF21_WRn, RSTq, b.ch4.NR42_ENV_TIMER1, b.bus.D1());
    /*p19.EZYK*/ next.ch4.NR42_ENV_TIMER2 = tock_pos(a.ch4.FF21_WRn, b.ch4.FF21_WRn, RSTq, b.ch4.NR42_ENV_TIMER2, b.bus.D2());
    /*p19.GEKY*/ next.ch4.NR42_ENV_DIR    = tock_pos(a.ch4.FF21_WRo, b.ch4.FF21_WRo, RSTq, b.ch4.NR42_ENV_DIR,    b.bus.D3());
    /*p19.GARU*/ next.ch4.NR42_ENV_VOL0   = tock_pos(a.ch4.FF21_WRo, b.ch4.FF21_WRo, RSTq, b.ch4.NR42_ENV_VOL0,   b.bus.D4());
    /*p19.GOKY*/ next.ch4.NR42_ENV_VOL1   = tock_pos(a.ch4.FF21_WRo, b.ch4.FF21_WRo, RSTq, b.ch4.NR42_ENV_VOL1,   b.bus.D5());
    /*p19.GOZO*/ next.ch4.NR42_ENV_VOL2   = tock_pos(a.ch4.FF21_WRo, b.ch4.FF21_WRo, RSTq, b.ch4.NR42_ENV_VOL2,   b.bus.D6());
    /*p19.GEDU*/ next.ch4.NR42_ENV_VOL3   = tock_pos(a.ch4.FF21_WRo, b.ch4.FF21_WRo, RSTq, b.ch4.NR42_ENV_VOL3,   b.bus.D7());

    if (FF21_RDa) next.bus.set_data(
      /*p19.DEMY*/ b.ch4.NR42_ENV_TIMER0,
      /*p19.COCE*/ b.ch4.NR42_ENV_TIMER1,
      /*p19.CUZU*/ b.ch4.NR42_ENV_TIMER2,
      /*p19.GOME*/ b.ch4.NR42_ENV_DIR,
      /*p19.HEDA*/ b.ch4.NR42_ENV_VOL0,
      /*p19.GODU*/ b.ch4.NR42_ENV_VOL1,
      /*p19.HOGE*/ b.ch4.NR42_ENV_VOL2,
      /*p19.HACU*/ b.ch4.NR42_ENV_VOL3
    );
  }

  //----------
  // FF22

  {
    /*p10.DOFA*/ wire ADDR_0010bn = nand(b.apu.ADDR_0xxx, b.apu.ADDR_x0xx, b.apu.ADDR_xx1x, b.apu.ADDR_xxx0);
    /*p10.EKEZ*/ wire FF22a = nor(b.apu.ADDR_FF2Xn, ADDR_0010bn);
    /*p19.KOKU*/ wire FF22n = not(FF22a);
    /*p19.GUGO*/ wire FF22o = not(FF22a);

    /*p19.HUMO*/ wire FF22_WRa = and(FF22a, b.apu.APU_WR);
    /*p19.GETU*/ wire FF22_WRn = nand(FF22a, b.apu.APU_WR);
    
    /*p09.AGUZ*/ wire CPU_RDn = not(b.ctl.TEDO_CPURD);
    /*p19.KEKA*/ wire FF22_RDa = or(CPU_RDn, FF22n);

    /*p19.KAGE*/ wire CPU_RDa = not(CPU_RDn);
    /*p19.JORA*/ wire FF22_RDb = nand(FF22a, CPU_RDa);
    /*p19.HEZE*/ wire FF22_RDc = or(CPU_RDn, FF22o);

    /*p09.KUBY*/ wire RSTn = not(b.apu.APU_RST);
    /*p09.KEBA*/ wire RSTa = not(RSTn);
    /*p09.KAME*/ wire RSTp = not(RSTa);
    /*p19.HYNE*/ wire RSTr = not(RSTa);
    /*p09.DAPA*/ wire RSTo = not(RSTa);

    /*p19.HOVA*/ next.ch4.FF22_WRo = not (FF22_WRa);
    /*p19.HOSO*/ next.ch4.FF22_WRp = nand(FF22a, b.apu.APU_WR);
    /*p19.EFUG*/ next.ch4.FF22_WRq = not (FF22_WRn);
    /*p19.JARE*/ next.ch4.NR43_DIV0  = tock_pos(a.ch4.FF22_WRo, b.ch4.FF22_WRo, RSTp, b.ch4.NR43_DIV0,  b.bus.D0());
    /*p19.JERO*/ next.ch4.NR43_DIV1  = tock_pos(a.ch4.FF22_WRo, b.ch4.FF22_WRo, RSTp, b.ch4.NR43_DIV1,  b.bus.D1());
    /*p19.JAKY*/ next.ch4.NR43_DIV2  = tock_pos(a.ch4.FF22_WRo, b.ch4.FF22_WRo, RSTp, b.ch4.NR43_DIV2,  b.bus.D2());
    /*p19.JAMY*/ next.ch4.NR43_MODE  = tock_pos(a.ch4.FF22_WRp, b.ch4.FF22_WRp, RSTr, b.ch4.NR43_MODE,  b.bus.D3());
    /*p19.FETA*/ next.ch4.NR43_FREQ0 = tock_pos(a.ch4.FF22_WRq, b.ch4.FF22_WRq, RSTo, b.ch4.NR43_FREQ0, b.bus.D4());
    /*p19.FYTO*/ next.ch4.NR43_FREQ1 = tock_pos(a.ch4.FF22_WRq, b.ch4.FF22_WRq, RSTo, b.ch4.NR43_FREQ1, b.bus.D5());
    /*p19.GOGO*/ next.ch4.NR43_FREQ2 = tock_pos(a.ch4.FF22_WRq, b.ch4.FF22_WRq, RSTo, b.ch4.NR43_FREQ2, b.bus.D6());
    /*p19.GAFO*/ next.ch4.NR43_FREQ3 = tock_pos(a.ch4.FF22_WRq, b.ch4.FF22_WRq, RSTo, b.ch4.NR43_FREQ3, b.bus.D7());

    if (FF22_RDa) next.bus.set_data(
      /*p19.KAMO*/ b.ch4.NR43_DIV0,
      /*p19.KAKU*/ b.ch4.NR43_DIV1,
      /*p19.KYRO*/ b.ch4.NR43_DIV2,
      /*p19.KETA*/ b.ch4.NR43_MODE,
      /*p19.GEDA*/ b.ch4.NR43_FREQ0,
      /*p19.GYPE*/ b.ch4.NR43_FREQ1,
      /*p19.GAKA*/ b.ch4.NR43_FREQ2,
      /*p19.HAPY*/ b.ch4.NR43_FREQ3
    );
  }

  //----------
  // FF23. Some weird debug voodoo here.

  {
    /*p09.AGUZ*/ wire CPU_RDn = not(b.ctl.TEDO_CPURD);
    /*p19.BYLO*/ wire CPU_RDb = not(CPU_RDn);

    /*p10.DUFE*/ wire ADDR_0011bn = nand(b.apu.ADDR_0xxx, b.apu.ADDR_x0xx, b.apu.ADDR_xx1x, b.apu.ADDR_xxx1); 
    
    /*p10.CUGE*/ wire FF23a    = nor(b.apu.ADDR_FF2Xn, ADDR_0011bn);
    /*p19.BARE*/ wire FF23_RDa = nand(FF23a, CPU_RDb); // polarity?

    /*p09.KUBY*/ wire RSTn = not(b.apu.APU_RST);
    /*p09.KEBA*/ wire RSTa = not(RSTn);
    /*p19.CABE*/ wire RSTs = not(RSTa);

    /*p16.ANUJ*/ wire CPU_WR_WEIRD = and(b.cpu.FROM_CPU5, b.apu.APU_WR);
    /*p19.DULU*/ next.ch4.FF23_WRn = nand(CPU_WR_WEIRD, FF23a);
    /*p19.CUNY*/ next.ch4.NR44_STOP  = tock_pos(a.ch4.FF23_WRn, b.ch4.FF23_WRn, RSTs, b.ch4.NR44_STOP,  b.bus.D6());

    /*p20.GUZY*/ wire NR44_START_RST = nor(b.ch4.CH4_START, RSTa);
    /*p19.FOXE*/ next.ch4.FF23_WRo = nand(b.apu.APU_WR, FF23a);
    /*p19.HOGA*/ next.ch4.NR44_START = tock_pos(a.ch4.FF23_WRo, b.ch4.FF23_WRo, NR44_START_RST, b.ch4.NR44_START, b.bus.D7());

    if (FF23_RDa) next.bus.set_data(
      0,
      0,
      0,
      0,
      0,
      0,
      /*p19.CURY*/ b.ch4.NR44_STOP
    );
  }

  //----------
  // Control

  {
    /*p09.KUBY*/ wire RSTn = not(b.apu.APU_RST);
    /*p09.KEBA*/ wire RSTa = not(RSTn);
    /*p20.FEBY*/ wire RSTt = not(RSTa);
    /*p20.GASO*/ wire RSTu = not(RSTa);

    // FIXME
    ///*p20.GYSU*/ next.ch4.CH4_START   = tock_pos(a.clk.DOVA_ABCDxxxx, b.clk.DOVA_ABCDxxxx, RSTu, b.ch4.CH4_START, b.ch4.NR44_START);

    /*p20.EFOT*/ wire CH4_STOP    = and(b.ch4.NR44_STOP,   b.ch4.LEN_STOP);
    /*p20.FEGY*/ wire CH4_OFF     = or(b.ch4.CH4_AMP_ENn, CH4_STOP, RSTa);

    // weird latch?
    /*p20.GENA*/ next.ch4.CH4_ACTIVE  = or(b.ch4.RESTART1, CH4_OFF);

    /*p20.FALE*/ wire RESTART_RSTn = nor(b.ch4.RESTART2, RSTa);
    /*p20.HELU*/ wire RESTART_RST  = not(RESTART_RSTn);

    // weird latch?
    /*p20.HAZO*/ wire RESTART_IN   = or(RESTART_RST, b.ch4.CH4_START);

    // one of these is wrong, right now we would stop the div clock when ch4 active
    // fixed kyku but still might be a polarity error?

    /*p20.GONE*/ next.ch4.RESTART1 = tock_pos(a.apu.CLK_512Kn, b.apu.CLK_512Kn, RESTART_RSTn, b.ch4.RESTART1, RESTART_IN);
    /*p20.GORA*/ next.ch4.RESTART2 = tock_pos(a.apu.CLK_512Kn, b.apu.CLK_512Kn, RSTt,         b.ch4.RESTART2, b.ch4.RESTART1);
    /*p20.GATY*/ next.ch4.RESTART3 = tock_pos(a.apu.CLK_512Kn, b.apu.CLK_512Kn, RSTt,         b.ch4.RESTART3, b.ch4.RESTART2);

    /*p20.HERY*/ wire DIV_GATE1 = nor(RSTa, b.ch4.CH4_AMP_ENn);
    /*p20.HAPU*/ wire DIV_GATE2 = not(b.ch4.RESTART3);

    // weird latch?
    /*p20.JERY*/ wire DIV_GATE3 = or(DIV_GATE1, DIV_GATE2);
    /*p20.KYKU*/ wire DIV_CLKb  = nor(b.apu.CLK_512Ka, DIV_GATE3); // schematic wrong
    /*p20.KONY*/ wire DIV_CLKn  = not(DIV_CLKb);
    /*p20.GOFU*/ wire DIV_LOADn = nor(b.ch4.RESTART1, b.ch4.FREQ_GATEn);
    /*p20.HUCE*/ wire DIV_LOAD  = not(DIV_LOADn);

    /*p20.KANU*/ next.ch4.DIV_CLKa  = not(DIV_CLKn);
    /*p20.JYCO*/ next.ch4.DIV0    = count_pos(a.ch4.DIV_CLKa, b.ch4.DIV_CLKa, DIV_LOAD, b.ch4.DIV0, !b.ch4.NR43_DIV0);
    /*p20.JYRE*/ next.ch4.DIV1    = count_pos(a.ch4.DIV0,     b.ch4.DIV0,     DIV_LOAD, b.ch4.DIV1, !b.ch4.NR43_DIV1);
    /*p20.JYFU*/ next.ch4.DIV2    = count_pos(a.ch4.DIV1,     b.ch4.DIV1,     DIV_LOAD, b.ch4.DIV2, !b.ch4.NR43_DIV2);
    /*p20.HYNO*/ next.ch4.DIV_MAX = and(b.ch4.DIV0, b.ch4.DIV1,  b.ch4.DIV2); // schematic wrong
  }

  //----------
  // Debug

  {
    /*p09.AGUZ*/ wire CPU_RDn = not(b.ctl.TEDO_CPURD);
    /*p20.COSA*/ wire CPU_RDc = not(CPU_RDn);
    /*p20.CEPY*/ wire NR44_STOPn = not(b.ch4.NR44_STOP);
    /*p10.DUFE*/ wire ADDR_0011bn = nand(b.apu.ADDR_0xxx, b.apu.ADDR_x0xx, b.apu.ADDR_xx1x, b.apu.ADDR_xxx1); 
    /*p10.CUGE*/ wire FF23a    = nor(b.apu.ADDR_FF2Xn, ADDR_0011bn);

    /*p20.DYRY*/ next.ch4.DBG_CH4 = and(NR44_STOPn, b.apu.NR52_DBG_APU);
    /*p20.COMO*/ next.ch4.DBG_COMO = and(CPU_RDc, b.ch4.DBG_CH4);
    /*p20.BAGU*/ next.ch4.DBG_BAGU = nand(FF23a, b.ch4.DBG_COMO);
    /*p20.BEFA*/ next.ch4.DBG_BEFA = not(b.ch4.FREQ_CLK);

    // FIXME
    ///*p20.ATEL*/ if (b.ch4.DBG_BAGU) next.\1() = b.ch4.DBG_BEFA;
  }

  //----------
  // Frequency timer

  {
    /*p09.KUBY*/ wire RSTn = not(b.apu.APU_RST);
    /*p09.KEBA*/ wire RSTa = not(RSTn);
    /*p09.DAPA*/ wire RSTo = not(RSTa);

    /*p20.GUNY*/ wire FREQ_GATE_RSTn = nor(b.ch4.RESTART1, RSTa);
    /*p20.GYBA*/ next.ch4.FREQ_GATE_CLK  = not(b.apu.BAVU_1M);
    /*p20.GARY*/ next.ch4.FREQ_GATEn = tock_pos(a.ch4.FREQ_GATE_CLK, b.ch4.FREQ_GATE_CLK, FREQ_GATE_RSTn, b.ch4.FREQ_GATEn, b.ch4.DIV_MAX);

    /*p20.CARY*/ next.ch4.FREQ_CLK = and(b.apu.BAVU_1M, b.ch4.FREQ_GATEn);
    /*p20.CEXO*/ next.ch4.FREQ_00 = tock_pos( a.ch4.FREQ_CLK,  b.ch4.FREQ_CLK, RSTo, b.ch4.FREQ_00, !b.ch4.FREQ_00);
    /*p20.DEKO*/ next.ch4.FREQ_01 = tock_pos(!a.ch4.FREQ_00,   !b.ch4.FREQ_00, RSTo, b.ch4.FREQ_01, !b.ch4.FREQ_01);
    /*p20.EZEF*/ next.ch4.FREQ_02 = tock_pos(!a.ch4.FREQ_01,   !b.ch4.FREQ_01, RSTo, b.ch4.FREQ_02, !b.ch4.FREQ_02);
    /*p20.EPOR*/ next.ch4.FREQ_03 = tock_pos(!a.ch4.FREQ_02,   !b.ch4.FREQ_02, RSTo, b.ch4.FREQ_03, !b.ch4.FREQ_03);
    /*p20.DURE*/ next.ch4.FREQ_04 = tock_pos(!a.ch4.FREQ_03,   !b.ch4.FREQ_03, RSTo, b.ch4.FREQ_04, !b.ch4.FREQ_04);
    /*p20.DALE*/ next.ch4.FREQ_05 = tock_pos(!a.ch4.FREQ_04,   !b.ch4.FREQ_04, RSTo, b.ch4.FREQ_05, !b.ch4.FREQ_05);
    /*p20.DOKE*/ next.ch4.FREQ_06 = tock_pos(!a.ch4.FREQ_05,   !b.ch4.FREQ_05, RSTo, b.ch4.FREQ_06, !b.ch4.FREQ_06);
    /*p20.DEMO*/ next.ch4.FREQ_07 = tock_pos(!a.ch4.FREQ_06,   !b.ch4.FREQ_06, RSTo, b.ch4.FREQ_07, !b.ch4.FREQ_07);
    /*p20.DOSE*/ next.ch4.FREQ_08 = tock_pos(!a.ch4.FREQ_07,   !b.ch4.FREQ_07, RSTo, b.ch4.FREQ_08, !b.ch4.FREQ_08);
    /*p20.DETE*/ next.ch4.FREQ_09 = tock_pos(!a.ch4.FREQ_08,   !b.ch4.FREQ_08, RSTo, b.ch4.FREQ_09, !b.ch4.FREQ_09);
    /*p20.ERUT*/ next.ch4.FREQ_10 = tock_pos(!a.ch4.FREQ_09,   !b.ch4.FREQ_09, RSTo, b.ch4.FREQ_10, !b.ch4.FREQ_10);
    /*p20.DOTA*/ next.ch4.FREQ_11 = tock_pos(!a.ch4.FREQ_10,   !b.ch4.FREQ_10, RSTo, b.ch4.FREQ_11, !b.ch4.FREQ_11);
    /*p20.DERE*/ next.ch4.FREQ_12 = tock_pos(!a.ch4.FREQ_11,   !b.ch4.FREQ_11, RSTo, b.ch4.FREQ_12, !b.ch4.FREQ_12);
    /*p20.ESEP*/ next.ch4.FREQ_13 = tock_pos(!a.ch4.FREQ_12,   !b.ch4.FREQ_12, RSTo, b.ch4.FREQ_13, !b.ch4.FREQ_13);
  }


  //----------
  // The actual LFSR

  {
    // 14-to-1 mux to select LFSR clock

    /*p20.EMOF*/ wire LFSR_CLK_MUX_7 = nor(!b.ch4.NR43_FREQ0, !b.ch4.NR43_FREQ1, !b.ch4.NR43_FREQ2);
    /*p20.ELAR*/ wire LFSR_CLK_MUX_6 = nor(!b.ch4.NR43_FREQ0, !b.ch4.NR43_FREQ1,  b.ch4.NR43_FREQ2);
    /*p20.DUDU*/ wire LFSR_CLK_MUX_5 = nor(!b.ch4.NR43_FREQ0,  b.ch4.NR43_FREQ1, !b.ch4.NR43_FREQ2);
    /*p20.ETAT*/ wire LFSR_CLK_MUX_4 = nor(!b.ch4.NR43_FREQ0,  b.ch4.NR43_FREQ1,  b.ch4.NR43_FREQ2);
    /*p20.FURA*/ wire LFSR_CLK_MUX_3 = nor( b.ch4.NR43_FREQ0, !b.ch4.NR43_FREQ1, !b.ch4.NR43_FREQ2);
    /*p20.ETAR*/ wire LFSR_CLK_MUX_2 = nor( b.ch4.NR43_FREQ0, !b.ch4.NR43_FREQ1,  b.ch4.NR43_FREQ2);
    /*p20.EVER*/ wire LFSR_CLK_MUX_1 = nor( b.ch4.NR43_FREQ0,  b.ch4.NR43_FREQ1, !b.ch4.NR43_FREQ2);
    /*p20.ETOV*/ wire LFSR_CLK_MUX_0 = nor( b.ch4.NR43_FREQ0,  b.ch4.NR43_FREQ1,  b.ch4.NR43_FREQ2);

    /*p20.ETYR*/ wire LFSR_CLK_MUX_A = amux6(b.ch4.FREQ_13, LFSR_CLK_MUX_5,
                                             b.ch4.FREQ_12, LFSR_CLK_MUX_4,
                                             b.ch4.FREQ_11, LFSR_CLK_MUX_3,
                                             b.ch4.FREQ_10, LFSR_CLK_MUX_2,
                                             b.ch4.FREQ_09, LFSR_CLK_MUX_1,
                                             b.ch4.FREQ_08, LFSR_CLK_MUX_0);

    /*p20.ELYX*/ wire LFSR_CLK_MUX_B = amux4(b.ch4.FREQ_07, LFSR_CLK_MUX_7,
                                             b.ch4.FREQ_06, LFSR_CLK_MUX_6,
                                             b.ch4.FREQ_05, LFSR_CLK_MUX_5,
                                             b.ch4.FREQ_04, LFSR_CLK_MUX_4);

    /*p20.DARY*/ wire LFSR_CLK_MUX_C = amux4(b.ch4.FREQ_03, LFSR_CLK_MUX_3,
                                             b.ch4.FREQ_02, LFSR_CLK_MUX_1,
                                             b.ch4.FREQ_01, LFSR_CLK_MUX_1,
                                             b.ch4.FREQ_00, LFSR_CLK_MUX_0);

    /*p20.ERYF*/ wire LFSR_CLK_MUX_D = or(LFSR_CLK_MUX_B, LFSR_CLK_MUX_C);

    /*p09.KUBY*/ wire RSTn = not(b.apu.APU_RST);
    /*p09.KEBA*/ wire RSTa = not(RSTn);

    /*p20.GEPO*/ wire LFSR_RSTa = or(b.ch4.RESTART1, RSTa);
    /*p20.GOGE*/ wire LFSR_RSTn = not(LFSR_RSTa);

    /*p20.HURA*/ wire LFSR_IN   = xor(b.ch4.LFSR_15, b.ch4.LFSR_14);
    /*p20.KAVU*/ wire LFSR_FB   = amux2(b.ch4.LFSR_00, b.ch4.NR43_MODE, !b.ch4.NR43_MODE, b.ch4.LFSR_08);

    /*p20.FEME*/ next.ch4.LFSR_CLKa = mux2(LFSR_CLK_MUX_A, LFSR_CLK_MUX_D, b.ch4.NR43_FREQ3);
    /*p20.JYJA*/ next.ch4.LFSR_CLKn = not(b.ch4.LFSR_CLKa);
    /*p20.GUFA*/ next.ch4.LFSR_CLKo = not(b.ch4.LFSR_CLKa);
    /*p20.GYVE*/ next.ch4.LFSR_CLKb = not(b.ch4.LFSR_CLKo);
    /*p20.KARA*/ next.ch4.LFSR_CLKp = not(b.ch4.LFSR_CLKb);
    /*p20.KOPA*/ next.ch4.LFSR_CLKc = not(b.ch4.LFSR_CLKp);

    /*p20.JOTO*/ next.ch4.LFSR_00 = tock_pos(a.ch4.LFSR_CLKn, b.ch4.LFSR_CLKn, LFSR_RSTn, b.ch4.LFSR_00, LFSR_IN);
    /*p20.KOMU*/ next.ch4.LFSR_01 = tock_pos(a.ch4.LFSR_CLKc, b.ch4.LFSR_CLKc, LFSR_RSTn, b.ch4.LFSR_01, b.ch4.LFSR_00);
    /*p20.KETU*/ next.ch4.LFSR_02 = tock_pos(a.ch4.LFSR_CLKc, b.ch4.LFSR_CLKc, LFSR_RSTn, b.ch4.LFSR_02, b.ch4.LFSR_01);
    /*p20.KUTA*/ next.ch4.LFSR_03 = tock_pos(a.ch4.LFSR_CLKc, b.ch4.LFSR_CLKc, LFSR_RSTn, b.ch4.LFSR_03, b.ch4.LFSR_02);
    /*p20.KUZY*/ next.ch4.LFSR_04 = tock_pos(a.ch4.LFSR_CLKc, b.ch4.LFSR_CLKc, LFSR_RSTn, b.ch4.LFSR_04, b.ch4.LFSR_03);
    /*p20.KYWY*/ next.ch4.LFSR_05 = tock_pos(a.ch4.LFSR_CLKc, b.ch4.LFSR_CLKc, LFSR_RSTn, b.ch4.LFSR_05, b.ch4.LFSR_04);
    /*p20.JAJU*/ next.ch4.LFSR_06 = tock_pos(a.ch4.LFSR_CLKc, b.ch4.LFSR_CLKc, LFSR_RSTn, b.ch4.LFSR_06, b.ch4.LFSR_05);
    /*p20.HAPE*/ next.ch4.LFSR_07 = tock_pos(a.ch4.LFSR_CLKc, b.ch4.LFSR_CLKc, LFSR_RSTn, b.ch4.LFSR_07, b.ch4.LFSR_06);
    /*p20.JUXE*/ next.ch4.LFSR_08 = tock_pos(a.ch4.LFSR_CLKc, b.ch4.LFSR_CLKc, LFSR_RSTn, b.ch4.LFSR_08, b.ch4.LFSR_07);
    /*p20.JEPE*/ next.ch4.LFSR_09 = tock_pos(a.ch4.LFSR_CLKb, b.ch4.LFSR_CLKb, LFSR_RSTn, b.ch4.LFSR_09, LFSR_FB);
    /*p20.JAVO*/ next.ch4.LFSR_10 = tock_pos(a.ch4.LFSR_CLKb, b.ch4.LFSR_CLKb, LFSR_RSTn, b.ch4.LFSR_10, b.ch4.LFSR_09);
    /*p20.HEPA*/ next.ch4.LFSR_11 = tock_pos(a.ch4.LFSR_CLKa, b.ch4.LFSR_CLKa, LFSR_RSTn, b.ch4.LFSR_11, b.ch4.LFSR_10);
    /*p20.HORY*/ next.ch4.LFSR_12 = tock_pos(a.ch4.LFSR_CLKa, b.ch4.LFSR_CLKa, LFSR_RSTn, b.ch4.LFSR_12, b.ch4.LFSR_11);
    /*p20.HENO*/ next.ch4.LFSR_13 = tock_pos(a.ch4.LFSR_CLKa, b.ch4.LFSR_CLKa, LFSR_RSTn, b.ch4.LFSR_13, b.ch4.LFSR_12);
    /*p20.HYRO*/ next.ch4.LFSR_14 = tock_pos(a.ch4.LFSR_CLKa, b.ch4.LFSR_CLKa, LFSR_RSTn, b.ch4.LFSR_14, b.ch4.LFSR_13);
    /*p20.HEZU*/ next.ch4.LFSR_15 = tock_pos(a.ch4.LFSR_CLKa, b.ch4.LFSR_CLKa, LFSR_RSTn, b.ch4.LFSR_15, b.ch4.LFSR_14);
  }

  //----------
  // Env

  {
    /*p09.KUBY*/ wire RSTn = not(b.apu.APU_RST);
    /*p09.KEBA*/ wire RSTa = not(RSTn);

    /*p20.FOWA*/ wire ENV_OFF = nor(b.ch4.NR42_ENV_TIMER0, b.ch4.NR42_ENV_TIMER1, b.ch4.NR42_ENV_TIMER0);

    /*p20.BOKY*/ wire ENV_TIMER_CLK_RST = not(RSTa);

    /*p20.ALOP*/ next.ch4.CLK_128n = not(b.apu.CLK_128a);
    /*p20.ABEL*/ next.ch4.ENV_TIMER_CLKa  = tock_pos(a.ch4.CLK_128n, b.ch4.CLK_128n, ENV_TIMER_CLK_RST, b.ch4.ENV_TIMER_CLKa, !b.ch4.ENV_TIMER_CLKa);

    /*p20.BAWA*/ wire ENV_TIMER_CLKn  = not(b.ch4.ENV_TIMER_CLKa);
    /*p20.ENEC*/ wire ENV_TIMER_LOADn = nor(b.ch4.RESTART1, b.ch4.ENV_PULSE);
    /*p20.DAPY*/ wire ENV_TIMER_LOAD  = not(ENV_TIMER_LOADn);

    /*p20.BUXO*/ next.ch4.ENV_TIMER_CLKb  = not(ENV_TIMER_CLKn);
    /*p20.CUNA*/ next.ch4.ENV_TIMER0      = count_pos(a.ch4.ENV_TIMER_CLKb, b.ch4.ENV_TIMER_CLKb, ENV_TIMER_LOAD, b.ch4.ENV_TIMER0, !b.ch4.NR42_ENV_TIMER0);
    /*p20.COFE*/ next.ch4.ENV_TIMER1      = count_pos(a.ch4.ENV_TIMER0,     b.ch4.ENV_TIMER0,     ENV_TIMER_LOAD, b.ch4.ENV_TIMER1, !b.ch4.NR42_ENV_TIMER1);
    /*p20.DOGO*/ next.ch4.ENV_TIMER2      = count_pos(a.ch4.ENV_TIMER1,     b.ch4.ENV_TIMER1,     ENV_TIMER_LOAD, b.ch4.ENV_TIMER2, !b.ch4.NR42_ENV_TIMER2);

    /*p20.EJEX*/ wire ENV_TIMER_MAX   = and(b.ch4.ENV_TIMER0, b.ch4.ENV_TIMER1, b.ch4.ENV_TIMER2);

    // Generates a 1 usec pulse when the env timer hits 111
    /*p20.GEXE*/ wire ENV_PULSEn     = not(b.ch4.ENV_PULSE);
    /*p20.HURY*/ wire ENV_PULSE_RST1 = nor(b.apu.CLK_512a, ENV_PULSEn);
    /*p20.GOPA*/ wire ENV_PULSE_RST2 = nor(ENV_PULSE_RST1, ENV_OFF, b.ch4.RESTART1, RSTa);

    /*p20.DARO*/ wire ENV_BOT  = nor (b.ch4.NR42_ENV_DIR, b.ch4.VOL0, b.ch4.VOL1, b.ch4.VOL2, b.ch4.VOL3);
    /*p20.CUTY*/ wire ENV_TOPn = nand(b.ch4.NR42_ENV_DIR, b.ch4.VOL0, b.ch4.VOL1, b.ch4.VOL2, b.ch4.VOL3);
    /*p20.DUBO*/ wire ENV_TOPa = not(ENV_TOPn);
    /*p20.EVUR*/ wire ENV_MAX  = or(ENV_BOT, ENV_TOPa);

    /*p20.EMET*/ wire ENV_STOP_RST = nor(b.ch4.RESTART1, RSTa);
    /*p20.FOSY*/ next.ch4.ENV_PULSE    = tock_pos(a.apu.CLK_512a, b.apu.CLK_512a, ENV_PULSE_RST2, b.ch4.ENV_PULSE, ENV_TIMER_MAX);
    /*p20.FYNO*/ next.ch4.ENV_STOP     = tock_pos(a.ch4.ENV_PULSE,   b.ch4.ENV_PULSE,   ENV_STOP_RST,   b.ch4.ENV_STOP,  ENV_MAX);

    // Schematic wrong, non-inverting FELO goes to EROX
    /*p20.ENUR*/ wire ENV_CLK1 = or(RSTa, b.ch4.RESTART1);
    /*p20.EROX*/ wire ENV_CLK2 = or(!b.ch4.ENV_STOP, ENV_CLK1); 

    // I should pull this out and test it standalone

    /*p20.FELO*/ wire ENV_CLK  = or(b.ch4.ENV_PULSE, ENV_CLK2, ENV_OFF);

    /*p20.FOLE*/ next.ch4.VOL_CLK0 = amux2(ENV_CLK, b.ch4.NR42_ENV_DIR, ENV_CLK,  !b.ch4.NR42_ENV_DIR);
    /*p20.ETEF*/ next.ch4.VOL_CLK1 = amux2(b.ch4.VOL0, b.ch4.NR42_ENV_DIR, !b.ch4.VOL0, !b.ch4.NR42_ENV_DIR);
    /*p20.EDYF*/ next.ch4.VOL_CLK2 = amux2(b.ch4.VOL1, b.ch4.NR42_ENV_DIR, !b.ch4.VOL1, !b.ch4.NR42_ENV_DIR);
    /*p20.ELAF*/ next.ch4.VOL_CLK3 = amux2(b.ch4.VOL2, b.ch4.NR42_ENV_DIR, !b.ch4.VOL2, !b.ch4.NR42_ENV_DIR);

    /*p20.FEKO*/ next.ch4.VOL0 = count_pos(a.ch4.VOL_CLK0, b.ch4.VOL_CLK0, b.ch4.RESTART1, b.ch4.VOL0, b.ch4.NR42_ENV_VOL0);
    /*p20.FATY*/ next.ch4.VOL1 = count_pos(a.ch4.VOL_CLK1, b.ch4.VOL_CLK1, b.ch4.RESTART1, b.ch4.VOL1, b.ch4.NR42_ENV_VOL1);
    /*p20.FERU*/ next.ch4.VOL2 = count_pos(a.ch4.VOL_CLK2, b.ch4.VOL_CLK2, b.ch4.RESTART1, b.ch4.VOL2, b.ch4.NR42_ENV_VOL2);
    /*p20.FYRO*/ next.ch4.VOL3 = count_pos(a.ch4.VOL_CLK3, b.ch4.VOL_CLK3, b.ch4.RESTART1, b.ch4.VOL3, b.ch4.NR42_ENV_VOL3);

  }

  //----------
  // Output

  {
    /*p20.GAME*/ wire LFSR_OUT     = nand(b.ch4.CH4_ACTIVE,  b.ch4.LFSR_15);
    /*p20.EZUL*/ wire CH4_BIT_MUX  = mux2(b.ch4.LFSR_CLKa, LFSR_OUT, b.ch4.DBG_CH4);
    /*p20.CEPY*/ wire NR44_STOPn = not(b.ch4.NR44_STOP);
    /*p20.COTE*/ wire DBG_CH4_MUTE = and(NR44_STOPn,  b.apu.NR52_DBG_APU);
    /*p20.DATO*/ wire CH4_RAW_BIT  = or  (CH4_BIT_MUX, DBG_CH4_MUTE);

    /*p20.GEVY*/ next.ch4.CH4_AMP_ENn = nor(b.ch4.NR42_ENV_DIR, b.ch4.NR42_ENV_VOL0, b.ch4.NR42_ENV_VOL1, b.ch4.NR42_ENV_VOL2, b.ch4.NR42_ENV_VOL3);
    /*p20.AKOF*/ next.ch4.CH4_DAC0 = and(b.ch4.VOL0, CH4_RAW_BIT);
    /*p20.BYZY*/ next.ch4.CH4_DAC1 = and(b.ch4.VOL1, CH4_RAW_BIT);
    /*p20.APYR*/ next.ch4.CH4_DAC2 = and(b.ch4.VOL2, CH4_RAW_BIT);
    /*p20.BOZA*/ next.ch4.CH4_DAC3 = and(b.ch4.VOL3, CH4_RAW_BIT);
  }
}

#endif

//-----------------------------------------------------------------------------

};