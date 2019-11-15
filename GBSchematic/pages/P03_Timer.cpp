#include "P03_Timer.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

const std::vector<SignalData> P03_Timer::signals() {
  return
  {
    SignalData("-----TAC-----"),
    SignalData("TAC_0", offsetof(P03_Timer, TAC_0)),
    SignalData("TAC_1", offsetof(P03_Timer, TAC_1)),
    SignalData("TAC_2", offsetof(P03_Timer, TAC_2)),

    SignalData("-----TMA-----"),
    SignalData("TMA_0", offsetof(P03_Timer, TMA_0)),
    SignalData("TMA_1", offsetof(P03_Timer, TMA_1)),
    SignalData("TMA_2", offsetof(P03_Timer, TMA_2)),
    SignalData("TMA_3", offsetof(P03_Timer, TMA_3)),
    SignalData("TMA_4", offsetof(P03_Timer, TMA_4)),
    SignalData("TMA_5", offsetof(P03_Timer, TMA_5)),
    SignalData("TMA_6", offsetof(P03_Timer, TMA_6)),
    SignalData("TMA_7", offsetof(P03_Timer, TMA_7)),

    SignalData("-----TIMA-----"),
    SignalData("TIMA_0", offsetof(P03_Timer, TIMA_0)),
    SignalData("TIMA_1", offsetof(P03_Timer, TIMA_1)),
    SignalData("TIMA_2", offsetof(P03_Timer, TIMA_2)),
    SignalData("TIMA_3", offsetof(P03_Timer, TIMA_3)),
    SignalData("TIMA_4", offsetof(P03_Timer, TIMA_4)),
    SignalData("TIMA_5", offsetof(P03_Timer, TIMA_5)),
    SignalData("TIMA_6", offsetof(P03_Timer, TIMA_6)),
    SignalData("TIMA_7", offsetof(P03_Timer, TIMA_7)),

    SignalData("-----Int-----"),
    SignalData("NYDU",   offsetof(P03_Timer, NYDU)),
    SignalData("INT_TIMER",   offsetof(P03_Timer, INT_TIMER)),

    SignalData("-----Clock mux-----"),
    SignalData("UVYR",   offsetof(P03_Timer, UVYR)),
    SignalData("UKAP",   offsetof(P03_Timer, UKAP)),
    SignalData("UBOT",   offsetof(P03_Timer, UBOT)),
    SignalData("TEKO",   offsetof(P03_Timer, TEKO)),
    SignalData("TECY",   offsetof(P03_Timer, TECY)),
    SignalData("SOGU",   offsetof(P03_Timer, SOGU)),
  };
}

