// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "OamBus.h"
#include "PPU.h"
#include "MemBus.h"
#include "Clocks.h"
#include "Sprites.h"
#include "VramBus.h"

//-----------------------------------------------------------------------------

void Sprites::tick_matcher() {
  bool WYNO_Q = WYNO.latch(clk.CLK3, oam.OAM_A_D4);
  bool CYRA_Q = CYRA.latch(clk.CLK3, oam.OAM_A_D5);
  bool ZUVE_Q = ZUVE.latch(clk.CLK3, oam.OAM_A_D6);
  bool ECED_Q = ECED.latch(clk.CLK3, oam.OAM_A_D7);
  bool XYKY_Q = XYKY.latch(clk.CLK3, oam.OAM_A_D0);
  bool YRUM_Q = YRUM.latch(clk.CLK3, oam.OAM_A_D1);
  bool YSEX_Q = YSEX.latch(clk.CLK3, oam.OAM_A_D2);
  bool YVEL_Q = YVEL.latch(clk.CLK3, oam.OAM_A_D3);

  if (spr.WEWU) {
    mem.D0 = XYKY_Q;
    mem.D1 = YRUM_Q;
    mem.D2 = YSEX_Q;
    mem.D3 = YVEL_Q;
    mem.D4 = WYNO_Q;
    mem.D5 = CYRA_Q;
    mem.D6 = ZUVE_Q;
    mem.D7 = ECED_Q;
  }

  bool XEGA = not(vram.COTA);
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
  bool WELO_Q = WELO.tock(spr.FUXU, spr.DYNA, COSE);
  bool XUNY_Q = XUNY.tock(spr.FUXU, spr.DYNA, AROP);
  bool WOTE_Q = WOTE.tock(spr.FUXU, spr.DYNA, XATU);
  bool XAKO_Q = XAKO.tock(spr.FUXU, spr.DYNA, BADY);
  bool XEPE_Q = XEPE.tock(spr.FUXU, spr.DYNA, ZAGO);
  bool YLAH_Q = YLAH.tock(spr.FUXU, spr.DYNA, ZOCY);
  bool ZOLA_Q = ZOLA.tock(spr.FUXU, spr.DYNA, YPUR);
  bool ZULU_Q = ZULU.tock(spr.FUXU, spr.DYNA, YVOK);

  bool WOJU = xor(WELO_Q, ppu.APUX);
  bool YFUN = xor(XUNY_Q, ppu.ABEF);
  bool WYZA = xor(WOTE_Q, ppu.ADAZ);
  bool YPUK = xor(XAKO_Q, ppu.ASAH);
  bool ZOGY = xor(XEPE_Q, ppu.ACAM);
  bool ZEBA = xor(YLAH_Q, ppu.AZUB);
  bool ZAHA = xor(ZOLA_Q, ppu.AMEL);
  bool ZOKY = xor(ZULU_Q, ppu.AHAL);
  XEBA = nor(WOJU, YFUN, WYZA, YPUK);
  ZAKO = nor(ZOGY, ZEBA, ZAHA, ZOKY);

  // matcher 2
  bool XOMY_Q = XOMY.tock(spr.YFAG, spr.WUPA, COSE);
  bool WUHA_Q = WUHA.tock(spr.YFAG, spr.WUPA, AROP);
  bool WYNA_Q = WYNA.tock(spr.YFAG, spr.WUPA, XATU);
  bool WECO_Q = WECO.tock(spr.YFAG, spr.WUPA, BADY);
  bool XOLY_Q = XOLY.tock(spr.YFAG, spr.WUPA, ZAGO);
  bool XYBA_Q = XYBA.tock(spr.YFAG, spr.WUPA, ZOCY);
  bool XABE_Q = XABE.tock(spr.YFAG, spr.WUPA, YPUR);
  bool XEKA_Q = XEKA.tock(spr.YFAG, spr.WUPA, YVOK);

  bool YVAP = xor(XOMY_Q, ppu.APUX);
  bool XENY = xor(WUHA_Q, ppu.ABEF);
  bool XAVU = xor(WYNA_Q, ppu.ADAZ);
  bool XEVA = xor(WECO_Q, ppu.ASAH);
  bool YHOK = xor(XOLY_Q, ppu.ACAM);
  bool YCAH = xor(XYBA_Q, ppu.AZUB);
  bool YDAJ = xor(XABE_Q, ppu.AMEL);
  bool YVUZ = xor(XEKA_Q, ppu.AHAL);
  YWOS = nor(YVAP, XENY, XAVU, XEVA);
  ZURE = nor(YHOK, YCAH, YDAJ, YVUZ);

  // matcher 3
  bool FAZU_Q = FAZU.tock(spr.GECY, spr.GAFY, COSE);
  bool FAXE_Q = FAXE.tock(spr.GECY, spr.GAFY, AROP);
  bool EXUK_Q = EXUK.tock(spr.GECY, spr.GAFY, XATU);
  bool FEDE_Q = FEDE.tock(spr.GECY, spr.GAFY, BADY);
  bool ERAZ_Q = ERAZ.tock(spr.GECY, spr.GAFY, ZAGO);
  bool EPUM_Q = EPUM.tock(spr.GECY, spr.GAFY, ZOCY);
  bool EROL_Q = EROL.tock(spr.GECY, spr.GAFY, YPUR);
  bool EHYN_Q = EHYN.tock(spr.GECY, spr.GAFY, YVOK);

  bool EJOT = xor(FAZU_Q, ppu.APUX);
  bool ESAJ = xor(FAXE_Q, ppu.ABEF);
  bool DUCU = xor(EXUK_Q, ppu.ADAZ);
  bool EWUD = xor(FEDE_Q, ppu.ASAH);
  bool DUSE = xor(ERAZ_Q, ppu.ACAM);
  bool DAGU = xor(EPUM_Q, ppu.AZUB);
  bool DYZE = xor(EROL_Q, ppu.AMEL);
  bool DESO = xor(EHYN_Q, ppu.AHAL);
  DAJE = nor(EJOT, ESAJ, DUCU, EWUD);
  CYCO = nor(DUSE, DAGU, DYZE, DESO);

  // matcher 4
  bool DAKE_Q = DAKE.tock(spr.ASYS, spr.DOKU, COSE);
  bool CESO_Q = CESO.tock(spr.ASYS, spr.DOKU, AROP);
  bool DYFU_Q = DYFU.tock(spr.ASYS, spr.DOKU, XATU);
  bool CUSY_Q = CUSY.tock(spr.ASYS, spr.DOKU, BADY);
  bool DANY_Q = DANY.tock(spr.ASYS, spr.DOKU, ZAGO);
  bool DUKO_Q = DUKO.tock(spr.ASYS, spr.DOKU, ZOCY);
  bool DESU_Q = DESU.tock(spr.ASYS, spr.DOKU, YPUR);
  bool DAZO_Q = DAZO.tock(spr.ASYS, spr.DOKU, YVOK);

  bool COLA = xor(DAKE_Q, ppu.APUX);
  bool BOBA = xor(CESO_Q, ppu.ABEF);
  bool COLU = xor(DYFU_Q, ppu.ADAZ);
  bool BAHU = xor(CUSY_Q, ppu.ASAH);
  bool EDYM = xor(DANY_Q, ppu.ACAM);
  bool EMYB = xor(DUKO_Q, ppu.AZUB);
  bool EBEF = xor(DESU_Q, ppu.AMEL);
  bool EWOK = xor(DAZO_Q, ppu.AHAL);
  CYVY = nor(COLA, BOBA, COLU, BAHU);
  EWAM = nor(EDYM, EMYB, EBEF, EWOK);

  // matcher 5
  bool ZOLY_Q = ZOLY.tock(spr.ZAPE, spr.XAHO, COSE);
  bool ZOGO_Q = ZOGO.tock(spr.ZAPE, spr.XAHO, AROP);
  bool ZECU_Q = ZECU.tock(spr.ZAPE, spr.XAHO, XATU);
  bool ZESA_Q = ZESA.tock(spr.ZAPE, spr.XAHO, BADY);
  bool YCOL_Q = YCOL.tock(spr.ZAPE, spr.XAHO, ZAGO);
  bool YRAC_Q = YRAC.tock(spr.ZAPE, spr.XAHO, ZOCY);
  bool YMEM_Q = YMEM.tock(spr.ZAPE, spr.XAHO, YPUR);
  bool YVAG_Q = YVAG.tock(spr.ZAPE, spr.XAHO, YVOK);

  bool ZARE = xor(ZOLY_Q, ppu.APUX);
  bool ZEMU = xor(ZOGO_Q, ppu.ABEF);
  bool ZYGO = xor(ZECU_Q, ppu.ADAZ);
  bool ZUZY = xor(ZESA_Q, ppu.ASAH);
  bool XOSU = xor(YCOL_Q, ppu.ACAM);
  bool ZUVU = xor(YRAC_Q, ppu.AZUB);
  bool XUCO = xor(YMEM_Q, ppu.AMEL);
  bool ZULO = xor(YVAG_Q, ppu.AHAL);
  YWAP = nor(ZARE, ZEMU, ZYGO, ZUZY);
  YDOT = nor(XOSU, ZUVU, XUCO, ZULO);

  // matcher 6
  bool YBED_Q = YBED.tock(spr.WUNU, spr.WOFO, COSE);
  bool ZALA_Q = ZALA.tock(spr.WUNU, spr.WOFO, AROP);
  bool WYDE_Q = WYDE.tock(spr.WUNU, spr.WOFO, XATU);
  bool XEPA_Q = XEPA.tock(spr.WUNU, spr.WOFO, BADY);
  bool WEDU_Q = WEDU.tock(spr.WUNU, spr.WOFO, ZAGO);
  bool YGAJ_Q = YGAJ.tock(spr.WUNU, spr.WOFO, ZOCY);
  bool ZYJO_Q = ZYJO.tock(spr.WUNU, spr.WOFO, YPUR);
  bool XURY_Q = XURY.tock(spr.WUNU, spr.WOFO, YVOK);

  bool ZYKU = xor(YBED_Q, ppu.APUX);
  bool ZYPU = xor(ZALA_Q, ppu.ABEF);
  bool XAHA = xor(WYDE_Q, ppu.ADAZ);
  bool ZEFE = xor(XEPA_Q, ppu.ASAH);
  bool XEJU = xor(WEDU_Q, ppu.ACAM);
  bool ZATE = xor(YGAJ_Q, ppu.AZUB);
  bool ZAKU = xor(ZYJO_Q, ppu.AMEL);
  bool YBOX = xor(XURY_Q, ppu.AHAL);
  YKOK = nor(ZYKU, ZYPU, XAHA, ZEFE);
  YNAZ = nor(XEJU, ZATE, ZAKU, YBOX);

  // matcher 7
  bool EZUF_Q = EZUF.tock(spr.CEXU, spr.WUZO, COSE);
  bool ENAD_Q = ENAD.tock(spr.CEXU, spr.WUZO, AROP);
  bool EBOW_Q = EBOW.tock(spr.CEXU, spr.WUZO, XATU);
  bool FYCA_Q = FYCA.tock(spr.CEXU, spr.WUZO, BADY);
  bool GAVY_Q = GAVY.tock(spr.CEXU, spr.WUZO, ZAGO);
  bool GYPU_Q = GYPU.tock(spr.CEXU, spr.WUZO, ZOCY);
  bool GADY_Q = GADY.tock(spr.CEXU, spr.WUZO, YPUR);
  bool GAZA_Q = GAZA.tock(spr.CEXU, spr.WUZO, YVOK);

  bool DUZE = xor(EZUF_Q, ppu.APUX);
  bool DAGA = xor(ENAD_Q, ppu.ABEF);
  bool DAWU = xor(EBOW_Q, ppu.ADAZ);
  bool EJAW = xor(FYCA_Q, ppu.ASAH);
  bool GOHO = xor(GAVY_Q, ppu.ACAM);
  bool GASU = xor(GYPU_Q, ppu.AZUB);
  bool GABU = xor(GADY_Q, ppu.AMEL);
  bool GAFE = xor(GAZA_Q, ppu.AHAL);
  DAMA = nor(DUZE, DAGA, DAWU, EJAW);
  FEHA = nor(GOHO, GASU, GABU, GAFE);
  
  // matcher 8
  bool YPOD_Q = YPOD.tock(spr.WEME, spr.DOSY, COSE);
  bool YROP_Q = YROP.tock(spr.WEME, spr.DOSY, AROP);
  bool YNEP_Q = YNEP.tock(spr.WEME, spr.DOSY, XATU);
  bool YZOF_Q = YZOF.tock(spr.WEME, spr.DOSY, BADY);
  bool XUVY_Q = XUVY.tock(spr.WEME, spr.DOSY, ZAGO);
  bool XERE_Q = XERE.tock(spr.WEME, spr.DOSY, ZOCY);
  bool XUZO_Q = XUZO.tock(spr.WEME, spr.DOSY, YPUR);
  bool XEXA_Q = XEXA.tock(spr.WEME, spr.DOSY, YVOK);

  bool ZYWU = xor(YPOD_Q, ppu.APUX);
  bool ZUZA = xor(YROP_Q, ppu.ABEF);
  bool ZEJO = xor(YNEP_Q, ppu.ADAZ);
  bool ZEDA = xor(YZOF_Q, ppu.ASAH);
  bool YMAM = xor(XUVY_Q, ppu.ACAM);
  bool YTYP = xor(XERE_Q, ppu.AZUB);
  bool YFOP = xor(XUZO_Q, ppu.AMEL);
  bool YVAC = xor(XEXA_Q, ppu.AHAL);
  YTUB = nor(ZYWU, ZUZA, ZEJO, ZEDA);
  YLEV = nor(YMAM, YTYP, YFOP, YVAC);

  // matcher 9
  bool CYWE_Q = CYWE.tock(spr.CYLA, spr.EJAD, COSE);
  bool DYBY_Q = DYBY.tock(spr.CYLA, spr.EJAD, AROP);
  bool DURY_Q = DURY.tock(spr.CYLA, spr.EJAD, XATU);
  bool CUVY_Q = CUVY.tock(spr.CYLA, spr.EJAD, BADY);
  bool FUSA_Q = FUSA.tock(spr.CYLA, spr.EJAD, ZAGO);
  bool FAXA_Q = FAXA.tock(spr.CYLA, spr.EJAD, ZOCY);
  bool FOZY_Q = FOZY.tock(spr.CYLA, spr.EJAD, YPUR);
  bool FESY_Q = FESY.tock(spr.CYLA, spr.EJAD, YVOK);

  bool BAZY = xor(CYWE_Q, ppu.APUX);
  bool CYLE = xor(DYBY_Q, ppu.ABEF);
  bool CEVA = xor(DURY_Q, ppu.ADAZ);
  bool BUMY = xor(CUVY_Q, ppu.ASAH);
  bool GUZO = xor(FUSA_Q, ppu.ACAM);
  bool GOLA = xor(FAXA_Q, ppu.AZUB);
  bool GEVE = xor(FOZY_Q, ppu.AMEL);
  bool GUDE = xor(FESY_Q, ppu.AHAL);
  COGY = nor(BAZY, CYLE, CEVA, BUMY);
  FYMA = nor(GUZO, GOLA, GEVE, GUDE);

  // matcher 10
  bool DUHY_Q = DUHY.tock(spr.CACU, spr.GAMY, COSE);
  bool EJUF_Q = EJUF.tock(spr.CACU, spr.GAMY, AROP);
  bool ENOR_Q = ENOR.tock(spr.CACU, spr.GAMY, XATU);
  bool DEPY_Q = DEPY.tock(spr.CACU, spr.GAMY, BADY);
  bool FOKA_Q = FOKA.tock(spr.CACU, spr.GAMY, ZAGO);
  bool FYTY_Q = FYTY.tock(spr.CACU, spr.GAMY, ZOCY);
  bool FUBY_Q = FUBY.tock(spr.CACU, spr.GAMY, YPUR);
  bool GOXU_Q = GOXU.tock(spr.CACU, spr.GAMY, YVOK);

  bool CEKO = xor(DUHY_Q, ppu.APUX);
  bool DETY = xor(EJUF_Q, ppu.ABEF);
  bool DOZO = xor(ENOR_Q, ppu.ADAZ);
  bool CONY = xor(DEPY_Q, ppu.ASAH);
  bool FUZU = xor(FOKA_Q, ppu.ACAM);
  bool FESO = xor(FYTY_Q, ppu.AZUB);
  bool FOKY = xor(FUBY_Q, ppu.AMEL);
  bool FYVA = xor(GOXU_Q, ppu.AHAL);
  CEHU = nor(CEKO, DETY, DOZO, CONY);
  EKES = nor(FUZU, FESO, FOKY, FYVA);
}