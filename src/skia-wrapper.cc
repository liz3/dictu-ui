#include "skia-wrapper.h"
#include <cstdint>
#include <stdlib.h>
#include "include/core/SkColor.h"
#include "include/core/SkFont.h"
#include "include/core/SkImage.h"
#include "include/core/SkPath.h"
#include "include/core/SkSurface.h"
#include "include/core/SkPaint.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkRRect.h"
#include "include/core/SkPathEffect.h"
#include "include/core/SkTextBlob.h"
#include "include/core/SkBitmap.h"
#include "include/core/SkTypeface.h"
#include "include/private/base/SkPoint_impl.h"
#include <include/core/SkData.h>
#include <include/core/SkBitmap.h>
#include <include/core/SkMatrix.h>

#include "la.h"
#include <include/core/SkFontMgr.h>
#ifdef __APPLE__
#include <include/ports/SkFontMgr_mac_ct.h>
#endif
#ifdef _WIN32
#include <include/ports/SkTypeface_win.h>
#endif
#ifdef __linux__
#include <include/ports/SkFontMgr_fontconfig.h>
#endif

DictuSkiaInstance *create_ds_instance(uint32_t width, uint32_t height) {
  DictuSkiaInstance *instance =
      (DictuSkiaInstance *)malloc(sizeof(DictuSkiaInstance));
  sk_sp<SkSurface> surf(
      SkSurfaces::Raster(SkImageInfo::MakeN32Premul(width, height)));

  SkSurface *surface = surf.release();
  instance->skia_raster_instance = surface->getCanvas();
  instance->surface_instance = surface;
  instance->width = width;
  instance->height = height;
  return instance;
}
void disposeInstance(DictuSkiaInstance *instance) {
  SkCanvas *canvas =
      reinterpret_cast<SkCanvas *>(instance->skia_raster_instance);
  SkSurface *surface =
      reinterpret_cast<SkSurface *>(instance->surface_instance);
  delete surface;
  free(instance);
}

void drawLine(DictuSkiaInstance *instance, int32_t startX, int32_t startY,
              int32_t endX, int32_t endY, int32_t strokeWidth, Vec4f color) {

  SkCanvas *canvas =
      reinterpret_cast<SkCanvas *>(instance->skia_raster_instance);
  SkPaint paint;
  paint.setColor(SkColorSetARGB(color.w * 255, color.x * 255, color.y * 255,
                                color.z * 255));
  paint.setStrokeWidth(strokeWidth);
  canvas->drawLine(startX, startY, endX, endY, paint);
}

void drawRect(DictuSkiaInstance *instance, int32_t x, int32_t y, int32_t width,
              int32_t height, int32_t border_radius, Vec4f color) {
  SkCanvas *canvas =
      reinterpret_cast<SkCanvas *>(instance->skia_raster_instance);
  SkRect rect = SkRect::MakeXYWH(x, y, width, height);
  if (border_radius > 0) {
    SkRRect rrect;
    SkVector radii[] = {SkVector::Make(border_radius, border_radius),
                        SkVector::Make(border_radius, border_radius),
                        SkVector::Make(border_radius, border_radius),
                        SkVector::Make(border_radius, border_radius)};

    rrect.setRectRadii(rect, radii);
    // Paint for the fill
    SkPaint fillPaint;
    fillPaint.setStyle(SkPaint::kFill_Style);
    fillPaint.setColor(SkColorSetARGB(color.w * 255, color.x * 255,
                                      color.y * 255, color.z * 255));
    canvas->drawRRect(rrect, fillPaint);

  } else {
    SkPaint fillPaint;
    fillPaint.setStyle(SkPaint::kFill_Style);
    fillPaint.setColor(SkColorSetARGB(color.w * 255, color.x * 255,
                                      color.y * 255, color.z * 255));
    canvas->drawRect(rect, fillPaint);
  }
}
void drawRectOutline(DictuSkiaInstance *instance, int32_t x, int32_t y,
                     int32_t width, int32_t height, int32_t strokeWidth,
                     int32_t border_radius, Vec4f color) {
  SkCanvas *canvas =
      reinterpret_cast<SkCanvas *>(instance->skia_raster_instance);
  SkRect rect = SkRect::MakeXYWH(x, y, width, height);
  if (border_radius > 0) {
    SkRRect rrect;
    SkVector radii[] = {SkVector::Make(border_radius, border_radius),
                        SkVector::Make(border_radius, border_radius),
                        SkVector::Make(border_radius, border_radius),
                        SkVector::Make(border_radius, border_radius)};

    rrect.setRectRadii(rect, radii);
    // Paint for the fill
    SkPaint fillPaint;
    fillPaint.setStyle(SkPaint::kStroke_Style);
    fillPaint.setStrokeWidth(strokeWidth);
    fillPaint.setColor(SkColorSetARGB(color.w * 255, color.x * 255,
                                      color.y * 255, color.z * 255));
    canvas->drawRRect(rrect, fillPaint);

  } else {
    SkPaint fillPaint;
    fillPaint.setStyle(SkPaint::kStroke_Style);
    fillPaint.setStrokeWidth(strokeWidth);
    fillPaint.setColor(SkColorSetARGB(color.w * 255, color.x * 255,
                                      color.y * 255, color.z * 255));
    canvas->drawRect(rect, fillPaint);
  }
}
void drawText(DictuSkiaInstance *instance, int32_t x, int32_t y,
              const char *text, int32_t font_size, Vec4f color) {
#ifdef __APPLE__
  drawTextWithFont(instance, x, y, text, "Helvetica", font_size, color);
#else
  drawTextWithFont(instance, x, y, text, "Arial", font_size, color);
#endif
}
void drawTextWithFont(DictuSkiaInstance *instance, int32_t x, int32_t y,
                      const char *text, const char *fontName, int32_t font_size,
                      Vec4f color) {
  SkCanvas *canvas =
      reinterpret_cast<SkCanvas *>(instance->skia_raster_instance);
  SkPaint paint;
  paint.setColor(SkColorSetARGB(color.w * (float)255, color.x * (float)255,
                                color.y * (float)255, color.z * (float)255));

  paint.setAntiAlias(true);
#ifdef __APPLE__
  sk_sp<SkFontMgr> fontMgr = SkFontMgr_New_CoreText(nullptr);
#endif
#ifdef _WIN32
  sk_sp<SkFontMgr> fontMgr = SkFontMgr_New_GDI();
#endif
#ifdef __linux__
  sk_sp<SkFontMgr> fontMgr = SkFontMgr_New_FontConfig(nullptr);
#endif
  sk_sp<SkTypeface> fTypeface =
      fontMgr->legacyMakeTypeface(fontName, SkFontStyle::Normal());
  SkFont font;
  font.setSize(font_size);
  font.setTypeface(fTypeface);

  canvas->drawSimpleText(text, strlen(text), SkTextEncoding::kUTF8, x, y, font,
                         paint);
}
float textWidth(DictuSkiaInstance *instance, const char *text,
                int32_t font_size) {
#ifdef __APPLE__
  return textWidthWithFont(instance, text, "Helvetica", font_size);
#else
  return textWidthWithFont(instance, text, "Arial", font_size);
#endif
}
float textWidthWithFont(DictuSkiaInstance *instance, const char *text,
                        const char *fontName, int32_t font_size) {
  SkCanvas *canvas =
      reinterpret_cast<SkCanvas *>(instance->skia_raster_instance);
  SkPaint paint;

  paint.setAntiAlias(true);
#ifdef __APPLE__
  sk_sp<SkFontMgr> fontMgr = SkFontMgr_New_CoreText(nullptr);
#endif
#ifdef _WIN32
  sk_sp<SkFontMgr> fontMgr = SkFontMgr_New_GDI();
#endif
#ifdef __linux__
  sk_sp<SkFontMgr> fontMgr = SkFontMgr_New_FontConfig(nullptr);
#endif
  sk_sp<SkTypeface> fTypeface =
      fontMgr->legacyMakeTypeface(fontName, SkFontStyle::Normal());
  SkFont font;
  font.setSize(font_size);
  font.setTypeface(fTypeface);

  SkRect bounds;
  font.measureText(text, strlen(text), SkTextEncoding::kUTF8, &bounds);
  return bounds.width();
}
void *getPath() {
  SkPath *path = new SkPath;
  return path;
}
void deletePath(void *in) {
  SkPath *p = reinterpret_cast<SkPath *>(in);
  delete p;
}
void pathArcTo(void *in, int32_t x, int32_t y, int32_t w, int32_t l,
               float start, float sweep, bool force) {

  SkPath *p = reinterpret_cast<SkPath *>(in);
  SkRect rect = SkRect::MakeLTRB(x, y, w, l);
  p->arcTo(rect, start, sweep, force);
}
void pathLineTo(void *in, int32_t x, int32_t y) {
  SkPath *p = reinterpret_cast<SkPath *>(in);
  p->lineTo(SkPoint::Make(x, y));
}
void pathMoveTo(void *in, int32_t x, int32_t y) {
  SkPath *p = reinterpret_cast<SkPath *>(in);
  p->moveTo(SkPoint::Make(x, y));
}
void drawPath(DictuSkiaInstance *instance, void *in, Vec4f color) {
  SkCanvas *canvas =
      reinterpret_cast<SkCanvas *>(instance->skia_raster_instance);
  SkPath *p = reinterpret_cast<SkPath *>(in);
  SkPaint paint;
  paint.setAntiAlias(true);
  p->close();
  paint.setColor(SkColorSetARGB(color.w * (float)255, color.x * (float)255,
                                color.y * (float)255, color.z * (float)255));
  canvas->drawPath(*p, paint);
}
void drawPathStroke(DictuSkiaInstance *instance, void *in, int32_t strokeWidth,
                    Vec4f color) {
  SkCanvas *canvas =
      reinterpret_cast<SkCanvas *>(instance->skia_raster_instance);
  SkPath *p = reinterpret_cast<SkPath *>(in);
  SkPaint paint;
  paint.setStyle(SkPaint::kStroke_Style);
  paint.setAntiAlias(true);
  p->close();
  paint.setColor(SkColorSetARGB(color.w * (float)255, color.x * (float)255,
                                color.y * (float)255, color.z * (float)255));
  paint.setStrokeWidth(strokeWidth);
  canvas->drawPath(*p, paint);
}
void drawBuffer(DictuSkiaInstance *instance, uint8_t* data, int32_t data_width, int32_t data_height, int32_t x, int32_t y, int32_t target_width, int32_t target_height, int color_type) {
   SkCanvas *canvas =
      reinterpret_cast<SkCanvas *>(instance->skia_raster_instance);
    SkImageInfo imageInfo = SkImageInfo::MakeN32Premul(data_width, data_height);
      SkBitmap bitmap;
    bitmap.installPixels(imageInfo, (void*)data, data_width * (color_type ==1 ? 3 : 4));   
    sk_sp<SkImage> image = SkImages::RasterFromBitmap(bitmap);

    SkMatrix matrix;
    matrix.setScale((float)target_width / data_width, (float)target_height / data_height);
    canvas->setMatrix(matrix);
    canvas->drawImage(image, x, y);
    canvas->resetMatrix();
  
}
void clear(DictuSkiaInstance *instance, Vec4f color) {
  SkCanvas *canvas =
      reinterpret_cast<SkCanvas *>(instance->skia_raster_instance);
  SkColor clearColor =
      SkColorSetARGB(color.w * (float)255, color.x * (float)255,
                     color.y * (float)255, color.z * (float)255);

  canvas->clear(clearColor);
}
void *renderOutput(DictuSkiaInstance *instance) {
  SkSurface *surface =
      reinterpret_cast<SkSurface *>(instance->surface_instance);
  sk_sp<SkImage> image = surface->makeImageSnapshot();

  SkBitmap bitmap;
  SkImageInfo info =
      SkImageInfo::MakeN32Premul(instance->width, instance->height);
  bitmap.allocPixels(info);

  if (!image->readPixels(bitmap.info(), bitmap.getPixels(), bitmap.rowBytes(),
                         0, 0)) {
    return nullptr;
  }

  void *pixels = bitmap.getPixels();
  void *data = malloc(instance->width * instance->height * 4);
  memcpy(data, pixels, instance->width * instance->height * 4);
  return data;
}