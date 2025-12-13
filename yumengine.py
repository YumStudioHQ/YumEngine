#!/usr/bin/env python3
import os
import sys
import time
import subprocess
import shutil
from typing import List, Dict, Optional
import zipfile

# ──────────────────────────────────────────────────────────────
# STYLISH ANSI UI
# ──────────────────────────────────────────────────────────────
RESET = "\033[0m"
BOLD  = "\033[1m"
DIM   = "\033[2m"
RED   = "\033[31m"
GREEN = "\033[32m"
YELLOW= "\033[33m"
CYAN  = "\033[36m"

CLEAR = "\033[2K"
UP    = "\033[A"

def bar(p: float, size: int = 24) -> str:
    fill = "█" * int(p * size)
    empty = "░" * (size - int(p * size))
    return f"{fill}{empty}"

def multiline(lines: List[str]) -> None:
    for l in lines:
        print(f"{CLEAR}{l}")
    print(UP * len(lines), end="", flush=True)

def header(title: str|None) -> None:
    print(f"\n{BOLD}{CYAN}==> {title}{RESET}")

def success(msg: str|None) -> None:
    print(f"{CLEAR}{GREEN}{msg}{RESET}")

def fail(msg: str|None) -> None:
    print(f"{CLEAR}{RED}{msg}{RESET}")


# ──────────────────────────────────────────────────────────────
# BUILD CONFIG
# ──────────────────────────────────────────────────────────────
FLAGS_CC_RELEASE  = "-std=c17 -Wall -Wextra -fPIC -O2 -c -I./ -I../ -I./inc/ -I./inc/lua"
FLAGS_CXX_RELEASE = "-std=c++23 -Wall -Wextra -fPIC -O2 -c -shared -I./ -I../ -I./inc/ -I./inc/lua"

FLAGS_CC_DEBUG  = "-std=c17 -Wall -Wextra -fPIC -g -O0 -c -I./ -I../ -I./inc/ -I./inc/lua -DDEBUG"
FLAGS_CXX_DEBUG = "-std=c++23 -Wall -Wextra -fPIC -g -O0 -c -shared -I./ -I../ -I./inc/ -I./inc/lua -DDEBUG"

ZIG_CC  = "zig cc"
ZIG_CXX = "zig c++"

OUTPUT_DIR_RELEASE = "bin/release"
OUTPUT_DIR_DEBUG   = "bin/debug"
TMP = "temp"

LINK_FLAGS_MACOS   = "-dynamiclib"
LINK_FLAGS_WINDOWS = "-shared -static -static-libstdc++ -static-libgcc"
LINK_FLAGS_LINUX   = "-shared"

Platform = Dict[str, Optional[str]]

PLATFORMS: List[Platform] = [
    { "name": "macOS (Universal)", "target": None, "cc": "gcc", "cxx": "g++",
      "extra": "-arch x86_64 -arch arm64", "out": "libyum_apple.dylib", "link": LINK_FLAGS_MACOS },

    { "name": "Windows x64",   "target": "x86_64-windows-gnu",  "cc": ZIG_CC, "cxx": ZIG_CXX,
      "extra": "", "out": "libyum_win_x64.dll", "link": LINK_FLAGS_WINDOWS },

    { "name": "Windows x86",   "target": "x86-windows-gnu",     "cc": ZIG_CC, "cxx": ZIG_CXX,
      "extra": "", "out": "libyum_win_x86.dll", "link": LINK_FLAGS_WINDOWS },

    { "name": "Windows ARM64", "target": "aarch64-windows-gnu", "cc": ZIG_CC, "cxx": ZIG_CXX,
      "extra": "", "out": "libyum_win_arm64.dll", "link": LINK_FLAGS_WINDOWS },

    { "name": "Linux x86_64",  "target": "x86_64-linux-gnu",    "cc": ZIG_CC, "cxx": ZIG_CXX,
      "extra": "", "out": "libyum_linux_x64.so", "link": LINK_FLAGS_LINUX },

    { "name": "Linux x86",     "target": "x86-linux-gnu",       "cc": ZIG_CC, "cxx": ZIG_CXX,
      "extra": "", "out": "libyum_linux_x86.so", "link": LINK_FLAGS_LINUX },

    { "name": "Linux ARM64",   "target": "aarch64-linux-gnu",   "cc": ZIG_CC, "cxx": ZIG_CXX,
      "extra": "", "out": "libyum_linux_arm64.so", "link": LINK_FLAGS_LINUX },
]


# ──────────────────────────────────────────────────────────────
# UTILITIES
# ──────────────────────────────────────────────────────────────

def run(cmd: str) -> bool:
    try:
        subprocess.check_call(cmd, shell=True)
        return True
    except subprocess.CalledProcessError:
        return False

def find_files(ext: str) -> List[str]:
    out: List[str] = []
    for root, _dirs, files in os.walk("."):
        for f in files:
            if f.endswith(ext) and not "docs/" in root: # exclude docs folder... This is dirty but should work lol.
                out.append(os.path.join(root, f))
    return out


def compile_files(files: List[str], compiler: str, flags: str, target: Optional[str] = None, extra: str = "") -> List[str]:
    objs: List[str] = []
    n = len(files)

    for i, src in enumerate(files):
        pct = (i + 1) / n
        obj = os.path.join(TMP, os.path.basename(src) + ".o")
        objs.append(obj)

        lines = [
            f"{BOLD}{CYAN}Compiling...{RESET}{' '*20}",
            f"[{bar(pct)}]  {int(pct*100)}%",
            f"{DIM}{src}{RESET}"
        ]
        multiline(lines)

        tgt = f"-target {target}" if target else ""
        cmd = f'{compiler} {flags} {tgt} {extra} "{src}" -o "{obj}"'

        if not run(cmd):
            fail(f"Failed compiling {src}")
            sys.exit(1)

        time.sleep(0.03)

    print("\n", end="")
    return objs


def link_binary(objlist: List[str], compiler: str, out: str, link: str, target: Optional[str], extra: str, output_dir: str):
    header(f"Linking{' '*20}")
    tgt = f"-target {target}" if target else ""
    objs = " ".join(f'"{o}"' for o in objlist)
    output_path = os.path.join(output_dir, out)

    cmd = f'{compiler} {link} {tgt} {extra} {objs} -o "{output_path}"'

    if not run(cmd):
        fail("Link failed")
        sys.exit(1)

    success(f"Built → {GREEN}{output_path}{RESET}")


# ──────────────────────────────────────────────────────────────
# BUILD ALL
# ──────────────────────────────────────────────────────────────

def build_all(flags_c: str, flags_cpp: str, output_dir: str):
    shutil.rmtree(output_dir, ignore_errors=True)
    shutil.rmtree(TMP, ignore_errors=True)
    os.makedirs(output_dir)
    os.makedirs(TMP)

    cfiles = find_files(".c")
    cppfiles = find_files(".cpp")

    total = len(PLATFORMS)

    for i, plat in enumerate(PLATFORMS):
        p = (i + 1) / total

        header(plat["name"])

        lines = [
            f"{CYAN}{plat['name']}{RESET}",
            f"[{bar(p)}] {int(p*100)}%",
            f"{DIM}Setting up...{RESET}"
        ]
        multiline(lines)

        objs_c = compile_files(cfiles,   plat["cc"],  flags_c,  plat["target"], plat["extra"])
        objs_cpp = compile_files(cppfiles, plat["cxx"], flags_cpp, plat["target"], plat["extra"])

        link_binary(
            objs_c + objs_cpp,
            plat["cxx"],
            plat["out"],
            plat["link"],
            plat["target"],
            plat["extra"],
            output_dir,
        )

        print("\n")

    success("ALL BUILDS COMPLETED SUCCESSFULLY.\n")


# ──────────────────────────────────────────────────────────────
# ZIP PACKAGING (from script #1, rewritten with UI)
# ──────────────────────────────────────────────────────────────

def zip_with_progress(zipname: str, filelist: List[str], rootdir: str):
    header(f"Packaging → {zipname}.zip")

    with zipfile.ZipFile(f"{zipname}.zip", "w", zipfile.ZIP_DEFLATED) as zipf:
        n = len(filelist)

        for i, f in enumerate(filelist):
            pct = (i + 1) / n
            rel = os.path.relpath(f, rootdir)

            lines = [
                f"{CYAN}Adding files...{RESET}",
                f"[{bar(pct)}] {int(pct*100)}%",
                f"{DIM}{rel}{RESET}",
            ]
            multiline(lines)

            zipf.write(f, rel)
            time.sleep(0.02)

    print("")
    success(f"Wrote {zipname}.zip")


def package_outputs(debug_dir: str, release_dir: str):
    header("PACKAGING LIBYUM")

    # DEBUG ZIP
    if os.path.isdir(debug_dir):
        files = [os.path.join(dp, f) for dp,_,fs in os.walk(debug_dir) for f in fs]
        zip_with_progress("bin/libyum_debug", files, debug_dir)

    # RELEASE ZIP
    if os.path.isdir(release_dir):
        files = [os.path.join(dp, f) for dp,_,fs in os.walk(release_dir) for f in fs]
        zip_with_progress("bin/libyum_release", files, release_dir)

    # PRODUCTION ZIP
    header("Packaging Production Bundle")
    prod_files = []

    for base in [debug_dir, release_dir]:
        if not os.path.isdir(base):
            continue
        for dp, _dirs, fs in os.walk(base):
            for f in fs:
                full = os.path.join(dp, f)
                rel = os.path.join(os.path.basename(base), os.path.relpath(full, base))
                prod_files.append((full, rel))

    zipname = "bin/libyum_production"
    header(f"Packaging → {zipname}.zip")

    with zipfile.ZipFile(f"{zipname}.zip", "w", zipfile.ZIP_DEFLATED) as zipf:
        n = len(prod_files)
        for i, (src, rel) in enumerate(prod_files):
            pct = (i + 1) / n
            lines = [
                f"{CYAN}Adding files...{RESET}",
                f"[{bar(pct)}] {int(pct*100)}%",
                f"{DIM}{rel}{RESET}",
            ]
            multiline(lines)

            zipf.write(src, rel)
            time.sleep(0.02)

    print("")
    success(f"Wrote {zipname}.zip")
    print("")


# ──────────────────────────────────────────────────────────────
# ENTRYPOINT
# ──────────────────────────────────────────────────────────────

def main():
    if "--release" in sys.argv:
        run(f"lua ./bump-version.lua {' '.join(sys.argv[1:])}")
        run("doxygen Doxyfile")

        header("BUILD: RELEASE")
        build_all(FLAGS_CC_RELEASE, FLAGS_CXX_RELEASE, OUTPUT_DIR_RELEASE)

        header("BUILD: DEBUG")
        build_all(FLAGS_CC_DEBUG, FLAGS_CXX_DEBUG, OUTPUT_DIR_DEBUG)

        package_outputs(OUTPUT_DIR_DEBUG, OUTPUT_DIR_RELEASE)

        success("DONE.\n")
    else:
        build_all(FLAGS_CC_DEBUG, FLAGS_CXX_DEBUG, OUTPUT_DIR_DEBUG)

if __name__ == "__main__":
    main()
