#include "AppBase.h"

#include <stdio.h>
#include <GL/gl3w.h>

#include <include/SDL.h>
#include <imgui.h>
#include <examples/imgui_impl_sdl.h>
#include <examples/imgui_impl_opengl3.h>

//-----------------------------------------------------------------------------

void AppBase::init() {
  SDL_Init(SDL_INIT_VIDEO);

  app_begin = SDL_GetPerformanceCounter();

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  window = SDL_CreateWindow("GBSchematic",
                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            fb_width, fb_height,
                            SDL_WINDOW_OPENGL /*| SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI*/);
  gl_context = SDL_GL_CreateContext(window);
  SDL_GL_SetSwapInterval(1); // Enable vsync
  gl3wInit();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init("#version 130");

  keyboard_state = SDL_GetKeyboardState(nullptr);
  timer_freq = SDL_GetPerformanceFrequency();

  quad_init();
  blit_init();

  printf("%d\n", glGetError());

  {
    checker_tex = create_texture(32, 32, 1);
    uint8_t blah[32*32];
    for (int y = 0; y < 32; y++)
    for (int x = 0; x < 32; x++) {
      blah[x + 32 * y] = ((x^y)&1) ? 0xFF : 0x00;
    }
    update_texture(checker_tex, 32, 32, 1, blah);
  }

  {
    const int tile_width = fb_width / 32;
    const int tile_height = fb_height / 32;
    bg_tex = create_texture(tile_width, tile_height, 1);
    uint8_t* tile_pix = new uint8_t[tile_width * tile_height];
    for (int y = 0; y < tile_height; y++)
    for (int x = 0; x < tile_width; x++) {
      tile_pix[x + y * tile_width] = ((x^y)&1) ? 0x0C : 0x17;
    }
    update_texture(bg_tex, tile_width, tile_height, 1, tile_pix);
    delete [] tile_pix;
  }

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  text_painter.init();
}

void AppBase::close() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  SDL_GL_DeleteContext(gl_context);
  gl_context = nullptr;
  SDL_DestroyWindow(window);
  window = nullptr;
  SDL_Quit();
}

//-----------------------------------------------------------------------------

uint32_t AppBase::create_vao() {
  uint32_t vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  return vao;
}

uint32_t AppBase::create_vbo(int size_bytes) {
  uint32_t vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferStorage(GL_ARRAY_BUFFER, size_bytes, nullptr,
                  GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
  return vbo;
}

uint32_t AppBase::create_vbo(void* data, int size_bytes) {
  uint32_t vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferStorage(GL_ARRAY_BUFFER, size_bytes, nullptr,
                  GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
  void* dst = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  memcpy(dst, data, size_bytes);
  glUnmapBuffer(GL_ARRAY_BUFFER);
  return vbo;
}

void AppBase::update_vbo(uint32_t vbo, void* data, int size_bytes) {
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  void* dst = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  memcpy(dst, data, size_bytes);
  glUnmapBuffer(GL_ARRAY_BUFFER);
}

//-----------------------------------------------------------------------------

void AppBase::quad_init() {
  float unit_quad[] = {
    0, 0,
    1, 0,
    1, 1,
    
    0, 0,
    1, 1,
    0, 1,
  };

  uint32_t vao = create_vao();
  uint32_t vbo = create_vbo(unit_quad, sizeof(unit_quad));

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  quad_vao = vao;
  quad_vbo = vbo;
}

//-----------------------------------------------------------------------------

void AppBase::blit_init() {
  const GLchar* blit_vert_source = R"(
#version 460

uniform vec4 screen_size = vec4(1920.0, 1080.0, 1.0 / 1920.0, 1.0 / 1080.0);
uniform vec4 quad_pos = vec4(128.4, 128.0, 32.0, 32.0);

layout(location = 0) in  vec2 vpos;
layout(location = 0) out vec2 vtex;

vec4 screen_to_norm(float x, float y) {
  x *= screen_size.z;
  y *= screen_size.w;

  x = x * 2.0 - 1.0;
  y = (1.0 - y) * 2.0 - 1.0;
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

  //----------------------------------------

  const GLchar* blit_frag_source = R"(
#version 460

uniform vec4 quad_col = vec4(1.0, 1.0, 1.0, 1.0);
uniform sampler2D tex;
uniform bool solid = false;
uniform bool mono = false;
uniform bool palette = false;

layout(location = 0) in vec2 ftex;
layout(location = 0) out vec4 frag;

void main() {
  if (solid) {
    frag = vec4(1.0, 0.0, 1.0, 1.0);
    return;
  }

  vec4 src = texture(tex, ftex);
  if (mono) {
    frag = vec4(src.r, src.r, src.r, 1.0) * quad_col;
  }
  else if (palette) {
    frag = vec4(1.0, 0.0, 1.0, 1.0);
  }
  else {
    frag = src * quad_col;
  }
  frag.a = 1.0;
}
  )";

  blit_prog = compile_shader("quad", blit_vert_source, blit_frag_source);
}

//-----------------------------------------------------------------------------

uint32_t AppBase::create_texture(int width, int height, int channels, bool filter) {
  uint32_t tex = 0;
  glGenTextures(1, &tex);
  printf("quad_tex %d\n", tex);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);

  if (channels == 1) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8,
                 width, height, 0,
                 GL_RED, GL_UNSIGNED_BYTE, nullptr);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  }
  else if (channels == 2) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8,
                 width, height, 0,
                 GL_RG, GL_UNSIGNED_BYTE, nullptr);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  }
  else if (channels == 3) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8,
                 width, height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  }
  else if (channels == 4) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                 width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter ? GL_LINEAR : GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter ? GL_LINEAR : GL_NEAREST);
  return tex;
}

//-----------------------------------------------------------------------------

void AppBase::update_texture(uint32_t tex, int width, int height, int channels, void* pix) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);

  if (channels == 1) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0,
                    0, 0, width, height,
                    GL_RED, GL_UNSIGNED_BYTE, pix);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  }
  else if (channels == 2) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0,
                    0, 0, width, height,
                    GL_RG, GL_UNSIGNED_BYTE, pix);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  }
  else if (channels == 3) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0,
                    0, 0, width, height,
                    GL_RGB, GL_UNSIGNED_BYTE, pix);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  }
  else if (channels == 4) {
    glTexSubImage2D(GL_TEXTURE_2D, 0,
                    0, 0, width, height,
                    GL_RGBA, GL_UNSIGNED_BYTE, pix);
  }
}

//-----------------------------------------------------------------------------

int AppBase::compile_shader(const char* name, const char* vert_source, const char* frag_source) {
  char buf[1024];
  int len = 0;

  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vert_source, NULL);
  glCompileShader(vertexShader);

  glGetShaderInfoLog(vertexShader, 1024, &len, buf);
  printf("%s vert shader log:\n%s", name, buf);

  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &frag_source, NULL);
  glCompileShader(fragmentShader);

  glGetShaderInfoLog(fragmentShader, 1024, &len, buf);
  printf("%s frag shader log:\n%s", name, buf);

  int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  glGetProgramInfoLog(shaderProgram, 1024, &len, buf);
  printf("%s shader prog log:\n%s", name, buf);

  return shaderProgram;
}

//-----------------------------------------------------------------------------

void AppBase::blit(uint32_t tex, int x, int y, int w, int h) {
  glUseProgram(blit_prog);

  glUniform4f(glGetUniformLocation(blit_prog, "screen_size"),
              (float)fb_width, (float)fb_height, 1.0f / fb_width, 1.0f / fb_height);

  glUniform1i(glGetUniformLocation(blit_prog, "mono"), 0);
  glUniform1i(glGetUniformLocation(blit_prog, "solid"), 0);
  glUniform1i(glGetUniformLocation(blit_prog, "palette"), 0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glUniform1i(glGetUniformLocation(blit_prog, "tex"), 0);

  glUniform4f(glGetUniformLocation(blit_prog, "quad_pos"),
              (float)x, (float)y, (float)w, (float)h);
  glUniform4f(glGetUniformLocation(blit_prog, "quad_col"),
              1.0, 1.0, 1.0, 1.1);

  glBindVertexArray(quad_vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void AppBase::blit_mono(uint32_t tex, int x, int y, int w, int h) {
  glUseProgram(blit_prog);

  glUniform4f(glGetUniformLocation(blit_prog, "screen_size"),
              (float)fb_width, (float)fb_height, 1.0f / fb_width, 1.0f / fb_height);

  glUniform1i(glGetUniformLocation(blit_prog, "mono"), 1);
  glUniform1i(glGetUniformLocation(blit_prog, "solid"), 0);
  glUniform1i(glGetUniformLocation(blit_prog, "palette"), 0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glUniform1i(glGetUniformLocation(blit_prog, "tex"), 0);

  glUniform4f(glGetUniformLocation(blit_prog, "quad_pos"),
              (float)x, (float)y, (float)w, (float)h);
  glUniform4f(glGetUniformLocation(blit_prog, "quad_col"),
              1.0, 1.0, 1.0, 1.1);

  glBindVertexArray(quad_vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

//-----------------------------------------------------------------------------

void AppBase::check_gl_error() {
  int error = glGetError();
  if (error) {
    printf("GL error %d\n", error);
  }
}

//-----------------------------------------------------------------------------

int AppBase::main(int, char**) {
  init();
  while (!quit) {

    update_begin = SDL_GetPerformanceCounter();
    update();
    update_end = SDL_GetPerformanceCounter();
    last_update_time = update_end - update_begin;
    last_update_time_smooth *= 0.98;
    last_update_time_smooth += (1000.0 * double(last_update_time) / double(timer_freq)) * 0.02;

    frame_begin = SDL_GetPerformanceCounter();
    begin_frame();
    render_frame();
    render_ui();
    end_frame();
    frame_end = SDL_GetPerformanceCounter();

    last_frame_time = frame_end - frame_begin;
    last_frame_time_smooth *= 0.98;
    last_frame_time_smooth += (1000.0 * double(last_frame_time) / double(timer_freq)) * 0.02;
    frame_count++;
  }
  close();
  return 0;
}

//-----------------------------------------------------------------------------

void AppBase::update() {
}

//-----------------------------------------------------------------------------

void AppBase::begin_frame() {
  now = float(frame_begin - app_begin) / float(timer_freq);

  text_painter.begin_frame();

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame(window);
  ImGui::NewFrame();

  glViewport(0, 0, fb_width, fb_height);
  glClearColor(0.1f, 0.1f, 0.2f, 0.f);
  glClearDepth(1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//-----------------------------------------------------------------------------

void AppBase::render_frame() {
  blit_mono(bg_tex, 0, 0, fb_width, fb_height);

  //text_painter.dprintf("Hello World\n");
  //text_painter.render(4, 4, 1.0);
}

//-----------------------------------------------------------------------------

void AppBase::render_ui() {
  ImGuiIO& io = ImGui::GetIO();
  ImGui::SetNextWindowPos({(float)fb_width,(float)fb_height}, 0, {1,1});
  ImGui::SetNextWindowBgAlpha(0.2f);
  ImGui::Begin("Status", nullptr,
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoDecoration |
    ImGuiWindowFlags_AlwaysAutoResize |
    ImGuiWindowFlags_NoSavedSettings |
    ImGuiWindowFlags_NoFocusOnAppearing |
    ImGuiWindowFlags_NoNav);
  ImGui::Text("(%.1f FPS)", io.Framerate);
  ImGui::End();

  //ImGui::ShowDemoWindow();
}

//-----------------------------------------------------------------------------

void AppBase::end_frame() {
  text_painter.end_frame();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  SDL_GL_SwapWindow(window);
  check_gl_error();
}

//-----------------------------------------------------------------------------
