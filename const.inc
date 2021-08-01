// SFX Music Maker basic setup
SFXMM_VER1_0      = $10;
SFXMM_VER1_1      = $11;
SFXMM_VER1_2		= $12;

FILEPATHMaxLength = 127;

   SFXNameLength     = 14;
   NOTETABnameLength = 11;
   TABNameLength     = 8;
   SONGNameLength    = 32;

maxSFXs           = 64;
maxTABs           = 64;

SFX_maxLength     = 128;
TAB_maxLength     = 128;
SONG_maxLength    = 64;
IO_pageLimit      = 48;

resourcesFile     = 'D:SFXMM.RES';

defaultSongTitle  = 'SONG TITLE';
defaultSearchPath = 'D:';
defaultFileName   = 'D:SONG.SMM';
defaultThemeFile  = 'D:DEFAULTH.EME';
defaultNoteTable  = 'D:DEFAULTN.OTE';
defaultSongBeat   = 4;
defaultSongTempo  = 5;
defaultPianoOctave= 1;

//
// INTERFACE CONSTANTS

width_menuTop     = 0;
width_menuBar     = 4;
width_menuOptions = 16;
width_menuIOOptions = 8;
width_menuModType = 8;
width_menuSFXFunc = 12;
width_menuTabFunc = 12;
width_menuSONGFunc= 12;
width_fileList    = 20;
width_filePath    = 20;

//
tuneBarsPos       = 86;
tunePianoPos      = 186;

winXStart         = 4;
winYStart         = 2;

winSONGXStart     = 8;

SFX_winWidth      = 16;
TAB_winHeight     = 9;
SONG_winHeight    = 9;

SFXNameX          = 4;
SFXNameY          = 1;
SFXNumberX        = 1;
SFXNumberY        = 1;
SFXCurPosX        = 13;
SFXCurPosY        = 10;
SFXLenX           = 17;
SFXLenY           = 10;

//

   SFXMenuOptions             = 6;
   SFXMenuOption_Back         = 3;
   SFXMenuModType             = 5;
   SFXMenuModType_Back        = 4;

   TUNEMenu                   = 3;
   TUNEMenuOptions            = 4;
   TUNEOptions_BackToEdit     = 1;
   NOTEDEFMenu                = 4;

   IOOptions                  = 2;

   TABMenuOptions             = 3;
   TABMenuOption_Back         = 2;

   TABEditOptions             = 7;
   TABEditOption_BackToEdit   = 5;
   TABEditOption_BackToMenuBar= 6;

   SONGMenuOptions            = 3;
   SONGMenuOption_Back        = 2;

   SONGEditOptions            = 8;
   SONGEditOption_BackToEdit  = 6;
   SONGEditOption_BackToMenuBar= 7;

//
// TAB FUNCTIONS (ORDERS)

//                                    order   |   param
   TABFn_note           = $00;   // 00ss ssss | 00oo nnnn - note
   TABFn_freq           = $40;   // 01ss ssss | vvvv vvvv - frequency divider

// TAB_FN_LOOP
   TABFn_jump           = $80;   // 1000 0000 | 0vvv vvvv

   TAB_maxRepeats       = 63;
   TABFn_repeat         = $81;   // 10xx xxxx | 0vvv vvvv
   TABFn_repeatMax      = $BF;

   TABFn_noteOff        = $C0;   // 1100 0000 | 1000 0000
   TABFn_noteOff_param  = $80;

   TABFn_blank          = $C0;   // 1100 0000 | 0000 0000
   TABFn_blank_param    = $00;

   TABFn_end            = $FF;   // 1111 1111 | 1111 1111
   TABFn_end_param      = $FF;

//
// SONG FUNCTIONS (ORDERS)

// in any channels
// chnPlayTab        = %000xxxxx;  // set TAB#id in channel
   chnBlank          = %01000000; // blank (No operation)
   chnOff            = %01111111; // off channel

// only in first channel           params in next channels
   chnOrder          = %10000000;
   chnOrder_Tempo    = %10000000; // %xxxxxxxx - set tempo; x - line pre beat 0-255
   chnOrder_JumpTo   = %10000010; // %00xxxxxx - JUMP TO SONG position; x - position 0-63
   chnOrder_Repeat   = %10000100; // %00xxxxxx - REPEAT; x - repeat times 0-63; %00xxxxxx - JUMP TO SONG position; x - position 0-63
   chnOrder_EndSong  = %11111111; // END SONG

//
// FILE SECTION TAGS
   section_main   :TTag = (83, 70, 88, 77, 77); // SFXMM
   section_SFX    :TTag = ( 0,  0, 83, 70, 88); // __SFX
   section_NOTE   :TTag = ( 0, 78, 79, 84, 69); // _NOTE
   section_TAB    :TTag = ( 0,  0, 84, 65, 66); // __TAB
   section_SONG   :TTag = ( 0, 83, 79, 78, 71); // _SONG
   section_theme  :TTag = (84, 72, 69, 77, 69); // THEME

//
// RESOURCES IDS

   base_menus                 = 0;
   base_strings               = 20;
   base_messages              = 36;
   base_appdata               = 65;

   menu_top                   = base_menus+0;
   menu_sfx                   = base_menus+1;
   menu_sfx_options           = base_menus+2;
   menu_note_tune             = base_menus+3;
   menu_tune_options          = base_menus+4;

   menu_IO_options            = base_menus+5;

   menu_NoteDef               = base_menus+6;

   menu_sfx_mode              = base_menus+7;
   menu_HFD_mode              = base_menus+8;
   menu_MFD_mode              = base_menus+9;
   menu_LFD_NLM_mode          = base_menus+10;
   menu_DFD_mode              = base_menus+11;

   menu_tabs                  = base_menus+12;
   menu_tabs_options          = base_menus+13;
   menu_tab_edit              = base_menus+14;
   menu_GSD                   = base_menus+15;
   menu_Song                  = base_menus+16;
   menu_Song_options          = base_menus+17;
   menu_Song_edit             = base_menus+18;
   menu_IO                    = base_menus+19;
	menu_settings					= base_menus+20;

   str_notDefined             = base_strings+0;
   str_IO_Page                = base_strings+1;
   str_NoteNames              = base_strings+2;
   str_EndSONGOrder           = base_strings+3;
   wild_allFiles              = base_strings+4;

   scan_to_scr                = base_strings+5;
   scan_key_codes             = base_strings+6;
   scan_piano_codes           = base_strings+7;
   shortcutSFX						= base_strings+8;

   val_menu_beatStep          = base_strings+9;
   val_menu_tempo             = base_strings+10;

   octaveShifts               = base_strings+11;
   pianoTuneOdd               = base_strings+12;
   pianoTuneEven              = base_strings+13;
   tuneIndChars               = base_strings+14;
   themes_names_list          = base_strings+15;

   msg_IO_DirPrompt           = base_messages+0;
   msg_IO_SavePrompt          = base_messages+1;
   msg_IO_LoadPrompt          = base_messages+2;
   msg_IO_noFiles             = base_messages+3;
   msg_IO_Error               = base_messages+4;
   msg_IO_Reading             = base_messages+5;
   msg_IO_Writing             = base_messages+6;
   msg_IO_Skipping            = base_messages+7;
   msg_IO_Quit                = base_messages+8;
   but_YesNo                  = base_messages+9;
   msg_Theme_Overwrite        = base_messages+10;
   msg_UnknownDefinition      = base_messages+11;
   msg_SFX_ValuePrompt        = base_messages+12;
   msg_TAB_JumpToPrompt       = base_messages+13;
   msg_TAB_RepeatPrompt       = base_messages+14;
   msg_TAB_FreqPrompt         = base_messages+15;
   msg_TAB_SFXIdPrompt        = base_messages+16;
   msg_BeatStepPrompt         = base_messages+17;
   msg_SetTempoPrompt         = base_messages+18;
   msg_pianoTuneInfo          = base_messages+19;
   msg_newPrompt              = base_messages+20;
	msg_ClipboardBadData			= base_messages+21;
	msg_ClipboardEmpty			= base_messages+22;
	msg_ClipboardCopied			= base_messages+23;
	msg_ClipboardPasted			= base_messages+24;
	msg_mem_stats					= base_messages+25;
	msg_SFX_FastKey_Assigned	= base_messages+26;
	msg_SFX_FastKey_Cleared		= base_messages+27;
	msg_TAB_StepChange			= base_messages+28;

   app_logo                   = base_appdata+0;
   app_virtual_piano          = base_appdata+1;

   app_dlist                  = base_appdata+2;
   app_color_schemas          = base_appdata+3;
   app_vis_tables             = base_appdata+4;
   app_charset                = base_appdata+5;

	chr_NoteShUp					= base_appdata+6;
	chr_NoteShDn					= base_appdata+7;
	chr_FreqShUp					= base_appdata+8;
	chr_FreqShDn					= base_appdata+9;

//
// FILE OPEN MODES

   file_read   = 0;
   file_write  = 1;

//
// PROGRAMM ERRORS

   IO_OK                      = 1;
   IO_EOF                     = 3;

   error_badSectionTag        = 12;
   error_badFileFormat        = %01000000;
   error_inSFXDef					= %00010000;
   error_inTABDef					= %00100000;
//   error_in
	error_indexOutOfRange		= %00000001;
	error_incorrectModMode		= %00000010;
	error_incorrectNoteTab		= %00000011;
	error_blockSizeTooBig		= %00000100;

//
// LOAD MODES

   LOAD_ALL_DATA  = 0;
   LOAD_NOTE_DEF  = 1;
   LOAD_SFX_DEF   = 2;
