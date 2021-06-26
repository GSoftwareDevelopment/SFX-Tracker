#!/bin/bash
export PATH="$HOME/Atari/MadPascal:$PATH"
MPBase="$HOME/Atari/MadPascal/base"
ProjectBin="./bin/SFXMM"

function break_build() {
	echo "!! Project not compiled"
	cat outmsg.log
	exit 0
}

echo "> Compiling project loader..."
cd loader
mp loader.pas -o >> outmsg.log
if [ $? -ne 0 ]; then
	break_build
fi

mads loader.a65 -x -i:$MPBase -o:../$ProjectBin/LOADER.XEX >> outmsg.log
if [ $? -ne 0 ]; then
	break_build
fi
cd ..

echo "> Build resources..."
cd resources
./build.sh
cd ..

echo "> Compiling main project..."

mp $1.pas -o $2 >> outmsg.log
if [ $? -ne 0 ]; then
	break_build
fi
mads $1.a65 -x -i:$MPBase -o:$ProjectBin/$1.XEX >> outmsg.log
if [ $? -ne 0 ]; then
	break_build
fi

echo "> Building ATR image..."
cd bin
./build.sh
cd ..
