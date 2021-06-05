; TAB - PLAY NOTE
			sty _regTemp									; store current TAB offset

; get note freq value
			and #%00111111									; extract SFX Id from Order
			asl @												; multiply SFX Id by 2 to get offset in SFXPtr offset table
			tay

; get SFX pointer offset from SFXPtr table and set in current channel
.ifndef DONT_CALC_ABS_ADDR
			lda SFX_TABLE_ADDR,y						// take low part of SFX Pointer
			clc
.ifndef DONT_CALC_SFX_NAMES
			adc #SFX_NameLength						// incrase about SFX name length
.endif
			adc #<DATA_ADDR
			sta SFX_CHANNELS_ADDR+_SFXPtrLo,x
			lda SFX_TABLE_ADDR+1,y
			adc #>DATA_ADDR
			sta SFX_CHANNELS_ADDR+_SFXPtrHi,x
.else
			lda SFX_TABLE_ADDR,y
			sta SFX_CHANNELS_ADDR+_SFXPtrLo,x
			iny
			lda SFX_TABLE_ADDR+1,y
			sta SFX_CHANNELS_ADDR+_SFXPtrHi,x
.endif

			lda #$00											; reset current SFX offset to the beginig of definition
			sta SFX_CHANNELS_ADDR+_chnOfs,x

; get Note frequency divider from NOTE_TABLE
         lda TABNote										; get TAB Note value
			cmp #FN_NOTE_FREQ								; check for Note or Frequency Divider Set
			bpl TAB_FN_Freq								; <64 its Note Set

TAB_FN_Note
			tay
			sta SFX_CHANNELS_ADDR+_chnNote,x
         lda NOTE_TABLE_ADDR,y						; get note frequency value from NOTE_TABLE

TAB_FN_Freq
			sta SFX_CHANNELS_ADDR+_chnFreq,x

         ldy _regTemp									; restore current TAB Offset

			jmp next_player_tick
