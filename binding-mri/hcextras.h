/*
** hcextras.h
**
** This file is part of HiddenChest.
**
** Copyright (C) 2019 Kyonides-Arkanthes <kyonides@gmail.com>
**
** HiddenChest is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 2 of the License, or
** (at your option) any later version.
**
** HiddenChest is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with HiddenChest.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HCEXTRAS_H
#define HCEXTRAS_H

#include <ruby.h>

#define RMF(func) ((VALUE (*)(ANYARGS))(func))
#define rb_str(x) rb_str_new_cstr(x)
#define rb_ary_c_ary(a) RARRAY(a)->as.heap.ptr

#define ARRAY_TYPE_P(obj) (!RB_SPECIAL_CONST_P(obj) && \
  RB_BUILTIN_TYPE(obj) == RUBY_T_ARRAY)

#if defined(hdec)
#elif (defined _MSC_VER)
#define hdec __declspec(dllexport)
#elif (defined __SUNPRO_C)
#define hdec __global
#else
#define hdec
#endif

static VALUE hc_sym(const char* str)
{
  return rb_id2sym(rb_intern(str));
}

static VALUE hc_sym2(VALUE str)
{
  return rb_id2sym(rb_intern_str(str));
}

void safe_mkdir(VALUE dir);

#endif
