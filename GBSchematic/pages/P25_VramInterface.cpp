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

    bool A00,A01,A02,A03,A04,A05,A06,A07,A08,A09,A10,A11,A12,A13,A14,A15;
    bool D0,D1,D2,D3,D4,D5,D6,D7;

    bool D0_C,D1_C,D2_C,D3_C,D4_C,D5_C,D6_C,D7_C;

    bool MA00,MA01,MA02,MA03,MA04,MA05,MA06,MA07,MA08,MA09,MA10,MA11,MA12;
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
    bool MA00,MA01,MA02,MA03,MA04,MA05,MA06,MA07,MA08,MA09,MA10,MA11,MA12;

    // internal vram data bus
    bool MD_OE;
    bool MD0,MD1,MD2,MD3,MD4,MD5,MD6,MD7;

    // actual pins to vram
    bool MCS_A,MCS_D;
    bool MWR_A,MWR_D;
    bool MOE_A,MOE_D;

    // actual pins to vram
    bool MA00_OUT,MA01_OUT,MA02_OUT,MA03_OUT,MA04_OUT,MA05_OUT,MA06_OUT,MA07_OUT,MA08_OUT,MA09_OUT,MA10_OUT,MA11_OUT,MA12_OUT;
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

    wire MYFU_00 = not(in.MA00);
    wire MASA_01 = not(in.MA01);
    wire MYRE_02 = not(in.MA02);
    wire MAVU_03 = not(in.MA03);
    wire MEPA_04 = not(in.MA04);
    wire MYSA_05 = not(in.MA05);
    wire MEWY_06 = not(in.MA06);
    wire MUME_07 = not(in.MA07);
    wire VOVA_08 = not(in.MA08);
    wire VODE_09 = not(in.MA09);
    wire RUKY_10 = not(in.MA10);
    wire RUMA_11 = not(in.MA11);
    wire REHO_12 = not(in.MA12);

    wire LEXE_00 = not(MYFU_00);
    wire LOZU_01 = not(MASA_01);
    wire LACA_02 = not(MYRE_02);
    wire LUVO_03 = not(MAVU_03);
    wire LOLY_04 = not(MEPA_04);
    wire LALO_05 = not(MYSA_05);
    wire LEFA_06 = not(MEWY_06);
    wire LUBY_07 = not(MUME_07);
    wire TUJY_08 = not(VOVA_08);
    wire TAGO_09 = not(VODE_09);
    wire NUVA_10 = not(RUKY_10);
    wire PEDU_11 = not(RUMA_11);
    wire PONY_12 = not(REHO_12);

    out.MA00_OUT = LEXE_00;
    out.MA01_OUT = LOZU_01;
    out.MA02_OUT = LACA_02;
    out.MA03_OUT = LUVO_03;
    out.MA04_OUT = LOLY_04;
    out.MA05_OUT = LALO_05;
    out.MA06_OUT = LEFA_06;
    out.MA07_OUT = LUBY_07;
    out.MA08_OUT = TUJY_08;
    out.MA09_OUT = TAGO_09;
    out.MA10_OUT = NUVA_10;
    out.MA11_OUT = PEDU_11;
    out.MA12_OUT = PONY_12;

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
    wire TEGU = nand(SOSE, in.AFAS);
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

    wire RODY_00 = in.MD0_IN;
    wire REBA_01 = in.MD1_IN;
    wire RYDO_02 = in.MD2_IN;
    wire REMO_03 = in.MD3_IN;
    wire ROCE_04 = in.MD4_IN;
    wire ROPU_05 = in.MD5_IN;
    wire RETA_06 = in.MD6_IN;
    wire RAKU_07 = in.MD7_IN;

    if (RENA) {
      out.MD0 = RODY_00;
      out.MD1 = REBA_01;
      out.MD2 = RYDO_02;
      out.MD3 = REMO_03;
      out.MD4 = ROCE_04;
      out.MD5 = ROPU_05;
      out.MD6 = RETA_06;
      out.MD7 = RAKU_07;
    }

    //----------

    wire TUSO = nor(in.NET02, in.BEDO);
    wire SOLE = not(TUSO);
    wire RUNY = not(in.P10_B);

    wire TOVU_00 = not(SOLE);
    wire SOSA_01 = not(SOLE);
    wire SEDU_02 = not(SOLE);
    wire TAXO_03 = not(SOLE);
    wire TAHY_04 = not(SOLE);
    wire TESU_05 = not(SOLE);
    wire TAZU_06 = not(SOLE);
    wire TEWA_07 = not(SOLE);

    if (RUNY) {
      out.D0 = TOVU_00;
      out.D1 = SOSA_01;
      out.D2 = SEDU_02;
      out.D3 = TAXO_03;
      out.D4 = TAHY_04;
      out.D5 = TESU_05;
      out.D6 = TAZU_06;
      out.D7 = TEWA_07;
    }

    //----------

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

    wire XAKY = in.A00;
    wire XUXU = in.A01;
    wire XYNE = in.A02;
    wire XODY = in.A03;
    wire XECA = in.A04;
    wire XOBA = in.A05;
    wire XOPO = in.A06;
    wire XYBO = in.A07;
    wire RYSU = in.A08;
    wire RESE = in.A09;
    wire RUSE = in.A10;
    wire RYNA = in.A11;
    wire RUMO = in.A12;

    if (XEDU) {
      out.MA00 = XAKY;
      out.MA01 = XUXU;
      out.MA02 = XYNE;
      out.MA03 = XODY;
      out.MA04 = XECA;
      out.MA05 = XOBA;
      out.MA06 = XOPO;
      out.MA07 = XYBO;
      out.MA08 = RYSU;
      out.MA09 = RESE;
      out.MA10 = RUSE;
      out.MA11 = RYNA;
      out.MA12 = RUMO;
    }

    //----------

    wire ROVE = not(RAHU);

    wire SEFA_00 = and(in.MD0, ROVE);
    wire SOGO_01 = and(in.MD1, ROVE);
    wire SEFU_02 = and(in.MD2, ROVE);
    wire SUNA_03 = and(in.MD3, ROVE);
    wire SUMO_04 = and(in.MD4, ROVE);
    wire SAZU_05 = and(in.MD5, ROVE);
    wire SAMO_06 = and(in.MD6, ROVE);
    wire SUKE_07 = and(in.MD7, ROVE);

    wire REGE_00 = not(SEFA_00);
    wire RYKY_01 = not(SOGO_01);
    wire RAZO_02 = not(SEFU_02);
    wire RADA_03 = not(SUNA_03);
    wire RYRO_04 = not(SUMO_04);
    wire REVU_05 = not(SAZU_05);
    wire REKU_06 = not(SAMO_06);
    wire RYZE_07 = not(SUKE_07);
    
    out.MD0_A = REGE_00;
    out.MD1_A = RYKY_01;
    out.MD2_A = RAZO_02;
    out.MD3_A = RADA_03;
    out.MD4_A = RYRO_04;
    out.MD5_A = REVU_05;
    out.MD6_A = REKU_06;
    out.MD7_A = RYZE_07;

    //----------

    wire CEDE = not(in.LUMA);

    wire RALO_00 = not(in.D0_C);
    wire TUNE_01 = not(in.D1_C);
    wire SERA_02 = not(in.D2_C);
    wire TENU_03 = not(in.D3_C);
    wire SYSA_04 = not(in.D4_C);
    wire SUGY_05 = not(in.D5_C);
    wire TUBE_06 = not(in.D6_C);
    wire SYZO_07 = not(in.D7_C);

    wire WEJO_00 = not(RALO_00);
    wire BUBO_01 = not(TUNE_01);
    wire BETU_02 = not(SERA_02);
    wire CYME_03 = not(TENU_03);
    wire BAXU_04 = not(SYSA_04);
    wire BUHU_05 = not(SUGY_05);
    wire BYNY_06 = not(TUBE_06);
    wire BYPY_07 = not(SYZO_07);

    wire WASA_00 = not(RALO_00);
    wire BOMO_01 = not(TUNE_01);
    wire BASA_02 = not(SERA_02);
    wire CAKO_03 = not(TENU_03);
    wire BUMA_04 = not(SYSA_04);
    wire BUPY_05 = not(SUGY_05);
    wire BASY_06 = not(TUBE_06);
    wire BAPE_07 = not(SYZO_07);

    if (CEDE) {
      out.OAM_A_D0 = WEJO_00;
      out.OAM_A_D1 = BUBO_01;
      out.OAM_A_D2 = BETU_02;
      out.OAM_A_D3 = CYME_03;
      out.OAM_A_D4 = BAXU_04;
      out.OAM_A_D5 = BUHU_05;
      out.OAM_A_D6 = BYNY_06;
      out.OAM_A_D7 = BYPY_07;

      out.OAM_B_D0 = WASA_00;
      out.OAM_B_D1 = BOMO_01;
      out.OAM_B_D2 = BASA_02;
      out.OAM_B_D3 = CAKO_03;
      out.OAM_B_D4 = BUMA_04;
      out.OAM_B_D5 = BUPY_05;
      out.OAM_B_D6 = BASY_06;
      out.OAM_B_D7 = BAPE_07;
    }

    //----------

    wire TYVY = nand(SERE, in.LEKO);
    wire SEBY = not(TYVY);

    wire RERY_00 = not(in.MD0);
    wire RUNA_01 = not(in.MD1);
    wire RONA_02 = not(in.MD2);
    wire RUNO_03 = not(in.MD3);
    wire SANA_04 = not(in.MD4);
    wire RORO_05 = not(in.MD5);
    wire RABO_06 = not(in.MD6);
    wire SAME_07 = not(in.MD7);

    wire RUGA_00 = not(RERY_00);
    wire ROTA_01 = not(RUNA_01);
    wire RYBU_02 = not(RONA_02);
    wire RAJU_03 = not(RUNO_03);
    wire TYJA_04 = not(SANA_04);
    wire REXU_05 = not(RORO_05);
    wire RUPY_06 = not(RABO_06);
    wire TOKU_07 = not(SAME_07);

    if (SEBY) {
      out.D0 = RUGA_00;
      out.D1 = ROTA_01;
      out.D2 = RYBU_02;
      out.D3 = RAJU_03;
      out.D4 = TYJA_04;
      out.D5 = REXU_05;
      out.D6 = RUPY_06;
      out.D7 = TOKU_07;
    }

    //----------

    wire XUCY = nand(in.NETA, in.PORE);
    wire XEZE = nand(in.POTU, in.PORE);
    wire WUKO = not(XEZE);

    //----------

    wire XONU_00 = not(in.XUHA);
    wire WUDO_01 = not(in.VYNO);
    wire WAWE_02 = not(in.VUJO);
    wire WOLU_03 = not(in.VYMU);

    if (XUCY) {
      out.MA00 = XONU_00;
      out.MA01 = WUDO_01;
      out.MA02 = WAWE_02;
      out.MA03 = WOLU_03;
    }

    //----------

    wire VAPY_04 = not(in.RAWU);
    wire SEZU_05 = not(in.POZO);
    wire VEJY_06 = not(in.PYZO);
    wire RUSA_07 = not(in.POXA);
    wire ROHA_08 = not(in.PULO);
    wire RESO_09 = not(in.POJU);
    wire SUVO_10 = not(in.POWY);
    wire TOBO_11 = not(in.PYJU);

    wire VUZA = nor(in.FF40_D4, in.PYJU);
    wire VURY_12 = not(VUZA);

    if (in.NETA) {
      out.MA04 = VAPY_04;
      out.MA05 = SEZU_05;
      out.MA06 = VEJY_06;
      out.MA07 = RUSA_07;
      out.MA08 = ROHA_08;
      out.MA09 = RESO_09;
      out.MA10 = SUVO_10;
      out.MA11 = TOBO_11;
      out.MA12 = VURY_12;
    }
  }
};