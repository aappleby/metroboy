// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "AddressDecoder.h"
#include "Clocks.h"
#include "CpuBus.h"
#include "MemBus.h"

//----------
// inputs

extern bool CH1_ACTIVEn;
extern bool CH2_ACTIVEn;
extern bool CH3_ACTIVEn;
extern bool CH4_ACTIVEn;

//----------
// outputs

bool FERO_Q;
bool NET03; // not sure what this is yet

bool CATE;
bool GAXO;

//----------
// registers

reg AJER;
reg CALO;

reg HADA;
reg BOWY;
reg BAZA;
reg FERO;

// FF24 NR50
reg BEDU,COZU,BUMO,BYRE,APOS,AGER,BYGA,APEG;

// FF25 NR51
reg BOGU,BAFO,ATUF,ANEV,BEPU,BEFO,BUME,BOFA;

//-----------------------------------------------------------------------------

void tick_apucontrol() {
  //----------
  // top left

  AJER_2MHZ = AJER.flip(APUV_4MHZ, APU_RESET3n);
  AJER_2MHZn = not(AJER_2MHZ);

  bool BATA = not(AJER_2MHZ);
  bool CALO_Q = CALO.flip(BATA, APU_RESETn);
  bool DYFA = not(!CALO_Q);
  DYFA_1MHZ = DYFA;

  bool DAPA = not(APU_RESET);
  bool AFAT = not(APU_RESET);
  bool AGUR = not(APU_RESET);
  bool ATYV = not(APU_RESET);
  bool KAME = not(APU_RESET);

  APU_RESET4n = DAPA;
  APU_RESET2n = AFAT;
  APU_RESETn = AGUR;
  APU_RESET3n = ATYV;
  APU_RESET5n = KAME;

  bool KYDU = not(CPU_RDn);

  bool HAWU = nand(FF26, CPU_WRQ);
  bool BOPY = nand(CPU_WRQ, FF26);
  bool JURE = nand(KYDU, FF26);
  
  bool HAPO = not(RESET2);
  bool GUFO = not(HAPO);
  bool HADA_Q = HADA.tock(HAWU, GUFO, D7);
  bool JYRO = or(HAPO, !HADA_Q);
  
  bool KUBY = not(JYRO);
  bool KEBA = not(KUBY);
  APU_RESET = KEBA;

  bool HOPE = not(!HADA_Q);
  if (JURE) {
    D7 = HOPE;
  }

  bool KEPY = not(JYRO);
  bool ETUC = and(CPU_WRQ, FF26);
  bool EFOP = and(D4, FROM_CPU);
  bool FOKU = not(ETUC);
  FERO_Q = FERO.tock(FOKU, KEPY, EFOP);
  bool EDEK = not(!FERO_Q);
  NET03 = EDEK;

  bool BOWY_Q = BOWY.tock(BOPY, KEPY, D5);
  bool BAZA_Q = BAZA.tock(AJER_2MHZn, APU_RESET3n, BOWY_Q);
  bool CELY = mux2(BAZA_Q, BYFE_128, NET03);
  bool CONE = not(CELY);
  CATE = not(CONE);

  //----------
  // FF24 NR50

  bool AGUZ = not(CPU_RD);
  CPU_RDn = AGUZ;

  bool BYMA = not(FF24);
  bool BEFU = nor(AGUZ, BYMA);
  bool ADAK = not(BEFU);

  bool BOSU = nand(FF24, CPU_WRQ);

  bool BAXY = not(BOSU);
  bool BUBU = not(BAXY);
  bool BEDU_Q = BEDU.tock(BUBU, JYRO, D7);
  bool COZU_Q = COZU.tock(BUBU, JYRO, D6);
  bool BUMO_Q = BUMO.tock(BUBU, JYRO, D5);
  bool BYRE_Q = BYRE.tock(BUBU, JYRO, D4);

  bool BOWE = not(BOSU);
  bool ATAF = not(BOWE);
  bool APOS_Q = APOS.tock(ATAF, JYRO, D3);
  bool AGER_Q = AGER.tock(ATAF, JYRO, D2);
  bool BYGA_Q = BYGA.tock(ATAF, JYRO, D1);
  bool APEG_Q = APEG.tock(ATAF, JYRO, D0);

  bool ATUM = not(!BEDU_Q);
  bool BOCY = not(!COZU_Q);
  bool ARUX = not(!BUMO_Q);
  bool AMAD = not(!BYRE_Q);
  bool AXEM = not(!APOS_Q);
  bool AVUD = not(!AGER_Q);
  bool AWED = not(!BYGA_Q);
  bool AKOD = not(!APEG_Q);

  if (ADAK) {
    D7 = ATUM;
    D6 = BOCY;
    D5 = ARUX;
    D4 = AMAD;
    D3 = AXEM;
    D2 = AVUD;
    D1 = AWED;
    D0 = AKOD;
  }

  //----------
  // FF25 NR51
  
  bool BUPO = nand(FF25, CPU_WRQ);
  bool BONO = not(BUPO);
  bool BYFA = not(BUPO);

  bool BOGU_Q = BOGU.tock(BONO, JYRO, D1);
  bool BAFO_Q = BAFO.tock(BONO, JYRO, D2);
  bool ATUF_Q = ATUF.tock(BONO, JYRO, D3);
  bool ANEV_Q = ANEV.tock(BONO, JYRO, D0);
  bool BEPU_Q = BEPU.tock(BYFA, JYRO, D7);
  bool BEFO_Q = BEFO.tock(BYFA, JYRO, D6);
  bool BUME_Q = BUME.tock(BYFA, JYRO, D4);
  bool BOFA_Q = BOFA.tock(BYFA, JYRO, D5);

  bool GEPA = not(FF25);
  bool HEFA = nor(GEPA, CPU_RDn);
  bool GUMU = not(HEFA);

  bool CAPU = not(!BOGU_Q);
  bool CAGA = not(!BAFO_Q);
  bool BOCA = not(!ATUF_Q);
  bool BUZU = not(!ANEV_Q);
  bool CERE = not(!BEPU_Q);
  bool CADA = not(!BEFO_Q);
  bool CAVU = not(!BUME_Q);
  bool CUDU = not(!BOFA_Q);

  if (GUMU) {
    D1 = CAPU;
    D2 = CAGA;
    D3 = BOCA;
    D0 = BUZU;
    D7 = CERE;
    D6 = CADA;
    D4 = CAVU;
    D5 = CUDU;
  }

  //----------
  // FF26 NR52

  bool CETO = not(CPU_RDn);
  bool KAZO = not(CPU_RDn);
  bool CURU = not(CPU_RDn);
  GAXO = not(CPU_RDn);

  bool DOLE = nand(FF26, CETO);
  bool KAMU = nand(FF26, KAZO);
  bool DURU = nand(FF26, CURU);
  bool FEWA = nand(FF26, GAXO);

  bool COTO = not(CH1_ACTIVEn);
  bool KOGE = not(CH4_ACTIVEn);
  bool EFUS = not(CH2_ACTIVEn);
  bool FATE = not(CH3_ACTIVEn);

  if (DOLE) D0 = COTO;
  if (KAMU) D3 = KOGE;
  if (DURU) D1 = EFUS;
  if (FEWA) D2 = FATE;
}