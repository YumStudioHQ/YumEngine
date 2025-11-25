#!/usr/bin/env lua
---@diagnostic disable

------------------------------------------------------------
--  FILE SYSTEM HELPERS
------------------------------------------------------------

local fs = {}

function fs.join(a, b)
    if a:sub(-1) == "/" then return a .. b end
    return a .. "/" .. b
end

function fs.exists(path)
    local f = io.open(path, "r")
    if f then f:close() return true end
    return false
end

------------------------------------------------------------
--  RUN COMMAND
------------------------------------------------------------

function run(cmd)
    print("\27[96m[RUN]\27[0m " .. cmd)
    local f = io.popen(cmd .. " 2>&1")
    for line in f:lines() do print(line) end
    local ok,_,code = f:close()
    if code ~= 0 then error("Command failed: " .. cmd) end
end


------------------------------------------------------------
--  PROJECT ROOT (relative)
------------------------------------------------------------

local ROOT = "."                    -- build.lua location
local SRC  = fs.join(ROOT, "src")
local LUA  = fs.join(ROOT, "lua")
local OUT  = fs.join(ROOT, "src/build")

------------------------------------------------------------
--  VERSIONING CONFIG
------------------------------------------------------------

VERSION_FILE   = "src/inc/engine_version.h"
VERSION_PREFIX = "YUM_ENGINE"
VERSION_BRANCH = "stable"
VERSION_STUDIO = "yum-official"

WEBSITE_JS_FILE = "engine_version.js"
README_FILE     = "README.md"


------------------------------------------------------------
--  VERSIONING FUNCTIONS
------------------------------------------------------------

local function parse_version(major, minor, patch, bump)
    if bump == "major" then
        major = major + 1
        minor = 0
        patch = 0
    elseif bump == "minor" then
        minor = minor + 1
        patch = 0
    elseif bump == "patch" then
        patch = patch + 1
    end
    return major, minor, patch
end


function update_readme(major, minor, patch)
    if not fs.exists(README_FILE) then return end

    local version = string.format("%d.%d.%d", major, minor, patch)

    local f = io.open(README_FILE, "r")
    local content = f:read("*a")
    f:close()

    local replacement = string.format([[
<!-- YUM_VERSION_START -->
- **Engine version:** `%s`
- **Studio:** `%s`
- **Branch:** `%s`
<!-- YUM_VERSION_END -->]], version, VERSION_STUDIO, VERSION_BRANCH)

    content = content:gsub("<!%-%- YUM_VERSION_START %-%->.-<!%-%- YUM_VERSION_END %-%->",
                           replacement, 1)

    local out = io.open(README_FILE, "w+")
    out:write(content)
    out:close()

    print("[VERSION] README.md updated → " .. version)
end


function update_version_header(filename, prefix, bump)
    local major, minor, patch = 0, 0, 0
    local f = io.open(filename, "r")

    if f then
        for line in f:lines() do
            local M = line:match("#define%s+" .. prefix .. "_VERSION_MAJOR%s+(%d+)")
            if M then major = tonumber(M) end
            local N = line:match("#define%s+" .. prefix .. "_VERSION_MINOR%s+(%d+)")
            if N then minor = tonumber(N) end
            local P = line:match("#define%s+" .. prefix .. "_VERSION_PATCH%s+(%d+)")
            if P then patch = tonumber(P) end
        end
        f:close()
    end

    major, minor, patch = parse_version(major, minor, patch, bump)

    local header = {
        "#pragma once",
        string.format("#define %s_VERSION_MAJOR %d", prefix, major),
        string.format("#define %s_VERSION_MINOR %d", prefix, minor),
        string.format("#define %s_VERSION_PATCH %d", prefix, patch),
        string.format("#define %s_VERSION_INT ((%d*10000)+(%d*100)+(%d))",
                      prefix, major, minor, patch),
        string.format("#define %s_VERSION_BRANCH \"%s\"", prefix, VERSION_BRANCH),
        string.format("#define %s_VERSION_STUDIO \"%s\"", prefix, VERSION_STUDIO),
        "#define _YUM_DEF_VERSION(MAJ, MIN, PAT) (((MAJ)*10000)+((MIN)*100)+(PAT))"
    }

    local out = io.open(filename, "w")
    for _,l in ipairs(header) do out:write(l, "\n") end
    out:close()

    print(string.format("[VERSION] %s updated → %d.%d.%d",
          filename, major, minor, patch))

    return major, minor, patch
end


function generate_version_js(major, minor, patch)
    local full = string.format("%d.%d.%d", major, minor, patch)

    local content = string.format([[
const YUM_ENGINE_VERSION = {
  major: %d,
  minor: %d,
  patch: %d,
  branch: "%s",
  studio: "%s",
  full: "%s.%s.%s"
};
]], major, minor, patch,
        VERSION_BRANCH, VERSION_STUDIO,
        VERSION_STUDIO, VERSION_BRANCH, full)

    local f = io.open(WEBSITE_JS_FILE, "w")
    f:write(content)
    f:close()

    print("[VERSION] JS metadata created → " .. WEBSITE_JS_FILE)
end


------------------------------------------------------------
--  VERSION ENTRYPOINT
------------------------------------------------------------

local function run_version_bump(bump)
    bump = bump or "patch"

    print("\n\27[95m===== VERSION BUMP (" .. bump .. ") =====\27[0m")

    local major, minor, patch =
        update_version_header(VERSION_FILE, VERSION_PREFIX, bump)

    generate_version_js(major, minor, patch)
    update_readme(major, minor, patch)

    print("\27[92mVERSION COMPLETE → " ..
          major .. "." .. minor .. "." .. patch .. "\27[0m")
end


------------------------------------------------------------
--  GLOBAL COMPILER CONFIG
------------------------------------------------------------

local CC  = os.getenv("CC")  or "gcc"
local CXX = os.getenv("CXX") or "g++"

local internal_compilers = {
    windows32 = { cc="i686-w64-mingw32-gcc",  cxx="i686-w64-mingw32-g++" },
    windows64 = { cc="x86_64-w64-mingw32-gcc", cxx="x86_64-w64-mingw32-g++" },
    linux64   = { cc="./src/build/linux64.sh", cxx="echo 'already done'" },
    linux32   = { cc="./src/build/linux32.sh", cxx="echo 'already done'" },
    macos     = { cc="gcc",       cxx="g++" },
}

------------------------------------------------------------
--  PLATFORM DEFINITIONS
------------------------------------------------------------

local platforms = {

    windows32 = {
        cc=CC, cxx=CXX,
        out=fs.join(OUT, "yum32.dll"),
        cflags="-std=c17 -O2 -fPIC -c",
        ldflags="-std=c++23 -O2 -fPIC -m32 -static -static-libstdc++ -static-libgcc -shared",
    },

    windows64 = {
        cc=CC, cxx=CXX,
        out=fs.join(OUT, "yum64.dll"),
        cflags="-std=c17 -O2 -fPIC -c",
        ldflags="-std=c++23 -O2 -fPIC -static -static-libstdc++ -static-libgcc -shared",
    },

    macos = {
        cc=CC, cxx=CXX,
        out=fs.join(OUT, "libyum.dylib"),
        cflags="-std=c17 -O2 -fPIC -c -arch x86_64 -arch arm64",
        ldflags="-std=c++23 -O2 -fPIC -dynamiclib -arch x86_64 -arch arm64",
    },

    linux64 = {
        cc=CC, cxx=CXX,
        out=fs.join(OUT, "libyum64.so"),
        cflags="-std=c17 -O2 -fPIC -c",
        ldflags="-std=c++23 -O2 -fPIC -shared",
    },

    linux32 = {
        cc=CC, cxx=CXX,
        out=fs.join(OUT, "libyum32.so"),
        cflags="-std=c17 -O2 -fPIC -c -m32",
        ldflags="-std=c++23 -O2 -fPIC -shared -m32",
    },
}

------------------------------------------------------------
--  BUILD STEPS
------------------------------------------------------------

local function compile_lua(cfg)
    print("\n\27[92mCompiling Lua...\27[0m")
    run(cfg.cc .. " " .. cfg.cflags .. " " .. LUA .. "/*.c")
end

local function compile_cpp(cfg)
    print("\n\27[92mCompiling YumEngine...\27[0m")
    run(cfg.cxx .. " " .. SRC .. "/*.cpp *.o -I" .. LUA ..
        "/ " .. cfg.ldflags .. " -o " .. cfg.out)
end

local function cleanup()
    print("\27[93mCleaning objects...\27[0m")
    run("rm -f *.o 2>/dev/null || true")
end

------------------------------------------------------------
--  ARGUMENT PARSER
------------------------------------------------------------

local target = nil
local use_internal = false
local build_all = false
local bump = nil
local do_version = false

for i=1,#arg do
    local a = arg[i]

    if a == "-all" then build_all = true
    elseif a == "-ic" then use_internal = true
    elseif a == "-bump" then bump = arg[i+1]
    elseif a == "version" then do_version = true; bump = arg[i+1]
    elseif not target then target = a end
end


------------------------------------------------------------
--  VERSION-ONLY MODE
------------------------------------------------------------

if do_version then
    run_version_bump(bump)
    os.exit(0)
end

------------------------------------------------------------
--  BUILD TARGET
------------------------------------------------------------

local function build_target(target, use_internal)
    local cfg = platforms[target]
    if not cfg then error("Unknown platform: " .. tostring(target)) end

    if use_internal then
        print("\27[95mUsing INTERNAL COMPILERS ("..target..")\27[0m")
        cfg.cc  = internal_compilers[target].cc
        cfg.cxx = internal_compilers[target].cxx
    end

    print("\n======== BUILD " .. target .. " ========\n")
    print("CC  = " .. cfg.cc)
    print("CXX = " .. cfg.cxx)
    print("OUT = " .. cfg.out .. "\n")

    compile_lua(cfg)
    compile_cpp(cfg)
    cleanup()
end

------------------------------------------------------------
--  MAIN: BUILD ALL / ONE
------------------------------------------------------------

if bump then
    run_version_bump(bump)
end

if build_all then
    print("\n\27[94mBuilding ALL PLATFORMS...\27[0m")
    for name,_ in pairs(platforms) do
        build_target(name, use_internal)
    end
    print("\n\27[92mALL BUILDS COMPLETED!\27[0m")
    os.exit(0)
end

if not target or not platforms[target] then
    print("Usage:")
    print("  lua build.lua <platform>")
    print("  lua build.lua version <major|minor|patch>")
    print("  lua build.lua <platform> -bump <major|minor|patch>")
    print("  lua build.lua -all [-bump X] [-ic]")
    print("\nPlatforms:")
    for k,_ in pairs(platforms) do print("  - " .. k) end
    os.exit(1)
end

build_target(target, use_internal)

print("\n\27[92mDONE!\27[0m\n")
