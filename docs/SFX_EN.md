# SFX

> In the beginning there was a sound effect (BOOM) :D
> That's what one might speculate, after the conclusions of scientists regarding the big bang that created Our Universe ;)
> 
> The origins of Music Maker go back to the SFX engine, which was created to soundtrack a game written in Turbo Basic. It was here that the " primary soup " was created, which evolved into SFX Music Maker.

Popularly referred to as an Instrument, however, it is simply a sound effect, or more accurately, a series of definitions that make up the final effect that is SFX .

## How is SFX created?

The definition of SFX is described by three elements:

- `VOL` - volume - is nothing more than loudness, and more expertly is the amplitude of the sound, and even more expertly, is the resultant amplitude of the sound
- `DST` - distortion - the POKEY sound chip offers several types of distortion it can generate. Learn about the full capabilities of POKEY](https://en.wikipedia.org/wiki/POKEY)
- MOD/VAL` modulation - this parameter is described in detail in the section [Modulation types](./modval_EN.md#modulation-types)

Each paretr describes one step of the SFX envelope (see figure below). You can define up to 128 steps.

![SFX-Envelope](./imgs/SFX-Envelope.png)

## Creating an SFX

In the main menu, you will see the `SFX` option. This is the module for creating sounds.

After selecting this option, using the __LEFT/RIGHT__ arrow keys and the __RETURN__ key opens a view of the editor.

### Menu bar

![SFX-Menu_bar](./imgs/SFX-Menu_bar.png)

On the left is the MenuBar, which contains the most important options. To move between them, use the __UP/DOWN__ arrow keys, and make your selection with the __RETURN__ key.

The first is `#00` and is the number of the currently selected SFX. Use the __LEFT/RIGHT__ arrow keys to sequentially change it. Pressing the __RETURN__ key will open a list with all (including undefined) SFXs.
Next to the number `#00` is the name of the SFX.

Just below the `#00` option are placed:

- `>>>` - enter edit mode
- PLY` - switch to the piano mode
- VOL`, `DST`, `MOD`, `VAL` - allows you to quickly switch to the editing of this particular SFX element
- `OPT` - contains configuration options for the SFX

### Edit area

![SFX-Edit_mode](./imgs/SFX-Edit_mode.png)

On the right side of the screen, there is a view of the SFX envelope and its components. Just below them (at the height of the `OPT` option) is information about:
- modulation mode
- current position of the editing cursor
- SFX definition length

## SFX edit mode

After entering the Edit Mode (SFX-`>>>`) the thing to pay attention to when making changes is the currently selected edit item, i.e: `VOL`, `DST`, `MOD`, `VAL`. This element can be changed by pressing __TAB__ or __SHIFT+TAB__, which causes sequential switching between these elements.

To set the desired value of an element, two methods can be used:

1. Using the __UP/DOWN__ arrow keys, sequentially select the value of an element between 0 and 15. Writing this value is done using the hexadecimal system.
2. You can also quickly enter the value you want, by pressing the numeric keys 0 through 9 and the keys A through F (which correspond to values between 10 and 15).

Moving the editing cursor is done with the __LEFT/RIGHT__ arrow keys and in this way, we move the cursor by one position (according to the direction).
It is also possible to move quickly within a definition. To do this, press the __CONTROL__ key in addition to the direction of movement, (the __LEFT/RIGHT__ arrow keys). The cursor position will change (according to the direction) by the width of the screen, this means the 16 points of the SFX definition visible on the edit screen.


## Piano mode

## SFX options

In the bar menu, under the shortcut `OPT`, are hidden options related to the settings of the SFX currently being edited.

![SFX-OPT-Options_list](./imgs/SFX-OPT-Options_list.png)

### SET NAME
`SET NAME`, allows you to give a name to the currently edited SFX. The name can be up to 14 characters long. Names are not validated against already existing (occurring) names.

### `SFX MOD MODE`.
This option is described in more detail under [Modulation in the program](./modval_EN.md#modulation-in-the-program)

### IO>LOAD

### IO>SAVE