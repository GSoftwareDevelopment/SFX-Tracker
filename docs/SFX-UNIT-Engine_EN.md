# SFX_Engine UNIT

## What it offers.

`INIT_SFXEngine(_SFXModModes, _SFXList, _TABList, _SONGData:word);`.

Engine Initialization. Specify the memory map:

`_SFXModModes` - an array of modulation modes for each SFX

`_SFXList` - an array of pointers (relative for now) for SFX definitions

`_TABList` - an array of indicators (relative for now) for TAB definitions

`_SONGData` - array of TAB layouts

### Why relative values?

Because it still works with HEAP, and it operates on an array `array[0..0] of byte`, which it assigns (via `ABSOLUTE`) a place in memory.

`SetNoteTable(_note_val:word);`.

Sets the memory location `_note_val` for the definition of the note table.
Its layout is linear, i.e. 0 is note C-0, 1 is C#0, 2 is D-0, etc...
Up to a maximum of 64 notes.

`SFX_Start();`.

Start the SFX engine.

`SFX_ChannelOff(channel:byte);`

It turns off the current SFX playing in the specified channel.

`SFX_Off();`

Here similar to `SFX_ChannelOff`, only for all channels.
This routine is fired during SFX engine shutdown (procedure `SFX_End()`).

`SFX_Note(channel,note,modMode:byte; SFXAddr:word);`.

`channel` - the channel on which SFX will be played

`note` - a note (see `SetNoteTable` note table initialization)

`modMode` - modulation mode. The engine does not read this data by itself.

`SFXAddr` - address of the SFX definition.

The problem is that in the definitions (SFXs and TABs) are at the very beginning of the names that need to be omitted, hence the definition of the address in the computer memory, not the SFX number

`SFX_End();`.

Disabling the SFX engine.
