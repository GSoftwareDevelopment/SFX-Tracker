// SFX Music Maker basic setup
SFXMM_VER1_0 		= $10;

FILEPATHMaxLength	= 255;

maxSFXs				= 100;
maxTABs				= 100;

SFX_maxLength		= 128;
TAB_maxLength		= 128;
IO_pageLimit		= 100;

SFXNameLength		= 14;
TABNameLength		= 8;
SONGNameLength		= 32;

defaultSongTitle	= 'SONG NAME';
defaultPath			= 'D:';
defaultFileName	= 'D:SONG.SMM';
defaultThemeFile	= 'D:DEFAULT.THM';

// Interface constants
width_menuTop		= 0;
width_menuBar		= 4;
width_themeEdit	= 11;
width_colorEdit	= 9;
width_menuOptions	= 16;
width_menuTabFunc	= 12;
width_fileList		= 20;
width_filePath		= 20;

//
winXStart			= 4;
winYStart			= 2;


SFX_winWidth		= 16;
TAB_winWidth		= 9;

SFXNameX			= 4;
SFXNameY			= 1;
SFXNumberX		= 1;
SFXNumberY		= 1;
SFXCurPosX			= 13;
SFXCurPosY			= 10;
SFXLenX			= 17;
SFXLenY			= 10;

//
//

//   C  C# D D# E  F   F# G G# A A# B | C  C# D D# E  F   F# G G# A A# B   C  C# D D# E |
//   1                                | 2                                | 3            |
// |   |  | |  |  |   |  | |  | |  |  |   |2 | |3 |  |   |5 | |6 | |7 |  |   |9 | |0 |  |
// |   |S | |D |  |   |G | |H | |J |  |   |L | |; |  |   |  | |  | |  |  |   |  | |  |  |
// |    |    |    |    |    |    |    | Q  | W  | E  | R  | T  | Y  | U  | I  | O  | P  |
// | Z  | X  | C  | V  | B  | N  | M  | ,  | .  | /  |    |    |    |    |    |    |    |

keys_notes:array[0..33] of byte = (
//Z  S  X  D  C  V  G  B  H  N  J  M
 23,62,22,58,18,16,61,21,57,35, 1,37,
//,  L  .  ;  /
 32, 0,34, 2,38,
//Q  2  W  3  E  R  5  T  6  Y  7  U
 47,30,46,26,42,40,29,45,27,43,51,11,
//I  9  O  0  P
 13,48, 8,50,10
);

key2Note:array[0..33] of byte = (
//Z  S  X  D  C  V  G  B  H  N  J  M
$00,$01,$02,$03,$04,$05,$06,$07,$08,$09,$0a,$0b,
//,  L  .  ;  /
$10,$11,$12,$13,$14,
//Q  2  W  3  E  R  5  T  6  Y  7  U
$10,$11,$12,$13,$14,$15,$16,$17,$18,$19,$1a,$1b,
//I  9  O  0  P
$20,$21,$22,$23,$24
);

//
note_val:array[0..63] of byte = (
// pure tones from RMT
	$F3,$E6,$D9,$CC,$C1,$B5,$AD,$A2,$99,$90,$88,$80,$79,$72,$6C,$66,
	$60,$5B,$55,$51,$4C,$48,$44,$40,$3C,$39,$35,$32,$2F,$2D,$2A,$28,
	$25,$23,$21,$1F,$1D,$1C,$1A,$18,$17,$16,$14,$13,$12,$11,$10,$0F,
	$0E,$0D,$0C,$0B,$0A,$09,$08,$07,$06,$05,$04,$03,$02,$01,$00,$00
);

// File TAGs sections
	section_main	:TTag = (83, 70, 88, 77, 77);	// SFXMM
	section_SFX		:TTag = ( 0,  0, 83, 70, 88);	// __SFX
	section_KEY		:TTag = ( 0,  0, 75, 69, 89);	// __KEY
	section_TAB 	:TTag = ( 0,  0, 84, 65, 66);	// __TAB
	section_SONG	:TTag = ( 0, 83, 79, 78, 71);	// _SONG
	section_theme	:TTag = (84, 72, 69, 77, 69); // THEME

// resources ids
	base_menus						= 0;
	base_strings					= 10;
	base_messages					= 21;
	base_tables						= 25;

	menu_top							= base_menus+0;
   menu_sfx							= base_menus+1;
   menu_sfx_options				= base_menus+2;
   menu_tabs						= base_menus+3;
	menu_tabs_options				= base_menus+4;
	menu_tab_edit					= base_menus+5;
	menu_GSD							= base_menus+6;
	menu_Themes						= base_menus+7;
	menu_ThemeEdit					= base_menus+8;
	menu_IO							= base_menus+9;

	str_SFX_notDefined			= base_strings+0;
	str_TAB_types					= base_strings+1;
	str_Credits						= base_strings+2;
	str_IO_Prompt					= base_strings+3;
	str_IO_noFiles					= base_strings+4;
	str_IO_nextPage				= base_strings+5;
	str_IO_prevPage				= base_strings+6;
	str_IO_Error					= base_strings+7;
	str_IO_Reading					= base_strings+8;
	str_IO_Writing					= base_strings+9;
	str_IO_Skipping				= base_strings+10;

	msg_IO_Quit						= base_messages+0;
	but_IO_Quit						= base_messages+1;
	msg_Theme_Overwrite			= base_messages+2;
	but_Theme_Overwrite			= base_messages+3;

	table_note_names				= base_tables+0;
	color_themes					= base_tables+1;

//

   SFXMenuOptions					= 3;
	SFXMenuOption_Back			= 2;

	TABMenuOptions					= 2;
	TABMenuOption_Back			= 1;

	TABEditOptions					= 8;
	TABEditOption_BackToEdit	= 6;
	TABEditOption_BackToMenuBar= 7;

// Program Errors
	error_notThemeFormat			= 1;

	IO_OK								= 1;
	IOError_fileNotFound			= 170;
