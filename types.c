// Inclusion de la bibliothèque SDL3 pour les fonctions et types graphiques
#include <SDL3/SDL.h>

// Inclusion du fichier d'en-tête contenant les définitions de types personnalisés
// Ce fichier définit les structures Player, ShipConfig, AIState, GameState, GraphicsContext
// ainsi que les constantes comme NAME_LEN
#include "types.h"

/* ==================== VARIABLES GLOBALES DU JEU ==================== */

// Déclaration du joueur 1 (joueur humain ou premier joueur en mode PVP)
Player player1;

// Déclaration du joueur 2 (peut être un second joueur humain ou l'IA)
Player player2;

// Configuration des bateaux avec leurs tailles
// .sizes = tableau de 6 éléments représentant les 6 bateaux :
//   - 3  : bateau de taille 3
//   - 4  : bateau de taille 4
//   - 4  : bateau de taille 4 (deuxième)
//   - 5  : bateau de taille 5
//   - 6  : bateau de taille 6
//   - -10 : bateau spécial 5x2 (la valeur négative indique un bateau rectangulaire)
ShipConfig shipConfig = {
    .sizes = {3, 4, 4, 5, 6, -10}
};

// État de l'intelligence artificielle (utilisé pour les modes contre IA)
// Contient la mémoire des tirs, les positions touchées, les stratégies de recherche
AIState aiState;

// État général du jeu (gestion des tours, délais, joueur courant)
// Utilisé pour contrôler le flux du jeu dans la boucle principale
GameState gameState;

// Contexte graphique SDL (fenêtre et renderer)
// Stocke les pointeurs vers la fenêtre SDL et le renderer pour le dessin
GraphicsContext graphics;

// Graine pour le générateur de nombres pseudo-aléatoires maison
// Initialisée à 98765, sera modifiée avec SDL_GetTicks() pour plus de variabilité
unsigned int seed = 98765;

// Mode de placement des bateaux (1 = automatique, 2 = manuel)
// Déterminé par l'utilisateur au début du jeu
int placementMode;

// Nom du joueur 1 (buffer de taille NAME_LEN, probablement 50 caractères)
// Initialisé comme chaîne vide, sera rempli selon le mode de jeu
char player1Name[NAME_LEN] = "";

// Nom du joueur 2 ou "IA" (buffer de taille NAME_LEN)
// En mode PVP : nom du deuxième joueur humain
// En mode solo : "IA" (défini dans main.c)
char player2Name[NAME_LEN] = "";

/* NOTES SUR LES VARIABLES GLOBALES :
 * 1. Ces variables sont déclarées au niveau fichier (scope global)
 * 2. Elles sont accessibles depuis tous les fichiers qui incluent types.h
 * 3. Elles représentent l'état complet du jeu en cours
 * 4. L'initialisation réelle se fait dans main() via initPlayer(), initAIState(), etc.
 * 5. Ces déclarations allouent la mémoire pour les structures, mais ne les initialisent pas
 */