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
    prim->btn_attr = NULL;
    prim->glyph = NULL;
    prim->glyph_height = 0;

    init_fonts(ctx);
}

void add_vertex_node(struct Context *ctx, struct VertexList *list, GLfloat x, GLfloat y, GLfloat z, vec4 *color) {
    struct Primitives *prim = &ctx->prim;
    GLfloat aspect = ctx->window_size.w;
    x = x * aspect / 600 * 2 - 1;
    y = 1 - y / 600 * 2;
    struct VertexNode orig = {NULL, x, y, z, color->x, color->y, color->z, color->w, prim->btn_id, prim->btn_callback, prim->btn_attr, prim->glyph, prim->glyph_height};
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
        memcpy(&list->last->x, &orig.x, sizeof(struct VertexNode) - sizeof(size_t));
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

    render_glyphs(ctx);

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
    set_text_alignment(ctx, 0, 0, 0);
    font->glyphs.first = NULL;
    font->glyphs.last = NULL;
    font->glyphs.n = font->glyphs.dropped_n = 0;

    glGenVertexArrays(1, &font->VAO);
    glGenBuffers(1, &font->VBO);
    glBindVertexArray(font->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, font->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 5, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
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
    font->text_color = vector4_new(R / 255, G / 255, B / 255, A / 255);
}

int unicoder(struct Context *ctx, text str, struct Character **glyphs) {
    int let = 0, size = 0;
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
    }
    return size;
}

void render_text(struct Context *ctx, text str, GLfloat x, GLfloat y, GLfloat scale) {
    struct Font *font = &ctx->font;
    int bytes = 0;
    while (str[bytes]) bytes++;
    struct Character *glyphs[bytes];
    int size = unicoder(ctx, str, glyphs);
    vec4 measure = measure_text(glyphs, size);

    scale /= font->current_height;
    if (font->width_limit >= 5) {
        GLfloat bust = measure.z * scale / font->width_limit;
        if (bust > 1) scale /= bust;
    }

    x /= scale;
    y /= scale;
    byte round = 5;
    x -= round;
    y -= round;
    GLfloat start_x = x;

    x -= measure.x + measure.z / 2 * font->align_left;
    y -= measure.w / 2 * font->align_up - measure.y;

    struct Primitives *prim = &ctx->prim;
    struct VertexList *glyphs_list = &font->glyphs;
    prim->glyph_height = scale;

    for (int let = 0; let < size; let++) {
        struct Character *glyph = glyphs[let];

        GLfloat xpos = (x + glyph->bearing.x) * scale;
        GLfloat ypos = (y + glyph->size.y - glyph->bearing.y) * scale;

        prim->glyph = glyph;
        GLfloat id = prim->all_prims_n++;
        add_vertex_node(ctx, glyphs_list, xpos, ypos, id, &font->text_color);

        if (glyph->code == '\n')
            x = start_x, y += font->current_height;
        else
            x += glyph->advance / 64.;
    }
    prim->glyph = NULL;
    prim->glyph_height = 0;
}

void render_glyphs(struct Context *ctx) {
    struct Font *font = &ctx->font;
    struct VertexList *list = &font->glyphs;
    struct VertexNode *p = list->first;

    GLfloat aspect = ctx->window_size.w;
    vec4 pred_color = vector4_new(-1, -1, -1, -1);

    glUseProgram(font->font_program);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(font->VAO);
    for (int i = 0; i < list->n; i++) {
        struct Character *glyph = p->glyph;
        GLfloat xpos = p->x, ypos = p->y, zpos = -0.999 - 0.001 * p->z / ctx->prim.all_prims_n;
        GLfloat scale = p->glyph_height;
        vec4 color = vector4_new(p->r, p->g, p->b, p->a);
        if (!vector4_cmp(pred_color, color)) {
            glUniform4fv(font->color_loc, 1, (GLfloat *) &color);
            pred_color = color;
        }

        GLfloat w = glyph->size.x * scale;
        GLfloat h = glyph->size.y * scale;
        w = w * aspect / 600 * 2;
        h = h / 600 * 2;

        GLfloat vertices[6][5] = {
            {xpos, ypos + h, zpos, 0, 0},
            {xpos, ypos, zpos, 0, 1},
            {xpos + w, ypos, zpos, 1, 1},
            {xpos, ypos + h, zpos, 0, 0},
            {xpos + w, ypos, zpos, 1, 1},
            {xpos + w, ypos + h, zpos, 1, 0}};

        glBindTexture(GL_TEXTURE_2D, glyph->texture_id);

        glBindBuffer(GL_ARRAY_BUFFER, font->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        p = p->next;
    }
    list->last = list->first;
    list->dropped_n = list->n;
    list->n = 0;

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

vec4 measure_text(struct Character **glyphs, int size) {
    GLfloat max_up = 0, max_down = 0, right = size ? glyphs[0]->bearing.x : 0, width = -right;
    for (int let = 0; let < size; let++) {
        struct Character *glyph = glyphs[let];
        if (glyph->bearing.y > max_up || !let) max_up = glyph->bearing.y;
        GLfloat down = glyph->size.y - glyph->bearing.y;
        if (down > max_down || !let) max_down = down;
        width += let == size - 1 ? glyph->bearing.x + glyph->size.x : glyph->advance / 64.;
    }
    return vector4_new(right, max_up, width, max_up + max_down / 2);
}

void set_text_alignment(struct Context *ctx, byte align_left, byte align_up, GLfloat width_limit) {
    struct Font *font = &ctx->font;
    font->align_left = align_left;
    font->align_up = align_up;
    font->width_limit = width_limit;
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
    struct VertexNode *p = font->glyphs.first, *next;
    while (p) {
        next = p->next;
        free(p);
        p = next;
    }
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

void draw_button(struct Context *ctx, GLfloat x, GLfloat y, GLfloat width, GLfloat height, void (*btn_callback)(struct Scene *scene, byte button), text str, void *attr) {
    struct Primitives *prim = &ctx->prim;
    prim->btn_id = prim->buttons++;
    prim->btn_callback = btn_callback;
    prim->btn_attr = attr;
    GLfloat w2 = width / 2, h2 = height / 2;
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
    prim->btn_attr = NULL;

    struct Font *font = &ctx->font;
    byte tmp = font->align_left, tmp2 = font->align_up;
    GLfloat tmp3 = font->width_limit;
    set_text_alignment(ctx, 1, 1, width);
    render_text(ctx, str, x + w2, y + h2, height);
    set_text_alignment(ctx, tmp, tmp2, tmp3);
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
