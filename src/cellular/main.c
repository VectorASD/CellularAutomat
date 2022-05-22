#define GLEW_STATIC
#include <GL/glew.h>
#include <context.h>
#include <scenes.h>
#include <shaders.h>
#include <stdio.h>
#include <stdlib.h>

//apt-get install libglew-dev libglfw3-dev
//Главное в будущем не забыть перенести куда-нибудь в Readme это перед отправкой на проверку
//LFLAGS = ... -lX11 -lpthread -lXrandr -ldl Возможно в будущем что-то из этого понадобиться...

GLFWwindow *glfw_glew_init(struct Context *ctx) {
    if (!glfwInit()) {
        printf("Инициализация GLFW провалена\n");
        getchar();
        exit(1);
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
        exit(2);
    }

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
        exit(3);
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    if (glfwRawMouseMotionSupported()) glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwSetWindowUserPointer(window, ctx);
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    ctx->window_size = vector2_new(width, height);

    glPointSize(5);
    glLineWidth(3);
    glEnable(GL_DEPTH_TEST);

    return window;
}

int main(int argc, char *argv[]) {
    struct Context ctx;
    load_context(&ctx);
    GLFWwindow *window = glfw_glew_init(&ctx);

    init_models(&ctx);

    struct Model *modell = get_model_by_id(&ctx, 1);
    GLuint VAO = modell->VAO;

    GLuint shader_program = build_main_program();
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
        glDrawElements(GL_TRIANGLES, modell->indices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        frames++;
    } while (glfwWindowShouldClose(window) == 0);

    glDeleteProgram(shader_program);
    free_models(&ctx);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
