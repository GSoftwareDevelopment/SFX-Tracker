#!/bin/bash

# This script uses a program from the ATARI-TOOLS utility package.
# It can be found at https://github.com/jhallen/atari-tools.

# After compiling the program, update path in the 'export PATH' line below
export PATH="$HOME/Atari/atari-tools-master:$PATH"

projectname="SFXMM"
imagefile="$projectname.ATR"
dosdir="DOS25/"
bsfile="bootsector.obj"

function error() {
	echo "! $1"
	exit 1
}
function break_link() {
	error "Error in linking project image"
}

[ ! -d $projectname ] && error "Project folder '$projectname' doesn\'t exist."
[ ! -d $dosdir ] && error "DOS directory doesn't exist."
[ ! -f $bsfile ] &&	error "Bootsector file doesn't exist."

echo "Creating '$projectname' image file..."
atr $imagefile mkfs dos2.5 $bsfile
[ $? -ne 0 ] && break_link

cd $dosdir
echo "Copying DOS files to image..."
atr ../$imagefile w *.*
[ $? -ne 0 ] && break_link
cd ..

cd $projectname

echo "Linking Loader & Project file together..."
cat LOADER.XEX SFXMM.XEX > AUTORUN.SYS
rm LOADER.XEX
rm SFXMM.XEX

echo "Copying Project files to image..."
atr ../$imagefile w *.*
if [ $? -ne 0 ]; then
	break_link
fi

cd ..

# echo "> Review of '$projectname' disk image"
# atr $imagefile ls -al
