
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char byte;
typedef const char *text;

//apt-get install libglew-dev libglfw3-dev libglm-dev
//Главное в будущем не забыть перенести куда-нибудь в Readme это перед отправкой на проверку
//LFLAGS = ... -lm -lX11 -lpthread -lXrandr -ldl Возможно в будущем что-то из этого понадобиться...

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    printf("key: %3d   scancode: %2d   action: %d   mode: %2d\n", key, scancode, action, mode);
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GL_TRUE);
        if (key == GLFW_KEY_1) glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        if (key == GLFW_KEY_2) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if (key == GLFW_KEY_3) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

text vertex_shader_source = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 position;
    out vec3 pos;
    void main() {
        pos = position;
        gl_Position = vec4(position, 1);
    }
)glsl";

text fragment_shader_source = R"glsl(
    #version 330 core
    in vec3 pos;
    out vec4 outColor;
    void main() {
	outColor = vec4(pos.x + 0.5, pos.y + 0.5, 0.5, 1);
    }
)glsl";

byte compile_shader(GLuint shader) {
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar info_log[512];
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        printf("vertexShader ошибка компиляции: %s\n", info_log);
        return 1;
    }
    return 0;
}

byte check_program(GLuint program) {
    glLinkProgram(program);
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar info_log[512];
        glGetProgramInfoLog(program, 512, NULL, info_log);
        printf("Ошибка слияния шейдеров: %s\n", info_log);
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (!glfwInit()) {
        printf("Инициализация GLFW провалена\n");
        getchar();
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Cellular Automat by VectorASD & Mapyax", NULL, NULL);
    if (window == NULL) {
        printf("Создание GLFW окна провалено\n");
        getchar();
        glfwTerminate();
        return 2;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Инициализация GLEW провалена\n");
        getchar();
        glfwTerminate();
        return 3;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    if (compile_shader(vertex_shader)) return 4;

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    if (compile_shader(fragment_shader)) return 5;

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    if (check_program(shader_program)) return 6;

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    GLfloat vertices[] = {
        0.5, 0.5, 0,
        0.5, -0.5, 0,
        -0.5, -0.5, 0,
        -0.5, 0.5, 0};
    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3};

    GLuint VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    
    glPointSize(5);
    glLineWidth(3);

    glClearColor(0, 0.5, 1, 0);
    do {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    glDeleteProgram(shader_program);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();

    return 0;
}
