#include "../Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P25_VramInterface {

  struct Input {
    bool CPU_RD_SYNC;
    bool RESET6;
    bool T1nT2;

    bool MOE_IN;
    bool MWR_IN;
    bool MCS_IN;

    bool VRAM_TO_OAM;
    bool P10_B;
    bool MOPA_PHI;
    bool NET02;
    bool FF40_D4;
    bool FEXXFFXXn;

    bool LULA;
    bool LENA;
    bool TACU;
    bool TUVO;
    bool ACYL;
    bool XYSO;
    bool SARO;
    bool XYMU;
    bool LUMA;
    bool TEXO;
    bool TEXY;
    bool ABUZ;
    bool AFAS;
    bool MYMA;
    bool BEDO;
    bool RORU;
    bool NETA;
    bool POTU;
    bool PORE;
    bool XUHA;
    bool VYNO;
    bool VUJO;
    bool VYMU;
    bool LEKO;
    bool MATU_Q;

    bool A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15;
    bool D0,D1,D2,D3,D4,D5,D6,D7;

    bool D0_IN,D1_IN,D2_IN,D3_IN,D4_IN,D5_IN,D6_IN,D7_IN;

    bool MA0,MA1,MA2,MA3,MA4,MA5,MA6,MA7,MA8,MA9,MA10,MA11,MA12;
    bool MD0,MD1,MD2,MD3,MD4,MD5,MD6,MD7;

    bool MD0_IN,MD1_IN,MD2_IN,MD3_IN,MD4_IN,MD5_IN,MD6_IN,MD7_IN;

    // not sure, drives high bits of vram addr if NETA is high
    bool RAWU,POZO,PYZO,POXA,PULO,POJU,POWY,PYJU;

  };

  struct Output {
    bool MD_B;

    // internal data bus
    bool D_OE;
    bool D0,D1,D2,D3,D4,D5,D6,D7;

    // actual pins from ram?
    bool D0_A,D1_A,D2_A,D3_A,D4_A,D5_A,D6_A,D7_A;

    // internal vram addr bus
    bool MA_OE;
    bool MA0,MA1,MA2,MA3,MA4,MA5,MA6,MA7,MA8,MA9,MA10,MA11,MA12;

    // internal vram data bus
    bool MD_OE;
    bool MD0,MD1,MD2,MD3,MD4,MD5,MD6,MD7;

    // actual pins to vram
    bool MCS_A,MCS_D;
    bool MWR_A,MWR_D;
    bool MOE_A,MOE_D;

    // actual pins to vram
    bool MA0_OUT,MA1_OUT,MA2_OUT,MA3_OUT,MA4_OUT,MA5_OUT,MA6_OUT,MA7_OUT,MA8_OUT,MA9_OUT,MA10_OUT,MA11_OUT,MA12_OUT;
    bool MD0_OUT,MD1_OUT,MD2_OUT,MD3_OUT,MD4_OUT,MD5_OUT,MD6_OUT,MD7_OUT;

    // actual pins from vram
    bool MD0_A,MD1_A,MD2_A,MD3_A,MD4_A,MD5_A,MD6_A,MD7_A;

    // actual pins to oam
    bool OAM_D_OE;
    bool OAM_A_D0,OAM_A_D1,OAM_A_D2,OAM_A_D3,OAM_A_D4,OAM_A_D5,OAM_A_D6,OAM_A_D7;
    bool OAM_B_D0,OAM_B_D1,OAM_B_D2,OAM_B_D3,OAM_B_D4,OAM_B_D5,OAM_B_D6,OAM_B_D7;
  };

  // dunno, driven by T1nT2? that doesn't make much sense.
  reg SOTO;

  void tick(const Input& in, Output& out) {
    //----------
    // top left stack of inverters

    wire REHO = not(in.MA12);
    wire RUMA = not(in.MA11);
    wire RUKY = not(in.MA10);
    wire VODE = not(in.MA9);
    wire VOVA = not(in.MA8);
    wire MUME = not(in.MA7);
    wire MEWY = not(in.MA6);
    wire MYSA = not(in.MA5);
    wire MEPA = not(in.MA4);
    wire MAVU = not(in.MA3);
    wire MYRE = not(in.MA2);
    wire MASA = not(in.MA1);
    wire MYFU = not(in.MA0);

    wire PONY = not(REHO);
    wire PEDU = not(RUMA);
    wire NUVA = not(RUKY);
    wire TAGO = not(VODE);
    wire TUJY = not(VOVA);
    wire LUBY = not(MUME);
    wire LEFA = not(MEWY);
    wire LALO = not(MYSA);
    wire LOLY = not(MEPA);
    wire LUVO = not(MAVU);
    wire LACA = not(MYRE);
    wire LOZU = not(MASA);
    wire LEXE = not(MYFU);

    out.MA12_OUT = PONY;
    out.MA11_OUT = PEDU;
    out.MA10_OUT = NUVA;
    out.MA9_OUT = TAGO;
    out.MA8_OUT = TUJY;
    out.MA7_OUT = LUBY;
    out.MA6_OUT = LEFA;
    out.MA5_OUT = LALO;
    out.MA4_OUT = LOLY;
    out.MA3_OUT = LUVO;
    out.MA2_OUT = LACA;
    out.MA1_OUT = LOZU;
    out.MA0_OUT = LEXE;

    //----------
    // why is this writing to the _A part of the data bus but not the _D?

    wire RYVO = nand(in.D5, in.LULA);
    wire RERA = nand(in.D3, in.LULA);
    wire RABY = nand(in.D2, in.LULA);
    wire RORY = nand(in.D4, in.LULA);
    wire RUJA = nand(in.D1, in.LULA);
    wire RAVU = nand(in.D7, in.LULA);
    wire RAFY = nand(in.D6, in.LULA);
    wire RUXA = nand(in.D0, in.LULA);

    out.D5_A = RYVO;
    out.D3_A = RERA;
    out.D2_A = RABY;
    out.D4_A = RORY;
    out.D1_A = RUJA;
    out.D7_A = RAVU;
    out.D6_A = RAFY;
    out.D0_A = RUXA;

    //----------

    wire CUFE = unk3(in.SARO, in.MATU_Q, in.MOPA_PHI);
    wire VAPE = and(in.TACU, in.TUVO);
    wire AVER = and(in.ACYL, in.XYSO);
    wire XUJY = not(VAPE);
    wire BYCU = nor(CUFE, XUJY, AVER);
    wire COTA = not(BYCU);

    //----------

    wire SYRO = not(in.FEXXFFXXn);
    wire TEFA = nor(SYRO, in.TEXO);
    wire SOSE = and(in.A15, TEFA); // odd...
    wire TUCA = and(SOSE, in.ABUZ); // ABUZ is a clocky type signal from P01, but that doesn't make sense
    wire TUJA = and(SOSE, in.CPU_RD_SYNC);
    wire TEGU = and(SOSE, in.AFAS);
    wire TAVY = not(in.MOE_IN);
    wire SYCY = not(in.NET02);
    wire SOTO_Q = SOTO.flip(SYCY, in.RESET6);
    wire TUTO = and(in.NET02, !SOTO_Q);
    wire SUDO = not(in.MWR_IN);
    wire TEFY = not(in.MCS_IN);
    wire SALE = mux2(TUTO, TAVY, TEGU);
    wire TYJY = mux2(TUTO, SUDO, TUJA);
    wire TOLE = mux2(TUTO, TEFY, TUCA);

    wire ROPY = not(in.XYMU);
    wire SERE = and(TOLE, ROPY);
    wire RUVY = not(SALE);
    wire SOHY = nand(TYJY, SERE);
    wire SAZO = and(RUVY, SERE);
    wire RYJE = not(SAZO);
    wire REVO = not(RYJE);
    wire RELA = or(REVO, SAZO);
    wire ROCY = and(SAZO, REVO);
    wire RENA = not(RELA);
    wire ROFA = not(RENA);
    wire RAHU = not(ROCY);

    out.MD_B = ROFA; // something about vram pins...

    //----------

    wire RYLU = nand(SALE, ROPY);
    wire SOHO = and(in.TACU, in.TEXY);
    wire RAWA = not(SOHO);
    wire APAM = not(in.VRAM_TO_OAM);
    wire SUTU = nor(in.LENA, in.VRAM_TO_OAM, in.TEXY, SERE);
    wire RACU = and(RYLU, RAWA, in.MYMA, APAM);
    wire RACO = not(TUTO);
    wire RUTE = or(TUTO, RACO); // wat? or of a signal with its own inverse...
    wire SEWO = or(TUTO, SUTU);
    wire TODE = and(SUTU, RACO);
    wire SEMA = and(RACU, RACO);
    wire TAXY = and(SOHY, RACO);
    wire SOFY = or(TUTO, SOHY);

    wire SAHA = not(RUTE);
    wire SETY = not(SEWO);
    wire SOKY = not(TODE);
    wire REFO = not(SEMA);
    wire SYSY = not(TAXY);
    wire RAGU = not(SOFY);

    out.MCS_A = SOKY;
    out.MCS_D = SETY;
    out.MWR_A = SYSY;
    out.MWR_D = RAGU;
    out.MOE_A = REFO;
    out.MOE_D = SAHA;

    //----------

    wire RAKU = in.MD7_IN;
    wire ROCE = in.MD4_IN;
    wire REMO = in.MD3_IN;
    wire ROPU = in.MD5_IN;
    wire RETA = in.MD6_IN;
    wire RYDO = in.MD2_IN;
    wire RODY = in.MD0_IN;
    wire REBA = in.MD1_IN;

    if (RENA) {
      out.MD_OE = true;
      out.MD7 = RAKU;
      out.MD4 = ROCE;
      out.MD3 = REMO;
      out.MD5 = ROPU;
      out.MD6 = RETA;
      out.MD2 = RYDO;
      out.MD0 = RODY;
      out.MD1 = REBA;
    }

    //----------

    wire TUSO = nor(in.NET02, in.BEDO);
    wire SOLE = not(TUSO);
    wire RUNY = not(in.P10_B);

    wire TAHY = not(SOLE);
    wire TESU = not(SOLE);
    wire TAXO = not(SOLE);
    wire TOVU = not(SOLE);
    wire TAZU = not(SOLE);
    wire TEWA = not(SOLE);
    wire SOSA = not(SOLE);
    wire SEDU = not(SOLE);

    if (RUNY) {
      out.D_OE;
      out.D4 = TAHY;
      out.D5 = TESU;
      out.D3 = TAXO;
      out.D0 = TOVU;
      out.D6 = TAZU;
      out.D7 = TEWA;
      out.D1 = SOSA;
      out.D2 = SEDU;
    }

    //----------
    // FIXME
    // this section is slightly odd as we both write to and read from the MDX bus
    // should probably read bus into locals, do logic, write locals to output

    wire TEME = not(in.D0);
    wire TEWU = not(in.D1);
    wire TYGO = not(in.D2);
    wire SOTE = not(in.D3);
    wire SEKE = not(in.D4);
    wire RUJO = not(in.D5);
    wire TOFA = not(in.D6);
    wire SUZA = not(in.D7);

    if (RAHU) {
      out.MD_OE;
      out.MD0 = TEME;
      out.MD1 = TEWU;
      out.MD2 = TYGO;
      out.MD3 = SOTE;
      out.MD4 = SEKE;
      out.MD5 = RUJO;
      out.MD6 = TOFA;
      out.MD7 = SUZA;
    }

    wire SYNU = or(RAHU, in.MD0);
    wire SYMA = or(RAHU, in.MD1);
    wire ROKO = or(RAHU, in.MD2);
    wire SYBU = or(RAHU, in.MD3);
    wire SAKO = or(RAHU, in.MD4);
    wire SEJY = or(RAHU, in.MD5);
    wire SEDO = or(RAHU, in.MD6);
    wire SAWU = or(RAHU, in.MD7);

    wire RURA = not(SYNU);
    wire RULY = not(SYMA);
    wire RARE = not(ROKO);
    wire RODU = not(SYBU);
    wire RUBE = not(SAKO);
    wire RUMU = not(SEJY);
    wire RYTY = not(SEDO);
    wire RADY = not(SAWU);

    out.MD0_OUT = RURA;
    out.MD1_OUT = RULY;
    out.MD2_OUT = RARE;
    out.MD3_OUT = RODU;
    out.MD4_OUT = RUBE;
    out.MD5_OUT = RUMU;
    out.MD6_OUT = RYTY;
    out.MD7_OUT = RADY;

    //----------

    wire XANE = nor(in.VRAM_TO_OAM, in.XYMU);
    wire XEDU = not(XANE);

    wire XECA = in.A4;
    wire XYBO = in.A7;
    wire RYSU = in.A8;
    wire RUSE = in.A10;
    wire RUMO = in.A12;
    wire XYNE = in.A2;
    wire XOBA = in.A5;
    wire XODY = in.A3;
    wire RYNA = in.A11;
    wire RESE = in.A9;
    wire XAKY = in.A0;
    wire XOPO = in.A6;
    wire XUXU = in.A1;

    if (XEDU) {
      out.MA_OE = true;
      out.MA4 = XECA;
      out.MA7 = XYBO;
      out.MA8 = RYSU;
      out.MA10 = RUSE;
      out.MA12 = RUMO;
      out.MA2 = XYNE;
      out.MA5 = XOBA;
      out.MA3 = XODY;
      out.MA11 = RYNA;
      out.MA9 = RESE;
      out.MA0 = XAKY;
      out.MA6 = XOPO;
      out.MA1 = XUXU;
    }

    //----------

    wire LYRA = nand(in.T1nT2, in.RORU);

    wire RYBA = not(in.D7_IN);
    wire RUZY = not(in.D1_IN);
    wire ROME = not(in.D2_IN);
    wire TEHE = not(in.D4_IN);
    wire SOCA = not(in.D6_IN);
    wire RATU = not(in.D5_IN);
    wire TOVO = not(in.D0_IN);
    wire SAZA = not(in.D3_IN);

    wire ROPA = not(RYBA);
    wire SYWA = not(RUZY);
    wire SUGU = not(ROME);
    wire TUTE = not(TEHE);
    wire TEMY = not(SOCA);
    wire SAJO = not(RATU);
    wire TUTY = not(TOVO);
    wire TAWO = not(SAZA);

    if (LYRA) {
      out.D_OE = true;
      out.D7 = ROPA;
      out.D1 = SYWA;
      out.D2 = SUGU;
      out.D4 = TUTE;
      out.D6 = TEMY;
      out.D5 = SAJO;
      out.D0 = TUTY;
      out.D3 = TAWO;
    }

    //----------

    wire ROVE = not(RAHU);

    wire SEFA = and(in.MD0, ROVE);
    wire SUNA = and(in.MD3, ROVE);
    wire SUMO = and(in.MD4, ROVE);
    wire SUKE = and(in.MD7, ROVE);
    wire SAMO = and(in.MD6, ROVE);
    wire SOGO = and(in.MD1, ROVE);
    wire SAZU = and(in.MD5, ROVE);
    wire SEFU = and(in.MD2, ROVE);

    wire REGE = not(SEFA);
    wire RADA = not(SUNA);
    wire RYRO = not(SUMO);
    wire RYZE = not(SUKE);
    wire REKU = not(SAMO);
    wire RYKY = not(SOGO);
    wire REVU = not(SAZU);
    wire RAZO = not(SEFU);

    out.MD0_A = REGE;
    out.MD3_A = RADA;
    out.MD4_A = RYRO;
    out.MD7_A = RYZE;
    out.MD6_A = REKU;
    out.MD1_A = RYKY;
    out.MD5_A = REVU;
    out.MD2_A = RAZO;

    //----------

    wire CEDE = not(in.LUMA);
    wire SYZO = not(in.D7_IN);
    wire TUNE = not(in.D1_IN);
    wire SERA = not(in.D2_IN);
    wire SYSA = not(in.D4_IN);
    wire TUBE = not(in.D6_IN);
    wire SUGY = not(in.D5_IN);
    wire RALO = not(in.D0_IN);
    wire TENU = not(in.D3_IN);

    wire BAPE = not(SYZO);
    wire BYPY = not(SYZO);
    wire BOMO = not(TUNE);
    wire BUBO = not(TUNE);
    wire BASA = not(SERA);
    wire BETU = not(SERA);
    wire BUMA = not(SYSA);
    wire BAXU = not(SYSA);
    wire BASY = not(TUBE);
    wire BYNY = not(TUBE);
    wire BUPY = not(SUGY);
    wire BUHU = not(SUGY);
    wire WASA = not(RALO);
    wire WEJO = not(RALO);
    wire CAKO = not(TENU);
    wire CYME = not(TENU);

    if (CEDE) {
      out.OAM_D_OE = true;
      out.OAM_B_D7 = BAPE;
      out.OAM_A_D7 = BYPY;
      out.OAM_B_D1 = BOMO;
      out.OAM_A_D1 = BUBO;
      out.OAM_B_D2 = BASA;
      out.OAM_A_D2 = BETU;
      out.OAM_B_D4 = BUMA;
      out.OAM_A_D4 = BAXU;
      out.OAM_B_D6 = BASY;
      out.OAM_A_D6 = BYNY;
      out.OAM_B_D5 = BUPY;
      out.OAM_A_D5 = BUHU;
      out.OAM_B_D0 = WASA;
      out.OAM_A_D0 = WEJO;
      out.OAM_B_D3 = CAKO;
      out.OAM_A_D3 = CYME;
    }

    //----------

    wire TYVY = nand(SERE, in.LEKO);
    wire SEBY = not(TYVY);

    wire RORO = not(in.MD5);
    wire RERY = not(in.MD0);
    wire RONA = not(in.MD2);
    wire RUNA = not(in.MD1);
    wire RUNO = not(in.MD3);
    wire SAME = not(in.MD7);
    wire SANA = not(in.MD4);
    wire RABO = not(in.MD6);

    wire REXU = not(RORO);
    wire RUGA = not(RERY);
    wire RYBU = not(RONA);
    wire ROTA = not(RUNA);
    wire RAJU = not(RUNO);
    wire TOKU = not(SAME);
    wire TYJA = not(SANA);
    wire RUPY = not(RABO);

    if (SEBY) {
      out.D_OE = true;
      out.D5 = REXU;
      out.D0 = RUGA;
      out.D2 = RYBU;
      out.D1 = ROTA;
      out.D3 = RAJU;
      out.D7 = TOKU;
      out.D4 = TYJA;
      out.D6 = RUPY;
    }

    //----------

    wire XUCY = nand(in.NETA, in.PORE);
    wire XEZE = nand(in.POTU, in.PORE);
    wire WUKO = not(XEZE);

    //----------

    wire XONU = not(in.XUHA);
    wire WUDO = not(in.VYNO);
    wire WAWE = not(in.VUJO);
    wire WOLU = not(in.VYMU);

    if (XUCY) {
      out.MA_OE = true;
      out.MA0 = XONU;
      out.MA1 = WUDO;
      out.MA2 = WAWE;
      out.MA3 = WOLU;
    }

    //----------

    wire VUZA = nor(in.FF40_D4, in.PYJU);
    wire VURY = not(VUZA);

    wire TOBO = not(in.PYJU);
    wire SUVO = not(in.POWY);
    wire RESO = not(in.POJU);
    wire ROHA = not(in.PULO);
    wire RUSA = not(in.POXA);
    wire VEJY = not(in.PYZO);
    wire SEZU = not(in.POZO);
    wire VAPY = not(in.RAWU);

    if (in.NETA) {
      out.MA_OE = true;
      out.MA12 = VURY;
      out.MA11 = TOBO;
      out.MA10 = SUVO;
      out.MA9 = RESO;
      out.MA8 = ROHA;
      out.MA7 = RUSA;
      out.MA6 = VEJY;
      out.MA5 = SEZU;
      out.MA4 = VAPY;
    }
  }
};