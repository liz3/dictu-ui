#ifndef DICTU_UI_H
#define DICTU_UI_H

#include <dictu-include.h>

#include <stdint.h>
#include "la.h"
#ifdef __cplusplus
extern "C" {
#endif


#define AS_SKIA_SURFACE(v) ((DictuSkiaInstance *)AS_ABSTRACT(v)->data)
#define AS_SKIA_PATH(v) ((void *)AS_ABSTRACT(v)->data)

#ifdef DICTU_UI_WINDOW_API
#include "glad.h"
#include <GLFW/glfw3.h>

#define IMAGE_SHADER_VERT                                                      \
  "#version 330 core\n"                                                        \
  "uniform vec2 resolution;\n"                                                 \
  "layout(location = 0) in vec2 position;\n"                                   \
  "layout(location = 1) in vec2 size;\n"                                       \
  "vec2 camera_project(vec2 point) {\n"                                        \
  "  return 2 * (point) * (1 / resolution);\n"                                 \
  "}\n"                                                                        \
  "out vec2 uv;\n"                                                             \
  "void main() {\n"                                                            \
  "vec2 uvIn = vec2(float(gl_VertexID & 1),\n"                                 \
  "              float((gl_VertexID >> 1) & 1));\n"                            \
  "    uv = uvIn;\n"                                                           \
  "    vec2 r = camera_project(uvIn * size + position);\n"                     \
  "    r.y *= -1;\n"                                                           \
  "   gl_Position = vec4(r, 0.0f, 1.0f);\n"                                    \
  "}"

#define IMAGE_SHADER_FRAG                                                      \
  "#version 330 core\n"                                                        \
  "uniform sampler2D img;\n"                                                   \
  "in vec2 uv;\n"                                                              \
  "out vec4 color;\n"                                                          \
  "void main() {\n"                                                            \
  "  color = texture(img, uv);\n"                                              \
  "} \n"

typedef struct {
  uint32_t count;
  uint32_t size;
  uint16_t type;
  void *offset;
} ShaderVar;

typedef struct {
  Vec2f pos;
  Vec2f size;
} SimpleShaderEntry;

typedef struct {
  GLuint pid, vertex_shader_id, fragment_shader_id, vao, vbo;

} Shader;
typedef struct {
  uint8_t r, g, b, a;
} RgbaColor;
enum ImageType { RGBA, RGB, BGRA };
typedef struct {
  uint32_t w, h;
  uint8_t *buffer;
  enum ImageType type;
  GLuint texture_id;
  size_t buffer_size;
  bool texture_was_allocated;
} Image;

GLuint simple_compile_shader(GLuint type, const char *content);

void shader_use(Shader *shader);

Shader *create_shader(const char *vertex_content, const char *fragment_content,
                      uint32_t size, ShaderVar *vars, size_t shader_var_len);

void shader_set2f(Shader *shader, const char *name, float x, float y);
void shader_set4f(Shader *shader, const char *name, float x, float y, float z,
                  float w);
void shader_set1f(Shader *shader, const char *name, float v);

static Value dictuUICreateInstance(DictuVM *vm, int argCount, Value *args);

typedef struct {
    uint32_t cp;
    bool is_ctrl;
    bool is_alt;
    bool is_meta;
} CharEvent;

typedef struct {
    int key;
    int mods;
    int action;
    bool is_ctrl;
    bool is_alt;
    bool is_meta;
} KeyEvent;

typedef struct {
    int width;
    int height;
} SizeEvent;
typedef struct {
    double x;
    double y;
    bool is_ctrl;
    bool is_alt;
    bool is_meta;
} MousePosEvent;
enum EventType { CHAR, KEY, WINDOW_FOCUS, WINDOW_RESIZE, MOUSE_BUTTON, MOUSE_POS};
typedef struct {
    enum EventType type;
    void* data;
} Event;

typedef struct {
  GLFWwindow* window;
  int32_t window_width;
  int32_t window_height;
  Shader* shader;
  Image render_buffer;
RgbaColor clear_color;
  void *list_entry;
  Event* events;
  size_t event_length;
} UiInstance;

#define AS_UI_INSTANCE(v) ((UiInstance *)AS_ABSTRACT(v)->data)



typedef struct ListEntry_t {
  GLFWwindow *window;
  UiInstance *instance;
  struct ListEntry_t *prev;
  struct ListEntry_t *next;
} ListEntry;

typedef struct List {
  size_t size;
  ListEntry *head;
  ListEntry *tail;
} List;

ListEntry *list_append(List *list, UiInstance *instance);
void list_remove(List *list, ListEntry *entry);
ListEntry *list_find_window(List *list, GLFWwindow *window);

void allocate_texture(Image *image);

GLint get_type_enum(Image *in, uint8_t type);

Vec2f normalize(UiInstance *instance, Vec2f in);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void append_event(Event event, UiInstance* instance);

#endif

static Value dictuUIDecodePng(DictuVM *vm, int argCount, Value *args);

static Value dictuUIEncodePng(DictuVM *vm, int argCount, Value *args);

static Value dictuUISkiaSurface(DictuVM *vm, int argCount, Value *args);

Vec4f colorToVec(DictuVM* vm, Value v);

#ifdef DICTU_UI_MAC_LOCATION
static Value dictuUIMacLocation(DictuVM *vm, int argCount, Value *args);
#endif


#ifdef __cplusplus
}
#endif
#endif