/*
** graphics-binding.cpp
**
** This file is part of mkxpplus and mkxp.
**
** Copyright (C) 2013 Jonas Kulla <Nyocurio@gmail.com>
** 2018 Modified by Kyonides-Arkanthes
**
** mkxp is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 2 of the License, or
** (at your option) any later version.
**
** mkxp is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with mkxp.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "hcextras.h"
#include "graphics.h"
#include "sharedstate.h"
#include "binding-util.h"
#include "binding-types.h"
#include "exception.h"

#define RV long long int

static RV graphicsUpdate(RV self)
{
  shState->graphics().update();
  return Qnil;
}

static RV graphicsFreeze(RV self)
{
  shState->graphics().freeze();
  return Qnil;
}

RB_METHOD(graphicsTransition)
{
  RB_UNUSED_PARAM;
  int duration = 8;
  const char *filename = "";
  int vague = 40;
  if (argc > 0)
    rb_get_args(argc, argv, "|izi", &duration, &filename, &vague RB_ARG_END);
  GUARD_EXC( shState->graphics().transition(duration, filename, vague); )
  return Qnil;
}

static RV graphicsFrameReset(RV self)
{
  shState->graphics().frameReset();
  return Qnil;
}

static RV graphicsWidth(RV self)
{
  return RB_INT2FIX(shState->graphics().width());
}

static RV graphicsHeight(RV self)
{
  return RB_INT2FIX(shState->graphics().height());
}

static RV graphics_dimensions(RV self)
{
  RV width = RB_INT2NUM( shState->graphics().width() );
  RV height = RB_INT2NUM( shState->graphics().height() );
  return rb_ary_new3(2, width, height);
}

RB_METHOD(graphicsWait)
{
  RB_UNUSED_PARAM;
  int duration;
  rb_get_args(argc, argv, "i", &duration RB_ARG_END);
  shState->graphics().wait(duration);
  return Qnil;
}

RB_METHOD(graphicsFadeout)
{
  RB_UNUSED_PARAM;
  int duration;
  rb_get_args(argc, argv, "i", &duration RB_ARG_END);
  shState->graphics().fadeout(duration);
  return Qnil;
}

RB_METHOD(graphicsFadein)
{
  RB_UNUSED_PARAM;
  int duration;
  rb_get_args(argc, argv, "i", &duration RB_ARG_END);
  shState->graphics().fadein(duration);
  return Qnil;
}

void bitmapInitProps(Bitmap *b, VALUE self);

static RV graphicsSnapToBitmap(RV self)
{
  Bitmap *result = 0;
  GUARD_EXC( result = shState->graphics().snapToBitmap(); );
  RV obj = wrapObject(result, BitmapType);
  bitmapInitProps(result, obj);
  return obj;
}

static RV graphics_save_screenshot(RV self)
{
  bool result = false;
  RV set = rb_const_get(rb_cObject, rb_intern("Settings"));
  safe_mkdir(rb_iv_get(set, "@snapshot_dir"));
  result = shState->graphics().save_screenshot();
  return result;
}

static RV graphicsResizeScreen(RV self, RV w, RV h)
{
  int width = RB_FIX2INT(w), height = RB_FIX2INT(h);
  shState->graphics().resizeScreen(width, height);
  return Qnil;
}

static RV graphicsReset(RV self)
{
  shState->graphics().reset();
  return Qnil;
}

static RV graphicsPlayMovie(RV self, RV filename)
{
  const char *fn = RSTRING_PTR(filename);
  shState->graphics().playMovie(fn);
  return Qnil;
}

static RV graphicsGetFrameRate(RV self)
{
  return rb_fix_new(shState->graphics().getFrameRate());
}

static RV graphicsSetFrameRate(RV self, RV num)
{
  shState->graphics().setFrameRate(RB_FIX2INT(num));
  return rb_fix_new(shState->graphics().getFrameRate());
}

static RV graphicsGetFrameCount(RV self)
{
  return rb_fix_new(shState->graphics().getFrameCount());
}

static RV graphicsSetFrameCount(RV self, RV num)
{
  shState->graphics().setFrameCount(RB_FIX2INT(num));
  return rb_fix_new(shState->graphics().getFrameCount());
}

static RV graphicsGetBrightness(RV self)
{
  return rb_fix_new(shState->graphics().getBrightness());
}

static RV graphicsSetBrightness(RV self, RV num)
{
  shState->graphics().setBrightness(RB_FIX2INT(num));
  return rb_fix_new(shState->graphics().getBrightness());
}

static RV graphics_get_block_fullscreen(RV self)
{
  return rb_iv_get(self, "@block_fullscreen");
}

static RV graphics_set_block_fullscreen(RV self, RV boolean)
{
  shState->graphics().set_block_fullscreen(boolean == Qtrue);
  return rb_iv_set(self, "@block_fullscreen", boolean);
}

static RV graphics_get_block_ftwelve(RV self)
{
  return rb_iv_get(self, "@block_f12");
}

static RV graphics_set_block_ftwelve(RV self, RV boolean)
{
  shState->graphics().set_block_ftwelve(boolean == Qtrue);
  return rb_iv_set(self, "@block_f12", boolean);
}

static RV graphics_get_block_fone(RV self)
{
  return rb_iv_get(self, "@block_f1");
}

static RV graphics_set_block_fone(RV self, RV boolean)
{
  shState->graphics().set_block_fone(boolean == Qtrue);
  return rb_iv_set(self, "@block_f1", boolean);
}

static RV graphicsGetFullscreen(RV self)
{
  return shState->graphics().get_fullscreen() ? Qtrue : Qfalse;
}

static RV graphicsSetFullscreen(RV self, RV boolean)
{
  if (rb_iv_get(self, "@block_fullscreen") == Qtrue) return Qfalse;
  shState->graphics().set_fullscreen(boolean == Qtrue);
  return shState->graphics().get_fullscreen() ? Qtrue : Qfalse;
}

static RV graphics_get_show_cursor(RV self)
{
  return shState->graphics().get_show_cursor() ? Qtrue : Qfalse;
}

static RV graphics_set_show_cursor(RV self, RV boolean)
{
  shState->graphics().set_show_cursor(boolean == Qtrue);
  return boolean;
}

void graphicsBindingInit()
{
  RV module = rb_define_module("Graphics");
  rb_iv_set(module, "@block_fullscreen", Qfalse);
  rb_iv_set(module, "@block_f12", Qfalse);
  rb_iv_set(module, "@block_f1", Qfalse);
  rb_define_module_function(module, "update", RMF(graphicsUpdate), 0);
  rb_define_module_function(module, "freeze", RMF(graphicsFreeze), 0);
  rb_define_module_function(module, "transition", RMF(graphicsTransition), -1);
  rb_define_module_function(module, "frame_reset", RMF(graphicsFrameReset), 0);
  rb_define_module_function(module, "__reset__", RMF(graphicsReset), 0);
  rb_define_module_function(module, "frame_rate", RMF(graphicsGetFrameRate), 0);
  rb_define_module_function(module, "frame_rate=", RMF(graphicsSetFrameRate), 1);
  rb_define_module_function(module, "frame_count", RMF(graphicsGetFrameCount), 0);
  rb_define_module_function(module, "frame_count=", RMF(graphicsSetFrameCount), 1);
  rb_define_module_function(module, "width", RMF(graphicsWidth), 0);
  rb_define_module_function(module, "height", RMF(graphicsHeight), 0);
  rb_define_module_function(module, "dimensions", RMF(graphics_dimensions), 0);
  rb_define_module_function(module, "size", RMF(graphics_dimensions), 0);
  rb_define_module_function(module, "wait", RMF(graphicsWait), -1);
  rb_define_module_function(module, "fadeout", RMF(graphicsFadeout), -1);
  rb_define_module_function(module, "fadein", RMF(graphicsFadein), -1);
  rb_define_module_function(module, "snap_to_bitmap", RMF(graphicsSnapToBitmap), 0);
  rb_define_module_function(module, "save_screenshot", RMF(graphics_save_screenshot), 0);
  rb_define_module_function(module, "save_snapshot", RMF(graphics_save_screenshot), 0);
  rb_define_module_function(module, "resize_screen", RMF(graphicsResizeScreen), 2);
  rb_define_module_function(module, "brightness", RMF(graphicsGetBrightness), 0);
  rb_define_module_function(module, "brightness=", RMF(graphicsSetBrightness), 1);
  rb_define_module_function(module, "play_movie", RMF(graphicsPlayMovie), 1);
  rb_define_module_function(module, "block_fullscreen", RMF(graphics_get_block_fullscreen), 0);
  rb_define_module_function(module, "block_fullscreen=", RMF(graphics_set_block_fullscreen), 1);
  rb_define_module_function(module, "block_f12", RMF(graphics_get_block_ftwelve), 0);
  rb_define_module_function(module, "block_f12=", RMF(graphics_set_block_ftwelve), 1);
  rb_define_module_function(module, "block_f1", RMF(graphics_get_block_fone), 0);
  rb_define_module_function(module, "block_f1=", RMF(graphics_set_block_fone), 1);
  rb_define_module_function(module, "fullscreen", RMF(graphicsGetFullscreen), 0);
  rb_define_module_function(module, "fullscreen=", RMF(graphicsSetFullscreen), 1);
  rb_define_module_function(module, "show_cursor", RMF(graphics_get_show_cursor), 0);
  rb_define_module_function(module, "show_cursor=", RMF(graphics_set_show_cursor), 1);
}
