#include "TextPainter.h"

#include <stdio.h>
#include <include/SDL.h>
#include <glad/glad.h>

extern const char* terminus;

//-----------------------------------------------------------------------------

const char* text_hdr = R"(
#version 300 es

precision highp float;
precision highp int;

const float glyph_size_x = 6.0;
const float glyph_size_y = 12.0;

const float atlas_stride_x = 8.0;
const float atlas_stride_y = 16.0;

const float atlas_width = 256.0;
const float atlas_height = 128.0;

const float inv_atlas_width = 1.0 / atlas_width;
const float inv_atlas_height = 1.0 / atlas_height;

uniform sampler2D font_tex;
 
layout(std140) uniform TextUniforms {
  vec4  screen_size;
  vec4  viewport;
  vec4  text_pos;
  vec4  bg_palette;
  vec4  palette[16];
};

)";

//-----------------------------------------------------------------------------

const char* text_vert_src = R"(

layout(location = 0) in uint glyph2;

out vec2  tc_glyph;
out vec4  fg_color;
out vec4  bg_color;

void main() {
  float corner_x = float((gl_VertexID >> 0) & 1);
  float corner_y = float((gl_VertexID >> 1) & 1);

  uint glyph_packed = glyph2;
  float glyph_x = float((glyph_packed >>  0) & 0xFFu);
  float glyph_y = float((glyph_packed >>  8) & 0xFFu);
  int col       = int  ((glyph_packed >> 16) & 0x1Fu);
  int row       = int  ((glyph_packed >> 21) & 0x07u);
  int fg_style  = int  ((glyph_packed >> 24) & 0xFFu);

  float glyph_tcx = (float(col) * atlas_stride_x) + (corner_x * glyph_size_x);
  float glyph_tcy = (float(row) * atlas_stride_y) + (corner_y * glyph_size_y);

  glyph_tcx *= inv_atlas_width;
  glyph_tcy *= inv_atlas_height;

  tc_glyph = vec2(glyph_tcx, glyph_tcy);
  fg_color = palette[fg_style];
  bg_color = bg_palette;

  //----------

  float quad_x = (glyph_x + corner_x) * glyph_size_x;
  float quad_y = (glyph_y + corner_y) * glyph_size_y;

  quad_x = quad_x * text_pos.z + text_pos.x;
  quad_y = quad_y * text_pos.w + text_pos.y;

  quad_x = quad_x - viewport[0];
  quad_y = quad_y - viewport[1];

  quad_x = quad_x * viewport[2];
  quad_y = quad_y * viewport[3];

  quad_x = quad_x * 2.0 - 1.0;
  quad_y = quad_y * 2.0 - 1.0;

  gl_Position = vec4(quad_x, -quad_y, 1.0, 1.0);
}
)";

//-----------------------------------------------------------------------------

const char* text_frag_src = R"(

in vec2 tc_glyph;
in vec4 fg_color;
in vec4 bg_color;

out vec4 fs_out;

void main() {
  float p = texture(font_tex, tc_glyph).r;
  fs_out = mix(bg_color, fg_color, p);
}
)";

//-----------------------------------------------------------------------------

void TextPainter::init() {
  const char* vert_srcs[] = { text_hdr, text_vert_src };
  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 2, vert_srcs, NULL);
  glCompileShader(vertexShader);


  const char* frag_srcs[] = { text_hdr, text_frag_src };
  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 2, frag_srcs, NULL);
  glCompileShader(fragmentShader);


  text_prog = glCreateProgram();
  glAttachShader(text_prog, vertexShader);
  glAttachShader(text_prog, fragmentShader);
  glLinkProgram(text_prog);

  {
    uint32_t program = text_prog;
    GLint i;
    GLint count;

    GLint size; // size of the variable
    GLenum type; // type of the variable (float, vec3 or mat4, etc)

    const GLsizei bufSize = 16; // maximum name length
    GLchar name[bufSize]; // variable name in GLSL
    GLsizei length; // name length

    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
    printf("Active Attributes: %d\n", count);

    for (i = 0; i < count; i++) {
      glGetActiveAttrib(program, (GLuint)i, bufSize, &length, &size, &type, name);
      printf("Attribute #%d Type: %u Name: %s\n", i, type, name);
    }

    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
    printf("Active Uniforms: %d\n", count);

    for (i = 0; i < count; i++) {
      glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, name);
      printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
    }

    glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &count);
    for (i = 0; i < count; i++) {
      glGetActiveUniformBlockName(program, i, bufSize, &length, name);
      printf("Uniform block #%d Name: %s\n", i, name);

      int temp[16];

      glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_BINDING, temp);
      printf("GL_UNIFORM_BLOCK_BINDING %d\n", temp[0]);

      glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_DATA_SIZE, temp);
      printf("GL_UNIFORM_BLOCK_DATA_SIZE %d\n", temp[0]);

      glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_NAME_LENGTH, temp);
      printf("GL_UNIFORM_BLOCK_NAME_LENGTH %d\n", temp[0]);

      glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, temp);
      printf("GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS %d\n", temp[0]);

      glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, temp);
      printf("GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES %d\n", temp[0]);

      glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER, temp);
      printf("GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER %d\n", temp[0]);

      glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER, temp);
      printf("GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER %d\n", temp[0]);
    }
  }

  set_pal(0, 0.4, 0.4, 0.4, 1.0); // grey
  set_pal(1, 0.8, 0.8, 0.8, 1.0); // white 
  set_pal(2, 0.6, 1.0, 0.6, 1.0); // lo-z out = green
  set_pal(3, 1.0, 0.6, 0.6, 1.0); // hi-z out = red
  set_pal(4, 0.6, 0.6, 1.0, 1.0); // lo-z in = blue
  set_pal(5, 1.0, 1.0, 0.6, 1.0); // hi-z in = yellow
  set_pal(6, 1.0, 0.6, 1.0, 1.0); // error magenta

  {
    glGenVertexArrays(1, &dummy_vao);
    glBindVertexArray(dummy_vao);

    glGenBuffers(1, &inst_vbo);
    inst_data = new uint32_t[65536];
    glBindBuffer(GL_ARRAY_BUFFER, inst_vbo);
    //glNamedBufferStorage(inst_vbo, 65536 * 4, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBufferData(GL_ARRAY_BUFFER, 65536 * 4, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, 4, 0);
    glVertexAttribDivisor(0, 1);
  }

  glGenTextures(1, &font_tex);
  glBindTexture(GL_TEXTURE_2D, font_tex);
  //glTextureStorage2D(font_tex, 1, GL_RGBA8, 256, 128);

  uint32_t* dst_pix = new uint32_t[32768];
  for (int i = 0; i < 32768; i++) dst_pix[i] = terminus[i] == '#' ? 0xFFFFFFFF : 0x00000000;
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, dst_pix);
  delete [] dst_pix;

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glGenBuffers(1, &text_ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, text_ubo);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(TextUniforms), nullptr, GL_DYNAMIC_DRAW);
  //glNamedBufferStorage(text_ubo, sizeof(TextUniforms), nullptr, GL_DYNAMIC_STORAGE_BIT);

  text_uniforms.bg_palette = vec4(0.0, 0.0, 0.0, 0.5);
}

//-----------------------------------------------------------------------------

void TextPainter::render(float x, float y, float scale) {
  int temp[4] = {0};
  glGetIntegerv(GL_VIEWPORT, temp);
  int fb_width  = temp[2];
  int fb_height = temp[3];

  text_uniforms.text_pos = {x, y, scale, scale};
  text_uniforms.screen_size = {(float)fb_width, (float)fb_height, 1.0f / fb_width, 1.0f / fb_height};
  text_uniforms.viewport = {(float)viewport.mx(), (float)viewport.my(), 1.0f / (float)viewport.dx(), 1.0f / (float)viewport.dy()};

  glBindBuffer(GL_UNIFORM_BUFFER, text_ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(text_uniforms), &text_uniforms);

  glUseProgram(text_prog);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, font_tex);
  glUniform1i(glGetUniformLocation(text_prog, "font_tex"), 0);

  //glNamedBufferSubData(inst_vbo, inst_begin * 4, (inst_end - inst_begin)*4, inst_data + inst_begin);
  glBindBuffer(GL_ARRAY_BUFFER, inst_vbo);
  glBufferSubData(GL_ARRAY_BUFFER, inst_begin * 4, (inst_end - inst_begin)*4, inst_data + inst_begin);

  glBindVertexArray(dummy_vao);
  glBindBufferBase(GL_UNIFORM_BUFFER, glGetUniformBlockIndex(text_prog, "TextUniforms"), text_ubo);

  //glDrawArraysInstancedBaseInstance(GL_TRIANGLE_STRIP, 0, 4, inst_end - inst_begin, inst_begin);
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, inst_end - inst_begin);

  text_x = 0;
  text_y = 0;
  inst_begin = inst_end = 0;
}

//-----------------------------------------------------------------------------

void TextPainter::begin_frame() {
  inst_begin = 0;
  inst_end = 0;
}

void TextPainter::end_frame() {
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
    uint32_t packed = (text_x << 0) | (text_y << 8) | (c << 16) | (fg_pal << 24);
    inst_data[inst_end] = packed;

    inst_end++;
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

void TextPainter::add_string(const std::string& text) {
  for (auto c : text) add_char(c);
}

void TextPainter::dprintf(const char* format, ...) {
  char buffer[256];
  va_list args;
  va_start(args, format);
  int count = vsnprintf(buffer, 256, format, args);
  va_end(args);
  add_text(buffer, count);
}

void TextPainter::set_pal(int index, float r, float g, float b, float a) {
  text_uniforms.palette[index] = {r,g,b,a};
}

//-----------------------------------------------------------------------------
