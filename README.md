# 🎮 Casse-Brique en C – Jeu Console

Ce jeu de **Casse-Brique** a été développé entièrement en **langage C**. Il utilise la bibliothèque **SDL2** pour l'interface graphique, et il doit être exécuté dans un environnement **Linux** pour une installation et une exécution faciles.

---

## 📁 Organisation des fichiers

Voici la structure des fichiers dans le projet :

- `main.c` : le fichier principal du jeu
- `game.c` : gestion de la logique du jeu
- `game.h` : déclaration des fonctions du jeu
- `function.c` : fonctions utilitaires
- `function.h` : déclarations des fonctions utilitaires
- `README.md` : documentation du projet
- `OFL.txt` : licence Open Font License (si nécessaire pour la police utilisée)
- `Police.ttf` : fichier de la police utilisée pour le jeu

---
## 🖥️ Cloner le projet

Pour cloner ce projet et y accéder directement depuis votre terminal, utilisez la commande suivante :

git clone https://github.com/SafiDial/jeu-casse-brique.git

## ▶️ Compilation du jeu

Pour compiler et exécuter le jeu sur un système Linux, suivez les étapes ci-dessous :

1. **Installer la bibliothèque SDL2** :
   Ouvrez un terminal et tapez les commandes suivantes pour installer les dépendances nécessaires :
   
   su -  # Devenir super utilisateur
   apt update  # Mettre à jour les packages
   apt upgrade  # Mettre à jour les packages existants
   apt install libsdl2-dev  # Installer la bibliothèque SDL2


2. **Compiler le jeu :**

   gcc -o main main.c function.c -lSDL2

3.**Exécuter le jeu :**

  ./main


## 🔚 REMERCIEMENTS 

Merci d'avoir consulté ce projet ! 🎮  
J'espère que vous apprécierez jouer à **Casse-Brique** et que vous trouverez ce projet intéressant.  
N'hésitez pas à me faire part de vos retours ou suggestions pour améliorer le jeu !

Si vous souhaitez contribuer ou poser des questions, vous pouvez me contacter via **Email : safi076711@gmail.com**.

---

Bonne partie, et à bientôt !🚀


