# ETZ

An embeddable header-only time zone library, written in modern platform-independent C++.

**This project is in development. Target completion early December 2020.**

[![License: BSD-2-Clause](https://img.shields.io/github/license/neilharan/etz.svg)](./LICENSE)

## Overview

There are myriad time zone libraries out there, and even plenty of C++ libraries. They broadly fall into one of these camps:

1. **Abstraction.** Provide a wrapper around the operating systems time zone functionality (e.g. Qt, .NET).
2. **Parser.** Import Olson format files at runtime (on Linux systems these can normally be found in ```/usr/share/zoneinfo```).
3. **Explicit.** Specify time zone and DST details manually (e.g. POSIX).
4. **Black box.** Built in non-human-readable tables.

There is a requirement in embedded systems, that may not have unfettered internet access - and therefore regular OS updates, to manage time zone computations internally. These systems are frequently resource constrained and it may be desirable to embed all, or a subset of, the time zone database directly into the program binary.

## Design goals

- **Platform independence.** We don't depend on the OS or any external files. The library is guaranteed to produce exactly the same result everywhere it's used.
- **Auditable.** The included tables are human-readable and easy to reason over.
- **Configurable**. The whims of governments have led to complex historical rules governing time zone offsets and day light savings (DST). We make the library configurable so the user can specify ```FULL```, ```EPOCH```, or ```LATEST``` to include all rules, only rules since the 1st January 1970, or only the prevailing rules, respectively.
- **Zero start-up cost.** Loading, parsing and indexing files has a one-off runtime cost. We embed the time zone data into the binary, completely moving that cost to compile time.
- **Memory efficiency.** All data is packed, constant and will normally be stored in the binaries .rodata or .text sections. This minimizes stack and heap usage.
- **Performance.** Queries are indexed and cached. Some queries can even be ```constexpr``` with zero runtime cost.

## Getting started

```C++
#include "etz.h"
```

That's it, no installation. The included CMake files are only required for the test tool.

## Example

TODO

## Licensing

ETZ is licensed under the BSD 2-Clause License. See [LICENSE][] for the full license text.

[LICENSE]: https://github.com/neilharan/etz/blob/master/LICENSE
