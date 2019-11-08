#include "../Schematics.h"
#include "Gameboy.h"

struct P19_Ch4Regs {

  bool DANO_00;
  bool FAVY_01;
  bool DENA_02;
  bool CEDO_03;
  bool FYLO_04;
  bool EDOP_05;

  bool FUGO,DOPU,CAZE,DODA,FURU,EPEK,DOTU,CUWA,GAPY;


  bool EMOK_00; bool DEMY_00;
  bool ETYJ_01; bool COCE_01;
  bool EZYK_02; bool CUZU_02;
  bool GEKY_03; bool GOME_03;
  bool GARU_04; bool HEDA_04;
  bool GOKY_05; bool GODU_05;
  bool GOZO_06; bool HOGE_06;
  bool GEDU_07; bool HACU_07;

  bool FEXO,GONY,HASU,GOKO,FUPA,DACO,DYKE,BOFY,BOXE;

  bool JARE_00; bool KAMO_00;
  bool JERO_01; bool KAKU_01;
  bool JAKY_02; bool KYRO_02;
  bool JAMY_03; bool KETA_03;
  bool FETA_04; bool GEDA_04;
  bool FYTO_05; bool GYPE_05;
  bool GOGO_06; bool GAKA_06;
  bool GAFO_07; bool HAPY_07;

  bool KAGE,JORA,GUGO,HEZE,HYNE,HOSO,GETU,EFUG,HUMO,HOVA,KOKU,KEKA;

  bool DULU,CABE,BYLO,FOXE,BARE,CUNY,HOGA,CURY;

  //----------

  bool FF20; // P10.DANU
  bool FF21; // P10.COVO
  bool FF22; // P10.EKEZ
  bool FF23; // P10.CUGE

  bool FF21_D0;
  bool FF21_D1;
  bool FF21_D2;
  bool FF21_D3;
  bool FF21_D4;
  bool FF21_D5;
  bool FF21_D6;
  bool FF21_D7;

  bool FF21_D0n;
  bool FF21_D1n;
  bool FF21_D2n;
  bool FF21_D3n;

  bool FF22_D3;
  bool FF22_D4;
  bool FF22_D5;
  bool FF22_D6;
  bool FF22_D7;

  bool FF22_D0n;
  bool FF22_D1n;
  bool FF22_D2n;
  bool FF22_D3n;
  bool FF22_D4n;
  bool FF22_D5n;
  bool FF22_D6n;
  
  bool FF23_D6;
  bool FF23_D7;

  bool CH4_RESTART; // P20.GONE
  bool RST_FF23_D7; // P20.GUZY
  bool ANUJ;        // P16.ANUJ

  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);
};

void P19_Ch4Regs::tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc) {
  const P19_Ch4Regs pa = {};
  const P19_Ch4Regs pb = {};
  P19_Ch4Regs pc;

  (void)ga;

  pc.FUGO = tock_pos(!pa.EDOP_05, !pb.EDOP_05, pb.GAPY, pb.FUGO, !pb.FUGO);
  pc.DOPU = not(!pb.CEDO_03);
  pc.CAZE = nand(gb.APU_WR, pb.FF20);
  pc.DODA = nor(pb.FUGO, gb.BUFY_256, pb.FF23_D6);
  pc.FURU = not(pb.CAZE);
  pc.EPEK = not(pb.CAZE);
  pc.DOTU = not(pb.CAZE);
  pc.CUWA = not(pb.DODA);
  pc.GAPY = nor(pb.FURU, gb.APU_RESET, pb.CH4_RESTART);

  pc.DANO_00 = count_pos(pa.CUWA,    pb.CUWA,    pb.DOTU, pb.DANO_00, gb.cpu.D0);
  pc.FAVY_01 = count_pos(pa.DANO_00, pb.DANO_00, pb.DOTU, pb.FAVY_01, gb.cpu.D1);
  pc.DENA_02 = count_pos(pa.FAVY_01, pb.FAVY_01, pb.DOTU, pb.DENA_02, gb.cpu.D2);
  pc.CEDO_03 = count_pos(pa.DENA_02, pb.DENA_02, pb.DOTU, pb.CEDO_03, gb.cpu.D3);
  pc.FYLO_04 = count_pos(pa.DOPU,    pb.DOPU,    pb.EPEK, pb.FYLO_04, gb.cpu.D4);
  pc.EDOP_05 = count_pos(pa.FYLO_04, pb.FYLO_04, pb.EPEK, pb.EDOP_05, gb.cpu.D5);

  pc.GOKO = and(pb.FF21, gb.APU_WR);
  pc.FEXO = not(gb.APU_RESET);
  pc.FUPA = not(pb.GOKO);
  pc.GONY = not(pb.FF21);
  pc.HASU = or(gb.CPU_RDn, pb.GONY);
  pc.DACO = and(gb.APU_WR, pb.FF21);
  pc.DYKE = not(pb.DACO);
  pc.BOFY = not(pb.FF21);
  pc.BOXE = or(pb.BOFY, gb.CPU_RDn);

  pc.EMOK_00 = tock_pos(pa.DYKE, pb.DYKE, pb.FEXO, pb.EMOK_00, gb.cpu.D0);
  pc.ETYJ_01 = tock_pos(pa.DYKE, pb.DYKE, pb.FEXO, pb.ETYJ_01, gb.cpu.D1);
  pc.EZYK_02 = tock_pos(pa.DYKE, pb.DYKE, pb.FEXO, pb.EZYK_02, gb.cpu.D2);
  pc.GEKY_03 = tock_pos(pa.FUPA, pb.FUPA, pb.FEXO, pb.GEKY_03, gb.cpu.D3);
  pc.GARU_04 = tock_pos(pa.FUPA, pb.FUPA, pb.FEXO, pb.GARU_04, gb.cpu.D4);
  pc.GOKY_05 = tock_pos(pa.FUPA, pb.FUPA, pb.FEXO, pb.GOKY_05, gb.cpu.D5);
  pc.GOZO_06 = tock_pos(pa.FUPA, pb.FUPA, pb.FEXO, pb.GOZO_06, gb.cpu.D6);
  pc.GEDU_07 = tock_pos(pa.FUPA, pb.FUPA, pb.FEXO, pb.GEDU_07, gb.cpu.D7);

  pc.DEMY_00 = not(!pb.EMOK_00);
  pc.COCE_01 = not(!pb.ETYJ_01);
  pc.CUZU_02 = not(!pb.EZYK_02);
  pc.GOME_03 = not(!pb.GEKY_03);
  pc.HEDA_04 = not(!pb.GARU_04);
  pc.GODU_05 = not(!pb.GOKY_05);
  pc.HOGE_06 = not(!pb.GOZO_06);
  pc.HACU_07 = not(!pb.GEDU_07);

  if (pb.BOXE) {
    gc.cpu.D0 = pb.DEMY_00;
    gc.cpu.D1 = pb.COCE_01;
    gc.cpu.D2 = pb.CUZU_02;
  }

  if (pb.HASU) {
    gc.cpu.D3 = pb.GOME_03;
    gc.cpu.D4 = pb.HEDA_04;
    gc.cpu.D5 = pb.GODU_05;
    gc.cpu.D6 = pb.HOGE_06;
    gc.cpu.D7 = pb.HACU_07;
  }

  pc.FF21_D0 = pb.EMOK_00;
  pc.FF21_D1 = pb.ETYJ_01;
  pc.FF21_D2 = pb.EZYK_02;
  pc.FF21_D3 = pb.GEKY_03;
  pc.FF21_D4 = pb.GARU_04;
  pc.FF21_D5 = pb.GOKY_05;
  pc.FF21_D6 = pb.GOZO_06;
  pc.FF21_D7 = pb.GEDU_07;

  pc.FF21_D0n = !pb.EMOK_00;
  pc.FF21_D1n = !pb.ETYJ_01;
  pc.FF21_D2n = !pb.EZYK_02;
  pc.FF21_D3n = !pb.GEKY_03;

  //----------

  pc.KAGE = not(gb.CPU_RDn);
  pc.JORA = nand(pb.FF22, pb.KAGE);
  pc.HOSO = nand(gb.APU_WR, pb.FF22);
  pc.GETU = nand(gb.APU_WR, pb.FF22);
  pc.EFUG = not(pb.GETU);
  pc.GUGO = not(pb.FF22);
  pc.HYNE = not(gb.APU_RESET);
  pc.HEZE = or(pb.GUGO, gb.CPU_RDn);
  pc.HUMO = and(gb.APU_WR, pb.FF22);
  pc.HOVA = not(pb.HUMO);
  pc.KOKU = not(pb.FF22);
  pc.KEKA = or(pb.KOKU, gb.CPU_RDn);

  pc.JARE_00 = tock_pos(pa.HOVA, pb.HOVA, gb.APU_RESET5n, pb.JARE_00, gb.cpu.D0);
  pc.JERO_01 = tock_pos(pa.HOVA, pb.HOVA, gb.APU_RESET5n, pb.JERO_01, gb.cpu.D1);
  pc.JAKY_02 = tock_pos(pa.HOVA, pb.HOVA, gb.APU_RESET5n, pb.JAKY_02, gb.cpu.D2);
  pc.JAMY_03 = tock_pos(pa.HOSO, pb.HOSO, pb.HYNE,        pb.JAMY_03, gb.cpu.D3);
  pc.FETA_04 = tock_pos(pa.EFUG, pb.EFUG, gb.APU_RESET4n, pb.FETA_04, gb.cpu.D4);
  pc.FYTO_05 = tock_pos(pa.EFUG, pb.EFUG, gb.APU_RESET4n, pb.FYTO_05, gb.cpu.D5);
  pc.GOGO_06 = tock_pos(pa.EFUG, pb.EFUG, gb.APU_RESET4n, pb.GOGO_06, gb.cpu.D6);
  pc.GAFO_07 = tock_pos(pa.EFUG, pb.EFUG, gb.APU_RESET4n, pb.GAFO_07, gb.cpu.D7);

  pc.KAMO_00 = not(!pb.JARE_00);
  pc.KAKU_01 = not(!pb.JERO_01);
  pc.KYRO_02 = not(!pb.JAKY_02);
  pc.KETA_03 = not(!pb.JAMY_03);
  pc.GEDA_04 = not(!pb.FETA_04);
  pc.GYPE_05 = not(!pb.FYTO_05);
  pc.GAKA_06 = not(!pb.GOGO_06);
  pc.HAPY_07 = not(!pb.GAFO_07);

  if (pb.KEKA) {
    gc.cpu.D0 = pb.KAMO_00;
    gc.cpu.D1 = pb.KAKU_01;
    gc.cpu.D2 = pb.KYRO_02;
  }

  if (pb.JORA) {
    gc.cpu.D3 = pb.KETA_03;
  }

  if (pb.HEZE) {
    gc.cpu.D4 = pb.GEDA_04;
    gc.cpu.D5 = pb.GYPE_05;
    gc.cpu.D6 = pb.GAKA_06;
    gc.cpu.D7 = pb.HAPY_07;
  }

  pc.FF22_D3 = pb.JAMY_03;
  pc.FF22_D4 = pb.FETA_04;
  pc.FF22_D5 = pb.FYTO_05;
  pc.FF22_D6 = pb.GOGO_06;
  pc.FF22_D7 = pb.GAFO_07;

  pc.FF22_D0n = !pb.JARE_00;
  pc.FF22_D1n = !pb.JERO_01;
  pc.FF22_D2n = !pb.JAKY_02;
  pc.FF22_D3n = !pb.JAMY_03;
  pc.FF22_D4n = !pb.FETA_04;
  pc.FF22_D5n = !pb.FYTO_05;
  pc.FF22_D6n = !pb.GOGO_06;

  //----------

  pc.DULU = nand(pb.ANUJ, pb.FF23);
  pc.CABE = not(gb.APU_RESET);
  pc.BYLO = not(gb.CPU_RDn);
  pc.FOXE = nand(gb.APU_WR, pb.FF23);
  pc.BARE = nand(pb.FF23, pb.BYLO);
  pc.CUNY = tock_pos(pa.DULU, pb.DULU, pb.CABE, pb.CUNY, gb.cpu.D6);
  pc.CURY = not(!pb.CUNY);

  if (pb.BARE) {
    gc.cpu.D6 = pb.CURY;
  }

  pc.FOXE = nand(gb.APU_WR, pb.FF23);
  pc.HOGA = tock_pos(pa.FOXE, pb.FOXE, pb.RST_FF23_D7, pb.HOGA, gb.cpu.D7);

  pc.FF23_D6 = pb.CUNY;
  pc.FF23_D7 = pb.HOGA;
}