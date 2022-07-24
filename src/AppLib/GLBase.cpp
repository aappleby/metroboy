#include "AppLib/GLBase.h"
#include <stdio.h>
#include <vector>
#include "SDL/include/SDL.h"
#include "glad/glad.h"

#include "CoreLib/Tests.h"

#include <map>

/*
std::map<uint32_t, const char*> messageMap = {
  {GL_DEBUG_SOURCE_API,               "API" },
  {GL_DEBUG_SOURCE_APPLICATION,       "APPLICATION" },
  {GL_DEBUG_SOURCE_OTHER,             "OTHER" },
  {GL_DEBUG_SOURCE_SHADER_COMPILER,   "SHADER_COMPILER" },
  {GL_DEBUG_SOURCE_THIRD_PARTY,       "THIRD_PARTY" },
  {GL_DEBUG_SOURCE_WINDOW_SYSTEM,     "WINDOW_SYSTEM" },

  {GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "DEPRECATED" },
  {GL_DEBUG_TYPE_ERROR,               "ERROR" },
  {GL_DEBUG_TYPE_MARKER,              "MARKER" },
  {GL_DEBUG_TYPE_OTHER,               "OTHER" },
  {GL_DEBUG_TYPE_PERFORMANCE,         "PERFORMANCE" },
  {GL_DEBUG_TYPE_POP_GROUP,           "POP_GROUP" },
  {GL_DEBUG_TYPE_PORTABILITY,         "PORTABILITY" },
  {GL_DEBUG_TYPE_PUSH_GROUP,          "PUSH_GROUP" },
  {GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR,  "UNDEFINED" },


  {GL_DEBUG_SEVERITY_HIGH,            "HIGH" },
  {GL_DEBUG_SEVERITY_MEDIUM,          "MEDIUM" },
  {GL_DEBUG_SEVERITY_LOW,             "LOW" },
  {GL_DEBUG_SEVERITY_NOTIFICATION,    "NOTIFICATION" },
};
*/

//-----------------------------------------------------------------------------

/*
void APIENTRY debugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
                          GLsizei length, const GLchar* message, const GLvoid* userParam) {

  (void)source;
  (void)type;
  (void)id;
  (void)severity;
  (void)length;
  (void)message;
  (void)userParam;

  // "will use VIDEO memory as the source..."
  if (id == 0x20071) return;

  LOG_B("GLDEBUG %.20s:%.20s:%.20s (0x%08x) %s\n",
    messageMap[source],
    messageMap[type],
    messageMap[severity],
    id,
    message);
}
*/

//-----------------------------------------------------------------------------

void* init_gl(void* window) {
  LOG_G("GLBase::init_gl\n");

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                      SDL_GL_CONTEXT_DEBUG_FLAG 
                      //| SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG
                      //| SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG
                      );

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  SDL_GLContext gl_context = SDL_GL_CreateContext((SDL_Window*)window);

  SDL_GL_SetSwapInterval(1);  // Enable vsync
  //SDL_GL_SetSwapInterval(0); // Disable vsync

  gladLoadGLLoader(SDL_GL_GetProcAddress);

  /*
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
  glDebugMessageCallback(debugOutput, nullptr);
  */

  int ext_count = 0;
  glGetIntegerv(GL_NUM_EXTENSIONS, &ext_count);

  LOG_INDENT();
  LOG_B("Vendor:   "); LOG_G("%s\n", glGetString(GL_VENDOR));
  LOG_B("Renderer: "); LOG_G("%s\n", glGetString(GL_RENDERER));
  LOG_B("Version:  "); LOG_G("%s\n", glGetString(GL_VERSION));
  LOG_B("GLSL:     "); LOG_G("%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
  LOG_B("Ext count "); LOG_G("%d\n", ext_count);
  LOG_DEDENT();

#if 0
  for (int i = 0; i < ext_count; i++) {
    LOG_B("Ext %2d: %s\n", i, glGetStringi(GL_EXTENSIONS, i));
  }
#endif

  //----------------------------------------
  // Set initial GL state

  glDisable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0.1f, 0.1f, 0.2f, 0.f);
  //glClearDepthf(1.0);
  glClearDepth(1.0);

  return (void*)gl_context;
}

//-----------------------------------------------------------------------------

void check_gl_error() {
  int err = glGetError();
  if (err) {
    LOG_B("glGetError %d\n", err);
  }
}

//-----------------------------------------------------------------------------

int create_vao() {
  int vao = 0;
  glGenVertexArrays(1, (GLuint*)&vao);
  glBindVertexArray(vao);
  return vao;
}

void bind_vao(int vao) {
  glBindVertexArray(vao);
}

//-----------------------------------------------------------------------------

int create_vbo() {
  int vbo = 0;
  glGenBuffers(1, (GLuint*)&vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  return vbo;
}

int create_vbo(int size_bytes, const void* data) {
  int vbo = create_vbo();
  glBufferData(GL_ARRAY_BUFFER, size_bytes, data, GL_DYNAMIC_DRAW);
  return vbo;
}

void update_vbo(int vbo, int size_bytes, const void* data) {
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, size_bytes, data, GL_DYNAMIC_DRAW);
}


//-----------------------------------------------------------------------------

int create_ibo() {
  int ibo = 0;
  glGenBuffers(1, (GLuint*)&ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  return ibo;
}

void update_ibo(int ibo, int size_bytes, const uint16_t* data) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_bytes, data, GL_DYNAMIC_DRAW);
}

//-----------------------------------------------------------------------------

int create_ubo() {
  int ubo = 0;
  glGenBuffers(1, (GLuint*)&ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, ubo);
  return ubo;
}

int create_ubo(int size_bytes) {
  int ubo = create_ubo();
  glBufferData(GL_UNIFORM_BUFFER, size_bytes, nullptr, GL_DYNAMIC_DRAW);
  return ubo;
}

void update_ubo(int ubo, int size_bytes, const void* data) {
  glBindBuffer(GL_UNIFORM_BUFFER, ubo);
  glBufferData(GL_UNIFORM_BUFFER, size_bytes, data, GL_DYNAMIC_DRAW);
}

void bind_ubo(int prog, const char* name, int index, int ubo) {
  glUniformBlockBinding(prog, glGetUniformBlockIndex(prog, name), index);
  glBindBufferBase(GL_UNIFORM_BUFFER, index, ubo);
}

//-----------------------------------------------------------------------------

int create_texture_u32(int width, int height, const void* data, bool filter) {
  int tex = 0;
  glGenTextures(1, (GLuint*)&tex);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter ? GL_LINEAR : GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter ? GL_LINEAR : GL_NEAREST);

  return tex;
}

//----------------------------------------

void update_texture_u32(int tex, int width, int height, const void* pix) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexSubImage2D(GL_TEXTURE_2D, 0,
                  0, 0, width, height,
                  GL_RGBA, GL_UNSIGNED_BYTE, pix);
}

//-----------------------------------------------------------------------------

int create_texture_u8(int width, int height, const void* data, bool filter) {
  int tex = 0;
  glGenTextures(1, (GLuint*)&tex);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter ? GL_LINEAR : GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter ? GL_LINEAR : GL_NEAREST);

  return tex;
}

//----------------------------------------

void update_texture_u8(int tex, int width, int height, const void* pix) {
  update_texture_u8(tex, 0, 0, width, height, pix);
}

void update_texture_u8(int tex, int dx, int dy, int dw, int dh, const void* pix) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexSubImage2D(GL_TEXTURE_2D, 0,
                  dx, dy, dw, dh,
                  GL_RED, GL_UNSIGNED_BYTE, pix);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

//-----------------------------------------------------------------------------

int create_table_u8(int width, int height, const void* data) {
  int tex = 0;
  glGenTextures(1, (GLuint*)&tex);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, data);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  return tex;
}

//----------------------------------------

void update_table_u8(int tex, int width, int height, const void* pix) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexSubImage2D(GL_TEXTURE_2D, 0,
                  0, 0, width, height,
                  GL_RED_INTEGER, GL_UNSIGNED_BYTE, pix);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

//-----------------------------------------------------------------------------

int create_table_u32(int width, int height, const void* data) {
  int tex = 0;
  glGenTextures(1, (GLuint*)&tex);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8UI, width, height, 0, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  return tex;
}

//----------------------------------------

void update_table_u32(int tex, int width, int height, const void* pix) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexSubImage2D(GL_TEXTURE_2D, 0,
                  0, 0, width, height,
                  GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, pix);
}

//-----------------------------------------------------------------------------

void bind_texture(int prog, const char* name, int index, int tex) {
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(GL_TEXTURE_2D, tex);
  glUniform1i(glGetUniformLocation(prog, name), index);
}

void bind_table(int prog, const char* name, int index, int tex) {
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(GL_TEXTURE_2D, tex);
  glUniform1i(glGetUniformLocation(prog, name), index);
}

//-----------------------------------------------------------------------------

int create_shader(const char* name, const char* src) {
  static bool verbose = false;
  DCHECK_N(glGetError());

  LOG_B("Compiling %s\n", name);

  auto vert_srcs = {
    //"#version 450\n",
    "#version 330\n"
    "precision highp float;\n",
    "precision highp int;\n",
    "precision highp usampler2D;\n",
    "#define _VERTEX_\n",
    src
  };

  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, (int)vert_srcs.size(), vert_srcs.begin(), NULL);
  glCompileShader(vertexShader);

  int vshader_result = 0;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vshader_result);

  if ((vshader_result == GL_FALSE) || verbose) {
    char buf[1024];
    int len = 0;
    glGetShaderInfoLog(vertexShader, 1024, &len, buf);
    LOG_B("  Vert shader log %s\n", buf);
  }

  auto frag_srcs = {
    //"#version 450\n",
    "#version 330\n"
    "precision highp float;\n",
    "precision highp int;\n",
    "precision highp usampler2D;\n",
    "#define _FRAGMENT_\n",
    src
  };
  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, (int)frag_srcs.size(), frag_srcs.begin(), NULL);
  glCompileShader(fragmentShader);

  int fshader_result = 0;
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fshader_result);

  if ((fshader_result == GL_FALSE) || verbose) {
    char buf[1024];
    int len = 0;
    glGetShaderInfoLog(fragmentShader, 1024, &len, buf);
    LOG_B("  Frag shader log %s\n", buf);
  }

  int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  int prog_result = 0;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &prog_result);

  if((prog_result == GL_FALSE) || verbose) {
    char buf[1024];
    int len = 0;
    glGetProgramInfoLog(shaderProgram, 1024, &len, buf);
    LOG_B("  Shader program log %s\n", buf);

  }

  glDetachShader(shaderProgram, vertexShader);
  glDetachShader(shaderProgram, fragmentShader);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  if (verbose) {
    int program = shaderProgram;
    int count = 0;


    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
    LOG_B("  Active Attributes: %d\n", count);

    for (int i = 0; i < count; i++) {
      const int bufSize = 16;
      GLenum type;
      GLchar var_name[bufSize];
      GLsizei length;
      GLint size;
      glGetActiveAttrib(program, (GLuint)i, bufSize, &length, &size, &type, var_name);
      LOG_B("    Attribute #%d Type: %u Name: %s\n", i, type, var_name);
    }

    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
    LOG_B("  Active Uniforms: %d\n", count);

    for (int i = 0; i < count; i++) {
      const int bufSize = 16;
      GLenum type;
      GLchar var_name[bufSize];
      GLsizei length;
      GLint size;
      glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, var_name);
      int loc = glGetUniformLocation(program, var_name);
      LOG_B("    Uniform '%16s' @ %2d Type: 0x%04x\n", var_name, loc, type);
    }

    glGetProgramiv(program, GL_ATTACHED_SHADERS, &count);
    LOG_B("  Attached shaders: %d\n", count);

    glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &count);
    LOG_B("  Uniform blocks: %d\n", count);

    for (int i = 0; i < count; i++) {
      const int bufSize = 16;
      GLchar var_name[bufSize];
      GLsizei length;

      glGetActiveUniformBlockName(program, i, bufSize, &length, var_name);
      LOG_B("  Uniform block #%d Name: %s\n", i, var_name);

      int temp[16];

      glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_BINDING, temp);
      LOG_B("    GL_UNIFORM_BLOCK_BINDING %d\n", temp[0]);

      glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_DATA_SIZE, temp);
      LOG_B("    GL_UNIFORM_BLOCK_DATA_SIZE %d\n", temp[0]);

      glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, temp);
      LOG_B("    GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS %d\n", temp[0]);

      glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER, temp);
      LOG_B("    GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER %d\n", temp[0]);

      glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER, temp);
      LOG_B("    GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER %d\n", temp[0]);
    }
  }

  LOG_B("Compiling %s done\n", name);
  if (verbose) LOG_B("\n");

  return shaderProgram;
}

//----------------------------------------

void bind_shader(int shader) {
  glUseProgram(shader);
}

//-----------------------------------------------------------------------------
