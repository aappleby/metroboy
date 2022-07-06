#include "AppLib/AppHost.h"
#include "AppLib/GLBase.h"

#include "imgui/imgui.h"
#include "glad/glad.h"

#include "CoreLib/Tests.h"

#include "SDL/include/SDL.h"

//-----------------------------------------------------------------------------

const GLchar* imgui_glsl = R"(

uniform vec4      viewport;
uniform sampler2D tex;

float remap(float x, float a1, float a2, float b1, float b2) {
  x = (x - a1) / (a2 - a1);
  x = x * (b2 - b1) + b1;
  return x;
}

#ifdef _VERTEX_

layout (location = 0) in vec2 vert_pos;
layout (location = 1) in vec2 vert_tc;
layout (location = 2) in vec4 vert_col;

out vec2 frag_tc;
out vec4 frag_col;

void main() {
  frag_tc = vert_tc;
  frag_col = vert_col;

  float view_w = viewport.z - viewport.x;
  float view_h = viewport.w - viewport.y;

  float x = remap(vert_pos.x, viewport.x, viewport.z, -1.0, 1.0);
  float y = remap(vert_pos.y, viewport.y, viewport.w, 1.0, -1.0);

  gl_Position = vec4(x, y, 0.0, 1.0);
}

#else

in vec2 frag_tc;
in vec4 frag_col;

out vec4 frag;

void main() {
  frag = frag_col * texture(tex, frag_tc.st).rrrr;
}

#endif
)";

//-----------------------------------------------------------------------------

int AppHost::app_main(int, char**) {
  LOG_G("AppHost::app_main()\n");

  //----------------------------------------
  // Create window

  //SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Init(SDL_INIT_VIDEO);

  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);

  int initial_screen_w, initial_screen_h;

  if (display_mode.w <= 1920 && display_mode.h <= 1080) {
    initial_screen_w = display_mode.w;
    initial_screen_h = display_mode.h;
    window = SDL_CreateWindow(app->app_get_title(),
                              0, 0,
                              display_mode.w, display_mode.h,
                              SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
  }
  else {
    initial_screen_w = 1920;
    initial_screen_h = 1080;
    window = SDL_CreateWindow(app->app_get_title(),
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              1920, 1080,
                              SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
  }


  const uint8_t* keyboard_state = SDL_GetKeyboardState(nullptr);

  gl_context = (SDL_GLContext)init_gl(window);

  //----------------------------------------
  // Initialize ImGui and ImGui renderer

  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGuiIO& io = ImGui::GetIO();

  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
  io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
  io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
  io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
  io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
  io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
  io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
  io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
  io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
  io.KeyMap[ImGuiKey_Insert] = SDL_SCANCODE_INSERT;
  io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
  io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
  io.KeyMap[ImGuiKey_Space] = SDL_SCANCODE_SPACE;
  io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
  io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;

  {
    imgui_prog = create_shader("imgui_glsl", imgui_glsl);

    unsigned char* pixels;
    int font_width, font_height;
    io.Fonts->GetTexDataAsAlpha8(&pixels, &font_width, &font_height);
    imgui_tex = create_texture_u8(font_width, font_height, pixels, false);

    imgui_vao = create_vao();
    imgui_vbo = create_vbo();
    imgui_ibo = create_ibo();

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 2, GL_FLOAT,         GL_FALSE, 20, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT,         GL_FALSE, 20, (void*)8);
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE,  20, (void*)16);
  }


  //----------------------------------------
  // Initialize internal renderers

  app->app_init(initial_screen_w, initial_screen_h);

  //----------------------------------------
  // Loop forever

  double old_now = timestamp();
  double new_now = timestamp();
  double delta = new_now - old_now;
  double last_event = 0;

  while (!quit) {
    if (keyboard_state[SDL_SCANCODE_ESCAPE] && keyboard_state[SDL_SCANCODE_LSHIFT]) {
      break;
    }

    int screen_w = 0, screen_h = 0;
    SDL_GL_GetDrawableSize((SDL_Window*)window, &screen_w, &screen_h);
    dvec2 screen_size(screen_w, screen_h);

    io.DisplaySize.x = float(screen_w);
    io.DisplaySize.y = float(screen_h);

    if (app->pause_when_idle() && (new_now - last_event) > 1.0) {
      LOG_B("waiting\n");
      SDL_WaitEvent(nullptr);
      delta = new_now - old_now;
      new_now = timestamp();
      old_now = new_now - delta;
    }
    else {
      SDL_PumpEvents();
      old_now = new_now;
      new_now = timestamp();
      delta = new_now - old_now;
    }

    io.DeltaTime = (float)delta;

    //----------------------------------------
    // Check for quit message

    SDL_Event events[64];
    const int nevents = SDL_PeepEvents(events, 64, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);

    if (nevents) last_event = new_now;

    for(int i = 0; i < nevents; i++) {
      if (events[i].type == SDL_QUIT) quit = true;
    }

    //----------------------------------------
    // Client app update

    //view_control.update(delta);

    app->app_update(screen_size, delta);

    //----------------------------------------
    // Client app render

    glViewport(0, 0, screen_w, screen_h);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui::NewFrame();

    app->app_render_frame(screen_size, delta);

    app->app_render_ui(screen_size, delta);

    //----------------------------------------
    // ImGui render

    //ImGui::ShowDemoWindow();
    ImGui::Render();

    const ImDrawData* draw_data = ImGui::GetDrawData();

    if (draw_data->CmdListsCount) {
      glUseProgram(imgui_prog);

      glUniform4f(glGetUniformLocation(imgui_prog, "viewport"), 0, 0, (float)screen_w, (float)screen_h);
      glUniform1i(glGetUniformLocation(imgui_prog, "tex"), 0);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, imgui_tex);
      glBindVertexArray(imgui_vao);
      glBindBuffer(GL_ARRAY_BUFFER, imgui_vbo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, imgui_ibo);

      glEnable(GL_SCISSOR_TEST);

      for (int n = 0; n < draw_data->CmdListsCount; n++) {
        const ImDrawList* l = draw_data->CmdLists[n];

        glBufferData(GL_ARRAY_BUFFER, l->VtxBuffer.Size * sizeof(ImDrawVert), l->VtxBuffer.Data, GL_STREAM_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, l->IdxBuffer.Size * sizeof(ImDrawIdx), l->IdxBuffer.Data, GL_STREAM_DRAW);

        for (int i = 0; i < l->CmdBuffer.Size; i++) {
          const ImDrawCmd* c = &l->CmdBuffer[i];
          glScissor(int(c->ClipRect.x),
                    screen_h - int(c->ClipRect.w),
                    int(c->ClipRect.z - c->ClipRect.x),
                    int(c->ClipRect.w - c->ClipRect.y));

          glDrawElements(GL_TRIANGLES, c->ElemCount, GL_UNSIGNED_SHORT,
                          reinterpret_cast<void*>(intptr_t(c->IdxOffset * 2)));
        }
      }

      glDisable(GL_SCISSOR_TEST);
    }

    //----------------------------------------
    // Swap

    SDL_GL_SwapWindow((SDL_Window*)window);

    // FIXME why was this here?
    //SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
  }

  //----------------------------------------
  // App exit

  app->app_close();

  //ImGui::DestroyContext();
  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow((SDL_Window*)window);
  SDL_Quit();

  return 0;
}
