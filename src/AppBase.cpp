#include "AppBase.h"

#include "GLBase.h"

#include <stdio.h>
#include <include/SDL.h>
#include <imgui.h>

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

layout (location = 0) out vec2 frag_tc;
layout (location = 1) out vec4 frag_col;

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

layout (location = 0) in vec2 frag_tc;
layout (location = 1) in vec4 frag_col;

layout (location = 0) out vec4 frag;

void main() {
  frag = frag_col * texture(tex, frag_tc.st).rrrr;
}

#endif
)";

//-----------------------------------------------------------------------------

int AppBase::main(int, char**) {

  //----------------------------------------
  // Create window

  SDL_Init(SDL_INIT_VIDEO);

  //screen_w = 1856;
  //screen_h = 1024;

  screen_w = 1920;
  screen_h = 1080;

  window = SDL_CreateWindow("MetroBoy Game Boy Simulator",
                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            screen_w, screen_h,
                            SDL_WINDOW_OPENGL /*| SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI*/);

  keyboard_state = SDL_GetKeyboardState(nullptr);
  app_start = SDL_GetPerformanceCounter();
  perf_freq = SDL_GetPerformanceFrequency();

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

  blitter.init();
  text_painter.init();
  {
    init();
  }

  //----------------------------------------
  // Loop forever

  static uint64_t now  = SDL_GetPerformanceCounter();

  while (!quit) {
    uint64_t new_now = SDL_GetPerformanceCounter();
    double delta = double(new_now - now) / double(perf_freq);
    now = new_now;

    //----------------------------------------
    // Peek events and dispatch to ImGui

    SDL_Event events[64];
    SDL_PumpEvents();
    int nevents = SDL_PeepEvents(events, 64, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
    for(int i = 0; i < nevents; i++) {
      SDL_Event* event = &events[i];

      switch (event->type)
      {
      case SDL_MOUSEWHEEL:
          {
              if (event->wheel.x > 0) io.MouseWheelH += 1;
              if (event->wheel.x < 0) io.MouseWheelH -= 1;
              if (event->wheel.y > 0) io.MouseWheel += 1;
              if (event->wheel.y < 0) io.MouseWheel -= 1;
              break;
          }
      case SDL_TEXTINPUT:
          {
              io.AddInputCharactersUTF8(event->text.text);
              break;
          }
      case SDL_KEYDOWN:
      case SDL_KEYUP:
          {
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

      if (event->type == SDL_QUIT) quit = true;
      if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) {
        if (keyboard_state[SDL_SCANCODE_LSHIFT]) quit = true;
      }
    }

    update(delta);

    /*
    if (!redraw_count) {
      SDL_GL_SwapWindow(window);
      SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
      SDL_Delay(10);
    }
    */

    //----------------------------------------
    // Client app update

    SDL_GL_GetDrawableSize(window, &screen_w, &screen_h);
    glViewport(0, 0, screen_w, screen_h);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Viewport view = get_viewport();
    text_painter.begin_frame(view);
    begin_frame();
    render_frame();

    //----------------------------------------
    // Render UI

    int mx, my;
    Uint32 mouse_buttons = SDL_GetMouseState(&mx, &my);

    io.MouseDown[0] = (mouse_buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
    io.MouseDown[1] = (mouse_buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
    io.MouseDown[2] = (mouse_buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;

    //io.MousePos = ImVec2((float)mx, (float)my);
    auto world_mouse = view.screenToWorld({mx, my});
    io.MousePos = {
      (float)world_mouse.x,
      (float)world_mouse.y
    };

    io.DeltaTime = (float)delta;
    io.DisplaySize.x = float(screen_w);
    io.DisplaySize.y = float(screen_h);
    ImGui::NewFrame();

    render_ui();

    /*
    ImGui::ShowDemoWindow();
    */

    /*
    ImGui::SetNextWindowPos({(float)screen_w,(float)screen_h}, 0, {1,1});
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
    */

    //----------------------------------------
    // Render ImGui

    ImGui::Render();

    glUseProgram(imgui_prog);

    glUniform4f(glGetUniformLocation(imgui_prog, "viewport"),
               (float)view.min.x, (float)view.min.y, (float)view.max.x, (float)view.max.y);
    glUniform1i(glGetUniformLocation(imgui_prog, "tex"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, imgui_tex);
    glBindVertexArray(imgui_vao);
    glBindBuffer(GL_ARRAY_BUFFER, imgui_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, imgui_ibo);

    glEnable(GL_SCISSOR_TEST);

    ImDrawData* draw_data = ImGui::GetDrawData();
    for (int n = 0; n < draw_data->CmdListsCount; n++) {
      const ImDrawList* l = draw_data->CmdLists[n];

      glBufferData(GL_ARRAY_BUFFER, l->VtxBuffer.Size * sizeof(ImDrawVert), l->VtxBuffer.Data, GL_STREAM_DRAW);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, l->IdxBuffer.Size * sizeof(ImDrawIdx), l->IdxBuffer.Data, GL_STREAM_DRAW);

      for (int i = 0; i < l->CmdBuffer.Size; i++) {
        const ImDrawCmd* c = &l->CmdBuffer[i];

        auto clip_min = view.worldToScreen({c->ClipRect.x, c->ClipRect.y});
        auto clip_max = view.worldToScreen({c->ClipRect.z, c->ClipRect.w});

        int clip_min_x = (int)clip_min.x;
        int clip_min_y = (int)clip_min.y;
        int clip_max_x = (int)clip_max.x;
        int clip_max_y = (int)clip_max.y;

        glScissor(clip_min_x, screen_h - clip_max_y,
                  clip_max_x - clip_min_x, clip_max_y - clip_min_y);
        glDrawElements(GL_TRIANGLES, c->ElemCount, GL_UNSIGNED_SHORT,
                        reinterpret_cast<void*>(intptr_t(c->IdxOffset * 2)));
      }
    }

    glDisable(GL_SCISSOR_TEST);

    //----------------------------------------
    // Client end frame

    end_frame();

    text_painter.end_frame();

    check_gl_error();

    SDL_GL_SwapWindow(window);
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
    frame_count++;
    if (redraw_count) redraw_count--;
  }

  //----------------------------------------
  // App exit

  close();

  ImGui::DestroyContext();
  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
