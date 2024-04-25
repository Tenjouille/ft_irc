#!/bin/bash

# Vérifie si un argument a été passé
if [ -z "$1" ]; then
    echo "Usage: $0 <commande_irssi>"
    exit 1
fi

# Fonction pour envoyer une commande à irssi et garder le terminal ouvert
send_to_irssi() {
    gnome-terminal --geometry=80x24+$1 -- irssi &
    sleep 1
    xdotool type "$2"
    xdotool key Return
}

# Ouvrir un terminal GNOME côte à côte en haut de l'écran, chacun avec irssi et la commande spécifiée
send_to_irssi "0+0" "$1"
send_to_irssi "700+0" "$1"
sleep 2  # Attendre un court délai pour s'assurer que le deuxième terminal est ouvert
send_to_irssi "1400+0" "$1"
