#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <context.h>
#include <matrix.h>
#include <stdio.h>
#include <stdlib.h>

//apt-get install libglew-dev libglfw3-dev
//Главное в будущем не забыть перенести куда-нибудь в Readme это перед отправкой на проверку
//LFLAGS = ... -lX11 -lpthread -lXrandr -ldl Возможно в будущем что-то из этого понадобиться...

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
    void main() {
        int edge_x = pos.x < -0.98 || pos.x > 0.98 ? 1 : 0;
        int edge_y = pos.y < -0.98 || pos.y > 0.98 ? 1 : 0;
        int edge_z = pos.z < -0.98 || pos.z > 0.98 ? 1 : 0;
        bool edge = edge_x + edge_y + edge_z >= 2;
        if (edge) color = vec4(0, 0, 0, 1);
	else color = vec4(our_color, 1);
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
    if (!glfwInit()) {
        printf("Инициализация GLFW провалена\n");
        getchar();
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Cellular Automat by VectorASD & Mapyax", NULL, NULL);
    if (window == NULL) {
        printf("Создание GLFW окна провалено\n");
        getchar();
        glfwTerminate();
        return 2;
    }

    struct Context ctx;
    load_context(&ctx);
    glfwSetWindowUserPointer(window, &ctx);

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Инициализация GLEW провалена\n");
        getchar();
        glfwDestroyWindow(window);
        glfwTerminate();
        return 3;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    if (glfwRawMouseMotionSupported()) glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    ctx.window_size = vector2_new(width, height);

    GLuint shader_program = build_program(vertex_shader_source, fragment_shader_source);

    GLfloat vertices[] = {
        -1, -1, -1, 1, 0, 0,
        1, -1, -1, 0, 1, 0,
        -1, -1, 1, 0, 0, 1,
        1, -1, 1, 1, 1, 0,
        -1, 1, -1, 0, 1, 1,
        1, 1, -1, 1, 0, 1,
        -1, 1, 1, 1, 1, 1,
        1, 1, 1, 0, 0, 0};
    GLuint indices[] = {
        0, 1, 2, 1, 2, 3,
        0, 1, 4, 1, 4, 5,
        0, 2, 4, 2, 4, 6,
        1, 3, 5, 3, 5, 7,
        2, 3, 6, 3, 6, 7,
        4, 5, 6, 5, 6, 7};

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
    glEnable(GL_DEPTH_TEST);

    ctx.projection_loc = glGetUniformLocation(shader_program, "projection");
    ctx.view_loc = glGetUniformLocation(shader_program, "view");
    ctx.model_loc = glGetUniformLocation(shader_program, "model");

    glUseProgram(shader_program);
    upd_projection_mat(&ctx);
    upd_view_mat(&ctx);

    vec3 angles = vector3_norm(vector3_new(1, 0.2, 0));

    int pred_sec;
    int frames = 0;

    glClearColor(0, 0.5, 1, 0);
    do {
        glfwPollEvents();
        do_movement(&ctx);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float time = glfwGetTime();
        ctx.delta_time = time - ctx.last_frame_time;
        ctx.last_frame_time = time;
        if ((int) time != pred_sec) {
            pred_sec = time;
            printf("time: %2u   fps: %u\n", pred_sec, frames);
            frames = 0;
        }
        glUseProgram(shader_program);

        float angle = radians(time * 50);
        mat4 model = rotate(matrix4_new(0.5), angle, angles);
        matrix4_push(model, ctx.model_loc);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        frames++;
    } while (glfwWindowShouldClose(window) == 0);

    glDeleteProgram(shader_program);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
