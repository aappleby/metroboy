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

void Main::print(SignalState s) {
  text.add_char(s.val ? '1' : '0');
}

void Main::render_frame() {
  base::render_frame();
  text.begin_frame();

  text.set_pal(0, 0.0, 0.0, 0.0, 0.3);
  text.set_pal(1, 0.8, 0.8, 0.8, 1.0);
  text.set_pal(2, 0.6, 1.0, 0.6, 1.0);

  text.dprintf(" ----- SYS_REG -----\n");
  text.dprintf("PHASE    %08d\n", phase_counter);
  text.dprintf("RST      %08d\n", RST);
  text.dprintf("CLKIN_A  %d\n", CLKIN_A);
  text.dprintf("CLKIN_B  %d\n", CLKIN_B);
  text.dprintf("CLKREQ   %d\n", CLKREQ);

  text.dprintf(" ----- CLK_REG -----\n");
  text.dprintf("PHAZ     \2%d%d%d%d\1\n",
               gb.clk_reg.PHAZ_ABCDxxxx.a.val,
               gb.clk_reg.PHAZ_xBCDExxx.a.val,
               gb.clk_reg.PHAZ_xxCDEFxx.a.val,
               gb.clk_reg.PHAZ_xxxDEFGx.a.val);

  text.dprintf(" ----- BUS_REG -----\n");
  text.dprintf("BOOT_BIT %d\n", gb.bus_reg.BOOT_BIT.a.val);
  text.dprintf("SOTO_DBG %d\n", gb.bus_reg.SOTO_DBG.a.val);

  text.dprintf("ADDR_LATCH ");
  print(gb.bus_reg.ADDR_LATCH_00.a);
  print(gb.bus_reg.ADDR_LATCH_01.a);
  print(gb.bus_reg.ADDR_LATCH_02.a);
  print(gb.bus_reg.ADDR_LATCH_03.a);
  print(gb.bus_reg.ADDR_LATCH_04.a);
  print(gb.bus_reg.ADDR_LATCH_05.a);
  print(gb.bus_reg.ADDR_LATCH_06.a);
  print(gb.bus_reg.ADDR_LATCH_07.a);
  print(gb.bus_reg.ADDR_LATCH_08.a);
  print(gb.bus_reg.ADDR_LATCH_09.a);
  print(gb.bus_reg.ADDR_LATCH_10.a);
  print(gb.bus_reg.ADDR_LATCH_11.a);
  print(gb.bus_reg.ADDR_LATCH_12.a);
  print(gb.bus_reg.ADDR_LATCH_13.a);
  print(gb.bus_reg.ADDR_LATCH_14.a);
  text.newline();

  text.dprintf("DATA_LATCH ");
  print(gb.bus_reg.DATA_LATCH_00.a);
  print(gb.bus_reg.DATA_LATCH_01.a);
  print(gb.bus_reg.DATA_LATCH_02.a);
  print(gb.bus_reg.DATA_LATCH_03.a);
  print(gb.bus_reg.DATA_LATCH_04.a);
  print(gb.bus_reg.DATA_LATCH_05.a);
  print(gb.bus_reg.DATA_LATCH_06.a);
  print(gb.bus_reg.DATA_LATCH_07.a);
  text.newline();

  text.dprintf(" ----- LCDC -----\n");
  text.dprintf("BGEN   %d\n", gb.cfg_reg.LCDC_BGEN.a.val);
  text.dprintf("SPEN   %d\n", gb.cfg_reg.LCDC_SPEN.a.val);   
  text.dprintf("SPSIZE %d\n", gb.cfg_reg.LCDC_SPSIZE.a.val);
  text.dprintf("BGMAP  %d\n", gb.cfg_reg.LCDC_BGMAP.a.val);
  text.dprintf("BGTILE %d\n", gb.cfg_reg.LCDC_BGTILE.a.val);
  text.dprintf("WINEN  %d\n", gb.cfg_reg.LCDC_WINEN.a.val);
  text.dprintf("WINMAP %d\n", gb.cfg_reg.LCDC_WINMAP.a.val);
  text.dprintf("EN     %d\n", gb.cfg_reg.LCDC_EN.a.val);

  text.dprintf(" ----- LCD CFG -----\n");
  text.dprintf("SCY  0x%02x\n", gb.cfg_reg.get_scy());
  text.dprintf("SCX  0x%02x\n", gb.cfg_reg.get_scx());
  text.dprintf("LYC  0x%02x\n", gb.cfg_reg.get_lyc());
  text.dprintf("BGP  0x%02x\n", gb.cfg_reg.get_bgp());
  text.dprintf("OBP0 0x%02x\n", gb.cfg_reg.get_obp0());
  text.dprintf("OBP1 0x%02x\n", gb.cfg_reg.get_obp1());
  text.dprintf("WY   0x%02x\n", gb.cfg_reg.get_wy());
  text.dprintf("WX   0x%02x\n", gb.cfg_reg.get_wx());

  text.dprintf(" ----- DBG REG ----- \n");
  text.dprintf("FF60_0 %d\n", gb.dbg_reg.FF60_0.a.val);
  text.dprintf("FF60_1 %d\n", gb.dbg_reg.FF60_1.a.val);

  text.dprintf(" ----- DMA REG -----\n");
  text.dprintf("FROM_CPU5_SYNC   %d\n", gb.dma_reg.FROM_CPU5_SYNC.a.val);
  text.dprintf("REG_DMA_RUNNING  %d\n", gb.dma_reg.REG_DMA_RUNNING.a.val);
  text.dprintf("DMA_DONE_SYNC    %d\n", gb.dma_reg.DMA_DONE_SYNC.a.val);
  text.dprintf("REG_DMA_EN_d0    %d\n", gb.dma_reg.REG_DMA_EN_d0.a.val);  
  text.dprintf("REG_DMA_EN_d4    %d\n", gb.dma_reg.REG_DMA_EN_d4.a.val);
  text.dprintf("LATCH_DMA_ENn_d0 %d\n", gb.dma_reg.LATCH_DMA_ENn_d0.a.val);
  text.dprintf("LATCH_DMA_EN_d4  %d\n", gb.dma_reg.LATCH_DMA_EN_d4.a.val);
  text.dprintf("DMA ADDR LO      0x%02x\n", gb.dma_reg.get_addr_lo());
  text.dprintf("DMA ADDR HI      0x%02x\n", gb.dma_reg.get_addr_hi());

  text.dprintf(" ----- INT REG -----\n");
  text.dprintf("FF0F_0  %d\n", gb.int_reg.FF0F_0.a.val);
  text.dprintf("FF0F_1  %d\n", gb.int_reg.FF0F_1.a.val);
  text.dprintf("FF0F_2  %d\n", gb.int_reg.FF0F_2.a.val);
  text.dprintf("FF0F_3  %d\n", gb.int_reg.FF0F_3.a.val);
  text.dprintf("FF0F_4  %d\n", gb.int_reg.FF0F_4.a.val);
  text.dprintf("FF0F_L0 %d\n", gb.int_reg.FF0F_L0.a.val);
  text.dprintf("FF0F_L1 %d\n", gb.int_reg.FF0F_L1.a.val);
  text.dprintf("FF0F_L2 %d\n", gb.int_reg.FF0F_L2.a.val);
  text.dprintf("FF0F_L3 %d\n", gb.int_reg.FF0F_L3.a.val);
  text.dprintf("FF0F_L4 %d\n", gb.int_reg.FF0F_L4.a.val);

  text.dprintf(" ----- JOY REG -----\n");
  text.dprintf("JP_GLITCH0  %d\n", gb.joy_reg.JP_GLITCH0  .a.val);
  text.dprintf("JP_GLITCH1  %d\n", gb.joy_reg.JP_GLITCH1  .a.val);
  text.dprintf("JP_GLITCH2  %d\n", gb.joy_reg.JP_GLITCH2  .a.val);
  text.dprintf("JP_GLITCH3  %d\n", gb.joy_reg.JP_GLITCH3  .a.val);
  text.dprintf("JOYP_RA     %d\n", gb.joy_reg.JOYP_RA     .a.val);
  text.dprintf("JOYP_LB     %d\n", gb.joy_reg.JOYP_LB     .a.val);
  text.dprintf("JOYP_UC     %d\n", gb.joy_reg.JOYP_UC     .a.val);
  text.dprintf("JOYP_DS     %d\n", gb.joy_reg.JOYP_DS     .a.val);
  text.dprintf("JOYP_UDLR   %d\n", gb.joy_reg.JOYP_UDLR   .a.val);
  text.dprintf("JOYP_ABCS   %d\n", gb.joy_reg.JOYP_ABCS   .a.val);
  text.dprintf("DBG_FF00_D6 %d\n", gb.joy_reg.DBG_FF00_D6 .a.val);
  text.dprintf("DBG_FF00_D7 %d\n", gb.joy_reg.DBG_FF00_D7 .a.val);
  text.dprintf("JOYP_L0     %d\n", gb.joy_reg.JOYP_L0     .a.val);
  text.dprintf("JOYP_L1     %d\n", gb.joy_reg.JOYP_L1     .a.val);
  text.dprintf("JOYP_L2     %d\n", gb.joy_reg.JOYP_L2     .a.val);
  text.dprintf("JOYP_L3     %d\n", gb.joy_reg.JOYP_L3     .a.val);
  text.dprintf("WAKE_CPU    %d\n", gb.joy_reg.WAKE_CPU    .a.val);

  text.render(4, 4, 1.0);

  text.dprintf(" ----- LCD REG -----\n");
  text.dprintf("LCD X       %d\n", gb.lcd_reg.x());
  text.dprintf("LCD Y       %d\n", gb.lcd_reg.y());
  text.dprintf("RUTU_NEW_LINE_d0  %d\n", gb.lcd_reg.RUTU_NEW_LINE_d0  .a.val);
  text.dprintf("VID_LINE_d4       %d\n", gb.lcd_reg.VID_LINE_d4       .a.val);
  text.dprintf("NYPE_NEW_LINE_d4  %d\n", gb.lcd_reg.NYPE_NEW_LINE_d4  .a.val);
  text.dprintf("VID_LINE_d6       %d\n", gb.lcd_reg.VID_LINE_d6       .a.val);
  text.dprintf("LINE_153_d4       %d\n", gb.lcd_reg.LINE_153_d4       .a.val);
  text.dprintf("POPU_IN_VBLANK_d4 %d\n", gb.lcd_reg.POPU_IN_VBLANK_d4 .a.val);
  text.dprintf("LINE_STROBE       %d\n", gb.lcd_reg.LINE_STROBE       .a.val);
  text.dprintf("X_8_SYNC          %d\n", gb.lcd_reg.X_8_SYNC          .a.val);
  text.dprintf("CPEN_LATCH        %d\n", gb.lcd_reg.CPEN_LATCH        .a.val);
  text.dprintf("POME              %d\n", gb.lcd_reg.POME              .a.val);
  text.dprintf("RUJU              %d\n", gb.lcd_reg.RUJU              .a.val);
  text.dprintf("VSYNC_OUTn        %d\n", gb.lcd_reg.VSYNC_OUTn        .a.val);
  text.dprintf("LINE_EVEN         %d\n", gb.lcd_reg.LINE_EVEN         .a.val);
  text.dprintf("FRAME_EVEN        %d\n", gb.lcd_reg.FRAME_EVEN        .a.val);

  text.render(200, 4, 1.0);

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
