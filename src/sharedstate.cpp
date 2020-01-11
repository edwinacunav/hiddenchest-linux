/*
** sharedstate.cpp
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

#include "sharedstate.h"
#include "util.h"
#include "filesystem.h"
#include "graphics.h"
#include "input.h"
#include "audio.h"
#include "glstate.h"
#include "shader.h"
#include "texpool.h"
#include "font.h"
#include "eventthread.h"
#include "gl-util.h"
#include "global-ibo.h"
#include "quad.h"
#include "exception.h"
#include "sharedmidistate.h"
#include <unistd.h>
#include <stdio.h>

SharedState *SharedState::instance = 0;
int SharedState::rgssVersion = 0;
static GlobalIBO *_globalIBO = 0;

static const char *gameArchExt()
{
  if (rgssVer == 0)
    return ".rhc";
  if (rgssVer == 1)
    return ".rgssad";
  else if (rgssVer == 2)
    return ".rgss2a";
  else if (rgssVer == 3)
    return ".rgss3a";
  assert(!"unreachable");
  return 0;
}

struct SharedStatePrivate
{
  void *bindingData;
  SDL_Window *sdlWindow;
  Scene *screen;
  FileSystem fileSystem;
  EventThread &eThread;
  RGSSThreadData &rtData;
  Config &config;
  SharedMidiState midiState;
  Graphics graphics;
  Input input;
  Audio audio;
  GLState _glState;
  ShaderSet shaders;
  TexPool texPool;
  SharedFontState fontState;
  Font *defaultFont;
  TEX::ID globalTex;
  int globalTexW, globalTexH;
  bool globalTexDirty;
  TEXFBO gpTexFBO;
  TEXFBO atlasTex;
  Quad gpQuad;
  unsigned int stampCounter;
  SharedStatePrivate(RGSSThreadData *threadData)
  : bindingData(0),
    sdlWindow(threadData->window),
    fileSystem(threadData->argv0, threadData->config.allowSymlinks),
    eThread(*threadData->ethread),
    rtData(*threadData),
    config(threadData->config),
    midiState(threadData->config),
    graphics(threadData),
    input(*threadData),
    audio(*threadData),
    _glState(threadData->config),
    fontState(threadData->config),
    stampCounter(0)
  { // Shaders have been compiled in ShaderSet's constructor
    if (gl.ReleaseShaderCompiler)
      gl.ReleaseShaderCompiler();
    std::string archPath = config.execName + gameArchExt();
    // Check if a game archive exists
    FILE *tmp = fopen(archPath.c_str(), "rb");
    if (tmp) {
      fileSystem.addPath(archPath.c_str());
      fclose(tmp);
    }
    fileSystem.addPath(".");
    for (size_t i = 0; i < config.rtps.size(); ++i)
      fileSystem.addPath(config.rtps[i].c_str());
    if (config.pathCache)
      fileSystem.createPathCache();
    fileSystem.initFontSets(fontState);
    globalTexW = 128;
    globalTexH = 64;
    globalTex = TEX::gen();
    TEX::bind(globalTex);
    TEX::setRepeat(false);
    TEX::setSmooth(false);
    TEX::allocEmpty(globalTexW, globalTexH);
    globalTexDirty = false;
    TEXFBO::init(gpTexFBO);
    // Reuse starting values
    TEXFBO::allocEmpty(gpTexFBO, globalTexW, globalTexH);
    TEXFBO::linkFBO(gpTexFBO);
    // RGSS3 games calls setup_midi, so there's no need to do it on startup
    if (rgssVer < 3)
      midiState.initIfNeeded(threadData->config);
  }

  ~SharedStatePrivate()
  {
    TEX::del(globalTex);
    TEXFBO::fini(gpTexFBO);
    TEXFBO::fini(atlasTex);
  }
};

void SharedState::initInstance(RGSSThreadData *threadData)
{
  /* This section is tricky because of dependencies:
   * SharedState depends on GlobalIBO existing,
   * Font depends on SharedState existing */
  rgssVersion = threadData->config.rgssVersion;
  _globalIBO = new GlobalIBO();
  _globalIBO->ensureSize(1);
  SharedState::instance = 0;
  Font *defaultFont = 0;
  try {
    SharedState::instance = new SharedState(threadData);
    Font::initDefaults(instance->p->fontState);
    defaultFont = new Font();
  } catch (const Exception &exc) {
    delete _globalIBO;
    delete SharedState::instance;
    delete defaultFont;
    throw exc;
  }
  SharedState::instance->p->defaultFont = defaultFont;
}

void SharedState::finiInstance()
{
  delete SharedState::instance->p->defaultFont;
  delete SharedState::instance;
  delete _globalIBO;
}

void SharedState::setScreen(Scene &screen)
{
  p->screen = &screen;
}

void* SharedState::bindingData() const
{
  return p->bindingData;
}

SDL_Window* SharedState::sdlWindow() const
{
  return p->sdlWindow;
}

Scene* SharedState::screen() const
{
  return p->screen;
}

FileSystem& SharedState::fileSystem() const
{
  return p->fileSystem;
}

EventThread& SharedState::eThread() const
{
  return p->eThread;
}

RGSSThreadData& SharedState::rtData() const
{
  return p->rtData;
}

Config& SharedState::config() const
{
  return p->config;
}

Graphics& SharedState::graphics() const
{
  return p->graphics;
}

Input& SharedState::input() const
{
  return p->input;
}

Audio& SharedState::audio() const
{
  return p->audio;
}

GLState& SharedState::_glState() const
{
  return p->_glState;
}

ShaderSet& SharedState::shaders() const
{
  return p->shaders;
}

TexPool& SharedState::texPool() const
{
  return p->texPool;
}

Quad& SharedState::gpQuad() const
{
  return p->gpQuad;
}

SharedFontState& SharedState::fontState() const
{
  return p->fontState;
}

SharedMidiState&SharedState::midiState() const
{
  return p->midiState;
}

void SharedState::setBindingData(void *data)
{
  p->bindingData = data;
}

void SharedState::ensureQuadIBO(size_t minSize)
{
  _globalIBO->ensureSize(minSize);
}

GlobalIBO &SharedState::globalIBO()
{
  return *_globalIBO;
}

void SharedState::bindTex()
{
  TEX::bind(p->globalTex);
  if (p->globalTexDirty) {
    TEX::allocEmpty(p->globalTexW, p->globalTexH);
    p->globalTexDirty = false;
  }
}

void SharedState::ensureTexSize(int minW, int minH, Vec2i &currentSizeOut)
{
  if (minW > p->globalTexW) {
    p->globalTexDirty = true;
    p->globalTexW = findNextPow2(minW);
  }
  if (minH > p->globalTexH) {
    p->globalTexDirty = true;
    p->globalTexH = findNextPow2(minH);
  }
  currentSizeOut = Vec2i(p->globalTexW, p->globalTexH);
}

TEXFBO &SharedState::gpTexFBO(int minW, int minH)
{
  bool needResize = false;
  if (minW > p->gpTexFBO.width) {
    p->gpTexFBO.width = findNextPow2(minW);
    needResize = true;
  }
  if (minH > p->gpTexFBO.height) {
    p->gpTexFBO.height = findNextPow2(minH);
    needResize = true;
  }
  if (needResize) {
    TEX::bind(p->gpTexFBO.tex);
    TEX::allocEmpty(p->gpTexFBO.width, p->gpTexFBO.height);
  }
  return p->gpTexFBO;
}

void SharedState::requestAtlasTex(int w, int h, TEXFBO &out)
{
  TEXFBO tex;
  if (w == p->atlasTex.width && h == p->atlasTex.height) {
    tex = p->atlasTex;
    p->atlasTex = TEXFBO();
  } else {
    TEXFBO::init(tex);
    TEXFBO::allocEmpty(tex, w, h);
    TEXFBO::linkFBO(tex);
  }
  out = tex;
}

void SharedState::releaseAtlasTex(TEXFBO &tex)
{ // No point in caching an invalid object
  if (tex.tex == TEX::ID(0)) return;
  TEXFBO::fini(p->atlasTex);
  p->atlasTex = tex;
}

void SharedState::checkShutdown()
{
  if (!p->rtData.rqTerm) return;
  p->rtData.rqTermAck.set();
  p->texPool.disable();
  scriptBinding->terminate();
}

void SharedState::checkReset()
{
  if (!p->rtData.rqReset) return;
  p->rtData.rqReset.clear();
  scriptBinding->reset();
}

Font &SharedState::defaultFont() const
{
  return *p->defaultFont;
}

unsigned int SharedState::genTimeStamp()
{
  return p->stampCounter++;
}

SharedState::SharedState(RGSSThreadData *threadData)
{
  p = new SharedStatePrivate(threadData);
  p->screen = p->graphics.getScreen();
}

SharedState::~SharedState()
{
  delete p;
}
