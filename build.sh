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

# clear log file
[ -f $logfile ] && rm $logfile

echo "> Compiling project loader..."
cd loader
mp loader.pas >> $logfile
# mp loader.pas -o >> $logfile
[ $? -ne 0 ] && break_build

mads loader.a65 -x -i:$MPBase -o:../$ProjectBin/LOADER.XEX >> $logfile
[ $? -ne 0 ] && break_build
cd ..

echo "> Build resources..."
cd resources
./build.sh
cd ..

echo "> Compiling main project..."

mp $1.pas -define:INCLUDE_RESOURCES >> $logfile
[ $? -ne 0 ] && break_build

mads $1.a65 -x -i:$MPBase -o:$ProjectBin/$1.XEX >> $logfile
[ $? -ne 0 ] && break_build

echo "> Building ATR image..."
cd bin
# clear log file
[ -f $logfile ] && rm $logfile
./build.sh >> $logfile
cd ..
