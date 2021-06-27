[TOC]

# Rodzaje modulacji

Jest to efekt wdrożenia inspiracji @marok, która pozwala na ustalenie dla pojedynczego SFXa w jaki sposób, będzie brana pod uwagę wartość MOD/VAL

Obrałem ten kierunek i utworzyłem definicje czterech trybów:

- *HFD - High Frequency Divider*
- *MFD - Middle Frequency Divider*
- *LFD/NLM - Low Frequency Divider/Note Level Modulation*
- *DSD - Direct Set Divider*

## Co one oznaczają?

### HFD - High Frequency Divider
Szerokie spektrum modulacji dzielnika częstotliwości w zakresie +/-127.  
Nie ma możliwości zapętlenia SFXa  
Pełna zgodność wsteczna z pierwotnym silnikiem SFX

### MFD - Middle Frequency Divider
Średnie spektrum modulacji.  
Zakres modulacji +/-64 od podstawy dźwięku  
Możliwość zapętlenia SFXa

### LFD - Low Frequency Divider
Niskie spektrum modulacji.  
Zakres modulacji +/-32 od podstawy dźwięku.  
Możliwość zapętlenia SFXa

### NLM - Note Level Modulation
Modulacja na poziomie nuty (pół tonów)  
Zakres modulacji +/-32 pół tony w odniesieniu do nuty bazowej (tej umieszczonej w TABie)  
Możliwość zapętlenia SFX

### DSD - Direct Set Divider
Bezpośrednia wartość dzielnika częstotliwości.  
Zakres od 0 do 255  
Brak możliwości zapętlenia.  
Stała, maksymalna długość SFXa (128 punktów obwiedni)

## Co kryje się pod pojęciami: dzielnik częstotliwości oraz modulacja?

### Dzielnik częstotliwości

Jest to drugi parametr instrukcji SOUND w Basicu. Jest ona odpowiedzialna za wysokość generowanego przez POKEY dźwięku. Im ta wartość jest większa, tym częstotliwość jest niższa, zgodnie ze wzorem:
$$
f_{out}=\frac{POKEY_{freq}}{freq_{div}}
$$


~~~
f_out - częstotliwość wyjścowa
POKEY_freq - wartość podstawy częstotliwości pracy POKEYa (danego kanału)
freq_div - wartość dzielnika częstotliwości
~~~

### A dlaczego modulacja?

Wartość dzielnika częstotliwości będzie ulegała zmianom, poprzez parametr MOD/VAL. W większości, modulacja ta odbywa się względnie, tzn. dla wartość bazowej określanej w TAB (niezależnie, czy będzie to nuta, czy też wartość bezpośrednia dzielnika) jej wartość jest zmieniana o wartość parametru MOD/VAL, która może przyjmować wartości dodatnie oraz ujemne. Ostatecznie, wartość wynikowa staje się wartością bazową.
$$
f_{base}=f_{base}+MOD_{val}
$$

## Definicje MOD/VAL
Dla poszczególnych rodzajów modulacji, istnieją różne formy jej zapisu i interpretacji. Najlepiej to przedstawi zapis binarny, gdzie wyraźnie widać, jakie bity odpowiadają za funkcje, a jakie za wartość.

I tak, dla:

### High Frequency Modulation

~~~
%00000000 - brak modulacji (blank)
%0xxxxxxx - zwiększenie dzielnika częstotliwości o wartość określoną w bitach 'x'
%1xxxxxxx - zmniejszenie dzielnika częstotliwości o wartość określoną w bitach 'x'
%10000000 - koniec definicji SFX (End Of SFX)
~~~

### Middle Frequency Modulation

~~~
%00000000 - brak modulacji
%00xxxxxx - zwiększenie dzielnika częstotliwości o wartość 'x'
%01xxxxxx - zmniejszenie dzielnika częst. o wartość 'x'
%1xxxxxxx - skok do pozycji określonej w bitach 'x' (w obrębie SFXa)
%10000000 - koniec definicji SFX
~~~

### Low Frequency Modulation/Note Level Modulation*

~~~
%00000000 - brak modulacji
%000xxxxx - zwiększenie dzielnika częst. o wartość 'x'
%001xxxxx - zmniejszenie dzielnika częst. o wartość 'x'
%010xxxxx - podniesienie nuty o 'x' półtonów
%011xxxxx - obniżenie nuty o 'x' półtonów
%1xxxxxxx - skok do pozycji określonej w bitach 'x' (w obrębie SFXa)
%10000000 - koniec definicji SFX
~~~

### Direct Set Divider

~~~
%xxxxxxxx - wartość bezpośrednia rejestru dzielnika częstotliwości
~~~

## Ważne rzeczy

### Zapis wartości ujemnej

Wartość ujemna musi być liczbą przeciwną, tzn.
$$
Value_{opposite}=256-Value
$$


Jest to wymagane, gdyż odejmowanie bazuje na dodawaniu liczb przeciwnych w modulo 256.
$$
result=(base_{value}+Value_{opposite})\ mod\ 256
$$
Odejmowanie to dodawanie przeciwnej liczby do wartości bazowej.
Ośmiobitowy rejestr (z natury działa w modulo 256) ulega przepełnieniu, ale jest to ignorowane, a pozostała wartość w tym rejestrze jest mniejsza o wartości `Value`.
