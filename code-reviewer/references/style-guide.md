# General Code Style Guide

This reference provides general style guidelines for common programming languages. Use these when no project-specific style is defined.

## C++ Guidelines

- **Naming**:
  - Classes: `PascalCase`
  - Methods/Functions: `camelCase`
  - Variables: `camelCase` or `snake_case` (be consistent)
  - Constants: `SCREAMING_SNAKE_CASE`
  - Private Members: Prefix with `m_` or suffix with `_`
- **Indentation**: 4 spaces (prefer spaces over tabs).
- **Braces**: K&R style (opening brace on same line) or Allman style (opening brace on new line). Be consistent.
- **Comments**: Use Doxygen-style (`///` or `/** ... */`) for headers.

## Python Guidelines (PEP 8)

- **Naming**:
  - Classes: `PascalCase`
  - Functions/Variables: `snake_case`
  - Constants: `SCREAMING_SNAKE_CASE`
- **Indentation**: 4 spaces.
- **Line Length**: Max 79 characters.
- **Imports**: One per line, grouped (stdlib, third-party, local).

## JavaScript/TypeScript Guidelines

- **Naming**:
  - Classes: `PascalCase`
  - Functions/Variables: `camelCase`
  - Constants: `SCREAMING_SNAKE_CASE`
- **Indentation**: 2 or 4 spaces.
- **Quotes**: Prefer single quotes `'` unless using template literals.
- **Semicolons**: Always use them.

## General Principles

- **DRY**: Don't Repeat Yourself.
- **KISS**: Keep It Simple, Stupid.
- **Documentation**: All public APIs should have comments explaining parameters and return values.
- **Error Handling**: Use exceptions or error codes consistently.
