# Wykorzystanie pamięci przez silnik SFX

|                                                              |          **hex** |         **dec** |
| ------------------------------------------------------------ | ---------------: | --------------: |
| **Kod silnika**                                              |         **036C** |         **876** |
| **Pamięć robocza wykorzystywana przez silnik:**              |          **154** |         **340** |
| Strona zerowa<br />- bufor synchronizacji audio<br />- rejestry robocze | <br />08<br />0C | <br />8<br />12 |
| Rejestry kanałów                                             |               40 |              64 |
| **Pamięć stała**                                             |                  |                 |
| _*_ Tablice nut<br />*4 definiowane tablice po 64 nuty*      |             0100 |             256 |
| _**_ SFXy (bez obwiedni)<br />na każdą definicję (wskaźnik, tryb modulacji, tablica nut) |         <br />04 |         <br />4 |
| _**_ TABy (bez danych)<br />na każdą definicję               |         <br />02 |         <br />2 |
| _**_ SONG<br />na każdy wiersz                               |               04 |               4 |
| **API dla MadPascal (code)**                                 |         **00C2** |         **192** |
| API memory usage<br />*każda zmienna deklarowna jest przez `absolute`* |            **0** |           **0** |
|                                                              |                  |                 |

_*_ wymagane wyrównanie do pełnej strony;

_**_  nie jest wymagane wyrównanie do strony, jednak należy się liczyć z dodatkowymi cyklami przy granicy stron

