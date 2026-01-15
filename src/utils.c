// Inclusion des bibliothèques nécessaires
#include <SDL3/SDL.h>   // Bibliothèque graphique SDL3 (pour showInfoBox)
#include <stdio.h>      // Bibliothèque standard d'entrée/sortie (pour snprintf)
#include "types.h"      // Définitions des types personnalisés (Player, AIState, etc.)
#include "utils.h"      // Déclarations des fonctions utilitaires (prototypes)

// ============================================================================
// FONCTION : clearGrid
// DESCRIPTION : Initialise une grille 2D à 0 (toutes les cases sont de l'eau)
// PARAMÈTRES :
//   - g : grille à initialiser (tableau 2D de GRID x GRID)
// RETOUR : aucun (void)
// ============================================================================
void clearGrid(int g[GRID][GRID]) {
    // Parcourt toutes les lignes de la grille
    for (int i = 0; i < GRID; i++)
        // Parcourt toutes les colonnes de la ligne courante
        for (int j = 0; j < GRID; j++)
            g[i][j] = 0;  // Met la case à 0 (eau vide)
}

// ============================================================================
// FONCTION : initPlayer
// DESCRIPTION : Initialise un joueur (grille vide, score à 0)
// PARAMÈTRES :
//   - p : pointeur vers le joueur à initialiser
// RETOUR : aucun (void)
// ============================================================================
void initPlayer(Player *p) {
    clearGrid(p->grid);  // Nettoie la grille du joueur
    p->score = 0;        // Initialise le score à 0
}

// ============================================================================
// FONCTION : initAIState
// DESCRIPTION : Initialise l'état de l'IA à ses valeurs par défaut
// PARAMÈTRES :
//   - ai : pointeur vers l'état de l'IA à initialiser
// RETOUR : aucun (void)
// ============================================================================
void initAIState(AIState *ai) {
    // Initialise les coordonnées de la dernière touche à -1 (aucune touche)
    ai->lastHitX = -1;
    ai->lastHitY = -1;
    
    // Initialise le point d'ancrage à -1 (aucun point d'ancrage)
    ai->anchorX = -1;
    ai->anchorY = -1;
    
    // Initialise la position courante à -1 (aucune recherche en cours)
    ai->currentX = -1;
    ai->currentY = -1;
    
    // Initialise la direction à 0 (aucune direction de recherche)
    ai->direction = 0;
    
    // Nettoie la grille des tirs de l'IA
    clearGrid(ai->shots);
}

// ============================================================================
// FONCTION : initGameState
// DESCRIPTION : Initialise l'état du jeu (début de partie)
// PARAMÈTRES :
//   - gs : pointeur vers l'état du jeu à initialiser
// RETOUR : aucun (void)
// ============================================================================
void initGameState(GameState *gs) {
    gs->currentPlayer = 0;          // Le joueur 1 commence
    gs->waitingNextPlayer = 1;      // En attente du premier tour (PvP)
    gs->waitingDelay = 0;           // Pas de délai IA actif
    gs->waitingPvPDelay = 0;        // Pas de délai PvP actif
    gs->lastActionTime = 0;         // Pas de dernière action
    gs->lastPlayerActionTime = 0;   // Pas de dernière action joueur
}

// ============================================================================
// FONCTION : isAreaFree
// DESCRIPTION : Vérifie si une zone rectangulaire est libre de tout bateau (avec marge)
// PARAMÈTRES :
//   - g      : grille à vérifier
//   - x, y   : coin supérieur gauche de la zone (coordonnées grille)
//   - width  : largeur de la zone
//   - height : hauteur de la zone
// RETOUR : 1 si la zone est libre, 0 sinon
// NOTE : Vérifie la zone étendue d'une case (marge de sécurité)
// ============================================================================
int isAreaFree(int g[GRID][GRID], int x, int y, int width, int height) {
    // Parcourt la zone étendue d'une case dans toutes les directions
    for (int i = x - 1; i <= x + height; i++) {
        for (int j = y - 1; j <= y + width; j++) {
            // Vérifie si les indices sont dans les limites de la grille
            if (i < 0 || i >= GRID || j < 0 || j >= GRID)
                continue;  // Hors grille, on ignore
            
            // Si la case n'est pas vide (0), la zone n'est pas libre
            if (g[i][j] != 0)
                return 0;  // Zone occupée
        }
    }
    return 1;  // Toute la zone est libre
}

// ============================================================================
// FONCTION : canPlaceShip
// DESCRIPTION : Vérifie si un bateau normal peut être placé à une position donnée
// PARAMÈTRES :
//   - g        : grille à vérifier
//   - x, y     : position de départ (coordonnées grille)
//   - size     : taille du bateau
//   - vertical : orientation (1 = vertical, 0 = horizontal)
// RETOUR : 1 si le placement est possible, 0 sinon
// ============================================================================
int canPlaceShip(int g[GRID][GRID], int x, int y, int size, int vertical) {
    // Calcule les dimensions selon l'orientation
    int width  = vertical ? 1 : size;   // Largeur = 1 si vertical, sinon = taille
    int height = vertical ? size : 1;   // Hauteur = taille si vertical, sinon = 1
    
    // Vérifie si le bateau dépasse de la grille
    if (x + height > GRID || y + width > GRID)
        return 0;  // Hors limites
    
    // Vérifie si la zone (avec marge) est libre
    return isAreaFree(g, x, y, width, height);
}

// ============================================================================
// FONCTION : canPlaceShip5x2Manual
// DESCRIPTION : Vérifie si le bateau spécial 5x2 peut être placé manuellement
// PARAMÈTRES :
//   - g        : grille à vérifier
//   - x, y     : position de départ (coordonnées grille)
//   - vertical : orientation (1 = vertical, 0 = horizontal)
// RETOUR : 1 si le placement est possible, 0 sinon
// ============================================================================
int canPlaceShip5x2Manual(int g[GRID][GRID], int x, int y, int vertical) {
    // Détermine les dimensions selon l'orientation
    int width  = vertical ? 2 : 5;   // 2 de large si vertical, 5 si horizontal
    int height = vertical ? 5 : 2;   // 5 de haut si vertical, 2 si horizontal
    
    // Vérifie les limites
    if (x + height > GRID || y + width > GRID)
        return 0;  // Hors limites
    
    // Vérifie la disponibilité de la zone
    return isAreaFree(g, x, y, width, height);
}

// ============================================================================
// FONCTION : placeShip5x2Manual
// DESCRIPTION : Place le bateau 5x2 manuellement dans la grille
// PARAMÈTRES :
//   - g        : grille où placer le bateau
//   - x, y     : position de départ (coordonnées grille)
//   - shipId   : identifiant du bateau (positif)
//   - vertical : orientation (1 = vertical, 0 = horizontal)
// RETOUR : aucun (void)
// ============================================================================
void placeShip5x2Manual(int g[GRID][GRID], int x, int y, int shipId, int vertical) {
    int width  = vertical ? 2 : 5;
    int height = vertical ? 5 : 2;
    
    // Remplit toutes les cases du bateau avec son ID
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            g[x + i][y + j] = shipId;  // shipId > 0 pour bateau intact
}

// ============================================================================
// FONCTION : placeShips
// DESCRIPTION : Place tous les bateaux automatiquement (placement aléatoire)
// PARAMÈTRES :
//   - g      : grille où placer les bateaux
//   - config : configuration des bateaux (tailles)
// RETOUR : aucun (void)
// ALGORITHME : Pour chaque bateau, cherche aléatoirement une position valide
// ============================================================================
void placeShips(int g[GRID][GRID], ShipConfig *config) {
    int shipId = 1;  // L'ID du premier bateau commence à 1
    
    // Pour chaque bateau dans la configuration
    for (int s = 0; s < SHIP_COUNT; s++) {
        if (config->sizes[s] > 0) {
            // Bateau normal (taille positive)
            int size = config->sizes[s];  // Taille du bateau
            int placed = 0;  // Flag pour savoir si le bateau est placé
            
            // Continue jusqu'à trouver une position valide
            while (!placed) {
                int vertical = myRand() % 2;  // Orientation aléatoire (0=horizontal, 1=vertical)
                int width  = vertical ? 1 : size;
                int height = vertical ? size : 1;
                
                // Calcule les positions maximales possibles
                int maxX = GRID - height;  // Position X maximale
                int maxY = GRID - width;   // Position Y maximale
                
                // Position aléatoire dans les limites
                int x = myRand() % (maxX + 1);
                int y = myRand() % (maxY + 1);
                
                // Vérifie si la position est valide
                if (isAreaFree(g, x, y, width, height)) {
                    // Place le bateau case par case
                    for (int i = 0; i < height; i++)
                        for (int j = 0; j < width; j++)
                            g[x + i][y + j] = shipId;  // Met l'ID du bateau
                    placed = 1;  // Bateau placé avec succès
                }
            }
        } else {
            // Bateau 5x2 (taille négative = -10)
            int placed = 0;
            while (!placed) {
                int vertical = myRand() % 2;
                int width  = vertical ? 2 : 5;
                int height = vertical ? 5 : 2;
                
                int maxX = GRID - height;
                int maxY = GRID - width;
                
                int x = myRand() % (maxX + 1);
                int y = myRand() % (maxY + 1);
                
                if (isAreaFree(g, x, y, width, height)) {
                    for (int i = 0; i < height; i++)
                        for (int j = 0; j < width; j++)
                            g[x + i][y + j] = shipId;
                    placed = 1;
                }
            }
        }
        shipId++;  // Passe à l'ID du prochain bateau
    }
}

// ============================================================================
// FONCTION : allDestroyed
// DESCRIPTION : Vérifie si tous les bateaux d'une grille sont détruits
// PARAMÈTRES :
//   - g : grille à vérifier
// RETOUR : 1 si tous détruits, 0 sinon
// ============================================================================
int allDestroyed(int g[GRID][GRID]) {
    // Parcourt toute la grille
    for (int i = 0; i < GRID; i++)
        for (int j = 0; j < GRID; j++)
            if (g[i][j] > 0)  // Si une case contient encore un bateau intact
                return 0;     // Il reste au moins un bateau
    return 1;  // Tous les bateaux sont détruits
}

// ============================================================================
// FONCTION : isShipSunk
// DESCRIPTION : Vérifie si un bateau spécifique (identifié par son ID) est complètement coulé
// PARAMÈTRES :
//   - g      : grille à vérifier
//   - shipId : ID du bateau à vérifier
// RETOUR : 1 si le bateau est coulé, 0 sinon
// ============================================================================
int isShipSunk(int g[GRID][GRID], int shipId) {
    // Parcourt toute la grille
    for (int i = 0; i < GRID; i++)
        for (int j = 0; j < GRID; j++)
            if (g[i][j] == shipId)  // Si une case contient encore ce bateau intact
                return 0;           // Le bateau n'est pas encore coulé
    return 1;  // Plus aucune case ne contient ce bateau = coulé
}

// ============================================================================
// FONCTION : markShipSunk
// DESCRIPTION : Marque un bateau comme coulé dans la grille (change sa représentation)
// PARAMÈTRES :
//   - g      : grille où marquer le bateau
//   - shipId : ID du bateau à marquer comme coulé
// RETOUR : aucun (void)
// NOTE : Change les cases avec valeur -shipId en -200 - shipId
// ============================================================================
void markShipSunk(int g[GRID][GRID], int shipId) {
    // Parcourt toute la grille
    for (int i = 0; i < GRID; i++)
        for (int j = 0; j < GRID; j++)
            if (g[i][j] == -shipId)  // Si case contient ce bateau touché
                g[i][j] = -200 - shipId;  // Marque comme coulé (valeur spéciale)
}

// ============================================================================
// FONCTION : getCurrentPlayer
// DESCRIPTION : Retourne le joueur dont c'est le tour
// PARAMÈTRES :
//   - mode : mode de jeu (1 = PvP, autre = solo)
//   - gs   : état du jeu
// RETOUR : pointeur vers le joueur actuel
// ============================================================================
Player* getCurrentPlayer(int mode, GameState *gs) {
    if (mode == 1)  // Mode PvP
        return (gs->currentPlayer == 0) ? &player1 : &player2;
    return &player1;  // Mode solo : toujours le joueur 1
}

// ============================================================================
// FONCTION : getOpponent
// DESCRIPTION : Retourne l'adversaire du joueur actuel
// PARAMÈTRES :
//   - mode : mode de jeu (1 = PvP, autre = solo)
//   - gs   : état du jeu
// RETOUR : pointeur vers l'adversaire
// ============================================================================
Player* getOpponent(int mode, GameState *gs) {
    if (mode == 1)  // Mode PvP
        return (gs->currentPlayer == 0) ? &player2 : &player1;
    return &player2;  // Mode solo : l'adversaire est toujours l'IA (joueur 2)
}

// ============================================================================
// FONCTION : showInfoBox
// DESCRIPTION : Affiche une boîte de dialogue d'information SDL
// PARAMÈTRES :
//   - window : fenêtre SDL parente
//   - format : format du message (comme printf)
//   - a, b   : valeurs à formater
// RETOUR : aucun (void)
// ============================================================================
void showInfoBox(SDL_Window *window, const char *format, int a, int b) {
    char buf[256];  // Buffer pour le message formaté
    snprintf(buf, sizeof(buf), format, a, b);  // Formate le message
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Info", buf, window);
}

// ============================================================================
// FONCTION : getScoreModeFromGameMode
// DESCRIPTION : Convertit le mode de jeu (1-4) en mode de score (enum ScoreMode)
// PARAMÈTRES :
//   - mode : mode de jeu (1 = PvP, 2 = facile, 3 = moyen, 4 = difficile)
// RETOUR : ScoreMode correspondant
// ============================================================================
ScoreMode getScoreModeFromGameMode(int mode) {
    // Instruction switch pour traiter chaque valeur possible
    switch (mode) {
        case 2: return SCORE_EASY;    // Mode 2 = IA facile
        case 3: return SCORE_MEDIUM;  // Mode 3 = IA moyen
        case 4: return SCORE_HARD;    // Mode 4 = IA difficile
        case 1: return SCORE_PVP;     // Mode 1 = PvP
        default: return SCORE_EASY;   // Par défaut (ne devrait pas arriver)
    }
}