#include "../Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P31_SpriteXMatchers {
  struct Input {
    bool CLK3;
    bool WEWU; // P28, drives OAM_A to data bus
    bool COTA;

    // P21 X counter
    bool ACAM,AZUB,AMEL,AHAL,APUX,ABEF,ADAZ,ASAH;

    // P29 sprite matcher clock lines
    bool FUXU,YFAG,GECY,DOKU,XAHO,WOFO,CEXU,WEME,CYLA,GAMY;

    // P29 sprite matcher reset lines
    bool DYNA,WUPA,GAFY,ASYS,ZAPE,WUNU,WUZO,DOSY,EJAD,CACU;

    bool OAM_A_D0,OAM_A_D1,OAM_A_D2,OAM_A_D3,OAM_A_D4,OAM_A_D5,OAM_A_D6,OAM_A_D7;
  };

  struct Output {
    // sprite match A
    bool XEBA,YWOS,DAJE,CYVY,YWAP,YKOK,DAMA,YTUB,COGY,CEHU;

    // sprite match B
    bool ZAKO,ZURE,CYCO,EWAM,YDOT,YNAZ,FEHA,YLEV,FYMA,EKES;

    // internal data bus
    bool D_OE;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  // oam_a_d latch
  reg WYNO, CYRA, ZUVE, ECED, XYKY, YRUM, YSEX, YVEL;

  // oam_a_d register
  reg GOMO,BAXO,YZOS,DEPO,YLOR,ZYTY,ZYVE,ZEZY;

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

  void tick(const Input& in, Output& out) {
    wire WYNO_Q = WYNO.latch(in.CLK3, in.OAM_A_D4);
    wire CYRA_Q = CYRA.latch(in.CLK3, in.OAM_A_D5);
    wire ZUVE_Q = ZUVE.latch(in.CLK3, in.OAM_A_D6);
    wire ECED_Q = ECED.latch(in.CLK3, in.OAM_A_D7);
    wire XYKY_Q = XYKY.latch(in.CLK3, in.OAM_A_D0);
    wire YRUM_Q = YRUM.latch(in.CLK3, in.OAM_A_D1);
    wire YSEX_Q = YSEX.latch(in.CLK3, in.OAM_A_D2);
    wire YVEL_Q = YVEL.latch(in.CLK3, in.OAM_A_D3);

    if (in.WEWU) {
      out.D_OE = true;
      out.D0 = XYKY_Q;
      out.D1 = YRUM_Q;
      out.D2 = YSEX_Q;
      out.D3 = YVEL_Q;
      out.D4 = WYNO_Q;
      out.D5 = CYRA_Q;
      out.D6 = ZUVE_Q;
      out.D7 = ECED_Q;
    }

    wire XEGA = not(in.COTA);
    wire GOMO_Q = GOMO.tock(XEGA, 0, WYNO_Q);
    wire BAXO_Q = BAXO.tock(XEGA, 0, CYRA_Q);
    wire YZOS_Q = YZOS.tock(XEGA, 0, ZUVE_Q);
    wire DEPO_Q = DEPO.tock(XEGA, 0, ECED_Q);
    wire YLOR_Q = YLOR.tock(XEGA, 0, XYKY_Q);
    wire ZYTY_Q = ZYTY.tock(XEGA, 0, YRUM_Q);
    wire ZYVE_Q = ZYVE.tock(XEGA, 0, YSEX_Q);
    wire ZEZY_Q = ZEZY.tock(XEGA, 0, YVEL_Q);

    wire COSE = not(!GOMO_Q);
    wire AROP = not(!BAXO_Q);
    wire XATU = not(!YZOS_Q);
    wire BADY = not(!DEPO_Q);
    wire ZAGO = not(!YLOR_Q);
    wire ZOCY = not(!ZYTY_Q);
    wire YPUR = not(!ZYVE_Q);
    wire YVOK = not(!ZEZY_Q);

    // CHECK CLK/RESET WIRES

    // matcher 1
    wire WELO_Q = WELO.tock(in.FUXU, in.DYNA, COSE);
    wire XUNY_Q = XUNY.tock(in.FUXU, in.DYNA, AROP);
    wire WOTE_Q = WOTE.tock(in.FUXU, in.DYNA, XATU);
    wire XAKO_Q = XAKO.tock(in.FUXU, in.DYNA, BADY);
    wire XEPE_Q = XEPE.tock(in.FUXU, in.DYNA, ZAGO);
    wire YLAH_Q = YLAH.tock(in.FUXU, in.DYNA, ZOCY);
    wire ZOLA_Q = ZOLA.tock(in.FUXU, in.DYNA, YPUR);
    wire ZULU_Q = ZULU.tock(in.FUXU, in.DYNA, YVOK);

    wire WOJU = xor(WELO_Q, in.APUX);
    wire YFUN = xor(XUNY_Q, in.ABEF);
    wire WYZA = xor(WOTE_Q, in.ADAZ);
    wire YPUK = xor(XAKO_Q, in.ASAH);
    wire ZOGY = xor(XEPE_Q, in.ACAM);
    wire ZEBA = xor(YLAH_Q, in.AZUB);
    wire ZAHA = xor(ZOLA_Q, in.AMEL);
    wire ZOKY = xor(ZULU_Q, in.AHAL);
    wire XEBA = nor(WOJU, YFUN, WYZA, YPUK);
    wire ZAKO = nor(ZOGY, ZEBA, ZAHA, ZOKY);

    // matcher 2
    wire XOMY_Q = XOMY.tock(in.YFAG, in.WUPA, COSE);
    wire WUHA_Q = WUHA.tock(in.YFAG, in.WUPA, AROP);
    wire WYNA_Q = WYNA.tock(in.YFAG, in.WUPA, XATU);
    wire WECO_Q = WECO.tock(in.YFAG, in.WUPA, BADY);
    wire XOLY_Q = XOLY.tock(in.YFAG, in.WUPA, ZAGO);
    wire XYBA_Q = XYBA.tock(in.YFAG, in.WUPA, ZOCY);
    wire XABE_Q = XABE.tock(in.YFAG, in.WUPA, YPUR);
    wire XEKA_Q = XEKA.tock(in.YFAG, in.WUPA, YVOK);

    wire YVAP = xor(XOMY_Q, in.APUX);
    wire XENY = xor(WUHA_Q, in.ABEF);
    wire XAVU = xor(WYNA_Q, in.ADAZ);
    wire XEVA = xor(WECO_Q, in.ASAH);
    wire YHOK = xor(XOLY_Q, in.ACAM);
    wire YCAH = xor(XYBA_Q, in.AZUB);
    wire YDAJ = xor(XABE_Q, in.AMEL);
    wire YVUZ = xor(XEKA_Q, in.AHAL);
    wire YWOS = nor(YVAP, XENY, XAVU, XEVA);
    wire ZURE = nor(YHOK, YCAH, YDAJ, YVUZ);

    // matcher 3
    wire FAZU_Q = FAZU.tock(in.GECY, in.GAFY, COSE);
    wire FAXE_Q = FAXE.tock(in.GECY, in.GAFY, AROP);
    wire EXUK_Q = EXUK.tock(in.GECY, in.GAFY, XATU);
    wire FEDE_Q = FEDE.tock(in.GECY, in.GAFY, BADY);
    wire ERAZ_Q = ERAZ.tock(in.GECY, in.GAFY, ZAGO);
    wire EPUM_Q = EPUM.tock(in.GECY, in.GAFY, ZOCY);
    wire EROL_Q = EROL.tock(in.GECY, in.GAFY, YPUR);
    wire EHYN_Q = EHYN.tock(in.GECY, in.GAFY, YVOK);

    wire EJOT = xor(FAZU_Q, in.APUX);
    wire ESAJ = xor(FAXE_Q, in.ABEF);
    wire DUCU = xor(EXUK_Q, in.ADAZ);
    wire EWUD = xor(FEDE_Q, in.ASAH);
    wire DUSE = xor(ERAZ_Q, in.ACAM);
    wire DAGU = xor(EPUM_Q, in.AZUB);
    wire DYZE = xor(EROL_Q, in.AMEL);
    wire DESO = xor(EHYN_Q, in.AHAL);
    wire DAJE = nor(EJOT, ESAJ, DUCU, EWUD);
    wire CYCO = nor(DUSE, DAGU, DYZE, DESO);

    // matcher 4
    wire DAKE_Q = DAKE.tock(in.ASYS, in.DOKU, COSE);
    wire CESO_Q = CESO.tock(in.ASYS, in.DOKU, AROP);
    wire DYFU_Q = DYFU.tock(in.ASYS, in.DOKU, XATU);
    wire CUSY_Q = CUSY.tock(in.ASYS, in.DOKU, BADY);
    wire DANY_Q = DANY.tock(in.ASYS, in.DOKU, ZAGO);
    wire DUKO_Q = DUKO.tock(in.ASYS, in.DOKU, ZOCY);
    wire DESU_Q = DESU.tock(in.ASYS, in.DOKU, YPUR);
    wire DAZO_Q = DAZO.tock(in.ASYS, in.DOKU, YVOK);

    wire COLA = xor(DAKE_Q, in.APUX);
    wire BOBA = xor(CESO_Q, in.ABEF);
    wire COLU = xor(DYFU_Q, in.ADAZ);
    wire BAHU = xor(CUSY_Q, in.ASAH);
    wire EDYM = xor(DANY_Q, in.ACAM);
    wire EMYB = xor(DUKO_Q, in.AZUB);
    wire EBEF = xor(DESU_Q, in.AMEL);
    wire EWOK = xor(DAZO_Q, in.AHAL);
    wire CYVY = nor(COLA, BOBA, COLU, BAHU);
    wire EWAM = nor(EDYM, EMYB, EBEF, EWOK);

    // matcher 5
    wire ZOLY_Q = ZOLY.tock(in.ZAPE, in.XAHO, COSE);
    wire ZOGO_Q = ZOGO.tock(in.ZAPE, in.XAHO, AROP);
    wire ZECU_Q = ZECU.tock(in.ZAPE, in.XAHO, XATU);
    wire ZESA_Q = ZESA.tock(in.ZAPE, in.XAHO, BADY);
    wire YCOL_Q = YCOL.tock(in.ZAPE, in.XAHO, ZAGO);
    wire YRAC_Q = YRAC.tock(in.ZAPE, in.XAHO, ZOCY);
    wire YMEM_Q = YMEM.tock(in.ZAPE, in.XAHO, YPUR);
    wire YVAG_Q = YVAG.tock(in.ZAPE, in.XAHO, YVOK);

    wire ZARE = xor(ZOLY_Q, in.APUX);
    wire ZEMU = xor(ZOGO_Q, in.ABEF);
    wire ZYGO = xor(ZECU_Q, in.ADAZ);
    wire ZUZY = xor(ZESA_Q, in.ASAH);
    wire XOSU = xor(YCOL_Q, in.ACAM);
    wire ZUVU = xor(YRAC_Q, in.AZUB);
    wire XUCO = xor(YMEM_Q, in.AMEL);
    wire ZULO = xor(YVAG_Q, in.AHAL);
    wire YWAP = nor(ZARE, ZEMU, ZYGO, ZUZY);
    wire YDOT = nor(XOSU, ZUVU, XUCO, ZULO);

    // matcher 6
    wire YBED_Q = YBED.tock(in.WUNU, in.WOFO, COSE);
    wire ZALA_Q = ZALA.tock(in.WUNU, in.WOFO, AROP);
    wire WYDE_Q = WYDE.tock(in.WUNU, in.WOFO, XATU);
    wire XEPA_Q = XEPA.tock(in.WUNU, in.WOFO, BADY);
    wire WEDU_Q = WEDU.tock(in.WUNU, in.WOFO, ZAGO);
    wire YGAJ_Q = YGAJ.tock(in.WUNU, in.WOFO, ZOCY);
    wire ZYJO_Q = ZYJO.tock(in.WUNU, in.WOFO, YPUR);
    wire XURY_Q = XURY.tock(in.WUNU, in.WOFO, YVOK);

    wire ZYKU = xor(YBED_Q, in.APUX);
    wire ZYPU = xor(ZALA_Q, in.ABEF);
    wire XAHA = xor(WYDE_Q, in.ADAZ);
    wire ZEFE = xor(XEPA_Q, in.ASAH);
    wire XEJU = xor(WEDU_Q, in.ACAM);
    wire ZATE = xor(YGAJ_Q, in.AZUB);
    wire ZAKU = xor(ZYJO_Q, in.AMEL);
    wire YBOX = xor(XURY_Q, in.AHAL);
    wire YKOK = nor(ZYKU, ZYPU, XAHA, ZEFE);
    wire YNAZ = nor(XEJU, ZATE, ZAKU, YBOX);

    // matcher 7
    wire EZUF_Q = EZUF.tock(in.CEXU, in.WUZO, COSE);
    wire ENAD_Q = ENAD.tock(in.CEXU, in.WUZO, AROP);
    wire EBOW_Q = EBOW.tock(in.CEXU, in.WUZO, XATU);
    wire FYCA_Q = FYCA.tock(in.CEXU, in.WUZO, BADY);
    wire GAVY_Q = GAVY.tock(in.CEXU, in.WUZO, ZAGO);
    wire GYPU_Q = GYPU.tock(in.CEXU, in.WUZO, ZOCY);
    wire GADY_Q = GADY.tock(in.CEXU, in.WUZO, YPUR);
    wire GAZA_Q = GAZA.tock(in.CEXU, in.WUZO, YVOK);

    wire DUZE = xor(EZUF_Q, in.APUX);
    wire DAGA = xor(ENAD_Q, in.ABEF);
    wire DAWU = xor(EBOW_Q, in.ADAZ);
    wire EJAW = xor(FYCA_Q, in.ASAH);
    wire GOHO = xor(GAVY_Q, in.ACAM);
    wire GASU = xor(GYPU_Q, in.AZUB);
    wire GABU = xor(GADY_Q, in.AMEL);
    wire GAFE = xor(GAZA_Q, in.AHAL);
    wire DAMA = nor(DUZE, DAGA, DAWU, EJAW);
    wire FEHA = nor(GOHO, GASU, GABU, GAFE);

    // matcher 8
    wire YPOD_Q = YPOD.tock(in.WEME, in.DOSY, COSE);
    wire YROP_Q = YROP.tock(in.WEME, in.DOSY, AROP);
    wire YNEP_Q = YNEP.tock(in.WEME, in.DOSY, XATU);
    wire YZOF_Q = YZOF.tock(in.WEME, in.DOSY, BADY);
    wire XUVY_Q = XUVY.tock(in.WEME, in.DOSY, ZAGO);
    wire XERE_Q = XERE.tock(in.WEME, in.DOSY, ZOCY);
    wire XUZO_Q = XUZO.tock(in.WEME, in.DOSY, YPUR);
    wire XEXA_Q = XEXA.tock(in.WEME, in.DOSY, YVOK);

    wire ZYWU = xor(YPOD_Q, in.APUX);
    wire ZUZA = xor(YROP_Q, in.ABEF);
    wire ZEJO = xor(YNEP_Q, in.ADAZ);
    wire ZEDA = xor(YZOF_Q, in.ASAH);
    wire YMAM = xor(XUVY_Q, in.ACAM);
    wire YTYP = xor(XERE_Q, in.AZUB);
    wire YFOP = xor(XUZO_Q, in.AMEL);
    wire YVAC = xor(XEXA_Q, in.AHAL);
    wire YTUB = nor(ZYWU, ZUZA, ZEJO, ZEDA);
    wire YLEV = nor(YMAM, YTYP, YFOP, YVAC);

    // matcher 9
    wire CYWE_Q = CYWE.tock(in.CYLA, in.EJAD, COSE);
    wire DYBY_Q = DYBY.tock(in.CYLA, in.EJAD, AROP);
    wire DURY_Q = DURY.tock(in.CYLA, in.EJAD, XATU);
    wire CUVY_Q = CUVY.tock(in.CYLA, in.EJAD, BADY);
    wire FUSA_Q = FUSA.tock(in.CYLA, in.EJAD, ZAGO);
    wire FAXA_Q = FAXA.tock(in.CYLA, in.EJAD, ZOCY);
    wire FOZY_Q = FOZY.tock(in.CYLA, in.EJAD, YPUR);
    wire FESY_Q = FESY.tock(in.CYLA, in.EJAD, YVOK);

    wire BAZY = xor(CYWE_Q, in.APUX);
    wire CYLE = xor(DYBY_Q, in.ABEF);
    wire CEVA = xor(DURY_Q, in.ADAZ);
    wire BUMY = xor(CUVY_Q, in.ASAH);
    wire GUZO = xor(FUSA_Q, in.ACAM);
    wire GOLA = xor(FAXA_Q, in.AZUB);
    wire GEVE = xor(FOZY_Q, in.AMEL);
    wire GUDE = xor(FESY_Q, in.AHAL);
    wire COGY = nor(BAZY, CYLE, CEVA, BUMY);
    wire FYMA = nor(GUZO, GOLA, GEVE, GUDE);

    // matcher 10
    wire DUHY_Q = DUHY.tock(in.CACU, in.GAMY, COSE);
    wire EJUF_Q = EJUF.tock(in.CACU, in.GAMY, AROP);
    wire ENOR_Q = ENOR.tock(in.CACU, in.GAMY, XATU);
    wire DEPY_Q = DEPY.tock(in.CACU, in.GAMY, BADY);
    wire FOKA_Q = FOKA.tock(in.CACU, in.GAMY, ZAGO);
    wire FYTY_Q = FYTY.tock(in.CACU, in.GAMY, ZOCY);
    wire FUBY_Q = FUBY.tock(in.CACU, in.GAMY, YPUR);
    wire GOXU_Q = GOXU.tock(in.CACU, in.GAMY, YVOK);

    wire CEKO = xor(DUHY_Q, in.APUX);
    wire DETY = xor(EJUF_Q, in.ABEF);
    wire DOZO = xor(ENOR_Q, in.ADAZ);
    wire CONY = xor(DEPY_Q, in.ASAH);
    wire FUZU = xor(FOKA_Q, in.ACAM);
    wire FESO = xor(FYTY_Q, in.AZUB);
    wire FOKY = xor(FUBY_Q, in.AMEL);
    wire FYVA = xor(GOXU_Q, in.AHAL);
    wire CEHU = nor(CEKO, DETY, DOZO, CONY);
    wire EKES = nor(FUZU, FESO, FOKY, FYVA);

    out.XEBA = XEBA;
    out.YWOS = YWOS;
    out.DAJE = DAJE;
    out.CYVY = CYVY;
    out.YWAP = YWAP;
    out.YKOK = YKOK;
    out.DAMA = DAMA;
    out.YTUB = YTUB;
    out.COGY = COGY;
    out.CEHU = CEHU;

    out.ZAKO = ZAKO;
    out.ZURE = ZURE;
    out.CYCO = CYCO;
    out.EWAM = EWAM;
    out.YDOT = YDOT;
    out.YNAZ = YNAZ;
    out.FEHA = FEHA;
    out.YLEV = YLEV;
    out.FYMA = FYMA;
    out.EKES = EKES;
  }
};