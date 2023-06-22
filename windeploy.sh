#!/bin/sh

set -e

# Create folders
mkdir windows
cd  windows

# Compile
x86_64-w64-mingw32-cmake -DCMAKE_BUILD_TYPE=Release ../src
make -j 4
find -mindepth 1 -maxdepth 1 -type d | xargs rm -rf
find -maxdepth 1 -iname "*cmake*" | xargs rm -rf

# Add libraries
WINEDEBUG=+loaddll x86_64-w64-mingw32-wine geometry.exe 2>&1 >/dev/null \
    | grep Loaded | grep -v 'system32\|: builtin\|.exe' \
    | awk -F'"' '{print $2}' \
    | sed -e 's@\\\\@/@g' -e 's/^[A-Z]://' \
    > dll.lst
cp $(cat dll.lst) . 
rm dll.lst

# Add additional dlls
QDIR=/usr/x86_64-w64-mingw32/lib/qt6
QPLUG=$QDIR/plugins

mkdir platforms
cp $QPLUG/platforms/qwindows.dll platforms

mkdir styles
cp $QPLUG/styles/qwindowsvistastyle.dll styles

mkdir imageformats
cp $QPLUG/imageformats/qsvg.dll imageformats

mkdir iconengines
cp $QPLUG/iconengines/qsvgicon.dll iconengines

cp $QDIR/bin/Qt6Svg.dll .
