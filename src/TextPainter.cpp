#include "TextPainter.h"

#include <stdio.h>
#include <SDL.h>
#include <GL/gl3w.h>

//-----------------------------------------------------------------------------

const char* inst_vert_src = R"(
#version 460

uniform vec4 screen_size = vec4(1920.0, 1080.0, 1.0 / 1920.0, 1.0 / 1080.0);
uniform vec4 text_pos = vec4(0.0, 0.0, 1.0, 1.0);
uniform vec4 palette[16];
uniform vec4 bg_palette = vec4(0.0, 0.0, 0.0, 0.5);

layout (location = 0) in vec2 corner_pos;
layout (location = 1) in vec4 v_inst;

const float glyph_size_x = 6.0;
const float glyph_size_y = 12.0;

const float glyph_shift_x = 0.0;
const float glyph_shift_y = 3.0;

const float screen_stride_x = 6.0;
const float screen_stride_y = 12.0;

const float atlas_stride_x = 8.0;
const float atlas_stride_y = 16.0;

const float atlas_width = 256.0;
const float atlas_height = 256.0;

out vec2 tc_glyph;
out vec4 fg_color;
out vec4 bg_color;
out float scale;

vec4 screen_to_norm(float x, float y) {
  x = x * screen_size.z * 2.0 - 1.0;
  y = -y * screen_size.w * 2.0 + 1.0;
  return vec4(x, y, 1.0, 1.0);
}

void main() {
  vec2 screen_pos = v_inst.xy;

  int c = int(v_inst.z);
  int s = int(v_inst.w);

  int col = c % 32;
  int row = c / 32;
  int fg_style = s % 16;
  int bg_style = s / 16;

  vec2 atlas_pos = vec2(col, row);

  float quad_x = (screen_pos.x * screen_stride_x) + (corner_pos.x * glyph_size_x);
  float quad_y = (screen_pos.y * screen_stride_y) + (corner_pos.y * glyph_size_y);

  quad_x = quad_x * text_pos.z + text_pos.x;
  quad_y = quad_y * text_pos.w + text_pos.y;

  float glyph_x = (atlas_pos.x * atlas_stride_x) + (corner_pos.x * glyph_size_x) + glyph_shift_x;
  float glyph_y = (atlas_pos.y * atlas_stride_y) + (corner_pos.y * glyph_size_y) + glyph_shift_y;

  glyph_x *= 1.0 / atlas_width;
  glyph_y *= 1.0 / atlas_height;

  gl_Position = screen_to_norm(quad_x, quad_y);
  tc_glyph = vec2(glyph_x, glyph_y);
  fg_color = palette[fg_style];
  bg_color = bg_palette;
  scale = text_pos.z;
}
)";

//----------------------------------------

const char* inst_frag_src = R"(
#version 460

uniform sampler2D font_tex;

in vec2 tc_glyph;
in vec4 fg_color;
in vec4 bg_color;
in float scale;

out vec4 fs_out;
  
void main() {
  float p = texture(font_tex, tc_glyph).r;
  fs_out = mix(bg_color, fg_color, p);
}
)";

//-----------------------------------------------------------------------------

void TextPainter::init(int fb_width_, int fb_height_) {
  fb_width = fb_width_;
  fb_height = fb_height_;

  const char* name = "TextPainter";
  char buf[1024];
  int len = 0;

  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &inst_vert_src, NULL);
  glCompileShader(vertexShader);

  glGetShaderInfoLog(vertexShader, 1024, &len, buf);
  printf("%s vert shader log:\n%s", name, buf);

  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &inst_frag_src, NULL);
  glCompileShader(fragmentShader);

  glGetShaderInfoLog(fragmentShader, 1024, &len, buf);
  printf("%s frag shader log:\n%s", name, buf);

  text_prog = glCreateProgram();
  glAttachShader(text_prog, vertexShader);
  glAttachShader(text_prog, fragmentShader);
  glLinkProgram(text_prog);
  glUseProgram(text_prog);

  glGetProgramInfoLog(text_prog, 1024, &len, buf);
  printf("%s shader prog log:\n%s", name, buf);

  glUseProgram(text_prog);
  glUniform4f(glGetUniformLocation(text_prog, "palette") + 0, 0, 0, 0, 1);
  glUniform4f(glGetUniformLocation(text_prog, "palette") + 1, 1, 1, 1, 1);

  uint8_t quad_tris[2*3*2] = {
    0, 0,
    0, 1,
    1, 1,

    0, 0,
    1, 1,
    1, 0
  };

  {
    glGenBuffers(1, &quad_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
    glBufferStorage(GL_ARRAY_BUFFER, sizeof(quad_tris), quad_tris,
                    GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
  }

  for (buf_idx = 0; buf_idx < 3; buf_idx++) {
    glGenBuffers(1, &inst_vbos[buf_idx]);
    glBindBuffer(GL_ARRAY_BUFFER, inst_vbos[buf_idx]);
    glBufferStorage(GL_ARRAY_BUFFER, 65536*4, nullptr,
                    GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
    glBindBuffer(GL_ARRAY_BUFFER, inst_vbos[buf_idx]);
    inst_maps[buf_idx] = (uint32_t*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    glGenVertexArrays(1, &text_vaos[buf_idx]);
    glBindVertexArray(text_vaos[buf_idx]);

    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_UNSIGNED_BYTE, GL_FALSE, 2, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, inst_vbos[buf_idx]);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 4, (void*)0);
    glVertexAttribDivisor(1, 1);
  }

  SDL_Surface* font_surf = SDL_LoadBMP("terminus2.bmp");
  uint8_t* pix = (uint8_t*)font_surf->pixels;
  for (int i = 0; i < 65536; i++) pix[i] = pix[i] ? 0xFF : 0x00;
  
  glGenTextures(1, &font_tex);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, font_tex);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R8,
                font_surf->w, font_surf->h, 0,
                GL_RED, GL_UNSIGNED_BYTE, pix);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

//-----------------------------------------------------------------------------

void TextPainter::begin_frame() {
  inst_begin = 0;
  inst_end = 0;
  buf_idx = (buf_idx + 1) % 3;
}

//-----------------------------------------------------------------------------

void TextPainter::add_char(const char c) {
  if (c < 10) {
    fg_pal = c;
  }
  else if (c == '\n') {
    text_x = 0;
    text_y++;
  }
  else {
    inst_maps[buf_idx][inst_end++] = (text_x << 0) | (text_y << 8) | (c << 16) | (fg_pal << 24) | (bg_pal << 28);
    text_x++;
  }
}

void TextPainter::add_char(const char c, const char d) {
  add_char(c);
  add_char(d);
}

void TextPainter::add_text(const char* text) {
  for(; *text; text++) {
    add_char(*text);
  }
}

void TextPainter::add_text(const char* text, int len) {
  for (int i = 0; i < len; i++) {
    add_char(text[i]);
  }
}

void TextPainter::dprintf(const char* format, ...) {
  char buffer[256];
  va_list args;
  va_start(args, format);
  int count = vsnprintf(buffer, 256, format, args);
  va_end(args);
  add_text(buffer, count);
}

void TextPainter::render(float x, float y, float scale) {
  glMemoryBarrier(GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT);

  // Render the glyphs
  glUseProgram(text_prog);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, font_tex);
  glUniform1i(glGetUniformLocation(text_prog, "font_tex"), 0);
  glUniform4f(glGetUniformLocation(text_prog, "text_pos"), x, y, scale, scale);
  glUniform4f(glGetUniformLocation(text_prog, "screen_size"),
              (float)fb_width, (float)fb_height, 1.0f / fb_width, 1.0f / fb_height);
  glBindVertexArray(text_vaos[buf_idx]);
  glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, 6, inst_end - inst_begin, inst_begin);
  text_x = 0;
  text_y = 0;
  inst_begin = inst_end;
}

void TextPainter::set_pal(int index, float r, float g, float b, float a) {
  glUseProgram(text_prog);
  glUniform4f(glGetUniformLocation(text_prog, "palette") + index, r, g, b, a);
}

//-----------------------------------------------------------------------------
