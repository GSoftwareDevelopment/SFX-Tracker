## File Format Assumptions:
- Division into sections, where each has a 5 byte unique header
- The order in which the blocks are arranged must not matter

### Main section:
- header: 'SFXMM'
- program version: $10 (means v1.0)
- number of bytes per title (constant SONGNameLength)
- title: SONGNameLength bytes

### SFX definition section:
- header: $00,$00,'SFX'
- SFX number: $00
- number of bytes per name (constant SFXNameLength)
- name: SFXNameLength bytes
- total length of the SFX (word): variable SFXLen*2
- SFX data: SFXLen*2 bytes

### KEY-NOTE definition section:
- header: $00,$00,'KEY'
- SFX number for which there is a KEY-NOTE array: $00
- size: $40 (64 notes)
- array of 64 note values

### TAB definition section:
- header: $00,$00,'TAB'
- TAB number: $00
- number of bytes per name (constant TABNameLength)
- name: TABNameLength bytes
- total length of the TAB: variable TABLen*2
- TAB data: TABLen*2 bytes

### SONG definition section:
- header: $00,'SONG'
- number of SONG lines
- SONG data: number of SONG lines*4
- velocity: tact, beat, line per bear


Translated with www.DeepL.com/Translator (free version)s
