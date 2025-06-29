# Sokoban 3D

Un jeu de puzzle Sokoban moderne en 3D développé avec C++ et Raylib.

## 📋 Prérequis

### Dépendances système
- Compilateur C++ compatible (GCC, Clang)
- CMake et Git
- Bibliothèques système pour OpenGL

## 🚀 Installation

### 1. Cloner le projet
```bash
git clone <votre-repo>
cd sokoban3d
```

### 2. Raylib inclus
Raylib est déjà inclus dans le projet. Aucune installation supplémentaire n'est nécessaire.

### 3. Compiler le projet
```bash
make
```

## 🎮 Exécution

### Lancer le jeu
```bash
./sokoban3d
```

## 🔧 Gestion Git et Raylib

### Si vous voyez l'avertissement Git sur le repository imbriqué

Raylib étant inclus comme sous-dossier, Git peut afficher un avertissement. Vous avez plusieurs options :

**Option 1 - Utiliser un submodule Git (recommandé) :**
```bash
git rm --cached raylib
git commit -m "Remove raylib from direct tracking"

git submodule add https://github.com/raysan5/raylib.git raylib
git commit -m "Add raylib as submodule"
```

**Option 2 - Ignorer l'avertissement :**
```bash
git config advice.addEmbeddedRepo false
```

**Option 3 - Garder raylib comme dossier simple :**
```bash
rm -rf raylib/.git
git add raylib
git commit -m "Include raylib source directly"
```

## 🔧 Commandes Makefile
| Commande | Description |
|----------|-------------|
| `make` ou `make all` | Compile le projet (mode par défaut) |
| `make debug` | Compile en mode debug avec symbols (-g -DDEBUG) |
| `make release` | Compile en mode release optimisé (-O3 -DNDEBUG) |

### Nettoyage
| Commande | Description |
|----------|-------------|
| `make clean` | Supprime les fichiers de build |
| `make clean-all` | Supprime les fichiers de build + nettoie Raylib |
| `make re` | Nettoie et recompile (clean + all) |

### Utilitaires
| Commande | Description |
|----------|-------------|
| `make install-deps` | Installe les dépendances système (Ubuntu/Debian) |
| `make help` | Affiche l'aide des commandes disponibles |

## 📁 Structure du projet

```
sokoban3d/
├── src/                    # Code source C++
│   ├── core/              # Module Core (moteur principal)
│   ├── graphics/          # Module Graphics (rendu 3D)
│   ├── physics/           # Module Physics (logique jeu)
│   ├── entities/          # Module Entities (objets jeu)
│   ├── screens/           # Module Screens (interface)
│   ├── level/             # Module Level (données niveau)
│   └── main.cpp           # Point d'entrée
├── build/                 # Fichiers de compilation (généré)
├── raylib/                # Bibliothèque Raylib
├── assets/                # Ressources du jeu
│   ├── models/            # Modèles 3D
│   ├── textures/          # Textures
│   ├── sounds/            # Sons
│   └── levels/            # Fichiers JSON des niveaux
├── Makefile               # Fichier de build
└── README.md              # Ce fichier
```

## 🎯 Gameplay

### Contrôles
- **WASD** ou **Flèches** : Déplacement du joueur
- **R** : Redémarrer le niveau
- **ESC** : Menu pause
- **Souris** : Contrôle de la caméra 3D

### Objectif
Poussez toutes les caisses (📦) sur les emplacements cibles (🎯) pour terminer le niveau.



**Développé par Alexandre Bacha - EPITECH Strasbourg 2025**