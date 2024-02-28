#!/bin/bash

cd ~
if [[ -d TUI_FileBrowser ]]; then
	cd TUI_FileBrowser
	git pull
else
	git clone https://github.com/Alexandre-Roussel48/TUI_FileBrowser.git
	cd TUI_FileBrowser
fi

clear

printf "\e[31m  _______ _    _ _____ ______ _ _      ____                                  \n |__   __| |  | |_   _|  ____(_) |    |  _ \                                 \n    | |  | |  | | | | | |__   _| | ___| |_) |_ __ _____      _____  ___ _ __ \n    | |  | |  | | | | |  __| | | |/ _ \  _ <| '__/ _ \ \ /\ / / __|/ _ \ '__|\n    | |  | |__| |_| |_| |    | | |  __/ |_) | | | (_) \ V  V /\__ \  __/ |   \n    |_|   \____/|_____|_|    |_|_|\___|____/|_|  \___/ \_/\_/ |___/\___|_|   \n\n\e[0m"
echo "Enter the absolute path of the starting directory <eg:/home/user/work> :"
read ABSOLUTEPATH
PATHNAME=$(basename "$ABSOLUTEPATH")

cd code
sed -i "s|{{PATHNAME}}|${PATHNAME}|g" main.c.sample
sed -i "s|{{ABSOLUTEPATH}}|${ABSOLUTEPATH}|g" main.c.sample
cp main.c.sample main.c
gcc main.c -lpanel -lncurses -o main

cd ~
nav_alias="alias nav='. ~/TUI_FileBrowser/script/navigation'"
if grep -q "^alias nav" .bashrc; then
    sed -i "s|^alias nav.*|$nav_alias|" .bashrc
else
    echo "$nav_alias" >> .bashrc
fi
source .bashrc

touch .local/share/applications/TUI_FileBrowser.desktop

echo -e "[Desktop Entry]\nName=TUI_FileBrowser\nExec=gnome-terminal -e \"bash -c '. /home/{{USER}}/TUI_FileBrowser/script/navigation; exec bash'\"\nType=Application\nIcon=/home/{{USER}}/TUI_FileBrowser/ressources/logo.png\nTerminal=false" > ~/.local/share/applications/TUI_FileBrowser.desktop
sed -i "s|{{USER}}|$(whoami)|g" .local/share/applications/TUI_FileBrowser.desktop

echo -e "Enjoy your browsing !\n\n"
