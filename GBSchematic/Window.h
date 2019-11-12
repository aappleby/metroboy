#pragma once
#include "Schematics.h"

struct Window {
public:

  void tick(const Resets& rst, const LCD& lcd, Vram& vram, const Clocks& clk);

  //----------
  // outputs

  bool MOFU;
  bool NYDY;
  bool NYXU;
  bool VYPO;
  bool ROXY;
  bool LYRY;
  bool SECA;
  bool POVA;
  bool MYVO;
  bool SYLO;
  bool LENA;
  bool MOSU;
  bool MYMA;
  bool NETA;
  bool PORE;
  bool POTU;
  bool XUHA;

  //bool WUKO; // controls something window

  // some 8 bit counter
  reg VYNO,VUJO,VYMU,TUFU,TAXA,TOZO,TATE,TEKE;

private:
  
  // probably mislabeled CUBA_Q
  bool CUBA1;
  
  //----------
  // registers

  reg SOBU;
  reg SUDA; // delayed SOBU

  reg WIN_Y_MATCH; /*SARY*/
  reg NOPA;
  reg WIN_X_MATCH; /*PYCO*/
  reg WIN_X_MATCH_DELAY; /*NUNU*/

  reg RYKU;
  reg ROGA;
  reg RUBU;
  reg NYZE;
  reg PUXA;

  reg RYFA;
  reg SOVY;
  reg RENE;
  reg LYZU;
  reg LOVY;

  // big address bus, bottom right

  // looks like a 3 bit counter
  reg LAXU,MESU,NYVA;

  // some 5 bit counter
  reg WYKA,WODY,WOBO,WYKO,XOLO;
};
