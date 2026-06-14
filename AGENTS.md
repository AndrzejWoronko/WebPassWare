# AGENTS.md

## Follow these rules when editing this repository
- Match the style of the file you edit.
- Keep changes small and focused.
- Do not perform broad refactors unless explicitly requested.
- Do not rename public APIs, signals, slots, or widely used members just for cleanup.
- Preserve existing behavior unless the task explicitly requires a behavior change.
- If a deeper subdirectory contains its own `AGENTS.md`, follow the more local instructions for files in that subtree.

## Project profile
- This is a Qt Widgets C++ password manager (password groups, generator, CSV import/export, auto-type).
- Prefer consistency with the surrounding file over introducing a new style or framework pattern.
- Prefer existing project widgets, models, and UI patterns over new abstractions.

## Boundaries
- Do not edit vendor code or imported external sources unless explicitly asked.
- Do not reformat vendor code.
- Link to external or vendor documentation instead of duplicating it.

## Language and framework
- Use Qt idioms and Qt types in Qt-facing code.
- Use `tr()` for all user-visible strings.
- Use `nullptr`, not `NULL` or `0`.
- Prefer parent-owned Qt widgets over manual memory management.
- Do not replace Qt ownership or container patterns unless there is a clear local reason.

## Code style
- Classes and methods: `PascalCase`
- Members: `m_` + `camelCase`
- Locals: `camelCase`
- UI handlers / slots: `onXxx...`
- Keep braces, spacing, and pointer/reference formatting consistent with the edited file.
- Avoid formatting-only diffs.
- Avoid using clang-tidy for refactor.

### Control flow
- Prefer early returns for invalid state checks.
- Always use braces for `if`, `else`, `for`, `while`, and `do` blocks, even for a single statement.
- Use early guard clauses to keep the main path readable.

## UI and event handling
- Preserve current UI flow and state transitions.
- Keep signal/slot signatures and connect behavior correct.
- Avoid duplicate signal connections when code paths can run more than once.
- Keep user confirmation flows intact when closing dialogs or switching context.

## Comments and includes
- Keep comments short and practical.
- Prefer comments that explain intent or assumptions.
- Keep the matching header first, then Qt/system headers, then project headers.

## Preferred edits
- Acceptable: local cleanup, missing initialization, guard clauses, small helper extraction, removal of stale debug text in touched code.
- Avoid unless requested: mass renaming, architecture rewrites, replacing Qt Widgets patterns with new frameworks, broad formatting-only commits.

## Before finishing
Verify:
- signal/slot signatures still match
- widget ownership is still valid
- array/vector/index access is bounded
- unsaved-changes flow still works
- UI state transitions still make sense
- touched code still follows local project conventions

## Refactor workflow
- For repeated staged refactors, prefer repository skills over one-off prompting.
- When refactoring service classes that inherit from `Singleton<T>`, use the `service-singleton-to-static` skill if available.
- For large refactors, produce a plan first and implement in small reviewable passes.
