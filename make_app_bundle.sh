#!/bin/bash

# Global Framework List
FRAMEWORKS="SDL2 SDL2_mixer"

#delete is we already exist
[[ -d GLTetris.app ]] && rm -rf GLTetris.app
mkdir -p GLTetris.app/Contents/{MacOS,Resources}
cp -v Info.plist GLTetris.app/Contents/Info.plist

# copy executable
cp -v *.bin GLTetris.app/Contents/MacOS

#resource copy
cp -r sound GLTetris.app/Contents/Resources
cp -r music GLTetris.app/Contents/Resources
cp -v *.ttf GLTetris.app/Contents/Resources

#linker stuff here lol
# We use SDL2 and SDL2_mixer so setup and copy the dylib file to
# Library/Frameworks
for x in $FRAMEWORKS; do 
  cp -r /Library/Frameworks/${x}.framework GLTetris.app/Contents/Resources
done


install_name_tool -change @rpath/SDL2.framework/Versions/A/SDL2 @executable_path/../Resources/SDL2.framework/Versions/A/SDL2 ./GLTetris.app/Contents/MacOS/GLTetris2022.bin
install_name_tool -change @rpath/SDL2_mixer.framework/Versions/A/SDL2_mixer @executable_path/../Resources/SDL2_mixer.framework/Versions/A/SDL2_mixer ./GLTetris.app/Contents/MacOS/GLTetris2022.bin

# freetype is from brew
cp /usr/local/lib/libfreetype*.dylib GLTetris.app/Contents/Resources
# TODO: fix freetype lib paths with otool
