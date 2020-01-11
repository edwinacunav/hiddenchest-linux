cmake_minimum_required(VERSION 2.8.12)
set(CMAKE_BUILD_TYPE Debug)#Release)
set(CMAKE_CXX_STANDARD 11)
Project(h)#iddenchest)

#set(CMAKE_EXECUTABLE_SUFFIX "_.bin")
set(BIN_RPATH "\$ORIGIN/lib")
set(LIB_PATH "lib")

set(CMAKE_SKIP_BUILD_RPATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH ${BIN_RPATH})
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

## Setup options ##

option(SHARED_FLUID "Dynamically link fluidsynth at build time" OFF)
option(WORKDIR_CURRENT "Keep current directory on startup" OFF)
set(BINDING "MRI" CACHE STRING "The Binding Type (MRI, MRUBY)")
set(EXTERNAL_LIB_PATH "" CACHE PATH "External precompiled lib prefix")

## Misc setup ##

include(cmake/PrepUtils.cmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(LINUX ON)

if (LINUX)
  set(OS_STRING "linux")
  set(OS_REAL_STRING "Ubuntu 18.04 LTS")
endif()

configure_file(src/author.h.in ../src/author.h @ONLY)

IF(WORKDIR_CURRENT)
  list(APPEND DEFINES WORKDIR_CURRENT)
ENDIF()

## Locate core libs ##

find_package(PkgConfig REQUIRED)

if (EXTERNAL_LIB_PATH)
  set(CMAKE_PREFIX_PATH ${EXTERNAL_LIB_PATH})
  if(EXISTS "${EXTERNAL_LIB_PATH}/${LIB_PATH}/pkgconfig/")
    SET(ENV{PKG_CONFIG_PATH} "${EXTERNAL_LIB_PATH}/${LIB_PATH}/pkgconfig/")
  endif()
endif()

#pkg_check_modules(FONTCONFIG   REQUIRED fontconfig)
pkg_check_modules(SIGCXX       REQUIRED sigc++-2.0)
pkg_check_modules(PIXMAN       REQUIRED pixman-1)
pkg_check_modules(PHYSFS       REQUIRED physfs>=2.1)
pkg_check_modules(VORBISFILE   REQUIRED vorbisfile)
pkg_check_modules(SDL2         REQUIRED sdl2)
pkg_check_modules(SDL2_TTF     REQUIRED SDL2_ttf)
pkg_check_modules(SDL2_IMAGE   REQUIRED SDL2_image)
pkg_check_modules(SDL_SOUND    REQUIRED SDL_sound)
pkg_check_modules(HIDDENCHEST  REQUIRED hiddenchest)

find_package(Boost 1.65 COMPONENTS program_options REQUIRED)
find_package(OpenAL REQUIRED)
find_package(ZLIB REQUIRED)

## Setup main source ##

set(MAIN_HEADERS
  src/author.h.in
  src/resolution.h
  src/quadarray.h
  src/audio.h
  src/binding.h
  src/bitmap.h
  src/disposable.h
  src/etc.h
  src/etc-internal.h
  src/eventthread.h
  src/flashable.h
  src/font.h
  src/input.h
  src/plane.h
  src/scene.h
  src/sprite.h
  src/msgboxsprite.h
  src/table.h
  src/texpool.h
  src/tilequad.h
  src/transform.h
  src/viewport.h
  src/window.h
  src/serializable.h
  src/shader.h
  src/glstate.h
  src/quad.h
  src/tilemap.h
  src/tilemap-common.h
  src/graphics.h
  src/gl-debug.h
  src/global-ibo.h
  src/exception.h
  src/filesystem.h
  src/serial-util.h
  src/intrulist.h
  src/binding.h
  src/gl-util.h
  src/util.h
  src/config.h
  src/settingsmenu.h
  src/keybindings.h
  src/tileatlas.h
  src/sharedstate.h
  src/al-util.h
  src/boost-hash.h
  src/debugwriter.h
  src/gl-fun.h
  src/gl-meta.h
  src/vertex.h
  src/soundemitter.h
  src/aldatasource.h
  src/alstream.h
  src/audiostream.h
  src/rgssad.h
  src/windowvx.h
  src/tilemapvx.h
  src/tileatlasvx.h
  src/sharedmidistate.h
  src/fluid-fun.h
  src/sdl-util.h #src/SDL_SavePNG.h
)

set(MAIN_SOURCE
  src/main.cpp
  src/audio.cpp
  src/font.cpp
  src/bitmap.cpp
  src/graphics.cpp
  src/shader.cpp
  src/filesystem.cpp
  src/eventthread.cpp
  src/input.cpp
  src/plane.cpp
  src/scene.cpp
  src/sprite.cpp
  src/msgboxsprite.cpp
  src/table.cpp
  src/tilequad.cpp
  src/viewport.cpp
  src/window.cpp
  src/texpool.cpp
  src/glstate.cpp
  src/tilemap.cpp
  src/autotiles.cpp
  src/gl-debug.cpp
  src/etc.cpp
  src/config.cpp
  src/settingsmenu.cpp
  src/keybindings.cpp
  src/tileatlas.cpp
  src/sharedstate.cpp
  src/gl-fun.cpp
  src/gl-meta.cpp
  src/vertex.cpp
  src/soundemitter.cpp
  src/sdlsoundsource.cpp
  src/alstream.cpp
  src/audiostream.cpp
  src/rgssad.cpp
  src/bundledfont.cpp
  src/vorbissource.cpp
  src/windowvx.cpp
  src/tilemapvx.cpp
  src/tileatlasvx.cpp
  src/autotilesvx.cpp
  src/midisource.cpp
  src/fluid-fun.cpp
)

source_group("HiddenChest Source" FILES ${MAIN_SOURCE} ${MAIN_HEADERS})

## Setup embedded source ##

set(EMBEDDED_INPUT
  shader/common.h
  shader/transSimple.frag
  shader/trans.frag
  shader/hue.frag
  shader/sprite.frag
  shader/plane.frag
  shader/gray.frag
  shader/basic_color.frag
  shader/sepia.frag
  shader/gradient.frag
  shader/gradient_map.frag
  shader/vignette.frag
  shader/vignette_sepia.frag
  shader/bitmapBlit.frag
  shader/flatColor.frag
  shader/simple.frag
  shader/simpleColor.frag
  shader/simpleAlpha.frag
  shader/simpleAlphaUni.frag
  shader/flashMap.frag
  shader/minimal.vert
  shader/simple.vert
  shader/simpleColor.vert
  shader/sprite.vert
  shader/tilemap.vert
  shader/tilemapvx.vert
  shader/blur.frag
  shader/blurH.vert
  shader/blurV.vert
  shader/simpleMatrix.vert
  assets/FreeSans.ttf
  assets/icon.png
  assets/app_logo_s01.png
  assets/app_logo_s02.png
  assets/app_logo_s03.png
  assets/app_logo.png
)

if (RGSS2)
  list(APPEND DEFINES RGSS2)
endif()

if (SHARED_FLUID)
  pkg_check_modules(FLUID REQUIRED fluidsynth)
  list(APPEND DEFINES SHARED_FLUID)
endif()

## Process Embeddeds ##

find_program(XXD_EXE xxd DOC "Location of the xxd executable")

macro(ProcessWithXXD outvar inputfile outdir)
  get_filename_component(basefile ${inputfile} NAME)
  set(outputfile ${outdir}/${basefile}.xxd)
  set_source_files_properties(${outputfile} PROPERTIES HEADER_ONLY TRUE)
  add_custom_command(
    OUTPUT ${outputfile}
    COMMAND ${XXD_EXE} -i ${inputfile} ${outputfile}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${inputfile}
    COMMENT "Generating XXD for ${inputfile}"
  )
  list(APPEND ${outvar} ${outputfile})
endmacro()

#file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/xxdhack)
include_directories(${CMAKE_CURRENT_BINARY_DIR})#/xxdhack)

foreach(item ${EMBEDDED_INPUT})
  ProcessWithXXD(EMBEDDED_SOURCE ${item} ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

source_group("Embedded Source" FILES ${EMBEDDED_INPUT} ${EMBEDDED_SOURCE})

## Setup binding source ##

if (BINDING STREQUAL "MRI")
  set(MRIVERSION "2.6" CACHE STRING "Version of MRI to link with")
  pkg_check_modules(MRI REQUIRED ruby-${MRIVERSION})
  list(APPEND DEFINES BINDING_MRI)
  set(BINDING_SOURCE binding-mri/binding-mri.cpp)
elseif(BINDING STREQUAL "MRUBY")
  message(FATAL_ERROR "Mruby support in CMake needs to be finished")
  list(APPEND DEFINES BINDING_MRUBY)
  set(BINDING_HEADERS
    binding-mruby/binding-util.h
    binding-mruby/disposable-binding.h
    binding-mruby/flashable-binding.h
    binding-mruby/binding-types.h
    binding-mruby/sceneelement-binding.h
    binding-mruby/viewportelement-binding.h
    binding-mruby/serializable-binding.h
    binding-mruby/mrb-ext/file.h
    binding-mruby/mrb-ext/rwmem.h
    binding-mruby/mrb-ext/marshal.h
  )
  set(BINDING_SOURCE
    binding-mruby/binding-mruby.cpp
    binding-mruby/binding-util.cpp
    binding-mruby/window-binding.cpp
    binding-mruby/bitmap-binding.cpp
    binding-mruby/sprite-binding.cpp
    binding-mruby/font-binding.cpp
    binding-mruby/viewport-binding.cpp
    binding-mruby/plane-binding.cpp
    binding-mruby/audio-binding.cpp
    binding-mruby/tilemap-binding.cpp
    binding-mruby/etc-binding.cpp
    binding-mruby/graphics-binding.cpp
    binding-mruby/input-binding.cpp
    binding-mruby/table-binding.cpp
    binding-mruby/module_rpg.c
    binding-mruby/mrb-ext/file.cpp
    binding-mruby/mrb-ext/marshal.cpp
    binding-mruby/mrb-ext/rwmem.cpp
    binding-mruby/mrb-ext/kernel.cpp
    binding-mruby/mrb-ext/time.cpp
  )
else()
  message(FATAL_ERROR "Must choose a valid binding type. MRI or MRUBY")
endif()

source_group("Binding Source" FILES ${BINDING_SOURCE} ${BINDING_HEADERS})

## Setup main executable ##

link_directories(
  ${SIGCXX_LIBRARY_DIRS}
  ${PIXMAN_LIBRARY_DIRS}
  ${PHYSFS_LIBRARY_DIRS}
  ${SDL2_LIBRARY_DIRS} # Blindly assume other SDL bits are in same directory
  ${Boost_LIBRARY_DIR}
  ${MRI_LIBDIR}
  ${HIDDENCHEST_LIBRARY_DIR}
)

add_executable(${PROJECT_NAME}
  ${MAIN_HEADERS}
  ${MAIN_SOURCE}#${BINDING_HEADERS}
  ${BINDING_SOURCE}
  ${EMBEDDED_SOURCE}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE ${DEFINES})
target_include_directories(${PROJECT_NAME} PRIVATE
  src
  binding-mri
  ${SIGCXX_INCLUDE_DIRS}
  ${PIXMAN_INCLUDE_DIRS}
  ${PHYSFS_INCLUDE_DIRS}
  ${SDL2_INCLUDE_DIRS} # Blindly assume other SDL bits are in same directory
  ${Boost_INCLUDE_DIR}
  ${MRI_INCLUDE_DIRS}
  ${HIDDENCHEST_INCLUDE_DIRS}
  ${VORBISFILE_INCLUDE_DIRS}
  ${FLUID_INCLUDE_DIRS}
  ${OPENAL_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
  ${SIGCXX_LIBRARIES}
  ${SDL2_LIBRARIES}
  ${SDL2_IMAGE_LIBRARIES}
  ${SDL2_TTF_LIBRARIES}
  ${SDL_SOUND_LIBRARIES}
  ${PHYSFS_LIBRARIES}
  ${PIXMAN_LIBRARIES}
  ${Boost_LIBRARIES}
  ${MRI_LIBRARIES}
  ${HIDDENCHEST_LIBRARIES}
  ${VORBISFILE_LIBRARIES}
  ${FLUID_LIBRARIES}
  ${OPENAL_LIBRARY}
  ${ZLIB_LIBRARY}
  ${PLATFORM_LIBRARIES}
)

PostBuildMacBundle(${PROJECT_NAME} "" "${PLATFORM_COPY_LIBS}")
