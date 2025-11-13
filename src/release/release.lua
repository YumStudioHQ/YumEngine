-- =========================
-- CONFIGURATION (edit me!)
-- DON'T FORGET THE '/' OR '\' AT THE END OF PATHS
-- =========================
CXX            = "g++-15"
CC             = "gcc-15"
SRC_PATH       = "../"
LUA_SRC        = "../../lua/"
YUM_SRC        = "../YumEngine.cpp" -- Path to YumEngine!
OUT            = "yum"
YUM_OUT        = "YumEngine" -- You can add extention, but compilers do it automatically
LUA_FLAGS      = { "-O2", }
YUM_FLAGS      = { "-O2", "-Wall", "-Wextra", "-std=c++23", "-D_YUM_STUDIO_OFFICIAL_COMPILER", "-fdiagnostics-color=always" }
ENG_FLAGS      = YUM_FLAGS -- Same flags for Engine and Yum — but you can still change them!
LINK_FLAGS     = { "-shared", "-fPIC", "-static-libstdc++", "-fdiagnostics-color=always" }
VERSION_FILE   = "../inc/engine_version.h"
VERSION_PREFIX = "YUM_ENGINE"
VERSION_BRANCH = "stable"   -- your branch name
VERSION_STUDIO = "yum-official" -- your studio name

WEBSITE_JS_FILE = "engine_version.js"
README_FILE     = "/Users/wys/Documents/Yum Studio/Yum Engine/README.md"

-- =========================
-- HELPERS
-- =========================


function join(tbl, sep)
  local s = ""
  for i, v in ipairs(tbl) do
    if i > 1 then s = s .. sep end
    s = s .. v
  end
  return s
end

-- Runs a command and prints output live
function run(cmd)
  print("[RUN] " .. cmd)
  local f = io.popen(cmd .. " 2>&1") -- redirect stderr to stdout
  for line in f:lines() do
    print(line)
  end
  local ok, _, code = f:close()
  if code ~= 0 then error("Command failed: " .. cmd) end
end

-- =========================
-- VERSIONING
-- =========================
local function parse_version(major, minor, patch, bump)
  if bump == "major" then
    major = major + 1; minor, patch = 0, 0
  elseif bump == "minor" then
    minor = minor + 1; patch = 0
  elseif bump == "patch" then
    patch = patch + 1
  end
  return major, minor, patch
end

function update_readme(major, minor, patch)
  local version = string.format("%d.%d.%d", major, minor, patch)

  local f = io.open(README_FILE, "r")
  if not f then return end
  local content = f:read("*a")
  f:close()

  local replacement = string.format([[
<!-- YUM_VERSION_START -->
- **Engine version:** `%s`
- **Studio:** `%s`
- **Branch:** `%s`
<!-- YUM_VERSION_END -->]], version, VERSION_STUDIO, VERSION_BRANCH)

  content = content:gsub("<!%-%- YUM_VERSION_START %-%->.-<!%-%- YUM_VERSION_END %-%->", replacement, 1)

  local out = io.open(README_FILE, "w+")
  out:write(content)
  out:close()

  print("[VERSION] " .. README_FILE .. " updated with " .. version)
end


function update_version_header(filename, prefix, bump)
  local major, minor, patch = 0, 0, 0
  local lines = {}

  local f = io.open(filename, "r")
  if f then
    for line in f:lines() do
      table.insert(lines, line)
      local m = line:match("#define%s+"..prefix.."_VERSION_MAJOR%s+(%d+)")
      if m then major = tonumber(m) end
      local n = line:match("#define%s+"..prefix.."_VERSION_MINOR%s+(%d+)")
      if n then minor = tonumber(n) end
      local p = line:match("#define%s+"..prefix.."_VERSION_PATCH%s+(%d+)")
      if p then patch = tonumber(p) end
    end
    f:close()
  end

  major, minor, patch = parse_version(major, minor, patch, bump)

  local content = {
    "#pragma once",
    string.format("#define %s_VERSION_MAJOR %d", prefix, major),
    string.format("#define %s_VERSION_MINOR %d", prefix, minor),
    string.format("#define %s_VERSION_PATCH %d", prefix, patch),
    string.format("#define %s_VERSION_INT ((%d*10000)+(%d*100)+(%d))", prefix, major, minor, patch),
    string.format("#define %s_VERSION_BRANCH \"%s\"", prefix, VERSION_BRANCH),
    string.format("#define %s_VERSION_STUDIO \"%s\"", prefix, VERSION_STUDIO),
    "#define _YUM_DEF_VERSION(MAJ, MIN, PAT) (((MAJ) * 10000) + ((MIN) * 100) + (PAT))"
  }

  local out = io.open(filename, "w")
  for _, l in ipairs(content) do out:write(l, "\n") end
  out:close()

  print(string.format("[VERSION] %s updated to %d.%d.%d (%s/%s)", filename, major, minor, patch, VERSION_BRANCH, VERSION_STUDIO))

  return major, minor, patch
end

function generate_version_js(major, minor, patch)
  local full_version = string.format("%d.%d.%d", major, minor, patch)
  local content = string.format([[
const YUM_ENGINE_VERSION = {
  major: %d,
  minor: %d,
  patch: %d,
  branch: "%s",
  studio: "%s",
  full: "%s.%s.%s"
};
]], major, minor, patch, VERSION_BRANCH, VERSION_STUDIO, VERSION_STUDIO, VERSION_BRANCH, full_version)

  local f = io.open(WEBSITE_JS_FILE, "w")
  f:write(content)
  f:close()
  print("[VERSION] " .. WEBSITE_JS_FILE .. " generated for website!")
end

-- =========================
-- BUILD SYSTEM
-- =========================
function build_project(major, minor, patch)
  --print("> Building Lua")
  --run(CC .. " -c " .. LUA_SRC .. "*.c " .. table.concat(LUA_FLAGS, " "))

  --print("> Compiling YumEngineAPI sources")
  --run(CXX .. " -c " .. SRC_PATH .. "*.cpp -I" .. LUA_SRC .. " " .. table.concat(YUM_FLAGS, " "))

  --print("> Linking")
  --run(CXX .. " -o " .. OUT .. " *.o " .. table.concat(LINK_FLAGS, " "))

  --print("> Linking Engine.")
  --run(CXX .. " -o " .. YUM_OUT .. " *.o " .. table.concat(ENG_FLAGS))

  --print(string.format("Compiled: %s.%s.%s.%d.%d.%d",
  --  VERSION_STUDIO, VERSION_BRANCH, OUT, major, minor, patch))
  run("/Users/wys/Documents/Yum\\ Studio/Yum\\ Engine/src/build/doc.sh")
  run("/Users/wys/Documents/Yum\\ Studio/Yum\\ Engine/src/build/macos/mac.sh")
  run("/Users/wys/Documents/Yum\\ Studio/Yum\\ Engine/src/build/windows/win.sh")
  run("/Users/wys/Documents/Yum\\ Studio/Yum\\ Engine/src/build/windows32/win32.sh")
  run("/Users/wys/Documents/Yum\\ Studio/Yum\\ Engine/src/build/linux/lin.sh")
end

print("> Building project...")

-- Handle command-line args safely
local bump = "patch"
if type(arg) == "table" and arg[1] then
  bump = arg[1]
else
  print("[WARN] No bump argument provided, defaulting to 'patch'")
end

local major, minor, patch = update_version_header(VERSION_FILE, VERSION_PREFIX, bump)

generate_version_js(major, minor, patch)
update_readme(major, minor, patch)
build_project(major, minor, patch)

run("git add '/Users/wys/Documents/Yum Studio/Yum Engine/'")
local commit = io.read("l")
print("> Please, enter a commit message:")
run("git commit -m '" .. commit .. "'")
print("> Commited !")
print("> Pushing...")
run("git push")
print("> Done")