// Directive de préprocesseur pour éviter les inclusions multiples
// Si TYPES_H n'est pas défini, définir TYPES_H et inclure le contenu
#ifndef TYPES_H
#define TYPES_H

// ==================== DÉCLARATIONS EXTERNES (variables globales définies ailleurs) ====================

// Déclaration externe de la variable globale 'seed' utilisée par myRand()
// 'extern' indique que la variable est définie dans un autre fichier (probablement types.c ou main.c)
extern unsigned int seed;

// Déclaration (prototype) de la fonction de génération aléatoire
// 'void' dans les paramètres indique que la fonction ne prend aucun paramètre
int myRand(void);

// ==================== CONSTANTES DE CONFIGURATION DU JEU ====================

// Taille de la grille de jeu (14x14 cases)
#define GRID 14

// Taille en pixels d'une cellule de la grille
#define CELL 40

// Largeur de la fenêtre : 2 grilles côte à côte (GRID * CELL * 2)
#define WIDTH (GRID * CELL * 2)

// Hauteur de la fenêtre : 1 grille (GRID * CELL)
#define HEIGHT (GRID * CELL)

// Nombre total de bateaux par joueur
#define SHIP_COUNT 6

// Délai entre les tours de l'IA en millisecondes (500ms = 0.5 seconde)
#define TURN_DELAY 500

// Délai entre les tours en mode PvP en millisecondes (900ms = 0.9 seconde)
#define PVP_DELAY 900

// ==================== CONSTANTES DE GESTION DES SCORES ====================

// Nombre maximum de scores sauvegardés par catégorie de difficulté
#define MAX_SCORES 10

// Longueur maximale des noms de joueurs (49 caractères + terminateur nul)
#define NAME_LEN 50

// ==================== DÉFINITIONS DES STRUCTURES DE DONNÉES ====================

// Structure représentant un joueur (humain ou IA)
typedef struct {
    // Grille de jeu 14x14 :
    //   > 0  = bateau intact (valeur = ID du bateau, 1 à 6)
    //   = 0  = eau vide
    //   = -99 = tir manqué (eau touchée)
    //   < 0  = bateau touché ou coulé
    //   ≤ -200 = bateau coulé (valeur spéciale)
    int grid[GRID][GRID];
    
    // Score actuel du joueur
    int score;
} Player;

// Structure de configuration des bateaux
typedef struct {
    // Tableau contenant les tailles des 6 bateaux :
    //   Valeurs positives = taille du bateau (3, 4, 4, 5, 6)
    //   Valeur négative = bateau spécial 5x2 (-10)
    int sizes[SHIP_COUNT];
} ShipConfig;

// Structure d'état de l'intelligence artificielle (pour modes moyen/difficile)
typedef struct {
    // Dernière case touchée (coordonnées)
    int lastHitX;   // -1 si aucune dernière touche
    int lastHitY;   // -1 si aucune dernière touche
    
    // Point d'ancrage pour la recherche directionnelle (première touche d'un bateau)
    int anchorX;    // -1 si aucun point d'ancrage
    int anchorY;    // -1 si aucun point d'ancrage
    
    // Position courante dans la recherche
    int currentX;   // -1 si aucune recherche en cours
    int currentY;   // -1 si aucune recherche en cours
    
    // Direction de recherche actuelle :
    //   0 = aucune direction
    //   1 = haut, 2 = bas, 3 = gauche, 4 = droite
    int direction;
    
    // Grille des tirs déjà effectués par l'IA :
    //   0 = pas encore tiré ici
    //   1 = déjà tiré ici
    int shots[GRID][GRID];
} AIState;

// Structure d'état général du jeu (gestion des tours et délais)
typedef struct {
    // Joueur dont c'est le tour : 0 = joueur 1, 1 = joueur 2/IA
    int currentPlayer;
    
    // Flag : en attente que le prochain joueur confirme son tour (mode PvP)
    int waitingNextPlayer;
    
    // Flag : en attente du délai IA (mode solo)
    int waitingDelay;
    
    // Flag : en attente du délai PvP (mode joueur contre joueur)
    int waitingPvPDelay;
    
    // Timestamp (en millisecondes) de la dernière action de l'IA
    Uint32 lastActionTime;
    
    // Timestamp (en millisecondes) de la dernière action d'un joueur (mode PvP)
    Uint32 lastPlayerActionTime;
} GameState;

// Structure de contexte graphique SDL
typedef struct {
    // Pointeur vers la fenêtre SDL (gestion de la fenêtre graphique)
    SDL_Window *window;
    
    // Pointeur vers le renderer SDL (contexte de dessin)
    SDL_Renderer *renderer;
} GraphicsContext;

// Structure pour stocker un score avec le nom du joueur
typedef struct {
    // Nom du joueur (terminé par '\0')
    char name[NAME_LEN];
    
    // Score obtenu
    int score;
} Score;

// Énumération des modes de difficulté pour le système de scores
typedef enum {
    SCORE_EASY,     // Mode facile contre IA
    SCORE_MEDIUM,   // Mode moyen contre IA
    SCORE_HARD,     // Mode difficile contre IA
    SCORE_PVP       // Mode joueur contre joueur
} ScoreMode;

// ==================== DÉCLARATIONS EXTERNES DES VARIABLES GLOBALES ====================

// Déclarations externes des instances globales des structures
// Ces variables sont définies (allouées) dans un autre fichier (probablement types.c)

// Les deux joueurs du jeu
extern Player player1;
extern Player player2;

// Configuration des bateaux (initialisée avec les tailles par défaut)
extern ShipConfig shipConfig;

// État de l'intelligence artificielle
extern AIState aiState;

// État général du jeu
extern GameState gameState;

// Contexte graphique SDL
extern GraphicsContext graphics;

// Mode de placement des bateaux (1 = automatique, 2 = manuel)
extern int placementMode;

// Fin de la condition #ifndef TYPES_H
#endif