#pragma once

struct P19_Ch4Regs {
public:

  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);

  bool DOPU;

///private:

  bool DANO_00;
  bool FAVY_01;
  bool DENA_02;
  bool CEDO_03;
  bool FYLO_04;
  bool EDOP_05;

  bool FUGO,CAZE,DODA,FURU,EPEK,DOTU,CUWA,GAPY;


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

};

