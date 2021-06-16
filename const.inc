// SFX Music Maker basic setup
SFXMM_VER1_0 		= $10;
SFXMM_VER1_1 		= $11;

FILEPATHMaxLength	= 127;

	SFXNameLength		= 14;
	NOTETABnameLength = 11;
	TABNameLength		= 8;
	SONGNameLength		= 32;

maxSFXs				= 64;
maxTABs				= 64;

SFX_maxLength		= 128;
TAB_maxLength		= 128;
SONG_maxLength		= 64;
IO_pageLimit		= 48;

defaultSongTitle	= 'SONG NAME';
defaultSearchPath	= 'H:';
defaultFileName	= 'H:SONG.SMM';
defaultThemeFile	= 'H:DEFAULT.THM';
defaultNoteTable	= 'H:DEFAULT.SNT';
defaultSongBeat	= 4;
defaultSongTempo	= 5;
defaultPianoOctave= 1;

//
// INTERFACE CONSTANTS

width_menuTop		= 0;
width_menuBar		= 4;
width_menuOptions	= 16;
width_menuIOOptions = 8;
width_menuModType = 8;
width_menuSFXFunc	= 12;
width_menuTabFunc	= 12;
width_fileList		= 20;
width_filePath		= 20;

//
tuneBarsPos			= 86;
tunePianoPos		= 186;

winXStart			= 4;
winYStart			= 2;

winSONGXStart		= 7;

SFX_winWidth		= 16;
TAB_winHeight		= 9;
SONG_winHeight		= 9;

SFXNameX				= 4;
SFXNameY				= 1;
SFXNumberX			= 1;
SFXNumberY			= 1;
SFXCurPosX			= 13;
SFXCurPosY			= 10;
SFXLenX				= 17;
SFXLenY				= 10;

//

   SFXMenuOptions					= 6;
	SFXMenuOption_Back			= 3;
	SFXMenuModType					= 5;
	SFXMenuModType_Back			= 4;

	TUNEMenu							= 3;
	TUNEMenuOptions				= 4;
	TUNEOptions_BackToEdit		= 1;
	NOTEDEFMenu						= 4;

	IOOptions						= 2;

	TABMenuOptions					= 3;
	TABMenuOption_Back			= 2;

	TABEditOptions					= 8;
	TABEditOption_BackToEdit	= 6;
	TABEditOption_BackToMenuBar= 7;

	SONGMenuOptions				= 3;
	SONGMenuOption_Back			= 2;
	SONGEditOption_BackToEdit	= 6;
	SONGEditOption_BackToMenuBar= 7;

//
//	TAB FUNCTIONS (ORDERS)

//												  order   |   param
//	TABFn_note 				= $00;	// 00ss ssss | 00oo nnnn - note
	TABFn_freq				= $40;	// 01ss ssss | vvvv vvvv - frequency divider

// TAB_FN_LOOP
	TABFn_jump				= $80;	// 1000 0000 | 0vvv vvvv

	TAB_maxRepeats 		= 63;
	TABFn_repeat			= $81;	// 10xx xxxx | 0vvv vvvv
//	TABFn_repeatMax		= $BF;

	TABFn_noteOff  		= $C0;	// 1100 0000 | 1000 0000
	TABFn_noteOff_param	= $80;

	TABFn_blank				= $C0;	// 1100 0000 | 0000 0000
	TABFn_blank_param		= $00;

	TABFn_end				= $FF;	// 1111 1111 | 1111 1111
	TABFn_end_param		= $FF;

//
// SONG FUNCTIONS (ORDERS)

//	in any channels
//	%00xxxxx - set TAB#id in channel
	chnBlank				= %01000000; // blank (No operation)
	chnOff				= %01111111; // off channel

//	only in first channel			  params in next channels
	chnOrder				= %10000000;
	chnOrder_Tempo		= %10000000; // %00000xxx - set tempo; x - line pre beat 0-9
	chnOrder_JumpTo	= %10000010; // %00xxxxxx - JUMP TO SONG position; x - position 0-63
	chnOrder_Repeat	= %10000100; // %00xxxxxx - REPEAT; x - repeat times 0-63
//	chnOrder_Title		= %11000000; // %xxxxxxxx %xxxxxxxx - Sub SONG Title; x - heap offset of title
	chnOrder_EndSong	= %11111111; // END SONG

//
// FILE SECTION TAGS
	section_main	:TTag = (83, 70, 88, 77, 77);	// SFXMM
	section_SFX		:TTag = ( 0,  0, 83, 70, 88);	// __SFX
	section_NOTE	:TTag = ( 0, 78, 79, 84, 69); // _NOTE
	section_TAB 	:TTag = ( 0,  0, 84, 65, 66);	// __TAB
	section_SONG	:TTag = ( 0, 83, 79, 78, 71);	// _SONG
	section_theme	:TTag = (84, 72, 69, 77, 69); // THEME

//
// RESOURCES IDS

	base_menus						= 0;
	base_strings					= 21;
	base_messages					= 35;

	menu_top							= base_menus+0;
   menu_sfx							= base_menus+1;
   menu_sfx_options				= base_menus+2;
	menu_note_tune					= base_menus+3;
	menu_tune_options				= base_menus+4;

	menu_IO_options				= base_menus+5;

	menu_NoteDef					= base_menus+6;

   menu_sfx_mode					= base_menus+7;
	menu_HFD_mode					= base_menus+8;
	menu_MFD_mode					= base_menus+9;
	menu_LFD_NLM_mode				= base_menus+10;
	menu_DFD_mode					= base_menus+11;

   menu_tabs						= base_menus+12;
	menu_tabs_options				= base_menus+13;
	menu_tab_edit					= base_menus+14;
	menu_GSD							= base_menus+15;
	menu_Themes						= base_menus+16;
	menu_Song						= base_menus+17;
	menu_Song_options				= base_menus+18;
	menu_Song_edit					= base_menus+19;
	menu_IO							= base_menus+20;

	str_notDefined					= base_strings+0;
	str_IO_nextPage				= base_strings+1;
	str_IO_prevPage				= base_strings+2;
	str_NoteNames					= base_strings+3;
	str_EndSONGOrder				= base_strings+4;
	wild_allFiles					= base_strings+5;

	scan_to_scr						= base_strings+6;
	scan_key_codes					= base_strings+7;
	scan_piano_codes				= base_strings+8;
	val_menu_beatStep				= base_strings+9;
	val_menu_tempo					= base_strings+10;

	pianoTuneOdd					= base_strings+11;
	pianoTuneEven					= base_strings+12;
	tuneIndChars					= base_strings+13;

	msg_IO_DirPrompt				= base_messages+0;
	msg_IO_SavePrompt				= base_messages+1;
	msg_IO_LoadPrompt				= base_messages+2;
	msg_IO_noFiles					= base_messages+3;
	msg_IO_Error					= base_messages+4;
	msg_IO_Reading					= base_messages+5;
	msg_IO_Writing					= base_messages+6;
	msg_IO_Skipping				= base_messages+7;
	msg_IO_Quit						= base_messages+8;
	but_IO_Quit						= base_messages+9;
	msg_Theme_Overwrite			= base_messages+10;
	but_Theme_Overwrite			= base_messages+11;
	msg_UnknownDefinition		= base_messages+12;
	msg_SFX_ValuePrompt			= base_messages+13;
	msg_TAB_JumpToPrompt			= base_messages+14;
	msg_TAB_RepeatPrompt			= base_messages+15;
	msg_TAB_FreqPrompt			= base_messages+16;
	msg_TAB_SFXIdPrompt			= base_messages+17;
	msg_BeatStepPrompt			= base_messages+18;
	msg_SetTempoPrompt			= base_messages+19;
	msg_pianoTuneInfo				= base_messages+20;

//
// PROGRAMM ERRORS

	IO_OK								= 1;

	error_badSectionTag			= 12;
	error_badFileFormat			= 13;

	IOError_fileNotFound			= 170;

//
// LOAD MODES

	LOAD_ALL_DATA 	= 0;
	LOAD_NOTE_DEF 	= 1;
	LOAD_SFX_DEF	= 2;
