#include "../Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P12_Ch1Sweep {
  struct Input {
    bool APU_WR;
    bool FF14;
    bool FF13;
    bool APU_RESET;
    bool BUSO;
    bool ADAD;
    bool KYLY;
    bool BOJE;
    bool KALA;
    bool EGOR;
    bool P10_B;
    bool FF10_D3n;

    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  struct Output {
    bool ATYS;
    bool AXAN3,EVAB3,DYGY3,HOPO3,HYXU3,HOLU3,FELY3,EDUL3,HAVO3,JYKA3,HYKA3;
  };

  reg AXAN,EVAB,DYGY,HOPO,HYXU,HOLU,FELY,EDUL,HAVO,JYKA,HYKA;
  
  reg DOLY,DOFY,DEXE,DELE,EXAP,FAXO,GYME,JYME,KARE,JODE,GALO;
  
  // shift register
  reg BEKU,AGEZ,ELUX,EXAC,FEDO,FUDE,JOTA,JOLU,GOGA,JEFA,FABU;

  reg DEVA,ETER,DEFA,EDOK,EPYR,GELE,JETE,JAPE,HELE,HOPA,HORA;

  void tick(const Input& in, Output& out) {

    // BUG - APU_WR
    wire DEPU = nand(in.APU_WR, in.FF13);
    wire DEBY = and(in.APU_WR, in.FF14);
    wire DYLA = not(DEPU);

    wire BYFU = not(in.D2);
    wire BOFU = not(in.D1);
    wire BYSU = not(in.D0);
    wire DULO = not(in.D7);
    wire DYLU = not(in.D6);
    wire JULO = not(in.D5);
    wire KOPU = not(in.D4);
    wire ETUV = not(in.D3);
    wire FULE = not(in.D2);
    wire GULU = not(in.D1);
    wire DEKE = not(in.D0);

    wire AFEG = nand(in.D2, DEBY);
    wire BUDO = nand(in.D1, DEBY);
    wire BUGU = nand(in.D0, DEBY);
    wire ETOL = nand(in.D7, DYLA);
    wire ELER = nand(in.D6, DYLA);
    wire KYPA = nand(in.D5, DYLA);
    wire KOVU = nand(in.D4, DYLA);
    wire GOPE = nand(in.D3, DYLA);
    wire GOLO = nand(in.D2, DYLA);
    wire GETA = nand(in.D1, DYLA);
    wire GYLU = nand(in.D0, DYLA);

    wire AJUX = and(DEBY, BYFU);
    wire AMAC = and(DEBY, BOFU);
    wire BASO = and(DEBY, BYSU);
    wire EMAR = and(DYLA, DULO);
    wire ETOK = and(DYLA, DYLU);
    wire KYFU = and(DYLA, JULO);
    wire KAVO = and(DYLA, KOPU);
    wire FEGA = and(DYLA, ETUV);
    wire FOKE = and(DYLA, FULE);
    wire FOPU = and(DYLA, GULU);
    wire EJYF = and(DYLA, DEKE);

    wire APAJ = nor(AJUX, in.APU_RESET);
    wire BOVU = nor(AMAC, in.APU_RESET);
    wire BOXU = nor(BASO, in.APU_RESET);
    wire ESEL = nor(EMAR, in.APU_RESET);
    wire ELUF = nor(ETOK, in.APU_RESET);
    wire KAJU = nor(KYFU, in.APU_RESET);
    wire KAPO = nor(KAVO, in.APU_RESET);
    wire GAMO = nor(FEGA, in.APU_RESET);
    wire GYFU = nor(FOKE, in.APU_RESET);
    wire GATO = nor(FOPU, in.APU_RESET);
    wire EFOR = nor(EJYF, in.APU_RESET);

    wire DOLY_Q = DOLY.q();
    wire DOFY_Q = DOLY.q();
    wire DEXE_Q = DOLY.q();
    wire DELE_Q = DOLY.q();
    wire EXAP_Q = DOLY.q();
    wire FAXO_Q = DOLY.q();
    wire GYME_Q = DOLY.q();
    wire JYME_Q = DOLY.q();
    wire KARE_Q = DOLY.q();
    wire JODE_Q = DOLY.q();
    wire GALO_Q = DOLY.q();

    wire DEVA_Q = DEVA.q();
    wire ETER_Q = ETER.q();
    wire DEFA_Q = DEFA.q();
    wire EDOK_Q = EDOK.q();
    wire EPYR_Q = EPYR.q();
    wire GELE_Q = GELE.q();
    wire JETE_Q = JETE.q();
    wire JAPE_Q = JAPE.q();
    wire HELE_Q = HELE.q();
    wire HOPA_Q = HOPA.q();
    wire HORA_Q = HORA.q();

    wire ARYL = not(in.FF10_D3n);

    wire GUXA_C = add_c(!GALO_Q, !HORA_Q, ARYL);
    wire HALU_C = add_c(!JODE_Q, !HOPA_Q, GUXA_C);
    wire JULE_C = add_c(!KARE_Q, !HELE_Q, HALU_C);
    wire JORY_C = add_c(!JYME_Q, !JAPE_Q, JULE_C);
    wire HEXO_C = add_c(!GYME_Q, !JETE_Q, JORY_C);
    wire GEVA_C = add_c(!FAXO_Q, !GELE_Q, HEXO_C);
    wire FEGO_C = add_c(!EXAP_Q, !EPYR_Q, GEVA_C);
    wire ETEK_C = add_c(!DELE_Q, !EDOK_Q, FEGO_C);
    wire DYXE_C = add_c(!DEXE_Q, !DEFA_Q, ETEK_C);
    wire DULE_C = add_c(!DOFY_Q, !ETER_Q, DYXE_C);
    wire CORU_C = add_c(!DOLY_Q, !DEVA_Q, DULE_C);

    wire GUXA_S = add_s(!GALO_Q, !HORA_Q, ARYL);
    wire HALU_S = add_s(!JODE_Q, !HOPA_Q, GUXA_C);
    wire JULE_S = add_s(!KARE_Q, !HELE_Q, HALU_C);
    wire JORY_S = add_s(!JYME_Q, !JAPE_Q, JULE_C);
    wire HEXO_S = add_s(!GYME_Q, !JETE_Q, JORY_C);
    wire GEVA_S = add_s(!FAXO_Q, !GELE_Q, HEXO_C);
    wire FEGO_S = add_s(!EXAP_Q, !EPYR_Q, GEVA_C);
    wire ETEK_S = add_s(!DELE_Q, !EDOK_Q, FEGO_C);
    wire DYXE_S = add_s(!DEXE_Q, !DEFA_Q, ETEK_C);
    wire DULE_S = add_s(!DOFY_Q, !ETER_Q, DYXE_C);
    wire CORU_S = add_s(!DOLY_Q, !DEVA_Q, DULE_C);

    wire AXAN_Q = AXAN.srtock(in.BUSO, AFEG, APAJ, CORU_S);
    wire EVAB_Q = EVAB.srtock(in.BUSO, BUDO, BOVU, DULE_S);
    wire DYGY_Q = DYGY.srtock(in.BUSO, BUGU, BOXU, DYXE_S);
    wire HOPO_Q = HOPO.srtock(in.BOJE, ETOL, ESEL, ETEK_S);
    wire HYXU_Q = HYXU.srtock(in.BOJE, ELER, ELUF, FEGO_S);
    wire HOLU_Q = HOLU.srtock(in.BOJE, KYPA, KAJU, GEVA_S);
    wire FELY_Q = FELY.srtock(in.BOJE, KOVU, KAPO, HEXO_S);
    wire EDUL_Q = EDUL.srtock(in.BOJE, GOPE, GAMO, JORY_S);
    wire HAVO_Q = HAVO.srtock(in.BOJE, GOLO, GYFU, JULE_S);
    wire JYKA_Q = JYKA.srtock(in.BOJE, GETA, GATO, HALU_S);
    wire HYKA_Q = HYKA.srtock(in.BOJE, GYLU, EFOR, GUXA_S);

    out.AXAN3 = AXAN_Q;
    out.EVAB3 = EVAB_Q;
    out.DYGY3 = DYGY_Q;
    out.HOPO3 = HOPO_Q;
    out.HYXU3 = HYXU_Q;
    out.HOLU3 = HOLU_Q;
    out.FELY3 = FELY_Q;
    out.EDUL3 = EDUL_Q;
    out.HAVO3 = HAVO_Q;
    out.JYKA3 = JYKA_Q;
    out.HYKA3 = HYKA_Q;

    DOLY.tock(in.ADAD, in.KYLY, AXAN_Q);
    DOFY.tock(in.ADAD, in.KYLY, EVAB_Q);
    DEXE.tock(in.ADAD, in.KYLY, DYGY_Q);
    DELE.tock(in.ADAD, in.KYLY, HOPO_Q);
    EXAP.tock(in.ADAD, in.KYLY, HYXU_Q);
    FAXO.tock(in.ADAD, in.KYLY, HOLU_Q);
    GYME.tock(in.ADAD, in.KYLY, FELY_Q);
    JYME.tock(in.ADAD, in.KYLY, EDUL_Q);
    KARE.tock(in.ADAD, in.KYLY, HAVO_Q);
    JODE.tock(in.ADAD, in.KYLY, JYKA_Q);
    GALO.tock(in.ADAD, in.KYLY, HYKA_Q);

    wire BOJO = not(AXAN_Q);
    wire APAT = not(EVAB_Q);
    wire BYRU = not(DYGY_Q);
    wire CYKY = not(HOPO_Q);
    wire DEBO = not(HYXU_Q);
    wire FOHY = not(HOLU_Q);
    wire KOVO = not(FELY_Q);
    wire KEKE = not(EDUL_Q);
    wire HUNY = not(HAVO_Q);
    wire HOXE = not(JYKA_Q);
    wire JUTA = not(HYKA_Q);

    wire KEDO = not(in.KALA);
    wire JUJU = not(in.KALA);
    wire KAPE = not(in.KALA);

    wire AFYR = and(KEDO, BOJO);
    wire BUVO = and(KEDO, APAT);
    wire AFUG = and(KEDO, BYRU);
    wire BAPU = and(KEDO, CYKY);
    wire EREG = and(JUJU, DEBO);
    wire EVOF = and(JUJU, FOHY);
    wire KEVY = and(JUJU, KOVO);
    wire KAXY = and(JUJU, KEKE);
    wire JEHY = and(KAPE, HUNY);
    wire JOCY = and(KAPE, HOXE);
    wire KOKO = and(KAPE, JUTA);

    wire BEJU = nand(AXAN_Q, KEDO);
    wire BESO = nand(EVAB_Q, KEDO);
    wire BEGE = nand(DYGY_Q, KEDO);
    wire DACE = nand(HOPO_Q, KEDO);
    wire EKEM = nand(HYXU_Q, JUJU);
    wire GOVO = nand(HOLU_Q, JUJU);
    wire KOLA = nand(FELY_Q, JUJU);
    wire KYRY = nand(EDUL_Q, JUJU);
    wire HAWY = nand(HAVO_Q, KAPE);
    wire HOLA = nand(JYKA_Q, KAPE);
    wire HOZU = nand(HYKA_Q, KAPE);

    wire AVUF = nor(in.APU_RESET, AFYR);
    wire AFUX = nor(in.APU_RESET, BUVO);
    wire AGOR = nor(in.APU_RESET, AFUG);
    wire BEWO = nor(in.APU_RESET, BAPU);
    wire ENOK = nor(in.APU_RESET, EREG);
    wire EZUK = nor(in.APU_RESET, EVOF);
    wire KYBO = nor(in.APU_RESET, KEVY);
    wire KETO = nor(in.APU_RESET, KAXY);
    wire HYVU = nor(in.APU_RESET, JEHY);
    wire HOBU = nor(in.APU_RESET, JOCY);
    wire JADO = nor(in.APU_RESET, KOKO);

    wire FAJA = not(in.EGOR);
    wire EJYB = not(FAJA);
    wire CYBE = not(EJYB);
    wire BECY = not(CYBE);

    // shift register
    wire BEKU_Q = BEKU.srtock(BECY,    BEJU, AVUF, in.P10_B);
    wire AGEZ_Q = AGEZ.srtock(BECY,    BESO, AFUX, BEKU_Q);
    wire ELUX_Q = ELUX.srtock(BECY,    BEGE, AGOR, AGEZ_Q);
    wire EXAC_Q = EXAC.srtock(BECY,    DACE, BEWO, ELUX_Q);
    wire FEDO_Q = FEDO.srtock(EJYB,    EKEM, ENOK, EXAC_Q);
    wire FUDE_Q = FUDE.srtock(EJYB,    GOVO, EZUK, FEDO_Q);
    wire JOTA_Q = JOTA.srtock(EJYB,    KOLA, KYBO, FUDE_Q);
    wire JOLU_Q = JOLU.srtock(EJYB,    KYRY, KETO, JOTA_Q);
    wire GOGA_Q = GOGA.srtock(in.EGOR, HAWY, HYVU, JOLU_Q);
    wire JEFA_Q = JEFA.srtock(in.EGOR, HOLA, HOBU, GOGA_Q);
    wire FABU_Q = FABU.srtock(in.EGOR, HOZU, JADO, JEFA_Q);

    wire CULU = xor(ARYL, BEKU_Q);
    wire DOZY = xor(ARYL, AGEZ_Q);
    wire CALE = xor(ARYL, ELUX_Q);
    wire DYME = xor(ARYL, EXAC_Q);
    wire FURE = xor(ARYL, FEDO_Q);
    wire GOLY = xor(ARYL, FUDE_Q);
    wire KEFE = xor(ARYL, JOTA_Q);
    wire HEFY = xor(ARYL, JOLU_Q);
    wire GOPO = xor(ARYL, GOGA_Q);
    wire GELA = xor(ARYL, JEFA_Q);
    wire GYLO = xor(ARYL, FABU_Q);

    DEVA.tock(in.ADAD, in.KYLY, CULU);
    ETER.tock(in.ADAD, in.KYLY, DOZY);
    DEFA.tock(in.ADAD, in.KYLY, CALE);
    EDOK.tock(in.ADAD, in.KYLY, DYME);
    EPYR.tock(in.ADAD, in.KYLY, FURE);
    GELE.tock(in.ADAD, in.KYLY, GOLY);
    JETE.tock(in.ADAD, in.KYLY, KEFE);
    JAPE.tock(in.ADAD, in.KYLY, HEFY);
    HELE.tock(in.ADAD, in.KYLY, GOPO);
    HOPA.tock(in.ADAD, in.KYLY, GELA);
    HORA.tock(in.ADAD, in.KYLY, GYLO);

    wire BYLE = nor(ARYL, CORU_C);
    wire ATYS = or(BYLE,ARYL);
    out.ATYS = ATYS;
  }
};
