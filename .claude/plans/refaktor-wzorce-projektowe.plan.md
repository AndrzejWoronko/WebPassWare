# Plan: Refaktor WebPassWare do wzorców projektowych

**Źródło**: żądanie użytkownika (bez PRD)
**Złożoność**: ŚREDNIO-WYSOKA (18-30 h)
**Status**: zatwierdzony; realizacja fazami; każda faza = osobny, kompilowalny i przetestowany krok (`refactor:` / `test:`, opis po polsku)

## Podsumowanie

Uporządkowanie architektury wokół jawnych wzorców projektowych etapami, bez przepisywania aplikacji i bez zmiany zachowania ukrytej pod refaktorem. Najpierw siatka bezpieczeństwa, potem małe ruchy strukturalne: Repository jako kompozycja, ograniczenie singletonów przez Dependency Injection, jawniejsza obsługa błędów, RAII dla wyników finderów i opcjonalnie Factory/API pól formularzy.

## Skills używane przy realizacji

Plan i realizacja korzystają z lokalnych/globalnych skills:

| Skill | Zastosowanie w planie |
|---|---|
| `working-effectively-with-legacy-code` | najpierw characterization tests, potem najmniejszy seam i dopiero zmiana struktury |
| `refactoring` | refaktor zachowuje zachowanie; małe, odwracalne, testowalne kroki |
| `refactoring-guru` | wzorce są odpowiedzią na konkretny smell, nie celem samym w sobie |
| `tdd-workflow` | testy przed ryzykownym refaktorem; ścieżki błędów i edge cases |
| `verification-loop` | build, testy, diff review i kontrola regresji po każdej fazie |
| `coding-standards` | prostota, brak spekulatywnych abstrakcji, czytelne nazwy i odpowiedzialności |
| `qt-cpp-review` checklist | kontrola QObject ownership, signal/slot, lifecycle, raw pointerów i kontraktów Qt |

## Zasady realizacji

- Refaktor ma być zachowawczy: zmiana struktury nie może po cichu zmieniać semantyki.
- Każda faza ma mieć jasno nazwany smell, minimalny zabieg, test/seam i warunek stop.
- Testy charakteryzacyjne chronią obecne zachowanie; testy docelowe dla zmiany semantyki dodajemy osobno.
- Nie dodawać `Q_OBJECT` do szablonów, jeśli nie ma twardej potrzeby i bezpiecznej obsługi `moc`.
- Nie zastępować Qt ownership ani wzorców rodzic-dziecko tam, gdzie działają lokalnie.
- Publiczne API zmieniać z przejściem kompatybilnym, chyba że faza jawnie dotyczy tej zmiany.
- Przy każdym kroku sprawdzić: signal/slot, własność widgetów, bounded access, unsaved-changes flow i stan UI.

## Diagnoza (zweryfikowana w kodzie)

### Wzorce już obecne - zachować, nie ruszać

| Wzorzec | Gdzie |
|---|---|
| Template Method | `AbstractFormDialog`/`AbstractFormDialogController`, `AbstractSettingsItemView` |
| Strategy | `AutoTypePlatform{Linux,Macos,Windows,Test}` (`src/Common/`) |
| Memento | `*StateManager` + `*State` (stan dialogów/tabel/splitterów) |
| Factory | `src/Common/GenericFactory.h` (istnieje, mało używany) |
| MVC (częściowo) | pary Dialog + Controller w `src/Gui/ModelForms/` |

### Problemy do refaktoru

1. **Active Record jako god-template** - `src/Database/OrmObject.h` (983 linie): schemat, query building, persystencja i model domenowy w jednej klasie CRTP.
2. **Nadużycie dziedziczenia** - `COrmService<T> : public COrmObject<T>` (`OrmService.h:9`), a jednocześnie kompozycja `m_object`. Martwe/niebezpieczne API: `getObject()` zwraca właścicielskie `unique_ptr` jako surowy wskaźnik.
3. **Błąd ukryty przez brak instancjacji** - `COrmService::getObjectsBy(field, value)` używa `this->clearError()`, mimo że błąd należy do `m_service`.
4. **Singletony jako globalny stan** - serwisy (`PassEntryService`, `PassGroupService`), `DB`, `APPI`, `Logger`, `Style`, `ApplicationSettings`. Kontrolery wołają `::getInstance()` w środku metod. `Singleton<T>` używa gołego `new` + `atexit`; `SingletonHolder<T>` jest bezpieczniejszy lokalnie.
5. **Połykane błędy** - `COrmService` łapie wyjątki, zapisuje `m_error`, zwraca pustą listę/`nullptr`; wywołujący często nie sprawdzają `getError()`.
6. **Niejasna własność pamięci** - findery zwracają `QList<ModelName*>` surowych wskaźników; caller ma ręcznie zwalniać.
7. **`dynamic_cast` w kontrolerach** do typów pól formularzy (`PassEntryDialogController`, `PassGroupDialogController`).

### Punkty zaczepienia

- `ICDatabase` (`src/Database/Database.h`) i `ICService` (`src/Database/Service.h`) - istniejące interfejsy pod Repository/DI.
- `SingletonHolder<T>` - dostępna alternatywa dla części singletonów.
- `QSharedPointer`/`std::unique_ptr` już występują w projekcie i mogą być użyte bez nowego frameworka.
- `tests/ORM_test/` - istniejący qmake/QtTest wzorzec dla kolejnych testów.

## Fazy

### Faza 0 - Siatka bezpieczeństwa (testy charakteryzacyjne) [3-5 h]

**Smell**: brak szybkiej ochrony dla ryzykownego refaktoru ORM/serwisów.

#### 0A - Baseline
- **Akcja**: uruchomić obecny build i istniejące testy bez zmian produkcyjnych.
- **Walidacja**:
  ```bash
  qmake WebPassWare.pro && make -j$(nproc)
  cd tests/ORM_test && qmake && make && ./test_orm
  ```
- **Stop condition**: znany stan startowy zapisany w podsumowaniu prac.

#### 0B - ORM characterization
- **Akcja**: rozbudować `tests/ORM_test/` tylko o brakujące ścieżki obecnego zachowania, jeśli baseline ujawni lukę: `findBy`, `setDeleted`, puste wyniki, ścieżki błędów.
- **Zasada**: testy opisują obecne zachowanie, nawet jeśli jest brzydkie.

#### 0C - Service characterization
- **Akcja**: dodać `tests/Service_test/` wzorowany na `test_orm.pro`.
- **Zakres testów**:
  - `PassEntryService` i `PassGroupService`: `addObject`, `editObject`, `getObject`, `getObjects`, `getObjectsBy`, `removeObject`, `deleteObject`, `getError`.
  - Kompilacyjnie wymusić instancjację ścieżki `getObjectsBy(field, value)`.
  - Udokumentować caller-owned raw pointers i sprzątanie w testach.
- **Walidacja**:
  ```bash
  cd tests/Service_test && qmake && make && ./test_service
  ```

### Faza 1 - Repository przez kompozycję, bez zmiany API publicznego [4-6 h]

**Smell**: service dziedziczy po modelu, choć faktycznie ma model-repozytorium jako collaborator.

1. Mały krok stabilizujący:
   - `getObject()` bezpiecznie zwraca `m_object.get()`,
   - `NULL` w dotykanym kodzie -> `nullptr`,
   - `this->clearError()` -> `m_service->clearError()`,
   - test z Fazy 0C musi to potwierdzać.
2. Wprowadzić `IRepository<ModelName>` / `COrmRepository<ModelName>` w `src/Database/` jako cienki adapter wokół istniejącego `COrmObject<ModelName>`.
3. `COrmService<T>` przestaje dziedziczyć po `COrmObject<T>` i używa repozytorium przez kompozycję.
4. Publiczne API `COrmService` zostaje początkowo kompatybilne (`QList<ModelName*>`, `ModelName*`, `bool`, `qint64`).

**Poza zakresem**: zdjęcie Active Record z modeli (`PassEntry`/`PassGroup` zostają na `COrmObject`).

### Faza 2 - Dependency Injection dla kontrolerów i serwisów [4-6 h]

**Smell**: kontrolery sięgają po globalne singletony w środku logiki.

- Dodać konstruktory przyjmujące zależności, zachowując stare konstruktory jako delegujące fallbacki do singletonów.
- Kolejność:
  1. `PassGroupDialogController`,
  2. `PassEntryDialogController`,
  3. kontrolery CSV,
  4. `WebPassWareMainWindow` / composition root.
- Nie wymuszać od razu interfejsów wszędzie, jeśli prosta referencja/wskaźnik daje wystarczający seam.
- `APPI`/`DB` zostają jako fasady przejściowe; `Logger`/`Style` można migrować do `SingletonHolder<T>` osobno.

### Faza 3 - Jawna obsługa błędów bez `QObject` w generycznym serwisie [2-4 h]

**Smell**: ciche awarie i `getError()` ignorowany przez wywołujących.

- Nie dodawać `Q_OBJECT`/sygnałów do `COrmService<T>` jako szablonu.
- Preferowany wariant:
  - zachować `getError()` jako przejściowy kanał,
  - dodać jawniejsze wyniki operacji tam, gdzie caller potrzebuje rozróżnić "brak danych" od "błąd",
  - GUI po operacji sprawdza wynik i pokazuje `CMessageBox` z `tr()` dla tekstów użytkownika.
- Testy charakteryzacyjne z Fazy 0 zostają jako ochrona starego API; zmienione zachowanie dostaje nowe testy docelowe.

### Faza 4 - Własność pamięci i RAII wyników finderów [3-5 h]

**Smell**: caller-owned `QList<ModelName*>` wymusza ręczne `delete` i łatwo przecieka.

- Najpierw dodać helper/alias ownership w repozytorium, bez masowej zmiany call sites.
- Potem migrować API po jednym finderze/serwisie na commit.
- Preferować istniejące style projektu: `QSharedPointer` w Qt-facing kodzie lub `std::shared_ptr` tam, gdzie lokalnie już pasuje.
- Po każdym kroku usuwać ręczne `safe_delete` tylko tam, gdzie ownership naprawdę przeszedł do smart pointera.

### Faza 5 (opcjonalna) - Factory/API formularzy zamiast `dynamic_cast` [2-4 h]

**Smell**: kontrolery znają konkretne klasy pól i powtarzają downcasty.

- Najpierw małe API w `CFormAbstractField` albo helpery typowanych pól, jeśli to usuwa realne powtórzenie.
- `GenericFactory` użyć tylko wtedy, gdy tworzenie dialogów/pól ma rzeczywistą powtarzalność.
- Faza niezależna od Repository, ale bezpieczniejsza po DI kontrolerów.

## Zależności

Faza 0 -> wszystkie. Faza 1 -> 3 -> 4. Faza 2 może iść po 0 albo po 1, ale preferowana kolejność to 0 -> 1 -> 2, żeby najpierw ustabilizować serwisy. Faza 5 niezależna, ale najlepiej po 2.

## Walidacja po każdej fazie

```bash
qmake WebPassWare.pro && make -j$(nproc)
cd tests/ORM_test && qmake && make && ./test_orm
cd ../Service_test && qmake && make && ./test_service   # od Fazy 0C
git diff --stat
git diff --check
```

Jeśli build/test pada z przyczyn zastanych, zapisać dokładny błąd i nie mieszać naprawy builda z refaktorem, chyba że jest to konieczny mały krok stabilizujący.

## Ryzyka

| Ryzyko | Poziom | Mitygacja |
|---|---|---|
| Interakcja szablonów z `moc` (`Q_OBJECT` + CRTP) | WYSOKIE | Nie dodawać `Q_OBJECT` do szablonów; nie ruszać mechanizmu `ORM_PROPERTY` bez testów |
| Brak CI | ŚREDNIE | Faza 0 najpierw; build + testy przed każdym krokiem |
| Zmiana semantyki błędów | ŚREDNIE | Rozdzielić characterization tests i testy docelowe |
| Podwójne zwolnienia / wycieki | ŚREDNIE | Jeden finder/serwis na commit; RAII dopiero po kompatybilnym repozytorium |
| Konflikt z "no broad refactors" | NISKIE | Refaktor zamówiony; małe, reviewable kroki |
| Ukryte zależności singletonów w GUI | ŚREDNIE | DI z fallbackiem do obecnych singletonów, potem stopniowe usuwanie reach-through |

## Akceptacja

### Postęp

- 2026-06-13: Faza 0A/0C wykonana. Główny build przechodzi; `ORM_test` ma 23/23 pass; `Service_test` ma 6/6 pass.
- 2026-06-13: wykonany pierwszy stabilizujący krok Fazy 1: `COrmService::getObject()` zwraca `m_object.get()`, a `getObjectsBy(field, value)` czyści błąd przez `m_service`.
- 2026-06-13: Faza 1 wykonana. Dodano `IRepository<ModelName>`/`COrmRepository<ModelName>` jako cienki adapter nad `COrmObject`; `COrmService` używa kompozycji i zachowuje publiczne API.

- [x] Faza 0A: baseline build/test opisany.
- [x] Faza 0B/0C: testy charakteryzacyjne ORM/Service przechodzą.
- [x] Faza 1: `COrmService` bez dziedziczenia po modelu, publiczne API kompatybilne.
- [ ] Faza 2: kontrolery mają dependency seam; bez `::getInstance()` w logice metod docelowych.
- [ ] Faza 3: brak cichych awarii w głównych ścieżkach GUI.
- [ ] Faza 4: finder API migrowane do RAII etapami.
- [ ] (opc.) Faza 5: mniej lub brak `dynamic_cast` w kontrolerach formularzy.
- [x] Po Fazie 0: build + wszystkie dostępne testy zielone.
- [x] Po Fazie 1: build + wszystkie dostępne testy zielone.
