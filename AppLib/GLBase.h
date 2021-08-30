#pragma once

#include "CoreLib/Types.h"

void* init_gl(void* window);
void  check_gl_error();

int   create_shader(const char* name, const char* src);
void  bind_shader(int shader);

int   create_vao();
void  bind_vao(int vao);

int   create_vbo();
int   create_vbo(int size_bytes, const void* data);
void  update_vbo(int vbo, int size_bytes, const void* data);

int   create_ibo();
void  update_ibo(int vbo, int size_bytes, const void* data);

int   create_ubo();
void  update_ubo(int ubo, int size_bytes, const void* data);
void  bind_ubo  (int prog, const char* name, int index, int ubo);

int   create_texture_u32(int width, int height, const void* data, bool filter);
void  update_texture_u32(int tex, int width, int height, const void* pix);

int   create_texture_u8(int width, int height, const void* data, bool filter);
void  update_texture_u8(int tex, int width, int height, const void* pix);
void  update_texture_u8(int tex, int dx, int dy, int dw, int dh, const void* pix);

// basically an unsigned 8-bit integer texture
int   create_table_u8(int width, int height, const void* data);
void  update_table_u8(int tex, int width, int height, const void* pix);

// basically an unsigned 32-bit integer texture
int   create_table_u32(int width, int height, const void* data);
void  update_table_u32(int tex, int width, int height, const void* pix);

void  bind_texture(int prog, const char* name, int index, int tex);
void  bind_table  (int prog, const char* name, int index, int tex);