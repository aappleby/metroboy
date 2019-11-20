#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P30_SpriteStore::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  /*p30.CYKE*/ c.p30.CYKE = not(b.p21.CLK_2Mb);
  /*p30.WUDA*/ c.p30.WUDA = not(b.p30.CYKE);

  /*p30.XECU*/ c.p30.IDX_5 = tock_pos(a.p30.WUDA, b.p30.WUDA, b.p28.P10_Bn, b.p30.IDX_5, b.p28.OAM_A7);
  /*p30.YDUF*/ c.p30.IDX_4 = tock_pos(a.p30.WUDA, b.p30.WUDA, b.p28.P10_Bn, b.p30.IDX_4, b.p28.OAM_A6);
  /*p30.XOBE*/ c.p30.IDX_3 = tock_pos(a.p30.WUDA, b.p30.WUDA, b.p28.P10_Bn, b.p30.IDX_3, b.p28.OAM_A5);
  /*p30.ZUZE*/ c.p30.IDX_2 = tock_pos(a.p30.WUDA, b.p30.WUDA, b.p28.P10_Bn, b.p30.IDX_2, b.p28.OAM_A4);
  /*p30.XEDY*/ c.p30.IDX_1 = tock_pos(a.p30.WUDA, b.p30.WUDA, b.p28.P10_Bn, b.p30.IDX_1, b.p28.OAM_A3);
  /*p30.XADU*/ c.p30.IDX_0 = tock_pos(a.p30.WUDA, b.p30.WUDA, b.p28.P10_Bn, b.p30.IDX_0, b.p28.OAM_A2);

  // so this is a bit weird as we have a tri-state bus inside one schematic...

  /*p30.WEZA*/ if (b.p29.STORE_SPRITE_IDX) c.p30.TS_IDX_0 = b.p30.IDX_5; // order wrong
  /*p30.WUCO*/ if (b.p29.STORE_SPRITE_IDX) c.p30.TS_IDX_1 = b.p30.IDX_4;
  /*p30.WYDA*/ if (b.p29.STORE_SPRITE_IDX) c.p30.TS_IDX_2 = b.p30.IDX_3;
  /*p30.ZYSU*/ if (b.p29.STORE_SPRITE_IDX) c.p30.TS_IDX_3 = b.p30.IDX_2;
  /*p30.WYSE*/ if (b.p29.STORE_SPRITE_IDX) c.p30.TS_IDX_4 = b.p30.IDX_1;
  /*p30.WUZY*/ if (b.p29.STORE_SPRITE_IDX) c.p30.TS_IDX_5 = b.p30.IDX_0;

  /*p30.WENU*/ if (b.p29.OAM_SCAN) c.p30.TS_LINE_0 = b.p29.SPRITE_DELTA1; // order wrong
  /*p30.CUCU*/ if (b.p29.OAM_SCAN) c.p30.TS_LINE_1 = b.p29.SPRITE_DELTA2;
  /*p30.CUCA*/ if (b.p29.OAM_SCAN) c.p30.TS_LINE_2 = b.p29.SPRITE_DELTA0;
  /*p30.CEGA*/ if (b.p29.OAM_SCAN) c.p30.TS_LINE_3 = b.p29.SPRITE_DELTA3;

  /*p30.AXUV*/ c.p30.AXUV = tock_pos(a.p29.AKOL, b.p29.AKOL, 0, b.p30.AXUV, b.p30.TS_IDX_0 );
  /*p30.BADA*/ c.p30.BADA = tock_pos(a.p29.AKOL, b.p29.AKOL, 0, b.p30.BADA, b.p30.TS_IDX_1 );
  /*p30.APEV*/ c.p30.APEV = tock_pos(a.p29.AKOL, b.p29.AKOL, 0, b.p30.APEV, b.p30.TS_IDX_2 );
  /*p30.BADO*/ c.p30.BADO = tock_pos(a.p29.AKOL, b.p29.AKOL, 0, b.p30.BADO, b.p30.TS_IDX_3 );
  /*p30.BEXY*/ c.p30.BEXY = tock_pos(a.p29.AKOL, b.p29.AKOL, 0, b.p30.BEXY, b.p30.TS_IDX_4 );
  /*p30.BYHE*/ c.p30.BYHE = tock_pos(a.p29.AKOL, b.p29.AKOL, 0, b.p30.BYHE, b.p30.TS_IDX_5 );
  /*p30.AFYM*/ c.p30.AFYM = tock_pos(a.p29.BYMY, b.p29.BYMY, 0, b.p30.AFYM, b.p30.TS_LINE_0);
  /*p30.AZAP*/ c.p30.AZAP = tock_pos(a.p29.BYMY, b.p29.BYMY, 0, b.p30.AZAP, b.p30.TS_LINE_1);
  /*p30.AFUT*/ c.p30.AFUT = tock_pos(a.p29.BYMY, b.p29.BYMY, 0, b.p30.AFUT, b.p30.TS_LINE_2);
  /*p30.AFYX*/ c.p30.AFYX = tock_pos(a.p29.BYMY, b.p29.BYMY, 0, b.p30.AFYX, b.p30.TS_LINE_3);

  /*p30.ADYB*/ if (b.p29.DENY) c.p30.TS_IDX_0  = b.p30.AXUV;
  /*p30.APOB*/ if (b.p29.DENY) c.p30.TS_IDX_1  = b.p30.BADA;
  /*p30.APYV*/ if (b.p29.DENY) c.p30.TS_IDX_2  = b.p30.APEV;
  /*p30.AFEN*/ if (b.p29.DENY) c.p30.TS_IDX_3  = b.p30.BADO;
  /*p30.AKYH*/ if (b.p29.DENY) c.p30.TS_IDX_4  = b.p30.BEXY;
  /*p30.APOC*/ if (b.p29.DENY) c.p30.TS_IDX_5  = b.p30.BYHE;
  /*p30.BUJY*/ if (b.p29.DENY) c.p30.TS_LINE_0 = b.p30.AFYM;
  /*p30.BOSO*/ if (b.p29.DENY) c.p30.TS_LINE_1 = b.p30.AZAP;
  /*p30.AHAC*/ if (b.p29.DENY) c.p30.TS_LINE_2 = b.p30.AFUT;
  /*p30.BAZU*/ if (b.p29.DENY) c.p30.TS_LINE_3 = b.p30.AFYX;
   
  /*p30.WOCY*/ c.p30.WOCY = not(!b.p30.GECU); if (b.p29.XYME) c.p30.TS_IDX_0  = b.p30.WOCY; c.p30.GECU = tock_pos(a.p29.WUSE, b.p29.WUSE, 0, b.p30.GECU, b.p30.TS_IDX_0 );
  /*p30.ELYC*/ c.p30.ELYC = not(!b.p30.FOXY); if (b.p29.XYME) c.p30.TS_IDX_1  = b.p30.ELYC; c.p30.FOXY = tock_pos(a.p29.WUSE, b.p29.WUSE, 0, b.p30.FOXY, b.p30.TS_IDX_1 );
  /*p30.WABO*/ c.p30.WABO = not(!b.p30.GOHU); if (b.p29.XYME) c.p30.TS_IDX_2  = b.p30.WABO; c.p30.GOHU = tock_pos(a.p29.WUSE, b.p29.WUSE, 0, b.p30.GOHU, b.p30.TS_IDX_2 );
  /*p30.EZOC*/ c.p30.EZOC = not(!b.p30.FOGO); if (b.p29.XYME) c.p30.TS_IDX_3  = b.p30.EZOC; c.p30.FOGO = tock_pos(a.p29.WUSE, b.p29.WUSE, 0, b.p30.FOGO, b.p30.TS_IDX_3 );
  /*p30.WYWY*/ c.p30.WYWY = not(!b.p30.GACY); if (b.p29.XYME) c.p30.TS_IDX_4  = b.p30.WYWY; c.p30.GACY = tock_pos(a.p29.WUSE, b.p29.WUSE, 0, b.p30.GACY, b.p30.TS_IDX_4 );
  /*p30.WATO*/ c.p30.WATO = not(!b.p30.GABO); if (b.p29.XYME) c.p30.TS_IDX_5  = b.p30.WATO; c.p30.GABO = tock_pos(a.p29.WUSE, b.p29.WUSE, 0, b.p30.GABO, b.p30.TS_IDX_5 );
  /*p30.ZUDO*/ c.p30.ZUDO = not(!b.p30.ZUBE); if (b.p29.XYME) c.p30.TS_LINE_0 = b.p30.ZUDO; c.p30.ZUBE = tock_pos(a.p29.ZURU, b.p29.ZURU, 0, b.p30.ZUBE, b.p30.TS_LINE_0);
  /*p30.YBUK*/ c.p30.YBUK = not(!b.p30.ZUMY); if (b.p29.XYME) c.p30.TS_LINE_1 = b.p30.YBUK; c.p30.ZUMY = tock_pos(a.p29.ZURU, b.p29.ZURU, 0, b.p30.ZUMY, b.p30.TS_LINE_1);
  /*p30.ZYTO*/ c.p30.ZYTO = not(!b.p30.ZEXO); if (b.p29.XYME) c.p30.TS_LINE_2 = b.p30.ZYTO; c.p30.ZEXO = tock_pos(a.p29.ZURU, b.p29.ZURU, 0, b.p30.ZEXO, b.p30.TS_LINE_2);
  /*p30.YKOZ*/ c.p30.YKOZ = not(!b.p30.ZAFU); if (b.p29.XYME) c.p30.TS_LINE_3 = b.p30.YKOZ; c.p30.ZAFU = tock_pos(a.p29.ZURU, b.p29.ZURU, 0, b.p30.ZAFU, b.p30.TS_LINE_3);
   
  /*p30.WAJA*/ c.p30.WAJA = not(!b.p30.XYNU); if (b.p29.GOWO) c.p30.TS_IDX_0  = b.p30.WAJA; c.p30.XYNU = tock_pos(a.p29.WYLU, b.p29.WYLU, 0, b.p30.XYNU, b.p30.TS_IDX_0 );
  /*p30.WOXY*/ c.p30.WOXY = not(!b.p30.XEGE); if (b.p29.GOWO) c.p30.TS_IDX_1  = b.p30.WOXY; c.p30.XEGE = tock_pos(a.p29.WYLU, b.p29.WYLU, 0, b.p30.XEGE, b.p30.TS_IDX_1 );
  /*p30.XYRE*/ c.p30.XYRE = not(!b.p30.XABO); if (b.p29.GOWO) c.p30.TS_IDX_2  = b.p30.XYRE; c.p30.XABO = tock_pos(a.p29.WYLU, b.p29.WYLU, 0, b.p30.XABO, b.p30.TS_IDX_2 );
  /*p30.WERU*/ c.p30.WERU = not(!b.p30.WANU); if (b.p29.GOWO) c.p30.TS_IDX_3  = b.p30.WERU; c.p30.WANU = tock_pos(a.p29.WYLU, b.p29.WYLU, 0, b.p30.WANU, b.p30.TS_IDX_3 );
  /*p30.WEPY*/ c.p30.WEPY = not(!b.p30.XEFE); if (b.p29.GOWO) c.p30.TS_IDX_4  = b.p30.WEPY; c.p30.XEFE = tock_pos(a.p29.WYLU, b.p29.WYLU, 0, b.p30.XEFE, b.p30.TS_IDX_4 );
  /*p30.WUXU*/ c.p30.WUXU = not(!b.p30.XAVE); if (b.p29.GOWO) c.p30.TS_IDX_5  = b.p30.WUXU; c.p30.XAVE = tock_pos(a.p29.WYLU, b.p29.WYLU, 0, b.p30.XAVE, b.p30.TS_IDX_5 );
  /*p30.BYDO*/ c.p30.BYDO = not(!b.p30.CUMU); if (b.p29.GOWO) c.p30.TS_LINE_0 = b.p30.BYDO; c.p30.CUMU = tock_pos(a.p29.EWOT, b.p29.EWOT, 0, b.p30.CUMU, b.p30.TS_LINE_0);
  /*p30.BUCE*/ c.p30.BUCE = not(!b.p30.CAPO); if (b.p29.GOWO) c.p30.TS_LINE_1 = b.p30.BUCE; c.p30.CAPO = tock_pos(a.p29.EWOT, b.p29.EWOT, 0, b.p30.CAPO, b.p30.TS_LINE_1);
  /*p30.BOVE*/ c.p30.BOVE = not(!b.p30.CONO); if (b.p29.GOWO) c.p30.TS_LINE_2 = b.p30.BOVE; c.p30.CONO = tock_pos(a.p29.EWOT, b.p29.EWOT, 0, b.p30.CONO, b.p30.TS_LINE_2);
  /*p30.BEVY*/ c.p30.BEVY = not(!b.p30.CAJU); if (b.p29.GOWO) c.p30.TS_LINE_3 = b.p30.BEVY; c.p30.CAJU = tock_pos(a.p29.EWOT, b.p29.EWOT, 0, b.p30.CAJU, b.p30.TS_LINE_3);
   
  /*p30.EVYT*/ c.p30.EVYT = not(!b.p30.FUZO); if (b.p29.GUGY) c.p30.TS_IDX_0  = b.p30.EVYT; c.p30.FUZO = tock_pos(a.p29.FEFO, b.p29.FEFO, 0, b.p30.FUZO, b.p30.TS_IDX_0 );
  /*p30.WABA*/ c.p30.WABA = not(!b.p30.GESY); if (b.p29.GUGY) c.p30.TS_IDX_1  = b.p30.WABA; c.p30.GESY = tock_pos(a.p29.FEFO, b.p29.FEFO, 0, b.p30.GESY, b.p30.TS_IDX_1 );
  /*p30.ETAD*/ c.p30.ETAD = not(!b.p30.FYSU); if (b.p29.GUGY) c.p30.TS_IDX_2  = b.p30.ETAD; c.p30.FYSU = tock_pos(a.p29.FEFO, b.p29.FEFO, 0, b.p30.FYSU, b.p30.TS_IDX_2 );
  /*p30.ELEP*/ c.p30.ELEP = not(!b.p30.FEFA); if (b.p29.GUGY) c.p30.TS_IDX_3  = b.p30.ELEP; c.p30.FEFA = tock_pos(a.p29.FEFO, b.p29.FEFO, 0, b.p30.FEFA, b.p30.TS_IDX_3 );
  /*p30.WYGO*/ c.p30.WYGO = not(!b.p30.GYNO); if (b.p29.GUGY) c.p30.TS_IDX_4  = b.p30.WYGO; c.p30.GYNO = tock_pos(a.p29.FEFO, b.p29.FEFO, 0, b.p30.GYNO, b.p30.TS_IDX_4 );
  /*p30.WAKO*/ c.p30.WAKO = not(!b.p30.GULE); if (b.p29.GUGY) c.p30.TS_IDX_5  = b.p30.WAKO; c.p30.GULE = tock_pos(a.p29.FEFO, b.p29.FEFO, 0, b.p30.GULE, b.p30.TS_IDX_5 );
  /*p30.WANA*/ c.p30.WANA = not(!b.p30.XYGO); if (b.p29.GUGY) c.p30.TS_LINE_0 = b.p30.WANA; c.p30.XYGO = tock_pos(a.p29.WABE, b.p29.WABE, 0, b.p30.XYGO, b.p30.TS_LINE_0);
  /*p30.WAXE*/ c.p30.WAXE = not(!b.p30.XYNA); if (b.p29.GUGY) c.p30.TS_LINE_1 = b.p30.WAXE; c.p30.XYNA = tock_pos(a.p29.WABE, b.p29.WABE, 0, b.p30.XYNA, b.p30.TS_LINE_1);
  /*p30.WABU*/ c.p30.WABU = not(!b.p30.XAKU); if (b.p29.GUGY) c.p30.TS_LINE_2 = b.p30.WABU; c.p30.XAKU = tock_pos(a.p29.WABE, b.p29.WABE, 0, b.p30.XAKU, b.p30.TS_LINE_2);
  /*p30.YPOZ*/ c.p30.YPOZ = not(!b.p30.YGUM); if (b.p29.GUGY) c.p30.TS_LINE_3 = b.p30.YPOZ; c.p30.YGUM = tock_pos(a.p29.WABE, b.p29.WABE, 0, b.p30.YGUM, b.p30.TS_LINE_3);
   
  /*p30.BEMO*/ c.p30.BEMO = not(!b.p30.CAJY); if (b.p29.DYDO) c.p30.TS_IDX_0  = b.p30.BEMO; c.p30.CAJY = tock_pos(a.p29.BYVY, b.p29.BYVY, 0, b.p30.CAJY, b.p30.TS_IDX_0 );
  /*p30.CYBY*/ c.p30.CYBY = not(!b.p30.CUZA); if (b.p29.DYDO) c.p30.TS_IDX_1  = b.p30.CYBY; c.p30.CUZA = tock_pos(a.p29.BYVY, b.p29.BYVY, 0, b.p30.CUZA, b.p30.TS_IDX_1 );
  /*p30.BETY*/ c.p30.BETY = not(!b.p30.COMA); if (b.p29.DYDO) c.p30.TS_IDX_2  = b.p30.BETY; c.p30.COMA = tock_pos(a.p29.BYVY, b.p29.BYVY, 0, b.p30.COMA, b.p30.TS_IDX_2 );
  /*p30.CEGY*/ c.p30.CEGY = not(!b.p30.CUFA); if (b.p29.DYDO) c.p30.TS_IDX_3  = b.p30.CEGY; c.p30.CUFA = tock_pos(a.p29.BYVY, b.p29.BYVY, 0, b.p30.CUFA, b.p30.TS_IDX_3 );
  /*p30.CELU*/ c.p30.CELU = not(!b.p30.CEBO); if (b.p29.DYDO) c.p30.TS_IDX_4  = b.p30.CELU; c.p30.CEBO = tock_pos(a.p29.BYVY, b.p29.BYVY, 0, b.p30.CEBO, b.p30.TS_IDX_4 );
  /*p30.CUBO*/ c.p30.CUBO = not(!b.p30.CADU); if (b.p29.DYDO) c.p30.TS_IDX_5  = b.p30.CUBO; c.p30.CADU = tock_pos(a.p29.BYVY, b.p29.BYVY, 0, b.p30.CADU, b.p30.TS_IDX_5 );
  /*p30.BEFE*/ c.p30.BEFE = not(!b.p30.ABUG); if (b.p29.DYDO) c.p30.TS_LINE_0 = b.p30.BEFE; c.p30.ABUG = tock_pos(a.p29.AHOF, b.p29.AHOF, 0, b.p30.ABUG, b.p30.TS_LINE_0);
  /*p30.BYRO*/ c.p30.BYRO = not(!b.p30.AMES); if (b.p29.DYDO) c.p30.TS_LINE_1 = b.p30.BYRO; c.p30.AMES = tock_pos(a.p29.AHOF, b.p29.AHOF, 0, b.p30.AMES, b.p30.TS_LINE_1);
  /*p30.BACO*/ c.p30.BACO = not(!b.p30.ABOP); if (b.p29.DYDO) c.p30.TS_LINE_2 = b.p30.BACO; c.p30.ABOP = tock_pos(a.p29.AHOF, b.p29.AHOF, 0, b.p30.ABOP, b.p30.TS_LINE_2);
  /*p30.AHUM*/ c.p30.AHUM = not(!b.p30.AROF); if (b.p29.DYDO) c.p30.TS_LINE_3 = b.p30.AHUM; c.p30.AROF = tock_pos(a.p29.AHOF, b.p29.AHOF, 0, b.p30.AROF, b.p30.TS_LINE_3);
   
  /*p30.DALO*/ c.p30.DALO = not(!b.p30.EKAP); if (b.p29.GYGY) c.p30.TS_IDX_0  = b.p30.DALO; c.p30.EKAP = tock_pos(a.p29.DYMO, b.p29.DYMO, 0, b.p30.EKAP, b.p30.TS_IDX_0 );
  /*p30.DALY*/ c.p30.DALY = not(!b.p30.ETAV); if (b.p29.GYGY) c.p30.TS_IDX_1  = b.p30.DALY; c.p30.ETAV = tock_pos(a.p29.DYMO, b.p29.DYMO, 0, b.p30.ETAV, b.p30.TS_IDX_1 );
  /*p30.DUZA*/ c.p30.DUZA = not(!b.p30.EBEX); if (b.p29.GYGY) c.p30.TS_IDX_2  = b.p30.DUZA; c.p30.EBEX = tock_pos(a.p29.DYMO, b.p29.DYMO, 0, b.p30.EBEX, b.p30.TS_IDX_2 );
  /*p30.WAGA*/ c.p30.WAGA = not(!b.p30.GORU); if (b.p29.GYGY) c.p30.TS_IDX_3  = b.p30.WAGA; c.p30.GORU = tock_pos(a.p29.DYMO, b.p29.DYMO, 0, b.p30.GORU, b.p30.TS_IDX_3 );
  /*p30.DYNY*/ c.p30.DYNY = not(!b.p30.ETYM); if (b.p29.GYGY) c.p30.TS_IDX_4  = b.p30.DYNY; c.p30.ETYM = tock_pos(a.p29.DYMO, b.p29.DYMO, 0, b.p30.ETYM, b.p30.TS_IDX_4 );
  /*p30.DOBO*/ c.p30.DOBO = not(!b.p30.EKOP); if (b.p29.GYGY) c.p30.TS_IDX_5  = b.p30.DOBO; c.p30.EKOP = tock_pos(a.p29.DYMO, b.p29.DYMO, 0, b.p30.EKOP, b.p30.TS_IDX_5 );
  /*p30.AWAT*/ c.p30.AWAT = not(!b.p30.ANED); if (b.p29.GYGY) c.p30.TS_LINE_0 = b.p30.AWAT; c.p30.ANED = tock_pos(a.p29.BUCY, b.p29.BUCY, 0, b.p30.ANED, b.p30.TS_LINE_0);
  /*p30.BACE*/ c.p30.BACE = not(!b.p30.ACEP); if (b.p29.GYGY) c.p30.TS_LINE_1 = b.p30.BACE; c.p30.ACEP = tock_pos(a.p29.BUCY, b.p29.BUCY, 0, b.p30.ACEP, b.p30.TS_LINE_1);
  /*p30.BODU*/ c.p30.BODU = not(!b.p30.ABUX); if (b.p29.GYGY) c.p30.TS_LINE_2 = b.p30.BODU; c.p30.ABUX = tock_pos(a.p29.BUCY, b.p29.BUCY, 0, b.p30.ABUX, b.p30.TS_LINE_2);
  /*p30.BUJA*/ c.p30.BUJA = not(!b.p30.ABEG); if (b.p29.GYGY) c.p30.TS_LINE_3 = b.p30.BUJA; c.p30.ABEG = tock_pos(a.p29.BUCY, b.p29.BUCY, 0, b.p30.ABEG, b.p30.TS_LINE_3);
   
  /*p30.DEZU*/ c.p30.DEZU = not(!b.p30.DAFU); if (b.p29.GYMA) c.p30.TS_IDX_0  = b.p30.DEZU; c.p30.DAFU = tock_pos(a.p29.FEKA, b.p29.FEKA, 0, b.p30.DAFU, b.p30.TS_IDX_0 );
  /*p30.EFUD*/ c.p30.EFUD = not(!b.p30.DEBA); if (b.p29.GYMA) c.p30.TS_IDX_1  = b.p30.EFUD; c.p30.DEBA = tock_pos(a.p29.FEKA, b.p29.FEKA, 0, b.p30.DEBA, b.p30.TS_IDX_1 );
  /*p30.DONY*/ c.p30.DONY = not(!b.p30.DUHA); if (b.p29.GYMA) c.p30.TS_IDX_2  = b.p30.DONY; c.p30.DUHA = tock_pos(a.p29.FEKA, b.p29.FEKA, 0, b.p30.DUHA, b.p30.TS_IDX_2 );
  /*p30.DOWA*/ c.p30.DOWA = not(!b.p30.DUNY); if (b.p29.GYMA) c.p30.TS_IDX_3  = b.p30.DOWA; c.p30.DUNY = tock_pos(a.p29.FEKA, b.p29.FEKA, 0, b.p30.DUNY, b.p30.TS_IDX_3 );
  /*p30.DYGO*/ c.p30.DYGO = not(!b.p30.DESE); if (b.p29.GYMA) c.p30.TS_IDX_4  = b.p30.DYGO; c.p30.DESE = tock_pos(a.p29.FEKA, b.p29.FEKA, 0, b.p30.DESE, b.p30.TS_IDX_4 );
  /*p30.ENAP*/ c.p30.ENAP = not(!b.p30.DEVY); if (b.p29.GYMA) c.p30.TS_IDX_5  = b.p30.ENAP; c.p30.DEVY = tock_pos(a.p29.FEKA, b.p29.FEKA, 0, b.p30.DEVY, b.p30.TS_IDX_5 );
  /*p30.ZYPO*/ c.p30.ZYPO = not(!b.p30.ZURY); if (b.p29.GYMA) c.p30.TS_LINE_0 = b.p30.ZYPO; c.p30.ZURY = tock_pos(a.p29.XYHA, b.p29.XYHA, 0, b.p30.ZURY, b.p30.TS_LINE_0);
  /*p30.ZEXE*/ c.p30.ZEXE = not(!b.p30.ZURO); if (b.p29.GYMA) c.p30.TS_LINE_1 = b.p30.ZEXE; c.p30.ZURO = tock_pos(a.p29.XYHA, b.p29.XYHA, 0, b.p30.ZURO, b.p30.TS_LINE_1);
  /*p30.YJEM*/ c.p30.YJEM = not(!b.p30.ZENE); if (b.p29.GYMA) c.p30.TS_LINE_2 = b.p30.YJEM; c.p30.ZENE = tock_pos(a.p29.XYHA, b.p29.XYHA, 0, b.p30.ZENE, b.p30.TS_LINE_2);
  /*p30.YWAV*/ c.p30.YWAV = not(!b.p30.ZYLU); if (b.p29.GYMA) c.p30.TS_LINE_3 = b.p30.YWAV; c.p30.ZYLU = tock_pos(a.p29.XYHA, b.p29.XYHA, 0, b.p30.ZYLU, b.p30.TS_LINE_3);
   
  /*p30.AXEC*/ c.p30.AXEC = not(!b.p30.BOXA); if (b.p29.FAME) c.p30.TS_IDX_0  = b.p30.AXEC; c.p30.BOXA = tock_pos(a.p29.BUZY, b.p29.BUZY, 0, b.p30.BOXA, b.p30.TS_IDX_0 );
  /*p30.CYRO*/ c.p30.CYRO = not(!b.p30.BUNA); if (b.p29.FAME) c.p30.TS_IDX_1  = b.p30.CYRO; c.p30.BUNA = tock_pos(a.p29.BUZY, b.p29.BUZY, 0, b.p30.BUNA, b.p30.TS_IDX_1 );
  /*p30.CUVU*/ c.p30.CUVU = not(!b.p30.BULU); if (b.p29.FAME) c.p30.TS_IDX_2  = b.p30.CUVU; c.p30.BULU = tock_pos(a.p29.BUZY, b.p29.BUZY, 0, b.p30.BULU, b.p30.TS_IDX_2 );
  /*p30.APON*/ c.p30.APON = not(!b.p30.BECA); if (b.p29.FAME) c.p30.TS_IDX_3  = b.p30.APON; c.p30.BECA = tock_pos(a.p29.BUZY, b.p29.BUZY, 0, b.p30.BECA, b.p30.TS_IDX_3 );
  /*p30.AFOZ*/ c.p30.AFOZ = not(!b.p30.BYHU); if (b.p29.FAME) c.p30.TS_IDX_4  = b.p30.AFOZ; c.p30.BYHU = tock_pos(a.p29.BUZY, b.p29.BUZY, 0, b.p30.BYHU, b.p30.TS_IDX_4 );
  /*p30.CUBE*/ c.p30.CUBE = not(!b.p30.BUHE); if (b.p29.FAME) c.p30.TS_IDX_5  = b.p30.CUBE; c.p30.BUHE = tock_pos(a.p29.BUZY, b.p29.BUZY, 0, b.p30.BUHE, b.p30.TS_IDX_5 );
  /*p30.ZABY*/ c.p30.ZABY = not(!b.p30.YKUK); if (b.p29.FAME) c.p30.TS_LINE_0 = b.p30.ZABY; c.p30.YKUK = tock_pos(a.p29.FUKE, b.p29.FUKE, 0, b.p30.YKUK, b.p30.TS_LINE_0);
  /*p30.ZUKE*/ c.p30.ZUKE = not(!b.p30.YLOV); if (b.p29.FAME) c.p30.TS_LINE_1 = b.p30.ZUKE; c.p30.YLOV = tock_pos(a.p29.FUKE, b.p29.FUKE, 0, b.p30.YLOV, b.p30.TS_LINE_1);
  /*p30.WUXE*/ c.p30.WUXE = not(!b.p30.XAZY); if (b.p29.FAME) c.p30.TS_LINE_2 = b.p30.WUXE; c.p30.XAZY = tock_pos(a.p29.FUKE, b.p29.FUKE, 0, b.p30.XAZY, b.p30.TS_LINE_2);
  /*p30.WERE*/ c.p30.WERE = not(!b.p30.XOSY); if (b.p29.FAME) c.p30.TS_LINE_3 = b.p30.WERE; c.p30.XOSY = tock_pos(a.p29.FUKE, b.p29.FUKE, 0, b.p30.XOSY, b.p30.TS_LINE_3);
   
  /*p30.YHAL*/ c.p30.YHAL = not(!b.p30.XUFO); if (b.p29.FADO) c.p30.TS_IDX_0  = b.p30.YHAL; c.p30.XUFO = tock_pos(a.p29.WUFA, b.p29.WUFA, 0, b.p30.XUFO, b.p30.TS_IDX_0 );
  /*p30.YRAD*/ c.p30.YRAD = not(!b.p30.XUTE); if (b.p29.FADO) c.p30.TS_IDX_1  = b.p30.YRAD; c.p30.XUTE = tock_pos(a.p29.WUFA, b.p29.WUFA, 0, b.p30.XUTE, b.p30.TS_IDX_1 );
  /*p30.XYRA*/ c.p30.XYRA = not(!b.p30.XOTU); if (b.p29.FADO) c.p30.TS_IDX_2  = b.p30.XYRA; c.p30.XOTU = tock_pos(a.p29.WUFA, b.p29.WUFA, 0, b.p30.XOTU, b.p30.TS_IDX_2 );
  /*p30.YNEV*/ c.p30.YNEV = not(!b.p30.XYFE); if (b.p29.FADO) c.p30.TS_IDX_3  = b.p30.YNEV; c.p30.XYFE = tock_pos(a.p29.WUFA, b.p29.WUFA, 0, b.p30.XYFE, b.p30.TS_IDX_3 );
  /*p30.ZOJY*/ c.p30.ZOJY = not(!b.p30.YZOR); if (b.p29.FADO) c.p30.TS_IDX_4  = b.p30.ZOJY; c.p30.YZOR = tock_pos(a.p29.WUFA, b.p29.WUFA, 0, b.p30.YZOR, b.p30.TS_IDX_4 );
  /*p30.ZARO*/ c.p30.ZARO = not(!b.p30.YBER); if (b.p29.FADO) c.p30.TS_IDX_5  = b.p30.ZARO; c.p30.YBER = tock_pos(a.p29.WUFA, b.p29.WUFA, 0, b.p30.YBER, b.p30.TS_IDX_5 );
  /*p30.CAWO*/ c.p30.CAWO = not(!b.p30.DEWU); if (b.p29.FADO) c.p30.TS_LINE_0 = b.p30.CAWO; c.p30.DEWU = tock_pos(a.p29.FAKA, b.p29.FAKA, 0, b.p30.DEWU, b.p30.TS_LINE_0);
  /*p30.BYME*/ c.p30.BYME = not(!b.p30.CANA); if (b.p29.FADO) c.p30.TS_LINE_1 = b.p30.BYME; c.p30.CANA = tock_pos(a.p29.FAKA, b.p29.FAKA, 0, b.p30.CANA, b.p30.TS_LINE_1);
  /*p30.COHO*/ c.p30.COHO = not(!b.p30.DYSY); if (b.p29.FADO) c.p30.TS_LINE_2 = b.p30.COHO; c.p30.DYSY = tock_pos(a.p29.FAKA, b.p29.FAKA, 0, b.p30.DYSY, b.p30.TS_LINE_2);
  /*p30.GATE*/ c.p30.GATE = not(!b.p30.FOFO); if (b.p29.FADO) c.p30.TS_LINE_3 = b.p30.GATE; c.p30.FOFO = tock_pos(a.p29.FAKA, b.p29.FAKA, 0, b.p30.FOFO, b.p30.TS_LINE_3);
   
  /*p30.YGUS*/ c.p30.YGUS = tock_pos(a.p29.GENY, b.p29.GENY, 0, b.p30.YGUS, b.p30.TS_IDX_0);
  /*p30.YSOK*/ c.p30.YSOK = tock_pos(a.p29.GENY, b.p29.GENY, 0, b.p30.YSOK, b.p30.TS_IDX_1);
  /*p30.YZEP*/ c.p30.YZEP = tock_pos(a.p29.GENY, b.p29.GENY, 0, b.p30.YZEP, b.p30.TS_IDX_2);
  /*p30.WYTE*/ c.p30.WYTE = tock_pos(a.p29.GENY, b.p29.GENY, 0, b.p30.WYTE, b.p30.TS_IDX_3);
  /*p30.ZONY*/ c.p30.ZONY = tock_pos(a.p29.GENY, b.p29.GENY, 0, b.p30.ZONY, b.p30.TS_IDX_4);
  /*p30.YWAK*/ c.p30.YWAK = tock_pos(a.p29.GENY, b.p29.GENY, 0, b.p30.YWAK, b.p30.TS_IDX_5);
  /*p30.FYHY*/ c.p30.FYHY = tock_pos(a.p29.ENOB, b.p29.ENOB, 0, b.p30.FYHY, b.p30.TS_LINE_0);
  /*p30.GYHO*/ c.p30.GYHO = tock_pos(a.p29.ENOB, b.p29.ENOB, 0, b.p30.GYHO, b.p30.TS_LINE_1);
  /*p30.BOZU*/ c.p30.BOZU = tock_pos(a.p29.ENOB, b.p29.ENOB, 0, b.p30.BOZU, b.p30.TS_LINE_2);
  /*p30.CUFO*/ c.p30.CUFO = tock_pos(a.p29.ENOB, b.p29.ENOB, 0, b.p30.CUFO, b.p30.TS_LINE_3);

  /*p30.ZETU*/ if (b.p29.FURO) c.p30.TS_IDX_0  = b.p30.YGUS;
  /*p30.ZECE*/ if (b.p29.FURO) c.p30.TS_IDX_1  = b.p30.YSOK;
  /*p30.ZAVE*/ if (b.p29.FURO) c.p30.TS_IDX_2  = b.p30.YZEP;
  /*p30.WOKO*/ if (b.p29.FURO) c.p30.TS_IDX_3  = b.p30.WYTE;
  /*p30.ZUMU*/ if (b.p29.FURO) c.p30.TS_IDX_4  = b.p30.ZONY;
  /*p30.ZEDY*/ if (b.p29.FURO) c.p30.TS_IDX_5  = b.p30.YWAK;
  /*p30.GOFO*/ if (b.p29.FURO) c.p30.TS_LINE_0 = b.p30.FYHY;
  /*p30.WEHE*/ if (b.p29.FURO) c.p30.TS_LINE_1 = b.p30.GYHO;
  /*p30.AJAL*/ if (b.p29.FURO) c.p30.TS_LINE_2 = b.p30.BOZU;
  /*p30.BUKY*/ if (b.p29.FURO) c.p30.TS_LINE_3 = b.p30.CUFO;
}