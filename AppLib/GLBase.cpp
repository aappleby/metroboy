#include "AppLib/GLBase.h"
#include <stdio.h>
#include <vector>
#include "SDL/include/SDL.h"
#include "glad/glad.h"

#include "CoreLib/Tests.h"

// using an OpenGL ES 3.0 context causes the gamma to be wrong due to some
// incorrect SRGB conversion (I think). So, we use OpenGL 4.3 (which supports glDebugMessageCallback).

// es broken right now
//#define USE_OPENGL_ES

/*
extern "C" {
_declspec(dllexport) int NvOptimusEnablement = 0x00000001;
}
*/

//-----------------------------------------------------------------------------

void APIENTRY debugOutput(GLenum /*source*/, GLenum /*type*/, GLuint /*id*/, GLenum severity,
                          GLsizei /*length*/, const GLchar* message, const GLvoid* /*userParam*/) {

  if (severity == GL_DEBUG_TYPE_ERROR ||
      severity == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR ||
      severity == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR ||
      severity == GL_DEBUG_TYPE_PORTABILITY ||
      severity == GL_DEBUG_TYPE_PERFORMANCE) {
  	printf("GLDEBUG: %s\n", message);
  }
  else {
  	//printf("GLDEBUG: %s\n", message);
  }
  //#define GL_DEBUG_TYPE_OTHER 0x8251

}

//-----------------------------------------------------------------------------

void* init_gl(void* window) {
  LOG_G("init_gl\n");
  LOG_INDENT();

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                      SDL_GL_CONTEXT_DEBUG_FLAG |
                          SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG |
                          SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

#ifdef USE_OPENGL_ES
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  SDL_GLContext gl_context = SDL_GL_CreateContext((SDL_Window*)window);
  SDL_GL_SetSwapInterval(1);  // Enable vsync
  // SDL_GL_SetSwapInterval(0); // Disable vsync

  //gladLoadGLES2Loader(SDL_GL_GetProcAddress);
  gladLoadGLLoader(SDL_GL_GetProcAddress);

  LOG_B("Vendor:   "); LOG_G("%s\n", glGetString(GL_VENDOR));
  LOG_B("Renderer: "); LOG_G("%s\n", glGetString(GL_RENDERER));
  LOG_B("Version:  "); LOG_G("%s\n", glGetString(GL_VERSION));
  LOG_B("GLSL:     "); LOG_G("%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

#ifndef USE_OPENGL_ES
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
  glDebugMessageCallback(debugOutput, nullptr);
#endif

  int ext_count = 0;
  glGetIntegerv(GL_NUM_EXTENSIONS, &ext_count);
  LOG_B("Ext count "); LOG_G("%d\n", ext_count);
#if 0
  for (int i = 0; i < ext_count; i++) {
    printf("Ext %2d: %s\n", i, glGetStringi(GL_EXTENSIONS, i));
  }
#endif

  //----------------------------------------
  // Set initial GL state

  // glEnable(GL_DEPTH_TEST);
  // glDepthFunc(GL_LEQUAL);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0.1f, 0.1f, 0.2f, 0.f);

#ifdef USE_OPENGL_ES
  glClearDepth(1.0);
#else
  glClearDepthf(1.0);
#endif

  LOG_DEDENT();
  return (void*)gl_context;
}

//-----------------------------------------------------------------------------

void check_gl_error() {
  int err = glGetError();
  if (err) {
    printf("glGetError %d\n", err);
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

int create_vbo(int size_bytes, const void* data) {
  int vbo = 0;
  glGenBuffers(1, (GLuint*)&vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, size_bytes, data, GL_DYNAMIC_DRAW);
  return vbo;
}

void update_vbo(int vbo, int size_bytes, const void* data) {
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, size_bytes, data, GL_DYNAMIC_DRAW);
}


//-----------------------------------------------------------------------------

int create_ibo(int size_bytes, const void* data) {
  int vbo = 0;
  glGenBuffers(1, (GLuint*)&vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_bytes, data, GL_DYNAMIC_DRAW);
  return vbo;
}

void update_ibo(int vbo, int size_bytes, const uint16_t* data) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_bytes, data, GL_DYNAMIC_DRAW);
}

//-----------------------------------------------------------------------------

int create_ubo(int size_bytes) {
  int ubo = 0;
  glGenBuffers(1, (GLuint*)&ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, ubo);
  glBufferData(GL_UNIFORM_BUFFER, size_bytes, nullptr, GL_DYNAMIC_DRAW);
  return ubo;
}

void update_ubo(int ubo, int size_bytes, const void* data) {
  glBindBuffer(GL_UNIFORM_BUFFER, ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, size_bytes, data);
}

void bind_ubo(int prog, const char* name, int index, int ubo) {
  glUniformBlockBinding(prog, glGetUniformBlockIndex(prog, name), index);
  glBindBufferBase(GL_UNIFORM_BUFFER, index, ubo);
}

//-----------------------------------------------------------------------------

int create_texture_u32(int width, int height, const void* data) {
  int tex = 0;
  glGenTextures(1, (GLuint*)&tex);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  bool filter = false;

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

  LOG_B("Compiling %s\n", name);

  auto vert_srcs = {
    "#version 300 es\n",
    "#define _VERTEX_\n",
    "precision highp float;\n",
    "precision highp int;\n",
    src
  };

  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, (int)vert_srcs.size(), vert_srcs.begin(), NULL);
  glCompileShader(vertexShader);

  {
    char buf[1024];
    int len = 0;
    glGetShaderInfoLog(vertexShader, 1024, &len, buf);
    if (verbose) printf("  Vert shader log %s\n", buf);
  }


  auto frag_srcs = {
    "#version 300 es\n",
    "#define _FRAGMENT_\n",
    "precision highp float;\n",
    "precision highp int;\n",
    src
  };
  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, (int)frag_srcs.size(), frag_srcs.begin(), NULL);
  glCompileShader(fragmentShader);

  {
    char buf[1024];
    int len = 0;
    glGetShaderInfoLog(fragmentShader, 1024, &len, buf);
    if (verbose) printf("  Frag shader log %s\n", buf);
  }

  int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  {
    char buf[1024];
    int len = 0;
    glGetProgramInfoLog(shaderProgram, 1024, &len, buf);
    if (verbose) printf("  Shader program log %s\n", buf);

  }

  glDetachShader(shaderProgram, vertexShader);
  glDetachShader(shaderProgram, fragmentShader);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  if (verbose) {
    int program = shaderProgram;
    int count = 0;


    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
    printf("  Active Attributes: %d\n", count);

    for (int i = 0; i < count; i++) {
      const int bufSize = 16;
      GLenum type;
      GLchar var_name[bufSize];
      GLsizei length;
      GLint size;
      glGetActiveAttrib(program, (GLuint)i, bufSize, &length, &size, &type, var_name);
      printf("    Attribute #%d Type: %u Name: %s\n", i, type, var_name);
    }

    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
    printf("  Active Uniforms: %d\n", count);

    for (int i = 0; i < count; i++) {
      const int bufSize = 16;
      GLenum type;
      GLchar var_name[bufSize];
      GLsizei length;
      GLint size;
      glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, var_name);
      int loc = glGetUniformLocation(program, var_name);
      printf("    Uniform '%16s' @ %2d Type: 0x%04x\n", var_name, loc, type);
    }

    glGetProgramiv(program, GL_ATTACHED_SHADERS, &count);
    printf("  Attached shaders: %d\n", count);

    glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &count);
    printf("  Uniform blocks: %d\n", count);

    for (int i = 0; i < count; i++) {
      const int bufSize = 16;
      GLchar var_name[bufSize];
      GLsizei length;

      glGetActiveUniformBlockName(program, i, bufSize, &length, var_name);
      printf("  Uniform block #%d Name: %s\n", i, var_name);

      int temp[16];

      glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_BINDING, temp);
      printf("    GL_UNIFORM_BLOCK_BINDING %d\n", temp[0]);

      glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_DATA_SIZE, temp);
      printf("    GL_UNIFORM_BLOCK_DATA_SIZE %d\n", temp[0]);

      glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, temp);
      printf("    GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS %d\n", temp[0]);

      glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER, temp);
      printf("    GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER %d\n", temp[0]);

      glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER, temp);
      printf("    GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER %d\n", temp[0]);
    }
  }

  LOG_B("Compiling %s done\n", name);
  if (verbose) printf("\n");

  return shaderProgram;
}

//----------------------------------------

void bind_shader(int shader) {
  glUseProgram(shader);
}

//-----------------------------------------------------------------------------
