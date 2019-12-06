#include "Sch_Channel4.h"
#include "Sch_Gameboy.h"

namespace Schematics {

//-----------------------------------------------------------------------------

/*
// inputs
p01.PHASE_ABCDxxxx7c;
p01.PHASE_ABCDxxxx7c;
p01.BAVU_1M;
p01.CLK_512Kn;
p01.CLK_512Kn
p01.CLK_512a;
p01.CLK_512a
p01.CLK_256a;
p01.CLK_128a;

p09.RSTa;

p09.CPU_RDn;
p10.APU_WR;
p16.CPU_WR_WEIRD;

p10.ADDR_FF2Xn;
p10.ADDR_0000bn;
p10.ADDR_0001bn;
p10.ADDR_0010bn;
p10.ADDR_0011bn;

p09.DBG_APU;
*/


void Channel4_tick(const ChipIn& chip_in, const CpuIn& cpu_in, const Gameboy& a, const Gameboy& b, Gameboy& c) {
  (void)chip_in;
  (void)cpu_in;

  const Channel4& pa = a.ch4;
  const Channel4& pb = b.ch4;
  Channel4& pc = c.ch4;

  /*p20.ALOP*/ pc.CLK_128n = not(b.apu.CLK_128a);

  /*p09.KUBY*/ pc.RSTn = not(b.apu.APU_RST);
  /*p09.KEBA*/ pc.RSTa = not(pb.RSTn);
  /*p09.DAPA*/ pc.RSTo = not(pb.RSTa);
  /*p09.KAME*/ pc.RSTp = not(pb.RSTa);
  /*p19.FEXO*/ pc.RSTq = not(pb.RSTa);
  /*p19.HYNE*/ pc.RSTr = not(pb.RSTa);
  /*p19.CABE*/ pc.RSTs = not(pb.RSTa);
  /*p20.FEBY*/ pc.RSTt = not(pb.RSTa);
  /*p20.GASO*/ pc.RSTu = not(pb.RSTa);

  /*p19.KAGE*/ pc.CPU_RDa = not(b.apu.CPU_RDn);
  /*p19.BYLO*/ pc.CPU_RDb = not(b.apu.CPU_RDn);
  /*p20.COSA*/ pc.CPU_RDc = not(b.apu.CPU_RDn);

  //----------
  // FF20 NR41 - the length register is also the length timer

  /*p10.DANU*/ pc.FF20a    = nor(b.apu.ADDR_FF2Xn, b.apu.ADDR_0000bn);
  /*p19.CAZE*/ pc.FF20_WRn = nand(b.apu.APU_WR, pb.FF20a);
  /*p19.FURU*/ pc.FF20_WRa = not(pb.FF20_WRn);
  /*p19.DOTU*/ pc.FF20_WRb = not(pb.FF20_WRn);
  /*p19.EPEK*/ pc.FF20_WRc = not(pb.FF20_WRn);

  /*p19.DODA*/ pc.LEN_CLKn = nor(b.apu.CLK_256a, pb.LEN_STOP, pb.NR43_FREQ2); // this doesn't seem right
  /*p19.CUWA*/ pc.LEN_CLKa = not(pb.LEN_CLKn);

  /*p19.DANO*/ pc.NR41_LEN0  = count_pos(pa.LEN_CLKa,  pb.LEN_CLKa,   pb.FF20_WRb, pb.NR41_LEN0,   b.D0);
  /*p19.FAVY*/ pc.NR41_LEN1  = count_pos(pa.NR41_LEN0, pb.NR41_LEN0,  pb.FF20_WRb, pb.NR41_LEN1,   b.D1);
  /*p19.DENA*/ pc.NR41_LEN2  = count_pos(pa.NR41_LEN1, pb.NR41_LEN1,  pb.FF20_WRb, pb.NR41_LEN2,   b.D2);
  /*p19.CEDO*/ pc.NR41_LEN3  = count_pos(pa.NR41_LEN2, pb.NR41_LEN2,  pb.FF20_WRb, pb.NR41_LEN3,   b.D3);
  /*p19.DOPU*/ pc.NR41_LEN3b = not(!pb.NR41_LEN3);                     
  /*p19.FYLO*/ pc.NR41_LEN4  = count_pos(pa.NR41_LEN3b,pb.NR41_LEN3b, pb.FF20_WRc, pb.NR41_LEN4,   b.D4);
  /*p19.EDOP*/ pc.NR41_LEN5  = count_pos(pa.NR41_LEN4, pb.NR41_LEN4,  pb.FF20_WRc, pb.NR41_LEN5,   b.D5);

  /*p19.GAPY*/ pc.LEN_RSTn = nor(pb.FF20_WRa, pb.RSTa, pb.RESTART1);
  /*p19.FUGO*/ pc.LEN_STOP = tock_pos(!pa.NR41_LEN5, !pb.NR41_LEN5, pb.LEN_RSTn, pb.LEN_STOP, !pb.LEN_STOP);

  // No read circuit for FF20?

  //----------
  // FF21

  /*p10.COVO*/ pc.FF21a    = nor(b.apu.ADDR_FF2Xn, b.apu.ADDR_0001bn);
  /*p19.BOFY*/ pc.FF21n    = not(pb.FF21a);
  /*p19.GONY*/ pc.FF21o    = not(pb.FF21a);
  /*p19.DACO*/ pc.FF21_WRa = and(b.apu.APU_WR, pb.FF21a);
  /*p19.GOKO*/ pc.FF21_WRb = and(b.apu.APU_WR, pb.FF21a);
  /*p19.DYKE*/ pc.FF21_WRn = not(pb.FF21_WRa);
  /*p19.FUPA*/ pc.FF21_WRo = not(pb.FF21_WRb);
  /*p19.BOXE*/ pc.FF21_RDa = or(b.apu.CPU_RDn, pb.FF21n); // polarity?
  /*p19.HASU*/ pc.FF21_RDb = or(b.apu.CPU_RDn, pb.FF21o);

  /*p19.EMOK*/ pc.NR42_ENV_TIMER0 = tock_pos(pa.FF21_WRn, pb.FF21_WRn, pb.RSTq, pb.NR42_ENV_TIMER0, b.D0);
  /*p19.ETYJ*/ pc.NR42_ENV_TIMER1 = tock_pos(pa.FF21_WRn, pb.FF21_WRn, pb.RSTq, pb.NR42_ENV_TIMER1, b.D1);
  /*p19.EZYK*/ pc.NR42_ENV_TIMER2 = tock_pos(pa.FF21_WRn, pb.FF21_WRn, pb.RSTq, pb.NR42_ENV_TIMER2, b.D2);
  /*p19.GEKY*/ pc.NR42_ENV_DIR    = tock_pos(pa.FF21_WRo, pb.FF21_WRo, pb.RSTq, pb.NR42_ENV_DIR,    b.D3);
  /*p19.GARU*/ pc.NR42_ENV_VOL0   = tock_pos(pa.FF21_WRo, pb.FF21_WRo, pb.RSTq, pb.NR42_ENV_VOL0,   b.D4);
  /*p19.GOKY*/ pc.NR42_ENV_VOL1   = tock_pos(pa.FF21_WRo, pb.FF21_WRo, pb.RSTq, pb.NR42_ENV_VOL1,   b.D5);
  /*p19.GOZO*/ pc.NR42_ENV_VOL2   = tock_pos(pa.FF21_WRo, pb.FF21_WRo, pb.RSTq, pb.NR42_ENV_VOL2,   b.D6);
  /*p19.GEDU*/ pc.NR42_ENV_VOL3   = tock_pos(pa.FF21_WRo, pb.FF21_WRo, pb.RSTq, pb.NR42_ENV_VOL3,   b.D7);

  /*p19.DEMY*/ if (pb.FF21_RDa) c.D0 = pb.NR42_ENV_TIMER0;
  /*p19.COCE*/ if (pb.FF21_RDa) c.D1 = pb.NR42_ENV_TIMER1;
  /*p19.CUZU*/ if (pb.FF21_RDa) c.D2 = pb.NR42_ENV_TIMER2;
  /*p19.GOME*/ if (pb.FF21_RDb) c.D3 = pb.NR42_ENV_DIR;
  /*p19.HEDA*/ if (pb.FF21_RDb) c.D4 = pb.NR42_ENV_VOL0;
  /*p19.GODU*/ if (pb.FF21_RDb) c.D5 = pb.NR42_ENV_VOL1;
  /*p19.HOGE*/ if (pb.FF21_RDb) c.D6 = pb.NR42_ENV_VOL2;
  /*p19.HACU*/ if (pb.FF21_RDb) c.D7 = pb.NR42_ENV_VOL3;

  //----------
  // FF22

  /*p10.EKEZ*/ pc.FF22a = nor(b.apu.ADDR_FF2Xn, b.apu.ADDR_0010bn);
  /*p19.KOKU*/ pc.FF22n = not(pb.FF22a);
  /*p19.GUGO*/ pc.FF22o = not(pb.FF22a);

  /*p19.HUMO*/ pc.FF22_WRa = and (pb.FF22a, b.apu.APU_WR);
  /*p19.GETU*/ pc.FF22_WRn = nand(pb.FF22a, b.apu.APU_WR);
  /*p19.HOSO*/ pc.FF22_WRp = nand(pb.FF22a, b.apu.APU_WR);
  /*p19.HOVA*/ pc.FF22_WRo = not (pb.FF22_WRa);
  /*p19.EFUG*/ pc.FF22_WRq = not (pb.FF22_WRn);

  /*p19.KEKA*/ pc.FF22_RDa = or(b.apu.CPU_RDn, pb.FF22n);
  /*p19.JORA*/ pc.FF22_RDb = nand(pb.FF22a, pb.CPU_RDa);
  /*p19.HEZE*/ pc.FF22_RDc = or(b.apu.CPU_RDn, pb.FF22o);

  /*p19.JARE*/ pc.NR43_DIV0  = tock_pos(pa.FF22_WRo, pb.FF22_WRo, pb.RSTp, pb.NR43_DIV0,  b.D0);
  /*p19.JERO*/ pc.NR43_DIV1  = tock_pos(pa.FF22_WRo, pb.FF22_WRo, pb.RSTp, pb.NR43_DIV1,  b.D1);
  /*p19.JAKY*/ pc.NR43_DIV2  = tock_pos(pa.FF22_WRo, pb.FF22_WRo, pb.RSTp, pb.NR43_DIV2,  b.D2);
  /*p19.JAMY*/ pc.NR43_MODE  = tock_pos(pa.FF22_WRp, pb.FF22_WRp, pb.RSTr, pb.NR43_MODE,  b.D3);
  /*p19.FETA*/ pc.NR43_FREQ0 = tock_pos(pa.FF22_WRq, pb.FF22_WRq, pb.RSTo, pb.NR43_FREQ0, b.D4);
  /*p19.FYTO*/ pc.NR43_FREQ1 = tock_pos(pa.FF22_WRq, pb.FF22_WRq, pb.RSTo, pb.NR43_FREQ1, b.D5);
  /*p19.GOGO*/ pc.NR43_FREQ2 = tock_pos(pa.FF22_WRq, pb.FF22_WRq, pb.RSTo, pb.NR43_FREQ2, b.D6);
  /*p19.GAFO*/ pc.NR43_FREQ3 = tock_pos(pa.FF22_WRq, pb.FF22_WRq, pb.RSTo, pb.NR43_FREQ3, b.D7);

  /*p19.KAMO*/ if (pb.FF22_RDa) c.D0 = pb.NR43_DIV0;
  /*p19.KAKU*/ if (pb.FF22_RDa) c.D1 = pb.NR43_DIV1;
  /*p19.KYRO*/ if (pb.FF22_RDa) c.D2 = pb.NR43_DIV2;
  /*p19.KETA*/ if (pb.FF22_RDb) c.D3 = pb.NR43_MODE;
  /*p19.GEDA*/ if (pb.FF22_RDc) c.D4 = pb.NR43_FREQ0;
  /*p19.GYPE*/ if (pb.FF22_RDc) c.D5 = pb.NR43_FREQ1;
  /*p19.GAKA*/ if (pb.FF22_RDc) c.D6 = pb.NR43_FREQ2;
  /*p19.HAPY*/ if (pb.FF22_RDc) c.D7 = pb.NR43_FREQ3;

  //----------
  // FF23. Some weird debug voodoo here.

  /*p10.CUGE*/ pc.FF23a    = nor(b.apu.ADDR_FF2Xn, b.apu.ADDR_0011bn);
  /*p19.BARE*/ pc.FF23_RDa = nand(pb.FF23a, pb.CPU_RDb); // polarity?
  /*p19.DULU*/ pc.FF23_WRn = nand(b.apu.CPU_WR_WEIRD, pb.FF23a);
  /*p19.FOXE*/ pc.FF23_WRo = nand(b.apu.APU_WR, pb.FF23a);

  /*p19.CUNY*/ pc.NR44_STOP  = tock_pos(pa.FF23_WRn, pb.FF23_WRn, pb.RSTs, pb.NR44_STOP,  b.D6);
  /*p20.CEPY*/ pc.NR44_STOPn = not(pb.NR44_STOP);

  /*p20.GUZY*/ pc.NR44_START_RST = nor(pb.CH4_START, pb.RSTa);
  /*p19.HOGA*/ pc.NR44_START     = tock_pos(pa.FF23_WRo, pb.FF23_WRo, pb.NR44_START_RST, pb.NR44_START, b.D7);

  /*p19.CURY*/ if (pb.FF23_RDa) c.D6 = pb.NR44_STOP;

  //----------
  // Control

  /*p20.GYSU*/ pc.CH4_START   = tock_pos(a.sys.PHASE_ABCDxxxx7c, b.sys.PHASE_ABCDxxxx7c, pb.RSTu, pb.CH4_START, pb.NR44_START);
  /*p20.EFOT*/ pc.CH4_STOP    = and(pb.NR44_STOP,   pb.LEN_STOP);
  /*p20.FEGY*/ pc.CH4_OFF     = or (pb.CH4_AMP_ENn, pb.CH4_STOP, pb.RSTa);
  /*p20.GENA*/ pc.CH4_ACTIVE  = or (pb.RESTART1, pb.CH4_OFF);
  /*p20.JUWA*/ pc.CH4_ACTIVEn = not(pb.CH4_ACTIVE);

  /*p20.HAZO*/ pc.RESTART_IN   = or(pb.RESTART_RST, pb.CH4_START);

  /*p20.GONE*/ pc.RESTART1 = tock_pos(a.apu.CLK_512Kn, b.apu.CLK_512Kn, pb.RESTART_RSTn, pb.RESTART1, pb.RESTART_IN);
  /*p20.GORA*/ pc.RESTART2 = tock_pos(a.apu.CLK_512Kn, b.apu.CLK_512Kn, pb.RSTt,         pb.RESTART2, pb.RESTART1);

  /*p20.FALE*/ pc.RESTART_RSTn = nor(pb.RESTART2, pb.RSTa);
  /*p20.HELU*/ pc.RESTART_RST  = not(pb.RESTART_RSTn);

  // one of these is wrong, right now we would stop the div clock when ch4 active
  // fixed kyku but still might be a polarity error?

  /*p20.GATY*/ pc.RESTART3  = tock_pos(a.apu.CLK_512Kn, b.apu.CLK_512Kn, pb.RSTt,         pb.RESTART3, pb.RESTART2);
  /*p20.HAPU*/ pc.DIV_GATE2 = not(pb.RESTART3);
  /*p20.HERY*/ pc.DIV_GATE1 = nor(pb.RSTa, pb.CH4_AMP_ENn);
  /*p20.JERY*/ pc.DIV_GATE3 = or(pb.DIV_GATE1, pb.DIV_GATE2);

  /*p20.KYKU*/ pc.DIV_CLKb  = nor(b.apu.CLK_512Ka, pb.DIV_GATE3); // schematic wrong
  /*p20.KONY*/ pc.DIV_CLKn  = not(pb.DIV_CLKb);
  /*p20.KANU*/ pc.DIV_CLKa  = not(pb.DIV_CLKn);
  /*p20.GOFU*/ pc.DIV_LOADn = nor(pb.RESTART1, pb.FREQ_GATEn);
  /*p20.HUCE*/ pc.DIV_LOAD  = not(pb.DIV_LOADn);

  /*p20.JYCO*/ pc.DIV0    = count_pos(pa.DIV_CLKa, pb.DIV_CLKa, pb.DIV_LOAD, pb.DIV0, !pb.NR43_DIV0);
  /*p20.JYRE*/ pc.DIV1    = count_pos(pa.DIV0,     pb.DIV0,     pb.DIV_LOAD, pb.DIV1, !pb.NR43_DIV1);
  /*p20.JYFU*/ pc.DIV2    = count_pos(pa.DIV1,     pb.DIV1,     pb.DIV_LOAD, pb.DIV2, !pb.NR43_DIV2);
  /*p20.HYNO*/ pc.DIV_MAX = and(pb.DIV0, pb.DIV1,  pb.DIV2); // schematic wrong

  //----------
  // Debug

  /*p20.DYRY*/ pc.DBG_CH4 = and(pb.NR44_STOPn, b.apu.NR52_DBG_APU);
  /*p20.COMO*/ pc.DBG_COMO = and(pb.CPU_RDc, pb.DBG_CH4);
  /*p20.BAGU*/ pc.DBG_BAGU = nand(pb.FF23a, pb.DBG_COMO);
  /*p20.BEFA*/ pc.DBG_BEFA = not(pb.FREQ_CLK);
  /*p20.ATEL*/ if (pb.DBG_BAGU) c.D0 = pb.DBG_BEFA;

  //----------
  // Frequency timer

  /*p20.GYBA*/ pc.FREQ_GATE_CLK  = not(b.apu.BAVU_1M);
  /*p20.GUNY*/ pc.FREQ_GATE_RSTn = nor(pb.RESTART1, pb.RSTa);
  /*p20.GARY*/ pc.FREQ_GATEn     = tock_pos(pa.FREQ_GATE_CLK, pb.FREQ_GATE_CLK, pb.FREQ_GATE_RSTn, pb.FREQ_GATEn, pb.DIV_MAX);
  /*p20.CARY*/ pc.FREQ_CLK = and(b.apu.BAVU_1M, pb.FREQ_GATEn);

  /*p20.CEXO*/ pc.FREQ_00 = tock_pos( pa.FREQ_CLK,  pb.FREQ_CLK, pb.RSTo, pb.FREQ_00, !pb.FREQ_00);
  /*p20.DEKO*/ pc.FREQ_01 = tock_pos(!pa.FREQ_00,   !pb.FREQ_00, pb.RSTo, pb.FREQ_01, !pb.FREQ_01);
  /*p20.EZEF*/ pc.FREQ_02 = tock_pos(!pa.FREQ_01,   !pb.FREQ_01, pb.RSTo, pb.FREQ_02, !pb.FREQ_02);
  /*p20.EPOR*/ pc.FREQ_03 = tock_pos(!pa.FREQ_02,   !pb.FREQ_02, pb.RSTo, pb.FREQ_03, !pb.FREQ_03);
  /*p20.DURE*/ pc.FREQ_04 = tock_pos(!pa.FREQ_03,   !pb.FREQ_03, pb.RSTo, pb.FREQ_04, !pb.FREQ_04);
  /*p20.DALE*/ pc.FREQ_05 = tock_pos(!pa.FREQ_04,   !pb.FREQ_04, pb.RSTo, pb.FREQ_05, !pb.FREQ_05);
  /*p20.DOKE*/ pc.FREQ_06 = tock_pos(!pa.FREQ_05,   !pb.FREQ_05, pb.RSTo, pb.FREQ_06, !pb.FREQ_06);
  /*p20.DEMO*/ pc.FREQ_07 = tock_pos(!pa.FREQ_06,   !pb.FREQ_06, pb.RSTo, pb.FREQ_07, !pb.FREQ_07);
  /*p20.DOSE*/ pc.FREQ_08 = tock_pos(!pa.FREQ_07,   !pb.FREQ_07, pb.RSTo, pb.FREQ_08, !pb.FREQ_08);
  /*p20.DETE*/ pc.FREQ_09 = tock_pos(!pa.FREQ_08,   !pb.FREQ_08, pb.RSTo, pb.FREQ_09, !pb.FREQ_09);
  /*p20.ERUT*/ pc.FREQ_10 = tock_pos(!pa.FREQ_09,   !pb.FREQ_09, pb.RSTo, pb.FREQ_10, !pb.FREQ_10);
  /*p20.DOTA*/ pc.FREQ_11 = tock_pos(!pa.FREQ_10,   !pb.FREQ_10, pb.RSTo, pb.FREQ_11, !pb.FREQ_11);
  /*p20.DERE*/ pc.FREQ_12 = tock_pos(!pa.FREQ_11,   !pb.FREQ_11, pb.RSTo, pb.FREQ_12, !pb.FREQ_12);
  /*p20.ESEP*/ pc.FREQ_13 = tock_pos(!pa.FREQ_12,   !pb.FREQ_12, pb.RSTo, pb.FREQ_13, !pb.FREQ_13);

  //----------
  // 14-to-1 mux to select LFSR clock

  /*p20.EMOF*/ pc.LFSR_CLK_MUX_7 = nor(!pb.NR43_FREQ0, !pb.NR43_FREQ1, !pb.NR43_FREQ2);
  /*p20.ELAR*/ pc.LFSR_CLK_MUX_6 = nor(!pb.NR43_FREQ0, !pb.NR43_FREQ1,  pb.NR43_FREQ2);
  /*p20.DUDU*/ pc.LFSR_CLK_MUX_5 = nor(!pb.NR43_FREQ0,  pb.NR43_FREQ1, !pb.NR43_FREQ2);
  /*p20.ETAT*/ pc.LFSR_CLK_MUX_4 = nor(!pb.NR43_FREQ0,  pb.NR43_FREQ1,  pb.NR43_FREQ2);
  /*p20.FURA*/ pc.LFSR_CLK_MUX_3 = nor( pb.NR43_FREQ0, !pb.NR43_FREQ1, !pb.NR43_FREQ2);
  /*p20.ETAR*/ pc.LFSR_CLK_MUX_2 = nor( pb.NR43_FREQ0, !pb.NR43_FREQ1,  pb.NR43_FREQ2);
  /*p20.EVER*/ pc.LFSR_CLK_MUX_1 = nor( pb.NR43_FREQ0,  pb.NR43_FREQ1, !pb.NR43_FREQ2);
  /*p20.ETOV*/ pc.LFSR_CLK_MUX_0 = nor( pb.NR43_FREQ0,  pb.NR43_FREQ1,  pb.NR43_FREQ2);

  /*p20.ETYR*/ pc.LFSR_CLK_MUX_A = amux6(pb.FREQ_13, pb.LFSR_CLK_MUX_5,
                                         pb.FREQ_12, pb.LFSR_CLK_MUX_4,
                                         pb.FREQ_11, pb.LFSR_CLK_MUX_3,
                                         pb.FREQ_10, pb.LFSR_CLK_MUX_2,
                                         pb.FREQ_09, pb.LFSR_CLK_MUX_1,
                                         pb.FREQ_08, pb.LFSR_CLK_MUX_0);

  /*p20.ELYX*/ pc.LFSR_CLK_MUX_B = amux4(pb.FREQ_07, pb.LFSR_CLK_MUX_7,
                                         pb.FREQ_06, pb.LFSR_CLK_MUX_6,
                                         pb.FREQ_05, pb.LFSR_CLK_MUX_5,
                                         pb.FREQ_04, pb.LFSR_CLK_MUX_4);

  /*p20.DARY*/ pc.LFSR_CLK_MUX_C = amux4(pb.FREQ_03, pb.LFSR_CLK_MUX_3,
                                         pb.FREQ_02, pb.LFSR_CLK_MUX_1,
                                         pb.FREQ_01, pb.LFSR_CLK_MUX_1,
                                         pb.FREQ_00, pb.LFSR_CLK_MUX_0);

  /*p20.ERYF*/ pc.LFSR_CLK_MUX_D = or(pb.LFSR_CLK_MUX_B, pb.LFSR_CLK_MUX_C);

  //----------
  // The actual LFSR

  /*p20.FEME*/ pc.LFSR_CLKa = mux2(pb.LFSR_CLK_MUX_A, pb.LFSR_CLK_MUX_D, pb.NR43_FREQ3);
  /*p20.JYJA*/ pc.LFSR_CLKn = not(pb.LFSR_CLKa);
  /*p20.GUFA*/ pc.LFSR_CLKo = not(pb.LFSR_CLKa);
  /*p20.GYVE*/ pc.LFSR_CLKb = not(pb.LFSR_CLKo);
  /*p20.KARA*/ pc.LFSR_CLKp = not(pb.LFSR_CLKb);
  /*p20.KOPA*/ pc.LFSR_CLKc = not(pb.LFSR_CLKp);

  /*p20.GEPO*/ pc.LFSR_RSTa = or(pb.RESTART1, pb.RSTa);
  /*p20.GOGE*/ pc.LFSR_RSTn = not(pb.LFSR_RSTa);

  /*p20.HURA*/ pc.LFSR_IN   = xor(pb.LFSR_15, pb.LFSR_14);
  /*p20.JOTO*/ pc.LFSR_00   = tock_pos(pa.LFSR_CLKn, pb.LFSR_CLKn, pb.LFSR_RSTn, pb.LFSR_00, pb.LFSR_IN);
  /*p20.KOMU*/ pc.LFSR_01   = tock_pos(pa.LFSR_CLKc, pb.LFSR_CLKc, pb.LFSR_RSTn, pb.LFSR_01, pb.LFSR_00);
  /*p20.KETU*/ pc.LFSR_02   = tock_pos(pa.LFSR_CLKc, pb.LFSR_CLKc, pb.LFSR_RSTn, pb.LFSR_02, pb.LFSR_01);
  /*p20.KUTA*/ pc.LFSR_03   = tock_pos(pa.LFSR_CLKc, pb.LFSR_CLKc, pb.LFSR_RSTn, pb.LFSR_03, pb.LFSR_02);
  /*p20.KUZY*/ pc.LFSR_04   = tock_pos(pa.LFSR_CLKc, pb.LFSR_CLKc, pb.LFSR_RSTn, pb.LFSR_04, pb.LFSR_03);
  /*p20.KYWY*/ pc.LFSR_05   = tock_pos(pa.LFSR_CLKc, pb.LFSR_CLKc, pb.LFSR_RSTn, pb.LFSR_05, pb.LFSR_04);
  /*p20.JAJU*/ pc.LFSR_06   = tock_pos(pa.LFSR_CLKc, pb.LFSR_CLKc, pb.LFSR_RSTn, pb.LFSR_06, pb.LFSR_05);
  /*p20.HAPE*/ pc.LFSR_07   = tock_pos(pa.LFSR_CLKc, pb.LFSR_CLKc, pb.LFSR_RSTn, pb.LFSR_07, pb.LFSR_06);
  /*p20.JUXE*/ pc.LFSR_08   = tock_pos(pa.LFSR_CLKc, pb.LFSR_CLKc, pb.LFSR_RSTn, pb.LFSR_08, pb.LFSR_07);
                            
  /*p20.KAVU*/ pc.LFSR_FB   = amux2(pb.LFSR_00, pb.NR43_MODE, !pb.NR43_MODE, pb.LFSR_08);
  /*p20.JEPE*/ pc.LFSR_09   = tock_pos(pa.LFSR_CLKb,  pb.LFSR_CLKb, pb.LFSR_RSTn, pb.LFSR_09, pb.LFSR_FB);
  /*p20.JAVO*/ pc.LFSR_10   = tock_pos(pa.LFSR_CLKb,  pb.LFSR_CLKb, pb.LFSR_RSTn, pb.LFSR_10, pb.LFSR_09);
  /*p20.HEPA*/ pc.LFSR_11   = tock_pos(pa.LFSR_CLKa,  pb.LFSR_CLKa, pb.LFSR_RSTn, pb.LFSR_11, pb.LFSR_10);
  /*p20.HORY*/ pc.LFSR_12   = tock_pos(pa.LFSR_CLKa,  pb.LFSR_CLKa, pb.LFSR_RSTn, pb.LFSR_12, pb.LFSR_11);
  /*p20.HENO*/ pc.LFSR_13   = tock_pos(pa.LFSR_CLKa,  pb.LFSR_CLKa, pb.LFSR_RSTn, pb.LFSR_13, pb.LFSR_12);
  /*p20.HYRO*/ pc.LFSR_14   = tock_pos(pa.LFSR_CLKa,  pb.LFSR_CLKa, pb.LFSR_RSTn, pb.LFSR_14, pb.LFSR_13);
  /*p20.HEZU*/ pc.LFSR_15   = tock_pos(pa.LFSR_CLKa,  pb.LFSR_CLKa, pb.LFSR_RSTn, pb.LFSR_15, pb.LFSR_14);

  //----------
  // Env

  /*p20.FOWA*/ pc.ENV_OFF = nor(pb.NR42_ENV_TIMER0, pb.NR42_ENV_TIMER1, pb.NR42_ENV_TIMER0);

  /*p20.BOKY*/ pc.ENV_TIMER_CLK_RST = not(pb.RSTa);
  /*p20.ABEL*/ pc.ENV_TIMER_CLKa    = tock_pos(pa.CLK_128n, pb.CLK_128n, pb.ENV_TIMER_CLK_RST, pb.ENV_TIMER_CLKa, !pb.ENV_TIMER_CLKa);
  /*p20.BAWA*/ pc.ENV_TIMER_CLKn    = not(pb.ENV_TIMER_CLKa);
  /*p20.BUXO*/ pc.ENV_TIMER_CLKb    = not(pb.ENV_TIMER_CLKn);

  /*p20.ENEC*/ pc.ENV_TIMER_LOADn = nor(pb.RESTART1, pb.ENV_PULSE);
  /*p20.DAPY*/ pc.ENV_TIMER_LOAD  = not(pb.ENV_TIMER_LOADn);
  /*p20.CUNA*/ pc.ENV_TIMER0      = count_pos(pa.ENV_TIMER_CLKb, pb.ENV_TIMER_CLKb, pb.ENV_TIMER_LOAD, pb.ENV_TIMER0, !pb.NR42_ENV_TIMER0);
  /*p20.COFE*/ pc.ENV_TIMER1      = count_pos(pa.ENV_TIMER0,     pb.ENV_TIMER0,     pb.ENV_TIMER_LOAD, pb.ENV_TIMER1, !pb.NR42_ENV_TIMER1);
  /*p20.DOGO*/ pc.ENV_TIMER2      = count_pos(pa.ENV_TIMER1,     pb.ENV_TIMER1,     pb.ENV_TIMER_LOAD, pb.ENV_TIMER2, !pb.NR42_ENV_TIMER2);
  /*p20.EJEX*/ pc.ENV_TIMER_MAX   = and(pb.ENV_TIMER0, pb.ENV_TIMER1, pb.ENV_TIMER2);

  // Generates a 1 usec pulse when the env timer hits 111
  /*p20.GEXE*/ pc.ENV_PULSEn     = not(pb.ENV_PULSE);
  /*p20.HURY*/ pc.ENV_PULSE_RST1 = nor(b.apu.CLK_512a, pb.ENV_PULSEn);
  /*p20.GOPA*/ pc.ENV_PULSE_RST2 = nor(pb.ENV_PULSE_RST1, pb.ENV_OFF, pb.RESTART1, pb.RSTa);
  /*p20.FOSY*/ pc.ENV_PULSE      = tock_pos(a.apu.CLK_512a, b.apu.CLK_512a, pb.ENV_PULSE_RST2, pb.ENV_PULSE, pb.ENV_TIMER_MAX);
  
  /*p20.EMET*/ pc.ENV_STOP_RST = nor(pb.RESTART1, pb.RSTa);
  /*p20.FYNO*/ pc.ENV_STOP     = tock_pos(pa.ENV_PULSE, pb.ENV_PULSE, pb.ENV_STOP_RST, pb.ENV_STOP, pb.ENV_MAX);

  // Schematic wrong, non-inverting FELO goes to EROX
  /*p20.ENUR*/ pc.ENV_CLK1 = or(pb.RSTa, pb.RESTART1);
  /*p20.EROX*/ pc.ENV_CLK2 = or(!pb.ENV_STOP, pb.ENV_CLK1); 
  /*p20.FELO*/ pc.ENV_CLK  = or(pb.ENV_PULSE, pb.ENV_CLK2, pb.ENV_OFF);

  // I should pull this out and test it standalone

  /*p20.FOLE*/ pc.VOL_CLK0 = amux2(pb.ENV_CLK, pb.NR42_ENV_DIR,  pb.ENV_CLK, !pb.NR42_ENV_DIR);
  /*p20.ETEF*/ pc.VOL_CLK1 = amux2(pb.VOL0,    pb.NR42_ENV_DIR, !pb.VOL0,    !pb.NR42_ENV_DIR);
  /*p20.EDYF*/ pc.VOL_CLK2 = amux2(pb.VOL1,    pb.NR42_ENV_DIR, !pb.VOL1,    !pb.NR42_ENV_DIR);
  /*p20.ELAF*/ pc.VOL_CLK3 = amux2(pb.VOL2,    pb.NR42_ENV_DIR, !pb.VOL2,    !pb.NR42_ENV_DIR);

  /*p20.FEKO*/ pc.VOL0 = count_pos(pa.VOL_CLK0, pb.VOL_CLK0, pb.RESTART1, pb.VOL0, pb.NR42_ENV_VOL0);
  /*p20.FATY*/ pc.VOL1 = count_pos(pa.VOL_CLK1, pb.VOL_CLK1, pb.RESTART1, pb.VOL1, pb.NR42_ENV_VOL1);
  /*p20.FERU*/ pc.VOL2 = count_pos(pa.VOL_CLK2, pb.VOL_CLK2, pb.RESTART1, pb.VOL2, pb.NR42_ENV_VOL2);
  /*p20.FYRO*/ pc.VOL3 = count_pos(pa.VOL_CLK3, pb.VOL_CLK3, pb.RESTART1, pb.VOL3, pb.NR42_ENV_VOL3);

  /*p20.DARO*/ pc.ENV_BOT  = nor (pb.NR42_ENV_DIR, pb.VOL0, pb.VOL1, pb.VOL2, pb.VOL3);
  /*p20.CUTY*/ pc.ENV_TOPn = nand(pb.NR42_ENV_DIR, pb.VOL0, pb.VOL1, pb.VOL2, pb.VOL3);
  /*p20.DUBO*/ pc.ENV_TOPa = not(pb.ENV_TOPn);
  /*p20.EVUR*/ pc.ENV_MAX  = or(pb.ENV_BOT, pb.ENV_TOPa);

  //----------
  // Output

  // lfsr output w/ debug overrides
  /*p20.GAME*/ pc.LFSR_OUT     = nand(pb.CH4_ACTIVE,  pb.LFSR_15);
  /*p20.EZUL*/ pc.CH4_BIT_MUX  = mux2(pb.LFSR_CLKa,   pb.LFSR_OUT, pb.DBG_CH4);
  /*p20.COTE*/ pc.DBG_CH4_MUTE = and (pb.NR44_STOPn,  b.apu.NR52_DBG_APU);
  /*p20.DATO*/ pc.CH4_RAW_BIT  = or  (pb.CH4_BIT_MUX, pb.DBG_CH4_MUTE);

  /*p20.GEVY*/ pc.CH4_AMP_ENn = nor(pb.NR42_ENV_DIR, pb.NR42_ENV_VOL0, pb.NR42_ENV_VOL1, pb.NR42_ENV_VOL2, pb.NR42_ENV_VOL3);
  /*p20.AKOF*/ pc.CH4_DAC0 = and(pb.VOL0, pb.CH4_RAW_BIT);
  /*p20.BYZY*/ pc.CH4_DAC1 = and(pb.VOL1, pb.CH4_RAW_BIT);
  /*p20.APYR*/ pc.CH4_DAC2 = and(pb.VOL2, pb.CH4_RAW_BIT);
  /*p20.BOZA*/ pc.CH4_DAC3 = and(pb.VOL3, pb.CH4_RAW_BIT);
}

//-----------------------------------------------------------------------------

};