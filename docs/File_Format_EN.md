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
- total SFX length (SFXLen*2) including name (SFXNameLength bytes) (word lo,hi)
- SFXa data: SFXNameLength+SFXLen*2 bytes

### TAB definition section:
- header: $00,$00,'TAB'
- TAB number: $00
- total TAB length (TABLen*2) including name (TABNameLength bytes) (word lo,hi)
- TAB data: TABNameLength+TABLen*2 bytes

### SONG definition section:
- header: $00,'SONG'
- velocity: tact, beat, line per bear
- number of SONG lines
- SONG data: number of SONG lines*4

