#include <shaders.h>
#include <stdio.h>
#include <stdlib.h>

void compile_shader(GLuint shader) {
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar info_log[512];
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        printf("vertexShader ошибка компиляции: %s\n", info_log);
        exit(1);
    }
}

GLuint build_program(text vertex_shader_source, text fragment_shader_source) {
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    compile_shader(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    compile_shader(fragment_shader);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    glLinkProgram(shader_program);
    GLint success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar info_log[512];
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        printf("Ошибка слияния шейдеров: %s\n", info_log);
        exit(2);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}

GLuint build_main_program() {
    text vertex_shader_source = R"glsl(
        #version 330 core
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 color;
        out vec3 our_color;
        out vec3 pos;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        void main() {
            gl_Position = projection * view * model * vec4(position, 1);
            our_color = color;
            pos = position;
        }
    )glsl";
    text fragment_shader_source = R"glsl(
        #version 330 core
        in vec3 our_color;
        in vec3 pos;
        out vec4 color;
        uniform vec4 main_color;
        uniform vec4 edge_color;
        uniform int color_mode;
        void main() {
            if (color_mode == 1 || color_mode == 3) color = main_color;
            else color = vec4(our_color, 1);
            if (color_mode >= 2) {
                int edge_x = pos.x < -0.98 || pos.x > 0.98 ? 1 : 0;
                int edge_y = pos.y < -0.98 || pos.y > 0.98 ? 1 : 0;
                int edge_z = pos.z < -0.98 || pos.z > 0.98 ? 1 : 0;
                bool edge = edge_x + edge_y + edge_z >= 2;
                if (edge) color = edge_color;
	    }
        }
    )glsl";
    return build_program(vertex_shader_source, fragment_shader_source);
}

GLuint build_gui_program() {
    text vertex_shader_source = R"glsl(
        #version 330 core
        layout (location = 0) in vec3 position;
        void main() {
            gl_Position = vec4(position, 1);
        }
    )glsl";
    text fragment_shader_source = R"glsl(
        #version 330 core
        out vec4 color;
        uniform vec4 our_color;
        void main() {
            color = our_color;
        }
    )glsl";
    return build_program(vertex_shader_source, fragment_shader_source);
}
