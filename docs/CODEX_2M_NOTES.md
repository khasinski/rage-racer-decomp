# CODEX 2M — przerwane rozpoznanie

Data: 2026-07-30

Zakres zadania: `func_8002C478` i `func_8002D398` z
`src/main/PAL/main/car/InitPlayerCar.c`.

Praca została przerwana na prośbę użytkownika przed zmianą kodu produkcyjnego. Nie
zastąpiono żadnego `INCLUDE_ASM`, nie zmieniono typów i nie uruchamiano końcowego
`make check`.

## Pomiary punktu wyjścia

Pomiary wykonano przez `measure_candidate.py --verbose` na najlepszych zastanych
kandydatach.

| funkcja | kandydat | retail | candidate | exact | aligned | pierwszy rozjazd |
|---|---|---:|---:|---:|---:|---:|
| `func_8002C478` | `candidate-01.c` | 548 | 545 | 306 | 140 | `+0x30` |
| `func_8002D398` | `candidate-02.c` | 729 | 721 | 627 | 517 | `+0x8` |

W obu funkcjach `aligned` jest istotnie niższy od `exact`. Oznacza to, że istnieją
lokalne różnice kształtu kodu — instrukcje obecne tylko po jednej stronie albo
odmiennie zbudowane fragmenty. Sama zmiana kolejności deklaracji lub instrukcji nie
może z definicji usunąć wszystkich tych różnic.

## `func_8002C478`

- Kandydat jest krótszy o trzy słowa, więc w rekonstrukcji prawdopodobnie brakuje
  rzeczywistej operacji, argumentu wywołania albo krótkiego fragmentu sterowania.
- Pierwszy zauważony rozjazd to `lh` w retail wobec `lhu` w kandydacie, co wskazuje
  na błędną signedness pola lub widoku typu.
- Dalsze różnice obejmują zarówno alokację rejestrów, jak i kształt fragmentów;
  plateau `exact 306` znane z wcześniejszych wariantów nie jest dowodem na jeden
  prosty problem z harmonogramem.
- Funkcja ma 24 wywołania, dlatego przy powrocie należy najpierw rozpisać każde
  wywołanie wraz z faktyczną liczbą argumentów i porównać obszar outgoing arguments.
- Szczególnej kontroli wymaga blok dzielenia oparty o magiczne stałe oraz pętla w
  okolicy retail `0x8002CAF0`; w verbose w tym rejonie widać brak lub inne
  uporządkowanie kodu.

## `func_8002D398`

- Retail ma ramkę `0x140`, a kandydat `0x138`; zapisy callee-saved są w retail
  przesunięte o osiem bajtów.
- Retail wcześnie utrzymuje bazę `D_801F1854` w `$fp` (`lui`/`addiu`), czego nie
  odtwarza kandydat.
- Różnica ośmiu bajtów w ramce jest konkretną wskazówką dotyczącą brakującego
  lokalnego obiektu lub szerszego wywołania, ale `aligned 517` pokazuje, że nie jest
  to jedyny problem.
- Duże fragmenty geometrii i pętli są zbudowane inaczej; dalsza permutacja samych
  statementów bez naprawy reprezentacji danych i control flow nie rokuje.

## Czego nie ustalono

Nie zakończono wymaganej klasyfikacji wszystkich różnic na:

1. przesunięcia `sp` o stałą,
2. różnice wyłącznie nazw rejestrów w tej samej pozycji,
3. rzeczywiste różnice instrukcji.

Nie należy dopisywać liczebności tych koszyków na podstawie oglądu verbose. Przy
ewentualnym wznowieniu trzeba najpierw wykonać automatyczne, jawnie zdefiniowane
wyrównanie strumieni instrukcji i zapisać jego wynik.

## Stan roboczy i zalecany powrót

- Czysty worktree eksperymentalny: `/tmp/wt-codex2m`.
- Kandydaci pozostają w
  `scratch/decomp-work/func_8002C478/` i
  `scratch/decomp-work/func_8002D398/`.
- W głównym drzewie zastano cudze zmiany w `include/game/car.h` i
  `src/main/PAL/main/sdk/CdRead.c`; nie były dotykane.
- Dłuższe przebiegi permutera można wykonywać na Darwinie przez `ssh darwine`.

Jeżeli zadanie wróci, sensowna kolejność to: automatyczna klasyfikacja trzech
koszyków, pełna tabela argumentów wszystkich callees, naprawa typów/signedness,
identyfikacja brakujących słów, a dopiero potem `alloc_diff.py` lub permuter.
