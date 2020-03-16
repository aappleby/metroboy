#include "GLBase.h"
#include <stdio.h>
#include <vector>

/*
extern "C" {
_declspec(dllexport) int NvOptimusEnablement = 0x00000001;
}
*/

//-----------------------------------------------------------------------------

  /*
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
  glDebugMessageCallback(debugOutput, nullptr);
  */


void debugOutput(GLenum /*source*/, GLenum /*type*/, GLuint /*id*/, GLenum /*severity*/,
                 GLsizei /*length*/, const GLchar* message, const GLvoid* /*userParam*/) {
	printf("GLDEBUG: %s\n", message);
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

int create_ibo(int size_bytes, const uint16_t* data) {
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

int create_texture_u32(int width, int height, const uint32_t* data) {
  int tex = 0;
  glGenTextures(1, (GLuint*)&tex);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  bool filter = false;

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter ? GL_LINEAR : GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter ? GL_LINEAR : GL_NEAREST);

  return tex;
}

//-----------------------------------------------------------------------------

void update_texture_u32(int tex, int width, int height, const uint32_t* pix) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexSubImage2D(GL_TEXTURE_2D, 0,
                  0, 0, width, height,
                  GL_RGBA, GL_UNSIGNED_BYTE, pix);
}

//-----------------------------------------------------------------------------

int create_texture_u8(int width, int height, const uint8_t* data) {
  int tex = 0;
  glGenTextures(1, (GLuint*)&tex);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

  bool filter = false;

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter ? GL_LINEAR : GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter ? GL_LINEAR : GL_NEAREST);

  return tex;
}

//-----------------------------------------------------------------------------

void update_texture_u8(int tex, int width, int height, const uint8_t* pix) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexSubImage2D(GL_TEXTURE_2D, 0,
                  0, 0, width, height,
                  GL_RED, GL_UNSIGNED_BYTE, pix);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

//-----------------------------------------------------------------------------

void bind_texture(int prog, const char* name, int index, int tex) {
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(GL_TEXTURE_2D, tex);
  glUniform1i(glGetUniformLocation(prog, name), index);
}

//-----------------------------------------------------------------------------

int create_shader(const char* name, const char* src) {
  static bool info = true;
  static bool verbose = false;

  printf("Compiling %s\n", name);
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
    if (info || len) printf("  Vert shader log %s\n", buf);
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
    if (info || len) printf("  Frag shader log %s\n", buf);
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
    if (info || len) printf("  Shader program log %s\n", buf);

  }

  glDetachShader(shaderProgram, vertexShader);
  glDetachShader(shaderProgram, fragmentShader);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  if (info) {
    int program = shaderProgram;
    int count = 0;


    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
    printf("  Active Attributes: %d\n", count);

    if (verbose) {
      for (int i = 0; i < count; i++) {
        const int bufSize = 16;
        GLenum type;
        GLchar var_name[bufSize];
        GLsizei length;
        GLint size;
        glGetActiveAttrib(program, (GLuint)i, bufSize, &length, &size, &type, var_name);
        printf("    Attribute #%d Type: %u Name: %s\n", i, type, var_name);
      }
    }

    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
    printf("  Active Uniforms: %d\n", count);

    if (verbose) {
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
    }

    glGetProgramiv(program, GL_ATTACHED_SHADERS, &count);
    printf("  Attached shaders: %d\n", count);

    glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &count);
    printf("  Uniform blocks: %d\n", count);

    if (verbose) {
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
  }

  printf("Compiling %s done\n", name);
  printf("\n");

  return shaderProgram;
}


void bind_shader(int shader) {
  glUseProgram(shader);
}

//-----------------------------------------------------------------------------
