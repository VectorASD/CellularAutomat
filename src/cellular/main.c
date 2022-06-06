#include <scene_0.h>
#include <scenes.h>
#include <shaders.h>
#include <stdio.h>
#include <stdlib.h>

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

    GLFWwindow *window = ctx->window = glfwCreateWindow(800, 600, "Cellular Automat by VectorASD & Mapyax", NULL, NULL);
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
    glCullFace(GL_FRONT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return window;
}

int main(int argc, char *argv[]) {
    struct Context ctx;
    load_context(&ctx);
    GLFWwindow *window = glfw_glew_init(&ctx);

    init_models(&ctx);
    init_primitives(&ctx);

    uint scene_0 = bind_scene(&ctx, "Самая первая", init_scene_0, render_scene_0, gui_scene_0, NULL);

    select_scene(&ctx, scene_0);

    GLuint shader_program = build_main_program();
    ctx.projection_loc = glGetUniformLocation(shader_program, "projection");
    ctx.view_loc = glGetUniformLocation(shader_program, "view");
    ctx.model_loc = glGetUniformLocation(shader_program, "model");
    ctx.main_color_loc = glGetUniformLocation(shader_program, "main_color");
    ctx.edge_color_loc = glGetUniformLocation(shader_program, "edge_color");
    ctx.color_mode_loc = glGetUniformLocation(shader_program, "color_mode");
    ctx.cursor_pos_loc = glGetUniformLocation(shader_program, "cursor_pos");
    ctx.part_id_loc = glGetUniformLocation(shader_program, "part_id");
    ctx.shader_program = shader_program;
    glUseProgram(shader_program);
    GLint empty_cursor[2] = {-1, -1};
    glUniform1iv(ctx.cursor_pos_loc, 2, empty_cursor);

    GLint buffer[6] = {0};
    glGenBuffers(1, &ctx.ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ctx.ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(buffer), buffer, GL_STREAM_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ctx.ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    upd_projection_mat(&ctx);
    upd_view_mat(&ctx);
    ctx.gui_program = build_gui_program();
    GLuint font_program = build_font_program();
    ctx.font.font_program = font_program;
    ctx.font.color_loc = glGetUniformLocation(font_program, "char_color");

    int pred_sec;
    int frames = 0;

    glClearColor(1, 0.8, 0.4, 0);
    do {
        context_tick(&ctx);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float time = ctx.time = glfwGetTime();
        ctx.time_delta = time - ctx.last_frame_time;
        ctx.last_frame_time = time;
        if ((int) time != pred_sec) {
            pred_sec = time;
            sprintf(ctx.fps_view[ctx.fps_view_n], "время: %2u   fps: %u\n", pred_sec, frames);
            ctx.fps_view_n = (ctx.fps_view_n + 1) % 6;
            frames = 0;
        }

        render_scene(&ctx);

        glfwSwapBuffers(window);
        frames++;
    } while (glfwWindowShouldClose(window) == 0);

    glDeleteProgram(shader_program);
    glDeleteProgram(ctx.gui_program);
    glDeleteProgram(ctx.font.font_program);
    free_scenes(&ctx);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
