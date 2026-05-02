#!/bin/bash

set -e

# Color definitions
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${BLUE}=======================================${NC}"
echo -e "${BLUE}       UELinker Installer v1.0         ${NC}"
echo -e "${BLUE}=======================================${NC}"
echo -e "${BLUE}  Linux Unreal Engine Desktop Linker   ${NC}"
echo -e "${BLUE}=======================================${NC}"

# ── Step 0: Auto-clone from GitHub if not already cloned ─────
if [ "$(basename "$PWD")" != "UELinker" ]; then
    echo -e "\n${YELLOW}[0/4] Cloning UELinker from GitHub...${NC}"
    if command -v git &> /dev/null; then
        git clone https://github.com/AliGalipALATLI/UELinker.git
        cd UELinker
        echo -e "${GREEN}    ✓ Cloned successfully. Restarting installer...${NC}"
        # Restart the script from the new directory
        exec ./install.sh
    else
        echo -e "${RED}[!] git not found. Please clone the repository manually and try again.${NC}"
        exit 1
    fi
fi

# ── Step 1: Dependencies ──────────────────────────────────────
echo -e "\n${YELLOW}[1/4] Checking and installing required dependencies...${NC}"
if command -v yay &> /dev/null; then
    echo -e "      Package manager: ${GREEN}yay${NC} (AUR)"
    yay -S --needed --noconfirm qt6-base cmake make gcc
elif command -v pacman &> /dev/null; then
    echo -e "      Package manager: ${GREEN}pacman${NC}"
    sudo pacman -S --needed --noconfirm qt6-base cmake make gcc
else
    echo -e "${RED}[!] Neither yay nor pacman found.${NC}"
    echo -e "    Please install the following packages manually: qt6-base cmake make gcc"
    exit 1
fi
echo -e "${GREEN}    ✓ Dependencies are ready.${NC}"

# ── Step 2: Icon ──────────────────────────────────────────────
echo -e "\n${YELLOW}[2/4] Copying UE icon to system...${NC}"
mkdir -p ~/.local/share/icons
if [ -f "assets/UE.png" ]; then
    cp assets/UE.png ~/.local/share/icons/UE.png
    echo -e "${GREEN}    ✓ UE.png copied to ~/.local/share/icons/${NC}"
else
    echo -e "${RED}[!] UE.png not found in current directory. Continuing without icon.${NC}"
fi

# ── Step 3: Build ─────────────────────────────────────────────
echo -e "\n${YELLOW}[3/4] Building UELinker with CMake...${NC}"
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_PROJECT_NAME=UELinker
cmake --build build -j$(nproc)
echo -e "${GREEN}    ✓ Build successful.${NC}"

# ── Step 4: Install ───────────────────────────────────────────
echo -e "\n${YELLOW}[4/4] Installing UELinker to your system...${NC}"
mkdir -p ~/.local/bin

# Support both possible binary names
if [ -f "build/UELinker" ]; then
    cp build/UELinker ~/.local/bin/UELinker
elif [ -f "build/UnrealLauncher" ]; then
    cp build/UnrealLauncher ~/.local/bin/UELinker
else
    echo -e "${RED}[!] Build binary not found. Build may have failed.${NC}"
    exit 1
fi
chmod +x ~/.local/bin/UELinker

# Create .desktop entry so UELinker appears in app menus
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

echo -e "${GREEN}    ✓ UELinker installed to ~/.local/bin/UELinker${NC}"
echo -e "${GREEN}    ✓ Desktop entry created at ~/.local/share/applications/UELinker.desktop${NC}"

# ── Done! ─────────────────────────────────────────────────────
echo -e "\n${GREEN}=======================================${NC}"
echo -e "${GREEN}  Installation Complete! 🎉             ${NC}"
echo -e "${GREEN}=======================================${NC}"
echo -e "  You can now find ${BLUE}UELinker${NC} in your application menu."
echo -e "  Run it, click ${YELLOW}Add Editor${NC}, and point it to your"
echo -e "  Unreal Engine root folder to create a desktop shortcut."
echo -e "${GREEN}=======================================${NC}\n"
