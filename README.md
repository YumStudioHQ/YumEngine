# YumEngine

<!-- YUM_VERSION_START -->
- **Engine version:** `3.0.8`
- **Studio:** `yum-official`
- **Branch:** `master-stable`
<!-- YUM_VERSION_END -->

**[Website](https://yumstudiohq.github.io/YumEngine/)** | **[Documentation](./docs/html/)**

---

## Overview

YumEngine is a lightweight, cross-language runtime and engine designed to make **Lua integration with Godot** simple, flexible, and programmatic.

Originally built to support Lua in Godot through the C# API, YumEngine has evolved into a **general-purpose Lua runtime** with **multi-language interoperability**, focusing on usability and modern scripting workflows without requiring deep Lua stack management.

**Key goals:**

* Minimal overhead
* Maximum flexibility
* Easy cross-language integration (C, C++, Lua, C#)
* Simplified Lua usage in Godot and other applications

---

## Features

* **Dynamic Library (Yum)** — Core utilities accessible from C, C++, Lua, and C#.
* **YumEngine** — Complete engine with linked Yum library, APIs, and internal classes.
* **YumiEngine** — Engine-only build, depends on Yum library.
* **Variant & Vector** — Type-safe, C-compatible containers for cross-language data exchange.
* **Cross-Language Callbacks** — Register functions callable from Lua, C#, or other languages.
* **Simplified Lua Integration** — Focus on programmatic Lua usage without manual stack management.

---

## Requirements

* **C++20 compiler** (or higher, e.g., `g++-15` on macOS)
* **C17-compatible compiler**
* Lua interpreter to run scripts
* Python interpreter

**Notes:**
- I, MONOE, by default, use zig compiler for other platforms, and g++ compiler for macOS as I'm working on macOS. You can and may change these. I might change this build system in further versions!

---

## Installation & Building

Official builds are **precompiled**, so you usually **don’t need to compile manually**.

To build from source:

```bash
python yumengine.py [patch|minor|major]
```

---

## Downloads

[Download YumEngine](https://yumstudiohq.github.io/YumEngine/pages/download.html)

Source code and contributions are available on the GitHub repository.

---

## Studio

**Yum Studio** is the team behind YumEngine:

* **Studio:** `yum-official`
* **Branch:** `stable`

We are a small, experimental studio passionate about **cross-language runtime engines** and **developer-friendly scripting tools** inspired by modern game development workflows.

---

## Contributors

* **MONOE** — Lead Developer ([GitHub](https://github.com/wys-prog))
* **YumStudio** — Contributor ([GitHub](https://github.com/yumstudiohq))

---

## License

[MIT License](./LICENSE)
