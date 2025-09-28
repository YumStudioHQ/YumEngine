import requests
import os

OWNER = "YumStudioHQ"
REPO = "YumEngine"

API_URL = f"https://api.github.com/repos/{OWNER}/{REPO}/releases/latest"

OUTPUT_DIR = "libraries"
os.makedirs(OUTPUT_DIR, exist_ok=True)

class Ansi:
    RESET = "\033[0m"
    BOLD = "\033[1m"
    CYAN = "\033[36m"
    GREEN = "\033[32m"
    YELLOW = "\033[33m"
    RED = "\033[31m"

def is_binary(asset_name: str):
  """Decide if an asset looks like a binary (not source archives)."""
  bad_exts = (".zip", ".tar.gz", ".tar", ".tgz")
  return not asset_name.lower().endswith(bad_exts)

def download_asset(asset: dict[str, str]):
  """Download a single asset."""
  url = asset["browser_download_url"]
  name = asset["name"]
  path = os.path.join(OUTPUT_DIR, name)

  print(f"Downloading {Ansi.CYAN}{name}{Ansi.RESET} ...")
  r = requests.get(url, stream=True)
  r.raise_for_status()
  with open(path, "wb") as f:
    for chunk in r.iter_content(chunk_size=4096):
      f.write(chunk)
  print(f"Saved to {Ansi.GREEN}{path}{Ansi.RESET}")

def main():
  print("Fetching latest release...")
  r = requests.get(API_URL)
  r.raise_for_status()
  release = r.json()

  print(f"Latest release: {Ansi.CYAN}{release['tag_name']} - {release['name']}{Ansi.RESET}")

  assets = release.get("assets", [])
  binaries = [a for a in assets if is_binary(a["name"])]

  if not binaries:
    print("No binaries found in this release.")
    return

  for asset in binaries:
    download_asset(asset)

  print("All binaries downloaded.")

if __name__ == "__main__":
  main()
