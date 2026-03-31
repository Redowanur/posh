# POSH — Personal Optimized SHell

POSH is a tiny Unix-like shell written in C.

<!-- Other fun expansions you can use:
- **Portable Operating-System SHell**
- **POSIX-Oriented SHell**
- **Personal Operating SHell** -->

## What it currently does

- Interactive prompt: `$ `
- Built-ins:
	- `exit` / `exit 0`
	- `echo`
	- `type`
	- `pwd`
	- `cd` (supports `cd ~` and empty `cd` -> `$HOME`)
- Finds executables from `$PATH`
- Runs external commands via `fork` + `execvp`

## Why POSH?

Because it aims to be small, practical, and pleasant to use while you learn how shells work under the hood.
