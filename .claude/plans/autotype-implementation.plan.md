# Plan implementacji funkcjonalności AutoType

## Cel

Doprowadzić podsystem **AutoType** (port z KeePassXC) do stanu działającego —
od kompilującego się szkieletu do funkcji, która wpisuje login/hasło do aktywnego
okna zewnętrznej aplikacji. Priorytet: **Linux/X11** (główna platforma deweloperska),
następnie domknięcie Windows i macOS.

---

## Stan obecny (audyt)

Wszystkie pliki istnieją i są w `WebPassWare.pro`, kod się kompiluje, ale jest
**niefunkcjonalny**. Komponenty:

| Komponent | Plik | Stan |
|-----------|------|------|
| Orkiestrator | `AutoType.cpp/.h` | Kompiluje się, ma **krytyczny bug** (patrz niżej) |
| Akcje + executor base | `AutoTypeAction.cpp/.h` | **Kompletne i poprawne** |
| Interfejs platformy | `AutoTypePlatform.cpp/.h` | Kompletny (część metod zakomentowana) |
| Platforma testowa | `AutoTypePlatformTest.cpp/.h` | **Działa w pełni** (używana w testach) |
| Platforma Windows | `AutoTypePlatformWindows.cpp/.h` | Niemal kompletna |
| Platforma macOS | `AutoTypePlatformMacos.cpp/.h` | Częściowa (`activeWindow`/`raiseWindow`/`hideOwnWindow` zwracają 0/false) |
| Platforma Linux | `AutoTypePlatformLinux.cpp/.h` | **CAŁOŚĆ ZAKOMENTOWANA** `/* ... */` |

### Bugi krytyczne (dotyczą wszystkich platform)

1. **`m_executor` nigdy nie jest tworzony.** `CAutoType` nigdy nie wywołuje
   `m_platform_intrerface->createExecutor()`. W `performAutoType()` linia
   `action->accept(m_executor)` przekazuje `nullptr` → **segfault** przy każdej próbie.
   To główny powód, dla którego funkcja nie działa nawet tam, gdzie platforma istnieje.

2. **Linux nie jest instancjonowany.** W `AutoType.cpp:41` linia
   `// m_platform_intrerface = new CAutoTypePlatformLinux(this);` jest zakomentowana,
   więc na Linuksie `m_platform_intrerface == nullptr` → `isAvailable()` zwraca `false`
   → cały podsystem milczy.

3. **`callEventFilter()` nie zwraca wartości** (`int`, ciało zakomentowane) → UB.
   Obecnie nieużywane, ale do naprawienia.

4. **Niespójność dyrektyw preprocesora:** `AutoType.cpp:11` używa `#elif Q_OS_MACOS`
   zamiast `#elif defined(Q_OS_MACOS)`. Na macOS spowoduje błąd preprocesora.
   (Na Linuksie wchodzi w gałąź `#else`, więc dziś nie boli.)

5. **Brak helperów keysym dla Linuksa:** executor Linuksa woła `qcharToNativeKeyCode()`
   i `qtToNativeKeyCode()` (wolne funkcje z `X11Funcs.h` z KeePassXC), których
   w projekcie **nie ma**. Trzeba je dostarczyć.

### Braki integracyjne

6. **Zero podłączenia do GUI.** `performAutoType()` nie jest wołane znikąd
   (grep po całym `src/` poza podsystemem = brak wyników). Brak akcji w menu,
   brak pozycji w menu kontekstowym tabeli, brak skrótu klawiszowego.

7. **Brak bibliotek X11 w `.pro`.** Jest tylko `QT += widgets`. Implementacja
   Linuksa wymaga X11, XTest, Xkb oraz `QX11Info` (moduł `x11extras` w Qt5).

8. **Skrót globalny** (hotkey wywołujący auto-type, gdy okno aplikacji jest w tle)
   — cała ścieżka zakomentowana/zaślepiona (`registerGlobalShortcut` zwraca `true`
   nic nie robiąc).

### Niedokończony parser sekwencji

9. `parseActions()` ma TODO: brak obsługi escapowania `{{}`, `{}}` oraz składni
   `{DELAY=X}` (stałe opóźnienie między klawiszami — to co innego niż `{DELAY X}`
   = pojedyncza pauza, która działa).

---

## Zakres i decyzje

**Rekomendowany zakres (MVP):** auto-type wyzwalany z aplikacji (okno aplikacji
aktywne, użytkownik wybiera wpis → aplikacja minimalizuje się i wpisuje do okna,
które stanie się aktywne). To pokrywa 90% wartości i nie wymaga globalnego hotkeya
ani filtra zdarzeń X11.

**Poza MVP (osobny, późniejszy etap):** globalny skrót klawiszowy + okno wyboru
okna docelowego. Wymaga `platformEventFilter`, rejestracji hotkeya w X11 i obsługi
`globalShortcutTriggered`. Duża złożoność, mała wartość na start.

**Platforma:** kod portu jest **X11-only** (XTest). Wayland nie jest wspierany przez
ten mechanizm i pozostanie poza zakresem (na Wayland `isAvailable()` powinno zwrócić
`false` i UI ma się wyłączyć łagodnie).

---

## Fazy implementacji

### Faza 0 — Naprawa rdzenia (wspólne dla wszystkich platform)

Cel: usunąć bug z executorem, żeby platforma testowa i Windows w ogóle mogły zadziałać.

1. W `CAutoType` zainicjalizować executor po utworzeniu platformy:
   w konstruktorze (lub leniwie w `performAutoType`) wywołać
   `m_executor = m_platform_intrerface->createExecutor();` gdy platforma istnieje.
2. Naprawić `#elif Q_OS_MACOS` → `#elif defined(Q_OS_MACOS)` w `AutoType.cpp`.
3. Naprawić `callEventFilter()` — dodać `return -1;` w zaślepce (lub usunąć metodę
   z MVP, skoro filtr zdarzeń jest poza zakresem).
4. **Test (TDD):** rozszerzyć/utworzyć projekt testowy analogiczny do `tests/ORM_test`
   (`tests/AutoType_test`) używający `CAutoType::createTestInstance()` i
   `CAutoTypePlatformTest`. Asercje na `actionChars()`:
   - `{USERNAME}{TAB}{PASSWORD}{ENTER}` dla wpisu (user, pass) → `"user[Key0x...]pass[Key0x...]"`.
   - sama nazwa użytkownika → `{USERNAME}{ENTER}`.
   - samo hasło → `{PASSWORD}{ENTER}`.
   - sekwencje specjalne: `{TAB 3}`, `{DELAY 100}`, `{F5}`, klawisze nawigacji.
   Ten test jest **wykonalny na Linuksie bez X11** (platforma testowa nie dotyka X11)
   i pełni rolę testu charakteryzującego przed dalszą pracą.

**Definicja ukończenia:** test platformy testowej przechodzi; brak segfaultu na
ścieżce `performAutoType`.

### Faza 1 — Implementacja Linux/X11

1. **Build (`WebPassWare.pro` / `includes.pri`):**
   - W bloku `unix:!macx` dodać `QT += x11extras` oraz
     `LIBS += -lX11 -lXtst` (XTest) i ewentualnie `-lXi`.
   - Upewnić się co do nagłówków `X11/XKBlib.h`, `X11/extensions/XTest.h`
     (pakiety dev: `libx11-dev`, `libxtst-dev`).
2. **Odkomentować i dostosować `AutoTypePlatformLinux.h/.cpp`:**
   - Kod jest kompletnym portem z KeePassXC; po odkomentowaniu wymaga drobnych
     korekt nazewnictwa (`NULL` → `nullptr`, zgodność z konwencją `m_`).
   - `QX11Info` pochodzi z `<QX11Info>` (x11extras).
3. **Dostarczyć helpery keysym** używane przez executor Linuksa:
   - `qcharToNativeKeyCode(const QChar&)` i `qtToNativeKeyCode(Qt::Key)`
     (port z `X11Funcs.cpp` KeePassXC) — nowy plik `src/Common/X11Funcs.cpp/.h`
     lub statyczne metody platformy. Dodać do `.pro`.
4. **Włączyć instancjonowanie Linuksa** w `AutoType.cpp:41` (odkomentować
   `new CAutoTypePlatformLinux(this)`).
5. **`updateKeymap()`** musi być wołane przed wysyłką klawiszy (w KeePassXC robi to
   ścieżka inicjalizacji `sendKey`) — zweryfikować, że jest wołane raz
   przy starcie sekwencji.
6. **Weryfikacja manualna:** uruchomić aplikację, otworzyć edytor tekstu obok,
   wywołać auto-type na wpisie testowym → sprawdzić, że login+TAB+hasło+ENTER
   trafiają do edytora. Sprawdzić zachowanie przy Caps Lock / wciśniętych
   modyfikatorach (kod ma już guardy → ma się przerwać z logiem).

**Definicja ukończenia:** `autoType()->isAvailable()` zwraca `true` na X11; sekwencja
trafia do aktywnego okna; na Wayland/braku XTest → `false` bez crasha.

### Faza 2 — Integracja z GUI

Wzorować się **dokładnie** na istniejących akcjach kopiowania
(`on_ACTION_COPY_PASS_ENTRY_PASSWORD_triggered`, `..._USER_triggered`)
w `CWebPassWareMainWindow` — auto-type wpisuje się w ten sam wzorzec.

1. **Nowa akcja** `CAction* m_autotype_action` (typ `CAction`, jak inne) tworzona
   w `setActions()`, z `tr("Auto-Type")`, ikoną i skrótem lokalnym (np. `Ctrl+Shift+V`,
   działającym tylko gdy okno aplikacji aktywne — to nie jest hotkey globalny).
2. **Slot** `on_ACTION_AUTOTYPE_PASS_ENTRY_triggered()`:
   - `qint64 id = getCurrentPassEntryId();` (już istnieje) — walidacja, early return.
   - pobrać wpis przez `m_passEntryService`, zbudować `Entry`
     (`QPair<QString,QString>` = {login, hasło}; `first`=login, `second`=hasło —
     zgodnie z `autoTypeSequence()`).
   - `autoType()->performAutoType(&entry, this);` — `this` jako `hideWindow`,
     aby aplikacja zminimalizowała się i klawisze trafiły do poprzednio aktywnego
     okna. **Uwaga:** odczytać `entry` z modelu/serwisu, nie trzymać wskaźnika do
     pól modelu (czas życia).
3. **Menu kontekstowe tabeli** (`showDataTableContextMenu`) — dodać pozycję
   „Auto-Type” obok kopiowania loginu/hasła.
4. **Menu główne / toolbar** (`setMenu`/`setToolBar`) — dodać akcję spójnie
   z resztą.
5. **Wyłączanie UI gdy niedostępne:** jeśli `!autoType()->isAvailable()`
   (np. Wayland), akcję ustawić `setEnabled(false)` / ukryć, żeby nie sugerować
   działania.
6. Wszystkie napisy przez `tr()`.

**Definicja ukończenia:** z menu kontekstowego i skrótu można wywołać auto-type
na zaznaczonym wpisie; przy braku zaznaczenia/niedostępności — brak akcji/crasha.

### Faza 3 — Parser sekwencji (domknięcie)

1. Dodać obsługę escapowania `{{}` → `{` i `{}}` → `}` w `parseActions()`.
2. Dodać `{DELAY=X}` (stałe opóźnienie między każdą akcją) odróżnione od
   `{DELAY X}` (pojedyncza pauza). Wzorzec z KeePassXC: `setDelay` w executorze.
3. Testy jednostkowe na platformie testowej dla nowych przypadków.

**Definicja ukończenia:** testy parsera zielone, w tym escapowanie i oba warianty DELAY.

### Faza 4 (opcjonalna, poza MVP) — Globalny skrót klawiszowy

Tylko jeśli wymagane później. Obejmuje: odkomentowanie i implementację
`registerGlobalShortcut`/`unregisterGlobalShortcut`/`platformEventFilter` w platformie
Linux, instalację natywnego event filtra w `CApplication`, obsługę sygnału
`globalShortcutTriggered`, ścieżkę `performGlobalAutoType` z wyborem okna docelowego.
Znaczna złożoność X11 — zaplanować jako osobne zadanie.

### Faza 5 — Domknięcie pozostałych platform (gdy będą potrzebne)

- **Windows:** zweryfikować spójność guardów (`Q_OS_WIN` w `AutoType.cpp`
  vs `Q_OS_WINDOWS` w plikach platformy), dolinkować `psapi` (już jest w `.pro`),
  przetestować. Kod wygląda na kompletny.
- **macOS:** zaimplementować `activeWindow()`, `raiseWindow()`, `hideOwnWindow()`,
  `raiseOwnWindow()` (obecnie zaślepki). Obsłużyć uprawnienia Accessibility.
  Naprawić `#elif defined(Q_OS_MACOS)` (Faza 0).

---

## Kolejność realizacji (zalecana)

```
Faza 0 (rdzeń + test)  →  Faza 1 (Linux/X11)  →  Faza 2 (GUI)  →  Faza 3 (parser)
                                                                  ↘ Faza 4/5 (później)
```

Po Fazach 0–2 funkcja jest **realnie używalna na Linuksie**. Fazy 3–5 to dopieszczenie
i rozszerzenie zasięgu platformowego.

## Ryzyka

- **X11/XTest tylko** — na Wayland funkcja nie zadziała; trzeba łagodnie wyłączyć UI.
- **Czas życia `Entry`** — `performAutoType` bierze `const Entry*`; trzeba zapewnić,
  że obiekt żyje przez całą (synchroniczną) sekwencję. Jest synchroniczna, więc lokalny
  obiekt na stosie slotu wystarcza.
- **Minimalizacja okna** (`hideWindow->showMinimized()`) zależnie od WM może nie
  przywrócić fokusu na właściwe okno — przetestować na docelowym środowisku.
- **Bezpieczeństwo:** auto-type wpisuje hasło symulacją klawiatury do dowolnego
  aktywnego okna — kod ma już guardy (zmiana aktywnego okna przerywa sekwencję,
  Caps Lock/modyfikatory przerywają). Zachować te zabezpieczenia.

## Zgodność z konwencjami projektu (AGENTS.md / CLAUDE.md)

- Dopasować styl do edytowanych plików; nie przemianowywać publicznych API/sygnałów.
- `tr()` dla napisów; `nullptr`; klamry zawsze; early returns.
- Widgety z właścicielem Qt (parent), bez ręcznego zarządzania pamięcią.
- Testy przed commitem; jeden commit = jedna logiczna zmiana; opis po polsku,
  conventional commits (`feat:`, `fix:`).
- Kolejność include: nagłówek własny → Qt/system → projekt.
