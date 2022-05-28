#pragma once

#include <GL/glew.h>
#include <context.h>

void compile_shader(GLuint shader);
GLuint build_program(text vertex_shader_source, text fragment_shader_source);
GLuint build_main_program();
GLuint build_gui_program();
GLuint build_font_program();
