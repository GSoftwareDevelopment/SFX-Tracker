## Założenia formatu pliku:
- Podział na sekcje, gdzie każda posiada 5-cio bajtowy, unikatowy nagłówek
- Kolejność ułożenia bloków nie może mieć znaczenia

### Sekcja główna:

| Nazwa          |           | ilość bajtów | wartość | opis                   |
| -------------- | --------- | :----------: | :-----: | ---------------------- |
| nagłówek       | header    |      5       | `SFXMM` |                        |
| wersja         | version   |      1       |   $11   | $11 oznacza wersję 1.1 |
| długość tytułu | title_len |      1       |   32    |                        |
| tytuł          | title     |      32      |         |                        |



- nagłówek: `'SFXMM'`
- wersja programu: `$10` (stała `SFXMM_VER1_0` oznacza v1.0)
- ilość bajtów przypadających na tytuł (stała `SONGNameLength`)
- tytuł: `SONGNameLength` bajtów

### Sekcja definicji SFX:

| Nazwa                     |           | ilość bajtów | wartość        | opis                                                         |
| ------------------------- | --------- | ------------ | -------------- | ------------------------------------------------------------ |
| nagłówek                  | header    | 5            | `$00,$00,'SFX` |                                                              |
| numer SFXa                | sfxid     | 1            | od 0 do 63     |                                                              |
| rodzaj modulacji MOD MODE | modMode   | 1            | od 0 do 3      |                                                              |
| tablica nut               | noteTabId | 1            | od 0 do 3      |                                                              |
| ilość danych              | len       | 2            |                |                                                              |
| dane                      | data      | len          |                | dane zawierają również nazwę SFXa, na którą przypada zawsze 14 bajtów. |

### Sekcja Tablicy nut:

| Nazwa       |           | ilość bajtów |   wartość    | opis |
| :---------- | --------- | :----------: | :----------: | ---- |
| nagłówek    | header    |      5       | `$00,'NOTE'` |      |
| tablica nut | noteTabId |      1       |  od 0 do 3   |      |
| dane        | data      |      64      |              |      |

### Sekcja definicji TAB:

| Nazwa        |        | ilość bajtów |     wartość     | opis                                                         |
| :----------- | ------ | :----------: | :-------------: | ------------------------------------------------------------ |
| nagłówek     | header |      5       | `$00,$00,'SFX'` |                                                              |
| numer TABa   | tabId  |      1       |   od 0 do 63    |                                                              |
| ilość danych | len    |      2       |                 |                                                              |
| dane         | data   |     len      |                 | zawierają także nazwę TABa na którą przypada zawsze 8 bajtów |

### Sekcja definicji SONG:

|              |        |      |              |      |
| ------------ | ------ | ---- | ------------ | ---- |
| nagłówek     | header | 5    | `$00,'SONG'` |      |
| ilość danych | len    | 2    |              |      |
| dane         | data   | len  |              |      |
|              |        |      |              |      |
