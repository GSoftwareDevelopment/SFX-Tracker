# SFX_Engine UNIT

## Co oferuje?

`INIT_SFXEngine(_SFXModModes, _SFXList, _TABList, _SONGData:word);`

Inicjacja silnika. Okresić trzeba mapę pamięci:

`_SFXModModes` - tablica trybów modulacji dla poszczególnych SFXów

`_SFXList` - tablica wskaźników (na razie względne) dla definicji SFXów

`_TABList` - tablica wskaźników (na razie względne) dla definicji TABów

`_SONGData` - tablica układów TABów

### Dlaczego wartości względne?

Gdyż nadal pracuje z użyciem HEAP, a on operuje na tablicy `array[0..0] of byte`, której przypisuje (przez `ABSOLUTE`) miejsce w pamięci.

`SetNoteTable(_note_val:word);`

Ustawia adres `_note_val` dla definicji tablicy nut.
Jej układ jest liniowy, tzn. 0 to nuta C-0, 1 to C#0, 2 to D-0, itd...

`SFX_Start();`

Rozpoczęcie pracy silnika SFX.

`SFX_ChannelOff(channel:byte);`

Wyłącza odtwarzany w podanym kanale `channel`, aktualny SFX.

`SFX_Off();`

Tu podobnie jak w `SFX_ChannelOff`, tylko dla wszystkich kanałów.
Ta procedura jest odpalana podczas wyłączania silnika SFX (procedure `SFX_End()`).

`SFX_Note(channel,note,modMode:byte; SFXAddr:word);`

`channel` - kanał na którym będzie odtwarzany SFX

`note` - nuta (patrz inicjacja tablicy nut `SetNoteTable`)

`modMode` - tryb modulacji. Silnik nie czyta tych danych samodzielnie.

`SFXAddr` - adres definicji SFXa

Problem jest taki, że w definicjach (SFXów oraz TABów) znajdują się na samym początku nazwy, które trzeba ominąć, stąd w definicji adres w pamięci komputera, a nie numer SFXa

`SFX_End();`

Wyłączenie silnika SFX.
