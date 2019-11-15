#include "../Schematics.h"
#include "Gameboy.h"

void P19_Ch4Regs_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // FF20

  /*p19.CAZE*/ c.p19.FF20_WRn = nand(b.p10.APU_WR, b.p10.ADDR_FF20);
  /*p19.FURU*/ c.p19.FF20_WRa = not(b.p19.FF20_WRn);
  /*p19.DOTU*/ c.p19.FF20_WRb = not(b.p19.FF20_WRn);
  /*p19.EPEK*/ c.p19.FF20_WRc = not(b.p19.FF20_WRn);

  /*p19.DODA*/ c.p19.CH4_LEN_CLKn = nor(b.p19.CH4_DONE, b.p01.BUFY_256, b.p19.CH4_FREQ_2); //weird
  /*p19.CUWA*/ c.p19.CH4_LEN_CLK  = not(b.p19.CH4_LEN_CLKn);

  /*p19.DANO*/ c.p19.CH4_LEN_0  = count_pos(a.p19.CH4_LEN_CLK, b.p19.CH4_LEN_CLK, b.p19.FF20_WRb, b.p19.CH4_LEN_0, b.D0);
  /*p19.FAVY*/ c.p19.CH4_LEN_1  = count_pos(a.p19.CH4_LEN_0,   b.p19.CH4_LEN_0,   b.p19.FF20_WRb, b.p19.CH4_LEN_1, b.D1);
  /*p19.DENA*/ c.p19.CH4_LEN_2  = count_pos(a.p19.CH4_LEN_1,   b.p19.CH4_LEN_1,   b.p19.FF20_WRb, b.p19.CH4_LEN_2, b.D2);
  /*p19.CEDO*/ c.p19.CH4_LEN_3  = count_pos(a.p19.CH4_LEN_2,   b.p19.CH4_LEN_2,   b.p19.FF20_WRb, b.p19.CH4_LEN_3, b.D3);
  /*p19.DOPU*/ c.p19.CH4_LEN_3b = not(!b.p19.CH4_LEN_3);
  /*p19.FYLO*/ c.p19.CH4_LEN_4  = count_pos(a.p19.CH4_LEN_3b, b.p19.CH4_LEN_3b, b.p19.FF20_WRc,    b.p19.CH4_LEN_4, b.D4);
  /*p19.EDOP*/ c.p19.CH4_LEN_5  = count_pos(a.p19.CH4_LEN_4,  b.p19.CH4_LEN_4,  b.p19.FF20_WRc,    b.p19.CH4_LEN_5, b.D5);

  /*p19.GAPY*/ c.p19.CH4_LEN_RST = nor(b.p19.FF20_WRa, b.p09.APU_RESET1, b.p20.GONE);
  /*p19.FUGO*/ c.p19.CH4_DONE    = tock_pos(!a.p19.CH4_LEN_5, !b.p19.CH4_LEN_5,  b.p19.CH4_LEN_RST, b.p19.CH4_DONE,  !b.p19.CH4_DONE);

  // Where's the readout circuit for FF20?

  //----------
  // FF21

  /*p19.DACO*/ c.p19.FF21_WRa    = and(b.p10.APU_WR, b.p10.ADDR_FF21);
  /*p19.GOKO*/ c.p19.FF21_WRb    = and(b.p10.APU_WR, b.p10.ADDR_FF21);
  /*p19.DYKE*/ c.p19.FF21_WRna   = not(b.p19.FF21_WRa);
  /*p19.FUPA*/ c.p19.FF21_WRnb   = not(b.p19.FF21_WRb);

  /*p19.BOFY*/ c.p19.ADDR_FF21na = not(b.p10.ADDR_FF21);
  /*p19.GONY*/ c.p19.ADDR_FF21nb = not(b.p10.ADDR_FF21);
  /*p19.BOXE*/ c.p19.FF21_RDa    = or(b.p09.CPU_RDn, b.p19.ADDR_FF21na); // polarity?
  /*p19.HASU*/ c.p19.FF21_RDb    = or(b.p09.CPU_RDn, b.p19.ADDR_FF21nb);

  /*p19.EMOK*/ c.p19.CH4_ENV_SWEEP_0 = tock_pos(a.p19.FF21_WRna, b.p19.FF21_WRna, b.p19.FEXO, b.p19.CH4_ENV_SWEEP_0, b.D0);
  /*p19.ETYJ*/ c.p19.CH4_ENV_SWEEP_1 = tock_pos(a.p19.FF21_WRna, b.p19.FF21_WRna, b.p19.FEXO, b.p19.CH4_ENV_SWEEP_1, b.D1);
  /*p19.EZYK*/ c.p19.CH4_ENV_SWEEP_2 = tock_pos(a.p19.FF21_WRna, b.p19.FF21_WRna, b.p19.FEXO, b.p19.CH4_ENV_SWEEP_2, b.D2);
  /*p19.GEKY*/ c.p19.CH4_ENV_DIR     = tock_pos(a.p19.FF21_WRnb, b.p19.FF21_WRnb, b.p19.FEXO, b.p19.CH4_ENV_DIR,     b.D3);
  /*p19.GARU*/ c.p19.CH4_ENV_VOL_0   = tock_pos(a.p19.FF21_WRnb, b.p19.FF21_WRnb, b.p19.FEXO, b.p19.CH4_ENV_VOL_0,   b.D4);
  /*p19.GOKY*/ c.p19.CH4_ENV_VOL_1   = tock_pos(a.p19.FF21_WRnb, b.p19.FF21_WRnb, b.p19.FEXO, b.p19.CH4_ENV_VOL_1,   b.D5);
  /*p19.GOZO*/ c.p19.CH4_ENV_VOL_2   = tock_pos(a.p19.FF21_WRnb, b.p19.FF21_WRnb, b.p19.FEXO, b.p19.CH4_ENV_VOL_2,   b.D6);
  /*p19.GEDU*/ c.p19.CH4_ENV_VOL_3   = tock_pos(a.p19.FF21_WRnb, b.p19.FF21_WRnb, b.p19.FEXO, b.p19.CH4_ENV_VOL_3,   b.D7);

  /*p19.DEMY*/ if (b.p19.FF21_RDa) c.D0 = b.p19.CH4_ENV_SWEEP_0;
  /*p19.COCE*/ if (b.p19.FF21_RDa) c.D1 = b.p19.CH4_ENV_SWEEP_1;
  /*p19.CUZU*/ if (b.p19.FF21_RDa) c.D2 = b.p19.CH4_ENV_SWEEP_2;
  /*p19.GOME*/ if (b.p19.FF21_RDb) c.D3 = b.p19.CH4_ENV_DIR;
  /*p19.HEDA*/ if (b.p19.FF21_RDb) c.D4 = b.p19.CH4_ENV_VOL_0;
  /*p19.GODU*/ if (b.p19.FF21_RDb) c.D5 = b.p19.CH4_ENV_VOL_1;
  /*p19.HOGE*/ if (b.p19.FF21_RDb) c.D6 = b.p19.CH4_ENV_VOL_2;
  /*p19.HACU*/ if (b.p19.FF21_RDb) c.D7 = b.p19.CH4_ENV_VOL_3;

  //----------
  // FF22

  /*p19.HUMO*/ c.p19.FF22_WRa = and(b.p10.APU_WR, b.p10.ADDR_FF22);
  /*p19.GETU*/ c.p19.FF22_WRna = nand(b.p10.APU_WR, b.p10.ADDR_FF22);

  /*p19.HOVA*/ c.p19.FF22_WRnb = not(b.p19.FF22_WRa);
  /*p19.HOSO*/ c.p19.FF22_WRnc = nand(b.p10.APU_WR, b.p10.ADDR_FF22);
  /*p19.EFUG*/ c.p19.FF22_WRnd = not(b.p19.FF22_WRna);

  /*p19.KOKU*/ c.p19.ADDR_FF22na = not(b.p10.ADDR_FF22);
  /*p19.KAGE*/ c.p19.CPU_RDa = not(b.p09.CPU_RDn);
  /*p19.GUGO*/ c.p19.ADDR_FF22nb = not(b.p10.ADDR_FF22);

  /*p19.KEKA*/ c.p19.FF22_RDa = or(b.p09.CPU_RDn, b.p19.ADDR_FF22na);
  /*p19.JORA*/ c.p19.FF22_RDb = nand(b.p10.ADDR_FF22, b.p19.CPU_RDa);
  /*p19.HEZE*/ c.p19.FF22_RDc = or(b.p09.CPU_RDn, b.p19.ADDR_FF22nb);

  /*p19.JARE*/ c.p19.CH4_DIV_0  = tock_pos(a.p19.FF22_WRnb, b.p19.FF22_WRnb, b.p09.APU_RESETn5, b.p19.CH4_DIV_0,  b.D0);
  /*p19.JERO*/ c.p19.CH4_DIV_1  = tock_pos(a.p19.FF22_WRnb, b.p19.FF22_WRnb, b.p09.APU_RESETn5, b.p19.CH4_DIV_1,  b.D1);
  /*p19.JAKY*/ c.p19.CH4_DIV_2  = tock_pos(a.p19.FF22_WRnb, b.p19.FF22_WRnb, b.p09.APU_RESETn5, b.p19.CH4_DIV_2,  b.D2);
  /*p19.JAMY*/ c.p19.CH4_MODE   = tock_pos(a.p19.FF22_WRnc, b.p19.FF22_WRnc, b.p19.HYNE,        b.p19.CH4_MODE,   b.D3);
  /*p19.FETA*/ c.p19.CH4_FREQ_0 = tock_pos(a.p19.FF22_WRnd, b.p19.FF22_WRnd, b.p09.APU_RESETn4, b.p19.CH4_FREQ_0, b.D4);
  /*p19.FYTO*/ c.p19.CH4_FREQ_1 = tock_pos(a.p19.FF22_WRnd, b.p19.FF22_WRnd, b.p09.APU_RESETn4, b.p19.CH4_FREQ_1, b.D5);
  /*p19.GOGO*/ c.p19.CH4_FREQ_2 = tock_pos(a.p19.FF22_WRnd, b.p19.FF22_WRnd, b.p09.APU_RESETn4, b.p19.CH4_FREQ_2, b.D6);
  /*p19.GAFO*/ c.p19.CH4_FREQ_3 = tock_pos(a.p19.FF22_WRnd, b.p19.FF22_WRnd, b.p09.APU_RESETn4, b.p19.CH4_FREQ_3, b.D7);

  /*p19.KAMO*/ if (b.p19.FF22_RDa) c.D0 = b.p19.CH4_DIV_0;
  /*p19.KAKU*/ if (b.p19.FF22_RDa) c.D1 = b.p19.CH4_DIV_1;
  /*p19.KYRO*/ if (b.p19.FF22_RDa) c.D2 = b.p19.CH4_DIV_2;
  /*p19.KETA*/ if (b.p19.FF22_RDb) c.D3 = b.p19.CH4_MODE;
  /*p19.GEDA*/ if (b.p19.FF22_RDc) c.D4 = b.p19.CH4_FREQ_0;
  /*p19.GYPE*/ if (b.p19.FF22_RDc) c.D5 = b.p19.CH4_FREQ_1;
  /*p19.GAKA*/ if (b.p19.FF22_RDc) c.D6 = b.p19.CH4_FREQ_2;
  /*p19.HAPY*/ if (b.p19.FF22_RDc) c.D7 = b.p19.CH4_FREQ_3;

  //----------
  // FF23

  /*p19.DULU*/ c.p19.FF23_WRna = nand(b.p16.CPU_WR_WEIRD, b.p10.ADDR_FF23);
  /*p19.FOXE*/ c.p19.FF23_WRnb = nand(b.p10.APU_WR, b.p10.ADDR_FF23);

  /*p19.BYLO*/ c.p19.CPU_RDb = not(b.p09.CPU_RDn);
  /*p19.BARE*/ c.p19.FF23_RD = nand(b.p10.ADDR_FF23, b.p19.CPU_RDb); // polarity?

  /*p19.CUNY*/ c.p19.CH4_STOP  = tock_pos(a.p19.FF23_WRna, b.p19.FF23_WRna, b.p19.CABE, b.p19.CH4_STOP,  b.D6);
  /*p19.HOGA*/ c.p19.CH4_START = tock_pos(a.p19.FF23_WRnb, b.p19.FF23_WRnb, b.p20.GUZY, b.p19.CH4_START, b.D7);

  /*p19.CURY*/ if (b.p19.FF23_RD) c.D6 = b.p19.CH4_STOP;

}