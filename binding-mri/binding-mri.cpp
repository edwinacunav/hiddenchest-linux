/*
** binding-mri.cpp
**
** This file is part of HiddenChest and mkxp.
**
** Copyright (C) 2013 Jonas Kulla <Nyocurio@gmail.com>
** 2019 Extended by Kyonides Arkanthes <kyonides@gmail.com>
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

#include "author.h"
#include "binding-util.h"
#include "sharedstate.h"
#include "eventthread.h"
#include "filesystem.h"
#include "util.h"
#include "sdl-util.h"
#include "debugwriter.h"
#include "graphics.h"
#include "audio.h"
#include "boost-hash.h"
#include "hcextras.h"
#include <ruby/encoding.h>
#include <ruby/io.h>
#include <assert.h>
#include <string>
#include <zlib.h>
#include <SDL_filesystem.h>
#include "scripts.h"

extern const char array[];
extern const char rpg_cache[];
extern const char rpg_sprite[];
extern const char rpg_weather[];
extern const char module_rpg1[];
extern const char module_rpg2[];
extern const char module_rpg3[];
void init_module();
void init_point();
void init_scripts();
void init_terms();
void init_settings();
void init_backdrop();
void init_array_ext();
extern void scripts_open_log(VALUE mod, VALUE klass, VALUE msg, VALUE backtrace);
extern void scripts_error_handling();
const char *os_kind = OS_STRING;
static VALUE hidden, script_ary;

ScriptBinding scriptBindingImpl =
{
  mriBindingExecute,
  mriBindingTerminate,
  mriBindingReset
};

ScriptBinding *scriptBinding = &scriptBindingImpl;

void tableBindingInit();
void etcBindingInit();
void fontBindingInit();
void bitmapBindingInit();
void SpriteBindingInit();
void MsgBoxSpriteBindingInit();
void viewportBindingInit();
void planeBindingInit();
void windowBindingInit();
void tilemapBindingInit();
void windowVXBindingInit();
void tilemapVXBindingInit();
void inputBindingInit();
void audioBindingInit();
void graphicsBindingInit();
void fileIntBindingInit();
void audio_setup_custom_se();

RB_METHOD(mriPrint);
RB_METHOD(mriP);
RB_METHOD(HCDataDirectory);
RB_METHOD(HCRawKeyStates);
static VALUE HCMouseInWindow(VALUE self);
RB_METHOD(mriRgssMain);
RB_METHOD(mriRgssStop);
RB_METHOD(_kernelCaller);

static void hc_c_splash(const char *error)
{
  rb_iv_set(hidden, "@error_msg", rb_str(error));
  scripts_error_handling();
}

static void hc_rb_splash(VALUE exception)
{
  VALUE klass = rb_obj_as_string(rb_obj_class(exception));
  VALUE message = rb_funcall(exception, rb_intern("message"), 0);
  VALUE backtrace = rb_funcall(exception, rb_intern("backtrace"), 0);
  scripts_open_log(hidden, klass, message, backtrace);
  scripts_error_handling();
}

static void init_hiddenchest()
{
  hidden = rb_define_module("HIDDENCHEST");
  rb_define_const(hidden, "LOGO", rb_str("app_logo"));
  rb_define_const(hidden, "AUTHOR", rb_str(HIDDENAUTHOR));
  rb_define_const(hidden, "VERSION", rb_str(HIDDENVERSION));
  rb_define_const(hidden, "RELEASE_DATE", rb_str(HIDDENDATE));
  rb_define_const(hidden, "DESCRIPTION",
    rb_str("An RGSS based engine derived from mkxp developed by Ancurio"));
  rb_define_module_function(hidden, "data_directory", RUBY_METHOD_FUNC(HCDataDirectory), -1);
  rb_define_module_function(hidden, "raw_key_states", RUBY_METHOD_FUNC(HCRawKeyStates), -1);
  rb_define_module_function(hidden, "mouse_in_window", RUBY_METHOD_FUNC(HCMouseInWindow), 0);
  rb_funcall(hidden, rb_intern("module_accessor"), 3,
    hc_sym("error_type"), hc_sym("error_msg"), hc_sym("show_backdrop"));
  rb_iv_set(hidden, "@error_type", rb_str(""));
  rb_iv_set(hidden, "@error_msg", rb_str(""));
  rb_eval_string(scene_hc);
}

static void mriBindingInit()
{
  VALUE os = rb_define_module("OS");
  rb_define_const(os, "NAME", rb_str(OS_REAL_STRING));
  rb_define_const(os, "FAMILY_NAME", rb_str(OS_STRING));
  tableBindingInit();
  etcBindingInit();
  fontBindingInit();
  bitmapBindingInit();
  SpriteBindingInit();
  MsgBoxSpriteBindingInit();
  viewportBindingInit();
  planeBindingInit();
  if (rgssVer < 2) {
    windowBindingInit();
    tilemapBindingInit();
  } else {
    windowVXBindingInit();
    tilemapVXBindingInit();
  }
  inputBindingInit();
  audioBindingInit();
  graphicsBindingInit();
  fileIntBindingInit();
  init_array_ext();
  rb_eval_string(array);
  init_module();
  init_hiddenchest();
  init_point();
  init_scripts();
  init_terms();
  init_settings();
  init_backdrop();
  rb_define_module_function(rb_mKernel, "msgbox", RUBY_METHOD_FUNC(mriPrint), -1);
  rb_define_module_function(rb_mKernel, "msgbox_p", RUBY_METHOD_FUNC(mriP), -1);
  rb_define_module_function(rb_mKernel, "print", RUBY_METHOD_FUNC(mriPrint), -1);
  rb_define_module_function(rb_mKernel, "p", RUBY_METHOD_FUNC(mriP), -1);
  if (rgssVer == 3) {
    rb_define_module_function(rb_mKernel, "rgss_main", RUBY_METHOD_FUNC(mriRgssMain), -1);
    rb_define_module_function(rb_mKernel, "rgss_stop", RUBY_METHOD_FUNC(mriRgssStop), -1);
    rb_define_global_const("RGSS_VERSION", rb_str("3.0.1"));
  } else {
    rb_define_alias(rb_singleton_class(rb_mKernel), "_HC_kernel_caller_alias", "caller");
    rb_define_module_function(rb_mKernel, "caller", RUBY_METHOD_FUNC(_kernelCaller), -1);
  }
  if (rgssVer == 1) {
    rb_eval_string(rpg_cache);
    rb_eval_string(rpg_sprite);
    rb_eval_string(rpg_weather);
    rb_eval_string(module_rpg1);
    audio_setup_custom_se();
  } else if (rgssVer == 2) {
    rb_eval_string(module_rpg2);
  } else if (rgssVer == 3) {
    rb_eval_string(module_rpg3);
  } else {
    Dbg() << "Loading HiddenChest Module now";
  }
  // Load global constants
  rb_gv_set("HIDDENCHEST", Qtrue);
  VALUE debug = shState->config().editor.debug ? Qtrue : Qfalse;
  if (rgssVer == 1) {
    rb_gv_set("DEBUG", debug);
  } else if (rgssVer > 1) {
    rb_gv_set("TEST", debug);
  }
  rb_gv_set("BTEST", shState->config().editor.battleTest ? Qtrue : Qfalse);
  VALUE game = rb_define_module("Game");
  const char* title = shState->config().game.title.c_str();
  const char* version = shState->config().game.version.c_str();
  rb_define_const(game, "TITLE", rb_str(title));
  rb_define_const(game, "VERSION", rb_str(version));
}

static void showMsg(const std::string &msg)
{
  shState->eThread().showMessageBox(msg.c_str());
}

static void printP(int argc, VALUE *argv, const char *convMethod, const char *sep)
{
  VALUE dispString = rb_str_buf_new(128);
  ID conv = rb_intern(convMethod);
  for (int i = 0; i < argc; ++i) {
    VALUE str = rb_funcall2(argv[i], conv, 0, NULL);
    rb_str_buf_append(dispString, str);
    if (i < argc) rb_str_buf_cat2(dispString, sep);
  }
  showMsg(RSTRING_PTR(dispString));
}

static VALUE mriPrint(int argc, VALUE* argv, VALUE self)
{
  printP(argc, argv, "to_s", "");
  return Qnil;
}

static VALUE mriP(int argc, VALUE* argv, VALUE self)
{
  printP(argc, argv, "inspect", "\n");
  return Qnil;
}

static VALUE HCDataDirectory(int argc, VALUE* argv, VALUE self)
{
  const std::string &path = shState->config().customDataPath;
  const char *s = path.empty() ? "." : path.c_str();
  return rb_str(s);
}

static VALUE HCRawKeyStates(int argc, VALUE* argv, VALUE self)
{
  VALUE str = rb_str_new(0, sizeof(EventThread::keyStates));
  memcpy(RSTRING_PTR(str), EventThread::keyStates, sizeof(EventThread::keyStates));
  return str;
}

static VALUE HCMouseInWindow(VALUE self)
{
  return EventThread::mouseState.inWindow ? Qtrue : Qfalse;
}

static VALUE rgssMainCb(VALUE block)
{
  rb_funcall2(block, rb_intern("call"), 0, 0);
  return Qnil;
}

static VALUE rgssMainRescue(VALUE arg, VALUE exc)
{
  VALUE *excRet = (VALUE*) arg;
  *excRet = exc;
  return Qnil;
}

static VALUE newStringUTF8(const char *string, long length)
{
  return rb_enc_str_new(string, length, rb_utf8_encoding());
}

struct evalArg
{
  VALUE string;
  VALUE filename;
};

static VALUE evalHelper(evalArg *arg)
{
  VALUE argv[] = { arg->string, Qnil, arg->filename };
  return rb_funcall2(Qnil, rb_intern("eval"), ARRAY_SIZE(argv), argv);
}

static VALUE evalString(VALUE string, VALUE filename, int *state)
{
  evalArg arg = { string, filename };
  return rb_protect((VALUE (*)(VALUE))evalHelper, (VALUE)&arg, state);
}

void process_main_script_reset()
{// Find Main Script Index and Execute Main
  shState->rtData().rqReset.clear();
  VALUE mod = rb_const_get(rb_cObject, rb_intern("Scripts"));
  int state, index = RB_FIX2INT(rb_iv_get(mod, "@main_index"));
  Dbg() << "Reloading Main Script";
  VALUE script_section = rb_ary_entry(script_ary, index);
  VALUE fname = rb_ary_entry(script_section, 1);
  VALUE script = rb_ary_entry(script_section, 3);
  shState->graphics().repaintWait(shState->rtData().rqResetFinish, false);
  VALUE string = newStringUTF8(RSTRING_PTR(script), RSTRING_LEN(script));
  evalString(string, fname, &state);
}

RB_METHOD(mriRgssMain)
{
  RB_UNUSED_PARAM;
  while (true) {
    VALUE exc = Qnil;
    rb_rescue2((VALUE(*)(ANYARGS)) rgssMainCb, rb_block_proc(),
               (VALUE(*)(ANYARGS)) rgssMainRescue, (VALUE) &exc,
               rb_eException, (VALUE) 0);
    if (exc == Qnil) break;
    if (rb_obj_class(exc) == getRbData()->exc[Reset])
      process_main_script_reset();
    else
      rb_exc_raise(exc);
  }
  return Qnil;
}

RB_METHOD(mriRgssStop)
{
  RB_UNUSED_PARAM;
  while (true)
    shState->graphics().update();
  return Qnil;
}

RB_METHOD(_kernelCaller)
{
  RB_UNUSED_PARAM;
  VALUE trace = rb_funcall2(rb_mKernel, rb_intern("_HC_kernel_caller_alias"), 0, 0);
  if (!RB_TYPE_P(trace, RUBY_T_ARRAY)) return trace;
  long len = RARRAY_LEN(trace);
  if (len < 2) return trace;
  /* Remove useless "ruby:1:in 'eval'" */
  rb_ary_pop(trace);
  // Also remove trace of this helper function
  rb_ary_shift(trace);
  len -= 2;
  if (len == 0) return trace;
  // RMXP does this, not sure if specific or 1.8 related
  VALUE args[] = { rb_str(":in `<main>'"), rb_str("") };
  rb_funcall2(rb_ary_entry(trace, len-1), rb_intern("gsub!"), 2, args);
  return trace;
}

static void runCustomScript(const std::string &filename)
{
  std::string scriptData;
  if (!readFileSDL(filename.c_str(), scriptData)) {
    showMsg(std::string("Unable to open '") + filename + "'");
    return;
  }
  evalString(newStringUTF8(scriptData.c_str(), scriptData.size()),
             newStringUTF8(filename.c_str(), filename.size()), NULL);
}

VALUE kernelLoadDataInt(const char *filename, bool rubyExc);

struct BacktraceData
{ // Maps: Ruby visible filename, To: Actual script name
  BoostHash<std::string, std::string> scriptNames;
};

#define SCRIPT_SECTION_FMT (rgssVer >= 3 ? "{%04ld}" : "Section%03ld")

static void runRMXPScripts(BacktraceData &btData)
{
  if (rgssVer == 0) {
    const char *error = "No game file was found!";
    Dbg() << "LoadError:" << error;
    rb_iv_set(hidden, "@error_type", rb_str("LoadError"));
    hc_c_splash(error);
    return;
  }
  const Config &conf = shState->rtData().config;
  const std::string &scriptPack = conf.game.scripts;
  if (scriptPack.empty()) {
    const char *error = "No game scripts specified (missing Game.ini?)";
    Dbg() << error;
    rb_iv_set(hidden, "@error_type", rb_str("Errno::NOSETUP"));
    hc_c_splash(error);
    return;
  }
  if (!shState->fileSystem().exists(scriptPack.c_str())) {
    std::string error = "Unable to open '";
    error += scriptPack + std::string("'");
    Dbg() << "Errno::ENOENT" << error;
    rb_iv_set(hidden, "@error_type", rb_str("Errno::ENOENT"));
    hc_c_splash(error.c_str());
    return;
  }
  // We checked if Scripts.rxdata exists, but something might still go wrong
  try {
    script_ary = kernelLoadDataInt(scriptPack.c_str(), false);
  } catch (const Exception &e) {
    std::string error = std::string("Failed to read script data: ") + e.msg;
    Dbg() << error;
    rb_iv_set(hidden, "@error_type", rb_str("IOError"));
    hc_c_splash(error.c_str());
    return;
  }
  if (!RB_TYPE_P(script_ary, RUBY_T_ARRAY)) {
    const char *error = "Failed to read script data";
    Dbg() << error;
    rb_iv_set(hidden, "@error_type", rb_str("IOError"));
    hc_c_splash(error);
    return;
  }
  rb_gv_set("$RGSS_SCRIPTS", script_ary);
  VALUE scripts_mod = rb_define_module("Scripts");
  scripts_main_index_set(scripts_mod, find_main_script_index(scripts_mod));
  Dbg() << "Loading Scripts now";
  long scriptCount = RARRAY_LEN(script_ary);
  std::string decodeBuffer;
  decodeBuffer.resize(0x8000);
  for (long i = 0; i < scriptCount; ++i) {
    VALUE script = rb_ary_entry(script_ary, i);
    if (!RB_TYPE_P(script, RUBY_T_ARRAY)) continue;
    VALUE scriptName   = rb_ary_entry(script, 1);
    VALUE scriptString = rb_ary_entry(script, 2);
    int result = Z_OK;
    unsigned long bufferLen;
    while (true) {
      unsigned char *bufferPtr =
              reinterpret_cast<unsigned char*>(const_cast<char*>(decodeBuffer.c_str()));
      const unsigned char *sourcePtr =
              reinterpret_cast<const unsigned char*>(RSTRING_PTR(scriptString));
      bufferLen = decodeBuffer.length();
      result = uncompress(bufferPtr, &bufferLen, sourcePtr, RSTRING_LEN(scriptString));
      bufferPtr[bufferLen] = '\0';
      if (result != Z_BUF_ERROR) break;
      decodeBuffer.resize(decodeBuffer.size()*2);
    }
    if (result != Z_OK) {
      static char buffer[256];
      snprintf(buffer, sizeof(buffer), "Error decoding script %ld: '%s'",
                i, RSTRING_PTR(scriptName));
      showMsg(buffer);
      break;
    }
    rb_ary_store(script, 3, rb_str(decodeBuffer.c_str()));
  } // Execute preloaded scripts
  for (std::set<std::string>::iterator i = conf.preloadScripts.begin();
      i != conf.preloadScripts.end(); ++i)
    runCustomScript(*i);
  VALUE exc = rb_gv_get("$!");
  if (exc != Qnil) return;
  int script_pos = 3, name_pos = 1;
  for (long i = 0; i < scriptCount; ++i) {
    VALUE section = rb_ary_entry(script_ary, i);
    VALUE script = rb_ary_entry(section, script_pos);
    VALUE string = newStringUTF8(RSTRING_PTR(script), RSTRING_LEN(script));
    VALUE fname;
    const char *scriptName = RSTRING_PTR(rb_ary_entry(section, name_pos));
    char buf[512];
    int len;
    if (conf.useScriptNames)
      len = snprintf(buf, sizeof(buf), "%03ld:%s", i, scriptName);
    else
      len = snprintf(buf, sizeof(buf), SCRIPT_SECTION_FMT, i);
    fname = newStringUTF8(buf, len);
    btData.scriptNames.insert(buf, scriptName);
    int state;
    evalString(string, fname, &state);
    if (state) break;
  }
  if (rb_obj_class(rb_gv_get("$!")) != getRbData()->exc[Reset]) return;
  while (true) {
    if (rb_gv_get("$scene") == Qnil) break;
    if (rb_obj_class(rb_gv_get("$!")) != getRbData()->exc[Reset]) break;
    process_main_script_reset();
  }
  shState->rtData().rqReset.clear();
}

static void showExc(VALUE exc, const BacktraceData &btData)
{
  VALUE bt = rb_funcall2(exc, rb_intern("backtrace"), 0, NULL);
  VALUE msg = rb_funcall2(exc, rb_intern("message"), 0, NULL);
  VALUE bt0 = rb_ary_entry(bt, 0);
  VALUE name = rb_class_path(rb_obj_class(exc));
  VALUE ds = rb_sprintf("%" PRIsVALUE ": %" PRIsVALUE " (%" PRIsVALUE ")",
                        bt0, exc, name);
  /* omit "useless" last entry (from ruby:1:in `eval') */
  for (long i = 1, btlen = RARRAY_LEN(bt) - 1; i < btlen; ++i)
    rb_str_catf(ds, "\n\tfrom %" PRIsVALUE, rb_ary_entry(bt, i));
  Dbg() << StringValueCStr(ds);
  char *s = RSTRING_PTR(bt0);
  char line[16];
  std::string file(512, '\0');
  char *p = s + strlen(s);
  char *e;
  while (p != s)
    if (*--p == ':') break;
  e = p;
  while (p != s)
    if (*--p == ':') break;
  /* s         p  e
   * SectionXXX:YY: in 'blabla' */
  *e = '\0';
  strncpy(line, *p ? p+1 : p, sizeof(line));
  line[sizeof(line)-1] = '\0';
  *e = ':';
  e = p;
  /* s         e
   * SectionXXX:YY: in 'blabla' */
  *e = '\0';
  strncpy(&file[0], s, file.size());
  *e = ':';
  /* Shrink to fit */
  file.resize(strlen(file.c_str()));
  file = btData.scriptNames.value(file, file);
  std::string ms(640, '\0');
  snprintf(&ms[0], ms.size(), "Script '%s' line %s: %s occured.\n\n%s",
           file.c_str(), line, RSTRING_PTR(name), RSTRING_PTR(msg));
  showMsg(ms);
}

void mriBindingExecute()
{/* Normally only a ruby executable would do a sysinit,
 * but not doing it will lead to crashes due to closed
 * stdio streams on some platforms (eg. Windows) */
  int argc = 0;
  char **argv = 0;
  ruby_sysinit(&argc, &argv);
  ruby_setup();
  rb_enc_set_default_external(rb_enc_from_encoding(rb_utf8_encoding()));
  VALUE rversion = rb_const_get(rb_cObject, rb_intern("RUBY_VERSION"));
  const char* version = StringValueCStr(rversion);
  Dbg() << "Ruby Version:" << version;
  Config &conf = shState->rtData().config;
  if (!conf.rubyLoadpaths.empty()) {
    // Setup custom load paths
    VALUE lpaths = rb_gv_get(":");
    for (size_t i = 0; i < conf.rubyLoadpaths.size(); ++i) {
      std::string &path = conf.rubyLoadpaths[i];
      VALUE pathv = rb_str_new(path.c_str(), path.size());
      rb_ary_push(lpaths, pathv);
    }
  }
  RbData rbData;
  shState->setBindingData(&rbData);
  BacktraceData btData;
  mriBindingInit();
  std::string &customScript = conf.customScript;
  if (!customScript.empty())
    runCustomScript(customScript);
  else
    runRMXPScripts(btData);
  VALUE exc = rb_errinfo();
  if (rb_obj_class(exc) == getRbData()->exc[Reset]) {
    exc = Qnil;
    Dbg() << "Game Reset";
  }
  if (!RB_NIL_P(exc) && !rb_obj_is_kind_of(exc, rb_eSystemExit))
    hc_rb_splash(exc);
  ruby_cleanup(0);
  shState->rtData().rqTermAck.set();
}

void mriBindingTerminate()
{
  rb_raise(rb_eSystemExit, " ");
}

void mriBindingReset()
{
  rb_raise(getRbData()->exc[Reset], " ");
}
