#include <math.h> // sin, cos
#include <scenes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strcmp

//
// Models
//

struct Model *create_model(text name, GLuint vertexes_n, GLuint indices_n, GLfloat vertexes[], GLuint indices[]) {
    struct Model *model = malloc(sizeof(struct Model));
    model->next = NULL;
    model->name = name;
    model->indices = indices_n;
    glGenBuffers(1, &model->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
    glGenVertexArrays(1, &model->VAO);
    glGenBuffers(1, &model->EBO);

    glBindVertexArray(model->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexes_n, vertexes, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices_n, indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    return model;
}

void free_model(struct Model *model) {
    glDeleteBuffers(1, &model->VBO);
    glDeleteVertexArrays(1, &model->VAO);
    glDeleteBuffers(1, &model->EBO);
    free(model);
}

void add_model(struct Context *ctx, struct Model *model) {
    if (ctx->last_model == NULL)
        ctx->models = ctx->last_model = model;
    else
        ctx->last_model = ctx->last_model->next = model;
    ctx->models_n++;
}

void free_models(struct Context *ctx) {
    struct Model *p = ctx->models, *next;
    while (p) {
        next = p->next;
        free_model(p);
        p = next;
    }
}

void print_model_list(struct Context *ctx) {
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("Доступные на данный момент модели:\n");
    struct Model *p = ctx->models;
    uint n = 0;
    while (p) {
        printf("  %2u: %s\n", n, p->name);
        n++;
        p = p->next;
    }
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

struct Model *get_model_by_id(struct Context *ctx, uint id) {
    struct Model *p = ctx->models;
    uint n = 0;
    while (p) {
        if (id == n) return p;
        n++;
        p = p->next;
    }
    printf("Допустимое id модели от 0 до %u. Вы же запросили id: %u\n", n - 1, id);
    print_model_list(ctx);
    exit(4);
}

struct Model *get_model_by_name(struct Context *ctx, text name) {
    struct Model *p = ctx->models;
    while (p) {
        if (strcmp(p->name, name) == 0) return p;
        p = p->next;
    }
    printf("Нет такой модели под именем: %s\n", name);
    print_model_list(ctx);
    exit(5);
}

vec3 random_color() {
    const float colors[][3] = {{1, 0, 0}, {0, 0.7, 0}, {0, 0, 1}, {1, 1, 0}, {0, 1, 0.5}, {1, 0.5, 0}, {0, 0.5, 1}, {1, 0, 1}, {1, 1, 1}};
    const float *color = colors[randint(0, 8)];
    return vector3_new(color[0], color[1], color[2]);
}

void vertex_add(GLfloat vertexes[], int v_pos, float x, float y, float z) {
    vertexes[v_pos] = x;
    vertexes[v_pos + 1] = y;
    vertexes[v_pos + 2] = z;
    vec3 color = random_color();
    vertexes[v_pos + 3] = color.x;
    vertexes[v_pos + 4] = color.y;
    vertexes[v_pos + 5] = color.z;
}

void init_models(struct Context *ctx) {
    GLfloat cube_vertexes[] = {
        -1, -1, -1, 1, 0, 0,
        1, -1, -1, 0, 1, 0,
        -1, -1, 1, 0, 0, 1,
        1, -1, 1, 1, 1, 0,
        -1, 1, -1, 0, 0.5, 1,
        1, 1, -1, 1, 0, 1,
        -1, 1, 1, 0, 1, 0.5,
        1, 1, 1, 1, 0.5, 0};
    GLuint cube_indices[] = {
        0, 1, 2, 1, 2, 3,
        0, 1, 4, 1, 4, 5,
        0, 2, 4, 2, 4, 6,
        1, 3, 5, 3, 5, 7,
        2, 3, 6, 3, 6, 7,
        4, 5, 6, 5, 6, 7};
    add_model(ctx, create_model("куб", 6 * 8, 3 * 12, cube_vertexes, cube_indices));

    int quality = 20;
    int quality2 = quality * 2;
    int vertexes_n = quality2 * (quality - 1) + 2;
    GLfloat vertexes[6 * vertexes_n];
    int indices_n = quality * (quality2 - 2) * 2;
    GLuint indices[3 * indices_n];
    vertex_add(vertexes, 0, 0, 1, 0);
    int v_pos = 6, i_pos = 0;
    for (int pitch = 1; pitch < quality; pitch++) {
        float pitch_rad = radians(pitch * 180. / quality);
        float sin_pitch = sin(pitch_rad);
        float cos_pitch = cos(pitch_rad);
        for (int yaw_deg = 0; yaw_deg < quality2; yaw_deg++) {
            float yaw = radians(yaw_deg * 360. / quality2);
            vertex_add(vertexes, v_pos, cos(yaw) * sin_pitch, cos_pitch, sin(yaw) * sin_pitch);
            v_pos += 6;
            int ver_a = yaw_deg + 1;
            int ver_b = ver_a % quality2 + 1;
            int ver_c = (pitch - 2) * quality2;
            int ver_d = (pitch - 1) * quality2;
            if (pitch == 1) {
                indices[i_pos++] = 0;
                indices[i_pos++] = ver_a;
                indices[i_pos++] = ver_b;
            } else {
                indices[i_pos++] = ver_c + ver_a;
                indices[i_pos++] = ver_c + ver_b;
                indices[i_pos++] = ver_d + ver_a;
                indices[i_pos++] = ver_c + ver_b;
                indices[i_pos++] = ver_d + ver_a;
                indices[i_pos++] = ver_d + ver_b;
            }
        }
    }
    vertex_add(vertexes, v_pos, 0, -1, 0);
    for (int yaw_deg = 1; yaw_deg <= quality2; yaw_deg++) {
        int ver_c = (quality - 2) * quality2;
        indices[i_pos++] = ver_c + yaw_deg;
        indices[i_pos++] = ver_c + yaw_deg % quality2 + 1;
        indices[i_pos++] = vertexes_n - 1;
    }
    add_model(ctx, create_model("сфера", 6 * vertexes_n, 3 * indices_n, vertexes, indices));
}

//
// Parts
//

struct Part *create_part(struct Context *ctx, struct Model *model) {
    struct Part *part = malloc(sizeof(struct Part));
    part->next = NULL;
    part->used_model = model;
    part->pos = part->orientation = vector3_new(0, 0, 0);
    part->size = vector3_new(1, 1, 1);
    struct Scene *scene = ctx->current_scene;
    if (scene->last_part == NULL)
        scene->parts = scene->last_part = part;
    else
        scene->last_part = scene->last_part->next = part;
    update_part(part);
    return part;
}

void update_part(struct Part *part) {
    mat4 model = translate(matrix4_new(1), part->pos);
    model = rotate(model, part->orientation.y, vector3_new(0, 1, 0));
    model = rotate(model, part->orientation.x, vector3_new(1, 0, 0));
    model = scale(model, vector3_div_s(part->size, 2));
    part->model_mat = rotate(model, part->orientation.z, vector3_new(0, 0, 1));
}

void render_part(struct Context *ctx, struct Part *part) {
    struct Model *model = part->used_model;
    matrix4_push(part->model_mat, ctx->model_loc);
    glBindVertexArray(model->VAO);
    glDrawElements(GL_TRIANGLES, model->indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void free_parts(struct Scene *scene) {
    struct Part *p = scene->parts, *next;
    while (p) {
        next = p->next;
        free(p);
        p = next;
    }
}

//
// Scenes
//

uint create_scene(struct Context *ctx) {
    struct Scene *scene = malloc(sizeof(struct Scene));
    scene->next = NULL;
    if (ctx->last_scene == NULL)
        ctx->scenes = ctx->last_scene = scene;
    else
        ctx->last_scene = ctx->last_scene->next = scene;
    return ctx->scenes_n++;
}

void select_scene(struct Context *ctx, uint id) {
    struct Scene *p = ctx->scenes;
    uint n = 0;
    while (p) {
        if (id == n) {
            ctx->current_scene = p;
            return;
        }
        n++;
        p = p->next;
    }
    printf("Допустимое id сцены от 0 до %u. Вы же выбрал сцену под id: %u\n", n - 1, id);
    exit(5);
}

void render_scene(struct Context *ctx) {
    struct Scene *scene = ctx->current_scene;
    struct Part *p = scene->parts;
    while (p) {
        render_part(ctx, p);
        p = p->next;
    }
}

void free_scenes(struct Context *ctx) {
    free_models(ctx);
    struct Scene *p = ctx->scenes, *next;
    while (p) {
        next = p->next;
        free_parts(p);
        free(p);
        p = next;
    }
}