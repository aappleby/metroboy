#include "Background.h"

#include "Clocks.h"
#include "VramBus.h"
#include "ExtBus.h"
#include "PPU.h"
#include "Window.h"
#include "Sprites.h"

//-----------------------------------------------------------------------------

void Background::tick(const Window& win, Vram& vram, const Clocks& clk) {
  //----------
  // y + scy;

  bool FAFO_S = add_s(ppu.V0, ppu.SCY_D0, 0);
  bool FAFO_C = add_c(ppu.V0, ppu.SCY_D0, 0);
  bool EMUX_S = add_s(ppu.V1, ppu.SCY_D1, FAFO_C);
  bool EMUX_C = add_c(ppu.V1, ppu.SCY_D1, FAFO_C);
  bool ECAB_S = add_s(ppu.V2, ppu.SCY_D2, EMUX_C);
  bool ECAB_C = add_c(ppu.V2, ppu.SCY_D2, EMUX_C);
  bool ETAM_S = add_s(ppu.V3, ppu.SCY_D3, ECAB_C);
  bool ETAM_C = add_c(ppu.V3, ppu.SCY_D3, ECAB_C);
  bool DOTO_S = add_s(ppu.V4, ppu.SCY_D4, ETAM_C);
  bool DOTO_C = add_c(ppu.V4, ppu.SCY_D4, ETAM_C);
  bool DABA_S = add_s(ppu.V5, ppu.SCY_D5, DOTO_C);
  bool DABA_C = add_c(ppu.V5, ppu.SCY_D5, DOTO_C);
  bool EFYK_S = add_s(ppu.V6, ppu.SCY_D6, DABA_C);
  bool EFYK_C = add_c(ppu.V6, ppu.SCY_D6, DABA_C);
  bool EJOK_S = add_s(ppu.V7, ppu.SCY_D7, EFYK_C);
  bool EJOK_C = add_c(ppu.V7, ppu.SCY_D7, EFYK_C);
  (void)EJOK_C;

  bool ASUM = not(win.XUHA);
  bool EVAD = not(FAFO_S);
  bool DAHU = not(EMUX_S);
  bool DODE = not(ECAB_S);
  bool DUHO = not(ETAM_S);
  bool CASE = not(DOTO_S);
  bool CYPO = not(DABA_S);
  bool CETA = not(EFYK_S);
  bool DAFE = not(EJOK_S);

  bool AXAD = not(win.PORE);
  bool ASUL = and(AXAD, win.NETA);
  bool BEJE = not(ASUL);
  if (BEJE) {
    vram.MA0 = ASUM;
    vram.MA1 = EVAD;
    vram.MA2 = DAHU;
    vram.MA3 = DODE;
  }

  //----------
  // x + scx

  bool XEHO_Q = ppu.X_R0.q();
  bool SAVY_Q = ppu.X_R1.q();
  bool XODU_Q = ppu.X_R2.q();
  bool XYDO_Q = ppu.X_R3.q();
  bool TUHU_Q = ppu.X_R4.q();
  bool TUKY_Q = ppu.X_R5.q();
  bool TAKO_Q = ppu.X_R6.q();
  bool SYBE_Q = ppu.X_R7.q();

  //bool ATAD_S = add_s(XEHO_Q, ppu.FF43_D0, 0);
  bool ATAD_C = add_c(XEHO_Q, ppu.SCX_D0, 0);

  //bool BEHU_S = add_s(SAVY_Q, ppu.FF43_D1, ATAD_C);
  bool BEHU_C = add_c(SAVY_Q, ppu.SCX_D1, ATAD_C);

  //bool APYH_S = add_s(XODU_Q, ppu.FF43_D2, BEHU_C);
  bool APYH_C = add_c(XODU_Q, ppu.SCX_D2, BEHU_C);

  bool BABE_S = add_s(XYDO_Q, ppu.SCX_D3, APYH_C);
  bool BABE_C = add_c(XYDO_Q, ppu.SCX_D3, APYH_C);

  bool ABOD_S = add_s(TUHU_Q, ppu.SCX_D4, BABE_C);
  bool ABOD_C = add_c(TUHU_Q, ppu.SCX_D4, BABE_C);

  bool BEWY_S = add_s(TUKY_Q, ppu.SCX_D5, ABOD_C);
  bool BEWY_C = add_c(TUKY_Q, ppu.SCX_D5, ABOD_C);

  bool BYCA_S = add_s(TAKO_Q, ppu.SCX_D6, BEWY_C);
  bool BYCA_C = add_c(TAKO_Q, ppu.SCX_D6, BEWY_C);

  bool ACUL_S = add_s(SYBE_Q, ppu.SCX_D7, BYCA_C);
  //bool ACUL_C = add_c(SYBE_Q, ppu.FF43_D7, BYCA_C);

  bool AMUV = not(ppu.LCDC_BGMAP);
  bool COXO = not(spr.WEFE);
  bool COVE = not(spr.WEFE);

  // maybe the 'unk1' block inverts its sum out? otherwise we'd be putting the inverted sum on the bus...
  // or maybe these are not inverters...
  bool AXEP = not(BABE_S);
  bool AFEB = not(ABOD_S);
  bool ALEL = not(BEWY_S);
  bool COLY = not(BYCA_S);
  bool AJAN = not(ACUL_S);

  bool ACEN = and(AXAD, win.POTU);
  bool BAFY = not(ACEN);
  if (BAFY) {
    vram.MA0 = AXEP;
    vram.MA1 = AFEB;
    vram.MA2 = ALEL;
    vram.MA3 = COLY;
    vram.MA4 = AJAN;

    vram.MA5 = DUHO;
    vram.MA6 = CASE;
    vram.MA7 = CYPO;
    vram.MA8 = CETA;
    vram.MA9 = DAFE;
    vram.MA10 = AMUV;
    vram.MA12 = COXO;
    vram.MA11 = COVE;
  }

  //----------
  // some shift register connected to VAVA3?
  // this is the "draw background here" shift register, it should probably move to pixelmux

  bool DEPO_Q = spr.DEPO.q();

  bool XOGA = not(!DEPO_Q);
  bool XURA = not(!DEPO_Q);
  bool TAJO = not(!DEPO_Q);
  bool XENU = not(!DEPO_Q);
  bool XYKE = not(!DEPO_Q);
  bool XABA = not(!DEPO_Q);
  bool TAFU = not(!DEPO_Q);
  bool XUHO = not(!DEPO_Q);

  bool TEDE = nand(!DEPO_Q, ppu.LESY);
  bool XALA = nand(!DEPO_Q, ppu.LOTA);
  bool TYRA = nand(!DEPO_Q, ppu.LYKU);
  bool XYRU = nand(!DEPO_Q, ppu.ROBY);
  bool XUKU = nand(!DEPO_Q, ppu.TYTA);
  bool XELY = nand(!DEPO_Q, ppu.TYCO);
  bool TYKO = nand(!DEPO_Q, ppu.SOKA);
  bool TUWU = nand(!DEPO_Q, ppu.XOVU);

  bool WOKA = nand(XOGA, ppu.LESY);
  bool WEDE = nand(XURA, ppu.LOTA);
  bool TUFO = nand(TAJO, ppu.LYKU);
  bool WEVO = nand(XENU, ppu.ROBY);
  bool WEDY = nand(XYKE, ppu.TYTA);
  bool WUJA = nand(XABA, ppu.TYCO);
  bool TENA = nand(TAFU, ppu.SOKA);
  bool WUBU = nand(XUHO, ppu.XOVU);

  //----------
  // registers = background pixel pipe?

  bool VEZO_Q = ppu.VEZO.srtock(clk.CLKPIPE, TEDE, WOKA, 0);
  bool WURU_Q = ppu.WURU.srtock(clk.CLKPIPE, XALA, WEDE, VEZO_Q);
  bool VOSA_Q = ppu.VOSA.srtock(clk.CLKPIPE, TYRA, TUFO, WURU_Q);
  bool WYFU_Q = ppu.WYFU.srtock(clk.CLKPIPE, XYRU, WEVO, VOSA_Q);
  bool XETE_Q = ppu.XETE.srtock(clk.CLKPIPE, XUKU, WEDY, WYFU_Q);
  bool WODA_Q = ppu.WODA.srtock(clk.CLKPIPE, XELY, WUJA, XETE_Q);
  bool VUMO_Q = ppu.VUMO.srtock(clk.CLKPIPE, TYKO, TENA, WODA_Q);
  bool VAVA_Q = ppu.VAVA.srtock(clk.CLKPIPE, TUWU, WUBU, VUMO_Q);
  VAVA3 = VAVA_Q;
}
//-----------------------------------------------------------------------------

void Background::tick_bgpixelshifter(const Window& win, const Vram& vram, const Clocks& clk) {

  wire LESO = not(win.MOFU);
  wire AJAR = not(LESO);
  wire LABU = not(AJAR);
  wire RAWU_Q = ppu.RAWU.tock(LABU, win.VYPO, vram.MD0);
  wire POZO_Q = ppu.POZO.tock(LABU, win.VYPO, vram.MD1);
  wire PYZO_Q = ppu.PYZO.tock(LABU, win.VYPO, vram.MD2);
  wire POXA_Q = ppu.POXA.tock(LABU, win.VYPO, vram.MD3);
  wire PULO_Q = ppu.PULO.tock(LABU, win.VYPO, vram.MD4);
  wire POJU_Q = ppu.POJU.tock(LABU, win.VYPO, vram.MD5);
  wire POWY_Q = ppu.POWY.tock(LABU, win.VYPO, vram.MD6);
  wire PYJU_Q = ppu.PYJU.tock(LABU, win.VYPO, vram.MD7);

  wire METE = not(win.NYDY);
  wire LOMA = not(METE);
  wire LEGU_Q = ppu.LEGU.latch(LOMA, vram.MD0);
  wire NUDU_Q = ppu.NUDU.latch(LOMA, vram.MD1);
  bool MUKU_Q = ppu.MUKU.latch(LOMA, vram.MD2);
  bool LUZO_Q = ppu.LUZO.latch(LOMA, vram.MD3);
  bool MEGU_Q = ppu.MEGU.latch(LOMA, vram.MD4);
  bool MYJY_Q = ppu.MYJY.latch(LOMA, vram.MD5);
  bool NASA_Q = ppu.NASA.latch(LOMA, vram.MD6);
  bool NEFO_Q = ppu.NEFO.latch(LOMA, vram.MD7);

  bool TOSA = not(!RAWU_Q);
  bool RUCO = not(!POZO_Q);
  bool TYCE = not(!PYZO_Q);
  bool REVY = not(!POXA_Q);
  bool RYGA = not(!PULO_Q);
  bool RYLE = not(!POJU_Q);
  bool RAPU = not(!POWY_Q);
  bool SOJA = not(!PYJU_Q);

  bool LUHE = not(LEGU_Q);
  bool NOLY = not(NUDU_Q);
  bool LEKE = not(MUKU_Q);
  bool LOMY = not(LUZO_Q);
  bool LALA = not(MEGU_Q);
  bool LOXA = not(MYJY_Q);
  bool NEZE = not(NASA_Q);
  bool NOBO = not(NEFO_Q);

  wire LUXA = not(win.NYXU);
  bool SEJA = nand(TOSA, LUXA);
  bool SENO = nand(RUCO, LUXA);
  bool SURE = nand(TYCE, LUXA);
  bool SEBO = nand(REVY, LUXA);
  bool SUCA = nand(RYGA, LUXA);
  bool SYWE = nand(RYLE, LUXA);
  bool SUPU = nand(RAPU, LUXA);
  bool RYJY = nand(SOJA, LUXA);

  bool TUXE = nand(LUXA, !RAWU_Q);
  bool SOLY = nand(LUXA, !POZO_Q);
  bool RUCE = nand(LUXA, !PYZO_Q);
  bool RYJA = nand(LUXA, !POXA_Q);
  bool RUTO = nand(LUXA, !PULO_Q);
  bool RAJA = nand(LUXA, !POJU_Q);
  bool RAJO = nand(LUXA, !POWY_Q);
  bool RAGA = nand(LUXA, !PYJU_Q);

  wire LOZE = not(win.NYXU);
  bool LOTY = nand(LUHE, LOZE);
  bool NEXA = nand(NOLY, LOZE);
  bool LUTU = nand(LEKE, LOZE);
  bool LUJA = nand(LOMY, LOZE);
  bool MOSY = nand(LALA, LOZE);
  bool LERU = nand(LOXA, LOZE);
  bool NYHA = nand(NEZE, LOZE);
  bool NADY = nand(NOBO, LOZE);

  bool LAKY = nand(LOZE, LEGU_Q);
  bool NYXO = nand(LOZE, NUDU_Q);
  bool LOTO = nand(LOZE, MUKU_Q);
  bool LYDU = nand(LOZE, LUZO_Q);
  bool MYVY = nand(LOZE, MEGU_Q);
  bool LODO = nand(LOZE, MYJY_Q);
  bool NUTE = nand(LOZE, NASA_Q);
  bool NAJA = nand(LOZE, NEFO_Q);

  bool TOMY_Q = ppu.TOMY.srtock(clk.CLKPIPE, TUXE, SEJA, ext.P10_B);
  bool TACA_Q = ppu.TACA.srtock(clk.CLKPIPE, SOLY, SENO, TOMY_Q);
  bool SADY_Q = ppu.SADY.srtock(clk.CLKPIPE, RUCE, SURE, TACA_Q);
  bool RYSA_Q = ppu.RYSA.srtock(clk.CLKPIPE, RYJA, SEBO, SADY_Q);
  bool SOBO_Q = ppu.SOBO.srtock(clk.CLKPIPE, RUTO, SUCA, RYSA_Q);
  bool SETU_Q = ppu.SETU.srtock(clk.CLKPIPE, RAJA, SYWE, SOBO_Q);
  bool RALU_Q = ppu.RALU.srtock(clk.CLKPIPE, RAJO, SUPU, SETU_Q);
  bool SOHU_Q = ppu.SOHU.srtock(clk.CLKPIPE, RAGA, RYJY, RALU_Q);

  ppu.BG_PIX_B_7 = SOHU_Q;

  bool MYDE_Q = ppu.MYDE.srtock(clk.CLKPIPE, LAKY, LOTY, ext.P10_B);
  bool NOZO_Q = ppu.NOZO.srtock(clk.CLKPIPE, NYXO, NEXA, MYDE_Q);
  bool MOJU_Q = ppu.MOJU.srtock(clk.CLKPIPE, LOTO, LUTU, NOZO_Q);
  bool MACU_Q = ppu.MACU.srtock(clk.CLKPIPE, LYDU, LUJA, MOJU_Q);
  bool NEPO_Q = ppu.NEPO.srtock(clk.CLKPIPE, MYVY, MOSY, MACU_Q);
  bool MODU_Q = ppu.MODU.srtock(clk.CLKPIPE, LODO, LERU, NEPO_Q);
  bool NEDA_Q = ppu.NEDA.srtock(clk.CLKPIPE, NUTE, NYHA, MODU_Q);
  bool PYBO_Q = ppu.PYBO.srtock(clk.CLKPIPE, NAJA, NADY, NEDA_Q);

  ppu.BG_PIX_A_7 = PYBO_Q;
}