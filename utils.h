// Directive de préprocesseur pour éviter les inclusions multiples
// Si UTILS_H n'est pas défini, définir UTILS_H et inclure le contenu
#ifndef UTILS_H
#define UTILS_H

// Inclusion du fichier d'en-tête contenant les définitions de types personnalisés
// Ce fichier contient les définitions de Player, AIState, GameState, etc.
#include "types.h"

// ========================================================================
// DÉCLARATIONS DES FONCTIONS UTILITAIRES
// ========================================================================

// Fonction : clearGrid
// Description : Initialise une grille 2D à 0 (toutes les cases sont de l'eau)
// Paramètres :
//   - g : grille à initialiser (tableau 2D de GRID x GRID)
// Retour : aucun (void)
void clearGrid(int g[GRID][GRID]);

// Fonction : initPlayer
// Description : Initialise un joueur (grille vide, score à 0)
// Paramètres :
//   - p : pointeur vers le joueur à initialiser
// Retour : aucun (void)
void initPlayer(Player *p);

// Fonction : initAIState
// Description : Initialise l'état de l'IA à ses valeurs par défaut
// Paramètres :
//   - ai : pointeur vers l'état de l'IA à initialiser
// Retour : aucun (void)
void initAIState(AIState *ai);

// Fonction : initGameState
// Description : Initialise l'état du jeu (début de partie)
// Paramètres :
//   - gs : pointeur vers l'état du jeu à initialiser
// Retour : aucun (void)
void initGameState(GameState *gs);

// Fonction : isAreaFree
// Description : Vérifie si une zone rectangulaire est libre de tout bateau (avec marge)
// Paramètres :
//   - g      : grille à vérifier
//   - x, y   : coin supérieur gauche de la zone (coordonnées grille)
//   - width  : largeur de la zone
//   - height : hauteur de la zone
// Retour : 1 si la zone est libre, 0 sinon
int isAreaFree(int g[GRID][GRID], int x, int y, int width, int height);

// Fonction : canPlaceShip
// Description : Vérifie si un bateau normal peut être placé à une position donnée
// Paramètres :
//   - g        : grille à vérifier
//   - x, y     : position de départ (coordonnées grille)
//   - size     : taille du bateau
//   - vertical : orientation (1 = vertical, 0 = horizontal)
// Retour : 1 si le placement est possible, 0 sinon
int canPlaceShip(int g[GRID][GRID], int x, int y, int size, int vertical);

// Fonction : canPlaceShip5x2Manual
// Description : Vérifie si le bateau spécial 5x2 peut être placé manuellement
// Paramètres :
//   - g        : grille à vérifier
//   - x, y     : position de départ (coordonnées grille)
//   - vertical : orientation (1 = vertical, 0 = horizontal)
// Retour : 1 si le placement est possible, 0 sinon
int canPlaceShip5x2Manual(int g[GRID][GRID], int x, int y, int vertical);

// Fonction : placeShip5x2Manual
// Description : Place le bateau 5x2 manuellement dans la grille
// Paramètres :
//   - g        : grille où placer le bateau
//   - x, y     : position de départ (coordonnées grille)
//   - shipId   : identifiant du bateau (positif)
//   - vertical : orientation (1 = vertical, 0 = horizontal)
// Retour : aucun (void)
void placeShip5x2Manual(int g[GRID][GRID], int x, int y, int shipId, int vertical);

// Fonction : placeShips
// Description : Place tous les bateaux automatiquement (placement aléatoire)
// Paramètres :
//   - g      : grille où placer les bateaux
//   - config : configuration des bateaux (tailles)
// Retour : aucun (void)
void placeShips(int g[GRID][GRID], ShipConfig *config);

// Fonction : allDestroyed
// Description : Vérifie si tous les bateaux d'une grille sont détruits
// Paramètres :
//   - g : grille à vérifier
// Retour : 1 si tous détruits, 0 sinon
int allDestroyed(int g[GRID][GRID]);

// Fonction : isShipSunk
// Description : Vérifie si un bateau spécifique (identifié par son ID) est complètement coulé
// Paramètres :
//   - g      : grille à vérifier
//   - shipId : ID du bateau à vérifier
// Retour : 1 si le bateau est coulé, 0 sinon
int isShipSunk(int g[GRID][GRID], int shipId);

// Fonction : markShipSunk
// Description : Marque un bateau comme coulé dans la grille (change sa représentation)
// Paramètres :
//   - g      : grille où marquer le bateau
//   - shipId : ID du bateau à marquer comme coulé
// Retour : aucun (void)
void markShipSunk(int g[GRID][GRID], int shipId);

// Fonction : getCurrentPlayer
// Description : Retourne le joueur dont c'est le tour
// Paramètres :
//   - mode : mode de jeu (1 = PvP, autre = solo)
//   - gs   : état du jeu
// Retour : pointeur vers le joueur actuel
Player* getCurrentPlayer(int mode, GameState *gs);

// Fonction : getOpponent
// Description : Retourne l'adversaire du joueur actuel
// Paramètres :
//   - mode : mode de jeu (1 = PvP, autre = solo)
//   - gs   : état du jeu
// Retour : pointeur vers l'adversaire
Player* getOpponent(int mode, GameState *gs);

// Fonction : showInfoBox
// Description : Affiche une boîte de dialogue d'information SDL
// Paramètres :
//   - window : fenêtre SDL parente
//   - format : format du message (comme printf)
//   - a, b   : valeurs à formater
// Retour : aucun (void)
void showInfoBox(SDL_Window *window, const char *format, int a, int b);

// Fonction : getScoreModeFromGameMode
// Description : Convertit le mode de jeu (1-4) en mode de score (enum ScoreMode)
// Paramètres :
//   - mode : mode de jeu (1 = PvP, 2 = facile, 3 = moyen, 4 = difficile)
// Retour : ScoreMode correspondant
ScoreMode getScoreModeFromGameMode(int mode);

// Fin de la condition #ifndef UTILS_H
#endif
// Note : Ce fichier d'en-tête contient uniquement les déclarations (prototypes)
// Les implémentations sont dans le fichier source correspondant (utils.c)