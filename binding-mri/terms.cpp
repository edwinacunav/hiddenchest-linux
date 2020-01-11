/*
** terms.cpp
**
** This file is part of HiddenChest
**
** Copyright (C) 2018-2019 Kyonides-Arkanthes
*/

#include "hcextras.h"

static VALUE terms_critical_hit_get(VALUE self)
{
  return rb_iv_get(self, "@critical_hit");
}

static VALUE terms_critical_hit_set(VALUE self, VALUE str)
{
  return rb_iv_set(self, "@critical_hit", str);
}

void init_terms()
{
  VALUE terms = rb_define_module("Terms");
  rb_iv_set(terms, "@critical_hit", rb_str_new_cstr("CRITICAL"));
  rb_define_module_function(terms, "critical_hit", RMF(terms_critical_hit_get), 0);
  rb_define_module_function(terms, "critical_hit=", RMF(terms_critical_hit_set), 1);
}
