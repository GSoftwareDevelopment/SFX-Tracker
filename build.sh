#!/bin/bash
export PATH="$HOME/Atari/MadPascal:$PATH"
MPBase="$HOME/Atari/MadPascal/base"
ProjectBin="./bin/SFXMM"
logfile="outmsg.log"

function break_build() {
	echo "!! Project not compiled"
	cat $logfile
	exit 0
}

if [ -f $logfile ]; then
	rm $logfile
fi

echo "> Compiling project loader..."
cd loader
mp loader.pas -o >> $logfile
if [ $? -ne 0 ]; then
	break_build
fi

mads loader.a65 -x -i:$MPBase -o:../$ProjectBin/LOADER.XEX >> $logfile
if [ $? -ne 0 ]; then
	break_build
fi
cd ..

echo "> Build resources..."
cd resources
./build.sh
cd ..

echo "> Compiling main project..."

mp $1.pas -o -define:INCLUDE_RESOURCES >> $logfile
#mp $1.pas -o >> $logfile
if [ $? -ne 0 ]; then
	break_build
fi
mads $1.a65 -x -i:$MPBase -o:$ProjectBin/$1.XEX >> $logfile
if [ $? -ne 0 ]; then
	break_build
fi

echo "> Building ATR image..."
cd bin
./build.sh >> $logfile
cd ..
