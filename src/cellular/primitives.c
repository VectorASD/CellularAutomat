#include <math.h> // fabs, sqrt
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
    prim->lines.n = prim->lines.dropped_n = 0;
    prim->triangles.first = NULL;
    prim->triangles.last = NULL;
    prim->triangles.n = prim->triangles.dropped_n = 0;
    prim->line_color = vector4_new(1, 1, 0, 1);
    prim->line_color2 = vector4_new(0, 0, 1, 0.5);
    prim->tri_color = vector4_new(0, 1, 0, 1);
    prim->tri_color2 = vector4_new(0, 0, 1, 1);
    prim->tri_color3 = vector4_new(1, 1, 0, 1);
    prim->tri_color4 = vector4_new(1, 0.5, 0, 1);
    prim->all_prims_n = 0;
    prim->btn_id = -1;
    prim->buttons = 0;
    prim->btn_callback = NULL;

    init_fonts(ctx);
}

void add_vertex_node(struct Context *ctx, struct VertexList *list, GLfloat x, GLfloat y, GLfloat z, vec4 *color) {
    struct Primitives *prim = &ctx->prim;
    GLfloat aspect = ctx->window_size.y / ctx->window_size.x;
    x = x * aspect / 600 * 2 - 1;
    y = 1 - y / 600 * 2;
    struct VertexNode orig = {NULL, x, y, z, color->x, color->y, color->z, color->w, prim->btn_id, prim->btn_callback};
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

void nodes_to_array(struct Context *ctx, struct VertexList *list, GLfloat *arr, int all_n, short hover_btn_id) {
    struct VertexNode *p = list->first;
    int pos = 0;
    for (int i = 0; i < list->n; i++) {
        p->z = -0.999 - 0.001 * p->z / all_n;
        if (p->btn_id >= 0) {
            byte is_hovered = p->btn_id == hover_btn_id;
            byte is_pressed = 0;
            for (byte key = 0; key < 8; key++)
                if (ctx->btn_id_press[key] == p->btn_id) is_pressed = 1;
            if (is_pressed) {
                p->r *= 1.1;
                p->g *= 1.1;
                p->b *= 1.1;
            } else if (is_hovered) {
                p->r /= 1.3;
                p->g /= 1.3;
                p->b /= 1.3;
            }
        }
        memcpy(arr + pos, &p->x, 7 * sizeof(GLfloat));
        pos += 7;
        p = p->next;
    }
    list->last = list->first;
    list->dropped_n = list->n;
    list->n = 0;
}

void draw_line(struct Context *ctx, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
    struct Primitives *prim = &ctx->prim;
    struct VertexList *lines = &prim->lines;
    GLfloat id = prim->all_prims_n++;
    add_vertex_node(ctx, lines, x1, y1, id, &prim->line_color);
    add_vertex_node(ctx, lines, x2, y2, id, &prim->line_color2);
}

void draw_triangle(struct Context *ctx, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3) {
    struct Primitives *prim = &ctx->prim;
    struct VertexList *triangles = &prim->triangles;
    GLfloat id = prim->all_prims_n++;
    add_vertex_node(ctx, triangles, x1, y1, id, &prim->tri_color);
    add_vertex_node(ctx, triangles, x2, y2, id, &prim->tri_color2);
    add_vertex_node(ctx, triangles, x3, y3, id, &prim->tri_color3);
}

void draw_box(struct Context *ctx, GLfloat x, GLfloat y, GLfloat width, GLfloat height) {
    GLfloat x2 = x + width, y2 = y + height;
    struct Primitives *prim = &ctx->prim;
    struct VertexList *triangles = &prim->triangles;
    GLfloat id = prim->all_prims_n++;
    add_vertex_node(ctx, triangles, x, y, id, &prim->tri_color);
    add_vertex_node(ctx, triangles, x2, y, id, &prim->tri_color2);
    add_vertex_node(ctx, triangles, x2, y2, id, &prim->tri_color4);
    add_vertex_node(ctx, triangles, x, y, id, &prim->tri_color);
    add_vertex_node(ctx, triangles, x, y2, id, &prim->tri_color3);
    add_vertex_node(ctx, triangles, x2, y2, id, &prim->tri_color4);
}

void draw_rect(struct Context *ctx, GLfloat x, GLfloat y, GLfloat width, GLfloat height) {
    GLfloat x2 = x + width, y2 = y + height;
    struct Primitives *prim = &ctx->prim;
    struct VertexList *lines = &prim->lines;
    GLfloat id = prim->all_prims_n++;
    add_vertex_node(ctx, lines, x - 1, y, id, &prim->line_color);
    add_vertex_node(ctx, lines, x2 + 1, y, id, &prim->line_color);
    add_vertex_node(ctx, lines, x2, y, id, &prim->line_color);
    add_vertex_node(ctx, lines, x2, y2, id, &prim->line_color2);
    add_vertex_node(ctx, lines, x, y, id, &prim->line_color);
    add_vertex_node(ctx, lines, x, y2, id, &prim->line_color2);
    add_vertex_node(ctx, lines, x - 1, y2, id, &prim->line_color2);
    add_vertex_node(ctx, lines, x2 + 1, y2, id, &prim->line_color2);
}

void draw_rect_box(struct Context *ctx, GLfloat x, GLfloat y, GLfloat width, GLfloat height) {
    draw_box(ctx, x, y, width, height);
    draw_rect(ctx, x, y, width, height);
}

void set_line_color(struct Context *ctx, float R, float G, float B, float A) {
    struct Primitives *prim = &ctx->prim;
    prim->line_color = prim->line_color2 = vector4_new(R / 255, G / 255, B / 255, A / 255);
}
void set_line_gradient_color(struct Context *ctx, float R, float G, float B, float R2, float G2, float B2, float A) {
    struct Primitives *prim = &ctx->prim;
    prim->line_color = vector4_new(R / 255, G / 255, B / 255, A / 255);
    prim->line_color2 = vector4_new(R2 / 255, G2 / 255, B2 / 255, A / 255);
}

void set_box_color(struct Context *ctx, float R, float G, float B, float A) {
    struct Primitives *prim = &ctx->prim;
    prim->tri_color = prim->tri_color2 = prim->tri_color3 = prim->tri_color4 = vector4_new(R / 255, G / 255, B / 255, A / 255);
}
void set_box_vert_gradient_color(struct Context *ctx, float R, float G, float B, float R2, float G2, float B2, float A) {
    struct Primitives *prim = &ctx->prim;
    prim->tri_color = prim->tri_color2 = vector4_new(R / 255, G / 255, B / 255, A / 255);
    prim->tri_color3 = prim->tri_color4 = vector4_new(R2 / 255, G2 / 255, B2 / 255, A / 255);
}
void set_box_horiz_gradient_color(struct Context *ctx, float R, float G, float B, float R2, float G2, float B2, float A) {
    struct Primitives *prim = &ctx->prim;
    prim->tri_color = prim->tri_color3 = vector4_new(R / 255, G / 255, B / 255, A / 255);
    prim->tri_color2 = prim->tri_color4 = vector4_new(R2 / 255, G2 / 255, B2 / 255, A / 255);
}

void render_primitives(struct Context *ctx) {
    glEnable(GL_DEPTH_TEST);
    glUseProgram(ctx->gui_program);
    struct Primitives *prim = &ctx->prim;
    int lines_n = prim->lines.n;
    int triangles_n = prim->triangles.n;
    short hover_btn_id = calculate_hovered_button(ctx, 0);

    GLfloat vertices[7 * (lines_n > triangles_n ? lines_n : triangles_n)];

    glBindVertexArray(prim->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, prim->VBO);

    nodes_to_array(ctx, &prim->triangles, vertices, prim->all_prims_n, hover_btn_id);
    glBufferData(GL_ARRAY_BUFFER, 7 * sizeof(GLfloat) * triangles_n, vertices, GL_STREAM_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, triangles_n);

    nodes_to_array(ctx, &prim->lines, vertices, prim->all_prims_n, hover_btn_id);
    glBufferData(GL_ARRAY_BUFFER, 7 * sizeof(GLfloat) * lines_n, vertices, GL_STREAM_DRAW);
    glDrawArrays(GL_LINES, 0, lines_n);

    glBindVertexArray(0);
    prim->all_prims_n = 0;
    prim->buttons = 0;
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
    if (FT_New_Face(font->ft, "fonts/Bukhari Script.ttf", 0, &font->face)) {
        printf("Не удаётся загрузить шрифт\n");
        exit(8);
    }
    font->map = NULL;
    FT_Face face = font->face;
    printf("Информацию о шрифте:\n");
    printf("  Число глифов: %lu\n", face->num_glyphs);
    printf("  Название: %s\n", face->family_name);
    printf("  Стиль: %s\n", face->style_name);
    set_font_height(ctx, 100);

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

    FT_Bitmap bitmap = face->glyph->bitmap;
    int width = bitmap.width, rows = bitmap.rows;
    byte round = 5;
    byte round_x2 = round * 2;
    byte new_bitmap[rows + round_x2][width + round_x2];
    for (int y = -round; y < rows + round; y++)
        for (int x = -round; x < width + round; x++) {
            byte alpha = x >= 0 && x < width && y >= 0 && y < rows ? bitmap.buffer[x + y * width] : 0;
            if (alpha == 255) goto label;
            for (char j = -round; j <= round; j++) {
                if (y + j < 0 || y + j >= rows) continue;
                for (char i = -round; i <= round; i++) {
                    if (x + i < 0 || x + i >= width || i * i + j * j > round * round || bitmap.buffer[x + i + (y + j) * width] == 0) continue;
                    alpha = 1;
                    goto label;
                }
            }
        label:
            new_bitmap[y + round][x + round] = alpha;
        }

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        width + round_x2,
        rows + round_x2,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        (byte *) new_bitmap);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    struct CharNode character = {
        NULL, NULL, {texture, vector2_new(face->glyph->bitmap.width, face->glyph->bitmap.rows), vector2_new(face->glyph->bitmap_left, face->glyph->bitmap_top), face->glyph->advance.x, code}, code, font->current_height};
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
    glDisable(GL_DEPTH_TEST);
    struct Font *font = &ctx->font;
    glUseProgram(font->font_program);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(font->VAO);

    GLfloat aspect = ctx->window_size.y / ctx->window_size.x;
    scale /= font->current_height;
    byte round = 5;
    x -= round;
    y -= round;

    int bytes = 0, size = 0, let = 0;
    while (str[bytes]) bytes++;
    struct Character *glyphs[bytes];
    GLfloat max_size = get_glyph(ctx, '(')->size.y;
    while (str[let]) {
        uint code = (byte) str[let++];
        if (code >> 5 == 6 && (byte) str[let] >> 6 == 2)
            code = (code & 31) << 6 | (str[let++] & 63);
        else if (code >> 4 == 14 && (byte) str[let] >> 6 == 2 && (byte) str[let + 1] >> 6 == 2) {
            code = (code & 15) << 12 | (str[let] & 63) << 6 | (str[let + 1] & 63);
            let += 2;
        } else if (code >> 3 == 30 && (byte) str[let] >> 6 == 2 && (byte) str[let + 1] >> 6 == 2 && (byte) str[let + 2] >> 6 == 2) {
            code = (code & 7) << 18 | (str[let] & 63) << 12 | (str[let + 1] & 63) << 6 | (str[let + 2] & 63);
            let += 3;
        }
        struct Character *glyph = get_glyph(ctx, code);
        glyphs[size++] = glyph;
        if (max_size < glyph->size.y) max_size = glyph->size.y;
    }
    y += max_size * scale;
    GLfloat start_x = x;
    for (let = 0; let < size; let++) {
        struct Character *glyph = glyphs[let];

        GLfloat xpos = x + glyph->bearing.x * scale;
        GLfloat ypos = y + (glyph->size.y - glyph->bearing.y) * scale;

        GLfloat w = glyph->size.x * scale;
        GLfloat h = glyph->size.y * scale;

        xpos = xpos * aspect / 600 * 2 - 1;
        ypos = 1 - ypos / 600 * 2;
        w = w * aspect / 600 * 2;
        h = h / 600 * 2;

        float pad = 0.03;
        GLfloat vertices[6][4] = {
            {xpos, ypos + h, -pad, -pad},
            {xpos, ypos, -pad, 1 + pad},
            {xpos + w, ypos, 1 + pad, 1 + pad},
            {xpos, ypos + h, -pad, -pad},
            {xpos + w, ypos, 1 + pad, 1 + pad},
            {xpos + w, ypos + h, 1 + pad, -pad}};
        glBindTexture(GL_TEXTURE_2D, glyph->texture_id);

        glBindBuffer(GL_ARRAY_BUFFER, font->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        if (glyph->code == '\n')
            x = start_x, y += max_size * scale;
        else
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

//
// Кнопки
//

void draw_btn_rect(struct Context *ctx, GLfloat x, GLfloat y, GLfloat width, GLfloat height) {
    GLfloat x2 = x + width, y2 = y + height;
    GLfloat h2 = height / 2;
    struct Primitives *prim = &ctx->prim;
    struct VertexList *lines = &prim->lines;
    GLfloat id = prim->all_prims_n++;
    add_vertex_node(ctx, lines, x - 1, y, id, &prim->line_color);
    add_vertex_node(ctx, lines, x2 + 1, y, id, &prim->line_color);
    add_vertex_node(ctx, lines, x, y, id, &prim->line_color);
    add_vertex_node(ctx, lines, x, y + h2, id, &prim->line_color);
    add_vertex_node(ctx, lines, x2, y, id, &prim->line_color);
    add_vertex_node(ctx, lines, x2, y + h2, id, &prim->line_color);

    add_vertex_node(ctx, lines, x, y + h2, id, &prim->line_color2);
    add_vertex_node(ctx, lines, x, y2, id, &prim->line_color2);
    add_vertex_node(ctx, lines, x2, y + h2, id, &prim->line_color2);
    add_vertex_node(ctx, lines, x2, y2, id, &prim->line_color2);
    add_vertex_node(ctx, lines, x - 1, y2, id, &prim->line_color2);
    add_vertex_node(ctx, lines, x2 + 1, y2, id, &prim->line_color2);
}

void draw_button(struct Context *ctx, GLfloat x, GLfloat y, GLfloat width, GLfloat height, void (*btn_callback)(struct Scene *scene, byte button)) {
    struct Primitives *prim = &ctx->prim;
    prim->btn_id = prim->buttons++;
    prim->btn_callback = btn_callback;
    GLfloat h2 = height / 2;
    vec4 color = prim->tri_color, color2 = prim->tri_color2, color3 = prim->tri_color3, color4 = prim->tri_color4;
    prim->tri_color3 = color;
    prim->tri_color4 = color2;
    draw_box(ctx, x, y, width, h2);
    prim->tri_color = prim->tri_color3 = color3;
    prim->tri_color2 = prim->tri_color4 = color4;
    draw_box(ctx, x, y + h2, width, h2);
    prim->tri_color = color;
    prim->tri_color2 = color2;
    draw_btn_rect(ctx, x, y, width, height);
    prim->btn_id = -1;
    prim->btn_callback = NULL;
}

void set_button_color(struct Context *ctx, float box_R, float box_G, float box_B, float line_R, float line_G, float line_B, float A) {
    A /= 255;
    struct Primitives *prim = &ctx->prim;
    prim->line_color = vector4_new(line_R / 255, line_G / 255, line_B / 255, A);
    prim->line_color2 = vector4_new(line_R / 1.32 / 255, line_G / 1.32 / 255, line_B / 1.32 / 255, A);
    prim->tri_color = vector4_new(box_R / 255, box_G / 255, box_B / 255, A);
    prim->tri_color2 = vector4_new(box_R * 1.1 / 255, box_G * 1.1 / 255, box_B * 1.1 / 255, A);
    prim->tri_color3 = vector4_new(box_R / 1.32 / 255, box_G / 1.32 / 255, box_B / 1.32 / 255, A);
    prim->tri_color4 = vector4_new(box_R / 1.32 * 1.1 / 255, box_G / 1.32 * 1.1 / 255, box_B / 1.32 * 1.1 / 255, A);
}
