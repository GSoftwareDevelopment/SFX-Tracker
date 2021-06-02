# SFX ENGINE

## Hardware registers

In the SFX engine procedure, hardware registers are strictly assigned to specific functions.

| Register | Description    |
|:--------:|:---------------|
| X        | channel offset |
| Y        | SFX offset     |
| A        | General data   |

### Important things

#### Custom SFX engine extensions

If you want to use them, take care to store their state in the temporary _regX & _regY registers located on page zero before using them.

#### Modulator section

If you want to create your own kind of modulator, you should keep in mind the input parameter as well as (especially) the output parameter.

At the input of the modulator section, the modulator value is stored in the accumulator (register A).
The section also returns a parameter in this register and this is the value of the frequency divider for the POKEY circuit (frequency).

## Software registers

### Zero page registers

#### SFX Tick-Loop registers

These registers are used Only in the SFX_Tick loop and store information about the currently playing channel.

| Register name | ZP addr | Description                      |
|:--------------|:-------:|:---------------------------------|
| sfxPtr        | $f5,$f6 | SFX Pointer                      |
| ~~chnOfs~~    | ~~$f7~~ | ~~SFX Offset in SFX definition~~ |
| chnMode       | $f7     | SFX modulation Mode              |
| chnNote       | $f8     | SFX Note                         |
| chnFreq       | $f9     | SFX Frequency                    |

Below parameters can be accessed by using the [conditional compilation labels](./SFX-Engine.md#conditional-compilation-labels) `SFX_previewChannels`.

| Register name | ZP addr | Description                  |
|:--------------|:-------:|:-----------------------------|
| chnMod        | $fa     | SFX Modulator value*         |
| chnCtrl       | $fb     | SFX distortion & volume      |         

_*_ in most cases contains a function and a value. Usually the oldest bits of this parameter describe the function. The rest is the value.

#### Temporary storage for hardware registers

| Register name | ZP addr | Description                  |
|:--------------|:-------:|:-----------------------------|
| _regA         | $fd     | temporary store for reg A    |
| _regX         | $fd     | temporary store for reg X    |
| _regY         | $fd     | temporary store for reg Y    |

Much faster than regular instructions that operate on the stack. Use the `STA _regA`, `STA _regX`, `STA _regY` commands to preserve registers and the `LDA _regA`, `LDX _regX`, `LDY _regY` commands to restore registers.

## Channels registers

This is an array that describes the state of all 4 channels that the SFX Engine supports.

Their layout is exactly the same as for the Tick loop registers.

| Register name | relative addr | Description                  |
|:--------------|:-------------:|:-----------------------------|
| sfxPtr        | chnOfs+0      | SFX Pointer                  |
| chnOfs        | chnOfs+2      | SFX Offset in SFX definition |
| chnMode       | chnOfs+3      | SFX Modulation Mode          |
| chnNote       | chnOfs+4      | SFX Note                     |

Below parameters can be accessed by using the [conditional compilation labels](./SFX-Engine.md#conditional-compilation-labels) `SFX_previewChannels`.

| Register name | relative addr | Description                  |
|:--------------|:-------------:|:-----------------------------|
| chnFreq       | chnOfs+5      | SFX Frequency                |
| chnMod        | chnOfs+6      | SFX Fn& Modulator value      |

The user specifies the memory space for these registers during engine initialization.

The required space is 32 bytes.

## SFX_Engine UNIT

### What it offers?

`INIT_SFXEngine(_SFXModModes, _SFXList, _TABList, _SONGData:word);`

Engine Initialization. Specify the memory map:

`_SFXModModes` - an array of modulation modes for each SFX

`_SFXList` - an array of pointers (relative for now) for SFX definitions

`_TABList` - an array of indicators (relative for now) for TAB definitions

`_SONGData` - array of TAB layouts

#### Why relative values?

Because it still works with HEAP, and it operates on an array `array[0..0] of byte`, which it assigns (via `ABSOLUTE`) a place in memory.

`SetNoteTable(_note_val:word);`

Sets the memory location `_note_val` for the definition of the note table.
Its layout is linear, i.e. 0 is note C-0, 1 is C#0, 2 is D-0, etc...
Up to a maximum of 64 notes.

`SFX_Start();`

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

`SFX_End();`

Disabling the SFX engine.

### Engine Customization

The __SFX-Engine__ allows you to customize your code to suit your needs. A number of compilation directives allow you to choose the features you want to use in your program.

### Conditional Compilation Labels

`SFX_SWITCH_ROM`.

Defining this label allows access to the RAM "covered" by the ROM. The definition works with the ROMOFF label in __MAD Pascal__, which allows RAM to be used in this area.

`SFX_previewChannels`.

An option that generates a small amount of code, providing the ability to view the current state of the modulator and the distortion and volume values, by transferring from the main loop (_SFX_TICK_) the changes made by the engine to the channel registers.

Additional information is placed in the `CHANNELS` table at offsets 6 and 7 of each channel (see at [Channels registers](./SFX-Engine.md#channels-registers)).

The absence of this definition, frees an additional two bytes on the null page.

`USE_MODULATORS`.

This complex label Must be defined to select supported modulation modes. Its absence at compile time, causes [operation without modulator section](./#mode-operation-without-modulator-section)

No modulator section frees one byte on the zero page.

The presence of the following labels during compilation, create the corresponding code:

- `DFD_MOD` - Direct Frequency Divider Modulation section.
	
- LFD_NLM_MOD` - Low Frequency Divider/Note Level Modulator section
	
- MFD` Middle Frequency Divider
	
- HFD` - High Frequency Divider

For more on modulation, see [Modulation types](./modval_EN.md)

~~`USE_ALL_MODULATORS`~~

~~Forces the use of all supported modulators, regardless of the state of the `USE_MODULATORS` declaration and its subordinates.~~

### Operation mode without modulator section

This is the simplest possible version of the SFX engine.

The SFX definition in this mode, occupies a maximum of 127 bytes and there is only one byte per step.

When operating without a modulator section, the length of the definition is contained in the `SFXModModes` modulation mode array. To use this mode of operation in the `SFXModModes` SFX array index, bit 7 must be set and the rest of the bits indicate length.

> In modulation mode, the end of the SFX definition is checked during its "execution" and is determined by the SFX-STOP function, so it is not required to "keep" its length in a separate array. This solution minimizes memory usage, at the expense of having to pay special attention to the use of the `JUMP_TO` function, since there is no way to quickly check that the jump location does not exceed the definition limit - which can result in a program crash.
