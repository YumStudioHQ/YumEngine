#!/usr/bin/env lua
---@diagnostic disable

------------------------------------------------------------
-- CONFIG
------------------------------------------------------------

local VERSION_FILE = "inc/version/engine_version.h"
local README_FILE  = "README.md"
local CONFIG_FILE  = "version_config.lua" -- must return {studio="X", branch="Y"}
local VERSION_PREFIX = "YUM_ENGINE"

------------------------------------------------------------
-- FILE SYSTEM HELPERS
------------------------------------------------------------

local function exists(path)
    local f = io.open(path, "r")
    if f then f:close() return true end
    return false
end

------------------------------------------------------------
-- LOAD CONFIG (studio + branch)
------------------------------------------------------------

local config = {}
if exists(CONFIG_FILE) then
    config = dofile(CONFIG_FILE)
end

local STUDIO = config.studio or "unknown-studio"
local BRANCH = config.branch or "unstable" -- PWAHAHAHAHAHHA IM SO FUNNY GUYSSSS

------------------------------------------------------------
-- VERSION FUNCTIONS
------------------------------------------------------------

local function load_version()
    local major, minor, patch = 0,0,0
    if not exists(VERSION_FILE) then return major, minor, patch end

    for line in io.lines(VERSION_FILE) do
        local M = line:match("#define%s+" .. VERSION_PREFIX .. "_VERSION_MAJOR%s+(%d+)")
        local m = line:match("#define%s+" .. VERSION_PREFIX .. "_VERSION_MINOR%s+(%d+)")
        local p = line:match("#define%s+" .. VERSION_PREFIX .. "_VERSION_PATCH%s+(%d+)")
        if M then major = tonumber(M) end
        if m then minor = tonumber(m) end
        if p then patch = tonumber(p) end
    end
    return major, minor, patch
end

local function write_version(major, minor, patch)
    local out = io.open(VERSION_FILE, "w")
    if not out then error("Cannot write version header") end
    out:write(string.format("#ifndef %s_INCLUDE_GUARD\n", VERSION_PREFIX))
    out:write(string.format("#define %s_INCLUDE_GUARD\n", VERSION_PREFIX))
    out:write(string.format("#define %s_VERSION_MAJOR %d\n", VERSION_PREFIX, major))
    out:write(string.format("#define %s_VERSION_MINOR %d\n", VERSION_PREFIX, minor))
    out:write(string.format("#define %s_VERSION_PATCH %d\n", VERSION_PREFIX, patch))
    out:write(string.format("#define %s_VERSION_INT ((%d*10000)+(%d*100)+(%d))\n",
                            VERSION_PREFIX, major, minor, patch))
    out:write(string.format("#define %s_VERSION_BRANCH \"%s\"\n", VERSION_PREFIX, BRANCH))
    out:write(string.format("#define %s_VERSION_STUDIO \"%s\"\n", VERSION_PREFIX, STUDIO))
    out:write("#endif")
    out:close()
end

local function parse_version(M, m, p, bump)
    if bump == "major" then
        M = M + 1; m=0; p=0
    elseif bump == "minor" then
        m = m +1; p=0
    else
        p = p +1
    end
    return M, m, p
end

------------------------------------------------------------
-- UPDATE README
------------------------------------------------------------

local function update_readme(M, m, p)
    if not exists(README_FILE) then return end

    local content = ""
    do
        local f = io.open(README_FILE, "r")
        content = f:read("*a")
        f:close()
    end

    local replacement = string.format([[
<!-- YUM_VERSION_START -->
- **Engine version:** `%d.%d.%d`
- **Studio:** `%s`
- **Branch:** `%s`
<!-- YUM_VERSION_END -->]], M, m, p, STUDIO, BRANCH)

    content = content:gsub("<!%-%- YUM_VERSION_START %-%->.-<!%-%- YUM_VERSION_END %-%->", replacement, 1)

    local f = io.open(README_FILE, "w")
    f:write(content)
    f:close()

    print("[README] Updated → " .. string.format("%d.%d.%d", M,m,p))
end

------------------------------------------------------------
-- MAIN
------------------------------------------------------------

local bump = arg[1] or "patch"
if bump ~= "major" and bump ~= "minor" and bump ~= "patch" then
    print("Invalid bump type. Use: major | minor | patch")
    os.exit(1)
end

local oldM, oldm, oldp = load_version()
local newM, newm, newp = parse_version(oldM, oldm, oldp, bump)

print(string.format("Version bump: %d.%d.%d → %d.%d.%d", oldM, oldm, oldp, newM, newm, newp))
print("Studio: " .. STUDIO .. ", Branch: " .. BRANCH)

write_version(newM, newm, newp)
update_readme(newM, newm, newp)

print("[VERSION] Bump complete!")
