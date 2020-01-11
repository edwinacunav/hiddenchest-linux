/*
** settings.cpp
**
** This file is part of HiddenChest
**
** Copyright (C) 2018-2019 Kyonides-Arkanthes
*/

#include "hcextras.h"
#include "graphics.h"
#include "sharedstate.h"

static VALUE settings_image_format_get(VALUE self)
{
  return rb_iv_get(self, "@image_format");
}

static VALUE settings_image_format_set(VALUE self, VALUE fmt)
{
  int form;
  if (fmt == hc_sym("jpg") || fmt == RB_INT2FIX(0)) form = 0;
  if (fmt == hc_sym("png") || fmt == RB_INT2FIX(1)) form = 1;
  shState->graphics().set_screenshot_format(form);
  return rb_iv_set(self, "@image_format", fmt);
}

static VALUE settings_snapshot_dir_get(VALUE self)
{
  return rb_iv_get(self, "@snapshot_dir");
}

static VALUE settings_snapshot_dir_set(VALUE self, VALUE sdir)
{
  char* dir = StringValueCStr(sdir);
  shState->graphics().set_screenshot_dir(dir);
  return rb_iv_set(self, "@snapshot_dir", sdir);
}

static VALUE settings_snapshot_filename_get(VALUE self)
{
  return rb_iv_get(self, "@snapshot_filename");
}

static VALUE settings_snapshot_filename_set(VALUE self, VALUE name)
{
  char* fn = StringValueCStr(name);
  shState->graphics().set_screenshot_fn(fn);
  return rb_iv_set(self, "@snapshot_filename", name);
}

static VALUE settings_save_dir_get(VALUE self)
{
  return rb_iv_get(self, "@save_dir");
}

static VALUE settings_save_dir_set(VALUE self, VALUE dir)
{
  return rb_iv_set(self, "@save_dir", dir);
}

static VALUE settings_save_filename_get(VALUE self)
{
  return rb_iv_get(self, "@save_filename");
}

static VALUE settings_save_filename_set(VALUE self, VALUE dir)
{
  return rb_iv_set(self, "@save_filename", dir);
}

static VALUE settings_auto_create_dirs(VALUE self)
{
  safe_mkdir(rb_iv_get(self, "@snapshot_dir"));
  safe_mkdir(rb_iv_get(self, "@save_dir"));
  return Qtrue;
}

void init_settings()
{
  VALUE set = rb_define_module("Settings");
  rb_iv_set(set, "@save_dir", rb_str_new_cstr("Saves"));
  rb_iv_set(set, "@save_filename", rb_str_new_cstr("Save"));
  settings_image_format_set(set, hc_sym("jpg"));
  settings_snapshot_dir_set(set, rb_str_new_cstr("Screenshots"));
  settings_snapshot_filename_set(set, rb_str_new_cstr("screenshot"));
  rb_define_module_function(set, "image_format", RMF(settings_image_format_get), 0);
  rb_define_module_function(set, "image_format=", RMF(settings_image_format_set), 1);
  rb_define_module_function(set, "snapshot_dir", RMF(settings_snapshot_dir_get), 0);
  rb_define_module_function(set, "snapshot_dir=", RMF(settings_snapshot_dir_set), 1);
  rb_define_module_function(set, "snapshot_filename", RMF(settings_snapshot_filename_get), 0);
  rb_define_module_function(set, "snapshot_filename=", RMF(settings_snapshot_filename_set), 1);
  rb_define_module_function(set, "save_dir", RMF(settings_save_dir_get), 0);
  rb_define_module_function(set, "save_dir=", RMF(settings_save_dir_set), 1);
  rb_define_module_function(set, "save_filename", RMF(settings_save_filename_get), 0);
  rb_define_module_function(set, "save_filename=", RMF(settings_save_filename_set), 1);
  rb_define_module_function(set, "auto_create_dirs", RMF(settings_auto_create_dirs), 0);
}
