/*
** point.cpp
**
** This file is part of HiddenChest
**
** Copyright (C) 2019 Kyonides-Arkanthes
*/

#include <SDL_rect.h>
#include "hcextras.h"

template<class C>
static void freeInstance(void *inst)
{
  delete static_cast<C*>(inst);
}

rb_data_type_t PointType = { "Point",
  { 0, freeInstance<SDL_Point>, 0, { 0, 0 } }, 0, 0, 0 };

static VALUE point_init(VALUE self, VALUE rx, VALUE ry)
{
  rb_iv_set(self, "@x", rx);
  rb_iv_set(self, "@y", rx);
  SDL_Point *p = new SDL_Point;
  p->x = RB_FIX2INT(rx);
  p->y = RB_FIX2INT(ry);
  RTYPEDDATA_DATA(self) = p;
  return self;
}

static VALUE point_x_get(VALUE self)
{
  return rb_iv_get(self, "@x");
}

static VALUE point_y_get(VALUE self)
{
  return rb_iv_get(self, "@y");
}

static VALUE point_x_set(VALUE self, VALUE rx)
{
  SDL_Point *p = static_cast<SDL_Point*>(RTYPEDDATA_DATA(self));
  if (!p) return Qnil;
  p->x = RB_FIX2INT(rx);
  return rb_iv_set(self, "@x", rx);
}

static VALUE point_y_set(VALUE self, VALUE ry)
{
  SDL_Point *p = static_cast<SDL_Point*>(RTYPEDDATA_DATA(self));
  if (!p) return Qnil;
  p->y = RB_FIX2INT(ry);
  return rb_iv_set(self, "@y", ry);
}

template<rb_data_type_t *PointType>
static VALUE point_allocate(VALUE klass)
{
  return rb_data_typed_object_wrap(klass, 0, PointType);
}

void init_point()
{
  VALUE point = rb_define_class("Point", rb_cObject);
  rb_define_alloc_func(point, point_allocate<&PointType>);
  rb_define_method(point, "initialize", RMF(point_init), 2);
  rb_define_method(point, "x", RMF(point_x_get), 0);
  rb_define_method(point, "y", RMF(point_y_get), 0);
  rb_define_method(point, "x=", RMF(point_x_set), 1);
  rb_define_method(point, "y=", RMF(point_y_set), 1);
}