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
		channels[chnOfs]:=$00; chnOfs:=chnOfs+1;	// SFX Modulation mode
		channels[chnOfs]:=$00; chnOfs:=chnOfs+1;	// SFX Note
		channels[chnOfs]:=$00; chnOfs:=chnOfs+1;	// SFX frequency
	until chnOfs>15;
end;

procedure SetNoteTable;
begin
	note_val:=pointer(_note_val);
end;

procedure SFX_tick(); Assembler; Interrupt;
asm
xitvbl   = $e462;
sysvbv   = $e45c;
audf     = $d200;
audc     = $d201;

// below variables is only for current channel
sfxPtr	= $f5; 		// SFX Pointer (2 bytes)
chnOfs	= $f7; 		// SFX Offset in SFX definition
chnMode	= $f8; 		// SFX Modulation Mode
chnNote	= $f9; 		// SFX Note
chnFreq	= $fa; 		// SFX Frequency
chnModVal	= $fb;	// SFX Modulator
chnCtrl	= $fc; 		// SFX Control (distortion & volume)

_regA 	= $fd;
_regX 	= $fe;
_regY 	= $ff;

			phr

tick_start
			ldx #0  // set channel offset to first channel

// fetching channel & sfx data
// prepare SFX Engine registers

channel_set
			ldy SFX_CHANNELS_ADDR+2,x	// get SFX offset
         cpy #$ff	// check SFX offset
         bne fetch_SFX_data
			jmp next_channel // $ff=no SFX

fetch_SFX_data
         sty chnOfs	// propably, completly unnessesery

// get SFX pointer
			lda SFX_CHANNELS_ADDR,x
         sta sfxPtr
         lda SFX_CHANNELS_ADDR+1,x
         sta sfxPtr+1

// get SFX modulation mode
         lda SFX_CHANNELS_ADDR+3,x
         sta chnMode
			cmp #3				// check DFD Modulation mode
			bne modulators
//
// DFD - Direct Frequency Divider
// first becouse, must be fast as possible
         lda (sfxPtr),y		// get MOD/VAL
         jmp setPokey

//
// modulators section
// input: A register = modulation mode
// output: A register = frequency value
//
modulators
			cmp #2			// check LFD/NLM
			bne check_MFD

//
// Low Frequency Divider Modulator/Note Value Modulator
LFD_NLM_mode				// code for LFD_NLM
         lda (sfxPtr),y	// get modulate value
         sta chnModVal	// store in loop register
         bne decode_LFD_NLM	// check modulation value
         jmp getChannelFreq	// if =0, means no modulation
decode_LFD_NLM
			bpl LFD_NLM_JumpTO	// jump to position in SFX definition, if 7th bit is set
			and #%01000000	// check 6th bit
			bne LFD_NLM_note_mod
// frequency modulation
			lda chnModVal
			cmp #32			// VAL<32 means positive value, otherwise negative
			bpl LFD_NLM_inc_freq
			ora #%11100000 // set 7th-5th bit to get oposite value
LFD_NLM_inc_freq
			clc
			adc chnFreq
			jmp setPokey	// return frequency in register A

// note modulation
LFD_NLM_note_mod
			lda chnModVal
			cmp #32			// VAL<32 means positive value, otherwise negative
			bpl LFD_NLM_inc_note
			ora #%11100000 // set 7th-5th bit to get oposite value
LFD_NLM_inc_note
			clc
			adc chnNote

// get frequency representation of note
			sty _regY
			tay
			lda adr.note_val,y
			ldy _regY
			jmp setPokey

// Jump to
LFD_NLM_JumpTo
			and #%01111111 // clear 7th bit
			bne LFD_NLM_set_SFX_ofs
			ldy #$ff	// end of SFX definition
			jmp next_SFX_Set
LFD_NLM_set_SFX_ofs
			tay // set value to SFX offset register
			jmp LFD_NLM_mode	// one more iteration


//
// MFD - Medium Frequency Divider Modulator
check_MFD
			cmp #1			// check MFD
			bne check_HFD

MFD_mode						// code for MFD
         lda (sfxPtr),y	// get modulate value
         sta chnModVal
         bne decode_MFD	// check modulation
         jmp getChannelFreq	// if 0, means no modulation

decode_MFD
			bpl MFD_JumpTO	// jump to position in SFX definition, if 7th bit is set
			cmp #64			// VAL<64 means positive value, otherwise negative
			bpl MFD_inc_freq	// VAL is positive
			ora #%11000000 // set 7th & 6th bit; VAL is negative
MFD_inc_freq
			clc
			adc chnFreq
			jmp setPokey	// return frequency in register A

// Jump To
MFD_JumpTo
			and #%01111111 // clear 7th bit
			bne MFD_set_SFX_ofs
			ldy #$ff	// end of SFX definition
			jmp next_SFX_Set
MFD_set_SFX_ofs
			tay // set value to SFX offset register
			jmp MFD_mode	// one more iteration

//
// HFD - High Frequency Divider Modulator
// only for compatibility with the original SFX engine
check_HFD
			cmp #0			// check HFD mode
			bne getChannelFreq

HFD_MODE						// code for HFD
         lda (sfxPtr),y	// get modulate value
         sta chnModVal
         bne decode_HFD	// check modulation
			jmp getChannelFreq	//	if 0, means no modulation
decode_HFD
			clc
			adc chnFreq
			jmp setPokey

//
// end modulator section
//


// current frequency in register A
getChannelFreq
         lda SFX_CHANNELS_ADDR+5,x // get current channel frequency
setPokey
			txa
         lsr @
         tax

         sta audf,x			// store direct to POKEY register
// get distortion & volume
         iny					// shift sfx offset to next byte of definition
         lda (sfxPtr),y		// get SFX distortion & volume definition
         sta audc,x			// store direct to POKEY register
			iny

         txa
         asl @
         tax


next_SFX_Set
			tya	// tranfer current SFX offset to A register
         sta SFX_CHANNELS_ADDR+2,x // store SFX offset in channel register

next_channel
			txa			// shift offset to next channel
			adc #8		// 8 bytes per channel
         cmp #$20 	// is it last channel?
         beq endtick	// yes, end tick

			tax			// no, go to fetching data
         jmp channel_set

endtick  plr
         jmp xitvbl
         rts
end;

end.
