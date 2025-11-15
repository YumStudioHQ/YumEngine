# YumEngine — Lua & Godot Runtime

## Version

<!-- YUM_VERSION_START -->
- **Engine version:** `2.1.39`
- **Studio:** `yum-official`
- **Branch:** `stable`
<!-- YUM_VERSION_END -->

---

**[Website](https://yumstudiohq.github.io/YumEngine/)**
**[Documentation](./docs/html/)**

## Overview

YumEngine is a lightweight, cross-language runtime and engine bridging **C++ / C** with **Lua** and **Godot**.  
It is designed for easy integration, extensibility, and rapid game development or scripting automation.

Key goals:  
- Minimal overhead  
- Maximum flexibility  
- Cross-language interoperability  

---

## Features

- **Dynamic Library (Yum)**: Provides core utilities accessible from C, C++, Lua, and C#.  
- **YumEngine**: Full engine with linked Yum library, including API and internal classes.  
- **YumiEngine**: Engine only, depends on Yum library.  
- **Variant & Vector**: Type-safe, C-compatible containers for multi-language data exchange.  
- **Cross-Language Callbacks**: Register functions callable from Lua, C#, or other languages.  

---

## Requirements

- C++20 compiler (or higher, e.g., `g++-15` on macOS)  
- C17 compatible compiler  
- Optional: Lua interpreter to run scripts  

---

## Installation & Building

Official builds are precompiled. You usually **don’t need to compile** anything manually.  

To build manually:

```bash
lua release.lua [patch|minor|major]
````

**Notes:**

* Adjust compiler paths or flags in `release.lua` if needed.
* Version bumping is handled automatically via `release.lua version [patch|minor|major]`.
* A `version.js` file is generated for website integration.
* release [file](./src/release/release.lua)

---

## Downloads

Get the latest builds here:

[Download YumEngine](https://yumstudiohq.github.io/YumEngine/pages/download.html)

For source code and contributions, see the GitHub repository.

---

## Studio

**Yum Studio** is the team behind YumEngine:

* Studio: `yum-official`
* Branch: `stable`

We are a small, experimental studio passionate about cross-language runtime engines and anime-inspired development tools.

---

## Contributors

* **MONOE** — Lead Developer ([GitHub](https://github.com/wys-prog))
* **YumStudio** — Someone ... ([GitHub](https://github.com/yumstudiohq))

---

## License

[License](./LICENSE)

