## File Format Assumptions:
- Division into sections, where each has a 5 byte unique header
- The order in which the blocks are arranged must not matter

### Main section:
- header: 'SFXMM'
- program version: $10 (constant SFXMM_VER1_0 means v1.0)
- number of bytes per title (constant SONGNameLength)
- title: SONGNameLength bytes

### SFX definition section:
- header: $00,$00,'SFX'
- SFX number: $00 (from 0 to maxSFXs-1)
- SFX modulation type: $00 (from 0 to 3)
- number of bytes per name (SFXNameLength constant)
- name: SFXNameLength bytes
- total length of SFX (variable SFXLen*2; word lo,hi)
- SFXa data: SFXLen*2 bytes

### TAB definition section:
- header: $00,$00,'TAB'
- TAB number: $00
- number of bytes per name (constant TABNameLength)
- name: TABNameLength bytes
- total length of the TAB: variable TABLen*2
- TAB data: TABLen*2 bytes

### SONG definition section:
- header: $00,'SONG'
- velocity: tact, beat, line per bear
- number of SONG lines
- SONG data: number of SONG lines*4


Translated with www.DeepL.com/Translator (free version)
