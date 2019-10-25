#include "Timer.h"

Timer::Output Timer::tock(const Timer::Input& in, TextPainter& tp) {

#if 1
  //----------
  // register outputs

  wire TAMA_Q = TAMA.q();
  wire UNYK_Q = UNYK.q();
  wire TERO_Q = TERO.q();
  wire UNER_Q = UNER.q();
  wire UFOR_Q = UFOR.q();
  wire UKUP_Q = UKUP.q();
  wire UGOT_Q = UGOT.q();
  wire TULU_Q = TULU.q();
  wire TUGO_Q = TUGO.q();
  wire TOFE_Q = TOFE.q();
  wire TERU_Q = TERU.q();
  wire SOLA_Q = SOLA.q();
  wire SUBU_Q = SUBU.q();
  wire TEKA_Q = TEKA.q();
  wire UKET_Q = UKET.q();
  wire UPOF_Q = UPOF.q();

  wire ADYK_Q = ADYK.q();
  wire AFUR_Q = AFUR.q();
  wire ALEF_Q = ALEF.q();
  wire APUK_Q = APUK.q();

  wire AFER_Q = AFER.q();

  wire BARA_Q = BARA.q();
  wire CARU_Q = CARU.q();
  wire BYLU_Q = BYLU.q();

  wire ATYK_Q = ATYK.q();
  wire AVOK_Q = AVOK.q();
  wire JESO_Q = JESO.q();

  //----------
  // bottom left

  wire UCOB = not(in.CLKIN_A);
  wire CLKIN_An = UCOB;

  wire ATAL = not(in.CLKIN_B);
  wire ATAL_4MHZ = ATAL;
  wire AZOF = not(ATAL);

  wire ATAG = not(AZOF);
  wire ZAXY = not(AZOF);

  wire AMUK = not(ATAG);
  wire ZEME = not(ZAXY);

  wire APUV = not(AMUK);
  wire ARYF = not(AMUK);
  wire ALET = not(ZEME);

  wire APUV_4MHZ = APUV;
  wire ARYF_4MHZ = ARYF;
  wire AMUK_4MHZ = AMUK;
  wire CLK1 = ZEME;
  wire CLK2 = ALET;

  //----------
  // bottom right

  wire UPYF = or(in.RESET, CLKIN_An);
  wire TUBO = unk2(in.ABOL_1MHZ, UPYF);
  wire UNUT = and(TUBO, UPOF_Q);
  wire TABA = or(in.T1nT2, in.T1T2n, UNUT);

  //----------
  // top left

  wire ADAR = not(ADYK_Q);
  wire ATYP = not(AFUR_Q);
  wire AFEP = not(ALEF_Q);
  wire AROV = not(APUK_Q);

  wire AFAS = nor(ADAR, ATYP);
  wire AJAX = not(ATYP);
  wire BUGO = not(AFEP);

  wire AREV = nand(in.FROM_CPU3, AFAS);
  wire AGUT = unk3(AJAX, AROV, in.FROM_CPU4);
  wire BATE = nor(BUGO, AROV, in.ABOL_1MHZ);

  wire APOV = not(AREV);
  wire CPU_RD_SYNC = APOV;
  wire AWOD = or(in.T1nT2, AGUT);
  wire BASU = not(BATE);

  wire ABUZ = not(AWOD);
  wire BUKE = not(BASU);

  wire BAPY = nor(in.ABOL_1MHZ, AROV, ATYP);
  wire NULE = nor(ATYP, in.ABOL_1MHZ);

  wire BERU = not(BAPY);
  wire BYRY = not(NULE);

  wire BUFA = not(BERU);
  wire BYLY = not(BERU);
  wire BUDE = not(BYRY);
  wire BEVA = not(BYRY);
  wire PHI_OUT = BEVA;
  wire DOVA = not(BEVA);
  wire PHIn = DOVA;

  wire BOLO = not(BUFA);
  wire BYDA = not(BYLY);
  wire BEKO = not(BUDE);
  wire BAVY = not(BEVA);

  wire BEJA = nand(BOLO, BYDA, BEKO, BAVY);
  wire BANE = not(BEJA);
  wire BELO = not(BANE);
  wire BAZE = not(BELO);
  wire BUTO = nand(AFEP, ATYP, BAZE);

  wire BELE = not(BUTO);
  wire ATEZ = not(in.CLKIN_A);

  wire BYJU = nor(BELE, ATEZ);
  wire ALYP = not(TABA);

  wire BUTY = not(in.ABOL_1MHZ);
  wire BALY = not(BYJU);
  wire AFAR = nor(ALYP, in.RESET);

  wire BUVU = and(BUTY, BALY);
  wire BYXO = not(BUVU);
  wire BEDO = not(BYXO);
  wire BOWA = not(BEDO);
  wire TO_CPU = BOWA;

  wire BOGA = not(BALY);
  wire BOGA1MHZ = BOGA;
  wire BOMA = not(BOGA);

  wire ASOL = unk2(AFAR, in.RESET);
  wire AVOR = or(AFER_Q, ASOL);
  wire ALUR = not(AVOR);
  wire RESET2 = ALUR;

  //----------

  wire TAPE = and(in.FF04_FF07, in.CPU_WR, in.TOLA_A1n, in.TOVY_A0n);
  wire UFOL = nor(CLKIN_An, in.RESET, TAPE);
  wire RESET_DIVn = UFOL;

  wire UVYN = not(TAMA_Q);
  wire CLK_16384 = UVYN;
  wire TAMA16384 = !TAMA_Q;
  wire CLK_65536 = TERO_Q;
  wire CLK_262144 = UFOR_Q;

  wire ULUR = mux2(BOGA1MHZ, TAMA16384, in.FF60_D1);

  wire UMEK = not(UGOT_Q);
  wire FF04_D0n = UMEK;
  wire UREK = not(TULU_Q);
  wire FF04_D1n = UREK;
  wire UTOK = not(TUGO_Q);
  wire SAPY = not(TOFE_Q);
  wire UMER = not(TERU_Q);
  wire RAVE = not(SOLA_Q);
  wire RYSO = not(SUBU_Q);
  wire UDOR = not(TEKA_Q);

  wire TAGY = and(in.FF04_FF07, in.CPU_RD, in.TOLA_A1n, in.TOVY_A0n);

  wire TAWU = not(UMEK);
  wire TAKU = not(UREK);
  wire TEMU = not(UTOK);
  wire TUSE = not(SAPY);
  wire UPUG = not(UMER);
  wire SEPU = not(RAVE);
  wire SAWA = not(RYSO);
  wire TATU = not(UDOR);

  // so this is not quite right...
  wire D_OE = TAGY;
  wire D0 = TAGY ? TAWU : 0;
  wire D1 = TAGY ? TAKU : 0;
  wire D2 = TAGY ? TEMU : 0;
  wire D3 = TAGY ? TUSE : 0;
  wire D4 = TAGY ? UPUG : 0;
  wire D5 = TAGY ? SEPU : 0;
  wire D6 = TAGY ? SAWA : 0;
  wire D7 = TAGY ? TATU : 0;

  //----------

  wire DULA = not(RESET2);
  wire CUNU = not(DULA);
  wire RESET6 = CUNU;
  wire XORE = not(CUNU);
  wire RESET7 = XORE;
  wire WESY = not(XORE);
  wire RESET9 = WESY;
  wire WALU = not(XORE);
  wire RESET8 = WALU;
  wire XEBE = not(RESET7);
  wire RESET7n = XEBE;
  wire XODO = and(in.FF40_D7, XEBE);
  wire XAPO = not(XODO);
  wire RESET_VIDEO = XAPO;

  //----------

  wire BOPO = not(in.APU_RESET);
  wire BAVU = not(AVOK_Q);
  wire BAVU_1MHZ = BAVU;
  wire HAMA = not(!JESO_Q);
  wire JESO_512K = JESO_Q;
  wire HAMA_512Kn = HAMA;

  //----------

  wire ATUS = not(in.APU_RESET);
  wire COKE = not(in.AJER_2MHZ);

  wire BURE = not(!BARA_Q);
  wire FYNE = not(BURE);
  wire CULO = not(!CARU_Q);
  wire APEF = not(!BYLU_Q);

  wire GALE = mux2(HAMA_512Kn, FYNE, in.FERO_Q);
  wire BEZE = mux2(HAMA_512Kn, CULO, in.FERO_Q);
  wire BULE = mux2(HAMA_512Kn, APEF, in.FERO_Q);

  wire GEXY = not(GALE);
  wire COFU = not(BEZE);
  wire BARU = not(BULE);

  wire HORU = not(GEXY);
  wire BUFY = not(COFU);
  wire BYFE = not(BARU);

  wire HORU_512Hz = HORU;
  wire BUFY_256Hz = BUFY;
  wire BYFE_128Hz = BYFE;

  //----------

  ATYK.tock(ARYF_4MHZ, BOPO, !ATYK_Q);
  AVOK.tock(ATYK_Q, BOPO, !AVOK_Q);
  JESO.tock(BAVU, in.APU_RESET5n, !JESO_Q);

  BARA.tock(COKE, ATUS, UMER);
  CARU.tock(BURE, ATUS, !CARU_Q);
  BYLU.tock(!CARU_Q, ATUS, !BYLU_Q);

  // NOTE - these FFs are daisy-chained, which means that the clock for each
  // link in the chain is the _new_ value of Q for the previous link.
  UKUP.flip(BOGA1MHZ,  RESET_DIVn);
  UFOR.flip(!UKUP.q(), RESET_DIVn);
  UNER.flip(!UFOR.q(), RESET_DIVn);
  TERO.flip(!UNER.q(), RESET_DIVn);
  UNYK.flip(!TERO.q(), RESET_DIVn);
  TAMA.flip(!UNYK.q(), RESET_DIVn);
  UGOT.flip(ULUR,      RESET_DIVn);
  TULU.flip(!UGOT.q(), RESET_DIVn);
  TUGO.flip(!TULU.q(), RESET_DIVn);
  TOFE.flip(!TUGO.q(), RESET_DIVn);
  TERU.flip(!TOFE.q(), RESET_DIVn);
  SOLA.flip(!TERU.q(), RESET_DIVn);
  SUBU.flip(!SOLA.q(), RESET_DIVn);
  TEKA.flip(!SUBU.q(), RESET_DIVn);
  UKET.flip(!TEKA.q(), RESET_DIVn);
  UPOF.flip(!UKET.q(), RESET_DIVn);

  // These register tick on _BOTH_EDGES_ of the master clock.
  ADYK.dtock(ATAL_4MHZ, in.T1nT2n, APUK_Q);
  AFUR.dtock(ATAL_4MHZ, in.T1nT2n, !ADYK_Q);
  ALEF.dtock(ATAL_4MHZ, in.T1nT2n, AFUR_Q);
  APUK.dtock(ATAL_4MHZ, in.T1nT2n, ALEF_Q);

  // This is the RESET2 register.
  AFER.tock(BOMA, in.T1nT2n, ASOL);

  //----------

  Output out;
  out.FF04_D0n = FF04_D0n;
  out.FF04_D1n = FF04_D1n;
  out.ATAL_4MHZ = ATAL_4MHZ;
  out.AMUK_4MHZ = AMUK_4MHZ;
  out.ARYF_4MHZ = ARYF_4MHZ;
  out.APUV_4MHZ = APUV_4MHZ;
  out.BAVU_1MHZ = BAVU_1MHZ;
  out.BOGA1MHZ = BOGA1MHZ;
  out.TAMA16384 = TAMA16384;
  out.CLK_16384 = CLK_16384;
  out.CLK_65536 = CLK_65536;
  out.CLK_262144 = CLK_262144;
  out.HAMA_512Kn = HAMA_512Kn;
  out.HORU_512Hz = HORU_512Hz;
  out.BUFY_256Hz = BUFY_256Hz;
  out.PHI_OUT = PHI_OUT;
  out.PHIn = PHIn;
  out.CLK2 = CLK2;
  out.CLK1 = CLK1;
  out.BYFE_128Hz = BYFE_128Hz;
  out.JESO_512K  = JESO_512K ;
  out.RESET6 = RESET6;
  out.RESET7 = RESET7;
  out.RESET9 = RESET9;
  out.RESET2 = RESET2;
  out.RESET_VIDEO = RESET_VIDEO;
  out.RESET_DIVn= RESET_DIVn;
  out.TO_CPU = TO_CPU;
  out.CPU_RD_SYNC = CPU_RD_SYNC;
  out.BUKE = BUKE;
  out.ABUZ = ABUZ;
  out.AFAS = AFAS;
  out.D_OE = D_OE;
  out.D0 = D0;
  out.D1 = D1;
  out.D2 = D2;
  out.D3 = D3;
  out.D4 = D4;
  out.D5 = D5;
  out.D6 = D6;
  out.D7 = D7;
  out.RESET8 = RESET8;
  out.RESET7n = RESET7n;

#else

  //----------
  // register outputs

  bool ADYK_Q = ADYK.q();
  bool AFUR_Q = AFUR.q();
  bool ALEF_Q = ALEF.q();
  bool APUK_Q = APUK.q();

  // These register tick on _BOTH_EDGES_ of the master clock.
  ADYK.dtock(not(in.CLKIN_B), true, APUK_Q);
  AFUR.dtock(not(in.CLKIN_B), true, !ADYK_Q);
  ALEF.dtock(not(in.CLKIN_B), true, AFUR_Q);
  APUK.dtock(not(in.CLKIN_B), true, ALEF_Q);

  bool BOGA1MHZ = or(ALEF_Q, AFUR_Q);
  //bool RESET_DIVn = nand(in.CPU_WR, in.addr == 0xFF04);
  bool RESET_DIVn = false;

  // NOTE - these FFs are daisy-chained, which means that the clock for each
  // link in the chain is the _new_ value of Q for the previous link.

  bool TAMA_Q = TAMA.q();
  bool UNYK_Q = UNYK.q();
  bool TERO_Q = TERO.q();
  bool UNER_Q = UNER.q();
  bool UFOR_Q = UFOR.q();
  bool UKUP_Q = UKUP.q();
  bool UGOT_Q = UGOT.q();
  bool TULU_Q = TULU.q();
  bool TUGO_Q = TUGO.q();
  bool TOFE_Q = TOFE.q();
  bool TERU_Q = TERU.q();
  bool SOLA_Q = SOLA.q();
  bool SUBU_Q = SUBU.q();
  bool TEKA_Q = TEKA.q();
  bool UKET_Q = UKET.q();
  bool UPOF_Q = UPOF.q();

  UKUP.tock(BOGA1MHZ,  RESET_DIVn, !UKUP_Q);
  UFOR.tock(!UKUP.q(), RESET_DIVn, !UFOR_Q);
  UNER.tock(!UFOR.q(), RESET_DIVn, !UNER_Q);
  TERO.tock(!UNER.q(), RESET_DIVn, !TERO_Q);
  UNYK.tock(!TERO.q(), RESET_DIVn, !UNYK_Q);
  TAMA.tock(!UNYK.q(), RESET_DIVn, !TAMA_Q);
  UGOT.tock(!TAMA.q(), RESET_DIVn, !UGOT_Q);
  TULU.tock(!UGOT.q(), RESET_DIVn, !TULU_Q);
  TUGO.tock(!TULU.q(), RESET_DIVn, !TUGO_Q);
  TOFE.tock(!TUGO.q(), RESET_DIVn, !TOFE_Q);
  TERU.tock(!TOFE.q(), RESET_DIVn, !TERU_Q);
  SOLA.tock(!TERU.q(), RESET_DIVn, !SOLA_Q);
  SUBU.tock(!SOLA.q(), RESET_DIVn, !SUBU_Q);
  TEKA.tock(!SUBU.q(), RESET_DIVn, !TEKA_Q);
  UKET.tock(!TEKA.q(), RESET_DIVn, !UKET_Q);
  UPOF.tock(!UKET.q(), RESET_DIVn, !UPOF_Q);

  //----------

  Output out;
  out.CLK_16384 = not(TAMA_Q);
  out.CLK_65536 = TERO_Q;
  out.CLK_262144 = UFOR_Q;

#endif

  tp.trace("CLKIN_B",     in.CLKIN_B);
  tp.trace("AFUR_Q",      AFUR.q());
  tp.trace("ALEF_Q",      ALEF.q());
  tp.trace("APUK_Q",      APUK.q());
  tp.trace("ADYK_Q",      ADYK.q());
  tp.trace("AFAS",      AFAS);
  tp.trace("ABUZ",      ABUZ);

  /*
  tp.trace("BOGA1MHZ",    BOGA1MHZ);
  tp.trace("CPU_RD_SYNC", CPU_RD_SYNC);
  tp.trace("PHI_OUT",     PHI_OUT);
  tp.trace("RESET2",      RESET2);
  tp.trace("AFER_Q",      AFER.q());
  tp.trace("UKUP_Q",      UKUP.q());
  tp.trace("UFOR_Q",      UFOR.q());
  tp.trace("UNER_Q",      UNER.q());
  tp.trace("TERO_Q",      TERO.q());
  tp.trace("UNYK_Q",      UNYK.q());
  tp.trace("TAMA_Q",      TAMA.q());
  tp.trace("BAPY",        BAPY);
  tp.trace("NULE",        NULE);
  tp.trace("BEJA",        BEJA);
  tp.trace("BUTO",        BUTO);
  tp.trace("TO_CPU",        TO_CPU);
  */

  return out;
}
