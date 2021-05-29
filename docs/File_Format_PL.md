## Założenia formatu pliku:
- Podział na sekcje, gdzie każda posiada 5cio bajtowy, unikatowy nagłówek
- Kolejność ułożenia bloków nie może mieć znaczenia

### Sekcja główna:
- nagłówek: 'SFXMM'
- wersja programu: $10 (stała SFXMM_VER1_0 oznacza v1.0)
- ilość bajtów przypadających na tytuł (stała SONGNameLength)
- tytuł: SONGNameLength bajtów

### Sekcja definicji SFX:
- nagłówek: $00,$00,'SFX'
- numer SFXa: $00	(od 0 do maxSFXs-1)
- rodzaj modulacji SFXa: $00 (od 0 do 3)
- całkowita długość SFXa (SFXLen*2) wraz z nazwą (SFXNameLength bajtów) (słowo lo,hi)
- dane SFXa: SFXNameLength+SFXLen*2 bajtów

### Sekcja definicji TAB:
- nagłówek: $00,$00,'TAB'
- numer TABa: $00
- całkowita długość TABa (TABLen*2) wraz z nazwą  (TABNameLength bajtów) (słowo lo,hi)
- dane TABa: TABNameLength+TABLen*2 bajtów

### Sekcja definicji SONG:
- nagłówek: $00,'SONG'
- prędkość: tact, beat, line per bear
- ilość linii SONG
- dane SONG: ilość lini SONG*4
