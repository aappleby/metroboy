#include "Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P1_ClocksReset {

  struct Input {
    // physical pins
    bool CPU_RD;
    bool CPU_WR;
    bool RESET;       // active high!
    bool CLKIN_A;     // this is actually CLOCK_GOOD or similar
    bool CLKIN_B;     // the master 4mhz clock

    bool ABOL_1MHZ;   // this is actually CPU_RESET or similar
    bool AJER_2MHZ;   // on apu sheet
    bool FROM_CPU3;
    bool FROM_CPU4;
    bool APU_RESET;
    bool APU_RESET5n;
    bool FERO_Q;      // something debug-related
    bool FF04_FF07;
    bool TOLA_A1n;
    bool TOVY_A0n;
    bool T1T2n;       // true for debugging
    bool T1nT2;       // true for debugging
    bool T1nT2n;      // true for normal mode
    bool FF40_D7;     // lcd on
    bool FF60_D1;     // debugging
  };

  struct Output {
    bool FF04_D0n;
    bool FF04_D1n;
    bool ATAL_4MHZ;
    bool AMUK_4MHZ;
    bool ARYF_4MHZ;
    bool APUV_4MHZ;
    bool BAVU_1MHZ;
    bool BOGA1MHZ;
    bool TAMA16384;
    bool CLK_16384;
    bool CLK_65536;
    bool CLK_262144;
    bool HAMA_512Kn;
    bool HORU_512Hz;
    bool BUFY_256Hz;
    bool PHI_OUT;     // cartridge clock
    bool PHIn;
    bool CLK2;
    bool CLK1;
    bool BYFE_128Hz;
    bool JESO_512K;
    bool RESET2;      // active low!
    bool RESET6;
    bool RESET7;
    bool RESET7n;
    bool RESET8;
    bool RESET9;
    bool RESET_VIDEO;
    bool RESET_DIVn;

    bool TO_CPU;
    bool CPU_RD_SYNC; 
    bool BUKE;
    bool ABUZ;
    bool AFAS;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  // 1mhz phase generator
  reg ADYK,AFUR,ALEF,APUK;

  // register for the RESET2 signal, roughly
  reg AFER;

  // div
  reg UKUP,UFOR,UNER,TERO,UNYK,TAMA,UGOT,TULU,TUGO,TOFE,TERU,SOLA,SUBU,TEKA,UKET,UPOF;

  // clock divider for apu sequencer
  reg BARA,CARU,BYLU;

  // clock divider for something else...
  reg ATYK,AVOK,JESO;

  Output tick(const Input& in) {
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
    UKUP.tock(BOGA1MHZ,  RESET_DIVn, !UKUP_Q);
    UFOR.tock(!UKUP.q(), RESET_DIVn, !UFOR_Q);
    UNER.tock(!UFOR.q(), RESET_DIVn, !UNER_Q);
    TERO.tock(!UNER.q(), RESET_DIVn, !TERO_Q);
    UNYK.tock(!TERO.q(), RESET_DIVn, !UNYK_Q);
    TAMA.tock(!UNYK.q(), RESET_DIVn, !TAMA_Q);
    UGOT.tock(ULUR,      RESET_DIVn, !UGOT_Q);
    TULU.tock(!UGOT.q(), RESET_DIVn, !TULU_Q);
    TUGO.tock(!TULU.q(), RESET_DIVn, !TUGO_Q);
    TOFE.tock(!TUGO.q(), RESET_DIVn, !TOFE_Q);
    TERU.tock(!TOFE.q(), RESET_DIVn, !TERU_Q);
    SOLA.tock(!TERU.q(), RESET_DIVn, !SOLA_Q);
    SUBU.tock(!SOLA.q(), RESET_DIVn, !SUBU_Q);
    TEKA.tock(!SUBU.q(), RESET_DIVn, !TEKA_Q);
    UKET.tock(!TEKA.q(), RESET_DIVn, !UKET_Q);
    UPOF.tock(!UKET.q(), RESET_DIVn, !UPOF_Q);

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

    return out;
  }

  static void test() {
    Input in = {};
    in.RESET = 0;
    in.CLKIN_A = 1;
    in.CLKIN_B = 0;
    in.ABOL_1MHZ = 0;
    in.AJER_2MHZ = 0;
    in.CPU_RD = 0;
    in.CPU_WR = 0;
    in.FROM_CPU3 = 1;
    in.FROM_CPU4 = 0;
    in.APU_RESET = 0;
    in.APU_RESET5n = 1;
    in.FERO_Q = 0;
    in.FF04_FF07 = 0;
    in.TOLA_A1n = 0;
    in.TOVY_A0n = 0;
    in.T1T2n = 0;
    in.T1nT2 = 0;
    in.T1nT2n = 1;
    in.FF40_D7 = 1;
    in.FF60_D1 = 0;

    P1_ClocksReset sch;
    for (int i = 0; i < 128; i++) {
      in.CLKIN_B = (~((i + 0) >> 0)) & 1;
      // dunno the phase of these yet
      in.AJER_2MHZ = ((i + 0) >> 1) & 1;

      sch.tick(in);
    }
  }
};
