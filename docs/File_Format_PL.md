## Założenia formatu pliku:
- Podział na sekcje, gdzie każda posiada 5cio bajtowy, unikatowy nagłówek
- Kolejność ułożenia bloków nie może mieć znaczenia

### Sekcja główna:
- nagłówek: d'SFXMM'
- wersja programu: $10 (oznacza v1.0)
- ilość bajtów przypadających na nazwę (stała SONGNameLength)
- nazwa: SONFNameLength bajtów

### Sekcja definicji SFX:
- nagłówek: $00,$00,d'SFX'
- numer SFXa: $00
- ilość bajtów przypadających na nazwę (stała SFXNameLength)
- nazwa: SFXNameLength bajtów
- całkowita długość SFXa (word): zmienna SFXLen*2
- dane SFXa: SFXLen*2 bajtów

### Sekcja definicji KEY-NOTE:
- nagłówek: $00,$00,d'KEY'
- numer SFXa dla którego jest tablica KEY-NOTE: $00
- wielkość: $40 (64 dźwięki)
- tablica 64 wartości nut

### Sekcja definicji TAB:
- nagłówek: $00,$00,d'TAB'
- numer TABa: $00
- ilość bajtów przypadających na nazwę (stała TABNameLength)
- nazwa: TABNameLength bajtów
- całkowita długość TABa: zmienna TABLen*2
- dane TABa: TABLen*2 bajtów

### Sekcja definicji SONG:
- nagłówek: $00,d'SONG'
- ilość linii SONG
- dane SONG: ilość lini SONG*4
- prędkość: tact, beat, line per bear
