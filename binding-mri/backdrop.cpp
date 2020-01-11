/*
** backdrop.cpp
**
** This file is part of HiddenChest
**
** Copyright (C) 2018-2019 Kyonides-Arkanthes
*/

#include "hcextras.h"
#include "bitmap.h"
#include "graphics.h"
#include "sharedstate.h"
#include "binding-util.h"
#include "binding-types.h"
#include "disposable-binding.h"

#define RV long long int

static RV backdrop, graphics;

static RV backdrop_keep_bitmap(RV self)
{
  Bitmap *result = shState->graphics().snapToBitmap();
  RV bitmap = wrapObject(result, BitmapType);
  return rb_iv_set(self, "@bitmap", bitmap);
}

static RV backdrop_invert_colors(RV self)
{
  Bitmap *result = shState->graphics().snapToBitmap();
  result->invert_colors();
  RV bitmap = wrapObject(result, BitmapType);
  return rb_iv_set(self, "@bitmap", bitmap);
}

static RV backdrop_gray_bitmap(RV self)
{
  Bitmap *result = shState->graphics().snap_to_gray_bitmap();
  RV bitmap = wrapObject(result, BitmapType);
  return self == graphics ? bitmap : rb_iv_set(self, "@bitmap", bitmap);
}

static RV backdrop_sepia_bitmap(RV self)
{
  Bitmap *result;
  result = shState->graphics().snap_to_sepia_bitmap();
  RV bitmap = wrapObject(result, BitmapType);
  return self == graphics ? bitmap : rb_iv_set(self, "@bitmap", bitmap);
}

static RV backdrop_color_bitmap(RV self, RV color)
{
  Bitmap *b;
  if (color == hc_sym("red"))
    b = shState->graphics().snap_to_color_bitmap(0);
  else if (color == hc_sym("green"))
    b = shState->graphics().snap_to_color_bitmap(1);
  else if (color == hc_sym("blue"))
    b = shState->graphics().snap_to_color_bitmap(2);
  else if (color == hc_sym("yellow"))
    b = shState->graphics().snap_to_color_bitmap(3);
  else if (color == hc_sym("black"))
    b = shState->graphics().snap_to_color_bitmap(4);
  else if (color == hc_sym("sepia"))
    b = shState->graphics().snap_to_sepia_bitmap();
  else if (color == hc_sym("gray"))
    b = shState->graphics().snap_to_gray_bitmap();
  else
    return Qnil;
  RV bitmap = wrapObject(b, BitmapType);
  return self == graphics ? bitmap : rb_iv_set(self, "@bitmap", bitmap);
}

static RV backdrop_gradient_bitmap(RV self, RV c1, RV c2, RV bln)
{
  Color *clr1 = static_cast<Color*>(Check_TypedStruct(c1, &ColorType));
  Color *clr2 = static_cast<Color*>(Check_TypedStruct(c2, &ColorType));
  int mode;
  if (bln == Qfalse) mode = 0;
  else if (bln == Qtrue) mode = 1;
  else if (bln == Qnil) mode = 2;
  Bitmap *b = shState->graphics().gradient_bitmap(clr1->norm, clr2->norm, mode);
  VALUE bitmap = wrapObject(b, BitmapType);
  return self == graphics ? bitmap : rb_iv_set(self, "@bitmap", bitmap);
}

static RV backdrop_pos_gradient_bitmap(RV self, RV x, RV y, RV cl1, RV cl2)
{
  Vec2i o(RB_FIX2INT(x), RB_FIX2INT(y));
  Color *c1 = static_cast<Color*>(Check_TypedStruct(cl1, &ColorType));
  Color *c2 = static_cast<Color*>(Check_TypedStruct(cl2, &ColorType));
  Bitmap *b = shState->graphics().snap_to_gradient_bitmap(o, c1->norm, c2->norm);
  RV bitmap = wrapObject(b, BitmapType);
  return self == graphics ? bitmap : rb_iv_set(self, "@bitmap", bitmap);
}

static RV backdrop_map_gradient_bitmap(RV self, RV op, RV x, RV y, RV cl1, RV cl2)
{
  int n = RB_FIX2INT(op);
  Vec2i v(RB_FIX2INT(x), RB_FIX2INT(y));
  Color *c1 = static_cast<Color*>(Check_TypedStruct(cl1, &ColorType));
  Color *c2 = static_cast<Color*>(Check_TypedStruct(cl2, &ColorType));
  Bitmap *b = shState->graphics().snap_to_map_gradient_bitmap(n, v, c1->norm, c2->norm);
  RV bitmap = wrapObject(b, BitmapType);
  return self == graphics ? bitmap : rb_iv_set(self, "@bitmap", bitmap);
}

Vec4 ary2vec4(RV color)
{
  int x = rb_ary_entry(color, 0), y = rb_ary_entry(color, 1);
  int z = rb_ary_entry(color, 2), w = rb_ary_entry(color, 3);
  return Vec4(x, y, z, w);
}

static RV backdrop_vignette_bitmap(RV self, RV color, RV sepia_bln)
{
  Vec4 vec;
  if (RB_TYPE_P(color, RUBY_T_ARRAY)) {
    vec = ary2vec4(color);
  } else {
    Color *c = static_cast<Color*>(Check_TypedStruct(color, &ColorType));
    vec = c->norm;
  }
  Bitmap *b = shState->graphics().snap_to_vignette(vec, sepia_bln == Qtrue);
  RV bitmap = wrapObject(b, BitmapType);
  return self == graphics ? bitmap : rb_iv_set(self, "@bitmap", bitmap);
}

static RV backdrop_blur_bitmap(RV self)
{
  Bitmap *b = shState->graphics().snapToBitmap();
  b->blur();
  RV bitmap = wrapObject(b, BitmapType);
  return rb_iv_set(self, "@bitmap", bitmap);
}

static RV backdrop_bitmap(RV self)
{
  return rb_iv_get(self, "@bitmap");
}

static RV backdrop_bitmap_dup(RV self)
{
  RV img = rb_iv_get(self, "@bitmap");
  return rb_obj_dup(img);
}

static RV backdrop_clear_bitmap(RV self)
{
  RV bitmap = rb_iv_get(self, "@bitmap");
  if (bitmap == Qnil) return Qnil;
  Bitmap *d = static_cast<Bitmap*>RTYPEDDATA_DATA(bitmap);
  if (!d || d->isDisposed()) return rb_iv_set(self, "@bitmap", Qnil);
  disposableDisposeChildren(bitmap);
  d->dispose();
  return rb_iv_set(self, "@bitmap", Qnil);
}

void init_backdrop()
{
  backdrop = rb_define_module("Backdrop");
  graphics = rb_define_module("Graphics");
  rb_iv_set(backdrop, "@bitmap", Qnil);
  rb_define_module_function(backdrop, "keep_bitmap", RMF(backdrop_keep_bitmap), 0);
  rb_define_module_function(backdrop, "invert_colors", RMF(backdrop_invert_colors), 0);
  rb_define_module_function(backdrop, "gray_bitmap", RMF(backdrop_gray_bitmap), 0);
  rb_define_module_function(backdrop, "sepia_bitmap", RMF(backdrop_sepia_bitmap), 0);
  rb_define_module_function(backdrop, "color_bitmap", RMF(backdrop_color_bitmap), 1);
  rb_define_module_function(backdrop, "gradient_bitmap", RMF(backdrop_gradient_bitmap), 3);
  rb_define_module_function(backdrop, "pos_gradient_bitmap", RMF(backdrop_pos_gradient_bitmap), 4);
  rb_define_module_function(backdrop, "map_gradient_bitmap", RMF(backdrop_map_gradient_bitmap), 5);
  rb_define_module_function(backdrop, "vignette_bitmap", RMF(backdrop_vignette_bitmap), 2);
  rb_define_module_function(backdrop, "blur_bitmap", RMF(backdrop_blur_bitmap), 0);
  rb_define_module_function(backdrop, "bitmap", RMF(backdrop_bitmap), 0);
  rb_define_module_function(backdrop, "bitmap_dup", RMF(backdrop_bitmap_dup), 0);
  rb_define_module_function(backdrop, "clear_bitmap", RMF(backdrop_clear_bitmap), 0);
  rb_define_module_function(graphics, "snap_to_gray_bitmap", RMF(backdrop_gray_bitmap), 0);
  rb_define_module_function(graphics, "snap_to_sepia_bitmap", RMF(backdrop_sepia_bitmap), 0);
  rb_define_module_function(graphics, "snap_to_color_bitmap", RMF(backdrop_color_bitmap), 1);
  rb_define_module_function(graphics, "gradient_bitmap", RMF(backdrop_gradient_bitmap), 3);
  rb_define_module_function(graphics, "pos_gradient_bitmap", RMF(backdrop_pos_gradient_bitmap), 4);
  rb_define_module_function(graphics, "map_gradient_bitmap", RMF(backdrop_map_gradient_bitmap), 5);
  rb_define_module_function(graphics, "vignette_bitmap", RMF(backdrop_vignette_bitmap), 2);
}
