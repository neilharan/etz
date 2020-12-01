# ETZ

An embeddable header-only time zone library, written in modern platform-independent C++.

**This project is in development. Target completion early December 2020.**

![Linux](https://github.com/neilharan/etz/workflows/linux/badge.svg?branch=master)
![Windows](https://github.com/neilharan/etz/workflows/windows/badge.svg?branch=master)
[![License: BSD-2-Clause](https://img.shields.io/github/license/neilharan/etz.svg)](./LICENSE)
[![C++ Standard](https://img.shields.io/badge/C%2B%2B-17%2F20-blue.svg)](https://img.shields.io/badge/C%2B%2B-17%2F20-blue.svg)

## Overview

There are myriad time zone libraries out there, and even plenty of C++ libraries. They broadly fall into one of these camps:

1. **Abstraction.** Provide a wrapper around the operating systems time zone functionality (e.g. Qt, .NET).
2. **Parser.** Import Olson format files at runtime (on Linux systems these can normally be found in ```/usr/share/zoneinfo```).
3. **Explicit.** Specify time zone and DST details manually (e.g. POSIX).
4. **Black box.** Built in non-human-readable tables.

There is a requirement in embedded systems, that may not have unfettered internet access - and therefore regular OS updates, to manage time zone computations internally. These systems can be resource constrained and it may be desirable to embed all, or a subset of, the time zone database directly into the program binary.

## Design goals

- **Platform independence.** We don't depend on OS time zone functionality or external files. The library is guaranteed to produce exactly the same result everywhere it's used.
- **Auditable.** The included tables are human-readable and easy to reason over.
- **Configurable**. The whims of governments have led to complex historical rules governing time zone offsets and day light savings (DST). We make the library configurable so the user can specify ```ALL```, ```DEFAULT```, or ```CURRENT``` to include all rules, only rules since the 1st January 1970, or only the prevailing rules, respectively.
- **Zero start-up cost.** Loading, parsing and indexing files has a one-off runtime cost. We embed the time zone data into the binary, completely moving that cost to compile time.
- **Memory efficiency.** All data is packed, constant, and will normally be stored in the binaries .rodata or .text sections. This minimizes stack and heap usage.
- **Performance.** Queries are indexed and cached. The common use case (repetitive queries for the same time zone and an incrementing time parameter) has negligible cost. Some queries can even be ```constexpr``` with zero runtime cost.

## Compiler support

All commits are automatically built with:

- gcc 10 (Linux)
- gcc 9 (Linux)
- gcc 8 (Linux)
- clang 10 (Linux)
- clang 11 (Linux)
- msvc 2016 (Windows win32 & x64)
- msvc 2019 (Windows win32 & x64)

## Getting started

```C++
#include "etz.h"
```

That's it, no installation. The included CMake files are only required for the test tool.

It's normally best to include etz.h in your precompiled header.

## Example

```C++
#include "etz.h"
using namespace ETZ;
...

// Convert some UTC ISO string to internal time type (which is a pair of time_t and boolean to indicate parse result)...
const auto time = Time::fromISOString("2006-04-19T13:14:15");
if (!time.second) {
    // Parse error
}

// Display with an arbitrary time zone...
std::cout << Time::toISOString(UTC::toLocal(TimeZone::America_New_York, time.first).first).c_str();
// -> 2006-04-19T08:14:15
```

## Datasets

TODO

## Licensing

ETZ is licensed under the BSD 2-Clause License. See [LICENSE][] for the full license text.

[LICENSE]: https://github.com/neilharan/etz/blob/master/LICENSE
