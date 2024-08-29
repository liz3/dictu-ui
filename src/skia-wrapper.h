#ifndef DICTU_UI_SKIA_WRAPPER_H
#define DICTU_UI_SKIA_WRAPPER_H

#include "la.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  void *skia_raster_instance;
  void *surface_instance;
  uint32_t width;
  uint32_t height;
  uint32_t buffer_size;
  uint8_t* pixels;
} DictuSkiaInstance;

DictuSkiaInstance *create_ds_instance(uint32_t width, uint32_t height);

void drawLine(DictuSkiaInstance *instance, int32_t startX, int32_t startY,
              int32_t endX, int32_t endY, int32_t strokeWidth, Vec4f color);

void drawRect(DictuSkiaInstance *instance, int32_t startX, int32_t startY,
              int32_t width, int32_t height, int32_t border_radius,
              Vec4f color);
void drawRectOutline(DictuSkiaInstance *instance, int32_t startX,
                     int32_t startY, int32_t width, int32_t height,
                     int32_t stroke_width, int32_t border_radius, Vec4f color);

void drawText(DictuSkiaInstance *instance, int32_t startX, int32_t startY,
              const char *text, int32_t font_size, Vec4f color);
void *renderOutput(DictuSkiaInstance *instance);
void clear(DictuSkiaInstance *instance, Vec4f color);
void* getPath();
void deletePath(void* in);
void pathArcTo(void* in, int32_t x, int32_t y,int32_t w,int32_t l, float start, float sweep, bool force);
void pathMoveTo(void* in, int32_t x, int32_t y);
void pathLineTo(void* in, int32_t x, int32_t y);
void drawPath(DictuSkiaInstance* instance, void* in, Vec4f color);
void drawPathStroke(DictuSkiaInstance* instance, void* in, int32_t strokeWidth, Vec4f color);

void drawTextWithFont(DictuSkiaInstance *instance, int32_t x, int32_t y,
              const char *text, const char* fontName, int32_t font_size, Vec4f color);

float textWidth(DictuSkiaInstance *instance,
              const char *text, int32_t font_size);
float textWidthWithFont(DictuSkiaInstance *instance, 
              const char *text, const char* fontName, int32_t font_size);

void disposeInstance(DictuSkiaInstance *instance);
#ifdef __cplusplus
}
#endif
#endif