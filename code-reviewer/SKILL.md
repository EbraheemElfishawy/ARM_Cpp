---
name: code-reviewer
description: Analyzes local source code files for styling errors, naming convention violations, and best practices. Use this when a user asks to review code, check style, or ensure adherence to coding standards.
---

# Code Reviewer

This skill enables Gemini CLI to perform comprehensive style reviews of local source code files.

## Workflow

1.  **Identify Target Files**: Determine which files the user wants to review.
2.  **Determine Language & Standards**: Identify the programming language and any existing project-specific style guides (e.g., `.clang-format`, `.eslintrc`, `pyproject.toml`).
3.  **Analyze Code**: Compare the code against the [General Style Guide](references/style-guide.md) or project-specific rules.
4.  **Report Findings**: Provide a structured report of styling errors, naming issues, and recommended improvements.

## Capabilities

### 1. General Style Review
Analyzes code for common styling issues:
- Indentation and spacing.
- Line length.
- Bracing styles.
- Naming conventions (classes, functions, variables).

### 2. Best Practices & Documentation
Checks for:
- Missing or inadequate comments.
- Potential code smells (long functions, deep nesting).
- Use of language-specific best practices.

### 3. Linter Integration
If the project has a linter configured, this skill can run it and interpret the results.
- **C++**: Look for `.clang-format` or `CPPLINT.cfg`.
- **Python**: Look for `flake8`, `pylint`, or `black` configuration.
- **JS/TS**: Look for `.eslintrc` or `prettier` configuration.

## Examples

### User: "Review main.cpp for styling errors."
**Action**: Read `main.cpp`, check against C++ style guidelines in `references/style-guide.md`, and report findings.

### User: "Check if my python scripts follow PEP 8."
**Action**: Search for `.py` files, analyze them using PEP 8 standards, and provide a summary of violations.

## Resources

- [references/style-guide.md](references/style-guide.md): General style guidelines for C++, Python, and JS/TS.
