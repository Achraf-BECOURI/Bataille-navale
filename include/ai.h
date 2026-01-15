// ========================================================================
// FICHIER : ai.h (Artificial Intelligence Header)
// DESCRIPTION : Déclaration des fonctions d'intelligence artificielle
//               pour le jeu de bataille navale
// ========================================================================

// Directive de préprocesseur pour éviter les inclusions multiples
// Si AI_H n'est pas défini, définir AI_H et inclure le contenu
#ifndef AI_H
#define AI_H

// Inclusion du fichier d'en-tête contenant les définitions de types
// Ce fichier contient probablement les définitions de Player et AIState
#include "types.h"

// ========================================================================
// DÉCLARATIONS DES FONCTIONS D'INTELLIGENCE ARTIFICIELLE
// ========================================================================

// Fonction : enemyEasy
// Description : Implémente une IA de niveau facile
//               Stratégie : tire complètement au hasard
// Paramètres :
//   - target : pointeur vers le joueur cible (adversaire)
//   - ai     : pointeur vers l'état de l'IA (historique des tirs)
// Retour : aucun (void)
void enemyEasy(Player *target, AIState *ai);

// Fonction : enemyMedium
// Description : Implémente une IA de niveau moyen
//               Stratégie : tire au hasard, mais poursuit si touché
//               Mémoire : se souvient de la dernière case touchée
// Paramètres :
//   - target : pointeur vers le joueur cible
//   - ai     : pointeur vers l'état de l'IA
// Retour : aucun (void)
void enemyMedium(Player *target, AIState *ai);

// Fonction : enemyHard
// Description : Implémente une IA de niveau difficile
//               Stratégie : recherche optimisée avec poursuite directionnelle
//               Utilise un état complexe pour mémoriser les bateaux touchés
// Paramètres :
//   - target : pointeur vers le joueur cible
//   - ai     : pointeur vers l'état de l'IA
// Retour : aucun (void)
void enemyHard(Player *target, AIState *ai);

// Fin de la condition #ifndef AI_H
#endif
// Note : Ce fichier d'en-tête ne contient QUE des déclarations (prototypes)
// Les implémentations réelles sont dans le fichier source correspondant (ai.c)