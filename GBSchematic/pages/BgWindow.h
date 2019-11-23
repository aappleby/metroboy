#pragma once

struct P23_VideoRegs {

  // LCDC
  /*p23.VYRE*/ bool FF40_RD;
  /*p23.WARU*/ bool FF40_WR;
  /*p23.WYCE*/ bool FF40_RDn;
  /*p23.XUBO*/ bool FF40_WRn;

  /*p23.VYXE*/ bool LCDC_BGEN;
  /*p23.XYLO*/ bool LCDC_SPEN;
  /*p23.XYMO*/ bool LCDC_SPSIZE;
  /*p23.XAFO*/ bool LCDC_BGMAP;
  /*p23.WEXU*/ bool BG_TILE_SEL;
  /*p23.WYMO*/ bool LCDC_WINEN;
  /*p23.WOKY*/ bool WIN_MAP_SEL;
  /*p23.XONA*/ bool LCDC_EN;

  // SCY
  /*p23.ANYP*/ bool ANYP;
  /*p23.BEDY*/ bool BEDY;
  /*p23.CAVO*/ bool CAVO;
  /*p23.BUWY*/ bool BUWY;

  /*p23.GAVE*/ bool SCY0;
  /*p23.FYMO*/ bool SCY1;
  /*p23.FEZU*/ bool SCY2;
  /*p23.FUJO*/ bool SCY3;
  /*p23.DEDE*/ bool SCY4;
  /*p23.FOTY*/ bool SCY5;
  /*p23.FOHA*/ bool SCY6;
  /*p23.FUNY*/ bool SCY7;

  // SCX
  /*p23.AVOG*/ bool AVOG;
  /*p23.ARUR*/ bool ARUR;
  /*p23.AMUN*/ bool AMUN;
  /*p23.BEBA*/ bool BEBA;

  /*p23.DATY*/ bool SCX0;
  /*p23.DUZU*/ bool SCX1;
  /*p23.CYXU*/ bool SCX2;
  /*p23.GUBO*/ bool SCX3;
  /*p23.BEMY*/ bool SCX4;
  /*p23.CUZY*/ bool SCX5;
  /*p23.CABU*/ bool SCX6;
  /*p23.BAKE*/ bool SCX7;

  // LYC
  /*p23.XYLY*/ bool XYLY;
  /*p23.XUFA*/ bool XUFA;
  /*p23.WANE*/ bool WANE;
  /*p23.WEKU*/ bool WEKU;

  /*p23.SYRY*/ bool LYC0;
  /*p23.VUCE*/ bool LYC1;
  /*p23.SEDY*/ bool LYC2;
  /*p23.SALO*/ bool LYC3;
  /*p23.SOTA*/ bool LYC4;
  /*p23.VAFA*/ bool LYC5;
  /*p23.VEVO*/ bool LYC6;
  /*p23.RAHA*/ bool LYC7;

  // WY
  /*p23.WAXU*/ bool WAXU;
  /*p23.WEKO*/ bool WEKO;
  /*p23.VEFU*/ bool VEFU;
  /*p23.VOMY*/ bool VOMY;

  /*p23.NESO*/ bool WY0;
  /*p23.NYRO*/ bool WY1;
  /*p23.NAGA*/ bool WY2;
  /*p23.MELA*/ bool WY3;
  /*p23.NULO*/ bool WY4;
  /*p23.NENE*/ bool WY5;
  /*p23.NUKA*/ bool WY6;
  /*p23.NAFU*/ bool WY7;

  // WX
  /*p23.WYZE*/ bool WYZE;
  /*p23.WUZA*/ bool WUZA;
  /*p23.VYCU*/ bool VYCU;
  /*p23.VOXU*/ bool VOXU;

  /*p23.MYPA*/ bool WX0;
  /*p23.NOFE*/ bool WX1;
  /*p23.NOKE*/ bool WX2;
  /*p23.MEBY*/ bool WX3;
  /*p23.MYPU*/ bool WX4;
  /*p23.MYCE*/ bool WX5;
  /*p23.MUVO*/ bool WX6;
  /*p23.NUKU*/ bool WX7;

  // LY
  /*p23.WAFU*/ bool FF44_RD;
  /*p23.VARO*/ bool FF44_RDn;

  /*p23.WURY*/ bool LY0n;
  /*p23.XEPO*/ bool LY1n;
  /*p23.MYFA*/ bool LY2n;
  /*p23.XUHY*/ bool LY3n;
  /*p23.WATA*/ bool LY4n;
  /*p23.XAGA*/ bool LY5n;
  /*p23.XUCE*/ bool LY6n;
  /*p23.XOWO*/ bool LY7n;
};

struct P24_LcdControl {

  /*p24.SACU*/ bool CLKPIPE;
  /*p24.ROXO*/ bool FINE_MATCH_CLK;
  /*p24.POKY*/ bool POKY;
  /*p24.PORY*/ bool PORY;
  /*p24.TOMU*/ bool TOMU;
  /*p24.NYKA*/ bool NYKA;
  /*p24.SEGU*/ bool SEGU_4M;
  /*p24.LOBY*/ bool RENDERINGn;

  /*p24.NAFY*/ bool NAFY;
  /*p24.PYGO*/ bool PYGO;
  /*p24.SOCY*/ bool SOCY;
  /*p24.VYBO*/ bool VYBO;
  /*p24.TYFA*/ bool TYFA;
  /*p24.PAHO*/ bool PAHO;
  /*p24.POME*/ bool POME;
  /*p24.RUJU*/ bool RUJU;
  /*p24.POFY*/ bool POFY;
  /*p24.NERU*/ bool LINE_000n;
  /*p24.MEDA*/ bool LINE_000_SYNCn;
  /*p24.LOFU*/ bool LINE_DONE;
  /*p24.LUCA*/ bool LUCA;
  /*p24.MAGU*/ bool MAGU;
  /*p24.LEBE*/ bool LEBE;
  /*p24.MECO*/ bool MECO;
  /*p24.KEBO*/ bool KEBO;
  /*p24.KASA*/ bool KASA;
  /*p24.UMOB*/ bool UMOB;
  /*p24.USEC*/ bool USEC;
  /*p24.KAHE*/ bool CPLn;
  /*p24.KUPA*/ bool FRn;

  /*p24.KEDY*/ bool LCDC_ENn;
};

struct P25_VramInterface {

  /*p25.SYRO*/ bool ADDR_FE00_FFFF;

  /*p25.XEZE*/ bool WIN_MAP_READn;
  /*p25.WUKO*/ bool WIN_MAP_READ;

  /*p25.COTA*/ bool OAM_IN_CLKn;

  /*p25.MYFU*/ bool MA00n;
  /*p25.MASA*/ bool MA01n;
  /*p25.MYRE*/ bool MA02n;
  /*p25.MAVU*/ bool MA03n;
  /*p25.MEPA*/ bool MA04n;
  /*p25.MYSA*/ bool MA05n;
  /*p25.MEWY*/ bool MA06n;
  /*p25.MUME*/ bool MA07n;
  /*p25.VOVA*/ bool MA08n;
  /*p25.VODE*/ bool MA09n;
  /*p25.RUKY*/ bool MA10n;
  /*p25.RUMA*/ bool MA11n;
  /*p25.REHO*/ bool MA12n;

  /*p25.LEXE*/ bool MA00b;
  /*p25.LOZU*/ bool MA01b;
  /*p25.LACA*/ bool MA02b;
  /*p25.LUVO*/ bool MA03b;
  /*p25.LOLY*/ bool MA04b;
  /*p25.LALO*/ bool MA05b;
  /*p25.LEFA*/ bool MA06b;
  /*p25.LUBY*/ bool MA07b;
  /*p25.TUJY*/ bool MA08b;
  /*p25.TAGO*/ bool MA09b;
  /*p25.NUVA*/ bool MA10b;
  /*p25.PEDU*/ bool MA11b;
  /*p25.PONY*/ bool MA12b;

  // guess
  /*p25.CUFE*/ bool OAM_WR;

  /*p25.VAPE*/ bool VAPE;
  /*p25.AVER*/ bool AVER;
  /*p25.XUJY*/ bool XUJY;
  /*p25.BYCU*/ bool OAM_IN_CLK;
  /*p25.TEFA*/ bool TEFA;
  /*p25.SOSE*/ bool ADDR_VRAM;
  /*p25.TUCA*/ bool CPU_VRAM_RD;
  /*p25.TUJA*/ bool CPU_VRAM_WR;
  /*p25.TEGU*/ bool CPU_VRAM_CLK;
  /*p25.TAVY*/ bool MOE_Cn;
  /*p25.SYCY*/ bool MODE_DBG2n;
  /*p25.SOTO*/ bool DBG_SOTO;
  /*p25.TUTO*/ bool DBG_TUTO;
  /*p25.SUDO*/ bool MWR_Cn;
  /*p25.TEFY*/ bool MCS_Cn;

  /*p25.TOLE*/ bool CPU_VRAM_RD2;
  /*p25.TYJY*/ bool CPU_VRAM_WR2;
  /*p25.SALE*/ bool CPU_VRAM_CLK2;

  /*p25.RENA*/ bool MD_IN;
  /*p25.RYJE*/ bool MD_INb;
  /*p25.RAHU*/ bool D_TO_MD;

  /*p25.ROFA*/ bool MD_OUT;
  /*p25.RELA*/ bool MD_OUTb;
  /*p25.REVO*/ bool MD_OUTc;
  /*p25.SAZO*/ bool MD_OUTd;
  /*p25.ROCY*/ bool MD_OUTe;

  /*p25.ROPY*/ bool RENDERINGn;
  /*p25.SERE*/ bool SERE;
  /*p25.RUVY*/ bool CPU_VRAM_CLK2n;
  /*p25.SOHY*/ bool MWR;

  /*p25.RYLU*/ bool RYLU;
  /*p25.SOHO*/ bool SOHO;
  /*p25.RAWA*/ bool RAWA;
  /*p25.APAM*/ bool VRAM_TO_OAM;
  /*p25.SUTU*/ bool MCS;
  /*p25.RACU*/ bool MOE;
  /*p25.RACO*/ bool DBG_TUTOn;
  /*p25.RUTE*/ bool MOE_Dn;
  /*p25.SEWO*/ bool MCS_Dn;
  /*p25.TODE*/ bool MCS_An;
  /*p25.SEMA*/ bool MOE_An;
  /*p25.TAXY*/ bool MWR_An;
  /*p25.SOFY*/ bool MWR_Dn;

  // something debug
  /*p25.TUSO*/ bool TUSO;
  /*p25.SOLE*/ bool SOLE;
  /*p25.????*/ bool P10_Bn;

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

  /*p25.ROVE*/ bool D_TO_MDn;

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

  /*p25.CEDE*/ bool DO_DMAn;

  /*p25.RALO*/ bool D0n;
  /*p25.TUNE*/ bool D1n;
  /*p25.SERA*/ bool D2n;
  /*p25.TENU*/ bool D3n;
  /*p25.SYSA*/ bool D4n;
  /*p25.SUGY*/ bool D5n;
  /*p25.TUBE*/ bool D6n;
  /*p25.SYZO*/ bool D7n;

  /*p25.XANE*/ bool A_TO_MAn;
  /*p25.XEDU*/ bool A_TO_MA;

  /*p25.TYVY*/ bool MD_TO_Dn;
  /*p25.SEBY*/ bool MD_TO_D;

  /*p25.RERY*/ bool RERY_00;
  /*p25.RUNA*/ bool RUNA_01;
  /*p25.RONA*/ bool RONA_02;
  /*p25.RUNO*/ bool RUNO_03;
  /*p25.SANA*/ bool SANA_04;
  /*p25.RORO*/ bool RORO_05;
  /*p25.RABO*/ bool RABO_06;
  /*p25.SAME*/ bool SAME_07;

  /*p25.XUCY*/ bool WIN_TILE_READb;
  /*p25.VUZA*/ bool WIN_TILE_BANK;
};




#pragma once

struct Gameboy;

struct P26_Background {
public:

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);


  /*p26.AXAD*/ bool AXAD;
  /*p26.ASUL*/ bool FETCH_TILEn;
  /*p26.ACEN*/ bool FETCH_MAPn;
  /*p26.BEJE*/ bool FETCH_TILE;
  /*p26.BAFY*/ bool FETCH_MAP;

  /*p26.FAFO*/ bool TILE_Y0;
  /*p26.EMUX*/ bool TILE_Y1;
  /*p26.ECAB*/ bool TILE_Y2;

  /*p26.ETAM*/ bool MAP_Y0;
  /*p26.DOTO*/ bool MAP_Y1;
  /*p26.DABA*/ bool MAP_Y2;
  /*p26.EFYK*/ bool MAP_Y3;
  /*p26.EJOK*/ bool MAP_Y4;

  /*p26.FAFO*/ bool TILE_Y0C;
  /*p26.EMUX*/ bool TILE_Y1C;
  /*p26.ECAB*/ bool TILE_Y2C;
  /*p26.ETAM*/ bool MAP_Y0C;
  /*p26.DOTO*/ bool MAP_Y1C;
  /*p26.DABA*/ bool MAP_Y2C;
  /*p26.EFYK*/ bool MAP_Y3C;
  /*p26.EJOK*/ bool MAP_Y4C;

  /*p26.ATAD*/ bool TILE_X0;
  /*p26.BEHU*/ bool TILE_X1;
  /*p26.APYH*/ bool TILE_X2;
  /*p26.BABE*/ bool MAP_X0;
  /*p26.ABOD*/ bool MAP_X1;
  /*p26.BEWY*/ bool MAP_X2;
  /*p26.BYCA*/ bool MAP_X3;
  /*p26.ACUL*/ bool MAP_X4;

  /*p26.ATAD*/ bool TILE_X0C;
  /*p26.BEHU*/ bool TILE_X1C;
  /*p26.APYH*/ bool TILE_X2C;
  /*p26.BABE*/ bool MAP_X0C;
  /*p26.ABOD*/ bool MAP_X1C;
  /*p26.BEWY*/ bool MAP_X2C;
  /*p26.BYCA*/ bool MAP_X3C;
  /*p26.ACUL*/ bool MAP_X4C;

  /*p26.XOGA*/ bool OAM_A_D7n;
  /*p26.XURA*/ bool OAM_A_D7o;
  /*p26.TAJO*/ bool OAM_A_D7p;
  /*p26.XENU*/ bool OAM_A_D7q;
  /*p26.XYKE*/ bool OAM_A_D7r;
  /*p26.XABA*/ bool OAM_A_D7s;
  /*p26.TAFU*/ bool OAM_A_D7t;
  /*p26.XUHO*/ bool OAM_A_D7u;

  /*p26.TEDE*/ bool MASK_PIPE_SET0;
  /*p26.XALA*/ bool MASK_PIPE_SET1;
  /*p26.TYRA*/ bool MASK_PIPE_SET2;
  /*p26.XYRU*/ bool MASK_PIPE_SET3;
  /*p26.XUKU*/ bool MASK_PIPE_SET4;
  /*p26.XELY*/ bool MASK_PIPE_SET5;
  /*p26.TYKO*/ bool MASK_PIPE_SET6;
  /*p26.TUWU*/ bool MASK_PIPE_SET7;

  /*p26.WOKA*/ bool MASK_PIPE_RST0;
  /*p26.WEDE*/ bool MASK_PIPE_RST1;
  /*p26.TUFO*/ bool MASK_PIPE_RST2;
  /*p26.WEVO*/ bool MASK_PIPE_RST3;
  /*p26.WEDY*/ bool MASK_PIPE_RST4;
  /*p26.WUJA*/ bool MASK_PIPE_RST5;
  /*p26.TENA*/ bool MASK_PIPE_RST6;
  /*p26.WUBU*/ bool MASK_PIPE_RST7;

  /*p26.VEZO*/ bool MASK_PIPE_0;
  /*p26.WURU*/ bool MASK_PIPE_1;
  /*p26.VOSA*/ bool MASK_PIPE_2;
  /*p26.WYFU*/ bool MASK_PIPE_3;
  /*p26.XETE*/ bool MASK_PIPE_4;
  /*p26.WODA*/ bool MASK_PIPE_5;
  /*p26.VUMO*/ bool MASK_PIPE_6;
  /*p26.VAVA*/ bool MASK_PIPE_7;
};



#pragma once

struct P27_WindowMapLookup {

  /*p27.LENA*/ bool LONYb;
  /*p27.MYMA*/ bool MYMA;
  /*p27.MOSU*/ bool MOSU;
  /*p27.LYRY*/ bool BG_SEQ_5;
  /*p27.SYLO*/ bool SYLO;
  /*p27.ROXY*/ bool FINE_MATCH_TRIG_OR_NOT_RENDERING;
  /*p27.POVA*/ bool FINE_MATCH_TRIG;
  /*p27.NETA*/ bool WIN_TILE_READa;
  /*p27.POTU*/ bool POTU;
  /*p27.PORE*/ bool TILE_Y_CLKn;
  /*p27.VYPO*/ bool P10_Bn;

  /*p27.MOFU*/ bool MOFU;
  /*p27.NYDY*/ bool BG_SEQ_TRIG_3n;
  /*p27.NYXU*/ bool BG_SEQ_RSTn;

  /*p27.SECA*/ bool SECA_RST;

  /*p27.NAZE*/ bool WY_MATCH0n;
  /*p27.PEBO*/ bool WY_MATCH1n;
  /*p27.POMO*/ bool WY_MATCH2n;
  /*p27.NEVU*/ bool WY_MATCH3n;
  /*p27.NOJO*/ bool WY_MATCH4n;
  /*p27.PAGA*/ bool WY_MATCH5n;
  /*p27.PEZO*/ bool WY_MATCH6n;
  /*p27.NUPA*/ bool WY_MATCH7n;

  /*p27.PALO*/ bool WY_MATCH_HI;
  /*p27.NELE*/ bool WY_MATCH_HIn;
  /*p27.PAFU*/ bool WY_MATCHn;
  /*p27.ROGE*/ bool WY_MATCH;
  /*p27.SARY*/ bool WY_MATCH_SYNC;
  /*p27.REPU*/ bool IN_FRAME_Y;
  /*p27.REJO*/ bool WIN_CHECK_X;

  /*p27.MYLO*/ bool WX_MATCH0n;
  /*p27.PUWU*/ bool WX_MATCH1n;
  /*p27.PUHO*/ bool WX_MATCH2n;
  /*p27.NYTU*/ bool WX_MATCH3n;
  /*p27.NEZO*/ bool WX_MATCH4n;
  /*p27.NORY*/ bool WX_MATCH5n;
  /*p27.NONO*/ bool WX_MATCH6n;
  /*p27.PASE*/ bool WX_MATCH7n;

  /*p27.PUKY*/ bool WX_MATCH_HI;
  /*p27.NUFA*/ bool WX_MATCH_HIn;
  /*p27.NOGY*/ bool WIN_MATCHn;
  /*p27.NUKO*/ bool WIN_MATCH;

  /*p27.ROZE*/ bool FINE_COUNT_STOPn;
  /*p27.PAHA*/ bool RENDERINGo;






  /*p27.PANY*/ bool WIN_MATCH_ONSCREEN;
  /*p27.SEKO*/ bool WIN_TRIGGER;
  /*p27.ROMO*/ bool ROMO;
  /*p27.SUVU*/ bool SUVU;
  /*p27.TAVE*/ bool TAVE;
  /*p27.XAHY*/ bool NEW_LINEn;
  /*p27.XOFO*/ bool MAP_X_RST;
  /*p27.XACO*/ bool MAP_X_RSTn;
  /*p27.PYNU*/ bool TILE_ABb;
  /*p27.NUNY*/ bool NUNY;
  /*p27.NOCU*/ bool TILE_Y_CLK;
  /*p27.PUKU*/ bool PUKU;
  /*p27.RYDY*/ bool RYDY;
  /*p27.TUXY*/ bool TUXY;
  /*p27.SUZU*/ bool SUZU;
  /*p27.TEVO*/ bool MAP_X_CLK_STOPn;
  /*p27.PASO*/ bool FINE_RST;
  /*p27.VETU*/ bool MAP_X_CLK;
  /*p27.ROCO*/ bool ROCO_4M;
  /*p27.NYFO*/ bool NYFO;
  /*p27.WAZY*/ bool TILE_ABo;
  /*p27.SYNY*/ bool IN_FRAME_Yn;
  /*p27.NOPA*/ bool NOPA;
  /*p27.PYCO*/ bool WIN_MATCH_SYNC1;
  /*p27.NUNU*/ bool WIN_MATCH_SYNC2;

  /*p27.PECU*/ bool FINE_CLK;
  /*p27.RYKU*/ bool FINE_CNT0;
  /*p27.ROGA*/ bool FINE_CNT1;
  /*p27.RUBU*/ bool FINE_CNT2;
  /*p27.SUHA*/ bool FINE_MATCH0;
  /*p27.SYBY*/ bool FINE_MATCH1;
  /*p27.SOZU*/ bool FINE_MATCH2;
  /*p27.RONE*/ bool FINE_MATCH;
  /*p27.POHU*/ bool FINE_MATCHn;

  /*p27.NYZE*/ bool FINE_MATCH_SYNC2;
  /*p27.PUXA*/ bool FINE_MATCH_SYNC1;
  /*p27.RYFA*/ bool WIN_MATCH_ONSCREEN_SYNC1;
  /*p27.RENE*/ bool WIN_MATCH_ONSCREEN_SYNC2;
  /*p27.SOVY*/ bool SOVY;
  /*p27.MOCE*/ bool BG_SEQ_5n;
  /*p27.LEBO*/ bool BG_SEQ_CLK;
  /*p27.LAXE*/ bool BG_SEQ0n;
  /*p27.MYSO*/ bool BG_SEQ_TRIG_1357;
  /*p27.NOGU*/ bool BG_SEQ_xx234567;
  /*p27.NENY*/ bool BG_SEQ_01xxxxxx;
  /*p27.LURY*/ bool LURY;
  /*p27.LONY*/ bool LONY;
  /*p27.LUSU*/ bool LONYn;
  /*p27.LYZU*/ bool BG_SEQ_x1x3x5x7_DELAY;

  /*p27.LAXU*/ bool BG_SEQ_x1x3x5x7;
  /*p27.MESU*/ bool BG_SEQ_xx23xx67;
  /*p27.NYVA*/ bool BG_SEQ_xxxx4567;
  /*p27.XUHA*/ bool BG_SEQ_xxxx4567b;

  /*p27.NAKO*/ bool BG_SEQ_01xx45xx;
  /*p27.NOFU*/ bool BG_SEQ_0123xxxx;


  /*p27.LOVY*/ bool BG_SEQ5_SYNC;

  /*p27.WYKA*/ bool MAP_X0;
  /*p27.WODY*/ bool MAP_X1;
  /*p27.WOBO*/ bool MAP_X2;
  /*p27.WYKO*/ bool MAP_X3;
  /*p27.XOLO*/ bool MAP_X4;

  /*p27.VYNO*/ bool TILE_Y0;
  /*p27.VUJO*/ bool TILE_Y1;
  /*p27.VYMU*/ bool TILE_Y2;

  /*p27.TUFU*/ bool MAP_Y0;
  /*p27.TAXA*/ bool MAP_Y1;
  /*p27.TOZO*/ bool MAP_Y2;
  /*p27.TATE*/ bool MAP_Y3;
  /*p27.TEKE*/ bool MAP_Y4;

  /*p27.VEVY*/ bool MA10;
  /*p27.VEZA*/ bool MA11;
  /*p27.VOGU*/ bool MA12;

  /*p27.RYCE*/ bool TEKY_TRIG;
  /*p27.VEKU*/ bool VEKU;
  /*p27.TAKA*/ bool TAKA;
  /*p27.TUKU*/ bool TUKU;
  /*p27.SOWO*/ bool SOWO;
  /*p27.TEKY*/ bool TEKY;
  /*p27.SOBU*/ bool TEKY_SYNC1;
  /*p27.SUDA*/ bool TEKY_SYNC2;
};



