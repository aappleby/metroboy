#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P31_SpriteXMatchers::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  c.p31.WYNO = latch_pos(b.p28.CLK3, b.p31.WYNO, b.OAM_A_D4);
  c.p31.CYRA = latch_pos(b.p28.CLK3, b.p31.CYRA, b.OAM_A_D5);
  c.p31.ZUVE = latch_pos(b.p28.CLK3, b.p31.ZUVE, b.OAM_A_D6);
  c.p31.ECED = latch_pos(b.p28.CLK3, b.p31.ECED, b.OAM_A_D7);
  c.p31.XYKY = latch_pos(b.p28.CLK3, b.p31.XYKY, b.OAM_A_D0);
  c.p31.YRUM = latch_pos(b.p28.CLK3, b.p31.YRUM, b.OAM_A_D1);
  c.p31.YSEX = latch_pos(b.p28.CLK3, b.p31.YSEX, b.OAM_A_D2);
  c.p31.YVEL = latch_pos(b.p28.CLK3, b.p31.YVEL, b.OAM_A_D3);

  /*p31.XACA*/ if (b.p28.OAM_A_CPU_RD) c.D0 = b.p31.XYKY;
  /*p31.XAGU*/ if (b.p28.OAM_A_CPU_RD) c.D1 = b.p31.YRUM;
  /*p31.XEPU*/ if (b.p28.OAM_A_CPU_RD) c.D2 = b.p31.YSEX;
  /*p31.XYGU*/ if (b.p28.OAM_A_CPU_RD) c.D3 = b.p31.YVEL;
  /*p31.XUNA*/ if (b.p28.OAM_A_CPU_RD) c.D4 = b.p31.WYNO;
  /*p31.DEVE*/ if (b.p28.OAM_A_CPU_RD) c.D5 = b.p31.CYRA;
  /*p31.ZEHA*/ if (b.p28.OAM_A_CPU_RD) c.D6 = b.p31.ZUVE;
  /*p31.FYRA*/ if (b.p28.OAM_A_CPU_RD) c.D7 = b.p31.ECED;

  c.p31.XEGA    = not(b.p25.COTA);
  c.p31.GOMO    = tock_pos(a.p31.XEGA, b.p31.XEGA, 0, b.p31.GOMO, b.p31.WYNO);
  c.p31.BAXO    = tock_pos(a.p31.XEGA, b.p31.XEGA, 0, b.p31.BAXO, b.p31.CYRA);
  c.p31.FLIP_Yn = tock_pos(a.p31.XEGA, b.p31.XEGA, 0, b.p31.FLIP_Yn, b.p31.ZUVE);
  c.p31.DEPO    = tock_pos(a.p31.XEGA, b.p31.XEGA, 0, b.p31.DEPO, b.p31.ECED);
  c.p31.YLOR    = tock_pos(a.p31.XEGA, b.p31.XEGA, 0, b.p31.YLOR, b.p31.XYKY);
  c.p31.ZYTY    = tock_pos(a.p31.XEGA, b.p31.XEGA, 0, b.p31.ZYTY, b.p31.YRUM);
  c.p31.ZYVE    = tock_pos(a.p31.XEGA, b.p31.XEGA, 0, b.p31.ZYVE, b.p31.YSEX);
  c.p31.ZEZY    = tock_pos(a.p31.XEGA, b.p31.XEGA, 0, b.p31.ZEZY, b.p31.YVEL);

  c.p31.COSE = not(!b.p31.GOMO);
  c.p31.AROP = not(!b.p31.BAXO);
  c.p31.XATU = not(!b.p31.FLIP_Yn);
  c.p31.BADY = not(!b.p31.DEPO);
  c.p31.ZAGO = not(!b.p31.YLOR);
  c.p31.ZOCY = not(!b.p31.ZYTY);
  c.p31.YPUR = not(!b.p31.ZYVE);
  c.p31.YVOK = not(!b.p31.ZEZY);

  // CHECK CLK/RESET WIRES

  // matcher 1
  c.p31.WELO = tock_pos(a.p29.FUXU, b.p29.FUXU, b.p29.DYNA, b.p31.WELO, b.p31.COSE);
  c.p31.XUNY = tock_pos(a.p29.FUXU, b.p29.FUXU, b.p29.DYNA, b.p31.XUNY, b.p31.AROP);
  c.p31.WOTE = tock_pos(a.p29.FUXU, b.p29.FUXU, b.p29.DYNA, b.p31.WOTE, b.p31.XATU);
  c.p31.XAKO = tock_pos(a.p29.FUXU, b.p29.FUXU, b.p29.DYNA, b.p31.XAKO, b.p31.BADY);
  c.p31.XEPE = tock_pos(a.p29.FUXU, b.p29.FUXU, b.p29.DYNA, b.p31.XEPE, b.p31.ZAGO);
  c.p31.YLAH = tock_pos(a.p29.FUXU, b.p29.FUXU, b.p29.DYNA, b.p31.YLAH, b.p31.ZOCY);
  c.p31.ZOLA = tock_pos(a.p29.FUXU, b.p29.FUXU, b.p29.DYNA, b.p31.ZOLA, b.p31.YPUR);
  c.p31.ZULU = tock_pos(a.p29.FUXU, b.p29.FUXU, b.p29.DYNA, b.p31.ZULU, b.p31.YVOK);

  c.p31.WOJU = xor(b.p31.WELO, b.p21.X4n);
  c.p31.YFUN = xor(b.p31.XUNY, b.p21.X5n);
  c.p31.WYZA = xor(b.p31.WOTE, b.p21.X6n);
  c.p31.YPUK = xor(b.p31.XAKO, b.p21.X7n);
  c.p31.ZOGY = xor(b.p31.XEPE, b.p21.X0n);
  c.p31.ZEBA = xor(b.p31.YLAH, b.p21.X1n);
  c.p31.ZAHA = xor(b.p31.ZOLA, b.p21.X2n);
  c.p31.ZOKY = xor(b.p31.ZULU, b.p21.X3n);
  c.p31.XEBA = nor(b.p31.WOJU, b.p31.YFUN, b.p31.WYZA, b.p31.YPUK);
  c.p31.ZAKO = nor(b.p31.ZOGY, b.p31.ZEBA, b.p31.ZAHA, b.p31.ZOKY);

  // matcher 2
  c.p31.XOMY = tock_pos(a.p29.YFAG, b.p29.YFAG, b.p29.WUPA, b.p31.XOMY, b.p31.COSE);
  c.p31.WUHA = tock_pos(a.p29.YFAG, b.p29.YFAG, b.p29.WUPA, b.p31.WUHA, b.p31.AROP);
  c.p31.WYNA = tock_pos(a.p29.YFAG, b.p29.YFAG, b.p29.WUPA, b.p31.WYNA, b.p31.XATU);
  c.p31.WECO = tock_pos(a.p29.YFAG, b.p29.YFAG, b.p29.WUPA, b.p31.WECO, b.p31.BADY);
  c.p31.XOLY = tock_pos(a.p29.YFAG, b.p29.YFAG, b.p29.WUPA, b.p31.XOLY, b.p31.ZAGO);
  c.p31.XYBA = tock_pos(a.p29.YFAG, b.p29.YFAG, b.p29.WUPA, b.p31.XYBA, b.p31.ZOCY);
  c.p31.XABE = tock_pos(a.p29.YFAG, b.p29.YFAG, b.p29.WUPA, b.p31.XABE, b.p31.YPUR);
  c.p31.XEKA = tock_pos(a.p29.YFAG, b.p29.YFAG, b.p29.WUPA, b.p31.XEKA, b.p31.YVOK);

  c.p31.YVAP = xor(b.p31.XOMY, b.p21.X4n);
  c.p31.XENY = xor(b.p31.WUHA, b.p21.X5n);
  c.p31.XAVU = xor(b.p31.WYNA, b.p21.X6n);
  c.p31.XEVA = xor(b.p31.WECO, b.p21.X7n);
  c.p31.YHOK = xor(b.p31.XOLY, b.p21.X0n);
  c.p31.YCAH = xor(b.p31.XYBA, b.p21.X1n);
  c.p31.YDAJ = xor(b.p31.XABE, b.p21.X2n);
  c.p31.YVUZ = xor(b.p31.XEKA, b.p21.X3n);
  c.p31.YWOS = nor(b.p31.YVAP, b.p31.XENY, b.p31.XAVU, b.p31.XEVA);
  c.p31.ZURE = nor(b.p31.YHOK, b.p31.YCAH, b.p31.YDAJ, b.p31.YVUZ);

  // matcher 3
  c.p31.FAZU = tock_pos(a.p29.GECY, b.p29.GECY, b.p29.GAFY, b.p31.FAZU, b.p31.COSE);
  c.p31.FAXE = tock_pos(a.p29.GECY, b.p29.GECY, b.p29.GAFY, b.p31.FAXE, b.p31.AROP);
  c.p31.EXUK = tock_pos(a.p29.GECY, b.p29.GECY, b.p29.GAFY, b.p31.EXUK, b.p31.XATU);
  c.p31.FEDE = tock_pos(a.p29.GECY, b.p29.GECY, b.p29.GAFY, b.p31.FEDE, b.p31.BADY);
  c.p31.ERAZ = tock_pos(a.p29.GECY, b.p29.GECY, b.p29.GAFY, b.p31.ERAZ, b.p31.ZAGO);
  c.p31.EPUM = tock_pos(a.p29.GECY, b.p29.GECY, b.p29.GAFY, b.p31.EPUM, b.p31.ZOCY);
  c.p31.EROL = tock_pos(a.p29.GECY, b.p29.GECY, b.p29.GAFY, b.p31.EROL, b.p31.YPUR);
  c.p31.EHYN = tock_pos(a.p29.GECY, b.p29.GECY, b.p29.GAFY, b.p31.EHYN, b.p31.YVOK);

  c.p31.EJOT = xor(b.p31.FAZU, b.p21.X4n);
  c.p31.ESAJ = xor(b.p31.FAXE, b.p21.X5n);
  c.p31.DUCU = xor(b.p31.EXUK, b.p21.X6n);
  c.p31.EWUD = xor(b.p31.FEDE, b.p21.X7n);
  c.p31.DUSE = xor(b.p31.ERAZ, b.p21.X0n);
  c.p31.DAGU = xor(b.p31.EPUM, b.p21.X1n);
  c.p31.DYZE = xor(b.p31.EROL, b.p21.X2n);
  c.p31.DESO = xor(b.p31.EHYN, b.p21.X3n);
  c.p31.DAJE = nor(b.p31.EJOT, b.p31.ESAJ, b.p31.DUCU, b.p31.EWUD);
  c.p31.CYCO = nor(b.p31.DUSE, b.p31.DAGU, b.p31.DYZE, b.p31.DESO);

  // matcher 4
  c.p31.DAKE = tock_pos(a.p29.ASYS, b.p29.ASYS, b.p29.DOKU, b.p31.DAKE, b.p31.COSE);
  c.p31.CESO = tock_pos(a.p29.ASYS, b.p29.ASYS, b.p29.DOKU, b.p31.CESO, b.p31.AROP);
  c.p31.DYFU = tock_pos(a.p29.ASYS, b.p29.ASYS, b.p29.DOKU, b.p31.DYFU, b.p31.XATU);
  c.p31.CUSY = tock_pos(a.p29.ASYS, b.p29.ASYS, b.p29.DOKU, b.p31.CUSY, b.p31.BADY);
  c.p31.DANY = tock_pos(a.p29.ASYS, b.p29.ASYS, b.p29.DOKU, b.p31.DANY, b.p31.ZAGO);
  c.p31.DUKO = tock_pos(a.p29.ASYS, b.p29.ASYS, b.p29.DOKU, b.p31.DUKO, b.p31.ZOCY);
  c.p31.DESU = tock_pos(a.p29.ASYS, b.p29.ASYS, b.p29.DOKU, b.p31.DESU, b.p31.YPUR);
  c.p31.DAZO = tock_pos(a.p29.ASYS, b.p29.ASYS, b.p29.DOKU, b.p31.DAZO, b.p31.YVOK);

  c.p31.COLA = xor(b.p31.DAKE, b.p21.X4n);
  c.p31.BOBA = xor(b.p31.CESO, b.p21.X5n);
  c.p31.COLU = xor(b.p31.DYFU, b.p21.X6n);
  c.p31.BAHU = xor(b.p31.CUSY, b.p21.X7n);
  c.p31.EDYM = xor(b.p31.DANY, b.p21.X0n);
  c.p31.EMYB = xor(b.p31.DUKO, b.p21.X1n);
  c.p31.EBEF = xor(b.p31.DESU, b.p21.X2n);
  c.p31.EWOK = xor(b.p31.DAZO, b.p21.X3n);
  c.p31.CYVY = nor(b.p31.COLA, b.p31.BOBA, b.p31.COLU, b.p31.BAHU);
  c.p31.EWAM = nor(b.p31.EDYM, b.p31.EMYB, b.p31.EBEF, b.p31.EWOK);

  // matcher 5
  c.p31.ZOLY = tock_pos(a.p29.ZAPE, b.p29.ZAPE, b.p29.XAHO, b.p31.ZOLY, b.p31.COSE);
  c.p31.ZOGO = tock_pos(a.p29.ZAPE, b.p29.ZAPE, b.p29.XAHO, b.p31.ZOGO, b.p31.AROP);
  c.p31.ZECU = tock_pos(a.p29.ZAPE, b.p29.ZAPE, b.p29.XAHO, b.p31.ZECU, b.p31.XATU);
  c.p31.ZESA = tock_pos(a.p29.ZAPE, b.p29.ZAPE, b.p29.XAHO, b.p31.ZESA, b.p31.BADY);
  c.p31.YCOL = tock_pos(a.p29.ZAPE, b.p29.ZAPE, b.p29.XAHO, b.p31.YCOL, b.p31.ZAGO);
  c.p31.YRAC = tock_pos(a.p29.ZAPE, b.p29.ZAPE, b.p29.XAHO, b.p31.YRAC, b.p31.ZOCY);
  c.p31.YMEM = tock_pos(a.p29.ZAPE, b.p29.ZAPE, b.p29.XAHO, b.p31.YMEM, b.p31.YPUR);
  c.p31.YVAG = tock_pos(a.p29.ZAPE, b.p29.ZAPE, b.p29.XAHO, b.p31.YVAG, b.p31.YVOK);

  c.p31.ZARE = xor(b.p31.ZOLY, b.p21.X4n);
  c.p31.ZEMU = xor(b.p31.ZOGO, b.p21.X5n);
  c.p31.ZYGO = xor(b.p31.ZECU, b.p21.X6n);
  c.p31.ZUZY = xor(b.p31.ZESA, b.p21.X7n);
  c.p31.XOSU = xor(b.p31.YCOL, b.p21.X0n);
  c.p31.ZUVU = xor(b.p31.YRAC, b.p21.X1n);
  c.p31.XUCO = xor(b.p31.YMEM, b.p21.X2n);
  c.p31.ZULO = xor(b.p31.YVAG, b.p21.X3n);
  c.p31.YWAP = nor(b.p31.ZARE, b.p31.ZEMU, b.p31.ZYGO, b.p31.ZUZY);
  c.p31.YDOT = nor(b.p31.XOSU, b.p31.ZUVU, b.p31.XUCO, b.p31.ZULO);

  // matcher 6
  c.p31.YBED = tock_pos(a.p29.WUNU, b.p29.WUNU, b.p29.WOFO, b.p31.YBED, b.p31.COSE);
  c.p31.ZALA = tock_pos(a.p29.WUNU, b.p29.WUNU, b.p29.WOFO, b.p31.ZALA, b.p31.AROP);
  c.p31.WYDE = tock_pos(a.p29.WUNU, b.p29.WUNU, b.p29.WOFO, b.p31.WYDE, b.p31.XATU);
  c.p31.XEPA = tock_pos(a.p29.WUNU, b.p29.WUNU, b.p29.WOFO, b.p31.XEPA, b.p31.BADY);
  c.p31.WEDU = tock_pos(a.p29.WUNU, b.p29.WUNU, b.p29.WOFO, b.p31.WEDU, b.p31.ZAGO);
  c.p31.YGAJ = tock_pos(a.p29.WUNU, b.p29.WUNU, b.p29.WOFO, b.p31.YGAJ, b.p31.ZOCY);
  c.p31.ZYJO = tock_pos(a.p29.WUNU, b.p29.WUNU, b.p29.WOFO, b.p31.ZYJO, b.p31.YPUR);
  c.p31.XURY = tock_pos(a.p29.WUNU, b.p29.WUNU, b.p29.WOFO, b.p31.XURY, b.p31.YVOK);

  c.p31.ZYKU = xor(b.p31.YBED, b.p21.X4n);
  c.p31.ZYPU = xor(b.p31.ZALA, b.p21.X5n);
  c.p31.XAHA = xor(b.p31.WYDE, b.p21.X6n);
  c.p31.ZEFE = xor(b.p31.XEPA, b.p21.X7n);
  c.p31.XEJU = xor(b.p31.WEDU, b.p21.X0n);
  c.p31.ZATE = xor(b.p31.YGAJ, b.p21.X1n);
  c.p31.ZAKU = xor(b.p31.ZYJO, b.p21.X2n);
  c.p31.YBOX = xor(b.p31.XURY, b.p21.X3n);
  c.p31.YKOK = nor(b.p31.ZYKU, b.p31.ZYPU, b.p31.XAHA, b.p31.ZEFE);
  c.p31.YNAZ = nor(b.p31.XEJU, b.p31.ZATE, b.p31.ZAKU, b.p31.YBOX);

  // matcher 7
  c.p31.EZUF = tock_pos(a.p29.CEXU, b.p29.CEXU, b.p29.WUZO, b.p31.EZUF, b.p31.COSE);
  c.p31.ENAD = tock_pos(a.p29.CEXU, b.p29.CEXU, b.p29.WUZO, b.p31.ENAD, b.p31.AROP);
  c.p31.EBOW = tock_pos(a.p29.CEXU, b.p29.CEXU, b.p29.WUZO, b.p31.EBOW, b.p31.XATU);
  c.p31.FYCA = tock_pos(a.p29.CEXU, b.p29.CEXU, b.p29.WUZO, b.p31.FYCA, b.p31.BADY);
  c.p31.GAVY = tock_pos(a.p29.CEXU, b.p29.CEXU, b.p29.WUZO, b.p31.GAVY, b.p31.ZAGO);
  c.p31.GYPU = tock_pos(a.p29.CEXU, b.p29.CEXU, b.p29.WUZO, b.p31.GYPU, b.p31.ZOCY);
  c.p31.GADY = tock_pos(a.p29.CEXU, b.p29.CEXU, b.p29.WUZO, b.p31.GADY, b.p31.YPUR);
  c.p31.GAZA = tock_pos(a.p29.CEXU, b.p29.CEXU, b.p29.WUZO, b.p31.GAZA, b.p31.YVOK);

  c.p31.DUZE = xor(b.p31.EZUF, b.p21.X4n);
  c.p31.DAGA = xor(b.p31.ENAD, b.p21.X5n);
  c.p31.DAWU = xor(b.p31.EBOW, b.p21.X6n);
  c.p31.EJAW = xor(b.p31.FYCA, b.p21.X7n);
  c.p31.GOHO = xor(b.p31.GAVY, b.p21.X0n);
  c.p31.GASU = xor(b.p31.GYPU, b.p21.X1n);
  c.p31.GABU = xor(b.p31.GADY, b.p21.X2n);
  c.p31.GAFE = xor(b.p31.GAZA, b.p21.X3n);
  c.p31.DAMA = nor(b.p31.DUZE, b.p31.DAGA, b.p31.DAWU, b.p31.EJAW);
  c.p31.FEHA = nor(b.p31.GOHO, b.p31.GASU, b.p31.GABU, b.p31.GAFE);

  // matcher 8
  c.p31.YPOD = tock_pos(a.p29.WEME, b.p29.WEME, b.p29.DOSY, b.p31.YPOD, b.p31.COSE);
  c.p31.YROP = tock_pos(a.p29.WEME, b.p29.WEME, b.p29.DOSY, b.p31.YROP, b.p31.AROP);
  c.p31.YNEP = tock_pos(a.p29.WEME, b.p29.WEME, b.p29.DOSY, b.p31.YNEP, b.p31.XATU);
  c.p31.YZOF = tock_pos(a.p29.WEME, b.p29.WEME, b.p29.DOSY, b.p31.YZOF, b.p31.BADY);
  c.p31.XUVY = tock_pos(a.p29.WEME, b.p29.WEME, b.p29.DOSY, b.p31.XUVY, b.p31.ZAGO);
  c.p31.XERE = tock_pos(a.p29.WEME, b.p29.WEME, b.p29.DOSY, b.p31.XERE, b.p31.ZOCY);
  c.p31.XUZO = tock_pos(a.p29.WEME, b.p29.WEME, b.p29.DOSY, b.p31.XUZO, b.p31.YPUR);
  c.p31.XEXA = tock_pos(a.p29.WEME, b.p29.WEME, b.p29.DOSY, b.p31.XEXA, b.p31.YVOK);

  c.p31.ZYWU = xor(b.p31.YPOD, b.p21.X4n);
  c.p31.ZUZA = xor(b.p31.YROP, b.p21.X5n);
  c.p31.ZEJO = xor(b.p31.YNEP, b.p21.X6n);
  c.p31.ZEDA = xor(b.p31.YZOF, b.p21.X7n);
  c.p31.YMAM = xor(b.p31.XUVY, b.p21.X0n);
  c.p31.YTYP = xor(b.p31.XERE, b.p21.X1n);
  c.p31.YFOP = xor(b.p31.XUZO, b.p21.X2n);
  c.p31.YVAC = xor(b.p31.XEXA, b.p21.X3n);
  c.p31.YTUB = nor(b.p31.ZYWU, b.p31.ZUZA, b.p31.ZEJO, b.p31.ZEDA);
  c.p31.YLEV = nor(b.p31.YMAM, b.p31.YTYP, b.p31.YFOP, b.p31.YVAC);

  // matcher 9
  c.p31.CYWE = tock_pos(a.p29.CYLA, b.p29.CYLA, b.p29.EJAD, b.p31.CYWE, b.p31.COSE);
  c.p31.DYBY = tock_pos(a.p29.CYLA, b.p29.CYLA, b.p29.EJAD, b.p31.DYBY, b.p31.AROP);
  c.p31.DURY = tock_pos(a.p29.CYLA, b.p29.CYLA, b.p29.EJAD, b.p31.DURY, b.p31.XATU);
  c.p31.CUVY = tock_pos(a.p29.CYLA, b.p29.CYLA, b.p29.EJAD, b.p31.CUVY, b.p31.BADY);
  c.p31.FUSA = tock_pos(a.p29.CYLA, b.p29.CYLA, b.p29.EJAD, b.p31.FUSA, b.p31.ZAGO);
  c.p31.FAXA = tock_pos(a.p29.CYLA, b.p29.CYLA, b.p29.EJAD, b.p31.FAXA, b.p31.ZOCY);
  c.p31.FOZY = tock_pos(a.p29.CYLA, b.p29.CYLA, b.p29.EJAD, b.p31.FOZY, b.p31.YPUR);
  c.p31.FESY = tock_pos(a.p29.CYLA, b.p29.CYLA, b.p29.EJAD, b.p31.FESY, b.p31.YVOK);

  c.p31.BAZY = xor(b.p31.CYWE, b.p21.X4n);
  c.p31.CYLE = xor(b.p31.DYBY, b.p21.X5n);
  c.p31.CEVA = xor(b.p31.DURY, b.p21.X6n);
  c.p31.BUMY = xor(b.p31.CUVY, b.p21.X7n);
  c.p31.GUZO = xor(b.p31.FUSA, b.p21.X0n);
  c.p31.GOLA = xor(b.p31.FAXA, b.p21.X1n);
  c.p31.GEVE = xor(b.p31.FOZY, b.p21.X2n);
  c.p31.GUDE = xor(b.p31.FESY, b.p21.X3n);
  c.p31.COGY = nor(b.p31.BAZY, b.p31.CYLE, b.p31.CEVA, b.p31.BUMY);
  c.p31.FYMA = nor(b.p31.GUZO, b.p31.GOLA, b.p31.GEVE, b.p31.GUDE);

  // matcher 10
  c.p31.DUHY = tock_pos(a.p29.CACU, b.p29.CACU, b.p29.GAMY, b.p31.DUHY, b.p31.COSE);
  c.p31.EJUF = tock_pos(a.p29.CACU, b.p29.CACU, b.p29.GAMY, b.p31.EJUF, b.p31.AROP);
  c.p31.ENOR = tock_pos(a.p29.CACU, b.p29.CACU, b.p29.GAMY, b.p31.ENOR, b.p31.XATU);
  c.p31.DEPY = tock_pos(a.p29.CACU, b.p29.CACU, b.p29.GAMY, b.p31.DEPY, b.p31.BADY);
  c.p31.FOKA = tock_pos(a.p29.CACU, b.p29.CACU, b.p29.GAMY, b.p31.FOKA, b.p31.ZAGO);
  c.p31.FYTY = tock_pos(a.p29.CACU, b.p29.CACU, b.p29.GAMY, b.p31.FYTY, b.p31.ZOCY);
  c.p31.FUBY = tock_pos(a.p29.CACU, b.p29.CACU, b.p29.GAMY, b.p31.FUBY, b.p31.YPUR);
  c.p31.GOXU = tock_pos(a.p29.CACU, b.p29.CACU, b.p29.GAMY, b.p31.GOXU, b.p31.YVOK);

  c.p31.CEKO = xor(b.p31.DUHY, b.p21.X4n);
  c.p31.DETY = xor(b.p31.EJUF, b.p21.X5n);
  c.p31.DOZO = xor(b.p31.ENOR, b.p21.X6n);
  c.p31.CONY = xor(b.p31.DEPY, b.p21.X7n);
  c.p31.FUZU = xor(b.p31.FOKA, b.p21.X0n);
  c.p31.FESO = xor(b.p31.FYTY, b.p21.X1n);
  c.p31.FOKY = xor(b.p31.FUBY, b.p21.X2n);
  c.p31.FYVA = xor(b.p31.GOXU, b.p21.X3n);
  c.p31.CEHU = nor(b.p31.CEKO, b.p31.DETY, b.p31.DOZO, b.p31.CONY);
  c.p31.EKES = nor(b.p31.FUZU, b.p31.FESO, b.p31.FOKY, b.p31.FYVA);
}