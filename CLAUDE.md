# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

**Key domains**: Password manager (Qt Widgets desktop app) — password groups, generator, CSV import/export, auto-type.

## Build

Qt 6.12 (Core, Gui, Sql, Svg, Widgets; Test for the tests), C++17, CMake with presets
and Ninja (`CMakePresets.json`: `debug`, `release`, `coverage`).

```bash
scripts/build.sh                  # debug: configure + build + ctest
scripts/build.sh release --clean
cmake --preset debug && cmake --build --preset debug && ctest --preset debug
```

- Qt from `/opt/Qt/6.12.0/gcc_64`, Ninja from `/opt/Qt/Tools/Ninja/ninja` (both in
  `CMakePresets.json`; `scripts/build.sh --qt DIR` overrides). Never build against the
  system Qt.
- Output: `build/<preset>/webpassware`. The version is `project(VERSION)` in
  `CMakeLists.txt` (define `WEBPASSWARE_VERSION_STR`); configuring also writes
  `CURRENT_VERSION.TXT`.
- Linux links X11 and Xtst (auto-type). Sources of the other platforms are added per
  platform in `CMakeLists.txt`.
- Packaging: `scripts/build-deb.sh` (release + tests + CPack DEB → `build/dist/`; Qt
  deployed into `/opt/webpassware`, see `cmake/Packaging.cmake`, `packaging/`).
  `scripts/windows/build-portable.sh` (MinGW, windeployqt) and
  `scripts/macosx/build-portable.sh` (macdeployqt) build portable archives; they are not
  tested on hardware.

## Tests

Qt Test, one executable per unit, registered in `tests/CMakeLists.txt` via
`wpw_add_test`, linked against `wpw_core` (runs under `QCoreApplication`) or `wpw_gui`.
`ctest` runs them offscreen; `tst_autotype` needs X11 (`QT_QPA_PLATFORM=xcb`).

- `tst_orm` (`tests/ORM_test`): ORM on its own models.
- `tst_service` (`tests/Service_test`): real Database, Models and services; pins the
  table DDL and the declared ORM type names.
- `tst_legacydb` (`tests/LegacyDb_test`): reads `tests/fixtures/legacy_qt5.db`,
  written by the Qt 5 build.
- `tst_schema` (`tests/Schema_test`): `wpw::Schema::upgrade` from an empty file, from the
  legacy file, twice, and refusing a newer file.
- `tst_transactionguard` (`tests/TransactionGuard_test`): commit, rollback, nesting.
- `tst_csv` (`tests/Csv_test`): CSV export bytes and import per encoding.
- `tst_masterpassword` (`tests/MasterPassword_test`): PBKDF2 hash, old MD5 form.
- `tst_logindialog` (`tests/LoginDialog_test`): first-run setup, login, MD5 upgrade.
- `tst_mainwindow` (`tests/MainWindow_test`): main window on a temporary database with a real
  `CApplication` (own `main()`); a timer closes and counts modal dialogs.
- `tst_forms` (`tests/Forms_test`): form field `isDirty()`.
- `tst_maintenancetool` (`tests/MaintenanceTool_test`): update tool availability.
- `tst_autotype` (`tests/AutoType_test`): auto-type sequence parsing.


Migration to Qt 6.12 + CMake and the second design-pattern round:
`.claude/plans/migracja-qt6-i-wzorce.plan.md` (Polish; update its "Postęp" section
after each phase). Reference project: `/home/woronek/git/IpcFirmwareTool`.

## Architecture

CMake targets: `wpw_core` (static; `src/Database`, `src/Models`, the non-GUI parts of
`src/Common`; Qt Core + Sql, never Widgets or Gui), `wpw_gui` (static; everything else
except `main.cpp`) and the `webpassware` executable. The core sees only its own include
directories. It shows activity through `wpw::BusyIndicator` hooks (`src/Database/`),
which `main.cpp` sets to the busy mouse cursor.

### Three-layer pattern

**ORM layer** (`src/Database/`):
- `COrmObject<ModelName>` (`OrmObject.h`) — CRTP base for all domain models: data and
  table description only, no SQL. Fields declared with `ORM_PROPERTY(type, name)`
  (`Q_PROPERTY` plus the declared type name for the column type); every model inherits
  `deleted` and `timestp`. Table name is derived from `metaObject()->className()`.
- `COrmRepository<ModelName>` (`OrmRepository.h`) — the table in a `CDatabase&`: schema
  (create/alter/indexes/drop), save/update/updateProperty, finders, remove/setDeleted,
  count. Implements `IRepository<ModelName>`, which the services use.
- `COrmService<ModelName>` (`OrmService.h`) — service base: catches `CExceptionSql`, keeps
  the error text, offers `Owned*` (QSharedPointer) finders; built on a repository.
- `CDatabase` (`Database.h/.cpp`) — wraps `QSqlDatabase`, **SQLite only** (`QSQLITE`
  driver). One instance, created in `main.cpp` and passed by reference (services,
  `CSqlModel`, `ModelTableCheck`, login dialog, database settings page).
- Support classes: `SqlField`, `SqlModel`, `SqlQuery`, `CException`/`CExceptionSql`.

**Models** (`src/Models/`):
- `PassEntry` / `PassGroup` — domain models (`class PassEntry : public COrmObject<PassEntry>` with `ORM_PROPERTY` fields named `m_xxx`).
- `PassEntryService` / `PassGroupService` — matching service classes.
- `ModelTableCheck` — creates missing tables, columns and indexes from the models.
- `wpw::Schema` (`Schema.h`) — `kVersion` in `PRAGMA user_version` (Qt 5 files have 0) and
  the append-only migrations; `upgrade()` runs at every start (`main.cpp`). `-c` re-runs
  `ModelTableCheck` by hand.

**View/Controller layer** (`src/Gui/`):
- Base classes: `AbstractFormDialog` + `AbstractFormDialogController`, `AbstractMainWindow`.
- `src/Gui/ModelForms/` — entity dialogs paired with controllers (`PassEntryDialog` + `PassEntryDialogController`, same for `PassGroup`).
- `src/Gui/Dialogs/` — app dialogs (`LoginDialog`, `PasswordGenerateDialog`, `CsvImportDialog`/`CsvExportDialog` + controllers, `AboutDialog`, `AbstractCloseDialog`).
- `src/Gui/Settings/` — settings views with controllers (`AbstractSettingsItemView`/`Controller`, `SettingsDialog`).
- `src/Gui/Widgets/` — custom widgets (`TableView`, `ToggleSwitch`, `PasswordGeneratorWidget`, `Button`, `DelayEditLine`, …). Prefer these over raw Qt widgets.
- `CApplication` (`src/Application.h`) — `QApplication` subclass; owns the `CStyle`,
  shows the login dialog (`run(CDatabase&)`).
- `CWebPassWareMainWindow` (`src/WebPassWareMainWindow.h`) — main window.

### Common utilities (`src/Common/`)
- State persistence: `AbstractDialogStateManager`, `AbstractTableStateManager`, `AbstractSplitterStateManager`, `AbstractTabMainWindowStateManager` + corresponding `*State` classes.
- Auto-type subsystem: `AutoType`, `AutoTypeAction`, `AutoTypePlatform{Linux,Macos,Windows,Test}`.
- `PasswordGenerator`, `ImportCsv`/`ExportCsv`/`CsvModel`, `Logger`, `Tools`, `MaintenanceTool` (internet update).
- `Singleton.h`, `GenericFactory.h`.
- `Global.h` — getter/setter boilerplate macros (`ADD_FIELD`, `ADD_PTR_PROPERTY`, `ADD_PTR`, smart-pointer variants) and `safe_delete`/`qobject_safe_delete`.

## Coding rules (from AGENTS.md)

- Match the style of the file you are editing. Prefer consistency over introducing new patterns.
- Keep changes small and focused. No broad refactors unless explicitly requested.
- Do not rename public APIs, signals, slots, or widely used members for cleanup.
- Use `tr()` for all user-visible strings.
- Use `nullptr`, not `NULL` or `0`.
- Prefer parent-owned Qt widgets over manual memory management.
- Naming: classes/methods `PascalCase`, members `m_camelCase`, locals `camelCase`, slots `onXxx`.
- Always use braces for `if`/`else`/`for`/`while`/`do`, even single-statement blocks.
- Prefer early returns for invalid state checks.
- Include order: matching header first, then Qt/system headers, then project headers.
- Avoid formatting-only diffs. Avoid clang-tidy for refactoring — analysis only.

### Before finishing any change, verify:
- Signal/slot signatures still match their connections.
- Widget ownership is still valid.
- Array/vector/index access is bounded.
- Unsaved-changes flow still works.
- UI state transitions still make sense.

## Engineering rules

- **TDD**: failing Qt Test first, then the minimal code, then refactor with green
  tests. Characterization tests before touching legacy behaviour (ORM, forms, CSV).
- **SOLID** and the patterns in use: Repository (`COrmRepository`), Template Method
  (`AbstractFormDialog`, settings pages), Strategy (`AutoTypePlatform*`, `ICState`),
  Factory (auto-type platform, settings pages), Facade (`PasswordVault`), Observer
  (Qt signals), Command (main window actions), RAII transaction. No patterns
  "for later" (YAGNI).
- Dependencies injected through constructors, composed in `main.cpp` (`CApplication`,
  `CDatabase`, the services); no global singletons or macros like the former `DB` /
  `SETT` / `APPI` / `STYLE`. Settings: create a `CApplicationSettings` (plain
  `QSettings`) where needed. Deliberate exceptions: the file loggers
  (`LOG_APP` / `LOG_SQL`, `CAppLogger` / `CSqlLogger`) and, until it is reworked,
  `CAutoType::instance()`.
- `wpw_core` never links Qt Widgets.
- SQL only through `prepare` + `bindValue`; identifiers only from `metaObject()`.
  Schema changes: bump `kSchemaVersion` and add a migration (tested from empty,
  from N-1 and from the legacy fixture database).
- Secrets: never log passwords; master password only as salted PBKDF2-SHA256.
- Coverage target 80%+ for `wpw_core`.

## Style

- C++17, Qt 6 APIs only (function-pointer connects, `QRegularExpression`,
  `QStringDecoder`, `QMetaType`, range-for instead of `Q_FOREACH`).
- Existing classes keep the `C` prefix; new classes go to namespace `wpw` without
  it. Members `m_camelCase`, slots `onXxx`; always braces; `nullptr`.
- User-visible strings through `tr()` (source strings in Polish). New and changed
  comments in English and explain why; no references to plan phases in code.
- Shell scripts: `set -euo pipefail`, usage from the header comment, `die()` helper.

## Git

- Conventional commits (`feat:`, `fix:`, `docs:`, `refactor:`, `test:`, `build:`),
  description in Polish, one logical change per commit, tests green before
  committing.
- Never add `.gitignore` to commits; no `Co-Authored-By:` trailers.
- `build/`, `bin/`, `.obj/`, `.moc/` and `*.user` stay out of git.
