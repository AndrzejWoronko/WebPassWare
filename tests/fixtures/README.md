# Test fixtures

## legacy_qt5.db

SQLite database written by the Qt 5.15 build (2026-10-25), before the move to
Qt 6. `tests/LegacyDb_test` reads it through the services: databases users
already have look like this one and must keep working.

How it was made: a throw-away program linked against `src/Database` and
`src/Models` ran `ModelTableCheck::checkAllTables()` (what `webpassware -c`
does) and then wrote the data through `PassGroupService` / `PassEntryService`
under an offscreen `QApplication`. It cannot be regenerated after the Qt 6
migration; that is the point of keeping the file.

Contents:

- `pass_group`: 0 "Wszystkie grupy", 1 "Bankowość", 2 "Usunięta grupa"
  (soft-deleted, `deleted` stored as the text `'true'`);
- `pass_entry`: 1 "Konto główne" (group 1, Polish characters, `;` and `"` in
  the password), 2 "Poczta" (group 0), 3 "Stare konto" (soft-deleted);
- the indexes created by `ModelTableCheck`.
