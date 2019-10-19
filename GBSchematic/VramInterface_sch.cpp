// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "ExtCpuBuses_sch.h"
#include "MemBus.h"
#include "VramBus.h"
#include "OamBus.h"
#include "ExtBus.h"
#include "PpuRegs.h"
#include "Debug.h"
#include "CpuBus.h"
#include "AddressDecoder.h"

//----------
// inputs

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

extern bool ABUZ;
extern bool AFAS;
extern bool MYMA;
extern bool LENA;
extern bool BEDO;


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

  bool RYVO = nand(mem.D5, ext_sch.LULA);
  bool RERA = nand(mem.D3, ext_sch.LULA);
  bool RABY = nand(mem.D2, ext_sch.LULA);
  bool RORY = nand(mem.D4, ext_sch.LULA);
  bool RUJA = nand(mem.D1, ext_sch.LULA);
  bool RAVU = nand(mem.D7, ext_sch.LULA);
  bool RAFY = nand(mem.D6, ext_sch.LULA);
  bool RUXA = nand(mem.D0, ext_sch.LULA);

  // why is this writing to the _A part of the data bus but not the _D?
  ext.D5_A = RYVO;
  ext.D3_A = RERA;
  ext.D2_A = RABY;
  ext.D4_A = RORY;
  ext.D1_A = RUJA;
  ext.D7_A = RAVU;
  ext.D6_A = RAFY;
  ext.D0_A = RUXA;

  bool MATU_Q = MATU.q();
  bool CUFE = unk3(SARO, MATU_Q, MOPA_PHI);
  bool VAPE = and(TACU, TUVO);
  bool AVER = and(ACYL, XYSO);
  bool XUJY = not(VAPE);
  bool BYCU = nor(CUFE, XUJY, AVER);
  COTA = not(BYCU);

  bool SYRO = not(dec.FEXXFFXXn);
  bool TEFA = nor(SYRO, ext_sch.TEXO);
  bool SOSE = and(mem.A15, TEFA); // odd...
  bool SOHO = and(TACU, TEXY);
  bool RAWA = not(SOHO);

  bool SYCY = not(NET02);
  SOTO.flip(SYCY, 0);

  bool TUCA = and(SOSE, ABUZ);
  bool TUJA = and(SOSE, cpu.CPU_RD_SYNC);
  bool TEGU = and(SOSE, AFAS);
  bool TAVY = not(ext.MOE_IN);
  bool TUTO = and(NET02, !SOTO_Q);
  bool SUDO = not(ext.MWR_IN);
  bool TEFY = not(ext.MCS_IN);

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

  ext.MCS_A = SOKY;
  ext.MCS_D = SETY;
  ext.MWR_A = SYSY;
  ext.MWR_D = RAGU;
  ext.MOE_A = REFO;
  ext.MOE_D = SAHA;

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
  bool RAKU = !not(ext.MD7_IN);
  bool ROCE = !not(ext.MD4_IN);
  bool REMO = !not(ext.MD3_IN);
  bool ROPU = !not(ext.MD5_IN);
  bool RETA = !not(ext.MD6_IN);
  bool RYDO = !not(ext.MD2_IN);
  bool RODY = !not(ext.MD0_IN);
  bool REBA = !not(ext.MD1_IN);

  if (RENA) {
    vram.MD7 = RAKU;
    vram.MD4 = ROCE;
    vram.MD3 = REMO;
    vram.MD5 = ROPU;
    vram.MD6 = RETA;
    vram.MD2 = RYDO;
    vram.MD0 = RODY;
    vram.MD1 = REBA;
  }

  bool TUSO = nor(NET02, BEDO);
  bool SOLE = not(TUSO);
  bool RUNY = not(ext.P10_B);

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
    vram.MD0 = TEME;
    vram.MD1 = TEWU;
    vram.MD2 = TYGO;
    vram.MD3 = SOTE;
    vram.MD4 = SEKE;
    vram.MD5 = RUJO;
    vram.MD6 = TOFA;
    vram.MD7 = SUZA;
  }

  bool SYNU = or(RAHU, vram.MD0);
  bool SYMA = or(RAHU, vram.MD1);
  bool ROKO = or(RAHU, vram.MD2);
  bool SYBU = or(RAHU, vram.MD3);
  bool SAKO = or(RAHU, vram.MD4);
  bool SEJY = or(RAHU, vram.MD5);
  bool SEDO = or(RAHU, vram.MD6);
  bool SAWU = or(RAHU, vram.MD7);

  bool RURA = not(SYNU);
  bool RULY = not(SYMA);
  bool RARE = not(ROKO);
  bool RODU = not(SYBU);
  bool RUBE = not(SAKO);
  bool RUMU = not(SEJY);
  bool RYTY = not(SEDO);
  bool RADY = not(SAWU);

  ext.MD0_OUT = RURA;
  ext.MD1_OUT = RULY;
  ext.MD2_OUT = RARE;
  ext.MD3_OUT = RODU;
  ext.MD4_OUT = RUBE;
  ext.MD5_OUT = RUMU;
  ext.MD6_OUT = RYTY;
  ext.MD7_OUT = RADY;

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
    vram.MA4 = XECA;
    vram.MA7 = XYBO;
    vram.MA8 = RYSU;
    vram.MA10 = RUSE;
    vram.MA12 = RUMO;
    vram.MA2 = XYNE;
    vram.MA5 = XOBA;
    vram.MA3 = XODY;
    vram.MA11 = RYNA;
    vram.MA9 = RESE;
    vram.MA0 = XAKY;
    vram.MA6 = XOPO;
    vram.MA1 = XUXU;
  }

  bool LYRA = nand(dbg.T1nT2, ext_sch.RORU);

  bool RYBA = not(ext.D7_IN);
  bool RUZY = not(ext.D1_IN);
  bool ROME = not(ext.D2_IN);
  bool TEHE = not(ext.D4_IN);
  bool SOCA = not(ext.D6_IN);
  bool RATU = not(ext.D5_IN);
  bool TOVO = not(ext.D0_IN);
  bool SAZA = not(ext.D3_IN);

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

  const bool SEFA = and(vram.MD0, ROVE);
  const bool SUNA = and(vram.MD3, ROVE);
  const bool SUMO = and(vram.MD4, ROVE);
  const bool SUKE = and(vram.MD7, ROVE);
  const bool SAMO = and(vram.MD6, ROVE);
  const bool SOGO = and(vram.MD1, ROVE);
  const bool SAZU = and(vram.MD5, ROVE);
  const bool SEFU = and(vram.MD2, ROVE);

  const bool REGE = not(SEFA);
  const bool RADA = not(SUNA);
  const bool RYRO = not(SUMO);
  const bool RYZE = not(SUKE);
  const bool REKU = not(SAMO);
  const bool RYKY = not(SOGO);
  const bool REVU = not(SAZU);
  const bool RAZO = not(SEFU);

  ext.MD0_A = REGE;
  ext.MD3_A = RADA;
  ext.MD4_A = RYRO;
  ext.MD7_A = RYZE;
  ext.MD6_A = REKU;
  ext.MD1_A = RYKY;
  ext.MD5_A = REVU;
  ext.MD2_A = RAZO;

  bool CEDE = not(LUMA);
  bool SYZO = not(ext.D7_IN);
  bool TUNE = not(ext.D1_IN);
  bool SERA = not(ext.D2_IN);
  bool SYSA = not(ext.D4_IN);
  bool TUBE = not(ext.D6_IN);
  bool SUGY = not(ext.D5_IN);
  bool RALO = not(ext.D0_IN);
  bool TENU = not(ext.D3_IN);

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
    oam.OAM_B_D7 = BAPE;
    oam.OAM_A_D7 = BYPY;
    oam.OAM_B_D1 = BOMO;
    oam.OAM_A_D1 = BUBO;
    oam.OAM_B_D2 = BASA;
    oam.OAM_A_D2 = BETU;
    oam.OAM_B_D4 = BUMA;
    oam.OAM_A_D4 = BAXU;
    oam.OAM_B_D6 = BASY;
    oam.OAM_A_D6 = BYNY;
    oam.OAM_B_D5 = BUPY;
    oam.OAM_A_D5 = BUHU;
    oam.OAM_B_D0 = WASA;
    oam.OAM_A_D0 = WEJO;
    oam.OAM_B_D3 = CAKO;
    oam.OAM_A_D3 = CYME;
  }

  bool TYVY = nand(SERE, LEKO);
  bool SEBY = not(TYVY);

  bool RORO = not(vram.MD5);
  bool RERY = not(vram.MD0);
  bool RONA = not(vram.MD2);
  bool RUNA = not(vram.MD1);
  bool RUNO = not(vram.MD3);
  bool SAME = not(vram.MD7);
  bool SANA = not(vram.MD4);
  bool RABO = not(vram.MD6);

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
    vram.MA0 = XONU;
    vram.MA1 = WUDO;
    vram.MA2 = WAWE;
    vram.MA3 = WOLU;
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
    vram.MA12 = VURY;
    vram.MA11 = TOBO;
    vram.MA10 = SUVO;
    vram.MA9 = RESO;
    vram.MA8 = ROHA;
    vram.MA7 = RUSA;
    vram.MA6 = VEJY;
    vram.MA5 = SEZU;
    vram.MA4 = VAPY;
  }
}