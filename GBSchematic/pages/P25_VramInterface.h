#pragma once

struct P25_VramInterface {

  union { bool SYRO; bool ADDR_FE00_FFFF; };


  /*p25.WUKO*/ bool WUKO;
  /*p25.COTA*/ bool COTA;

  /*p25.MYFU*/ bool MYFU_00;
  /*p25.MASA*/ bool MASA_01;
  /*p25.MYRE*/ bool MYRE_02;
  /*p25.MAVU*/ bool MAVU_03;
  /*p25.MEPA*/ bool MEPA_04;
  /*p25.MYSA*/ bool MYSA_05;
  /*p25.MEWY*/ bool MEWY_06;
  /*p25.MUME*/ bool MUME_07;
  /*p25.VOVA*/ bool VOVA_08;
  /*p25.VODE*/ bool VODE_09;
  /*p25.RUKY*/ bool RUKY_10;
  /*p25.RUMA*/ bool RUMA_11;
  /*p25.REHO*/ bool REHO_12;

  /*p25.LEXE*/ bool LEXE_00;
  /*p25.LOZU*/ bool LOZU_01;
  /*p25.LACA*/ bool LACA_02;
  /*p25.LUVO*/ bool LUVO_03;
  /*p25.LOLY*/ bool LOLY_04;
  /*p25.LALO*/ bool LALO_05;
  /*p25.LEFA*/ bool LEFA_06;
  /*p25.LUBY*/ bool LUBY_07;
  /*p25.TUJY*/ bool TUJY_08;
  /*p25.TAGO*/ bool TAGO_09;
  /*p25.NUVA*/ bool NUVA_10;
  /*p25.PEDU*/ bool PEDU_11;
  /*p25.PONY*/ bool PONY_12;

  /*p25.RYVO*/ bool RYVO;
  /*p25.RERA*/ bool RERA;
  /*p25.RABY*/ bool RABY;
  /*p25.RORY*/ bool RORY;
  /*p25.RUJA*/ bool RUJA;
  /*p25.RAVU*/ bool RAVU;
  /*p25.RAFY*/ bool RAFY;
  /*p25.RUXA*/ bool RUXA;

  /*p25.CUFE*/ bool CUFE;
  /*p25.VAPE*/ bool VAPE;
  /*p25.AVER*/ bool AVER;
  /*p25.XUJY*/ bool XUJY;
  /*p25.BYCU*/ bool BYCU;
  /*p25.TEFA*/ bool TEFA;
  /*p25.SOSE*/ bool SOSE;
  /*p25.TUCA*/ bool TUCA;
  /*p25.TUJA*/ bool TUJA;
  /*p25.TEGU*/ bool TEGU;
  /*p25.TAVY*/ bool TAVY;
  /*p25.SYCY*/ bool SYCY;
  /*p25.SOTO*/ bool SOTO;
  /*p25.TUTO*/ bool TUTO;
  /*p25.SUDO*/ bool SUDO;
  /*p25.TEFY*/ bool TEFY;
  /*p25.SALE*/ bool SALE;
  /*p25.TYJY*/ bool TYJY;
  /*p25.TOLE*/ bool TOLE;
  /*p25.ROPY*/ bool ROPY;
  /*p25.SERE*/ bool SERE;
  /*p25.RUVY*/ bool RUVY;
  /*p25.SOHY*/ bool SOHY;
  /*p25.SAZO*/ bool SAZO;
  /*p25.RYJE*/ bool RYJE;
  /*p25.REVO*/ bool REVO;
  /*p25.RELA*/ bool RELA;
  /*p25.ROCY*/ bool ROCY;
  /*p25.RENA*/ bool RENA;
  /*p25.ROFA*/ bool ROFA;
  /*p25.RAHU*/ bool RAHU;

  /*p25.RYLU*/ bool RYLU;
  /*p25.SOHO*/ bool SOHO;
  /*p25.RAWA*/ bool RAWA;
  /*p25.APAM*/ bool APAM;
  /*p25.SUTU*/ bool SUTU;
  /*p25.RACU*/ bool RACU;
  /*p25.RACO*/ bool RACO;
  /*p25.RUTE*/ bool RUTE;
  /*p25.SEWO*/ bool SEWO;
  /*p25.TODE*/ bool TODE;
  /*p25.SEMA*/ bool SEMA;
  /*p25.TAXY*/ bool TAXY;
  /*p25.SOFY*/ bool SOFY;
  /*p25.SAHA*/ bool SAHA;
  /*p25.SETY*/ bool SETY;
  /*p25.SOKY*/ bool SOKY;
  /*p25.REFO*/ bool REFO;
  /*p25.SYSY*/ bool SYSY;
  /*p25.RAGU*/ bool RAGU;

  /*p25.RODY*/ bool RODY_00;
  /*p25.REBA*/ bool REBA_01;
  /*p25.RYDO*/ bool RYDO_02;
  /*p25.REMO*/ bool REMO_03;
  /*p25.ROCE*/ bool ROCE_04;
  /*p25.ROPU*/ bool ROPU_05;
  /*p25.RETA*/ bool RETA_06;
  /*p25.RAKU*/ bool RAKU_07;

  /*p25.TUSO*/ bool TUSO;
  /*p25.SOLE*/ bool SOLE;
  /*p25.P10_*/ bool P10_Bn;

  /*p25.SYNU*/ bool SYNU;
  /*p25.SYMA*/ bool SYMA;
  /*p25.ROKO*/ bool ROKO;
  /*p25.SYBU*/ bool SYBU;
  /*p25.SAKO*/ bool SAKO;
  /*p25.SEJY*/ bool SEJY;
  /*p25.SEDO*/ bool SEDO;
  /*p25.SAWU*/ bool SAWU;
  /*p25.RURA*/ bool RURA;
  /*p25.RULY*/ bool RULY;
  /*p25.RARE*/ bool RARE;
  /*p25.RODU*/ bool RODU;
  /*p25.RUBE*/ bool RUBE;
  /*p25.RUMU*/ bool RUMU;
  /*p25.RYTY*/ bool RYTY;
  /*p25.RADY*/ bool RADY;

  /*p25.ROVE*/ bool ROVE;

  /*p25.SEFA*/ bool SEFA_00;
  /*p25.SOGO*/ bool SOGO_01;
  /*p25.SEFU*/ bool SEFU_02;
  /*p25.SUNA*/ bool SUNA_03;
  /*p25.SUMO*/ bool SUMO_04;
  /*p25.SAZU*/ bool SAZU_05;
  /*p25.SAMO*/ bool SAMO_06;
  /*p25.SUKE*/ bool SUKE_07;

  /*p25.REGE*/ bool REGE_00;
  /*p25.RYKY*/ bool RYKY_01;
  /*p25.RAZO*/ bool RAZO_02;
  /*p25.RADA*/ bool RADA_03;
  /*p25.RYRO*/ bool RYRO_04;
  /*p25.REVU*/ bool REVU_05;
  /*p25.REKU*/ bool REKU_06;
  /*p25.RYZE*/ bool RYZE_07;

  /*p25.CEDE*/ bool CEDE;

  /*p25.RALO*/ bool RALO_00;
  /*p25.TUNE*/ bool TUNE_01;
  /*p25.SERA*/ bool SERA_02;
  /*p25.TENU*/ bool TENU_03;
  /*p25.SYSA*/ bool SYSA_04;
  /*p25.SUGY*/ bool SUGY_05;
  /*p25.TUBE*/ bool TUBE_06;
  /*p25.SYZO*/ bool SYZO_07;
  /*p25.WEJO*/ bool WEJO_00;
  /*p25.BUBO*/ bool BUBO_01;
  /*p25.BETU*/ bool BETU_02;
  /*p25.CYME*/ bool CYME_03;
  /*p25.BAXU*/ bool BAXU_04;
  /*p25.BUHU*/ bool BUHU_05;
  /*p25.BYNY*/ bool BYNY_06;
  /*p25.BYPY*/ bool BYPY_07;
  /*p25.WASA*/ bool WASA_00;
  /*p25.BOMO*/ bool BOMO_01;
  /*p25.BASA*/ bool BASA_02;
  /*p25.CAKO*/ bool CAKO_03;
  /*p25.BUMA*/ bool BUMA_04;
  /*p25.BUPY*/ bool BUPY_05;
  /*p25.BASY*/ bool BASY_06;
  /*p25.BAPE*/ bool BAPE_07;

  /*p25.XANE*/ bool XANE;
  /*p25.XEDU*/ bool XEDU;
  /*p25.XAKY*/ bool XAKY;
  /*p25.XUXU*/ bool XUXU;
  /*p25.XYNE*/ bool XYNE;
  /*p25.XODY*/ bool XODY;
  /*p25.XECA*/ bool XECA;
  /*p25.XOBA*/ bool XOBA;
  /*p25.XOPO*/ bool XOPO;
  /*p25.XYBO*/ bool XYBO;
  /*p25.RYSU*/ bool RYSU;
  /*p25.RESE*/ bool RESE;
  /*p25.RUSE*/ bool RUSE;
  /*p25.RYNA*/ bool RYNA;
  /*p25.RUMO*/ bool RUMO;

  /*p25.TYVY*/ bool TYVY;
  /*p25.SEBY*/ bool SEBY;

  /*p25.RERY*/ bool RERY_00;
  /*p25.RUNA*/ bool RUNA_01;
  /*p25.RONA*/ bool RONA_02;
  /*p25.RUNO*/ bool RUNO_03;
  /*p25.SANA*/ bool SANA_04;
  /*p25.RORO*/ bool RORO_05;
  /*p25.RABO*/ bool RABO_06;
  /*p25.SAME*/ bool SAME_07;

  /*p25.RUGA*/ bool RUGA_00;
  /*p25.ROTA*/ bool ROTA_01;
  /*p25.RYBU*/ bool RYBU_02;
  /*p25.RAJU*/ bool RAJU_03;
  /*p25.TYJA*/ bool TYJA_04;
  /*p25.REXU*/ bool REXU_05;
  /*p25.RUPY*/ bool RUPY_06;
  /*p25.TOKU*/ bool TOKU_07;

  /*p25.XUCY*/ bool XUCY;
  /*p25.XEZE*/ bool XEZE;
  /*p25.VUZA*/ bool VUZA;

  /*p25.XONU*/ bool XONU_00;
  /*p25.WUDO*/ bool WUDO_01;
  /*p25.WAWE*/ bool WAWE_02;
  /*p25.WOLU*/ bool WOLU_03;
  /*p25.VAPY*/ bool VAPY_04;
  /*p25.SEZU*/ bool SEZU_05;
  /*p25.VEJY*/ bool VEJY_06;
  /*p25.RUSA*/ bool RUSA_07;
  /*p25.ROHA*/ bool ROHA_08;
  /*p25.RESO*/ bool RESO_09;
  /*p25.SUVO*/ bool SUVO_10;
  /*p25.TOBO*/ bool TOBO_11;
  /*p25.VURY*/ bool VURY_12;
};


