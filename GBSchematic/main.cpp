#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include <string>
#include <vector>
#include <math.h>

#include <GL/gl3w.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include <SDL.h>
#include <imgui.h>
#include <examples/imgui_impl_sdl.h>
#include <examples/imgui_impl_opengl3.h>

using namespace Schematics;

#pragma warning(disable:4189)
//#pragma warning(disable:4702)

//----------------------------------------

#if 0
const GLchar* viz_vert_source = R"(
#version 460

uniform vec4 screen_size = vec4(1920.0, 1080.0, 1.0 / 1920.0, 1.0 / 1080.0);
uniform vec4 quad_pos = vec4(128.4, 128.0, 32.0, 32.0);

layout(location = 0) in  vec2 vpos;
layout(location = 0) out vec2 vtex;

vec4 screen_to_norm(float x, float y) {
  x = x * screen_size.z * 2.0 - 1.0;
  y = -y * screen_size.w * 2.0 + 1.0;
  return vec4(x, y, 1.0, 1.0);
}

void main() {
  float x = quad_pos.x;
  float y = quad_pos.y;
  float w = quad_pos.z;
  float h = quad_pos.w;

  x = mix(x, x + w, vpos.x);
  y = mix(y, y + h, vpos.y);

  gl_Position = screen_to_norm(x, y);
  vtex = vpos;
}
)";

//----------

const GLchar* viz_frag_source = R"(
#version 460

uniform sampler2D atlas_sampler;

layout(location = 0) in vec2 ftex;
layout(location = 0) out vec4 frag;

void main() {
  int t = int(texture(tex, ftex).r * 255.0);

  bool val = (t & 0x01) == 0x01;
  bool hiz = (t & 0x02) == 0x02;
  bool clk = (t & 0x04) == 0x04;
  bool set = (t & 0x08) == 0x08;
  bool rst = (t & 0x10) == 0x10;
  bool err = (t & 0x20) == 0x20;


  frag = vec4(val, hiz, clk, 1.0);
}
)";
#endif

//-----------------------------------------------------------------------------

void Main::init() {
  base::init();

  text.init(fb_width, fb_height);

  gb_tex = create_texture(64, 64, 2);

  RST = 0;
  CLKIN_A = 1;
  CLKIN_B = 0;
  CLKREQ = 1;

  gb.cfg_reg.LCDC_EN.a.val = 1;

  //----------------------------------------

  //viz_prog = compile_shader("gb_viz", viz_vert_source, viz_frag_source);
}

void Main::close() {
  base::close();
}

//-----------------------------------------------------------------------------

int main(int argc, char** argv)
{
  Main m;
  return m.main(argc, argv);
  //m.init();
  //return 0;
}

//-----------------------------------------------------------------------------

void Main::begin_frame() {
  base::begin_frame();
}

//-----------------------------------------------------------------------------

void Main::update() {
  base::update();

  gb.cpu_preset(CLKREQ, 0x0000, 0x00);

  if ((frame_count % 1) == 0) {

    while(1) {
      CLKIN_B = phase_counter & 1;
      gb.sys_preset(RST, CLKIN_A, CLKIN_B);
      gb.cpu_preset(CLKREQ, 0x0000, 0x00);
      gb.ext_preset();
      gb.ser_preset();
      gb.joy_preset();
      gb.vram_preset();

      //gb.pin_init();
      gb.tick_everything();

      // FIXME still don't know who drives these, so we always set them to 0.
      gb.joy_pins.P10_B.set(0);
      gb.joy_pins.P11_B.set(0);
      gb.joy_pins.P12_B.set(0);
      gb.joy_pins.P13_B.set(0);

      // this is going to break once the gb is actually driving the data bus
      gb.cpu_pins.set_data(true, 0);

      bool changed = gb.commit_everything();
      if (!changed) {
        phase_counter++;
        break;
      }
    }

  }
  
  {
    uint8_t* pix = new uint8_t[64 * 64 * 4];
    memset(pix, 0, 64*64*4);
    memcpy(pix, &gb, sizeof(gb));
    update_texture(gb_tex, 64, 64, 2, pix);
    delete [] pix;
  }
}

//-----------------------------------------------------------------------------

void Main::render_frame() {
  base::render_frame();
  text.begin_frame();

  text.set_pal(0, 0.0, 0.0, 0.0, 0.5);
  text.set_pal(1, 0.8, 0.8, 0.8, 1.0);
  text.set_pal(2, 0.6, 1.0, 0.6, 1.0);

  text.dprintf(" ----- SYS_REG -----\n");
  text.dprintf("PHASE    %08d\n", phase_counter);

  int p = phase_counter & 7;
  text.dprintf("PHASE    %c%c%c%c%c%c%c%c\n",
               p == 0 ? 'A' : '_',
               p == 1 ? 'B' : '_',
               p == 2 ? 'C' : '_',
               p == 3 ? 'D' : '_',
               p == 4 ? 'E' : '_',
               p == 5 ? 'F' : '_',
               p == 6 ? 'G' : '_',
               p == 7 ? 'H' : '_');

  text.dprintf("RST      %08d\n", RST);
  text.dprintf("CLKIN_A  %d\n", CLKIN_A);
  text.dprintf("CLKIN_B  %d\n", CLKIN_B);
  text.dprintf("CLKREQ   %d\n", CLKREQ);

  gb.clk_reg.dump(text);
  gb.bus_reg.dump(text);
  gb.cfg_reg.dump(text);
  gb.dbg_reg.dump(text);
  gb.dma_reg.dump(text);
  gb.int_reg.dump(text);
  gb.joy_reg.dump(text);
  text.render(192 * 0, 4, 1.0);

  gb.lcd_reg.dump(text);
  gb.pxp_reg.dump(text);
  gb.rst_reg.dump(text);
  gb.ser_reg.dump(text);
  gb.spr_reg.dump(text);
  gb.sst_reg.dump(text);
  gb.tim_reg.dump(text);
  text.render(192 * 1 + 4, 4, 1.0);

  gb.vid_reg.dump(text);
  gb.vclk_reg.dump(text);
  gb.oam_reg.dump(text);
  text.render(192 * 2 + 4, 4, 1.0);

  gb.oam_pins.dump(text);
  gb.cpu_pins.dump(text);
  gb.sys_pins.dump(text);
  gb.vram_pins.dump(text);
  gb.ser_pins.dump(text);
  gb.lcd_pins.dump(text);
  gb.joy_pins.dump(text);
  text.render(192 * 3 + 4, 4, 1.0);

  gb.ext_pins.dump(text);
  gb.wave_pins.dump(text);
  text.render(192 * 4 + 4, 4, 1.0);

  /*
  glUseProgram(viz_prog);

  glUniform4f(glGetUniformLocation(viz_prog, "screen_size"),
              (float)fb_width, (float)fb_height, 1.0f / fb_width, 1.0f / fb_height);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, gb_tex);
  glUniform1i(glGetUniformLocation(viz_prog, "tex"), 0);

  glUniform4f(glGetUniformLocation(viz_prog, "quad_pos"),
              (float)128, (float)128, (float)64 * 8, (float)64 * 8);

  glBindVertexArray(quad_buf.vao);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  */

  //blit(font_tex, 512, 128, 256, 256);
}

//-----------------------------------------------------------------------------

void Main::render_ui() {
  ImGui::Begin("GB Sim Stats");
  ImGui::Text("now   %f\n", now);
  ImGui::Text("phase %d\n", phase_counter);
  ImGui::Text("freq  %f\n", phase_counter / now);
  ImGui::End();

  base::render_ui();
}

//-----------------------------------------------------------------------------

void Main::end_frame() {
  base::end_frame();
}

//-----------------------------------------------------------------------------



















//-----------------------------------------------------------------------------

void Main::sim_phase() {
}



//-----------------------------------------------------------------------------

int main2(int /*argc*/, char** /*argv*/) {
#if 0
  //test_clock_phases();
  //test_timer();

  TestGB gb;
  //memset(&gb, 0, sizeof(gb));

  uint64_t timeA = SDL_GetPerformanceCounter();

  int phase_counter = 0;

  bool RST = 1;
  bool CLKIN_A = 0;
  bool CLKIN_B = 0;
  bool CLKREQ = 0;

  printf("reset\n");
  RST = 1;
  for (int i = 0; i < 16; i++) {
    sim_phase(gb, RST, CLKIN_A, CLKIN_B, CLKREQ);
    printf("\n");
  }

  printf("!reset\n");
  RST = 0;
  for (int i = 0; i < 16; i++) {
    sim_phase(gb, RST, CLKIN_A, CLKIN_B, CLKREQ);
    printf("\n");
  }

  printf("clkgood\n");
  CLKIN_A = 1;
  for (int i = 0; i < 16; i++) {
    sim_phase(gb, RST, CLKIN_A, CLKIN_B, CLKREQ);
    printf("\n");
  }

  printf("clkreq\n");
  CLKREQ = 1;
  for (int i = 0; i < 16; i++) {
    sim_phase(gb, RST, CLKIN_A, CLKIN_B, CLKREQ);
    printf("\n");
  }

  printf("lcd %d %d\n", gb.lcd_reg.x(), gb.lcd_reg.y());

  for (int i = 0; i < 1024; i++) {
    sim_phase(gb, RST, CLKIN_A, CLKIN_B, CLKREQ);
  }

  for (int i = 0; i < 1024; i++) {
    sim_phase(gb, RST, CLKIN_A, CLKIN_B, CLKREQ);
  }

  printf("lcd %d %d\n", gb.lcd_reg.x(), gb.lcd_reg.y());

  uint64_t timeB = SDL_GetPerformanceCounter();
  //gb.commit_everything();

  printf("end\n");

  double elapsed = double(timeB - timeA) / SDL_GetPerformanceFrequency();

  printf("freq %lld\n", SDL_GetPerformanceFrequency());
  printf("time %lld\n", (timeB - timeA));
  printf("time %f\n", 1000.0 * elapsed);
  printf("freq %f\n", phase_counter / elapsed);
#endif

#if 0

  TestGB gb;
  gb.reset();

  //gb.sys_reg.CPU_RAW_RD = 1;

  //gb.sim(456*2*152);

  for (int i = 0; i < 912*2; i++) {
    gb.sim(1);

    ClkSignals clk_sig1 = gb.clk_reg1.signals(gb.sys_reg);
    RstSignals rst_sig1 = gb.rst_reg.rst_signals(gb.sys_reg, clk_sig1);
    VclkSignals vid_clk_sig = gb.vclk_reg.signals();
    LcdSignals lcd_sig = gb.lcd_reg.signals(vid_rst);
    BusSignals bus_ctl = BusSignals::tick(gb.sys_reg, clk_sig1);

    Decoder dcd = Decoder::tick(gb.bus, clk_sig1, gb.sys_reg.BOOT_BIT, gb.sys_reg.MODE_DBG2, gb.sys_reg.ADDR_VALID);


    printf("%03d:%03d:%03d %03d %d %03d %03d %d%d%d%d%d%d %d%d%d%d%d%d %d%d%d%d\n",
           gb.sys_reg.phase_count / (912*154),
           (gb.sys_reg.phase_count / 912) % 154,
           gb.sys_reg.phase_count % 912,
           gb.sys_reg.phase(),
           vid_clk_sig.TALU_xBCDExxx,
           gb.lcd_reg.x(),
           gb.lcd_reg.y(),
           (bool)gb.lcd_reg.NEW_LINE_d0a,
           (bool)gb.lcd_reg.VID_LINE_d4,
           (bool)gb.lcd_reg.NEW_LINE_d4a,
           (bool)gb.lcd_reg.VID_LINE_d6,
           (bool)gb.lcd_reg.LINE_153_d4,
           (bool)gb.lcd_reg.VBLANK_d4,
           lcd_sig.VBLANK_d4b,
           lcd_sig.VID_LINE_TRIG_d4n,
           lcd_sig.VID_LINE_TRIG_d4a,
           lcd_sig.VID_LINE_TRIG_d4p,
           lcd_sig.VID_LINE_TRIG_d4c,
           lcd_sig.XAHY_VID_LINE_TRIG_d4n,
           bus_ctl.TEDO_CPURD,
           bus_ctl.TAPU_CPUWR,
           bus_ctl.ASOT_CPURD,
           bus_ctl.CUPA_CPUWR);


 

    
    if ((i % 8) == 7) printf("\n");

  }
#endif

  return 0;
}

//-----------------------------------------------------------------------------
