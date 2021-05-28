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

| Register name | ZP addr | Description                  |
|:--------------|:-------:|:-----------------------------|
| sfxPtr        | $f5,$f6 | SFX Pointer                  |
| chnOfs        | $f7     | SFX Offset in SFX definition |
| chnMode       | $f8     | SFX modulation Mode          |
| chnNote       | $f9     | SFX Note                     |
| chnFreq       | $fa     | SFX Frequency                |
| chnMod        | $fb     | SFX Modulator value*         |
| chnCtrl       | $fc     | SFX distortion & volume      |

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
| sfxPtr        | chnfs+0       | SFX Pointer                  |
| chnOfs        | chnfs+2       | SFX Offset in SFX definition |
| chnMode       | chnfs+3       | SFX Modulation Mode          |
| chnNote       | chnfs+4       | SFX Note                     |
| chnFreq       | chnfs+5       | SFX Frequency                |
| chnMod        | chnfs+6       | SFX Fn& Modulator value      |

The user specifies the memory space for these registers during engine initialization.

The required space is 32 bytes.
