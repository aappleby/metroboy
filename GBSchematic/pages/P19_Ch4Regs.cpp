#include "../Schematics.h"
#include "Gameboy.h"

void P19_Ch4Regs::tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc) {
  const P19_Ch4Regs pa = {};
  const P19_Ch4Regs pb = {};
  P19_Ch4Regs pc;

  (void)ga;

  pc.FUGO = tock_pos(!pa.EDOP_05, !pb.EDOP_05, pb.GAPY, pb.FUGO, !pb.FUGO);
  pc.DOPU = not(!pb.CEDO_03);
  pc.CAZE = nand(gb.p10.APU_WR, pb.FF20);
  pc.DODA = nor(pb.FUGO, gb.p01.BUFY_256, pb.FF23_D6);
  pc.FURU = not(pb.CAZE);
  pc.EPEK = not(pb.CAZE);
  pc.DOTU = not(pb.CAZE);
  pc.CUWA = not(pb.DODA);
  pc.GAPY = nor(pb.FURU, gb.p09.APU_RESET, pb.CH4_RESTART);

  pc.DANO_00 = count_pos(pa.CUWA,    pb.CUWA,    pb.DOTU, pb.DANO_00, gb.D0);
  pc.FAVY_01 = count_pos(pa.DANO_00, pb.DANO_00, pb.DOTU, pb.FAVY_01, gb.D1);
  pc.DENA_02 = count_pos(pa.FAVY_01, pb.FAVY_01, pb.DOTU, pb.DENA_02, gb.D2);
  pc.CEDO_03 = count_pos(pa.DENA_02, pb.DENA_02, pb.DOTU, pb.CEDO_03, gb.D3);
  pc.FYLO_04 = count_pos(pa.DOPU,    pb.DOPU,    pb.EPEK, pb.FYLO_04, gb.D4);
  pc.EDOP_05 = count_pos(pa.FYLO_04, pb.FYLO_04, pb.EPEK, pb.EDOP_05, gb.D5);

  pc.GOKO = and(pb.FF21, gb.p10.APU_WR);
  pc.FEXO = not(gb.p09.APU_RESET);
  pc.FUPA = not(pb.GOKO);
  pc.GONY = not(pb.FF21);
  pc.HASU = or(gb.p09.CPU_RDn, pb.GONY);
  pc.DACO = and(gb.p10.APU_WR, pb.FF21);
  pc.DYKE = not(pb.DACO);
  pc.BOFY = not(pb.FF21);
  pc.BOXE = or(pb.BOFY, gb.p09.CPU_RDn);

  pc.EMOK_00 = tock_pos(pa.DYKE, pb.DYKE, pb.FEXO, pb.EMOK_00, gb.D0);
  pc.ETYJ_01 = tock_pos(pa.DYKE, pb.DYKE, pb.FEXO, pb.ETYJ_01, gb.D1);
  pc.EZYK_02 = tock_pos(pa.DYKE, pb.DYKE, pb.FEXO, pb.EZYK_02, gb.D2);
  pc.GEKY_03 = tock_pos(pa.FUPA, pb.FUPA, pb.FEXO, pb.GEKY_03, gb.D3);
  pc.GARU_04 = tock_pos(pa.FUPA, pb.FUPA, pb.FEXO, pb.GARU_04, gb.D4);
  pc.GOKY_05 = tock_pos(pa.FUPA, pb.FUPA, pb.FEXO, pb.GOKY_05, gb.D5);
  pc.GOZO_06 = tock_pos(pa.FUPA, pb.FUPA, pb.FEXO, pb.GOZO_06, gb.D6);
  pc.GEDU_07 = tock_pos(pa.FUPA, pb.FUPA, pb.FEXO, pb.GEDU_07, gb.D7);

  pc.DEMY_00 = not(!pb.EMOK_00);
  pc.COCE_01 = not(!pb.ETYJ_01);
  pc.CUZU_02 = not(!pb.EZYK_02);
  pc.GOME_03 = not(!pb.GEKY_03);
  pc.HEDA_04 = not(!pb.GARU_04);
  pc.GODU_05 = not(!pb.GOKY_05);
  pc.HOGE_06 = not(!pb.GOZO_06);
  pc.HACU_07 = not(!pb.GEDU_07);

  if (pb.BOXE) {
    gc.D0 = pb.DEMY_00;
    gc.D1 = pb.COCE_01;
    gc.D2 = pb.CUZU_02;
  }

  if (pb.HASU) {
    gc.D3 = pb.GOME_03;
    gc.D4 = pb.HEDA_04;
    gc.D5 = pb.GODU_05;
    gc.D6 = pb.HOGE_06;
    gc.D7 = pb.HACU_07;
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

  pc.KAGE = not(gb.p09.CPU_RDn);
  pc.JORA = nand(pb.FF22, pb.KAGE);
  pc.HOSO = nand(gb.p10.APU_WR, pb.FF22);
  pc.GETU = nand(gb.p10.APU_WR, pb.FF22);
  pc.EFUG = not(pb.GETU);
  pc.GUGO = not(pb.FF22);
  pc.HYNE = not(gb.p09.APU_RESET);
  pc.HEZE = or(pb.GUGO, gb.p09.CPU_RDn);
  pc.HUMO = and(gb.p10.APU_WR, pb.FF22);
  pc.HOVA = not(pb.HUMO);
  pc.KOKU = not(pb.FF22);
  pc.KEKA = or(pb.KOKU, gb.p09.CPU_RDn);

  pc.JARE_00 = tock_pos(pa.HOVA, pb.HOVA, gb.p09.APU_RESET5n, pb.JARE_00, gb.D0);
  pc.JERO_01 = tock_pos(pa.HOVA, pb.HOVA, gb.p09.APU_RESET5n, pb.JERO_01, gb.D1);
  pc.JAKY_02 = tock_pos(pa.HOVA, pb.HOVA, gb.p09.APU_RESET5n, pb.JAKY_02, gb.D2);
  pc.JAMY_03 = tock_pos(pa.HOSO, pb.HOSO, pb.HYNE,            pb.JAMY_03, gb.D3);
  pc.FETA_04 = tock_pos(pa.EFUG, pb.EFUG, gb.p09.APU_RESET4n, pb.FETA_04, gb.D4);
  pc.FYTO_05 = tock_pos(pa.EFUG, pb.EFUG, gb.p09.APU_RESET4n, pb.FYTO_05, gb.D5);
  pc.GOGO_06 = tock_pos(pa.EFUG, pb.EFUG, gb.p09.APU_RESET4n, pb.GOGO_06, gb.D6);
  pc.GAFO_07 = tock_pos(pa.EFUG, pb.EFUG, gb.p09.APU_RESET4n, pb.GAFO_07, gb.D7);

  pc.KAMO_00 = not(!pb.JARE_00);
  pc.KAKU_01 = not(!pb.JERO_01);
  pc.KYRO_02 = not(!pb.JAKY_02);
  pc.KETA_03 = not(!pb.JAMY_03);
  pc.GEDA_04 = not(!pb.FETA_04);
  pc.GYPE_05 = not(!pb.FYTO_05);
  pc.GAKA_06 = not(!pb.GOGO_06);
  pc.HAPY_07 = not(!pb.GAFO_07);

  if (pb.KEKA) {
    gc.D0 = pb.KAMO_00;
    gc.D1 = pb.KAKU_01;
    gc.D2 = pb.KYRO_02;
  }

  if (pb.JORA) {
    gc.D3 = pb.KETA_03;
  }

  if (pb.HEZE) {
    gc.D4 = pb.GEDA_04;
    gc.D5 = pb.GYPE_05;
    gc.D6 = pb.GAKA_06;
    gc.D7 = pb.HAPY_07;
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
  pc.CABE = not(gb.p09.APU_RESET);
  pc.BYLO = not(gb.p09.CPU_RDn);
  pc.FOXE = nand(gb.p10.APU_WR, pb.FF23);
  pc.BARE = nand(pb.FF23, pb.BYLO);
  pc.CUNY = tock_pos(pa.DULU, pb.DULU, pb.CABE, pb.CUNY, gb.D6);
  pc.CURY = not(!pb.CUNY);

  if (pb.BARE) {
    gc.D6 = pb.CURY;
  }

  pc.FOXE = nand(gb.p10.APU_WR, pb.FF23);
  pc.HOGA = tock_pos(pa.FOXE, pb.FOXE, pb.RST_FF23_D7, pb.HOGA, gb.D7);

  pc.FF23_D6 = pb.CUNY;
  pc.FF23_D7 = pb.HOGA;
}