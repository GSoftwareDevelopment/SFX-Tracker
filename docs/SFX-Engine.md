# SFX ENGINE

[TOC]

## Rejestry sprzętowe

W głównej procedurze silnika SFX, rejestry sprzętowe mają przypisaną konkretną funkcję:

| Rejestr | Opis                                         |
| :-----: | :------------------------------------------- |
|    X    | aktualny offset kanału                       |
|    Y    | offset w definicji SFX, TAB lub tablicy SONG |
|    A    | Rejestr ogólnego przeznaczenia               |

### Ważne sprawy

#### Własne rozszerzenia silnika SFX

Jeżeli chcesz rozszerzyć funkcjonalność silnika, musisz zadbać o przechowania wartości rejestrów sprzętowych, celem ich użycia. Do tego celu warto wykorzystać tymczasowe rejestry _regTemp i _regTemp2, które są ulokowane na zerowej stronie.

#### Sekcje modulatora

Tworząc własny rodzaj modulatora, warto pamiętać o parametrach wyjściowych sekcji.

Musi ona zwracać wartość dzielnika częstotliwości w rejestrze A.

## Rejestry programowe

### Rejestry na stronie zerowej

#### Rejestry SFX Tick-Loop

Te rejestry używane są tylko na potrzeby pętli i przechowują informacje dotyczące aktualnie przetwarzanego dźwięku.

| Nazwa rejestru |  Adres  | Opis                                      |
| :------------- | :-----: | :---------------------------------------- |
| sfxPtr         | $F3,$F4 | wskaźnik do definicji SFX                 |
| chnNoteOfs     |   $F5   | offset tablicy nut SFXa ($00,$40,$80,$C0) |
| chnNote        |   $F6   | numer nuty                                |
| chnFreq        |   $F7   | wartość dzielnika częstotliwości SFXa     |

Poniższe rejestry są dostępne w zależności od zastosowanych warunków kompilacji silnika SFX.

| Nazwa rejestru | Adres | Opis               |
|:--------------|:-------:|:-----------------------------|
| chnMode       | $F8    | Tryb modulacji SFXa |
| chnMod        | $F9    | Wartość modulacji* parametr MOD/VAL definicji |
| chnCtrl       | $FA    | Wartość zniekształcenia i głośności |

_*_ w większości przypadków zawiera funkcję oraz wartość. Przeważnie najstarsze bity określają funkcję, zaś reszta jest parametrem (patrz [Denicje MOD/VAL](./Moduators.md#definicje-mod-val) )

#### Rejestry tymczasowe

| Nazwa rejestru | Adres | Opis                                          |
| :------------- | :---: | :-------------------------------------------- |
| _regTemp       |  $FB  | wykorzystywany w pętli przetwarzania SFX      |
| _regTemp2      |  $FC  | wykorzystywany w pętli przetwarzania TAB/SONG |

### Rejestry kanałów

Jest to tablica opisująca stan wszystkich 4-ech kanałów, jakie wspiera silnik SFX.

| Nazwa rejestru | względny adres | opis                                      |
| :------------- | :------------: | :---------------------------------------- |
| sfxPtr         |    chnOfs+0    | Wskaźnik definicji SFXa                   |
| chnNoteTabOfs  |    chnOfs+2    | Ofset dla tablicy nut przypisanej do SFXa |
| chnOfs         |    chnOfs+3    | Ofset definicji SFXa                      |
| chnNote        |    chnOfs+4    | aktualna wartość nuty                     |
| chnFreq        |    chnOfs+5    | aktualna wartość dzielnika częstotliwości |

Poniższe rejestry są dostępne w zależności od zastosowanych warunków kompilacji silnika SFX.

| Register name | relative addr | Description                                  |
| :------------ | :-----------: | :------------------------------------------- |
| chnMode       |   chnOfs+6    | aktualny tryb modulacji SFXa                 |
| chnModVal     |   chnOfs+7    | aktualna wartość MOD/VAL definicji SFXa      |
| chnCtrl       |   chnOfs+8    | aktualna wartość zniekształcenia i głośności |
| trackOfs      |   chnOfs+10   | aktualny ofset ścieżki SONG                  |
| tabPtr        |   chnOfs+12   | wskaźnik do definicji TABa                   |
| tabOfs        |   chnOfs+14   | ofset definicji TABa (wskazuje wiersz TABa)  |
| tabRep        |   chnOfs+15   | licznik pętli dla funkcji REPEAT             |

Wymagane miejsce dla rejestrów: 64 bajty (dużo, ale w dowolnym miejscu pamięci RAM)

## Biblioteka MAD Pascal `SFX_Engine`

### Stałe

### Zmienne

### Procedury i funkcje

- __INIT_SFXEngine__

  `INIT_SFXEngine();`

​	Inicjacja silnika. Ustawia początkowe wartości dla POKEYa oraz rejestrów kanałów.

- __SFX_Start__

  `SFX_Start();`

​	Włącza pracę silnika. Wykonuje procedurę SFX_Start oraz inicjuje przerwanie VBLANK.

- __SFX_ChannelOff__

  `SFX_ChannelOff(channel:byte);`

​	Wyłącza odtwarzanie SFXa w podanym kanale dźwiękowym.

- __SFX_Off__

  `SFX_Off();`

  Wyłącza odtwarzanie we wszystkich kanałach dźwiękowych.

  Ta procedura jest "odpalana" też przy wyłączaniu silnika SFX (procedura `SFX_End()`)

- __SFX_Note__

  `SFX_Note(channel,note,SFXId:byte);`

  `channel` - kanał dźwiękowy na którym będzie grany SFX

  `note` - numer nuty (wartość od 0-63)

  `SFXId` - Index SFX (numer definicji SFX)

  Odtwarza wybrany SFX w podanym kanale z częstotliwością podanej nuty.

- **SFX_Freq**

  `SFX_Freq(channel,freq,SFXId:byte);`

  Procedura podobna w działaniu do `SFX_Note` z tą różnicą, że ustawia zadaną częstotliwość (dzielnik częstotliwości)

- **SFX_SetTAB**

  `SFX_SetTAB(channel,TABId:byte);`

  Ustawia rejestry kanału `channel` do odtwarzania pojedynczego TABa.

  Nie powoduje jego automatycznego odtwarzania o ile, nie jest już jakiś odtwarzany.

  Wartość `TABId` powyżej 64 powoduje wyłączenie odtwarzania w danym kanale.

- **SFX_PlayTAB**

  `SFX_PlayTAB(channel,TABId:byte);`

  Działanie procedury jest podobne do `SFX_SetTAB` z tą różnicą, że włącza odtwarzanie.

- **SFX_PlaySong**

  `SFX_PlaySONG(startPos:byte);`

  Pozwala włączyć odtwarzanie z listy SONG od zadanej pozycji. 

- __SFX_End__

`SFX_End();`

Wyłącza pracę silnika SFX. Przywraca poprzedni wektor przerwania.

## Dostosowanie silnika SFX

Konstrukcja SFX-Engine pozwala na dostosowanie do własnych potrzeb za pomocą dyrektyw kompilacji warunkowej. Pozwalają one na wybranie rozwiązań, które są wykorzystywane w programie, skracając kod wynikowy silnika.

### Etykiety kompilacji warunkowej

- `SFX_SWITCH_ROM`

  Etykieta pozwala na swobodny dostęp do pamięci RAM "ukrytej" pod ROM-em. Współpracuje z etykietą `ROMOFF` dostępną z poziomu **MAD Pascala**, która zezwala na wykorzystanie tej pamięci.

- `SFX_previewChannels`

  Etykieta generuje niewielki kod, dający możliwość wglądu w aktualny stan modulatora oraz wartości zniekształceń i głośności. Przenosi on z głównej pętli (*SFX_TICK*) stan rejestrów do rejestrów kanałów.

  Dodatkowe informacje umieszczane są w rejestrach kanałów pod offsetami 6 oraz 7 każdego kanału.

  Brak obecności tej etykiety, zwalnia dwa bajty ze strony zerowej z użytku przez silnik SFX.

- `DONT_CALC_ABS_ADDR` & `DONT_CALC_SFX_NAMES`

  Użycie tych etykiet wyłącza przeliczanie adresów względnych na absolutne w silniku SFX.

  Przeliczanie to odbywa się przez zsumowanie następujących wartości:

  - adres względny definicji (SFX/TAB)
  - długość nazwy dla definicji. W przypadku SFX wartość ta wynosi 14 bajtów, dla TAB jest ona równa 8.
  - adres bazowy definicji. Jest on określany stałą DATA_ADDR

  Właściwy (absolutny) adres można przedstawić wzorami:
  $$
  SFX_{addr}=DATA_{addr}+SFX_{ptr}+SFX_{namelength}
  $$
  
  $$
  TAB_{addr}=DATA_{addr}+TAB_{ptr}+SFX_{namelength}
  $$
  

- `SFX_SYNCAUDIOOUT`

  Użycie tej etykiety powoduje zastosowanie buforu dla rejestrów POKEYa, którego zawartość jest wysyłana na zakończenie działania całej pętli silnika SFX.

  Zalecane jest jego stosowanie, gdyż pętla może mieć różne czasy wykonywania, które mogą być odczuwalne dla ludzkiego ucha.

  Koszt użycia to 8 bajtów na stronie zerowej i kilkanaście dodatkowych bajtów kodu.

  

- `USE_MODULATORS`

  Etykieta zezwalająca na selektywne zdefiniowanie modulatorów.

  Brak definicji powoduje [tryb pracy bez sekcji modulatorów]().

  Użycie modulatorów zajmuje 1 bajt na stronie zerowej.

  Wraz z tą etykietą powinno się wybrać przynajmniej jedną sekcje modulatora, za pomocą definicji etykiet:

  - `DFD_MOD`
  - `LFD_NLM_MOD`
  - `MFD_MOD`
  - `HFD_MOD`

### Tryb pracy bez sekcji modulatorów

To najprostsza wersja silnika SFX.

Definicja SFXa w tym trybie, zajmuje maksymalnie 127 bajtów (1 bajt na krok obwiedni) i opisuje tylko parametry dotyczące zniekształcenia (starszy nibbel bajtu) i głośności (młodszy nibbel bajtu).

Długość definicji zawarta jest w 6 młodszych bitach definicji rodzaju modulacji SFXa (tablica `SFXModModes`). 7 bit wykorzystany jest do wskazania wykorzystania tego trybu pracy.

> W trybach modulacji, koniec definicji SFXa sprawdzany jest w trakcie jego wykonywania i definiuje go funkcja SFX-STOP, stąd brak konieczności zapisywania długości definicji w trybach modulacji.
>
> Rozwiązanie to minimalizuje zużycie pamięci, jakie byłoby potrzebne na tablicę przechowującą długości definicji, jednak, wiąże się to ze **zwróceniem szczególnej uwagi** na wykorzystanie funkcji `JUMP TO`, gdyż nie ma możliwości szybkiego sprawdzenia zakresu skoku w trakcie jego odtwarzania przez silnik SFX.
>
> Wykonanie skoku poza obszar definicji SFXa może skutkować niekontrolowanym zachowaniem silnika, a nawet, zawieszeniem się komputera.
