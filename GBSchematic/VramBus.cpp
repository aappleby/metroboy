#include "VramBus.h"

#include "ExtCpuBuses_sch.h"
#include "MemBus.h"
#include "OAM.h"
#include "ExtBus.h"
#include "PPU.h"
#include "Debug.h"
#include "CpuBus.h"
#include "AddressDecoder.h"
#include "System.h"
#include "Clocks.h"
#include "DmaBus.h"
#include "Sprites.h"
#include "Window.h"

//-----------------------------------------------------------------------------
// 25_VRAM_INTERFACE.png

void Vram::tick(OAM& oam, const AddressDecoder& dec, const Window& win, Vram& vram, const ExtCpuBuses_sch& ext_sch) {
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

  bool SYRO = not(dec.FEXXFFXXn);
  bool TEFA = nor(SYRO, ext_sch.TEXO);
  bool SOSE = and(mem.A15, TEFA); // odd...
  bool SOHO = and(spr.TACU, spr.TEXY);
  bool RAWA = not(SOHO);

  bool SYCY = not(NET02);
  SOTO.flip(SYCY, 0);

  bool TUCA = and(SOSE, clk.ABUZ);
  bool TUJA = and(SOSE, cpu.CPU_RD_SYNC);
  bool TEGU = and(SOSE, clk.AFAS);
  bool TAVY = not(ext.MOE_IN);
  bool TUTO = and(NET02, !SOTO_Q);
  bool SUDO = not(ext.MWR_IN);
  bool TEFY = not(ext.MCS_IN);

  bool SALE = mux2(TUTO, TAVY, TEGU);
  bool TYJY = mux2(TUTO, SUDO, TUJA);
  bool TOLE = mux2(TUTO, TEFY, TUCA);

  bool ROPY = not(ppu.XYMU);
  bool RYLU = nand(SALE, ROPY);

  bool APAM = not(dma.VRAM_TO_OAM);
  bool RUVY = not(SALE);
  bool SERE = and(TOLE, ROPY);
  bool SOHY = nand(TYJY, SERE);

  bool SUTU = nor(win.LENA, dma.VRAM_TO_OAM, spr.TEXY, SERE);
  bool SAZO = and(RUVY, SERE);
  bool RACU = and(RYLU, RAWA, win.MYMA, APAM);
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

  ext.MD_B = ROFA; // something about vram pins...

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
    MD7 = RAKU;
    MD4 = ROCE;
    MD3 = REMO;
    MD5 = ROPU;
    MD6 = RETA;
    MD2 = RYDO;
    MD0 = RODY;
    MD1 = REBA;
  }

  bool TUSO = nor(NET02, clk.BEDO);
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
    MD0 = TEME;
    MD1 = TEWU;
    MD2 = TYGO;
    MD3 = SOTE;
    MD4 = SEKE;
    MD5 = RUJO;
    MD6 = TOFA;
    MD7 = SUZA;
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

  bool XANE = nor(dma.VRAM_TO_OAM, ppu.XYMU);
  bool XEDU = not(XANE);

  // probably not actually inverters...
  bool XAKY = !mem.A0;
  bool XUXU = !mem.A1;
  bool XYNE = !mem.A2;
  bool XODY = !mem.A3;
  bool XECA = !mem.A4;
  bool XOBA = !mem.A5;
  bool XOPO = !mem.A6;
  bool XYBO = !mem.A7;
  bool RYSU = !mem.A8;
  bool RESE = !mem.A9;
  bool RUSE = !mem.A10;
  bool RYNA = !mem.A11;
  bool RUMO = !mem.A12;

  if (XEDU) {
    MA0  = XAKY;
    MA1  = XUXU;
    MA2  = XYNE;
    MA3  = XODY;
    MA4  = XECA;
    MA5  = XOBA;
    MA6  = XOPO;
    MA7  = XYBO;
    MA8  = RYSU;
    MA9  = RESE;
    MA10 = RUSE;
    MA11 = RYNA;
    MA12 = RUMO;
  }

  bool LYRA = nand(dbg.T1nT2, ext_sch.RORU);

  bool TOVO = not(ext.D0_IN);
  bool RUZY = not(ext.D1_IN);
  bool ROME = not(ext.D2_IN);
  bool SAZA = not(ext.D3_IN);
  bool TEHE = not(ext.D4_IN);
  bool RATU = not(ext.D5_IN);
  bool SOCA = not(ext.D6_IN);
  bool RYBA = not(ext.D7_IN);

  bool TUTY = not(TOVO);
  bool SYWA = not(RUZY);
  bool SUGU = not(ROME);
  bool TAWO = not(SAZA);
  bool TUTE = not(TEHE);
  bool SAJO = not(RATU);
  bool TEMY = not(SOCA);
  bool ROPA = not(RYBA);

  if (LYRA) {
    mem.D0 = TUTY;
    mem.D1 = SYWA;
    mem.D2 = SUGU;
    mem.D3 = TAWO;
    mem.D4 = TUTE;
    mem.D5 = SAJO;
    mem.D6 = TEMY;
    mem.D7 = ROPA;
  }

  const bool ROVE = not(RAHU);

  const bool SEFA = and(vram.MD0, ROVE);
  const bool SOGO = and(vram.MD1, ROVE);
  const bool SEFU = and(vram.MD2, ROVE);
  const bool SUNA = and(vram.MD3, ROVE);
  const bool SUMO = and(vram.MD4, ROVE);
  const bool SAZU = and(vram.MD5, ROVE);
  const bool SAMO = and(vram.MD6, ROVE);
  const bool SUKE = and(vram.MD7, ROVE);

  const bool REGE = not(SEFA);
  const bool RYKY = not(SOGO);
  const bool RAZO = not(SEFU);
  const bool RADA = not(SUNA);
  const bool RYRO = not(SUMO);
  const bool REVU = not(SAZU);
  const bool REKU = not(SAMO);
  const bool RYZE = not(SUKE);
  
  ext.MD0_A = REGE;
  ext.MD1_A = RYKY;
  ext.MD2_A = RAZO;
  ext.MD3_A = RADA;
  ext.MD4_A = RYRO;
  ext.MD5_A = REVU;
  ext.MD6_A = REKU;
  ext.MD7_A = RYZE;

  bool RALO = not(ext.D0_IN);
  bool TUNE = not(ext.D1_IN);
  bool SERA = not(ext.D2_IN);
  bool TENU = not(ext.D3_IN);
  bool SYSA = not(ext.D4_IN);
  bool SUGY = not(ext.D5_IN);
  bool TUBE = not(ext.D6_IN);
  bool SYZO = not(ext.D7_IN);

  bool WASA = not(RALO);
  bool WEJO = not(RALO);
  bool BOMO = not(TUNE);
  bool BUBO = not(TUNE);
  bool BASA = not(SERA);
  bool BETU = not(SERA);
  bool CAKO = not(TENU);
  bool CYME = not(TENU);
  bool BUMA = not(SYSA);
  bool BAXU = not(SYSA);
  bool BUPY = not(SUGY);
  bool BUHU = not(SUGY);
  bool BASY = not(TUBE);
  bool BYNY = not(TUBE);
  bool BAPE = not(SYZO);
  bool BYPY = not(SYZO);

  bool CEDE = not(dma.LUMA);
  if (CEDE) {
    oam.OAM_B_D0 = WASA;
    oam.OAM_A_D0 = WEJO;
    oam.OAM_B_D1 = BOMO;
    oam.OAM_A_D1 = BUBO;
    oam.OAM_B_D2 = BASA;
    oam.OAM_A_D2 = BETU;
    oam.OAM_B_D3 = CAKO;
    oam.OAM_A_D3 = CYME;
    oam.OAM_B_D4 = BUMA;
    oam.OAM_A_D4 = BAXU;
    oam.OAM_B_D5 = BUPY;
    oam.OAM_A_D5 = BUHU;
    oam.OAM_B_D6 = BASY;
    oam.OAM_A_D6 = BYNY;
    oam.OAM_B_D7 = BAPE;
    oam.OAM_A_D7 = BYPY;
  }

  bool TYVY = nand(SERE, spr.LEKO);
  bool SEBY = not(TYVY);

  if (SEBY) {
    mem.D0 = vram.MD0;
    mem.D1 = vram.MD1;
    mem.D2 = vram.MD2;
    mem.D3 = vram.MD3;
    mem.D4 = vram.MD4;
    mem.D5 = vram.MD5;
    mem.D6 = vram.MD6;
    mem.D7 = vram.MD7;
  }

  bool VYNO_Q = win.VYNO.q();
  bool VUJO_Q = win.VUJO.q();
  bool VYMU_Q = win.VYMU.q();

  bool XONU = not(win.XUHA);
  bool WUDO = not(VYNO_Q);
  bool WAWE = not(VUJO_Q);
  bool WOLU = not(VYMU_Q);

  bool XUCY = nand(win.NETA, win.PORE);
  if (XUCY) {
    vram.MA0 = XONU;
    vram.MA1 = WUDO;
    vram.MA2 = WAWE;
    vram.MA3 = WOLU;
  }

  bool VUZA = nor(ppu.FF40_D4, ppu.PYJU.q());

  bool VURY = not(VUZA);

  bool TOBO = not(ppu.PYJU.q());
  bool SUVO = not(ppu.POWY.q());
  bool RESO = not(ppu.POJU.q());
  bool ROHA = not(ppu.PULO.q());
  bool RUSA = not(ppu.POXA.q());
  bool VEJY = not(ppu.PYZO.q());
  bool SEZU = not(ppu.POZO.q());
  bool VAPY = not(ppu.RAWU.q());

  if (win.NETA) {
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