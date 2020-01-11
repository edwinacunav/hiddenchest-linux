/*
** window-binding.cpp
**
** This file is part of mkxp.
**
** Copyright (C) 2013 Jonas Kulla <Nyocurio@gmail.com>
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
#include "windowvx.h"
#include "disposable-binding.h"
#include "viewportelement-binding.h"
#include "binding-util.h"
#include "bitmap.h"

DEF_TYPE_CUSTOMNAME(WindowVX, "Window");

void bitmapInitProps(Bitmap *b, VALUE self);

RB_METHOD(windowVXInitialize)
{
  WindowVX *w;
  if (rgssVer == 3) {
    int x, y, width, height;
    x = y = width = height = 0;
    if (argc == 4)
      rb_get_args(argc, argv, "iiii", &x, &y, &width, &height RB_ARG_END);
    w = new WindowVX(x, y, width, height);
  } else {
	w = viewportElementInitialize<WindowVX>(argc, argv, self);
  }
  setPrivateData(self, w);
  w->initDynAttribs();
  rb_iv_set(self, "@area", rb_ary_new());
  wrapProperty(self, &w->getCursorRect(), "cursor_rect", RectType);
  if (rgssVer == 3)
    wrapProperty(self, &w->getTone(), "tone", ToneType);
  Bitmap *contents = new Bitmap(1, 1);
  VALUE contentsObj = wrapObject(contents, BitmapType);
  bitmapInitProps(contents, contentsObj);
  rb_iv_set(self, "contents", contentsObj);
  return self;
}

static VALUE windowVXUpdate(VALUE self)
{
  WindowVX *w = getPrivateData<WindowVX>(self);
  w->update();
  return Qnil;
}

RB_METHOD(windowVXMove)
{
  WindowVX *w = getPrivateData<WindowVX>(self);
  int x, y, width, height;
  rb_get_args(argc, argv, "iiii", &x, &y, &width, &height RB_ARG_END);
  w->move(x, y, width, height);
  return Qnil;
}

static VALUE windowVXIsOpen(VALUE self)
{
  WindowVX *w = getPrivateData<WindowVX>(self);
  if (w == 0) return Qnil;
  return w->isOpen() ? Qtrue : Qfalse;
}

static VALUE windowVXIsClosed(VALUE self)
{
  WindowVX *w = getPrivateData<WindowVX>(self);
  if (w == 0) return Qnil;
  return w->isClosed() ? Qtrue : Qfalse;
}

static VALUE window_is_mouse_inside(VALUE self, VALUE pos)
{
  WindowVX *win = getPrivateData<WindowVX>(self);
  if (win == 0) return Qnil;
  int index = RB_FIX2INT(pos);
  VALUE rect = rb_ary_entry(rb_iv_get(self, "@area"), index);
  if (RB_NIL_P(rect)) return Qnil;
  int x = RB_FIX2INT(rb_ary_entry(rect, 0));
  int y = RB_FIX2INT(rb_ary_entry(rect, 1));
  int w = RB_FIX2INT(rb_ary_entry(rect, 2));
  int h = RB_FIX2INT(rb_ary_entry(rect, 3));
  return win->isMouseInside(x, y, w, h) ? Qtrue : Qfalse;
}

DEF_PROP_OBJ_REF(WindowVX, Bitmap, Windowskin, "windowskin")
DEF_PROP_OBJ_REF(WindowVX, Bitmap, Contents, "contents")
DEF_PROP_OBJ_VAL(WindowVX, Rect, CursorRect, "cursor_rect")
DEF_PROP_OBJ_VAL(WindowVX, Tone, Tone,       "tone")
DEF_PROP_I(WindowVX, X)
DEF_PROP_I(WindowVX, Y)
DEF_PROP_I(WindowVX, OX)
DEF_PROP_I(WindowVX, OY)
DEF_PROP_I(WindowVX, Width)
DEF_PROP_I(WindowVX, Height)
DEF_PROP_I(WindowVX, Padding)
DEF_PROP_I(WindowVX, PaddingBottom)
DEF_PROP_I(WindowVX, Opacity)
DEF_PROP_I(WindowVX, BackOpacity)
DEF_PROP_I(WindowVX, ContentsOpacity)
DEF_PROP_I(WindowVX, Openness)
DEF_PROP_B(WindowVX, Active)
DEF_PROP_B(WindowVX, ArrowsVisible)
DEF_PROP_B(WindowVX, Pause)

void windowVXBindingInit()
{
  VALUE klass = rb_define_class("Window", rb_cObject);
  rb_define_alloc_func(klass, classAllocate<&WindowVXType>);
  disposableBindingInit<WindowVX>(klass);
  viewportElementBindingInit<WindowVX>(klass);
  rb_define_attr(klass, "area", 1, 0);
  _rb_define_method(klass, "initialize", windowVXInitialize);
  rb_define_method(klass, "update", RMF(windowVXUpdate), 0);
  INIT_PROP_BIND(WindowVX, Windowskin,      "windowskin"      );
  INIT_PROP_BIND(WindowVX, Contents,        "contents"        );
  INIT_PROP_BIND(WindowVX, CursorRect,      "cursor_rect"     );
  INIT_PROP_BIND(WindowVX, Active,          "active"          );
  INIT_PROP_BIND(WindowVX, Pause,           "pause"           );
  INIT_PROP_BIND(WindowVX, X,               "x"               );
  INIT_PROP_BIND(WindowVX, Y,               "y"               );
  INIT_PROP_BIND(WindowVX, Width,           "width"           );
  INIT_PROP_BIND(WindowVX, Height,          "height"          );
  INIT_PROP_BIND(WindowVX, OX,              "ox"              );
  INIT_PROP_BIND(WindowVX, OY,              "oy"              );
  INIT_PROP_BIND(WindowVX, Opacity,         "opacity"         );
  INIT_PROP_BIND(WindowVX, BackOpacity,     "back_opacity"    );
  INIT_PROP_BIND(WindowVX, ContentsOpacity, "contents_opacity");
  INIT_PROP_BIND(WindowVX, Openness,        "openness"        );
  rb_define_method(klass, "mouse_inside?", RMF(window_is_mouse_inside), 1);
  rb_define_method(klass, "mouse_above?", RMF(window_is_mouse_inside), 1);
  _rb_define_method(klass, "move", windowVXMove);
  rb_define_method(klass, "open?", RMF(windowVXIsOpen), 0);
  rb_define_method(klass, "close?", RMF(windowVXIsClosed), 0);
  INIT_PROP_BIND(WindowVX, ArrowsVisible,   "arrows_visible");
  INIT_PROP_BIND(WindowVX, Padding,         "padding"       );
  INIT_PROP_BIND(WindowVX, PaddingBottom,   "padding_bottom");
  INIT_PROP_BIND(WindowVX, Tone,            "tone"          );
}
