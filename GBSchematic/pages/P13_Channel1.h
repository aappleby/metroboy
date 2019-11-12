#pragma once

struct Gameboy;

struct P13_Channel1 {

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { bool DUNA; bool CH1_BIT; };
  union { bool CARA; bool CH1_ACTIVEn; };
  union { bool ACEG; bool CH1_OUT3; };
  union { bool AGOF; bool CH1_OUT2; };
  union { bool ASON; bool CH1_OUT1; };
  union { bool AMOP; bool CH1_OUT0; };
  union { bool HOCA; bool CH1_AMP_ENn; };
  union { bool GEXU; };
  union { bool FEKU; };
  union { bool COPE; };
  union { bool BUSO; };
  union { bool BOJE; };
  union { bool ADAD; };
  union { bool KYLY; };
  union { bool KALA; };
  union { bool EGOR; };

private:

#if 0
  //----------
  // inputs

  bool CATE;
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
#endif

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
  bool FARE,FYTE,DUPE,EZEC,KYNO;

  // Ch 1 initial volume
  bool HEVO,HOKO,HEMY,HAFO;

  // ???
  bool COMY;

  //----------
  // cells

  bool DAJO,DUVO,EZOZ,ENEK,CODO,COSO,CAVA,CEVU,CAXO;
  bool BORO,BEPE,BOKA,BUGY,CORY,CUSO,CAPY,CYFA,CANU,BONE,FEMY,BERY,GEPU;

  bool KAZA,KUXU,JONE,KADO,KERE,JOLA,KOTE,KURY,KUKU,KOMA,KORO;
  bool ERUM,EGET,GEFE,DOGE,DADO,DUKA,FYFO,KEKO,KABA,KEZU,KAKE,CYTO,COWE,BOTO;

  bool HESU,HETO,HYTO,JUFY,HUFU,HANO,HAKE,JADE,KORU;

  bool CYTE,DOKA,CALA,DYRU;
  bool DAFA,CYMU,BAVE,BURY,COZE;
  bool DACU,CYLU,BUGE,CELE;
  bool EPUK,EVOL,FEMU,EGYP,DODY,DAPU;
  bool COPY,ATAT,ATUV;
};

