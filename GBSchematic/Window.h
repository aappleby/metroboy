#pragma once
#include "Schematics.h"

struct Resets;

struct Window {
public:

  void tick(const Resets& rst);

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
  bool TAVA;
  bool XUHA;

  //bool WUKO; // controls something window

  reg VYNO;
  reg VUJO;
  reg VYMU;

private:
  
  // probably mislabeled CUBA_Q
  bool CUBA1;
  
  //----------
  // registers

  reg SOBU;
  reg SUDA;
  reg SARY;
  reg NOPA;
  reg PYCO;
  reg NUNU;

  reg RYKU;
  reg ROGA;
  reg RUBU;
  reg NYZE;
  reg PUXA;

  reg RYFA;
  reg SOVY;
  reg RENE;
  reg LYZU;
  reg LAXU;
  reg MESU;
  reg NYVA;
  reg LOVY;

  // big address bus, bottom right

  reg TUFU;
  reg TAXA;
  reg TOZO;
  reg TATE;
  reg TEKE;
  reg WYKA;
  reg WODY;
  reg WOBO;
  reg WYKO;
  reg XOLO;
};
