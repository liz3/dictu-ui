#include "dictu-ui.h"
#ifdef DICTU_UI_SOURCES
#include "GLFW/glfw3.h"
#endif
#include "dictu-include.h"
#include "la.h"
#include "skia-wrapper.h"
#include <lodepng.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef DICTU_UI_WINDOW_API
List g_list;
bool loaded_glad;
#endif

int dictu_ffi_init(DictuVM *vm, Table *method_table) {
  // mongoc_init();
  defineNative(vm, method_table, "decodePng", dictuUIDecodePng);
  defineNative(vm, method_table, "encodePng", dictuUIEncodePng);
  defineNative(vm, method_table, "skiaSurface", dictuUISkiaSurface);
#ifdef DICTU_UI_WINDOW_API
  defineNative(vm, method_table, "createWindow", dictuUICreateInstance);
  // defineNative(vm, method_table, "objectId", dictu_mongo_object_id);
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwSwapInterval(1);
  g_list.size = 0;
  g_list.head = NULL;
  g_list.tail = NULL;
  loaded_glad = false;
#endif
  return 0;
}

void freeDictuPng(DictuVM *vm, ObjAbstract *abstract) {
  // FREE_ARRAY(vm, uint8_t, buffer->bytes, buffer->size);
}

char *DictuPngToString(ObjAbstract *abstract) {
  UNUSED(abstract);

  char *bufferString = malloc(sizeof(char) * 6);
  snprintf(bufferString, 6, "<Png>");
  return bufferString;
}
void freeDictuSkiaSurface(DictuVM *vm, ObjAbstract *abstract) {
  if (abstract->data) {
    DictuSkiaInstance *instance = (DictuSkiaInstance *)abstract->data;
    disposeInstance(instance);
    abstract->data = NULL;
  }
  // FREE_ARRAY(vm, uint8_t, buffer->bytes, buffer->size);
}

char *DictuSkiaSurfaceToString(ObjAbstract *abstract) {
  UNUSED(abstract);

  char *bufferString = malloc(sizeof(char) * 13);
  snprintf(bufferString, 13, "<SkiaSurface>");
  return bufferString;
}
void freeDictuSkiaPath(DictuVM *vm, ObjAbstract *abstract) {
  if (abstract->data)
    deletePath(abstract->data);
  abstract->data = NULL;
}

char *DictuSkiaPathToString(ObjAbstract *abstract) {
  UNUSED(abstract);

  char *bufferString = malloc(sizeof(char) * 11);
  snprintf(bufferString, 11, "<SkiaPath>");
  return bufferString;
}
static Value dictuUISkiaSurfaceDrawLine(DictuVM *vm, int argCount,
                                        Value *args) {
  if (argCount == 6) {
    DictuSkiaInstance *instance = AS_SKIA_SURFACE(args[0]);
    drawLine(instance, AS_NUMBER(args[1]), AS_NUMBER(args[2]),
             AS_NUMBER(args[3]), AS_NUMBER(args[4]), AS_NUMBER(args[5]),
             colorToVec(vm, args[6]));
  }
  return NIL_VAL;
}
static Value dictuUISkiaSurfaceDrawRect(DictuVM *vm, int argCount,
                                        Value *args) {
  if (argCount == 6) {
    DictuSkiaInstance *instance = AS_SKIA_SURFACE(args[0]);
    drawRect(instance, AS_NUMBER(args[1]), AS_NUMBER(args[2]),
             AS_NUMBER(args[3]), AS_NUMBER(args[4]), AS_NUMBER(args[5]),
             colorToVec(vm, args[6]));
  }
  return NIL_VAL;
}
static Value dictuUISkiaSurfaceDrawRectOutline(DictuVM *vm, int argCount,
                                               Value *args) {
  if (argCount == 7) {
    DictuSkiaInstance *instance = AS_SKIA_SURFACE(args[0]);
    drawRectOutline(instance, AS_NUMBER(args[1]), AS_NUMBER(args[2]),
                    AS_NUMBER(args[3]), AS_NUMBER(args[4]), AS_NUMBER(args[5]),
                    AS_NUMBER(args[6]), colorToVec(vm, args[7]));
  }
  return NIL_VAL;
}
static Value dictuUISkiaSurfaceDrawText(DictuVM *vm, int argCount,
                                        Value *args) {

  if (argCount == 5) {
    ObjString *str = AS_STRING(args[3]);
    DictuSkiaInstance *instance = AS_SKIA_SURFACE(args[0]);
    drawText(instance, AS_NUMBER(args[1]), AS_NUMBER(args[2]), str->chars,
             AS_NUMBER(args[4]), colorToVec(vm, args[5]));
  }
  return NIL_VAL;
}
static Value dictuUISkiaSurfaceDrawTextWithFont(DictuVM *vm, int argCount,
                                                Value *args) {

  if (argCount == 6) {
    ObjString *str = AS_STRING(args[3]);
    ObjString *fontstr = AS_STRING(args[4]);
    DictuSkiaInstance *instance = AS_SKIA_SURFACE(args[0]);
    drawTextWithFont(instance, AS_NUMBER(args[1]), AS_NUMBER(args[2]),
                     str->chars, fontstr->chars, AS_NUMBER(args[5]),
                     colorToVec(vm, args[6]));
  }
  return NIL_VAL;
}
static Value dictuUISkiaSurfaceMeasureText(DictuVM *vm, int argCount,
                                           Value *args) {

  if (argCount == 2) {
    ObjString *str = AS_STRING(args[1]);
    DictuSkiaInstance *instance = AS_SKIA_SURFACE(args[0]);
    return textWidth(instance, str->chars, AS_NUMBER(args[2]));
  }
  return NIL_VAL;
}
static Value dictuUISkiaSurfaceMeasureTextWithFont(DictuVM *vm, int argCount,
                                                   Value *args) {

  if (argCount == 3) {
    ObjString *str = AS_STRING(args[1]);
    ObjString *fontstr = AS_STRING(args[2]);
    DictuSkiaInstance *instance = AS_SKIA_SURFACE(args[0]);
    return textWidthWithFont(instance, str->chars, fontstr->chars,
                             AS_NUMBER(args[3]));
  }
  return NIL_VAL;
}
static Value dictuUISkiaSurfaceRender(DictuVM *vm, int argCount, Value *args) {
  DictuSkiaInstance *instance = AS_SKIA_SURFACE(args[0]);
  uint8_t *data = renderOutput(instance);
  Value v =
      OBJ_VAL(copyString(vm, data, instance->width * instance->height * 4));
  free(data);
  return v;
}
static Value dictuUISkiaSurfaceDrawBuffer(DictuVM *vm, int argCount,
                                          Value *args) {
  if (argCount < 7)
    return NIL_VAL;
  DictuSkiaInstance *instance = AS_SKIA_SURFACE(args[0]);
  ObjString *buffer = AS_STRING(args[1]);
  int type = 0;
  if (argCount == 8)
    type = AS_NUMBER(args[8]);
  drawBuffer(instance, buffer->chars, AS_NUMBER(args[2]), AS_NUMBER(args[3]),
             AS_NUMBER(args[4]), AS_NUMBER(args[5]), AS_NUMBER(args[6]),
             AS_NUMBER(args[7]), type);
  return NIL_VAL;
}
static Value dictuUISkiaSurfaceClearColor(DictuVM *vm, int argCount,
                                          Value *args) {
  if (argCount != 1)
    return NIL_VAL;
  DictuSkiaInstance *instance = AS_SKIA_SURFACE(args[0]);
  clear(instance, colorToVec(vm, args[1]));
  return NIL_VAL;
}
static Value dictuUISkiaPathArcTo(DictuVM *vm, int argCount, Value *args) {
  if (argCount != 6)
    return NIL_VAL;
  void *instance = AS_SKIA_PATH(args[0]);
  pathArcTo(instance, AS_NUMBER(args[1]), AS_NUMBER(args[2]),
            AS_NUMBER(args[3]), AS_NUMBER(args[4]), AS_NUMBER(args[5]),
            AS_NUMBER(args[6]), false);
  return NIL_VAL;
}
static Value dictuUISkiaPathMoveTo(DictuVM *vm, int argCount, Value *args) {
  if (argCount != 2)
    return NIL_VAL;
  void *instance = AS_SKIA_PATH(args[0]);
  pathMoveTo(instance, AS_NUMBER(args[1]), AS_NUMBER(args[2]));
  return NIL_VAL;
}
static Value dictuUISkiaPathLineTo(DictuVM *vm, int argCount, Value *args) {
  if (argCount != 2)
    return NIL_VAL;
  void *instance = AS_SKIA_PATH(args[0]);
  pathLineTo(instance, AS_NUMBER(args[1]), AS_NUMBER(args[2]));
  return NIL_VAL;
}
static Value dictuUISkiaSurfaceDrawPath(DictuVM *vm, int argCount,
                                        Value *args) {
  if (argCount != 2)
    return NIL_VAL;
  DictuSkiaInstance *instance = AS_SKIA_SURFACE(args[0]);
  drawPath(instance, AS_SKIA_PATH(args[1]), colorToVec(vm, args[2]));
  return NIL_VAL;
}
static Value dictuUISkiaSurfaceDrawPathStroke(DictuVM *vm, int argCount,
                                              Value *args) {
  if (argCount != 3)
    return NIL_VAL;
  DictuSkiaInstance *instance = AS_SKIA_SURFACE(args[0]);
  drawPathStroke(instance, AS_SKIA_PATH(args[1]), AS_NUMBER(args[2]),
                 colorToVec(vm, args[3]));
  return NIL_VAL;
}
static Value dictuUISkiaSurfaceRotate(DictuVM *vm, int argCount, Value *args) {
  if (argCount != 1)
    return NIL_VAL;
  DictuSkiaInstance *instance = AS_SKIA_SURFACE(args[0]);
  rotate(instance, AS_NUMBER(args[1]));
  return NIL_VAL;
}
static Value dictuUISkiaSurfaceTranslate(DictuVM *vm, int argCount,
                                         Value *args) {
  if (argCount != 2)
    return NIL_VAL;
  DictuSkiaInstance *instance = AS_SKIA_SURFACE(args[0]);
  translate(instance, AS_NUMBER(args[1]), AS_NUMBER(args[2]));
  return NIL_VAL;
}
static Value dictuUISkiaSurfaceGetPath(DictuVM *vm, int argCount, Value *args) {

  DictuSkiaInstance *instance = AS_SKIA_SURFACE(args[0]);
  ObjAbstract *abstract =
      newAbstract(vm, freeDictuSkiaPath, DictuSkiaPathToString);
  push(vm, OBJ_VAL(abstract));
  defineNative(vm, &abstract->values, "arcTo", dictuUISkiaPathArcTo);
  defineNative(vm, &abstract->values, "moveTo", dictuUISkiaPathMoveTo);
  defineNative(vm, &abstract->values, "lineTo", dictuUISkiaPathLineTo);

  abstract->data = getPath();
  pop(vm);
  return OBJ_VAL(abstract);
}
static Value dictuUISkiaSurface(DictuVM *vm, int argCount, Value *args) {
  if (argCount != 2 || !IS_NUMBER(args[0]) || !IS_NUMBER(args[1]))
    return NIL_VAL;
  ObjAbstract *abstract =
      newAbstract(vm, freeDictuSkiaSurface, DictuSkiaSurfaceToString);
  push(vm, OBJ_VAL(abstract));
  defineNative(vm, &abstract->values, "drawLine", dictuUISkiaSurfaceDrawLine);
  defineNative(vm, &abstract->values, "drawRect", dictuUISkiaSurfaceDrawRect);
  defineNative(vm, &abstract->values, "drawRectOutline",
               dictuUISkiaSurfaceDrawRectOutline);
  defineNative(vm, &abstract->values, "drawText", dictuUISkiaSurfaceDrawText);
  defineNative(vm, &abstract->values, "drawTextWithFont",
               dictuUISkiaSurfaceDrawTextWithFont);
  defineNative(vm, &abstract->values, "measureText",
               dictuUISkiaSurfaceMeasureText);
  defineNative(vm, &abstract->values, "measureTextWithFont",
               dictuUISkiaSurfaceMeasureTextWithFont);
  defineNative(vm, &abstract->values, "render", dictuUISkiaSurfaceRender);
  defineNative(vm, &abstract->values, "clear", dictuUISkiaSurfaceClearColor);
  defineNative(vm, &abstract->values, "drawPath", dictuUISkiaSurfaceDrawPath);
  defineNative(vm, &abstract->values, "drawPathStroke",
               dictuUISkiaSurfaceDrawPathStroke);
  defineNative(vm, &abstract->values, "createPath", dictuUISkiaSurfaceGetPath);
  defineNative(vm, &abstract->values, "drawBuffer",
               dictuUISkiaSurfaceDrawBuffer);
  defineNative(vm, &abstract->values, "rotate", dictuUISkiaSurfaceRotate);
  defineNative(vm, &abstract->values, "translate", dictuUISkiaSurfaceTranslate);

  DictuSkiaInstance *instance =
      create_ds_instance(AS_NUMBER(args[0]), AS_NUMBER(args[1]));
  defineNativeProperty(vm, &abstract->values, "width",
                       NUMBER_VAL(instance->width));
  defineNativeProperty(vm, &abstract->values, "height",
                       NUMBER_VAL(instance->height));
  abstract->data = instance;
  pop(vm);
  return OBJ_VAL(abstract);
}
static Value dictuUIEncodePng(DictuVM *vm, int argCount, Value *args) {
  if (argCount < 3 || !IS_STRING(args[0]) || !IS_NUMBER(args[1]) ||
      !IS_NUMBER(args[2]))
    return NIL_VAL;
  ObjString *data = AS_STRING(args[0]);
  uint32_t width = AS_NUMBER(args[1]);
  uint32_t height = AS_NUMBER(args[2]);
  uint8_t *out;
  size_t out_size;
  int ret;
  LodePNGColorType type = LCT_RGBA;
  if (argCount == 4 && IS_NUMBER(args[3])) {
    int input_format = AS_NUMBER(args[3]);
    uint8_t *copy = malloc(data->length);
    memcpy(copy, data->chars, data->length);
    if (input_format == 2) { // BGRA
      for (size_t i = 0; i < data->length; i += 4) {
        uint8_t b = copy[i];
        copy[i] = copy[i + 2]; // swap red to first
        copy[i + 2] = b;       // set third to blue
      }
    } else if (input_format == 1) {
      type = LCT_RGB;
    }
    ret = lodepng_encode_memory(&out, &out_size, copy, width, height, type, 8);
    free(copy);
  } else {
    ret = lodepng_encode_memory(&out, &out_size, data->chars, width, height,
                                type, 8);
  }
  if (ret != 0) {
    return NIL_VAL;
  }
  Value outv = OBJ_VAL(copyString(vm, out, out_size));
  free(out);
  return outv;
}
static Value dictuUIDecodePng(DictuVM *vm, int argCount, Value *args) {
  if (argCount != 1 || !IS_STRING(args[0]))
    return NIL_VAL;
  ObjString *str = AS_STRING(args[0]);
  ObjAbstract *abstract = newAbstract(vm, freeDictuPng, DictuPngToString);
  push(vm, OBJ_VAL(abstract));
  uint32_t w, h;
  uint8_t *data;
  lodepng_decode_memory(&data, &w, &h, str->chars, str->length, LCT_RGBA, 8);
  defineNativeProperty(vm, &abstract->values, "data",
                       OBJ_VAL(copyString(vm, data, w * h * 4)));
  defineNativeProperty(vm, &abstract->values, "width", NUMBER_VAL(w));
  defineNativeProperty(vm, &abstract->values, "height", NUMBER_VAL(h));
  pop(vm);
  free(data);
  return OBJ_VAL(abstract);
}
#ifdef DICTU_UI_WINDOW_API
void freeDictuIUInstance(DictuVM *vm, ObjAbstract *abstract) {
  // FREE_ARRAY(vm, uint8_t, buffer->bytes, buffer->size);
}

char *DictuIUInstanceToString(ObjAbstract *abstract) {
  UNUSED(abstract);

  char *bufferString = malloc(sizeof(char) * 13);
  snprintf(bufferString, 13, "<UiInstance>");
  return bufferString;
}

static Value dictuUICopyBuffer(DictuVM *vm, int argCount, Value *args) {
  UiInstance *instance = AS_UI_INSTANCE(args[0]);
  if (argCount < 3 || !IS_STRING(args[1]) || !IS_NUMBER(args[2]) ||
      !IS_NUMBER(args[3]))
    return BOOL_VAL(false);
  if (instance->render_buffer.buffer)
    free(instance->render_buffer.buffer);
  if (argCount == 4 && !IS_NUMBER(args[4]))
    instance->render_buffer.type = AS_NUMBER(args[4]);
  ObjString *buffer = AS_STRING(args[1]);
  int32_t width = AS_NUMBER(args[2]);
  int32_t height = AS_NUMBER(args[3]);
  size_t pixel_size = instance->render_buffer.type == RGB ? 3 : 4;
  instance->render_buffer.buffer_size = width * height * pixel_size;
  instance->render_buffer.buffer = malloc(instance->render_buffer.buffer_size);
  instance->render_buffer.w = width;
  instance->render_buffer.h = height;
  memcpy(instance->render_buffer.buffer, buffer->chars,
         instance->render_buffer.buffer_size);
  glfwMakeContextCurrent(instance->window);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, instance->render_buffer.texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  GLint color_t = get_type_enum(&instance->render_buffer, 0);
  GLint p_type = get_type_enum(&instance->render_buffer, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, p_type, (GLsizei)width, (GLsizei)height, 0,
               color_t, GL_UNSIGNED_BYTE, NULL);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, color_t,
                  GL_UNSIGNED_BYTE, instance->render_buffer.buffer);
  return BOOL_VAL(true);
}
static Value dictuUIRender(DictuVM *vm, int argCount, Value *args) {
  UiInstance *instance = AS_UI_INSTANCE(args[0]);
  glfwMakeContextCurrent(instance->window);
  glfwGetFramebufferSize(instance->window, &instance->window_width,
                         &instance->window_height);
  glViewport(0, 0, instance->window_width, instance->window_height);
  shader_set2f(instance->shader, "resolution", (float)instance->window_width,
               (float)instance->window_height);
  RgbaColor clear_color = instance->clear_color;
  glClearColor((float)clear_color.r / 255, (float)clear_color.g / 255,
               (float)clear_color.b / 255, (float)clear_color.a / 255);
  glClear(GL_COLOR_BUFFER_BIT);
  Vec2f window_size;
  Vec2f start_pos = {0, 0};
  float bufferAspectRatio =
      (float)instance->render_buffer.w / (float)instance->render_buffer.h;
  float windowAspectRatio =
      (float)instance->window_width / (float)instance->window_height;

  if (bufferAspectRatio > windowAspectRatio) {
    window_size.x = instance->window_width;
    window_size.y = instance->window_width / bufferAspectRatio;
    start_pos.y = (instance->window_height - window_size.y) / 2;
  } else {
    window_size.y = instance->window_height;
    window_size.x = instance->window_height * bufferAspectRatio;
    start_pos.x = (instance->window_width - window_size.x) / 2;
  }
  SimpleShaderEntry entry = {normalize(instance, start_pos), window_size};
  shader_use(instance->shader);
  glBindTexture(GL_TEXTURE_2D, instance->render_buffer.texture_id);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(SimpleShaderEntry), &entry);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, 1);
  glfwSwapBuffers(instance->window);
  return NIL_VAL;
}

static Value dictuUIShouldClose(DictuVM *vm, int argCount, Value *args) {
  UiInstance *instance = AS_UI_INSTANCE(args[0]);
  return BOOL_VAL(glfwWindowShouldClose(instance->window));
}

static Value dictuUIShowWindow(DictuVM *vm, int argCount, Value *args) {
  if(argCount < 1 || !IS_BOOL(args[1]))
    return NIL_VAL;
  UiInstance *instance = AS_UI_INSTANCE(args[0]);
  if(AS_BOOL(args[1])) {
    glfwShowWindow(instance->window);

  } else {
    glfwHideWindow(instance->window);
  }
  return NIL_VAL;
}

static Value dictuUIClose(DictuVM *vm, int argCount, Value *args) {
  if (AS_ABSTRACT(args[0])->data == NULL)
    return BOOL_VAL(false);
  UiInstance *instance = AS_UI_INSTANCE(args[0]);
  glfwMakeContextCurrent(instance->window);
  list_remove(&g_list, (ListEntry *)instance->list_entry);
  instance->list_entry = NULL;
  glfwDestroyWindow(instance->window);
  if (instance->render_buffer.texture_was_allocated)
    glDeleteTextures(1, &(instance->render_buffer.texture_id));
  if (instance->render_buffer.buffer)
    free(instance->render_buffer.buffer);

  glDeleteProgram(instance->shader->pid);
  free(instance->shader);
  FREE(vm, UiInstance, instance);
  AS_ABSTRACT(args[0])->data = NULL;
  return BOOL_VAL(true);
}

static Value dictuUISetClearColor(DictuVM *vm, int argCount, Value *args) {
  if (argCount != 4 || !IS_NUMBER(args[1]) || !IS_NUMBER(args[2]) ||
      !IS_NUMBER(args[3]) || !IS_NUMBER(args[4])) {
    return NIL_VAL;
  }
  UiInstance *instance = AS_UI_INSTANCE(args[0]);
  RgbaColor clear_color = {.r = AS_NUMBER(args[1]),
                           .g = AS_NUMBER(args[2]),
                           .b = AS_NUMBER(args[3]),
                           .a = AS_NUMBER(args[4])};
  instance->clear_color = clear_color;
  return NIL_VAL;
}

static Value dictuUIWaitEvents(DictuVM *vm, int argCount, Value *args) {

  glfwWaitEvents();
  return NIL_VAL;
}
static Value dictuUIWaitEventsTimeout(DictuVM *vm, int argCount, Value *args) {
  if (argCount != 1 || !IS_NUMBER(args[1]))
    return NIL_VAL;
  glfwWaitEventsTimeout(AS_NUMBER(args[1]));
  return NIL_VAL;
}

void append_event(Event event, UiInstance *instance) {
  if (instance->event_length == 0) {
    instance->events = malloc(sizeof(Event));
  } else {
    instance->events =
        realloc(instance->events, sizeof(Event) * (instance->event_length + 1));
  }
  instance->events[instance->event_length++] = event;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  ListEntry *entry = list_find_window(&g_list, window);
  if (entry == NULL)
    return;
  UiInstance *instance = entry->instance;
  KeyEvent *ev_data = malloc(sizeof(KeyEvent));
  ev_data->is_ctrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
                     glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;
  ev_data->is_alt = glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS;
  ev_data->is_meta = glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS;
  ev_data->key = key;
  ev_data->mods = mods;
  ev_data->action = action;
  Event event = {KEY, ev_data};
  append_event(event, instance);
}
void character_callback(GLFWwindow *window, unsigned int codepoint) {
  ListEntry *entry = list_find_window(&g_list, window);
  if (entry == NULL)
    return;
  UiInstance *instance = entry->instance;
  CharEvent *ev_data = malloc(sizeof(CharEvent));
  ev_data->is_ctrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
                     glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;
  ev_data->is_alt = glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS;
  ev_data->is_meta = glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS;
  ev_data->cp = codepoint;
  Event event = {CHAR, ev_data};
  append_event(event, instance);
}
void window_focus_callback(GLFWwindow *window, int focused) {
  ListEntry *entry = list_find_window(&g_list, window);
  if (entry == NULL)
    return;

  UiInstance *instance = entry->instance;
  bool *v = malloc(sizeof(bool));
  *v = focused == 1;
  Event event = {WINDOW_FOCUS, v};
  append_event(event, instance);
}

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {
  ListEntry *entry = list_find_window(&g_list, window);
  if (entry == NULL)
    return;

  UiInstance *instance = entry->instance;
  KeyEvent *ev_data = malloc(sizeof(KeyEvent));
  ev_data->is_ctrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
                     glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;
  ev_data->is_alt = glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS;
  ev_data->is_meta = glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS;
  ev_data->key = button;
  ev_data->mods = mods;
  ev_data->action = action;
  Event event = {MOUSE_BUTTON, ev_data};
  append_event(event, instance);
}
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
  ListEntry *entry = list_find_window(&g_list, window);
  if (entry == NULL)
    return;

  UiInstance *instance = entry->instance;
  MousePosEvent *ev_data = malloc(sizeof(MousePosEvent));
  ev_data->is_ctrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
                     glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;
  ev_data->is_alt = glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS;
  ev_data->is_meta = glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS;
  ev_data->x = xpos;
  ev_data->y = ypos;

  Event event = {MOUSE_POS, ev_data};
  append_event(event, instance);
}
static Value dictuUIPollEvents(DictuVM *vm, int argCount, Value *args) {
  UiInstance *instance = AS_UI_INSTANCE(args[0]);
  ObjList *list = newList(vm);
  push(vm, OBJ_VAL(list));
  for (size_t i = 0; i < instance->event_length; i++) {
    Event ev = instance->events[i];
    ObjDict *d = newDict(vm);
    push(vm, OBJ_VAL(d));
    if (ev.type == CHAR) {
      CharEvent *data = (CharEvent *)ev.data;
      dictSet(vm, d, OBJ_VAL(copyString(vm, "type", 4)),
              OBJ_VAL(copyString(vm, "char", 4)));
      dictSet(vm, d, OBJ_VAL(copyString(vm, "ctrl", 4)),
              BOOL_VAL(data->is_ctrl));
      dictSet(vm, d, OBJ_VAL(copyString(vm, "alt", 3)), BOOL_VAL(data->is_alt));
      dictSet(vm, d, OBJ_VAL(copyString(vm, "meta", 4)),
              BOOL_VAL(data->is_meta));
      dictSet(vm, d, OBJ_VAL(copyString(vm, "cp", 2)), NUMBER_VAL(data->cp));
      writeValueArray(vm, &list->values, OBJ_VAL(d));
      free(data);
    } else if (ev.type == KEY) {
      KeyEvent *data = (KeyEvent *)ev.data;
      dictSet(vm, d, OBJ_VAL(copyString(vm, "type", 4)),
              OBJ_VAL(copyString(vm, "key", 3)));
      dictSet(vm, d, OBJ_VAL(copyString(vm, "ctrl", 4)),
              BOOL_VAL(data->is_ctrl));
      dictSet(vm, d, OBJ_VAL(copyString(vm, "alt", 3)), BOOL_VAL(data->is_alt));
      dictSet(vm, d, OBJ_VAL(copyString(vm, "meta", 4)),
              BOOL_VAL(data->is_meta));
      dictSet(vm, d, OBJ_VAL(copyString(vm, "key", 3)), NUMBER_VAL(data->key));
      dictSet(vm, d, OBJ_VAL(copyString(vm, "mods", 4)),
              NUMBER_VAL(data->mods));
      dictSet(vm, d, OBJ_VAL(copyString(vm, "action", 6)),
              NUMBER_VAL(data->action));
      writeValueArray(vm, &list->values, OBJ_VAL(d));
      free(data);
    } else if (ev.type == MOUSE_BUTTON) {
      KeyEvent *data = (KeyEvent *)ev.data;
      dictSet(vm, d, OBJ_VAL(copyString(vm, "type", 4)),
              OBJ_VAL(copyString(vm, "mouse_button", 12)));
      dictSet(vm, d, OBJ_VAL(copyString(vm, "ctrl", 4)),
              BOOL_VAL(data->is_ctrl));
      dictSet(vm, d, OBJ_VAL(copyString(vm, "alt", 3)), BOOL_VAL(data->is_alt));
      dictSet(vm, d, OBJ_VAL(copyString(vm, "meta", 4)),
              BOOL_VAL(data->is_meta));
      dictSet(vm, d, OBJ_VAL(copyString(vm, "button", 6)),
              NUMBER_VAL(data->key));
      dictSet(vm, d, OBJ_VAL(copyString(vm, "mods", 4)),
              NUMBER_VAL(data->mods));
      dictSet(vm, d, OBJ_VAL(copyString(vm, "action", 6)),
              NUMBER_VAL(data->action));
      writeValueArray(vm, &list->values, OBJ_VAL(d));
      free(data);
    } else if (ev.type == MOUSE_POS) {
      MousePosEvent *data = (MousePosEvent *)ev.data;
      dictSet(vm, d, OBJ_VAL(copyString(vm, "type", 4)),
              OBJ_VAL(copyString(vm, "mouse_position", 14)));
      dictSet(vm, d, OBJ_VAL(copyString(vm, "ctrl", 4)),
              BOOL_VAL(data->is_ctrl));
      dictSet(vm, d, OBJ_VAL(copyString(vm, "alt", 3)), BOOL_VAL(data->is_alt));
      dictSet(vm, d, OBJ_VAL(copyString(vm, "meta", 4)),
              BOOL_VAL(data->is_meta));
      dictSet(vm, d, OBJ_VAL(copyString(vm, "x", 1)), NUMBER_VAL(data->x));
      dictSet(vm, d, OBJ_VAL(copyString(vm, "y", 1)), NUMBER_VAL(data->y));
      writeValueArray(vm, &list->values, OBJ_VAL(d));
      free(data);
    } else if (ev.type == WINDOW_FOCUS) {
      bool *data = (bool *)ev.data;
      dictSet(vm, d, OBJ_VAL(copyString(vm, "type", 4)),
              OBJ_VAL(copyString(vm, "window_focus", 12)));
      dictSet(vm, d, OBJ_VAL(copyString(vm, "focus", 5)), BOOL_VAL(*data));

      writeValueArray(vm, &list->values, OBJ_VAL(d));
      free(data);
    } else if (ev.type == WINDOW_RESIZE) {
      SizeEvent *data = (SizeEvent *)ev.data;
      dictSet(vm, d, OBJ_VAL(copyString(vm, "type", 4)),
              OBJ_VAL(copyString(vm, "window_resize", 13)));
      dictSet(vm, d, OBJ_VAL(copyString(vm, "width", 5)),
              NUMBER_VAL(data->width));
      dictSet(vm, d, OBJ_VAL(copyString(vm, "height", 6)),
              NUMBER_VAL(data->height));
      writeValueArray(vm, &list->values, OBJ_VAL(d));
      free(data);
    }
    pop(vm);
  }
  pop(vm);
  free(instance->events);
  instance->events = NULL;
  instance->event_length = 0;
  return OBJ_VAL(list);
}
static Value dictuUIKeyState(DictuVM *vm, int argCount, Value *args) {
  if (argCount < 1 || !IS_NUMBER(args[1]))
    return NIL_VAL;
  UiInstance *instance = AS_UI_INSTANCE(args[0]);
  return NUMBER_VAL(glfwGetKey(instance->window, AS_NUMBER(args[1])));
}
static Value dictuUICreateInstance(DictuVM *vm, int argCount, Value *args) {
  if (argCount < 3 || !IS_STRING(args[0]) || !IS_NUMBER(args[1]) ||
      !IS_NUMBER(args[2]))
    return NIL_VAL;
  glfwDefaultWindowHints();
  ObjString *window_title = AS_STRING(args[0]);
  uint32_t window_width = AS_NUMBER(args[1]);
  uint32_t window_height = AS_NUMBER(args[2]);

  ObjAbstract *abstract =
      newAbstract(vm, freeDictuIUInstance, DictuIUInstanceToString);
  push(vm, OBJ_VAL(abstract));
  defineNative(vm, &abstract->values, "close", dictuUIClose);
  defineNative(vm, &abstract->values, "show", dictuUIShowWindow);
  defineNative(vm, &abstract->values, "keyState", dictuUIKeyState);
  defineNative(vm, &abstract->values, "shouldClose", dictuUIShouldClose);
  defineNative(vm, &abstract->values, "copyBuffer", dictuUICopyBuffer);
  defineNative(vm, &abstract->values, "setClearColor", dictuUISetClearColor);
  defineNative(vm, &abstract->values, "render", dictuUIRender);
  defineNative(vm, &abstract->values, "pollEvents", dictuUIPollEvents);
  defineNative(vm, &abstract->values, "waitEvents", dictuUIWaitEvents);
  defineNative(vm, &abstract->values, "waitEventsTimeout",
               dictuUIWaitEventsTimeout);
  UiInstance *instance = ALLOCATE(vm, UiInstance, 1);
  instance->window_width = window_width;
  instance->window_height = window_height;
  RgbaColor clear_color = {.r = 80, .g = 80, .b = 80, .a = 255};
  instance->clear_color = clear_color;
  instance->render_buffer.type = RGBA;
  instance->render_buffer.buffer = NULL;
  instance->events = NULL;
  instance->event_length = 0;
  instance->render_buffer.buffer_size = 0;
  instance->render_buffer.w = 0;
  instance->render_buffer.h = 0;
  if (argCount == 4 && IS_DICT(args[3])) {
    ObjDict *d = AS_DICT(args[3]);

    for (size_t i = 0; i < d->capacityMask + 1; i++) {
      if (IS_EMPTY(d->entries[i].key))
        continue;
      ObjString *key = AS_STRING(d->entries[i].key);
      Value v = d->entries[i].value;
      if (strcmp(key->chars, "resizable") == 0) {
        glfwWindowHint(GLFW_RESIZABLE, AS_BOOL(v));
      }
      if (strcmp(key->chars, "visible") == 0) {
        glfwWindowHint(GLFW_VISIBLE, AS_BOOL(v));
      }
      if (strcmp(key->chars, "decorated") == 0) {
        glfwWindowHint(GLFW_DECORATED, AS_BOOL(v));
      }
      if (strcmp(key->chars, "focused") == 0) {
        glfwWindowHint(GLFW_FOCUSED, AS_BOOL(v));
      }
      if (strcmp(key->chars, "always_top") == 0) {
        glfwWindowHint(GLFW_FLOATING, AS_BOOL(v));
      }
      if (strcmp(key->chars, "transparent") == 0) {
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, AS_BOOL(v));
      }
    }
  }

  instance->window = glfwCreateWindow(window_width, window_height,
                                      window_title->chars, NULL, NULL);
  glfwMakeContextCurrent(instance->window);
  glfwSetFramebufferSizeCallback(instance->window, framebuffer_size_callback);
  glfwSetCharCallback(instance->window, character_callback);
  glfwSetKeyCallback(instance->window, key_callback);
  glfwSetMouseButtonCallback(instance->window, mouse_button_callback);
  glfwSetWindowFocusCallback(instance->window, window_focus_callback);
  glfwSetCursorPosCallback(instance->window, cursor_position_callback);
  float xscale, yscale;
  glfwGetWindowContentScale(instance->window, &xscale, &yscale);
  instance->window_width *= xscale;
  instance->window_height *= yscale;
  if (!loaded_glad) {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      return NIL_VAL;
    }
    loaded_glad = true;
  }
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  ShaderVar vars[2] = {{2, sizeof(SimpleShaderEntry), GL_FLOAT,
                        (void *)offsetof(SimpleShaderEntry, pos)},
                       {2, sizeof(SimpleShaderEntry), GL_FLOAT,
                        (void *)offsetof(SimpleShaderEntry, size)}};
  instance->shader =
      create_shader(IMAGE_SHADER_VERT, IMAGE_SHADER_FRAG, 16, vars, 2);
  allocate_texture(&(instance->render_buffer));
  instance->list_entry = list_append(&g_list, instance);
  glfwPollEvents();
  abstract->data = instance;
  pop(vm);
  return OBJ_VAL(abstract);
}

GLuint simple_compile_shader(GLuint type, const char *content) {
  GLuint id = glCreateShader(type);
  glShaderSource(id, 1, &content, NULL);
  glCompileShader(id);

  GLint success;
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    printf("shader compile error: %s\n", content);
  }
  return id;
}

void shader_use(Shader *shader) {
  glUseProgram(shader->pid);
  glBindVertexArray(shader->vao);
  glBindBuffer(GL_ARRAY_BUFFER, shader->vbo);
}

void shader_set2f(Shader *shader, const char *name, float x, float y) {
  glUniform2f(glGetUniformLocation(shader->pid, name), x, y);
}
void shader_set4f(Shader *shader, const char *name, float x, float y, float z,
                  float w) {
  glUniform4f(glGetUniformLocation(shader->pid, name), x, y, z, w);
}

void shader_set1f(Shader *shader, const char *name, float v) {
  glUniform1f(glGetUniformLocation(shader->pid, name), v);
}
Shader *create_shader(const char *vertex_content, const char *fragment_content,
                      uint32_t size, ShaderVar *vars, size_t shader_var_len) {

  Shader *shader = calloc(1, sizeof(Shader));
  glGenVertexArrays(1, &(shader->vao));
  glGenBuffers(1, &(shader->vbo));
  glBindVertexArray(shader->vao);
  glBindBuffer(GL_ARRAY_BUFFER, shader->vbo);
  glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
  int count = 0;
  for (size_t i = 0; i < shader_var_len; i++) {
    ShaderVar entry = vars[i];
    glEnableVertexAttribArray(count);
    glVertexAttribPointer(count, entry.count, entry.type, GL_FALSE, entry.size,
                          entry.offset);
    glVertexAttribDivisor(count, 1);
    count++;
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  GLuint pid = glCreateProgram();
  GLuint vertex_shader =
      simple_compile_shader(GL_VERTEX_SHADER, vertex_content);
  GLuint fragment_shader =
      simple_compile_shader(GL_FRAGMENT_SHADER, fragment_content);
  glAttachShader(pid, vertex_shader);
  glAttachShader(pid, fragment_shader);
  glLinkProgram(pid);
  shader->pid = pid;
  shader->vertex_shader_id = vertex_shader;
  shader->fragment_shader_id = fragment_shader;

  return shader;
}

ListEntry *list_append(List *list, UiInstance *instance) {
  ListEntry *entry = malloc(sizeof(ListEntry));
  entry->prev = NULL;
  entry->next = NULL;
  entry->window = instance->window;
  entry->instance = instance;
  if (list->head == NULL) {
    list->head = entry;
    list->tail = entry;
  } else {
    ListEntry *prev_tail = list->tail;
    prev_tail->next = entry;
    entry->prev = prev_tail;
    list->tail = entry;
  }
  list->size++;

  return entry;
}
void list_remove(List *list, ListEntry *entry) {
  if (list->size == 1) {
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
  } else {
    ListEntry *prev = entry->prev;
    ListEntry *next = entry->next;
    if (prev != NULL && entry == list->tail) {
      list->tail = prev;
      prev->next = NULL;
    } else if (next != NULL && entry == list->head) {
      list->head = next;
      next->prev = NULL;
    } else if (prev != NULL && next != NULL) {
      next->prev = prev;
      prev->next = next;
    }
    list->size--;
  }
  free(entry);
}
ListEntry *list_find_window(List *list, GLFWwindow *window) {
  ListEntry *p = list->head;
  while (p != NULL) {
    if (p->window == window)
      return p;
    p = p->next;
  }
  return NULL;
}
Vec2f normalize(UiInstance *instance, Vec2f in) {
  Vec2f a = {.x = in.x - ((float)instance->window_width / 2),
             .y = in.y - ((float)instance->window_height / 2)};
  return a;
}
void allocate_texture(Image *image) {
  if (image->texture_was_allocated) {
    glDeleteTextures(1, &(image->texture_id));
  }
  glGenTextures(1, &(image->texture_id));
  image->texture_was_allocated = true;
}

GLint get_type_enum(Image *in, uint8_t type) {
  if (in->type == RGB) {
    return type == 1 ? GL_RGB8 : GL_RGB;
  }
  if (in->type == RGBA) {
    return type == 1 ? GL_RGBA8 : GL_RGBA;
  }
  if (in->type == BGRA) {
    return type == 1 ? GL_RGBA8 : GL_BGRA;
  }
  return 0;
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  ListEntry *entry = list_find_window(&g_list, window);
  if (entry == NULL)
    return;

  UiInstance *instance = entry->instance;

  float xscale, yscale;
  glfwGetWindowContentScale(instance->window, &xscale, &yscale);

  SizeEvent *ev_data = malloc(sizeof(SizeEvent));
  ev_data->height = height;
  ev_data->width = width;
  Event ev = {WINDOW_RESIZE, ev_data};
  append_event(ev, instance);
}
#endif

Vec4f colorToVec(DictuVM *vm, Value v) {
  float f = 255;
  int32_t r, g, b, a;
  ObjDict *d = AS_DICT(v);
  for (size_t i = 0; i < d->capacityMask + 1; i++) {
    if (IS_EMPTY(d->entries[i].key))
      continue;
    ObjString *key = AS_STRING(d->entries[i].key);
    int32_t value = AS_NUMBER(d->entries[i].value);
    if (strcmp(key->chars, "r") == 0)
      r = value;
    if (strcmp(key->chars, "g") == 0)
      g = value;
    if (strcmp(key->chars, "b") == 0)
      b = value;
    if (strcmp(key->chars, "a") == 0)
      a = value;
  }
#ifdef _WIN32
  return vec4f((float)b / f, (float)g / f, (float)r / f, (float)a / f);
#else
  return vec4f((float)r / f, (float)g / f, (float)b / f, (float)a / f);
#endif
}