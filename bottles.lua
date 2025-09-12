local function split(str, sep)
  local t = {}
  for s in string.gmatch(str, "([^" .. sep .. "]+)") do
    table.insert(t, s)
  end
  return t
end

local function write_file(path, data)
  local f = assert(io.open(path, "w"))
  f:write(data)
  f:close()
end

local function mkdir_p(path)
  os.execute(string.format("mkdir -p %q", path))
end

local function copy_file(src, dst)
  mkdir_p(dst:match("(.+)/[^/]+$") or ".")
  os.execute(string.format("cp -r %q %q", src, dst))
end

local function split_version(ver)
  local major, minor, patch = ver:match("(%d+)%.(%d+)%.(%d+)")
  return tonumber(major) or 0, tonumber(minor) or 0, tonumber(patch) or 0
end


local function bump_version(ver, level)
  local parts = split(ver, ".")
  local major, minor, patch = tonumber(parts[1]) or 0, tonumber(parts[2]) or 0, tonumber(parts[3]) or 0

  if level == "major" then
    major = major + 1
    minor = 0
    patch = 0
  elseif level == "minor" then
    minor = minor + 1
    patch = 0
  else
    patch = patch + 1
  end

  return string.format("%d.%d.%d", major, minor, patch)
end


local function load_settings(path)
  local f, err = loadfile(path)
  if not f then return nil, err end
  return f()
end

local args = { ... }
local cmd = args[1]

if not cmd then
  print("Usage: lua bottle.lua -new ... | -run name | -update name")
  os.exit(1)
end

local bump_level = "patch"

if cmd == "-new" then
  local bottle_name = nil
  local cps = {}
  local bcs = {}
  local branch = "main"
  local studio = "default"
  local version = "0.0.0"

  local i = 2
  while i <= #args do
    local a = args[i]
    if a == "-cp" then
      table.insert(cps, { args[i + 1], args[i + 2] })
      i = i + 2
    elseif a == "-bc" then
      table.insert(bcs, args[i + 1])
      i = i + 1
    elseif a == "-branch" then
      branch = args[i + 1]
      i = i + 1
    elseif a == "-studio" then
      studio = args[i + 1]
      i = i + 1
    else
      bottle_name = a
    end
    i = i + 1
  end

  if not bottle_name then
    print("Bottle name required at end of -new")
    os.exit(1)
  end

  mkdir_p(bottle_name .. "/local")

  local settings = {
    branch = branch,
    studio = studio,
    version = version,
    copies = cps,
    builds = bcs
  }
  local sdata = "return " .. string.format("{\n branch=%q,\n studio=%q,\n version=%q,\n copies=%s,\n builds=%s \n}",
    settings.branch, settings.studio, settings.version,
    "{" ..
    table.concat(
      (function()
        local t = {}
        for _, v in ipairs(cps) do table.insert(t, string.format("{%q,%q}", v[1], v[2])) end
        return t
      end)(), ",") .. "}",
    "{" ..
    table.concat(
      (function()
        local t = {}
        for _, v in ipairs(bcs) do table.insert(t, string.format("%q", v)) end
        return t
      end)(), ",") .. "}")
  write_file(bottle_name .. "/settings.lua", sdata)

  for _, cp in ipairs(cps) do
    copy_file(cp[1], bottle_name .. "/" .. cp[2])
  end

  print("Bottle " .. bottle_name .. " created.")
elseif cmd == "-run" then
  local bottle_name = args[2]
  if not bottle_name then
    print("Bottle name required")
    os.exit(1)
  end
  local settings = load_settings(bottle_name .. "/settings.lua")
  if not settings then
    print("No settings found for " .. bottle_name)
    os.exit(1)
  end

  settings.version = bump_version(settings.version)
  local major, minor, patch = split_version(settings.version)

  local sdata = "return { branch=" ..
      string.format("%q", settings.branch) ..
      ", studio=" ..
      string.format("%q", settings.studio) ..
      ", version=" ..
      string.format("%q", settings.version) ..
      ", copies={" ..
      table.concat(
        (function()
          local t = {}
          for _, v in ipairs(settings.copies) do table.insert(t, string.format("{%q,%q}", v[1], v[2])) end
          return t
        end)(), ",") ..
      "}, builds={" ..
      table.concat(
        (function()
          local t = {}
          for _, v in ipairs(settings.builds) do table.insert(t, string.format("%q", v)) end
          return t
        end)(), ",") .. "} }"
  write_file(bottle_name .. "/settings.lua", sdata)

  local bottle_path = bottle_name

  for _, cmd in ipairs(settings.builds) do
    local replaced = cmd
    replaced = replaced:gsub("%$YUM_BOTTLE_BRANCH", settings.branch)
    replaced = replaced:gsub("%$YUM_BOTTLE_STUDIO", settings.studio)
    replaced = replaced:gsub("%$YUM_BOTTLE_VERSION", settings.version)

    replaced = replaced:gsub("%$YUM_BOTTLE_MAJOR", tostring(major))
    replaced = replaced:gsub("%$YUM_BOTTLE_MINOR", tostring(minor))
    replaced = replaced:gsub("%$YUM_BOTTLE_PATCH", tostring(patch))

    -- prefix 'local/' paths automatically --
    replaced = replaced:gsub("%$YUM_BOTTLE_PATH", bottle_path)

    print("[BUILD] executing: " .. replaced)
    os.execute(replaced)
  end
elseif cmd == "-update" then
  local bottle_name = args[2]
  local settings = load_settings(bottle_name .. "/settings.lua")
  if not settings then
    print("No settings found")
    os.exit(1)
  end
  for _, cp in ipairs(settings.copies) do
    copy_file(cp[1], bottle_name .. "/" .. cp[2])
    print(cp[1], bottle_name .. "/" .. cp[2])
  end
  print("Bottle " .. bottle_name .. " updated.")
  if args[3] == "-major" then
    bump_level = "major"
  elseif args[3] == "-minor" then
    bump_level = "minor"
  end

  settings.version = bump_version(settings.version, bump_level)
else
  print("Unknown command: " .. cmd)
end
