#!/usr/bin/env python3
import os, sys, time, subprocess, shutil

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

CLEAR  = "\033[2K"
UP     = "\033[A"

def bar(p, size=24):
    fill = "█" * int(p * size)
    empty = "░" * (size - int(p * size))
    return f"{fill}{empty}"

def multiline(lines):
    for l in lines:
        print(f"{CLEAR}{l}")
    print(UP * len(lines), end="", flush=True)

def header(title):
    print(f"\n{BOLD}{CYAN}==> {title}{RESET}")

def success(msg):
    print(f"{CLEAR}{GREEN}{msg}{RESET}")

def fail(msg):
    print(f"{CLEAR}{RED}{msg}{RESET}")

# ──────────────────────────────────────────────────────────────
# CONFIG
# ──────────────────────────────────────────────────────────────
FLAGS_CC  = "-std=c17 -Wall -Wextra -fPIC -O2 -c -I./ -I../ -I./inc/ -I./inc/lua"
FLAGS_CXX = "-std=c++23 -Wall -Wextra -fPIC -O2 -c -shared -I./ -I../ -I./inc/ -I./inc/lua"

ZIG_CC  = "zig cc"
ZIG_CXX = "zig c++"

OUTPUT_DIR = "bin"
TMP = "tmp"

LINK_FLAGS_MACOS   = "-dynamiclib"
LINK_FLAGS_WINDOWS = "-shared -static -static-libstdc++ -static-libgcc"
LINK_FLAGS_LINUX   = "-shared"

PLATFORMS = [
    # macOS universal
    {
        "name": "macOS (Universal)",
        "target": None,  # native macOS clang
        "cc":  "gcc",
        "cxx": "g++",
        "extra": "-arch x86_64 -arch arm64",
        "out": "libyum.dylib",
        "link": LINK_FLAGS_MACOS,
    },
    # Windows x64
    {
        "name": "Windows x64",
        "target": "x86_64-windows-gnu",
        "cc":  ZIG_CC,
        "cxx": ZIG_CXX,
        "extra": "",
        "out": "winyum64.dll",
        "link": LINK_FLAGS_WINDOWS,
    },
    # Windows x86
    {
        "name": "Windows x86",
        "target": "x86-windows-gnu",
        "cc":  ZIG_CC,
        "cxx": ZIG_CXX,
        "extra": "",
        "out": "winyum32.dll",
        "link": LINK_FLAGS_WINDOWS,
    },
    # Windows ARM64
    {
        "name": "Windows ARM64",
        "target": "aarch64-windows-gnu",
        "cc":  ZIG_CC,
        "cxx": ZIG_CXX,
        "extra": "",
        "out": "winyum_arm64.dll", 
        "link": LINK_FLAGS_WINDOWS,
    },
    # Linux x86_64
    {
        "name": "Linux x86_64",
        "target": "x86_64-linux-gnu",
        "cc":  ZIG_CC,
        "cxx": ZIG_CXX,
        "extra": "",
        "out": "libyum64.so",
        "link": LINK_FLAGS_LINUX,
    },
    # Linux x86
    {
        "name": "Linux x86",
        "target": "x86-linux-gnu",
        "cc":  ZIG_CC,
        "cxx": ZIG_CXX,
        "extra": "",
        "out": "libyum32.so",
        "link": LINK_FLAGS_LINUX,
    },
    # Linux ARM64
    {
        "name": "Linux ARM64",
        "target": "aarch64-linux-gnu",
        "cc":  ZIG_CC,
        "cxx": ZIG_CXX,
        "extra": "",
        "out": "libyum_arm64.so",
        "link": LINK_FLAGS_LINUX,
    },
]

# ──────────────────────────────────────────────────────────────
# UTILITIES
# ──────────────────────────────────────────────────────────────
def run(cmd):
    try:
        subprocess.check_call(cmd, shell=True)
        return True
    except:
        return False

def find_files(ext):
    out = []
    for root, dirs, files in os.walk("."):
        for f in files:
            if f.endswith(ext):
                out.append(os.path.join(root, f))
    return out

def compile_files(files, compiler, flags, target=None, extra=""):
    objs = []
    n = len(files)

    for i, src in enumerate(files):
        pct = (i+1)/n
        obj = os.path.join(TMP, os.path.basename(src) + ".o")
        objs.append(obj)

        # UI
        lines = [
            f"{BOLD}{CYAN}Compiling...{RESET}{' ' * 20}",
            f"[{bar(pct)}]  {int(pct*100)}%",
            f"{DIM}{src}{RESET}"
        ]
        multiline(lines)

        # actual command
        tgt = f"-target {target}" if target else ""
        cmd = f'{compiler} {flags} {tgt} {extra} "{src}" -o "{obj}"'

        if not run(cmd):
            fail(f"Failed compiling {src}")
            sys.exit(1)

        time.sleep(0.03)

    print("\n", end="")
    return objs

def link_binary(objlist:str, compiler:str, out:str, link:str, target: str|None = None, extra: str=""):
    header(f"Linking{' ' * 20}")

    tgt = f"-target {target}" if target else ""
    objs = " ".join(f'"{o}"' for o in objlist)
    cmd = f'{compiler} {link} {tgt} {extra} {objs} -o {OUTPUT_DIR}/{out}'

    if not run(cmd):
        fail("Link failed")
        sys.exit(1)

    success(f"Built → {GREEN}{out}{RESET}")

# ──────────────────────────────────────────────────────────────
# MAIN BUILD
# ──────────────────────────────────────────────────────────────
def main():
    # prepare
    shutil.rmtree(OUTPUT_DIR, ignore_errors=True)
    shutil.rmtree(TMP, ignore_errors=True)
    os.makedirs(OUTPUT_DIR)
    os.makedirs(TMP)

    cfiles = find_files(".c")
    cppfiles = find_files(".cpp")

    total = len(PLATFORMS)

    for i, plat in enumerate(PLATFORMS):
        p = (i+1)/total

        header(plat["name"])

        # platform progress bar
        lines = [
            f"{CYAN}{plat['name']}{RESET}",
            f"[{bar(p)}] {int(p*100)}%",
            f"{DIM}Setting up...{RESET}"
        ]
        
        multiline(lines)

        # compile C
        objs_c = compile_files(
            cfiles,
            plat["cc"],
            FLAGS_CC,
            plat["target"],
            plat["extra"]
        )

        # compile C++
        objs_cpp = compile_files(
            cppfiles,
            plat["cxx"],
            FLAGS_CXX,
            plat["target"],
            plat["extra"]
        )

        # link
        link_binary(objs_c + objs_cpp, plat["cxx"], plat["out"], plat["link"], plat["target"], plat["extra"])

        print("\n")

    success("ALL BUILDS COMPLETED SUCCESSFULLY.\n")

if __name__ == "__main__":
    run(f"lua ./bump-version.lua {" ".join(sys.argv[1:])}")
    run("doxygen Doxyfile")
    main()
