// ========================================================================
// FICHIER : random.h (Random Header)
// DESCRIPTION : Déclaration des fonctions de génération aléatoire
//               pour le jeu de bataille navale
// ========================================================================

// Directive de préprocesseur pour éviter les inclusions multiples
// Si RANDOM_H n'est pas défini, définir RANDOM_H et inclure le contenu
#ifndef RANDOM_H
#define RANDOM_H

// Inclusion du fichier d'en-tête contenant les définitions de types
// Ce fichier contient probablement la déclaration de la variable globale 'seed'
// qui est utilisée par la fonction myRand()
#include "types.h"

// ========================================================================
// DÉCLARATION DE LA FONCTION DE GÉNÉRATION ALÉATOIRE
// ========================================================================

// Fonction : myRand
// Description : Générateur de nombres pseudo-aléatoires maison (LCG)
//               Implémente un générateur congruentiel linéaire
// Caractéristiques :
//   - Utilise la formule : seed = seed * 1103515245 + 12345
//   - Retourne les bits 16-30 de la graine (valeurs 0-32767)
//   - Dépend de la variable globale 'seed' qui doit être initialisée
// Paramètres : aucun (utilise la variable globale 'seed')
// Retour : Nombre pseudo-aléatoire entre 0 et 32767 (int)
// Note : Cette fonction modifie la variable globale 'seed'
int myRand();

// Fin de la condition #ifndef RANDOM_H
#endif
// Note : Ce fichier d'en-tête ne contient QUE la déclaration (prototype)
// L'implémentation réelle est dans le fichier source correspondant (random.c)