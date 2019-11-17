#include "../Schematics.h"
#include "Gameboy.h"

void Channel4::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  c.p01.APUCLK_512Kn = not(!b.p01.JESO_512K);

  /*P09.KAME*/ c.p09.APU_RESETn5 = not(b.p09.APU_RESET1);
  /*p19.KAGE*/ c.ch4.CPU_RDa = not(b.p09.CPU_RDn);

  /*p20.ALOP*/ c.ch4.CLK_128n = not(b.p01.CLK_128);
  /*p19.FEXO*/ c.ch4.FEXO = not(b.p09.APU_RESET1);
  /*p19.HYNE*/ c.ch4.HYNE = not(b.p09.APU_RESET1);
  /*p19.CABE*/ c.ch4.CABE = not(b.p09.APU_RESET1);
  /*p20.FEBY*/ c.ch4.FEBY = not(b.p09.APU_RESET1);

  //----------
  // FF20 NR41 - the length register is also the length timer

  /*p19.CAZE*/ c.ch4.FF20_WRn = nand(b.p10.APU_WR, b.p10.ADDR_FF20);
  /*p19.FURU*/ c.ch4.FF20_WRa = not(b.ch4.FF20_WRn);
  /*p19.DOTU*/ c.ch4.FF20_WRb = not(b.ch4.FF20_WRn);
  /*p19.EPEK*/ c.ch4.FF20_WRc = not(b.ch4.FF20_WRn);

  /*p19.DODA*/ c.ch4.CH4_LEN_CLKn = nor(b.p01.BUFY_256, b.ch4.CH4_LEN_STOP, b.ch4.NR43_FREQ_2); // this doesn't seem right
  /*p19.CUWA*/ c.ch4.CH4_LEN_CLKa = not(b.ch4.CH4_LEN_CLKn);
  /*p19.GAPY*/ c.ch4.CH4_LEN_RST  = nor(b.ch4.FF20_WRa, b.p09.APU_RESET1, b.ch4.CH4_RESTART);

  /*p19.DANO*/ c.ch4.NR41_LEN_0  = count_pos(a.ch4.CH4_LEN_CLKa, b.ch4.CH4_LEN_CLKa, b.ch4.FF20_WRb,    b.ch4.NR41_LEN_0,   b.D0);
  /*p19.FAVY*/ c.ch4.NR41_LEN_1  = count_pos(a.ch4.NR41_LEN_0,   b.ch4.NR41_LEN_0,   b.ch4.FF20_WRb,    b.ch4.NR41_LEN_1,   b.D1);
  /*p19.DENA*/ c.ch4.NR41_LEN_2  = count_pos(a.ch4.NR41_LEN_1,   b.ch4.NR41_LEN_1,   b.ch4.FF20_WRb,    b.ch4.NR41_LEN_2,   b.D2);
  /*p19.CEDO*/ c.ch4.NR41_LEN_3  = count_pos(a.ch4.NR41_LEN_2,   b.ch4.NR41_LEN_2,   b.ch4.FF20_WRb,    b.ch4.NR41_LEN_3,   b.D3);
  /*p19.DOPU*/ c.ch4.NR41_LEN_3b = not(!b.ch4.NR41_LEN_3);
  /*p19.FYLO*/ c.ch4.NR41_LEN_4  = count_pos(a.ch4.NR41_LEN_3b,  b.ch4.NR41_LEN_3b,  b.ch4.FF20_WRc,    b.ch4.NR41_LEN_4,   b.D4);
  /*p19.EDOP*/ c.ch4.NR41_LEN_5  = count_pos(a.ch4.NR41_LEN_4,   b.ch4.NR41_LEN_4,   b.ch4.FF20_WRc,    b.ch4.NR41_LEN_5,   b.D5);

  /*p19.FUGO*/ c.ch4.CH4_LEN_STOP = tock_pos(!a.ch4.NR41_LEN_5, !b.ch4.NR41_LEN_5,   b.ch4.CH4_LEN_RST, b.ch4.CH4_LEN_STOP, !b.ch4.CH4_LEN_STOP);

  // Where's the readout circuit for FF20?

  //----------
  // FF21

  /*p19.BOFY*/ c.ch4.FF21n    = not(b.p10.ADDR_FF21);
  /*p19.GONY*/ c.ch4.FF21o    = not(b.p10.ADDR_FF21);
  /*p19.DACO*/ c.ch4.FF21_WRa = and(b.p10.APU_WR, b.p10.ADDR_FF21);
  /*p19.GOKO*/ c.ch4.FF21_WRb = and(b.p10.APU_WR, b.p10.ADDR_FF21);
  /*p19.DYKE*/ c.ch4.FF21_WRn = not(b.ch4.FF21_WRa);
  /*p19.FUPA*/ c.ch4.FF21_WRo = not(b.ch4.FF21_WRb);
  /*p19.BOXE*/ c.ch4.FF21_RDa = or(b.p09.CPU_RDn, b.ch4.FF21n); // polarity?
  /*p19.HASU*/ c.ch4.FF21_RDb = or(b.p09.CPU_RDn, b.ch4.FF21o);

  /*p19.EMOK*/ c.ch4.NR42_ENV_TIMER0 = tock_pos(a.ch4.FF21_WRn, b.ch4.FF21_WRn, b.ch4.FEXO, b.ch4.NR42_ENV_TIMER0, b.D0);
  /*p19.ETYJ*/ c.ch4.NR42_ENV_TIMER1 = tock_pos(a.ch4.FF21_WRn, b.ch4.FF21_WRn, b.ch4.FEXO, b.ch4.NR42_ENV_TIMER1, b.D1);
  /*p19.EZYK*/ c.ch4.NR42_ENV_TIMER2 = tock_pos(a.ch4.FF21_WRn, b.ch4.FF21_WRn, b.ch4.FEXO, b.ch4.NR42_ENV_TIMER2, b.D2);
  /*p19.GEKY*/ c.ch4.NR42_ENV_DIR    = tock_pos(a.ch4.FF21_WRo, b.ch4.FF21_WRo, b.ch4.FEXO, b.ch4.NR42_ENV_DIR,    b.D3);
  /*p19.GARU*/ c.ch4.NR42_ENV_VOL_0  = tock_pos(a.ch4.FF21_WRo, b.ch4.FF21_WRo, b.ch4.FEXO, b.ch4.NR42_ENV_VOL_0,  b.D4);
  /*p19.GOKY*/ c.ch4.NR42_ENV_VOL_1  = tock_pos(a.ch4.FF21_WRo, b.ch4.FF21_WRo, b.ch4.FEXO, b.ch4.NR42_ENV_VOL_1,  b.D5);
  /*p19.GOZO*/ c.ch4.NR42_ENV_VOL_2  = tock_pos(a.ch4.FF21_WRo, b.ch4.FF21_WRo, b.ch4.FEXO, b.ch4.NR42_ENV_VOL_2,  b.D6);
  /*p19.GEDU*/ c.ch4.NR42_ENV_VOL_3  = tock_pos(a.ch4.FF21_WRo, b.ch4.FF21_WRo, b.ch4.FEXO, b.ch4.NR42_ENV_VOL_3,  b.D7);

  /*p19.DEMY*/ if (b.ch4.FF21_RDa) c.D0 = b.ch4.NR42_ENV_TIMER0;
  /*p19.COCE*/ if (b.ch4.FF21_RDa) c.D1 = b.ch4.NR42_ENV_TIMER1;
  /*p19.CUZU*/ if (b.ch4.FF21_RDa) c.D2 = b.ch4.NR42_ENV_TIMER2;
  /*p19.GOME*/ if (b.ch4.FF21_RDb) c.D3 = b.ch4.NR42_ENV_DIR;
  /*p19.HEDA*/ if (b.ch4.FF21_RDb) c.D4 = b.ch4.NR42_ENV_VOL_0;
  /*p19.GODU*/ if (b.ch4.FF21_RDb) c.D5 = b.ch4.NR42_ENV_VOL_1;
  /*p19.HOGE*/ if (b.ch4.FF21_RDb) c.D6 = b.ch4.NR42_ENV_VOL_2;
  /*p19.HACU*/ if (b.ch4.FF21_RDb) c.D7 = b.ch4.NR42_ENV_VOL_3;

  //----------
  // FF22

  /*p10.EKEZ*/ c.ch4.FF22a = nor(b.p10.ADDR_FF2Xn, b.p10.ADDR_0010bn);
  /*p19.KOKU*/ c.ch4.FF22n = not(b.ch4.FF22a);
  /*p19.GUGO*/ c.ch4.FF22o = not(b.ch4.FF22a);

  /*p19.HUMO*/ c.ch4.FF22_WRa = and (b.ch4.FF22a, b.p10.APU_WR);
  /*p19.GETU*/ c.ch4.FF22_WRn = nand(b.ch4.FF22a, b.p10.APU_WR);
  /*p19.HOSO*/ c.ch4.FF22_WRp = nand(b.ch4.FF22a, b.p10.APU_WR);
  /*p19.HOVA*/ c.ch4.FF22_WRo = not (b.ch4.FF22_WRa);
  /*p19.EFUG*/ c.ch4.FF22_WRq = not (b.ch4.FF22_WRn);

  /*p19.KEKA*/ c.ch4.FF22_RDa = or(b.p09.CPU_RDn, b.ch4.FF22n);
  /*p19.JORA*/ c.ch4.FF22_RDb = nand(b.ch4.FF22a, b.ch4.CPU_RDa);
  /*p19.HEZE*/ c.ch4.FF22_RDc = or(b.p09.CPU_RDn, b.ch4.FF22o);

  /*p19.JARE*/ c.ch4.NR43_DIV_0  = tock_pos(a.ch4.FF22_WRo, b.ch4.FF22_WRo, b.p09.APU_RESETn5, b.ch4.NR43_DIV_0,  b.D0);
  /*p19.JERO*/ c.ch4.NR43_DIV_1  = tock_pos(a.ch4.FF22_WRo, b.ch4.FF22_WRo, b.p09.APU_RESETn5, b.ch4.NR43_DIV_1,  b.D1);
  /*p19.JAKY*/ c.ch4.NR43_DIV_2  = tock_pos(a.ch4.FF22_WRo, b.ch4.FF22_WRo, b.p09.APU_RESETn5, b.ch4.NR43_DIV_2,  b.D2);
  /*p19.JAMY*/ c.ch4.NR43_MODE   = tock_pos(a.ch4.FF22_WRp, b.ch4.FF22_WRp, b.ch4.HYNE,        b.ch4.NR43_MODE,   b.D3);
  /*p19.FETA*/ c.ch4.NR43_FREQ_0 = tock_pos(a.ch4.FF22_WRq, b.ch4.FF22_WRq, b.p09.APU_RESETn4, b.ch4.NR43_FREQ_0, b.D4);
  /*p19.FYTO*/ c.ch4.NR43_FREQ_1 = tock_pos(a.ch4.FF22_WRq, b.ch4.FF22_WRq, b.p09.APU_RESETn4, b.ch4.NR43_FREQ_1, b.D5);
  /*p19.GOGO*/ c.ch4.NR43_FREQ_2 = tock_pos(a.ch4.FF22_WRq, b.ch4.FF22_WRq, b.p09.APU_RESETn4, b.ch4.NR43_FREQ_2, b.D6);
  /*p19.GAFO*/ c.ch4.NR43_FREQ_3 = tock_pos(a.ch4.FF22_WRq, b.ch4.FF22_WRq, b.p09.APU_RESETn4, b.ch4.NR43_FREQ_3, b.D7);

  /*p19.KAMO*/ if (b.ch4.FF22_RDa) c.D0 = b.ch4.NR43_DIV_0;
  /*p19.KAKU*/ if (b.ch4.FF22_RDa) c.D1 = b.ch4.NR43_DIV_1;
  /*p19.KYRO*/ if (b.ch4.FF22_RDa) c.D2 = b.ch4.NR43_DIV_2;
  /*p19.KETA*/ if (b.ch4.FF22_RDb) c.D3 = b.ch4.NR43_MODE;
  /*p19.GEDA*/ if (b.ch4.FF22_RDc) c.D4 = b.ch4.NR43_FREQ_0;
  /*p19.GYPE*/ if (b.ch4.FF22_RDc) c.D5 = b.ch4.NR43_FREQ_1;
  /*p19.GAKA*/ if (b.ch4.FF22_RDc) c.D6 = b.ch4.NR43_FREQ_2;
  /*p19.HAPY*/ if (b.ch4.FF22_RDc) c.D7 = b.ch4.NR43_FREQ_3;

  //----------
  // FF23. Some weird debug voodoo here.

  /*p19.DULU*/ c.ch4.FF23_WRn = nand(b.p16.CPU_WR_WEIRD, b.p10.ADDR_FF23);
  /*p19.FOXE*/ c.ch4.FF23_WRo = nand(b.p10.APU_WR, b.p10.ADDR_FF23);

  /*p19.BYLO*/ c.ch4.CPU_RDb = not(b.p09.CPU_RDn);
  /*p19.BARE*/ c.ch4.FF23_RD = nand(b.p10.ADDR_FF23, b.ch4.CPU_RDb); // polarity?

  /*p19.CUNY*/ c.ch4.NR44_STOP  = tock_pos(a.ch4.FF23_WRn, b.ch4.FF23_WRn, b.ch4.CABE, b.ch4.NR44_STOP,  b.D6);

  /*p20.GUZY*/ c.ch4.NR44_START_RST = nor(b.ch4.CH4_START, b.p09.APU_RESET1);
  /*p19.HOGA*/ c.ch4.NR44_START     = tock_pos(a.ch4.FF23_WRo, b.ch4.FF23_WRo, b.ch4.NR44_START_RST, b.ch4.NR44_START, b.D7);

  /*p19.CURY*/ if (b.ch4.FF23_RD) c.D6 = b.ch4.NR44_STOP;

  /*p20.DYRY*/ c.ch4.DBG_CH4 = and(b.ch4.NR44_STOPn, b.p09.DBG_APU);
  /*p20.COMO*/ c.ch4.DBG_COMO = and(b.ch4.CPU_RD, b.ch4.DBG_CH4);
  /*p20.BAGU*/ c.ch4.DBG_BAGU = nand(b.p10.ADDR_FF23, b.ch4.DBG_COMO);

  /*p20.BEFA*/ c.ch4.DBG_BEFA = not(b.ch4.CH4_FREQ_CLK);
  /*p20.ATEL*/ if (b.ch4.DBG_BAGU) c.D0 = b.ch4.DBG_BEFA;

  //----------

  /*p20.COSA*/ c.ch4.CPU_RD = not(b.p09.CPU_RDn);
  /*p20.CEPY*/ c.ch4.NR44_STOPn = not(b.ch4.NR44_STOP);
  

  // delay line shift reg?
  /*p20.GONE*/ c.ch4.CH4_RESTART = tock_pos(a.p01.APUCLK_512Kn, b.p01.APUCLK_512Kn, b.ch4.FALE, b.ch4.CH4_RESTART, b.ch4.HAZO);
  /*p20.GORA*/ c.ch4.GORA        = tock_pos(a.p01.APUCLK_512Kn, b.p01.APUCLK_512Kn, b.ch4.FEBY, b.ch4.GORA,        b.ch4.CH4_RESTART);
  /*p20.GATY*/ c.ch4.GATY        = tock_pos(a.p01.APUCLK_512Kn, b.p01.APUCLK_512Kn, b.ch4.FEBY, b.ch4.GATY,        b.ch4.GORA);
  /*p20.HAPU*/ c.ch4.HAPU        = not(b.ch4.GATY);

  /*p20.FALE*/ c.ch4.FALE = nor(b.ch4.GORA, b.p09.APU_RESET1);
  /*p20.HELU*/ c.ch4.HELU = not(b.ch4.FALE);


  // moar 3 bit counters
  /*p20.HERY*/ c.ch4.HERY = nor(b.p09.APU_RESET1, b.ch4.CH4_AMP_ENn);
  /*p20.JERY*/ c.ch4.JERY = or(b.ch4.HERY, b.ch4.HAPU);
  /*p20.KYKU*/ c.ch4.KYKU = or(b.ch4.JERY, b.p01.JESO_512K);
  /*p20.KONY*/ c.ch4.KONY = not(b.ch4.KYKU);
  /*p20.KANU*/ c.ch4.KANU = not(b.ch4.KONY);

  /*p20.GOFU*/ c.ch4.GOFU = nor(b.ch4.CH4_RESTART, b.ch4.GARY);
  /*p20.HUCE*/ c.ch4.HUCE = not(b.ch4.GOFU);

  /*p20.JYCO*/ c.ch4.JYCO = count_pos(a.ch4.KANU, b.ch4.KANU, b.ch4.HUCE, b.ch4.JYCO, !b.ch4.NR43_DIV_0);
  /*p20.JYRE*/ c.ch4.JYRE = count_pos(a.ch4.JYCO, b.ch4.JYCO, b.ch4.HUCE, b.ch4.JYRE, !b.ch4.NR43_DIV_1);
  /*p20.JYFU*/ c.ch4.JYFU = count_pos(a.ch4.JYRE, b.ch4.JYRE, b.ch4.HUCE, b.ch4.JYFU, !b.ch4.NR43_DIV_2);

  /*p20.GYBA*/ c.ch4.GYBA = not(b.p01.BAVU_1M);
  /*p20.GUNY*/ c.ch4.GUNY = nor(b.ch4.CH4_RESTART, b.p09.APU_RESET1);
  /*p20.HYNO*/ c.ch4.HYNO = or(b.ch4.JYCO, b.ch4.JYRE, b.ch4.JYFU);
  /*p20.GARY*/ c.ch4.GARY = tock_pos(a.ch4.GYBA, b.ch4.GYBA, b.ch4.GUNY, b.ch4.GARY, b.ch4.HYNO);

  

  // ch4 freq counter

  /*p20.CARY*/ c.ch4.CH4_FREQ_CLK = and(b.p01.BAVU_1M, b.ch4.GARY);

  /*p20.CEXO*/ c.ch4.FREQ_00 = tock_pos( a.ch4.CH4_FREQ_CLK,  b.ch4.CH4_FREQ_CLK, b.p09.APU_RESETn4, b.ch4.FREQ_00, !b.ch4.FREQ_00);
  /*p20.DEKO*/ c.ch4.FREQ_01 = tock_pos(!a.ch4.FREQ_00,      !b.ch4.FREQ_00,      b.p09.APU_RESETn4, b.ch4.FREQ_01, !b.ch4.FREQ_01);
  /*p20.EZEF*/ c.ch4.FREQ_02 = tock_pos(!a.ch4.FREQ_01,      !b.ch4.FREQ_01,      b.p09.APU_RESETn4, b.ch4.FREQ_02, !b.ch4.FREQ_02);
  /*p20.EPOR*/ c.ch4.FREQ_03 = tock_pos(!a.ch4.FREQ_02,      !b.ch4.FREQ_02,      b.p09.APU_RESETn4, b.ch4.FREQ_03, !b.ch4.FREQ_03);
  /*p20.DURE*/ c.ch4.FREQ_04 = tock_pos(!a.ch4.FREQ_03,      !b.ch4.FREQ_03,      b.p09.APU_RESETn4, b.ch4.FREQ_04, !b.ch4.FREQ_04);
  /*p20.DALE*/ c.ch4.FREQ_05 = tock_pos(!a.ch4.FREQ_04,      !b.ch4.FREQ_04,      b.p09.APU_RESETn4, b.ch4.FREQ_05, !b.ch4.FREQ_05);
  /*p20.DOKE*/ c.ch4.FREQ_06 = tock_pos(!a.ch4.FREQ_05,      !b.ch4.FREQ_05,      b.p09.APU_RESETn4, b.ch4.FREQ_06, !b.ch4.FREQ_06);
  /*p20.DEMO*/ c.ch4.FREQ_07 = tock_pos(!a.ch4.FREQ_06,      !b.ch4.FREQ_06,      b.p09.APU_RESETn4, b.ch4.FREQ_07, !b.ch4.FREQ_07);
  /*p20.DOSE*/ c.ch4.FREQ_08 = tock_pos(!a.ch4.FREQ_07,      !b.ch4.FREQ_07,      b.p09.APU_RESETn4, b.ch4.FREQ_08, !b.ch4.FREQ_08);
  /*p20.DETE*/ c.ch4.FREQ_09 = tock_pos(!a.ch4.FREQ_08,      !b.ch4.FREQ_08,      b.p09.APU_RESETn4, b.ch4.FREQ_09, !b.ch4.FREQ_09);
  /*p20.ERUT*/ c.ch4.FREQ_10 = tock_pos(!a.ch4.FREQ_09,      !b.ch4.FREQ_09,      b.p09.APU_RESETn4, b.ch4.FREQ_10, !b.ch4.FREQ_10);
  /*p20.DOTA*/ c.ch4.FREQ_11 = tock_pos(!a.ch4.FREQ_10,      !b.ch4.FREQ_10,      b.p09.APU_RESETn4, b.ch4.FREQ_11, !b.ch4.FREQ_11);
  /*p20.DERE*/ c.ch4.FREQ_12 = tock_pos(!a.ch4.FREQ_11,      !b.ch4.FREQ_11,      b.p09.APU_RESETn4, b.ch4.FREQ_12, !b.ch4.FREQ_12);
  /*p20.ESEP*/ c.ch4.FREQ_13 = tock_pos(!a.ch4.FREQ_12,      !b.ch4.FREQ_12,      b.p09.APU_RESETn4, b.ch4.FREQ_13, !b.ch4.FREQ_13);

  //----------
  // 14-to-1 mux to select lfsr clock

  /*p20.EMOF*/ c.ch4.LFSR_CLK_MUX_7 = nor(!b.ch4.NR43_FREQ_0, !b.ch4.NR43_FREQ_1, !b.ch4.NR43_FREQ_2);
  /*p20.ELAR*/ c.ch4.LFSR_CLK_MUX_6 = nor(!b.ch4.NR43_FREQ_0, !b.ch4.NR43_FREQ_1,  b.ch4.NR43_FREQ_2);
  /*p20.DUDU*/ c.ch4.LFSR_CLK_MUX_5 = nor(!b.ch4.NR43_FREQ_0,  b.ch4.NR43_FREQ_1, !b.ch4.NR43_FREQ_2);
  /*p20.ETAT*/ c.ch4.LFSR_CLK_MUX_4 = nor(!b.ch4.NR43_FREQ_0,  b.ch4.NR43_FREQ_1,  b.ch4.NR43_FREQ_2);
  /*p20.FURA*/ c.ch4.LFSR_CLK_MUX_3 = nor( b.ch4.NR43_FREQ_0, !b.ch4.NR43_FREQ_1, !b.ch4.NR43_FREQ_2);
  /*p20.ETAR*/ c.ch4.LFSR_CLK_MUX_2 = nor( b.ch4.NR43_FREQ_0, !b.ch4.NR43_FREQ_1,  b.ch4.NR43_FREQ_2);
  /*p20.EVER*/ c.ch4.LFSR_CLK_MUX_1 = nor( b.ch4.NR43_FREQ_0,  b.ch4.NR43_FREQ_1, !b.ch4.NR43_FREQ_2);
  /*p20.ETOV*/ c.ch4.LFSR_CLK_MUX_0 = nor( b.ch4.NR43_FREQ_0,  b.ch4.NR43_FREQ_1,  b.ch4.NR43_FREQ_2);

  /*p20.ETYR*/ c.ch4.LFSR_CLK_MUX_A = amux6(b.ch4.FREQ_13, b.ch4.LFSR_CLK_MUX_5,
                                            b.ch4.FREQ_12, b.ch4.LFSR_CLK_MUX_4,
                                            b.ch4.FREQ_11, b.ch4.LFSR_CLK_MUX_3,
                                            b.ch4.FREQ_10, b.ch4.LFSR_CLK_MUX_2,
                                            b.ch4.FREQ_09, b.ch4.LFSR_CLK_MUX_1,
                                            b.ch4.FREQ_08, b.ch4.LFSR_CLK_MUX_0);

  /*p20.ELYX*/ c.ch4.LFSR_CLK_MUX_B = amux4(b.ch4.FREQ_07, b.ch4.LFSR_CLK_MUX_7,
                                            b.ch4.FREQ_06, b.ch4.LFSR_CLK_MUX_6,
                                            b.ch4.FREQ_05, b.ch4.LFSR_CLK_MUX_5,
                                            b.ch4.FREQ_04, b.ch4.LFSR_CLK_MUX_4);

  /*p20.DARY*/ c.ch4.LFSR_CLK_MUX_C = amux4(b.ch4.FREQ_03, b.ch4.LFSR_CLK_MUX_3,
                                            b.ch4.FREQ_02, b.ch4.LFSR_CLK_MUX_1,
                                            b.ch4.FREQ_01, b.ch4.LFSR_CLK_MUX_1,
                                            b.ch4.FREQ_00, b.ch4.LFSR_CLK_MUX_0);

  /*p20.ERYF*/ c.ch4.LFSR_CLK_MUX_D = or(b.ch4.LFSR_CLK_MUX_B, b.ch4.LFSR_CLK_MUX_C);


  //----------


  /*p20.EMET*/ c.ch4.EMET = nor(b.ch4.CH4_RESTART, b.p09.APU_RESET1);

  /*p20.FYNO*/ c.ch4.FYNO = tock_pos(a.ch4.CH4_EG_TICK, b.ch4.CH4_EG_TICK, b.ch4.EMET, b.ch4.FYNO, b.ch4.CH4_ENV_MAX);
  /*p20.ENUR*/ c.ch4.ENUR = or(b.p09.APU_RESET1, b.ch4.CH4_RESTART);
  /*p20.EROX*/ c.ch4.EROX = or(!b.ch4.FYNO, b.ch4.ENUR);

  // this is some sort of "ch4_off" signal, but HURY doesn't make sense
  /*p20.GOPA*/ c.ch4.GOPA = nor(b.ch4.HURY, b.ch4.CH4_ENV_OFF, b.ch4.CH4_RESTART, b.p09.APU_RESET1);

  //----------
  // Control

  /*p20.GASO*/ c.ch4.GASO      = not(b.p09.APU_RESET1);
  /*p20.GYSU*/ c.ch4.CH4_START = tock_pos(a.p01.CPUCLK_xxxxEFGH9, b.p01.CPUCLK_xxxxEFGH9, b.ch4.GASO, b.ch4.CH4_START, b.ch4.NR44_START);
  /*p20.HAZO*/ c.ch4.HAZO      = or(b.ch4.HELU, b.ch4.CH4_START);

  /*p20.EFOT*/ c.ch4.CH4_STOP    = and(b.ch4.NR44_STOP,    b.ch4.CH4_LEN_STOP);
  /*p20.FEGY*/ c.ch4.CH4_OFF     = or (b.ch4.CH4_AMP_ENn,  b.ch4.CH4_STOP, b.p09.APU_RESET1);
  /*p20.GENA*/ c.ch4.CH4_ACTIVE  = or (b.ch4.CH4_RESTART,  b.ch4.CH4_OFF);
  /*p20.JUWA*/ c.ch4.CH4_ACTIVEn = not(b.ch4.CH4_ACTIVE);

  //----------
  // The actual LFSR

  /*p20.FEME*/ c.ch4.LFSR_CLKa = mux2(b.ch4.LFSR_CLK_MUX_A, b.ch4.LFSR_CLK_MUX_D, b.ch4.NR43_FREQ_3);
  /*p20.JYJA*/ c.ch4.LFSR_CLKn = not(b.ch4.LFSR_CLKa);
  /*p20.GUFA*/ c.ch4.LFSR_CLKo = not(b.ch4.LFSR_CLKa);
  /*p20.GYVE*/ c.ch4.LFSR_CLKb = not(b.ch4.LFSR_CLKo);
  /*p20.KARA*/ c.ch4.LFSR_CLKp = not(b.ch4.LFSR_CLKb);
  /*p20.KOPA*/ c.ch4.LFSR_CLKc = not(b.ch4.LFSR_CLKp);

  /*p20.GEPO*/ c.ch4.LFSR_RSTa = or(b.ch4.CH4_RESTART, b.p09.APU_RESET1);
  /*p20.GOGE*/ c.ch4.LFSR_RSTn = not(b.ch4.LFSR_RSTa);

  /*p20.HURA*/ c.ch4.LFSR_IN = xor(b.ch4.LFSR_15, b.ch4.LFSR_14);
  /*p20.JOTO*/ c.ch4.LFSR_00 = tock_pos(a.ch4.LFSR_CLKn, b.ch4.LFSR_CLKn, b.ch4.LFSR_RSTn, b.ch4.LFSR_00, b.ch4.LFSR_IN);
  /*p20.KOMU*/ c.ch4.LFSR_01 = tock_pos(a.ch4.LFSR_CLKc, b.ch4.LFSR_CLKc, b.ch4.LFSR_RSTn, b.ch4.LFSR_01, b.ch4.LFSR_00);
  /*p20.KETU*/ c.ch4.LFSR_02 = tock_pos(a.ch4.LFSR_CLKc, b.ch4.LFSR_CLKc, b.ch4.LFSR_RSTn, b.ch4.LFSR_02, b.ch4.LFSR_01);
  /*p20.KUTA*/ c.ch4.LFSR_03 = tock_pos(a.ch4.LFSR_CLKc, b.ch4.LFSR_CLKc, b.ch4.LFSR_RSTn, b.ch4.LFSR_03, b.ch4.LFSR_02);
  /*p20.KUZY*/ c.ch4.LFSR_04 = tock_pos(a.ch4.LFSR_CLKc, b.ch4.LFSR_CLKc, b.ch4.LFSR_RSTn, b.ch4.LFSR_04, b.ch4.LFSR_03);
  /*p20.KYWY*/ c.ch4.LFSR_05 = tock_pos(a.ch4.LFSR_CLKc, b.ch4.LFSR_CLKc, b.ch4.LFSR_RSTn, b.ch4.LFSR_05, b.ch4.LFSR_04);
  /*p20.JAJU*/ c.ch4.LFSR_06 = tock_pos(a.ch4.LFSR_CLKc, b.ch4.LFSR_CLKc, b.ch4.LFSR_RSTn, b.ch4.LFSR_06, b.ch4.LFSR_05);
  /*p20.HAPE*/ c.ch4.LFSR_07 = tock_pos(a.ch4.LFSR_CLKc, b.ch4.LFSR_CLKc, b.ch4.LFSR_RSTn, b.ch4.LFSR_07, b.ch4.LFSR_06);
  /*p20.JUXE*/ c.ch4.LFSR_08 = tock_pos(a.ch4.LFSR_CLKc, b.ch4.LFSR_CLKc, b.ch4.LFSR_RSTn, b.ch4.LFSR_08, b.ch4.LFSR_07);
  
  /*p20.KAVU*/ c.ch4.LFSR_FB = amux2(b.ch4.LFSR_00, b.ch4.NR43_MODE, !b.ch4.NR43_MODE, b.ch4.LFSR_08);

  /*p20.JEPE*/ c.ch4.LFSR_09 = tock_pos(a.ch4.LFSR_CLKb,  b.ch4.LFSR_CLKb, b.ch4.LFSR_RSTn, b.ch4.LFSR_09, b.ch4.LFSR_FB);
  /*p20.JAVO*/ c.ch4.LFSR_10 = tock_pos(a.ch4.LFSR_CLKb,  b.ch4.LFSR_CLKb, b.ch4.LFSR_RSTn, b.ch4.LFSR_10, b.ch4.LFSR_09);
  /*p20.HEPA*/ c.ch4.LFSR_11 = tock_pos(a.ch4.LFSR_CLKa,  b.ch4.LFSR_CLKa, b.ch4.LFSR_RSTn, b.ch4.LFSR_11, b.ch4.LFSR_10);
  /*p20.HORY*/ c.ch4.LFSR_12 = tock_pos(a.ch4.LFSR_CLKa,  b.ch4.LFSR_CLKa, b.ch4.LFSR_RSTn, b.ch4.LFSR_12, b.ch4.LFSR_11);
  /*p20.HENO*/ c.ch4.LFSR_13 = tock_pos(a.ch4.LFSR_CLKa,  b.ch4.LFSR_CLKa, b.ch4.LFSR_RSTn, b.ch4.LFSR_13, b.ch4.LFSR_12);
  /*p20.HYRO*/ c.ch4.LFSR_14 = tock_pos(a.ch4.LFSR_CLKa,  b.ch4.LFSR_CLKa, b.ch4.LFSR_RSTn, b.ch4.LFSR_14, b.ch4.LFSR_13);
  /*p20.HEZU*/ c.ch4.LFSR_15 = tock_pos(a.ch4.LFSR_CLKa,  b.ch4.LFSR_CLKa, b.ch4.LFSR_RSTn, b.ch4.LFSR_15, b.ch4.LFSR_14);

  // lfsr output w/ debug overrides
  /*p20.GAME*/ c.ch4.LFSR_OUT     = nand(b.ch4.CH4_ACTIVE,  b.ch4.LFSR_15);
  /*p20.EZUL*/ c.ch4.CH4_BIT_MUX  = mux2(b.ch4.LFSR_CLKa,   b.ch4.LFSR_OUT, b.ch4.DBG_CH4);
  /*p20.COTE*/ c.ch4.DBG_CH4_MUTE = and (b.ch4.NR44_STOPn,  b.p09.DBG_APU);
  /*p20.DATO*/ c.ch4.CH4_RAW_BIT  = or  (b.ch4.CH4_BIT_MUX, b.ch4.DBG_CH4_MUTE);

  //----------
  // Env

  /*p20.BOKY*/ c.ch4.CH4_ENV_TIMER_CLK_RST = not(b.p09.APU_RESET1);
  /*p20.ABEL*/ c.ch4.CH4_ENV_TIMER_CLKa    = tock_pos(a.ch4.CLK_128n, b.ch4.CLK_128n, b.ch4.CH4_ENV_TIMER_CLK_RST, b.ch4.CH4_ENV_TIMER_CLKa, !b.ch4.CH4_ENV_TIMER_CLKa);
  /*p20.BAWA*/ c.ch4.CH4_ENV_TIMER_CLKn    = not(b.ch4.CH4_ENV_TIMER_CLKa);
  /*p20.BUXO*/ c.ch4.CH4_ENV_TIMER_CLKb    = not(b.ch4.CH4_ENV_TIMER_CLKn);
  /*p20.ENEC*/ c.ch4.CH4_ENV_TIMER_RST     = nor(b.ch4.CH4_RESTART, b.ch4.CH4_EG_TICK);
  /*p20.DAPY*/ c.ch4.CH4_ENV_TIMER_RSTn    = not(b.ch4.CH4_ENV_TIMER_RST);
  /*p20.CUNA*/ c.ch4.CH4_ENV_TIMER0        = count_pos(a.ch4.CH4_ENV_TIMER_CLKb, b.ch4.CH4_ENV_TIMER_CLKb, b.ch4.CH4_ENV_TIMER_RSTn, b.ch4.CH4_ENV_TIMER0, !b.ch4.NR42_ENV_TIMER0);
  /*p20.COFE*/ c.ch4.CH4_ENV_TIMER1        = count_pos(a.ch4.CH4_ENV_TIMER0,     b.ch4.CH4_ENV_TIMER0,     b.ch4.CH4_ENV_TIMER_RSTn, b.ch4.CH4_ENV_TIMER1, !b.ch4.NR42_ENV_TIMER1);
  /*p20.DOGO*/ c.ch4.CH4_ENV_TIMER2        = count_pos(a.ch4.CH4_ENV_TIMER1,     b.ch4.CH4_ENV_TIMER1,     b.ch4.CH4_ENV_TIMER_RSTn, b.ch4.CH4_ENV_TIMER2, !b.ch4.NR42_ENV_TIMER2);

  // this can't be right, must be and...
  /*p20.EJEX*/ c.ch4.CH4_ENV_TIMER_TICK    = or(b.ch4.CH4_ENV_TIMER0, b.ch4.CH4_ENV_TIMER1, b.ch4.CH4_ENV_TIMER2);
  
  /*p20.FOWA*/ c.ch4.CH4_ENV_OFF = nor(b.ch4.NR42_ENV_TIMER0, b.ch4.NR42_ENV_TIMER1, b.ch4.NR42_ENV_TIMER0);
  
  // clk_256?
  /*p20.FOSY*/ c.ch4.CH4_EG_TICK = tock_pos(a.p01.HORU_512, b.p01.HORU_512, b.ch4.GOPA, b.ch4.CH4_EG_TICK, b.ch4.CH4_ENV_TIMER_TICK);
  /*p20.GEXE*/ c.ch4.CH4_EG_TICKn = not(b.ch4.CH4_EG_TICK);
  /*p20.HURY*/ c.ch4.HURY = nor(b.p01.HORU_512, b.ch4.CH4_EG_TICKn);
  // FIXME not sure this is right

  /*p20.FELO*/ c.ch4.CH4_ENV_CLK  = or(b.ch4.CH4_EG_TICK, b.ch4.EROX, b.ch4.CH4_ENV_OFF);
  /*p20.FOLE*/ c.ch4.CH4_ENV_CLK0 = amux2(b.ch4.CH4_ENV_CLK, b.ch4.NR42_ENV_DIR,  b.ch4.CH4_ENV_CLK, !b.ch4.NR42_ENV_DIR);
  /*p20.ETEF*/ c.ch4.CH4_ENV_CLK1 = amux2(b.ch4.CH4_ENV0,    b.ch4.NR42_ENV_DIR, !b.ch4.CH4_ENV0,    !b.ch4.NR42_ENV_DIR);
  /*p20.EDYF*/ c.ch4.CH4_ENV_CLK2 = amux2(b.ch4.CH4_ENV1,    b.ch4.NR42_ENV_DIR, !b.ch4.CH4_ENV1,    !b.ch4.NR42_ENV_DIR);
  /*p20.ELAF*/ c.ch4.CH4_ENV_CLK3 = amux2(b.ch4.CH4_ENV2,    b.ch4.NR42_ENV_DIR, !b.ch4.CH4_ENV2,    !b.ch4.NR42_ENV_DIR);

  /*p20.FEKO*/ c.ch4.CH4_ENV0 = count_pos(a.ch4.CH4_ENV_CLK0, b.ch4.CH4_ENV_CLK0, b.ch4.CH4_RESTART, b.ch4.CH4_ENV0, b.ch4.NR42_ENV_VOL_0);
  /*p20.FATY*/ c.ch4.CH4_ENV1 = count_pos(a.ch4.CH4_ENV_CLK1, b.ch4.CH4_ENV_CLK1, b.ch4.CH4_RESTART, b.ch4.CH4_ENV1, b.ch4.NR42_ENV_VOL_1);
  /*p20.FERU*/ c.ch4.CH4_ENV2 = count_pos(a.ch4.CH4_ENV_CLK2, b.ch4.CH4_ENV_CLK2, b.ch4.CH4_RESTART, b.ch4.CH4_ENV2, b.ch4.NR42_ENV_VOL_2);
  /*p20.FYRO*/ c.ch4.CH4_ENV3 = count_pos(a.ch4.CH4_ENV_CLK3, b.ch4.CH4_ENV_CLK3, b.ch4.CH4_RESTART, b.ch4.CH4_ENV3, b.ch4.NR42_ENV_VOL_3);

  /*p20.DARO*/ c.ch4.CH4_ENV_BOT  = nor (b.ch4.NR42_ENV_DIR, b.ch4.CH4_ENV0, b.ch4.CH4_ENV1, b.ch4.CH4_ENV2, b.ch4.CH4_ENV3);
  /*p20.CUTY*/ c.ch4.CH4_ENV_TOPn = nand(b.ch4.NR42_ENV_DIR, b.ch4.CH4_ENV0, b.ch4.CH4_ENV1, b.ch4.CH4_ENV2, b.ch4.CH4_ENV3);
  /*p20.DUBO*/ c.ch4.CH4_ENV_TOPa = not(b.ch4.CH4_ENV_TOPn);
  /*p20.EVUR*/ c.ch4.CH4_ENV_MAX = or(b.ch4.CH4_ENV_BOT, b.ch4.CH4_ENV_TOPa);

  //----------
  // Output

  /*p20.GEVY*/ c.ch4.CH4_AMP_ENn = nor(b.ch4.NR42_ENV_DIR, b.ch4.NR42_ENV_VOL_0, b.ch4.NR42_ENV_VOL_1, b.ch4.NR42_ENV_VOL_2, b.ch4.NR42_ENV_VOL_3);

  /*p20.AKOF*/ c.ch4.CH4_DAC0 = and(b.ch4.CH4_ENV0, b.ch4.CH4_RAW_BIT);
  /*p20.BYZY*/ c.ch4.CH4_DAC1 = and(b.ch4.CH4_ENV1, b.ch4.CH4_RAW_BIT);
  /*p20.APYR*/ c.ch4.CH4_DAC2 = and(b.ch4.CH4_ENV2, b.ch4.CH4_RAW_BIT);
  /*p20.BOZA*/ c.ch4.CH4_DAC3 = and(b.ch4.CH4_ENV3, b.ch4.CH4_RAW_BIT);
}
