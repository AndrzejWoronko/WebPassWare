# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

**Key domains**: Password manager (Qt Widgets desktop app) — password groups, generator, CSV import/export, auto-type.

## Build

Qt 5 (modules: core, gui, sql, svg), C++14, qmake.

```bash
qmake WebPassWare.pro
make -j$(nproc)
```

Build outputs: `bin/` (executable `webpassware`). Version is defined in `includes.pri` (`WEBPASSWARE_VERSION_*`), which also regenerates `CURRENT_VERSION.TXT` on every qmake run. Platform packaging scripts live in `scripts/{linux,macosx,windows}/`, installer files in `installer/`.

## Tests

One standalone test project, **not** built by the main .pro:

```bash
cd tests/ORM_test
qmake test_orm.pro
make -j$(nproc)
./test_orm
```

## Architecture

### Three-layer pattern

**ORM layer** (`src/Database/`):
- `COrmObject<ModelName>` (`OrmObject.h`) — CRTP base for all domain models. Fields declared with `ORM_PROPERTY(type, name)` macro (uses `Q_PROPERTY` internally); every model inherits `deleted` and `timestp` properties. Table name is derived from `metaObject()->className()`.
- `COrmService<ModelName>` (`OrmService.h`) — inherits `COrmObject<ModelName>`; service base for domain entities.
- `CDatabase` (`Database.h/.cpp`) — wraps `QSqlDatabase`, **SQLite only** (`QSQLITE` driver).
- Support classes: `SqlField`, `SqlModel`, `SqlQuery`, `CException`/`CExceptionSql`.

**Models** (`src/Models/`):
- `PassEntry` / `PassGroup` — domain models (`class PassEntry : public COrmObject<PassEntry>` with `ORM_PROPERTY` fields named `m_xxx`).
- `PassEntryService` / `PassGroupService` — matching service classes.
- `ModelTableCheck` — schema verification.

**View/Controller layer** (`src/Gui/`):
- Base classes: `AbstractFormDialog` + `AbstractFormDialogController`, `AbstractMainWindow`.
- `src/Gui/ModelForms/` — entity dialogs paired with controllers (`PassEntryDialog` + `PassEntryDialogController`, same for `PassGroup`).
- `src/Gui/Dialogs/` — app dialogs (`LoginDialog`, `PasswordGenerateDialog`, `CsvImportDialog`/`CsvExportDialog` + controllers, `AboutDialog`, `AbstractCloseDialog`).
- `src/Gui/Settings/` — settings views with controllers (`AbstractSettingsItemView`/`Controller`, `SettingsDialog`).
- `src/Gui/Widgets/` — custom widgets (`TableView`, `ToggleSwitch`, `PasswordGeneratorWidget`, `Button`, `DelayEditLine`, …). Prefer these over raw Qt widgets.
- `CApplication` (`src/Application.h`, Singleton via `APPI` macro) — `QApplication` subclass.
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
