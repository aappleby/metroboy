#include "P25_VramInterface.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P25_VramInterface::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // top left stack of inverters

  c.p25.MYFU_00 = not(b.chip.MA00);
  c.p25.MASA_01 = not(b.chip.MA01);
  c.p25.MYRE_02 = not(b.chip.MA02);
  c.p25.MAVU_03 = not(b.chip.MA03);
  c.p25.MEPA_04 = not(b.chip.MA04);
  c.p25.MYSA_05 = not(b.chip.MA05);
  c.p25.MEWY_06 = not(b.chip.MA06);
  c.p25.MUME_07 = not(b.chip.MA07);
  c.p25.VOVA_08 = not(b.chip.MA08);
  c.p25.VODE_09 = not(b.chip.MA09);
  c.p25.RUKY_10 = not(b.chip.MA10);
  c.p25.RUMA_11 = not(b.chip.MA11);
  c.p25.REHO_12 = not(b.chip.MA12);

  c.p25.LEXE_00 = not(c.p25.MYFU_00);
  c.p25.LOZU_01 = not(c.p25.MASA_01);
  c.p25.LACA_02 = not(c.p25.MYRE_02);
  c.p25.LUVO_03 = not(c.p25.MAVU_03);
  c.p25.LOLY_04 = not(c.p25.MEPA_04);
  c.p25.LALO_05 = not(c.p25.MYSA_05);
  c.p25.LEFA_06 = not(c.p25.MEWY_06);
  c.p25.LUBY_07 = not(c.p25.MUME_07);
  c.p25.TUJY_08 = not(c.p25.VOVA_08);
  c.p25.TAGO_09 = not(c.p25.VODE_09);
  c.p25.NUVA_10 = not(c.p25.RUKY_10);
  c.p25.PEDU_11 = not(c.p25.RUMA_11);
  c.p25.PONY_12 = not(c.p25.REHO_12);

  c.chip.MA00 = c.p25.LEXE_00;
  c.chip.MA01 = c.p25.LOZU_01;
  c.chip.MA02 = c.p25.LACA_02;
  c.chip.MA03 = c.p25.LUVO_03;
  c.chip.MA04 = c.p25.LOLY_04;
  c.chip.MA05 = c.p25.LALO_05;
  c.chip.MA06 = c.p25.LEFA_06;
  c.chip.MA07 = c.p25.LUBY_07;
  c.chip.MA08 = c.p25.TUJY_08;
  c.chip.MA09 = c.p25.TAGO_09;
  c.chip.MA10 = c.p25.NUVA_10;
  c.chip.MA11 = c.p25.PEDU_11;
  c.chip.MA12 = c.p25.PONY_12;

  //----------

  c.p25.RYVO = nand(b.D5, b.sys.DBUS_OUT);
  c.p25.RERA = nand(b.D5, b.sys.DBUS_OUT);
  c.p25.RABY = nand(b.D5, b.sys.DBUS_OUT);
  c.p25.RORY = nand(b.D5, b.sys.DBUS_OUT);
  c.p25.RUJA = nand(b.D5, b.sys.DBUS_OUT);
  c.p25.RAVU = nand(b.D5, b.sys.DBUS_OUT);
  c.p25.RAFY = nand(b.D5, b.sys.DBUS_OUT);
  c.p25.RUXA = nand(b.D5, b.sys.DBUS_OUT);

  //----------

  c.p25.CUFE = unk3(b.sys.ADDR_OAM, b.sys.DMA_RUNNING_SYNC, b.sys.CPUCLK_ABCDxxxx9);
  c.p25.VAPE = and(b.p29.TACU, b.p29.TUVO);
  c.p25.AVER = and(b.p28.ACYL, b.p29.XYSO);
  c.p25.XUJY = not(b.p25.VAPE);
  c.p25.BYCU = nor(b.p25.CUFE, b.p25.XUJY, b.p25.AVER);
  c.p25.COTA = not(b.p25.BYCU);

  //----------

  c.p25.ADDR_FE00_FFFF = not(b.sys.ADDR_0000_FE00);
  c.p25.TEFA = nor(b.p25.ADDR_FE00_FFFF, b.sys.ADDR_VALID_AND_NOT_VRAM);
  c.p25.SOSE = and(b.A15, b.p25.TEFA); // odd...
  c.p25.TUCA = and(b.p25.SOSE, b.sys.CPU_RD_SYNC);
  c.p25.TUJA = and(b.p25.SOSE, b.sys.CPU_WR_SYNC);

  // what is this doing to the clock?
  c.p25.TEGU = nand(b.p25.SOSE, b.sys.CLK_xxxDxxxx1);

  c.p25.TAVY = not(b.chip.MOE_C);
  c.p25.SYCY = not(b.sys.MODE_DBG2);
  c.p25.SOTO = tock_pos(a.p25.SYCY, b.p25.SYCY, b.sys.SYS_RESETn2, b.p25.SOTO, !b.p25.SOTO);
  c.p25.TUTO = and(b.sys.MODE_DBG2, !b.p25.SOTO);
  c.p25.SUDO = not(b.chip.MWR_C);
  c.p25.TEFY = not(b.chip.MCS_C);
  c.p25.SALE = mux2(b.p25.TUTO, b.p25.TAVY, b.p25.TEGU);
  c.p25.TYJY = mux2(b.p25.TUTO, b.p25.SUDO, b.p25.TUJA);
  c.p25.TOLE = mux2(b.p25.TUTO, b.p25.TEFY, b.p25.TUCA);

  c.p25.ROPY = not(b.p21.XYMU);
  c.p25.SERE = and(b.p25.TOLE, b.p25.ROPY);
  c.p25.RUVY = not(b.p25.SALE);
  c.p25.SOHY = nand(b.p25.TYJY, b.p25.SERE);
  c.p25.SAZO = and(b.p25.RUVY, b.p25.SERE);
  c.p25.RYJE = not(b.p25.SAZO);
  c.p25.REVO = not(b.p25.RYJE);
  c.p25.RELA = or(b.p25.REVO, b.p25.SAZO);
  c.p25.ROCY = and(b.p25.SAZO, b.p25.REVO);
  c.p25.RENA = not(b.p25.RELA);
  c.p25.ROFA = not(b.p25.RENA);
  c.p25.RAHU = not(b.p25.ROCY);

  c.chip.MD0_B = b.p25.ROFA;
  c.chip.MD1_B = b.p25.ROFA;
  c.chip.MD2_B = b.p25.ROFA;
  c.chip.MD3_B = b.p25.ROFA;
  c.chip.MD4_B = b.p25.ROFA;
  c.chip.MD5_B = b.p25.ROFA;
  c.chip.MD6_B = b.p25.ROFA;
  c.chip.MD7_B = b.p25.ROFA;

  c.p25.RYLU = nand(b.p25.SALE, b.p25.ROPY);
  c.p25.SOHO = and(b.p29.TACU, b.p29.TEXY);
  c.p25.RAWA = not(b.p25.SOHO);
  c.p25.APAM = not(b.sys.VRAM_TO_OAMn);
  c.p25.SUTU = nor(b.p27.LENA, b.sys.VRAM_TO_OAMn, b.p29.TEXY, b.p25.SERE);
  c.p25.RACU = and(b.p25.RYLU, b.p25.RAWA, b.p27.MYMA, b.p25.APAM);
  c.p25.RACO = not(b.p25.TUTO);
  c.p25.RUTE = or(b.p25.TUTO, b.p25.RACO); // wat? or of a signal with its own inverse...
  c.p25.SEWO = or(b.p25.TUTO, b.p25.SUTU);
  c.p25.TODE = and(b.p25.SUTU, b.p25.RACO);
  c.p25.SEMA = and(b.p25.RACU, b.p25.RACO);
  c.p25.TAXY = and(b.p25.SOHY, b.p25.RACO);
  c.p25.SOFY = or(b.p25.TUTO, b.p25.SOHY);

  c.p25.SAHA = not(b.p25.RUTE);
  c.p25.SETY = not(b.p25.SEWO);
  c.p25.SOKY = not(b.p25.TODE);
  c.p25.REFO = not(b.p25.SEMA);
  c.p25.SYSY = not(b.p25.TAXY);
  c.p25.RAGU = not(b.p25.SOFY);

  c.chip.MCS_A = b.p25.SOKY;
  c.chip.MCS_D = b.p25.SETY;
  c.chip.MWR_A = b.p25.SYSY;
  c.chip.MWR_D = b.p25.RAGU;
  c.chip.MOE_A = b.p25.REFO;
  c.chip.MOE_D = b.p25.SAHA;

  //----------

  c.p25.RODY_00 = b.chip.MD0_C;
  c.p25.REBA_01 = b.chip.MD1_C;
  c.p25.RYDO_02 = b.chip.MD2_C;
  c.p25.REMO_03 = b.chip.MD3_C;
  c.p25.ROCE_04 = b.chip.MD4_C;
  c.p25.ROPU_05 = b.chip.MD5_C;
  c.p25.RETA_06 = b.chip.MD6_C;
  c.p25.RAKU_07 = b.chip.MD7_C;

  if (b.p25.RENA) {
    c.MD0 = b.p25.RODY_00;
    c.MD1 = b.p25.REBA_01;
    c.MD2 = b.p25.RYDO_02;
    c.MD3 = b.p25.REMO_03;
    c.MD4 = b.p25.ROCE_04;
    c.MD5 = b.p25.ROPU_05;
    c.MD6 = b.p25.RETA_06;
    c.MD7 = b.p25.RAKU_07;
  }

  //----------
  // more debug stuff

  c.p25.TUSO = nor(b.sys.MODE_DBG2, b.sys.CPUCLK_xxxxxFGH2);
  c.p25.SOLE = not(b.p25.TUSO);
  c.p25.RUNY = not(b.chip.P10_B);

  c.p25.TOVU_00 = not(b.p25.SOLE);
  c.p25.SOSA_01 = not(b.p25.SOLE);
  c.p25.SEDU_02 = not(b.p25.SOLE);
  c.p25.TAXO_03 = not(b.p25.SOLE);
  c.p25.TAHY_04 = not(b.p25.SOLE);
  c.p25.TESU_05 = not(b.p25.SOLE);
  c.p25.TAZU_06 = not(b.p25.SOLE);
  c.p25.TEWA_07 = not(b.p25.SOLE);

  if (b.p25.RUNY) {
    c.D0 = b.p25.TOVU_00;
    c.D1 = b.p25.SOSA_01;
    c.D2 = b.p25.SEDU_02;
    c.D3 = b.p25.TAXO_03;
    c.D4 = b.p25.TAHY_04;
    c.D5 = b.p25.TESU_05;
    c.D6 = b.p25.TAZU_06;
    c.D7 = b.p25.TEWA_07;
  }

  //----------

  c.p25.TEME = not(b.D0);
  c.p25.TEWU = not(b.D1);
  c.p25.TYGO = not(b.D2);
  c.p25.SOTE = not(b.D3);
  c.p25.SEKE = not(b.D4);
  c.p25.RUJO = not(b.D5);
  c.p25.TOFA = not(b.D6);
  c.p25.SUZA = not(b.D7);

  if (b.p25.RAHU) {
    c.MD0 = b.p25.TEME;
    c.MD1 = b.p25.TEWU;
    c.MD2 = b.p25.TYGO;
    c.MD3 = b.p25.SOTE;
    c.MD4 = b.p25.SEKE;
    c.MD5 = b.p25.RUJO;
    c.MD6 = b.p25.TOFA;
    c.MD7 = b.p25.SUZA;
  }

  c.p25.SYNU = or(b.p25.RAHU, b.MD0);
  c.p25.SYMA = or(b.p25.RAHU, b.MD1);
  c.p25.ROKO = or(b.p25.RAHU, b.MD2);
  c.p25.SYBU = or(b.p25.RAHU, b.MD3);
  c.p25.SAKO = or(b.p25.RAHU, b.MD4);
  c.p25.SEJY = or(b.p25.RAHU, b.MD5);
  c.p25.SEDO = or(b.p25.RAHU, b.MD6);
  c.p25.SAWU = or(b.p25.RAHU, b.MD7);

  c.p25.RURA = not(b.p25.SYNU);
  c.p25.RULY = not(b.p25.SYMA);
  c.p25.RARE = not(b.p25.ROKO);
  c.p25.RODU = not(b.p25.SYBU);
  c.p25.RUBE = not(b.p25.SAKO);
  c.p25.RUMU = not(b.p25.SEJY);
  c.p25.RYTY = not(b.p25.SEDO);
  c.p25.RADY = not(b.p25.SAWU);

  c.chip.MD0_D = b.p25.RURA;
  c.chip.MD1_D = b.p25.RULY;
  c.chip.MD2_D = b.p25.RARE;
  c.chip.MD3_D = b.p25.RODU;
  c.chip.MD4_D = b.p25.RUBE;
  c.chip.MD5_D = b.p25.RUMU;
  c.chip.MD6_D = b.p25.RYTY;
  c.chip.MD7_D = b.p25.RADY;

  //----------

  c.p25.XANE = nor(b.sys.VRAM_TO_OAMn, b.p21.XYMU);
  c.p25.XEDU = not(b.p25.XANE);

  c.p25.XAKY = b.A00;
  c.p25.XUXU = b.A01;
  c.p25.XYNE = b.A02;
  c.p25.XODY = b.A03;
  c.p25.XECA = b.A04;
  c.p25.XOBA = b.A05;
  c.p25.XOPO = b.A06;
  c.p25.XYBO = b.A07;
  c.p25.RYSU = b.A08;
  c.p25.RESE = b.A09;
  c.p25.RUSE = b.A10;
  c.p25.RYNA = b.A11;
  c.p25.RUMO = b.A12;

  if (b.p25.XEDU) {
    c.chip.MA00 = b.p25.XAKY;
    c.chip.MA01 = b.p25.XUXU;
    c.chip.MA02 = b.p25.XYNE;
    c.chip.MA03 = b.p25.XODY;
    c.chip.MA04 = b.p25.XECA;
    c.chip.MA05 = b.p25.XOBA;
    c.chip.MA06 = b.p25.XOPO;
    c.chip.MA07 = b.p25.XYBO;
    c.chip.MA08 = b.p25.RYSU;
    c.chip.MA09 = b.p25.RESE;
    c.chip.MA10 = b.p25.RUSE;
    c.chip.MA11 = b.p25.RYNA;
    c.chip.MA12 = b.p25.RUMO;
  }

  //----------

  c.p25.ROVE = not(b.p25.RAHU);

  c.p25.SEFA_00 = and(b.MD0, b.p25.ROVE);
  c.p25.SOGO_01 = and(b.MD1, b.p25.ROVE);
  c.p25.SEFU_02 = and(b.MD2, b.p25.ROVE);
  c.p25.SUNA_03 = and(b.MD3, b.p25.ROVE);
  c.p25.SUMO_04 = and(b.MD4, b.p25.ROVE);
  c.p25.SAZU_05 = and(b.MD5, b.p25.ROVE);
  c.p25.SAMO_06 = and(b.MD6, b.p25.ROVE);
  c.p25.SUKE_07 = and(b.MD7, b.p25.ROVE);

  c.p25.REGE_00 = not(b.p25.SEFA_00);
  c.p25.RYKY_01 = not(b.p25.SOGO_01);
  c.p25.RAZO_02 = not(b.p25.SEFU_02);
  c.p25.RADA_03 = not(b.p25.SUNA_03);
  c.p25.RYRO_04 = not(b.p25.SUMO_04);
  c.p25.REVU_05 = not(b.p25.SAZU_05);
  c.p25.REKU_06 = not(b.p25.SAMO_06);
  c.p25.RYZE_07 = not(b.p25.SUKE_07);

  c.chip.MD0_A = b.p25.REGE_00;
  c.chip.MD1_A = b.p25.RYKY_01;
  c.chip.MD2_A = b.p25.RAZO_02;
  c.chip.MD3_A = b.p25.RADA_03;
  c.chip.MD4_A = b.p25.RYRO_04;
  c.chip.MD5_A = b.p25.REVU_05;
  c.chip.MD6_A = b.p25.REKU_06;
  c.chip.MD7_A = b.p25.RYZE_07;

  //----------


  c.p25.RALO_00 = not(b.chip.D0_C);
  c.p25.TUNE_01 = not(b.chip.D1_C);
  c.p25.SERA_02 = not(b.chip.D2_C);
  c.p25.TENU_03 = not(b.chip.D3_C);
  c.p25.SYSA_04 = not(b.chip.D4_C);
  c.p25.SUGY_05 = not(b.chip.D5_C);
  c.p25.TUBE_06 = not(b.chip.D6_C);
  c.p25.SYZO_07 = not(b.chip.D7_C);

  c.p25.WEJO_00 = not(b.p25.RALO_00);
  c.p25.BUBO_01 = not(b.p25.TUNE_01);
  c.p25.BETU_02 = not(b.p25.SERA_02);
  c.p25.CYME_03 = not(b.p25.TENU_03);
  c.p25.BAXU_04 = not(b.p25.SYSA_04);
  c.p25.BUHU_05 = not(b.p25.SUGY_05);
  c.p25.BYNY_06 = not(b.p25.TUBE_06);
  c.p25.BYPY_07 = not(b.p25.SYZO_07);

  c.p25.WASA_00 = not(b.p25.RALO_00);
  c.p25.BOMO_01 = not(b.p25.TUNE_01);
  c.p25.BASA_02 = not(b.p25.SERA_02);
  c.p25.CAKO_03 = not(b.p25.TENU_03);
  c.p25.BUMA_04 = not(b.p25.SYSA_04);
  c.p25.BUPY_05 = not(b.p25.SUGY_05);
  c.p25.BASY_06 = not(b.p25.TUBE_06);
  c.p25.BAPE_07 = not(b.p25.SYZO_07);

  c.p25.CEDE = not(b.sys.DO_DMA);
  if (b.p25.CEDE) {
    c.OAM_A_D0 = b.p25.WEJO_00;
    c.OAM_A_D1 = b.p25.BUBO_01;
    c.OAM_A_D2 = b.p25.BETU_02;
    c.OAM_A_D3 = b.p25.CYME_03;
    c.OAM_A_D4 = b.p25.BAXU_04;
    c.OAM_A_D5 = b.p25.BUHU_05;
    c.OAM_A_D6 = b.p25.BYNY_06;
    c.OAM_A_D7 = b.p25.BYPY_07;

    c.OAM_B_D0 = b.p25.WASA_00;
    c.OAM_B_D1 = b.p25.BOMO_01;
    c.OAM_B_D2 = b.p25.BASA_02;
    c.OAM_B_D3 = b.p25.CAKO_03;
    c.OAM_B_D4 = b.p25.BUMA_04;
    c.OAM_B_D5 = b.p25.BUPY_05;
    c.OAM_B_D6 = b.p25.BASY_06;
    c.OAM_B_D7 = b.p25.BAPE_07;
  }

  //----------

  c.p25.TYVY = nand(b.p25.SERE, b.p28.LEKO);
  c.p25.SEBY = not(b.p25.TYVY);

  c.p25.RERY_00 = not(b.MD0);
  c.p25.RUNA_01 = not(b.MD1);
  c.p25.RONA_02 = not(b.MD2);
  c.p25.RUNO_03 = not(b.MD3);
  c.p25.SANA_04 = not(b.MD4);
  c.p25.RORO_05 = not(b.MD5);
  c.p25.RABO_06 = not(b.MD6);
  c.p25.SAME_07 = not(b.MD7);

  c.p25.RUGA_00 = not(b.p25.RERY_00);
  c.p25.ROTA_01 = not(b.p25.RUNA_01);
  c.p25.RYBU_02 = not(b.p25.RONA_02);
  c.p25.RAJU_03 = not(b.p25.RUNO_03);
  c.p25.TYJA_04 = not(b.p25.SANA_04);
  c.p25.REXU_05 = not(b.p25.RORO_05);
  c.p25.RUPY_06 = not(b.p25.RABO_06);
  c.p25.TOKU_07 = not(b.p25.SAME_07);

  if (b.p25.SEBY) {
    c.D0 = b.p25.RUGA_00;
    c.D1 = b.p25.ROTA_01;
    c.D2 = b.p25.RYBU_02;
    c.D3 = b.p25.RAJU_03;
    c.D4 = b.p25.TYJA_04;
    c.D5 = b.p25.REXU_05;
    c.D6 = b.p25.RUPY_06;
    c.D7 = b.p25.TOKU_07;
  }

  //----------

  c.p25.XUCY = nand(b.p27.NETA, b.p27.PORE);
  c.p25.XEZE = nand(b.p27.POTU, b.p27.PORE);
  c.p25.WUKO = not(b.p25.XEZE);

  //----------

  c.p25.XONU_00 = not(b.p27.XUHA);
  c.p25.WUDO_01 = not(b.p27.VYNO);
  c.p25.WAWE_02 = not(b.p27.VUJO);
  c.p25.WOLU_03 = not(b.p27.VYMU);

  if (b.p25.XUCY) {
    c.MA00 = b.p25.XONU_00;
    c.MA01 = b.p25.WUDO_01;
    c.MA02 = b.p25.WAWE_02;
    c.MA03 = b.p25.WOLU_03;
  }

  //----------

  c.p25.VAPY_04 = not(b.p32.RAWU);
  c.p25.SEZU_05 = not(b.p32.POZO);
  c.p25.VEJY_06 = not(b.p32.PYZO);
  c.p25.RUSA_07 = not(b.p32.POXA);
  c.p25.ROHA_08 = not(b.p32.PULO);
  c.p25.RESO_09 = not(b.p32.POJU);
  c.p25.SUVO_10 = not(b.p32.POWY);
  c.p25.TOBO_11 = not(b.p32.PYJU);

  c.p25.VUZA = nor(b.p23.BG_TILE_SEL, b.p32.PYJU);
  c.p25.VURY_12 = not(b.p25.VUZA);

  if (b.p27.NETA) {
    c.MA04 = b.p25.VAPY_04;
    c.MA05 = b.p25.SEZU_05;
    c.MA06 = b.p25.VEJY_06;
    c.MA07 = b.p25.RUSA_07;
    c.MA08 = b.p25.ROHA_08;
    c.MA09 = b.p25.RESO_09;
    c.MA10 = b.p25.SUVO_10;
    c.MA11 = b.p25.TOBO_11;
    c.MA12 = b.p25.VURY_12;
  }
}