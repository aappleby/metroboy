#include "AppBase.h"

#include <stdio.h>
#include <GL/gl3w.h>

#include <include/SDL.h>
#include <imgui.h>
#include <examples/imgui_impl_sdl.h>
#include <examples/imgui_impl_opengl3.h>

#include <dwmapi.h>

//-----------------------------------------------------------------------------

const char* blit_hdr = R"(
#version 460
#extension GL_ARB_bindless_texture : require

layout(std140, binding = 0) uniform BlitUniforms
{
  uvec2  tex_ptr;
  double pad1;

  vec4 quad_pos;
  vec4 viewport;
  vec4 screen_size;
  vec4 quad_col;
  int  solid;
  int  mono;
  int  palette;
  int  pad2;
};

)";

//-----------------------------------------------------------------------------

const char* blit_vert_src = R"(

layout(location = 0) in  vec2 vpos;
layout(location = 0) out vec2 vtex;

void main() {
  float x = quad_pos.x;
  float y = quad_pos.y;
  float w = quad_pos.z;
  float h = quad_pos.w;

  x = mix(x, x + w, vpos.x);
  y = mix(y, y + h, vpos.y);

  x *= screen_size.z;
  y *= screen_size.w;

  x = x * 2.0 - 1.0;
  y = (1.0 - y) * 2.0 - 1.0;
  gl_Position = vec4(x, y, 1.0, 1.0);

  vtex = vpos;
}

)";

//-----------------------------------------------------------------------------

const char* blit_frag_src = R"(

layout(location = 0) in  vec2 ftex;
layout(location = 0) out vec4 frag;

void main() {
  if (bool(solid)) {
    frag = vec4(1.0, 0.0, 1.0, 1.0);
  }
  else if (bool(mono)) {
    frag = vec4(texture(sampler2D(tex_ptr), ftex).rrr, 1.0) * quad_col;
  }
  else if (bool(palette)) {
    frag = vec4(1.0, 0.0, 1.0, 1.0);
  }
  else {
    frag = texture(sampler2D(tex_ptr), ftex) * quad_col;
  }
}

)";

//-----------------------------------------------------------------------------

void debugOutput(GLenum /*source*/, GLenum /*type*/, GLuint /*id*/, GLenum /*severity*/,
                 GLsizei /*length*/, const GLchar* message, const GLvoid* /*userParam*/) {
	printf("GLDEBUG: %s\n", message);
}

//-----------------------------------------------------------------------------

void AppBase::init() {
  SDL_Init(SDL_INIT_VIDEO);

  app_begin = SDL_GetPerformanceCounter();

  // debug context?
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                      SDL_GL_CONTEXT_DEBUG_FLAG |
                      SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG |
                      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
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
  SDL_GL_SetSwapInterval(0); // Enable vsync

  gl3wInit();
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
  glDebugMessageCallback(debugOutput, nullptr);

  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init("#version 130");

  keyboard_state = SDL_GetKeyboardState(nullptr);
  timer_freq = SDL_GetPerformanceFrequency();

  quad_init();
  blit_init();

  {
    checker_tex = create_texture(32, 32);
    uint32_t blah[32*32];
    for (int y = 0; y < 32; y++)
    for (int x = 0; x < 32; x++) {
      blah[x + 32 * y] = ((x^y)&1) ? 0xFFFFFFFF : 0xFF000000;
    }
    update_texture(checker_tex, 32, 32, blah);
  }

  {
    const int tile_width = fb_width / 32;
    const int tile_height = fb_height / 32;
    bg_tex = create_texture(tile_width, tile_height);
    uint32_t* tile_pix = new uint32_t[tile_width * tile_height];
    for (int y = 0; y < tile_height; y++)
    for (int x = 0; x < tile_width; x++) {
      tile_pix[x + y * tile_width] = ((x^y)&1) ? 0xFF0C0C0C : 0xFF171717;
    }
    update_texture(bg_tex, tile_width, tile_height, tile_pix);
    delete [] tile_pix;
  }

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  text_painter.init();

  glGenBuffers(1, &blit_ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, blit_ubo);
  glNamedBufferStorage(blit_ubo, sizeof(BlitUniforms), nullptr, GL_DYNAMIC_STORAGE_BIT);
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
  blit_prog = compile_shader(blit_hdr, blit_vert_src, blit_frag_src);
}

//-----------------------------------------------------------------------------

uint32_t AppBase::create_texture(int width, int height) {
  uint32_t tex = 0;
  glGenTextures(1, &tex);
  printf("quad_tex %d\n", tex);
  glBindTexture(GL_TEXTURE_2D, tex);

  /*
  if (channels == 1) {
    glTextureStorage2D(tex, 1, GL_R8, width, height);
  }
  else if (channels == 2) {
    glTextureStorage2D(tex, 1, GL_RG8, width, height);
  }
  else if (channels == 3) {
    glTextureStorage2D(tex, 1, GL_RGB8, width, height);
  }
  else if (channels == 4) {
    glTextureStorage2D(tex, 1, GL_RGBA8, width, height);
  }
  */
  glTextureStorage2D(tex, 1, GL_RGBA8, width, height);

  bool filter = false;

  glTextureParameteri(tex, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(tex, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, filter ? GL_LINEAR : GL_NEAREST);
  glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, filter ? GL_LINEAR : GL_NEAREST);


	uint64_t handle = glGetTextureHandleARB(tex);
  printf("texture handle 0x%016llx\n", handle);
	glMakeTextureHandleResidentARB(handle);

  return tex;
}

//-----------------------------------------------------------------------------

void AppBase::update_texture(uint32_t tex, int width, int height, void* pix) {
  /*
  if (channels == 1) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTextureSubImage2D(tex, 0,
                    0, 0, width, height,
                    GL_RED, GL_UNSIGNED_BYTE, pix);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  }
  else if (channels == 2) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTextureSubImage2D(tex, 0,
                    0, 0, width, height,
                    GL_RG, GL_UNSIGNED_BYTE, pix);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  }
  else if (channels == 3) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTextureSubImage2D(tex, 0,
                    0, 0, width, height,
                    GL_RGB, GL_UNSIGNED_BYTE, pix);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  }
  else if (channels == 4) {
    glTextureSubImage2D(tex, 0,
                    0, 0, width, height,
                    GL_RGBA, GL_UNSIGNED_BYTE, pix);
  }
  */

  glTextureSubImage2D(tex, 0,
                  0, 0, width, height,
                  GL_RGBA, GL_UNSIGNED_BYTE, pix);
}

//-----------------------------------------------------------------------------

int AppBase::compile_shader(const char* hdr, const char* vert_src, const char* frag_src) {
  const char* vert_srcs[] = { hdr, vert_src };
  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 2, vert_srcs, NULL);
  glCompileShader(vertexShader);

  const char* frag_srcs[] = { hdr, frag_src };
  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 2, frag_srcs, NULL);
  glCompileShader(fragmentShader);

  int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  return shaderProgram;
}

//-----------------------------------------------------------------------------

#pragma warning(disable:4100)

void AppBase::blit(uint32_t tex, int x, int y, int w, int h) {
  int gl_width, gl_height;
  SDL_GL_GetDrawableSize(window, &gl_width, &gl_height);

  blit_uniforms = {
    .tex_ptr = glGetTextureHandleARB(tex),
    .pad1 = 0,

    .quad_pos = {x,y,w,h},
    .viewport = {0,0,0,0},
    .screen_size = {(float)gl_width, (float)gl_height, 1.0f / gl_width, 1.0f / gl_height},
    .quad_col = {1,1,1,1},

    .solid = 0,
    .mono = 0,
    .palette = 0,
    .pad2 = 0,
  };
  glNamedBufferSubData(blit_ubo, 0, sizeof(blit_uniforms), &blit_uniforms);

  glUseProgram(blit_prog);
  glBindVertexArray(quad_vao);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, blit_ubo);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

//-----------------------------------------------------------------------------

#pragma warning(disable:4458)
#pragma warning(disable:4189)

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

    SDL_GL_SwapWindow(window);
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

  //ImGui_ImplOpenGL3_NewFrame();
  //ImGui_ImplSDL2_NewFrame(window);
  //ImGui::NewFrame();

  glViewport(0, 0, fb_width, fb_height);
  glClearColor(0.1f, 0.1f, 0.2f, 0.f);
  glClearDepth(1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//-----------------------------------------------------------------------------

void AppBase::render_frame() {
}

//-----------------------------------------------------------------------------

void AppBase::render_ui() {
  /*
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

  text_painter.dprintf("%f\n", last_update_time_smooth);
  text_painter.dprintf("%f\n", last_frame_time_smooth);
  text_painter.render(0, float(fb_height - 48));
  */

  //ImGui::ShowDemoWindow();
}

//-----------------------------------------------------------------------------

void AppBase::end_frame() {
  text_painter.end_frame();
  //ImGui::Render();
  //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

//-----------------------------------------------------------------------------
