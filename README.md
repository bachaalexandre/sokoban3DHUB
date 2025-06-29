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

### 2. Installer Raylib
Assurez-vous que Raylib est présent dans le dossier `raylib/` à la racine du projet. Si ce n'est pas le cas :

```bash
git clone https://github.com/raysan5/raylib.git
cd raylib
git checkout 4.5.0
cd ..
```

### 3. Compiler le projet
```bash
make
```

## 🎮 Exécution

### Lancer le jeu
```bash
./sokoban3d
```

## 🔧 Commandes Makefile

### Compilation
| Commande | Description |
|----------|-------------|
| `make` ou `make all` | Compile le projet (mode par défaut) |
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

## 🛠️ Développement

### Compilation optimisée
```bash
make release
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