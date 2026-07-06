# Rage Racer decompilation guide

Ten dokument opisuje praktyczny workflow dekompilacji w tym repo oraz
troubleshooting przypadkow, ktore najczesciej blokuja funkcje. Celem jest
matching: kod C musi budowac ten sam output, a `INCLUDE_ASM` zostaje tylko tam,
gdzie funkcja nie jest jeszcze zdekompilowana.

## Zasady bazowe

- Matching jest wymagany. Jesli zmiana psuje matching, najpierw naprawiamy
  matching.
- Nie wrzucamy nonmatching C do `src/` jako zamiennika dzialajacego stubu.
- Duze eksperymenty i nieudane wersje trzymaj w ignorowanym miejscu, np.
  `scratch/`.
- Commit robimy dopiero po przejsciu `make check`.
- Nie zgadujemy struktur "na ladnie". Najpierw zbieramy dowody z offsetow,
  szerokosci load/store i uzycia w kilku funkcjach.
- Nie przepisujemy calej funkcji na inline asm. Dopuszczalne sa male,
  lokalne sztuczki matchingowe, kiedy C nadal opisuje algorytm.

## Standardowy workflow funkcji

1. Wybierz funkcje i zapisz jej rozmiar, plik oraz obecny stub.
2. Zbierz target asm i diff dla pierwszej proby.
3. Ustal klase problemu: text, rodata, reloc, sdata/sbss, link order.
4. Najpierw napraw typy, prototypy, signedness, kolejnosc wyrazen i zakres
   zycia zmiennych.
5. Dopiero potem probuj register pinning, local asm barriers albo drobne
   matching notes.
6. Po kazdej sensownej zmianie uruchom waski check/diff, a na koncu
   `make check`.
7. Jesli funkcja jest za duza, wytnij najmniejszy powtarzalny blocker i
   zapisz notatke wedlug szablonu nizej.

Minimalny raport z nieudanej proby:

```text
Symbol:
Region/object:
Build command:
Target frame/save set:
First different instruction:
Diff class:
Suspected cause:
What was confirmed:
Next attempt:
```

## Pierwszy diff: szybka triage

| Objaw | Najbardziej prawdopodobna przyczyna | Pierwsze kroki |
| --- | --- | --- |
| Extra `addiu sp` albo inny save set | Za duzo live locals, bledny model stosu, custom ABI potraktowane jak zwykla funkcja | Ogranicz scope, rozbij temporaries, sprawdz prolog/epilog |
| `lw` zamiast `lb/lbu/lh/lhu` | Zly typ pola albo signedness | Zrob offset map i popraw storage type |
| Extra `sll/sra` | Narrow signed operation albo cast w zlym miejscu | Przesun cast, rozdziel storage type od operation type |
| Te same instrukcje, inne `sN` | Regalloc/lifetime/order locals | Zmien kolejnosc deklaracji i zakresy, pin dopiero na koncu |
| Rozjechany delay slot | Inny ksztalt control flow albo kolejnosc wyrazen | Probuj odwrocic warunek, early return, shared tail |
| `lui/addiu` zamiast `$gp` | Zla sekcja albo `-G`/sdata/sbss | Sprawdz symbol section i realne flagi obiektu |
| Text pasuje, rodata nie | Jump table, string alignment, section order | Sprawdz rodata przy funkcji i sasiednie symbole |
| Text pasuje, reloc nie | Alias symbolu, gp/linker, entrypoint | Sprawdz adres call targetu i symbol boundaries |

## Toolchain truth

Dla Rage Racer zrodlem prawdy sa lokalne skrypty i Makefile, nie zalozenia z
zewnatrz.

- `tools/scripts/cc.sh` uruchamia GCC, potem `maspsx.py`, potem assembler.
- Domyslny `RAGE_CC1_VERSION` to obecnie `2.6.3`, ale obiekty moga miec
  per-object override w Makefile.
- Typowy zestaw flag obejmuje `-mcpu=3000`, `-mgas`, `-gcoff`, `-O2`, `-G0`
  i `-funsigned-char`.
- Jesli YAML albo scratch notes mowia cos innego niz Makefile, sprawdz
  Makefile i rzeczywisty command line.
- Przy podejrzeniu runtime/Psy-Q najpierw sprawdz, czy funkcja nie jest
  wariantem bibliotecznym, zanim zaczniesz ja dekompilowac ad hoc.

Typowe problemy maspsx/assembler:

- `--aspsx-version` zmienia ekspansje niektorych pseudoinstrukcji.
- `.sdata/.sbss` i `%gp_rel` sa czule na `-G`, symbol section i wersje
  assemblera.
- `div/rem`, `mflo/mfhi`, `$at` i `li/la` moga dawac rozne sekwencje w
  zaleznosci od wersji.
- Przy GTE i COP2 zwracaj uwage na hazard nopy oraz instrukcje `mfc2/cfc2`.

## Register matching

Register mismatch zwykle nie oznacza, ze trzeba od razu pinowac rejestry.
Kolejnosc prob:

1. Sprawdz prototypy wywolywanych funkcji.
2. Sprawdz signedness i szerokosc typow.
3. Zmien kolejnosc deklaracji locals.
4. Ogranicz lifetime przez mniejsze scope albo dodatkowe bloki.
5. Przeksztalc `if`, `while`, `for`, early return i shared tail.
6. Dopiero wtedy uzyj `register ... asm("$sN")`, lokalnie i z komentarzem.

Dobra zasada: pinning ma stabilizowac ostatnie 5-10% diffu, nie maskowac
niezrozumiana funkcje.

Male inline asm jest akceptowalne tylko jako lokalna sztuczka matchingowa,
np. pojedyncza instrukcja albo barrier. Komentarz powinien zaczynac sie od
`Match note:` i wyjasniac, jaki diff naprawia.

### func_80032098: track setup register schedule

`scratch/decomp-work/func_80032098/` pokazuje funkcje, w ktorej semantyka jest
rozpoznana, a blockerem zostal gorny setup rejestrow. Target chce:

```text
mfhi    a3
load    a2,D_8009E688
offset  v1 = index * 24
cur     s1 = v1 + a2
lhu     s2,0x16(s1) jako filler po lhu 0(s1)
offset  v0 = nextIndex * 24
jal     func_8001A530
next    s0 = v0 + a2 w delay slot
```

Preferowany eksperyment C:

```c
register TrackPointWindow *base asm("$6");
register TrackPointWindow *cur asm("$17");
register TrackPointWindow *next asm("$16");
register u32 segmentLengthRaw asm("$18");
register s32 nextIndex asm("$7");
register s32 curOff asm("$3");
register s32 nextOff asm("$2");
s32 argX, curX, argZ, curZ, angle;

index = func_80030EB4(arg0, arg0->field_30);
nextIndex = (index + 1) % D_8009E6A8;

base = (TrackPointWindow *)D_8009E688;
asm("" : "=r"(base) : "0"(base)); /* Match note: materialize base in $a2 before current offset. */

curOff = (index * 3) << 3;
cur = (TrackPointWindow *)(curOff + (s32)base);

argX = arg0->field_0;
curX = (u16)cur->x;
segmentLengthRaw = cur->segmentLength;
vec[0] = argX - curX;

argZ = arg0->field_8;
curZ = (u16)cur->z;
vec[1] = 0;
vec[2] = argZ - curZ;

angle = cur->angle;
nextOff = (nextIndex * 3) << 3;
next = (TrackPointWindow *)(nextOff + (s32)base);
func_8001A530(&mtx, (0x1000 - angle) & 0xFFF);
```

Jesli zostaje tylko operand order dla `cur = curOff + base`, dopuszczalny jest
pojedynczy asm lever:

```c
asm("addu %0,%1,%2" : "=r"(cur) : "r"(curOff), "r"(base));
```

Nie uzywaj tego, jesli offset nie jest jeszcze w `$v1` albo base nie jest w
`$a2`; wtedy asm maskuje zly source order.

W tym repo wariant z `"=r"(base) : "0"(base)` jest wymagany, bo lokalny GCC
odrzuca `"+r"` w tym kontekscie. Ten scaffold domknal matching dla
`func_80032098` i `make check` przeszlo.

## Signedness i narrow types

Oddziel storage type od operation type. Pole moze byc `u8`, ale konkretna
operacja moze celowo robic signed compare po castowaniu.

Szybkie wskazowki:

- `lb` oznacza signed byte.
- `lbu` oznacza unsigned byte.
- `lh` oznacza signed halfword.
- `lhu` oznacza unsigned halfword.
- `slti` vs `sltiu` zwykle ujawnia signedness porownania.
- Sentinel `0xFF` czesto chce `u8`, a nie `char == -1`.
- Dodatkowe `sll/sra` czesto znaczy, ze cast jest w zlym miejscu.
- Dzielenie signed przez potege dwojki moze wymagac sekwencji z korekta znaku,
  nie zwyklego shiftu.

## Struktury i offset maps

Nie zaczynaj od pelnej struktury. Zacznij od mapy offsetow:

```text
Base:
Offset:
Width:
Signedness:
Read/write:
Confirming functions:
Before/after calls:
Notes:
```

Wiele funkcji w Rage operuje na pointerach do srodka wiekszego runtime state.
Wtedy pelna struktura moze pogorszyc matching, bo compiler zacznie hoistowac
loady, zmieni aliasing albo stack frame. Uzyj "window struct": malego widoku
na potwierdzony fragment layoutu.

Stride jest mocnym dowodem. Jesli petla idzie co `0x34`, to prawdopodobnie
masz tablice rekordow `0x34`, ale pola nadal trzeba potwierdzic load/store.

Nie typuj agresywnie, gdy:

- offset widac tylko w jednej funkcji,
- funkcja ma custom ABI,
- dane sa command streamem,
- kod robi raw copy/memcpy-like,
- ladna struktura psuje CSE, hoisting albo frame.

## Stack frame i fake locals

Extra stack frame czesto pochodzi z tego, ze C wymusza lokalne zmienne, ktorych
oryginalny kod nie materializowal. Najpierw sprawdz:

- czy temporary moze byc wyrazeniem inline,
- czy zmienna moze miec mniejszy scope,
- czy nie tworzysz adresowalnego local przez `&`,
- czy nie pomyliles struct copy z pointer passing,
- czy funkcja nie ma custom entrypointu bez standardowego prologu.

## Control flow i delay slots

Delay slot mismatch rzadko naprawia sie jednym typem. Probuj zmieniac ksztalt
control flow:

- odwroc warunek,
- zamien nested `if` na early return,
- sprawdz `do/while` vs `while`,
- rozdziel albo polacz shared tail,
- przenies expression order blizej target asm.

Jesli diff pokazuje poprawny kod, ale zla instrukcje w delay slot, patrz na
instrukcje bezposrednio przed branchem. Compiler czesto bierze slot z ostatniej
bezpiecznej operacji, nie z tego, co wyglada najczytelniej w C.

## Custom ABI i command handlers

Nie kazdy symbol jest zwykla funkcja C. W tym projekcie szczegolnie podejrzane
sa renderery, interpretery command streamow i handlery.

Objawy custom ABI:

- brak normalnego prologu,
- `ra` zapisywany w strukturze/kontekscie zamiast na stosie,
- handler dziedziczy `t*`, `s*` albo `a*` z petli nadrzednej,
- asm block ma kilka globalnych labeli,
- wejscie jest targetem jump table albo `jalr`,
- funkcja wyglada jak kilka malych handlerow sklejonych fallthrough.

Workflow:

1. Nazwij role rejestrow w petli nadrzednej.
2. Zidentyfikuj context pointer, stream pointer, output pointer, OT/scratch.
3. Spisz kontrakt: ktore rejestry handler dziedziczy, ktore modyfikuje,
   gdzie wraca.
4. Nie wymuszaj zwyklego prototypu z 6-8 argumentami, jesli asm pokazuje
   odziedziczone rejestry.
5. Najpierw zdekompiluj petle/dispatcher i male handlery osobno, zachowujac
   entrypointy.

Rage-specific podejrzenia:

- `func_800296B4` i `func_80029E50` wygladaja jak dispatchery zapisujace
  `ra` w kontekscie przed `jalr`.
- `func_80028DEC`, `func_80029340` i `func_80029EA8` maja persistent register
  roles typowe dla command/render loop.
- Handlery w okolicy `func_80029064`, `func_800293F0`, `func_80029458` moga
  nie miec normalnego prologu i dziedziczyc rejestry.

## Internal entrypoints i fallthrough

Call target nie zawsze wskazuje poczatek symbolu C. Rozrozniaj:

- zwykly branch target wewnatrz funkcji,
- alternate entrypoint,
- jump table target,
- fallthrough setup,
- symbol alias potrzebny tylko dla relokacji/call targetu.

Nie tworz aliasu, dopoki offset nie jest stabilny w mapie i relokacjach.
Przypadek do pilnowania: `func_80069568`, gdzie entrypoint wywolania moze byc
o kilka bajtow przesuniety wzgledem wygodnego symbolu C.

## GTE, scratchpad i hardware state

Przy GTE i scratchpad zwykly C moze wygladac dziwnie, bo oryginalny kod liczy
na state procesora albo konkretne hazardy.

Sprawdz:

- czy load/store idzie do scratchpad albo fixed hardware address,
- czy kolejnosc `mfc2/cfc2` wymaga nopa,
- czy macierze/wektory maja potwierdzony layout,
- czy funkcja nie miesza transformacji z command emission,
- czy temporary w C nie przesuwa instrukcji przez GTE boundary.

## Rodata, jump tables i sasiedztwo funkcji

Rodata pomaga ustalic pierwotne compilation units, ale trzeba ja traktowac
jako dowod pomocniczy.

Zbieraj:

- stringi bezposrednio po funkcji,
- jump table addresses,
- alignment przed i po rodata,
- relokacje do sasiednich funkcji,
- wspolne stale uzywane przez grupe funkcji.

Jesli text pasuje, ale rodata nie, nie zmieniaj losowo kodu. Najpierw sprawdz
czy jump table albo string nie nalezy do innej funkcji lub innego obiektu.

## Psy-Q i biblioteki

Przed dekompilacja malej funkcji o runtime'owym charakterze sprawdz, czy nie
jest to wariant Psy-Q/libgcc/runtime:

- nazwy i sekwencje w symbolach z innych decompow,
- typowe funkcje math, memory, controller, GPU/GTE,
- calling convention i register clobbers,
- rodata lub error stringi wskazujace SDK.

Jesli to biblioteka, lepiej dopasowac znany wariant niz tworzyc lokalny kod,
ktory przypadkiem matchuje tylko dla jednego obiektu.

## Case studies

### `func_8003F02C`

Problem wygladal jak kwestia compiler configu, ale matching przyszedl przez
uproszczenie C:

```c
func_8001A530(&mtx0, 0x800 - D_801E4354);
```

Wniosek: zanim zmienisz YAML/Makefile albo zaczniesz pinowac rejestry,
sprawdz, czy C nie tworzy sztucznych temporaries i nie zmienia schedule.

### `func_80032098`

Semantyka jest rozpoznana: funkcja bierze indeks punktu trasy, liczy nastepny
punkt, buduje wektor wzgledem aktualnego punktu, robi rotacje Y przez
`func_8001A530`, transformuje wektor i interpoluje pola aktualnego/nastepnego
rekordu. Obecny blocker to nie algorytm, tylko gorny register/schedule block.

Target chce:

```text
mfhi    a3
load    a2, D_8009E688
offset  v1 = index * 0x18
addu    s1, v1, a2
lhu     v0, 0(s0)
lhu     v1, 0(s1)
lhu     s2, 0x16(s1)
...
offset  v0 = a3 * 0x18
jal     func_8001A530
addu    s0, v0, a2
```

Najblizszy falsyfikowalny eksperyment:

```c
register TrackPointWindow *base asm("$6");
register TrackPointWindow *cur asm("$17");
register TrackPointWindow *next asm("$16");
register u32 segmentLengthRaw asm("$18");
register s32 nextIndex asm("$7");
register s32 curOff asm("$3");
register s32 nextOff asm("$2");
s32 argX, curX, argZ, curZ, angle;

nextIndex = (index + 1) % D_8009E6A8;
base = (TrackPointWindow *)D_8009E688;
asm("" : "+r"(base)); /* Match note: materialize base in $a2 before current offset. */

curOff = (index * 3) << 3;
cur = (TrackPointWindow *)(curOff + (s32)base);

argX = arg0->field_0;
curX = (u16)cur->x;
segmentLengthRaw = cur->segmentLength;
vec[0] = argX - curX;

argZ = arg0->field_8;
curZ = (u16)cur->z;
vec[1] = 0;
vec[2] = argZ - curZ;

angle = cur->angle;
nextOff = (nextIndex * 3) << 3;
next = (TrackPointWindow *)(nextOff + (s32)base);
func_8001A530(&mtx, (0x1000 - angle) & 0xFFF);
```

Sprawdz tylko zakres `mfhi` -> delay slot `func_8001A530`. Jesli ten fragment
matchuje, usuwaj piny pojedynczo i zostaw tylko minimalne `Match note:`. Jesli
`mfhi` nadal idzie do `$t0`, pin `nextIndex` albo jego lifetime jest pierwszym
problemem. Jesli `lhu s2,0x16(s1)` nie wpada jako filler, load segmentu musi
zostac bezposrednio po `cur->x`, przed skladowaniem `vec[0]`.
Jesli `li a1,0x1000` idzie za wczesnie, upewnij sie, ze nie ma osobnego
`angleBase` locala przed zapisami `vec`; angle load powinien byc po `vec[2]`.
Jesli zostaje tylko operand order `addu s1,s1,a2` kontra `addu s1,v1,a2`,
dopuszczalny jest pojedynczy asm dla pointer add, ale tylko gdy `curOff` jest
juz w `$v1`, a `base` w `$a2`.

### `func_8003F0F8`

Funkcja operuje na `D_801E4FB8`; widac stride okolo `0x34` i pola m.in.
`0x0`, `0x8`, `0xC`, `0xE`, `0x10..0x28`. Proba z ladna struktura moze
powodowac hoisting i inny stack frame.

Wniosek: tu najpierw offset map, potem maly window struct. Pelny model danych
dopiero po potwierdzeniu w sasiednich funkcjach.

### Renderer/dispatcher okolo `80028xxx-80029xxx`

Kilka funkcji w tym obszarze wyglada jak command interpreter. Dziedziczone
rejestry i brak normalnych prologow oznaczaja, ze zwykle prototypy C beda
klamac.

Wniosek: dokumentuj register contract, rozdziel dispatcher od handlerow i nie
zamieniaj alternate entrypointow w sztuczne argumenty.

### `func_80069568`

Podejrzany przypadek przesunietego entrypointu: wywolanie moze chciec adres
`0x80069568`, podczas gdy wygodny symbol C zaczyna sie pozniej.

Wniosek: to jest problem symbol/reloc/entrypoint, nie necessarily problem
algorytmu funkcji.

## Attack order dla duzych funkcji

1. Sprawdz, czy funkcja nie jest biblioteczna albo custom ABI.
2. Zrob offset map dla wszystkich bazowych pointerow.
3. Ustal command tables, jump tables i rodata.
4. Wydziel male, potwierdzone helpery.
5. Dopasuj control flow bez struktur, jesli struktury psuja frame.
6. Dopiero po stabilnym szkielecie dodawaj typy i window structs.
7. Na koncu register pinning/barriers dla resztkowych diffow.

## Kiedy przerwac i zapisac blocker

Przerwij probe, gdy przez kilka iteracji diff nie zmienia klasy problemu albo
kazda poprawka jednego miejsca psuje prolog/frame. Wtedy zapisz blocker z
dowodami, nie tylko z opisem frustracji.

Dobry blocker odpowiada na pytania:

- jaki jest pierwszy konkretny mismatch,
- co juz wykluczylismy,
- ktore typy/prototypy sa potwierdzone,
- czy problem jest lokalny, czy dotyczy toolchain/link/rodata,
- jaki jest nastepny falsyfikowalny eksperyment.
