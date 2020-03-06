#include "metroboy_main.h"

#include "Audio.h"
#include "MetroBoy.h"
#include "Assembler.h"

#include "test_codegen.h"
#include "test_micro.h"
#include "test_mooneye.h"
#include "test_wpol.h"
#include "test_screenshot.h"

#ifdef _MSC_VER
#include <include/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include <imgui.h>
#include <examples/imgui_impl_sdl.h>
#include <examples/imgui_impl_opengl3.h>
#include <glm/glm.hpp>

extern const uint32_t gb_colors[];
extern uint8_t rom_buf[];
extern uint8_t vram_dump[];

void run_test(const std::string& prefix, const std::string& name);

//----------------------------------------

struct BlitMapUniforms {
  vec4 screen_size;
  vec4 quad_pos;
  vec4 quad_tex;
  vec4 pal[4];
  int  which_map;
  int  use_map;
  int  alt_map;
  int  pad1;
};

//----------------------------------------

const char* blit_map_hdr = R"(
#version 460

layout(std140, binding = 0) uniform BlitMapUniforms {
  vec4 screen_size2;
  vec4 quad_pos2;
  vec4 quad_tex2;
  vec4 pal2[4];
  int  which_map2;
  int  use_map2;
  int  alt_map2;
  int  pad2;
};

layout(std140, binding = 1) uniform vramBuffer {
  uvec4 tiles[384]; // 6k of tile data
  uvec4 maps[128];   // 2k of map data
};

)";

//----------------------------------------

const char* blit_map_vert_src = R"(

layout(location = 0) in  vec2 vpos;
layout(location = 0) out vec2 vtex;

vec4 screen_to_norm(float x, float y) {
  x *= screen_size2.z;
  y *= screen_size2.w;

  x = x * 2.0 - 1.0;
  y = (1.0 - y) * 2.0 - 1.0;
  return vec4(x, y, 1.0, 1.0);
}

void main() {
  float x = quad_pos2.x;
  float y = quad_pos2.y;
  float w = quad_pos2.z;
  float h = quad_pos2.w;

  x = mix(x, x + w, vpos.x);
  y = mix(y, y + h, vpos.y);

  gl_Position = screen_to_norm(x, y);

  vtex.x = mix(quad_tex2.x, quad_tex2.x + quad_tex2.z, vpos.x);
  vtex.y = mix(quad_tex2.y, quad_tex2.y + quad_tex2.w, vpos.y);
}

)";

//----------------------------------------

const char* blit_map_frag_src = R"(

layout(location = 0) in  vec2 ftex;
layout(location = 0) out vec4 frag;

uint decode_tile2(uint tile_index, uint tile_x, uint tile_y) {
  uint flat_index = tile_index * 16 + tile_y * 2;
  uvec4 packed_uvec4 = tiles[flat_index / 16];
  uint packed_tile = packed_uvec4[(flat_index >> 2) & 3];
  vec4 temp = unpackUnorm4x8(packed_tile) * 255.0;

  uint pix_a = ((tile_y & 1) == 0) ? int(temp[0]) : int(temp[2]);
  uint pix_b = ((tile_y & 1) == 0) ? int(temp[1]) : int(temp[3]);

  pix_a = (pix_a >> (7 - tile_x)) & 1;
  pix_b = (pix_b >> (7 - tile_x)) & 1;

  return pix_a + (2 * pix_b);
}

void main() {
  uint pix_x = int(ftex.x) & 0xFF;
  uint pix_y = int(ftex.y) & 0xFF;

  uint map_x = pix_x >> 3;
  uint map_y = pix_y >> 3;

  uint tile_x = pix_x & 7;
  uint tile_y = pix_y & 7;

  if (bool(use_map2)) {
    uint flat_map_index = map_y * 32 + map_x;
    if (bool(which_map2)) flat_map_index += 1024;

    uvec4 packed_map_uvec4 = maps[flat_map_index >> 4];
    uint  packed_map_uint  = packed_map_uvec4[(flat_map_index >> 2) & 3];
    vec4  unpacked_map     = unpackUnorm4x8(packed_map_uint);
    uint tile_index = uint(unpacked_map[flat_map_index & 3] * 255.0);

    if (bool(alt_map2) && (tile_index < 128)) tile_index += 256;

    frag = pal2[decode_tile2(tile_index, tile_x, tile_y)];
  }
  else {
    uint tile_index = map_y * 16 + map_x;
    frag = pal2[decode_tile2(tile_index, tile_x, tile_y)];
  }
}
)";


//-----------------------------------------------------------------------------

void MetroBoyApp::init() {
  AppBase::init();

  gb_tex = create_texture(160, 144);

  uint32_t* trace = new uint32_t[456 * 154];
  trace_tex = create_texture(456, 154);
  update_texture(trace_tex, 456, 154, trace);
  delete [] trace;

  audio_init();

  //run_microtests();
  //run_screenshot_tests();
  //run_mooneye_acceptance();
  //run_wpol_acceptance();
  //run_mealybug_tests();
  //return 0;

  //---------

  //load("oh"); // broken eye
  //load("pocket");
  //load("gejmboj");
  //load("LinksAwakening");
  //load("Prehistorik Man (U)");
  //load("SML");
  //load("tetris");

  //load("cpu_instrs");
  //load("instr_timing");

  //load("microtests/build/dmg", "poweron_000_div");

  load("microtests/build/dmg", "minimal");

  runmode = STEP_CYCLE;
  //runmode = RUN_FAST;
  //runmode = RUN_VSYNC;

  keyboard_state = SDL_GetKeyboardState(nullptr);

  //----------------------------------------

  blit_map_prog = compile_shader(blit_map_hdr, blit_map_vert_src, blit_map_frag_src);

  //----------------------------------------

  const int vram_size = 8192;

  glGenBuffers(1, &vram_ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, vram_ubo);
  //glBufferStorage(GL_UNIFORM_BUFFER, vram_size, nullptr, GL_DYNAMIC_STORAGE_BIT);
  glBufferData(GL_UNIFORM_BUFFER, vram_size, nullptr, GL_DYNAMIC_DRAW);

  grid_painter.init();

  view.min.x = 0;
  view.max.x = fb_width;
  view.min.y = 0;
  view.max.y = fb_height;
  view.screen_size.x = fb_width;
  view.screen_size.y = fb_height;
  view_smooth = view;
  view_snap = view;

  glGenBuffers(1, &blit_map_ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, blit_map_ubo);
  //glNamedBufferStorage(blit_map_ubo, sizeof(BlitMapUniforms), nullptr, GL_DYNAMIC_STORAGE_BIT);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(BlitMapUniforms), nullptr, GL_DYNAMIC_DRAW);

  {
    int temp = -3;
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &temp);
    printf("GL_MAX_UNIFORM_BLOCK_SIZE %d\n", temp);
  }
};

//-----------------------------------------------------------------------------

void MetroBoyApp::blit_map() {
  BlitMapUniforms blit_map_uniforms = {
    .screen_size = {(float)fb_width, (float)fb_height, 1.0f / fb_width, 1.0f / fb_height},
    .quad_pos = {},
    .quad_tex = {},
    .pal = {
      {0.48, 0.48, 0.48, 1.0},
      {0.36, 0.36, 0.36, 1.0},
      {0.24, 0.24, 0.24, 1.0},
      {0.12, 0.12, 0.12, 1.0},
    },
    .which_map = 0,
    .use_map = 0,
    .alt_map = 0,
    .pad1 = 0,
  };

  //----------

  const int vram_size = 8192;
  //glNamedBufferSubData(vram_ubo, 0, vram_size, vram_dump);
  glBindBuffer(GL_UNIFORM_BUFFER, vram_ubo);
  glBufferData(GL_UNIFORM_BUFFER, vram_size, vram_dump, GL_DYNAMIC_DRAW);

  //----------

  glUseProgram(blit_map_prog);
  glBindVertexArray(quad_vao);
  glBindBufferBase(GL_UNIFORM_BUFFER, glGetUniformBlockIndex(blit_map_prog, "BlitMapUniforms"), blit_map_ubo);
  glBindBufferBase(GL_UNIFORM_BUFFER, glGetUniformBlockIndex(blit_map_prog, "vramBuffer"), vram_ubo);

  //----------

  float x = float(fb_width - 256 - 32);
  float y = 32 * 1;

  blit_map_uniforms.quad_pos = {x, y, 256, 384};
  blit_map_uniforms.quad_tex = {0, 0, 128, 192};
  blit_map_uniforms.use_map = 0;
  blit_map_uniforms.which_map = 0;
  blit_map_uniforms.alt_map = 0;

  //glNamedBufferSubData(blit_map_ubo, 0, sizeof(blit_map_uniforms), &blit_map_uniforms);
  glBindBuffer(GL_UNIFORM_BUFFER, blit_map_ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(blit_map_uniforms), &blit_map_uniforms);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  y += 384 + 32;

  blit_map_uniforms.quad_pos = {x, y, 256, 256};
  blit_map_uniforms.quad_tex = {0, 0, 256, 256};
  blit_map_uniforms.use_map = 1;
  blit_map_uniforms.which_map = 1;
  blit_map_uniforms.alt_map = 1;

  //glNamedBufferSubData(blit_map_ubo, 0, sizeof(blit_map_uniforms), &blit_map_uniforms);
  glBindBuffer(GL_UNIFORM_BUFFER, blit_map_ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(blit_map_uniforms), &blit_map_uniforms);

  glDrawArrays(GL_TRIANGLES, 0, 6);

  y += 256 + 32;

  blit_map_uniforms.quad_pos = {x, y, 256, 256};
  blit_map_uniforms.quad_tex = {0, 0, 256, 256};
  blit_map_uniforms.use_map = 1;
  blit_map_uniforms.which_map = 0;
  blit_map_uniforms.alt_map = 1;

  //glNamedBufferSubData(blit_map_ubo, 0, sizeof(blit_map_uniforms), &blit_map_uniforms);
  glBindBuffer(GL_UNIFORM_BUFFER, blit_map_ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(blit_map_uniforms), &blit_map_uniforms);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

//-----------------------------------------------------------------------------

void MetroBoyApp::load(const std::string& prefix, const std::string& name) {
  std::string gb_filename = prefix + "/" + name + ".gb";

#if 0
  std::string golden_filename = prefix + "/" + name + ".bmp";
  SDL_Surface* golden_surface = SDL_LoadBMP(golden_filename.c_str());

  if (!golden_surface) {
    overlay_mode = 0;
    memset(golden, 0, 160 * 144);
  }

  if (golden_surface && golden_surface->format->format == SDL_PIXELFORMAT_INDEX8) {
    printf("Loaded i8 golden\n");
    uint8_t* src = (uint8_t*)golden_surface->pixels;
    uint32_t* pal = (uint32_t*)golden_surface->format->palette->colors;
    for (int y = 0; y < 144; y++) {
      for (int x = 0; x < 160; x++) {
        uint8_t a = pal[src[x + y * 160]] & 0xFF;

        if (a < 40) a = 3;
        else if (a < 128) a = 2;
        else if (a < 210) a = 1;
        else a = 0;

        golden[x + y * 160] = a;
      }
    }
    overlay_mode = 1;
  }

  else if (golden_surface && golden_surface->format->format == SDL_PIXELFORMAT_BGR24) {
    printf("Loaded argb golden\n");
    uint8_t* src = (uint8_t*)golden_surface->pixels;
    for (int y = 0; y < 144; y++) {
      for (int x = 0; x < 160; x++) {
        uint8_t a = src[x * 3 + y * golden_surface->pitch];

        if (a < 40) a = 3;
        else if (a < 128) a = 2;
        else if (a < 210) a = 1;
        else a = 0;

        golden[x + y * 160] = a;
      }
    }
    overlay_mode = 1;
  }
#endif

  printf("Loading rom %s\n", gb_filename.c_str());
  memset(rom_buf, 0, 1024 * 1024);
  metroboy.load_rom(gb_filename.c_str(), false);
  rom_loaded = true;
  runmode = STEP_CYCLE;
}

//-----------------------------------------------------------------------------

void MetroBoyApp::close() {
  audio_stop();
  AppBase::close();
};

//-----------------------------------------------------------------------------

void MetroBoyApp::update() {
  AppBase::update();

  int mouse_x = 0, mouse_y = 0;
  SDL_GetMouseState(&mouse_x, &mouse_y);

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_QUIT) quit = true;
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) quit = true;

    if (event.type == SDL_MOUSEWHEEL) {
      view = view.zoom({mouse_x, mouse_y}, double(event.wheel.y) * 0.25);
    }

    if (event.type == SDL_MOUSEMOTION && (event.motion.state & SDL_BUTTON_LMASK)) {
      view = view.pan({event.motion.xrel, event.motion.yrel});
    }

    if (event.type == SDL_DROPFILE) {
      metroboy.load_rom(event.drop.file, false);
      rom_loaded = true;
      runmode = RUN_VSYNC;
      SDL_free(event.drop.file);
    }

    if (event.type == SDL_KEYDOWN) switch (event.key.keysym.sym) {
    case SDLK_f:      runmode = RUN_FAST; break;
    case SDLK_v:      runmode = RUN_VSYNC; break;
    case SDLK_s:      runmode = STEP_FRAME; break;
    case SDLK_o:      overlay_mode = (overlay_mode + 1) % 3; break;
    case SDLK_RIGHT:  {
      if (keyboard_state[SDL_SCANCODE_LCTRL]) {
        step_forward += 10;
      } else {
        step_forward++;
      }
      break;
    }
    case SDLK_LEFT:   {
      if (keyboard_state[SDL_SCANCODE_LCTRL]) {
        step_backward += 10; 
      } else {
        step_backward++; 
      }
      break;
    }
    case SDLK_UP:     step_up = true; break;
    case SDLK_DOWN:   step_down = true; break;

    case SDLK_r:      reset = true; break;
    case SDLK_F1:     load_dump = true; break;
    case SDLK_F4:     save_dump = true; break;
    case SDLK_ESCAPE: quit = true; break;
    }
  }
  
  //----------------------------------------
  // Button input

  buttons = 0;
  if (keyboard_state[SDL_SCANCODE_RIGHT])  buttons |= 0x01;
  if (keyboard_state[SDL_SCANCODE_LEFT])   buttons |= 0x02;
  if (keyboard_state[SDL_SCANCODE_UP])     buttons |= 0x04;
  if (keyboard_state[SDL_SCANCODE_DOWN])   buttons |= 0x08;
  if (keyboard_state[SDL_SCANCODE_X])      buttons |= 0x10;
  if (keyboard_state[SDL_SCANCODE_Z])      buttons |= 0x20;
  if (keyboard_state[SDL_SCANCODE_RSHIFT]) buttons |= 0x40;
  if (keyboard_state[SDL_SCANCODE_RETURN]) buttons |= 0x80;

  // probably too fast for debugging
  //if (keyboard_state[SDL_SCANCODE_RIGHT])  step_forward++;
  //if (keyboard_state[SDL_SCANCODE_LEFT])   step_backward++;

  //----------------------------------------
  // Handle sim controls

  if (reset) {
    metroboy.reset(0x0100);
    reset = false;
  }

  if (load_dump) {
    metroboy.load_dump();
    load_dump = false;
    rom_loaded = true;
  }

  if (save_dump) {
    metroboy.save_dump();
    save_dump = false;
  }

  if (step_up) {
    if (runmode == STEP_CYCLE) runmode = STEP_LINE;
    else if (runmode == STEP_LINE) runmode = STEP_FRAME;
  }

  if (step_down) {
    if (runmode == STEP_FRAME) runmode = STEP_LINE;
    else if (runmode == STEP_LINE) runmode = STEP_CYCLE;
  }

  //----------------------------------------
  // Run simulation

  cycles_begin = metroboy.total_tcycles();

  if (runmode == RUN_FAST) {
    fast_cycles += (16.0 - 1000 * (double(last_frame_time) / double(timer_freq))) * 100;
    metroboy.run_fast(buttons, (int)fast_cycles);
    //metroboy.run_fast(buttons, 100000);
  }
  else if (runmode == RUN_VSYNC) {
    //printf("%d --------\n", frame_count);
    metroboy.run_vsync(buttons);
  }
  else if (runmode == STEP_CYCLE) {
    while (step_forward--) {
      if (keyboard_state[SDL_SCANCODE_LSHIFT]) {
        metroboy.step_over();
      }
      else {
        metroboy.step_cycle();
      }
    }
    while (step_backward--) {
      metroboy.unstep_cycle();
    }
  }
  else if (runmode == STEP_FRAME) {
    while (step_forward--)  metroboy.step_frame();
    while (step_backward--) metroboy.unstep_frame();
  }
  else if (runmode == STEP_LINE) {
    while (step_forward--)  metroboy.step_line();
    while (step_backward--) metroboy.unstep_line();
  }

  step_forward = 0;
  step_backward = 0;
  step_up = false;
  step_down = false;

  cycles_end = metroboy.total_tcycles();

  uint32_t* dummy = new uint32_t[160 * 144];
  for (int i = 0; i < 160*144; i++) { dummy[i] = i * 0x1234567; }
  //update_texture(gb_tex, 160, 144, 1, (void*)metroboy.fb().buf);
  update_texture(gb_tex, 160, 144, dummy);
  delete [] dummy;


  update_texture(trace_tex, 456, 154, (void*)metroboy.get_trace());

  double ease_speed = 1.8;

  view_smooth.min.x = ease(view_smooth.min.x, view.min.x, ease_speed);
  view_smooth.min.y = ease(view_smooth.min.y, view.min.y, ease_speed);
  view_smooth.max.x = ease(view_smooth.max.x, view.max.x, ease_speed);
  view_smooth.max.y = ease(view_smooth.max.y, view.max.y, ease_speed);

  Viewport snap = view.snap();
  view_snap.min.x = ease(view_snap.min.x, snap.min.x, ease_speed);
  view_snap.min.y = ease(view_snap.min.y, snap.min.y, ease_speed);
  view_snap.max.x = ease(view_snap.max.x, snap.max.x, ease_speed);
  view_snap.max.y = ease(view_snap.max.y, snap.max.y, ease_speed);

  if (view_snap == snap) {
    view = view_snap;
    view_smooth = view_snap;
  }

  text_painter.set_viewport(view_snap);
  grid_painter.set_viewport(view_snap);
}

//-----------------------------------------------------------------------------

void MetroBoyApp::begin_frame() {
  AppBase::begin_frame();
}

//-----------------------------------------------------------------------------

void MetroBoyApp::render_frame() {
  AppBase::render_frame();

  grid_painter.render();

  //----------------------------------------
  // Wave thingy

  /*
  for (int i = 0; i < 16; i++) {
    uint8_t a = (gameboy.get_spu().get_wave()[i] & 0x0F) >> 0;
    uint8_t b = (gameboy.get_spu().get_wave()[i] & 0xF0) >> 4;
    uint32_t color = 0xFFFFFFFF;
  
    framebuffer[(512 + 2 * i + 0) + (100 + b) * fb_width] = color;
    framebuffer[(512 + 2 * i + 1) + (100 + a) * fb_width] = color;
  }
  */

  //----------------------------------------
  // Gameboy screen

  const int gb_screenx = 1248;
  const int gb_screeny = fb_height - 288 - 32;

  //blit_mono(gb_tex, gb_screenx, gb_screeny, 160 * 2, 144 * 2);
  blit(gb_tex, gb_screenx, gb_screeny, 160 * 2, 144 * 2);
  blit_map();
  blit(trace_tex, 512 + 32 * 8, fb_height - 160 - 32, 456, 154);

  /*
  if (overlay_mode == 0 || overlay_mode == 1) {
    for (int y = 0; y < 144; y++) {
      uint32_t* line1 = &framebuffer[(y * 2 + gb_screeny + 0) * fb_width + gb_screenx];
      uint32_t* lineM2 = &framebuffer[(y * 2 + gb_screeny + 1) * fb_width + gb_screenx];
      for (int x = 0; x < 160; x++) {
        uint32_t c = gb_colors[fb.buf[x + (y * 160)] & 7];
        *line1++ = c; *line1++ = c;
        *lineM2++ = c; *lineM2++ = c;
      }
    }
  }

  tp.draw_bbox(gb_screenx - 2, gb_screeny - 2, 320 + 3, 288 + 3, 0x505050);
  tp.draw_bbox(gb_screenx - 1, gb_screeny - 1, 320+1, 288+1, 0x101010);
  */


#if 0

  //----------------------------------------
  // Reference image

  if (overlay_mode == 2) {
    for (int y = 0; y < 144; y++) {
      uint32_t* line1 = &framebuffer[(y * 2 + gb_screeny + 0) * fb_width + gb_screenx];
      uint32_t* lineM2 = &framebuffer[(y * 2 + gb_screeny + 1) * fb_width + gb_screenx];
      for (int x = 0; x < 160; x++) {
        uint32_t c = gb_colors[golden[x + y * 160]];
        c += 0x100000;
        *line1++ = c; *line1++ = c;
        *lineM2++ = c; *lineM2++ = c;
      }
    }
  }

  //----------------------------------------
  // Diff overlay

  if (overlay_mode == 1) {
    for (int y = 0; y < 144; y++) {
      uint32_t* line1 = &framebuffer[(y * 2 + gb_screeny + 0) * fb_width + gb_screenx];
      uint32_t* lineM2 = &framebuffer[(y * 2 + gb_screeny + 1) * fb_width + gb_screenx];
      for (int x = 0; x < 160; x++) {
        int c = fb.buf[x + (y * 160)];
        if (c != golden[x + y * 160]) {
          *line1++ += 0x808000;
          *line1++ += 0x808000;
          *lineM2++ += 0x808000;
          *lineM2++ += 0x808000;
        }
        else {
          line1++;
          line1++;
          lineM2++;
          lineM2++;
        }
      }
    }
  }

  //----------------------------------------
  // Step highlight

  if (runmode == STEP_LINE || runmode == STEP_CYCLE) {

    int px = metroboy.gb_out.x;
    int py = metroboy.gb_out.y;

    for (int x = 0; x < 320; x++) {
      int color = (px == (x / 2)) ? 0x00606000 : 0x00600000;
      framebuffer[(gb_screenx + x) + (gb_screeny + py * 2 + 0) * fb_width] += color;
      framebuffer[(gb_screenx + x) + (gb_screeny + py * 2 + 1) * fb_width] += color;
    }
  }

#endif
}

//-----------------------------------------------------------------------------

void MetroBoyApp::render_ui() {
  AppBase::render_ui();

  //----------------------------------------
  // Stat bar

  const char* mode_names[] = {
    "RUN_FAST",
    "RUN_VSYNC",
    "STEP_FRAME",
    "STEP_LINE",
    "STEP_CYCLE",
  };

  const int gb_screenx = fb_width - 288 - 288 - 288;
  const int gb_screeny = 32 * 10;

  sprintf(text_buf, "%s %d", mode_names[runmode], (int)(metroboy.gb().get_tcycle() & 3));
  //text_painter.render(text_buf, gb_screenx, 32 * 20);
  text_buf.clear();

  //----------------------------------------
  // Left column text

  Gameboy& gameboy = metroboy.gb();

  int spacing = 192 + 32;

  gameboy.dump1(text_buf);
  text_painter.render(text_buf, spacing * 0 + 4, 4);
  text_buf.clear();

  sprintf(text_buf, "\002--------------PPU--------------\001\n");
  gameboy.get_ppu().dump(text_buf);
  text_painter.render(text_buf, spacing * 1 + 4, 4);
  text_buf.clear();

  gameboy.dump3(text_buf);
  text_painter.render(text_buf, spacing * 2 + 4, 4);
  text_buf.clear();

  sprintf(text_buf, "\002--------------SPU--------------\001\n");
  gameboy.get_spu().dump(text_buf);
  text_painter.render(text_buf, spacing * 2 + 4, 640 + 4);
  text_buf.clear();

  gameboy.dump4(text_buf);
  text_painter.render(text_buf, spacing * 3 + 4, 4);
  text_buf.clear();

  //----------------------------------------
  // Perf timer

  sprintf(text_buf, "view        zoom %f view_x %f view_y %f\n", view.get_zoom(),        view.min.x, view.min.y);
  sprintf(text_buf, "view_smooth zoom %f view_x %f view_y %f\n", view_smooth.get_zoom(), view_smooth.min.x, view_smooth.min.y);
  sprintf(text_buf, "view_snap   zoom %f view_x %f view_y %f\n", view_snap.get_zoom(),   view_snap.min.x, view_snap.min.y);
  sprintf(text_buf, "frame time %2.2f msec, %6d cyc/frame\n", last_frame_time_smooth, (int)(cycles_end - cycles_begin) / 4);
  text_painter.render(text_buf, 0, fb_height - 48);
  text_buf.clear();
}

//-----------------------------------------------------------------------------

void MetroBoyApp::end_frame() {
  AppBase::end_frame();
}

//-----------------------------------------------------------------------------
