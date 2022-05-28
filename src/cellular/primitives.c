#include <primitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memcpy

//
// Линии, треугольники и прямоугольники
//

void init_primitives(struct Context *ctx) {
    struct Primitives *prim = &ctx->prim;

    glGenBuffers(1, &prim->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, prim->VBO);
    glGenVertexArrays(1, &prim->VAO);

    glBindVertexArray(prim->VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    prim->lines.first = NULL;
    prim->lines.last = NULL;
    prim->lines.n = 0;
    prim->triangles.first = NULL;
    prim->triangles.last = NULL;
    prim->triangles.n = 0;
    prim->line_color = vector4_new(1, 1, 0, 1);
    prim->line_color2 = vector4_new(0, 0, 1, 0.5);
    prim->tri_color = vector4_new(0, 1, 0, 1);
    prim->tri_color2 = vector4_new(0, 0, 1, 0.5);
    prim->tri_color3 = vector4_new(1, 1, 0, 1);
    prim->tri_color4 = vector4_new(1, 0.5, 0, 1);

    init_fonts(ctx);
}

void add_vertex_node(GLfloat aspect, struct VertexList *list, GLfloat x, GLfloat y, GLfloat z, vec4 *color) {
    x = x * aspect / 600 * 2 - 1;
    y = 1 - y / 600 * 2;
    struct VertexNode orig = {NULL, x, y, z, color->x, color->y, color->z, color->w};
    if (list->first == NULL) {
        struct VertexNode *vertex = malloc(sizeof(struct VertexNode));
        memcpy(vertex, &orig, sizeof(struct VertexNode));
        list->first = list->last = vertex;
    } else if (list->last->next == NULL) {
        struct VertexNode *vertex = malloc(sizeof(struct VertexNode));
        memcpy(vertex, &orig, sizeof(struct VertexNode));
        list->last = list->last->next = vertex;
    } else {
        if (list->n) list->last = list->last->next;
        memcpy(&list->last->x, &orig.x, 7 * sizeof(GLfloat));
    }
    list->n++;
}

void nodes_to_array(struct VertexList *list, GLfloat *arr) {
    struct VertexNode *p = list->first;
    int pos = 0;
    for (int i = 0; i < list->n; i++) {
        memcpy(arr + pos, &p->x, 7 * sizeof(GLfloat));
        pos += 7;
        p = p->next;
    }
    list->last = list->first;
    list->n = 0;
}

void draw_line(struct Context *ctx, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
    struct Primitives *prim = &ctx->prim;
    struct VertexList *lines = &prim->lines;
    GLfloat aspect = ctx->window_size.y / ctx->window_size.x;
    add_vertex_node(aspect, lines, x1, y1, -0.995, &prim->line_color);
    add_vertex_node(aspect, lines, x2, y2, -0.995, &prim->line_color2);
}

void draw_triangle(struct Context *ctx, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3) {
    struct Primitives *prim = &ctx->prim;
    struct VertexList *triangles = &prim->triangles;
    GLfloat aspect = ctx->window_size.y / ctx->window_size.x;
    add_vertex_node(aspect, triangles, x1, y1, -0.99, &prim->tri_color);
    add_vertex_node(aspect, triangles, x2, y2, -0.99, &prim->tri_color2);
    add_vertex_node(aspect, triangles, x3, y3, -0.99, &prim->tri_color3);
}

void draw_box(struct Context *ctx, GLfloat x, GLfloat y, GLfloat width, GLfloat height) {
    GLfloat x2 = x + width, y2 = y + height;
    struct Primitives *prim = &ctx->prim;
    struct VertexList *triangles = &prim->triangles;
    GLfloat aspect = ctx->window_size.y / ctx->window_size.x;
    add_vertex_node(aspect, triangles, x, y, -0.99, &prim->tri_color);
    add_vertex_node(aspect, triangles, x2, y, -0.99, &prim->tri_color2);
    add_vertex_node(aspect, triangles, x2, y2, -0.99, &prim->tri_color4);
    add_vertex_node(aspect, triangles, x, y, -0.99, &prim->tri_color);
    add_vertex_node(aspect, triangles, x, y2, -0.99, &prim->tri_color3);
    add_vertex_node(aspect, triangles, x2, y2, -0.99, &prim->tri_color4);
}

void set_line_color(struct Context *ctx, float R, float G, float B, float A) {
    struct Primitives *prim = &ctx->prim;
    prim->line_color = prim->line_color2 = vector4_new(R / 255, G / 255, B / 255, A / 255);
}

void set_box_color(struct Context *ctx, float R, float G, float B, float A) {
    struct Primitives *prim = &ctx->prim;
    prim->tri_color = prim->tri_color2 = prim->tri_color3 = prim->tri_color4 = vector4_new(R / 255, G / 255, B / 255, A / 255);
}

void render_primitives(struct Context *ctx) {
    glUseProgram(ctx->gui_program);
    struct Primitives *prim = &ctx->prim;
    int lines_n = prim->lines.n;
    int triangles_n = prim->triangles.n;
    GLfloat vertices[7 * (lines_n > triangles_n ? lines_n : triangles_n)];

    glBindVertexArray(prim->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, prim->VBO);

    nodes_to_array(&prim->lines, vertices);
    glBufferData(GL_ARRAY_BUFFER, 7 * sizeof(GLfloat) * lines_n, vertices, GL_STREAM_DRAW);
    glDrawArrays(GL_LINES, 0, lines_n);

    nodes_to_array(&prim->triangles, vertices);
    glBufferData(GL_ARRAY_BUFFER, 7 * sizeof(GLfloat) * triangles_n, vertices, GL_STREAM_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, triangles_n);

    glBindVertexArray(0);
}

void free_primitives(struct Context *ctx) {
    struct Primitives *prim = &ctx->prim;
    glDeleteBuffers(1, &prim->VBO);
    glDeleteVertexArrays(1, &prim->VAO);
    struct VertexNode *p = prim->lines.first, *next;
    while (p) {
        next = p->next;
        free(p);
        p = next;
    }
    p = prim->triangles.first;
    while (p) {
        next = p->next;
        free(p);
        p = next;
    }
    free_fonts(ctx);
}

//
// Шрифты
//

void init_fonts(struct Context *ctx) {
    struct Font *font = &ctx->font;
    if (FT_Init_FreeType(&font->ft)) {
        printf("Не удаётся инициализировать FreeType библиотеку\n");
        exit(7);
    }
    if (FT_New_Face(font->ft, "fonts/Airfool.ttf", 0, &font->face)) {
        printf("Не удаётся загрузить шрифт\n");
        exit(8);
    }
    font->map = NULL;
    FT_Face face = font->face;
    printf("Информацию о шрифте:\n");
    printf("  Число глифов: %lu\n", face->num_glyphs);
    printf("  Название: %s\n", face->family_name);
    printf("  Стиль: %s\n", face->style_name);
    set_font_height(ctx, 96);

    glGenVertexArrays(1, &font->VAO);
    glGenBuffers(1, &font->VBO);
    glBindVertexArray(font->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, font->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void set_font_height(struct Context *ctx, uint height) {
    struct Font *font = &ctx->font;
    font->current_height = height;
    FT_Set_Pixel_Sizes(font->face, 0, height);
}

struct CharNode *load_glyph(struct Context *ctx, uint code) {
    struct Font *font = &ctx->font;
    FT_Face face = font->face;
    if (FT_Load_Char(face, code, FT_LOAD_RENDER)) {
        printf("Не удаётся загрузить глиф 0x%x\n", code);
        if (code != '?') return load_glyph(ctx, '?');
        exit(9);
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    printf("Удачная загрузка: %u\n", code);
    struct CharNode character = {
        NULL, NULL, {texture, vector2_new(face->glyph->bitmap.width, face->glyph->bitmap.rows), vector2_new(face->glyph->bitmap_left, face->glyph->bitmap_top), face->glyph->advance.x}, code, font->current_height};
    struct CharNode *res = malloc(sizeof(struct CharNode));
    memcpy(res, &character, sizeof(struct CharNode));
    return res;
}

struct Character *get_glyph(struct Context *ctx, uint code) {
    struct Font *font = &ctx->font;
    if (font->map == NULL) return &(font->map = load_glyph(ctx, code))->chr;
    struct CharNode *p = font->map;
    uint height = font->current_height;
    while (1) {
        byte right = 0;
        if (p->height == height) {
            if (p->code == code) return &p->chr;
            if (p->code > code) right = 1;
        } else if (p->height > height)
            right = 1;
        if (right) {
            if (p->right == NULL) return &(p->right = load_glyph(ctx, code))->chr;
            p = p->right;
        } else {
            if (p->left == NULL) return &(p->left = load_glyph(ctx, code))->chr;
            p = p->left;
        }
    }
}

void set_text_color(struct Context *ctx, float R, float G, float B, float A) {
    struct Font *font = &ctx->font;
    glUseProgram(font->font_program);
    vec4 color = font->text_color = vector4_new(R / 255, G / 255, B / 255, A / 255);
    glUniform4fv(font->color_loc, 1, (GLfloat *) &color);
}

void render_text(struct Context *ctx, text str, GLfloat x, GLfloat y, GLfloat scale) {
    struct Font *font = &ctx->font;
    glUseProgram(font->font_program);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(font->VAO);

    GLfloat aspect = ctx->window_size.y / ctx->window_size.x;
    int let = 0;
    while (str[let]) {
        struct Character *glyph = get_glyph(ctx, str[let++] & 255);

        GLfloat xpos = x + glyph->bearing.x * scale;
        GLfloat ypos = y - (glyph->size.y - glyph->bearing.y) * scale;

        GLfloat w = glyph->size.x * scale;
        GLfloat h = glyph->size.y * scale;

        xpos = xpos * aspect / 600 * 2 - 1;
        ypos = 1 - ypos / 600 * 2;
        w = w * aspect / 600 * 2;
        h = h / 600 * 2;

        GLfloat vertices[6][4] = {
            {xpos, ypos + h, 0, 0},
            {xpos, ypos, 0, 1},
            {xpos + w, ypos, 1, 1},
            {xpos, ypos + h, 0, 0},
            {xpos + w, ypos, 1, 1},
            {xpos + w, ypos + h, 1, 0}};
        glBindTexture(GL_TEXTURE_2D, glyph->texture_id);

        glBindBuffer(GL_ARRAY_BUFFER, font->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (glyph->advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void free_map(struct CharNode *node, int level) {
    if (node == NULL) return;
    free_map(node->left, level + 1);
    free_map(node->right, level + 1);
    glDeleteTextures(1, &node->chr.texture_id);
    free(node);
}

void free_fonts(struct Context *ctx) {
    struct Font *font = &ctx->font;
    FT_Done_Face(font->face);
    FT_Done_FreeType(font->ft);
    free_map(font->map, 0);
    glDeleteBuffers(1, &font->VBO);
    glDeleteVertexArrays(1, &font->VAO);
}
