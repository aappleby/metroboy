// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

//----------
// inputs

extern bool CLK3;
extern bool COTA;
extern bool WEWU;

extern bool OAM_A_D0;
extern bool OAM_A_D1;
extern bool OAM_A_D2;
extern bool OAM_A_D3;
extern bool OAM_A_D4;
extern bool OAM_A_D5;
extern bool OAM_A_D6;
extern bool OAM_A_D7;

// sprite resets
extern bool DYNA, WUPA, GAFY, ASYS, ZAPE, WUNU, WUZO, DOSY, EJAD, CACU;

// sprite clocks
extern bool FUXU, YFAG, GECY, DOKU, XAHO, WOFO, CEXU, WEME, CYLA, GAMY;

// screen x coordinate
extern bool ACAM, AZUB, AMEL, AHAL, APUX, ABEF, ADAZ, ASAH;

//----------
// outputs

bool XEBA, ZAKO;
bool YWOS, ZURE;
bool DAJE, CYCO;
bool CYVY, EWAM;
bool YWAP, YDOT;
bool YKOK, YNAZ;
bool DAMA, FEHA;
bool YTUB, YLEV;
bool COGY, FYMA;
bool CEHU, EKES;

//----------
// registers

// oam_a_d latch
reg WYNO, CYRA, ZUVE, ECED, XYKY, YRUM, YSEX, YVEL;

reg GOMO, BAXO, YZOS, DEPO; // note, these use !Q as their output signal
reg YLOR, ZYTY, ZYVE, ZEZY;

// match registers
reg WELO, XUNY, WOTE, XAKO, XEPE, YLAH, ZOLA, ZULU;
reg XOMY, WUHA, WYNA, WECO, XOLY, XYBA, XABE, XEKA;
reg FAZU, FAXE, EXUK, FEDE, ERAZ, EPUM, EROL, EHYN;
reg DAKE, CESO, DYFU, CUSY, DANY, DUKO, DESU, DAZO;
reg ZOLY, ZOGO, ZECU, ZESA, YCOL, YRAC, YMEM, YVAG;
reg YBED, ZALA, WYDE, XEPA, WEDU, YGAJ, ZYJO, XURY;
reg EZUF, ENAD, EBOW, FYCA, GAVY, GYPU, GADY, GAZA;
reg YPOD, YROP, YNEP, YZOF, XUVY, XERE, XUZO, XEXA;
reg CYWE, DYBY, DURY, CUVY, FUSA, FAXA, FOZY, FESY;
reg DUHY, EJUF, ENOR, DEPY, FOKA, FYTY, FUBY, GOXU;


//-----------------------------------------------------------------------------

void tick_spritexmatcher() {
  bool WYNO_Q = WYNO.latch(CLK3, OAM_A_D4);
  bool CYRA_Q = CYRA.latch(CLK3, OAM_A_D5);
  bool ZUVE_Q = ZUVE.latch(CLK3, OAM_A_D6);
  bool ECED_Q = ECED.latch(CLK3, OAM_A_D7);
  bool XYKY_Q = XYKY.latch(CLK3, OAM_A_D0);
  bool YRUM_Q = YRUM.latch(CLK3, OAM_A_D1);
  bool YSEX_Q = YSEX.latch(CLK3, OAM_A_D2);
  bool YVEL_Q = YVEL.latch(CLK3, OAM_A_D3);

  // ordering is weird
  bool XACA = not(!XYKY_Q);
  bool XAGU = not(!YRUM_Q);
  bool XEPU = not(!YSEX_Q);
  bool XYGU = not(!YVEL_Q);
  bool XUNA = not(!WYNO_Q);
  bool DEVE = not(!CYRA_Q);
  bool ZEHA = not(!ZUVE_Q);
  bool FYRA = not(!ECED_Q);

  if (WEWU) {
    D0 = XACA;
    D1 = XAGU;
    D2 = XEPU;
    D3 = XYGU;
    D4 = XUNA;
    D5 = DEVE;
    D6 = ZEHA;
    D7 = FYRA;
  }

  bool XEGA = not(COTA);
  bool GOMO_Q = GOMO.tock(XEGA, 0, WYNO_Q);
  bool BAXO_Q = BAXO.tock(XEGA, 0, CYRA_Q);
  bool YZOS_Q = YZOS.tock(XEGA, 0, ZUVE_Q);
  bool DEPO_Q = DEPO.tock(XEGA, 0, ECED_Q);
  bool YLOR_Q = YLOR.tock(XEGA, 0, XYKY_Q);
  bool ZYTY_Q = ZYTY.tock(XEGA, 0, YRUM_Q);
  bool ZYVE_Q = ZYVE.tock(XEGA, 0, YSEX_Q);
  bool ZEZY_Q = ZEZY.tock(XEGA, 0, YVEL_Q);

  bool COSE = not(!GOMO_Q);
  bool AROP = not(!BAXO_Q);
  bool XATU = not(!YZOS_Q);
  bool BADY = not(!DEPO_Q);
  bool ZAGO = not(!YLOR_Q);
  bool ZOCY = not(!ZYTY_Q);
  bool YPUR = not(!ZYVE_Q);
  bool YVOK = not(!ZEZY_Q);

  // matcher 1
  bool WELO_Q = WELO.tock(FUXU, DYNA, COSE);
  bool XUNY_Q = XUNY.tock(FUXU, DYNA, AROP);
  bool WOTE_Q = WOTE.tock(FUXU, DYNA, XATU);
  bool XAKO_Q = XAKO.tock(FUXU, DYNA, BADY);
  bool XEPE_Q = XEPE.tock(FUXU, DYNA, ZAGO);
  bool YLAH_Q = YLAH.tock(FUXU, DYNA, ZOCY);
  bool ZOLA_Q = ZOLA.tock(FUXU, DYNA, YPUR);
  bool ZULU_Q = ZULU.tock(FUXU, DYNA, YVOK);

  bool WOJU = xor(WELO_Q, APUX);
  bool YFUN = xor(XUNY_Q, ABEF);
  bool WYZA = xor(WOTE_Q, ADAZ);
  bool YPUK = xor(XAKO_Q, ASAH);
  bool ZOGY = xor(XEPE_Q, ACAM);
  bool ZEBA = xor(YLAH_Q, AZUB);
  bool ZAHA = xor(ZOLA_Q, AMEL);
  bool ZOKY = xor(ZULU_Q, AHAL);
  XEBA = nor(WOJU, YFUN, WYZA, YPUK);
  ZAKO = nor(ZOGY, ZEBA, ZAHA, ZOKY);

  // matcher 2
  bool XOMY_Q = XOMY.tock(YFAG, WUPA, COSE);
  bool WUHA_Q = WUHA.tock(YFAG, WUPA, AROP);
  bool WYNA_Q = WYNA.tock(YFAG, WUPA, XATU);
  bool WECO_Q = WECO.tock(YFAG, WUPA, BADY);
  bool XOLY_Q = XOLY.tock(YFAG, WUPA, ZAGO);
  bool XYBA_Q = XYBA.tock(YFAG, WUPA, ZOCY);
  bool XABE_Q = XABE.tock(YFAG, WUPA, YPUR);
  bool XEKA_Q = XEKA.tock(YFAG, WUPA, YVOK);

  bool YVAP = xor(XOMY_Q, APUX);
  bool XENY = xor(WUHA_Q, ABEF);
  bool XAVU = xor(WYNA_Q, ADAZ);
  bool XEVA = xor(WECO_Q, ASAH);
  bool YHOK = xor(XOLY_Q, ACAM);
  bool YCAH = xor(XYBA_Q, AZUB);
  bool YDAJ = xor(XABE_Q, AMEL);
  bool YVUZ = xor(XEKA_Q, AHAL);
  YWOS = nor(YVAP, XENY, XAVU, XEVA);
  ZURE = nor(YHOK, YCAH, YDAJ, YVUZ);
}