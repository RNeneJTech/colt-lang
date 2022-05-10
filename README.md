# Colti
### The `colt` interpreter:
An interpreter in development for the `colt` language.

## TODO:
- [X] Add WORD|BYTE
- [X] Fix API to get DWORD|QWORD
- [X] Fix interpreting loop
- [X] Reformat interpreting loop
- [ ] Add a utility to print description of OpCode at runtime
- [ ] Start documenting the code

## Error Codes:
- 0: Successful
- 1: Assertion related failure
- 2: Allocation related failure

## Coding Style:
- Header include guards should use `HG_COLTI_{FILENAME}`, no `pragma once`
- Free Functions that do not expect preconditions should be in `camelCase`
  - Functions that should not be called because they expect preconditions start with `impl_{name}`
- Structs and Unions should be in `PascalCase`: `struct Chunk`
  - Exception: Unions `[DQ]WORD`
  - Structs should be defined in the following way `typedef struct {...} NAME;`
  - Any functions that can be seen as a method for that struct should be in `PascalCase`, starting with the struct name: `ChunkInit(Chunk*)`
- Macros should be in all `UPPERCASE_SNAKE`
  - Only exception is for assertion and allocation macros
  - Function-like macros should use `do { /* CODE */ } while (0)` trick
- Globals should start with `g_`
- Enums should be `UPPERCASE_SNAKE`