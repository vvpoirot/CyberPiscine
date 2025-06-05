#!/bin/bash

# Variables utilisateur
USERNAME="user"
PASSWORD="pass"

# Crée l'utilisateur avec son home
useradd -m "$USERNAME"

# Définit le mot de passe
echo "$USERNAME:$PASSWORD" | chpasswd

# Crée les répertoires FTP
mkdir -p /home/$USERNAME/ftp/upload

# Applique les permissions recommandées
chown nobody:nogroup /home/$USERNAME/ftp
chmod a-w /home/$USERNAME/ftp
chown $USERNAME:$USERNAME /home/$USERNAME/ftp/upload

echo "[+] Utilisateur FTP '$USERNAME' créé avec succès."
