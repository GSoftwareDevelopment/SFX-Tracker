# Prototyp liniowego zapisu utworu w SFXMM

## Typowa notacja

Każdy wiersz zawiera informacje dla wszystkich ścieżek (nawet tych niemodyfikowanych)

Tylko pierwsza ścieżka może zawierać komendę i w takiej sytuacji, następne ścieżki zawierają parametry dla tej komendy.

~~~
	wiersz    ścieżki                          Adres  zapis w pamięci
		00: 00 01 -- --							0000: 00 01 40 40
		01: R: 01>00							0004: 84 01 00 00
		02: -- -- -- 03							0008: 40 40 40 03
		03: -- -- -- 04							000c: 40 40 40 04
		04: -- -- 02 03							0010: 40 40 02 03
		05: -- -- -- 04							0014: 40 40 40 04
		06: -- -- -- ==							0018: 40 40 40 7f
		07: -- -- -- --							001c: 40 40 40 40
		08: J> 00								0020: 82 00 00 00
		09: END SONG							0024: ff 00 00 00
~~~

## Zapis liniowy

Przedstawione rozwiązanie, nie zawiera jednoznacznych wierszy czy ścieżek.

Składa się z znacznika czasowego do ktorego przypisana jest komenda.

~~~
        *ZCz  Komenda                          Adres  zapis w pamięci
		0000: SetTracks %1100 00 01				0000: 00 00 8c 00 01
		0080: SetTracks %0001 03				0005: 00 80 81 03
		00c0: SetTracks %0001 04				0009: c0 00 81 04
		0100: SetTracks %0011 02 03				000d: 00 01 83 02 03
		0140: SetTracks %0001 04				0013: 40 01 81 04
		0180: TrackOff %0001					0017: 80 01 F1
		0200: JumpTo $0000						001a: 00 02 A0 00 00
		0280: EndSsong							001f: 80 02 F0
~~~
## _*_ ZCz - znacznik czasowy

Wykonywanie komend odbywa się w ściśle określonym czasie

Cykl, jest to jeden przebieg procedury odtwarzającej `SFX_Tick` i jest on powtarzany 50 razy na seundę (dla systemu TV-PAL) Tempo natomiast, to ilość cykli składających się na jeden wiersz TABa.



## Komendy

Komendy składają się z minimum jednego bajtu, który może zawierać tzw. składnik opisujący, oznaczający, do których ścieżek będzie odnosiło się polecenie.

| Nazwa komendy |     hex     |     bin     |  składowa  | parametry |
| ------------- | :---------: | :---------: | :--------: | :-------: |
| SetTracks     | `81`...`8F` | `1000 xxxx` | bity 3...0 | od 1 do 4 |
| TPTrack       | `91`...`9F` | `1001 xxxx` | bity 3...0 | od 1 do 4 |
| JumpTo        |    `A0`     | `1010 0000` |     -      |     1     |
| Repeat        |    `B0`     | `1011 0000` |     -      |     2     |
| SetTempo      |    `C0`     | `1100 0000` |     -      |     1     |
| SongEnd       |    `F0`     | `1111 0000` |     -      |     -     |
| TrackOff      | `F1`...`FF` | `1111 xxxx` | bity 3...0 |     -     |

Po komendzie mogą występować parametry, których liczba zależy od funkcji komendy.
W przypadku komend zawierających "składową" - choć nie jest to regułą - liczba parametrów jest opisana przez poszczególne bity "składowej".



### SetTracks

Ustawia TAB-y w określonych ścieżkach, które są wskazywane przez stan bitów 0...3 (części składowej komendy) odpowiadające ścieżką #1...#4.

**Parametry**

- jeden bajt, dla każdej wybranej ścieżki - reprezentuje numer TABa.



### SongEnd

Zatrzymuje licznik cykli, a rejestry TABOfs, SFXOfs są ustawiane na $FF, co powoduje, że utwór nie jest przetwarzany.

Mówiąc krótko, zatrzymuje odtwarzanie utworu

**Ta komenda nie posiada parametrów.**

### TrackOff

Wyłącza przetwarzanie ścieżek określonych w części składowej komendy.

**Ta komenda nie posiada parametrów.**

### Repeat

Komenda ta ustawia znacznik pętli (jeśli nie jest ustawiony) i wykonuje skok pod podany adres. Przy następnym wywołaniu, zmniejsza licznik pętli (znacznik) i wykonuje skok, aż do momentu, gdy licznik osiągnie zero.

Po wyzerowaniu markera, wykonywany jest rozkaz, zapisany jako trzeci parametr komendy

**Parametry**

- bajt określający liczbę powtórzeń
- adres skoku (dwa bajty) - jest wartością absolutną, wskazującą miejsce w pamięci komputera
- komenda do wywołania, gdy licznik będzie równy zero.

Długość tej komendy, zalezna jest od trzeciego parametru, gdyż jego zapis wyróżnia tylko brak znacznika czasowego.

### JumpTo

Wykonuje skok pod podany adres definicji utworu.

**Parametry**

- adres skoku (dwa bajty) - jest wartością absolutną, wskazującą na miejsce w pamięci komputera



### SetTempo

Ustawia tempo odtwarzania utworu.

**Parametry**

- Jednobajtowa wartość określająca tempo. Zakres wartości od 0 do 127.



### TPTrack

Ustawia wartość transpozycji na wymienionych ścieżkach.

**Parametry**

- jeden bajt dla każdej ustawionej (w bitach 0 do 3) ścieżki, reprezentuje wartość półtonu transpozycji od -63 do +63 

## Krótko o pozycjonowaniu

W rozkazach **JumpTo** oraz **Repeat** występuje parametr *pozycja skoku*. Jest on opisany jako wartość absolutna odnosząca się do konkretnego miejsca w pamięci komputera.

Bardziej trafnym byłoby zastosowanie wartości względnej, która odnosiłaby się do adresu bazowego w którym rozpoczynają się dane utworu. Jednak takie rozwiązanie, wymaga przeliczania adresu za każdym razem, gdy jest on wymagany.

W przypadku wartości absolutnej, taka potrzeba nie zachodzi i wystarczy tylko przepisać wartość miejsca skoku do wskaźnika.

Ostatecznie, jest to bardziej kwestia, jak często taki adres będzie wymagany w procedurze analizy.

