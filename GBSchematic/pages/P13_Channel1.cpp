#include "../Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P13_Channel1 {
  struct Input {
    bool HORU_512HZ;
    bool ABOL_1MHZ;
    bool PHIn;
    bool CEPO;
    bool COPE;
    bool AJER_2MHZ;
    bool APU_WR;
    bool FF11;
    bool APU_RESET;
    bool BUFY_256HZ;
    bool ATYS;
    bool COPU_COUT;
    bool DYFA_1MHZ;
    bool BEXA;
    bool CATE;
    bool DOPU_OUT;
    bool BYFE_128HZ;
    bool NET03;

    bool FF10_D0n;
    bool FF10_D1n;
    bool FF10_D2n;

    bool FF10_D4n;
    bool FF10_D5n;
    bool FF10_D6n;

    bool FF11_D6;
    bool FF11_D6n;
    bool FF11_D7;
    bool FF11_D7n;

    bool FF12_D0n;
    bool FF12_D1n;
    bool FF12_D2n;
    bool FF12_D3n;

    bool FF12_D0,FF12_D1,FF12_D2,FF12_D3,FF12_D4,FF12_D5,FF12_D6,FF12_D7;

    bool FF14_D6;
    bool FF14_D6n;

    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  struct Output {
    bool GEXU;
    bool BERY;
    bool COPE;
    bool BEXA;
    bool ADAD;
    bool EGOR;
    bool BOJE;
    bool BUSO;
    bool KALA;
    bool KYLY;

    bool CH1_BIT;
    bool CH1_ACTIVEn;

    bool CH1_OUT0,CH1_OUT1,CH1_OUT2,CH1_OUT3;
  };

  // Channel 1 length timer
  reg BACY,CAVY,BOVY,CUNO;
  reg CURA,ERAM,CERO; // clock divider or something

  // Sweep timer
  reg CAXY,CYPU,CUPO,BEXA;

  // Waveform counter
  reg ESUT,EROS,DAPE;

  // Sweep shift counter
  reg COPA,CAJA,BYRA,BYTE;

  // EG timer
  reg KALY,JOVA,KENU,KERA,KOZY,DUWO;
  reg FARE,FYTE,DUPE,EZEC,FEKU,KYNO;

  // Ch 1 initial volume
  reg HEVO,HOKO,HEMY,HAFO;


  reg COMY;

  void tick(const Input& in, Output& out) {
    bool FEKU_Q = FEKU.q();
    bool KOZY_Q = KOZY.q();
    bool FYTE_Q = FYTE.q();

    //----------
    // Waveform counter

    bool DAJO = not(in.COPE);

    bool ESUT_Q = ESUT.flip(DAJO,      in.CEPO);
    bool EROS_Q = EROS.flip(!ESUT.q(), in.CEPO);
    bool DAPE_Q = DAPE.flip(!EROS.q(), in.CEPO);

    bool DUVO = not(ESUT_Q);
    bool EZOZ = and(EROS_Q, DAPE_Q);
    bool ENEK = and(DUVO, EZOZ);
    bool CODO = not(EZOZ);
    bool COSO = nor(in.FF11_D6,  in.FF11_D7);
    bool CAVA = nor(in.FF11_D6n, in.FF11_D7);
    bool CEVU = nor(in.FF11_D6,  in.FF11_D7n);
    bool CAXO = nor(in.FF11_D6n, in.FF11_D7n);

    bool DUNA = amux4(ENEK,COSO, EZOZ,CAVA, DAPE_Q,CEVU, CODO, CAXO);

    out.CH1_BIT= DUNA;

    //----------
    // Length timer

    bool BORO = nand(in.APU_WR, in.FF11);
    bool BEPE = not(BORO);
    bool BOKA = not(BORO);
    bool BUGY = not(BORO);
    bool CORY = nor(FEKU_Q, in.APU_RESET, BOKA);

    bool CUNO_Q = CUNO.q();
    bool CUSO = not(!CUNO_Q);

    // FIXME daisy chain
    CURA.count(CUSO, BEPE, in.D4);
    ERAM.count(CURA.q(), BEPE, in.D5);
    bool CERO_Q = CERO.flip(!ERAM.q(), CORY);

    bool CAPY = nor(in.FF14_D6n, in.BUFY_256HZ, CERO_Q);
    bool CYFA = and(CERO_Q, in.FF14_D6);
    bool CANU = not(CAPY);

    BACY.count(CANU,     BUGY, in.D0);
    CAVY.count(BACY.q(), BUGY, in.D1);
    BOVY.count(CAVY.q(), BUGY, in.D2);
    CUNO.count(BOVY.q(), BUGY, in.D3);

    bool BONE = not(in.ATYS);
    bool HOCA = nor(in.FF12_D3, in.FF12_D4, in.FF12_D5, in.FF12_D6, in.FF12_D7);
    bool FEMY = nor(HOCA, in.APU_RESET);
    bool BERY = or(BONE, in.APU_RESET, CYFA, HOCA);
    bool GEPU = not(FYTE_Q);
    bool GEXU = unk2(FEMY, GEPU);

    out.GEXU = GEXU;
    out.BERY = BERY;

    //----------
    // EG timer

    bool KAZA = nor(FEKU_Q, KOZY_Q);
    bool KUXU = not(KAZA);

    bool JONE = not(in.BYFE_128HZ);
    bool KADO = not(in.APU_RESET);
    bool KALY_Q = KALY.flip(JONE, KADO);
    bool KERE = not(KALY_Q);
    bool JOLA = not(KERE);

    // FIXME daisy chain
    bool JOVA_Q = JOVA.count(JOLA,     KUXU, in.FF12_D0n);
    bool KENU_Q = KENU.count(JOVA.q(), KUXU, in.FF12_D1n);
    bool KERA_Q = KERA.count(KENU.q(), KUXU, in.FF12_D2n);

    bool KOTE = and(JOVA_Q, KENU_Q, KERA_Q);
    bool KURY = not(KOZY_Q);
    bool KUKU = nor(in.ABOL_1MHZ, KURY);
    bool KOMA = nor(in.FF12_D0, in.FF12_D1, in.FF12_D2);
    bool KORO = nor(KUKU, KOMA);
    KOZY.tock(in.HORU_512HZ, KORO, KOTE);


    bool ERUM = not(in.APU_RESET);
    bool FARE_Q = FARE.tock(in.DYFA_1MHZ, ERUM, FEKU_Q);
    FYTE.tock(in.DYFA_1MHZ, ERUM, FARE_Q);
    bool EGET = nor(in.APU_RESET, FARE_Q);
    bool GEFE = not(EGET);


    bool EZEC_Q = EZEC.q();
    // BUG - APU_WR
    bool DOGE = nand(in.APU_WR, in.FF14_D6);
    bool DADO = nor(in.APU_RESET, EZEC_Q);
    bool DUPE_Q = DUPE.tock(DOGE, DADO, in.D7);
    bool DUKA = not(in.APU_RESET);
    EZEC.tock(in.PHIn, DUKA, DUPE_Q);

    bool FYFO = unk2(GEFE, EZEC_Q);
    FEKU.tock(in.DYFA_1MHZ, EGET, FYFO);
    bool KEKO = or(in.APU_RESET, FEKU_Q);
    bool KABA = or(in.APU_RESET, FEKU_Q);
    bool KYLY = not(KABA);
    out.KYLY = KYLY;

    bool KYNO_Q = KYNO.q();
    bool KEZU = unk2(KYNO_Q, KEKO);
    bool KAKE = and(KOZY_Q, KOMA, KEZU);

    bool CYTO = unk2(FEKU_Q, BERY);
    bool DUWO_Q = DUWO.tock(in.COPE, in.CEPO, out.CH1_BIT);
    bool CARA = not(CYTO);
    out.CH1_ACTIVEn = CARA;
    bool COWE = and(CYTO,DUWO_Q);
    bool BOTO = or(COWE, in.NET03);

    bool HAFO_Q = HAFO.q();
    bool HEMY_Q = HEMY.q();
    bool HOKO_Q = HOKO.q();
    bool HEVO_Q = HEVO.q();

    // weird things are going on with the reg clocks and muxes... probably broken

    bool HESU = amux2(in.FF12_D3, HOKO_Q, !HOKO_Q, in.FF12_D3n);
    bool HETO = amux2(in.FF12_D3, HEMY_Q, !HEMY_Q, in.FF12_D3n);
    bool HYTO = amux2(in.FF12_D3, HAFO_Q, !HAFO_Q, in.FF12_D3n);
    bool JUFY = amux2(in.FF12_D3, KAKE, KAKE, in.FF12_D3n);

    HEVO.count(HESU, FEKU_Q, in.FF12_D7);
    HOKO.count(HETO, FEKU_Q, in.FF12_D6);
    HEMY.count(HYTO, FEKU_Q, in.FF12_D5);
    HAFO.count(JUFY, FEKU_Q, in.FF12_D4);

    bool ACEG = and(HEVO_Q, BOTO);
    bool AGOF = and(HOKO_Q, BOTO);
    bool ASON = and(HEMY_Q, BOTO);
    bool AMOP = and(HAFO_Q, BOTO);

    out.CH1_OUT3 = ACEG;
    out.CH1_OUT2 = AGOF;
    out.CH1_OUT1 = ASON;
    out.CH1_OUT0 = AMOP;

    bool HUFU = nand(in.FF12_D3, HAFO_Q, HEMY_Q, HOKO_Q, HEVO_Q);
    bool HANO = nor(in.FF12_D3, HAFO_Q, HEMY_Q, HOKO_Q, HEVO_Q);
    bool HAKE = not(HUFU);
    bool JADE = or(HAKE, HANO);
    bool KORU = nor(FEKU_Q, in.APU_RESET);

    KYNO.tock(KOZY_Q, KORU, JADE);

    //----------
    // little thing left side

    bool COMY_Q = COMY.q();
    bool CYTE = not(COMY_Q);
    bool COPE = not(CYTE);
    bool DOKA = and(COMY_Q, in.DYFA_1MHZ);
    bool CALA = not(in.COPU_COUT);
    bool DYRU = nor(in.APU_RESET, FEKU_Q, DOKA);
    COMY.flip(CALA, DYRU);

    out.COPE = COPE;

    //----------
    // Sweep timer

    bool DAFA = nor(in.BEXA, FEKU_Q);
    bool CYMU = not(DAFA);
    bool BAVE = and(in.FF10_D4n, in.FF10_D5n, in.FF10_D6n);

    // FIXME daisy chain
    bool CUPO_Q = CUPO.count(in.CATE,  CYMU, in.FF10_D4n);
    bool CYPU_Q = CYPU.count(CUPO.q(), CYMU, in.FF10_D5n);
    bool CAXY_Q = CAXY.count(CYPU.q(), CYMU, in.FF10_D6n);

    bool BURY = nor(BAVE, in.APU_RESET);
    bool COZE = and(CAXY_Q, CYPU_Q, CUPO_Q);

    bool BEXA_Q = BEXA.tock(in.AJER_2MHZ, BURY, COZE);
    out.BEXA = BEXA_Q;

    //----------
    // Sweep shift counter

    bool DACU = nor(FEKU_Q, in.BEXA);
    bool CYLU = not(DACU);
    bool BUGE = nand(in.FF10_D2n, in.FF10_D1n, in.FF10_D0n);
    bool CELE = not(BUGE);

    bool BYTE_Q = BYTE.q();
    bool ADAD = not(!BYTE_Q);
    bool EPUK = nor(ADAD, in.APU_RESET);
    bool EVOL = nor(in.BEXA, FYTE_Q);
    bool FEMU = unk2(EPUK, EVOL);
    bool EGYP = nor(in.DYFA_1MHZ, FEMU);
    bool DODY = nor(EGYP, CELE);
    bool EGOR = and(in.DOPU_OUT, DODY);
    bool DAPU = not(EGOR);

    bool COPA_Q = COPA.count(DAPU,     CYLU, in.FF10_D0n);
    bool CAJA_Q = CAJA.count(COPA.q(), CYLU, in.FF10_D1n);
    bool BYRA_Q = BYRA.count(CAJA.q(), CYLU, in.FF10_D2n);

    bool COPY = and(COPA_Q, CAJA_Q, BYRA_Q);
    bool ATAT = nor(in.APU_RESET, in.BEXA);

    BYTE.tock(in.AJER_2MHZ, ATAT, COPY);

    bool ATUV = and(in.BEXA, in.ATYS);
    bool BOJE = and(ATUV, BUGE);
    bool BUSO = or(BUGE, in.ATYS, in.BEXA);
    bool KALA = nor(in.BEXA, FEKU_Q);

    out.ADAD = ADAD;
    out.EGOR = EGOR;
    out.BOJE = BOJE;
    out.BUSO = BUSO;
    out.KALA = KALA;
  }
};