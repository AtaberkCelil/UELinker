# UELinker

**UELinker** is a lightweight Linux desktop application that lets you register multiple Unreal Engine installations as `.desktop` entries — without the Epic Games Launcher. Simply point it to your engine root folder, give it a name, and UELinker will create a proper application shortcut in `~/.local/share/applications/`, making the editor searchable from GNOME Activities, KDE Kickoff, Rofi, or any other app launcher.

![Platform](https://img.shields.io/badge/platform-Linux-lightgrey.svg)
![C++](https://img.shields.io/badge/language-C%2B%2B17-blue)

---

## Features

- **Minimal & Dark UI** — Clean black-grey Qt6 interface, no bloat.
- **Automatic Shortcut Creation** — Generates a `.desktop` entry from your Unreal Engine root directory.
- **App Menu Integration** — Works out of the box with GNOME, KDE, XFCE, Rofi, and other launchers.
- **UE Icon Support** — Every shortcut uses `UE.png` as its icon, automatically installed to `~/.local/share/icons/`.
- **Wayland & X11 Support** — Wayland-native with automatic XWayland fallback.
- **No Root Required** — Everything installs to user directories (`~/.local/`).

---

## Prerequisites

| Dependency | Version |
|---|---|
| Qt 6 (Widgets) | 6.x |
| CMake | 3.20+ |
| GCC / Clang | C++17 capable |
| make | any recent version |

---

## Installation

### Arch Linux / CachyOS / Manjaro (Recommended)

Use the provided `install.sh` script. It handles dependency installation, building, and system integration automatically:

```bash
git clone https://github.com/yourusername/UELinker.git
cd UELinker
chmod +x install.sh
./install.sh
```

**What the script does, step by step:**
1. Installs `qt6-base`, `cmake`, `make`, `gcc` via `yay` (AUR) or `pacman`.
2. Copies `UE.png` to `~/.local/share/icons/` for use as the application icon.
3. Configures and builds the project with CMake in Release mode.
4. Installs the compiled binary to `~/.local/bin/UELinker`.
5. Creates `~/.local/share/applications/UELinker.desktop` so it appears in your app menu.

After installation, search for **UELinker** in your application menu and launch it.

---

### Ubuntu / Debian / Linux Mint

Install dependencies using `apt`, then build manually:

```bash
sudo apt update
sudo apt install -y qt6-base-dev cmake make g++

git clone https://github.com/yourusername/UELinker.git
cd UELinker

# Copy icon
mkdir -p ~/.local/share/icons
# The repository ships icons under the assets/ directory; copy from there
cp assets/UE.png ~/.local/share/icons/UE.png

# Build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)

# Install binary
mkdir -p ~/.local/bin
cp build/UELinker ~/.local/bin/UELinker || cp build/UnrealLauncher ~/.local/bin/UELinker
chmod +x ~/.local/bin/UELinker

# Create desktop entry
mkdir -p ~/.local/share/applications
cat <<EOF > ~/.local/share/applications/UELinker.desktop
[Desktop Entry]
Type=Application
Name=UELinker
Comment=Register and launch Unreal Engine editors on Linux
Exec=$HOME/.local/bin/UELinker
Icon=$HOME/.local/share/icons/UE.png
Terminal=false
Categories=Development;Utility;
EOF
```

> **Note:** If `qt6-base-dev` is not available on older Ubuntu versions (< 22.04), try `qtbase5-dev` for Qt 5 and adjust `CMakeLists.txt` accordingly.

---

### Fedora / RHEL / CentOS Stream

```bash
sudo dnf install -y qt6-qtbase-devel cmake make gcc-c++

git clone https://github.com/yourusername/UELinker.git
cd UELinker

    mkdir -p ~/.local/share/icons
    cp assets/UE.png ~/.local/share/icons/UE.png

cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)

mkdir -p ~/.local/bin
cp build/UELinker ~/.local/bin/UELinker || cp build/UnrealLauncher ~/.local/bin/UELinker
chmod +x ~/.local/bin/UELinker

mkdir -p ~/.local/share/applications
cat <<EOF > ~/.local/share/applications/UELinker.desktop
[Desktop Entry]
Type=Application
Name=UELinker
Comment=Register and launch Unreal Engine editors on Linux
Exec=$HOME/.local/bin/UELinker
Icon=$HOME/.local/share/icons/UE.png
Terminal=false
Categories=Development;Utility;
EOF
```

---

### openSUSE Tumbleweed / Leap

```bash
sudo zypper install -y cmake make gcc-c++ libqt6-qtbase-devel

git clone https://github.com/yourusername/UELinker.git
cd UELinker

    mkdir -p ~/.local/share/icons
    cp assets/UE.png ~/.local/share/icons/UE.png

cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)

mkdir -p ~/.local/bin
cp build/UELinker ~/.local/bin/UELinker || cp build/UnrealLauncher ~/.local/bin/UELinker
chmod +x ~/.local/bin/UELinker

mkdir -p ~/.local/share/applications
cat <<EOF > ~/.local/share/applications/UELinker.desktop
[Desktop Entry]
Type=Application
Name=UELinker
Comment=Register and launch Unreal Engine editors on Linux
Exec=$HOME/.local/bin/UELinker
Icon=$HOME/.local/share/icons/UE.png
Terminal=false
Categories=Development;Utility;
EOF
```

---

### Making `~/.local/bin` available in PATH

If your terminal cannot find the `UELinker` command after installation, add `~/.local/bin` to your `PATH`:

```bash
# For Bash
echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.bashrc && source ~/.bashrc

# For Zsh
echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.zshrc && source ~/.zshrc
```

---

## Usage

1. Open your application menu and search for **UELinker**, then launch it.
2. Click the **Add Editor** button.
3. In the dialog, click **Browse** and select your Unreal Engine root directory  
   (e.g., `/home/user/Linux_Unreal_Engine_5.6`).
4. UELinker will verify that `Engine/Binaries/Linux/UnrealEditor` exists inside the folder.
5. Enter a short name for the engine version (e.g., `UE5.6`).
6. Click **Add** — the `.desktop` shortcut is created instantly.
7. Search for your engine name (e.g., `UE5.6`) in your application menu and launch it directly!

---

## How It Works

UELinker generates `.desktop` files in the following format:

```ini
[Desktop Entry]
Type=Application
Name=UE5.6
Exec=/path/to/Linux_Unreal_Engine_5.6/Engine/Binaries/Linux/UnrealEditor
Icon=/home/user/.local/share/icons/UE.png
Terminal=false
Categories=Development;
```

These files are stored in `~/.local/share/applications/` and are immediately picked up by any XDG-compliant desktop environment.

---

## Project Structure

```
UELinker/
├── CMakeLists.txt
├── install.sh                 ← Arch/Manjaro one-click installer
├── README.md
├── UE.png                     ← Unreal Engine icon
├── AGENTS.md
└── src/
    ├── main.cpp
    ├── MainWindow.h/.cpp
    ├── AddEditorDialog.h/.cpp
    ├── DesktopEntryWriter.h/.cpp
    └── EditorEntry.h
```

---

## Tech Stack

| Layer | Technology |
|---|---|
| Language | C++17 |
| GUI Framework | Qt 6 (Widgets) |
| Build System | CMake 3.20+ |
| Target Platform | Linux x86_64 |

---

## License

[![License: GPL v2](https://img.shields.io/badge/License-GPL_v2-blue.svg)](https://opensource.org/licenses/GPL-2.0)
