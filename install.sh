#!/bin/bash

cd ~
git clone git@github.com:Alexandre-Roussel48/TUI_FileBrowser.git
cd TUI_FileBrowser

clear

printf "\e[31m  _______ _    _ _____ ______ _ _      ____                                  \n |__   __| |  | |_   _|  ____(_) |    |  _ \                                 \n    | |  | |  | | | | | |__   _| | ___| |_) |_ __ _____      _____  ___ _ __ \n    | |  | |  | | | | |  __| | | |/ _ \  _ <| '__/ _ \ \ /\ / / __|/ _ \ '__|\n    | |  | |__| |_| |_| |    | | |  __/ |_) | | | (_) \ V  V /\__ \  __/ |   \n    |_|   \____/|_____|_|    |_|_|\___|____/|_|  \___/ \_/\_/ |___/\___|_|   \n\n\e[0m"

echo "Enter the absolute path of the starting directory <eg:/home/user/work> :"
read ABSOLUTEPATH
PATHNAME=$(basename "$ABSOLUTEPATH")

sed -i "s|{{PATHNAME}}|${PATHNAME}|g" main.c.sample
sed -i "s|{{ABSOLUTEPATH}}|${ABSOLUTEPATH}|g" main.c.sample

mv main.c.sample main.c

gcc main.c -lpanel -lncurses -o main

echo "alias nav='. ~/TUI_FileBrowser/navigation'" >> ~/.bashrc
source ~/.bashrc

touch ~/.local/share/applications/TUI_FileBrowser.desktop

echo -e "[Desktop Entry]\nName=TUI_FileBrowser\nExec=/home/{{USER}}/TUI_FileBrowser/navigation\nType=Application\nIcon=/home/{{USER}}/TUI_FileBrowser/logo.png\nTerminal=true" >> ~/.local/share/applications/TUI_FileBrowser.desktop
sed -i "s|{{USER}}|$(whoami)|g" ~/.local/share/applications/TUI_FileBrowser.desktop

echo "Enjoy your browsing !\n\n"