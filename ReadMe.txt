HiddenChest is Kyonides-Arkanthes's fork of an older project called
mkxp developed by Ancurio.

This project can be compiled using CMake. The binaries I provide were compiled
on Kubuntu 18.04 LTS.

* Box.com Online Directory
https://app.box.com/s/8956vvm2spfx5sdmeo2jrrtanawsoj1j

* Mega Online Directory
https://mega.nz/#F!YEITBAwY!W9cDFX7d-cibDFcLoKbHNg

The hiddenchest shared object alias libhiddenchest.so might be installed in the
default library path or placed in a local lib directory to let the HiddenChest
engine run as expected.

** This project is distributed under the terms of the GNU General
** Public License as published by the Free Software Foundation version 2.
** This project is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.

Files included in the TAR XZ file usually are:

h800 or h800
  Resolution 800*608 and includes extra buttons not found in vanilla RGSS games
  plus tons of other features already described in README.md
h*anyotherwidth or h*anyotherwidth*
  Possible resolutions 800*608, 960*736, 1088*608, 1280*768, 1680*1050 and 1920*1080
  Includes extra buttons not found in vanilla RGSS games

** extras directory **

MapCustomResFixes.rb
  A Ruby script you can open with your favorite text editor to
  copy and paste it in any RGSS 1 or 2 game to let old scripts
  become compatible with the increased window resolution.
WindowOpenness.rb
  A Ruby script you can copy and paste to check how the window openness feature
  works in a common menu.
SceneExample.rb
  A Ruby script that shows you how to combine a window openness feature with any
  sprite reduce_speed increase_width! and reduce_width! features.
