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

  // CHECK CLK/RESET WIRES

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

  // matcher 3
  bool FAZU_Q = FAZU.tock(GECY, GAFY, COSE);
  bool FAXE_Q = FAXE.tock(GECY, GAFY, AROP);
  bool EXUK_Q = EXUK.tock(GECY, GAFY, XATU);
  bool FEDE_Q = FEDE.tock(GECY, GAFY, BADY);
  bool ERAZ_Q = ERAZ.tock(GECY, GAFY, ZAGO);
  bool EPUM_Q = EPUM.tock(GECY, GAFY, ZOCY);
  bool EROL_Q = EROL.tock(GECY, GAFY, YPUR);
  bool EHYN_Q = EHYN.tock(GECY, GAFY, YVOK);

  bool EJOT = xor(FAZU_Q, APUX);
  bool ESAJ = xor(FAXE_Q, ABEF);
  bool DUCU = xor(EXUK_Q, ADAZ);
  bool EWUD = xor(FEDE_Q, ASAH);
  bool DUSE = xor(ERAZ_Q, ACAM);
  bool DAGU = xor(EPUM_Q, AZUB);
  bool DYZE = xor(EROL_Q, AMEL);
  bool DESO = xor(EHYN_Q, AHAL);
  DAJE = nor(EJOT, ESAJ, DUCU, EWUD);
  CYCO = nor(DUSE, DAGU, DYZE, DESO);

  // matcher 4
  bool DAKE_Q = DAKE.tock(ASYS, DOKU, COSE);
  bool CESO_Q = CESO.tock(ASYS, DOKU, AROP);
  bool DYFU_Q = DYFU.tock(ASYS, DOKU, XATU);
  bool CUSY_Q = CUSY.tock(ASYS, DOKU, BADY);
  bool DANY_Q = DANY.tock(ASYS, DOKU, ZAGO);
  bool DUKO_Q = DUKO.tock(ASYS, DOKU, ZOCY);
  bool DESU_Q = DESU.tock(ASYS, DOKU, YPUR);
  bool DAZO_Q = DAZO.tock(ASYS, DOKU, YVOK);

  bool COLA = xor(DAKE_Q, APUX);
  bool BOBA = xor(CESO_Q, ABEF);
  bool COLU = xor(DYFU_Q, ADAZ);
  bool BAHU = xor(CUSY_Q, ASAH);
  bool EDYM = xor(DANY_Q, ACAM);
  bool EMYB = xor(DUKO_Q, AZUB);
  bool EBEF = xor(DESU_Q, AMEL);
  bool EWOK = xor(DAZO_Q, AHAL);
  CYVY = nor(COLA, BOBA, COLU, BAHU);
  EWAM = nor(EDYM, EMYB, EBEF, EWOK);

  // matcher 5
  bool ZOLY_Q = ZOLY.tock(ZAPE, XAHO, COSE);
  bool ZOGO_Q = ZOGO.tock(ZAPE, XAHO, AROP);
  bool ZECU_Q = ZECU.tock(ZAPE, XAHO, XATU);
  bool ZESA_Q = ZESA.tock(ZAPE, XAHO, BADY);
  bool YCOL_Q = YCOL.tock(ZAPE, XAHO, ZAGO);
  bool YRAC_Q = YRAC.tock(ZAPE, XAHO, ZOCY);
  bool YMEM_Q = YMEM.tock(ZAPE, XAHO, YPUR);
  bool YVAG_Q = YVAG.tock(ZAPE, XAHO, YVOK);

  bool ZARE = xor(ZOLY_Q, APUX);
  bool ZEMU = xor(ZOGO_Q, ABEF);
  bool ZYGO = xor(ZECU_Q, ADAZ);
  bool ZUZY = xor(ZESA_Q, ASAH);
  bool XOSU = xor(YCOL_Q, ACAM);
  bool ZUVU = xor(YRAC_Q, AZUB);
  bool XUCO = xor(YMEM_Q, AMEL);
  bool ZULO = xor(YVAG_Q, AHAL);
  YWAP = nor(ZARE, ZEMU, ZYGO, ZUZY);
  YDOT = nor(XOSU, ZUVU, XUCO, ZULO);

  // matcher 6
  bool YBED_Q = YBED.tock(WUNU, WOFO, COSE);
  bool ZALA_Q = ZALA.tock(WUNU, WOFO, AROP);
  bool WYDE_Q = WYDE.tock(WUNU, WOFO, XATU);
  bool XEPA_Q = XEPA.tock(WUNU, WOFO, BADY);
  bool WEDU_Q = WEDU.tock(WUNU, WOFO, ZAGO);
  bool YGAJ_Q = YGAJ.tock(WUNU, WOFO, ZOCY);
  bool ZYJO_Q = ZYJO.tock(WUNU, WOFO, YPUR);
  bool XURY_Q = XURY.tock(WUNU, WOFO, YVOK);

  bool ZYKU = xor(YBED_Q, APUX);
  bool ZYPU = xor(ZALA_Q, ABEF);
  bool XAHA = xor(WYDE_Q, ADAZ);
  bool ZEFE = xor(XEPA_Q, ASAH);
  bool XEJU = xor(WEDU_Q, ACAM);
  bool ZATE = xor(YGAJ_Q, AZUB);
  bool ZAKU = xor(ZYJO_Q, AMEL);
  bool YBOX = xor(XURY_Q, AHAL);
  YKOK = nor(ZYKU, ZYPU, XAHA, ZEFE);
  YNAZ = nor(XEJU, ZATE, ZAKU, YBOX);

  // matcher 7
  bool EZUF_Q = EZUF.tock(CEXU, WUZO, COSE);
  bool ENAD_Q = ENAD.tock(CEXU, WUZO, AROP);
  bool EBOW_Q = EBOW.tock(CEXU, WUZO, XATU);
  bool FYCA_Q = FYCA.tock(CEXU, WUZO, BADY);
  bool GAVY_Q = GAVY.tock(CEXU, WUZO, ZAGO);
  bool GYPU_Q = GYPU.tock(CEXU, WUZO, ZOCY);
  bool GADY_Q = GADY.tock(CEXU, WUZO, YPUR);
  bool GAZA_Q = GAZA.tock(CEXU, WUZO, YVOK);

  bool DUZE = xor(EZUF_Q, APUX);
  bool DAGA = xor(ENAD_Q, ABEF);
  bool DAWU = xor(EBOW_Q, ADAZ);
  bool EJAW = xor(FYCA_Q, ASAH);
  bool GOHO = xor(GAVY_Q, ACAM);
  bool GASU = xor(GYPU_Q, AZUB);
  bool GABU = xor(GADY_Q, AMEL);
  bool GAFE = xor(GAZA_Q, AHAL);
  DAMA = nor(DUZE, DAGA, DAWU, EJAW);
  FEHA = nor(GOHO, GASU, GABU, GAFE);
  
  // matcher 8
  bool YPOD_Q = YPOD.tock(WEME, DOSY, COSE);
  bool YROP_Q = YROP.tock(WEME, DOSY, AROP);
  bool YNEP_Q = YNEP.tock(WEME, DOSY, XATU);
  bool YZOF_Q = YZOF.tock(WEME, DOSY, BADY);
  bool XUVY_Q = XUVY.tock(WEME, DOSY, ZAGO);
  bool XERE_Q = XERE.tock(WEME, DOSY, ZOCY);
  bool XUZO_Q = XUZO.tock(WEME, DOSY, YPUR);
  bool XEXA_Q = XEXA.tock(WEME, DOSY, YVOK);

  bool ZYWU = xor(YPOD_Q, APUX);
  bool ZUZA = xor(YROP_Q, ABEF);
  bool ZEJO = xor(YNEP_Q, ADAZ);
  bool ZEDA = xor(YZOF_Q, ASAH);
  bool YMAM = xor(XUVY_Q, ACAM);
  bool YTYP = xor(XERE_Q, AZUB);
  bool YFOP = xor(XUZO_Q, AMEL);
  bool YVAC = xor(XEXA_Q, AHAL);
  YTUB = nor(ZYWU, ZUZA, ZEJO, ZEDA);
  YLEV = nor(YMAM, YTYP, YFOP, YVAC);

  // matcher 9
  bool CYWE_Q = CYWE.tock(CYLA, EJAD, COSE);
  bool DYBY_Q = DYBY.tock(CYLA, EJAD, AROP);
  bool DURY_Q = DURY.tock(CYLA, EJAD, XATU);
  bool CUVY_Q = CUVY.tock(CYLA, EJAD, BADY);
  bool FUSA_Q = FUSA.tock(CYLA, EJAD, ZAGO);
  bool FAXA_Q = FAXA.tock(CYLA, EJAD, ZOCY);
  bool FOZY_Q = FOZY.tock(CYLA, EJAD, YPUR);
  bool FESY_Q = FESY.tock(CYLA, EJAD, YVOK);

  bool BAZY = xor(CYWE_Q, APUX);
  bool CYLE = xor(DYBY_Q, ABEF);
  bool CEVA = xor(DURY_Q, ADAZ);
  bool BUMY = xor(CUVY_Q, ASAH);
  bool GUZO = xor(FUSA_Q, ACAM);
  bool GOLA = xor(FAXA_Q, AZUB);
  bool GEVE = xor(FOZY_Q, AMEL);
  bool GUDE = xor(FESY_Q, AHAL);
  COGY = nor(BAZY, CYLE, CEVA, BUMY);
  FYMA = nor(GUZO, GOLA, GEVE, GUDE);

  // matcher 10
  bool DUHY_Q = DUHY.tock(CACU, GAMY, COSE);
  bool EJUF_Q = EJUF.tock(CACU, GAMY, AROP);
  bool ENOR_Q = ENOR.tock(CACU, GAMY, XATU);
  bool DEPY_Q = DEPY.tock(CACU, GAMY, BADY);
  bool FOKA_Q = FOKA.tock(CACU, GAMY, ZAGO);
  bool FYTY_Q = FYTY.tock(CACU, GAMY, ZOCY);
  bool FUBY_Q = FUBY.tock(CACU, GAMY, YPUR);
  bool GOXU_Q = GOXU.tock(CACU, GAMY, YVOK);

  bool CEKO = xor(DUHY_Q, APUX);
  bool DETY = xor(EJUF_Q, ABEF);
  bool DOZO = xor(ENOR_Q, ADAZ);
  bool CONY = xor(DEPY_Q, ASAH);
  bool FUZU = xor(FOKA_Q, ACAM);
  bool FESO = xor(FYTY_Q, AZUB);
  bool FOKY = xor(FUBY_Q, AMEL);
  bool FYVA = xor(GOXU_Q, AHAL);
  CEHU = nor(CEKO, DETY, DOZO, CONY);
  EKES = nor(FUZU, FESO, FOKY, FYVA);
}