#pragma once

struct Gameboy;

struct P13_Channel1 {

  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);

  //----------
  // inputs

  bool CATE;
  bool COPE;
  bool CEPO;
  bool ATYS;
  bool COPU_COUT;
  bool DOPU_OUT;

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

  bool FF12_D0;
  bool FF12_D0n;
  bool FF12_D1;
  bool FF12_D1n;
  bool FF12_D2;
  bool FF12_D2n;
  bool FF12_D3;
  bool FF12_D3n;
  bool FF12_D4;
  bool FF12_D5;
  bool FF12_D6;
  bool FF12_D7;

  bool FF14_D6;
  bool FF14_D6n;

  bool FF11;

  //----------
  // outputs

  bool CH1_ACTIVEn;
  bool CH1_BIT;
  bool CH1_OUT0;
  bool CH1_OUT1;
  bool CH1_OUT2;
  bool CH1_OUT3;

  //----------
  // regs

  // Channel 1 length timer
  bool BACY,CAVY,BOVY,CUNO;
  bool CURA,ERAM,CERO; // clock divider or something

                       // Sweep timer
  bool CAXY,CYPU,CUPO,BEXA;

  // Waveform counter
  bool ESUT,EROS,DAPE;

  // Sweep shift counter
  bool COPA,CAJA,BYRA,BYTE;

  // EG timer
  bool KALY,JOVA,KENU,KERA,KOZY,DUWO;
  bool FARE,FYTE,DUPE,EZEC,FEKU,KYNO;

  // Ch 1 initial volume
  bool HEVO,HOKO,HEMY,HAFO;

  // ???
  bool COMY;

  //----------
  // cells

  bool DAJO,DUVO,EZOZ,ENEK,CODO,COSO,CAVA,CEVU,CAXO,DUNA;
  bool BORO,BEPE,BOKA,BUGY,CORY,CUSO,CAPY,CYFA,CANU,BONE,HOCA,FEMY,BERY,GEPU,GEXU;

  bool KAZA,KUXU,JONE,KADO,KERE,JOLA,KOTE,KURY,KUKU,KOMA,KORO;
  bool ERUM,EGET,GEFE,DOGE,DADO,DUKA,FYFO,KEKO,KABA,KYLY,KEZU,KAKE,CYTO,CARA,COWE,BOTO;

  bool HESU,HETO,HYTO,JUFY,ACEG,AGOF,ASON,AMOP,HUFU,HANO,HAKE,JADE,KORU;

  bool CYTE,DOKA,CALA,DYRU;
  bool DAFA,CYMU,BAVE,BURY,COZE;
  bool DACU,CYLU,BUGE,CELE;
  bool ADAD,EPUK,EVOL,FEMU,EGYP,DODY,EGOR,DAPU;
  bool COPY,ATAT,ATUV,BOJE,BUSO,KALA;
};

