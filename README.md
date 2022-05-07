# Colti
### The `colt` interpreter:
An interpreter in development for the `colt` language.

## Coding Style:
- Header include guards should use `HG_COLTI_{FILENAME}`, no `pragma once`
- Functions that do not expect preconditions should be in `camelCase`
  - Functions that should not be called because they expect preconditions start with `impl_{name}`
- Macros should be in all `UPPERCASE_SNAKE`
  - Only exception is for assertion macros
  - Function-like macros should use `do { /* CODE */ } while (0)` trick
- Globals should start with `g_`
- Enums should be `UPPERCASE_SNAKE`