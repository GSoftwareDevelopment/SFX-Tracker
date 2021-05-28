unit SFX_Engine;

interface
const
	SFX_CHANNELS_ADDR	= $6F0;
	SFXNameLength		= 14;
	TABNameLength		= 8;
	SONGNameLength		= 32;

var
(* SFX Mod Modes:
	0 - HFD - High Freq. Div.     - relative modulation of the frequency divider in the range of +/- 127
											- without the possibility of looping the SFX
											- Full backwards compatibility with the original SFX engine
	1 - MFD - Middle Freq. Div.   - relative modulation of the frequency divider in the range of +/- 63
											- SFX looping possible
	2 - LFD/NLM - Low Freq Div.	- note level modulation in relative range of +/- 32 half tones;
											- relative modulation of freq. divider in the range of +/- 32
											- SFX looping possible
	3 - DSD - Direct Set Div.		- direct set of the frequency divider - without looping possible
*)
	SFXModMode:array[0..0] of byte;	// indicates the type of modulation used in the SFX.
	SFXPtr:array[0..0] of word;		// heap pointers to SFX definitions
	TABPtr:array[0..0] of word;		// heap pointera to TAB definitions
	SONGData:array[0..0] of byte;		// table for SONG data

	song_tact,song_beat,song_lpb:byte;

	channels:array[0..15] of byte absolute SFX_CHANNELS_ADDR;

procedure INIT_SFXEngine(_SFXModModes,_SFXList,_TABList,_SONGData:word);
procedure SetNoteTable(_note_val:word);

implementation
var
	AUDIO:array[0..0] of byte absolute $d200;
	note_val:array[0..0] of byte;

procedure INIT_SFXEngine;
var
	chnOfs:byte;

begin
	SFXModMode:=pointer(_SFXModModes);
	SFXPtr:=pointer(_SFXList);
	TABPtr:=pointer(_TABList);
	SONGData:=pointer(_SONGData);

	chnOfs:=0;
	repeat
		channels[chnOfs]:=$ff; chnOfs:=chnOfs+1;	// SFX address lo
		channels[chnOfs]:=$ff; chnOfs:=chnOfs+1;	// SFX address hi
		channels[chnOfs]:=$ff; chnOfs:=chnOfs+1;	// SFX offset
		channels[chnOfs]:=$00; chnOfs:=chnOfs+1;	// SFX modulation Mode
		channels[chnOfs]:=$00; chnOfs:=chnOfs+1;	// SFX Note
		channels[chnOfs]:=$00; chnOfs:=chnOfs+1;	// SFX frequency
		channels[chnOfs]:=$00; chnOfs:=chnOfs+1;	// SFX modulation Value
	until chnOfs>15;
end;

procedure SetNoteTable;
begin
	note_val:=pointer(_note_val);
end;

procedure SFX_tick(); Assembler; Interrupt;
asm
	icl 'units/sfx_engine.asm'
end;

end.
