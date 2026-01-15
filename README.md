# 🚢 Bataille Navale SDL3

Jeu de bataille navale complet en C avec interface graphique SDL3 et intelligence artificielle évoluée.

![Version](https://img.shields.io/badge/version-1.0-blue.svg)
![Language](https://img.shields.io/badge/language-C-brightgreen.svg)
![SDL3](https://img.shields.io/badge/SDL3-required-red.svg)
![License](https://img.shields.io/badge/license-MIT-yellow.svg)

## 📋 Description

Implémentation complète du jeu classique de bataille navale avec :
- **Interface graphique réaliste** avec effets visuels avancés (vagues, explosions, fumée)
- **3 niveaux d'IA** avec stratégies différentes (facile, moyen, difficile)
- **Mode PvP** (Joueur contre Joueur)
- **Système de scores** persistant avec classements par difficulté
- **Placement manuel ou automatique** des bateaux

## ✨ Fonctionnalités

### Modes de jeu
- 🎮 **Joueur vs Joueur** (mode local tour par tour)
- 🤖 **Joueur vs IA Facile** (tirs aléatoires)
- 🧠 **Joueur vs IA Moyen** (ciblage après touche)
- 🎯 **Joueur vs IA Difficile** (recherche directionnelle optimisée)

### Graphismes
- Bateaux ultra-détaillés avec ancres, hublots, canons, radars
- Animations fluides (vagues, bulles, feu, fumée)
- Effets visuels réalistes pour les impacts et naufrages
- Interface intuitive avec aperçu des bateaux

### Système de points
- **+10 points** : Toucher un bateau
- **+20 points** : Couler un bateau
- **-1 point** : Tir manqué
- **+100 points** : Victoire
- **-50 points** : Défaite

### Types de bateaux
| Bateau | Dimensions | Quantité |
|--------|-----------|----------|
| Petit  | 3x1      | 1        |
| Moyen  | 4x1      | 2        |
| Grand  | 5x1      | 1        |
| Énorme | 6x1      | 1        |
| Spécial| 5x2      | 1        |

**Total : 6 bateaux par joueur**

## 🛠️ Technologies

- **Langage** : C (norme C11)
- **Bibliothèque graphique** : SDL3
- **Architecture** : Modulaire avec séparation des responsabilités
- **IA** : Algorithmes de recherche adaptatifs

## 📦 Prérequis

### Logiciels nécessaires
- Compilateur C (GCC 7.0+, Clang 5.0+, ou MSVC 2019+)
- SDL3 (version 3.0.0 ou supérieure)
- Make (optionnel mais recommandé)

### Installation de SDL3

#### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install libsdl3-dev
```

#### macOS (Homebrew)
```bash
brew install sdl3
```

#### Windows (MinGW)
1. Téléchargez SDL3 depuis [libsdl.org](https://www.libsdl.org/)
2. Extrayez dans `C:\SDL3`
3. Configurez les variables d'environnement :
   - `SDL3_INCLUDE` : `C:\SDL3\include`
   - `SDL3_LIB` : `C:\SDL3\lib`

## 🚀 Installation et compilation

### Cloner le repository
```bash
git clone https://github.com/VOTRE-USERNAME/bataille-navale.git
cd bataille-navale
```

### Compilation

#### Linux / macOS (avec Makefile)
```bash
make
./bataille_navale
```

#### Compilation manuelle
```bash
# Linux / macOS
gcc src/*.c -o bataille_navale -I./include -lSDL3 -lm

# Windows (MinGW)
gcc src/*.c -o bataille_navale.exe -I./include -IC:\SDL3\include -LC:\SDL3\lib -lmingw32 -lSDL3main -lSDL3
```

## 🎮 Comment jouer

### Démarrage
1. Lancez l'exécutable `./bataille_navale`
2. Choisissez le mode de jeu (1-4)
3. Entrez votre/vos nom(s)
4. Choisissez le placement (automatique ou manuel)

### Placement manuel
- **Clic gauche** : Placer le bateau
- **Touche V** : Changer l'orientation (vertical/horizontal)
- Un aperçu vert/rouge indique si le placement est valide

### Jeu
- **Cliquez** sur la grille de droite (grille adverse) pour tirer
- **Grille de gauche** : Vos propres bateaux
- **Grille de droite** : Grille adverse (bateaux cachés)

### Légende
- 🟦 **Eau** : Case vide non visée
- ⚫ **Manqué** : Tir dans l'eau
- 🔴 **Touché** : Bateau endommagé
- ⚫ **Coulé** : Bateau complètement détruit

## 📁 Architecture du projet
