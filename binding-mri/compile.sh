#!/bin/bash
echo "Making 'obj' directory"
mkdir 'obj'

echo "Compiling objects" && echo "Building array_ext.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -o obj/array_ext.o -c array_ext.cpp -fPIC -lruby-2.6 || exit 1

echo "Building module.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -o obj/module.o -c module.cpp -fPIC -lruby-2.6 || exit 1

echo "Building settings.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -I/usr/include/sigc++-2.0 -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include -I/usr/include/pixman-1 -I/usr/include/SDL2 -o obj/settings.o -c settings.cpp -fPIC -lsigc-2.0 -lruby-2.6 || exit 1

echo "Building terms.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -I/usr/include/sigc++-2.0 -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include -I/usr/include/pixman-1 -I/usr/include/SDL2 -o obj/terms.o -c terms.cpp -fPIC -lsigc-2.0 -lruby-2.6 || exit 1

echo "Building backdrop.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -I/usr/include/sigc++-2.0 -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include -I/usr/include/pixman-1 -I/usr/include/SDL2 -o obj/backdrop.o -c backdrop.cpp -fPIC -lsigc-2.0 -lruby-2.6 || exit 1

echo "Building scripts.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -I/usr/include/sigc++-2.0 -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include -I/usr/include/pixman-1 -I/usr/include/SDL2 -o obj/scripts.o -c scripts.cpp -fPIC -lsigc-2.0 -lruby-2.6 || exit 1

echo "Building binding-util.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -I/usr/include/sigc++-2.0 -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include -I/usr/include/pixman-1 -I/usr/include/SDL2 -o obj/binding-util.o -c binding-util.cpp -fPIC -lsigc-2.0 -lruby-2.6 || exit 1

echo "Building point.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -I/usr/include/SDL2 -o obj/point.o -c point.cpp -fPIC -lruby-2.6 || exit 1

echo "Building audio-binding.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -I/usr/include/sigc++-2.0 -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include -I/usr/include/SDL2 -o obj/audio-binding.o -c audio-binding.cpp -fPIC -lsigc-2.0 -lruby-2.6 || exit 1

echo "Building bitmap-binding.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -I/usr/include/sigc++-2.0 -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include -I/usr/include/pixman-1 -I/usr/include/SDL2 -o obj/bitmap-binding.o -c bitmap-binding.cpp -fPIC -lruby-2.6 || exit 1

echo "Building etc-binding.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -I/usr/include/sigc++-2.0 -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include -I/usr/include/SDL2 -o obj/etc-binding.o -c etc-binding.cpp -fPIC -lruby-2.6 || exit 1

echo "Building filesystem-binding.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -I/usr/include/sigc++-2.0 -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include -I/usr/include/SDL2 -o obj/filesystem-binding.o -c filesystem-binding.cpp -fPIC -lruby-2.6 || exit 1

echo "Building font-binding.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -I/usr/include/sigc++-2.0 -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include -I/usr/include/SDL2 -o obj/font-binding.o -c font-binding.cpp -fPIC -lruby-2.6 || exit 1

echo "Building graphics-binding.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -I/usr/include/sigc++-2.0 -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include -I/usr/include/SDL2 -o obj/graphics-binding.o -c graphics-binding.cpp -fPIC -lruby-2.6 || exit 1

echo "Building input-binding.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -I/usr/include/sigc++-2.0 -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include -o obj/input-binding.o -c input-binding.cpp -fPIC -lruby-2.6 || exit 1

echo "Building module_rpg.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -o obj/module_rpg.o -c module_rpg.cpp -lruby-2.6 || exit 1

echo "Building msgboxsprite-binding.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -I/usr/include/sigc++-2.0 -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include -I/usr/include/SDL2 -o obj/msgboxsprite-binding.o -c msgboxsprite-binding.cpp -fPIC -lruby-2.6 || exit 1

echo "Building sprite-binding.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -I/usr/include/sigc++-2.0 -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include -I/usr/include/SDL2 -o obj/sprite-binding.o -c sprite-binding.cpp -fPIC -lruby-2.6 || exit 1

echo "Building plane-binding.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -I/usr/include/sigc++-2.0 -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include -I/usr/include/SDL2 -o obj/plane-binding.o -c plane-binding.cpp -fPIC -lruby-2.6 || exit 1

echo "Building table-binding.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -I/usr/include/sigc++-2.0 -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include -o obj/table-binding.o -c table-binding.cpp -fPIC -lruby-2.6 || exit 1

echo "Building tilemap-binding.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -I/usr/include/sigc++-2.0 -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include -I/usr/include/SDL2 -o obj/tilemap-binding.o -c tilemap-binding.cpp -fPIC -lruby-2.6 || exit 1

echo "Building tilemapvx-binding.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -I/usr/include/sigc++-2.0 -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include -I/usr/include/SDL2 -o obj/tilemapvx-binding.o -c tilemapvx-binding.cpp -fPIC -lruby-2.6 || exit 1

echo "Building viewport-binding.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -I/usr/include/sigc++-2.0 -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include -I/usr/include/SDL2 -o obj/viewport-binding.o -c viewport-binding.cpp -fPIC -lruby-2.6 || exit 1

echo "Building window-binding.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -I/usr/include/sigc++-2.0 -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include -I/usr/include/SDL2 -o obj/window-binding.o -c window-binding.cpp -fPIC -lruby-2.6 || exit 1

echo "Building windowvx-binding.cpp"
g++ -I. -I../src -I/usr/include/x86_64-linux-gnu/ruby-2.6.0 -I/usr/include/ruby-2.6.0 -I/usr/include/sigc++-2.0 -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include -I/usr/include/SDL2 -o obj/windowvx-binding.o -c windowvx-binding.cpp -fPIC -lruby-2.6 || exit 1

echo "Linking shared object..."
g++ -O3 -DNDEBUG -shared obj/array_ext.o obj/audio-binding.o obj/module.o obj/backdrop.o obj/scripts.o obj/terms.o obj/settings.o obj/binding-util.o obj/point.o obj/bitmap-binding.o obj/etc-binding.o obj/filesystem-binding.o obj/font-binding.o obj/graphics-binding.o obj/input-binding.o obj/module_rpg.o obj/msgboxsprite-binding.o obj/sprite-binding.o obj/plane-binding.o obj/table-binding.o obj/tilemap-binding.o obj/tilemapvx-binding.o obj/viewport-binding.o obj/window-binding.o obj/windowvx-binding.o -o libhiddenchest.so -fPIC -lsigc-2.0 -lSDL2 -lSDL2_image -lSDL2 -lSDL2_ttf -lSDL2 -lSDL_sound -lSDL2 -lphysfs -lpixman-1 -lruby-2.6 -lm -lz

strip libhiddenchest.so
rm -r obj
