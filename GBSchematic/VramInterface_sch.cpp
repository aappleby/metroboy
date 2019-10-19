// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "MemBus.h"
#include "VramBus.h"
#include "OamBus.h"
#include "ExtBus.h"
#include "PpuRegs.h"

//----------
// inputs

extern bool CPU_RD_SYNC;
extern bool FEXXFFXXn;
extern reg MATU;
extern bool MOPA_PHI;
extern bool VRAM_TO_OAM;

extern reg RAWU;
extern reg POZO;
extern reg PYZO;
extern reg POXA;
extern reg PULO;
extern reg POJU;
extern reg POWY;
extern reg PYJU;

extern bool SARO;
extern bool TUVO;
extern bool XYMU;
extern bool LUMA;
extern bool XYSO;
extern bool TACU;
extern bool ACYL;
extern bool TEXY;
extern bool TEXO;
extern bool ABUZ;
extern bool AFAS;
extern bool MYMA;
extern bool LENA;
extern bool BEDO;
extern bool RORU;
extern bool LULA;
extern bool LEKO;
extern bool NETA;
extern bool PORE;
extern bool POTU;
extern bool XUHA;
extern reg VYNO;
extern reg VUJO;
extern reg VYMU;

//----------
// outputs

bool MD_B; // this has gotta be one of the vram wr/cs/oe pins or something
bool WUKO; // controls something window
bool COTA; // controls something sprite related

//----------
// registers

reg SOTO;

//----------
// 25_VRAM_INTERFACE.png

void tock_vram() {
  bool SOTO_Q = SOTO.q();

  bool RYVO = nand(mem.D5, LULA);
  bool RERA = nand(mem.D3, LULA);
  bool RABY = nand(mem.D2, LULA);
  bool RORY = nand(mem.D4, LULA);
  bool RUJA = nand(mem.D1, LULA);
  bool RAVU = nand(mem.D7, LULA);
  bool RAFY = nand(mem.D6, LULA);
  bool RUXA = nand(mem.D0, LULA);

  // why is this writing to the _A part of the data bus but not the _D?
  D5_A = RYVO;
  D3_A = RERA;
  D2_A = RABY;
  D4_A = RORY;
  D1_A = RUJA;
  D7_A = RAVU;
  D6_A = RAFY;
  D0_A = RUXA;

  bool MATU_Q = MATU.q();
  bool CUFE = unk3(SARO, MATU_Q, MOPA_PHI);
  bool VAPE = and(TACU, TUVO);
  bool AVER = and(ACYL, XYSO);
  bool XUJY = not(VAPE);
  bool BYCU = nor(CUFE, XUJY, AVER);
  COTA = not(BYCU);

  bool SYRO = not(FEXXFFXXn);
  bool TEFA = nor(SYRO, TEXO);
  bool SOSE = and(mem.A15, TEFA); // odd...
  bool SOHO = and(TACU, TEXY);
  bool RAWA = not(SOHO);

  bool SYCY = not(NET02);
  SOTO.flip(SYCY, 0);

  bool TUCA = and(SOSE, ABUZ);
  bool TUJA = and(SOSE, CPU_RD_SYNC);
  bool TEGU = and(SOSE, AFAS);
  bool TAVY = not(MOE_IN);
  bool TUTO = and(NET02, !SOTO_Q);
  bool SUDO = not(MWR_IN);
  bool TEFY = not(MCS_IN);

  bool SALE = mux2(TUTO, TAVY, TEGU);
  bool TYJY = mux2(TUTO, SUDO, TUJA);
  bool TOLE = mux2(TUTO, TEFY, TUCA);

  bool ROPY = not(XYMU);
  bool RYLU = nand(SALE, ROPY);

  bool APAM = not(VRAM_TO_OAM);
  bool RUVY = not(SALE);
  bool SERE = and(TOLE, ROPY);
  bool SOHY = nand(TYJY, SERE);

  bool SUTU = nor(LENA, VRAM_TO_OAM, TEXY, SERE);
  bool SAZO = and(RUVY, SERE);
  bool RACU = and(RYLU, RAWA, MYMA, APAM);
  bool RACO = not(TUTO);

  bool RUTE = or(TUTO, RACO); // wat? or of a signal with its own inverse...
  bool SEWO = or(TUTO, SUTU);
  bool TODE = and(SUTU, RACO);
  bool SEMA = and(RACU, RACO);
  bool TAXY = and(SOHY, RACO);
  bool SOFY = or(TUTO, SOHY);

  bool SAHA = not(RUTE);
  bool SETY = not(SEWO);
  bool SOKY = not(TODE);
  bool REFO = not(SEMA);
  bool SYSY = not(TAXY);
  bool RAGU = not(SOFY);

  MCS_A = SOKY;
  MCS_D = SETY;
  MWR_A = SYSY;
  MWR_D = RAGU;
  MOE_A = REFO;
  MOE_D = SAHA;

  // this is some pulse generator sorta thing?
  bool RYJE = not(SAZO);
  bool REVO = not(RYJE);
  bool RELA = or(REVO, SAZO);
  bool ROCY = and(SAZO, REVO);

  bool RENA = not(RELA);
  bool RAHU = not(ROCY);
  bool ROFA = not(RENA);

  MD_B = ROFA; // something about vram pins...

  // buffer or inverter? looks like buffer...
  bool RAKU = !not(MD7_IN);
  bool ROCE = !not(MD4_IN);
  bool REMO = !not(MD3_IN);
  bool ROPU = !not(MD5_IN);
  bool RETA = !not(MD6_IN);
  bool RYDO = !not(MD2_IN);
  bool RODY = !not(MD0_IN);
  bool REBA = !not(MD1_IN);

  if (RENA) {
    MD7 = RAKU;
    MD4 = ROCE;
    MD3 = REMO;
    MD5 = ROPU;
    MD6 = RETA;
    MD2 = RYDO;
    MD0 = RODY;
    MD1 = REBA;
  }

  bool TUSO = nor(NET02, BEDO);
  bool SOLE = not(TUSO);
  bool RUNY = not(P10_B);

  bool TAHY = not(SOLE);
  bool TESU = not(SOLE);
  bool TAXO = not(SOLE);
  bool TOVU = not(SOLE);
  bool TAZU = not(SOLE);
  bool TEWA = not(SOLE);
  bool SOSA = not(SOLE);
  bool SEDU = not(SOLE);

  if (RUNY) {
    mem.D4 = TAHY;
    mem.D5 = TESU;
    mem.D3 = TAXO;
    mem.D0 = TOVU;
    mem.D6 = TAZU;
    mem.D7 = TEWA;
    mem.D1 = SOSA;
    mem.D2 = SEDU;
  }

  bool TEME = not(mem.D0);
  bool TEWU = not(mem.D1);
  bool TYGO = not(mem.D2);
  bool SOTE = not(mem.D3);
  bool SEKE = not(mem.D4);
  bool RUJO = not(mem.D5);
  bool TOFA = not(mem.D6);
  bool SUZA = not(mem.D7);

  if (RAHU) {
    MD0 = TEME;
    MD1 = TEWU;
    MD2 = TYGO;
    MD3 = SOTE;
    MD4 = SEKE;
    MD5 = RUJO;
    MD6 = TOFA;
    MD7 = SUZA;
  }

  bool SYNU = or(RAHU, MD0);
  bool SYMA = or(RAHU, MD1);
  bool ROKO = or(RAHU, MD2);
  bool SYBU = or(RAHU, MD3);
  bool SAKO = or(RAHU, MD4);
  bool SEJY = or(RAHU, MD5);
  bool SEDO = or(RAHU, MD6);
  bool SAWU = or(RAHU, MD7);

  bool RURA = not(SYNU);
  bool RULY = not(SYMA);
  bool RARE = not(ROKO);
  bool RODU = not(SYBU);
  bool RUBE = not(SAKO);
  bool RUMU = not(SEJY);
  bool RYTY = not(SEDO);
  bool RADY = not(SAWU);

  MD0_OUT = RURA;
  MD1_OUT = RULY;
  MD2_OUT = RARE;
  MD3_OUT = RODU;
  MD4_OUT = RUBE;
  MD5_OUT = RUMU;
  MD6_OUT = RYTY;
  MD7_OUT = RADY;

  bool XANE = nor(VRAM_TO_OAM, XYMU);
  bool XEDU = not(XANE);

  bool XECA = !mem.A4;
  bool XYBO = !mem.A7;
  bool RYSU = !mem.A8;
  bool RUSE = !mem.A10;
  bool RUMO = !mem.A12;
  bool XYNE = !mem.A2;
  bool XOBA = !mem.A5;
  bool XODY = !mem.A3;
  bool RYNA = !mem.A11;
  bool RESE = !mem.A9;
  bool XAKY = !mem.A0;
  bool XOPO = !mem.A6;
  bool XUXU = !mem.A1;

  if (XEDU) {
    MA4 = XECA;
    MA7 = XYBO;
    MA8 = RYSU;
    MA10 = RUSE;
    MA12 = RUMO;
    MA2 = XYNE;
    MA5 = XOBA;
    MA3 = XODY;
    MA11 = RYNA;
    MA9 = RESE;
    MA0 = XAKY;
    MA6 = XOPO;
    MA1 = XUXU;
  }

  bool LYRA = nand(T1nT2, RORU);

  bool RYBA = not(D7_IN);
  bool RUZY = not(D1_IN);
  bool ROME = not(D2_IN);
  bool TEHE = not(D4_IN);
  bool SOCA = not(D6_IN);
  bool RATU = not(D5_IN);
  bool TOVO = not(D0_IN);
  bool SAZA = not(D3_IN);

  bool ROPA = not(RYBA);
  bool SYWA = not(RUZY);
  bool SUGU = not(ROME);
  bool TUTE = not(TEHE);
  bool TEMY = not(SOCA);
  bool SAJO = not(RATU);
  bool TUTY = not(TOVO);
  bool TAWO = not(SAZA);

  if (LYRA) {
    mem.D7 = ROPA;
    mem.D1 = SYWA;
    mem.D2 = SUGU;
    mem.D4 = TUTE;
    mem.D6 = TEMY;
    mem.D5 = SAJO;
    mem.D0 = TUTY;
    mem.D3 = TAWO;
  }

  const bool ROVE = not(RAHU);

  const bool SEFA = and(MD0, ROVE);
  const bool SUNA = and(MD3, ROVE);
  const bool SUMO = and(MD4, ROVE);
  const bool SUKE = and(MD7, ROVE);
  const bool SAMO = and(MD6, ROVE);
  const bool SOGO = and(MD1, ROVE);
  const bool SAZU = and(MD5, ROVE);
  const bool SEFU = and(MD2, ROVE);

  const bool REGE = not(SEFA);
  const bool RADA = not(SUNA);
  const bool RYRO = not(SUMO);
  const bool RYZE = not(SUKE);
  const bool REKU = not(SAMO);
  const bool RYKY = not(SOGO);
  const bool REVU = not(SAZU);
  const bool RAZO = not(SEFU);

  MD0_A = REGE;
  MD3_A = RADA;
  MD4_A = RYRO;
  MD7_A = RYZE;
  MD6_A = REKU;
  MD1_A = RYKY;
  MD5_A = REVU;
  MD2_A = RAZO;

  bool CEDE = not(LUMA);
  bool SYZO = not(D7_IN);
  bool TUNE = not(D1_IN);
  bool SERA = not(D2_IN);
  bool SYSA = not(D4_IN);
  bool TUBE = not(D6_IN);
  bool SUGY = not(D5_IN);
  bool RALO = not(D0_IN);
  bool TENU = not(D3_IN);

  bool BAPE = not(SYZO);
  bool BYPY = not(SYZO);
  bool BOMO = not(TUNE);
  bool BUBO = not(TUNE);
  bool BASA = not(SERA);
  bool BETU = not(SERA);
  bool BUMA = not(SYSA);
  bool BAXU = not(SYSA);
  bool BASY = not(TUBE);
  bool BYNY = not(TUBE);
  bool BUPY = not(SUGY);
  bool BUHU = not(SUGY);
  bool WASA = not(RALO);
  bool WEJO = not(RALO);
  bool CAKO = not(TENU);
  bool CYME = not(TENU);

  if (CEDE) {
    OAM_B_D7 = BAPE;
    OAM_A_D7 = BYPY;
    OAM_B_D1 = BOMO;
    OAM_A_D1 = BUBO;
    OAM_B_D2 = BASA;
    OAM_A_D2 = BETU;
    OAM_B_D4 = BUMA;
    OAM_A_D4 = BAXU;
    OAM_B_D6 = BASY;
    OAM_A_D6 = BYNY;
    OAM_B_D5 = BUPY;
    OAM_A_D5 = BUHU;
    OAM_B_D0 = WASA;
    OAM_A_D0 = WEJO;
    OAM_B_D3 = CAKO;
    OAM_A_D3 = CYME;
  }

  bool TYVY = nand(SERE, LEKO);
  bool SEBY = not(TYVY);

  bool RORO = not(MD5);
  bool RERY = not(MD0);
  bool RONA = not(MD2);
  bool RUNA = not(MD1);
  bool RUNO = not(MD3);
  bool SAME = not(MD7);
  bool SANA = not(MD4);
  bool RABO = not(MD6);

  bool REXU = not(RORO);
  bool RUGA = not(RERY);
  bool RYBU = not(RONA);
  bool ROTA = not(RUNA);
  bool RAJU = not(RUNO);
  bool TOKU = not(SAME);
  bool TYJA = not(SANA);
  bool RUPY = not(RABO);

  if (SEBY) {
    mem.D5 = REXU;
    mem.D0 = RUGA;
    mem.D2 = RYBU;
    mem.D1 = ROTA;
    mem.D3 = RAJU;
    mem.D7 = TOKU;
    mem.D4 = TYJA;
    mem.D6 = RUPY;
  }

  bool XUCY = nand(NETA, PORE);
  bool XEZE = nand(POTU, PORE);
  WUKO = not(XEZE);

  bool VYNO_Q = VYNO.q();
  bool VUJO_Q = VUJO.q();
  bool VYMU_Q = VYMU.q();

  bool XONU = not(XUHA);
  bool WUDO = not(VYNO_Q);
  bool WAWE = not(VUJO_Q);
  bool WOLU = not(VYMU_Q);

  if (XUCY) {
    MA0 = XONU;
    MA1 = WUDO;
    MA2 = WAWE;
    MA3 = WOLU;
  }

  bool VUZA = nor(FF40_D4, PYJU.q());

  bool VURY = not(VUZA);

  bool TOBO = not(PYJU.q());
  bool SUVO = not(POWY.q());
  bool RESO = not(POJU.q());
  bool ROHA = not(PULO.q());
  bool RUSA = not(POXA.q());
  bool VEJY = not(PYZO.q());
  bool SEZU = not(POZO.q());
  bool VAPY = not(RAWU.q());

  if (NETA) {
    MA12 = VURY;
    MA11 = TOBO;
    MA10 = SUVO;
    MA9 = RESO;
    MA8 = ROHA;
    MA7 = RUSA;
    MA6 = VEJY;
    MA5 = SEZU;
    MA4 = VAPY;
  }
}