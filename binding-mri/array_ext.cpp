/*
** array.cpp - Array Extensions
**
** This file is part of HiddenChest
**
** Copyright (C) 2018-2019 Kyonides-Arkanthes
*/

#include "hcextras.h"
//#define ARY_MAX_SIZE (LONG_MAX / (int)sizeof(VALUE))

static VALUE hc_ary_items_min_max(VALUE self, VALUE min, VALUE max)
{
  if (min > max)
    rb_raise(rb_eArgError, "(min argument must be smaller than max argument)");
  rb_check_type(min, RUBY_T_FIXNUM);
  rb_check_type(max, RUBY_T_FIXNUM);
  rb_iv_set(self, "@min_items", min);
  rb_iv_set(self, "@max_items", max);
  return rb_ary_new_from_args(2, min, max);
}

static VALUE hc_ary_push_upto_max(VALUE self, VALUE val)
{
  long n = RB_FIX2INT(rb_iv_get(self, "@max_items"));
  return (n > 0 && RARRAY_LEN(self) == n)? self : rb_ary_push(self, val);
}

static VALUE hc_ary_unshift_upto_max(VALUE self, VALUE val)
{
  long n = RB_FIX2INT(rb_iv_get(self, "@max_items"));
  return (n > 0 && RARRAY_LEN(self) == n)? self : rb_ary_unshift(self, val);
}

static VALUE hc_ary_pop_upto_min(VALUE self)
{
  long n = RB_FIX2INT(rb_iv_get(self, "@min_items"));
  if (RARRAY_LEN(self) <= n) return self;
  return rb_ary_pop(self);
}

static VALUE hc_ary_shift_upto_min(VALUE self)
{
  long n = RB_FIX2INT(rb_iv_get(self, "@min_items"));
  if (RARRAY_LEN(self) <= n) return self;
  return rb_ary_shift(self);
}

static VALUE hc_ary_clear_upto_min(VALUE self)
{
  long n = RB_FIX2INT(rb_iv_get(self, "@min_items"));
  if (RARRAY_LEN(self) <= n) return self;
  return rb_ary_replace(rb_ary_subseq(self, 0, n), self);
}

static VALUE hc_ary_push_uniq(VALUE self, VALUE val)
{
  long n = RB_FIX2INT(rb_iv_get(self, "@max_items"));
  if (rb_ary_includes(self, val) == Qtrue || RARRAY_LEN(self) == n) return self;
  return rb_ary_push(self, val);
}

static VALUE hc_ary_unshift_uniq(VALUE self, VALUE val)
{
  long n = RB_FIX2INT(rb_iv_get(self, "@max_items"));
  if (rb_ary_includes(self, val) == Qtrue || RARRAY_LEN(self) == n) return self;
  return rb_ary_unshift(self, val);
}

static VALUE hc_ary_even_indices(VALUE self)
{
  VALUE ary = rb_ary_new();
  long max = RARRAY_LEN(self);
  for (long n = 1; n < max; n += 2)
    rb_ary_push(ary, rb_ary_entry(self, n));
  return ary;
}

static VALUE hc_ary_odd_indices(VALUE self)
{
  VALUE ary = rb_ary_new();
  long max = RARRAY_LEN(self);
  for (long n = 0; n < max; n += 2)
    rb_ary_push(ary, rb_ary_entry(self, n));
  return ary;
}

void init_array_ext()
{
  rb_define_attr(rb_cArray, "max_items", 1, 1);
  rb_define_attr(rb_cArray, "min_items", 1, 1);
  rb_define_method(rb_cArray, "items_min_max", RMF(hc_ary_items_min_max), 2);
  rb_define_method(rb_cArray, "push_upto_max", RMF(hc_ary_push_upto_max), 1);
  rb_define_method(rb_cArray, "unshift_upto_max", RMF(hc_ary_unshift_upto_max), 1);
  rb_define_method(rb_cArray, "pop_upto_min", RMF(hc_ary_pop_upto_min), 0);
  rb_define_method(rb_cArray, "shift_upto_min", RMF(hc_ary_shift_upto_min), 0);
  rb_define_method(rb_cArray, "clear_upto_min", RMF(hc_ary_clear_upto_min), 0);
  rb_define_method(rb_cArray, "push_uniq", RMF(hc_ary_push_uniq), 1);
  rb_define_method(rb_cArray, "unshift_uniq", RMF(hc_ary_unshift_uniq), 1);
  rb_define_method(rb_cArray, "even_indices", RMF(hc_ary_even_indices), 0);
  rb_define_method(rb_cArray, "odd_indices", RMF(hc_ary_odd_indices), 0);
  rb_define_method(rb_cArray, "even_indexes", RMF(hc_ary_even_indices), 0);
  rb_define_method(rb_cArray, "odd_indexes", RMF(hc_ary_odd_indices), 0);
}
