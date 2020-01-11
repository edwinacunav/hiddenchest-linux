/*
** scripts.cpp
**
** This file is part of HiddenChest
**
** Copyright (C) 2018-2019 Kyonides-Arkanthes
*/

#include "binding-util.h"
#include "sharedstate.h"
#include "eventthread.h"
#include "hcextras.h"
#include "scripts.h"

#define CRMF(func) ((int (*)(ANYARGS))(func))

static VALUE hidden;

static int get_keys(VALUE key, VALUE value, VALUE ary)
{
  rb_ary_push(ary, key);
  return 0;
}

static VALUE rb_hash_keys(VALUE hash)
{
  VALUE keys = rb_ary_new();
  rb_hash_foreach(hash, CRMF(get_keys), keys);
  return keys;
}

static VALUE scripts_all(VALUE self)
{
  return rb_iv_get(self, "@pack");
}

static VALUE scripts_names(VALUE self)
{
  return rb_hash_keys(rb_iv_get(self, "@pack"));
}

static VALUE scripts_dependencies(VALUE self)
{
  VALUE pack, keys, result;
  pack = rb_iv_get(self, "@pack");
  keys = rb_hash_keys(pack);
  result = rb_ary_new();
  int max = RARRAY_LEN(keys);
  for (int i = 0; i < max; i++)
    rb_ary_push(result, rb_hash_aref(pack, rb_ary_entry(keys, i)));
  return result;
}

static VALUE scripts_is_included(VALUE self, VALUE name)
{
  VALUE keys = rb_hash_keys(rb_iv_get(self, "@pack"));
  return rb_ary_includes(keys, name);
}

static VALUE scripts_get(VALUE self, VALUE name)
{
  return rb_hash_aref(rb_iv_get(self, "@pack"), name);
}

static VALUE scripts_set(VALUE self, VALUE name, VALUE dependencies)
{
  return rb_hash_aset(rb_iv_get(self, "@pack"), name, dependencies);
}

static VALUE scripts_scene_get(VALUE self)
{
  return rb_iv_get(self, "@scene");
}

static VALUE scripts_scene_set(VALUE self, VALUE name)
{
  return rb_iv_set(self, "@scene", name);
}

static VALUE scripts_main_index_get(VALUE self)
{
  return rb_iv_get(self, "@main_index");
}

static VALUE scripts_main_section_name_get(VALUE self)
{
  return rb_iv_get(self, "@main_section_name");
}

static VALUE scripts_main_section_name_set(VALUE self, VALUE name)
{
  return rb_iv_set(self, "@main_section_name", name);
}

void scripts_open_log(VALUE mod, VALUE klass, VALUE msg, VALUE backtrace)
{
  rb_iv_set(mod, "@show_backdrop", Qtrue);
  rb_iv_set(mod, "@error_type", klass);
  rb_iv_set(mod, "@error_msg", msg);
  int max = RARRAY_LEN(backtrace);
  VALUE line, file = rb_file_open("error.log", "w");
  VALUE bt[max + 2];
  bt[0] = rb_str_plus(rb_str("Error Type: "), klass);
  bt[1] = rb_str_plus(msg, rb_str("\n"));
  rb_io_write(file, bt[0]);
  rb_io_write(file, rb_str("\n"));
  rb_io_write(file, bt[1]);
  for (int n = 0; n < max - 1; n++) {
    line = rb_ary_entry(backtrace, n);
    bt[n + 2] = line;
    rb_io_write(file, line);
    rb_io_write(file, rb_str("\n"));
  }
  rb_io_close(file);
  rb_io_puts(max, bt, rb_stdout);
}

void scripts_error_handling()
{
  int state;
  rb_eval_string_protect(main_hc, &state);
  VALUE rb_error = rb_gv_get("$!");
  if (!state || rb_obj_class(rb_error) != getRbData()->exc[Reset]) return;
  shState->rtData().rqReset.clear();
}

static VALUE scripts_log(VALUE self)
{
  VALUE exception = rb_gv_get("$!");
  VALUE klass = rb_obj_as_string(rb_obj_class(exception));
  VALUE message = rb_funcall(exception, rb_intern("message"), 0);
  VALUE backtrace = rb_funcall(exception, rb_intern("backtrace"), 0);
  scripts_open_log(hidden, klass, message, backtrace);
  scripts_error_handling();
  return Qnil;
}

void init_scripts()
{
  hidden = rb_define_module("HIDDENCHEST");
  VALUE module = rb_define_module("Scripts");
  rb_iv_set(module, "@pack", rb_hash_new());
  rb_iv_set(module, "@main_section_name", rb_str_new_cstr("Main"));
  rb_iv_set(module, "@main_index", RB_INT2FIX(0));
  rb_define_module_function(module, "all", RMF(scripts_all), 0);
  rb_define_module_function(module, "names", RMF(scripts_names), 0);
  rb_define_module_function(module, "dependencies", RMF(scripts_dependencies), 0);
  rb_define_module_function(module, "include?", RMF(scripts_is_included), 1);
  rb_define_module_function(module, "[]", RMF(scripts_get), 1);
  rb_define_module_function(module, "[]=", RMF(scripts_set), 2);
  rb_define_module_function(module, "main_index", RMF(scripts_main_index_get), 0);
  rb_define_module_function(module, "main_index=", RMF(scripts_main_index_set), 1);
  rb_define_module_function(module, "main_section_name", RMF(scripts_main_section_name_get), 0);
  rb_define_module_function(module, "main_section_name=", RMF(scripts_main_section_name_set), 1);
  rb_define_module_function(module, "scene", RMF(scripts_scene_get), 0);
  rb_define_module_function(module, "scene=", RMF(scripts_scene_set), 1);
  rb_define_module_function(module, "log", RMF(scripts_log), 0);
}
