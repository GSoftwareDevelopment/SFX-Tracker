#!/bin/bash
export PATH="$HOME/Atari/MadPascal:$PATH"

mads resources.asm -o:resources.dat >> outmsg.log
if [ $? -ne 0 ]; then
	break_build
fi

# cp resources.dat ../bin/SFXMM/SFXMM.RES

