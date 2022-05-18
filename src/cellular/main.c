
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <matrix.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char byte;
typedef const char *text;

//apt-get install libglew-dev libglfw3-dev libglm-dev
//Главное в будущем не забыть перенести куда-нибудь в Readme это перед отправкой на проверку
//LFLAGS = ... -lX11 -lpthread -lXrandr -ldl Возможно в будущем что-то из этого понадобиться...

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
    layout (location = 1) in vec3 color;
    out vec3 our_color;
    uniform mat4 transform;
    void main() {
        gl_Position = transform * vec4(position, 1);
        our_color = color;
    }
)glsl";

text fragment_shader_source = R"glsl(
    #version 330 core
    in vec3 our_color;
    out vec4 color;
    void main() {
	color = vec4(our_color, 1);
    }
)glsl";

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

int main(int argc, char *argv[]) {
    printf("Максимальное число входных переменных шейдера этой GPU: %u\n", GL_MAX_VERTEX_ATTRIBS);

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

    GLuint shader_program = build_program(vertex_shader_source, fragment_shader_source);

    GLfloat vertices[] = {
        -0.5, 0.5, 0, 1, 0, 0,
        0.5, 0.5, 0, 0, 1, 0,
        -0.5, -0.5, 0, 1, 1, 1,
        0, -0.5, 0, 0, 0, 1,
        0.5, -0.5, 0, 1, 1, 0};
    GLuint indices[] = {
        0, 1, 3,
        0, 2, 3,
        1, 3, 4};

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    glPointSize(5);
    glLineWidth(3);

    GLint transform_location = glGetUniformLocation(shader_program, "transform");
    mat4 trans = matrix4_new(1.2);
    vec3 angles = vector3_norm(vector3_new(0, 0.2, 1));
    
    glClearColor(0, 0.5, 1, 0);
    do {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        float angle = radians(glfwGetTime() * 50);
        mat4 inter_mat = rotate(trans, angle, angles);
        matrix4_push(inter_mat, transform_location);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
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
