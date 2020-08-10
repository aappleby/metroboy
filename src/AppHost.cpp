#include "AppHost.h"

#include "GLBase.h"

#include <imgui.h>
#include <glad/glad.h>

#ifdef _MSC_VER
#include <include/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

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
  printf("AppHost::app_main()\n");

  //----------------------------------------
  // Create window

  SDL_Init(SDL_INIT_VIDEO);

  //int initial_screen_w = 3200;
  //int initial_screen_h = 1600;
  int initial_screen_w = 1664;
  int initial_screen_h = 1024;

  window = SDL_CreateWindow(app->app_get_title(),
                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            initial_screen_w, initial_screen_h,
                            SDL_WINDOW_OPENGL /*| SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI*/);

  keyboard_state = SDL_GetKeyboardState(nullptr);

  gl_context = (SDL_GLContext)init_gl(window);

  //----------------------------------------
  // Initialize ImGui and ImGui renderer

  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGuiIO& io = ImGui::GetIO();

  {
    imgui_prog = create_shader("imgui_glsl", imgui_glsl);

    unsigned char* pixels;
    int font_width, font_height;
    io.Fonts->GetTexDataAsAlpha8(&pixels, &font_width, &font_height);
    imgui_tex = create_texture_u8(font_width, font_height, pixels);

    imgui_vao = create_vao();
    imgui_vbo = create_vbo(65536);
    imgui_ibo = create_ibo(65536);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 2, GL_FLOAT,         GL_FALSE, 20, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT,         GL_FALSE, 20, (void*)8);
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE,  20, (void*)16);
  }


  //----------------------------------------
  // Initialize internal renderers

  view_raw = view_raw.reset(initial_screen_w, initial_screen_h);
  view_smooth = view_raw;
  view_snap = view_raw;

  printf("\n");

  app->app_init();

  //----------------------------------------
  // Loop forever

  static uint64_t old_now = SDL_GetPerformanceCounter();
  static uint64_t new_now = SDL_GetPerformanceCounter();

  while (!quit) {
    old_now = new_now;
    new_now = SDL_GetPerformanceCounter();
    const double delta = double(new_now - old_now) / double(SDL_GetPerformanceFrequency());

    //----------------------------------------
    // Peek events and dispatch to ImGui

    int screen_w = 0, screen_h = 0;
    SDL_GL_GetDrawableSize((SDL_Window*)window, &screen_w, &screen_h);

    int mouse_x = 0, mouse_y = 0;
    uint32_t mouse_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

    SDL_Event events[64];
    SDL_PumpEvents();
    const int nevents = SDL_PeepEvents(events, 64, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
    for(int i = 0; i < nevents; i++) {
      const SDL_Event* event = &events[i];

      switch (event->type) {
      case SDL_MOUSEWHEEL: {
        if (io.WantCaptureMouse) {
          if (event->wheel.x > 0) io.MouseWheelH += 1;
          if (event->wheel.x < 0) io.MouseWheelH -= 1;
          if (event->wheel.y > 0) io.MouseWheel += 1;
          if (event->wheel.y < 0) io.MouseWheel -= 1;
        }
        else {
          view_raw = view_raw.zoom({mouse_x, mouse_y}, double(event->wheel.y) * 0.25);
        }
        break;
      }
      case SDL_MOUSEMOTION: {
        if (io.WantCaptureMouse) {
        }
        else {
          if (event->motion.state & SDL_BUTTON_LMASK) {
            view_raw = view_raw.pan({event->motion.xrel, event->motion.yrel});
          }
        }
        break;
      }
      case SDL_TEXTINPUT: {

        io.AddInputCharactersUTF8(event->text.text);
        break;
      }
      case SDL_KEYDOWN:
      case SDL_KEYUP: {
        int key = event->key.keysym.scancode;
        IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
        io.KeysDown[key] = (event->type == SDL_KEYDOWN);
        io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
        io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
        io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
        io.KeySuper = false;
        break;
      }
      }

      if (event->type == SDL_QUIT) {
        quit = true;
      }

      if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) {
        view_raw = view_raw.reset(screen_w, screen_h);
        if (keyboard_state[SDL_SCANCODE_LSHIFT]) quit = true;
      }
    }

    io.MouseDown[0] = (mouse_buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
    io.MouseDown[1] = (mouse_buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
    io.MouseDown[2] = (mouse_buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;

    const auto world_mouse = view_snap.screenToWorld({mouse_x, mouse_y});
    io.MousePos = { (float)world_mouse.x, (float)world_mouse.y };

    /*
    io.DeltaTime = 1.0f/60.0f;              // set the time elapsed since the previous frame (in seconds)
    io.DisplaySize.x = 1920.0f;             // set the current display width
    io.DisplaySize.y = 1280.0f;             // set the current display height here
    io.MousePos = my_mouse_pos;             // set the mouse position
    io.MouseDown[0] = my_mouse_buttons[0];  // set the mouse button states
    io.MouseDown[1] = my_mouse_buttons[1];
    */

    io.DeltaTime = (float)delta;
    io.DisplaySize.x = float(screen_w);
    io.DisplaySize.y = float(screen_h);


    ImGui::NewFrame();

    //----------------------------------------
    // Client app update

    app->app_update(delta);

    Viewport snapped = view_raw.snap();
    view_smooth = view_smooth.ease(view_raw, delta);
    view_snap = view_snap.ease(snapped, delta);

    if (view_snap == snapped) {
      view_raw = view_snap;
      view_smooth = view_snap;
    }

    //----------------------------------------
    // Client app render

    glViewport(0, 0, screen_w, screen_h);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    app->app_render_frame(view_snap);

    app->app_render_ui(view_snap);

    //ImGui::ShowDemoWindow();

    //----------------------------------------
    // Render ImGui

    ImGui::Render();
    const ImDrawData* draw_data = ImGui::GetDrawData();

    if (draw_data->CmdListsCount) {
      glUseProgram(imgui_prog);

      glUniform4f(glGetUniformLocation(imgui_prog, "viewport"),
                 (float)view_snap.min.x, (float)view_snap.min.y, (float)view_snap.max.x, (float)view_snap.max.y);
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

          const auto clip_min = view_snap.worldToScreen({c->ClipRect.x, c->ClipRect.y});
          const auto clip_max = view_snap.worldToScreen({c->ClipRect.z, c->ClipRect.w});

          const int clip_min_x = (int)clip_min.x;
          const int clip_min_y = (int)clip_min.y;
          const int clip_max_x = (int)clip_max.x;
          const int clip_max_y = (int)clip_max.y;

          glScissor(clip_min_x, screen_h - clip_max_y,
                    clip_max_x - clip_min_x, clip_max_y - clip_min_y);
          glDrawElements(GL_TRIANGLES, c->ElemCount, GL_UNSIGNED_SHORT,
                          reinterpret_cast<void*>(intptr_t(c->IdxOffset * 2)));
        }
      }

      glDisable(GL_SCISSOR_TEST);
    }

    //----------------------------------------
    // Client end frame

    check_gl_error();

    SDL_GL_SwapWindow((SDL_Window*)window);
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
  }

  //----------------------------------------
  // App exit

  app->app_close();

  ImGui::DestroyContext();
  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow((SDL_Window*)window);
  SDL_Quit();

  return 0;
}
