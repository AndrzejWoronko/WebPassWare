# Plan: WebPassWare → Qt 6.12 + CMake, druga runda wzorców projektowych

Kopia robocza planu trafi do repo jako `.claude/plans/migracja-qt6-i-wzorce.plan.md`
(obok `refaktor-wzorce-projektowe.plan.md`), z sekcją „Postęp” aktualizowaną po
każdej fazie.

## Context

WebPassWare to Qt 5.15, qmake i C++14. Projekt-wzór `/home/woronek/git/IpcFirmwareTool`
(ten sam autor) używa Qt 6.12 z `/opt/Qt/6.12.0/gcc_64`, CMake z presetami i Ninja.
Ma też `scripts/build.sh`, testy Qt Test przez `ctest`, bibliotekę logiki bez
Widgets, DI w `main.cpp` (bez singletonów i makr `DB`/`SETT`), SQL tylko przez
`prepare` + `bindValue`, `PRAGMA user_version` z migracjami, RAII `TransactionGuard`,
CPack DEB oraz sekcje CLAUDE.md „Engineering rules”, „Style” i „Git”.

Cel: przenieść WebPassWare na ten sam fundament i dokończyć refaktor do wzorców.
Pierwsza runda (`.claude/plans/refaktor-wzorce-projektowe.plan.md`, 2026-06-13)
jest już zrobiona: `COrmRepository` przez kompozycję, konstruktory DI z fallbackiem
do singletonów, błędy serwisów w GUI, RAII `Owned*` i `getFieldAs<T>()`.
Ta runda usuwa fallbacki i globalny stan, naprawia SQL, schemat i logowanie oraz
rozbija god-class okna głównego.

## Decyzje (z użytkownikiem, 2026-10-25)

- Prefiks `C` w istniejących klasach zostaje (bez masowych zmian nazw). Nowe klasy
  dostają namespace `wpw` i nazwy bez prefiksu.
- Tylko Qt 6.12 + CMake: `.pro`/`.pri` znikają, nie utrzymujemy kompatybilności z Qt 5.
- Kolejność: najpierw migracja 1:1 na Qt 6 (Etap A), potem wzorce (Etap B).
- Linux jest budowany, testowany i pakowany. Windows i macOS mają się kompilować
  (port kodu i skryptów), bez weryfikacji na sprzęcie.
- Bezpieczeństwo: naprawić logowanie (PBKDF2 z solą, `-l`, strona ustawień bazy,
  hasła w logach). Szyfrowanie haseł wpisów idzie do osobnego planu; repozytorium
  zostawia na nie miejsce.
- Teksty `tr()` zostają po polsku. Nowe i zmieniane komentarze piszemy po angielsku;
  istniejących nie tłumaczymy hurtem.
- Pakowanie: CPack DEB jak w IpcFirmwareTool. Qt Installer Framework usuwamy,
  skrypty Win/macOS przechodzą na `cmake` + `windeployqt`/`macdeployqt`.

## Stan wyjściowy (zweryfikowany w kodzie)

**Qt 5 → 6, nie skompiluje się:**
- `QTextStream::setCodec`: `ImportCsv.cpp:11,35`, `ExportCsv.cpp:15,45`. Listy
  kodowań ISO-8859-2, Windows-1250 i CP852 są w `CsvImportDialog.cpp:105` i
  `CsvExportDialog.cpp:62`.
- `QRegExp`: `AutoType.cpp:338,349,498`, `Tools.cpp:221-225`.
- `QX11Info`/`x11extras`: `AutoTypePlatformLinux.cpp:8,25,26,585`, oba `.pro`.
- `QPalette::Background`: `ToggleSwitch.cpp:114,130`.
- `QT += script` w `test_orm.pro` i `test_service.pro`.
- `<bits/stdc++.h>` i `#define NULL` w `Global.h:15,227-230` (łamie MSVC i Apple clang).

**Qt 5 → 6, zmiana zachowania (ryzyko ciche):**
- `QMetaProperty::typeName()` w `OrmObject.h:118` służy jako klucz mapy typów DDL.
  W Qt 6 typedefy (`Varchar255`, `Timestamp`) mogą się rozwijać do typu bazowego,
  a wtedy DDL zmieni się po cichu.
- `QVariant::Type`/`type()`: `SqlField.*`, `OrmObject.h:130-157`, `SqlModel.cpp:55-60`,
  `Form*Field.cpp`, `ExportCsv.cpp:99-119`.
- Surowsze `QVariant ==`: `FormAbstractField.cpp:32` (`isDirty`) może fałszywie
  pytać o niezapisane zmiany.
- `removeAt(indexOf(..) == -1)` w `OrmObject.h:760` asertuje w Qt 6.
- `ADD_PTR_PROPERTY` (105 użyć) generuje `Q_PROPERTY` ze wskaźnikami, a Qt 6 wymaga
  wtedy pełnych typów.
- Narrowing `qsizetype` → `int`: `Form.cpp`, `ExportCsv.cpp`, `CsvModel.cpp:34-36`,
  `Tools.cpp:208`.
- Mniejsze: `Q_FOREACH` (~40 miejsc), `QLibraryInfo::location`, `Q_ENUMS`,
  `QMouseEvent::pos`, `QString::fromUtf16(ushort*)` (Windows).

**Dane:**
- Wartości są sklejane z SQL w `OrmObject.h:460,598,629,658,690` i
  `Database.cpp:303,376,410,420,434,479,494`.
- Mapa typów jest MySQL-owa (`LONGTEXT`, `UNSIGNED`, `'0000-00-00'`). `REPLACE … SET`
  (`Database.cpp:391`) nie działa w SQLite.
- `deleted` zapisywane jako `'true'`, ale filtr szuka `deleted=false`.
- `SqlModel.cpp:112-121` gubi parametry zapytania.
- Transakcje bez RAII; „już w transakcji” liczy się jako sukces.
- Brak wersji schematu. `ModelTableCheck` działa tylko z flagą `-c`.

**Bezpieczeństwo:**
- Hasło główne to niesolony MD5 z `toLatin1()` (`Database.cpp:34-38`,
  `LoginDialog.cpp:92`). Pusty hash wpuszcza każdego.
- `-l` omija logowanie (`Application.cpp:31-35`).
- `SettingsDatabaseViewController.cpp:27-53` haszuje hash, co blokuje logowanie.
- Hasła trafiają do logów debug (`WebPassWareMainWindow.cpp:672,701,796`,
  `AbstractFormDialogController.cpp:47`).
- Hasła wpisów są w bazie jawnym tekstem (osobny plan).

**Globalny stan:**
- `SETT`: 61 linii w 11 plikach. `DB`: 13 plików. `APPI`: 13 plików.
- `STYLE`, `LOG_APP`/`LOG_SQL`, `CAutoType::instance`.
- `PassEntryService`/`PassGroupService::getInstance` są już tylko fallbackami w
  konstruktorach.
- `GenericFactory.h` nie jest używany.

**Smelle i błędy:**
- `WebPassWareMainWindow.cpp` (822 linie, ~14 odpowiedzialności). Blok „wczytaj wpis
  albo pokaż błąd” powtarza się 4×.
- Ctrl+X wisi na Wyjściu i na kopiowaniu użytkownika.
- `getCurrentPassEntryId` sprawdza zły model (`:423`).
- Usunięcie grupy odświeża zły model (`:561`).
- `ACTION_VISIT_WEBSITE` jest null.
- Dialogi formularzy są tworzone `new` przy każdym otwarciu i nigdy nie zwalniane.
- `QTranslator` na stosie w pętli (`Application.cpp:113`), więc tłumaczenia Qt nigdy
  nie działają. Obejściem jest `CMessageBox::translateButtons`.
- Combo grup mapuje indeks na id przez hack `getOwnedObject(0)`.
- `CAbstractFormDialog` i `CAbstractCloseDialog` to prawie kopie.
- Strony ustawień są dopasowywane po indeksie.
- CSV używa `split(delim)` bez cudzysłowów i nie ma transakcji.
- Martwe menedżery stanu.
- Skrypty mają zaszyty `QT_VERSION=5.15.2`, a `create_portable.sh` pomija
  `libQt5X11Extras`.

## Architektura docelowa

Etap A zostawia drzewo `src/` bez zmian: CMake dostaje te same katalogi include co
`INCLUDEPATH`, więc nie ruszamy `#include`. Etap B dzieli kod na cele CMake według
istniejących katalogów:

```
CMakeLists.txt, CMakePresets.json (debug/release/coverage, /opt/Qt/6.12.0/gcc_64, Ninja)
cmake/Packaging.cmake, packaging/ (webpassware.desktop, ikona)
scripts/build.sh, scripts/build-deb.sh   (wzór: IpcFirmwareTool/scripts/build.sh)
src/CMakeLists.txt
  wpw_core  (STATIC, Qt Core + Sql + Network, bez Widgets)
            Database/, Models/, Security/ (nowe), Common/{PasswordGenerator, ImportCsv,
            ExportCsv, CsvParser, Logger, Tools}
  wpw_gui   (STATIC, + Widgets/Svg) Gui/**, Common/{AutoType*, *State*}, Application,
            ApplicationSettings, WebPassWareMainWindow
  webpassware (main.cpp = composition root)
tests/CMakeLists.txt   wpw_add_test(); tst_*.cpp linkują wpw_core / wpw_gui (bez #include .cpp)
```

## Etap A — migracja na Qt 6.12 (zachowanie bez zmian)

### A0. Reguły i siatka bezpieczeństwa (na obecnym Qt 5 / qmake)

1. **CLAUDE.md:** dopisać sekcje „Engineering rules”, „Style” i „Git” (szkic niżej) i
   poprawić opis testów (są trzy projekty, nie jeden). Commit `docs:`.
2. **Baseline:** systemowy Qt 5.15, `qmake && make` oraz trzy projekty testowe
   (`ORM_test` 23, `Service_test` 7, `AutoType_test` 9). Wynik zapisać w „Postęp”.
3. **Testy charakteryzujące punkty ryzyka Qt 6** (commit `test:`):
   - DDL `CREATE TABLE` dla `PassEntry`/`PassGroup`, porównany znak w znak z obecnym
     (chroni przed zmianą `typeName()`);
   - `isDirty()` pól formularza dla wartości z QSettings (string vs int/bool);
   - zapis i odczyt CSV w UTF-8, Windows-1250 i ISO-8859-2 na fiksturach
     `tests/fixtures/*.csv`;
   - otwarcie kopii bazy utworzonej przez obecną wersję (fikstura
     `tests/fixtures/legacy_qt5.db`): odczyt wpisów i grup.

### A1. Porty wspólne dla Qt 5.15 i Qt 6 (dalej qmake/Qt 5, testy zielone)

Po jednym commicie `refactor:` na temat:
- `QRegExp` → `QRegularExpression` (`AutoType.cpp`, `Tools.cpp`);
- `Q_FOREACH` i `CLEAR_PTR_LIST` → range-for;
- `QVariant::type()`/`QVariant::Type` → `userType()`/`QMetaType::Type`. Klucz DDL to
  typ zapisany w `ORM_PROPERTY` (`#type`, rejestr `declaredTypeNames()`), bo typedef
  (`Varchar255`) ma ten sam `QMetaType` co `QString`, a Qt 6 zwraca w `typeName()`
  typ bazowy;
- `QPalette::Background` → `Window`, `Q_ENUMS` → `Q_ENUM`;
- `Global.h`: usunąć `<bits/stdc++.h>` i redefinicję `NULL`, a w `safe_delete` dodać
  `do {} while (0)`;
- strażnik `indexOf != -1` w `OrmObject.h:760`.

### A2. Przejście na CMake + Qt 6.12 (jeden commit `build:` / `feat:`, kompiluje się i testy zielone)

- Dodać `CMakeLists.txt` i `CMakePresets.json` skopiowane z IpcFirmwareTool i
  dostosowane: projekt `WebPassWare VERSION 0.1.0`, `qt_standard_project_setup(REQUIRES 6.12)`,
  `-Wall -Wextra -Wpedantic` i opcja `WPW_COVERAGE`.
  - Zostaje define `WEBPASSWARE_VERSION_STR`, którego używa `APP_VERSION`.
  - `CURRENT_VERSION.TXT` powstaje przez `file(WRITE)`.
  - Zasoby: `qt_add_resources` z `style.qrc` i `icons.qrc`.
  - Na Windows dochodzi `mainicon.rc` i `psapi`; na macOS `MacUtils.mm` i `AppKit`;
    na Linuksie `X11` i `Xtst` przez `find_package(X11)`.
- Dodać `scripts/build.sh` z IpcFirmwareTool (zmienić nazwy i wypisywane ścieżki).
- Poprawki tylko pod Qt 6:
  - `QX11Info` → `QNativeInterface::QX11Application::display()`, `XDefaultRootWindow`,
    `CurrentTime` oraz kontrola braku display (Wayland → auto-type wyłączony z
    komunikatem, bez crasha);
  - `setCodec` → `setEncoding` dla UTF-8, a dla Windows-1250, ISO-8859-2 i CP852
    `QStringDecoder`/`QStringEncoder` z nazwą (ICU w Qt 6.12). Bez Core5Compat.
    Kodowania, których nie da się utworzyć, są ukryte na liście;
  - `QLibraryInfo::location` → `path`, `QMouseEvent::pos` → `position().toPoint()`,
    `fromUtf16` → `fromWCharArray`;
  - pełne typy lub `Q_DECLARE_OPAQUE_POINTER` dla właściwości z `ADD_PTR_PROPERTY`;
  - porównania `QVariant` w testach z jawną konwersją;
  - jawne `static_cast<int>` przy narrowing `qsizetype` tam, gdzie kompilator Qt 6
    to zgłosi (w Qt 5 `size()` zwraca `int`).
- Testy: `tests/CMakeLists.txt` z `wpw_add_test`. Trzy istniejące projekty stają się
  `tst_orm`, `tst_service` i `tst_autotype`. `ApplicationSettingsStub.cpp` zostaje,
  a `QT += script` znika.
- Usunąć `WebPassWare.pro`, `includes.pri`, `tests/**/*.pro` i `test_clean.pri`.
  Do `.gitignore` (lokalnie, poza commitem) dopisać `build/`.

### A3. Pakowanie i skrypty platform

- Dodać `cmake/Packaging.cmake` (wzór IpcFirmwareTool): `qt_generate_deploy_app_script`,
  Qt w `/opt/webpassware`, RPATH `$ORIGIN/../lib`, tylko wtyczki xcb, sqlite i svg,
  plik `.desktop`, ikona i dowiązanie w `/usr/bin`. Do tego `scripts/build-deb.sh`.
- `scripts/windows/*`, `scripts/macosx/*`: `cmake --preset` + `windeployqt`/`macdeployqt`,
  Qt 6 (`mingw_64`, `macos`). Usunąć `scripts/linux/create_portable.sh` i `installer/`.

### A4. Dokumentacja

- README: budowa, testy, paczka.
- CLAUDE.md: sekcje Build/Tests/Layout jak w IpcFirmwareTool.
- `TODO` uzupełnić o zadania odłożone.

## Etap B — wzorce projektowe i naprawy (Qt 6, CMake, testy przed zmianą)

Każda faza: test (red) → zmiana → green → commit. Pierwszeństwo mają fazy B1–B3.

### B1. Podział na `wpw_core` / `wpw_gui` (Layered, Dependency Rule)

- Utworzyć cele jak w „Architektura docelowa”. `wpw_core` nie linkuje Widgets.
  Zależności z `Common` i `Database`, które wciągają GUI albo `APPI`/`SETT`
  (np. `Database.cpp` czyta `SETT`), przerwać na seamach. Ostatecznie znikają w B2.
- Testy linkują biblioteki zamiast `#include "....cpp"` (`test_orm.cpp:10-15`).

### B2. Composition root i koniec singletonów (DI, SOLID-D)

- `main.cpp` tworzy po kolei `CApplication`, `CApplicationSettings`, `CDatabase`,
  serwisy, `CStyle`, loggery i `CAutoType` i przekazuje je konstruktorami (wzór:
  `IpcFirmwareTool/src/app/main.cpp`).
- Kolejność usuwania: fallbacki `getInstance()` w kontrolerach (już mają konstruktory
  DI) → `DB` (13 plików) → `SETT` (11 plików, najwięcej w `SettingsLookController` i
  `PasswordGeneratorWidget`) → `APPI` → `STYLE`, `LOG_*` → `CAutoType::instance`.
  Jeden globalny symbol na commit.
- Na końcu usunąć `Singleton.h`, `SingletonHolder` i nieużywany `GenericFactory.h`.
- Usunąć ukrytą zależność `CApplicationSettings()` → `APPI`.

### B3. Warstwa danych: Repository, prepared statements, RAII, migracje

- SQL tylko przez `prepare` + `bindValue`. Nazwy kolumn i tabel tylko z `metaObject()`
  (biała lista), a `orderby` jako lista dozwolonych kolumn plus kierunek.
- Dotyczy `findBy*`, `getIdCondition`, `Database.cpp:303-494`, `SqlModel::m_where`
  (naprawa zgubionych parametrów).
- Mapa typów SQLite (`TEXT`, `INTEGER`, `REAL`, `BLOB`). `REPLACE … SET` →
  `INSERT OR REPLACE`. `deleted` jako `0/1`, ze zgodnym filtrem.
- `wpw::TransactionGuard` (port `IpcFirmwareTool/src/core/storage/TransactionGuard.*`,
  `BEGIN IMMEDIATE`, rollback w destruktorze) zastępuje gołe `BEGIN EXCLUSIVE`.
  Import CSV działa w jednej transakcji, a błędy wierszy pokazuje jednym podsumowaniem.
- Wersjonowanie: `PRAGMA user_version`, `kSchemaVersion` i lista migracji (wzór:
  `IpcFirmwareTool/src/core/storage/Database.cpp:20-160`).
  - Wersja 0 to baza ze starego programu: migracja 1 przepisuje tabele do typów
    SQLite i zamienia `deleted='true'` na `1`.
  - Migracje działają przy każdym starcie. `-c` zostaje tylko jako ręczne sprawdzenie
    zgodności (`ModelTableCheck`).
  - Testy: od pustej bazy, od wersji N-1 i od fikstury `legacy_qt5.db`.
- `COrmService` wymaga wstrzykniętego `IRepository<T>` (dziś tworzy go sam,
  `OrmService.h:36`).
  - Naprawić `m_insertedId` przy nieudanym insercie.
  - Usunąć martwe `translateRecToThisObj` i nieużywane wrappery transakcji.
  - Active Record (`ORM_PROPERTY` w modelach) zostaje jako opis pól (YAGNI).

### B4. Logowanie i hasło główne (fix, osobne commity)

- Dodać `wpw::MasterPassword` w `src/Security/`, wzorowany na
  `IpcFirmwareTool/src/core/security/ServicePin.*`:
  - format `pbkdf2-sha256$iter$salt$key` z `QPasswordDigestor` i
    `QRandomGenerator::system()`, hasło w UTF-8;
  - stary MD5 jest weryfikowany jeden raz i od razu przepisywany na PBKDF2;
  - pusty hash oznacza pierwsze uruchomienie: trzeba ustawić hasło, nie wolno wpuścić
    bez niego.
- Usunąć `-l`. Test GUI dostaje własny konstruktor albo DI zamiast obejścia.
- Strona ustawień bazy: pola są puste, a puste pole znaczy „bez zmiany”. Zmiana hasła
  wymaga podania starego.
- Usunąć hasła z `DEBUG_WITH_LINE` (4 miejsca).
- Zostawić miejsce na szyfrowanie: odczyt i zapis `m_pass` przechodzi przez jedną
  metodę repozytorium wpisów. Szyfrowanie pól idzie do osobnego planu.

### B5. Okno główne: Facade + Observer + Command

- Dodać fasadę `wpw::PasswordVault` nad dwoma serwisami, z sygnałami `entriesChanged`
  i `groupsChanged`. Modele odświeżają się na sygnał, a okno przestaje ręcznie
  przeładowywać modele.
- Akcje wpisów, grup i schowka przenieść do osobnych klas z handlerami
  (`EntryActions`, `GroupActions`, `ClipboardActions`). Okno tylko je składa.
  Wspólny helper „wpis albo komunikat błędu” zastępuje 4 kopie.
- Naprawić (każde jako `fix:` z testem offscreen `tst_mainwindow`):
  - konflikt Ctrl+X;
  - `getCurrentPassEntryId`;
  - odświeżanie po usunięciu grupy;
  - null `ACTION_VISIT_WEBSITE`;
  - podwójne wyzwalanie Enter/Insert/Delete;
  - tooltip „Export danych”;
  - wyciek dialogów formularzy (rodzic + `deleteLater` albo dialog lokalny).

### B6. Formularze i dialogi (Template Method bez duplikatów)

- Połączyć wspólną część `CAbstractFormDialog` i `CAbstractCloseDialog` w jedną bazę.
- Kontrolery dziedziczą po `QObject`, nie po `QWidget`.
- Wydzielić wspólne `isEmptyForm`/`validateForm`.
- Combo grup trzyma id w `itemData` (koniec hacka z `getOwnedObject(0)`).
- `m_validator` należy do rodzica i jest inicjalizowany.

### B7. Auto-type, ustawienia, CSV, stan okien

- **Auto-type (Factory Method + Strategy):** `CAutoTypePlatformFactory` zamiast
  `#ifdef` w konstruktorze `AutoType.cpp:8-43`; platforma testowa jest wstrzykiwana.
  RAII dla `Display*`, naprawa `unloadPlugin` i wycieku w `parseActions`.
- **Ustawienia (Registry/Factory):** lista `{tytuł, ikona, fabryka strony+kontrolera}`
  zamiast dopasowania po indeksie. Kontrolery dostają rodzica.
- **CSV:** `CsvParser` zgodny z RFC 4180 (cudzysłowy, separator, BOM) w `wpw_core`,
  z testami na fiksturach. Wspólne listy kodowań i separatorów dla obu dialogów.
- **Stan okien:** usunąć martwe `CAbstractTable/TabMainWindow/SplitterStateManager`,
  `ICStateManager` i `CTableState`. Aktywne menedżery dostają `QSettings&` z
  composition root.
- **Tłumaczenia:** `QTranslator` jako członek `CApplication` (albo w `main.cpp`).
  `qt_add_translations` tylko z `QT_TRANSLATION_CATALOGS qtbase` (polskie przyciski).
  Potem usunąć `CMessageBox::translateButtons`.
- **Drobne:**
  - `CStyle` stosowany raz;
  - `MaintenanceTool` bez `waitForFinished` w wątku GUI;
  - `CLogger::PATH` i `m_month` zainicjalizowane;
  - `main.cpp` zwraca kod z `exec()` i 1 przy nieudanym logowaniu.

## Szkic sekcji do CLAUDE.md (A0)

```markdown
## Engineering rules
- **TDD**: failing Qt Test first, then the minimal code, then refactor with green
  tests. Characterization tests before touching legacy behaviour (ORM, forms, CSV).
- **SOLID** and the patterns in use: Repository (COrmRepository), Template Method
  (AbstractFormDialog, settings pages), Strategy (AutoTypePlatform*, ICState),
  Factory (auto-type platform, settings pages), Facade (PasswordVault),
  Observer (Qt signals), Command (main window actions), RAII transaction. No
  patterns "for later" (YAGNI).
- Dependencies injected through constructors, composed in `main.cpp`; no new
  global singletons or macros like `DB` / `SETT` / `APPI` (existing ones are being removed).
- `wpw_core` never links Qt Widgets.
- SQL only through `prepare` + `bindValue`; identifiers only from `metaObject()`.
  Schema changes: bump `kSchemaVersion` and add a migration (tested from empty,
  from N-1 and from the legacy fixture database).
- Secrets: never log passwords; master password only as salted PBKDF2-SHA256.
- Coverage target 80%+ for `wpw_core`.

## Style
- C++17, Qt 6 APIs only (function-pointer connects, `QRegularExpression`,
  `QStringDecoder`, `QMetaType`, range-for instead of `Q_FOREACH`).
- Existing classes keep the `C` prefix; new classes go to namespace `wpw` without it.
  Members `m_camelCase`, slots `onXxx`; always braces; `nullptr`.
- User-visible strings through `tr()` (source strings in Polish). New and changed
  comments in English and explain why; no references to plan phases in code.
- Shell scripts: `set -euo pipefail`, usage from the header comment, `die()` helper.

## Git
- Conventional commits (`feat:`, `fix:`, `docs:`, `refactor:`, `test:`, `build:`),
  description in Polish, one logical change per commit, tests green before committing.
- Never add `.gitignore` to commits; no `Co-Authored-By:` trailers.
- `build/`, `bin/`, `.obj/`, `.moc/` and `*.user` stay out of git.
```

AGENTS.md: dopisać, że zakaz szerokich refaktorów nie dotyczy kroków tego planu.
Każdy krok nadal ma być mały.

## Kolejność commitów (przykład)

`docs: Reguły Engineering/Style/Git w CLAUDE.md` → `test: Testy charakteryzujące DDL, formularzy i CSV`
→ `refactor: QRegularExpression zamiast QRegExp` → … → `build: Przejście na CMake i Qt 6.12`
→ `build: Paczka Debiana (CPack)` → `refactor: Biblioteki wpw_core i wpw_gui`
→ `refactor: Usunięcie singletonu DB` → … → `fix: Hasło główne jako PBKDF2 z solą` → …

## Szacunek

Etap A: 4–6 MD (najwięcej niepewności w A2: DDL i właściwości wskaźnikowe).
Etap B: 8–12 MD (B3 i B5 największe).

## Weryfikacja

- Po każdym commicie: `scripts/build.sh debug` (configure, build i `ctest`) oraz
  `git diff --check`.
- Od B1: `cmake --preset coverage && cmake --build --preset coverage && ctest --preset coverage`
  i lcov dla `wpw_core` ≥ 80%.
- GUI: `tst_mainwindow` offscreen (`QT_QPA_PLATFORM=offscreen`): logowanie,
  dodanie/edycja/usunięcie grupy i wpisu, kopiowanie do schowka, skróty bez konfliktów.
- Ręcznie po A2 i po B3–B4:
  - uruchomić `build/debug/src/webpassware` na kopii prawdziwej bazy z wersji Qt 5;
  - zalogować się starym hasłem (MD5 przechodzi na PBKDF2) i sprawdzić, że wpisy są
    nienaruszone;
  - import i eksport CSV w Windows-1250;
  - auto-type pod X11 do edytora tekstu; pod Waylandem bez crasha.
- Paczka: `scripts/build-deb.sh`, potem `dpkg-deb -x` do katalogu tymczasowego i start
  programu z wdrożonym Qt.
- Win/macOS: tylko kompilacja, gdy będzie dostęp do maszyny lub CI (odnotować w „Postęp”).

## Ryzyka

| Ryzyko | Mitygacja |
|---|---|
| Qt 6 rozwija typedefy w `typeName()` i DDL zmienia się po cichu | Test DDL w A0. Mapa po `QMetaType` w A1. Migracja w B3 |
| Istniejące bazy użytkowników (typy MySQL, `deleted='true'`) | Fikstura `legacy_qt5.db`. Migracja 1 testowana. Kopia pliku bazy przed migracją |
| Kodowania 1250/8859-2/852 bez ICU | Test w A2. Niedostępne kodowania ukryte na liście |
| Wayland zamiast X11 | Kontrola `display()`, auto-type wyłączony z komunikatem |
| Usuwanie `SETT`/`APPI` dotyka wielu plików | Jeden symbol na commit, testy offscreen |
| Brak sprzętu Win/macOS | Tylko kompilowalność. Ryzyko odnotowane |

## Postęp

- 2026-10-25: plan zatwierdzony.
- 2026-10-25: A0.2 baseline na systemowym Qt 5.15.13 (qmake): build aplikacji OK
  (tylko ostrzeżenia `-Wunused-parameter` w `AutoType.cpp:216`); `test_orm` 23/23,
  `test_service` 7/7, `test_autotype` 11/11 (z init/cleanup).
- 2026-10-25: A0.1 sekcje Engineering rules / Style / Git dopisane do CLAUDE.md;
  wklejona wersja z IpcFirmwareTool usunięta (decyzja użytkownika).
- 2026-10-25: A0.3 testy charakteryzujące (Qt 5.15, wszystkie zielone):
  - `test_service` +1 (8/8): DDL `pass_entry`/`pass_group` znak w znak;
  - `tests/LegacyDb_test` (5/5) na fiksturze `tests/fixtures/legacy_qt5.db`
    (schemat z `ModelTableCheck`, `deleted` = `'true'` albo `0`);
  - `tests/Csv_test` (11/11): bajty eksportu i import dla ISO-8859-2,
    Windows-1250, UTF-8. **Odkrycie:** „DOS/OS2-852” nigdy nie działało — Qt 5
    nie zna tej nazwy, plik powstaje w UTF-8 (test to utrwala; w A2 decyzja:
    prawdziwe CP852 przez `QStringDecoder("IBM852")` albo usunięcie z listy);
  - `tests/Forms_test` (5/5): `isDirty()` po odtworzeniu wartości z QSettings
    (stringi) jest `false`; pole wyboru z wartością startową `0` jest „brudne”
    od początku (zastane zachowanie);
  - przy okazji: warstwa SQL wymaga `QApplication` (`CSqlQuery::setSqlCursor`
    ustawia kursor), bez niej crash — do rozdzielenia w B1.
- 2026-10-25: A1 zrobione (qmake/Qt 5.15, po każdym kroku pełny build i 6 projektów
  testowych zielone, 0 ostrzeżeń deprecated): C++17, `QRegularExpression`, range-for
  zamiast `Q_FOREACH` (`std::as_const` dla nazwanych kontenerów), `QMetaType`
  zamiast `QVariant::Type` + rejestr typów `ORM_PROPERTY` (test
  `test_declaredTypeNamesKeepTypedefs`), `Q_ENUM`, `QPalette::Window`, `Global.h` bez
  `<bits/stdc++.h>` i redefinicji `NULL` (`NULL` → `nullptr`, 114 miejsc, bez literałów
  SQL), `safe_delete`/`CLEAR_PTR_LIST` w `do {} while (0)`, `removeOne` w
  `updateProperty`. Testy: `test_service` 9/9, `test_legacydb` 6/6 (+ wyrównanie
  kolumn liczbowych). Narrowing `qsizetype` przeniesiony do A2.
- 2026-10-25: A2 zrobione (`fd2c54a`): CMake + Qt 6.12, `scripts/build.sh`, ctest 6/6
  zielone, czysty build bez ostrzeżeń deprecated (zostały zastane `-Wunused-parameter`
  w `AutoType.cpp:216`). Pliki qmake usunięte. Ustalenia:
  - makra Xlib (`#define Bool int`) psuły wspólny `mocs_compilation.cpp`; moc klasy
    `CAutoTypePlatformLinux` dołączony w jej `.cpp` (docelowo nagłówek bez X11, B7);
  - platforma offscreen ma `QX11Application` z pustym `display()`; konstruktor
    platformy linuksowej to sprawdza (wcześniej crash przy starcie bez X11);
  - ICU w Qt 6.12 zna ISO-8859-2, Windows-1250 i `IBM852`/`cp852`, nie zna
    „DOS/OS2-852” (fallback UTF-8 jak w Qt 5); kompilator Qt 6 nie zgłosił narrowingu
    `qsizetype` przy `-Wall -Wextra`, więc rzutowań nie dodano;
  - test dymny: program startuje offscreen na kopii `legacy_qt5.db` (izolowany HOME).
  - Skrypty `scripts/{linux,macosx,windows}` i `installer/` są nadal qmake/Qt 5 → A3.
- 2026-10-25: A3 zrobione (`aa227fc`): CPack DEB (`scripts/build-deb.sh`, paczka 28 MB,
  71 MB po rozpakowaniu, Qt + qtbase_pl.qm w `/opt/webpassware`), skrypty przenośne
  Windows/macOS na CMake (niesprawdzone), Qt IFW i stare skrypty usunięte.
  **Skutek do decyzji:** „Aktualizacja przez internet” (`CMaintenanceTool`) wymaga
  `maintenancetool` z Qt IFW; bez niego pokazuje „Brak aktualizacji” (wpis w `TODO`).
- 2026-10-25: A4 zrobione: README (budowa, testy, paczki), CLAUDE.md (Build/Tests),
  `TODO` z odłożonymi decyzjami. **Etap A zakończony.**
- 2026-10-25: B1 zrobione (`2768ea0`): `wpw_core` (Qt Core + Sql) / `wpw_gui` /
  `webpassware`; kursor przez `wpw::BusyIndicator`, hasło główne przez `QSettings`;
  testy bazy pod `QCoreApplication` (ldd: tylko Core, Sql, Test); ctest 6/6.
- 2026-10-25: B2 krok 1 (`cf61e06`): `PassEntryService`/`PassGroupService` bez `Singleton`,
  tworzone w `main.cpp`; konstruktory z fallbackiem `getInstance()` usunięte.
  Następny symbol: `DB` — 33 użycia w szablonie `COrmObject` (Active Record woła
  `DB.*` w każdej metodzie), więc jego usunięcie to przebudowa ORM (repozytorium
  trzyma `CDatabase&`), do uzgodnienia przed realizacją.
- 2026-10-25: decyzje użytkownika: pełne repozytorium zamiast singletonu `DB`;
  akcja aktualizacji tylko z maintenancetool (`4219607`); prawdziwe CP852 (`246529d`).
- 2026-10-25: B2/B3 repozytorium (`f35c12e`): SQL w `COrmRepository(CDatabase&)`,
  modele bez SQL, `CDatabase` z `main.cpp`, makro `DB` usunięte. ctest 7/7 (orm 23,
  service 10, legacydb 6); test dymny: świeża baza z `-c` (schemat bez zmian) i stara
  baza. Zostało w B2: `SETT`, `APPI`, `STYLE`, `LOG_*`, `CAutoType::instance`; w B3:
  prepared statements, typy SQLite, `deleted` 0/1, `TransactionGuard`, migracje.
- 2026-10-25: B3 część 1 (`dc50e80`): wartości przez `bindValue` (koniec wstrzyknięcia
  SQL w `findBy*`), kolumny i ORDER BY tylko z modelu, `CSqlModel` z powiązaniami.
- 2026-10-25: B4 zrobione: `wpw::MasterPassword` (PBKDF2-SHA256, sól, UTF-8; MD5
  przyjmowany i wymieniany przy logowaniu), pierwsze uruchomienie ustawia hasło,
  strona ustawień bez skrótu i z obecnym hasłem, `-l` usunięte, hasła poza logami
  (`2bc7e55`..`cf756b9`). ctest 9/9 (nowe: tst_masterpassword, tst_logindialog).
  Dalej: reszta B3 (typy SQLite, `deleted` 0/1, migracje `user_version`,
  `TransactionGuard`, import CSV w transakcji), potem reszta B2 i B5–B7.
