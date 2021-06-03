// SFX Music Maker basic setup
SFXMM_VER1_0 		= $10;

FILEPATHMaxLength	= 128;

	SFXNameLength		= 14;
	TABNameLength		= 8;
	SONGNameLength		= 32;

maxSFXs				= 64;
maxTABs				= 64;

SFX_maxLength		= 128;
TAB_maxLength		= 128;
IO_pageLimit		= 48;

defaultSongTitle	= 'SONG NAME';
defaultSearchPath	= 'H:';
defaultFileName	= 'H:SONG.SMM';
defaultThemeFile	= 'H:DEFAULT.THM';

// Interface constants
width_menuTop		= 0;
width_menuBar		= 4;
width_menuOptions	= 16;
width_menuModType = 8;
width_menuSFXFunc	= 12;
width_menuTabFunc	= 12;
width_fileList		= 20;
width_filePath		= 20;

//
winXStart			= 4;
winYStart			= 2;


SFX_winWidth		= 16;
TAB_winWidth		= 9;

SFXNameX				= 4;
SFXNameY				= 1;
SFXNumberX			= 1;
SFXNumberY			= 1;
SFXCurPosX			= 13;
SFXCurPosY			= 10;
SFXLenX				= 17;
SFXLenY				= 10;

//
//	TAB Function (Order)

//												  order   |   param
	TABFn_note 				= $00;	// 00ss ssss | 00oo nnnn - note
	TABFn_freq				= $40;	// 01ss ssss | vvvv vvvv - frequency divider

// TAB_FN_LOOP
	TABFn_jump				= $80;	// 1000 0000 | 0vvv vvvv

	TAB_maxRepeats 		= 63;
	TABFn_repeat			= $81;	// 10xx xxxx | 0vvv vvvv
	TABFn_repeatMax		= $BF;

	TABFn_noteOff  		= $C0;	// 1100 0000 | 1000 0000
	TABFn_noteOff_param	= $80;

	TABFn_blank				= $C0;	// 1100 0000 | 0000 0000
	TABFn_blank_param		= $00;

	TABFn_end				= $FF;	// 1111 1111 | 1111 1111
	TABFn_end_param		= $FF;

// File TAGs sections
	section_main	:TTag = (83, 70, 88, 77, 77);	// SFXMM
	section_SFX		:TTag = ( 0,  0, 83, 70, 88);	// __SFX
	section_TAB 	:TTag = ( 0,  0, 84, 65, 66);	// __TAB
	section_SONG	:TTag = ( 0, 83, 79, 78, 71);	// _SONG
	section_theme	:TTag = (84, 72, 69, 77, 69); // THEME

// resources ids
	base_menus						= 0;
	base_strings					= 14;
	base_messages					= 21;

	menu_top							= base_menus+0;
   menu_sfx							= base_menus+1;
   menu_sfx_options				= base_menus+2;
   menu_sfx_mode					= base_menus+3;

	menu_HFD_mode					= base_menus+4;
	menu_MFD_mode					= base_menus+5;
	menu_LFD_NLM_mode				= base_menus+6;
	menu_DFD_mode					= base_menus+7;

   menu_tabs						= base_menus+8;
	menu_tabs_options				= base_menus+9;
	menu_tab_edit					= base_menus+10;
	menu_GSD							= base_menus+11;
	menu_Themes						= base_menus+12;
	menu_IO							= base_menus+13;

	str_notDefined					= base_strings+0;
	str_IO_nextPage				= base_strings+1;
	str_IO_prevPage				= base_strings+2;
	str_NoteNames					= base_strings+3;

	scan_to_scr						= base_strings+4;
	scan_key_codes					= base_strings+5;
	scan_piano_codes				= base_strings+6;

	msg_IO_Prompt					= base_messages+0;
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
	msg_SFX_ValuePrompt			= base_messages+12;

//

   SFXMenuOptions					= 3;
	SFXMenuOption_Back			= 2;
	SFXMenuModType					= 5;
	SFXMenuModType_Back			= 4;

	TABMenuOptions					= 2;
	TABMenuOption_Back			= 1;

	TABEditOptions					= 8;
	TABEditOption_BackToEdit	= 6;
	TABEditOption_BackToMenuBar= 7;

// Program Errors
	IO_OK								= 1;

	error_badSectionTag			= 12;
	error_badFileFormat			= 13;

	IOError_fileNotFound			= 170;
