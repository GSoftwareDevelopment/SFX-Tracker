program SFXMM;
{$librarypath './units/'}
{$librarypath './sfx_engine/'}

{$DEFINE ROMOFF}

uses SFX_API, sysutils, strings, gr2, ui, pmgraph;
{$I-}
{$i types.inc}

const
{$i memory.inc}
{$i const.inc}
{$ifdef INCLUDE_RESOURCES}
{$r resources.rc}
{$endif}

var
// buffers

   listBuf:array[0..0] of byte absolute LIST_BUFFER_ADDR; // universal list buffer array
   IOBuf:array[0..0] of byte absolute IO_BUFFER_ADDR; // buffer for IO and Copy/Paste operations (clipboard)

// UI color themes

   themesNames:array[0..0] of byte; // list of themes names
   currentTheme:byte = 0;

// heap

   HEAP_TOP:word;
   HEAP_PTR:array[0..0] of word absolute HEAP_PTRLIST_ADDR;
   _heap_sizes:array[0..0] of word absolute HEAP_SIZES_ADDR;
	heapStoreBuf:array[0..0] of byte absolute EDIT_BUF3_ADDR;
//

   SONGTitle:string[SONGNameLength];

   otherFile:string[FILEPATHMaxLength] absolute OTHER_FILE_ADDR;
   searchPath:string[FILEPATHMaxLength] absolute SEARCH_PATH_ADDR; // used only in IO->DIR
   currentFile:string[FILEPATHMaxLength] absolute CURRENT_FILE_ADDR; // indicate a current opened SFXMM file with full path and device
   FName:string[16];

   cursorPos,cursorShift:byte;         // general cursor position and view offset

   SONGChn,SONGPos,SONGShift:byte;     // SONG current channel,position and view offset

   currentMenu:byte;
   section:byte;

   currentSFX:byte;
   currentOct:byte;
   currentTAB:byte;

   SONG_Beat:byte;

   modified:boolean = false;

//
   statusBar:array[0..0] of byte absolute STATUSBAR_ADDR;
   moduleBar:array[0..0] of byte absolute MODULE_ADDR;
   note_names:array[0..0] of byte;
   octShift:array[0..0] of byte;

   IO_preventSFX:boolean = false;
   IO_messages:boolean = false;
   f:file;

// global access function and procedures
{$i modules/ui_helpers.inc}
{$i units/heap_manage.inc}
{$i modules/io/io_clear_all_data.inc}
// {$i modules/io/io_tag_compare.inc}
{$i modules/io/io_manage.inc}
{$i modules/io/io_options.inc}
{$i modules/io/io_dir.inc}
{$i modules/vis_piano.inc}
{$i modules/clipboard.inc}

// modules
{$i modules/gsd/gsd.pas}
{$i modules/io/io.pas}
{$i modules/notetune/notetune.pas}
{$i modules/sfx/sfx.pas}
{$i modules/tab/tab.pas}
{$i modules/song/song.pas}

procedure init();
begin
{$ifndef INCLUDE_RESOURCES}
   Resources_Load();
{$endif}
   Init_UI(RESOURCES_ADDR);

// load defaults
   IO_messages:=false;

   themesNames:=resptr[themes_names_list];
   if not IOLoadDefaultTheme() then
      moveRes(app_color_schemas,DLI_COLOR_TABLE_ADDR,5);

   IO_clearAllData();

   IOLoadDefaultNoteTable();

// keyboard set
   KRPDEL:=20; KEYREP:=3;

// keyboard resources set
   chars_alphaNum:=resptr[scan_to_scr];
   keys_alphaNum:=resptr[scan_key_codes];
   keys_notes:=resptr[scan_piano_codes];

// other resources set
   note_names:=resptr[str_NoteNames];
   octShift:=resptr[octaveShifts];

   currentMenu:=0;

// timer:=0; repeat until timer>100;

// set defaults files
   clearFilename(otherFile);
   setFilename(defaultFileName,currentFile);
   setFilename(defaultSearchPath,searchPath);
   moveRes(app_vis_tables,VIS_TABLE_ADDR,58);
   moveRes(app_charset,CHARSET_ADDR,512);
   moveRes(app_dlist,DLIST_ADDR,$18);
   initGraph(DLIST_ADDR,VIDEO_ADDR,SCREEN_BUFFER_ADDR); CHBAS:=CHARSET_PAGE;
   PMGInit(PMG_BASE);
   getTheme(currentTheme,PFCOLS); // set default theme color

   showAppSplash();
   moveRes(app_virtual_piano,VIDEO_PIANO_ADDR,40);
   reset_pianoVis();
   updatePiano();

   SFX_Start();
   IO_preventSFX:=true;
   IO_messages:=true;
end;

begin
   init();
   repeat
      clearTopMenu();
      if optionsList(menu_top,width_menuTop,5,currentMenu,key_Left,key_Right) then
         case currentMenu of
            0: GSDModule();
            1: IOModule();
            2: SFXModule();
            3: TABModule();
            4: SONGModule();
         end;
      uncolorWorkarea();
   until false;
end.
