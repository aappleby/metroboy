#include "../Schematics.h"
#include "Gameboy.h"

void P19_Ch4Regs::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  c.p19.FUGO = tock_pos(!a.p19.EDOP_05, !b.p19.EDOP_05, b.p19.GAPY, b.p19.FUGO, !b.p19.FUGO);
  c.p19.DOPU = not(!b.p19.CEDO_03);
  c.p19.CAZE = nand(b.p10.APU_WR, b.p19.FF20);
  c.p19.DODA = nor(b.p19.FUGO, b.p01.BUFY_256, b.p19.FF23_D6);
  c.p19.FURU = not(b.p19.CAZE);
  c.p19.EPEK = not(b.p19.CAZE);
  c.p19.DOTU = not(b.p19.CAZE);
  c.p19.CUWA = not(b.p19.DODA);
  c.p19.GAPY = nor(b.p19.FURU, b.p09.APU_RESET, b.p19.CH4_RESTART);

  c.p19.DANO_00 = count_pos(a.p19.CUWA,    b.p19.CUWA,    b.p19.DOTU, b.p19.DANO_00, b.D0);
  c.p19.FAVY_01 = count_pos(a.p19.DANO_00, b.p19.DANO_00, b.p19.DOTU, b.p19.FAVY_01, b.D1);
  c.p19.DENA_02 = count_pos(a.p19.FAVY_01, b.p19.FAVY_01, b.p19.DOTU, b.p19.DENA_02, b.D2);
  c.p19.CEDO_03 = count_pos(a.p19.DENA_02, b.p19.DENA_02, b.p19.DOTU, b.p19.CEDO_03, b.D3);
  c.p19.FYLO_04 = count_pos(a.p19.DOPU,    b.p19.DOPU,    b.p19.EPEK, b.p19.FYLO_04, b.D4);
  c.p19.EDOP_05 = count_pos(a.p19.FYLO_04, b.p19.FYLO_04, b.p19.EPEK, b.p19.EDOP_05, b.D5);

  c.p19.GOKO = and(b.p19.FF21, b.p10.APU_WR);
  c.p19.FEXO = not(b.p09.APU_RESET);
  c.p19.FUPA = not(b.p19.GOKO);
  c.p19.GONY = not(b.p19.FF21);
  c.p19.HASU = or(b.p09.CPU_RDn, b.p19.GONY);
  c.p19.DACO = and(b.p10.APU_WR, b.p19.FF21);
  c.p19.DYKE = not(b.p19.DACO);
  c.p19.BOFY = not(b.p19.FF21);
  c.p19.BOXE = or(b.p19.BOFY, b.p09.CPU_RDn);

  c.p19.EMOK_00 = tock_pos(a.p19.DYKE, b.p19.DYKE, b.p19.FEXO, b.p19.EMOK_00, b.D0);
  c.p19.ETYJ_01 = tock_pos(a.p19.DYKE, b.p19.DYKE, b.p19.FEXO, b.p19.ETYJ_01, b.D1);
  c.p19.EZYK_02 = tock_pos(a.p19.DYKE, b.p19.DYKE, b.p19.FEXO, b.p19.EZYK_02, b.D2);
  c.p19.GEKY_03 = tock_pos(a.p19.FUPA, b.p19.FUPA, b.p19.FEXO, b.p19.GEKY_03, b.D3);
  c.p19.GARU_04 = tock_pos(a.p19.FUPA, b.p19.FUPA, b.p19.FEXO, b.p19.GARU_04, b.D4);
  c.p19.GOKY_05 = tock_pos(a.p19.FUPA, b.p19.FUPA, b.p19.FEXO, b.p19.GOKY_05, b.D5);
  c.p19.GOZO_06 = tock_pos(a.p19.FUPA, b.p19.FUPA, b.p19.FEXO, b.p19.GOZO_06, b.D6);
  c.p19.GEDU_07 = tock_pos(a.p19.FUPA, b.p19.FUPA, b.p19.FEXO, b.p19.GEDU_07, b.D7);

  c.p19.DEMY_00 = not(!b.p19.EMOK_00);
  c.p19.COCE_01 = not(!b.p19.ETYJ_01);
  c.p19.CUZU_02 = not(!b.p19.EZYK_02);
  c.p19.GOME_03 = not(!b.p19.GEKY_03);
  c.p19.HEDA_04 = not(!b.p19.GARU_04);
  c.p19.GODU_05 = not(!b.p19.GOKY_05);
  c.p19.HOGE_06 = not(!b.p19.GOZO_06);
  c.p19.HACU_07 = not(!b.p19.GEDU_07);

  if (b.p19.BOXE) {
    c.D0 = b.p19.DEMY_00;
    c.D1 = b.p19.COCE_01;
    c.D2 = b.p19.CUZU_02;
  }

  if (b.p19.HASU) {
    c.D3 = b.p19.GOME_03;
    c.D4 = b.p19.HEDA_04;
    c.D5 = b.p19.GODU_05;
    c.D6 = b.p19.HOGE_06;
    c.D7 = b.p19.HACU_07;
  }

  c.p19.FF21_D0 = b.p19.EMOK_00;
  c.p19.FF21_D1 = b.p19.ETYJ_01;
  c.p19.FF21_D2 = b.p19.EZYK_02;
  c.p19.FF21_D3 = b.p19.GEKY_03;
  c.p19.FF21_D4 = b.p19.GARU_04;
  c.p19.FF21_D5 = b.p19.GOKY_05;
  c.p19.FF21_D6 = b.p19.GOZO_06;
  c.p19.FF21_D7 = b.p19.GEDU_07;

  c.p19.FF21_D0n = !b.p19.EMOK_00;
  c.p19.FF21_D1n = !b.p19.ETYJ_01;
  c.p19.FF21_D2n = !b.p19.EZYK_02;
  c.p19.FF21_D3n = !b.p19.GEKY_03;

  //----------

  c.p19.KAGE = not(b.p09.CPU_RDn);
  c.p19.JORA = nand(b.p19.FF22, b.p19.KAGE);
  c.p19.HOSO = nand(b.p10.APU_WR, b.p19.FF22);
  c.p19.GETU = nand(b.p10.APU_WR, b.p19.FF22);
  c.p19.EFUG = not(b.p19.GETU);
  c.p19.GUGO = not(b.p19.FF22);
  c.p19.HYNE = not(b.p09.APU_RESET);
  c.p19.HEZE = or(b.p19.GUGO, b.p09.CPU_RDn);
  c.p19.HUMO = and(b.p10.APU_WR, b.p19.FF22);
  c.p19.HOVA = not(b.p19.HUMO);
  c.p19.KOKU = not(b.p19.FF22);
  c.p19.KEKA = or(b.p19.KOKU, b.p09.CPU_RDn);

  c.p19.JARE_00 = tock_pos(a.p19.HOVA, b.p19.HOVA, b.p09.APU_RESET5n, b.p19.JARE_00, b.D0);
  c.p19.JERO_01 = tock_pos(a.p19.HOVA, b.p19.HOVA, b.p09.APU_RESET5n, b.p19.JERO_01, b.D1);
  c.p19.JAKY_02 = tock_pos(a.p19.HOVA, b.p19.HOVA, b.p09.APU_RESET5n, b.p19.JAKY_02, b.D2);
  c.p19.JAMY_03 = tock_pos(a.p19.HOSO, b.p19.HOSO, b.p19.HYNE,            b.p19.JAMY_03, b.D3);
  c.p19.FETA_04 = tock_pos(a.p19.EFUG, b.p19.EFUG, b.p09.APU_RESET4n, b.p19.FETA_04, b.D4);
  c.p19.FYTO_05 = tock_pos(a.p19.EFUG, b.p19.EFUG, b.p09.APU_RESET4n, b.p19.FYTO_05, b.D5);
  c.p19.GOGO_06 = tock_pos(a.p19.EFUG, b.p19.EFUG, b.p09.APU_RESET4n, b.p19.GOGO_06, b.D6);
  c.p19.GAFO_07 = tock_pos(a.p19.EFUG, b.p19.EFUG, b.p09.APU_RESET4n, b.p19.GAFO_07, b.D7);

  c.p19.KAMO_00 = not(!b.p19.JARE_00);
  c.p19.KAKU_01 = not(!b.p19.JERO_01);
  c.p19.KYRO_02 = not(!b.p19.JAKY_02);
  c.p19.KETA_03 = not(!b.p19.JAMY_03);
  c.p19.GEDA_04 = not(!b.p19.FETA_04);
  c.p19.GYPE_05 = not(!b.p19.FYTO_05);
  c.p19.GAKA_06 = not(!b.p19.GOGO_06);
  c.p19.HAPY_07 = not(!b.p19.GAFO_07);

  if (b.p19.KEKA) {
    c.D0 = b.p19.KAMO_00;
    c.D1 = b.p19.KAKU_01;
    c.D2 = b.p19.KYRO_02;
  }

  if (b.p19.JORA) {
    c.D3 = b.p19.KETA_03;
  }

  if (b.p19.HEZE) {
    c.D4 = b.p19.GEDA_04;
    c.D5 = b.p19.GYPE_05;
    c.D6 = b.p19.GAKA_06;
    c.D7 = b.p19.HAPY_07;
  }

  c.p19.FF22_D3 = b.p19.JAMY_03;
  c.p19.FF22_D4 = b.p19.FETA_04;
  c.p19.FF22_D5 = b.p19.FYTO_05;
  c.p19.FF22_D6 = b.p19.GOGO_06;
  c.p19.FF22_D7 = b.p19.GAFO_07;

  c.p19.FF22_D0n = !b.p19.JARE_00;
  c.p19.FF22_D1n = !b.p19.JERO_01;
  c.p19.FF22_D2n = !b.p19.JAKY_02;
  c.p19.FF22_D3n = !b.p19.JAMY_03;
  c.p19.FF22_D4n = !b.p19.FETA_04;
  c.p19.FF22_D5n = !b.p19.FYTO_05;
  c.p19.FF22_D6n = !b.p19.GOGO_06;

  //----------

  c.p19.DULU = nand(b.p19.ANUJ, b.p19.FF23);
  c.p19.CABE = not(b.p09.APU_RESET);
  c.p19.BYLO = not(b.p09.CPU_RDn);
  c.p19.FOXE = nand(b.p10.APU_WR, b.p19.FF23);
  c.p19.BARE = nand(b.p19.FF23, b.p19.BYLO);
  c.p19.CUNY = tock_pos(a.p19.DULU, b.p19.DULU, b.p19.CABE, b.p19.CUNY, b.D6);
  c.p19.CURY = not(!b.p19.CUNY);

  if (b.p19.BARE) {
    c.D6 = b.p19.CURY;
  }

  c.p19.FOXE = nand(b.p10.APU_WR, b.p19.FF23);
  c.p19.HOGA = tock_pos(a.p19.FOXE, b.p19.FOXE, b.p19.RST_FF23_D7, b.p19.HOGA, b.D7);

  c.p19.FF23_D6 = b.p19.CUNY;
  c.p19.FF23_D7 = b.p19.HOGA;
}