#include "AppLib/Console.h"

#include <stdio.h> /* standard input/output */
#include <stdint.h>

#ifdef _MSC_VER

#include <windows.h> /* for HANDLE type, and console funcs */
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

void dprintf(const char* format, ...) {
  char source_buf[1024];
  va_list args;
  va_start (args, format);
  vsnprintf (source_buf, 1024, format, args);
  va_end (args);
  OutputDebugStringA(source_buf);
}

void Console::init() {
  hin  = GetStdHandle(STD_INPUT_HANDLE);
  hout = GetStdHandle(STD_OUTPUT_HANDLE);

  DWORD mode = 0;
  GetConsoleMode(hin, &mode);
  mode &= ~ENABLE_LINE_INPUT;
  mode |= ENABLE_PROCESSED_INPUT;
  mode |= ENABLE_MOUSE_INPUT;
  SetConsoleMode(hin, mode);

  CONSOLE_CURSOR_INFO cci;
  cci.dwSize = 25;
  cci.bVisible = FALSE;
  SetConsoleCursorInfo(hout, &cci);
}

void Console::begin_frame() {
  DWORD Events;
  GetNumberOfConsoleInputEvents(hin, &Events);
  if (Events) {
    INPUT_RECORD InputRecord;
    while(Events) {
      ReadConsoleInput(hin, &InputRecord, 1, &Events);
      if (InputRecord.EventType == MOUSE_EVENT) {
        mouse_x = InputRecord.Event.MouseEvent.dwMousePosition.X;
        mouse_y = InputRecord.Event.MouseEvent.dwMousePosition.Y;
      }
      Events = 0;
    }
  }

  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(hout, &csbi);
  short new_screen_w = (csbi.srWindow.Right - csbi.srWindow.Left) + 1;
  short new_screen_h = (csbi.srWindow.Bottom - csbi.srWindow.Top)+ 1;

  if (new_screen_w != screen_w || new_screen_h != screen_h) {
    screen_w = new_screen_w;
    screen_h = new_screen_h;
    SetConsoleScreenBufferSize(hout, {screen_w, screen_h});
    delete[] back;
    back = new unsigned int[screen_w * screen_h];
  }

  memset(back, 0, screen_w * screen_h * 4);
}

void Console::end_frame() {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(hout, &csbi);
  WriteConsoleOutputA(hout, (CHAR_INFO*)back, {screen_w, screen_h}, {0,0}, &csbi.srWindow);
  DwmFlush();
  frame++;
}

void Console::plot(int x, int y, int color, char c) {
  if (x >= 0 && x < screen_w && y >= 0 && y < screen_h) {
    back[x + y * screen_w] = (color << 16) | c;
  }
}

void Console::print_at(int x, int y, int color, const char* format, ...) {
  char source_buf[1024];
  va_list args;
  va_start (args, format);
  int len = vsnprintf (source_buf, 1024, format, args);
  va_end (args);
  for (int i = 0; i < len; i++) plot(x + i, y, color, source_buf[i]);
}

#endif